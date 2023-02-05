// dllmain.cpp : Defines the entry point for the DLL application.

#include <windows.h>
#include <iostream>
#include <cstdint>
#include <thread>
#include <cstdio>
#include <iomanip>
#include <chrono>

#include "../headers/netvar.h"
#include "../headers/offsets.h"
#include "../headers/funcs.h"
#include "../headers/interfaces.h"

void HackThread(const HMODULE hModule) noexcept

{
    std::uintptr_t localPlayer = 0;
    int fov = 0;
    client = GetInterface<IClient>("VClient018", "client.dll");
    entityList = GetInterface<IClientEntityList>("VClientEntityList003", "client.dll");
    const auto baseaddress = reinterpret_cast<std::uintptr_t>(GetModuleHandle("client.dll")); // baseaddress declared here so it can be accessed anywhere
    
    SetupNetvars();
    GUICon();

    while (!GetAsyncKeyState(VK_END))
    {
        // // // // // // //  
        // Variables checks that needs to be updated all the time
        const auto localPlayer = *reinterpret_cast<std::uintptr_t*>(baseaddress + signatures::dwLocalPlayer);
        if (!localPlayer)
        {
            continue;
        }
        const auto health = *reinterpret_cast<std::int32_t*>(localPlayer + netvars2::m_iHealth);
        if (!health)
        {
            continue;
        }
        const auto flags = *reinterpret_cast<std::int32_t*>(localPlayer + netvars2::m_fFlags);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // Loading hacks functions
        RevealRadar();
        if (GetAsyncKeyState(VK_SPACE))
        {
             BunnyHop(flags, baseaddress);
        }
          
        if (GetAsyncKeyState(VK_SUBTRACT))
        {
            FovChanger(fov, localPlayer);
        }

        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            PrintHP();
        }

    }
   std::cout << "Uninjected. You can now close this console" << std::endl;
   FreeConsole();
   FreeLibraryAndExitThread(hModule, 0);
    
}
    
// EntryPoint
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    {
        if (ul_reason_for_call == 1)
        {
            AllocConsole();
            FILE* console_in;
            FILE* console_out;
            freopen_s(&console_out, "CONOUT$", "w", stdout);
            freopen_s(&console_in, "CONIN$", "r", stdin);
            HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hStdout, 0x0C);
            DisableThreadLibraryCalls(hModule);

            const auto thread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(HackThread), hModule, 0, nullptr);
            if (thread)
                MessageBoxA(NULL, "RatioHook Injected", "Injection", NULL);
                CloseHandle(thread);
        }
        return TRUE;
    }
}

