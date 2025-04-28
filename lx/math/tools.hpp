// lx
#include <lx/common/non_constructible.hpp>

// std
#include <algorithm>
#include <limits>

namespace lx::math {
struct tools : private common::non_constructible
{
    template<typename Type> static inline [[nodiscard]] bool is_equal(Type a, Type b)
    {
        return std::abs(a - b) <= std::numeric_limits<Type>::epsilon() * std::max(static_cast<Type>(1), std::max(std::abs(a), std::abs(b)));
    }
};
} // namespace lx::math