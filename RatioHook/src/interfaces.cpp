#include "../headers/interfaces.h"
#include <Windows.h>
#include "../valve/istudiorenderer.h"
#include "../valve/iglobalvars.h"


void interfaces::SetupInterfaces() noexcept
{
    entityList = GetInterface<IClientEntityList>("VClientEntityList003", "client.dll");
    engine = GetInterface<IEngineClient>("VEngineClient014", "engine.dll");
    client = GetInterface<IClient>("VClient018", "client.dll");
    bclient = Capture<IBaseClientDLL>("VClient018", "client.dll");
    trace = GetInterface<IEngineTraceClient>("EngineTraceClient004", "engine.dll");
    clientMode = **reinterpret_cast<void***>((*reinterpret_cast<unsigned int**>(client))[10] + 5);
    studioRender = GetInterface<IStudioRender>("VStudioRender026", "studiorender.dll");
    materialSystem = GetInterface<IMaterialSystem>("VMaterialSystem080", "materialsystem.dll");
    globalvars = **reinterpret_cast<IGlobalVars***>((*reinterpret_cast<uintptr_t**>(client))[11] + 10);
   // modelRender = GetInterface<IVModelRender>("VEngineModel016", "engine.dll");

    // get the exported KeyValuesSystem function
    if (const HINSTANCE handle = GetModuleHandle("vstdlib.dll"))
        // set our pointer by calling the function
        keyValuesSystem = reinterpret_cast<void* (__cdecl*)()>(GetProcAddress(handle, "KeyValuesSystem"))();

}



template <typename T>
T* interfaces::Capture(const char* name, const char* lib) noexcept
{
    const HINSTANCE handle = GetModuleHandle(lib);

    if (!handle)
        return nullptr;

    using Function = T * (*)(const char*, int*);
    Function CreateInterface = reinterpret_cast<Function>(GetProcAddress(handle, "CreateInterface"));
}