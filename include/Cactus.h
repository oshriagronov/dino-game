#ifndef CACTUS_H
#define CACTUS_H
#include <string>
class Cactus
{
    private:
        static constexpr int height = 95;
        static constexpr int width = 48;
        static constexpr int x = 500;
        static constexpr int y = 430;
        const std::string path = "assets/cactus/largeCactus1.png";
    public:
        Cactus();
        ~Cactus();
        int get_height();
        int get_width();
        int get_x();
        int get_y();
        std::string get_path();

};

#endif // CACTUS_H
