include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.11.0
        SYSTEM
)
FetchContent_MakeAvailable(Catch2)

set(TESTS_NAME unit_tests)
add_executable(${TESTS_NAME}
        textures_test.cpp
        player_test.cpp
        audio_test.cpp
        button_test.cpp
        game_rules_test.cpp)

set_target_properties(${TESTS_NAME} PROPERTIES
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        COMPILE_WARNING_AS_ERROR ON) # Treat warning as errors! Same as "-Werror"
target_compile_features(${TESTS_NAME} PRIVATE cxx_std_23)

# From https://www.learncpp.com/cpp-tutorial/configuring-your-compiler-warning-and-error-levels/
target_compile_options(${TESTS_NAME} PRIVATE -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion)

# Adding the resources' folder.
add_custom_target(copy_resources_tests
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_LIST_DIR}/../src/resources
        $<TARGET_FILE_DIR:${TESTS_NAME}>/../resources #copy to the same place as the test executable
)
add_dependencies(${TESTS_NAME} copy_resources_tests)

target_link_libraries(${TESTS_NAME} PRIVATE ${PROJECT_NAME} Catch2::Catch2WithMain)

# we use a catch_discover_tests() command defined in the module provided by Catch2
#to automatically detect all test cases from unit_tests and register them with CTest
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(unit_tests)