#include "coli/utility.h"

namespace Coli::inline Types
{
    template <bool Is2D>
    rotator_type<Is2D> make_zero_rotation() noexcept
    {
        if constexpr (Is2D)
            return static_cast<rotator_type<true>>(0);
        else
            return glm::qua<float_type>::wxyz(
                static_cast<float_type>(1),
                static_cast<float_type>(0),
                static_cast<float_type>(0),
                static_cast<float_type>(0)
            );
    }

    template COLI_EXPORT rotator_type<false> make_zero_rotation<false>() noexcept;
    template COLI_EXPORT rotator_type<true> make_zero_rotation<true>() noexcept;
}