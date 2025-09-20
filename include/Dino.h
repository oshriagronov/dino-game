#ifndef DINO_H
#define DINO_H
#include <string>
class Dino
{
private:
    static constexpr int height = 94;
    static constexpr int width = 88;
    static constexpr int x = 20;
    const std::string PATH_RUN1 = "assets/dino/dinoRun1.png";
    const std::string PATH_RUN2 = "assets/dino/dinoRun2.png";
    int y = 440;
public:
    Dino();
    ~Dino();
    int get_height();
    int get_width();
    int get_x();
    int get_y();
    void set_y(int y);
    std::string get_path_run1();
    std::string get_path_run2();
};

#endif // DINO_H