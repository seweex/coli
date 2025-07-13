#include <coli/game-engine.h>
#include <gtest/gtest.h>

using namespace Coli;

inline Game::Scene scene;
inline Game::ObjectHandle object = nullptr;

TEST(Object, Creation)
{
    object = scene.create();
    EXPECT_TRUE(!object.expired());
}

TEST(Object, Destroying)
{
    ASSERT_TRUE(!object.expired());
    scene.destroy(object);

    EXPECT_TRUE(object.expired());
}