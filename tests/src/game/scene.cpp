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

/* Validity */

TEST_F(SceneTest, CheckValidity)
{
    CREATE_SCENE
    EXPECT_TRUE(scene->is_valid());
}

TEST_F(SceneTest, Reset)
{
    CREATE_SCENE
    ASSERT_TRUE(scene->is_valid());

    scene->reset();
    EXPECT_FALSE(scene->is_valid());
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
