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