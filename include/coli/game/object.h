#ifndef COLI_GAME_OBJECT_H
#define COLI_GAME_OBJECT_H

#include "coli/utility.h"

namespace Coli::Game
{
    namespace Detail
    {
        class COLI_EXPORT ObjectsOrderer;
    }

    /**
     * @brief Handle to a game object in a scene in the ECS architecture.
     * Provides safe (not thread-safe) access to an entity in a scene
     *
     * @warning This class is not thread-safe
    */
    class COLI_EXPORT ObjectHandle final
    {
        [[noreturn]] static void fail_on_expired();
        [[noreturn]] static void fail_not_exists();

    public:
        /// @warning Not supposed to be used by the user.
        /// It is public for free access from 'std'
        ObjectHandle(std::weak_ptr<entt::registry> registry, entt::entity handle) noexcept;

        /// @brief Makes an invalid (expired) handle
        ObjectHandle(std::nullptr_t) noexcept;

        friend class Scene;
        friend class Detail::ObjectsOrderer;

        ObjectHandle(ObjectHandle const&) noexcept;
        ObjectHandle(ObjectHandle &&) noexcept;

        ObjectHandle &operator=(ObjectHandle const&) noexcept;
        ObjectHandle &operator=(ObjectHandle &&) noexcept;

        ~ObjectHandle() noexcept;

        /// @brief Check has an object expired
        [[nodiscard]] bool expired() const noexcept;

        /**
         * @brief Adds a component to handling object
         *
         * @tparam T Type of component to add
         * @tparam Args Types of component constructor arguments
         *
         * @param args Arguments that will pass to constructor
         *
         * @throw std::bad_weak_ptr If the handle has expired
         *
         * @return A reference to the created component
         */
        template <class T, class... Args>
            requires (std::constructible_from<std::remove_cvref_t<T>, Args...>)
        std::remove_cvref_t<T>& emplace(Args&&... args)
        {
            using type = std::remove_cvref_t<T>;

            if (auto const registry = myRegistry.lock()) [[likely]]
                return registry->emplace_or_replace<type, Args...>(myHandle, std::forward<Args>(args)...);

            fail_on_expired();
        }

        /**
         * @brief Gets a component from handling object
         *
         * @tparam T Type of component to get
         *
         * @throw std::bad_weak_ptr If the handle has expired
         * @throw std::invalid_argument If the object doesn't contain
         *      a component of this type
         *
         * @return A const reference to the component
         */
        template <class T>
        [[nodiscard]] std::remove_cvref_t<T> const& get() const
        {
            using type = std::remove_cvref_t<T>;

            if (auto const registry = myRegistry.lock()) [[likely]]
            {
                if (auto const component = registry->try_get<type>(myHandle)) [[likely]]
                    return *component;

                fail_not_exists();
            }

            fail_on_expired();
        }

        /// @copydoc get() const
        /// @return A non-const reference to the component
        template <class T>
        [[nodiscard]] std::remove_cvref_t<T>& get() {
            return const_cast<std::remove_cvref_t<T>&>(std::as_const(*this).get<T>());
        }

        /**
         * @brief Tries to get a component from handling object
         *
         * @tparam T Type of component to get
         *
         * @return A const pointer to the component
         */
        template <class T>
        [[nodiscard]] std::remove_cvref_t<T> const* try_get() const noexcept
        {
            using type = std::remove_cvref_t<T>;

            if (auto const registry = myRegistry.lock()) [[likely]]
                return registry->try_get<type>(myHandle);

            return nullptr;
        }

        /// @copydoc try_get() const
        /// @return A non-const pointer to the component
        template <class T>
        [[nodiscard]] std::remove_cvref_t<T>* try_get() noexcept {
            return const_cast<std::remove_cvref_t<T>*>(std::as_const(*this).try_get<T>());
        }

        /**
         * @brief Says if the object have a component of this type
         *
         * @tparam T Type of component
         *
         * @return The bool flag which means the object has a component of this T
         */
        template <class T>
        [[nodiscard]] bool contains() const noexcept
        {
            using type = std::remove_cvref_t<T>;

            if (auto const registry = myRegistry.lock()) [[likely]]
                return registry->view<type>().contains(myHandle);

            return false;
        }

    private:
        std::weak_ptr<entt::registry> myRegistry;
        entt::entity myHandle;
    };
}

#endif
