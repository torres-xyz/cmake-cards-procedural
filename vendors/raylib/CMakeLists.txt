include(FetchContent)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
        GIT_SHALLOW 1 # This performs a shallow clone, which is faster as it skips downloading historical commits.
        SYSTEM # Makes it so that target_compile_options doesn't apply to this
)
set(BUILD_EXAMPLES OFF CACHE INTERNAL "") # We don't want to build raylib's (or any, since this is universal) examples
FetchContent_MakeAvailable(raylib)