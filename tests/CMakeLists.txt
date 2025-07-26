include(FetchContent)
include(CTest)

set(INSTALL_GTEST OFF)
set(BUILD_GMOCK OFF)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(googletest)

add_executable(coli-test-graphics-context   src/graphics/context.cpp)
add_executable(coli-test-graphics-window    src/graphics/window.cpp)
add_executable(coli-test-graphics-buffer    src/graphics/buffer.cpp)

add_executable(coli-test-game-object    src/game/object.cpp)
add_executable(coli-test-game-scene     src/game/scene.cpp)

set (COLI_ALL_TEST_NAMES
        coli-test-graphics-context
        coli-test-graphics-window
        coli-test-graphics-buffer

        coli-test-game-object
        coli-test-game-scene
)

foreach (target IN LISTS COLI_ALL_TEST_NAMES)
    target_link_libraries(${target}
        PRIVATE
            coli::game-engine
            GTest::gtest_main
    )
endforeach ()

add_test(NAME coli-graphics-context COMMAND coli-test-graphics-context)
add_test(NAME coli-graphics-window COMMAND coli-test-graphics-window)
add_test(NAME coli-graphics-buffer COMMAND coli-test-graphics-buffer)

add_test(NAME coli-game-object COMMAND coli-test-game-object)
add_test(NAME coli-game-scene COMMAND coli-test-game-scene)

add_test(NAME coli-graphics
    COMMAND
        ${CMAKE_CTEST_COMMAND}
        --output-on-failure
        -R "^coli-graphics-"
)

add_test(NAME coli-game
    COMMAND
        ${CMAKE_CTEST_COMMAND}
        --output-on-failure
        -R "^coli-game-"
)