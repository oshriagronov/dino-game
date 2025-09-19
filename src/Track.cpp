#include "Track.h"

Track::Track(){}

Track::~Track(){}
int Track::get_height(){
    return height;
}

int Track::get_width(){
    return width;
}

int Track::get_x(){
    return x;
}

int Track::get_y(){
    return y;
}

std::string Track::get_path(){
    return path;
}
