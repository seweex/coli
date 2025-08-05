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
#include <thread>
#include <mutex>
#include <bit>

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

    class COLI_EXPORT HashMixer final
    {
    public:
        [[nodiscard]] size_t operator()(size_t h1, size_t h2) const noexcept;
    };
}

namespace std
{
    template <glm::length_t L, class T, glm::qualifier Q>
    struct hash<glm::vec<L, T, Q>> final
    {
        [[nodiscard]] size_t operator()(const glm::vec<L, T, Q>& v) const noexcept
        {
            Coli::HashMixer const mixer;
            size_t hash;

            for (glm::length_t i = 0; i < L; ++i)
                hash = mixer(hash, std::hash<T>()(v[i]));

            return hash;
        }
    };

    template <class T, glm::qualifier Q>
    struct hash<glm::qua<T, Q>> final
    {
        [[nodiscard]] size_t operator()(const glm::qua<T, Q>& v) const noexcept {
            return std::hash<glm::vec<4, T, Q>>()(glm::vec<4, T, Q>{ v.x, v.y, v.z, v.w });
        }
    };

    template <glm::length_t C, glm::length_t R, class T, glm::qualifier Q>
    struct hash<glm::mat<C, R, T, Q>> final
    {
        [[nodiscard]] size_t operator()(const glm::mat<C, R, T, Q>& v) const noexcept
        {
            Coli::HashMixer const mixer;
            size_t hash;

            for (glm::length_t i = 0; i < C; ++i)
                hash = mixer(hash, std::hash<std::remove_cvref_t<decltype(v[i])>>()(v[i]));

            return hash;
        }
    };
}

#endif