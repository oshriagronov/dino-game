#include "Game.h"
#include <filesystem>

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

std::string asset_path(const std::filesystem::path& base, const std::string& relative) {
    return (base / relative).string();
}
} // namespace

// Constructor: Initializes all member variables, especially the smart pointers and SDL_Rects.
Game::Game():
    window{nullptr, SDL_DestroyWindow},
    renderer{nullptr, SDL_DestroyRenderer}, 
    track_ptr{nullptr, SDL_DestroyTexture},
    dino_ptr_run1{nullptr, SDL_DestroyTexture},
    dino_ptr_run2{nullptr, SDL_DestroyTexture},
    largeCactus1_ptr{nullptr, SDL_DestroyTexture},
    largeCactus2_ptr{nullptr, SDL_DestroyTexture},
    largeCactus3_ptr{nullptr, SDL_DestroyTexture},
    score_font_ptr{nullptr, TTF_CloseFont},
    game_over_font_ptr{nullptr, TTF_CloseFont},
    scoreSurface{nullptr, SDL_FreeSurface},
    scoreTexture{nullptr, SDL_DestroyTexture},
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
{}

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
    const auto base_path = get_base_path();
    this->track_ptr.reset(IMG_LoadTexture(
        this->renderer.get(), 
        asset_path(base_path, track.get_path()).c_str()));
    if(this->track_ptr == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    this->dino_ptr_run1.reset(IMG_LoadTexture(
        this->renderer.get(), 
        asset_path(base_path, dino.get_path_run1()).c_str()));
    if(this->dino_ptr_run1 == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    this->dino_ptr_run2.reset(IMG_LoadTexture(
        this->renderer.get(), 
        asset_path(base_path, dino.get_path_run2()).c_str()));
    if(this->dino_ptr_run2 == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    this->largeCactus1_ptr.reset(IMG_LoadTexture(
        this->renderer.get(), 
        asset_path(base_path, largeCactus1.get_path()).c_str()));
    if(this->largeCactus1_ptr == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    this->largeCactus2_ptr.reset(IMG_LoadTexture(
        this->renderer.get(), 
        asset_path(base_path, largeCactus2.get_path()).c_str()));
    if(this->largeCactus2_ptr == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    this->largeCactus3_ptr.reset(IMG_LoadTexture(
        this->renderer.get(), 
        asset_path(base_path, largeCactus3.get_path()).c_str()));
    if(this->largeCactus3_ptr == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    this->score_font_ptr.reset(TTF_OpenFont(asset_path(base_path, "assets/ArcadeClassic.ttf").c_str(), SCORE_FONT_SIZE));
    if(this->score_font_ptr == nullptr){
        throw std::runtime_error(TTF_GetError());
    }
    this->game_over_font_ptr.reset(TTF_OpenFont(asset_path(base_path, "assets/ArcadeClassic.ttf").c_str(), GAME_OVER_FONT_SIZE));
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
}

// Updates the track's position to create an infinite scrolling effect.
void Game::trackUpdate(){
    floorOffsetX += TRACK_SPEED;
    if (floorOffsetX >= track.get_width()) {
        floorOffsetX = track.get_default_x();
    }
    this->trackDestRect1.x = -floorOffsetX + 5;
    this->trackDestRect2.x = -floorOffsetX + track.get_width();
}

// Toggles between the two running animation frames for the dinosaur.
void Game::updateDinoRunAnimation(){
    Uint32 now = SDL_GetTicks();
    if (now - lastFrameTime > frameInterval) {
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
    if(largeCactus1DestRect.x > -largeCactus1.get_width())
        largeCactus1DestRect.x -= TRACK_SPEED;
    else
        largeCactus1DestRect.x = largeCactus1_spawn_point;
    
    if(largeCactus2DestRect.x > -largeCactus2.get_width())
        largeCactus2DestRect.x -= TRACK_SPEED;
    else
        largeCactus2DestRect.x = largeCactus1DestRect.x + dist(gen);

    if(largeCactus3DestRect.x > -largeCactus3.get_width())
        largeCactus3DestRect.x -= TRACK_SPEED;
    else
        largeCactus3DestRect.x = largeCactus2DestRect.x  + dist(gen);
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
                switch (event.key.keysym.sym)
                {
                case SDLK_SPACE:
                    if(gameOver){
                        // Reset game state when the game is over and space is pressed.
                        gameOver = false;
                        score = 0;
                        largeCactus1DestRect.x = largeCactus1_spawn_point;
                        largeCactus2DestRect.x = largeCactus2_spawn_point;
                        largeCactus3DestRect.x = largeCactus3_spawn_point;
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
        if(!gameOver){
            // Update all game elements if the game is active.
            this->trackUpdate();
            this->updateDinoAnimation();
            this->updateCactus();
            this->updateScore();
        }
        else{
            // If the game is over, display the "Game Over" text.
            SDL_RenderCopy(this->renderer.get(), this->gameOverTexture.get(), nullptr, &this->gameOverDestRect);
            SDL_RenderCopy(this->renderer.get(), this->gameOverSubTextTexture.get(), nullptr, &this->gameOverSubTextDestRect);
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
        // Render the score.
        this->renderScore(this->renderer.get());
        // Present the back buffer to the screen to show the rendered frame.
        SDL_RenderPresent(this->renderer.get());
        // the delay create some what 60fps feeling
        SDL_Delay(16);
        // Check for collisions to trigger the game over state.
        if(!gameOver && (SDL_HasIntersection(&this->dinoDestRect, &this->largeCactus1DestRect) || SDL_HasIntersection(&this->dinoDestRect, &this->largeCactus2DestRect) || SDL_HasIntersection(&this->dinoDestRect, &this->largeCactus3DestRect))){
            gameOver = true;
        }
    }
}
