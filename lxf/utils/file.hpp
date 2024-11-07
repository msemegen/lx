#pragma once

/*
 *   Name: file.cpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <span>
#include <string_view>
#include <vector>

// lxf
#include <lxf/common/Out.hpp>
#include <lxf/common/non_constructible.hpp>
#include <lxf/common/various.hpp>

namespace lxf::utils {
struct file : private common::non_constructible
{
    enum class Kind : std::uint64_t
    {
        binary,
        text
    };
    enum class Mode : std::uint64_t
    {
        read,
        write,
        read_write
    };

    struct Handle;

    static [[nodiscard]] bool exists(const Handle& handle);
    static [[nodiscard]] std::size_t size_bytes(const Handle& handle);
    template<typename Type>
    static std::size_t read(const Handle& handle, std::size_t elements_to_read, common::Out<std::vector<Type>> buffer)
    {
        assert(file::Mode::read == handle.mode || file::Mode::read_write == handle.mode);

        std::size_t length = buffer->size();

        buffer->resize(buffer->size() + elements_to_read);
        std::size_t ret = std::fread(buffer->data() + length, 1u, elements_to_read, handle.p_file);
        buffer->shrink_to_fit();

        return ret;
    }

    static void write(const Handle& handle, std::span<std::byte> data, common::Out<std::vector<std::byte>> buffer);

    static [[nodiscard]] Handle open(const char* name, Mode mode);
    static bool close(Handle* handle);

    struct Handle
    {
    public:
        friend static bool file::exists(const Handle& handle);
        friend static std::size_t file::size_bytes(const Handle& handle);
        template<typename Type>
        friend static std::size_t file::read(const Handle& handle, std::size_t bytes_to_read, common::Out<std::vector<Type>> data);
        friend static void file::write(const Handle& handle, std::span<std::byte> data, common::Out<std::vector<std::byte>> buffer);

        friend static Handle file::open(const char* p_name, Mode mode);
        friend static bool file::close(Handle* p_handle);

    private:
        std::FILE* p_file = nullptr;
        std::size_t length_bytes = 0u;
        Mode mode = common::various::get_enum_incorrect_value<Mode>();
    };
};

inline [[nodiscard]] bool file::exists(const Handle& handle)
{
    return nullptr != handle.p_file;
}
inline [[nodiscard]] std::size_t file::size_bytes(const Handle& handle)
{
    return handle.length_bytes;
}
} // namespace lxf::utils