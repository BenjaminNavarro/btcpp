include(CMakePackageConfigHelpers)

install(
  EXPORT btcppTargets
  FILE btcppTargets.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/btcpp
  NAMESPACE btcpp::
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/btcppConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/btcppConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/btcpp"
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/btcppConfigVersion.cmake
    VERSION ${CMAKE_PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/btcppConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/btcppConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/btcpp"
)