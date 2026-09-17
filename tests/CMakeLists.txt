include(CTest)

add_library(test_utils)

target_sources(
    test_utils
    PUBLIC
    FILE_SET HEADERS
    FILES
        tests/test_utils.hpp
)

set_target_properties(
    test_utils
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
)

target_link_libraries(
    test_utils
    PUBLIC
        btcpp::btcpp
)

target_compile_features(
    test_utils
    INTERFACE cxx_std_20
)

function(add_btcpp_test name)
    if (BUILD_TESTING)
        add_executable(${name}_test)

        target_sources(
            ${name}_test
            PRIVATE
                tests/${name}.cpp
        )

        set_target_properties(
            ${name}_test
            PROPERTIES
                CXX_STANDARD 20
                CXX_STANDARD_REQUIRED YES
        )

        target_link_libraries(
            ${name}_test
            PRIVATE
                btcpp::btcpp
                Catch2::Catch2WithMain
                test_utils
        )

        add_test(
            NAME ${name}_test
            COMMAND ${name}_test
        )
    endif()
endfunction(add_btcpp_test)

add_btcpp_test(conditions)
add_btcpp_test(actions)
add_btcpp_test(control_nodes)
add_btcpp_test(decorators)
add_btcpp_test(node)