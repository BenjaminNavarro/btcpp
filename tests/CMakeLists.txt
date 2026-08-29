include(CTest)

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
                CXX_STANDARD 23
                CXX_STANDARD_REQUIRED YES
        )

        target_link_libraries(
            ${name}_test
            PRIVATE
                btcpp::btcpp
                Catch2::Catch2WithMain
        )

        add_test(
            NAME ${name}_test
            COMMAND ${name}_test
        )
    endif()
endfunction(add_btcpp_test)

add_btcpp_test(conditions)
add_btcpp_test(actions)