#pragma once

#include "Offsets.h"
#include "LoadedVariables.h"
//#include "Functions.h"
#include "Settings.h"
#include "Attributes.h"
#include "Classes.h"
#include "TraceManager.h"
//#include "Penetration/bvh.cpp"
//#include "Penetration/entities.cpp"
//#include "Penetration/hitboxes.cpp"
//#include "Penetration/schemas.cpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "kieroincludes.h"

#include <iostream>
#include <cstdint>
#include <string>
#include <map>
#include <vector>
#include <windows.h>
#include <psapi.h>
#include <string>
#include <thread>
#include <chrono>

namespace Includes {
	inline uintptr_t GetDLLAddress(std::string DLL) {
		if (DLL == "client.dll" && LoadedVariables::ClientDLL) return LoadedVariables::ClientDLL;
		return (uintptr_t)GetModuleHandleA(DLL.c_str());
	}

	inline uintptr_t GetLocalPlayerPawn() {
		return *(uintptr_t*)LoadedVariables::LocalPlayerPawn;
	}

	inline uintptr_t GetLocalPlayerController() {
		return *(uintptr_t*)LoadedVariables::LocalPlayerController;
	}

	inline int GetLocalPlayerTeam() {
		uintptr_t LocalPlayerController = Includes::GetLocalPlayerController();
		if (!LocalPlayerController) return 0;
		return *(int*)(LocalPlayerController + Offsets::m_iTeamNum);
	}

	inline uintptr_t GetEntityList() {
		return *(uintptr_t*)LoadedVariables::EntityList;
	}

	inline int GetPlayerTeam(uintptr_t Player_ControllerOrPawn) {
		return *(int*)(Player_ControllerOrPawn + Offsets::m_iTeamNum);
	}

	inline bool IsPlayerAnEnemy(uintptr_t TargetPlayer_ControllerOrPawn) {
		int LocalPlayerTeam = Includes::GetLocalPlayerTeam();
		int TargetPlayerTeam = Includes::GetPlayerTeam(TargetPlayer_ControllerOrPawn);
		return LocalPlayerTeam != TargetPlayerTeam;
	}

	inline void PreciseSleep(const double milliseconds) {
		//auto start = std::chrono::high_resolution_clock::now();
		//double target = milliseconds;
		//while (std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start).count() < target) {
		//	// Fica girando aqui sem parar (consome CPU, mas é instantâneo)
		//	_mm_pause(); // Instrução para não fritar o processador à toa
		//}
		std::this_thread::sleep_for(std::chrono::milliseconds((long long)milliseconds));
	}

	inline ImColor ConvertRGBAToImColor(const ColorRGBAStruct Color) {
		return ImColor(Color.Red, Color.Blue, Color.Green, Color.Alpha);
	}
	
	inline ColorRGBAStruct LerpRGBAColor(const float healthPercent, const ColorRGBAStruct& c1, const ColorRGBAStruct& c2) {
		const float r = c2.Red + (c1.Red - c2.Red) * healthPercent;
		const float g = c2.Green + (c1.Green - c2.Green) * healthPercent;
		const float b = c2.Blue + (c1.Blue - c2.Blue) * healthPercent;
		const float a = c2.Alpha + (c1.Alpha - c2.Alpha) * healthPercent;
		return ColorRGBAStruct{ r * 255, g * 255, b * 255, a * 255 };
	}

	inline ViewMatrixStruct& GetViewMatrix() {
		return *(ViewMatrixStruct*)(LoadedVariables::ViewMatrix);
	}

