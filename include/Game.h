#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <memory>
class Game
{
private:
    static constexpr int Height = 600;
    static constexpr int Width = 800;
    SDL_Event event;
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> background;
    const std::string title = "Dino Game";

public:

    Game();
    void init();
    void loading_media();
    void run();
};

#endif // GAME_H