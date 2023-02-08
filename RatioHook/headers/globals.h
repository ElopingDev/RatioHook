#pragma once
#include <chrono>
#include <Windows.h>

namespace hacks
{
	inline bool chams = false;
	inline float chamsColorVisible[] = { 0.8f, 0.2f, 1.f, };
	inline float chamsColorHidden[] = { 0.4f, 0.4f, 1.f, };
	inline bool bunnyhop = false;
	inline bool triggerbot = false;
	inline int triggerDelay = 0; // 500 ms delay
	inline bool walls = false;
	
}


class CEntity;
namespace globals
{
	inline CEntity* localPlayer = nullptr;
}

