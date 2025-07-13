#ifndef COLI_GAME_SCENE_H
#define COLI_GAME_SCENE_H

#include "coli/utility.h"
#include "coli/game/object.h"

namespace Coli::Game
{
    /// @warning This namespace and all contained there should not be used by the user
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
     * @brief The class of scene for objects containing
     *
     * @warning This class is not thread-safe
     */
    class COLI_EXPORT Scene final
    {
    public:
        /// @throw std::bad_alloc If allocation fails
        Scene();

        Scene(Scene&&) noexcept;
        Scene(Scene const&) = delete;

        Scene& operator=(Scene &&) noexcept;
        Scene& operator=(Scene const&) = delete;

        ~Scene() noexcept;

        /**
         * @brief Creates an object and returns it handle
         *
         * @throw std::bad_alloc If allocation fails
         *
         * @return A handle to newly created object
         */
        [[nodiscard]] ObjectHandle create();

        /**
         * @brief Destroys the object
         *
         * @param handle A handle to object
         *
         * @note If an invalid handle passed the method does nothing
         */
        void destroy(ObjectHandle handle) noexcept;

        /**
         * @brief Returns all objects ordered by layer value.
         * Objects that don't have a layer value will be placed at the end.
         *
         * @throw std::bad_alloc If allocation fails
         *
         * @return A buffer of ordered objects
         *
         * @warning The returned buffer is valid until the next call of this method
         *      with any T param and any const method modifier
         */
        [[nodiscard]] std::vector<ObjectHandle> const& ordered() const;

        /// @copydoc ordered() const
        [[nodiscard]] std::vector<ObjectHandle>& ordered();

        /**
         * @brief Returns all objects that have a component of the type T
         *
         * @tparam T A type of component the object should have
         *
         * @throw std::bad_alloc If allocation fails
         *
         * @return A buffer of filtered objects
         *
         * @warning The returned buffer is valid until the next call of this method
         *      with any T param and any const method modifier
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
