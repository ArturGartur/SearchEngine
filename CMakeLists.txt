cmake_minimum_required(VERSION 3.20)
project(search_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include(FetchContent)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(search_engine
        main.cpp
        ConverterJSON.cpp
        InvertedIndex.cpp
        SearchServer.cpp
)

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

enable_testing()

add_executable(search_engine_tests
        test_inverted_index.cpp
        test_search_server.cpp
)

target_link_libraries(search_engine_tests PRIVATE gtest_main nlohmann_json::nlohmann_json)

include(GoogleTest)
gtest_discover_tests(search_engine_tests)