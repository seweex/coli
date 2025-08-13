#ifndef COLI_PHYSICS_BODY_H
#define COLI_PHYSICS_BODY_H

#include "coli/geometry/shape.h"

/// @brief Namespace for the all physical stuff.
namespace Coli::Physics
{
    /**
     * @brief Physical body.
     * @details Implements dynamic physical body.
     *
     * @tparam Is2D Dimensions flag. True for 2D, false for default 3D.
     *
     * @note Not thread-safe. Concurrent access requires
     * external synchronization.
     */
    template <bool Is2D>
    class COLI_EXPORT Body final
    {
        [[noreturn]] static void fail_invalid_argument(std::string_view msg);

        using vector_type = Types::vector_type<Is2D>;

        [[nodiscard]] static std::shared_ptr<Geometry::Shape<Is2D>>
        verify_shape(std::weak_ptr<Geometry::Shape<Is2D>> const& shape);

        [[nodiscard]] static btTransform
        make_transform(vector_type const& position) noexcept;

        [[nodiscard]] static btVector3
        get_inertia(float_type mass, std::shared_ptr<Geometry::Shape<Is2D>> shape);

        void bind_to_world(btDynamicsWorld* world) noexcept;

    public:
        /**
         * @brief Creates body.
         * @details Creates a physical dynamic body with user-specified
         * properties.
         *
         * @param mass Object mass, must be not less than;
         * @param position Initial world position;
         * @param shape Valid pointer to a shape.
         *
         * @throw std::invalid_argument If the mass is less than 0;
         * @throw std::invalid_argument If the shape pointer is expired.
         */
        Body(float_type mass, vector_type const& position, std::weak_ptr<Geometry::Shape<Is2D>> shape);

        Body(Body&&) = delete;
        Body(const Body&) = delete;

        Body& operator=(Body&&) = delete;
        Body& operator=(const Body&) = delete;

        /**
         * @brief Destroys body.
         * @details Destroys the physical body and unregister it from the world.
         */
        ~Body() noexcept;

        /**
         * @brief Returns shape.
         * @details Returns the currently used shape.
         *
         * @return Unexpired pointer to the used shape.
         */
        [[nodiscard]] std::weak_ptr<Geometry::Shape<Is2D>> shape() const noexcept;

        /**
         * @brief Sets up new shape.
         * @details Sets up a new shape.
         *
         * @param newValue New valid shape pointer.
         *
         * @throws std::invalid_argument If the shape pointer is expired.
         */
        void shape(std::weak_ptr<Geometry::Shape<Is2D>> newValue);

    private:
        btDefaultMotionState myMotionState;
        std::shared_ptr<Geometry::Shape<Is2D>> myShape;
        btRigidBody myRigidBody;

        btDynamicsWorld* myWorld;
    };

#if COLI_BUILD
#else
    extern template class COLI_EXPORT Body<true>;
    extern template class COLI_EXPORT Body<false>;
#endif

    /// @brief 2D physical dynamic body.
    using Body2D = Body<true>;

    /// @brief 3D physical dynamic body.
    using Body3D = Body<false>;
}

#endif