#include <coli/game-engine.h>
#include <gtest/gtest.h>

#include <memory>
#include <thread>
#include <coli/graphics/vertex_array.h>

using namespace Coli;

class VertexArrayTest :
    public ::testing::Test
{
protected:
    using vertex_type = Geometry::Vertex2D;

    void SetUp() override
    {
        try {
            if (!context)
                context = std::make_shared<Graphics::Context>();

            if (!window)
                window = std::make_unique<Graphics::Window>(context, settings);

            if (!vertices) {
                vertex_type const data [] = {
                    { Types::vector_type<true>{ -0.5,  0.5 }, Types::vector_type<true>{ 0.0, 1.0 } },
                    { Types::vector_type<true>{  0.5,  0.5 }, Types::vector_type<true>{ 1.0, 1.0 } },
                    { Types::vector_type<true>{  0.5, -0.5 }, Types::vector_type<true>{ 1.0, 0.0 } },
                    { Types::vector_type<true>{ -0.5, -0.5 }, Types::vector_type<true>{ 0.0, 0.0 } },
                };

                void const* ptr = data;
                vertices = std::make_shared<Graphics::VertexStorage>(context, ptr, sizeof(vertices));
            }

            if (!indices) {
                unsigned const data [] = { 0, 1, 2,   2, 3, 0 };
                void const* ptr = data;

                indices = std::make_shared<Graphics::IndexStorage>(context, ptr, sizeof(indices));
            }
        }
        catch (std::bad_alloc const&) {
            GTEST_SKIP() << "A 'std::bad_alloc' was thrown" << std::endl;
        }
    }

    const Graphics::Window::Settings settings { "VertexArray Testing", 640, 480 };
    
    std::shared_ptr<Graphics::Context> context;
    std::unique_ptr<Graphics::Window> window;
    
    std::shared_ptr<Graphics::VertexStorage> vertices;
    std::shared_ptr<Graphics::IndexStorage> indices;

    static const inline vertex_type dummy_vertex {
        Types::vector_type<true>{ 0 }, Types::vector_type<true>{ 0 } };
};

#ifdef CREATE_VERTEX_ARRAY
#undef CREATE_VERTEX_ARRAY
#endif

#define CREATE_VERTEX_ARRAY \
std::unique_ptr<Graphics::VertexArray> vertexArray; \
try { \
    vertexArray = std::make_unique<Graphics::VertexArray>(context, vertices, indices, dummy_vertex); \
} \
catch (std::exception const& e) { \
    GTEST_SKIP() << "An exception was thrown: " << e.what() << "." << std::endl; \
}

/* Create */

TEST_F(VertexArrayTest, CreateSuccess)
{
    EXPECT_NO_THROW({
        Graphics::VertexArray vertexArray (context, vertices, indices, dummy_vertex);
        EXPECT_TRUE(vertexArray.is_valid());
    });
}

TEST_F(VertexArrayTest, CreateInvalidContext)
{
    EXPECT_THROW(
        Graphics::VertexArray vertexArray ({}, vertices, indices, dummy_vertex),
        std::invalid_argument);
}

TEST_F(VertexArrayTest, CreateAnotherThread)
{
    auto createVertexArray = [&] {
        EXPECT_THROW(
            Graphics::VertexArray vertexArray (context, vertices, indices, dummy_vertex),
            std::logic_error);
    };

    std::thread thread(createVertexArray);
    thread.join();
}

TEST_F(VertexArrayTest, CreateInvalidStorages)
{
    vertices.reset();
    indices.reset();

    EXPECT_THROW(
        Graphics::VertexArray vertexArray (context, vertices, indices, dummy_vertex),
        std::invalid_argument);
}

/* Move */

TEST_F(VertexArrayTest, MoveSuccess)
{
    CREATE_VERTEX_ARRAY;

    EXPECT_NO_THROW({
        Graphics::VertexArray other (std::move(*vertexArray));

        EXPECT_TRUE(other.is_valid());
        EXPECT_FALSE(vertexArray->is_valid());
    });
}

TEST_F(VertexArrayTest, MoveAnotherThread)
{
    CREATE_VERTEX_ARRAY;

    auto moveVertexArray = [&] {
        EXPECT_THROW(
            Graphics::VertexArray other (std::move(*vertexArray)),
            std::logic_error);
    };

    std::thread thread (moveVertexArray);
    thread.join();
}

/* Bind */

TEST_F(VertexArrayTest, BindSuccess)
{
    CREATE_VERTEX_ARRAY;
    EXPECT_NO_THROW(Graphics::Binding binding { *vertexArray });
}

TEST_F(VertexArrayTest, BindMany)
{
    CREATE_VERTEX_ARRAY;

    EXPECT_NO_THROW({
        Graphics::Binding binding { *vertexArray };
        EXPECT_THROW(
            Graphics::Binding other { *vertexArray },
            std::logic_error
        );
    });
}