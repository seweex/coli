#ifndef COLI_GAME_OBJECT_H
#define COLI_GAME_OBJECT_H

#include "coli/utility.h"

/// @brief Namespace for the all game-related stuff.
namespace Coli::Game
{
    /**
     * @brief For internal details.
     * @note The user should not use this namespace.
     */
    namespace Detail
    {
        class COLI_EXPORT ObjectsOrderer;
    }

    /**
     * @brief Object handle class.
     * @details Handles to a game object in scenes in the ECS architecture.
     * Provides safe access to an entity in scenes.
     *
     * @note Not thread-safe. Concurrent access requires
     * external synchronization.
     *
     * @note If an object has lost all of its handles, it continues to exist.
    */
    class COLI_EXPORT ObjectHandle final
    {
        [[noreturn]] static void fail_on_expired();
        [[noreturn]] static void fail_not_exists();

    public:
        /**
         * @brief Creates handle to already existing entity.
         * @details Creates handle and binds it to already
         * existing entity and its registry.
         *
         * @param registry Entity's creation registry;
         * @param handle Handle to the entity.
         *
         * @warning The user should not use this constructor.
         * You can get the handle in the scene class.
         */
        ObjectHandle(std::weak_ptr<entt::registry> registry, entt::entity handle) noexcept;

        /**
         * @brief Creates invalid handle.
         * @details Creates handle that is not bound to any entity.
         */
        ObjectHandle(std::nullptr_t) noexcept;

        friend class Scene;
        friend class Detail::ObjectsOrderer;

        /**
         * @brief Creates handle that is bound to the other.
         * @details Creates a handle and binds it to the other's
         * bound entity.
         *
         * @param other Other handle.
         */
        ObjectHandle(ObjectHandle const& other) noexcept;

        /**
         * @brief Moves handle.
         * @details Creates a handle and binds it to the other's
         * bound entity, then resets the other.
         *
         * @param other Other handle.
         */
        ObjectHandle(ObjectHandle && other) noexcept;

        /**
         * @brief Copies handle.
         * @details Binds the handle to the other's bound entity.
         *
         * @param other Other handle.
         */
        ObjectHandle &operator=(ObjectHandle const& other) noexcept;

        /**
         * @brief Moves handle.
         * @details Binds the handle to the other's bound entity and
         * resets the other handle.
         *
         * @param other Other handle.
         */
        ObjectHandle &operator=(ObjectHandle && other) noexcept;

        /**
         * @brief Destroys the handle.
         * @details Destroys the handle to the entity, but it does not
         * destroy the entity.
         */
        ~ObjectHandle() noexcept;

        /**
         * @brief Returns handle expiration
         * @details Return the validity of the handle
         *
         * @return Handle expiration
         *
         * @retval True If the handle is valid
         * @retval False Otherwise
         */
        [[nodiscard]] bool expired() const noexcept;

        /**
         * @brief Adds component to handle
         * @details Creates a component of the specific type in
         * the handled entity.
         *
         * @tparam T Type of component to create;
         * @tparam Args Types of argument passed to the
         * component constructor.
         *
         * @param args Argument passed to the component constructor.
         *
         * @throw std::bad_weak_ptr If called on expired handle;
         * @throw std::bad_alloc If allocation fails;
         * @throw T Any T(Args...) exception.
         *
         * @return Reference to newly created component.
         */
        template <class T, class... Args>
            requires (std::constructible_from<std::remove_cvref_t<T>, Args...>)
        std::remove_cvref_t<T>& emplace(Args&&... args)
        {
            using type = std::remove_cvref_t<T>;

            if (auto const registry = myRegistry.lock()) [[likely]]
            {
                if constexpr (sizeof...(Args) == 0)
                    return registry->emplace_or_replace<type>(myHandle);
                else
                    return registry->emplace_or_replace<type, Args...>(myHandle, std::forward<Args>(args)...);{}
            }

            fail_on_expired();
        }

        /**
         * @brief Retrieves component.
         * @details Returns the storing component of the specific type
         * that contains in the handled entity.
         *
         * @tparam T Type of component to get.
         *
         * @throw std::bad_weak_ptr If called on expired handle;
         * @throw std::invalid_argument If there is no component of T type.
         *
         * @return Reference to the component.
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
        template <class T>
        [[nodiscard]] std::remove_cvref_t<T>& get() {
            return const_cast<std::remove_cvref_t<T>&>(std::as_const(*this).get<T>());
        }

        /**
         * @brief Trying to retrieve component.
         * @details Returns a pointer to the storing component of the specific type
         * that contains in the handled entity, or nullptr if there is no component
         * of this T.
         *
         * @tparam T Type of component to try to get.
         *
         * @return Pointer to the component.
         *
         * @retval Nullptr If there is no component of this T or the handle is invalid;
         * @retval ValidPointer If a component of T type is found.
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
        template <class T>
        [[nodiscard]] std::remove_cvref_t<T>* try_get() noexcept {
            return const_cast<std::remove_cvref_t<T>*>(std::as_const(*this).try_get<T>());
        }

        /**
         * @brief Returns component's presence.
         * @details Returns a boolean flag indicating the component's presence.
         *
         * @tparam T Type of component.
         *
         * @return Boolean flag indicating the component presence.
         *
         * @retval True If there is a component of T type or the handle is invalid;
         * @retval False Otherwise.
         */
        template <class T>
        [[nodiscard]] bool contains() const noexcept
        {
            using type = std::remove_cvref_t<T>;

            if (auto const registry = myRegistry.lock()) [[likely]]
                return registry->view<type>().contains(myHandle);

            return false;
        }

        /**
         * @brief Destroys component.
         * @details Destroys the component of the specific type in
         * the handled entity.
         *
         * @tparam T Type of component to destroy.
         */
        template <class T>
        void destroy() noexcept
        {
            using type = std::remove_cvref_t<T>;

            if (auto const registry = myRegistry.lock()) [[likely]]
                registry->remove<type>(myHandle);
        }

    private:
        std::weak_ptr<entt::registry> myRegistry;
        entt::entity myHandle;
    };
}

#endif
