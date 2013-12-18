#include "StdInc.h"
#include <winsock.h>

void CL_OpenMenu_f()
{
	if (Cmd_Argc() != 2)
	{
		Com_Printf(0, "USAGE: openmenu <menu name>\n");
		return;
	}

	char* menu = Cmd_Argv(1);
	__asm
	{
		push menu
			push 62E2858h
			mov eax, 4CCE60h
			call eax
	}
}

int lobbyIP = 0;
int lobbyPort = 0;
int lobbyIPLoc = 0;
int lobbyPortLoc = 0;

char lobbyIPBuf[1024];
char lobbyPortBuf[1024];
char lobbyIPLocBuf[1024];
char lobbyPortLocBuf[1024];

static netadr_t currentLobbyTarget;

const char* IWClient_HandleLobbyData(const char* key)
{
	netadr_t address = currentLobbyTarget;

	if (!strcmp(key, "addr"))
	{
		return va("%d", address.ip[0] | (address.ip[1] << 8) | (address.ip[2] << 16) | (address.ip[3] << 24));
	}
	else if (!strcmp(key, "port"))
	{
		return va("%d", htons(currentLobbyTarget.port));
	}
	else if (!strcmp(key, "addrLoc"))
	{
		return va("%d", address.ip[0] | (address.ip[1] << 8) | (address.ip[2] << 16) | (address.ip[3] << 24));
	}
	else if (!strcmp(key, "portLoc"))
	{
		return va("%d", htons(currentLobbyTarget.port));
	}
        
	return "212";
}

void ConnectToAddress(netadr_t address)
{
	currentLobbyTarget = address;

	CSteamID steamIDLobby = CSteamID(1337132, 0x40000, k_EUniversePublic, k_EAccountTypeChat );
	Steam_JoinLobby(steamIDLobby, 0);
}

void CL_Connect_f()
{
	if (Cmd_Argc() < 2)
	{
		Com_Printf(0, "USAGE: connect <server>\n");
		return;
	}

	const char* str = Cmd_Argv(1);
	netadr_t adr;

	if (NET_StringToAdr(str, &adr))
	{
		ConnectToAddress(adr);
	}
}

void CL_Reconnect_f()
{
	Cmd_ExecuteSingleCommand(0, 0, "disconnect");

	netadr_t adr = currentLobbyTarget;
	ConnectToAddress(adr);
}

void PatchMW2_Offline();

extern bool weWantOffline;

void CL_Offline_f()
{
	if (GAME_FLAG(GAME_FLAG_OFFLINE) || weWantOffline)
	{
		Com_Error(2, "You are already in offline mode.");
		return;
	}
	else if (!GAME_FLAG(GAME_FLAG_OFFLINE) || !weWantOffline)
	{
		PatchMW2_Offline();
	}
}

void PatchMW2_Commands()
{
	static cmd_function_t openmenuCmd;
	static cmd_function_t connectCmd;
	static cmd_function_t reconnectCmd;
	static cmd_function_t offlineCmd;
	Cmd_AddCommand("openmenu", CL_OpenMenu_f, &openmenuCmd, 0);
	Cmd_AddCommand("connect", CL_Connect_f, &connectCmd, 0);
	Cmd_AddCommand("reconnect", CL_Reconnect_f, &reconnectCmd, 0);
	Cmd_AddCommand("toogleoffline", CL_Offline_f, &offlineCmd, 0);
}