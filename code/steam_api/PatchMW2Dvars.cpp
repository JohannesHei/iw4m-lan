#include "StdInc.h"

float cgFov90 = 90.0f;

void PatchMW2_Dvars()
{
	// logfile default to 2 as in IW4M1
	//*(BYTE*)0x60AE4A = 2;

	// remove write protection from fs_game
	*(DWORD*)0x6431EA ^= DVAR_FLAG_WRITEPROTECTED;

	// un-cheat cg_fov and add archive flags
	*(BYTE*)0x4F8E35 ^= DVAR_FLAG_CHEAT | DVAR_FLAG_SAVED; // 65

	// set cg_fov max to 90.0
	*(DWORD*)0x4F8E28 = (DWORD)&cgFov90;

	// set flags of cg_drawFPS to archive
	*(BYTE*)0x4F8F69 |= DVAR_FLAG_SAVED;

	/*
	// un-cheat cg_debugInfoCornerOffset and add archive flags
	*(BYTE*)0x4F8FC2 ^= DVAR_FLAG_CHEAT | DVAR_FLAG_SAVED;

	// remove archive flags for cg_hudchatposition
	*(BYTE*)0x4F9992 ^= DVAR_FLAG_SAVED;

	// set ui_browserMod's default to -1 (All)
	//*(BYTE*)0x630FD1 = 0xFF;

	dvarSetVariantHook.initialize(dvarSetVariantHookLoc, DvarSetVariantHookStub);
	dvarSetVariantHook.installHook(); 

	// allow cheat dvars to be modified in demos.
	dvarSetVariantHook2.initialize(dvarSetVariantHook2Loc, DvarSetVariantHook2Stub);
	dvarSetVariantHook2.installHook();

	// set ui_buildSize to 0.3
	//*(DWORD*)0x6310C3 = (DWORD)"1";
	//*(BYTE*)0x6310AF = 0;

	//*(int*)0x6310A2 = 1;
	*/
}