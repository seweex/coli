#ifndef COLI_PHYSICS_WORLD_H
#define COLI_PHYSICS_WORLD_H

#include "coli/utility.h"

/// @brief Namespace for the all physical stuff.
namespace Coli::Physics
{
    /**
     * @brief Physical world.
     * @details Implements world for physical simulation.
     *
     * @tparam Is2D Dimensions flag. True for 2D, false for default 3D.
     *
     * @note Not thread-safe. Concurrent access requires
     * external synchronization.
     */
    template <bool Is2D>
    class COLI_EXPORT World final
    {
        using vector_type = Types::vector_type<Is2D>;

    public:
        /**
         * @brief Creates world.
         * @details Creates a world and sets the gravity to user-specified value.
         *
         * @param gravity World gravity.
         */
        World(vector_type const& gravity) noexcept;

        World(World&&) = delete;
        World& operator=(World&&) = delete;

        World(World const&) = delete;
        World& operator=(World const&) = delete;

        /**
         * @brief Destroys world.
         * @details Destroys the world and all its objects.
         */
        ~World() noexcept;

    private:
        btDefaultCollisionConfiguration myConfig;
        btCollisionDispatcher myDispatcher;
        btDbvtBroadphase myBroadphase;
        btSequentialImpulseConstraintSolver myConstraintSolver;
        btDiscreteDynamicsWorld myWorld;
    };

#if COLI_BUILD
#else
    extern template class COLI_EXPORT World<true>;
    extern template class COLI_EXPORT World<false>;
#endif

    /// @brief World for 2D physical simulations.
    using World2D = World<true>;

    /// @brief World for 3D physical simulations.
    using World3D = World<false>;
}

#endif