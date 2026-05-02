#pragma once
#include <Windows.h>
#include <cstdint>
#include <iostream>

struct Vector3 {
    float x, y, z;
    Vector3 operator+(const Vector3& v) const { return { x + v.x, y + v.y, z + v.z }; }
    Vector3 operator-(const Vector3& v) const { return { x - v.x, y - v.y, z - v.z }; }
};

struct alignas(16) CGameTrace {
    void* m_Surface;
    void* m_pHitEntity;     // Offset 0x08[cite: 3]
    void* m_HitboxData;
    char _pad1[0x10];
    uint32_t m_Contents;    // Offset 0x50[cite: 3]
    char _pad2[0x4A];
    Vector3 m_StartPos;
    Vector3 m_EndPos;
    Vector3 m_Normal;
    Vector3 m_Pos;
    char _pad3[0x4];
    float fraction;         // Valor de 0.0 a 1.0[cite: 3]
    char _pad[0xC];
    char _pad4[0x6];
    bool m_all_solid;
    char _pad5[0x4D];
};

struct bullet_modulate_entry_t {
    float startFrac;
    float endFrac;
    float damage;           // Dano resultante após este impacto
    int maxSecondaryTraces;
    uint16_t surfStart;
    uint16_t surfEnd;
    uint8_t flags;
    uint8_t pad[3];
};

struct bullet_mod_array_t {
    int size;
    char pad4[4];
    bullet_modulate_entry_t* data;
    char pad16[8];
};

struct trace_array_element_t { char data[0x30]; };

struct alignas(16) TraceData_t {
    char pad0[16];          // Padding ajustado para alinhar o damage
    float initial_damage;   // Adicionado aqui para o cálculo de autowall[cite: 6]
    char pad_extra[4];      // Preenchimento para manter o tamanho original
    trace_array_element_t m_Arr[0x80];
    char pad6168[8];
    bullet_mod_array_t mod_array;
    bullet_modulate_entry_t mod_inline[8];
    Vector3 tail_start;
    Vector3 tail_end;
    char _pad6200[12];
};

struct alignas(16) CTraceFilter {
    char pad[164];
};

// Mantendo seus typedefs com __fastcall[cite: 6]
typedef void(__fastcall* InitTraceData_t)(TraceData_t*);
typedef void(__fastcall* InitTraceInfo_t)(CGameTrace*);
typedef void* (__fastcall* InitTraceFilter_t)(CTraceFilter* pFilter, uintptr_t pPawn, uint64_t mask, int traceType, int filterType);
typedef bool(__fastcall* CreateTrace_t)(TraceData_t* trace, Vector3 start, Vector3 delta, CTraceFilter* filter, int penetration_count, bool unk);
typedef void(__fastcall* GetTraceInfo_t)(TraceData_t* trace, CGameTrace* hit, float unknown_float, void* unknown);