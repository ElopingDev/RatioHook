#pragma once
#include "funcs.h"
#include "datatable.h"
#include "netvar.h"
class CEntity
{
public:
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

inline IClientEntityList* entityList = nullptr;
inline IClient* client = nullptr;

