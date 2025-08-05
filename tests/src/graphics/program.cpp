#include <coli/game-engine.h>
#include <gtest/gtest.h>

#include <memory>
#include <thread>
#include <coli/graphics/program.h>

using namespace Coli;

class ProgramTest :
    public ::testing::Test
{
protected:
    void SetUp() override
    {
        try {
            settings = { "Context Testing", 640, 480 };
            context = std::make_shared<Graphics::Context>();
            window = std::make_unique<Graphics::Window>(context, settings);

            vertex = std::make_unique<Graphics::VertexShader>(context, vertexSource);
            fragment = std::make_unique<Graphics::FragmentShader>(context, fragmentSource);
        }
        catch (std::bad_alloc const&) {
            GTEST_SKIP() << "A 'std::bad_alloc' was thrown" << std::endl;
        }
    }

    void TearDown() override {
        vertex.reset();
        fragment.reset();
        
        window.reset();
        context.reset();
    }

    Graphics::Window::Settings settings { "Program Testing", 640, 480 };
    
    std::shared_ptr<Graphics::Context> context;
    std::unique_ptr<Graphics::Window> window;
    
    std::unique_ptr<Graphics::VertexShader> vertex;
    std::unique_ptr<Graphics::FragmentShader> fragment;

    static constexpr std::string_view vertexSource = {
        "#version 450"                             "\n"

        "void main()"                                   "\n"
        "{"                                             "\n"
        "   gl_Position = vec4(0, 0, 0, 1);"            "\n"
        "}"                                             "\n"
    };

    static constexpr std::string_view fragmentSource = {
        "#version 450"                             "\n"
        
        "out vec4 color;"                               "\n"

        "void main()"                                   "\n"
        "{"                                             "\n"
        "   color = vec4(1, 1, 1, 1);"                  "\n"
        "}"                                             "\n"
    };
    
    static constexpr size_t size = 40;
    static constexpr size_t offset = 20;
};

#ifdef CREATE_PROGRAM
#undef CREATE_PROGRAM
#endif

#define CREATE_PROGRAM \
std::unique_ptr<Graphics::Program> program; \
try { \
    program = std::make_unique<Graphics::Program>(context, *vertex, *fragment); \
} \
catch (std::exception const& e) { \
    GTEST_SKIP() << "An exception was thrown: " << e.what() << "." << std::endl; \
}

/* Create */

TEST_F(ProgramTest, CreateSuccess)
{
    EXPECT_NO_THROW({
        Graphics::Program program (context, *vertex, *fragment);
        EXPECT_TRUE(program.is_valid());
    });
}

TEST_F(ProgramTest, CreateInvalidContext)
{
    EXPECT_THROW(
        Graphics::Program program ({}, *vertex, *fragment),
        std::invalid_argument);
}

TEST_F(ProgramTest, CreateAnotherThread)
{
    auto createProgram = [&] {
        EXPECT_THROW(
            Graphics::Program program (context, *vertex, *fragment),
            std::logic_error);
    };

    std::thread thread(createProgram);
    thread.join();
}

TEST_F(ProgramTest, CreateInvalidShaders)
{
    vertex->clear();
    fragment->clear();

    EXPECT_THROW(
        Graphics::Program program (context, *vertex, *fragment),
        std::invalid_argument);
}

/* Move */

TEST_F(ProgramTest, MoveSuccess)
{
    CREATE_PROGRAM;

    EXPECT_NO_THROW({
        Graphics::Program other (std::move(*program));

        EXPECT_TRUE(other.is_valid());
        EXPECT_FALSE(program->is_valid());
    });
}

TEST_F(ProgramTest, MoveAnotherThread)
{
    CREATE_PROGRAM;

    auto moveProgram = [&] {
        EXPECT_THROW(
            Graphics::Program other (std::move(*program)),
            std::logic_error);
    };

    std::thread thread (moveProgram);
    thread.join();
}

/* Bind */

TEST_F(ProgramTest, BindSuccess)
{
    CREATE_PROGRAM;
    EXPECT_NO_THROW(Graphics::Binding binding { *program });
}

TEST_F(ProgramTest, BindMany)
{
    CREATE_PROGRAM;

    EXPECT_NO_THROW({
        Graphics::Binding binding { *program };
        EXPECT_THROW(
            Graphics::Binding other { *program },
            std::logic_error
        );
    });
}