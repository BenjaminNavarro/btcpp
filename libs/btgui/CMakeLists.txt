add_library(btgui)
add_library(btgui::btgui ALIAS btgui)

target_sources(
    btgui
    PUBLIC
    FILE_SET CXX_MODULES
    FILES
        btgui.cppm
        graph_layout.cppm
        graph_gui.cppm
)

set_target_properties(
    btgui
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_MODULE_STD YES
)

target_compile_features(
    btgui
    PRIVATE   cxx_std_23
    INTERFACE cxx_std_20)

target_link_libraries(
    btgui
    PUBLIC
        btcpp::btcpp
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)