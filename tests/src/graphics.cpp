#include <coli/game-engine.h>
#include <gtest/gtest.h>

using namespace Coli;

static std::shared_ptr<Graphics::Context> context;
static std::unique_ptr<Graphics::Window> window;

TEST(Graphics, Context) {
    EXPECT_NO_THROW(context = std::make_shared<Graphics::Context>());
    EXPECT_TRUE(context);
}

TEST(Graphics, WindowCreation)
{
    ASSERT_TRUE(context && context->is_owning());

    Graphics::Window::Settings const settings { "Coli Graphics Test", 640, 480 };
    window = std::make_unique<Graphics::Window>(context, settings);

    EXPECT_TRUE(window->is_valid());
}

TEST(Graphics, ContextValidationWithWindow)
{
    ASSERT_TRUE(window && window->is_valid());

    EXPECT_NO_THROW(context->verify_thread());
    EXPECT_NO_THROW(context->verify_context());
}

TEST(Graphics, WindowDestroying)
{
    ASSERT_TRUE(window);

    window->clear();
    EXPECT_FALSE(window->is_valid());

    window.reset();
}

TEST(Graphics, ContextValidationWithNoWindow)
{
    ASSERT_FALSE(window);
    EXPECT_THROW(context->verify_context(), std::logic_error);
}
