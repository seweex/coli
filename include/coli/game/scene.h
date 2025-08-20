#ifndef COLI_GAME_SCENE_H
#define COLI_GAME_SCENE_H

#include "coli/utility.h"
#include "coli/game/object.h"

/// @brief Namespace for the all game-related stuff.
namespace Coli::Game
{
    /**
     * @brief Game scene class.
     * @details Supposed to contain game objects and supports their order.
     *
     * @note Not thread-safe. Concurrent access requires
     * external synchronization.
    */
    class COLI_EXPORT Scene final
    {
    public:
        /**
         * @brief Creates scene.
         * @details Creates an empty scene. You can create
         * and destroy game objects. They stored inside scene.
         */
        Scene();

        /**
         * @brief Moves scene.
         * @details Moves the scene. Copies the other scene and resets it.
         *
         * @param other Other scene.
         */
        Scene(Scene&& other) noexcept;
        Scene(Scene const&) = delete;

        /**
         * @brief Moves scene.
         * @details Moves the scene. Copies the other's objects scene and resets they.
         *
         * @param other Other scene.
         */
        Scene& operator=(Scene && other) noexcept;
        Scene& operator=(Scene const&) = delete;

        /**
         * @brief Destroys scene.
         * @details Destroys the scene and all its entities.
         */
        ~Scene() noexcept;

        /**
         * @brief Resets scene.
         * @details Resets the scene.
         */
        void reset() noexcept;

        /**
         * @brief Validates scene.
         * @details Returns the scene validity.
         *
         * @return The scene validity.
         *
         * @retval True If the scene is valid;
         * @retval False Otherwise.
         */
        [[nodiscard]] bool is_valid() const noexcept;

        /**
         * @brief Creates entity in scene.
         * @details Creates an entity inside the scene and returns a
         * handle to the newly created entity.
         *
         * @throw std::bad_alloc If allocation fails.
         *
         * @return Valid handle to newly created entity.
         */
        [[nodiscard]] ObjectHandle create();

        /**
         * @brief Destroys entity in scene.
         * @details Destroys the entity inside the scene handled
         * by the specific handle.
         *
         * @param handle Handle to the entity to destroy.
         */
        void destroy(ObjectHandle const& handle) noexcept;

        /**
         * @brief Returns view to filtered.
         * @details Filters all storing objects and returns view
         * to the objects that has required components.
         *
         * @throw std::bad_alloc If allocation fails.
         *
         * @return View to filtered objects.
         */
        template <class... Types>
            requires (sizeof...(Types) > 0)
        [[nodiscard]] auto filtered() const {
            return myRegistry->view<std::add_const_t<Types>...>();
        }

        /// @copydoc filtered() const
        template <class... Types>
            requires (sizeof...(Types) > 0)
        [[nodiscard]] auto filtered() {
            return myRegistry->view<Types...>();
        }

    private:
        std::shared_ptr<entt::registry> myRegistry;
    };
}

#endif
