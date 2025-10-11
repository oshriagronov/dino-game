#include "Track.h"

// Constructor: Initializes the Track object.
Track::Track() {}

// Destructor: Cleans up the Track object.
Track::~Track() {}

// Returns the height of the track sprite.
int Track::get_height() const {
    return height;
}

// Returns the width of the track sprite.
int Track::get_width() const {
    return width;
}

// Returns the default starting horizontal position of the track.
int Track::get_default_x() const {
    return default_x;
}

// Returns the fixed vertical position of the track.
int Track::get_default_y() const {
    return default_y;
}

// Returns the file path for the track's texture.
const std::string& Track::get_path() const {
    return path;
}
