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
    static constexpr int speed = 5;
    const std::string title = "Dino Game";
    SDL_Event event;
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> track_ptr;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> dino_ptr_run1;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> dino_ptr_run2;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> cactus_ptr;
    Dino dino;
    Track track;
    Cactus cactus;
    SDL_Rect trackDestRect1;
    SDL_Rect trackDestRect2;
    SDL_Rect dinoDestRect;
    SDL_Rect cactusDestRect;
    // game world movement: track, dino run etc
    int floorOffsetX = 0;
    bool useLeftFrame = true;
    Uint32 lastFrameTime = 0;       // time of last frame switch
    Uint32 frameInterval = 150;     // how often to switch (ms)
public:
    Game();
    void init();
    void loading_media();
    void run();
    void track_movement();
    void renderDino(SDL_Renderer* renderer);
    void updateDinoAnimation();
};

#endif // GAME_H