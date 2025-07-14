#ifndef COLI_COMPONENTS_TRANSFORM_H
#define COLI_COMPONENTS_TRANSFORM_H

#include "coli/utility.h"

namespace Coli::Game::Components
{
    template <bool Is2D>
    class BasicTransform final
    {
    public:
        /// @brief Default constructor, zero transformation
        BasicTransform() noexcept;

        BasicTransform(const BasicTransform&) noexcept;
        BasicTransform(BasicTransform&&) noexcept;

        BasicTransform& operator=(const BasicTransform&) noexcept;
        BasicTransform& operator=(BasicTransform&&) noexcept;

        /// @brief Sets the transform to the zero state
        void reset() noexcept;

        /**
         * @brief Checks are the transforms equals
         *
         * @param other Other transform to compare with
         *
         * @return True if the transforms are equals or false otherwise
         */
        [[nodiscard]] bool operator==(const BasicTransform& other) const noexcept;

        /**
         * @brief Checks are the transforms not equals
         *
         * @param other Other transform to compare with
         *
         * @return True if the transforms are not equals or false otherwise
         */
        [[nodiscard]] bool operator!=(const BasicTransform& other) const noexcept;

        Types::vector_type<Is2D> position;
        Types::vector_type<Is2D> scale;
        Types::rotator_type<Is2D> rotation;
    };

    using Transform3D = BasicTransform<false>;
    using Transform2D = BasicTransform<true>;

#if COLI_BUILD
#else
    extern template class COLI_EXPORT BasicTransform<false>;
    extern template class COLI_EXPORT BasicTransform<true>;
#endif
}

#endif
