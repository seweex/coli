#ifndef COLI_GEOMETRY_MESH_H
#define COLI_GEOMETRY_MESH_H

#include "coli/geometry/vertex.h"

namespace Coli::Geometry
{
    /**
     * @brief Vertex mesh class.
     * @details Contains vertices that makes geometry mesh.
     *
     * @tparam VertexTy Vertex type. Must have a position and texcoord.
     *
     * @note Not thread-safe. Concurrent access requires
     * external synchronization.
    */
    template <Detail::VertexType VertexTy>
    class Mesh final
    {
    public:
        /**
         * @brief Makes mesh from vertices array.
         * @details Creates the mesh of unique vertices and indices from
         * a non-unique vertices sequence.
         *
         * @tparam Iter Input iterator type to a non-unique vertices sequence.
         *
         * @param first The beginning of the sequence;
         * @param last The end of the sequence.
         *
         * @throw std::bad_alloc If allocation fails.
         * @warning It's UB if passed invalid iterators.
         */
        template <std::input_iterator Iter>
            requires (std::same_as<VertexTy, std::remove_cvref_t<decltype(*std::declval<Iter>())>>)
        Mesh (Iter first, Iter last)
        {
            auto const size = std::distance(first, last);

            std::unordered_map<VertexTy, unsigned, Utility::Hash<VertexTy>> unique;
            unique.reserve(size);

            myVertices.reserve(size);
            myIndices.reserve(size);

            for (auto it = first; it != last; ++it)
            {
                auto [newIter, inserted] = unique.try_emplace(*it, unique.size());

                if (inserted)
                    myVertices.emplace_back(newIter->first);

                myIndices.emplace_back(newIter->second);
            }
        }

        /**
         * @brief Makes mesh from unique vertices and indices.
         * @details Copies the unique sequence of vertices and indices to the mesh.
         *
         * @tparam VertIter Input iterator type to a unique vertices sequence;
         * @tparam IdxIter Input iterator type to an index sequence.
         *
         * @param vertexFirst The beginning of the unique vertex sequence;
         * @param vertexLast The end of the unique vertex sequence;
         * @param indexFirst The beginning of the index sequence;
         * @param indexLast The end of the index sequence.
         */
        template <std::input_iterator VertIter, std::input_iterator IdxIter> requires (
            std::same_as<VertexTy, std::remove_cvref_t<decltype(*std::declval<VertIter>())>> &&
            std::same_as<unsigned, std::remove_cvref_t<decltype(*std::declval<IdxIter>())>>
        )
        Mesh (VertIter vertexFirst, VertIter vertexLast, IdxIter indexFirst, IdxIter indexLast) :
            myVertices (vertexFirst, vertexLast),
            myIndices (indexFirst, indexLast)
        {}

        /**
         * @brief Moves unique vertices and indices.
         * @details Moves a unique sequence of vertices and indices to the mesh.
         *
         * @param vertices R-value ref to a unique vertices vector;
         * @param indices R-value ref to an inices vector.
         */
        Mesh(std::vector<VertexTy>&& vertices, std::vector<unsigned>&& indices) :
            myVertices (std::move(vertices)),
            myIndices (std::move(indices))
        {}

        /**
         * @brief Copies mesh.
         * @details Just copies the mesh.
         *
         * @param other Other mesh.
         */
        Mesh(Mesh const& other) = default;

        /**
         * @brief Moves mesh.
         * @details Just moves the mesh.
         *
         * @param other Other mesh.
         */
        Mesh(Mesh&& other) noexcept = default;

        /// @copydoc Mesh(Mesh const&)
        Mesh& operator=(Mesh const&) = default;


        /// @copydoc Mesh(Mesh&&)
        Mesh& operator=(Mesh&&) noexcept = default;

        /**
         * @brief Destroys mesh.
         * @details Just destroys the mesh.
         */
        ~Mesh() noexcept = default;

        /**
         * @brief Returns vertices.
         * @details Returns the currently stored vertices in the mesh.
         * The returned span contains unique vertices.
         *
         * @return Span of unique vertices stored in the mesh.
         */
        [[nodiscard]] std::span<VertexTy const> get_vertices() const noexcept {
            return { myVertices.begin(), myVertices.size() };
        }

        /**
         * @brief Returns indices.
         * @details Returns the currently stored indices in the mesh.
         *
         * @return Span of indices stored in the mesh.
         */
        [[nodiscard]] std::span<unsigned const> get_indices() const noexcept {
            return { myIndices.begin(), myIndices.size() };
        }

        /**
         * @brief Returns number of vertices.
         * @details Returns the currently stored indices count in the mesh.
         *
         * @return Number of indices stored in the mesh.
         */
        [[nodiscard]] size_t size() const noexcept {
            return myIndices.size();
        }

    private:
        std::vector<VertexTy> myVertices;
        std::vector<unsigned> myIndices;
    };
}

#endif
