#ifndef COLI_GENERIC_ENGINE_H
#define COLI_GENERIC_ENGINE_H

#include "coli/utility.h"
#include "coli/generic/system.h"
#include "coli/game/scene.h"

namespace Coli::Generic
{
    /**
     * @brief Class for managing subsystems that handles objects in a scene
     *
     * @warning This class is not thread-safe
     */
    class COLI_EXPORT Engine final
    {
        [[noreturn]] static void fail_already_exist();

    public:
        Engine() noexcept(std::is_nothrow_default_constructible_v<decltype(mySystems)>);

        Engine(Engine const &) = delete;
        Engine(Engine &&) noexcept;

        Engine &operator=(Engine const &) = delete;
        Engine &operator=(Engine &&) noexcept;

        ~Engine() noexcept;

        /**
         * @brief Gets the current active scene
         *
         * @return A weak smart pointer to current const active scene or expired if no scene
         */
        _NODISCARD std::weak_ptr<Game::Scene const> active_scene() const noexcept;

        /**
         * @copybrief active_scene() const
         *
         * @return A weak smart pointer to current active scene or expired if no scene
         */
        _NODISCARD std::weak_ptr<Game::Scene> active_scene() noexcept;

        /**
         * @brief Sets the current active scene
         *
         * @param scene A new scene to be processed or nullptr to reset
         */
        void active_scene(std::weak_ptr<Game::Scene> scene) noexcept;

        /**
         * @brief Makes a system of the T type and returns a weak smart pointer to it
         *
         * @tparam T Type of system to add, must be derived from Coli::Generic::SystemBase
         * @tparam Args Types of T constructor arguments
         *
         * @param args Arguments to a T type constructor
         *
         * @throw std::bad_alloc If allocation fails
         * @throw std::logic_error If the system are already exists
         * @throw T Any of T::T(Args&&...) constructor exception
         *
         * @return A weak pointer to newly created system
         */
        template <std::derived_from<SystemBase> T, class... Args>
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
         * @brief Gets the system of required T type
         *
         * @tparam T Type of system to get
         *
         * @return A weak pointer to the const system if it exists, otherwise an expired pointer
         */
        template <class T>
        _NODISCARD std::weak_ptr<std::remove_cvref_t<T> const> get_system() const noexcept
        {
            using type = std::remove_cvref_t<T>;

            constexpr auto const& type_info = typeid(type);
            auto iter = mySystems.find(type_info);

            if (iter != mySystems.end())
                if (auto const system = std::dynamic_pointer_cast<type>(iter->second))
                    return system;

            return nullptr;
        }

        /**
         * @copydoc get_system() const
         *
         * @return A weak pointer to the system if it exists, otherwise an expired pointer
         */
        template <class T>
        _NODISCARD std::weak_ptr<std::remove_cvref_t<T>> get_system() noexcept {
            return std::const_pointer_cast<std::remove_cvref_t<T>>(std::as_const(*this).get_system<T>().lock());
        }

        /**
         * @brief Removes the system of required T type
         *
         * @tparam T Type of system to remove
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
         * @brief Starts the game loop: update all objects from the active scene
         * by theirs systems while the stop flag is false
         *
         * @throw Systems Any of systems operator() exceptions
         */
        void run();

        /// @brief Sets the stop flag to true
        void stop() noexcept;

    private:
        std::unordered_map<std::type_index, std::shared_ptr<SystemBase>> mySystems;
        std::weak_ptr<Game::Scene> myScene;

        volatile /* <- temp */ bool myStopFlag;
    };
}

#endif