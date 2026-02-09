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
- CMake
- vcpkg

## Download and Play

From GitHub Releases, download the package for your platform:

- **Windows portable:** `dino-game-windows-portable.zip` (extract and double-click `dino-game.exe`)
- **macOS portable:** `dino-game-macos-portable.zip` (unzip and open `dino-game.app`)
- **Linux AppImage:** `dino-game-linux-x86_64.AppImage` (single file, no install; if needed run `chmod +x dino-game-linux-x86_64.AppImage` once)

If macOS says the app is damaged or blocked, run once:

```bash
xattr -dr com.apple.quarantine dino-game.app
```

## Manual Build and Test (Local)

### Prerequisites

- Linux, macOS, or Windows
- C++ compiler
- CMake 3.21+
- vcpkg

### 1. Clone

```bash
git clone https://github.com/oshriagronov/dino-game && cd dino-game
```

### 2. Configure

Use the vcpkg toolchain and a platform triplet:

- Linux: `x64-linux`
- macOS Apple Silicon: `arm64-osx`
- macOS Intel: `x64-osx`
- Windows: `x64-windows`

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=<triplet>
```

### 3. Build

```bash
cmake --build build --config Release
```

### 4. Install to a runnable folder

```bash
cmake --install build --config Release --prefix dist
```

### 5. Run

- Windows: `dist\dino-game.exe`
- macOS: `open dist/dino-game.app`
- Linux: `./dist/dino-game`

## Local Packaging

### Linux AppImage

From the project root:

```bash
mkdir -p AppDir/usr/bin
cp -a dist/. AppDir/usr/bin/
cp packaging/linux/AppRun AppDir/AppRun
cp packaging/linux/dino-game.desktop AppDir/dino-game.desktop
cp assets/logo.png AppDir/dino-game.png
mkdir -p AppDir/usr/share/icons/hicolor/256x256/apps
cp assets/logo.png AppDir/usr/share/icons/hicolor/256x256/apps/dino-game.png
wget -q https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage -O appimagetool
chmod +x appimagetool
ARCH=x86_64 APPIMAGE_EXTRACT_AND_RUN=1 ./appimagetool AppDir dino-game-linux-x86_64.AppImage
```

Run it:

```bash
chmod +x dino-game-linux-x86_64.AppImage
./dino-game-linux-x86_64.AppImage
```

### Windows Portable Zip

Build and install to `dist`, then zip that folder:

```powershell
cmake -E tar cfv dino-game-windows-portable.zip --format=zip dist
```

Users only need to extract and run `dino-game.exe`.

## Test Checklist

After any local build/package, test these quickly:

1. Game window opens.
2. Space key jumps.
3. Collision triggers game over.
4. Space restarts after game over.
5. No missing textures/fonts.

## CI/CD Release Flow

A GitHub Action builds and packages all platforms.

- Manual run: trigger `Build and Package` workflow.
- Tagged release: push a tag like `v0.1.0` and artifacts are attached automatically.
- macOS release bundle is re-signed and zipped with `ditto` during CI packaging.

## Acknowledgements

I would like to thank the youtuber "Programming Rainbow" for creating the great playlist "Beginners Guide to SDL2 in C++", that helped me a lot.

> Link to the playlist can be found [here](https://www.youtube.com/playlist?list=PLO02jwa2ZaiBNRSRRyzS7sg1isf-K2OEI)
