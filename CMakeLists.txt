cmake_minimum_required(VERSION 3.28)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/deps/sanitizers-cmake/cmake" ${CMAKE_MODULE_PATH})

project(
    btcpp
    LANGUAGES CXX
    VERSION 0.1.0
)

# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

find_package(pugixml 1.16 REQUIRED)
find_package(Qt6 6.10 REQUIRED COMPONENTS Core Gui Widgets)
find_package(cppzmq REQUIRED)
find_package(Sanitizers)

add_subdirectory(libs)
add_subdirectory(apps)

if(BUILD_TESTING)
    find_package(Catch2 3.15 REQUIRED)

    # use include() to define tests at the top level and be able to run them
    # simply with ctest --preset conan-xxx

    include(tests/CMakeLists.txt)
endif()

# install(
#     TARGETS
#         btcpp
#         btgui
#         btnet
#         btviewer
#     EXPORT          btcppTargets
#     ARCHIVE         DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     LIBRARY         DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME         DESTINATION ${CMAKE_INSTALL_BINDIR}
#     PUBLIC_HEADER   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/btcpp
#     FILE_SET        CXX_MODULES DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/btcpp/modules
# )
# export(
#     EXPORT btcppTargets
#     FILE "${CMAKE_CURRENT_BINARY_DIR}/btcppConfig.cmake"
#     NAMESPACE btcpp::
# )
# include(cmake/generate_install_config.cmake)
