-- premake5.lua
workspace "lx"
   configurations { "Debug Windows", "Release Windows" }
   startproject "game"
   
project "game"
   kind "WindowedApp"
   architecture "x64"
   language "C++"
   cppdialect "C++23"
   location "game"
   targetdir "output/game"
   objdir "output/game"
   dependson { "lx" }
   warnings "Extra"
   characterset "MBCS"

   includedirs { ".", "$(GRDKLatest)/GameKit/Include", "$(VULKAN_SDK)/Include" }
   libdirs { "output/lx/", "$(GRDKLatest)/GameKit/Lib/amd64/", "$(VULKAN_SDK)/Lib" }

   files { "game/**.hpp", "game/**.cpp" }

   vpaths {
       ["**"] = { "game/**.hpp", "game/**.cpp" }
   }

   os.mkdir("output/game/assets/shaders")
   
   filter "configurations:Debug Windows"
      defines { "DEBUG", "LX_AMD64", "LX_ASSERTION", "VK_USE_PLATFORM_WIN32_KHR", "VK_NO_PROTOTYPES", "WIN32_LEAN_AND_MEAN", "NOMINMAX" }
      symbols "On"
      links { "lx_d.lib", "vulkan-1.lib" }
      targetname "game_d"
      debugdir "output/game"

   filter "configurations:Release Windows"
      defines { "NDEBUG", "LX_AMD64", "VK_USE_PLATFORM_WIN32_KHR", "VK_NO_PROTOTYPES", "WIN32_LEAN_AND_MEAN", "NOMINMAX" }
      optimize "On"
      links { "lx.lib" }
      targetname "game"

project "lx"
   kind "staticlib"
   architecture "x64"
   language "C++"
   cppdialect "C++23"
   location "lx"
   targetdir "output/lx"
   objdir "output/lx"
   warnings "Extra"
   characterset "MBCS"
   
   includedirs { ".", "$(VULKAN_SDK)/Include", "externals/" }
   
   files { "lx/**.hpp", "lx/**.h", "lx/**.cpp", "externals/**", "lx/**.md" }

   vpaths {
       ["**"] = { "lx/**.hpp", "lx/**.h", "lx/**.cpp" }
   }

   filter "configurations:Debug Windows"
      defines { "DEBUG", "LX_AMD64", "LX_ASSERTION", "VK_USE_PLATFORM_WIN32_KHR", "VK_NO_PROTOTYPES", "WIN32_LEAN_AND_MEAN", "NOMINMAX" }
      symbols "On"
      targetname "lx_d"

   filter "configurations:Release Windows"
      defines { "NDEBUG", "LX_AMD64", "VK_USE_PLATFORM_WIN32_KHR", "VK_NO_PROTOTYPES", "WIN32_LEAN_AND_MEAN", "NOMINMAX" }
      optimize "On"
      targetname "lx"

project "tests"
   kind "ConsoleApp"
   architecture "x64"
   language "C++"
   cppdialect "C++23"
   location "tests"
   targetdir "output/tests"
   objdir "output/tests"
   dependson { "lx" }
   warnings "Extra"
   characterset "MBCS"
   
   includedirs { ".", "tests/externals/Catch2/src/" }
   files { "tests/**.hpp", "tests/**.cpp", "externals/**" }
   vpaths {
       ["**"] = { "tests/**.hpp", "tests/**.cpp", "externals/**.cpp", "externals/**.c", "externals/**.hpp", "externals/**.h" }
   }
   
   filter "configurations:Debug Windows"
      defines { "DEBUG", "LX_AMD64", "LX_ASSERTION", "VK_USE_PLATFORM_WIN32_KHR", "VK_NO_PROTOTYPES", "CATCH_AMALGAMATED_CUSTOM_MAIN" }
      symbols "On"
      targetname "tests_d"

   filter "configurations:Release Windows"
      defines { "NDEBUG", "LX_AMD64", "VK_USE_PLATFORM_WIN32_KHR", "VK_NO_PROTOTYPES", "CATCH_AMALGAMATED_CUSTOM_MAIN" }
      optimize "On"
      targetname "tests"