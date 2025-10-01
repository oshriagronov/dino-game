#include "Game.h"
Game::Game():
    window{nullptr, SDL_DestroyWindow},
    renderer{nullptr, SDL_DestroyRenderer}, 
    track_ptr{nullptr, SDL_DestroyTexture},
    dino_ptr_run1{nullptr, SDL_DestroyTexture},
    dino_ptr_run2{nullptr, SDL_DestroyTexture},
    cactus_ptr{nullptr, SDL_DestroyTexture},
    score_font_ptr{nullptr, TTF_CloseFont},
    game_over_font_ptr{nullptr, TTF_CloseFont},
    scoreSurface{nullptr, SDL_FreeSurface},
    scoreTexture{nullptr, SDL_DestroyTexture},
    gameOverSurface{nullptr, SDL_FreeSurface},
    gameOverTexture{nullptr, SDL_DestroyTexture},
    gameOverSubTextSurface{nullptr, SDL_FreeSurface},
    gameOverSubTextTexture{nullptr, SDL_DestroyTexture},
    trackDestRect1{-floorOffsetX, track.get_default_y(), track.get_width(), track.get_height()},
    dinoDestRect{dino.get_x(), dino.get_y(), dino.get_width(), dino.get_height()},
    cactusDestRect{spawn_point, cactus.get_y(), cactus.get_width(), cactus.get_height()},
    trackDestRect2{-floorOffsetX + track.get_width(), track.get_default_y(), track.get_width(), track.get_height()}
{}


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

void Game::loading_media(){
    this->track_ptr.reset(IMG_LoadTexture(
        this->renderer.get(), 
        track.get_path().c_str()));
    if(this->track_ptr == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    this->dino_ptr_run1.reset(IMG_LoadTexture(
        this->renderer.get(), 
        dino.get_path_run1().c_str()));
    if(this->dino_ptr_run1 == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    this->dino_ptr_run2.reset(IMG_LoadTexture(
        this->renderer.get(), 
        dino.get_path_run2().c_str()));
    if(this->dino_ptr_run2 == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    this->cactus_ptr.reset(IMG_LoadTexture(
        this->renderer.get(), 
        cactus.get_path().c_str()));
    if(this->cactus_ptr == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    this->score_font_ptr.reset(TTF_OpenFont("assets/ArcadeClassic.ttf", SCORE_FONT_SIZE));
    if(this->score_font_ptr == nullptr){
        throw std::runtime_error(TTF_GetError());
    }
    this->game_over_font_ptr.reset(TTF_OpenFont("assets/ArcadeClassic.ttf", GAME_OVER_FONT_SIZE));
    if(this->game_over_font_ptr == nullptr){
        throw std::runtime_error(TTF_GetError());
    }
    this->gameOverSurface.reset(TTF_RenderText_Blended(this->game_over_font_ptr.get(), this->GAME_OVER_TEXT.c_str(), textColor));
    if(this->gameOverSurface == nullptr){
        throw std::runtime_error(TTF_GetError());
    }
    this->gameOverDestRect = {Width / 4 + 10, Height / 3, gameOverSurface->w, gameOverSurface->h};
    this->gameOverTexture.reset(SDL_CreateTextureFromSurface(this->renderer.get(), this->gameOverSurface.get()));
    if(this->gameOverTexture == nullptr){
        throw std::runtime_error(SDL_GetError());
    }

    this->gameOverSubTextSurface.reset(TTF_RenderText_Blended(this->score_font_ptr.get(), this->GAME_OVER_SUB_TEXT.c_str(), textColor));
    if(this->gameOverSubTextSurface == nullptr){
        throw std::runtime_error(TTF_GetError());
    }
    this->gameOverSubTextDestRect = {Width / 4 + 30, Height / 2, gameOverSubTextSurface->w, gameOverSubTextSurface->h};
    this->gameOverSubTextTexture.reset(SDL_CreateTextureFromSurface(this->renderer.get(), this->gameOverSubTextSurface.get()));
    if(this->gameOverSubTextTexture == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
}
void Game::trackUpdate(){
    floorOffsetX += TRACK_SPEED;
    if (floorOffsetX >= track.get_width()) {
        floorOffsetX = track.get_default_x();
    }
    this->trackDestRect1.x = -floorOffsetX + 5;
    this->trackDestRect2.x = -floorOffsetX + track.get_width();
}
void Game::updateDinoRunAnimation(){
    Uint32 now = SDL_GetTicks();
    if (now - lastFrameTime > frameInterval) {
        useLeftFrame = !useLeftFrame;   // toggle frame
        lastFrameTime = now;
    }
}

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

void Game::renderDino(SDL_Renderer* renderer){
    if(useLeftFrame)
        SDL_RenderCopy(renderer, this->dino_ptr_run1.get(), nullptr, &this->dinoDestRect);
    else
        SDL_RenderCopy(renderer, this->dino_ptr_run2.get(), nullptr, &this->dinoDestRect);
}

void Game::updateCactus(){
    if(cactusDestRect.x > -cactus.get_width())
        cactusDestRect.x -= TRACK_SPEED;
    else
        cactusDestRect.x = spawn_point;
}

void Game::updateScore(){
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime > lastScoreUpdate + 75) {  // every 75ms
        score++;
        lastScoreUpdate = currentTime;
    }
}

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
// game loop
void Game::run(){
    // continuos loop   
    while(true){
        /*
        * poll of events, take care of the events one by one until they all done which the function
        * will return 0 when there no events to take care of, and 1 when there still events to take care of.
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
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_SPACE:
                    if(gameOver){
                        // reset game state when the game is over and space is pressed
                        gameOver = false;
                        score = 0;
                        cactusDestRect.x = spawn_point;
                        isJumping = false;
                        dinoDestRect.y = dino.get_y();
                        velocityY = 0.0f;
                    }
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
        SDL_RenderClear(this->renderer.get());
        if(!gameOver){
            // world moving
            this->trackUpdate();
            this->updateDinoAnimation();
            this->updateCactus();
            this->updateScore();
        }
        else{
            SDL_RenderCopy(this->renderer.get(), this->gameOverTexture.get(), nullptr, &this->gameOverDestRect);
            SDL_RenderCopy(this->renderer.get(), this->gameOverSubTextTexture.get(), nullptr, &this->gameOverSubTextDestRect);
        }
        // rendering.
        SDL_RenderCopy(this->renderer.get(), this->track_ptr.get(), nullptr, &this->trackDestRect1);
        SDL_RenderCopy(this->renderer.get(), this->track_ptr.get(), nullptr, &this->trackDestRect2);
        // function the render the dino movement base on the frame
        this->renderDino(this->renderer.get());
        SDL_RenderCopy(this->renderer.get(), this->cactus_ptr.get(), nullptr, &this->cactusDestRect);
        this->renderScore(this->renderer.get());
        // present the back buffer
        SDL_RenderPresent(this->renderer.get());
        // the delay create some what 60fps feeling
        SDL_Delay(16);
        //game over check, need to work on game over screen and cleanup here.
        if(!gameOver && SDL_HasIntersection(&this->dinoDestRect, &this->cactusDestRect)){
            gameOver = true;
        }
    }
}