	inline int16_t GetPlayerWeaponAlgorithm(uintptr_t PlayerPawn) {
		// Acessando WeaponServices
		const uintptr_t WeaponServices = *(uintptr_t*)(PlayerPawn + Offsets::m_pWeaponServices);
		if (!WeaponServices) return 0;

		// Acessando o Handle da arma ativa
		const uint32_t ActiveWeaponHandle = *(uint32_t*)(WeaponServices + Offsets::m_hActiveWeapon);
		if (ActiveWeaponHandle == 0xFFFFFFFF) return 0;

		// Tradução do Handle para Entity
		const uintptr_t EntityList = Includes::GetEntityList();
		if (!EntityList) return 0;

		const uintptr_t ListEntry = *(uintptr_t*)(EntityList + 0x8 * ((ActiveWeaponHandle & 0x7FFF) >> 9) + 0x10);
		if (!ListEntry) return 0;

		const uintptr_t WeaponEntity = *(uintptr_t*)(ListEntry + 112 * (ActiveWeaponHandle & 0x1FF));
		if (!WeaponEntity) return 0;

		// --- LÓGICA DAS LINHAS 406 DO CÓDIGO QUE VOCÊ ACHOU ---

		// O ID da arma no CS2 é lido somando os offsets de C_EconEntity -> AttributeManager -> Item -> Index
		// IMPORTANTE: Use exatamente os nomes de offsets da sua dump (m_AttributeManager, m_Item, m_iItemDefinitionIndex)

		uintptr_t FinalIDAddress = WeaponEntity
			+ Offsets::m_AttributeManager
			+ Offsets::m_Item
			+ Offsets::m_iItemDefinitionIndex;

		// Lendo como uint16_t (2 bytes)
		int16_t id = *(int16_t*)(FinalIDAddress);

		return id;
	}

	inline int16_t GetPlayerWeapon(uintptr_t PlayerPawn = 0) {
		if (PlayerPawn) return Includes::GetPlayerWeaponAlgorithm(PlayerPawn);
		const uintptr_t LocalPlayerPawn = Includes::GetLocalPlayerPawn();
		if (LocalPlayerPawn) {
			return Includes::GetPlayerWeaponAlgorithm(LocalPlayerPawn);
		}
		return 0;
	}
}








////////////////////////////////////////////// FUNCTIONS ///////////////////////////////////////////////////////////
#pragma once

#include "Includes.h"
#include "LoadedVariables.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "Classes.h"

#include <iostream>

namespace Functions {
	namespace Ragebot {
		void Ragebot();
		void Antiaim();
	}

	namespace Legitbot {
		void Legitbot();
	}

	namespace ESP {
		void ESP();
		void Visuals();
	}

	namespace Miscellaneous {

	}

	namespace Calculations {
		inline bool WorldToScreen(const Vector3Struct& WorldPosition, Vector2Struct& ScreenPosition) {
			// Cálculo do clip W (profundidade)
			const ViewMatrixStruct ViewMatrix = Includes::GetViewMatrix();
			const float w = ViewMatrix.Matrix[3][0] * WorldPosition.X + ViewMatrix.Matrix[3][1] * WorldPosition.Y + ViewMatrix.Matrix[3][2] * WorldPosition.Z + ViewMatrix.Matrix[3][3];

			// Se W < 0.01, o jogador está atrás da câmera
			if (w < 0.01f) return false;

			const float invw = 1.0f / w;
			const float x = (ViewMatrix.Matrix[0][0] * WorldPosition.X + ViewMatrix.Matrix[0][1] * WorldPosition.Y + ViewMatrix.Matrix[0][2] * WorldPosition.Z + ViewMatrix.Matrix[0][3]) * invw;
			float y = (ViewMatrix.Matrix[1][0] * WorldPosition.X + ViewMatrix.Matrix[1][1] * WorldPosition.Y + ViewMatrix.Matrix[1][2] * WorldPosition.Z + ViewMatrix.Matrix[1][3]) * invw;

			const ImGuiIO IO = ImGui::GetIO();
			const float ScreenSizeX = IO.DisplaySize.x;
			const float ScreenSizeY = IO.DisplaySize.y;

			ScreenPosition.X = (ScreenSizeX * 0.5f) + (x * ScreenSizeX * 0.5f);
			ScreenPosition.Y = (ScreenSizeY * 0.5f) - (y * ScreenSizeY * 0.5f);

			return true;
		}
	}

	namespace ImGuiFunctions {
		inline void DrawBox(const float X, const float Y, const float W, const float H, const ColorRGBAStruct& Color) {
			// 1. Guardamos o DrawList em uma variável local (Cache)
			auto* drawList = ImGui::GetForegroundDrawList();

			// 2. Preparamos os ImVec2 uma única vez para evitar reconstrução na stack
			const ImVec2 min = { X, Y };
			const ImVec2 max = { X + W, Y + H };

			// 3. Desenhar o Outline (Contorno) primeiro
			// Usamos um offset sutil e uma espessura fina para o visual minimalista
			// Nota: ImColor(0,0,0) convertido para ImU32 é mais rápido
			drawList->AddRect(
				{ min.x - 1.0f, min.y - 1.0f },
				{ max.x + 1.0f, max.y + 1.0f },
				IM_COL32(0, 0, 0, 255), // Macro para cor preta (U32)
				0.0f,
				0,
				1.0f
			);

			// 4. Desenhar a Caixa Principal por cima
			drawList->AddRect(
				min,
				max,
				ImGui::ColorConvertFloat4ToU32(ImVec4(Color.Red, Color.Green, Color.Blue, Color.Alpha)),
				0.0f,
				0,
				1.5f
			);
		}

