btcpp_add_library(btgui)

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
        AUTOMOC YES
)

target_link_libraries(
    btgui
    PUBLIC
        btcpp::btcpp
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)