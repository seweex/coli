#ifndef COLI_GAME_COMPONENTS_LAYER_H
#define COLI_GAME_COMPONENTS_LAYER_H

#include "coli/utility.h"

namespace Coli::Game::Components
{
    /**
     * @brief A component class that provides objects sorting by its layer value
     *
     * @warning This class is not thread-safe
     */
    class COLI_EXPORT Layer final
    {
    public:
        Layer() noexcept;
        Layer(long long layer) noexcept;

        Layer(const Layer&) noexcept;
        Layer(Layer&&) noexcept;

        Layer& operator=(const Layer&) noexcept;
        Layer& operator=(Layer&&) noexcept;

        ~Layer() noexcept;

        /**
         * @brief Gets the layer value
         *
         * @return The layer value
         */
        [[nodiscard]] long long layer() const;

        /**
         * @brief Sets the layer value
         *
         * @param newValue The new layer value
         */
        void layer(long long newValue);

    private:
        long long myValue;
    };
}

#endif
