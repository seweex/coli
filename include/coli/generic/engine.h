#ifndef COLI_GENERIC_ENGINE_H
#define COLI_GENERIC_ENGINE_H

#include "coli/utility.h"
#include "coli/generic/system.h"
#include "coli/game/scene.h"

namespace Coli::Generic
{
    /**
     * @brief Main game engine class.
     * @details Processes systems passing the active scene.
     * Manages the game state.
     *
     * @note Not thread-safe. Concurrent access requires
     * external synchronization.
    */
    class COLI_EXPORT Engine final
    {
        [[noreturn]] static void fail_already_exist();

    public:
        /**
         * @brief Creates game engine.
         * @details Creates a game engine. No active scene, no systems.
         */
        Engine() noexcept(std::is_nothrow_default_constructible_v<decltype(mySystems)>);

        /**
         * @brief Moves game engine.
         * @details Just moves the game engine.
         *
         * @param other Other game engine.
         */
        Engine(Engine && other) noexcept;
        Engine(Engine const &) = delete;

        /**
         * @brief Moves game engine.
         * @details Just moves the game engine.
         *
         * @param other Other game engine.
         * @return Reference to itself.
         */
        Engine &operator=(Engine && other) noexcept;
        Engine &operator=(Engine const &) = delete;

        /**
         * @brief Destroys game engine.
         * @details Destroys the game engine. Destroys the systems and
         * loses the active scene reference.
         */
        ~Engine() noexcept;

        /**
         * @brief Returns active scene.
         * @details Returns the active scene was previously set.
         *
         * @return Weak smart pointer to the active scene.
         *
         * @retval Valid If there is an active scene set;
         * @retval Expired If no active scene was set.
         */
        [[nodiscard]] std::weak_ptr<Game::Scene const> active_scene() const noexcept;

        /// @copydoc active_scene() const
        [[nodiscard]] std::weak_ptr<Game::Scene> active_scene() noexcept;

        /**
         * @brief Sets active scene.
         * @details Sets the specific scene active.
         * The engine will process objects in this scene.
         *
         * @param scene New active scene weak pointer. Pass expired one to
         * reset the active scene.
         */
        void active_scene(std::weak_ptr<Game::Scene> scene) noexcept;

        /**
         * @brief Makes system.
         * @detail Makes a system of the specific type and returns a weak smart pointer to it.
         *
         * @tparam T Type of system to add. It must be derived from Coli::Generic::SystemBase;
         * @tparam Args Types of T constructor arguments.
         *
         * @param args Arguments to a T type constructor.
         *
         * @throw std::bad_alloc If allocation fails;
         * @throw std::logic_error If the system are already exists;
         * @throw T Any of T(Args...) constructor exceptions.
         *
         * @return A weak pointer to the newly created system.
         */
        template <std::derived_from<Detail::SystemBase> T, class... Args>
            requires (std::constructible_from<std::remove_cvref_t<T>, Args...>)
        std::weak_ptr<std::remove_cvref_t<T>> make_system(Args&&... args)
        {
            using type = std::remove_cvref_t<T>;

            constexpr auto const& type_info = typeid(type);

            auto [iter, inserted] = mySystems.try_emplace(type_info, nullptr);

            if (inserted) {
                std::shared_ptr<type> newSystem;

                try {
                    newSystem = std::make_shared<type>(std::forward<Args>(args)...);
                }
                catch (...) {
                    mySystems.erase(iter);
                    throw;
                }

                iter->second = newSystem;
                return newSystem;
            }

            fail_already_exist();
        }

        /**
         * @brief Gets system.
         * @detail Returns the system of the specific type if
         * the game engine contains some.
         *
         * @tparam T Type of system to get. It must be derived from Coli::Generic::SystemBase.
         *
         * @return A weak smart pointer to the system.
         *
         * @retval Valid If there is a system of this type;
         * @retval Expired If no systems of the T type.
         */
        template <class T>
        [[nodiscard]] std::weak_ptr<std::remove_cvref_t<T> const> get_system() const noexcept
        {
            using type = std::remove_cvref_t<T>;

            constexpr auto const& type_info = typeid(type);
            auto iter = mySystems.find(type_info);

            if (iter != mySystems.end())
                if (auto const system = std::dynamic_pointer_cast<type>(iter->second))
                    return system;

            return nullptr;
        }

        /// @copydoc get_system() const
        template <class T>
        [[nodiscard]] std::weak_ptr<std::remove_cvref_t<T>> get_system() noexcept {
            return std::const_pointer_cast
                <std::remove_cvref_t<T>>(std::as_const(*this).get_system<T>().lock());
        }

        /**
         * @brief Destroys system.
         * @detail Destroys the system of the specific type if
         * the game engine contains some.
         *
         * @tparam T Type of system to destroy. It must be derived from Coli::Generic::SystemBase.
         */
        template <class T>
        void remove_system() noexcept
        {
            using type = std::remove_cvref_t<T>;

            constexpr auto const& type_info = typeid(type);
            auto iter = mySystems.find(type_info);

            if (iter != mySystems.end())
                mySystems.erase(iter);
        }

        /**
         * @brief Runs game.
         * @detail Runs the game loop.
         * It is running while the running flag is True.
         * To stop it, call @ref stop().
         */
        void run();

        /**
         * @brief Stops game.
         * @detail Stops the game loop.
         */
        void stop() noexcept;

    private:
        std::unordered_map<std::type_index,
                           std::shared_ptr<Detail::SystemBase>>
        mySystems;

        std::weak_ptr<Game::Scene> myScene;
        volatile /* <- temp */ bool myStopFlag;
    };
}

#endif