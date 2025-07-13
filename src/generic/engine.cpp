#include "coli/generic/engine.h"

namespace Coli::Generic
{
    /* Engine */

    Engine::Engine(Engine&&) noexcept = default;
    Engine& Engine::operator=(Engine&&) noexcept = default;

    Engine::~Engine() noexcept = default;

    Engine::Engine() noexcept(std::is_nothrow_default_constructible_v<decltype(mySystems)>) :
        myStopFlag(false)
    {}

    void Engine::fail_already_exist() {
        throw std::logic_error("The system already exists");
    }

    std::weak_ptr<Game::Scene const> Engine::active_scene() const noexcept {
        return myScene;
    }

    std::weak_ptr<Game::Scene> Engine::active_scene() noexcept {
        return myScene;
    }

    void Engine::active_scene(std::weak_ptr<Game::Scene> scene) noexcept {
        myScene = scene;
    }

    void Engine::run()
    {
        myStopFlag = false;

        while (!myStopFlag)
            if (auto const scene = myScene.lock()) [[likely]]
            {
                auto& object = scene->ordered();

                for (auto const& system : mySystems | std::views::values)
                    (*system)(object);
            }
            else
                break;
    }

    void Engine::stop() noexcept {
        myStopFlag = true;
    }
}
