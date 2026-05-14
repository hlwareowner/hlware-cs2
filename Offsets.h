#pragma once

#include <windows.h>

// Você também pode guardar os números dos offsets aqui
namespace Offsets {
    // Retirado de offsets.cs
        inline constexpr uintptr_t dwLocalPlayerController = 0x230A4F0;
        inline constexpr uintptr_t dwLocalPlayerPawn = 0x2056700;
        inline constexpr uintptr_t dwEntityList = 0x24D0DC0;
        inline constexpr uintptr_t dwCSGOInput = 0x233FC00;
        inline constexpr uintptr_t dwViewAngles = 0x2340288;

    // Retirado de client_dll.cs
        // C_BaseEntity {
            inline constexpr uintptr_t m_iHealth = 0x34C; // int32
            inline constexpr uintptr_t m_iTeamNum = 0x3EB; // uint8
            inline constexpr uintptr_t m_pGameSceneNode = 0x330; // CGameSceneNode*
            inline constexpr uintptr_t m_modelState = 0x150; // CModelState
            inline constexpr uintptr_t m_nBloodType = 0x5F8; // BloodType
        // }

        // CBasePlayerController {
            inline constexpr uintptr_t m_bIsLocalPlayerController = 0x788; // bool
            inline constexpr uintptr_t m_iszPlayerName = 0x6F0; // char[128]
            //inline constexpr uintptr_t m_hPawn = 0x6BC; // CHandle<C_BasePlayerPawn>
        // }

        // CCSPlayerController {
            inline constexpr uintptr_t m_hPlayerPawn = 0x90C; // CHandle<C_CSPlayerPawn>
            inline constexpr uintptr_t m_iPawnHealth = 0x918; // uint32
            inline constexpr uintptr_t m_bPawnIsAlive = 0x914; // bool
            inline constexpr uintptr_t m_hObserverPawn = 0x910; // CHandle<C_CSObserverPawn>
        // }

        // C_BasePlayerPawn {
            inline constexpr uintptr_t m_pWeaponServices = 0x11E0; // CPlayer_WeaponServices*
            inline constexpr uintptr_t m_vOldOrigin = 0x1390; // Vector
        // }

        // C_CSPlayerPawnBase {
            inline constexpr uintptr_t m_flFlashDuration = 0x1400; // float32
        // }

        // CPlayer_MovementServices {
            inline constexpr uintptr_t m_nButtons = 0x50; // CInButtonState
            inline constexpr uintptr_t m_flCmdLeftMove = 0x1A4; // float32
        // }

        // C_CSPlayerPawn {
            inline constexpr uintptr_t m_pAimPunchServices = 0x1490; // CCSPlayer_AimPunchServices*
            inline constexpr uintptr_t m_ArmorValue = 0x1C7C; // int32
            inline constexpr uintptr_t m_angEyeAngles = 0x3360; // QAngle
            inline constexpr uintptr_t m_bIsDefusing = 0x1C52; // bool
            inline constexpr uintptr_t m_bGunGameImmunity = 0x3288; // bool
            inline constexpr uintptr_t m_pMovementServices = 0x1220; // CPlayer_MovementServices*
            inline constexpr uintptr_t m_iIDEntIndex = 0x343C; // CEntityIndex
            inline constexpr uintptr_t m_flViewmodelOffsetX = 0x1B70; // float32
            inline constexpr uintptr_t m_flViewmodelOffsetY = 0x1B74; // float32
            inline constexpr uintptr_t m_flViewmodelOffsetZ = 0x1B78; // float32
            inline constexpr uintptr_t m_flViewmodelFOV = 0x1B7C; // float32
        // }

        // C_BaseModelEntity {
            inline constexpr uintptr_t m_vecViewOffset = 0xE70; // CNetworkViewOffsetVector
            inline constexpr uintptr_t m_Glow = 0xDD8; // CGlowProperty
        // }

        // C_GlowProperty {
            inline constexpr uintptr_t m_bGlowing = 0x51; // bool
            inline constexpr uintptr_t m_glowColorOverride = 0x40; // Color
        // }

        //  CPlayer_WeaponServices {
            inline constexpr uintptr_t m_hActiveWeapon = 0x60; // CHandle<C_BasePlayerWeapon>
            inline constexpr uintptr_t m_iAmmo = 0x68; // uint16[32]
        // }

        // C_EconEntity {
            inline constexpr uintptr_t m_AttributeManager = 0x1180; // C_AttributeContainer
            inline constexpr uintptr_t m_iItemDefinitionIndex = 0x1BA; // uint16
        // }
        
        // C_AttributeContainer{
            inline constexpr uintptr_t m_Item = 0x50; // C_EconItemView
        // }

        
        inline constexpr uintptr_t m_bInReload = 0x17F4; // bool
        
    
        inline constexpr uintptr_t m_pCameraServices = 0x1218; // CPlayer_CameraServices*
        inline constexpr uintptr_t m_iShotsFired = 0x1C64; // int32

    
    // Retirado de buttons.cs
        inline constexpr uintptr_t dwViewMatrix = 0x2330AE0;
        inline constexpr uintptr_t attack = 0x2050990;
        inline constexpr uintptr_t attack2 = 0x204DAB0;
        inline constexpr uintptr_t duck = 0x204DFC0;

    // CCSWeaponBaseVData {
        inline constexpr uintptr_t m_nDamage = 0x828; // int32
        inline constexpr uintptr_t m_flPenetration = 0x834; // float32
        inline constexpr uintptr_t m_flRange = 0x838; // float32
        inline constexpr uintptr_t m_flRangeModifier = 0x83C; // float32
    // }
}
