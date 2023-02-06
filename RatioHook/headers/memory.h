#pragma once
#include <cstdint>

namespace memory
{
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