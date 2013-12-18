#include "StdInc.h"

using namespace std;

#define SV_OUTPUTBUF_LENGTH ( 16384 - 16 )
char sv_outputbuf[SV_OUTPUTBUF_LENGTH];

StompHook writeStatsHook;
DWORD writeStatsHookLoc = 0x682E70;

CallHook loadOldStatsHook;
DWORD loadOldStatsHookLoc = 0x60A846;

const char* fileID;
bool useSteamCloud;
unsigned int bufferSize;

bool WriteStatsFunc(int remote, void* buffer)
{
	// filename buffer
	char filename[256];

	// check buffer size to be valid, though it doesn't matter if we don't do TEA
	if (bufferSize > 8192 || bufferSize & 3) return false; // & 3 is for 4-byte alignment

	// ask about dumping stats
	//DumpStatsOnWrite(buffer, bufferSize);

	// sprintf a filename
	_snprintf(filename, sizeof(filename), "%s.stat", fileID);

	// return the file
	return FS_WriteFile(filename, "players", buffer, bufferSize);
}

void __declspec(naked) WriteStatsHookStub()
{
	__asm
	{
		// prepare weird optimized parameters
		mov fileID, ecx
		mov useSteamCloud, dl
		mov bufferSize, eax

		// and jump to our normal cdecl function
		jmp WriteStatsFunc
	}
}

static char oldStats[8192];

int LoadOldStatsHookFunc(int a1, int a2, void* buffer, int size)
{
	if (oldStats[0] != 0)
	{
		memcpy(buffer, oldStats, size);

		return 0;
	}

	return 1;
}

StompHook readStatsHook;
DWORD readStatsHookLoc = 0x682820;
DWORD oldReadStats = 0x68282A;

void __declspec(naked) OldReadStats()
{
	__asm
	{
		mov eax, [esp + 10h]
		sub esp, 118h

		jmp oldReadStats
	}
}

int ReadStatsLegacy(int useSteamCloud, char* fileID, void* buffer, int isize)
{
	__asm
	{
		push isize
		push buffer
		push fileID
		push useSteamCloud

		call OldReadStats

		add esp, 10h
	}
}

// We do not need this shit at the moment
// void parseFile(char* statz);

string stats;
bool writingStats = false;

void savePlayerStats(char *outputbuf) 
{
	stats += string(outputbuf);

	if (strstr(outputbuf, "winLossRatio"))
	{
		writingStats = true;
		char* statBuff = (char*)malloc( sizeof( char ) *(stats.length() +1) );
		strcpy(statBuff, stats.c_str());
		//parseFile(statBuff);
		writingStats = false;
	}
}

void getPlayerStats()
{
	Sleep(10000);

	if (!writingStats)
	{
		writingStats = true;
		char* remaining;

		stats = "";

		Com_BeginRedirect( sv_outputbuf, SV_OUTPUTBUF_LENGTH, savePlayerStats );

		remaining = "dumpplayerdata";
		
		Cmd_ExecuteSingleCommand(0, 0, remaining);

		Com_EndRedirect();
	}
}

int ReadStatsFunc(int useSteamCloud, char* fileID, void* buffer, int size)
{
	// filename buffer
	char filename[256];

	// check buffer size to be valid, though it doesn't matter if we don't do TEA
	if (bufferSize > 8192 || bufferSize & 3) return 2; // & 3 is for 4-byte alignment

	// sprintf a filename
	_snprintf(filename, sizeof(filename), "%s.stat", fileID);

	// read file length/existence
	int length = FS_ReadFile(filename, 0);
	
	if (length == size)
	{
		char* fileData = NULL;
		FS_ReadFile(filename, &fileData);

		if (fileData)
		{
			memcpy(buffer, fileData, size);

			return 0;
		}
	}
	else if (length < 0)
	{		
		// we don't have file
		// try old file
		// if won't work
		// user have no stats
		// and want play

		// mark SteamID as requiring 'legacy' code (with all its added bugginess :/ )
		//SetSteamIDLegacy(true);

		// call old function (yes, I said *call*, not *jump*)
		int result = ReadStatsLegacy(useSteamCloud, fileID, buffer, size);

		// reset legacy flag and return result
		//SetSteamIDLegacy(false);

		return result;
	}

	return 1;
}

void __declspec(naked) ReadStatsHookStub()
{
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)getPlayerStats, NULL, NULL, NULL);
	MessageBoxA(0, "Creating getPlayerStats thread.", "Thread create notice", MB_OK);

	__asm
	{
		// jump to cdecl func
		jmp ReadStatsFunc
	}
}

void PatchMW2_Stats()
{
	writeStatsHook.initialize(writeStatsHookLoc, WriteStatsHookStub);
	writeStatsHook.installHook();

	loadOldStatsHook.initialize(loadOldStatsHookLoc, LoadOldStatsHookFunc);
	loadOldStatsHook.installHook();

	readStatsHook.initialize(readStatsHookLoc, ReadStatsHookStub);
	readStatsHook.installHook();

	// make statWriteNeeded ignored (kill stat saving?)
	//*(BYTE*)0x424BD0 = 0xEB;
}