#version 450
#extension GL_KHR_vulkan_glsl : enable

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUbo {
	mat4 projectionMatrix;
} ubo;

layout(set = 1, binding = 1) uniform sampler2D image;

layout(push_constant) uniform Push {
	mat2 transform;
	vec2 offset;
	vec4 color;
} push;

void main() {
	
	vec4 imageColor = texture(image, fragUV).rgba;

	if (imageColor.a < 1.0 || fragColor.a < 1.0) {
        discard;
    }

	outColor = fragColor * imageColor;
}