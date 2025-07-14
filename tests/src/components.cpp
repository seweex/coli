#include <coli/game-engine.h>
#include <gtest/gtest.h>

using namespace Coli;

static Game::Scene scene;
static Game::ObjectHandle object = scene.create();

struct EmptyComponent { int dummy = 0; };

TEST(Component, Creation)
{
    ASSERT_TRUE(!object.expired());
    object.emplace<EmptyComponent>();

    EXPECT_TRUE(object.contains<EmptyComponent>());
}

TEST(Component, Getting)
{
    ASSERT_TRUE(!object.expired());
    auto const component = object.try_get<EmptyComponent>();

    EXPECT_TRUE(component == std::addressof(object.get<EmptyComponent>()));
}

TEST(Component, Destroying)
{
    ASSERT_TRUE(!object.expired());
    object.destroy<EmptyComponent>();

    EXPECT_FALSE(object.contains<EmptyComponent>());
}