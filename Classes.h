#pragma once

#define NOMINMAX
#undef max

#include <unordered_map>
#include <string>
#include <Windows.h>
#include <stdint.h>
#include <limits>

#define MULTIPLAYER_BACKUP 150

struct ColorRGBAStruct {
    float Red = 0.0f;
    float Green = 0.0f;
    float Blue = 0.0f;
    float Alpha = 1.0f;

    ColorRGBAStruct(float _r, float _g, float _b, float _a = 255.0f) {
        Red = _r / 255.0f;
        Green = _g / 255.0f;
        Blue = _b / 255.0f;
        Alpha = _a / 255.0f;
    }
};

struct Vector3Struct {
    float X = 0.0f;
    float Y = 0.0f;
    float Z = 0.0f;
    float Length() {
        return std::sqrt(X * X + Y * Y + Z * Z);
    }
    Vector3Struct(float X = 0, float Y = 0, float Z = 0) : X{ X }, Y{ Y }, Z{ Z } {}
    Vector3Struct operator+(const Vector3Struct& Sum) const {
        return Vector3Struct(X + Sum.X, Y + Sum.Y, Z + Sum.Z);
    }
};

struct Vector2Struct {
    float X = 0.0f;
    float Y = 0.0f;
};

struct QAngleStruct {
    float Pitch = 0.0f;
    float Yaw = 0.0f;
    float Roll = 0.0f;
};

struct ViewMatrixStruct {
    float Matrix[4][4];
};

struct WeaponInformationStruct {
    std::string Name = "";
	bool HasScope = false;
    float Damage = 0;
    float Penetration = 0;
    float RangeModifier = 0;
    float Range = 0;
};

enum CommandButtons : int {
    IN_ATTACK = (1 << 0),
    IN_JUMP = (1 << 1),
    IN_DUCK = (1 << 2),
    IN_FORWARD = (1 << 3),
    IN_BACK = (1 << 4),
    IN_USE = (1 << 5),
    IN_CANCEL = (1 << 6),
    IN_LEFT = (1 << 7),
    IN_RIGHT = (1 << 8),
    IN_MOVELEFT = (1 << 9),
    IN_MOVERIGHT = (1 << 10),
    IN_SECOND_ATTACK = (1 << 11),
    IN_RUN = (1 << 12),
    IN_RELOAD = (1 << 13),
    IN_LEFT_ALT = (1 << 14),
    IN_RIGHT_ALT = (1 << 15),
    IN_SCORE = (1 << 16),
    IN_SPEED = (1 << 17),
    IN_WALK = (1 << 18),
    IN_ZOOM = (1 << 19),
    IN_FIRST_WEAPON = (1 << 20),
    IN_SECOND_WEAPON = (1 << 21),
    IN_BULLRUSH = (1 << 22),
    IN_FIRST_GRENADE = (1 << 23),
    IN_SECOND_GRENADE = (1 << 24),
    IN_MIDDLE_ATTACK = (1 << 25),
    IN_USE_OR_RELOAD = (1 << 26)
};

struct RagebotHitboxDataStruct {
    int Damage;
    bool IsSelected;
    Vector3Struct Position;
};

enum EnabledHitboxesEnum {
    NONE = 0,
    HEAD = 1 << 0, // 1  (00001 em binário)
    CHEST = 1 << 1, // 2  (00010 em binário)
    PELVIS = 1 << 2, // 4  (00100 em binário)
    ARMS = 1 << 3, // 8 (01000 em binário)
    LEGS = 1 << 4  // 16 (10000 em binário)
};

struct WeaponRagebotAttributesStruct {
    bool IsEnabled = false;
    EnabledHitboxesEnum Hitboxes;
	bool AutomaticScope = false;
    int MinimumDamage = 0;
    int Hitchance = 0;
};

