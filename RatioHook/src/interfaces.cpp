#include "../headers/interfaces.h"
#include <Windows.h>


void interfaces::SetupInterfaces() noexcept
{
    entityList = GetInterface<IClientEntityList>("VClientEntityList003", "client.dll");
    engine = GetInterface<IEngineClient>("VEngineClient014", "engine.dll");
    client = GetInterface<IClient>("VClient018", "client.dll");
    trace = GetInterface<IEngineTraceClient>("EngineTraceClient004", "engine.dll");
    clientMode = **reinterpret_cast<void***>((*reinterpret_cast<unsigned int**>(client))[10] + 5);
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