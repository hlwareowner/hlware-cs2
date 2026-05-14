#pragma once

#include "../Includes.h"
#include "CreateMove.h"
#include <iostream>
#include <bitset>

CreateMoveFn oCreateMove = (CreateMoveFn)(Memory::FindPattern("client.dll", "48 8B C4 4C 89 40 ? 48 89 48 ? 55 53 41 54"));

double __fastcall hkCreateMove(CCSGOInput* pThis, unsigned int slot, CUserCommandStruct* Command) {
	//if (pCmd->nButtons.nValue & IN_JUMP)
	//pCmd->nButtons.nValue |= IN_JUMP;
	LoadedVariables::UserCommand = Command;

	double ReturnValue = oCreateMove(pThis, slot, Command);

	Functions::Ragebot::Ragebot();

	return ReturnValue;
};
