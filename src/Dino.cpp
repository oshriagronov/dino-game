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

std::string Dino::get_path(){
    return path;
}

int Dino::get_x(){
    return x;
}

int Dino::get_y(){
    return y;
}

void Dino::set_y(int y){
    this->y = y;
}