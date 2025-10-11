#ifndef DINO_H
#define DINO_H
#include <string>

// Represents the player-controlled dinosaur character.
class Dino
{
private:
    // --- Dino Constants ---
    static constexpr int height = 94; // Height of the dinosaur sprite.
    static constexpr int width = 88; // Width of the dinosaur sprite.
    static constexpr int x = 50; // Fixed horizontal position of the dinosaur on the screen.
    const int y = 440; // The dinosaur's default vertical position (on the ground).

    // --- Asset Paths ---
    const std::string PATH_RUN1 = "assets/dino/dinoRun1.png"; // Path to the first running animation frame.
    const std::string PATH_RUN2 = "assets/dino/dinoRun2.png"; // Path to the second running animation frame.

public:
    // Constructor.
    Dino();
    // Destructor.
    ~Dino();

    // --- Getters for dinosaur properties ---
    int get_height() const;
    int get_width() const;
    int get_x() const;
    int get_y() const;
    const std::string& get_path_run1() const;
    const std::string& get_path_run2() const;
};

#endif // DINO_H