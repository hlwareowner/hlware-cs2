#pragma once

#include "Offsets.h"
#include "LoadedVariables.h"
//#include "Functions.h"
#include "Settings.h"
#include "Attributes.h"
#include "Classes.h"
#include "TraceManager.h"
//#include "Modules/CreateMove.h" circular dependency
#include "xorstr.hpp"
#include "kiero/minhook/include/MinHook.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "kieroincludes.h"

#include <print>
#include <cstdint>
#include <string>
#include <map>
#include <algorithm>
#include <numbers>
#include <vector>
#include <windows.h>
#include <psapi.h>
#include <thread>
#include <chrono>

#define ClientDllString "client.dll"
#ifdef _DEBUG
#define XOR(STRING) STRING
#else
#define XOR(STRING) xorstr_(STRING)
#endif

namespace Includes {
	uintptr_t GetDLLAddress(std::string DLL);

	uintptr_t GetLocalPlayerPawn();

	uintptr_t GetLocalPlayerController();

	int GetLocalPlayerTeam();

	uintptr_t GetEntityList();

	int GetPlayerTeam(uintptr_t Player_ControllerOrPawn);

	bool IsPlayerAnEnemy(uintptr_t TargetPlayer_ControllerOrPawn);

	void PreciseSleep(const double milliseconds);

	ImColor ConvertRGBAToImColor(const ColorRGBAStruct Color);

	ColorRGBAStruct LerpRGBAColor(const float healthPercent, const ColorRGBAStruct& c1, const ColorRGBAStruct& c2);

	ViewMatrixStruct& GetViewMatrix();

	Vector3Struct GetPlayerBonePosition(const uintptr_t PlayerPawn, const int Bone);

	bool IsShooting(CUserCommandStruct* Command);

	int16_t GetPlayerWeaponAlgorithm(const uintptr_t PlayerPawn);

	int16_t GetPlayerWeapon(const uintptr_t PlayerPawn = 0);
}








////////////////////////////////////////////// FUNCTIONS ///////////////////////////////////////////////////////////


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
		void Miscellaneous();
	}

	namespace Calculations {
		bool WorldToScreen(const Vector3Struct& WorldPosition, Vector2Struct& ScreenPosition);
	}

	namespace ImGuiFunctions {
		void DrawBox(const float X, const float Y, const float W, const float H, const float Thickness, const ColorRGBAStruct& Color);

		void DrawLine(const ImVec2& X, const ImVec2& Y, const float Thickness, const ColorRGBAStruct& Color);

		void DrawText(const char* Text, const ImVec2& Position, const ColorRGBAStruct& Color);
	}

	namespace Raycast {
		bool TraceLine(Vector3Struct start, Vector3Struct targetPos, CGameTraceStruct* outTrace);

		float CalculateDistanceDamageModifier(const float Distance, const float RangeModifier);

		int GetDamage(const Vector3Struct& StartPosition, const Vector3Struct& TargetPosition, uintptr_t skipPawnPtr, uintptr_t targetPawnPtr, const float WeaponDamage, const float WeaponPenetration, const float WeaponRangeModifier, const float WeaponRange);

		bool DoesRayHitEntity(const Vector3Struct& StartPosition, const Vector3Struct& TargetPosition, uintptr_t skipPawnPtr, uintptr_t* EntityHit, float WeaponDamage, float wpnPenetration, float wpnRangeMod, float wpnRange);

		float GetHitgroupDamageMultiplier(EnabledHitboxesEnum Hitbox);

		int ScaleDamage(const int Damage, const EnabledHitboxesEnum Hitbox/*, uintptr_t TargetPawn*/);
	}
}






///////////////////////////////////////////////// MEMORY /////////////////////////////////////////////////







namespace Memory {
	std::vector<int> pattern_to_byte(const char* pattern);

	uintptr_t FindPattern(const char* module_name, const char* signature);

	uintptr_t GetAbsoluteAddress(uintptr_t addr, int offset, int size);
}
