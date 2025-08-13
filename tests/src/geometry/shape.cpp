#include <coli/game-engine.h>
#include <gtest/gtest.h>

using namespace Coli;

class ShapeTest :
    public ::testing::Test
{
protected:
    static constexpr double radius = 10;
    static constexpr double height = 15;
    static constexpr glm::dvec3 sizes = { 10.0, 10.0, 10.0 };
};

/* Create */

TEST_F(ShapeTest, CreateSuccess)
{
    EXPECT_NO_THROW (Geometry::Shape3D shape (Geometry::Shape3D::SphereTag{}, radius));
    EXPECT_NO_THROW (Geometry::Shape3D shape (Geometry::Shape3D::BoxTag{}, sizes));
    EXPECT_NO_THROW (Geometry::Shape3D shape (Geometry::Shape3D::CapsuleTag{}, radius, height));
    EXPECT_NO_THROW (Geometry::Shape3D shape (Geometry::Shape3D::CylinderTag{}, sizes));
}

TEST_F(ShapeTest, CreateNegativeParams)
{
    EXPECT_THROW (Geometry::Shape3D shape (Geometry::Shape3D::SphereTag{}, -radius), std::invalid_argument);
    EXPECT_THROW (Geometry::Shape3D shape (Geometry::Shape3D::BoxTag{}, -sizes), std::invalid_argument);
    EXPECT_THROW (Geometry::Shape3D shape (Geometry::Shape3D::CapsuleTag{}, -radius, -height), std::invalid_argument);
    EXPECT_THROW (Geometry::Shape3D shape (Geometry::Shape3D::CylinderTag{}, -sizes), std::invalid_argument);
}