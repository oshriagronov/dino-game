// g++ src/*.cpp -I include -o demo -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf
// g++ -std=c++17 src/*.cpp -Iinclude $(pkg-config --cflags sdl2 SDL2_image SDL2_ttf) -o demo $(pkg-config --libs sdl2 SDL2_image SDL2_ttf)
#include "Game.h"
// Function prototypes for SDL initialization and cleanup
void initialize_sdl();
void close_sdl();

// Main entry point of the application
int main(){
    try
    {
        // Initialize all SDL subsystems
        initialize_sdl();
        Game game;
        game.init();
        game.loading_media();
        game.run();
    }
    catch(const std::exception& e)
    {
        // Catch and report any exceptions during initialization or runtime
        std::cerr << e.what() << '\n';
    }
    // Clean up SDL resources before exiting
    close_sdl();
    return EXIT_SUCCESS;
}

// Initializes SDL, SDL_image, and SDL_ttf libraries
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

// Shuts down and cleans up SDL, SDL_image, and SDL_ttf
void close_sdl() {
    TTF_Quit();
    IMG_Quit(); 
    SDL_Quit(); 
}