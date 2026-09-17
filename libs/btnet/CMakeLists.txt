btcpp_add_library(btnet)

target_sources(
    btnet
    PRIVATE
        btnet.cpp
    PUBLIC
    FILE_SET HEADERS
    FILES
        btnet/btnet.hpp
)

target_link_libraries(
    btnet
    PUBLIC
        btcpp::btcpp
        cppzmq
)