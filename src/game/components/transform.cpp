#include "coli/game/components/transform.h"

namespace Coli::Game::Components
{
    template class COLI_EXPORT BasicTransform<false>;
    template class COLI_EXPORT BasicTransform<true>;

    template <bool Is2D>
    BasicTransform<Is2D>::BasicTransform() noexcept :
        position (static_cast<Types::float_type>(0)),
        scale    (static_cast<Types::float_type>(1)),
        rotation (Types::make_zero_rotation<Is2D>())
    {}

    template <bool Is2D>
    void BasicTransform<Is2D>::reset() noexcept {
        *this = BasicTransform {};
    }

    template <bool Is2D>
    BasicTransform<Is2D>::BasicTransform(const BasicTransform&) noexcept = default;

    template <bool Is2D>
    BasicTransform<Is2D>::BasicTransform(BasicTransform&&) noexcept = default;

    template <bool Is2D>
    BasicTransform<Is2D>& BasicTransform<Is2D>::operator=(const BasicTransform&) noexcept = default;

    template <bool Is2D>
    BasicTransform<Is2D>& BasicTransform<Is2D>::operator=(BasicTransform&&) noexcept = default;

    template <bool Is2D>
    bool BasicTransform<Is2D>::operator==(const BasicTransform& other) const noexcept = default;

    template <bool Is2D>
    bool BasicTransform<Is2D>::operator!=(const BasicTransform& other) const noexcept = default;
}