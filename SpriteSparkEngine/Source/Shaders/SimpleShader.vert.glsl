#version 450

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragUV;

layout(set = 0, binding = 0) uniform GlobalUbo {
	mat4 projectionMatrix;
} ubo;

layout(push_constant) uniform Push {
	mat2 transform;
	vec2 offset;
	vec4 color;
} push;

void main() {
	gl_Position = ubo.projectionMatrix * vec4(push.transform * position + push.offset, 0.0, 1.0);
	fragColor = push.color;
	fragUV = uv;
}