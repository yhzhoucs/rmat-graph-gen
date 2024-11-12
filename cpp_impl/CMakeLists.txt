cmake_minimum_required(VERSION 3.19)

project(rmat_graph_gen
    VERSION 1.0
    DESCRIPTION "RMAT Graph Generator"
    LANGUAGES CXX)

option(BUILD_TESTING "Build the tests" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(generator src/generator.cpp)
target_include_directories(generator PUBLIC include)

find_package(OpenMP)
if (OpenMP_CXX_FOUND)
    target_link_libraries(generator PUBLIC OpenMP::OpenMP_CXX)
endif()

add_executable(rmat_graph_gen src/main.cpp)
target_link_libraries(rmat_graph_gen PRIVATE generator)

include(FetchContent)
FetchContent_Declare(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG v3.2.0
)

FetchContent_MakeAvailable(cxxopts)
target_link_libraries(rmat_graph_gen PRIVATE cxxopts::cxxopts)

if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()