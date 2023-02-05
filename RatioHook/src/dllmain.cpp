// dllmain.cpp : Defines the entry point for the DLL application.

#include <windows.h>
#include <iostream>
#include <cstdint>
#include <thread>
#include "../headers/offsets.h"
#include <cstdio>
#include <iomanip>
#include "../headers/funcs.h"
#include <chrono>

void HackThread(const HMODULE hModule) noexcept

{
    const auto baseaddress = reinterpret_cast<std::uintptr_t>(GetModuleHandle("client.dll")); // baseaddress declared here so it can be accessed anywhere
    std::uintptr_t localPlayer = 0;
    int fov = 0;

    std::cout << std::endl << std::endl;
    std::cout << std::setw(50) << std::left << R"(
 /\_/\
( o.o ) RatioHook
 > ^ < 
)" << std::endl;
    std::cout << std::endl << std::endl;
    std::cout << "Hold space to BunnyHop" << std::endl;
    std::cout << "Press minus to change FOV" << std::endl;
    std::cout << "Reveal Radar toggled." << std::endl;

    while (!GetAsyncKeyState(VK_END))
    {

    const auto localPlayer = *reinterpret_cast<std::uintptr_t*>(baseaddress + signatures::dwLocalPlayer);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    if (!localPlayer)
    {
        continue;
    }

    const auto health = *reinterpret_cast<std::int32_t*>(localPlayer + netvars::m_iHealth);

    if (!health)
    {
        continue;
    }
    revealRadar(baseaddress);
         const auto flags = *reinterpret_cast<std::int32_t*>(localPlayer + netvars::m_fFlags);
         if (GetAsyncKeyState(VK_SPACE))
         {
             (flags & (1 << 0)) ? *reinterpret_cast<std::uintptr_t*>(baseaddress + signatures::dwForceJump) = 6 : *reinterpret_cast<std::uintptr_t*>(baseaddress + signatures::dwForceJump) = 4;
         }
          
        if (GetAsyncKeyState(VK_SUBTRACT))
        {
            std::cout << "Enter desired FOV Value" << std::endl;
            std::cin >> fov;
            *reinterpret_cast<std::uintptr_t*>(localPlayer + netvars::m_iDefaultFOV) = fov;
        }

    }

   FreeConsole();
   FreeLibraryAndExitThread(hModule, 0);
    
}
    
// EntryPoint
BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
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

