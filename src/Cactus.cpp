#include "Cactus.h"
Cactus::Cactus(){}

Cactus::~Cactus(){}
int Cactus::get_height(){
    return height;
}

int Cactus::get_width(){
    return width;
}

int Cactus::get_x(){
    return x;
}

int Cactus::get_y(){
    return y;
}

std::string Cactus::get_path(){
    return path;
}