#include "StdInc.h"

extern bool weWantOffline;

// patches fastfile integrity checking
void PatchMW2_FFHash()
{
	// basic checks (hash jumps, both normal and playlist)
	*(WORD*)0x5B97A3 = 0x9090;
	*(WORD*)0x5BA493 = 0x9090;

	*(WORD*)0x5B991C = 0x9090;
	*(WORD*)0x5BA60C = 0x9090;

	*(WORD*)0x5B97B4 = 0x9090;
	*(WORD*)0x5BA4A4 = 0x9090;

	// some other, unknown, check
	*(BYTE*)0x5B9912 = 0xB8;
	*(DWORD*)0x5B9913 = 1;

	*(BYTE*)0x5BA602 = 0xB8;
	*(DWORD*)0x5BA603 = 1;

	if (IsDebuggerPresent())
	{
		// dirty disk breakpoint
		*(BYTE*)0x4CF7F0 = 0xCC;
	}
}

CallHook execIsFSHook;
DWORD execIsFSHookLoc = 0x6098FD;

// note this function has to return 'int', as otherwise only the bottom byte will get cleared
int ExecIsFSHookFunc(const char* execFilename, const char* dummyMatch) { // dummyMatch isn't used by us
	// check if the file exists in our FS_* path
	if (FS_ReadFile(execFilename, NULL) >= 0)
	{
		return false;
	}

	return true;
}

void __declspec(naked) ExecIsFSHookStub() {
	__asm jmp ExecIsFSHookFunc
}

DWORD pureShouldBeZero = 0;

// patches fs_game/IWD script support
void PatchMW2_Modding()
{
	// remove limit on IWD file loading
	//memset((void*)0x643B94, 0x90, 6);
	*(BYTE*)0x642BF3 = 0xEB;

	// remove convar write protection (why?)
	//*(BYTE*)0x647DD4 = 0xEB;

	// kill most of pure (unneeded in 159, 180+ messed it up)
	//memset((void*)0x45513D, 0x90, 5);
	//memset((void*)0x45515B, 0x90, 5);
	//memset((void*)0x45516C, 0x90, 5);

	//memset((void*)0x45518E, 0x90, 5);
	//memset((void*)0x45519F, 0x90, 5);

	//*(BYTE*)0x449089 = 0xEB;

	// other IWD things (pure?)
	//*(BYTE*)0x4C5E7B = 0xEB;
	//*(BYTE*)0x465107 = 0xEB;

	// default sv_pure to 0
	// TODO: implement client-side downloading/default to 1 for no-mods
	*(BYTE*)0x4D3A74 = 0;

	// disable sv_pure for .iwi/.wav/.mp3-only IWDs (for now we only want to get rid of scripts)
	/*strcpy((char*)0x6F1DBC, ".iwi");
	strcpy((char*)0x6E1B94, ".wav");
	strcpy((char*)0x71325C, ".mp3");*/

	// disable configstring checksum matching (it's unreliable at most)
	//*(BYTE*)0x4A75A7 = 0xEB; // SV_SpawnServer
	//*(BYTE*)0x5AC2CF = 0xEB; // CL_ParseGamestate
	*(BYTE*)0x5AC2C3 = 0xEB; // CL_ParseGamestate

	// steam stuff (steam authentication)
	*(DWORD*)0x414ACC = 0x90909090;
	*(WORD*)0x414AD0 = 0x9090;

	// hopefully allow alt-tab during game, used at least in alt-enter handling
	*(BYTE*)0x45ACE0 = 0xB0;
	*(BYTE*)0x45ACE1 = 0x01;
	*(BYTE*)0x45ACE2 = 0xC3;

	// set cg_scoreboardPingText to 1
	*(BYTE*)0x45888E = 1;

	// disable migration_dvarErrors
	*(BYTE*)0x60BDA7 = 0;

	// allow vid_restart even if 'connected'
	memset((void*)0x4CA1FA, 0x90, 6);

	// remove 'impure stats' checking
	*(BYTE*)0x4BB250 = 0x33;
	*(BYTE*)0x4BB251 = 0xC0;
	*(DWORD*)0x4BB252 = 0xC3909090;

	// remove fs_game profiles
	*(WORD*)0x4A5D74 = 0x9090;

	// fs_game crash fix removing some calls
	// (NOTE: CoD4 comparison shows this is related to LoadObj weaponDefs, might fix the crash we're having)
	*(BYTE*)0x452C1D = 0xEB;

	// remove fs_game check for moddable rawfiles - allows non-fs_game to modify rawfiles
	*(WORD*)0x61AB76 = 0x9090;

	// kill filesystem init default_mp.cfg check -- IW made it useless while moving .cfg files to fastfiles
	// and it makes fs_game crash

	// not nopping everything at once, there's cdecl stack cleanup in there
	memset((void*)0x461A9E, 0x90, 5);
	memset((void*)0x461AAA, 0x90, 5);
	memset((void*)0x461AB2, 0x90, 0xB1);

	// for some reason fs_game != '' makes the game load mp_defaultweapon, which does not exist in MW2 anymore as a real asset
	// kill the call and make it act like fs_game == ''
	// UPDATE 2010-09-12: this is why CoD4 had text weapon files, those are used with fs_game.
	// CLARIFY 2010-09-27: we don't have textual weapon files, as such we should load them from fastfile as usual
	// TODO: change this into a delay-loading hook for fastfile/loadobj (2011-05-20)
	*(BYTE*)0x4081FD = 0xEB;

	// exec fixing
	execIsFSHook.initialize(execIsFSHookLoc, ExecIsFSHookStub);
	execIsFSHook.installHook();

	// exec whitelist removal (YAYFINITY WARD)
	memset((void*)0x609685, 0x90, 5);
	*(WORD*)0x60968C = 0x9090;

	if (GAME_FLAG(GAME_FLAG_OFFLINE) || weWantOffline)
	{
		// allow joining 'developer 1' servers
		*(BYTE*)0x478BA2 = 0xEB;
	}

	PatchMW2_FFHash();
}