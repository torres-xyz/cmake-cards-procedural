add_executable(unit_tests
        unit_tests.cpp
        textures_test.cpp
        ../src/textures.cpp)

target_include_directories(unit_tests PRIVATE ../src)
add_test(NAME GetGameTextureFromCardType_ReturnsTexture COMMAND unit_tests GetGameTextureFromCardType_ReturnsTexture)