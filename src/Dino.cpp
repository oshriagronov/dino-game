#include "Dino.h"

Dino::Dino(/* args */)
{
}

Dino::~Dino() {}

int Dino::get_height(){
    return height;
}

int Dino::get_width(){
    return width;
}

std::string Dino::get_path_run1(){
    return PATH_RUN1;
}

std::string Dino::get_path_run2(){
    return PATH_RUN2;
}

int Dino::get_x(){
    return x;
}

int Dino::get_y(){
    return y;
}