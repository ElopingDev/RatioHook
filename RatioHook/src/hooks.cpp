#include "../headers/hooks.h"
#include "../ext/minhook/MinHook.h"
#include "../valve/cusercmd.h"
#include "../valve/centity.h"
#include "../headers/offsets.h"

void hooks::InitHooks()
{
    MH_Initialize();
    std::cout << interfaces::clientMode << std::endl;
    std::cout << CreateMove << std::endl;
    std::cout << CreateMoveOriginal << std::endl;

    MH_CreateHook(memory::Get(interfaces::clientMode, 24), &CreateMove, reinterpret_cast<void**>(&CreateMoveOriginal));
    MH_EnableHook(MH_ALL_HOOKS);
}


bool __stdcall hooks::CreateMove(float frameTime, UserCmd* cmd) noexcept
{
    const bool result = CreateMoveOriginal(interfaces::clientMode, frameTime, cmd);

    if (!cmd || !cmd->commandNumber)
        return result;

    if (result)
        interfaces::engine->SetViewAngles(cmd->viewAngles);

    globals::localPlayer = interfaces::entityList->GetEntityFromIndex(interfaces::engine->GetLocalPlayerIndex());
    if (globals::localPlayer || !globals::localPlayer->IsAlive())
    {
        if (!(*reinterpret_cast<int*>(globals::localPlayer + netvars2::m_fFlags) & 1))
        {
            cmd->buttons &= ~IN_JUMP;
        }

        if (!GetAsyncKeyState(VK_XBUTTON2))
            return false; 
        else {
            CVector eyePosition;
            globals::localPlayer->GetEyePosition(eyePosition);

            CVector aimPunch;
            globals::localPlayer->GetAimPunch(aimPunch);

            const CVector dst = eyePosition + CVector{ cmd->viewAngles + aimPunch }.ToVector() * 8192.f;

            CTrace trace;
            interfaces::trace->TraceRay({ eyePosition, dst }, 0x46004009, globals::localPlayer, trace);

            if (!trace.entity || !trace.entity->IsPlayer())
                return false;

            if (!trace.entity->IsAlive() || trace.entity->GetTeam() == globals::localPlayer->GetTeam())
                return false;

            cmd->buttons |= IN_ATTACK;
            return false;
        }


    }
    return false;

}


void hooks::CleanupHooks()
{
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();
}

