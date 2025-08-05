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
