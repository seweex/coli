#ifndef COLI_GEOMETRY_VERTEX_H
#define COLI_GEOMETRY_VERTEX_H

#include "coli/utility.h"

/// @brief Namespace for the all geometry stuff.
namespace Coli::Geometry
{
    /**
     * @brief Vertex struct.
     * @details Represents vertex for mesh.
     *
     * @note Not thread-safe. Concurrent access requires
     * external synchronization.
    */
    template <bool Is2D>
    struct Vertex;

    /**
     * @brief 2D vertex struct.
     * @details Represents 2D vertex for mesh. Contains only a 2D
     * position and 2D texture coords.
     *
     * @note Not thread-safe. Concurrent access requires
     * external synchronization.
    */
    template <>
    struct COLI_EXPORT Vertex<true> final
    {
        /**
         * @brief Position type.
         * @detail The used world position type.
         * It is always a glm vector of 2 float_type components.
         */
        using position_type = Types::vector_type<true>;

        /**
         * @brief Texture coords type.
         * @detail The used texture coords type.
         * It is always a glm vector of 2 float_type components.
         */
        using texcoord_type = Types::vector_type<true>;

        Vertex() = delete;

        /**
         * @brief Creates 2D vertex.
         * @detail Creates a 2D vertex and sets up its properties
         * to user defined values.
         *
         * @param position 2D world position;
         * @param texcoord 2D texture coords.
         */
        Vertex(position_type const& position, texcoord_type const& texcoord) noexcept;

        /**
         * @brief Copies 2D vertex.
         * @details Just copies the 2D vertex.
         *
         * @param other Other 2D vertex.
         */
        Vertex(Vertex const& other) noexcept;

        /**
         * @brief Copies 2D vertex.
         * @details Just moves the 2D vertex.
         * The other vertex will have no changes.
         *
         * @param other Other 2D vertex.
         */
        Vertex(Vertex&& other) noexcept;

        /// @copydoc Vertex(Vertex const&)
        Vertex& operator=(Vertex const&) noexcept;

        /// @copydoc Vertex(Vertex&&)
        Vertex& operator=(Vertex&&) noexcept;

        /**
         * @brief Compares two 2D vertices.
         * @details Compares the properties of two 2D vertices.
         *
         * @param other Other vertex to compare with.
         *
         * @return Compare result of two 2D vertices.
         *
         * @retval True If the vertices are equal;
         * @retval False Otherwise.
         */
        [[nodiscard]] bool operator==(Vertex const& other) const noexcept;

        /**
         * @brief Compares two 2D vertices.
         * @details Compares the properties of two 2D vertices.
         *
         * @param other Other vertex to compare with.
         *
         * @return Compare result of two 2D vertices.
         *
         * @retval False If the vertices are equal;
         * @retval True Otherwise.
         */
        [[nodiscard]] bool operator!=(Vertex const& other) const noexcept;

        /**
         * @brief World position.
         * @detail World position component.
         */
        position_type position;

        /**
         * @brief Texture coords.
         * @detail Texture coords component.
         */
        texcoord_type texcoord;
    };

    /**
     * @brief 3D vertex struct.
     * @details Represents 3D vertex for mesh. Contains only a 3D
     * position, 3D normals, and 2D texture coords.
     *
     * @note Not thread-safe. Concurrent access requires
     * external synchronization.
    */
    template <>
    struct COLI_EXPORT Vertex<false> final
    {
        /**
         * @brief Position type.
         * @detail The used world position type.
         * It is always a glm vector of 3 float_type components.
         */
        using position_type = Types::vector_type<true>;

        /**
         * @brief Normals type.
         * @detail The used normals type.
         * It is always a glm vector of 3 float_type components.
         */
        using normal_type = Types::vector_type<false>;

        /**
         * @brief Texture coords type.
         * @detail The used texture coords type.
         * It is always a glm vector of 2 float_type components.
         */
        using texcoord_type = Types::vector_type<true>;

        Vertex() = delete;

        /**
         * @brief Creates 3D vertex.
         * @detail Creates a 3D vertex and sets up its properties
         * to user defined values.
         *
         * @param position 3D world position;
         * @param normal 3D normals;
         * @param texcoord 2D texture coords.
         */
        Vertex(
            position_type const& position,
            normal_type const& normal,
            texcoord_type const& texcoord
        ) noexcept;

