#ifndef TRACK_H
#define TRACK_H
#include <string>
class Track
{
    private:
        static constexpr int height = 28;
        static constexpr int width = 2404;
        static constexpr int default_x = 0;
        static constexpr int default_y = 500;
        const std::string path = "assets/other/track.png";
    public:
        Track();
        ~Track();
        int get_height();
        int get_width();
        int get_default_x();
        int get_default_y();
        void update_x();
        std::string get_path();
};
#endif // TRACK_H