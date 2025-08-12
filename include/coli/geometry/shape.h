#ifndef COLI_GEOMETRY_SHAPE_H
#define COLI_GEOMETRY_SHAPE_H

#include "coli/utility.h"

/// @brief Namespace for the all physical stuff.
namespace Coli::Physics
{
    template <bool Is2D>
    class Body;
}

/// @brief Namespace for the all geometry stuff.
namespace Coli::Geometry
{
    /**
     * @brief Represents shape.
     * @details Represents a simple shape.
     * Can be sphere, box, capsule, and cylinder.
     *
     * @tparam Is2D Dimensions flag. True for 2D, false for default 3D.
     *
     * @note Not thread-safe. Concurrent access requires
     * external synchronization.
     *
     * @note You should create this class via std::make_shared.
     */
    template <bool Is2D>
    class COLI_EXPORT Shape final
    {
        [[noreturn]] static void fail_invalid_argument(std::string_view msg);

        [[nodiscard]] btCollisionShape* get() const noexcept;

        friend class Physics::Body<Is2D>;

        using vector_type = Types::vector_type<Is2D>;

    public:
        /// @brief Specifies the shape as a sphere
        struct SphereTag {};

        /// @brief Specifies the shape as a box
        struct BoxTag {};

        /// @brief Specifies the shape as a capsule
        struct CapsuleTag {};

        /// @brief Specifies the shape as a cylinder
        struct CylinderTag {};

        /**
         * @brief Creates sphere shape.
         * @details Creates a sphere shape from user-specified radius.
         *
         * @param radius Radius of the sphere, must be greater than 0.
         *
         * @throw std::invalid_argument If radius not greater than 0.
         */
        Shape(SphereTag, float_type radius);

        /**
        * @brief Creates box shape.
        * @details Creates a box shape from user-specified sizes.
        *
        * @param sizes Sizes of box, each component must be greater than 0.
        *
        * @throw std::invalid_argument If at least one component of sizes not greater than 0.
        */
        Shape(BoxTag, vector_type const& sizes);

        /**
        * @brief Creates capsule shape.
        * @details Creates a capsule shape from user-specified radius of
        * spheres and distance between their centers.
        *
        * @param radius Radius of spheres in capsule, must be greater than 0;
        * @param height Height of capsule, it's a distance between centers of
        * two spheres, must be greater than 0.
        *
        * @throw std::invalid_argument If radius not greater than 0;
        * @throw std::invalid_argument If height not greater than 0.
        */
        Shape(CapsuleTag, float_type radius, float_type height);

        /**
        * @brief Creates cylinder shape.
        * @details Creates a cylinder shape from user-specified radius and height.
        *
        * @param sizes Sizes of cylinder, each component must be greater than 0.
        *
        * @warning Cylinder shape is available only in 3D mode, because
        * it becomes a box in 2D.
        *
        * @throw std::invalid_argument If at least one component of sizes not greater than 0.
        */
        Shape(CylinderTag, vector_type const& sizes) requires Is2D;

        Shape(Shape&&) = delete;
        Shape(Shape const&) = delete;

        Shape& operator=(Shape&&) = delete;
        Shape& operator=(const Shape &) = delete;

        /**
         * @brief Destroys shape.
         * @details Destroys the shape.
         */
        ~Shape() noexcept;

    private:
        std::variant<btSphereShape, btBoxShape, btCapsuleShapeZ, btCylinderShapeZ> myShape;
    };

#if COLI_BUILD
#else
    extern template class COLI_EXPORT Shape<true>;
    extern template class COLI_EXPORT Shape<false>;
#endif

    /// @brief 2D shape.
    using Shape2D = Shape<true>;

    /// @brief 3D shape.
    using Shape3D = Shape<false>;
}

#endif