        /**
          * @brief Copies 3D vertex.
          * @details Just copies the 3D vertex.
          *
          * @param other Other 3D vertex.
          */
        Vertex(Vertex const& other) noexcept;

        /**
         * @brief Copies 3D vertex.
         * @details Just moves the 3D vertex.
         * The other vertex will have no changes.
         *
         * @param other Other 3D vertex.
         */
        Vertex(Vertex&& other) noexcept;

        /// @copydoc Vertex(Vertex const&)
        Vertex& operator=(Vertex const&) noexcept;

        /// @copydoc Vertex(Vertex&&)
        Vertex& operator=(Vertex&&) noexcept;

        /**
         * @brief Compares two 3D vertices.
         * @details Compares the properties of two 3D vertices.
         *
         * @param other Other vertex to compare with.
         *
         * @return Compare result of two 3D vertices.
         *
         * @retval True If the vertices are equal;
         * @retval False Otherwise.
         */
        [[nodiscard]] bool operator==(Vertex const& other) const noexcept;

        /**
         * @brief Compares two 3D vertices.
         * @details Compares the properties of two 3D vertices.
         *
         * @param other Other vertex to compare with.
         *
         * @return Compare result of two 3D vertices.
         *
         * @retval False If the vertices are equal;
         * @retval True Otherwise.
         */
        [[nodiscard]] bool operator!=(Vertex const& other) const noexcept;

        /**
         * @brief World position.
         * @detail World position component.
         */
        position_type position;

        /**
         * @brief Normals.
         * @detail Normals component.
         */
        normal_type normal;

        /**
         * @brief Texture coords.
         * @detail Texture coords component.
         */
        texcoord_type texcoord;
    };

#if COLI_BUILD
#else
    extern template struct COLI_EXPORT Vertex<false>;
    extern template struct COLI_EXPORT Vertex<true>;
#endif

    /// @copydoc Vertex<true>
    using Vertex2D = Vertex<true>;

    /// @copydoc Vertex<false>
    using Vertex3D = Vertex<false>;
};

/**
 * @brief For internal details.
 * @note The user should not use this namespace.
 */
namespace Coli::Geometry::Detail
{
    template <class Ty>
    concept VertexType = requires (Ty const& val)
    {
        val.position;
        val.texcoord;
    };
}

/// @brief Namespace for the all geometry stuff.
namespace Coli::Geometry
{
    /**
     * @brief Vertices traits.
     * @details Provides all properties of the specific
     * vertex type.
     *
     * @tparam VertexTy Type of vertex. Must have at least 'position'
     * and 'texcoord' fields.
     *
     * @note Not thread-safe. Concurrent access requires
     * external synchronization.
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
        /**
         * @brief Vertex type.
         * @details Provided vertex type.
         */
        using vertex_type = VertexTy;

        /**
         * @brief Vertex position type.
         * @details Type of world position used in the vertex.
         */
        using position_type = decltype(std::declval<vertex_type>().position);

        /**
         * @brief Texture coords type.
         * @details Type of texture coords used in the vertex.
         */
        using texcoord_type = decltype(std::declval<vertex_type>().texcoord);

        /**
         * @brief Vertex position components type.
         * @details Scalar type of components used in world position of the vertex.
         */
        using position_float_type = decltype(std::declval<position_type>().x);

        /**
         * @brief Vertex texture coords components type.
         * @details Scalar type of components used in texture coords of the vertex.
         */
        using texcoord_float_type = decltype(std::declval<texcoord_type>().x);

        static_assert(std::is_floating_point_v<position_float_type>,
            "position must be a floating point vector");

        static_assert(std::is_floating_point_v<texcoord_float_type>,
            "texcoord must be a floating point vector");

        /**
         * @brief Typed boolean flag of normal presence.
         * @details Checks normals presence in the vertex.
         * It is 'std::true_type' if the vertex provides normals,
         * 'std::false_type' otherwise.
         */
        using has_normal = std::conditional_t<
            requires (vertex_type vertex) { vertex.normal; }, std::true_type, std::false_type>;

        /**
         * @brief Vertex normals type.
         * @detail Type of normals used in the vertex.
         * If the vertex does not provide normals, it's a typedef of void.
         */
        using normal_type = typename normal_properties<vertex_type, has_normal::value>::vector_type;

