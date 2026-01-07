# CMake-Cards-Procedural

TCG game and gameplay testing tool. Written using Procedural Programing instead of OOP as an experiment after watching
Brian Will's ["Object-Oriented Programming is Bad" video](https://www.youtube.com/watch?v=QM1iUe6IofM).

## Libraries used:

- raylib - [Homepage](https://www.raylib.com/)
- raylib-cpp - [GitHub](https://github.com/RobLoach/raylib-cpp), [Docs](https://robloach.github.io/raylib-cpp/)
- dear imgui - [GitHub](https://github.com/ocornut/imgui)
- rlimgui - [GitHub](https://github.com/raylib-extras/rlImGui)

### For testing:

- catch2 - [GitHub](https://github.com/catchorg/Catch2)

## Assets Used

### Textures

- Kenney's Fantasy UI Borders - [Link](https://kenney.nl/assets/fantasy-ui-borders)
- Bakku No Oni - SABI 4 - [Archive.org Link](https://archive.org/details/bakku-no-oni-sabi4/)
- Pokemon TCG Game Boy Sprites - [Link](https://www.spriters-resource.com/game_boy_gbc/pokemontradingcardgame/)

### Audio

- Kenney's Interface Sounds - [Link](https://kenney.nl/assets/interface-sounds)
- Kenney's Casino Audio - [Link](https://kenney.nl/assets/casino-audio)
- Rusted Music Studio's Free Music Lo-Fi Tracks - [Link](https://rustedstudio.itch.io/free-music-lofi-tracks)

### Waiting implementation

- Diablo Luna ど苛ッ's Butterfly - looped ambience sounds pack. - [Link](https://pudretediablo.itch.io/butterfly)

# Useful Links

- [Raylib's Github](https://github.com/raysan5/raylib)
- [Raylib's Examples](https://www.raylib.com/examples.html)
- [Pokemon GB TCG Sprites](https://www.spriters-resource.com/game_boy_gbc/pokemontradingcardgame/)

# CLI Setup

0 - Make sure you have CMake and ninja-build installed on your system.

1 - Run

    cmake -B build -G "Ninja Multi-Config"

this will produce build files for your current platform. <sup>1</sup>

2 - Run

    cmake --build build --config Debug

Runs the build system created previously, and creates the executable.
`build` is the folder where these will be placed.
`--config Debug` tells CMake to create a Debug build. This needs to be specified because we set the Ninja generator to
be
Multi-Config, which gives us the option to create "Debug", "Release" and "RelWithDebInfo" builds.

3 - Run

    cd build/src/Debug
    ./main

to run the executable we just created.

# CLion Setup

1 - Open the project and let CLion load the CMake project.

2 - Edit the CMake profile created, probably "Debug", and set Generator to
"Ninja Multi-Config". <sup>1</sup>

3 - Run.

# Notes

<sup>1</sup> I am using Ninja Multi-Config to easily switch between a Debug config (has imgui sidebar for extra info)
and a Release config (just the game).

