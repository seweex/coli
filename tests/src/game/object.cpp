#include <coli/game-engine.h>
#include <gtest/gtest.h>

#include <memory>

using namespace Coli;

class ObjectTest :
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

#ifdef CREATE_OBJECT
#undef CREATE_OBJECT
#endif

#define CREATE_OBJECT \
Game::ObjectHandle object { nullptr }; \
try { \
    object = scene->create(); \
} \
catch (std::exception const& e) { \
    GTEST_SKIP() << "An exception was thrown: " << e.what() << "." << std::endl; \
}

/* Create */

TEST_F(ObjectTest, CreateSuccess)
{
    EXPECT_NO_THROW({
        auto object = scene->create();
        EXPECT_FALSE(object.expired());
    });
}

TEST_F(ObjectTest, CreateNull) {
    EXPECT_TRUE(Game::ObjectHandle(nullptr).expired());
}

/* Move */

TEST_F(ObjectTest, MoveSuccess)
{
    CREATE_OBJECT;

    EXPECT_FALSE(Game::ObjectHandle(std::move(object)).expired());
    EXPECT_TRUE(object.expired());
}

/* Copy */

TEST_F(ObjectTest, CopySuccess)
{
    CREATE_OBJECT;

    EXPECT_FALSE(Game::ObjectHandle(object).expired());
    EXPECT_FALSE(object.expired());
}

/* Components */

TEST_F(ObjectTest, AddComponent)
{
    CREATE_OBJECT;

    object.emplace<Game::Components::Layer>(1);
    EXPECT_TRUE(object.contains<Game::Components::Layer>());
}

TEST_F(ObjectTest, RemoveComponent)
{
    CREATE_OBJECT;

    object.emplace<Game::Components::Layer>(1);
    ASSERT_TRUE(object.contains<Game::Components::Layer>());

    object.destroy<Game::Components::Layer>();
    EXPECT_FALSE(object.contains<Game::Components::Layer>());
}

TEST_F(ObjectTest, GetComponent)
{
    CREATE_OBJECT;

    object.emplace<Game::Components::Layer>(1);
    ASSERT_TRUE(object.contains<Game::Components::Layer>());

    auto const ptr = object.try_get<Game::Components::Layer>();
    auto& ref = object.get<Game::Components::Layer>();

    EXPECT_TRUE(ptr && ptr == std::addressof(ref));
}

/* Destroy */

TEST_F(ObjectTest, Destroy)
{
    CREATE_OBJECT;
    scene->destroy(object);

    EXPECT_TRUE(object.expired());
}