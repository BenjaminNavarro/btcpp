add_library(btcpp)
add_library(btcpp::btcpp ALIAS btcpp)

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

set_target_properties(
    btcpp
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
)

target_compile_features(
    btcpp
    INTERFACE cxx_std_20
    PRIVATE cxx_std_23
)

target_link_libraries(
    btcpp
    PRIVATE pugixml::pugixml
)

add_sanitizers(btcpp)

install(TARGETS btcpp
        ARCHIVE         DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY         DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/btcpp
        CXX_MODULES_BMI DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/btcpp/modules
)