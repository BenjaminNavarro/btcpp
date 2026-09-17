add_library(btnet)
add_library(btnet::btnet ALIAS btnet)

target_sources(
    btnet
    PRIVATE
        btnet.cpp
    PUBLIC
    FILE_SET HEADERS
    FILES
        btnet/btnet.hpp
)

set_target_properties(
    btnet
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
)

target_compile_features(
    btnet
    PRIVATE   cxx_std_23
    INTERFACE cxx_std_20)

target_link_libraries(
    btnet
    PUBLIC
        btcpp::btcpp
        cppzmq
)

add_sanitizers(btnet)