// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: Patches for compatibility/important changes in
//          early game versions (IW4M 159, for instance)
//
// Initial author: NTAuthority
// Started: 2011-05-20
// ==========================================================

#include "StdInc.h"

// prefix - IWNet ping-based searching
static int currentMaxPing = 0;
static int pingSearches = 0;
static int failedSearches = 0;

int GetCurrentMaxPing()
{
	return currentMaxPing;
}

// UI_SafeTranslateString
typedef char* (__cdecl * UI_SafeTranslateString_t)(const char* name);
UI_SafeTranslateString_t UI_SafeTranslateString = (UI_SafeTranslateString_t)0x4F1700;

// UI_ReplaceConversions_Int
typedef char* (__cdecl * UI_ReplaceConversions_Int_t)(const char* string, int number);
UI_ReplaceConversions_Int_t UI_ReplaceConversions_Int = (UI_ReplaceConversions_Int_t)0x501090;

// local variables for PingDisplayHookFunc
typedef struct pdh_value_s
{
	int type;
	char* string;
} pdh_value_t;

static pdh_value_t* pdh_value;
static int curPing;
static char* searchString;

StompHook pingDisplayHook;
DWORD pingDisplayHookLoc = 0x62EE01;

void __declspec(naked) PingDisplayHookFunc()
{
	__asm mov pdh_value, esi

	if ( GetCurrentMaxPing() && GetCurrentMaxPing() <= 150 )
	{
		curPing = GetCurrentMaxPing();
		searchString = UI_SafeTranslateString("PATCH_SEARCHINGFORGAMES_NMS");
		pdh_value->type = 2;
		pdh_value->string = UI_ReplaceConversions_Int(searchString, curPing);
	}
	else
	{
		pdh_value->type = 2;
		pdh_value->string = UI_SafeTranslateString("MENU_SEARCHING_FOR_LOBBY");
	}

	__asm pop edi
	__asm retn
}

// values for hpong result
DWORD* gameState = (DWORD*)0xB2C540;

// random fact that used to be here: I called va() 'Com_Sprintf' back then. oh, how wrong I was. :D

typedef int (__cdecl * GetNumPartyPlayers_t)(void* party);
GetNumPartyPlayers_t GetNumPartyPlayers = (GetNumPartyPlayers_t)0x497330;

typedef int (__cdecl * GetMaxPartyPlayers_t)(void* party);
GetMaxPartyPlayers_t GetMaxPartyPlayers = (GetMaxPartyPlayers_t)0x4F5D60;

// local variables for hpong result
static int curPlayers;
static int maxPlayers;
static int** currentParty;

const char* __cdecl FormatHpongResult(const char* string, int num, char* v1, char* v2)
{
	//__asm mov eax, [esp + 40h] // 34 + 4 (call return) + 4 (arg 1) + some other 4
	__asm mov currentParty, esi

	curPlayers = GetNumPartyPlayers(*currentParty);
	maxPlayers = GetMaxPartyPlayers(*currentParty);

	return va("%ihpong %s %s %d %d %d", num, v1, v2, (*gameState > 0), curPlayers, maxPlayers);
}

CallHook hpongResultHook;
DWORD hpongResultHookLoc = 0x5B44D7;

void __declspec(naked) HpongResultHookFunc()
{
	__asm jmp FormatHpongResult
}

dvar_t* ping_default_min;
dvar_t* ping_increment;
dvar_t* ping_searches_per;

void RetryPingSearch()
{
	if (!currentMaxPing)
	{
		currentMaxPing = ping_default_min->current.integer;
		pingSearches = -1;
	}

	pingSearches++;
	failedSearches++;

	Com_Printf(25, "Min Ping %d Search %d Failed Search Count %d\n", currentMaxPing, pingSearches, failedSearches);

	if (pingSearches > ping_searches_per->current.integer)
	{
		currentMaxPing += ping_increment->current.integer;
		pingSearches = 0;
	}
}

CallHook retryGameSearchHook;
DWORD retryGameSearchHookLoc = 0x4352FF;

void __declspec(naked) RetryGameSearchHookFunc()
{
	RetryPingSearch();
	__asm jmp retryGameSearchHook.pOriginal
}

CallHook qosHook;
DWORD qosHookLoc = 0x4C024E;

int qhPingMS;
int qhTime;
int qhBias = 0;

void __declspec(naked) QosHookFunc() {
	__asm mov qhPingMS, ebx
	__asm mov qhTime, ebp

	qhPingMS += qhBias;

	if (qhPingMS < 1)
	{
		qhPingMS = 1;
	}

	if (qhPingMS > GetCurrentMaxPing())
	{
		// this method is a bit lazy, but don't want to translate it to proper C
		// it sets the party slot to inactive
		__asm
		{
			mov eax, [edi + 0F0h]
			mov byte ptr [esi + eax], 0
		}
	}

	__asm mov ebx, qhPingMS
	__asm mov ebp, qhTime

	// and for security, we put ebx to the printf parameter
	__asm mov [esp + 0Ch], ebx // 4 + 8 (arg3)

	__asm jmp qosHook.pOriginal
}

void PatchMW2_PatchPing()
{
	pingDisplayHook.initialize(pingDisplayHookLoc, PingDisplayHookFunc);
	pingDisplayHook.installHook();

	hpongResultHook.initialize(hpongResultHookLoc, HpongResultHookFunc);
	hpongResultHook.installHook();

	retryGameSearchHook.initialize(retryGameSearchHookLoc, RetryGameSearchHookFunc);
	retryGameSearchHook.installHook();

	qosHook.initialize(qosHookLoc, QosHookFunc);
	qosHook.installHook();

	ping_searches_per = Dvar_RegisterInt("ping_searches_per", 2, 1, 30, DVAR_FLAG_USERINFO | DVAR_FLAG_SAVED, "Number of searches at each min ping value");
	ping_increment = Dvar_RegisterInt("ping_increment", 10, 1, 500, DVAR_FLAG_NONE | DVAR_FLAG_SAVED, "Number of milliseconds to increase min ping after each set of searches");
	ping_default_min = Dvar_RegisterInt("ping_default_min", 50, 10, 500, DVAR_FLAG_NONE | DVAR_FLAG_SAVED, "Minimum ping for the initial set of searches");
}

// javelin glitch stuff, seems to reset a flag on weapnext
StompHook javelinResetHook; // don't exactly know what this is, but it's changed between 169 and 172
DWORD javelinResetHookLoc = 0x578F52;

void __declspec(naked) JavelinResetHookStub()
{
	__asm
	{
		mov eax, 577A10h
		call eax
		pop edi
		mov dword ptr [esi+34h], 0
		pop esi
		pop ebx
		retn
	}
}

void PatchMW2_Prefix()
{
	PatchMW2_PatchPing();

	javelinResetHook.initialize(javelinResetHookLoc, JavelinResetHookStub);
	javelinResetHook.installHook();
}