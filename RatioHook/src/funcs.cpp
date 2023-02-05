
#include <windows.h>
#include <iostream>
#include <cstdint>
#include <thread>
#include "../headers/offsets.h"
#include <cstdio>
#include <iomanip>
#include <chrono>

void revealRadar(std::uintptr_t baseaddress) {
        for (short int i = 1; i <= 64; ++i) {
            auto entity = *reinterpret_cast<std::uintptr_t*> (baseaddress + signatures::dwEntityList + i * 0x10);

            if (entity) {
                bool dormant = *reinterpret_cast<bool*>(entity + signatures::m_bDormant);

                if (dormant) {
                    continue;
                }
                *reinterpret_cast<std::uintptr_t*>(entity + netvars::m_bSpotted) = 1;
            }
        }
}
 