// Usando unordered_map para performance máxima no loop do ragebot
inline std::unordered_map<int16_t, WeaponInformationStruct> WeaponMap = {
    // --- Pistolas (Dano no Peito s/ Colete) ---
    { 1,  { "Deagle", false, 63.0f, 1.86f, 0.81f, 8192.0f } },
    { 2,  { "Dual Berettas", false, 38.0f, 1.15f, 0.79f, 8192.0f } },
    { 3,  { "Five-Seven", false, 32.0f, 1.82f, 0.81f, 8192.0f } },
    { 4,  { "Glock", false, 28.0f, 0.94f, 0.85f, 8192.0f } },
    { 30, { "Tec-9", false, 33.0f, 1.81f, 0.79f, 8192.0f } },
    { 32, { "P2000", false, 35.0f, 1.01f, 0.85f, 8192.0f } },
    { 36, { "P250", false, 38.0f, 1.28f, 0.85f, 8192.0f } },
    { 61, { "USP-S", false, 35.0f, 1.01f, 0.91f, 8192.0f } },
    { 63, { "CZ75", false, 31.0f, 1.55f, 0.85f, 8192.0f } },
    { 64, { "R8", false, 86.0f, 1.86f, 0.90f, 8192.0f } },

    // --- Rifles ---
    { 7,  { "AK-47", false, 36.0f, 1.55f, 0.98f, 8192.0f } },
    { 8,  { "AUG", true, 28.0f, 1.80f, 0.98f, 8192.0f } },
    { 10, { "FAMAS", false, 30.0f, 1.40f, 0.96f, 8192.0f } },
    { 13, { "Galil AR", false, 30.0f, 1.55f, 0.98f, 8192.0f } },
    { 16, { "M4A4", false, 33.0f, 1.40f, 0.97f, 8192.0f } },
    { 39, { "SG 553", true, 30.0f, 2.00f, 0.98f, 8192.0f } },
    { 60, { "M4A1-S", false, 38.0f, 1.40f, 0.99f, 8192.0f } },

    // --- Snipers ---
    { 9,  { "AWP", true, 115.0f, 1.95f, 0.99f, 8192.0f } },
    { 11, { "G3SG1", true, 80.0f, 1.65f, 0.98f, 8192.0f } },
    { 38, { "SCAR-20", true, 80.0f, 1.65f, 0.98f, 8192.0f } },
    { 40, { "SSG 08", true, 88.0f, 1.70f, 0.98f, 8192.0f } },

    // --- Submetralhadoras (SMGs) ---
    { 17, { "MAC-10", false, 29.0f, 1.15f, 0.80f, 8192.0f } },
    { 19, { "P90", false, 26.0f, 1.38f, 0.84f, 8192.0f } },
    { 23, { "MP5-SD", false, 27.0f, 1.25f, 0.84f, 8192.0f } },
    { 24, { "UMP-45", false, 35.0f, 1.30f, 0.82f, 8192.0f } },
    { 26, { "PP-Bizon", false, 27.0f, 1.15f, 0.80f, 8192.0f } },
    { 34, { "MP9", false, 26.0f, 1.20f, 0.87f, 8192.0f } },

    // --- Pesadas (Heavy) ---
    { 14, { "M249", false, 32.0f, 1.60f, 0.97f, 8192.0f } },
    { 25, { "XM1014", false, 20.0f, 1.60f, 0.70f, 3000.0f } }, // Por pellet (x6)
    { 35, { "Nova", false, 26.0f, 1.00f, 0.70f, 3000.0f } },   // Por pellet (x9)
    { 27, { "MAG-7", false, 30.0f, 1.50f, 0.70f, 3000.0f } },  // Por pellet (x8)
    { 28, { "Negev", false, 35.0f, 1.42f, 0.97f, 8192.0f } },
    { 29, { "Sawed-Off", false, 32.0f, 1.50f, 0.70f, 3000.0f } }, // Por pellet (x8)
    { 31, { "Taser", false, 0.0f, 0.0f, 0.0f, 180.0f } },      // Lógica de dano especial

    // --- Granadas / Utilitários ---
    { 43, { "Flashbang", false, 0.0f, 0.0f, 0.0f, 0.0f } },
    { 44, { "HE Grenade", false, 98.0f, 1.20f, 1.0f, 0.0f } },
    { 45, { "Smoke Grenade", false, 0.0f, 0.0f, 0.0f, 0.0f } },
    { 46, { "Molotov", false, 40.0f, 1.0f, 1.0f, 0.0f } },
    { 47, { "Decoy Grenade", false, 0.0f, 0.0f, 0.0f, 0.0f } },
    { 48, { "Incendiary Grenade", false, 40.0f, 1.0f, 1.0f, 0.0f } },
    { 49, { "C4", false, 500.0f, 1.0f, 1.0f, 0.0f } },

    // --- Facas ---
    { 42, { "Knife (CT)", false, 34.0f, 1.70f, 1.0f, 64.0f } },
    { 59, { "Knife (T)", false, 34.0f, 1.70f, 1.0f, 64.0f } }
};

