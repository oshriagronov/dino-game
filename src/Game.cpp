#include "Game.h"
Game::Game():
    window{nullptr, SDL_DestroyWindow},
    renderer{nullptr, SDL_DestroyRenderer}, 
    track_ptr{nullptr, SDL_DestroyTexture},
    dino_ptr{nullptr, SDL_DestroyTexture},
    cactus_ptr{nullptr, SDL_DestroyTexture}
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
    this->dino_ptr.reset(IMG_LoadTexture(
        this->renderer.get(), 
        dino.get_path().c_str()));
    if(this->dino_ptr == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    this->cactus_ptr.reset(IMG_LoadTexture(
        this->renderer.get(), 
        cactus.get_path().c_str()));
    if(this->dino_ptr == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
}

// game loop
void Game::run(){
    // continuos loop   
    // track rectangle, will be moved to it's own class later
    SDL_Rect trackSrcRect = {0, 0, track.get_width(), track.get_height()};
    SDL_Rect trackDestRect = {0, track.get_y(), track.get_width(), track.get_height()};
    // dino rectangle, will be moved to it's own class later
    SDL_Rect dinoSrcRect = {0, 0, dino.get_width(), dino.get_height()};
    // the number 500-60 is for the dino to be exactly on the track
    SDL_Rect dinoDestRect = {20, 500 - 60, dino.get_width(), dino.get_height()};
    // cactus rectangle
    SDL_Rect cactusSrcRect = {0, 0, cactus.get_width(), cactus.get_height()};
    SDL_Rect cactusDestRect = {cactus.get_x(), cactus.get_y(), cactus.get_width(), cactus.get_height()};
    // game loop
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
        // rendering.

        SDL_RenderClear(this->renderer.get());
        SDL_RenderCopy(this->renderer.get(), this->track_ptr.get(), &trackSrcRect, &trackDestRect);
        SDL_RenderCopy(this->renderer.get(), this->dino_ptr.get(), &dinoSrcRect, &dinoDestRect);
        SDL_RenderCopy(this->renderer.get(), this->cactus_ptr.get(), &cactusSrcRect, &cactusDestRect);
        SDL_RenderPresent(this->renderer.get());
        // the delay create some what 60fps feeling
        SDL_Delay(16);
    }
}