/*
 *   Name: file.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// this
#include <lxf/utils/file.hpp>

// std
#include <cassert>
#include <cstdio>

namespace lxf::utils {
using namespace lxf::common;

[[nodiscard]] file::Handle file::open(const char* name, file::Mode mode)
{
    Handle ret;

    char acc[4] = { 'b', '\0', '+', '\0' };

    switch (mode)
    {
        case Mode::read: {
            acc[1] = 'r';
        }
        break;
        case Mode::write: {
            acc[1] = 'w';
        }
        break;
        case Mode::read_write: {
            acc[1] = 'r';
        }
        break;
    }

    errno_t err = fopen_s(&(ret.p_file), name, acc);

    if (0 == err && nullptr != ret.p_file)
    {
        std::fseek(ret.p_file, 0, SEEK_END);
        ret.length_bytes = static_cast<std::size_t>(std::ftell(ret.p_file));
        std::rewind(ret.p_file);

        ret.mode = mode;
    }

    return ret;
}
bool file::close(file::Handle* handle)
{
    if (nullptr != handle->p_file)
    {
        std::fclose(handle->p_file);

        handle->p_file = nullptr;
        handle->length_bytes = 0u;
        handle->mode = various::get_enum_incorrect_value<file::Mode>();

        return true;
    }

    return false;
}

} // namespace lxf::utils