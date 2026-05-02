//#include "Includes.h"
//
//void ScriptPrincipal(HMODULE hModule) {
//    AllocConsole();
//    FILE* f = nullptr;
//    freopen_s(&f, "CONOUT$", "w", stdout);
//
//    uintptr_t ClientDLL = Includes::GetDLLAddress("client.dll");
//
//    LoadedVariables::ClientDLL = ClientDLL;
//    
//    while (true) {
//        if (GetAsyncKeyState(VK_END) & 0x8000) break;
//
//        // Apenas chamamos a função que está no outro arquivo
//        Legitbot::PrintarDadosDoJogador();
//        ESP::ESP();
//
//        Sleep(Settings::MAIN_LOOP_RATE);
//    }
//
//    if (f) fclose(f);
//    FreeConsole();
//    FreeLibraryAndExitThread(hModule, 0);
//}

// O "Main" da DLL
//BOOL APIENTRY DllMain(HMODULE hModule,  // Endereço de onde a DLL foi carregada na RAM
//    DWORD  ul_reason_for_call,
//    LPVOID lpReserved) {
//    switch (ul_reason_for_call) {
//    case DLL_PROCESS_ATTACH:
//        // Quando a DLL entra no jogo, criamos uma "Thread" (um braço extra)
//        // para não travar o código principal do jogo.
//        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ScriptPrincipal, hModule, 0, nullptr));
//        break;
//    case DLL_PROCESS_DETACH:
//        break;
//    }
//    return TRUE;
//}