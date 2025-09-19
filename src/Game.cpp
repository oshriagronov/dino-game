#include "Game.h"
Game::Game():
    window{nullptr, SDL_DestroyWindow},
    renderer{nullptr, SDL_DestroyRenderer}, 
    background{nullptr, SDL_DestroyTexture}
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
    this->background.reset(IMG_LoadTexture(
        this->renderer.get(), 
        "assets/other/track.png"));
    if(this->background == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
}

// game loop
void Game::run(){
    // continuos loop   
    while(true){
        // track rectangle, will be moved to it's own class later
        SDL_Rect srcRect = {0, 0, 800, 28};
        SDL_Rect destRect = {0, 500, 800, 28};
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
        SDL_RenderCopy(this->renderer.get(), this->background.get(), &srcRect, &destRect);
        SDL_RenderPresent(this->renderer.get());
        // the delay create some what 60fps feeling
        SDL_Delay(16);
    }
}