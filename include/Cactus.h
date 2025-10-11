#ifndef CACTUS_H
#define CACTUS_H
#include <string>

// Represents a single cactus obstacle in the game.
class Cactus
{
    private:
        const int y = 430; // The fixed vertical position of the cactus on the ground.
        int x; // The horizontal position of the cactus. Changes as it scrolls.
        int height; // The height of the cactus sprite.
        int width; // The width of the cactus sprite.
        std::string path; // The file path to the cactus texture.

    public:
        // Constructor to initialize a cactus with its properties.
        Cactus(int height, int width, int x, std::string path);
        // Destructor.
        ~Cactus();

        // --- Getters for cactus properties ---
        // Returns the height of the cactus.
        int get_height() const;
        // Returns the width of the cactus.
        int get_width() const;
        // Returns the current horizontal position of the cactus.
        int get_x() const;
        // Returns the fixed vertical position of the cactus.
        int get_y() const;
        // Returns the file path for the cactus's texture.
        const std::string& get_path() const;
};

#endif // CACTUS_H
