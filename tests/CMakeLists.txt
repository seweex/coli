include(FetchContent)

set(INSTALL_GTEST OFF)
set(BUILD_GMOCK OFF)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(googletest)

set (COLI_TESTS
        src/object.cpp
        src/scene.cpp
        src/components.cpp
        src/version.cpp
        src/graphics.cpp
)

add_executable(coli-tests ${COLI_TESTS})

target_link_libraries(coli-tests
        coli::game-engine
        GTest::gtest_main
)

add_test(NAME coli-tests COMMAND coli-tests)