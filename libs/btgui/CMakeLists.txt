add_library(btgui)
add_library(btgui::btgui ALIAS btgui)

target_sources(
    btgui
    PRIVATE
        graph_gui.cpp
        graph_layout.cpp
    PUBLIC
    FILE_SET HEADERS
    FILES
        btgui/btgui.hpp
        btgui/graph_gui.hpp
        btgui/graph_layout.hpp
)

set_target_properties(
    btgui
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        AUTOMOC YES
)

target_compile_features(
    btgui
    PRIVATE   cxx_std_23
    INTERFACE cxx_std_20
)

target_link_libraries(
    btgui
    PUBLIC
        btcpp::btcpp
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

add_sanitizers(btgui)