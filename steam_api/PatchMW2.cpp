#include "StdInc.h"

void PatchMW2_Game();

void Sys_RunInit()
{
	PatchMW2();
}

void PatchMW2()
{
	// check version
	if (!strcmp((char*)0x6E9638, "177")) { // 1.0.159 (iw4m) version-specific address
		DetermineGameFlags();
		PatchMW2_Game();
		return;
	}

	TerminateProcess(GetCurrentProcess(), 0);
}


void CrashHandlerInit();

void PatchMW2_Modding();
void PatchMW2_Redirect();
void PatchMW2_Load();
void PatchMW2_Prefix();
void PatchMW2_AssetRestrict();
void PatchMW2_ClientConsole();
void PatchMW2_LogInitGame();
void PatchMW2_Branding();
void PatchMW2_Stats();
void PatchMW2_ScoreboardInfo();
void PatchMW2_Dvars();
void PatchMW2_XPBar();
void PatchMW2_VA();
void PatchMW2_LocalizedStrings();
void PatchMW2_Commands();
void PatchMW2_Networking();
void PatchMW2_SuicideMessages();

void PatchMW2_Offline();
bool weWantOffline = false;

void PatchMW2_Game()
{
	// remove system pre-init stuff (improper quit, disk full)
	*(BYTE*)0x411350 = 0xC3;
	
	// remove STEAMSTART checking for DRM IPC
	memset((void*)0x451145, 0x90, 5);
	*(BYTE*)0x45114C = 0xEB;

	// internal version
	*(int*)0x463C61 = 99;

	// protocol version (workaround for hacks)
	*(int*)0x4FB501 = 1337; // was 8E

	// protocol command
	*(int*)0x4D36A9 = 1337; // was 8E
	*(int*)0x4D36AE = 1337; // was 8E
	*(int*)0x4D36B3 = 1337; // was 8E

	// cause 'does current Steam lobby match' calls in Steam_JoinLobby to be ignored (issue #8)
	*(BYTE*)0x49D007 = 0xEB;

	// cause Steam auth to always be requested
	memset((void*)0x46FB8C, 0x90, 6);

	// disable Steam auth
	*(BYTE*)0x4663A8 = 0xEB;

	// un-neuter Com_ParseCommandLine to allow non-connect_lobby
	*(BYTE*)0x464AE4 = 0xEB;

	CrashHandlerInit();

	PatchMW2_Modding();
	PatchMW2_Redirect();
	PatchMW2_Load();
	PatchMW2_Prefix();
	PatchMW2_AssetRestrict();
	PatchMW2_ClientConsole();
	PatchMW2_LogInitGame();
	PatchMW2_Branding();
	PatchMW2_Stats();
	PatchMW2_ScoreboardInfo();
	PatchMW2_Dvars();
	PatchMW2_XPBar();
	PatchMW2_VA();
	PatchMW2_LocalizedStrings();
	PatchMW2_Commands();
	PatchMW2_Networking();
	PatchMW2_SuicideMessages();

	//if (GAME_FLAG(GAME_FLAG_OFFLINE))
	//{
		PatchMW2_Offline();
		weWantOffline = true;
	//}

	if (GAME_FLAG(GAME_FLAG_CONSOLE))
	{
		// always enable system console, not just if generating reflection probes
		memset((void*)0x60BB58, 0x90, 11);
	}

	FreeConsole();
}

bool* gotPlaylists = (bool*)0x1AD3680;

void PatchMW2_Offline()
{
	// disable playlist download function
	*(BYTE*)0x4D4790 = 0xC3;

	// disable playlist.ff loading function
	*(BYTE*)0x4D6E60 = 0xC3;

	// playlist minimum -1
	*(BYTE*)0x404CD5 = 0xFF;

	// make the game think that we loaded playlists :)
	*gotPlaylists = true; 

	// disable playlist checking
	*(BYTE*)0x5B69E9 = 0xEB; // too new
	*(BYTE*)0x5B696E = 0xEB; // too old
	
	// disable the IWNet IP detection (default 'got ipdetect' flag to 1)
	*(BYTE*)0x649D6F0 = 1;

	// set flags of upnp_maxAttempts to archive
	*(BYTE*)0x4FD420 |= DVAR_FLAG_SAVED;

	memset((void*)0x60BE24, 0x90, 5); // NO UPNP

	// LSP disabled
	*(BYTE*)0x435950 = 0xC3; // LSP HELLO
	*(BYTE*)0x49C220 = 0xC3; // We wanted to send a logging packet, but we haven't connected to LSP!s
	*(BYTE*)0x4BD900 = 0xC3; // main LSP response func
	*(BYTE*)0x682170 = 0xC3; // Telling LSP that we're playing a private match

	// use M2 playlists
	strcpy((char*)0x6EE7AC, "mp_playlists_m2");
	*(DWORD*)0x4D47FB = (DWORD)"mp_playlists_m2.ff";
}