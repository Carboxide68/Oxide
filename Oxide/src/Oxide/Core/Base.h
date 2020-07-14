#pragma once

#include <memory>
#include <signal.h>

#ifdef CO_DEBUG
    #define CO_DEBUGBREAK() raise(SIGTRAP)
    #define CO_ENABLE_ASSERTS
#else
#define CO_DEBUGBREAK()
#endif

#ifdef CO_ENABLE_ASSERTS
    #define CO_ASSERT(x, ...) { if(!(x)) { CO_ERROR("Assertion Failed: {0}", __VA_ARGS__); CO_DEBUGBREAK(); } }
    #define CO_CORE_ASSERT(x, ...) { if(!(x)) { CO_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); CO_DEBUGBREAK(); } }
#else
    #define CO_ASSERT(x, ...)
    #define CO_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

namespace Oxide {
    
    template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}