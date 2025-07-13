find_package(Doxygen REQUIRED)

configure_file(
        ${CMAKE_SOURCE_DIR}/docs/doxy.in
        ${CMAKE_BINARY_DIR}/doxy
        @ONLY
)

add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/doxy
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        VERBATIM
)

install(
        DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs/html
        DESTINATION docs/
        COMPONENT docs
)
