add_library(btcpp)
add_library(btcpp::btcpp ALIAS btcpp)

target_sources(
    btcpp
    PUBLIC
    FILE_SET CXX_MODULES
    FILES
        btcpp/btcpp.cppm
        btcpp/node.cppm
        btcpp/decorators.cppm

        btcpp/control_node.cppm
        btcpp/sequence.cppm
        btcpp/fallback.cppm
        btcpp/parallel.cppm

        btcpp/execution_node.cppm
        btcpp/generic_action.cppm
        btcpp/generic_condition.cppm

        btcpp/fmt.cppm
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