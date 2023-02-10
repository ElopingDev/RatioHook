#pragma once
#include "../headers/datatable.h"
#include "../headers/netvar.h"
#include "../headers/memory.h"
#include "cvector.h"
#include "cmatrix.h"
#include "cclientclass.h"

class CEntity
{

public:
	enum EFlags : std::int32_t
	{
		FL_ONGROUND = (1 << 0),
		FL_DUCKING = (1 << 1),
		FL_ANIMDUCKING = (1 << 2),
		FL_WATERJUMP = (1 << 3),
		FL_ONTRAIN = (1 << 4),
		FL_INRAIN = (1 << 5),
		FL_FROZEN = (1 << 6),
		FL_ATCONTROLS = (1 << 7),
		FL_CLIENT = (1 << 8),
		FL_FAKECLIENT = (1 << 9),
		FL_INWATER = (1 << 10),
		FL_FLY = (1 << 11),
		FL_SWIM = (1 << 12),
		FL_CONVEYOR = (1 << 13),
		FL_NPC = (1 << 14),
		FL_GODMODE = (1 << 15),
		FL_NOTARGET = (1 << 16),
		FL_AIMTARGET = (1 << 17),
		FL_PARTIALGROUND = (1 << 18),
		FL_STATICPROP = (1 << 19),
		FL_GRAPHED = (1 << 20),
		FL_GRENADE = (1 << 21),
		FL_STEPMOVEMENT = (1 << 22),
		FL_DONTTOUCH = (1 << 23),
		FL_BASEVELOCITY = (1 << 24),
		FL_WORLDBRUSH = (1 << 25),
		FL_OBJECT = (1 << 26),
		FL_KILLME = (1 << 27),
		FL_ONFIRE = (1 << 28),
		FL_DISSOLVING = (1 << 29),
		FL_TRANSRAGDOLL = (1 << 30),
		FL_UNBLOCKABLE_BY_PLAYER = (1 << 31)
	};

	enum EWeaponType : int
	{
		WEAPONTYPE_KNIFE = 0,
		WEAPONTYPE_PISTOL = 1,
		WEAPONTYPE_SUBMACHINEGUN = 2,
		WEAPONTYPE_RIFLE = 3,
		WEAPONTYPE_SHOTGUN = 4,
		WEAPONTYPE_SNIPER = 5,
		WEAPONTYPE_MACHINEGUN = 6,
		WEAPONTYPE_C4 = 7,
		WEAPONTYPE_PLACEHOLDER = 8,
		WEAPONTYPE_GRENADE = 9,
		WEAPONTYPE_HEALTHSHOT = 11,
		WEAPONTYPE_FISTS = 12,
		WEAPONTYPE_BREACHCHARGE = 13,
		WEAPONTYPE_BUMPMINE = 14,
		WEAPONTYPE_TABLET = 15,
		WEAPONTYPE_MELEE = 16
	};

public: // networkable virtual functions (+0x8)
	constexpr CClientClass* GetClientClass() noexcept
	{
		return memory::Call<CClientClass*>(this + 0x8, 2);
	}

public:
	constexpr std::int32_t GetTeam() noexcept
	{
		return memory::Call<std::int32_t>(this, 88);
	}

	constexpr CEntity* GetActiveWeapon() noexcept
	{
		return memory::Call<CEntity*>(this, 268);
	}

	constexpr int GetWeaponType() noexcept
	{
		return memory::Call<int>(this, 455);
	}

	constexpr bool IsAlive() noexcept
	{
		return memory::Call<bool>(this, 156);
	}

	constexpr bool IsPlayer() noexcept
	{
		return memory::Call<bool>(this, 158);
	}

	constexpr bool IsDormant() noexcept
	{
		return memory::Call<bool>(this + 0x8, 9);
	}

	constexpr void GetEyePosition(CVector& eyePositionOut) noexcept
	{
		memory::Call<void, CVector&>(this, 285, eyePositionOut);
	}

	constexpr void GetAimPunch(CVector& aimPunchOut) noexcept
	{
		memory::Call<void, CVector&>(this, 346, aimPunchOut);
	}

	constexpr bool SetupBones(CMatrix3x4* out, std::int32_t max, std::int32_t mask, float currentTime) noexcept
	{
		return memory::Call<bool>(this + 0x4, 13, out, max, mask, currentTime);
	}

	
	// Netvars
	NETVAR(GetFlags, "CBasePlayer->m_fFlags", std::int32_t);
	NETVAR(Spotted, "CBaseEntity->m_bSpotted", bool)
	NETVAR(GetHealth, "CBaseEntity->m_iHealth", int)
	NETVAR(IsDefusing, "DT_CSPlayer->m_bIsDefusing", bool)
	NETVAR(GetClip, "CBaseCombatWeapon->m_iClip1", int)
	NETVAR(IsScoped, "CCSPlayer->m_bIsScoped", bool)
	NETVAR(HasGunGameImmunity, "CCSPlayer->m_bGunGameImmunity", bool)



		const int& GetHP() const noexcept
	{
		return *reinterpret_cast<int*>(std::uintptr_t(this) + 0x100);
	}


};

class IClientEntityList
{
public:
	constexpr CEntity* GetEntityFromIndex(const std::int32_t index) noexcept
	{
		return memory::Call<CEntity*>(this, 3, index);
	}
	// Get IClientNetworkable interface for specified entity
	virtual void* GetClientNetworkable(int entnum) = 0;
	virtual void* GetClientNetworkableFromHandle(unsigned long hEnt) = 0;
	virtual void* GetClientUnknownFromHandle(unsigned long hEnt) = 0;

	// NOTE: This function is only a convenience wrapper.
	// It returns GetClientNetworkable( entnum )->GetIClientEntity().
	virtual CEntity* GetClientEntity(int entnum) = 0;
	virtual CEntity* GetClientEntityFromHandle(unsigned long hEnt) = 0;
	
	// Returns number of entities currently in use
	virtual int					NumberOfEntities(bool bIncludeNonNetworkable) = 0;

	// Returns highest index actually used
	virtual int					GetHighestEntityIndex(void) = 0;

	// Sizes entity list to specified size
	virtual void				SetMaxEntities(int maxents) = 0;
	virtual int					GetMaxEntities() = 0;
};

class IClient
{
private:
	virtual void Unused_0() = 0;
	virtual void Unused_1() = 0;
	virtual void Unused_2() = 0;
	virtual void Unused_3() = 0;
	virtual void Unused_4() = 0;
	virtual void Unused_5() = 0;
	virtual void Unused_6() = 0;
	virtual void Unused_7() = 0;

public:
	virtual ClientClass* GetAllClasses(void) = 0;
};

class IClientUnknown
{
public:
	constexpr CEntity* GetBaseEntity() noexcept
	{
		return memory::Call<CEntity*>(this, 7);
	}
};

class IClientRenderable
{
public:
	constexpr IClientUnknown* GetIClientUnknown() noexcept
	{
		return memory::Call<IClientUnknown*>(this, 0);
	}
};

inline IClientEntityList* entityList = nullptr;
inline IClient* client = nullptr;
