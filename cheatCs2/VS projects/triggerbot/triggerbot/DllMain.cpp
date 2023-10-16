#include "Includes.h"

BOOL WINAPI TriggerThread(HMODULE hModule)
{
    uintptr_t Client = (uintptr_t)GetModuleHandle("client.dll");
    
    while (!GetAsyncKeyState(VK_END))
    {
        uintptr_t LP = *(uintptr_t*)(Client + dwLocalPlayer);
        if (LP == NULL) continue;
        uint32_t LT = *(uint32_t*)(LP + m_iTeamNum);
        uint32_t CI = *(uint32_t*)(LP + m_iCrosshairId);

        if (CI != 0 && CI < 32)
        {
            uintptr_t EN = *(uintptr_t*)(Client + dwEntityList + (CI - 1) * 0x10);
            if (EN == NULL) continue;

            uint32_t EH = *(uint32_t*)(EN + m_iHealth);
            uint32_t ET = *(uintptr_t*)(EN + m_iTeamNum);
            
            if (ET != LT && EH > 0 && EH < 100)
            {
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                Sleep(20);
            }
            Sleep(1);
        }
    }

    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)TriggerThread, 0, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}