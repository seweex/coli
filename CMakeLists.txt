cmake_minimum_required(VERSION 3.14...4.0.3)

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
else()
    if (CMAKE_CXX_STANDARD LESS 20)
        message(FATAL_ERROR "The 'coli' library requires C++20 or higher")
    endif()
endif()

set(COLI_VERSION_MAJOR 0)
set(COLI_VERSION_MINOR 2)
set(COLI_VERSION_PATCH 7)

project(coli VERSION ${COLI_VERSION_MAJOR}.${COLI_VERSION_MINOR}.${COLI_VERSION_PATCH} LANGUAGES CXX C)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

option(COLI_BUILD_DYNAMIC "Build the library dynamic (shared)" OFF)
option(COLI_BUILD_TESTS "Enable tests" OFF)
option(COLI_BUILD_DOCS "Build documentation with library" OFF)
option(COLI_FORCE_SINGLE_FLOAT "Forces `float` type instead of `double`" OFF)

set (COLI_SOURCES
        src/utility.cpp

        src/game/components/layer.cpp
        src/game/components/transform.cpp

        src/game/object.cpp
        src/game/scene.cpp

        src/generic/system.cpp
        src/generic/engine.cpp

        src/graphics/context.cpp
        src/graphics/resource.cpp
        src/graphics/window.cpp
        src/graphics/buffer.cpp
        src/graphics/shader.cpp
        src/graphics/program.cpp
        src/graphics/vertex_array.cpp
        src/geometry/vertex.cpp
)

if (COLI_BUILD_DYNAMIC)
    add_library(coli-game-engine SHARED ${COLI_SOURCES})
    target_compile_definitions(coli-game-engine PRIVATE COLI_DYNAMIC=1)
else ()
    add_library(coli-game-engine STATIC ${COLI_SOURCES})
endif ()

if (COLI_FORCE_SINGLE_FLOAT)
    target_compile_definitions(coli-game-engine PRIVATE COLI_FORCE_SINGLE_FLOAT=1)
endif ()

if (${PROJECT_NAME} STREQUAL "coli")
    target_compile_definitions(coli-game-engine PRIVATE COLI_BUILD=1)
else ()
    target_compile_definitions(coli-game-engine PRIVATE COLI_BUILD=0)
endif ()

target_compile_definitions(coli-game-engine PRIVATE COLI_VERSION_MAJOR=${PROJECT_VERSION_MAJOR})
target_compile_definitions(coli-game-engine PRIVATE COLI_VERSION_MINOR=${PROJECT_VERSION_MINOR})
target_compile_definitions(coli-game-engine PRIVATE COLI_VERSION_PATCH=${PROJECT_VERSION_PATCH})

add_library(coli::game-engine ALIAS coli-game-engine)

if (COLI_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()

set_target_properties(coli-game-engine
    PROPERTIES
        VERSION ${COLI_VERSION_MAJOR}.${COLI_VERSION_MINOR}.${COLI_VERSION_PATCH}
        CMAKE_CXX_STANDARD 20
        CMAKE_CXX_STANDARD_REQUIRED ON
)

add_subdirectory(libs/glad)

target_compile_definitions(coli-game-engine PRIVATE COLI_OPENGL_VERSION_MAJOR=${GLAD_OPENGL_VERSION_MAJOR})
target_compile_definitions(coli-game-engine PRIVATE COLI_OPENGL_VERSION_MINOR=${GLAD_OPENGL_VERSION_MINOR})

find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(Bullet CONFIG REQUIRED)

target_link_libraries(coli-game-engine PUBLIC
        coli::glad
        EnTT::EnTT
        glm::glm
        glfw
        ${BULLET_LIBRARIES}
)

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

configure_file(
        version.h.in
        ${CMAKE_SOURCE_DIR}/include/coli/version.h
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
