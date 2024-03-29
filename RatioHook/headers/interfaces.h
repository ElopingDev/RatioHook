#pragma once
#include "globals.h"
#include "../valve/centity.h"
#include "../valve/iengineclient.h"
#include "../valve/ienginetraceclient.h"
#include "../valve/istudiorenderer.h"
#include "../valve/imaterialsystem.h"
#include "../valve/iglobalvars.h"
#include "../valve/ibaseclientdll.h"


namespace interfaces
{
	void SetupInterfaces() noexcept;

	template<typename T>
	T* Capture(const char* name, const char* lib) noexcept;

	inline IClientEntityList* entityList = nullptr;
	inline IEngineClient* engine = nullptr;
	inline IEngineTraceClient* trace = nullptr;
	inline IMaterialSystem* materialSystem = nullptr;
	inline IStudioRender* studioRender = nullptr;
	inline IGlobalVars* globalvars = nullptr;
	inline IBaseClientDLL* bclient = nullptr;

	inline void* client = nullptr;
	inline void* clientMode = nullptr;



	inline void* keyValuesSystem = nullptr;

}