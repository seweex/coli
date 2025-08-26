#ifndef COLI_GENERIC_SYSTEM_H
#define COLI_GENERIC_SYSTEM_H

#include "coli/game/object.h"
#include "coli/game/scene.h"

/**
 * @brief For internal details.
 * @note The user should not use this namespace.
 */
namespace Coli::Generic::Detail
{
    class COLI_EXPORT SystemBase
    {
    protected:
        SystemBase() noexcept;

    public:
        SystemBase(SystemBase&&) noexcept;
        SystemBase(const SystemBase&) noexcept;

        SystemBase& operator=(SystemBase&&) noexcept;
        SystemBase& operator=(const SystemBase&) noexcept;

        virtual ~SystemBase() noexcept;

        virtual void execute(Game::Scene& scene) = 0;
    };
}

/// @brief Namespace for the all generic for game engines stuff.
namespace Coli::Generic
{
    /**
     * @brief Base for systems.
     * @details The basic interface of the systems type.
     * Systems used in game engines to process the objects.
     *
     * @note Not thread-safe. Concurrent access requires
     * external synchronization.
     *
     * @note Interface base. No direct instances are allowed.
    */
    template <class... ComponentTys>
        requires (sizeof...(ComponentTys) > 0)
    class COLI_EXPORT SystemBase :
        public Detail::SystemBase
    {
    protected:
        /**
         * @detail Creates system base.
         * @details Default constructor. Call it from your
         * derived classes.
         */
        SystemBase() noexcept = default;

    public:
        /**
         * @detail Copies system base.
         * @details Has the default implementation.
         */
        SystemBase(SystemBase const&) noexcept = default;

        /**
         * @detail Moves system base.
         * @details Has the default implementation.
         */
        SystemBase(SystemBase&&) noexcept = default;

        /**
         * @detail Copies system base.
         * @details Has the default implementation.
         */
        SystemBase& operator=(SystemBase const&) noexcept = default;

        /**
         * @detail Moves system base.
         * @details Has the default implementation.
         */
        SystemBase& operator=(SystemBase&&) noexcept = default;

        /**
         * @detail Destroys system base.
         * @details Has the default implementation.
         */
        ~SystemBase() noexcept override = default;

        /**
         * @brief Processes components in one object.
         * @details Processes the user-required components in an object.
         * It calls for each object that has all of required components many
         * times per frame.
         *
         * @param components Required components to process.
         */
        virtual void process(ComponentTys&... components) = 0;

        /**
         * @brief Updates system.
         * @details Updates the system. Calls once per frame after
         * processing all objects.
         */
        virtual void update() = 0;

        /**
         * @brief Executes the system.
         * @note The user should not use this method.
         */
        void execute(Game::Scene& scene) override
        {
            auto objects = scene.filtered<ComponentTys...>();

            objects.each([&] (auto&... components) {
                this->process(components...);
            });

            this->update();
        }
    };
}

#endif
