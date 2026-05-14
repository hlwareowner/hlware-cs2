#include "Includes.h"

namespace Includes {
	uintptr_t GetDLLAddress(std::string DLL) {
		if (DLL == ClientDllString && LoadedVariables::ClientDLL) return LoadedVariables::ClientDLL;
		return reinterpret_cast<uintptr_t>(GetModuleHandleA(DLL.c_str()));
	}

	uintptr_t GetLocalPlayerPawn() {
		return *reinterpret_cast<uintptr_t*>(LoadedVariables::LocalPlayerPawn);
	}

	uintptr_t GetLocalPlayerController() {
		return *(uintptr_t*)LoadedVariables::LocalPlayerController;
	}

	int GetLocalPlayerTeam() {
		const uintptr_t LocalPlayerController = Includes::GetLocalPlayerController();
		if (!LocalPlayerController) return 0;
		return *reinterpret_cast<int*>(LocalPlayerController + Offsets::m_iTeamNum);
	}

	uintptr_t GetEntityList() {
		return *(uintptr_t*)LoadedVariables::EntityList;
	}

	int GetPlayerTeam(uintptr_t Player_ControllerOrPawn) {
		return *reinterpret_cast<int*>(Player_ControllerOrPawn + Offsets::m_iTeamNum);
	}

	bool IsPlayerAnEnemy(uintptr_t TargetPlayer_ControllerOrPawn) {
		const int LocalPlayerTeam = Includes::GetLocalPlayerTeam();
		const int TargetPlayerTeam = Includes::GetPlayerTeam(TargetPlayer_ControllerOrPawn);
		return LocalPlayerTeam != TargetPlayerTeam;
	}

	void PreciseSleep(const double milliseconds) {
		//auto start = std::chrono::high_resolution_clock::now();
		//double target = milliseconds;
		//while (std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start).count() < target) {
		//	// Fica girando aqui sem parar (consome CPU, mas é instantâneo)
		//	_mm_pause(); // Instrução para não fritar o processador à toa
		//}
		std::this_thread::sleep_for(std::chrono::milliseconds((long long)milliseconds));
	}

	ImColor ConvertRGBAToImColor(const ColorRGBAStruct Color) {
		return ImColor(Color.Red, Color.Blue, Color.Green, Color.Alpha);
	}

	ColorRGBAStruct LerpRGBAColor(const float healthPercent, const ColorRGBAStruct& c1, const ColorRGBAStruct& c2) {
		const float r = c2.Red + (c1.Red - c2.Red) * healthPercent;
		const float g = c2.Green + (c1.Green - c2.Green) * healthPercent;
		const float b = c2.Blue + (c1.Blue - c2.Blue) * healthPercent;
		const float a = c2.Alpha + (c1.Alpha - c2.Alpha) * healthPercent;
		return ColorRGBAStruct{ r * 255, g * 255, b * 255, a * 255 };
	}

	ViewMatrixStruct& GetViewMatrix() {
		return *(ViewMatrixStruct*)(LoadedVariables::ViewMatrix);
	}

	Vector3Struct GetPlayerBonePosition(const uintptr_t PlayerPawn, const int Bone) {
		const uintptr_t SceneNode = *(uintptr_t*)(PlayerPawn + Offsets::m_pGameSceneNode);
		const uintptr_t BoneMatrix = *(uintptr_t*)(SceneNode + Offsets::m_modelState + 0x80);
		return *(Vector3Struct*)(BoneMatrix + Bone * 32);
	}

	bool IsShooting(CUserCommandStruct* Command) {
		return static_cast<bool>(Command->nButtons.nValueChanged & IN_ATTACK); // valuechanged em vez de value: corrige o negocio de so atirar 1 vez
	}

	int16_t GetPlayerWeaponAlgorithm(const uintptr_t PlayerPawn) {
		// Acessando WeaponServices
		const uintptr_t WeaponServices = *reinterpret_cast<uintptr_t*>(PlayerPawn + Offsets::m_pWeaponServices);
		if (!WeaponServices) return 0;

		// Acessando o Handle da arma ativa
		const uint32_t ActiveWeaponHandle = *(uint32_t*)(WeaponServices + Offsets::m_hActiveWeapon);
		if (!ActiveWeaponHandle || ActiveWeaponHandle == 0xFFFFFFFF) return 0;

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

		const uintptr_t FinalIDAddress = WeaponEntity
			+ Offsets::m_AttributeManager
			+ Offsets::m_Item
			+ Offsets::m_iItemDefinitionIndex;

		// Lendo como uint16_t (2 bytes)
		const int16_t id = *reinterpret_cast<uint16_t*>(FinalIDAddress);

		return id;
	}

