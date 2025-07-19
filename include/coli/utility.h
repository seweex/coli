#ifndef COLI_UTILITY_H
#define COLI_UTILITY_H

#include "coli/version.h"

#if COLI_DYNAMIC && _WIN32
    #if COLI_BUILD
        #define COLI_EXPORT __declspec(dllexport)
    #else
        #define COLI_EXPORT __declspec(dllimport)
    #endif
#else
    #define COLI_EXPORT
#endif

#include <memory>
#include <vector>
#include <concepts>
#include <algorithm>
#include <typeindex>
#include <string>
#include <string_view>

#define GLM_ENABLE_EXPERIMENTAL

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Coli::inline Types
{
#if COLI_FORCE_SINGLE_FLOAT
    using float_type = float;
#else
    using float_type = double;
#endif

    template <bool Is2D>
    using vector_type = glm::vec<Is2D ? 2 : 3, float_type>;

    template <bool Is2D>
    using rotator_type = std::conditional_t<Is2D, float_type, glm::qua<float_type>>;

    template <bool Is2D>
    [[nodiscard]] COLI_EXPORT rotator_type<Is2D> make_zero_rotation() noexcept;

#if COLI_BUILD
#else
    extern template COLI_EXPORT rotator_type<false> make_zero_rotation<false>() noexcept;
    extern template COLI_EXPORT rotator_type<true> make_zero_rotation<true>() noexcept;
#endif
}

#endif