# READ ME

Project uses:

- [Raylib](https://github.com/raysan5/raylib)
- [raylib-cpp](https://github.com/RobLoach/raylib-cpp)
-

# CLI Setup

1 - Run

    cmake -B build -G "Ninja Multi-Config"

this will produce build files for your current platform. <sup>1</sup>

2 - Run

    cmake --build build --config Debug

Runs the build system created previously, and creates the executable.
`build` is the folder where these will be placed.
`--config Debug` tells cmake create a Debug build. This needs to be specified because we set the Ninja generator to be
Multi-Config, which gives us the option to create "Debug", "Release" and "RelWithDebInfo" builds.

3 - Run

    ./build/Debug/raylib-template

to run the executable we just created.

# CLion Setup

1 - Open the project and let CLion load the CMake project.

2 - Edit the CMake profile created, probably "Debug", and set Generator to
"Ninja Multi-Config". <sup>1</sup>

3 - Run (Shift + F10).

# Notes

<sup>1</sup> Ninja Multi-Config needs to be specified because the
`add_custom_target(copy_resources ...)` command expects a multi config setup.
It will try to copy the resources folder into this directory `${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/resources`
and `$<CONFIG>` won't be set unless we have a multi-config setup.

# Useful Links

[Raylib's Examples](https://www.raylib.com/examples.html)
[rayli-cpp documentation](https://robloach.github.io/raylib-cpp/index.html)

