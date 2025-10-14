include(FetchContent)

FetchContent_Declare(
        rlimgui
        GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
        GIT_TAG 4d8a61842903978bc42adf3347cd34f4e6524efc
        GIT_SHALLOW 1
        SYSTEM
)
FetchContent_MakeAvailable(rlimgui)

add_library(rlimgui)

target_include_directories(rlimgui SYSTEM
        PUBLIC
        ${rlimgui_SOURCE_DIR})

target_sources(
        rlimgui
        PRIVATE
        ${rlimgui_SOURCE_DIR}/rlImGui.cpp
        ${rlimgui_SOURCE_DIR}/rlImGui.h
        ${rlimgui_SOURCE_DIR}/rlImGuiColors.h
)

target_link_libraries(rlimgui PRIVATE imgui raylib)