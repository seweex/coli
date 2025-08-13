#include "coli/geometry/shape.h"

namespace Coli::Geometry
{
    template <bool Is2D>
    void Shape<Is2D>::fail_invalid_argument(std::string_view msg) {
        throw std::invalid_argument(msg.data());
    }

    template <bool Is2D>
    btCollisionShape* Shape<Is2D>::get() const noexcept
    {
        auto visitor = [] (auto&& shape) {
            return static_cast<btCollisionShape *>(std::addressof(shape));
        };

        return std::visit(visitor, myShape);
    }

    template <bool Is2D>
    Shape<Is2D>::Shape(SphereTag, float_type radius) noexcept :
        myShape(std::in_place_type<btSphereShape>, static_cast<btScalar>(radius))
    {
        if (!(radius > 0))
            fail_invalid_argument("Radius must be greater than 0");
    }

    template <>
    Shape<false>::Shape(BoxTag, vector_type const& sizes) noexcept :
        myShape(std::in_place_type<btBoxShape>, btVector3(
            static_cast<btScalar>(sizes.x) / 2.f,
            static_cast<btScalar>(sizes.y) / 2.f,
            static_cast<btScalar>(sizes.z)) / 2.f)
    {
        for (glm::length_t i = 0; i < vector_type::length(); ++i)
            if (!(sizes[i] > 0))
                fail_invalid_argument("All sizes components must be greater than 0");
    }

    template <>
    Shape<true>::Shape(BoxTag, vector_type const& sizes) noexcept :
        myShape(std::in_place_type<btBoxShape>, btVector3(
            static_cast<btScalar>(sizes.x) / 2.f,
            static_cast<btScalar>(sizes.y) / 2.f,
            0.f))
    {
        for (glm::length_t i = 0; i < vector_type::length(); ++i)
            if (!(sizes[i] > 0))
                fail_invalid_argument("All sizes components must be greater than 0");
    }

    template <bool Is2D>
    Shape<Is2D>::Shape(CapsuleTag, float_type radius, float_type height) noexcept :
        myShape(std::in_place_type<btCapsuleShapeZ>, static_cast<btScalar>(radius), static_cast<btScalar>(height))
    {
        if (!(radius > 0))
            fail_invalid_argument("Radius must be greater than 0");

        if (!(height > 0))
            fail_invalid_argument("Height must be greater than 0");
    }

    template <bool Is2D>
    Shape<Is2D>::Shape(CylinderTag, vector_type const& sizes) noexcept requires !Is2D :
        myShape(std::in_place_type<btCylinderShapeZ>, btVector3(
            static_cast<btScalar>(sizes.x) / 2.f,
            static_cast<btScalar>(sizes.y) / 2.f,
            static_cast<btScalar>(sizes.z) / 2.f))
    {
        for (glm::length_t i = 0; i < vector_type::length(); ++i)
            if (!(sizes[i] > 0))
                fail_invalid_argument("All sizes components must be greater than 0");
    }

    template <bool Is2D>
    Shape<Is2D>::~Shape() noexcept = default;

    template class COLI_EXPORT Shape<true>;
    template class COLI_EXPORT Shape<false>;
}