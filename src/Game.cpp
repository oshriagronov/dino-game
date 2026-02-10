#include "Game.h"
#include <algorithm>
#include <filesystem>
#include <vector>

namespace {
std::filesystem::path get_base_path() {
    char* base_path = SDL_GetBasePath();
    if (base_path != nullptr) {
        std::filesystem::path path(base_path);
        SDL_free(base_path);
        return path;
    }
    return std::filesystem::current_path();
}

std::vector<std::filesystem::path> get_asset_base_dirs() {
    const auto base = get_base_path();
    std::vector<std::filesystem::path> dirs;
    dirs.reserve(4);

    // 1) SDL reported app base path.
    dirs.push_back(base);
    // 2) Standard macOS bundle resources dir.
    dirs.push_back(base.parent_path() / "Resources");
    // 3) macOS bundle executable dir.
    dirs.push_back(base.parent_path() / "MacOS");
    // 4) Current working directory (for local runs from project root).
    dirs.push_back(std::filesystem::current_path());

    return dirs;
}

std::string resolve_asset_path(const std::string& relative, const std::vector<std::filesystem::path>& base_dirs) {
    for (const auto& dir : base_dirs) {
        const auto candidate = dir / relative;
        if (std::filesystem::exists(candidate)) {
            return candidate.string();
        }
    }
    return (base_dirs.front() / relative).string();
}

SDL_Surface* load_surface_rgba32(const std::string& path) {
    SDL_Surface* loaded = IMG_Load(path.c_str());
    if (loaded == nullptr) {
        return nullptr;
    }

    SDL_Surface* converted = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(loaded);
    return converted;
}

bool is_solid_pixel(const SDL_Surface* surface, int x, int y) {
    const Uint8* row = static_cast<const Uint8*>(surface->pixels) + y * surface->pitch;
    const Uint32 pixel = *(reinterpret_cast<const Uint32*>(row) + x);
    Uint8 r = 0;
    Uint8 g = 0;
    Uint8 b = 0;
    Uint8 a = 0;
    SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

    // Ignore transparent pixels and very light anti-aliased border pixels.
    if (a < 200) {
        return false;
    }
    const int luminance = (299 * static_cast<int>(r) + 587 * static_cast<int>(g) + 114 * static_cast<int>(b)) / 1000;
    return luminance < 205;
}

bool has_precise_collision(const SDL_Surface* dinoSurface, const SDL_Rect& dinoRect, const SDL_Surface* cactusSurface, const SDL_Rect& cactusRect) {
    if (dinoSurface == nullptr || cactusSurface == nullptr) {
        return false;
    }

    SDL_Rect overlap;
    if (!SDL_IntersectRect(&dinoRect, &cactusRect, &overlap)) {
        return false;
    }

    const bool needsLockDino = SDL_MUSTLOCK(dinoSurface) != 0;
    const bool needsLockCactus = SDL_MUSTLOCK(cactusSurface) != 0;
    SDL_Surface* mutableDinoSurface = const_cast<SDL_Surface*>(dinoSurface);
    SDL_Surface* mutableCactusSurface = const_cast<SDL_Surface*>(cactusSurface);

    if (needsLockDino && SDL_LockSurface(mutableDinoSurface) != 0) {
        return false;
    }
    if (needsLockCactus && SDL_LockSurface(mutableCactusSurface) != 0) {
        if (needsLockDino) {
            SDL_UnlockSurface(mutableDinoSurface);
        }
        return false;
    }

    bool collided = false;
    for (int y = 0; y < overlap.h && !collided; ++y) {
        const int dinoY = overlap.y - dinoRect.y + y;
        const int cactusY = overlap.y - cactusRect.y + y;

        for (int x = 0; x < overlap.w; ++x) {
            const int dinoX = overlap.x - dinoRect.x + x;
            const int cactusX = overlap.x - cactusRect.x + x;
            if (is_solid_pixel(dinoSurface, dinoX, dinoY) && is_solid_pixel(cactusSurface, cactusX, cactusY)) {
                collided = true;
                break;
            }
        }
    }

    if (needsLockCactus) {
        SDL_UnlockSurface(mutableCactusSurface);
    }
    if (needsLockDino) {
        SDL_UnlockSurface(mutableDinoSurface);
    }

    return collided;
}
} // namespace

