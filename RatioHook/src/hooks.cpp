#include "../headers/hooks.h"
#include "../ext/minhook/MinHook.h"
#include "../headers/usercmd.h"
#include "../headers/interfaces.h"
#include "../headers/offsets.h"

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

void InitHooks()
{
    MH_CreateHook((*static_cast<void***>(g_ClientMode))[24], &CreateMoveHook, reinterpret_cast<void**>(&CreateMoveOriginal));
    MH_EnableHook(MH_ALL_HOOKS);
}

void CleanupHooks()
{
    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
}

