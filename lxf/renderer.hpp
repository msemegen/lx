#pragma once

/*
 *   Name: renderer.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <cassert>
#include <cstdint>
#include <utility>

// lxf
#include <lxf/canvas.hpp>
#include <lxf/common/non_constructible.hpp>
#include <lxf/common/non_copyable.hpp>
#include <lxf/device.hpp>
#include <lxf/loader/vulkan.hpp>

namespace lxf {
struct renderer : private common::non_constructible
{
    class Context : private common::non_copyable
    {
    public:
        struct Queue_family
        {
            enum class Kind : std::uint32_t
            {
                graphics = VK_QUEUE_GRAPHICS_BIT,
                compute = VK_QUEUE_COMPUTE_BIT,
                transfer = VK_QUEUE_TRANSFER_BIT,
                sparse_binding = VK_QUEUE_SPARSE_BINDING_BIT,
                protected_memory = VK_QUEUE_PROTECTED_BIT,
                video_decode = VK_QUEUE_VIDEO_DECODE_BIT_KHR,
                video_encode = VK_QUEUE_VIDEO_ENCODE_BIT_KHR,
                optical_flow = VK_QUEUE_OPTICAL_FLOW_BIT_NV,
                presentation = 0x200u
            };

            using enum Kind;

            Kind kind;
            std::size_t count;

            std::span<const std::uint16_t> priorities;
        };
        struct Swap_chain
        {
            enum class Pixel : std::uint64_t
            {
                undefined = VK_FORMAT_UNDEFINED,
                r4g4_unorm_pack8 = VK_FORMAT_R4G4_UNORM_PACK8,
                r4g4b4a4_unorm_pack16 = VK_FORMAT_R4G4B4A4_UNORM_PACK16,
                b4g4r4a4_unorm_pack16 = VK_FORMAT_B4G4R4A4_UNORM_PACK16,
                r5g6b5_unorm_pack16 = VK_FORMAT_R5G6B5_UNORM_PACK16,
                b5g6r5_unorm_pack16 = VK_FORMAT_B5G6R5_UNORM_PACK16,
                r5g5b5a1_unorm_pack16 = VK_FORMAT_R5G5B5A1_UNORM_PACK16,
                b5g5r5a1_unorm_pack16 = VK_FORMAT_B5G5R5A1_UNORM_PACK16,
                a1r5g5b5_unorm_pack16 = VK_FORMAT_A1R5G5B5_UNORM_PACK16,
                r8_unorm = VK_FORMAT_R8_UNORM,
                r8_snorm = VK_FORMAT_R8_SNORM,
                r8_uscaled = VK_FORMAT_R8_USCALED,
                r8_sscaled = VK_FORMAT_R8_SSCALED,
                r8_uint = VK_FORMAT_R8_UINT,
                r8_sint = VK_FORMAT_R8_SINT,
                r8_srgb = VK_FORMAT_R8_SRGB,
                r8g8_unorm = VK_FORMAT_R8G8_UNORM,
                r8g8_snorm = VK_FORMAT_R8G8_SNORM,
                r8g8_uscaled = VK_FORMAT_R8G8_USCALED,
                r8g8_sscaled = VK_FORMAT_R8G8_SSCALED,
                r8g8_uint = VK_FORMAT_R8G8_UINT,
                r8g8_sint = VK_FORMAT_R8G8_SINT,
                r8g8_srgb = VK_FORMAT_R8G8_SRGB,
                r8g8b8_unorm = VK_FORMAT_R8G8B8_UNORM,
                r8g8b8_snorm = VK_FORMAT_R8G8B8_SNORM,
                r8g8b8_uscaled = VK_FORMAT_R8G8B8_USCALED,
                r8g8b8_sscaled = VK_FORMAT_R8G8B8_SSCALED,
                r8g8b8_uint = VK_FORMAT_R8G8B8_UINT,
                r8g8b8_sint = VK_FORMAT_R8G8B8_SINT,
                r8g8b8_srgb = VK_FORMAT_R8G8B8_SRGB,
                b8g8r8_unorm = VK_FORMAT_B8G8R8_UNORM,
                b8g8r8_snorm = VK_FORMAT_B8G8R8_SNORM,
                b8g8r8_uscaled = VK_FORMAT_B8G8R8_USCALED,
                b8g8r8_sscaled = VK_FORMAT_B8G8R8_SSCALED,
                b8g8r8_uint = VK_FORMAT_B8G8R8_UINT,
                b8g8r8_sint = VK_FORMAT_B8G8R8_SINT,
                b8g8r8_srgb = VK_FORMAT_B8G8R8_SRGB,
                r8g8b8a8_unorm = VK_FORMAT_R8G8B8A8_UNORM,
                r8g8b8a8_snorm = VK_FORMAT_R8G8B8A8_SNORM,
                r8g8b8a8_uscaled = VK_FORMAT_R8G8B8A8_USCALED,
                r8g8b8a8_sscaled = VK_FORMAT_R8G8B8A8_SSCALED,
                r8g8b8a8_uint = VK_FORMAT_R8G8B8A8_UINT,
                r8g8b8a8_sint = VK_FORMAT_R8G8B8A8_SINT,
                r8g8b8a8_srgb = VK_FORMAT_R8G8B8A8_SRGB,
                b8g8r8a8_unorm = VK_FORMAT_B8G8R8A8_UNORM,
                b8g8r8a8_snorm = VK_FORMAT_B8G8R8A8_SNORM,
                b8g8r8a8_uscaled = VK_FORMAT_B8G8R8A8_USCALED,
                b8g8r8a8_sscaled = VK_FORMAT_B8G8R8A8_SSCALED,
                b8g8r8a8_uint = VK_FORMAT_B8G8R8A8_UINT,
                b8g8r8a8_sint = VK_FORMAT_B8G8R8A8_SINT,
                b8g8r8a8_srgb = VK_FORMAT_B8G8R8A8_SRGB,
                a8b8g8r8_unorm_pack32 = VK_FORMAT_A8B8G8R8_UNORM_PACK32,
                a8b8g8r8_snorm_pack32 = VK_FORMAT_A8B8G8R8_SNORM_PACK32,
                a8b8g8r8_uscaled_pack32 = VK_FORMAT_A8B8G8R8_USCALED_PACK32,
                a8b8g8r8_sscaled_pack32 = VK_FORMAT_A8B8G8R8_SSCALED_PACK32,
                a8b8g8r8_uint_pack32 = VK_FORMAT_A8B8G8R8_UINT_PACK32,
                a8b8g8r8_sint_pack32 = VK_FORMAT_A8B8G8R8_SINT_PACK32,
                a8b8g8r8_srgb_pack32 = VK_FORMAT_A8B8G8R8_SRGB_PACK32,
                a2r10g10b10_unorm_pack32 = VK_FORMAT_A2R10G10B10_UNORM_PACK32,
                a2r10g10b10_snorm_pack32 = VK_FORMAT_A2R10G10B10_SNORM_PACK32,
                a2r10g10b10_uscaled_pack32 = VK_FORMAT_A2R10G10B10_USCALED_PACK32,
                a2r10g10b10_sscaled_pack32 = VK_FORMAT_A2R10G10B10_SSCALED_PACK32,
                a2r10g10b10_uint_pack32 = VK_FORMAT_A2R10G10B10_UINT_PACK32,
                a2r10g10b10_sint_pack32 = VK_FORMAT_A2R10G10B10_SINT_PACK32,
                a2b10g10r10_unorm_pack32 = VK_FORMAT_A2B10G10R10_UNORM_PACK32,
                a2b10g10r10_snorm_pack32 = VK_FORMAT_A2B10G10R10_SNORM_PACK32,
                a2b10g10r10_uscaled_pack32 = VK_FORMAT_A2B10G10R10_USCALED_PACK32,
                a2b10g10r10_sscaled_pack32 = VK_FORMAT_A2B10G10R10_SSCALED_PACK32,
                a2b10g10r10_uint_pack32 = VK_FORMAT_A2B10G10R10_UINT_PACK32,
                a2b10g10r10_sint_pack32 = VK_FORMAT_A2B10G10R10_SINT_PACK32,
                r16_unorm = VK_FORMAT_R16_UNORM,
                r16_snorm = VK_FORMAT_R16_SNORM,
                r16_uscaled = VK_FORMAT_R16_USCALED,
                r16_sscaled = VK_FORMAT_R16_SSCALED,
                r16_uint = VK_FORMAT_R16_UINT,
                r16_sint = VK_FORMAT_R16_SINT,
                r16_sfloat = VK_FORMAT_R16_SFLOAT,
                r16g16_unorm = VK_FORMAT_R16G16_UNORM,
                r16g16_snorm = VK_FORMAT_R16G16_SNORM,
                r16g16_uscaled = VK_FORMAT_R16G16_USCALED,
                r16g16_sscaled = VK_FORMAT_R16G16_SSCALED,
                r16g16_uint = VK_FORMAT_R16G16_UINT,
                r16g16_sint = VK_FORMAT_R16G16_SINT,
                r16g16_sfloat = VK_FORMAT_R16G16_SFLOAT,
                r16g16b16_unorm = VK_FORMAT_R16G16B16_UNORM,
                r16g16b16_snorm = VK_FORMAT_R16G16B16_SNORM,
                r16g16b16_uscaled = VK_FORMAT_R16G16B16_USCALED,
                r16g16b16_sscaled = VK_FORMAT_R16G16B16_SSCALED,
                r16g16b16_uint = VK_FORMAT_R16G16B16_UINT,
                r16g16b16_sint = VK_FORMAT_R16G16B16_SINT,
                r16g16b16_sfloat = VK_FORMAT_R16G16B16_SFLOAT,
                r16g16b16a16_unorm = VK_FORMAT_R16G16B16A16_UNORM,
                r16g16b16a16_snorm = VK_FORMAT_R16G16B16A16_SNORM,
                r16g16b16a16_uscaled = VK_FORMAT_R16G16B16A16_USCALED,
                r16g16b16a16_sscaled = VK_FORMAT_R16G16B16A16_SSCALED,
                r16g16b16a16_uint = VK_FORMAT_R16G16B16A16_UINT,
                r16g16b16a16_sint = VK_FORMAT_R16G16B16A16_SINT,
                r16g16b16a16_sfloat = VK_FORMAT_R16G16B16A16_SFLOAT,
                r32_uint = VK_FORMAT_R32_UINT,
                r32_sint = VK_FORMAT_R32_SINT,
                r32_sfloat = VK_FORMAT_R32_SFLOAT,
                r32g32_uint = VK_FORMAT_R32G32_UINT,
                r32g32_sint = VK_FORMAT_R32G32_SINT,
                r32g32_sfloat = VK_FORMAT_R32G32_SFLOAT,
                r32g32b32_uint = VK_FORMAT_R32G32B32_UINT,
                r32g32b32_sint = VK_FORMAT_R32G32B32_SINT,
                r32g32b32_sfloat = VK_FORMAT_R32G32B32_SFLOAT,
                r32g32b32a32_uint = VK_FORMAT_R32G32B32A32_UINT,
                r32g32b32a32_sint = VK_FORMAT_R32G32B32A32_SINT,
                r32g32b32a32_sfloat = VK_FORMAT_R32G32B32A32_SFLOAT,
                r64_uint = VK_FORMAT_R64_UINT,
                r64_sint = VK_FORMAT_R64_SINT,
                r64_sfloat = VK_FORMAT_R64_SFLOAT,
                r64g64_uint = VK_FORMAT_R64G64_UINT,
                r64g64_sint = VK_FORMAT_R64G64_SINT,
                r64g64_sfloat = VK_FORMAT_R64G64_SFLOAT,
                r64g64b64_uint = VK_FORMAT_R64G64B64_UINT,
                r64g64b64_sint = VK_FORMAT_R64G64B64_SINT,
                r64g64b64_sfloat = VK_FORMAT_R64G64B64_SFLOAT,
                r64g64b64a64_uint = VK_FORMAT_R64G64B64A64_UINT,
                r64g64b64a64_sint = VK_FORMAT_R64G64B64A64_SINT,
                r64g64b64a64_sfloat = VK_FORMAT_R64G64B64A64_SFLOAT,
                b10g11r11_ufloat_pack32 = VK_FORMAT_B10G11R11_UFLOAT_PACK32,
                e5b9g9r9_ufloat_pack32 = VK_FORMAT_E5B9G9R9_UFLOAT_PACK32,
                d16_unorm = VK_FORMAT_D16_UNORM,
                x8_d24_unorm_pack32 = VK_FORMAT_X8_D24_UNORM_PACK32,
                d32_sfloat = VK_FORMAT_D32_SFLOAT,
                s8_uint = VK_FORMAT_S8_UINT,
                d16_unorm_s8_uint = VK_FORMAT_D16_UNORM_S8_UINT,
                d24_unorm_s8_uint = VK_FORMAT_D24_UNORM_S8_UINT,
                d32_sfloat_s8_uint = VK_FORMAT_D32_SFLOAT_S8_UINT,
                bc1_rgb_unorm_block = VK_FORMAT_BC1_RGB_UNORM_BLOCK,
                bc1_rgb_srgb_block = VK_FORMAT_BC1_RGB_SRGB_BLOCK,
                bc1_rgba_unorm_block = VK_FORMAT_BC1_RGBA_UNORM_BLOCK,
                bc1_rgba_srgb_block = VK_FORMAT_BC1_RGBA_SRGB_BLOCK,
                bc2_unorm_block = VK_FORMAT_BC2_UNORM_BLOCK,
                bc2_srgb_block = VK_FORMAT_BC2_SRGB_BLOCK,
                bc3_unorm_block = VK_FORMAT_BC3_UNORM_BLOCK,
                bc3_srgb_block = VK_FORMAT_BC3_SRGB_BLOCK,
                bc4_unorm_block = VK_FORMAT_BC4_UNORM_BLOCK,
                bc4_snorm_block = VK_FORMAT_BC4_SNORM_BLOCK,
                bc5_unorm_block = VK_FORMAT_BC5_UNORM_BLOCK,
                bc5_snorm_block = VK_FORMAT_BC5_SNORM_BLOCK,
                bc6h_ufloat_block = VK_FORMAT_BC6H_UFLOAT_BLOCK,
                bc6h_sfloat_block = VK_FORMAT_BC6H_SFLOAT_BLOCK,
                bc7_unorm_block = VK_FORMAT_BC7_UNORM_BLOCK,
                bc7_srgb_block = VK_FORMAT_BC7_SRGB_BLOCK,
                etc2_r8g8b8_unorm_block = VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK,
                etc2_r8g8b8_srgb_block = VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK,
                etc2_r8g8b8a1_unorm_block = VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK,
                etc2_r8g8b8a1_srgb_block = VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK,
                etc2_r8g8b8a8_unorm_block = VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK,
                etc2_r8g8b8a8_srgb_block = VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK,
                eac_r11_unorm_block = VK_FORMAT_EAC_R11_UNORM_BLOCK,
                eac_r11_snorm_block = VK_FORMAT_EAC_R11_SNORM_BLOCK,
                eac_r11g11_unorm_block = VK_FORMAT_EAC_R11G11_UNORM_BLOCK,
                eac_r11g11_snorm_block = VK_FORMAT_EAC_R11G11_SNORM_BLOCK,
                astc_4x4_unorm_block = VK_FORMAT_ASTC_4x4_UNORM_BLOCK,
                astc_4x4_srgb_block = VK_FORMAT_ASTC_4x4_SRGB_BLOCK,
                astc_5x4_unorm_block = VK_FORMAT_ASTC_5x4_UNORM_BLOCK,
                astc_5x4_srgb_block = VK_FORMAT_ASTC_5x4_SRGB_BLOCK,
                astc_5x5_unorm_block = VK_FORMAT_ASTC_5x5_UNORM_BLOCK,
                astc_5x5_srgb_block = VK_FORMAT_ASTC_5x5_SRGB_BLOCK,
                astc_6x5_unorm_block = VK_FORMAT_ASTC_6x5_UNORM_BLOCK,
                astc_6x5_srgb_block = VK_FORMAT_ASTC_6x5_SRGB_BLOCK,
                astc_6x6_unorm_block = VK_FORMAT_ASTC_6x6_UNORM_BLOCK,
                astc_6x6_srgb_block = VK_FORMAT_ASTC_6x6_SRGB_BLOCK,
                astc_8x5_unorm_block = VK_FORMAT_ASTC_8x5_UNORM_BLOCK,
                astc_8x5_srgb_block = VK_FORMAT_ASTC_8x5_SRGB_BLOCK,
                astc_8x6_unorm_block = VK_FORMAT_ASTC_8x6_UNORM_BLOCK,
                astc_8x6_srgb_block = VK_FORMAT_ASTC_8x6_SRGB_BLOCK,
                astc_8x8_unorm_block = VK_FORMAT_ASTC_8x8_UNORM_BLOCK,
                astc_8x8_srgb_block = VK_FORMAT_ASTC_8x8_SRGB_BLOCK,
                astc_10x5_unorm_block = VK_FORMAT_ASTC_10x5_UNORM_BLOCK,
                astc_10x5_srgb_block = VK_FORMAT_ASTC_10x5_SRGB_BLOCK,
                astc_10x6_unorm_block = VK_FORMAT_ASTC_10x6_UNORM_BLOCK,
                astc_10x6_srgb_block = VK_FORMAT_ASTC_10x6_SRGB_BLOCK,
                astc_10x8_unorm_block = VK_FORMAT_ASTC_10x8_UNORM_BLOCK,
                astc_10x8_srgb_block = VK_FORMAT_ASTC_10x8_SRGB_BLOCK,
                astc_10x10_unorm_block = VK_FORMAT_ASTC_10x10_UNORM_BLOCK,
                astc_10x10_srgb_block = VK_FORMAT_ASTC_10x10_SRGB_BLOCK,
                astc_12x10_unorm_block = VK_FORMAT_ASTC_12x10_UNORM_BLOCK,
                astc_12x10_srgb_block = VK_FORMAT_ASTC_12x10_SRGB_BLOCK,
                astc_12x12_unorm_block = VK_FORMAT_ASTC_12x12_UNORM_BLOCK,
                astc_12x12_srgb_block = VK_FORMAT_ASTC_12x12_SRGB_BLOCK,
                g8b8g8r8_422_unorm = VK_FORMAT_G8B8G8R8_422_UNORM,
                b8g8r8g8_422_unorm = VK_FORMAT_B8G8R8G8_422_UNORM,
                g8_b8_r8_3plane_420_unorm = VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM,
                g8_b8r8_2plane_420_unorm = VK_FORMAT_G8_B8R8_2PLANE_420_UNORM,
                g8_b8_r8_3plane_422_unorm = VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM,
                g8_b8r8_2plane_422_unorm = VK_FORMAT_G8_B8R8_2PLANE_422_UNORM,
                g8_b8_r8_3plane_444_unorm = VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM,
                r10x6_unorm_pack16 = VK_FORMAT_R10X6_UNORM_PACK16,
                r10x6g10x6_unorm_2pack16 = VK_FORMAT_R10X6G10X6_UNORM_2PACK16,
                r10x6g10x6b10x6a10x6_unorm_4pack16 = VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16,
                g10x6b10x6g10x6r10x6_422_unorm_4pack16 = VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16,
                b10x6g10x6r10x6g10x6_422_unorm_4pack16 = VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16,
                g10x6_b10x6_r10x6_3plane_420_unorm_3pack16 = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16,
                g10x6_b10x6r10x6_2plane_420_unorm_3pack16 = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16,
                g10x6_b10x6_r10x6_3plane_422_unorm_3pack16 = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16,
                g10x6_b10x6r10x6_2plane_422_unorm_3pack16 = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16,
                g10x6_b10x6_r10x6_3plane_444_unorm_3pack16 = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16,
                r12x4_unorm_pack16 = VK_FORMAT_R12X4_UNORM_PACK16,
                r12x4g12x4_unorm_2pack16 = VK_FORMAT_R12X4G12X4_UNORM_2PACK16,
                r12x4g12x4b12x4a12x4_unorm_4pack16 = VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16,
                g12x4b12x4g12x4r12x4_422_unorm_4pack16 = VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16,
                b12x4g12x4r12x4g12x4_422_unorm_4pack16 = VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16,
                g12x4_b12x4_r12x4_3plane_420_unorm_3pack16 = VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16,
                g12x4_b12x4r12x4_2plane_420_unorm_3pack16 = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16,
                g12x4_b12x4_r12x4_3plane_422_unorm_3pack16 = VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16,
                g12x4_b12x4r12x4_2plane_422_unorm_3pack16 = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16,
                g12x4_b12x4_r12x4_3plane_444_unorm_3pack16 = VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16,
                g16b16g16r16_422_unorm = VK_FORMAT_G16B16G16R16_422_UNORM,
                b16g16r16g16_422_unorm = VK_FORMAT_B16G16R16G16_422_UNORM,
                g16_b16_r16_3plane_420_unorm = VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM,
                g16_b16r16_2plane_420_unorm = VK_FORMAT_G16_B16R16_2PLANE_420_UNORM,
                g16_b16_r16_3plane_422_unorm = VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM,
                g16_b16r16_2plane_422_unorm = VK_FORMAT_G16_B16R16_2PLANE_422_UNORM,
                g16_b16_r16_3plane_444_unorm = VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM,
                g8_b8r8_2plane_444_unorm = VK_FORMAT_G8_B8R8_2PLANE_444_UNORM,
                g10x6_b10x6r10x6_2plane_444_unorm_3pack16 = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16,
                g12x4_b12x4r12x4_2plane_444_unorm_3pack16 = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16,
                g16_b16r16_2plane_444_unorm = VK_FORMAT_G16_B16R16_2PLANE_444_UNORM,
                a4r4g4b4_unorm_pack16 = VK_FORMAT_A4R4G4B4_UNORM_PACK16,
                a4b4g4r4_unorm_pack16 = VK_FORMAT_A4B4G4R4_UNORM_PACK16,
                astc_4x4_sfloat_block = VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK,
                astc_5x4_sfloat_block = VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK,
                astc_5x5_sfloat_block = VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK,
                astc_6x5_sfloat_block = VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK,
                astc_6x6_sfloat_block = VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK,
                astc_8x5_sfloat_block = VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK,
                astc_8x6_sfloat_block = VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK,
                astc_8x8_sfloat_block = VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK,
                astc_10x5_sfloat_block = VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK,
                astc_10x6_sfloat_block = VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK,
                astc_10x8_sfloat_block = VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK,
                astc_10x10_sfloat_block = VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK,
                astc_12x10_sfloat_block = VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK,
                astc_12x12_sfloat_block = VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK,
                pvrtc1_2bpp_unorm_block_img = VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG,
                pvrtc1_4bpp_unorm_block_img = VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG,
                pvrtc2_2bpp_unorm_block_img = VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG,
                pvrtc2_4bpp_unorm_block_img = VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG,
                pvrtc1_2bpp_srgb_block_img = VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG,
                pvrtc1_4bpp_srgb_block_img = VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG,
                pvrtc2_2bpp_srgb_block_img = VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG,
                pvrtc2_4bpp_srgb_block_img = VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG,
                r16g16_sfixed5_nv = VK_FORMAT_R16G16_SFIXED5_NV,
                a1b5g5r5_unorm_pack16_khr = VK_FORMAT_A1B5G5R5_UNORM_PACK16_KHR,
                a8_unorm_khr = VK_FORMAT_A8_UNORM_KHR,
                astc_4x4_sfloat_block_ext = VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT,
                astc_5x4_sfloat_block_ext = VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK_EXT,
                astc_5x5_sfloat_block_ext = VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK_EXT,
                astc_6x5_sfloat_block_ext = VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK_EXT,
                astc_6x6_sfloat_block_ext = VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK_EXT,
                astc_8x5_sfloat_block_ext = VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK_EXT,
                astc_8x6_sfloat_block_ext = VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK_EXT,
                astc_8x8_sfloat_block_ext = VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK_EXT,
                astc_10x5_sfloat_block_ext = VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK_EXT,
                astc_10x6_sfloat_block_ext = VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK_EXT,
                astc_10x8_sfloat_block_ext = VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK_EXT,
                astc_10x10_sfloat_block_ext = VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK_EXT,
                astc_12x10_sfloat_block_ext = VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK_EXT,
                astc_12x12_sfloat_block_ext = VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT,
                g8b8g8r8_422_unorm_khr = VK_FORMAT_G8B8G8R8_422_UNORM_KHR,
                b8g8r8g8_422_unorm_khr = VK_FORMAT_B8G8R8G8_422_UNORM_KHR,
                g8_b8_r8_3plane_420_unorm_khr = VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM_KHR,
                g8_b8r8_2plane_420_unorm_khr = VK_FORMAT_G8_B8R8_2PLANE_420_UNORM_KHR,
                g8_b8_r8_3plane_422_unorm_khr = VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM_KHR,
                g8_b8r8_2plane_422_unorm_khr = VK_FORMAT_G8_B8R8_2PLANE_422_UNORM_KHR,
                g8_b8_r8_3plane_444_unorm_khr = VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM_KHR,
                r10x6_unorm_pack16_khr = VK_FORMAT_R10X6_UNORM_PACK16_KHR,
                r10x6g10x6_unorm_2pack16_khr = VK_FORMAT_R10X6G10X6_UNORM_2PACK16_KHR,
                r10x6g10x6b10x6a10x6_unorm_4pack16_khr = VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16_KHR,
                g10x6b10x6g10x6r10x6_422_unorm_4pack16_khr = VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16_KHR,
                b10x6g10x6r10x6g10x6_422_unorm_4pack16_khr = VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16_KHR,
                g10x6_b10x6_r10x6_3plane_420_unorm_3pack16_khr = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR,
                g10x6_b10x6r10x6_2plane_420_unorm_3pack16_khr = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR,
                g10x6_b10x6_r10x6_3plane_422_unorm_3pack16_khr = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR,
                g10x6_b10x6r10x6_2plane_422_unorm_3pack16_khr = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR,
                g10x6_b10x6_r10x6_3plane_444_unorm_3pack16_khr = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR,
                r12x4_unorm_pack16_khr = VK_FORMAT_R12X4_UNORM_PACK16_KHR,
                r12x4g12x4_unorm_2pack16_khr = VK_FORMAT_R12X4G12X4_UNORM_2PACK16_KHR,
                r12x4g12x4b12x4a12x4_unorm_4pack16_khr = VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16_KHR,
                g12x4b12x4g12x4r12x4_422_unorm_4pack16_khr = VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16_KHR,
                b12x4g12x4r12x4g12x4_422_unorm_4pack16_khr = VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16_KHR,
                g12x4_b12x4_r12x4_3plane_420_unorm_3pack16_khr = VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR,
                g12x4_b12x4r12x4_2plane_420_unorm_3pack16_khr = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR,
                g12x4_b12x4_r12x4_3plane_422_unorm_3pack16_khr = VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR,
                g12x4_b12x4r12x4_2plane_422_unorm_3pack16_khr = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR,
                g12x4_b12x4_r12x4_3plane_444_unorm_3pack16_khr = VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR,
                g16b16g16r16_422_unorm_khr = VK_FORMAT_G16B16G16R16_422_UNORM_KHR,
                b16g16r16g16_422_unorm_khr = VK_FORMAT_B16G16R16G16_422_UNORM_KHR,
                g16_b16_r16_3plane_420_unorm_khr = VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM_KHR,
                g16_b16r16_2plane_420_unorm_khr = VK_FORMAT_G16_B16R16_2PLANE_420_UNORM_KHR,
                g16_b16_r16_3plane_422_unorm_khr = VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM_KHR,
                g16_b16r16_2plane_422_unorm_khr = VK_FORMAT_G16_B16R16_2PLANE_422_UNORM_KHR,
                g16_b16_r16_3plane_444_unorm_khr = VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM_KHR,
                g8_b8r8_2plane_444_unorm_ext = VK_FORMAT_G8_B8R8_2PLANE_444_UNORM_EXT,
                g10x6_b10x6r10x6_2plane_444_unorm_3pack16_ext = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16_EXT,
                g12x4_b12x4r12x4_2plane_444_unorm_3pack16_ext = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16_EXT,
                g16_b16r16_2plane_444_unorm_ext = VK_FORMAT_G16_B16R16_2PLANE_444_UNORM_EXT,
                a4r4g4b4_unorm_pack16_ext = VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT,
                a4b4g4r4_unorm_pack16_ext = VK_FORMAT_A4B4G4R4_UNORM_PACK16_EXT,
                r16g16_s10_5_nv = VK_FORMAT_R16G16_S10_5_NV,
            };
            enum class Color_space : std::uint64_t
            {
                srgb_nonlinear_khr = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
                display_p3_nonlinear_ext = VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT,
                extended_srgb_linear_ext = VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT,
                display_p3_linear_ext = VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT,
                dci_p3_nonlinear_ext = VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT,
                bt709_linear_ext = VK_COLOR_SPACE_BT709_LINEAR_EXT,
                bt709_nonlinear_ext = VK_COLOR_SPACE_BT709_NONLINEAR_EXT,
                bt2020_linear_ext = VK_COLOR_SPACE_BT2020_LINEAR_EXT,
                hdr10_st2084_ext = VK_COLOR_SPACE_HDR10_ST2084_EXT,
                dolbyvision_ext = VK_COLOR_SPACE_DOLBYVISION_EXT,
                hdr10_hlg_ext = VK_COLOR_SPACE_HDR10_HLG_EXT,
                adobergb_linear_ext = VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT,
                adobergb_nonlinear_ext = VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT,
                pass_through_ext = VK_COLOR_SPACE_PASS_THROUGH_EXT,
                extended_srgb_nonlinear_ext = VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT,
                display_native_amd = VK_COLOR_SPACE_DISPLAY_NATIVE_AMD,
                space_dci_p3_linear_ext = VK_COLOR_SPACE_DCI_P3_LINEAR_EXT,
            };
            enum class Mode : std::uint64_t
            {
                immediate = VK_PRESENT_MODE_IMMEDIATE_KHR,
                mailbox = VK_PRESENT_MODE_MAILBOX_KHR,
                fifo = VK_PRESENT_MODE_FIFO_KHR,
                fifo_relaxed = VK_PRESENT_MODE_FIFO_RELAXED_KHR,
                shared_demand_refresh = VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR,
                shared_continuous_refresh = VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR,
            };

            Pixel pixel;
            Color_space color_space;
            Mode mode;

            std::size_t images_count;
        };

        using Feature = device::GPU::Feature;

        struct Properties
        {
            Feature features;
            std::span<const Queue_family> queue_families;
            std::span<const char*> extensions;
            Swap_chain swap_chain;
        };

        [[nodiscard]] bool is_created() const
        {
            return VK_NULL_HANDLE != this->vk_device;
        }

        operator VkDevice() const
        {
            return this->vk_device;
        }

        operator VkSwapchainKHR() const
        {
            return this->vk_swap_chain;
        }

    private:
        Context()
            : vk_device(VK_NULL_HANDLE)
        {
        }

        bool create(const device::GPU* gpu, HWND window_handle, VkSurfaceKHR vk_surface, const Properties& properties);
        void release();

        VkDevice vk_device;
        VkSwapchainKHR vk_swap_chain;

        std::vector<VkQueue> vk_queues;
        std::vector<VkImage> vk_swap_chain_images;

        friend struct renderer;
    };

    template<typename Type, typename Window>
    static [[nodiscard]] Type* create(const device::GPU* gpu, Window* window, const typename Type::Properties& properties) = delete;
    template<typename Type_t> static void destroy(Type_t** object) = delete;

private:
    inline static Context context;
};

template<>
[[nodiscard]] inline renderer::Context* renderer::create<renderer::Context, canvas::Windowed>(const device::GPU* gpu,
                                                                                              canvas::Windowed* window,
                                                                                              const Context::Properties& properties)
{
    assert(false == context.is_created());

    if (true == context.create(gpu, *window, *window, properties))
    {
        return &(context);
    }

    return nullptr;
}

template<> inline void renderer::destroy<renderer::Context>(renderer::Context** ctx)
{
    assert(true == (*ctx)->is_created());

    (*ctx)->release();
    *ctx = nullptr;
}

constexpr renderer::Context::Queue_family::Kind operator|(renderer::Context::Queue_family::Kind left,
                                                          renderer::Context::Queue_family::Kind right)
{
    return static_cast<renderer::Context::Queue_family::Kind>(static_cast<std::uint64_t>(left) | static_cast<std::uint64_t>(right));
}
constexpr renderer::Context::Queue_family::Kind operator&(renderer::Context::Queue_family::Kind left,
                                                          renderer::Context::Queue_family::Kind right)
{
    return static_cast<renderer::Context::Queue_family::Kind>(static_cast<std::uint64_t>(left) & static_cast<std::uint64_t>(right));
}

} // namespace lxf