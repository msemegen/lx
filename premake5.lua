-- premake5.lua
workspace "lx"
   configurations { "Debug Windows", "Release Windows" }
   startproject "game"
   
project "game"
   kind "WindowedApp"
   architecture "x64"
   language "C++"
   cppdialect "C++20"
   location "game"
   targetdir "output/game"
   objdir "output/game"
   dependson { "lxf", "lxe" }
   warnings "Extra"
   characterset "MBCS"

   includedirs { ".", "$(GRDKLatest)/GameKit/Include", "$(VULKAN_SDK)/Include" }
   libdirs { "output/lxf/", "output/lxe/", "$(GRDKLatest)/GameKit/Lib/amd64/", "$(VULKAN_SDK)/Lib" }

   files { "game/**.hpp", "game/**.cpp" }

   vpaths {
       ["**"] = { "game/**.hpp", "game/**.cpp" }
   }
   
   filter "configurations:Debug Windows"
      defines { "DEBUG", "LX_AMD64", "LX_ASSERTION", "VK_USE_PLATFORM_WIN32_KHR" }
      symbols "On"
      links { "lxf_d.lib", "vulkan-1.lib" }
      targetname "game_d"
      buildoptions { "/W4" }

   filter "configurations:Release Windows"
      defines { "NDEBUG", "LX_AMD64", "VK_USE_PLATFORM_WIN32_KHR" }
      optimize "On"
      links { "lxf.lib", "vulkan-1.lib" }
      targetname "game"
      buildoptions { "/W4" }

project "lxf"
   kind "staticlib"
   architecture "x64"
   language "C++"
   cppdialect "C++20"
   location ""
   targetdir "output/lxf"
   objdir "output/lxf"
   warnings "Extra"
   characterset "MBCS"
   
   includedirs { ".", "$(VULKAN_SDK)/Include", "externals/" }
   
   files { "lxf/**.hpp", "lxf/**.cpp", "externals/**" }

   vpaths {
       ["**"] = { "lxf/**.hpp", "lxf/**.cpp" }
   }

   filter "configurations:Debug Windows"
      defines { "DEBUG", "LX_AMD64", "LX_ASSERTION", "VK_USE_PLATFORM_WIN32_KHR" }
      symbols "On"
      targetname "lxf_d"
      buildoptions { "/W4" }

   filter "configurations:Release Windows"
      defines { "NDEBUG", "LX_AMD64", "VK_USE_PLATFORM_WIN32_KHR" }
      optimize "On"
      targetname "lxf"
      buildoptions { "/W4" }

project "lxe"
   kind "staticlib"
   architecture "x64"
   language "C++"
   cppdialect "C++20"
   location ""
   targetdir "output/lxe"
   objdir "output/lxe"
   warnings "Extra"
   characterset "MBCS"
   
   includedirs { ".", "$(VULKAN_SDK)/Include", "externals/" }
   
   files { "lxe/**.hpp", "lxe/**.cpp", "externals/**" }

   vpaths {
       ["**"] = { "lxe/**.hpp", "lxe/**.cpp" }
   }

   filter "configurations:Debug Windows"
      defines { "DEBUG", "LX_AMD64", "LX_ASSERTION", "VK_USE_PLATFORM_WIN32_KHR" }
      symbols "On"
      targetname "lxf_d"
	  buildoptions { "/W4" }

   filter "configurations:Release Windows"
      defines { "NDEBUG", "LX_AMD64", "VK_USE_PLATFORM_WIN32_KHR" }
      optimize "On"
      targetname "lxe"
      buildoptions { "/W4" }
