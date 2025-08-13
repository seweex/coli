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

namespace Coli::Utility
{
    size_t HashMixer::operator()(size_t h1, size_t h2) const noexcept
    {
        if constexpr (sizeof(size_t) == 8)
            return std::rotl(h1, 13) ^ std::rotl(h2, 37) + 0x9e3779b97f4a7c15;
        else
            return std::rotl(h1, 5) ^ std::rotl(h2, 17) + 0x9e3779b9;
    }

    template class COLI_EXPORT Hash<Types::float_type>;

    template class COLI_EXPORT Hash<Types::vector_type<true>>;
    template class COLI_EXPORT Hash<Types::vector_type<false>>;

    template class COLI_EXPORT Hash<Types::rotator_type<false>>;

    LockFreeBase::LockFreeBase() noexcept = default;

    LockFreeBase::LockFreeBase(LockFreeBase const&) noexcept :
        LockFreeBase()
    {}

    LockFreeBase::LockFreeBase(LockFreeBase&&) noexcept :
        LockFreeBase()
    {}

    LockFreeBase& LockFreeBase::operator=(LockFreeBase const&) noexcept { return *this; }
    LockFreeBase& LockFreeBase::operator=(LockFreeBase&&) noexcept { return *this; }

    LockFreeTaker::LockFreeTaker(LockFreeBase& object) noexcept :
        myObject (object),
        myFlag   (!object.myFlag.test_and_set(std::memory_order::memory_order_acquire))
    {}

    LockFreeTaker::~LockFreeTaker() noexcept {
        if (myFlag)
            myObject.myFlag.clear(std::memory_order::memory_order_release);
    }

    LockFreeTaker::operator bool() const noexcept {
        return myFlag;
    }
}
