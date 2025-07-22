set(GLAD_SOURCES src/glad.c)

set(GLAD_OPENGL_VERSION_MAJOR 4 PARENT_SCOPE)
set(GLAD_OPENGL_VERSION_MINOR 5 PARENT_SCOPE)

if (COLI_BUILD_DYNAMIC)
    add_library(coli-glad SHARED ${GLAD_SOURCES})
else ()
    add_library(coli-glad STATIC ${GLAD_SOURCES})
endif ()

add_library(coli::glad ALIAS coli-glad)

target_include_directories(coli-glad PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS coli-glad
        EXPORT coli-glad-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/glad include/KHR
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/
        FILES_MATCHING
        PATTERN "*.h"
)

install(EXPORT coli-glad-targets
        FILE coli-glad-targets.cmake
        NAMESPACE coli::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/coli-glad
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/coli-glad-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/coli-glad-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/coli-glad
)

configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/coli-glad-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/coli-glad
)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/coli-glad-config-version.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)