#include "coli/game/scene.h"
#include "coli/game/components/layer.h"

namespace Coli::Game
{
    namespace Detail
    {
        /* Objects Orderer */

        ObjectsOrderer::ObjectsOrderer() noexcept = default;

        ObjectsOrderer::ObjectsOrderer(ObjectsOrderer const&) = default;
        ObjectsOrderer::ObjectsOrderer(ObjectsOrderer&&) noexcept = default;

        ObjectsOrderer& ObjectsOrderer::operator=(ObjectsOrderer const&) = default;
        ObjectsOrderer& ObjectsOrderer::operator=(ObjectsOrderer&&) noexcept = default;

        ObjectsOrderer::~ObjectsOrderer() noexcept = default;

        std::vector<ObjectHandle>& ObjectsOrderer::operator()
        (std::shared_ptr<entt::registry> const& registry) const
        {
            auto const& layers = registry->storage<Components::Layer>();
            auto const& entities = registry->storage<entt::entity>();

            myBuffer.assign(entities.size(), { registry, {} });

            auto iterLayered = myBuffer.begin();
            auto iterUnlayered = iterLayered + static_cast<ptrdiff_t>(layers.size());

            for (auto const entity : entities)
                if (layers.contains(entity))
                    iterLayered++ ->myHandle = entity;
                else
                    iterUnlayered++ ->myHandle = entity;

            std::sort(myBuffer.begin(), iterLayered,
            [&](ObjectHandle const& left, ObjectHandle const& right) -> bool
            {
                auto const leftLayer = registry->get<Components::Layer>(left.myHandle).layer();
                auto const rightLayer = registry->get<Components::Layer>(right.myHandle).layer();

                return leftLayer < rightLayer;
            });

            return myBuffer;
        }

        /* Objects Filter */

        ObjectsFilter::ObjectsFilter() noexcept = default;

        ObjectsFilter::ObjectsFilter(ObjectsFilter const&) = default;
        ObjectsFilter::ObjectsFilter(ObjectsFilter&&) noexcept = default;

        ObjectsFilter& ObjectsFilter::operator=(ObjectsFilter const&) = default;
        ObjectsFilter& ObjectsFilter::operator=(ObjectsFilter&&) noexcept = default;

        ObjectsFilter::~ObjectsFilter() noexcept = default;
    }

    /* Scene */

    Scene::Scene(Scene&&) noexcept = default;
    Scene& Scene::operator=(Scene&&) noexcept = default;

    Scene::~Scene() noexcept = default;

    Scene::Scene() :
        myRegistry (std::make_shared<entt::registry>())
    {}

    ObjectHandle Scene::create() {
        return { myRegistry, myRegistry->create() };
    }

    void Scene::destroy(ObjectHandle handle) noexcept {
        if (handle.myRegistry.lock() == myRegistry && !handle.expired())
            myRegistry->destroy(handle.myHandle);
    }

    std::vector<ObjectHandle> const& Scene::ordered() const {
        return myOrderer(myRegistry);
    }

    std::vector<ObjectHandle>& Scene::ordered() {
        return myOrderer(myRegistry);
    }
}
