#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include <iostream>

class Game
{
private:
    static constexpr int Height = 600;
    static constexpr int Width = 800;

public:

    Game();
    void init();
};

#endif // GAME_H