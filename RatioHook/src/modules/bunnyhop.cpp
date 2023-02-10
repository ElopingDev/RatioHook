#include "modules.h"
#include "../../headers/globals.h"
#include "../../headers/interfaces.h"
#include "../../headers/offsets.h"
#include "../../valve/cusercmd.h"

void modules::BunnyHop(CUserCmd* cmd) noexcept 
{
        if (!(*reinterpret_cast<int*>(globals::localPlayer + netvars2::m_fFlags) & 1))
            cmd->buttons &= ~CUserCmd::IN_JUMP;  
}