#include "coli/physics/world.h"

namespace Coli::Physics
{
    template <bool Is2D>
    Coli::Physics::World<Is2D>::World(vector_type const& gravity) noexcept :
        myDispatcher(std::addressof(myConfig)),
        myWorld(
            std::addressof(myDispatcher),
            std::addressof(myBroadphase),
            std::addressof(myConstraintSolver),
            std::addressof(myConfig))
    {
        if constexpr (Is2D)
            myWorld.setGravity(btVector3(
                static_cast<float>(gravity.x),
                static_cast<float>(gravity.y),
                0.f));
        else
            myWorld.setGravity(btVector3(
                static_cast<float>(gravity.x),
                static_cast<float>(gravity.y),
                static_cast<float>(gravity.z)));
    }

    template <bool Is2D>
    Coli::Physics::World<Is2D>::~World() noexcept = default;

    template class COLI_EXPORT World<true>;
    template class COLI_EXPORT World<false>;
}
