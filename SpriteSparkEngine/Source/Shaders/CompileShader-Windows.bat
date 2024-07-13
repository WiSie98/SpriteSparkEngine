forfiles /s /m *.vert.glsl /c "cmd /c %VULKAN_SDK%/Bin/glslc.exe -fshader-stage=vert @path -o @fname.spv"
forfiles /s /m *.frag.glsl /c "cmd /c %VULKAN_SDK%/Bin/glslc.exe -fshader-stage=frag @path -o @fname.spv"
pause