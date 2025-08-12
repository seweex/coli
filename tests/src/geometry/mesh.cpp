#include <coli/game-engine.h>
#include <gtest/gtest.h>

using namespace Coli;

/* Create */

#ifdef CREATE_UNIQUE_DATA
#undef CREATE_UNIQUE_DATA
#endif

#define CREATE_UNIQUE_DATA \
std::vector<Geometry::Vertex2D> vertices;\
std::vector<unsigned> indices; \
try { \
    vertices = {\
        Geometry::Vertex2D{ glm::dvec2{ -0.5,  -0.5 }, glm::dvec2{ 0.0, 0.0 } }, \
        Geometry::Vertex2D{ glm::dvec2{ -0.5,   0.5 }, glm::dvec2{ 0.0, 1.0 } }, \
        Geometry::Vertex2D{ glm::dvec2{  0.5,   0.5 }, glm::dvec2{ 1.0, 1.0 } }, \
        Geometry::Vertex2D{ glm::dvec2{  0.5,  -0.5 }, glm::dvec2{ 1.0, 0.0 } }, \
    };\
    \
    indices = { \
        0, 1, 2, 2, 3, 0 \
    };\
}\
catch (std::exception const& e) { \
    GTEST_SKIP() << "An exception was thrown: " << e.what() << "." << std::endl; \
}

#ifdef CREATE_DATA
#undef CREATE_DATA
#endif

#define CREATE_DATA \
std::vector<Geometry::Vertex2D> vertices;\
try { \
    vertices = {\
        Geometry::Vertex2D{ glm::dvec2{ -0.5,  -0.5 }, glm::dvec2{ 0.0, 0.0 } }, \
        Geometry::Vertex2D{ glm::dvec2{ -0.5,   0.5 }, glm::dvec2{ 0.0, 1.0 } }, \
        Geometry::Vertex2D{ glm::dvec2{  0.5,   0.5 }, glm::dvec2{ 1.0, 1.0 } }, \
        Geometry::Vertex2D{ glm::dvec2{  0.5,   0.5 }, glm::dvec2{ 1.0, 1.0 } }, \
        Geometry::Vertex2D{ glm::dvec2{  0.5,  -0.5 }, glm::dvec2{ 1.0, 0.0 } }, \
        Geometry::Vertex2D{ glm::dvec2{ -0.5,  -0.5 }, glm::dvec2{ 0.0, 0.0 } }, \
    };\
}\
catch (std::exception const& e) { \
    GTEST_SKIP() << "An exception was thrown: " << e.what() << "." << std::endl; \
}

TEST(MeshTest, CreateAndUnique)
{
    CREATE_DATA;

    Geometry::Mesh<Geometry::Vertex2D> mesh
    { vertices.begin(), vertices.end() };

    std::unordered_set<Geometry::Vertex2D, Utility::Hash<Geometry::Vertex2D>> uniqueVertices;
    uniqueVertices.reserve(vertices.size());

    uniqueVertices.insert(vertices.begin(), vertices.end());
    auto const meshSpan = mesh.get_vertices();

    EXPECT_TRUE(std::any_of(
        meshSpan.begin(),
        meshSpan.end(),
        [&] (Geometry::Vertex2D const& vertex) {
            return uniqueVertices.contains(vertex);
        }
    ));
}

TEST(MeshTest, CreateAlreadyUnique)
{
    CREATE_UNIQUE_DATA;

    Geometry::Mesh<Geometry::Vertex2D> mesh
    { vertices.begin(), vertices.end(), indices.begin(), indices.end() };

    std::unordered_set<Geometry::Vertex2D, Utility::Hash<Geometry::Vertex2D>> uniqueVertices;
    uniqueVertices.reserve(vertices.size());

    uniqueVertices.insert(vertices.begin(), vertices.end());
    auto const meshSpan = mesh.get_vertices();

    EXPECT_TRUE(std::any_of(
        meshSpan.begin(),
        meshSpan.end(),
        [&] (Geometry::Vertex2D const& vertex) {
            return uniqueVertices.contains(vertex);
        }
    ));
}

TEST(MeshTest, CreateMoved)
{
    CREATE_UNIQUE_DATA;

    Geometry::Mesh<Geometry::Vertex2D> mesh1
    { vertices.begin(), vertices.end(), indices.begin(), indices.end() };

    Geometry::Mesh<Geometry::Vertex2D> mesh2
    { std::move(vertices), std::move(indices) };

    auto const span1 = mesh1.get_vertices();
    auto const span2 = mesh2.get_vertices();

    EXPECT_TRUE(std::equal(
        span1.begin(),
        span1.end(),
        span2.begin()));
}