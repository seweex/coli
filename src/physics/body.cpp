#include "coli/physics/body.h"

#include <BulletDynamics/Dynamics/btRigidBody.h>

namespace Coli::Physics
{
    template <bool Is2D>
    void Body<Is2D>::fail_invalid_argument(std::string_view msg) {
        throw std::invalid_argument(msg.data());
    }

    template <bool Is2D>
    void Body<Is2D>::bind_to_world(btDynamicsWorld* world) noexcept
    {
        if (!myWorld) {
            myWorld = world;
            world->addRigidBody(std::addressof(myRigidBody));
        }
    }

    template <bool Is2D>
    std::shared_ptr<Geometry::Shape<Is2D>>
    Body<Is2D>::verify_shape(std::weak_ptr<Geometry::Shape<Is2D>> const& shape)
    {
        if (auto locked = shape.lock())
            return locked;
        else
            fail_invalid_argument("Shape pointer is expired");
    }

    template <bool Is2D>
    btTransform Body<Is2D>::make_transform(vector_type const& position) noexcept
    {
        btQuaternion const rot { 0.f, 0.f, 0.f, 1.f };
        btVector3 pos;

        if constexpr (Is2D) {
            pos.setX(static_cast<btScalar>(position.x()));
            pos.setY(static_cast<btScalar>(position.y()));
            pos.setZ(static_cast<btScalar>(position.z()));
        }
        else {
            pos.setX(static_cast<btScalar>(position.x()));
            pos.setY(static_cast<btScalar>(position.y()));
            pos.setZ(0);
        }

        return btTransform { rot, pos };
    }

    template <bool Is2D>
    btVector3 Body<Is2D>::get_inertia(float_type mass, std::shared_ptr<Geometry::Shape<Is2D>> shape) noexcept
    {
        if (!(mass >= 0))
            fail_invalid_argument("Mass must be not less than 0");

        btVector3 result;
        auto const raw = shape->get();

        raw->calculateLocalInertia(
            static_cast<btScalar>(mass),
            result);

        return result;
    }

    template <bool Is2D>
    Body<Is2D>::Body(float_type mass, vector_type const& position, std::weak_ptr<Geometry::Shape<Is2D>> shape) :
        myMotionState (make_transform(position)),
        myShape       (verify_shape(shape)),
        myRigidBody   (btRigidBody::btRigidBodyConstructionInfo{
            static_cast<btScalar>(mass),
            std::addressof(myMotionState),
            myShape.get(),
            get_inertia(mass, myShape) }),
        myWorld (nullptr)
    {}

    template <bool Is2D>
    Body<Is2D>::~Body() noexcept {
        if (myWorld)
            myWorld->removeRigidBody(std::addressof(myRigidBody));
    }

    template <bool Is2D>
    std::weak_ptr<Geometry::Shape<Is2D>> Body<Is2D>::shape() const noexcept {
        return myShape;
    }

    template <bool Is2D>
    void Body<Is2D>::shape(std::weak_ptr<Geometry::Shape<Is2D>> newValue) {
        myShape = verify_shape(newValue);
    }
}
