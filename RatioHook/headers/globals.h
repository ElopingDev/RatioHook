#pragma once
#include <chrono>
#include <Windows.h>

namespace hacks
{
	inline bool chams = false;
	inline float chamsColorVisible[] = { 0.8f, 0.2f, 1.f, };
	inline float chamsColorHidden[] = { 0.4f, 0.4f, 1.f, };
	inline bool bunnyhop = false;
	inline bool aimbot = false;
	inline bool triggerbot = false;
	inline int triggerDelay = 0; 
	inline bool walls = false;
	inline float fov = 1.7f;
	inline float smoothing = 0.2f;

	
}


class CEntity;
namespace globals
{
	inline CEntity* localPlayer = nullptr;
}

