#include "Includes.h"
#include "Modules/CreateMove.h"

HMODULE g_hModule = nullptr;
bool init = false;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

//using CreateMoveFn = void(__fastcall*)(void*, int, float, bool);
//CreateMoveFn oCreateMove = nullptr;

//void __fastcall HookedCreateMove(void* pThis, int nSlot, float flInputSampleTime, bool bActive) {
//	oCreateMove(pThis, nSlot, flInputSampleTime, bActive);
//
//	// 2. Acessa o CSGOInput (usando o seu endereço base da client.dll)
//	uintptr_t ClientDll = LoadedVariables::ClientDLL;
//	auto* pInput = *reinterpret_cast<CCSGOInputStruct**>(ClientDll + Offsets::dwCSGOInput);
//}

void InicializarPatterns() {
		bool carregado = false;

		while (!carregado) {
			// Tenta encontrar os patterns
			auto fnInitTraceData = (InitTraceData_t)Memory::FindPattern(XOR(ClientDllString), ("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8D 79 ? 33 F6 C7 47"));
			auto fnInitTraceInfo = (InitTraceInfo_t)Memory::FindPattern(XOR(ClientDllString), ("40 55 41 55 41 57 48 83 EC"));
			auto fnInitFilter = (InitTraceFilter_t)Memory::FindPattern(XOR(ClientDllString), ("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 0F B6 41 ? 33 FF 24"));
			auto fnCreateTrace = (CreateTrace_t)Memory::FindPattern(XOR(ClientDllString), ("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? ? ? ? ? 4D 8D 71"));
			auto fnGetTraceInfo = (GetTraceInfo_t)Memory::FindPattern(XOR(ClientDllString), ("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B E9 0F 29 74 24"));
			auto fnHandleBulletPenetration = (HandleBulletPen_t)Memory::FindPattern(XOR(ClientDllString), ("48 8B C4 44 89 48 ? 48 89 50 ? 48 89 48 ? 55 57"));
			auto CreateMove = (CreateMoveFn)Memory::FindPattern(XOR(ClientDllString), ("48 8B C4 4C 89 40 ? 48 89 48 ? 55 53 41 54"));
			
			// Verifica se todos foram encontrados
			if (fnInitTraceData && fnInitTraceInfo && fnInitFilter && fnCreateTrace && fnGetTraceInfo && fnHandleBulletPenetration && CreateMove) {
				LoadedVariables::fnInitTraceData = fnInitTraceData;
				LoadedVariables::fnInitTraceInfo = fnInitTraceInfo;
				LoadedVariables::fnInitFilter = fnInitFilter;
				LoadedVariables::fnCreateTrace = fnCreateTrace;
				LoadedVariables::fnGetTraceInfo = fnGetTraceInfo;
				LoadedVariables::fnHandleBulletPenetration = fnHandleBulletPenetration;
				LoadedVariables::CreateMove = CreateMove;

				carregado = true; // Sai do loop
			}
			else {
				// Se falhar, espera um pouco e tenta de novo na próxima iteração do while
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
	}

void InicializarHack() {
	AllocConsole();
	FILE* f = nullptr;
	freopen_s(&f, XOR("CONOUT$"), XOR("w"), stdout);

	std::string XORClientDllString = XOR(ClientDllString);

	uintptr_t ClientDll = Includes::GetDLLAddress(XORClientDllString);
	uintptr_t ViewMatrix = (ClientDll + Offsets::dwViewMatrix);
	uintptr_t LocalPlayerController = (ClientDll + Offsets::dwLocalPlayerController);
	uintptr_t LocalPlayerPawn = (ClientDll + Offsets::dwLocalPlayerPawn);
	uintptr_t EntityList = (ClientDll + Offsets::dwEntityList);
	uintptr_t attack = (ClientDll + Offsets::attack);
	
	LoadedVariables::ClientDLL = ClientDll;
	LoadedVariables::ViewMatrix = ViewMatrix;
	LoadedVariables::LocalPlayerController = LocalPlayerController;
	LoadedVariables::LocalPlayerPawn = LocalPlayerPawn;
	LoadedVariables::EntityList = EntityList;
	LoadedVariables::attack = attack;

	InicializarPatterns();

	MH_Initialize();
	if (MH_CreateHook((LPVOID)LoadedVariables::CreateMove, (LPVOID)hkCreateMove, (LPVOID*)&oCreateMove) == MH_OK) {
		if (MH_EnableHook((LPVOID)LoadedVariables::CreateMove) == MH_OK) {
		}
	}
}

void RenderizarLogica() {
	//if (GetAsyncKeyState(VK_END) & 0x8000) {
		/*kiero::shutdown();
		FreeConsole();
		FreeLibraryAndExitThread(g_hModule, 0);*/
	//}
	//unsigned __int64 ciclosInicio = __rdtsc();
	//std::println("\n\nantiaim");
	Functions::Ragebot::Ragebot();
	Functions::Ragebot::Antiaim();
	//std::println("legitbot");
	Functions::Legitbot::Legitbot();
	//std::println("esp");
	Functions::ESP::ESP();
	//std::println("visuals");
	Functions::ESP::Visuals();
	//std::println("misc");
	Functions::Miscellaneous::Miscellaneous();
	//unsigned __int64 ciclosFim = __rdtsc();
	//unsigned __int64 totalCiclos = ciclosFim - ciclosInicio;
	//std::cout << "A funcao demorou " << totalCiclos << " ciclos de CPU." << std::endl;
}

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

void AdicionarAjuda(const char* desc) {
	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f); // Evita que o texto fique em uma linha infinita
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

static const char* weapon_names[] = {
					"Deagle", "Dual Berettas", "Five-Seven", "Glock", "Tec-9", "P2000", "P250", "USP-S", "CZ75", "R8",
					"AK-47", "AUG", "FAMAS", "Galil AR", "M4A4", "SG 553", "M4A1-S",
					"AWP", "G3SG1", "SCAR-20", "SSG 08",
					"MAC-10", "P90", "MP5-SD", "UMP-45", "PP-Bizon", "MP9",
					"M249", "XM1014", "Nova", "MAG-7", "Negev", "Sawed-Off"
};

static const int weapon_ids[] = {
	1, 2, 3, 4, 30, 32, 36, 61, 63, 64,   // Pistolas
	7, 8, 10, 13, 16, 39, 60,             // Rifles
	9, 11, 38, 40,                        // Snipers
	17, 19, 23, 24, 26, 34,               // SMGs
	14, 25, 35, 27, 28, 29                // Heavy
};
struct HitboxOption { const char* name; EnabledHitboxesEnum flag; };
static int ui_weapon_index = 0; // Índice visual (Deagle)

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);

			InicializarHack();
			InitImGui();
			init = true;
		}
		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	RenderizarLogica();

	ImGui::SetNextWindowSize(ImVec2(700, 470), ImGuiCond_Always);
	ImGui::Begin(XOR("hlware"));

	if (ImGui::BeginTabBar(XOR("Menu tabs")))
	{
		if (ImGui::BeginTabItem(XOR("Ragebot"))) {
			ImGui::Columns(3, XOR("Ragebot_layout"), false);

			ImGui::BeginChild(XOR("WeaponChild"), ImVec2(0, 410), true);
			{

				ImGui::Text(("Weapon"));

				ImGui::SetNextItemWidth(-1);
				if (ImGui::Combo(XOR("##WeaponSelect"), &ui_weapon_index, weapon_names, IM_ARRAYSIZE(weapon_names))) {
					Attributes::Ragebot::Ragebot::SelectedWeaponIndex = ui_weapon_index;
				}

				ImGui::Dummy(ImVec2(0, 10));
				ImGui::Separator();
				ImGui::Dummy(ImVec2(0, 10));

				WeaponRagebotAttributesStruct& cfg = RagebotWeaponSettingsMap[weapon_ids[ui_weapon_index]];
				WeaponInformationStruct& WeaponInformation = WeaponMap[weapon_ids[ui_weapon_index]];
				
				ImGui::Checkbox(XOR("Enabled"), &cfg.IsEnabled);

				std::string preview_text = "";
				if (cfg.Hitboxes == NONE) {
					preview_text = XOR("Hitboxes");
				}
				else {
					if (cfg.Hitboxes & HEAD)   preview_text += XOR("Head, ");
					if (cfg.Hitboxes & CHEST)  preview_text += XOR("Chest, ");
					if (cfg.Hitboxes & PELVIS) preview_text += XOR("Pelvis, ");
					if (cfg.Hitboxes & ARMS)   preview_text += XOR("Arms, ");
					if (cfg.Hitboxes & LEGS)   preview_text += XOR("Legs, ");

					// Remove a última vírgula e espaço para ficar limpo
					if (preview_text.size() > 0)
						preview_text.erase(preview_text.size() - 2);
				}

				ImGui::SetNextItemWidth(-1);
				if (ImGui::BeginCombo("", preview_text.c_str())) {
					HitboxOption options[] = {
						{"Head", HEAD}, {"Chest", CHEST}, {"Pelvis", PELVIS}, {"Arms", ARMS}, {"Legs", LEGS}
					};

					for (int i = 0; i < IM_ARRAYSIZE(options); i++) {
						bool is_selected = (cfg.Hitboxes & options[i].flag);
						if (ImGui::Selectable(options[i].name, is_selected, ImGuiSelectableFlags_DontClosePopups)) {
							if (is_selected)
								cfg.Hitboxes = static_cast<EnabledHitboxesEnum>(cfg.Hitboxes & ~options[i].flag);
							else
								cfg.Hitboxes = static_cast<EnabledHitboxesEnum>(cfg.Hitboxes | options[i].flag);
						}
					}
					ImGui::EndCombo();
				}

				ImGui::SetNextItemWidth(-1); // Faz o slider ocupar toda a largura do Child
				ImGui::SliderInt(("##DamageSlider"), &cfg.MinimumDamage, 0, 100, "%d Damage");

				ImGui::SetNextItemWidth(-1);
				ImGui::SliderInt(("##HitchanceSlider"), &cfg.Hitchance, 0, 100, "%d Hitchance");
				if (WeaponInformation.HasScope) {
					ImGui::Checkbox(XOR("Automatic scope"), &cfg.AutomaticScope);
				};
			}
			ImGui::EndChild();

			ImGui::NextColumn(); // Pula para a segunda coluna

			ImGui::BeginChild(XOR("ExtraChild"), ImVec2(0, 410), true);
			{
				ImGui::Text(XOR("Extra"));
				ImGui::Checkbox(XOR("Automatic stop"), &Attributes::Ragebot::Ragebot::IsAutomaticStopEnabled);
			}
			ImGui::EndChild();

			ImGui::NextColumn();

			ImGui::BeginChild(XOR("GeneralChild"), ImVec2(0, 410), true);
			{
				ImGui::Text(XOR("General"));
				ImGui::Checkbox(XOR("Ragebot"), &Attributes::Ragebot::Ragebot::IsEnabled);
				ImGui::Checkbox(XOR("Wallbang"), &Attributes::Ragebot::Ragebot::IsWallbangEnabled);
				ImGui::Checkbox(XOR("Automatic fire"), &Attributes::Ragebot::Ragebot::IsAutomaticFireEnabled);
				ImGui::Checkbox(XOR("Silent aim"), &Attributes::Ragebot::Ragebot::IsSilentAimEnabled);
			}
			ImGui::EndChild();

			ImGui::Columns(1); // Importante: Resetar as colunas ao sair
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(XOR("Legitbot"))) {
			ImGui::Columns(2, XOR("Legitbot_layout"), false);

			ImGui::BeginChild(XOR("TriggerbotChild"), ImVec2(0, 150), true);
			{
				ImGui::Text(XOR("Triggerbot"));
				ImGui::Checkbox(XOR("Triggerbot"), &Attributes::Legitbot::IsTriggerbotEnabled);
				ImGui::Checkbox(XOR("Wallbang"), &Attributes::Legitbot::IsWallbangEnabled);
				ImGui::SliderInt(XOR("Minimum damage"), &Attributes::Legitbot::TriggerbotMinimumDamage, 0, 100);
				ImGui::SliderInt(XOR("Delay (ms)"), &Attributes::Legitbot::TriggerbotDelay, Settings::MINIMUM_TRIGGERBOT_DELAY, Settings::MAXIMUM_TRIGGERBOT_DELAY);
			}
			ImGui::EndChild();

			ImGui::NextColumn();

			ImGui::BeginChild(XOR("ExtraChild"), ImVec2(0, 150), true);
			{
				ImGui::Text(XOR("Extra"));
			}
			ImGui::EndChild();

			ImGui::Columns(1);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(XOR("ESP"))) {
			if (ImGui::BeginTabBar(XOR("ESP tabs"))) {
				if (ImGui::BeginTabItem(XOR("ESP"))) {
					// Dividimos em 2 colunas para os Groupboxes
					ImGui::Columns(2, XOR("ESP_layout"), false);

					// ImVec2(0, 150) -> 0 significa "toda a largura da coluna", 150 é a altura
					ImGui::BeginChild(XOR("PlayerESPChild"), ImVec2(0, 150), true);
					{
						ImGui::Text(XOR("Player ESP"));
						ImGui::Separator();
						ImGui::Checkbox(XOR("Box"), &Attributes::ESP::ESP::IsBoxEnabled);
						ImGui::SameLine(ImGui::GetContentRegionAvail().x - 15); // Joga o seletor de cor pra direita
						ImGui::ColorEdit4("##BoxColor", &Attributes::ESP::ESP::BoxColor.Red, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

						ImGui::Checkbox(XOR("Health Bar"), &Attributes::ESP::ESP::IsHealthBarEnabled);
						// Se quiser dois seletores (gradiente), coloque-os lado a lado
						ImGui::SameLine(ImGui::GetContentRegionAvail().x - 40 - 2);
						ImGui::ColorEdit4("##HealthBarColor1", &Attributes::ESP::ESP::HealthBarColor1.Red, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
						ImGui::SameLine();
						ImGui::ColorEdit4("##HealthBarColor2", &Attributes::ESP::ESP::HealthBarColor2.Red, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
						ImGui::Checkbox(XOR("Name"), &Attributes::ESP::ESP::IsNameEnabled);
						ImGui::SameLine(ImGui::GetContentRegionAvail().x - 15);
						ImGui::ColorEdit4("##NameColor", &Attributes::ESP::ESP::NameColor.Red, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
						ImGui::Checkbox(XOR("Skeleton"), &Attributes::ESP::ESP::IsSkeletonEnabled);
						ImGui::SameLine(ImGui::GetContentRegionAvail().x - 15);
						ImGui::ColorEdit4("##SkeletonColor", &Attributes::ESP::ESP::SkeletonColor.Red, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
						ImGui::Checkbox(XOR("Glow"), &Attributes::ESP::ESP::IsGlowEnabled);
						ImGui::SameLine(ImGui::GetContentRegionAvail().x - 15);
						ImGui::ColorEdit4("##GlowColor", &Attributes::ESP::ESP::GlowColor.Red, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
					}
					ImGui::EndChild();

					ImGui::NextColumn(); // Pula para a segunda coluna

					ImGui::BeginChild(XOR("TeammatesChild"), ImVec2(0, 150), true);
					{
						ImGui::Text(XOR("Teammates"));
					
					}
					ImGui::EndChild();

					ImGui::Columns(1); // Importante: Resetar as colunas ao sair
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem(XOR("Visuals"))) {
					//ImGui::Checkbox("Visuals", &Attributes::ESP::Visuals::IsEnabled);
					ImGui::Columns(2, XOR("Visuals_layout"), false);
					ImGui::BeginChild(XOR("VisualsChild"), ImVec2(0, 150), true);
					{
						ImGui::Text(XOR("Visuals"));
						ImGui::Checkbox(XOR("Remove flashbang"), &Attributes::ESP::Visuals::RemoveFlashbang);
						ImGui::Checkbox(XOR("Remove smoke"), &Attributes::ESP::Visuals::RemoveSmoke);
					}
					ImGui::EndChild();

					ImGui::NextColumn();

					ImGui::BeginChild(XOR("ViewmodelChild"), ImVec2(0, 150), true);
					{
						ImGui::Text(XOR("Viewmodel"));
					}
					ImGui::EndChild();
					
					ImGui::Columns(1); // Importante: Resetar as colunas ao sair
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)&oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		g_hModule = hMod;
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}
