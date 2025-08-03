#include <coli/game-engine.h>
#include <gtest/gtest.h>

#include <memory>
#include <thread>

using namespace Coli;

class ShaderTest :
    public ::testing::Test
{
protected:
    void SetUp() override
    {
        try {
            settings = { "Context Testing", 640, 480 };
            context = std::make_shared<Graphics::Context>();
            window = std::make_unique<Graphics::Window>(context, settings);
        }
        catch (std::bad_alloc const&) {
            GTEST_SKIP() << "A 'std::bad_alloc' was thrown" << std::endl;
        }
    }

    void TearDown() override {
        context.reset();
        window.reset();
    }

    std::shared_ptr<Graphics::Context> context;
    std::unique_ptr<Graphics::Window> window;
    Graphics::Window::Settings settings { "Context Testing", 640, 480 };

    static constexpr size_t size = 40;
    static constexpr size_t offset = 20;

    static constexpr std::string_view source = {
        "#version 450"                             "\n"

        "void main()"                                   "\n"
        "{"                                             "\n"
        "   gl_Position = vec4(0, 0, 0, 1);"            "\n"
        "}"                                             "\n"
    };
};

#ifdef CREATE_SHADER
#undef CREATE_SHADER
#endif

#define CREATE_SHADER \
std::unique_ptr<Graphics::VertexShader> shader; \
try { \
    shader = std::make_unique<Graphics::VertexShader>(context, source); \
} \
catch (std::exception const& e) { \
    GTEST_SKIP() << "An exception was thrown: " << e.what() << "." << std::endl; \
}

/* Create */

TEST_F(ShaderTest, CreateSuccess)
{
    EXPECT_NO_THROW({
        Graphics::VertexShader shader (context, source);
        EXPECT_TRUE(shader.is_valid());
    });
}

TEST_F(ShaderTest, CreateInvalidContext)
{
    EXPECT_THROW(
        Graphics::VertexShader shader ({}, source),
        std::invalid_argument);
}

TEST_F(ShaderTest, CreateAnotherThread)
{
    auto createShader = [&] {
        EXPECT_THROW(
            Graphics::VertexShader shader (context, source),
            std::logic_error);
    };

    std::thread thread(createShader);
    thread.join();
}

TEST_F(ShaderTest, CreateInvalidSource)
{
    constexpr std::string_view invalid = {};

    EXPECT_THROW(
        Graphics::VertexShader shader (context, invalid),
        std::invalid_argument);
}

/* Move */

TEST_F(ShaderTest, MoveSuccess)
{
    CREATE_SHADER;

    EXPECT_NO_THROW({
        Graphics::VertexShader other (std::move(*shader));

        EXPECT_TRUE(other.is_valid());
        EXPECT_FALSE(shader->is_valid());
    });
}

TEST_F(ShaderTest, MoveAnotherThread)
{
    CREATE_SHADER;

    auto moveShader = [&] {
        EXPECT_THROW(
            Graphics::VertexShader other (std::move(*shader)),
            std::logic_error);
    };

    std::thread thread (moveShader);
    thread.join();
}