cmake_minimum_required(VERSION 3.14...4.0.3)
project(coli VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

option(COLI_BUILD_DYNAMIC "Build the library dynamic (shared)" OFF)
option(COLI_BUILD_TESTS "Enable tests" OFF)
option(COLI_BUILD_DOCS "Build documentation with library" OFF)

set (COLI_SOURCES
        src/game/components/layer.cpp

        src/game/object.cpp
        src/game/scene.cpp

        src/generic/system.cpp
        src/generic/engine.cpp
)

if (COLI_BUILD_DYNAMIC)
    add_library(coli-game-engine SHARED ${COLI_SOURCES})
    target_compile_definitions(coli-game-engine PRIVATE COLI_DYNAMIC=1)
else ()
    add_library(coli-game-engine STATIC ${COLI_SOURCES})
endif ()

if (${PROJECT_NAME} STREQUAL "coli")
    target_compile_definitions(coli-game-engine PRIVATE COLI_BUILD=1)
else ()
    target_compile_definitions(coli-game-engine PRIVATE COLI_BUILD=0)
endif ()


add_library(coli::game-engine ALIAS coli-game-engine)

if (COLI_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()

set_target_properties(coli-game-engine PROPERTIES
        VERSION ${PROJECT_VERSION}
        PUBLIC_HEADER "${COLI_HEADERS}"
)

find_package(EnTT CONFIG REQUIRED)

target_link_libraries(coli-game-engine PUBLIC EnTT::EnTT)
target_include_directories(coli-game-engine PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS coli-game-engine
        EXPORT coli-game-engine-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/coli
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/
        FILES_MATCHING
        PATTERN "*.h"
)

install(EXPORT coli-game-engine-targets
        FILE coli-game-engine-targets.cmake
        NAMESPACE coli::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/coli-game-engine
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/coli-game-engine-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/coli-game-engine-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/coli-game-engine
)

configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/coli-game-engine-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/coli-game-engine
)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/coli-game-engine-config-version.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

if (COLI_BUILD_DOCS)
    add_subdirectory(docs)
    install(CODE "
        execute_process(
            COMMAND \"${CMAKE_COMMAND}\" --build \"${CMAKE_BINARY_DIR}\" --target docs
            WORKING_DIRECTORY \"${CMAKE_BINARY_DIR}\"
        )
    " COMPONENT docs)
endif ()