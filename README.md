<br />
<div align="center">
  <a href="https://github.com/oshriagronov/dino-game">
    <img src="/assets/logo.png" alt="Logo" width="200" height="200">
  </a>

<h3 align="center">T-Rex Runner</h3>
  <p align="center">
    A simple C++ clone of Chrome's offline Dinosaur game built with SDL.
  </p>
</div>

## About

This project is a C++ recreation of Chrome's classic offline Dinosaur game, developed using SDL. The game features smooth character animation, jumping mechanics, and obstacle generation, providing a fun way to practice C++ fundamentals, object-oriented programming, and game development concepts.

## Technologies used

- C++
- SDL2
- memory
- random

## Getting Started

To get a local copy up and running follow these simple steps.

### Prerequisites

- Linux, MacOS or Windows
- C++ compiler
- SDL2 library - SDL2, SDL_image, SDL2_ttf

> SDL2 installation guide can be found [here](https://wiki.libsdl.org/SDL2/Installation)

### Installation

---

1. **Clone and enter the dino-game repository:**

   ```bash
   git clone https://github.com/oshriagronov/dino-game && cd dino-game
   ```

2. **Compile:**

   ```bash
   g++ src/*.cpp -I include -o demo -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf
   ```

3. **Run:**

   ```bash
   ./demo
   ```

## Acknowledgements

I would like to thank the youtuber "Programming Rainbow" for creating the great playlist "Beginners Guide to SDL2 in C++", that helped me a lot.

> Link to the playlist can be found [here](https://www.youtube.com/playlist?list=PLO02jwa2ZaiBNRSRRyzS7sg1isf-K2OEI)
