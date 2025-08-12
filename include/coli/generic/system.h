#ifndef COLI_GENERIC_SYSTEM_H
#define COLI_GENERIC_SYSTEM_H

#include "coli/game/object.h"

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
    class COLI_EXPORT SystemBase
    {
    protected:
        /**
         * @detail Creates system base.
         * @details Default constructor. Call it from your
         * derived classes.
         */
        SystemBase() noexcept;

    public:
        /**
         * @detail Copies system base.
         * @details Has the default implementation.
         */
        SystemBase(SystemBase const&) noexcept;

        /**
         * @detail Moves system base.
         * @details Has the default implementation.
         */
        SystemBase(SystemBase&&) noexcept;

        /**
         * @detail Copies system base.
         * @details Has the default implementation.
         */
        SystemBase& operator=(SystemBase const&) noexcept;

        /**
         * @detail Moves system base.
         * @details Has the default implementation.
         */
        SystemBase& operator=(SystemBase&&) noexcept;

        /**
         * @detail Destroys system base.
         * @details Has the default implementation.
         */
        virtual ~SystemBase() noexcept;

        /**
         * @brief Processing method.
         * @detail Virtual processing method. Override this method to
         * specify your system's behavior.
         *
         * @param object Sorted objects storing in the processed scene.
         */
        virtual void operator()(std::vector<Game::ObjectHandle>& object) = 0;
    };
}

#endif