inline std::unordered_map<int16_t, WeaponRagebotAttributesStruct> RagebotWeaponSettingsMap = {
    // --- Pistolas ---
    { 1,  { true, HEAD, false, 100, 60 } },
    { 2,  { true, HEAD, false, 100, 60 } },
    { 3,  { true, HEAD, false, 100, 60 } },
    { 4,  { true, HEAD, false, 100, 60 } },
    { 30, { true, HEAD, false, 100, 60 } },
    { 32, { true, HEAD, false, 100, 60 } },
    { 36, { true, HEAD, false, 100, 60 } },
    { 61, { true, HEAD, false, 100, 60 } },
    { 63, { true, HEAD, false, 100, 60 } },
    { 64, { true, HEAD, false, 100, 60 } },

    // --- Rifles ---
    { 7,  { true, HEAD, false, 100, 60 } }, // No CS2 alguns dumps usam 7 ou 4, verifique seu dumper
    { 8,  { true, HEAD, false, 100, 60 } },
    { 10, { true, HEAD, false, 100, 60 } },
    { 13, { true, HEAD, false, 100, 60 } },
    { 16, { true, HEAD, false, 100, 60 } },
    { 39, { true, HEAD, false, 100, 60 } },
    { 60, { true, HEAD, false, 100, 60 } },

    // --- Snipers ---
    { 9,  { true, HEAD, false, 100, 60 } },
    { 11, { true, HEAD, false, 100, 60 } },
    { 38, { true, HEAD, false, 100, 60 } },
    { 40, { true, HEAD, false, 100, 60 } },

    // --- Submetralhadoras (SMGs) ---
    { 17, { true, HEAD, false, 100, 60 } },
    { 19, { true, HEAD, false, 100, 60 } },
    { 23, { true, HEAD, false, 100, 60 } },
    { 24, { true, HEAD, false, 100, 60 } },
    { 26, { true, HEAD, false, 100, 60 } },
    { 34, { true, HEAD, false, 100, 60 } },


    // --- Pesadas (Heavy) ---
    { 14, { true, HEAD, false, 100, 60 } },
    { 25, { true, HEAD, false, 100, 60 } },
    { 35, { true, HEAD, false, 100, 60 } },
    { 27, { true, HEAD, false, 100, 60 } },
    { 28, { true, HEAD, false, 100, 60 } },
    { 29, { true, HEAD, false, 100, 60 } },
    { 31, { true, HEAD, false, 100, 60 } },

    // --- Granadas / Utilitários ---
    { 43, { true, HEAD, false, 100, 60 } },
    { 44, { true, HEAD, false, 100, 60 } },
    { 45, { true, HEAD, false, 100, 60 } },
    { 46, { true, HEAD, false, 100, 60 } },
    { 47, { true, HEAD, false, 100, 60 } },
    { 48, { true, HEAD, false, 100, 60 } },
    { 49, { true, HEAD, false, 100, 60 } },

    // --- Facas (IDs Base) ---
    { 42, { true, HEAD, false, 0, 60 } },
    { 59, { true, HEAD, false, 0, 60 } }
};









///////////////////////////////////////////////// CSGO INPUT ////////////////////////////////////////////////////////////







enum ESubtickMoveStepBits : std::uint32_t
{
	MOVESTEP_BITS_BUTTON = 0x1U,
	MOVESTEP_BITS_PRESSED = 0x2U,
	MOVESTEP_BITS_WHEN = 0x4U,
	MOVESTEP_BITS_ANALOG_FORWARD_DELTA = 0x8U,
	MOVESTEP_BITS_ANALOG_LEFT_DELTA = 0x10U
};

enum EInputHistoryBits : std::uint32_t
{
	INPUT_HISTORY_BITS_VIEWANGLES = 0x1U,
	INPUT_HISTORY_BITS_SHOOTPOSITION = 0x2U,
	INPUT_HISTORY_BITS_TARGETHEADPOSITIONCHECK = 0x4U,
	INPUT_HISTORY_BITS_TARGETABSPOSITIONCHECK = 0x8U,
	INPUT_HISTORY_BITS_TARGETANGCHECK = 0x10U,
	INPUT_HISTORY_BITS_CL_INTERP = 0x20U,
	INPUT_HISTORY_BITS_SV_INTERP0 = 0x40U,
	INPUT_HISTORY_BITS_SV_INTERP1 = 0x80U,
	INPUT_HISTORY_BITS_PLAYER_INTERP = 0x100U,
	INPUT_HISTORY_BITS_RENDERTICKCOUNT = 0x200U,
	INPUT_HISTORY_BITS_RENDERTICKFRACTION = 0x400U,
	INPUT_HISTORY_BITS_PLAYERTICKCOUNT = 0x800U,
	INPUT_HISTORY_BITS_PLAYERTICKFRACTION = 0x1000U,
	INPUT_HISTORY_BITS_FRAMENUMBER = 0x2000U,
	INPUT_HISTORY_BITS_TARGETENTINDEX = 0x4000U
};

