include(FetchContent)

FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG v5.5.0 # Keep this synced with raylib's version
        GIT_SHALLOW 1
        SYSTEM # Makes it so that target_compile_options doesn't apply to this
)
FetchContent_MakeAvailable(raylib_cpp)

# ralib_cpp depends on raylib. And we will depend on raylib_cpp
target_link_libraries(raylib_cpp INTERFACE raylib)