#include "coli/game/object.h"

namespace Coli::Game
{
    /* ObjectHandle */

    ObjectHandle::ObjectHandle(ObjectHandle const& other) noexcept = default;
    ObjectHandle& ObjectHandle::operator=(ObjectHandle const& other) noexcept = default;

    ObjectHandle::ObjectHandle(ObjectHandle&&) noexcept = default;
    ObjectHandle& ObjectHandle::operator=(ObjectHandle&&) noexcept = default;

    ObjectHandle::~ObjectHandle() noexcept = default;

    ObjectHandle::ObjectHandle(std::nullptr_t) noexcept :
        myHandle ()
    {}

    void ObjectHandle::fail_on_expired() {
        throw std::bad_weak_ptr();
    }

    void ObjectHandle::fail_not_exists() {
        throw std::invalid_argument("Object doesn't contain a component of this type");
    }

    ObjectHandle::ObjectHandle(std::weak_ptr<entt::registry> registry, entt::entity handle) noexcept :
        myRegistry (std::move(registry)),
        myHandle   (handle)
    {}

    bool ObjectHandle::expired() const noexcept
    {
        if (auto const registry = myRegistry.lock()) [[likely]]
            return !registry->valid(myHandle);

        return true;
    }
}