// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: Adds a T5-style string next to obituaries of
//          players committing suicide.
//
// Initial author: NTAuthority
// Started: 2012-04-10
// ==========================================================

#include "StdInc.h"
#include <time.h>

#define NUM_SUICIDE_MESSAGES 3

static const char* suicideMessages[] = 
{
	// no offense, NemexisNet
	"tried to use Nemexis updater",
	"installed StuxNet",
	"tried to search for mods in a volcano"
};

CallHook printObituaryHook;
DWORD printObituaryHookLoc = 0x401453;

void ModifyObituaryMessage(char* attackerName, char* buffer)
{
	if (!attackerName[0])
	{
		char suicideMessage[128] = { 0 };
		strcat(suicideMessage, " ");
		strcat(suicideMessage, suicideMessages[rand() % NUM_SUICIDE_MESSAGES]);
		strcat(suicideMessage, "\n");

		char* end = &buffer[strlen(buffer) - 1];
		strcpy(end, suicideMessage);
	}
}

void __declspec(naked) PrintObituaryHookStub()
{
	__asm
	{
		push ecx
		push eax				// message buffer

		mov ecx, [esp + 438h]	// function argument 2, 'attacker name'

		push ecx
		call ModifyObituaryMessage
		add esp, 4h

		pop eax
		pop ecx
		jmp printObituaryHook.pOriginal
	}
}

void PatchMW2_SuicideMessages()
{
	srand((unsigned int)time(NULL));

	printObituaryHook.initialize(printObituaryHookLoc, PrintObituaryHookStub);
	printObituaryHook.installHook();
}