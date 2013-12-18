#include "StdInc.h"

CallHook logInitGameHook;
DWORD logInitGameHookLoc = 0x48ED70;

void LogInitGameHookFunc()
{
	char infostring[2048];

	strncpy(infostring, Dvar_InfoString_Big(1024), 2048);

	G_LogPrintf("InitGame: %s\n", infostring);
}

void __declspec(naked) LogInitGameHookStub()
{
	__asm
	{
		jmp LogInitGameHookFunc
	}
}

void PatchMW2_LogInitGame()
{
	logInitGameHook.initialize(logInitGameHookLoc, LogInitGameHookStub);
	logInitGameHook.installHook();
}