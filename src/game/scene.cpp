#include "coli/game/scene.h"
#include "coli/game/components/layer.h"

namespace Coli::Game
{
    Scene::Scene(Scene&&) noexcept = default;
    Scene& Scene::operator=(Scene&&) noexcept = default;

    Scene::~Scene() noexcept = default;

    Scene::Scene() :
        myRegistry (std::make_shared<entt::registry>())
    {}

    void Scene::reset() noexcept {
        myRegistry.reset();
    }

    bool Scene::is_valid() const noexcept {
        return static_cast<bool>(myRegistry);
    }

    ObjectHandle Scene::create() {
        return { myRegistry, myRegistry->create() };
    }

    void Scene::destroy(ObjectHandle const& handle) noexcept {
        if (handle.myRegistry.lock() == myRegistry && !handle.expired())
            myRegistry->destroy(handle.myHandle);
    }
}
