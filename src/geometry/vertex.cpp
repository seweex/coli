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

    bool Vertex<true>::operator==(Vertex<true> const&) const noexcept = default;
    bool Vertex<true>::operator!=(Vertex<true> const&) const noexcept = default;

    Vertex<false>::Vertex(position_type const& pos, normal_type const& norm, texcoord_type const& tex) noexcept :
       position(pos),
       normal(norm),
       texcoord(tex)
    {}

    Vertex<false>::Vertex(Vertex<false> const&) noexcept = default;
    Vertex<false>::Vertex(Vertex<false>&&) noexcept = default;

    Vertex<false>& Vertex<false>::operator=(Vertex<false> const&) noexcept = default;
    Vertex<false>& Vertex<false>::operator=(Vertex<false>&&) noexcept = default;

    bool Vertex<false>::operator==(Vertex<false> const&) const noexcept = default;
    bool Vertex<false>::operator!=(Vertex<false> const&) const noexcept = default;

    // template struct COLI_EXPORT Vertex<false>;
    // template struct COLI_EXPORT Vertex<true>;
}

namespace Coli::Utility
{
    template <bool Is2D>
    size_t Hash<Geometry::Vertex<Is2D>>::operator()
    (Geometry::Vertex<Is2D> const& vertex) const noexcept
    {
        using traits = Coli::Geometry::VertexTraits<Coli::Geometry::Vertex<Is2D>>;

        constexpr HashMixer mixer;
        size_t hash;

        hash = Hash<typename traits::position_type>()(vertex.position);
        hash = mixer(hash, Hash<typename traits::texcoord_type>()(vertex.texcoord));

        if constexpr (traits::has_normal::value)
            hash = mixer(hash, Hash<typename traits::normal_type>()(vertex.normal));

        return hash;
    }

    template class COLI_EXPORT Hash<Geometry::Vertex<false>>;
    template class COLI_EXPORT Hash<Geometry::Vertex<true>>;
}