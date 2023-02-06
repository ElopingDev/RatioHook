#pragma once
#include "../headers/hacks.h"
#include "../headers/datatable.h"
#include "../headers/netvar.h"
#include "../headers/memory.h"
#include "cvector.h"
class CEntity
{
public:
	constexpr std::int32_t GetTeam() noexcept
	{
		return memory::Call<std::int32_t>(this, 88);
	}

	constexpr bool IsAlive() noexcept
	{
		return memory::Call<bool>(this, 156);
	}

	constexpr bool IsPlayer() noexcept
	{
		return memory::Call<bool>(this, 158);
	}

	constexpr void GetEyePosition(CVector& eyePositionOut) noexcept
	{
		memory::Call<void, CVector&>(this, 285, eyePositionOut);
	}

	constexpr void GetAimPunch(CVector& aimPunchOut) noexcept
	{
		memory::Call<void, CVector&>(this, 346, aimPunchOut);
	}

	// Netvars
	NETVAR(Spotted, "CBaseEntity->m_bSpotted", bool)
	NETVAR(GetHealth, "CBaseEntity->m_iHealth", int)
	NETVAR(Flags, "CBaseEntity->m_fFlags", int)
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

