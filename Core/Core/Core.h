#pragma once

#include <memory>
#include <string>
#include <filesystem>


#ifdef _WIN32
	#ifdef _WIN64
		#define APP_PLATFORM_WINDOWS
	#else
		#error "x86 Builds are not supported!"
	#endif
#else
	#error "Not supported OS!"
#endif


#ifdef APP_DEBUG
	#if defined(APP_PLATFORM_WINDOWS)
		#define _DEBUGBREAK() __debugbreak()
	#elif defined(APP_PLATFORM_LINUX)
		#include <signal.h>
		#define _DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif

	#define APP_ASSERTS

#else
	#define _DEBUGBREAK()
#endif


#ifdef APP_ASSERTS
	#define ASSERT(check) { if(!(check)) { _DEBUGBREAK(); } }
#else
	#define ASSERT(check) 
#endif


#define EXPAND_MACRO(x) x
#define STRINGIFY_MACRO(x) #x
#define BIT(x) (1 << x)
#define APP_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define APP_ENABLE_CUSTOM_TYPES 1


namespace Core
{

	using byte = ::std::uint8_t; // type of size 1 byte

	using int8 = ::std::int8_t;  // 8-bit int
	using int16 = ::std::int16_t; // 16-bit int
	using int32 = ::std::int32_t; // 32-bit int
	using int64 = ::std::int64_t; // 64-bit int

	using uint8 = ::std::uint8_t;  // 8-bit unsigned int
	using uint16 = ::std::uint16_t; // 16-bit unsigned int
	using uint32 = ::std::uint32_t; // 32-bit unsigned int
	using uint64 = ::std::uint64_t; // 64-bit unsigned int

	using String = ::std::string; // string type
	using FilePath = ::std::filesystem::path; // filepath type


	template <typename T>
	using Scope = ::std::unique_ptr<T>;

	template<typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&&... args)
	{
		return ::std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Ref = ::std::shared_ptr<T>;

	template<typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return ::std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#if APP_ENABLE_CUSTOM_TYPES

using byte = Core::byte;

using int8 = Core::int8;
using int16 = Core::int16;
using int32 = Core::int32;
using int64 = Core::int64;

using uint8 = Core::uint8;
using uint16 = Core::uint16;
using uint32 = Core::uint32;
using uint64 = Core::uint64;

using String = Core::String;
using FilePath = Core::FilePath;

template <typename T>
using Scope = Core::Scope<T>;

template <typename T>
using Ref = Core::Ref<T>;

#endif
