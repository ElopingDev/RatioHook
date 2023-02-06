#pragma once
#include <iostream>
#include <Windows.h>

void RevealRadar();
void BunnyHop(int flags, unsigned int baseaddress);
void FovChanger(int fov, uintptr_t localPlayer);
void PrintHP();
void GUICon();


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

