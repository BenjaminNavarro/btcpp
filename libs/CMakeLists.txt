function(btcpp_add_library name)
    add_library(${name})
    add_library(${name}::${name} ALIAS ${name})

    target_compile_features(
        ${name}
        PRIVATE   cxx_std_23
        INTERFACE cxx_std_20)

    add_sanitizers(${name})

    install(
        TARGETS ${name}
        EXPORT  btcppTargets
        FILE_SET HEADERS
    )
endfunction(btcpp_add_library)


add_subdirectory(btcpp)
add_subdirectory(btgui)
add_subdirectory(btnet)