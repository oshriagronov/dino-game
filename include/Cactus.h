#ifndef CACTUS_H
#define CACTUS_H
#include <string>
class Cactus
{
    private:
        const int y = 430;
        int x;
        int height;
        int width;
        std::string path;
    public:
        Cactus(int height, int width, int x, std::string path);
        ~Cactus();
        int get_height();
        int get_width();
        int get_x();
        int get_y();
        std::string get_path();

};

#endif // CACTUS_H
