#include <windows.h>
 
DWORD WINAPI MainThread(LPVOID)
{ 
    while (true) 
    { 
     if (GetAsyncKeyState(VK_F6) & 1)
        {
         MessageBoxA(0, "C++ Mod Loaded!", "GTA IV", MB_OK);
        }
        Sleep(100);
    }
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        CreateThread(0, 0, MainThread, 0, 0, 0);
    }
    return TRUE;
}
