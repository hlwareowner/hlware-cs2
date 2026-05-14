#include "../Includes.h"

void MakeBoxForPlayer(const uintptr_t PlayerPawn) {
    const Vector3Struct FeetPosition = *(Vector3Struct*)(PlayerPawn + Offsets::m_vOldOrigin);
    const Vector3Struct HeadPosition = { FeetPosition.X, FeetPosition.Y, FeetPosition.Z + Settings::CHARACTER_HEIGHT }; // 72 é a altura média no CS

    Vector2Struct ScreenFeetPosition;
    Vector2Struct ScreenHeadPosition;

    if (Functions::Calculations::WorldToScreen(FeetPosition, ScreenFeetPosition) && Functions::Calculations::WorldToScreen(HeadPosition, ScreenHeadPosition)) {
        const float Height = ScreenFeetPosition.Y - ScreenHeadPosition.Y;
        const float Width = Height / Settings::BOX_ASPECT_RATIO; // Proporção largura/altura
        const float X = ScreenHeadPosition.X - (Width * 0.5f);
        const float Y = ScreenHeadPosition.Y;
        Functions::ImGuiFunctions::DrawBox(X, Y, Width, Height, Settings::BOX_THICKNESS, Attributes::ESP::ESP::BoxColor);
    }
}

void MakeHealthBarForPlayer(const uintptr_t PlayerPawn) {
    const Vector3Struct FeetPosition = *(Vector3Struct*)(PlayerPawn + Offsets::m_vOldOrigin);
    const Vector3Struct HeadPosition = { FeetPosition.X, FeetPosition.Y, FeetPosition.Z + Settings::CHARACTER_HEIGHT }; // 72 é a altura média no CS

    Vector2Struct ScreenFeetPosition;
    Vector2Struct ScreenHeadPosition;

    if (Functions::Calculations::WorldToScreen(FeetPosition, ScreenFeetPosition) && Functions::Calculations::WorldToScreen(HeadPosition, ScreenHeadPosition)) {
        const float Height = ScreenFeetPosition.Y - ScreenHeadPosition.Y;
        const float Width = Height / Settings::BOX_ASPECT_RATIO; // Proporção largura/altura

        const float LeftX = ScreenHeadPosition.X - (Width / 2.0f);
        const float RightX = ScreenHeadPosition.X + (Width / 2.0f);
        const float TopY = ScreenHeadPosition.Y;
        const float BottomY = ScreenFeetPosition.Y;

        const int PlayerHealth = *(int*)(PlayerPawn + Offsets::m_iHealth);
        const float PlayerHealthFloat = static_cast<float>(PlayerHealth);

        const ImVec2 HealthBarTop{ LeftX - Settings::HEALTH_BAR_DISTANCE_OFFSET, TopY - (TopY - BottomY) * (1.0f - PlayerHealthFloat * 0.01f)};
        const ImVec2 HealthBarBottom{ LeftX - Settings::HEALTH_BAR_DISTANCE_OFFSET, BottomY};
        
        Functions::ImGuiFunctions::DrawLine(HealthBarTop, HealthBarBottom, Settings::HEALTH_BAR_THICKNESS, Includes::LerpRGBAColor(PlayerHealthFloat / 100.0f, Attributes::ESP::ESP::HealthBarColor1, Attributes::ESP::ESP::HealthBarColor2));
    }
}

void MakeNameForPlayer(const uintptr_t PlayerPawn) {
    const Vector3Struct FeetPosition = *reinterpret_cast<Vector3Struct*>(PlayerPawn + Offsets::m_vOldOrigin);
    const Vector3Struct HeadPosition = { FeetPosition.X, FeetPosition.Y, FeetPosition.Z + Settings::CHARACTER_HEIGHT }; // 72 é a altura média no CS

    Vector2Struct ScreenHeadPosition;

    if (Functions::Calculations::WorldToScreen(HeadPosition, ScreenHeadPosition)) {
        const char* PlayerName = ("Name");

        ImVec2 TextSize = ImGui::CalcTextSize(PlayerName);

        ImVec2 TextPosition = {
            ScreenHeadPosition.X - (TextSize.x / 2.0f),
            ScreenHeadPosition.Y - TextSize.y - Settings::NAME_POSITION_OFFSET
        };

        // Desenhamos uma sombra ou contorno (opcional, mas ajuda na visibilidade)
        //drawList->AddText({ textPos.x + 1, textPos.y + 1 }, ImColor(0, 0, 0, 255), name);
        Functions::ImGuiFunctions::DrawText(PlayerName, TextPosition, Attributes::ESP::ESP::NameColor); //drawList->AddText(textPos, color, name);
    }
}

