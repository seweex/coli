#ifndef COLI_GEOMETRY_VERTEX_H
#define COLI_GEOMETRY_VERTEX_H

#include "coli/utility.h"

namespace Coli::Geometry
{
    /**
     * @brief A simple vertex struct
     *
     * @tparam Is2D Set to true if the vertex is in two dimensions mode, false otherwise
     */
    template <bool Is2D>
    struct Vertex;

    /// @copydoc Vertex
    template <>
    struct COLI_EXPORT Vertex<true> final
    {
        /// @brief The type of the used position
        using position_type = Types::vector_type<true>;

        /// @brief The type of the used texture coordinates
        using texcoord_type = Types::vector_type<true>;

        Vertex() = delete;

        /**
         * @brief Creates a 2D vertex
         *
         * @param position The vertex position
         * @param texcoord The vertex texture coordinates
         */
        Vertex(position_type const& position, texcoord_type const& texcoord) noexcept;

        Vertex(Vertex const&) noexcept;
        Vertex(Vertex&&) noexcept;

        Vertex& operator=(Vertex const&) noexcept;
        Vertex& operator=(Vertex&&) noexcept;

        [[nodiscard]] bool operator==(Vertex const&) const noexcept = default;
        [[nodiscard]] bool operator!=(Vertex const&) const noexcept = default;

        position_type position;
        texcoord_type texcoord;
    };

    /// @copydoc Vertex
    template <>
    struct COLI_EXPORT Vertex<false> final
    {
        /// @brief The type of the used position
        using position_type = Types::vector_type<false>;

        /// @brief The type of the used normal
        using normal_type = Types::vector_type<false>;

        /// @brief The type of the used texture coordinates
        using texcoord_type = Types::vector_type<true>;

        Vertex() = delete;

        /**
         * @brief Creates a 2D vertex
         *
         * @param position The vertex position
         * @param normal The vertex normal
         * @param texcoord The vertex texture coordinates
         */
        Vertex(
            position_type const& position,
            normal_type const& normal,
            texcoord_type const& texcoord
        ) noexcept;

        Vertex(Vertex const&) noexcept;
        Vertex(Vertex&&) noexcept;

        Vertex& operator=(Vertex const&) noexcept;
        Vertex& operator=(Vertex&&) noexcept;

        [[nodiscard]] bool operator==(Vertex const&) const noexcept = default;
        [[nodiscard]] bool operator!=(Vertex const&) const noexcept = default;

        position_type position;
        normal_type normal;
        texcoord_type texcoord;
    };

#if COLI_BUILD
#else
    extern template struct COLI_EXPORT Vertex<false>;
    extern template struct COLI_EXPORT Vertex<true>;
#endif

    using Vertex2D = Vertex<true>;
    using Vertex3D = Vertex<false>;
};

namespace Coli::Geometry::Detail
{
    template <class Ty>
    concept VertexType = requires (Ty const& val)
    {
        val.position;
        val.texcoord;
    };
}

namespace Coli::Geometry
{
    /**
     * @brief The traits class for retrieving the vertex properties
     *
     * @tparam VertexTy The vertex type
     */
    template <Detail::VertexType VertexTy>
    class VertexTraits final
    {
        template <class VertexTy2, bool Exists>
        struct normal_properties final {
            using vector_type = void;
            using scalar_type = void;
        };

        template <class VertexTy2>
        struct normal_properties <VertexTy2, true> final {
            using vector_type = decltype(std::declval<VertexTy2>().normal);
            using scalar_type = decltype(std::declval<vector_type>().x);
        };

    public:
        using vertex_type = VertexTy;

        using position_type = decltype(std::declval<vertex_type>().position);
        using texcoord_type = decltype(std::declval<vertex_type>().texcoord);

        using position_float_type = decltype(std::declval<position_type>().x);
        using texcoord_float_type = decltype(std::declval<texcoord_type>().x);

        static_assert(std::is_floating_point_v<position_float_type>,
            "position must be a floating point vector");

        static_assert(std::is_floating_point_v<texcoord_float_type>,
            "texcoord must be a floating point vector");

        using has_normal = std::conditional_t<
            requires (vertex_type vertex) { vertex.normal; }, std::true_type, std::false_type>;

        using normal_type = typename normal_properties<vertex_type, has_normal::value>::vector_type;
        using normal_float_type = typename normal_properties<vertex_type, has_normal::value>::scalar_type;

        [[nodiscard]] static constexpr size_t position_offset() noexcept {
            return offsetof(vertex_type, position);
        }

        [[nodiscard]] static constexpr size_t texcoord_offset() noexcept {
            return offsetof(vertex_type, texcoord);
        }

        [[nodiscard]] static constexpr size_t normal_offset() noexcept requires (has_normal::value) {
            return offsetof(vertex_type, normal);
        }

        [[nodiscard]] static constexpr size_t position_size() noexcept {
            return sizeof(position_type);
        }

        [[nodiscard]] static constexpr size_t texcoord_size() noexcept {
            return sizeof(texcoord_type);
        }

        [[nodiscard]] static constexpr size_t normal_size() noexcept requires (has_normal::value) {
            return sizeof(normal_type);
        }

        [[nodiscard]] static constexpr size_t position_length() noexcept {
            return position_size() / sizeof(position_float_type);
        }

        [[nodiscard]] static constexpr size_t texcoord_length() noexcept {
            return texcoord_size() / sizeof(texcoord_float_type);
        }

        [[nodiscard]] static constexpr size_t normal_length() noexcept requires (has_normal::value) {
            return normal_size() / sizeof(normal_float_type);
        }

        [[nodiscard]] static constexpr size_t stride() noexcept {
            return sizeof(vertex_type);
        }
    };
}

namespace std
{
    template <bool Is2D>
    struct COLI_EXPORT std::hash <Coli::Geometry::Vertex<Is2D>> final {
        [[nodiscard]] size_t operator()(Coli::Geometry::Vertex<Is2D> const& vertex) const noexcept;
    };

#if COLI_BUILD
#else
    extern template struct COLI_EXPORT hash<Coli::Geometry::Vertex<false>>;
    extern template struct COLI_EXPORT hash<Coli::Geometry::Vertex<true>>;
#endif
}

#endif
