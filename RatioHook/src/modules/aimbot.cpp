#include "modules.h"
#include "../../headers/globals.h"
#include "../../headers/interfaces.h"

void modules::Aimbot(CUserCmd* cmd) noexcept
{
	if (!(cmd->buttons & CUserCmd::IN_ATTACK))
		return;

	if (globals::localPlayer->IsDefusing())
		return;

	CEntity* activeWeapon = globals::localPlayer->GetActiveWeapon();

	if (!activeWeapon)
		return;

	const int weaponType = activeWeapon->GetWeaponType();

	switch (weaponType)
	{
	case CEntity::WEAPONTYPE_MACHINEGUN:
	default:
		break;
	}
}