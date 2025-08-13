#include <coli/game-engine.h>
#include <gtest/gtest.h>

using namespace Coli;

class BodyTest :
    public ::testing::Test
{
protected:
    void SetUp() override
    {
        try {
            shape = std::make_shared<Geometry::Shape3D>(Geometry::Shape3D::SphereTag{}, 10);
        }
        catch (std::bad_alloc const&) {
            GTEST_SKIP() << "A 'std::bad_alloc' was thrown" << std::endl;
        }
    }

    void TearDown() override {
        shape.reset();
    }

    std::shared_ptr<Geometry::Shape3D> shape;

    static constexpr double mass = 10;
    static constexpr glm::dvec3 position = { 0.0, 0.0, 0.0 };
};

/* Create */

TEST_F(BodyTest, CreateSuccess)
{
    EXPECT_NO_THROW({
        Physics::Body3D body (mass, position, shape);
    });
}

TEST_F(BodyTest, CreateNullShape)
{
    shape.reset();

    EXPECT_THROW(
        Physics::Body3D body (mass, position, shape),
        std::invalid_argument);
}

TEST_F(BodyTest, CreateNegativeMass)
{
    EXPECT_THROW(
        Physics::Body3D body (-1.f, position, shape),
        std::invalid_argument);
}
