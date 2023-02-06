#pragma once
#include "interfaces.h"
#include "../valve/cmatrix.h"

namespace hooks
{
	void InitHooks();
	void CleanupHooks();

	using CreateMoveFunction = bool(__thiscall*)(void*, float, UserCmd*) noexcept;
	inline CreateMoveFunction CreateMoveOriginal = nullptr;
	bool __stdcall CreateMove(float frameTime, UserCmd* cmd) noexcept;

	using DrawModelFn = void(__thiscall*)(
		void*,
		void*,
		const CDrawModelInfo&,
		CMatrix3x4*,
		float*,
		float*,
		const CVector&,
		const std::int32_t
		) noexcept;
	inline DrawModelFn DrawModelOriginal = nullptr;

	void __stdcall DrawModel(
		void* results,
		const CDrawModelInfo& info,
		CMatrix3x4* bones,
		float* flexWeights,
		float* flexDelayedWeights,
		const CVector& modelOrigin,
		const std::int32_t flags

	) noexcept;
}