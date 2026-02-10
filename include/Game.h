#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <memory>
#include <random>
#if __has_include(<SDL2/SDL.h>)
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#endif
#include "Dino.h"
#include "Track.h"
#include "Cactus.h"

class Game
{
private:
    // --- Game Constants ---
    static constexpr int Height = 720; // Window height
    static constexpr int Width = 1280; // Window width
    static constexpr int BASE_TRACK_SPEED = 8; // Starting horizontal speed of the game world
    static constexpr int MAX_TRACK_SPEED = 20; // Max horizontal speed cap for later-game difficulty
    static constexpr int SCORE_PER_SPEED_STEP = 100; // Score needed for each speed increase
    static constexpr int MIN_CACTUS_GAP = 260; // Minimum playable gap between consecutive cacti
    static constexpr int GAP_GROWTH_PER_SPEED = 18; // Keep patterns fair as speed increases
    static constexpr int MAX_CACTUS_GAP_RANDOM_EXTRA = 320; // Extra random gap range on top of minimum
    static constexpr int SCORE_FONT_SIZE = 30; // Font size for the score display
    static constexpr int GAME_OVER_FONT_SIZE = 80; // Font size for the "Game Over" text
    static constexpr Uint32 BASE_FRAME_INTERVAL = 150; // Starting dino run animation interval (ms)
    static constexpr Uint32 MIN_FRAME_INTERVAL = 80; // Minimum dino run animation interval (ms)
    const std::string title = "Dino Game"; // Window title
    const std::string START_TEXT = "Press  Any  Key"; // Start screen message
    const std::string GAME_OVER_TEXT = "Game Over!"; // Game over message
    const std::string GAME_OVER_SUB_TEXT = "Press  Space  to  Restart"; // Restart instruction

    // --- Cactus Properties ---
    const int largeCactus1_spawn_point = Width; // Initial X position for the first cactus
    const int largeCactus2_spawn_point = largeCactus1_spawn_point + 400; // Initial X for the second
    const int largeCactus3_spawn_point = largeCactus1_spawn_point + 800; // Initial X for the third

    // --- Random Number Generation for Cactus Spacing ---
    std::random_device rd; // Obtains a seed from the hardware
    std::mt19937 gen; // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<int> dist; // Generates random distances for cacti

    // --- SDL and Game Objects ---
    SDL_Color textColor = {0, 0, 0, 255}; // Black color for text rendering
    SDL_Event event; // Union to handle SDL events

    // Smart pointers for automatic resource management of SDL objects
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> track_ptr;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> dino_ptr_run1;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> dino_ptr_run2;
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> dino_run1_surface;
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> dino_run2_surface;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> largeCactus1_ptr;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> largeCactus2_ptr;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> largeCactus3_ptr;
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> largeCactus1_surface;
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> largeCactus2_surface;
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> largeCactus3_surface;
    std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> score_font_ptr;
    std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> game_over_font_ptr;
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> scoreSurface;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> scoreTexture;
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> startSurface;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> startTexture;
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> gameOverSurface;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> gameOverTexture;
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> gameOverSubTextSurface;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> gameOverSubTextTexture;

    // Game entity objects
    Dino dino;
    Track track;
    Cactus largeCactus1 = Cactus(95, 48, largeCactus1_spawn_point, "assets/cactus/largeCactus1.png");
    Cactus largeCactus2 = Cactus(95, 99, largeCactus2_spawn_point,"assets/cactus/largeCactus2.png");
    Cactus largeCactus3 = Cactus(95, 102, largeCactus3_spawn_point,"assets/cactus/largeCactus3.png");

    // SDL_Rects for positioning and sizing objects on the screen
    SDL_Rect trackDestRect1;
    SDL_Rect trackDestRect2;
    SDL_Rect dinoDestRect;
    SDL_Rect largeCactus1DestRect;
    SDL_Rect largeCactus2DestRect;
    SDL_Rect largeCactus3DestRect;
    SDL_Rect scoreDestRect;
    SDL_Rect startDestRect;
    SDL_Rect gameOverDestRect;
    SDL_Rect gameOverSubTextDestRect;

    // --- Game State and Logic Variables ---
    // World movement
    int floorOffsetX = 0; // Horizontal offset for the scrolling track

    // Dino animation
    bool useLeftFrame = true; // Toggles between dino's run animation frames
    Uint32 lastFrameTime = 0; // Time of the last animation frame switch

    // Dino jumping physics
    float velocityY = 0.0f; // Current vertical velocity of the dino
    float gravity = 0.5f; // Downward acceleration applied each frame
    float jumpStrength = -20; // Initial upward velocity for a jump
    bool isJumping = false; // Flag to check if the dino is currently jumping

    // Score
    int score = 0; // Player's current score
    Uint32 lastScoreUpdate = 0; // Time of the last score increment

    // Game state
    bool gameStarted = false; // Flag to indicate if the initial start key was pressed
    bool gameOver = false; // Flag to indicate if the game is over

public:
    // Constructor
    Game();

    // Initializes SDL, the window, and the renderer
    void init();

    // Loads all media (textures, fonts)
    void loading_media();

    // Contains the main game loop
    void run();

    // Updates the position of the track to create a scrolling effect
    void trackUpdate();

    // Returns a minimum safe cactus gap for the current speed
    int getMinimumCactusGap() const;

    // Returns a randomized cactus gap with a guaranteed minimum safe spacing
    int getRandomCactusGap();

    // Returns the right-most cactus position currently active
    int getFarthestCactusX() const;

    // Respawns one cactus after the farthest active cactus with a safe random gap
    void respawnCactus(SDL_Rect& cactusRect);

    // Resets all cactus positions with randomized but playable spacing
    void resetCactusPositions();

    // Returns the current movement speed based on score progression
    int getCurrentTrackSpeed() const;

    // Returns the dino run frame interval so animation speeds up with score
    Uint32 getCurrentFrameInterval() const;

    // Renders the dinosaur to the screen
    void renderDino(SDL_Renderer* renderer);

    // Updates the dinosaur's running animation frame
    void updateDinoRunAnimation();

    // Updates the dinosaur's state (jumping or running)
    void updateDinoAnimation();

    // Increments the score over time
    void updateScore();

    // Renders the current score to the screen
    void renderScore(SDL_Renderer* renderer);

    // Updates the positions of the cacti
    void updateCactus();

    // Renders the cacti (currently empty)
    void renderCactus();
};

#endif // GAME_H