enum EButtonStatePBBits : uint32_t
{
	BUTTON_STATE_PB_BITS_BUTTONSTATE1 = 0x1U,
	BUTTON_STATE_PB_BITS_BUTTONSTATE2 = 0x2U,
	BUTTON_STATE_PB_BITS_BUTTONSTATE3 = 0x4U
};

enum EBaseCmdBits : std::uint32_t
{
	BASE_BITS_MOVE_CRC = 0x1U,
	BASE_BITS_BUTTONPB = 0x2U,
	BASE_BITS_VIEWANGLES = 0x4U,
	BASE_BITS_COMMAND_NUMBER = 0x8U,
	BASE_BITS_CLIENT_TICK = 0x10U,
	BASE_BITS_FORWARDMOVE = 0x20U,
	BASE_BITS_LEFTMOVE = 0x40U,
	BASE_BITS_UPMOVE = 0x80U,
	BASE_BITS_IMPULSE = 0x100U,
	BASE_BITS_WEAPON_SELECT = 0x200U,
	BASE_BITS_RANDOM_SEED = 0x400U,
	BASE_BITS_MOUSEDX = 0x800U,
	BASE_BITS_MOUSEDY = 0x1000U,
	BASE_BITS_CONSUMED_SERVER_ANGLE = 0x2000U,
	BASE_BITS_CMD_FLAGS = 0x4000U,
	BASE_BITS_ENTITY_HANDLE = 0x8000U
};

enum ECSGOUserCmdBits : std::uint32_t
{
	CSGOUSERCMD_BITS_BASECMD = 0x1U,
	CSGOUSERCMD_BITS_LEFTHAND = 0x2U,
	CSGOUSERCMD_BITS_PREDICTING_BODY_SHOT = 0x4U,
	CSGOUSERCMD_BITS_PREDICTING_HEAD_SHOT = 0x8U,
	CSGOUSERCMD_BITS_PREDICTING_KILL_RAGDOLLS = 0x10U,
	CSGOUSERCMD_BITS_ATTACK3START = 0x20U,
	CSGOUSERCMD_BITS_ATTACK1START = 0x40U,
	CSGOUSERCMD_BITS_ATTACK2START = 0x80U
};

template <typename T>
struct RepeatedPtrField_t
{
	struct Rep_t
	{
		int nAllocatedSize;
		T* tElements[(std::numeric_limits<int>::max() - 2 * sizeof(int)) / sizeof(void*)];
	};

	void* pArena;
	int nCurrentSize;
	int nTotalSize;
	Rep_t* pRep;
};

class CBasePB
{
public:
	std::byte pad01[0x8];
	std::uint32_t nHasBits; // 0x8
	std::uint64_t nCachedBits; // 0xC
};//size: 0x18

struct CMsgQAngle : public CBasePB {
	QAngleStruct angValue;
}; //size: 0x28

class CMsgVector : public CBasePB
{
public:
	void* vecValue; // 0x18 //vec4
};

class CCSGOInterpolationInfoPB : public CBasePB
{
public:
	float flFraction; // 0x18
	//int nSrcTick; // 0x1C
	//int nDstTick; // 0x20
};

class CCSGOInputHistoryEntryPB : public CBasePB
{
public:
	CMsgQAngle* pViewAngles; // 0x18
	CMsgVector* pShootPosition; // 0x20
	CMsgVector* pTargetHeadPositionCheck; // 0x28
	CMsgVector* pTargetAbsPositionCheck; // 0x30
	CMsgQAngle* pTargetAngPositionCheck; // 0x38
	CCSGOInterpolationInfoPB* cl_interp; // 0x40
	CCSGOInterpolationInfoPB* sv_interp0; // 0x48
	CCSGOInterpolationInfoPB* sv_interp1; // 0x50
	CCSGOInterpolationInfoPB* player_interp; // 0x58
	int nRenderTickCount; // 0x60
	float flRenderTickFraction; // 0x64
	int nPlayerTickCount; // 0x68
	float flPlayerTickFraction; // 0x6C
	int nFrameNumber; // 0x70
	int nTargetEntIndex; // 0x74
};

struct CInButtonStatePBStruct : CBasePB
{
	std::uint64_t nValue;
	std::uint64_t nValueChanged;
	std::uint64_t nValueScroll;
};

struct CSubtickMoveStep : CBasePB {
	std::uint64_t nButton;
	bool bPressed;
	float flWhen;
	float flAnalogForwardDelta;
	float flAnalogLeftDelta;
};

