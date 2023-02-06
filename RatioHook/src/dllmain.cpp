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
#include "../ext/minhook/MinHook.h"
#include "../headers/usercmd.h"
static void* g_Client = GetInterface<IClient>("VClient018", "client.dll");
static void* g_ClientMode = **reinterpret_cast<void***>((*reinterpret_cast<unsigned int**>(g_Client))[10] + 5);
using CreateMove = bool(__thiscall*)(void*, float, UserCmd*);
static CreateMove CreateMoveOriginal = nullptr;

bool __stdcall CreateMoveHook(float frameTime, UserCmd* cmd)
{
    const bool result = CreateMoveOriginal(g_ClientMode, frameTime, cmd);

    if (!cmd || !cmd->commandNumber)
        return result;

    static unsigned int client = reinterpret_cast<unsigned int>(GetModuleHandle("client.dll"));
    const unsigned int localPlayer = *reinterpret_cast<unsigned int*>(client + signatures::dwLocalPlayer);

    if (localPlayer)
    {
        if (!(*reinterpret_cast<int*>(localPlayer + netvars2::m_fFlags) & 1))
        {
            cmd->buttons &= ~IN_JUMP;
        }
    }
    return false;
}

DWORD WINAPI HackThread(LPVOID instance)
{
    MH_CreateHook((*static_cast<void***>(g_ClientMode))[24], &CreateMoveHook, reinterpret_cast<void**>(&CreateMoveOriginal));
    MH_EnableHook(MH_ALL_HOOKS);

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
        //RevealRadar();
        if (GetAsyncKeyState(VK_SPACE))
        {
            
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
   MH_DisableHook(MH_ALL_HOOKS);
   MH_RemoveHook(MH_ALL_HOOKS);
   MH_Uninitialize;
   FreeConsole();
   FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
    
}
    
// EntryPoint
BOOL APIENTRY DllMain(HINSTANCE instance, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    {
        if (ul_reason_for_call == 1)
        {
            MessageBoxA(NULL, "Attempting to Initialize Minhook", "Injection", NULL);
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

