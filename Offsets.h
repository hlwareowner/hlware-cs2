#pragma once

#include <windows.h>

// Você também pode guardar os números dos offsets aqui
namespace Offsets {
    // Retirado de offsets.cs
    inline constexpr uintptr_t dwLocalPlayerController = 0x230B5D0;
    inline constexpr uintptr_t dwLocalPlayerPawn = 0x2057720;
    inline constexpr uintptr_t dwEntityList = 0x24D1DF0;

    // Retirado de client_dll.cs
    inline constexpr uintptr_t m_iHealth = 0x34C; // int32
    inline constexpr uintptr_t m_sSanitizedPlayerName = 0x760; // CUtlString
    inline constexpr uintptr_t m_iTeamNum = 0x3EB; // uint8
    inline constexpr uintptr_t m_nBloodType = 0x5F8; // BloodType
    inline constexpr uintptr_t m_bIsLocalPlayerController = 0x780; // bool
    inline constexpr uintptr_t m_iIDEntIndex = 0x343C; // CEntityIndex
    inline constexpr uintptr_t m_hPlayerPawn = 0x904; // CHandle<C_CSPlayerPawn>
    inline constexpr uintptr_t m_bPawnIsAlive = 0x90C; // bool
    inline constexpr uintptr_t m_iPawnHealth = 0x910; // uint32
    inline constexpr uintptr_t m_vOldOrigin = 0x1390; // Vector
    inline constexpr uintptr_t m_pGameSceneNode = 0x330; // CGameSceneNode*


    inline constexpr uintptr_t m_pWeaponServices = 0x11E0; // CPlayer_WeaponServices*
    inline constexpr uintptr_t m_hActiveWeapon = 0x60; // CHandle<C_BasePlayerWeapon>
    inline constexpr uintptr_t m_AttributeManager = 0x1180; // C_AttributeContainer
    inline constexpr uintptr_t m_Item = 0x50; // C_EconItemView
    inline constexpr uintptr_t m_iItemDefinitionIndex = 0x1BA; // uint16
    inline constexpr uintptr_t m_bInReload = 0x17F4; // bool
    inline constexpr uintptr_t m_vecViewOffset = 0xE70; // CNetworkViewOffsetVector

    
    inline constexpr uintptr_t m_glowColorOverride = 0x40; // Color
    inline constexpr uintptr_t m_bGlowing = 0x51; // bool
    inline constexpr uintptr_t m_Glow = 0xDD8; // CGlowProperty


    inline constexpr uintptr_t m_angEyeAngles = 0x3360; // QAngle
    inline constexpr uintptr_t m_pCameraServices = 0x1218; // CPlayer_CameraServices*
    inline constexpr uintptr_t m_modelState = 0x150; // CModelState
    inline constexpr uintptr_t m_iShotsFired = 0x1C64; // int32

    inline constexpr uintptr_t m_flFlashDuration = 0x1400; // float32

    // Retirado de buttons.cs
    inline constexpr uintptr_t dwViewMatrix = 0x2331B30;
    inline constexpr uintptr_t attack = 0x204DA20;
    inline constexpr uintptr_t attack2 = 0x204DAB0;
    inline constexpr uintptr_t back = 0x204DCF0;
    inline constexpr uintptr_t duck = 0x204DFC0;
    inline constexpr uintptr_t forward = 0x204DC60;
    inline constexpr uintptr_t jump = 0x204DF30;
    inline constexpr uintptr_t left = 0x204DD80;
    inline constexpr uintptr_t lookatweapon = 0x233DCA0;
    inline constexpr uintptr_t reload = 0x204D990;
    inline constexpr uintptr_t right = 0x204DE10;
    inline constexpr uintptr_t showscores = 0x233DB80;
    inline constexpr uintptr_t sprint = 0x204D900;
    inline constexpr uintptr_t turnleft = 0x204DB40;
    inline constexpr uintptr_t turnright = 0x204DBD0;
    inline constexpr uintptr_t use = 0x204DEA0;
    inline constexpr uintptr_t zoom = 0x233DC10;
}