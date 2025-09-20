#include "Track.h"

Track::Track(){}

Track::~Track(){}
int Track::get_height(){
    return height;
}

int Track::get_width(){
    return width;
}

int Track::get_default_x(){
    return default_x;
}

int Track::get_default_y(){
    return default_y;
}

std::string Track::get_path(){
    return path;
}
