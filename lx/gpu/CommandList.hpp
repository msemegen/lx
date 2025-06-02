#pragma once

// lx
#include <lx/common/non_copyable.hpp>

namespace lx::gpu {
class CommandList : private lx::common::non_copyable
{
};
} // namespace lx::gpu