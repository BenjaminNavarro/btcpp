add_executable(btviewer)

target_sources(
    btviewer
    PRIVATE
        main.cpp
)

set_target_properties(
    btviewer
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
)

target_link_libraries(
    btviewer
    PRIVATE
        btcpp::btcpp
        btgui::btgui
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)