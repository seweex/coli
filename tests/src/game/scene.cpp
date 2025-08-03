#include <coli/game-engine.h>
#include <gtest/gtest.h>

#include <memory>

using namespace Coli;

class SceneTest :
    public ::testing::Test
{
protected:
    void SetUp() override
    {
        try {
            scene = std::make_unique<Game::Scene>();
        }
        catch (std::bad_alloc const&) {
            GTEST_SKIP() << "A 'std::bad_alloc' was thrown" << std::endl;
        }
    }

    void TearDown() override {
        scene.reset();
    }

    std::unique_ptr<Game::Scene> scene;
};

#ifdef CREATE_SCENE
#undef CREATE_SCENE
#endif

#define CREATE_SCENE \
std::unique_ptr<Game::Scene> scene; \
try { \
    scene = std::make_unique<Game::Scene>(); \
} \
catch (std::exception const& e) { \
    GTEST_SKIP() << "An exception was thrown: " << e.what() << "." << std::endl; \
}

/* Create */

TEST_F(SceneTest, CreateSuccess) {
    EXPECT_NO_THROW(Game::Scene scene);
}

/* Objects */

TEST_F(SceneTest, AddObject)
{
    CREATE_SCENE;

    auto object = scene->create();
    EXPECT_FALSE(object.expired());
}

TEST_F(SceneTest, RemoveObject)
{
    CREATE_SCENE;

    Game::ObjectHandle object = nullptr;

    ASSERT_NO_THROW(object = scene->create());
    EXPECT_FALSE(object.expired());
}

TEST_F(SceneTest, Order)
{
    CREATE_SCENE;

    for (int i = 0; i < 10; ++i)
    {
        auto object = scene->create();

        if (i % 2)
            object.emplace<Game::Components::Layer>(i * 50 + 2);
    }

    auto const ordered = scene->ordered();
    auto comparator = [] (Game::ObjectHandle const& lhs, Game::ObjectHandle const& rhs)
    {
        constexpr size_t no_layer = std::numeric_limits<long long>::max();

        auto const left = lhs.try_get<Game::Components::Layer>();
        auto const right = rhs.try_get<Game::Components::Layer>();

        auto const leftValue = left ? left->layer() : no_layer;
        auto const rightValue = right ? right->layer() : no_layer;

        return leftValue < rightValue;
    };

    EXPECT_TRUE(std::is_sorted(ordered.begin(), ordered.end(), comparator));
}

TEST_F(SceneTest, Filter)
{
    CREATE_SCENE;

    size_t layered = 0;

    for (int i = 0; i < 10; ++i)
    {
        auto object = scene->create();

        if (i % 4) {
            object.emplace<Game::Components::Layer>(i * 50 + 2);
            ++layered;
        }
    }

    auto const filtered = scene->filtered<Game::Components::Layer>();
    auto predicate = [] (Game::ObjectHandle const& lhs) {
        return lhs.contains<Game::Components::Layer>();
    };

    EXPECT_TRUE(
        layered == filtered.size() &&
        std::all_of(filtered.begin(), filtered.end(), predicate));
}
