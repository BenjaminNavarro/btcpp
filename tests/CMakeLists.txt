include(CTest)

add_library(test_utils INTERFACE)

target_sources(
    test_utils
    INTERFACE
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
    INTERFACE
        btcpp::btcpp
)

target_compile_features(
    test_utils
    INTERFACE cxx_std_20
)

function(btcpp_add_test name)
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
endfunction(btcpp_add_test)

btcpp_add_test(conditions)
btcpp_add_test(actions)
btcpp_add_test(control_nodes)
btcpp_add_test(decorators)
btcpp_add_test(node)