void MakeSkeletonForPlayer(const uintptr_t PlayerPawn) {
    const uintptr_t SceneNode = *reinterpret_cast<uintptr_t*>(PlayerPawn + Offsets::m_pGameSceneNode);
    const uintptr_t BoneMatrix = *reinterpret_cast<uintptr_t*>(SceneNode + Offsets::m_modelState + 0x80);

    const Vector3Struct HeadPosition = *(Vector3Struct*)(BoneMatrix + 7 * 32);
    const Vector3Struct NeckPosition = *(Vector3Struct*)(BoneMatrix + 6 * 32);
    const Vector3Struct ChestPosition = *(Vector3Struct*)(BoneMatrix + 23 * 32);
    const Vector3Struct PelvisPosition = *(Vector3Struct*)(BoneMatrix + 1 * 32);
    const Vector3Struct LeftShoulderPosition = *(Vector3Struct*)(BoneMatrix + 9 * 32);
    const Vector3Struct LeftElbowPosition = *(Vector3Struct*)(BoneMatrix + 10 * 32);
    const Vector3Struct LeftHandPosition = *(Vector3Struct*)(BoneMatrix + 11 * 32);
    const Vector3Struct RightShoulderPosition = *(Vector3Struct*)(BoneMatrix + 13 * 32);
    const Vector3Struct RightElbowPosition = *(Vector3Struct*)(BoneMatrix + 14 * 32);
    const Vector3Struct RightHandPosition = *(Vector3Struct*)(BoneMatrix + 15 * 32);
    const Vector3Struct LeftHipPosition = *(Vector3Struct*)(BoneMatrix + 17 * 32);
    const Vector3Struct LeftKneePosition = *(Vector3Struct*)(BoneMatrix + 18 * 32);
    const Vector3Struct LeftFootPosition = *(Vector3Struct*)(BoneMatrix + 19 * 32);
    const Vector3Struct RightHipPosition = *(Vector3Struct*)(BoneMatrix + 20 * 32);
    const Vector3Struct RightKneePosition = *(Vector3Struct*)(BoneMatrix + 21 * 32);
    const Vector3Struct RightFootPosition = *(Vector3Struct*)(BoneMatrix + 22 * 32);

    const ColorRGBAStruct SkeletonColor = Attributes::ESP::ESP::SkeletonColor;

    auto DrawLine = [&](const Vector3Struct& Bone13DPosition, const Vector3Struct& Bone23DPosition) {
        Vector2Struct Bone1ScreenPosition;
        Vector2Struct Bone2ScreenPosition;

        if (Functions::Calculations::WorldToScreen(Bone13DPosition, Bone1ScreenPosition) && Functions::Calculations::WorldToScreen(Bone23DPosition, Bone2ScreenPosition))
            Functions::ImGuiFunctions::DrawLine(ImVec2(Bone1ScreenPosition.X, Bone1ScreenPosition.Y), ImVec2(Bone2ScreenPosition.X, Bone2ScreenPosition.Y), Settings::SKELETON_THICKNESS, SkeletonColor);
    };

    DrawLine(HeadPosition, NeckPosition);
    DrawLine(NeckPosition, ChestPosition);
    DrawLine(ChestPosition, PelvisPosition);
    DrawLine(NeckPosition, LeftShoulderPosition); // Left arm
    DrawLine(LeftShoulderPosition, LeftElbowPosition);
    DrawLine(LeftElbowPosition, LeftHandPosition);
    DrawLine(NeckPosition, RightShoulderPosition); // Right arm
    DrawLine(RightShoulderPosition, RightElbowPosition);
    DrawLine(RightElbowPosition, RightHandPosition);
    DrawLine(PelvisPosition, LeftHipPosition);
    DrawLine(LeftHipPosition, LeftKneePosition);
    DrawLine(LeftKneePosition, LeftFootPosition);
    DrawLine(PelvisPosition, RightHipPosition);
    DrawLine(RightHipPosition, RightKneePosition);
    DrawLine(RightKneePosition, RightFootPosition);

    //auto DrawCurvedBone = [&](const Vector3Struct& p0p, const Vector3Struct& p1p, const Vector3Struct& p2p) {
    //    Vector2Struct p0, p1, p2;
    //    Functions::Calculations::WorldToScreen(p0p, p0);
    //    Functions::Calculations::WorldToScreen(p1p, p1);
    //    Functions::Calculations::WorldToScreen(p2p, p2);

    //    const int segments = 10; // Quanto mais segmentos, mais liso fica
    //    Vector2Struct previousPoint = p0;

    //    for (int i = 1; i <= segments; i++) {
    //        float t = (float)i / (float)segments;

    //        // Aplica a fórmula de Bézier
    //        float x = (1 - t) * (1 - t) * p0.X + 2 * (1 - t) * t * p1.X + t * t * p2.X;
    //        float y = (1 - t) * (1 - t) * p0.Y + 2 * (1 - t) * t * p1.Y + t * t * p2.Y;

    //        Vector2Struct currentPoint = { x, y };
    //        ImVec2 curr = { x, y };
    //        ImVec2 curr2 = { previousPoint.X, previousPoint.Y };
    //        Functions::ImGuiFunctions::DrawLine(curr, curr2, Settings::SKELETON_THICKNESS, SkeletonColor);
    //        previousPoint = currentPoint;
    //    }
    //};
    //auto DrawCurvedBone2 = [&](const Vector3Struct& p0p, const Vector3Struct& p1p, const Vector3Struct& p2p, const Vector3Struct& p3p) {
    //    Vector2Struct p0, p1, p2, p3;
    //    Functions::Calculations::WorldToScreen(p0p, p0);
    //    Functions::Calculations::WorldToScreen(p1p, p1);
    //    Functions::Calculations::WorldToScreen(p2p, p2);
    //    Functions::Calculations::WorldToScreen(p3p, p3);

    //    const int segments = 10; // Quanto mais segmentos, mais liso fica
    //    Vector2Struct previousPoint = p0;

    //    for (int i = 1; i <= segments; i++) {
    //        float t = static_cast<float>(i) / static_cast<float>(segments);

    //        // Aplica a fórmula de Bézier
    //        float x = (1 - t) * (1 - t) * (1 - t) * p0.X + 3 * (1 - t) * (1 - t) * t * p1.X + 3 * (1 - t) * t * t * p2.X + t * t * t * p3.X;
    //        float y = (1 - t) * (1 - t) * (1 - t) * p0.Y + 3 * (1 - t) * (1 - t) * t * p1.Y + 3 * (1 - t) * t * t * p2.Y + t * t * t * p3.Y;

    //        Vector2Struct currentPoint = { x, y };
    //        ImVec2 curr = { x, y };
    //        ImVec2 curr2 = { previousPoint.X, previousPoint.Y };
    //        Functions::ImGuiFunctions::DrawLine(curr, curr2, Settings::SKELETON_THICKNESS, SkeletonColor);
    //        previousPoint = currentPoint;
    //    }
    //};

    //DrawCurvedBone(HeadPosition, NeckPosition, ChestPosition);
    //DrawCurvedBone2(NeckPosition, LeftShoulderPosition, LeftElbowPosition, LeftHandPosition);
    //DrawCurvedBone2(NeckPosition, RightShoulderPosition, RightElbowPosition, RightHandPosition);
    //DrawCurvedBone(ChestPosition, PelvisPosition, LeftHipPosition);
    //DrawCurvedBone(LeftHipPosition, LeftKneePosition, LeftFootPosition);
    //DrawCurvedBone(ChestPosition, PelvisPosition, RightHipPosition); 
    //DrawCurvedBone(RightHipPosition, RightKneePosition, RightFootPosition);
}

