cmake_minimum_required(VERSION 3.28)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")

project(
    btcpp
    LANGUAGES CXX
    VERSION 0.1.0
)


add_subdirectory(libs)
add_subdirectory(tests)
add_subdirectory(apps)
