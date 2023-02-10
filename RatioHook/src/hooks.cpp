#include "../headers/hooks.h"
#include "../ext/minhook/MinHook.h"
#include "../valve/cusercmd.h"
#include "../valve/centity.h"
#include "../headers/offsets.h"
#include "../headers/interfaces.h"
#include "../ext/imgui/imgui.h"
#include "../ext/imgui/imgui_impl_dx9.h"
#include "../ext/imgui/imgui_impl_win32.h"
#include <intrin.h>
#include <chrono>
#include <stdexcept>
#include <iostream>
#include "modules/modules.h"

    
void hooks::InitHooks()
{
    MH_Initialize();
    std::cout << interfaces::clientMode << std::endl;
    std::cout << interfaces::studioRender << std::endl;
    std::cout << interfaces::materialSystem << std::endl;

    // AllocKeyValuesMemory hook
    MH_CreateHook(memory::Get(interfaces::keyValuesSystem, 2), &AllocKeyValuesMemory, reinterpret_cast<void**>(&AllocKeyValuesMemoryOriginal));

    //Gui
    MH_CreateHook(VirtualFunction(gui::device, 42), &EndScene, reinterpret_cast<void**>(&EndSceneOriginal));
    MH_CreateHook(VirtualFunction(gui::device, 16), &Reset, reinterpret_cast<void**>(&ResetOriginal));

    //CreateMove
    MH_CreateHook(memory::Get(interfaces::clientMode, 24), &CreateMove, reinterpret_cast<void**>(&CreateMoveOriginal));

    //DrawModel
    if (!interfaces::studioRender)
    {
        std::cout << "studioRender is pointing null. prevented crash." << std::endl;
    }
    else {
        MH_CreateHook(memory::Get(interfaces::studioRender, 29), &DrawModel, reinterpret_cast<void**>(&DrawModelOriginal));
    }
    MH_EnableHook(MH_ALL_HOOKS);
    gui::DestroyDirectX();
}

long __stdcall hooks::EndScene(IDirect3DDevice9* device) noexcept
{
    static const auto returnAddress = _ReturnAddress();
    const auto result = EndSceneOriginal(device, device);

    if (_ReturnAddress() == returnAddress)
        return result;
    if (!gui::setup)
        gui::SetupMenu(device);
    if (gui::open)
        gui::Render();
    return result;
}


HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    const auto result = ResetOriginal(device, device, params);
    ImGui_ImplDX9_CreateDeviceObjects();
    return result;
}

void* __stdcall hooks::AllocKeyValuesMemory(const std::int32_t size) noexcept
{
    if (const std::uint32_t address = reinterpret_cast<std::uint32_t>(_ReturnAddress());
        address == (memory::allocKeyValuesEngine) ||
        address == (memory::allocKeyValuesClient))
        return nullptr;

    return AllocKeyValuesMemoryOriginal(interfaces::keyValuesSystem, size);
}

bool __stdcall hooks::CreateMove(float frameTime, CUserCmd* cmd) noexcept
{
    globals::localPlayer = interfaces::entityList->GetEntityFromIndex(interfaces::engine->GetLocalPlayerIndex());
    const bool result = CreateMoveOriginal(interfaces::clientMode, frameTime, cmd);

    if (!cmd || !cmd->commandNumber)
        return result;

 
    if (globals::localPlayer && hacks::bunnyhop && globals::localPlayer->IsAlive())
    {
        modules::BunnyHop(cmd);
    }

    // TriggerBot Function
    if (globals::localPlayer && hacks::triggerbot && globals::localPlayer->IsAlive())
    {
        modules::Triggerbot(cmd);
    }

    if (globals::localPlayer && hacks::aimbot && globals::localPlayer->IsAlive())
    {
        modules::Aimbot(cmd);
    }

    if (result)
        interfaces::engine->SetViewAngles(cmd->viewAngles);

    return false;
}




void __stdcall hooks::DrawModel(void* results, const CDrawModelInfo& info, CMatrix3x4* bones, float* flexWeights, float* flexDelayedWeights, const CVector& modelOrigin, const std::int32_t flags) noexcept
{
    
    globals::localPlayer = interfaces::entityList->GetEntityFromIndex(interfaces::engine->GetLocalPlayerIndex());
    if (globals::localPlayer && info.renderable && hacks::chams && globals::localPlayer->IsAlive())
    {
        CEntity* entity = info.renderable->GetIClientUnknown()->GetBaseEntity();

        if (entity && entity->IsPlayer() && entity->GetTeam() != globals::localPlayer->GetTeam())
        {
            static IMaterial* material = interfaces::materialSystem->FindMaterial("debug/debugambientcube");

            // constexpr float hidden[3] = { 0.f, 1.f, 1.f };
            // constexpr float visible[3] = { 1.f, 0.f, 1.f };

            interfaces::studioRender->SetAlphaModulation(1.f);

            // through walls
            if (hacks::walls && hacks::chams) {
                material->SetMaterialVarFlag(IMaterial::IGNOREZ, true);
                interfaces::studioRender->SetColorModulation(hacks::chamsColorHidden);
                interfaces::studioRender->ForcedMaterialOverride(material);
                DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);
            }
            // visible
            material->SetMaterialVarFlag(IMaterial::IGNOREZ, false);
            interfaces::studioRender->SetColorModulation(hacks::chamsColorVisible);
            interfaces::studioRender->ForcedMaterialOverride(material);
            DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);

            //reset
            return interfaces::studioRender->ForcedMaterialOverride(nullptr);
        }
    }

    DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);
}


void hooks::CleanupHooks()
{
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();
}
