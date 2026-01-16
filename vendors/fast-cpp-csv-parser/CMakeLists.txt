include(FetchContent)

FetchContent_Declare(
        fast-cpp-csv-parser
        GIT_REPOSITORY https://github.com/ben-strasser/fast-cpp-csv-parser.git
        GIT_TAG 574a9fe4d323ba63416877a4a5fe59088d37aa34
        SYSTEM
)

FetchContent_MakeAvailable(fast-cpp-csv-parser)


add_library(fast-cpp-csv-parser)

set_target_properties(fast-cpp-csv-parser PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(fast-cpp-csv-parser
        SYSTEM
        PUBLIC
        ${fast-cpp-csv-parser_SOURCE_DIR})

target_sources(
        fast-cpp-csv-parser
        PRIVATE
        ${fast-cpp-csv-parser_SOURCE_DIR}/csv.h
)
