#pragma once

#include <unordered_map>
#include <string>

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
};

struct Vector2Struct {
    float X = 0.0f;
    float Y = 0.0f;
};

struct ViewMatrixStruct {
    float Matrix[4][4];
};

struct WeaponInformationStruct {
    std::string Name;
};

enum EnabledHitboxesEnum {
    NONE = 0,
    HEAD = 1 << 0, // 1  (0001 em binário)
    CHEST = 1 << 1, // 2  (0010 em binário)
    PELVIS = 1 << 2, // 4  (0100 em binário)
    LEGS = 1 << 3  // 8  (1000 em binário)
};

struct WeaponRagebotSettingsStruct {
    bool IsEnabled;
    EnabledHitboxesEnum Hitboxes;
    int MinimumDamage;
    int Hitchance;
};

// Usando unordered_map para performance máxima no loop do trigger
inline std::unordered_map<int16_t, WeaponInformationStruct> WeaponMap = {
    // --- Pistolas ---
    { 1,  { "Deagle" } },
    { 2,  { "Dual Berettas" } },
    { 3,  { "Five-Seven" } },
    { 4,  { "Glock" } },
    { 30, { "Tec-9" } },
    { 32, { "P2000" } },
    { 36, { "P250" } },
    { 61, { "USP-S" } },
    { 63, { "CZ75" } },
    { 64, { "R8" } },

    // --- Rifles ---
    { 7,  { "AK-47" } }, // No CS2 alguns dumps usam 7 ou 4, verifique seu dumper
    { 8,  { "AUG" } },
    { 10, { "FAMAS" } },
    { 13, { "Galil AR" } },
    { 16, { "M4A4" } },
    { 39, { "SG 553" } },
    { 60, { "M4A1-S" } },

    // --- Snipers ---
    { 9,  { "AWP" } },
    { 11, { "G3SG1" } },
    { 38, { "SCAR-20" } },
    { 40, { "SSG 08" } },

    // --- Submetralhadoras (SMGs) ---
    { 17, { "MAC-10" } },
    { 19, { "P90" } },
    { 23, { "MP5-SD" } },
    { 24, { "UMP-45" } },
    { 26, { "PP-Bizon" } },
    { 34, { "MP9" } },


    // --- Pesadas (Heavy) ---
    { 14, { "M249" } },
    { 25, { "XM1014" } },
    { 35, { "Nova" } },
    { 27, { "MAG-7" } },
    { 28, { "Negev" } },
    { 29, { "Sawed-Off" } },
    { 31, { "Taser" } },

    // --- Granadas / Utilitários ---
    { 43, { "Flashbang" } },
    { 44, { "HE Grenade" } },
    { 45, { "Smoke Grenade" } },
    { 46, { "Molotov" } },
    { 47, { "Decoy Grenade" } },
    { 48, { "Incendiary Grenade" } },
    { 49, { "C4" } },

    // --- Facas (IDs Base) ---
    { 42, { "Knife (CT)" } },
    { 59, { "Knife (T)" } }
};

inline std::unordered_map<int16_t, WeaponRagebotSettingsStruct> RagebotEquippedWeaponSettingsMap = {
    // --- Pistolas ---
    { 1,  { true, NONE, 0, 60 } },
    { 2,  { true, NONE, 0, 60 } },
    { 3,  { true, NONE, 0, 60 } },
    { 4,  { true, NONE, 0, 60 } },
    { 30, { true, NONE, 0, 60 } },
    { 32, { true, NONE, 0, 60 } },
    { 36, { true, NONE, 0, 60 } },
    { 61, { true, NONE, 0, 60 } },
    { 63, { true, NONE, 0, 60 } },
    { 64, { true, NONE, 0, 60 } },

    // --- Rifles ---
    { 7,  { true, NONE, 0, 60 } }, // No CS2 alguns dumps usam 7 ou 4, verifique seu dumper
    { 8,  { true, NONE, 0, 60 } },
    { 10, { true, NONE, 0, 60 } },
    { 13, { true, NONE, 0, 60 } },
    { 16, { true, NONE, 0, 60 } },
    { 39, { true, NONE, 0, 60 } },
    { 60, { true, NONE, 0, 60 } },

    // --- Snipers ---
    { 9,  { true, NONE, 0, 60 } },
    { 11, { true, NONE, 0, 60 } },
    { 38, { true, NONE, 0, 60 } },
    { 40, { true, NONE, 0, 60 } },

    // --- Submetralhadoras (SMGs) ---
    { 17, { true, NONE, 0, 60 } },
    { 19, { true, NONE, 0, 60 } },
    { 23, { true, NONE, 0, 60 } },
    { 24, { true, NONE, 0, 60 } },
    { 26, { true, NONE, 0, 60 } },
    { 34, { true, NONE, 0, 60 } },


    // --- Pesadas (Heavy) ---
    { 14, { true, NONE, 0, 60 } },
    { 25, { true, NONE, 0, 60 } },
    { 35, { true, NONE, 0, 60 } },
    { 27, { true, NONE, 0, 60 } },
    { 28, { true, NONE, 0, 60 } },
    { 29, { true, NONE, 0, 60 } },
    { 31, { true, NONE, 0, 60 } },

    // --- Granadas / Utilitários ---
    { 43, { true, NONE, 0, 60 } },
    { 44, { true, NONE, 0, 60 } },
    { 45, { true, NONE, 0, 60 } },
    { 46, { true, NONE, 0, 60 } },
    { 47, { true, NONE, 0, 60 } },
    { 48, { true, NONE, 0, 60 } },
    { 49, { true, NONE, 0, 60 } },

    // --- Facas (IDs Base) ---
    { 42, { true, NONE, 0, 60 } },
    { 59, { true, NONE, 0, 60 } }
};