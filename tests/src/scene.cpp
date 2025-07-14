#include <coli/game-engine.h>
#include <gtest/gtest.h>

using namespace Coli;

static Game::Scene scene;

TEST(Scene, Ordering)
{
    constexpr int objects = 10;

    for (int i = 0; i < objects; ++i)
        (void)scene.create();

    auto const& ordered = scene.ordered();

    EXPECT_TRUE(
        ordered.size() == objects &&
        std::is_sorted(ordered.begin(), ordered.end(),
        [](Game::ObjectHandle const& a, Game::ObjectHandle const& b)
        {
            constexpr auto no_layer =  std::numeric_limits<long long>::max();

            auto componentA = a.try_get<Game::Components::Layer>();
            auto componentB = b.try_get<Game::Components::Layer>();

            auto const layerA = componentA ? componentA->layer() : no_layer;
            auto const layerB = componentB ? componentB->layer() : no_layer;

            return layerA < layerB;
        })
    );
}