struct CBaseUserCmdPB : public CBasePB {
	RepeatedPtrField_t<CSubtickMoveStep> subtickMovesField; //0x18
	std::string* strMoveCrc; //0x30
	CInButtonStatePBStruct* pInButtonState; //0x38
	CMsgQAngle* pViewAngles;
	std::int32_t nLegacyCommandNumber;
	std::int32_t nClientTick;
	float flForwardMove; //Forwardmove, sidemove (and upmove?) - Range: -1.0 - 1.0
	float flSideMove;
	float flUpMove;
	std::int32_t nImpulse;
	std::int32_t nWeaponSelect;
	std::int32_t nRandomSeed;
	std::int32_t nMousedX;
	std::int32_t nMousedY;
	std::uint32_t nConsumedServerAngleChanges;
	std::int32_t nCmdFlags;
	std::uint32_t nPawnEntityHandle;

}; //size: 0x68?

struct CCSGOUserCmdPB {
	std::uint32_t nHasBits; //0x0
	std::uint64_t nCachedSize; //0x4
	RepeatedPtrField_t<CCSGOInputHistoryEntryPB> inputHistoryField; //0xC
	CBaseUserCmdPB* pBaseCmd; //0x14 //0x40
	bool bLeftHandDesired; //0x1C
	bool bIsPredictingBodyShotFX;
	bool bIsPredictingHeadShotFX;
	bool bIsPredictingKillRagdolls;
	std::int32_t nAttack3StartHistoryIndex; //0x20
	std::int32_t nAttack1StartHistoryIndex; //0x24
	std::int32_t nAttack2StartHistoryIndex; //0x28

	void CheckAndSetBits(std::uint32_t nBits) {
		if (!(nHasBits & nBits))
			nHasBits |= nBits;
	}
};

struct CInButtonStateStruct {
	std::byte pad01[0x8];
	std::uint64_t nValue; // 0x8
	std::uint64_t nValueChanged; // 0x10
	std::uint64_t nValueScroll; // 0x18
};

struct CUserCommandStruct {
	std::byte pad01[0x8];
	std::byte pad02[0x10];
	CCSGOUserCmdPB csgoUserCmd; // 0x18
	CInButtonStateStruct nButtons; // 0x58
	std::byte pad03[0x20];

	CCSGOInputHistoryEntryPB* GetInputHistoryEntry(int nIndex) {
		if (nIndex >= csgoUserCmd.inputHistoryField.pRep->nAllocatedSize || nIndex >= csgoUserCmd.inputHistoryField.nCurrentSize)
			return nullptr;

		return csgoUserCmd.inputHistoryField.pRep->tElements[nIndex];
	}

	void SetSubTickAngle(const QAngleStruct& angView) {
		for (int i = 0; i < this->csgoUserCmd.inputHistoryField.pRep->nAllocatedSize; i++) {
			CCSGOInputHistoryEntryPB* pInputEntry = this->GetInputHistoryEntry(i);
			if (!pInputEntry || !pInputEntry->pViewAngles)
				continue;

			pInputEntry->pViewAngles->angValue = angView;
			//pInputEntry->nCachedBits |= EInputHistoryBits::INPUT_HISTORY_BITS_VIEWANGLES;
		}
	}

	bool IsButtonPressed(uint64_t button) const {
		if (!csgoUserCmd.pBaseCmd)
			return false;

		return csgoUserCmd.pBaseCmd->pInButtonState->nValue & button;
	}
};

struct CCSGOInput {
	char pad_0000[0x250]; //0x0000
	bool block_shot; //0x0250
	bool in_thirdperson; //0x0251
	char pad_0252[0x6]; //0x0252
	Vector3Struct third_person_angles; //0x0258 note: Vec3
	char pad_0264[0x14]; //0x0264
	uint64_t button_pressed; //0x0278
	uint64_t mouse_button_pressed; //0x0280
	uint64_t button_un_pressed; //0x0288
	uint64_t keyboard_copy; //0x0290
	float forward_move; //0x0298
	float left_move; //0x029C
	float up_move; //0x02A0
	int mouse_delta_x;
	int mouse_delta_y;
	int32_t subtick_count; //0x02AC
	void* subticks[0xC];//c_subtick_input subticks[12]; //0x02B0
	QAngleStruct view_angles; //0x03D0 note: QAngle_t
	int32_t target_entity_index; //0x03DC
	char pad_03E0[0x230]; //0x03E0
	int32_t attack_history_1; //0x0610
	int32_t attack_history_2; //0x0614
	int32_t attack_history_3; //0x0618
	char pad_061C[0x4]; //0x061C
	int32_t message_size; //0x0620
	char pad_0624[0x4]; //0x0624
	void* message;//c_cs_input_message* message; //0x0628
};
