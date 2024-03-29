#pragma once
#include "../headers/memory.h"
#include "cvector.h"
#include "cusercmd.h"

class IEngineClient
{
public:
	constexpr std::uint32_t GetLocalPlayerIndex() noexcept
	{
		return memory::Call<std::int32_t>(this, 12);
	}

	constexpr void SetViewAngles(const CVector& viewAngles) noexcept
	{
		memory::Call<void, const CVector&>(this, 19, viewAngles);
	}
};