#pragma once
#include <cstdint>
#include <Windows.h>

namespace memory
{
	// call once to scan for all patterns
	void Setup() noexcept;

	template <typename Return, typename ... Args>
	constexpr Return Call(void* _this, std::uintptr_t index, Args ... args) noexcept
	{
		using Function = Return(__thiscall*)(void*, decltype(args)...);
		return (*static_cast<Function**>(_this))[index](_this, args...);
	}

	constexpr void* Get(void* _this, const std::uintptr_t index) noexcept
	{
		return (*static_cast<void***>(_this))[index];
	}


	inline std::uintptr_t RelativeToAbsolute(std::uintptr_t relAddr) noexcept {
		return static_cast<std::uintptr_t>(relAddr + 4 + *reinterpret_cast<std::int32_t*>(relAddr));
	}

	// simple Pattern/AOB/Signature scanner
	std::uint8_t* PatternScan(const char* moduleName, const char* pattern) noexcept;

	// return addresses
	inline std::uintptr_t allocKeyValuesClient = 0;
	inline std::uintptr_t allocKeyValuesEngine = 0;
	inline std::uintptr_t insertIntoTree = 0;

	// functions
	inline std::uint8_t* keyValuesFromString = nullptr;

	// interfaces
	inline std::uint8_t* glowManager = nullptr;
}

template <typename T>
T* GetInterface(const char* name, const char* library)
{

	const auto handle = GetModuleHandle(library);
	if (!handle)
		return nullptr;

	const auto functionAddress = GetProcAddress(handle, "CreateInterface");
	if (!functionAddress)
		return nullptr;

	using Fn = T * (*)(const char*, int*);
	const auto CreateInterface = reinterpret_cast<Fn>(functionAddress);

	return CreateInterface(name, nullptr);
}