void MakeGlowForPlayer(const uintptr_t PlayerPawn) {
    const ColorRGBAStruct Color = Attributes::ESP::ESP::GlowColor;

    // 1. Converte para o formato ARGB (32 bits)
    // Alpha (w) em 255 torna o brilho 100% visível (sem transparência)
    const uint32_t ColorArgb = (
        (static_cast<uint32_t>(Color.Alpha * 255) << 24) | // Alpha (FF)
        (static_cast<uint32_t>(Color.Blue * 255) << 16) | // Blue  (FF)
        (static_cast<uint32_t>(Color.Green * 255) << 8) | // Green (00)
        static_cast<uint32_t>(Color.Red * 255)           // Red   (00)
        );

    const uintptr_t GlowOffset = PlayerPawn + Offsets::m_Glow;

    *reinterpret_cast<uint32_t*>(GlowOffset + Offsets::m_glowColorOverride) = ColorArgb;
    *reinterpret_cast<bool*>(GlowOffset + Offsets::m_bGlowing) = true;
}

void MakeFlagsForPlayer(const uintptr_t PlayerPawn) {
    const int16_t PlayerWeapon = Includes::GetPlayerWeapon(PlayerPawn);
    //if (PlayerWeapon) std::cout << WeaponMap[PlayerWeapon].Name << "\n";
}

