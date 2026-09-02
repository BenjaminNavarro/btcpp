cmake_minimum_required(VERSION 3.28)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")

project(
    btcpp
    LANGUAGES CXX
    VERSION 0.1.0
)

find_package(Catch2 3.15 REQUIRED)
find_package(pugixml 1.16 REQUIRED)

add_subdirectory(libs)
add_subdirectory(apps)

# use include() to define tests at the top level and be able to run them
# simply with ctest --preset conan-xxx
include(tests/CMakeLists.txt)
