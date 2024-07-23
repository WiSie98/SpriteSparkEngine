project "SpriteSparkGame"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files {
    "Source/**.h", 
    "Source/**.cpp", 
    "Source/**.hpp", 
    "Source/**.cxx",
    "Source/**.c",
    "Shaders/**.glsl",
    "Shaders/**.vert",
    "Shaders/**.frag"
    }

   includedirs {
      "Source",
      "Shaders",

	  -- Include SpriteSparkEngine
	  "../SpriteSparkEngine/Source",
      "../SpriteSparkEngine/Vendor/Vulkan/Include",
      "../SpriteSparkEngine/Vendor/GLFW/include",
      "../SpriteSparkEngine/Vendor/OpenAL/include",
      "../SpriteSparkEngine/Vendor/GLM"
   }

   libdirs {
       "../SpriteSparkEngine/Vendor/Vulkan/Lib",
       "../SpriteSparkEngine/Vendor/OpenAL/libs/Win64",
       "../SpriteSparkEngine/Vendor/GLFW/lib-vc2022"
    }

   links {
      "SpriteSparkEngine",
      "glfw3.lib",
      "vulkan-1.lib",
      "OpenAL32.lib"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"
