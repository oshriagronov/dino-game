#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Dino.h"
#include "Track.h"
#include "Cactus.h"
class Game
{
private:
    static constexpr int Height = 600;
    static constexpr int Width = 800;
    static constexpr int TRACK_SPEED = 8;
    static constexpr int SCORE_FONT_SIZE = 30;
    static constexpr int GAME_OVER_FONT_SIZE = 80;
    const std::string title = "Dino Game";
    const std::string GAME_OVER_TEXT = "Game Over!";
    const std::string GAME_OVER_SUB_TEXT = "Press Space to Restart";
    SDL_Color textColor = {0, 0, 0, 255}; // Black color
    SDL_Event event;
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> track_ptr;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> dino_ptr_run1;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> dino_ptr_run2;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> cactus_ptr;
    std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> score_font_ptr;
    std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> game_over_font_ptr;
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> scoreSurface;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> scoreTexture;
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> gameOverSurface;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> gameOverTexture;
    Dino dino;
    Track track;
    Cactus cactus;
    SDL_Rect trackDestRect1;
    SDL_Rect trackDestRect2;
    SDL_Rect dinoDestRect;
    SDL_Rect cactusDestRect;
    SDL_Rect scoreDestRect;
    SDL_Rect gameOverDestRect;
    // game world movement: track, dino run etc
    int floorOffsetX = 0;
    bool useLeftFrame = true;
    Uint32 lastFrameTime = 0;       // time of last frame switch
    Uint32 frameInterval = 150;     // how often to switch (ms)
    // Jumping physics
    float velocityY = 0.0f;
    float gravity = 0.5f;     // strength of gravity
    float jumpStrength = -20; // initial upward velocity (negative = up)
    bool isJumping = false;
    //cactus section
    const int spawn_point = Width + 100;
    // score section
    int score = 0;
    Uint32 lastScoreUpdate = 0; // time of last score update
    // end game flag
    bool gameOver = false;
public:
    Game();
    void init();
    void loading_media();
    void run();
    void trackUpdate();
    void renderDino(SDL_Renderer* renderer);
    void updateDinoRunAnimation();
    void updateDinoAnimation();
    void updateScore();
    void renderScore(SDL_Renderer* renderer);
    void updateCactus();
    void renderCactus();
};

#endif // GAME_H