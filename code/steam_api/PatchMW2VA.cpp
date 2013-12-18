#include "StdInc.h"

StompHook vaHook;
DWORD vaHookLoc = 0x4785B0;

void PatchMW2_VA()
{
	vaHook.initialize(vaHookLoc, va);
	vaHook.installHook();
}