btcpp_add_library(btcpp)

target_sources(
    btcpp
    PRIVATE
        decorators.cpp
        fallback.cpp
        fmt.cpp
        generic_action.cpp
        generic_condition.cpp
        io.cpp
        node.cpp
        parallel.cpp
        sequence.cpp
    PUBLIC
    FILE_SET HEADERS
    FILES
        btcpp/btcpp.hpp
        btcpp/control_node.hpp
        btcpp/decorators.hpp
        btcpp/execution_node.hpp
        btcpp/fallback.hpp
        btcpp/fmt.hpp
        btcpp/generic_action.hpp
        btcpp/generic_condition.hpp
        btcpp/io.hpp
        btcpp/node_data.hpp
        btcpp/node.hpp
        btcpp/parallel.hpp
        btcpp/sequence.hpp
)

target_link_libraries(
    btcpp
    PRIVATE pugixml::pugixml
)