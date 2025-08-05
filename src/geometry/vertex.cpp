#include "coli/geometry/vertex.h"

namespace Coli::Geometry
{
    Vertex<true>::Vertex(position_type const& pos, texcoord_type const& tex) noexcept :
        position(pos),
        texcoord(tex)
    {}

    Vertex<true>::Vertex(Vertex<true> const&) noexcept = default;
    Vertex<true>::Vertex(Vertex<true>&&) noexcept = default;

    Vertex<true>& Vertex<true>::operator=(Vertex<true> const&) noexcept = default;
    Vertex<true>& Vertex<true>::operator=(Vertex<true>&&) noexcept = default;

    Vertex<false>::Vertex(position_type const& pos, normal_type const& norm, texcoord_type const& tex) noexcept :
       position(pos),
       normal(norm),
       texcoord(tex)
    {}

    Vertex<false>::Vertex(Vertex<false> const&) noexcept = default;
    Vertex<false>::Vertex(Vertex<false>&&) noexcept = default;

    Vertex<false>& Vertex<false>::operator=(Vertex<false> const&) noexcept = default;
    Vertex<false>& Vertex<false>::operator=(Vertex<false>&&) noexcept = default;

    template struct COLI_EXPORT Vertex<false>;
    template struct COLI_EXPORT Vertex<true>;
}

namespace std
{
    template <bool Is2D>
    size_t hash<Coli::Geometry::Vertex<Is2D>>::operator()
    (Coli::Geometry::Vertex<Is2D> const& vertex) const noexcept
    {
        using traits = Coli::Geometry::VertexTraits<Coli::Geometry::Vertex<Is2D>>;

        Coli::HashMixer const mixer;
        size_t hash;

        hash = std::hash<typename traits::position_type>()(vertex.position);
        hash = mixer(hash, std::hash<typename traits::texcoord_type>()(vertex.texcoord));

        if constexpr (traits::has_normal::value)
            hash = mixer(hash, std::hash<typename traits::normal_type>()(vertex.normal));

        return hash;
    }

    template struct COLI_EXPORT hash<Coli::Geometry::Vertex<false>>;
    template struct COLI_EXPORT hash<Coli::Geometry::Vertex<true>>;
}