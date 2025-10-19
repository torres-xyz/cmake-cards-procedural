add_executable(unit_tests
        unit_tests.cpp
        textures_test.cpp)

target_link_libraries(unit_tests PRIVATE ${PROJECT_NAME})
target_compile_features(unit_tests PRIVATE cxx_std_23)

target_include_directories(unit_tests PRIVATE ../src)
add_test(NAME GetGameTextureFromCardType_ReturnsTexture COMMAND unit_tests GetGameTextureFromCardType_ReturnsTexture)