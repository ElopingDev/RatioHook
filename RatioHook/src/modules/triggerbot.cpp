#include "modules.h"
#include "../../valve/cvector.h"
#include <winuser.rh>
#include "../../headers/globals.h"
#include "../../headers/interfaces.h"



bool modules::Triggerbot(CUserCmd* cmd) noexcept
{
    static int last_tick = 0;
    static bool did_save = false;
    static bool shotOnce = false;
    if (!GetAsyncKeyState(VK_XBUTTON2)) {
        return false;
    }
    else {
        CVector eyePosition;
        globals::localPlayer->GetEyePosition(eyePosition);

        CVector aimPunch;
        globals::localPlayer->GetAimPunch(aimPunch);

        const CVector dst = eyePosition + CVector{ cmd->viewAngles + aimPunch }.ToVector() * 8192.f;

        CTrace trace;
        interfaces::trace->TraceRay({ eyePosition, dst }, 0x46004009, globals::localPlayer, trace);

        if (!trace.entity || !trace.entity->IsPlayer()) {
            shotOnce = false;
            did_save = false;
            return false;
        }

        if (!trace.entity->IsAlive() || trace.entity->GetTeam() == globals::localPlayer->GetTeam()) {
            return false;
        }


        int tickcount = GetTickCount64();
        if (!did_save && trace.entity->IsPlayer()) {
            // std::cout << "shotOnce1 : " << shotOnce << std::endl;
            did_save = true;
            last_tick = tickcount + hacks::triggerDelay;

        }

        if (tickcount > last_tick) {
            last_tick = 0;
            tickcount = 0;
        }

        if (tickcount == last_tick && did_save && !shotOnce) {
            cmd->buttons |= CUserCmd::IN_ATTACK;
            shotOnce = true;
        }

        if (shotOnce && trace.entity->IsPlayer() && trace.entity->IsAlive() && did_save)
        {
            cmd->buttons |= CUserCmd::IN_ATTACK;

        }
    }
}
    
    
