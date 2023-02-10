#include "modules.h"
#include "../../headers/globals.h"
#include "../../headers/interfaces.h"
#include "../../valve/cmatrix.h"
#include <iostream>
#include "../../headers/offsets.h"

void modules::Aimbot(CUserCmd* cmd) noexcept
{
	globals::localPlayer = interfaces::entityList->GetEntityFromIndex(interfaces::engine->GetLocalPlayerIndex());
	if (!(cmd->buttons & CUserCmd::IN_ATTACK)) {
		return;
	}

	//	if (netvars2::m_bIsDefusing) {
		//	std::cout << "is defusing!" << std::endl;
		//	return;
	//	}

	CEntity* activeWeapon = globals::localPlayer->GetActiveWeapon();

	if (!activeWeapon) {
		return;
	}

	const int weaponType = activeWeapon->GetWeaponType();

	switch (weaponType)
	{
	case CEntity::WEAPONTYPE_MACHINEGUN:
	case CEntity::WEAPONTYPE_RIFLE:
	case CEntity::WEAPONTYPE_PISTOL:
	case CEntity::WEAPONTYPE_SNIPER:
	case CEntity::WEAPONTYPE_SHOTGUN:
	{
		//if (!activeWeapon->GetClip())
		//	std::cout << " player has no ammo ! " << std::endl;
		//	return;

		//if (weaponType == CEntity::WEAPONTYPE_SNIPER)
	//	{
		//	std::cout << "weapon is snipoer" << std::endl;
		
		//	if (!globals::localPlayer->IsScoped())
		//		return;
		//}

		break;
	}

	default:
		return;
	}

	CVector bestAngle{ };
	float bestFov{ hacks::fov };
	for (int i = 1; i <= interfaces::globalvars->maxClients; ++i)
	{

		CEntity* player = interfaces::entityList->GetEntityFromIndex(i);
		if (!player)
			continue;

		if (player->IsDormant() || !player->IsAlive())
			continue;

		if (player->GetTeam() == globals::localPlayer->GetTeam())
			continue;

		

		CMatrix3x4 bones[128];
		if (!player->SetupBones(bones, 128, 256, interfaces::globalvars->currentTime))
		{
			continue;
		}
		CVector localEyePosition;
		globals::localPlayer->GetEyePosition(localEyePosition);

		CVector aimPunch{ };
		switch (weaponType)
		{
		case CEntity::WEAPONTYPE_MACHINEGUN:
		case CEntity::WEAPONTYPE_RIFLE:
		case CEntity::WEAPONTYPE_SUBMACHINEGUN:
			globals::localPlayer->GetAimPunch(aimPunch);

		}


		CTrace trace;
		interfaces::trace->TraceRay(
			CRay{ localEyePosition, bones[8].Origin() },
			MASK_SHOT,
			{ globals::localPlayer },
			trace
		);
		//if (!trace.entity || trace.fraction < 0.97f)
		//{
		//	std::cout << "flagged weird check i dont understand" << std::endl;

		//	return;
		//}
		CVector enemyAngle
		{
			(bones[8].Origin() - localEyePosition).ToAngle() - (cmd->viewAngles + aimPunch)
		};

		if (const float fov = std::hypot(enemyAngle.x, enemyAngle.y); fov < bestFov)
		{
			bestFov = fov;
			bestAngle = enemyAngle;
		}
	}
	cmd->viewAngles = cmd->viewAngles + bestAngle.Scale(hacks::smoothing);

}