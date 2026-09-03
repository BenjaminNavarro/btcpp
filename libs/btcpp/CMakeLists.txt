add_library(btcpp)
add_library(btcpp::btcpp ALIAS btcpp)

target_sources(
    btcpp
    PUBLIC
    FILE_SET CXX_MODULES
    FILES
        btcpp.cppm
        node.cppm
        decorators.cppm

        control_node.cppm
        sequence.cppm
        fallback.cppm
        parallel.cppm

        execution_node.cppm
        generic_action.cppm
        generic_condition.cppm

        io.cppm
        demangle.cppm
        fmt.cppm
)

set_target_properties(
    btcpp
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_MODULE_STD YES
)

target_compile_features(
    btcpp
    PRIVATE   cxx_std_23
    INTERFACE cxx_std_20)

target_link_libraries(
    btcpp
    PRIVATE pugixml::pugixml
)