void Functions::ESP::ESP() {
    const uintptr_t EntityList = Includes::GetEntityList();
    if (!EntityList) return;
    
    for (int i = 1; i < 64; i++) {
        const uintptr_t ListEntry = *reinterpret_cast<uintptr_t*>(EntityList + (8 * (i >> 9)) + 16);
        if (!ListEntry) continue;

        const uintptr_t PlayerController = *reinterpret_cast<uintptr_t*>(ListEntry + 112 * (i & 0x1FF));
        if (!PlayerController) continue;

        if (*reinterpret_cast<bool*>(PlayerController + Offsets::m_bIsLocalPlayerController)) continue;
        if (!*reinterpret_cast<bool*>(PlayerController + Offsets::m_bPawnIsAlive)) continue;

        const uint32_t PlayerPawnHandle = *reinterpret_cast<uint32_t*>(PlayerController + Offsets::m_hPlayerPawn);
        if (!PlayerPawnHandle) continue;
        const uintptr_t ListEntry2 = *reinterpret_cast<uintptr_t*>(EntityList + 8 * ((PlayerPawnHandle & 0x7FFF) >> 9) + 16);
        if (!ListEntry2) continue;

        const uintptr_t PlayerPawn = *reinterpret_cast<uintptr_t*>(ListEntry2 + 112 * (PlayerPawnHandle & 0x1FF));
        if (!PlayerPawn) continue;

        /////////////////////////////////////////////////////////////////////////////////////////////////////

        if (Includes::IsPlayerAnEnemy(PlayerController)) {
            if (Attributes::ESP::ESP::IsBoxEnabled) MakeBoxForPlayer(PlayerPawn);
            if (Attributes::ESP::ESP::IsHealthBarEnabled) MakeHealthBarForPlayer(PlayerPawn);
            if (Attributes::ESP::ESP::IsNameEnabled) MakeNameForPlayer(PlayerPawn);
            if (Attributes::ESP::ESP::IsSkeletonEnabled) MakeSkeletonForPlayer(PlayerPawn);
            if (Attributes::ESP::ESP::IsGlowEnabled) MakeGlowForPlayer(PlayerPawn);
            if (Attributes::ESP::ESP::IsFlagsEnabled) MakeFlagsForPlayer(PlayerPawn);
        }
    }
}










///////////////////////////////////////////////// VISUALS /////////////////////////////////////////////////////////////













void RemoveFlashbang() {
    uintptr_t LocalPlayerPawn = Includes::GetLocalPlayerPawn();
    if (!LocalPlayerPawn) return;
    *reinterpret_cast<float*>(LocalPlayerPawn + Offsets::m_flFlashDuration) = 0;
}

void Functions::ESP::Visuals() {
    if (Attributes::ESP::Visuals::RemoveFlashbang) RemoveFlashbang();
}
