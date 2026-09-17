add_executable(example)

target_sources(
    example
    PRIVATE
        main.cpp
)

set_target_properties(
    example
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
)

target_link_libraries(
    example
    PRIVATE
        btcpp::btcpp
        btnet::btnet
)

add_sanitizers(example)