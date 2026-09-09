add_library(btnet)
add_library(btnet::btnet ALIAS btnet)

target_sources(
    btnet
    PUBLIC
    FILE_SET CXX_MODULES
    FILES
        btnet.cppm
)

set_target_properties(
    btnet
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_MODULE_STD YES
)

target_compile_features(
    btnet
    PRIVATE   cxx_std_23
    INTERFACE cxx_std_20)

target_link_libraries(
    btnet
    PUBLIC
        btcpp::btcpp
    PRIVATE
        cppzmq
)

add_sanitizers(btnet)