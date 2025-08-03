#include <coli/game-engine.h>
#include <gtest/gtest.h>

#include <memory>
#include <thread>

using namespace Coli;

class ContextTest :
    public ::testing::Test
{
protected:
    void SetUp() override
    {
        try {
            settings = { "Context Testing", 640, 480 };
        }
        catch (std::bad_alloc const&) {
            GTEST_SKIP() << "A 'std::bad_alloc' was thrown" << std::endl;
        }
    }

    Graphics::Window::Settings settings { "Context Testing", 640, 480 };
};

#ifdef CREATE_CONTEXT
#undef CREATE_CONTEXT
#endif

#define CREATE_CONTEXT \
std::shared_ptr<Graphics::Context> context; \
try { \
    context = std::make_shared<Graphics::Context>(); \
} \
catch (std::exception const& e) { \
    GTEST_SKIP() << "An exception was thrown: " << e.what() << "." << std::endl; \
}

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

TEST_F(ContextTest, CreateSuccess)
{
    EXPECT_NO_THROW({
        Graphics::Context context;
        EXPECT_TRUE(context.is_owning());
    });
}

TEST_F(ContextTest, CreateAnotherContext)
{
    CREATE_CONTEXT;

    EXPECT_THROW(
        Graphics::Context another,
        std::logic_error);
}

/* Move */

TEST_F(ContextTest, MoveSuccess)
{
    CREATE_CONTEXT;

    EXPECT_NO_THROW({
        Graphics::Context other(std::move(*context));
        EXPECT_TRUE(other.is_owning());
        EXPECT_FALSE(context->is_owning());
    });
}

TEST_F(ContextTest, MoveAnotherThread)
{
    CREATE_CONTEXT;

    auto moveContext = [&] {
        EXPECT_THROW(
            Graphics::Context other (std::move(*context)),
            std::logic_error);
    };

    std::thread thread (moveContext);
    thread.join();
}

/* Window Integration */

TEST_F(ContextTest, HasNoWindow)
{
    CREATE_CONTEXT;
    EXPECT_FALSE(context->has_window());
}

TEST_F(ContextTest, HasWindow)
{
    CREATE_CONTEXT;

    EXPECT_FALSE(context->has_window());

    CREATE_WINDOW;

    EXPECT_TRUE(context->has_window());
}

/* Validation */

TEST_F(ContextTest, ValidationSuccess)
{
    CREATE_CONTEXT;
    CREATE_WINDOW;

    EXPECT_NO_THROW(context->verify_context());
}

TEST_F(ContextTest, ValidationNoWindow)
{
    CREATE_CONTEXT;
    EXPECT_THROW(context->verify_context(), std::logic_error);
}

TEST_F(ContextTest, ValidationNotOwning)
{
    CREATE_CONTEXT;
    CREATE_WINDOW;

    ASSERT_NO_THROW(context->verify_context());

    Graphics::Context other (std::move(*context));

    EXPECT_THROW(
        context->verify_context(),
        std::logic_error);
}

TEST_F(ContextTest, ValidationThreadSuccess)
{
    CREATE_CONTEXT;
    EXPECT_NO_THROW(context->verify_thread());
}

TEST_F(ContextTest, ValidationAnotherThread)
{
    CREATE_CONTEXT;

    auto verifyThread = [&] {
        EXPECT_THROW(
            context->verify_thread(),
            std::logic_error);
    };

    std::thread thread (verifyThread);
    thread.join();
}