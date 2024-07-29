#pragma once

#define BIT(x) (1 << x)

#ifndef DIST
#define SP_ENABLE_ASSERTS 1
#endif

#ifdef SP_ENABLE_ASSERTS

#define SP_ASSERT(x, ...) { if(!(x)) { SP_APP_ERROR("Assertion Failed: ", __VA_ARGS__); __debugbreak(); } }
#define SP_CORE_ASSERT(x, ...) { if(!(x)) { SP_CORE_ERROR("Assertion Failed: ", __VA_ARGS__); __debugbreak(); } }

#else

#define SP_ASSERT(x, ...)
#define SP_CORE_ASSERT(x, ...)

#endif

struct Rect {
	float x, y, width, height;

	Rect() :x(0), y(0), width(0), height(0) {}
	Rect(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {}

	void getUVs(float& u_min, float& v_min, float& u_max, float& v_max, float textureWidth, float textureHeight) {
		u_min = x / textureWidth;
		v_min = y / textureHeight;
		u_max = (x + width) / textureWidth;
		v_max = (y + height) / textureHeight;
	}
};