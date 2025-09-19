#ifndef TRACK_H
#define TRACK_H
#include <string>
class Track
{
    private:
        static constexpr int height = 28;
        static constexpr int width = 800;
        static constexpr int x = 0;
        static constexpr int y = 500;
        const std::string path = "assets/other/track.png";
    public:
        Track();
        ~Track();
        int get_height();
        int get_width();
        int get_x();
        int get_y();
        std::string get_path();

};
#endif // TRACK_H