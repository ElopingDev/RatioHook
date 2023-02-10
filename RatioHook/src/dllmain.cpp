// dllmain.cpp : Defines the entry point for the DLL application.

#include <windows.h>
#include <iostream>
#include <cstdint>
#include <thread>
#include <cstdio>
#include <iomanip>
#include <chrono>

#include "../headers/gui.h"
#include "../headers/hooks.h"
#include "../headers/netvar.h"
#include "../headers/offsets.h"
#include "../valve/centity.h"
#include "../ext/minhook/MinHook.h"
#include "../valve/cusercmd.h"

DWORD WINAPI HackThread(LPVOID instance)
{
    std::uintptr_t localPlayer = 0;
    int fov = 0;
    client;
    entityList;
    const auto baseaddress = reinterpret_cast<std::uintptr_t>(GetModuleHandle("client.dll")); // baseaddress declared here so it can be accessed anywhere

    
    memory::Setup();
    interfaces::SetupInterfaces();
    gui::Setup();
    hooks::InitHooks();
    //netvars::Setup();

    //GUICon();
    
    while (!GetAsyncKeyState(VK_END))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

   std::cout << "Uninjected. You can now close this console" << std::endl;
   hooks::CleanupHooks();
   gui::Destroy();
   FreeConsole();
   FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
    
}
    
// EntryPoint
BOOL APIENTRY DllMain(HINSTANCE instance, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    {
        if (ul_reason_for_call == 1)
        {
            MH_Initialize();
            AllocConsole();
            FILE* console_in;
            FILE* console_out;
            freopen_s(&console_out, "CONOUT$", "w", stdout);
            freopen_s(&console_in, "CONIN$", "r", stdin);
            HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hStdout, 0x0C);
            DisableThreadLibraryCalls(instance);

            const HANDLE thread = CreateThread(
                nullptr,
                NULL,
                HackThread,
                instance,
                NULL,
                nullptr
            );
            if (thread)
                MessageBoxA(NULL, "RatioHook Injected", "Injection", NULL);
                CloseHandle(thread);
        }
        return TRUE;


    }
}