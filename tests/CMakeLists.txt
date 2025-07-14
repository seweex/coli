include(FetchContent)

find_package(GTest REQUIRED)

set (COLI_TESTS
        src/object.cpp
        src/scene.cpp
        src/components.cpp
)

add_executable(coli-tests ${COLI_TESTS})

target_link_libraries(coli-tests
        coli::game-engine
        GTest::gtest_main
)

add_test(NAME coli-tests COMMAND coli-tests)