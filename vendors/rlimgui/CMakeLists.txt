include(FetchContent)

FetchContent_Declare(
        rlimgui
        GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
        GIT_TAG dc7f97679a024eee8f5f009e77cc311748200415
        # GIT_SHALLOW 1 #If GIT_SHALLOW is enabled then GIT_TAG works only with branch names and tags. A commit hash is not allowed.
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