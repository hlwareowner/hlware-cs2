#include "../Includes.h"

void Functions::Ragebot::Ragebot() {
    if (!Attributes::Ragebot::Ragebot::IsEnabled) return;
    if (!Attributes::Ragebot::Ragebot::IsAutomaticFireEnabled && Attributes::Ragebot::Ragebot::IsSilentAimEnabled) return; // não mira e nem atira

    CUserCommandStruct* Command = LoadedVariables::UserCommand;
    if (!Command) return;

    const uintptr_t LocalPlayerPawn = Includes::GetLocalPlayerPawn();
    if (!LocalPlayerPawn) return;

    const int16_t LocalPlayerWeapon = Includes::GetPlayerWeapon(); // problema do crash era q o END e a bind de disconnect e de dar unload no imgui
    if (!LocalPlayerWeapon) return;

    const WeaponRagebotAttributesStruct WeaponAttributes = RagebotWeaponSettingsMap[LocalPlayerWeapon];
    if (!WeaponAttributes.IsEnabled) return;

    const WeaponInformationStruct WeaponInformation = WeaponMap[LocalPlayerWeapon];
    const EnabledHitboxesEnum EnabledHitboxes = WeaponAttributes.Hitboxes;

    const float WeaponDamage = WeaponInformation.Damage;
    const float WeaponPenetration = WeaponInformation.Penetration;
    const float WeaponRangeModifier = WeaponInformation.RangeModifier;
    const float WeaponRange = WeaponInformation.Range;

    const uintptr_t EntityList = Includes::GetEntityList();
    if (!EntityList) return;

    for (int i = 1; i < 64; i++) {
        const uintptr_t ListEntry = *reinterpret_cast<uintptr_t*>(EntityList + (8 * (i >> 9)) + 16);
        if (!ListEntry) continue;

        const uintptr_t PlayerController = *reinterpret_cast<uintptr_t*>(ListEntry + 112 * (i & 0x1FF));
        if (!PlayerController) continue;

        if (!Includes::IsPlayerAnEnemy(PlayerController)) continue;
        if (*reinterpret_cast<bool*>(PlayerController + Offsets::m_bIsLocalPlayerController)) continue;
        if (*reinterpret_cast<bool*>(PlayerController + Offsets::m_bGunGameImmunity)) continue;
        if (!*reinterpret_cast<bool*>(PlayerController + Offsets::m_bPawnIsAlive)) continue;

        const uint32_t PlayerPawnHandle = *reinterpret_cast<uint32_t*>(PlayerController + Offsets::m_hPlayerPawn);
        if (!PlayerPawnHandle) continue;
        const uintptr_t ListEntry2 = *reinterpret_cast<uintptr_t*>(EntityList + 8 * ((PlayerPawnHandle & 0x7FFF) >> 9) + 16);
        if (!ListEntry2) continue;

        const uintptr_t PlayerPawn = *reinterpret_cast<uintptr_t*>(ListEntry2 + 112 * (PlayerPawnHandle & 0x1FF));
        if (!PlayerPawn) continue;

        /////////////////////////////////////////////////////////////////////////////////////////////////////

        const Vector3Struct LocalHead3D = *reinterpret_cast<Vector3Struct*>(LocalPlayerPawn + Offsets::m_vOldOrigin);
        const Vector3Struct LocalHead3DSum = LocalHead3D + *reinterpret_cast<Vector3Struct*>(LocalPlayerPawn + Offsets::m_vecViewOffset);

        const Vector3Struct HeadPosition = Includes::GetPlayerBonePosition(PlayerPawn, Settings::HEAD_BONE_NUMBER);
        const Vector3Struct ChestPosition = Includes::GetPlayerBonePosition(PlayerPawn, Settings::CHEST_BONE_NUMBER);
        const Vector3Struct PelvisPosition = Includes::GetPlayerBonePosition(PlayerPawn, Settings::PELVIS_BONE_NUMBER);

        const int PlayerHealth = *reinterpret_cast<int*>(PlayerPawn + Offsets::m_iHealth);
        const int MinimumDamage = (std::min)(PlayerHealth, WeaponAttributes.MinimumDamage);

        const int IsHeadSelected = EnabledHitboxes & HEAD;
        const int IsChestSelected = EnabledHitboxes & CHEST;
        const int IsPelvisSelected = EnabledHitboxes & PELVIS;

        const int HeadDamage = Functions::Raycast::ScaleDamage(Functions::Raycast::GetDamage(LocalHead3DSum, HeadPosition, LocalPlayerPawn, PlayerPawn, WeaponDamage, WeaponPenetration, WeaponRangeModifier, WeaponRange), HEAD);
        const int ChestDamage = Functions::Raycast::ScaleDamage(Functions::Raycast::GetDamage(LocalHead3DSum, ChestPosition, LocalPlayerPawn, PlayerPawn, WeaponDamage, WeaponPenetration, WeaponRangeModifier, WeaponRange), CHEST);
        const int PelvisDamage = Functions::Raycast::ScaleDamage(Functions::Raycast::GetDamage(LocalHead3DSum, PelvisPosition, LocalPlayerPawn, PlayerPawn, WeaponDamage, WeaponPenetration, WeaponRangeModifier, WeaponRange), PELVIS);

        auto Shoot = [&](const Vector3Struct& TargetPosition) {
            if (Includes::IsShooting(Command)) return;
            if (!Attributes::Ragebot::Ragebot::IsAutomaticFireEnabled) return;

            auto pBaseCmd = Command->csgoUserCmd.pBaseCmd;
            QAngleStruct ViewAngles = pBaseCmd->pViewAngles->angValue;
            const Vector3Struct Delta = {
                TargetPosition.X - LocalHead3DSum.X,
                TargetPosition.Y - LocalHead3DSum.Y,
                TargetPosition.Z - LocalHead3DSum.Z
            };

            QAngleStruct NewAngles;

            // 2. Calcula a distância horizontal (hipotenusa do plano XY)
            const float horizontalDistance = std::sqrt(Delta.X * Delta.X + Delta.Y * Delta.Y);

            // 3. Calcula o Pitch (Vertical) - Convertendo radianos para graus
            // No CS, o Pitch positivo geralmente é para baixo, então usamos o sinal negativo
            NewAngles.Pitch = -std::atan2(Delta.Z, horizontalDistance) * (180.0f / std::numbers::pi_v<float>);

            // 4. Calcula o Yaw (Horizontal) - Convertendo radianos para graus
            NewAngles.Yaw = std::atan2(Delta.Y, Delta.X) * (180.0f / std::numbers::pi_v<float>);

            // 5. Roll geralmente é zero em FPS
            NewAngles.Roll = 0.0f;

            QAngleStruct OriginalAngle = pBaseCmd->pViewAngles->angValue;
            pBaseCmd->pViewAngles->angValue = NewAngles;
            pBaseCmd->nHasBits |= BASE_BITS_VIEWANGLES;
            Command->SetSubTickAngle(NewAngles);
            Command->nButtons.nValueChanged |= IN_ATTACK;
            pBaseCmd->pViewAngles->angValue = OriginalAngle;
        };
        auto Scope = [&]() {
            Command->nButtons.nValueChanged |= IN_SECOND_ATTACK;
        };

        if (IsPelvisSelected && PelvisDamage >= MinimumDamage && PelvisDamage >= PlayerHealth) { // Pelvis lethal
            //std::println("pelvis lethal {}", PelvisDamage);
            Shoot(PelvisPosition);
            continue;
        }
        if (IsChestSelected && ChestDamage >= MinimumDamage && ChestDamage >= PlayerHealth) { // Chest lethal
            //std::println("chest lethal {}", ChestDamage);
            Shoot(ChestPosition);
            continue;
        }

        std::vector<RagebotHitboxDataStruct> Hitboxes = {
            { HeadDamage, static_cast<bool>(IsHeadSelected), HeadPosition},
            { ChestDamage, static_cast<bool>(IsChestSelected), ChestPosition},
            { PelvisDamage, static_cast<bool>(IsPelvisSelected), PelvisPosition},
        };

        RagebotHitboxDataStruct* BestHitbox = nullptr;
        int BestDamage = -1;

        for (RagebotHitboxDataStruct& Hitbox : Hitboxes) {
            if (Hitbox.Damage <= BestDamage) continue;
            if (Hitbox.Damage < MinimumDamage) continue;
            if (!Hitbox.IsSelected) continue;
            BestHitbox = &Hitbox;
            BestDamage = Hitbox.Damage;
        }

        if (BestHitbox) {
            //std::println("{}: {}", BestHitbox->Name, BestHitbox->Damage);
            if (WeaponAttributes.AutomaticScope) Scope();
            Shoot(BestHitbox->Position);
        }
    }
}

///////////////////////////////////////////////// Antiaim /////////////////////////////////////////

void Functions::Ragebot::Antiaim() {

}
