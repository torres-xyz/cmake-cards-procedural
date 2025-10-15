add_executable(${PROJECT_NAME}
        main.cpp
        imgui_sidebar.cpp
        imgui_sidebar.hpp
        button.cpp
        button.hpp
        helper_functions.hpp
        audio.cpp
        audio.hpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        COMPILE_WARNING_AS_ERROR ON) # Treat warning as errors! Same as "-Werror"
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# From https://www.learncpp.com/cpp-tutorial/configuring-your-compiler-warning-and-error-levels/
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion)

#Using this to easily exclude Debug behaviours from non-debug builds
add_compile_definitions("DEBUG=$<CONFIG:Debug>")

# Adding the resources' folder.
add_custom_target(copy_resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_LIST_DIR}/resources
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources #copy to the same place as the executable
)
add_dependencies(${PROJECT_NAME} copy_resources)

# Dependencies
include(FetchContent)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        raylib_cpp
        imgui
        rlimgui)
