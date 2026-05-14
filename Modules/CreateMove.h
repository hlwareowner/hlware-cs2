#pragma once

#include "../Includes.h"

typedef double(__fastcall* CreateMoveFn)(CCSGOInput* pThis, unsigned int slot, CUserCommandStruct* pCmd);

extern CreateMoveFn oCreateMove;

double __fastcall hkCreateMove(CCSGOInput* pThis, unsigned int slot, CUserCommandStruct* pCmd);
