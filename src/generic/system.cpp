#include "coli/generic/system.h"

namespace Coli::Generic
{
    /* System Base */

    SystemBase::SystemBase() noexcept = default;

    SystemBase::SystemBase(SystemBase const&) noexcept = default;
    SystemBase::SystemBase(SystemBase&&) noexcept = default;

    SystemBase& SystemBase::operator=(SystemBase const&) noexcept = default;
    SystemBase& SystemBase::operator=(SystemBase&&) noexcept = default;

    SystemBase::~SystemBase() noexcept = default;
}