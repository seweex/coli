#include <coli/game-engine.h>
#include <gtest/gtest.h>

#include <memory>
#include <thread>

using namespace Coli;

class WindowTest :
    public ::testing::Test
{
protected:
    void SetUp() override
    {
        try {
            settings = { "Window Testing", 640, 480 };
            context = std::make_shared<Graphics::Context>();
        }
        catch (std::exception const& e) {
            GTEST_SKIP() << "An exception was thrown: " << e.what() << "." << std::endl;
        }
    }

    void TearDown() override {
        context.reset();
    }

    std::shared_ptr<Graphics::Context> context;
    Graphics::Window::Settings settings { "Window Testing", 640, 480 };
};

#ifdef CREATE_WINDOW
#undef CREATE_WINDOW
#endif

#define CREATE_WINDOW \
std::unique_ptr<Graphics::Window> window; \
try { \
    window = std::make_unique<Graphics::Window>(context, settings); \
} \
catch (std::exception const& e) { \
    GTEST_SKIP() << "An exception was thrown: " << e.what() << "." << std::endl; \
}

/* Create */

TEST_F(WindowTest, CreateSuccess)
{
    EXPECT_NO_THROW({
        Graphics::Window window (context, settings);
        EXPECT_TRUE(window.is_valid());
    });
}

TEST_F(WindowTest, CreateEmptyTitle)
{
    settings.title = "";

    EXPECT_THROW(
        Graphics::Window window (context, settings),
        std::invalid_argument);
}

TEST_F(WindowTest, CreateInvalidContext)
{
    EXPECT_THROW(
        Graphics::Window window ({}, settings),
        std::invalid_argument);
}

TEST_F(WindowTest, CreateAnotherWindow)
{
    CREATE_WINDOW;

    EXPECT_THROW(
        Graphics::Window another (context, settings),
        std::logic_error);
}

TEST_F(WindowTest, CreateInAnotherThread)
{
    auto createWindow = [&] {
        EXPECT_THROW(
            Graphics::Window window (context, settings),
            std::logic_error);
    };

    std::thread thread (createWindow);
    thread.join();
}

/* Move */

TEST_F(WindowTest, MoveSuccess)
{
    CREATE_WINDOW;

    EXPECT_NO_THROW({
        Graphics::Window other (std::move(*window));

        EXPECT_TRUE(other.is_valid());
        EXPECT_FALSE(window->is_valid());
        EXPECT_TRUE(context->has_window());
    });
}

TEST_F(WindowTest, MoveAnotherThread)
{
    CREATE_WINDOW;

    auto moveWindow = [&] {
        EXPECT_THROW(
            Graphics::Window other (std::move(*window)),
            std::logic_error);
    };

    std::thread thread (moveWindow);
    thread.join();
}

/* Get Size */

TEST_F(WindowTest, GetSize)
{
    CREATE_WINDOW;

    auto const [width, height] = window->size();

    EXPECT_EQ(width, settings.width);
    EXPECT_EQ(height, settings.height);
}

TEST_F(WindowTest, GetSizeOnNull)
{
    CREATE_WINDOW;

    window->clear();

    EXPECT_THROW(
        (void)window->size(),
        std::invalid_argument);
}

/* Get Closing */

TEST_F(WindowTest, GetClosing)
{
    CREATE_WINDOW;
    EXPECT_FALSE(window->should_close());
}

TEST_F(WindowTest, GetCloseOnNull)
{
    CREATE_WINDOW;
    window->clear();

    EXPECT_THROW(
        (void)window->should_close(),
        std::invalid_argument);
}