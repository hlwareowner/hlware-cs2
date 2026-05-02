#pragma once

#include "Classes.h"
#include "TraceManager.h"

#include <cstdint>

namespace LoadedVariables {
	inline uintptr_t ClientDLL;
	inline uintptr_t ViewMatrix;
	inline uintptr_t LocalPlayerController;
	inline uintptr_t LocalPlayerPawn;
	inline uintptr_t EntityList;
	inline uintptr_t attack;

	inline InitTraceData_t fnInitTraceData;
	inline InitTraceInfo_t fnInitTraceInfo;
	inline InitTraceFilter_t fnInitFilter;
	inline CreateTrace_t fnCreateTrace;
	inline GetTraceInfo_t fnGetTraceInfo;
}