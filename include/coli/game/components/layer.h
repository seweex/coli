#ifndef COLI_GAME_COMPONENTS_LAYER_H
#define COLI_GAME_COMPONENTS_LAYER_H

#include "coli/utility.h"

namespace Coli::Game::Components
{
    /**
     * @brief Layer component class.
     *
     * @details Stores the object's layer value for sorting in
     * the scene. Systems often processes the objects in the
     * certain order. Objects with lower layer values are processed
     * firstly.
     *
     * @note Not thread-safe. Concurrent access requires
     * external synchronization.
     */
    class COLI_EXPORT Layer final
    {
    public:
        /**
         * @brief Creates a zero layer.
         * @details Creates a layer component and sets the layer value to 0.
         */
        Layer() noexcept;

        /**
         * @brief Creates a certain layer.
         * @details Creates a layer component and sets the layer value to
         * the user's value.
         *
         * @param layer New layer value. Lower values processes firstly.
         */
        Layer(long long layer) noexcept;

        /**
         * @brief Copies the layer component.
         * @details Creates a layer component and sets the layer value to
         * the other's layer component value.
         *
         * @param other Other layer component.
         */
        Layer(const Layer& other) noexcept;

        /**
         * @brief Moves the layer component.
         * @details Creates a layer component, sets the new layer value to
         * the other's layer component value, and resets other's layer.
         *
         * @param other Other layer component.
         */
        Layer(Layer&& other) noexcept;

        /**
         * @brief Sets the layer value.
         * @details Sets the layer value to the other's layer component
         * value.
         *
         * @param other Other layer component.
         */
        Layer& operator=(const Layer& other) noexcept;

        /**
         * @brief Moves the layer value.
         * @details Sets the layer value to the other's layer component
         * value and resets other's layer.
         *
         * @param other Other layer component.
         */
        Layer& operator=(Layer&& other) noexcept;

        /// @brief Destroys the layer component.
        ~Layer() noexcept;

        /**
         * @brief Gets the layer value.
         * @details Returns the currently stored layer value.
         *
         * @return Layer value.
         */
        [[nodiscard]] long long layer() const;

        /**
         * @brief Sets the layer value.
         * @details Sets the currently stored layer value to
         * the new user's value.
         *
         * @param newValue New layer value. Lower values processes firstly.
         */
        void layer(long long newValue);

    private:
        long long myValue;
    };
}

#endif
