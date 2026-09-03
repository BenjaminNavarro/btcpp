add_executable(btgui)

target_sources(
    btgui
    PRIVATE
        main.cpp
        graph_layout.cpp
)

set_target_properties(
    btgui
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
)

target_link_libraries(
    btgui
    PRIVATE
        btcpp::btcpp
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)