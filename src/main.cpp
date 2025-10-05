// g++ src/*.cpp -I include -o demo -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf
#include "Game.h"
void initialize_sdl();
void close_sdl();
int main(){
    try
    {
        initialize_sdl();
        Game game;
        game.init();
        game.loading_media();
        game.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    close_sdl();
    return EXIT_SUCCESS;
}

void initialize_sdl(){
        if (SDL_Init(SDL_INIT_EVERYTHING)) {
            throw std::runtime_error(
                std::string("Error initializing SDL: ") + SDL_GetError());
        }
        if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
            throw std::runtime_error(
                std::string("Error initializing SDL_image: ") + IMG_GetError());
        }
        if (TTF_Init()) {
            throw std::runtime_error(
                std::string("Error initializing SDL_ttf: ") + TTF_GetError());
        }
}

void close_sdl() {
    TTF_Quit();
    IMG_Quit(); 
    SDL_Quit(); 
}