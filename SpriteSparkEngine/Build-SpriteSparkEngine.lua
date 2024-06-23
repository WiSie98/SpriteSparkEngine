project "SpriteSparkEngine"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files {
    "Source/**.h", 
    "Source/**.cpp", 
    "Source/**.hpp", 
    "Source/**.cxx",
    "Source/**.c"
    }

   includedirs {
      "Source",
      "Vendor/GLFW/include",
      "Vendor/Vulkan/Include",
      "Vendor/GLM"
   }

   libdirs {
        "Vendor/Vulkan/Lib",
        "Vendor/GLFW/lib-vc2022"
   }

   links {
        "glfw3.lib",
        "vulkan-1.lib"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   pchheader "Sparkpch.h"
   pchsource "Source/Sparkpch.cpp"

   filter "system:windows"
       systemversion "latest"
       defines { }

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




    project "GLFW"
    location "Vendor/GLFW"
    kind "StaticLib"
    language "C"

    targetdir ("../../../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../../../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

    files
    {
        "Vendor/GLFW/include/GLFW/glfw3.h",
        "Vendor/GLFW/include/GLFW/glfw3native.h",
        "Vendor/GLFW/src/glfw_config.h",
        "Vendor/GLFW/src/context.c",
        "Vendor/GLFW/src/init.c",
        "Vendor/GLFW/src/input.c",
        "Vendor/GLFW/src/monitor.c",
        "Vendor/GLFW/src/platform.c",
        "Vendor/GLFW/src/vulkan.c",
        "Vendor/GLFW/src/window.c",
        "Vendor/GLFW/src/osmesa_context.c",
        "Vendor/GLFW/src/egl_context.c",
        "Vendor/GLFW/src/wgl_context.c",
        "Vendor/GLFW/src/win32_init.c",
        "Vendor/GLFW/src/win32_joystick.c",
        "Vendor/GLFW/src/win32_monitor.c",
        "Vendor/GLFW/src/win32_time.c",
        "Vendor/GLFW/src/win32_thread.c",
        "Vendor/GLFW/src/win32_window.c"
    }

    filter "system:windows"
        systemversion "latest"

        defines {
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"