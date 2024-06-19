workspace "SpriteSparkProject"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "SpriteSparkGame"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "SpriteSparkEngine"
	include "SpriteSparkEngine/Build-SpriteSparkEngine.lua"
group ""

include "SpriteSparkGame/Build-SpriteSparkGame.lua"
