#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Dino.h"
#include "Track.h"
#include "Cactus.h"
class Game
{
private:
    static constexpr int Height = 600;
    static constexpr int Width = 800;
    const std::string title = "Dino Game";
    SDL_Event event;
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> track_ptr;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> dino_ptr;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> cactus_ptr;
    Dino dino;
    Track track;
    Cactus cactus;
public:

    Game();
    void init();
    void loading_media();
    void run();
};

#endif // GAME_H