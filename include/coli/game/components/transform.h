#ifndef COLI_COMPONENTS_TRANSFORM_H
#define COLI_COMPONENTS_TRANSFORM_H

#include "coli/utility.h"

namespace Coli::Game::Components
{
    /**
     * @brief Transform component class.
     *
     * @details Stores object's transformations:
     * - Position;
     * - Rotation;
     * - Scale.
     *
     * @tparam Is2D Use 2 dimensions flag. True for 2D mode,
     * false for 3D.
     *
     * @note Not thread-safe. Concurrent access requires
     * external synchronization.
     */
    template <bool Is2D>
    class BasicTransform final
    {
    public:
        /**
         * @brief Creates zero transformations.
         * @details Creates a transform component and sets all
         * values to zeroes.
         */
        BasicTransform() noexcept;

        /**
         * @brief Copies the transform component.
         * @details Creates a transform component and sets all
         * values to the other's ones.
         *
         * @param other Other transform component.
         */
        BasicTransform(const BasicTransform& other) noexcept;

        /**
         * @brief Moves the transform component.
         * @details Makes a copy of other and resets all other's values.
         *
         * @param other Other transform component.
         */
        BasicTransform(BasicTransform&& other) noexcept;

        /**
         * @brief Copies the transform component's values.
         * @details Sets the all transform component's values
         * equal to other's ones.
         *
         * @param other Other transform component.
         */
        BasicTransform& operator=(const BasicTransform& other) noexcept;

        /**
         * @brief Moves the transform component's values.
         * @details Sets the all transform component's values
         * equal to other's ones.
         *
         * @param other Other transform component.
         */
        BasicTransform& operator=(BasicTransform&& other) noexcept;

        /**
         * @brief Resets the transform to zeroes.
         * @details Sets up all component's values to the zeroes.
         * The new transformation has no effect.
         */
        void reset() noexcept;

        /**
         * @brief Compares two transforms.
         * @details Compares the values of two transform components.
         *
         * @param other Component to compare.
         *
         * @return Compare result of two transforms.
         *
         * @retval True If the transforms are equal;
         * @retval False Otherwise.
         */
        [[nodiscard]] bool operator==(const BasicTransform& other) const noexcept;

        /**
         * @brief Compares two transforms.
         * @details Compares the values of two transform components.
         *
         * @param other Component to compare.
         *
         * @return Compare result of two transforms.
         *
         * @retval False If the transforms are equal;
         * @retval True Otherwise.
         */
        [[nodiscard]] bool operator!=(const BasicTransform& other) const noexcept;

        /// @brief Position value in the space
        Types::vector_type<Is2D> position;

        /// @brief Scale value
        Types::vector_type<Is2D> scale;

        /// @brief Rotation value in the space
        Types::rotator_type<Is2D> rotation;
    };

    /// @brief Type definition for 3D transformation
    using Transform3D = BasicTransform<false>;

    /// @brief Type definition for 2D transformation
    using Transform2D = BasicTransform<true>;

#if COLI_BUILD
#else
    extern template class COLI_EXPORT BasicTransform<false>;
    extern template class COLI_EXPORT BasicTransform<true>;
#endif
}

#endif
