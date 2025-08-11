#ifndef COLI_GAME_SCENE_H
#define COLI_GAME_SCENE_H

#include "coli/utility.h"
#include "coli/game/object.h"

/// @brief Namespace for the all game-related stuff.
namespace Coli::Game
{
    /**
     * @brief For internal details.
     * @note The user should not use this namespace.
     */
    namespace Detail
    {
        class COLI_EXPORT ObjectsOrderer final
        {
        public:
            ObjectsOrderer() noexcept;

            ObjectsOrderer(ObjectsOrderer const&);
            ObjectsOrderer(ObjectsOrderer &&) noexcept;

            ObjectsOrderer& operator=(ObjectsOrderer const&);
            ObjectsOrderer& operator=(ObjectsOrderer &&) noexcept;

            ~ObjectsOrderer() noexcept;

            [[nodiscard]] std::vector<ObjectHandle>& operator()
            (std::shared_ptr<entt::registry> const& registry) const;

        private:
            std::vector<ObjectHandle> mutable myBuffer;
        };

        class COLI_EXPORT ObjectsFilter final
        {
        public:
            ObjectsFilter() noexcept;

            ObjectsFilter(ObjectsFilter const&);
            ObjectsFilter(ObjectsFilter &&) noexcept;

            ObjectsFilter& operator=(ObjectsFilter const&);
            ObjectsFilter& operator=(ObjectsFilter &&) noexcept;

            ~ObjectsFilter() noexcept;

            template <class T>
            [[nodiscard]] std::vector<ObjectHandle>& operator()
            (std::shared_ptr<entt::registry> const& registry) const
            {
                auto const& requested = registry->view<T>();

                myBuffer.clear();
                myBuffer.reserve(requested.size());

                for (auto const entity : requested)
                    myBuffer.emplace_back(registry, entity);

                return myBuffer;
            }

            template <class T>
            [[nodiscard]] std::vector<ObjectHandle>& operator()
            (std::vector<ObjectHandle> const& objects) const
            {
                myBuffer.clear();
                myBuffer.reserve(objects.size());

                for (auto const& object : objects)
                    if (object.contains<T>())
                        myBuffer.emplace_back(object);

                return myBuffer;
            }

        private:
            std::vector<ObjectHandle> mutable myBuffer;
        };
    }

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
        void destroy(ObjectHandle handle) noexcept;

        /**
         * @brief Returns ordered objects.
         * @details Orders all storing objects by its layer value
         * and returns the sorted handles. If object has no layer
         * it will be placed in the end.
         *
         * @throw std::bad_alloc If allocation fails.
         *
         * @return Ordered object's handles.
         *
         * @note Returned buffer is valid until the next call of this
         * method or its overloads.
         */
        [[nodiscard]] std::vector<ObjectHandle> const& ordered() const;

        /// @copydoc ordered() const
        [[nodiscard]] std::vector<ObjectHandle>& ordered();

        /**
         * @brief Returns filtered objects.
         * @details Filters all storing objects and returns handles to
         * the objects that have T component inside.
         *
         * @throw std::bad_alloc If allocation fails.
         *
         * @return Filtered object's handles.
         *
         * @note Returned buffer is valid until the next call of this
         * method or its overloads.
         */
        template <class T>
        [[nodiscard]] std::vector<ObjectHandle> const& filtered() const {
            return myFilter.operator()<T>(myRegistry);
        }

        /// @copydoc filtered() const
        template <class T>
        [[nodiscard]] std::vector<ObjectHandle>& filtered() {
            return myFilter.operator()<T>(myRegistry);
        }

    private:
        std::shared_ptr<entt::registry> myRegistry;

        Detail::ObjectsOrderer myOrderer;
        Detail::ObjectsFilter myFilter;
    };
}

#endif
