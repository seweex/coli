#ifndef COLI_GEOMETRY_MESH_H
#define COLI_GEOMETRY_MESH_H

#include "coli/geometry/vertex.h"

namespace Coli::Geometry
{
    template <Detail::VertexType VertexTy>
    class Mesh final
    {
    public:
        /**
         * @brief Creates the mesh of unique vertices and indices from
         * non-unique vertices sequence
         *
         * @tparam Iter The type of input iterator to non-unique vertices sequence
         *
         * @param first The beginning of the sequence
         * @param last The end of the sequence
         *
         * @throw std::bad_alloc If allocation fails
         * @warning UB If passed invalid iterators
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
         * @brief Copies the unique sequence of vertices and indices
         *
         * @tparam VertIter The type of input iterator to unique vertices sequence
         * @tparam IdxIter The type of input iterator to index sequence
         *
         * @param vertexFirst The beginning of the unique vertex sequence
         * @param vertexLast The end of the unique vertex sequence
         * @param indexFirst The beginning of the index sequence
         * @param indexLast The end of the index sequence
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
         * @brief Moves the unique sequence of vertices and indices
         *
         * @param vertices The rvalue ref to unique vertices vector
         * @param indices The rvalue ref to inices vector
         */
        Mesh(std::vector<VertexTy>&& vertices, std::vector<unsigned>&& indices) :
            myVertices (std::move(vertices)),
            myIndices (std::move(indices))
        {}

        /// @brief Default copy-ctor
        Mesh(Mesh const&) = default;

        /// @brief Default move-ctor
        Mesh(Mesh&&) noexcept = default;

        /**
         * @brief Default copy assignment
         * @return The reference to itself
         */
        Mesh& operator=(Mesh const&) = default;

        /**
         * @brief Default move assignment
         * @return The reference to itself
         */
        Mesh& operator=(Mesh&&) noexcept = default;

        /// @brief Destroys the mesh
        ~Mesh() noexcept = default;

        /**
         * @brief Gets the currently stored vertices
         * @return The const span to storing vertices
         */
        [[nodiscard]] std::span<VertexTy const> get_vertices() const noexcept {
            return myVertices;
        }

        /**
         * @brief Gets the currently stored indices
         * @return The const span to storing indices
         */
        [[nodiscard]] std::span<unsigned const> get_indices() const noexcept {
            return myIndices;
        }

        /**
         * @brief Gets the number of the currently stored non-unique vertices
         * @return The number of storing non-unique vertices
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