		inline void DrawLine(const ImVec2& X, const ImVec2& Y, const ColorRGBAStruct& Color) {

			ImGui::GetForegroundDrawList()->AddLine(
				X, Y, ImGui::ColorConvertFloat4ToU32(ImVec4(Color.Red, Color.Green, Color.Blue, Color.Alpha)), 2.0f
			);
		}
	}
}














namespace Memory {
	// Converte a string do pattern para um vetor de inteiros (com suporte a wildcards '?')
	inline std::vector<int> pattern_to_byte(const char* pattern) {
		auto bytes = std::vector<int>{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;
				if (*current == '?') ++current;
				bytes.push_back(-1); // -1 representa um wildcard
			}
			else {
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	}

	// Função principal que encontra o endereço no módulo
	inline uintptr_t FindPattern(const char* module_name, const char* signature) {
		auto module_handle = GetModuleHandleA(module_name);
		if (!module_handle) return 0;

		MODULEINFO module_info;
		GetModuleInformation(GetCurrentProcess(), module_handle, &module_info, sizeof(MODULEINFO));

		auto pattern_bytes = pattern_to_byte(signature);
		auto scan_bytes = reinterpret_cast<std::uint8_t*>(module_handle);

		auto s = pattern_bytes.size();
		auto d = pattern_bytes.data();

		for (auto i = 0ul; i < module_info.SizeOfImage - s; ++i) {
			bool found = true;
			for (auto j = 0ul; j < s; ++j) {
				if (scan_bytes[i + j] != d[j] && d[j] != -1) {
					found = false;
					break;
				}
			}
			if (found) {
				return reinterpret_cast<uintptr_t>(&scan_bytes[i]);
			}
		}
		return 0;
	}

	inline uintptr_t GetAbsoluteAddress(uintptr_t addr, int offset, int size) {
		if (!addr) return 0;

		// 1. Lê o valor do offset relativo (geralmente um int32_t de 4 bytes)
		// O 'offset' é a distância do início da instrução até onde o número está
		int32_t relative_offset = *reinterpret_cast<int32_t*>(addr + offset);

		// 2. O endereço absoluto é: 
		// Endereço da Instrução + Tamanho Total da Instrução + Offset Lido
		uintptr_t absolute_address = addr + size + relative_offset;

		return absolute_address;
	}
}

/////////////////////////////////////////////////

//class CGameTraceManager {
//public:
//	// Função estática para obter a instância (muitos cheats buscam isso via pattern ou interface)
//	static CGameTraceManager* Get() {
//		// No CS2, o TraceManager pode ser encontrado via interface de Engine ou Pattern
//		// Para simplificar, vamos usar o padrão do TraceShape diretamente
//		return nullptr;
//	}
//
//	bool TraceShape(Ray_t* pRay, Vector_t vecStart, Vector_t vecEnd, TraceFilter_t* pFilter, GameTrace_t* pGameTrace) {
//		using fnTraceShape = bool(__fastcall*)(void*, Ray_t*, Vector_t*, Vector_t*, TraceFilter_t*, GameTrace_t*);
//
//		// Pattern que você encontrou: 48 89 5C 24 ? 48 89 4C 24 ? 55 57
//		static fnTraceShape oTraceShape = nullptr;
//		if (!oTraceShape) {
//			// Substitua 'SeuModuloDeMemoria::FindPattern' pela sua função de Scan
//			oTraceShape = (fnTraceShape)Memory::FindPattern("client.dll", "48 89 5C 24 ? 48 89 4C 24 ? 55 57");
//		}
//
//		if (!oTraceShape) return false;
//		return oTraceShape(this, pRay, &vecStart, &vecEnd, pFilter, pGameTrace);
//	}
//};