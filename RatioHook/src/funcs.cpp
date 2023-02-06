
#include <windows.h>
#include <iostream>
#include <iomanip>

#include "../headers/offsets.h"
#include "../headers/interfaces.h"
#include "../headers/funcs.h"



void RevealRadar()
{
    for (auto i = 1; i <= 64; i++)
    {
        const auto entity = entityList->GetClientEntity(i);
        if (!entity)
            continue;
        entity->Spotted() = true;
    }
}

void BunnyHop(int flags, unsigned int baseaddress) {
    (flags & (1 << 0)) ? *reinterpret_cast<std::uintptr_t*>(baseaddress + signatures::dwForceJump) = 6 : *reinterpret_cast<std::uintptr_t*>(baseaddress + signatures::dwForceJump) = 4;
}

void FovChanger(int fov, uintptr_t localPlayer) {
    std::cout << "Enter desired FOV Value" << std::endl;
    std::cin >> fov;
    *reinterpret_cast<std::uintptr_t*>(localPlayer + netvars2::m_iDefaultFOV) = fov;
}

void PrintHP() {
    std::cout << "============" << std::endl;
    for (auto i = 1; i <= 64; i++)
    {
        const auto entity = entityList->GetClientEntity(i);

        if (!entity)
            continue;
    std::cout << " Entity " << i << " health = " << entity->GetHP() << '\n';
    }
    std::cout << "============" << std::endl;
    std::cout << "Printed entites health" << std::endl;
}

void GUICon() {
    std::cout << std::endl << std::endl;
    std::cout << std::setw(50) << std::left << R"(
 /\_/\
( o.o ) RatioHook
 > ^ < 
)" << std::endl;
    std::cout << std::endl << std::endl;
    std::cout << "Hold space to BunnyHop" << std::endl;
    std::cout << "Press minus to change FOV" << std::endl;
    std::cout << "Reveal Radar toggled." << std::endl;
}

