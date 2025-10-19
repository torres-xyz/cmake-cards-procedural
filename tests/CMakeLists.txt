set(TESTS unit_tests)
add_executable(${TESTS}
        unit_tests.cpp
        textures_test.cpp)
set_target_properties(${TESTS} PROPERTIES
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        COMPILE_WARNING_AS_ERROR ON) # Treat warning as errors! Same as "-Werror"
target_compile_features(${TESTS} PRIVATE cxx_std_23)

# From https://www.learncpp.com/cpp-tutorial/configuring-your-compiler-warning-and-error-levels/
target_compile_options(${TESTS} PRIVATE -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion)

target_link_libraries(unit_tests PRIVATE ${PROJECT_NAME})

target_include_directories(unit_tests PRIVATE ../src)
add_test(NAME GetGameTextureFromCardType_ReturnsTexture COMMAND unit_tests GetGameTextureFromCardType_ReturnsTexture)