// Constructor: Initializes all member variables, especially the smart pointers and SDL_Rects.
Game::Game():
    window{nullptr, SDL_DestroyWindow},
    renderer{nullptr, SDL_DestroyRenderer}, 
    track_ptr{nullptr, SDL_DestroyTexture},
    dino_ptr_run1{nullptr, SDL_DestroyTexture},
    dino_ptr_run2{nullptr, SDL_DestroyTexture},
    dino_run1_surface{nullptr, SDL_FreeSurface},
    dino_run2_surface{nullptr, SDL_FreeSurface},
    largeCactus1_ptr{nullptr, SDL_DestroyTexture},
    largeCactus2_ptr{nullptr, SDL_DestroyTexture},
    largeCactus3_ptr{nullptr, SDL_DestroyTexture},
    largeCactus1_surface{nullptr, SDL_FreeSurface},
    largeCactus2_surface{nullptr, SDL_FreeSurface},
    largeCactus3_surface{nullptr, SDL_FreeSurface},
    score_font_ptr{nullptr, TTF_CloseFont},
    game_over_font_ptr{nullptr, TTF_CloseFont},
    scoreSurface{nullptr, SDL_FreeSurface},
    scoreTexture{nullptr, SDL_DestroyTexture},
    startSurface{nullptr, SDL_FreeSurface},
    startTexture{nullptr, SDL_DestroyTexture},
    gameOverSurface{nullptr, SDL_FreeSurface},
    gameOverTexture{nullptr, SDL_DestroyTexture},
    gameOverSubTextSurface{nullptr, SDL_FreeSurface},
    gameOverSubTextTexture{nullptr, SDL_DestroyTexture},
    rd{},
    gen{rd()},
    dist{Width + 300, Width + 600},
    trackDestRect1{track.get_default_x(), track.get_default_y(), track.get_width(), track.get_height()},
    dinoDestRect{dino.get_x(), dino.get_y(), dino.get_width(), dino.get_height()},
    largeCactus1DestRect{largeCactus1_spawn_point, largeCactus1.get_y(), largeCactus1.get_width(), largeCactus1.get_height()},
    largeCactus2DestRect{largeCactus2_spawn_point, largeCactus2.get_y(), largeCactus2.get_width(), largeCactus2.get_height()},
    largeCactus3DestRect{largeCactus3_spawn_point, largeCactus3.get_y(), largeCactus3.get_width(), largeCactus3.get_height()},
    trackDestRect2{track.get_default_x() + track.get_width(), track.get_default_y(), track.get_width(), track.get_height()}
{
    resetCactusPositions();
}