        /**
         * @brief Vertex normals components type.
         * @detail Scalar type of components used in normals of the vertex.
         * If the vertex does not provide normals, it's a typedef of void.
         */
        using normal_float_type = typename normal_properties<vertex_type, has_normal::value>::scalar_type;

        /**
         * @brief Returns position offset.
         * @detail Returns the offset of world position in the vertex, in bytes.
         *
         * @return Offset of position field in the vertex.
         */
        [[nodiscard]] static constexpr size_t position_offset() noexcept {
            return offsetof(vertex_type, position);
        }

        /**
         * @brief Returns texture coords offset.
         * @detail Returns the offset of texture coords in the vertex, in bytes.
         *
         * @return Offset of texture coords field in the vertex.
         */
        [[nodiscard]] static constexpr size_t texcoord_offset() noexcept {
            return offsetof(vertex_type, texcoord);
        }

        /**
         * @brief Returns normals offset.
         * @detail Returns the offset of normals in the vertex, in bytes.
         *
         * @return Offset of normals field in the vertex.
         *
         * @warning The traits provide this method only if the vertex
         * provides normals.
         */
        [[nodiscard]] static constexpr size_t normal_offset() noexcept requires (has_normal::value) {
            return offsetof(vertex_type, normal);
        }

        /**
         * @brief Returns position size.
         * @detail Returns the size of world position in the vertex, in bytes.
         *
         * @return Size of position field in the vertex.
         */
        [[nodiscard]] static constexpr size_t position_size() noexcept {
            return sizeof(position_type);
        }

        /**
         * @brief Returns texture coords size.
         * @detail Returns the size of texture coords in the vertex, in bytes.
         *
         * @return Size of texture coords field in the vertex.
         */
        [[nodiscard]] static constexpr size_t texcoord_size() noexcept {
            return sizeof(texcoord_type);
        }

        /**
         * @brief Returns normals size.
         * @detail Returns the size of normals in the vertex, in bytes.
         *
         * @return Size of normals field in the vertex.
         *
         * @warning The traits provide this method only if the vertex
         * provides normals.
         */
        [[nodiscard]] static constexpr size_t normal_size() noexcept requires (has_normal::value) {
            return sizeof(normal_type);
        }

        /**
         * @brief Returns position length.
         * @detail Returns the number of components in world position of the vertex.
         *
         * @return Number of position's components in the vertex.
         */
        [[nodiscard]] static constexpr size_t position_length() noexcept {
            return position_size() / sizeof(position_float_type);
        }

        /**
         * @brief Returns texture coords length.
         * @detail Returns the number of components in texture coords of the vertex.
         *
         * @return Number of texture coords components in the vertex.
         */
        [[nodiscard]] static constexpr size_t texcoord_length() noexcept {
            return texcoord_size() / sizeof(texcoord_float_type);
        }

        /**
         * @brief Returns normals length.
         * @detail Returns the number of components in normals of the vertex.
         *
         * @return Number of normals components in the vertex.
         *
         * @warning The traits provide this method only if the vertex
         * provides normals.
         */
        [[nodiscard]] static constexpr size_t normal_length() noexcept requires (has_normal::value) {
            return normal_size() / sizeof(normal_float_type);
        }

        /**
         * @drief Returns vertex size.
         * @details Returns the size of the vertex, in bytes.
         *
         * @return Size of the vertex.
         */
        [[nodiscard]] static constexpr size_t stride() noexcept {
            return sizeof(vertex_type);
        }
    };
}


/// @brief Namespace for the library utilities stuff.
namespace Coli::Utility
{
    /**
     * @brief Hash func for vertices.
     * @details Provides hashing for the builtin vertex types.
     *
     * @tparam Is2D Dimensions flag of the builtin vertex type.
     */
    template <bool Is2D>
    class COLI_EXPORT Hash<Geometry::Vertex<Is2D>> final
    {
    public:
        /**
         * @brief Hash func for vertices.
         * @details Returns hash of the vertex.
         *
         * @param vertex Vertex to get hash of.
         *
         * @return Hash of the vertex.
         */
        [[nodiscard]] size_t operator()(Geometry::Vertex<Is2D> const& vertex) const noexcept;
    };

#if COLI_BUILD
#else
    extern template class COLI_EXPORT Hash<Geometry::Vertex<false>>;
    extern template class COLI_EXPORT Hash<Geometry::Vertex<true>>;
#endif
}

#endif
