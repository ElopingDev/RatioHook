#pragma once

#include "cclientclass.h"
#include "../headers/memory.h"

class IBaseClientDLL
{
public:
	constexpr CClientClass* GetAllClasses() noexcept
	{
		return memory::Call<CClientClass*>(this, 8);
	}
};