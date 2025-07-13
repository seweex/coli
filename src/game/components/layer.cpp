#include "coli/game/components/layer.h"

namespace Coli::Game::Components
{
    /* Layer */

    Layer::Layer(const Layer&) noexcept = default;
    Layer::Layer(Layer&&) noexcept = default;

    Layer& Layer::operator=(const Layer&) noexcept = default;
    Layer& Layer::operator=(Layer&&) noexcept = default;

    Layer::~Layer() noexcept = default;

    Layer::Layer() noexcept:
        myValue (0)
    {}

    Layer::Layer(long long layer) noexcept :
        myValue (layer)
    {}

    long long Layer::layer() const {
        return myValue;
    }

    void Layer::layer(long long newValue) {
        myValue = newValue;
    }
}