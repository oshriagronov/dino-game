#include "Cactus.h"

// Constructor: Initializes a Cactus object using a member initializer list.
// This is more efficient than assigning values inside the constructor body.
Cactus::Cactus(int height, int width, int x, std::string path): 
    x(x),
    height(height),
    width(width),
    path(path) 
{
}

// Destructor: Cleans up the Cactus object. Currently empty as no dynamic memory is managed directly by this class.
Cactus::~Cactus() {}

// Returns the height of the cactus.
int Cactus::get_height() const {
    return height;
}

// Returns the width of the cactus.
int Cactus::get_width() const {
    return width;
}

// Returns the current horizontal position of the cactus.
int Cactus::get_x() const {
    return x;
}

// Returns the fixed vertical position of the cactus.
int Cactus::get_y() const {
    return y;
}

// Returns the file path for the cactus's texture.
// Returning by const reference avoids making a copy of the string.
const std::string& Cactus::get_path() const {
    return path;
}