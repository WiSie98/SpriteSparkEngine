workspace "SpriteSparkProject"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "SpriteSparkGame"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

   -- Custom build step to compile GLSL shaders with glslc
   local glslc_path = "C:/VulkanSDK/Bin/glslc.exe"

   filter { "files:**.vert", "files:**.frag" }
      buildmessage "Compiling GLSL shaders with glslc..."
      buildcommands {
         glslc_path .. " %{file.basename}.vert -o %{file.basename}.vert.spv",
         glslc_path .. " %{file.basename}.frag -o %{file.basename}.frag.spv"
      }
      --buildoutputs { "%{file.directory}/Shaders/%{file.basename}.spv" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "SpriteSparkEngine"
	include "SpriteSparkEngine/Build-SpriteSparkEngine.lua"
group ""

include "SpriteSparkGame/Build-SpriteSparkGame.lua"