// Initializes the SDL window and renderer.
void Game::init()
{
    this->window.reset(SDL_CreateWindow(
        this->title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        this->Width, 
        this->Height, 
        SDL_WINDOW_SHOWN));
    if(this->window == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    this->renderer.reset(SDL_CreateRenderer(
        this->window.get(), 
        -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    if(this->renderer == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    SDL_SetRenderDrawColor(this->renderer.get(), 255, 255, 255, 255);
}

// Loads all necessary media files (textures and fonts) into memory.
void Game::loading_media(){
    const auto asset_dirs = get_asset_base_dirs();
    const std::string trackPath = resolve_asset_path(track.get_path(), asset_dirs);
    const std::string dinoRun1Path = resolve_asset_path(dino.get_path_run1(), asset_dirs);
    const std::string dinoRun2Path = resolve_asset_path(dino.get_path_run2(), asset_dirs);
    const std::string largeCactus1Path = resolve_asset_path(largeCactus1.get_path(), asset_dirs);
    const std::string largeCactus2Path = resolve_asset_path(largeCactus2.get_path(), asset_dirs);
    const std::string largeCactus3Path = resolve_asset_path(largeCactus3.get_path(), asset_dirs);
    this->track_ptr.reset(IMG_LoadTexture(
        this->renderer.get(), 
        trackPath.c_str()));
    if(this->track_ptr == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    this->dino_ptr_run1.reset(IMG_LoadTexture(
        this->renderer.get(), 
        dinoRun1Path.c_str()));
    if(this->dino_ptr_run1 == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    this->dino_ptr_run2.reset(IMG_LoadTexture(
        this->renderer.get(), 
        dinoRun2Path.c_str()));
    if(this->dino_ptr_run2 == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    this->largeCactus1_ptr.reset(IMG_LoadTexture(
        this->renderer.get(), 
        largeCactus1Path.c_str()));
    if(this->largeCactus1_ptr == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    this->largeCactus2_ptr.reset(IMG_LoadTexture(
        this->renderer.get(), 
        largeCactus2Path.c_str()));
    if(this->largeCactus2_ptr == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    this->largeCactus3_ptr.reset(IMG_LoadTexture(
        this->renderer.get(), 
        largeCactus3Path.c_str()));
    if(this->largeCactus3_ptr == nullptr){
        throw std::runtime_error(SDL_GetError());
    }

    this->dino_run1_surface.reset(load_surface_rgba32(dinoRun1Path));
    if(this->dino_run1_surface == nullptr){
        throw std::runtime_error(IMG_GetError());
    }
    this->dino_run2_surface.reset(load_surface_rgba32(dinoRun2Path));
    if(this->dino_run2_surface == nullptr){
        throw std::runtime_error(IMG_GetError());
    }
    this->largeCactus1_surface.reset(load_surface_rgba32(largeCactus1Path));
    if(this->largeCactus1_surface == nullptr){
        throw std::runtime_error(IMG_GetError());
    }
    this->largeCactus2_surface.reset(load_surface_rgba32(largeCactus2Path));
    if(this->largeCactus2_surface == nullptr){
        throw std::runtime_error(IMG_GetError());
    }
    this->largeCactus3_surface.reset(load_surface_rgba32(largeCactus3Path));
    if(this->largeCactus3_surface == nullptr){
        throw std::runtime_error(IMG_GetError());
    }

    this->score_font_ptr.reset(TTF_OpenFont(resolve_asset_path("assets/ArcadeClassic.ttf", asset_dirs).c_str(), SCORE_FONT_SIZE));
    if(this->score_font_ptr == nullptr){
        throw std::runtime_error(TTF_GetError());
    }
    this->game_over_font_ptr.reset(TTF_OpenFont(resolve_asset_path("assets/ArcadeClassic.ttf", asset_dirs).c_str(), GAME_OVER_FONT_SIZE));
    if(this->game_over_font_ptr == nullptr){
        throw std::runtime_error(TTF_GetError());
    }
    this->gameOverSurface.reset(TTF_RenderText_Blended(this->game_over_font_ptr.get(), this->GAME_OVER_TEXT.c_str(), textColor));
    if(this->gameOverSurface == nullptr){
        throw std::runtime_error(TTF_GetError());
    }
    this->gameOverDestRect = {Width / 3 + 30, Height / 3, gameOverSurface->w, gameOverSurface->h};
    this->gameOverTexture.reset(SDL_CreateTextureFromSurface(this->renderer.get(), this->gameOverSurface.get()));
    if(this->gameOverTexture == nullptr){
        throw std::runtime_error(SDL_GetError());
    }

    this->gameOverSubTextSurface.reset(TTF_RenderText_Blended(this->score_font_ptr.get(), this->GAME_OVER_SUB_TEXT.c_str(), textColor));
    if(this->gameOverSubTextSurface == nullptr){
        throw std::runtime_error(TTF_GetError());
    }
    this->gameOverSubTextDestRect = {Width / 3 + 40, Height / 3 + 70, gameOverSubTextSurface->w, gameOverSubTextSurface->h};
    this->gameOverSubTextTexture.reset(SDL_CreateTextureFromSurface(this->renderer.get(), this->gameOverSubTextSurface.get()));
    if(this->gameOverSubTextTexture == nullptr){
        throw std::runtime_error(SDL_GetError());
    }

    this->startSurface.reset(TTF_RenderText_Blended(this->game_over_font_ptr.get(), this->START_TEXT.c_str(), textColor));
    if(this->startSurface == nullptr){
        throw std::runtime_error(TTF_GetError());
    }
    this->startDestRect = {
        (Width - startSurface->w) / 2,
        (Height - startSurface->h) / 2,
        startSurface->w,
        startSurface->h
    };
    this->startTexture.reset(SDL_CreateTextureFromSurface(this->renderer.get(), this->startSurface.get()));
    if(this->startTexture == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
}

// Updates the track's position to create an infinite scrolling effect.
void Game::trackUpdate(){
    floorOffsetX += getCurrentTrackSpeed();
    if (floorOffsetX >= track.get_width()) {
        floorOffsetX = track.get_default_x();
    }
    this->trackDestRect1.x = -floorOffsetX + 5;
    this->trackDestRect2.x = -floorOffsetX + track.get_width();
}

int Game::getMinimumCactusGap() const {
    const int speedDelta = getCurrentTrackSpeed() - BASE_TRACK_SPEED;
    return MIN_CACTUS_GAP + speedDelta * GAP_GROWTH_PER_SPEED;
}

int Game::getRandomCactusGap() {
    const int minGap = getMinimumCactusGap();
    const int maxGap = minGap + MAX_CACTUS_GAP_RANDOM_EXTRA;
    return dist(gen, std::uniform_int_distribution<int>::param_type(minGap, maxGap));
}

int Game::getFarthestCactusX() const {
    return std::max(largeCactus1DestRect.x, std::max(largeCactus2DestRect.x, largeCactus3DestRect.x));
}

void Game::respawnCactus(SDL_Rect& cactusRect) {
    const int spawnAfter = std::max(Width, getFarthestCactusX());
    cactusRect.x = spawnAfter + getRandomCactusGap();
}

void Game::resetCactusPositions() {
    largeCactus1DestRect.x = largeCactus1_spawn_point + getRandomCactusGap() / 2;
    largeCactus2DestRect.x = largeCactus1DestRect.x + getRandomCactusGap();
    largeCactus3DestRect.x = largeCactus2DestRect.x + getRandomCactusGap();
}

int Game::getCurrentTrackSpeed() const {
    const int speedIncrease = score / SCORE_PER_SPEED_STEP;
    return std::min(BASE_TRACK_SPEED + speedIncrease, MAX_TRACK_SPEED);
}

Uint32 Game::getCurrentFrameInterval() const {
    const int speedDelta = getCurrentTrackSpeed() - BASE_TRACK_SPEED;
    const int frameDecrease = speedDelta * 8;
    const int interval = static_cast<int>(BASE_FRAME_INTERVAL) - frameDecrease;
    return static_cast<Uint32>(std::max(static_cast<int>(MIN_FRAME_INTERVAL), interval));
}

// Toggles between the two running animation frames for the dinosaur.
void Game::updateDinoRunAnimation(){
    Uint32 now = SDL_GetTicks();
    if (now - lastFrameTime > getCurrentFrameInterval()) {
        useLeftFrame = !useLeftFrame;   // toggle frame
        lastFrameTime = now;
    }
}

// Manages the dinosaur's vertical movement for jumping and applies gravity.
void Game::updateDinoAnimation(){
    if(isJumping){
        dinoDestRect.y += (int)velocityY;
        velocityY += gravity;
        if(dinoDestRect.y >= dino.get_y()){
            dinoDestRect.y = dino.get_y();
            isJumping = false;
        }   
    }
    // if not jumping then running.
    else
        this->updateDinoRunAnimation();
    velocityY += gravity;
}

// Renders the correct dinosaur animation frame to the screen.
void Game::renderDino(SDL_Renderer* renderer){
    if(useLeftFrame)
        SDL_RenderCopy(renderer, this->dino_ptr_run1.get(), nullptr, &this->dinoDestRect);
    else
        SDL_RenderCopy(renderer, this->dino_ptr_run2.get(), nullptr, &this->dinoDestRect);
}

// Updates the position of the cacti, moving them from right to left.
void Game::updateCactus(){
    const int speed = getCurrentTrackSpeed();
    if(largeCactus1DestRect.x > -largeCactus1.get_width())
        largeCactus1DestRect.x -= speed;
    else
        respawnCactus(largeCactus1DestRect);
    
    if(largeCactus2DestRect.x > -largeCactus2.get_width())
        largeCactus2DestRect.x -= speed;
    else
        respawnCactus(largeCactus2DestRect);

    if(largeCactus3DestRect.x > -largeCactus3.get_width())
        largeCactus3DestRect.x -= speed;
    else
        respawnCactus(largeCactus3DestRect);
}

void Game::updateScore(){
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime > lastScoreUpdate + 75) {  // every 75ms
        score++;
        lastScoreUpdate = currentTime;
    }
}

// Renders the current score to the top-left of the screen.
void Game::renderScore(SDL_Renderer* renderer){
    this->scoreSurface.reset(TTF_RenderText_Blended(this->score_font_ptr.get(), std::to_string(score).c_str(), textColor));
    if(this->scoreSurface == nullptr){
        throw std::runtime_error(TTF_GetError());
    }
    this->scoreDestRect = {10, 10, scoreSurface->w, scoreSurface->h};
    this->scoreTexture.reset(SDL_CreateTextureFromSurface(renderer, this->scoreSurface.get()));
    if(this->scoreTexture == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    SDL_RenderCopy(renderer, this->scoreTexture.get(), nullptr, &this->scoreDestRect);
}

void Game::renderCactus(){}

// The main game loop where all game logic, event handling, and rendering happens.
void Game::run(){
    // Continuous loop that runs until the game is quit.
    while(true){
        /*
        * Poll for events. SDL_PollEvent returns 1 if there is a pending event,
        * and 0 if there are none. The loop continues until the event queue is empty.
        */ 
        while (SDL_PollEvent(&this->event))
        {
            // checking the type of the event and take care of it accordingly
            switch (event.type)
            {
            // handle quit button(x at the top right window)
            case SDL_QUIT:
                return;
                break;
            // Handle keyboard input
            case SDL_KEYDOWN:
                if(!gameStarted){
                    gameStarted = true;
                    break;
                }
                switch (event.key.keysym.sym)
                {
                case SDLK_SPACE:
                    if(gameOver){
                        // Reset game state when the game is over and space is pressed.
                        gameOver = false;
                        score = 0;
                        resetCactusPositions();
                        isJumping = false;
                        dinoDestRect.y = dino.get_y();
                        velocityY = 0.0f;
                    }
                    // If not game over and not already jumping, initiate a jump.
                    else if(!isJumping){
                        isJumping = true;
                        velocityY = jumpStrength;
                    }
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }
        // Clear the renderer with the draw color (white).
        SDL_RenderClear(this->renderer.get());

        // --- Game Logic Update ---
        if(gameStarted && !gameOver){
            // Update all game elements if the game is active.
            this->trackUpdate();
            this->updateDinoAnimation();
            this->updateCactus();
            this->updateScore();
        }

        // --- Rendering ---
        // Render the scrolling track.
        SDL_RenderCopy(this->renderer.get(), this->track_ptr.get(), nullptr, &this->trackDestRect1);
        SDL_RenderCopy(this->renderer.get(), this->track_ptr.get(), nullptr, &this->trackDestRect2);
        // Render the dinosaur.
        this->renderDino(this->renderer.get());
        // Render the cacti.
        SDL_RenderCopy(this->renderer.get(), this->largeCactus1_ptr.get(), nullptr, &this->largeCactus1DestRect);
        SDL_RenderCopy(this->renderer.get(), this->largeCactus2_ptr.get(), nullptr, &this->largeCactus2DestRect);
        SDL_RenderCopy(this->renderer.get(), this->largeCactus3_ptr.get(), nullptr, &this->largeCactus3DestRect);
        if(gameStarted){
            // Render the score.
            this->renderScore(this->renderer.get());
        }
        if(!gameStarted){
            SDL_RenderCopy(this->renderer.get(), this->startTexture.get(), nullptr, &this->startDestRect);
        }
        else if(gameOver){
            // If the game is over, display the "Game Over" text.
            SDL_RenderCopy(this->renderer.get(), this->gameOverTexture.get(), nullptr, &this->gameOverDestRect);
            SDL_RenderCopy(this->renderer.get(), this->gameOverSubTextTexture.get(), nullptr, &this->gameOverSubTextDestRect);
        }
        // Present the back buffer to the screen to show the rendered frame.
        SDL_RenderPresent(this->renderer.get());
        // the delay create some what 60fps feeling
        SDL_Delay(16);
        // Check for collisions to trigger the game over state.
        const SDL_Surface* activeDinoSurface = useLeftFrame ? this->dino_run1_surface.get() : this->dino_run2_surface.get();
        if(gameStarted && !gameOver &&
            (has_precise_collision(activeDinoSurface, this->dinoDestRect, this->largeCactus1_surface.get(), this->largeCactus1DestRect) ||
             has_precise_collision(activeDinoSurface, this->dinoDestRect, this->largeCactus2_surface.get(), this->largeCactus2DestRect) ||
             has_precise_collision(activeDinoSurface, this->dinoDestRect, this->largeCactus3_surface.get(), this->largeCactus3DestRect))){
            gameOver = true;
        }
    }
}
