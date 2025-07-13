#ifndef COLI_GENERIC_SYSTEM_H
#define COLI_GENERIC_SYSTEM_H

#include "coli/game/object.h"

namespace Coli::Generic
{
    /**
     * @brief Base class for all systems handling objects
     *
     * @warning This class is not thread-safe
     * @note This is a base class
     */
    class COLI_EXPORT SystemBase
    {
    protected:
        SystemBase() noexcept;

    public:
        SystemBase(SystemBase const&) noexcept;
        SystemBase(SystemBase&&) noexcept;

        SystemBase& operator=(SystemBase const&) noexcept;
        SystemBase& operator=(SystemBase&&) noexcept;

        virtual ~SystemBase() noexcept;

        /**
         * @brief Virtual method that specifying the behavior of objects
         *
         * @param object The object that is currently being processing
         */
        virtual void operator()(std::vector<Game::ObjectHandle>& object) = 0;
    };
}

#endif
