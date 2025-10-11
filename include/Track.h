#ifndef TRACK_H
#define TRACK_H
#include <string>

// Represents the scrolling ground track in the game.
class Track
{
    private:
        // --- Track Constants ---
        static constexpr int height = 28; // Height of the track sprite.
        static constexpr int width = 2404; // Width of the track sprite (designed for seamless scrolling).
        static constexpr int default_x = 0; // Default starting horizontal position.
        static constexpr int default_y = 500; // Fixed vertical position of the track.
        const std::string path = "assets/other/track.png"; // File path to the track texture.

    public:
        // Constructor.
        Track();
        // Destructor.
        ~Track();

        // --- Getters for track properties ---
        int get_height() const;
        int get_width() const;
        int get_default_x() const;
        int get_default_y() const;
        const std::string& get_path() const;
};
#endif // TRACK_H