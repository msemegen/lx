#pragma once

/*
 *	Name: bit.hpp
 *  Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <chrono>
#include <cstdint>

// xmcu
#include <lxf/common/non_constructible.hpp>

namespace lxf::common {
struct bit : private non_constructible
{
    template<typename Register_t, typename Index_t> [[nodiscard]] constexpr static bool is(Register_t register_a, Index_t index_a)
    {
        const Register_t flag = static_cast<Register_t>(0x1u) << index_a;
        return flag == (register_a & flag);
    }

    template<typename Register_t, typename Mask_t> [[nodiscard]] constexpr static bool is_any(Register_t register_a, Mask_t mask_a)
    {
        return static_cast<Mask_t>(0u) != (register_a & mask_a);
    }

    template<typename Register_t, typename Index_t> constexpr static void set(Register_t* p_register_a, Index_t index_a)
    {
        (*p_register_a) = (*p_register_a) | (0x1u << index_a);
    }

    template<typename Register_t, typename Index_t> constexpr static void clear(Register_t* p_register_a, Index_t index_a)
    {
        (*p_register_a) = (*p_register_a) & ~(0x1u << index_a);
    }

    template<typename Register_t, typename Index_t> constexpr static void toggle(Register_t* p_register_a, Index_t index_a)
    {
        (*p_register_a) = (*p_register_a) ^ (0x1u << index_a);
    }

    struct flag : private non_constructible
    {
        template<typename Register_t, typename Flag_t> [[nodiscard]] constexpr static inline bool is(Register_t register_a, Flag_t flag_a)
        {
            return flag_a == (register_a & flag_a);
        }

        template<typename Register_t, typename Mask_t>
        [[nodiscard]] constexpr static inline Mask_t get(Register_t register_a, Mask_t mask_a)
        {
            return (register_a & mask_a);
        }

        template<typename Register_t, typename Flag_t> constexpr static inline void set(Register_t* p_register_a, Flag_t flag_a)
        {
            (*p_register_a) = (*p_register_a) | flag_a;
        }

        template<typename Register_t, typename Clear_mask_t, typename Flag_t>
        constexpr static inline void set(Register_t* p_register_a, Clear_mask_t clear_mask_a, Flag_t set_flag_a)
        {
            (*p_register_a) = ((*p_register_a & (~clear_mask_a)) | set_flag_a);
        }

        template<typename Register_t, typename Flag_t> constexpr static inline void clear(Register_t* p_register_a, Flag_t flag_a)
        {
            (*p_register_a) = (*p_register_a) & ~flag_a;
        }

        template<typename Register_t, typename Mask_t> constexpr static inline void toggle(Register_t* p_register_a, Mask_t mask_a)
        {
            (*p_register_a) = (*p_register_a) ^ mask_a;
        }
    };

    struct wait_for : private non_constructible
    {
        template<typename Register_t, typename Mask_t> static void all_set(volatile const Register_t& register_a, Mask_t mask_a)
        {
            while (false == bit::flag::is(register_a, mask_a))
                ;
        }

        template<typename Register_t, typename Mask_t> static void any_set(volatile const Register_t& register_a, Mask_t mask_a)
        {
            while (false == bit::is_any(register_a, mask_a))
                ;
        }

        template<typename Register_t, typename Mask_t> static void all_cleared(volatile const Register_t& register_a, Mask_t mask_a)
        {
            while (false == bit::flag::is(~register_a, mask_a))
                ;
        }

        template<typename Register_t, typename Mask_t> static void any_cleared(volatile const Register_t& register_a, Mask_t mask_a)
        {
            while (false == bit::is_any(~register_a, mask_a))
                ;
        }

        template<typename Register_t, typename Mask_t>
        static bool all_set(volatile const Register_t& register_a, Mask_t mask_a, std::chrono::milliseconds timeout_a)
        {
            const std::chrono::steady_clock::time_point timeout = std::chrono::steady_clock::now() + timeout_a;
            bool status = false;

            while (std::chrono::steady_clock::now() < timeout && false == status)
            {
                status = bit::flag::is(register_a, mask_a);
            }

            return status;
        }

        template<typename Register_t, typename Mask_t>
        static bool any_set(volatile const Register_t& register_a, Mask_t mask_a, std::chrono::milliseconds timeout_a)
        {
            const std::chrono::steady_clock::time_point timeout = std::chrono::steady_clock::now() + timeout_a;
            bool status = false;

            while (std::chrono::steady_clock::now() < timeout && false == status)
            {
                status = bit::is_any(register_a, mask_a);
            }

            return status;
        }

        template<typename Register_t, typename Mask_t>
        static bool all_cleared(const Register_t& register_a, Mask_t mask_a, std::chrono::milliseconds timeout_a)
        {
            const std::chrono::steady_clock::time_point timeout = std::chrono::steady_clock::now() + timeout_a;
            bool status = false;

            while (std::chrono::steady_clock::now() < timeout && false == status)
            {
                status = bit::flag::is(~register_a, mask_a);
            }

            return status;
        }

        template<typename Register_t, typename Mask_t>
        static bool any_cleared(volatile const Register_t& register_a, Mask_t mask_a, std::chrono::milliseconds timeout_a)
        {
            const std::chrono::steady_clock::time_point timeout = std::chrono::steady_clock::now() + timeout_a;
            bool status = false;

            while (std::chrono::steady_clock::now() < timeout && false == status)
            {
                status = bit::is_any(~register_a, mask_a);
            }

            return status;
        }
    };
};
} // namespace lxf::common
