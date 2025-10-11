#include "Dino.h"

// Constructor: Initializes the Dino object.
Dino::Dino()
{
}

// Destructor: Cleans up the Dino object.
Dino::~Dino() {}

// Returns the height of the dinosaur sprite.
int Dino::get_height() const {
    return height;
}

// Returns the width of the dinosaur sprite.
int Dino::get_width() const {
    return width;
}

// Returns the file path for the first running animation frame.
const std::string& Dino::get_path_run1() const {
    return PATH_RUN1;
}

// Returns the file path for the second running animation frame.
const std::string& Dino::get_path_run2() const {
    return PATH_RUN2;
}

// Returns the fixed horizontal position of the dinosaur.
int Dino::get_x() const {
    return x;
}

// Returns the default vertical position of the dinosaur (on the ground).
int Dino::get_y() const {
    return y;
}