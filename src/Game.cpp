#include "Game.h"
Game::Game():
    window{nullptr, SDL_DestroyWindow},
    renderer{nullptr, SDL_DestroyRenderer}, 
    track_ptr{nullptr, SDL_DestroyTexture},
    dino_ptr_run1{nullptr, SDL_DestroyTexture},
    dino_ptr_run2{nullptr, SDL_DestroyTexture},
    cactus_ptr{nullptr, SDL_DestroyTexture},
    trackDestRect1{-floorOffsetX, track.get_default_y(), track.get_width(), track.get_height()},
    dinoDestRect{dino.get_x(), dino.get_y(), dino.get_width(), dino.get_height()},
    cactusDestRect{cactus.get_x(), cactus.get_y(), cactus.get_width(), cactus.get_height()},
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
}
void Game::track_movement(){
    floorOffsetX += speed;
    if (floorOffsetX >= track.get_width()) {
        floorOffsetX = track.get_default_x();
    }
    this->trackDestRect1.x = -floorOffsetX + 5;
    this->trackDestRect2.x = -floorOffsetX + track.get_width();
}
void Game::updateDinoAnimation(){
    Uint32 now = SDL_GetTicks();
    if (now - lastFrameTime > frameInterval) {
        useLeftFrame = !useLeftFrame;   // toggle frame
        lastFrameTime = now;
    }
}
void Game::renderDino(SDL_Renderer* renderer){
    if(useLeftFrame)
        SDL_RenderCopy(renderer, this->dino_ptr_run1.get(), nullptr, &this->dinoDestRect);
    else
        SDL_RenderCopy(renderer, this->dino_ptr_run2.get(), nullptr, &this->dinoDestRect);
}


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
            default:
                break;
            }
        }
        // world moving
        this->track_movement();
        this->updateDinoAnimation();
        // rendering.
        SDL_RenderClear(this->renderer.get());
        SDL_RenderCopy(this->renderer.get(), this->track_ptr.get(), nullptr, &this->trackDestRect1);
        SDL_RenderCopy(this->renderer.get(), this->track_ptr.get(), nullptr, &this->trackDestRect2);
        // function the render the dino movement base on the frame
        this->renderDino(this->renderer.get());
        //SDL_RenderCopy(this->renderer.get(), this->dino_ptr.get(), nullptr, &this->dinoDestRect);
        //SDL_RenderCopy(this->renderer.get(), this->cactus_ptr.get(), nullptr, &this->cactusDestRect);
        SDL_RenderPresent(this->renderer.get());
        // the delay create some what 60fps feeling
        SDL_Delay(16);
    }
}