	int16_t GetPlayerWeapon(const uintptr_t PlayerPawn) {
		if (PlayerPawn) return Includes::GetPlayerWeaponAlgorithm(PlayerPawn);
		const uintptr_t LocalPlayerPawn = Includes::GetLocalPlayerPawn();
		if (LocalPlayerPawn) {
			return Includes::GetPlayerWeaponAlgorithm(LocalPlayerPawn);
		}
		return 0;
	}
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
		bool WorldToScreen(const Vector3Struct& WorldPosition, Vector2Struct& ScreenPosition) {
			// Cálculo do clip W (profundidade)
			const ViewMatrixStruct ViewMatrix = Includes::GetViewMatrix();
			const float w = ViewMatrix.Matrix[3][0] * WorldPosition.X + ViewMatrix.Matrix[3][1] * WorldPosition.Y + ViewMatrix.Matrix[3][2] * WorldPosition.Z + ViewMatrix.Matrix[3][3];

			// Se W < 0.01, o jogador está atrás da câmera
			if (w < 0.01f) return false;

			const float invw = 1.0f / w;
			const float x = (ViewMatrix.Matrix[0][0] * WorldPosition.X + ViewMatrix.Matrix[0][1] * WorldPosition.Y + ViewMatrix.Matrix[0][2] * WorldPosition.Z + ViewMatrix.Matrix[0][3]) * invw;
			const float y = (ViewMatrix.Matrix[1][0] * WorldPosition.X + ViewMatrix.Matrix[1][1] * WorldPosition.Y + ViewMatrix.Matrix[1][2] * WorldPosition.Z + ViewMatrix.Matrix[1][3]) * invw;

			const ImGuiIO IO = ImGui::GetIO();
			const float ScreenSizeX = IO.DisplaySize.x;
			const float ScreenSizeY = IO.DisplaySize.y;

			ScreenPosition.X = (ScreenSizeX * 0.5f) + (x * ScreenSizeX * 0.5f);
			ScreenPosition.Y = (ScreenSizeY * 0.5f) - (y * ScreenSizeY * 0.5f);

			return true;
		}
	}

	namespace ImGuiFunctions {
		void DrawBox(const float X, const float Y, const float W, const float H, const float Thickness, const ColorRGBAStruct& Color) {
			// 1. Guardamos o DrawList em uma variável local (Cache)
			auto* drawList = ImGui::GetForegroundDrawList();

			// 2. Preparamos os ImVec2 uma única vez para evitar reconstrução na stack
			const ImVec2 min = { X, Y };
			const ImVec2 max = { X + W, Y + H };

			drawList->AddRect(
				min,
				max,
				ImGui::ColorConvertFloat4ToU32(ImVec4(Color.Red, Color.Green, Color.Blue, Color.Alpha)),
				0.0f,
				0,
				Thickness
			);
		}

		void DrawLine(const ImVec2& X, const ImVec2& Y, const float Thickness, const ColorRGBAStruct& Color) {
			ImGui::GetForegroundDrawList()->AddLine(
				X, Y, ImGui::ColorConvertFloat4ToU32(ImVec4(Color.Red, Color.Green, Color.Blue, Color.Alpha)), Thickness
			);
		}

		void DrawText(const char* Text, const ImVec2& Position, const ColorRGBAStruct& Color) {
			ImDrawList* drawList = ImGui::GetForegroundDrawList();
			drawList->AddText(Position, ImGui::ColorConvertFloat4ToU32(ImVec4(Color.Red, Color.Green, Color.Blue, Color.Alpha)), Text);
		}
	}

	namespace Raycast {
		bool TraceLine(Vector3Struct start, Vector3Struct targetPos, CGameTraceStruct* outTrace) {
			static InitTraceData_t fnInitTraceData = LoadedVariables::fnInitTraceData;
			static InitTraceInfo_t fnInitTraceInfo = LoadedVariables::fnInitTraceInfo;
			static InitTraceFilter_t fnInitFilter = LoadedVariables::fnInitFilter;
			static CreateTrace_t fnCreateTrace = LoadedVariables::fnCreateTrace;
			static GetTraceInfo_t fnGetTraceInfo = LoadedVariables::fnGetTraceInfo;

			uintptr_t localPawn = Includes::GetLocalPlayerPawn();
			if (!localPawn) return false;

			Vector3Struct delta = { targetPos.X - start.X, targetPos.Y - start.Y, targetPos.Z - start.Z };

			alignas(16) TraceDataStruct td = {}; // align 16
			fnInitTraceData(&td);

			alignas(16) CTraceFilterStruct filter = {}; // align 16
			uint64_t mask = 0x1C100B;
			fnInitFilter(&filter, localPawn, mask, 4, 7);

			fnCreateTrace(&td, start, delta, &filter, 4, true);

			fnInitTraceInfo(outTrace);

			bullet_mod_array_t& arr = td.mod_array;

			if (arr.Size > 0 && arr.Data != nullptr) {
				bullet_modulate_entry_t* entry = &arr.Data[0];

				uint16_t surf_idx = entry->surfEnd & 0x7FFF;

				if (surf_idx < 0x80) {
					trace_array_element_t* surf = &td.m_Arr[surf_idx];

					fnGetTraceInfo(&td, outTrace, entry->StartFraction, surf);
					return true;
				}
			}

			return false;
		}

		float CalculateDistanceDamageModifier(const float Distance, const float RangeModifier) {
			return std::pow(RangeModifier, Distance * 0.002f); // / 500.0f
		}

		int GetDamage(const Vector3Struct& StartPosition, const Vector3Struct& TargetPosition, uintptr_t skipPawnPtr, uintptr_t targetPawnPtr, const float WeaponDamage, const float WeaponPenetration, const float WeaponRangeModifier, const float WeaponRange) {
			const Vector3Struct Direction = { TargetPosition.X - StartPosition.X, TargetPosition.Y - StartPosition.Y, TargetPosition.Z - StartPosition.Z };
			const float DistanceToTarget = sqrt(Direction.X * Direction.X + Direction.Y * Direction.Y + Direction.Z * Direction.Z);
			if (DistanceToTarget < 1.0f) return -1;

			const Vector3Struct NormalDirection = { Direction.X / DistanceToTarget, Direction.Y / DistanceToTarget, Direction.Z / DistanceToTarget };

			Vector3Struct Delta = { NormalDirection.X * WeaponRange, NormalDirection.Y * WeaponRange, NormalDirection.Z * WeaponRange };

			alignas(16) TraceDataStruct TraceData {};
			LoadedVariables::fnInitTraceData(&TraceData);

			alignas(16) CTraceFilterStruct Filter;
			LoadedVariables::fnInitFilter(&Filter, skipPawnPtr, 0x1C100B, 4, 7);
			LoadedVariables::fnCreateTrace(&TraceData, StartPosition, Delta, &Filter, 4, true);

			handle_bullet_data_t Bullet{};
			Bullet.Damage = WeaponDamage;
			Bullet.Penetration = WeaponPenetration;
			Bullet.RangeModifier = WeaponRangeModifier;
			Bullet.PenetrationCount = 4;
			Bullet.PenetrationStopped = false;

			bullet_mod_array_t& arr = TraceData.mod_array;

			for (int i = 0; i < arr.Size; ++i) {
				bullet_modulate_entry_t* Entry = &arr.Data[i];

				uint16_t SurfaceIndex = Entry->surfEnd & 0x7FFF;
				if (SurfaceIndex >= 0x80) break;

				trace_array_element_t* Surface = &TraceData.m_Arr[SurfaceIndex];

				alignas(16) CGameTraceStruct Trace {};
				LoadedVariables::fnInitTraceInfo(&Trace);
				LoadedVariables::fnGetTraceInfo(&TraceData, &Trace, Entry->StartFraction, Surface);

				Bullet.TailLength = TraceData.tail_end.Length();

				const void* m_pHitEntity = Trace.m_pHitEntity;
				if (m_pHitEntity) {
					if (reinterpret_cast<uintptr_t>(m_pHitEntity) == targetPawnPtr) {
						const float travelDist = sqrt(pow(Trace.m_EndPosition.X - StartPosition.X, 2.0f) + pow(Trace.m_EndPosition.Y - StartPosition.Y, 2.0f) + pow(Trace.m_EndPosition.Z - StartPosition.Z, 2.0f));
						const float distanceMod = Functions::Raycast::CalculateDistanceDamageModifier(travelDist, Bullet.RangeModifier);

						float FinalDmg = Bullet.Damage * distanceMod;

						// FinalDmg *= 4.0f;
						return static_cast<int>(FinalDmg);
					}
				}

				int team_num = 3;
				if (LoadedVariables::fnHandleBulletPenetration(&TraceData, &Bullet, Entry, team_num, nullptr)) {
					break;
				}

				if (Bullet.PenetrationCount <= 0 || Bullet.PenetrationStopped) {
					break;
				}
			}
			return -1;
		}

		bool DoesRayHitEntity(const Vector3Struct& StartPosition, const Vector3Struct& TargetPosition, uintptr_t skipPawnPtr, uintptr_t* EntityHit, float WeaponDamage, float wpnPenetration, float wpnRangeMod, float wpnRange) {
			Vector3Struct dir = { TargetPosition.X - StartPosition.X, TargetPosition.Y - StartPosition.Y, TargetPosition.Z - StartPosition.Z };
			float distToTarget = sqrt(dir.X * dir.X + dir.Y * dir.Y + dir.Z * dir.Z);
			if (distToTarget < 1.0f) return false;

			Vector3Struct dirNorm = { dir.X / distToTarget, dir.Y / distToTarget, dir.Z / distToTarget };

			Vector3Struct delta = { dirNorm.X * wpnRange, dirNorm.Y * wpnRange, dirNorm.Z * wpnRange };

			alignas(16) TraceDataStruct TraceData {};
			LoadedVariables::fnInitTraceData(&TraceData);

			alignas(16) CTraceFilterStruct Filter;
			LoadedVariables::fnInitFilter(&Filter, skipPawnPtr, 0x1C100B, 100, 7);
			LoadedVariables::fnCreateTrace(&TraceData, StartPosition, delta, &Filter, 100, true);

			handle_bullet_data_t bullet{};
			bullet.Damage = WeaponDamage;
			bullet.Penetration = wpnPenetration;
			bullet.RangeModifier = wpnRangeMod;
			bullet.PenetrationCount = 100;
			bullet.PenetrationStopped = false;

			bullet_mod_array_t& arr = TraceData.mod_array;

			for (int i = 0; i < arr.Size; ++i) {
				bullet_modulate_entry_t* entry = &arr.Data[i];

				uint16_t surf_idx = entry->surfEnd & 0x7FFF;
				if (surf_idx >= 0x80) break;

				trace_array_element_t* surf = &TraceData.m_Arr[surf_idx];

				alignas(16) CGameTraceStruct Trace {};
				LoadedVariables::fnInitTraceInfo(&Trace);
				LoadedVariables::fnGetTraceInfo(&TraceData, &Trace, entry->StartFraction, surf);

				bullet.TailLength = TraceData.tail_end.Length();

				auto m_pHitEntity = Trace.m_pHitEntity;
				if (m_pHitEntity && *(int*)((uintptr_t)m_pHitEntity + Offsets::m_iHealth)) {
					*EntityHit = (uintptr_t)m_pHitEntity;
					return true;
				}

				int team_num = 3;
				if (LoadedVariables::fnHandleBulletPenetration(&TraceData, &bullet, entry, team_num, nullptr)) {
					break;
				}

				if (bullet.PenetrationCount <= 0 || bullet.PenetrationStopped) {
					break;
				}
			}
			return false;
		}

		float GetHitgroupDamageMultiplier(EnabledHitboxesEnum Hitbox) {
			switch (Hitbox) {
			case HEAD:
				return 4.0f;
			case CHEST:
				return 1.0f;
			case ARMS:
				return 1.0f;
			case PELVIS:
				return 1.25f;
			case LEGS:
				return 0.75f;
			default:
				return 1.0f;
			}
		}

		int ScaleDamage(const int Damage, const EnabledHitboxesEnum Hitbox/*, uintptr_t TargetPawn*/) {
			return static_cast<int>(Damage * Functions::Raycast::GetHitgroupDamageMultiplier(Hitbox));

			/*if (enemy->GetArmor() > 0)
			{
				if (hitgroup == HitGroups::HITGROUP_HEAD)
				{
					if (enemy->HasHelmet())
						current_damage *= weapon_armor_ratio * 0.5f;
				}
				else
					current_damage *= weapon_armor_ratio * 0.5f;
			}*/
		}
	}
}






///////////////////////////////////////////////// MEMORY /////////////////////////////////////////////////







namespace Memory {
	// Converte a string do pattern para um vetor de inteiros (com suporte a wildcards '?')
	std::vector<int> pattern_to_byte(const char* pattern) {
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
	uintptr_t FindPattern(const char* module_name, const char* signature) {
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

	uintptr_t GetAbsoluteAddress(uintptr_t addr, int offset, int size) {
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
