#pragma once

/*
 *	Name: bit.hpp
 *  Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <chrono>
#include <cstdint>

// lxf
#include <lx/common/non_constructible.hpp>

namespace lx::common {
struct bit : private non_constructible
{
    template<typename Register, typename Index> [[nodiscard]] constexpr static bool is(Register reg, Index index)
    {
        const Register flag = static_cast<Register>(0x1u) << index;
        return flag == (reg & flag);
    }

    template<typename Register, typename Mask> [[nodiscard]] constexpr static bool is_any(Register reg, Mask mask)
    {
        return static_cast<Mask>(0u) != (reg & mask);
    }

    template<typename Register, typename Index> constexpr static void set(Register* reg, Index index)
    {
        (*reg) = (*reg) | (static_cast<Register>(0x1u) << index);
    }

    template<typename Register, typename Index> constexpr static void clear(Register* reg, Index index)
    {
        (*reg) = (*reg) & ~(static_cast<Register>(0x1u) << index);
    }

    template<typename Register, typename Index> constexpr static void toggle(Register* reg, Index index)
    {
        (*reg) = (*reg) ^ (static_cast<Register>(0x1u) << index);
    }

    struct flag : private non_constructible
    {
        template<typename Register, typename Flag> [[nodiscard]] constexpr static inline bool is(Register reg, Flag flag)
        {
            return flag == (reg & flag);
        }

        template<typename Register, typename Mask> [[nodiscard]] constexpr static inline Mask get(Register reg, Mask mask)
        {
            return (reg & mask);
        }

        template<typename Register, typename Flag> constexpr static inline void set(Register* reg, Flag flag)
        {
            (*reg) = (*reg) | flag;
        }

        template<typename Register, typename Clear_mask, typename Flag>
        constexpr static inline void set(Register* reg, Clear_mask clear_mask, Flag set_flag)
        {
            (*reg) = ((*reg & (~clear_mask)) | set_flag);
        }

        template<typename Register, typename Flag> constexpr static inline void clear(Register* reg, Flag flag)
        {
            (*reg) = (*reg) & ~flag;
        }

        template<typename Register, typename Mask> constexpr static inline void toggle(Register* reg, Mask mask)
        {
            (*reg) = (*reg) ^ mask;
        }
    };

    struct wait_for : private non_constructible
    {
        template<typename Register, typename Mask> static void all_set(volatile const Register& reg, Mask mask)
        {
            while (false == bit::flag::is(reg, mask));
        }

        template<typename Register, typename Mask> static void any_set(volatile const Register& reg, Mask mask)
        {
            while (false == bit::isny(reg, mask));
        }

        template<typename Register, typename Mask> static void all_cleared(volatile const Register& reg, Mask mask)
        {
            while (false == bit::flag::is(~reg, mask));
        }

        template<typename Register, typename Mask> static void any_cleared(volatile const Register& reg, Mask mask)
        {
            while (false == bit::isny(~reg, mask));
        }

        template<typename Register, typename Mask>
        static bool all_set(volatile const Register& reg, Mask mask, std::chrono::milliseconds timeout)
        {
            const std::chrono::steady_clock::time_point timeout = std::chrono::steady_clock::now() + timeout;
            bool status = false;

            while (std::chrono::steady_clock::now() < timeout && false == status)
            {
                status = bit::flag::is(reg, mask);
            }

            return status;
        }

        template<typename Register, typename Mask>
        static bool any_set(volatile const Register& reg, Mask mask, std::chrono::milliseconds timeout)
        {
            const std::chrono::steady_clock::time_point timeout = std::chrono::steady_clock::now() + timeout;
            bool status = false;

            while (std::chrono::steady_clock::now() < timeout && false == status)
            {
                status = bit::isny(reg, mask);
            }

            return status;
        }

        template<typename Register, typename Mask>
        static bool all_cleared(const Register& reg, Mask mask, std::chrono::milliseconds timeout)
        {
            const std::chrono::steady_clock::time_point timeout = std::chrono::steady_clock::now() + timeout;
            bool status = false;

            while (std::chrono::steady_clock::now() < timeout && false == status)
            {
                status = bit::flag::is(~reg, mask);
            }

            return status;
        }

        template<typename Register, typename Mask>
        static bool any_cleared(volatile const Register& reg, Mask mask, std::chrono::milliseconds timeout)
        {
            const std::chrono::steady_clock::time_point timeout = std::chrono::steady_clock::now() + timeout;
            bool status = false;

            while (std::chrono::steady_clock::now() < timeout && false == status)
            {
                status = bit::isny(~reg, mask);
            }

            return status;
        }
    };
};
} // namespace lx::common
