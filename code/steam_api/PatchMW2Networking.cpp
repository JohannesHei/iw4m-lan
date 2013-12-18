#include "StdInc.h"
#include <time.h>
#include <winsock.h>
#include <dbghelp.h>

// gethostbyname hook
dvar_t** masterServerName = (dvar_t**)0x1AD8F48;

char* serverName = NULL;
char* webName = NULL;

unsigned int oneAtATimeHash(char* inpStr)
{
    unsigned int value = 0,temp = 0;
    for(size_t i=0;i<strlen(inpStr);i++)
    {
            char ctext = tolower(inpStr[i]);
            temp = ctext;
            temp += value;
            value = temp << 10;
            temp += value;
            value = temp >> 6;
            value ^= temp;
    }
    temp = value << 3;
    temp += value;
    unsigned int temp2 = temp >> 11;
    temp = temp2 ^ temp;
    temp2 = temp << 15;
    value = temp2 + temp;
    if(value < 2) value += 2;
    return value;
}

hostent* WINAPI custom_gethostbyname(const char* name) {
    // if the name is IWNet's stuff...
    unsigned int ip1 = oneAtATimeHash("ip1.pc.iw4.iwnet.infinityward.com");
    unsigned int log1 = oneAtATimeHash("log1.pc.iw4.iwnet.infinityward.com");
    unsigned int match1 = oneAtATimeHash("match1.pc.iw4.iwnet.infinityward.com");
    unsigned int web1 = oneAtATimeHash("web1.pc.iw4.iwnet.infinityward.com");
    unsigned int blob1 = oneAtATimeHash("blob1.pc.iw4.iwnet.infinityward.com");

    unsigned int current = oneAtATimeHash((char*)name);
    char* hostname = (char*)name;

    if (current == log1 || current == match1 || current == blob1 || current == ip1 || current == web1) {
            hostname = (*masterServerName)->current.string;
    }

    return gethostbyname(hostname);
}

extern bool weWantOffline;

void PatchMW2_Networking()
{
	// gethostbyname
	*(DWORD*)0x6D7458 = (DWORD)custom_gethostbyname;

	char* webName = "http://" masterName ":13000/pc/";

	if (GAME_FLAG(GAME_FLAG_OFFLINE) || weWantOffline)
	{
		// master server
		strcpy((char*)0x6D9CBC, "127.0.0.1");

		*(DWORD*)0x4D4800 = (DWORD)"127.0.0.1:13000/pc/";
		*(DWORD*)0x4D481F = (DWORD)"127.0.0.1:13000/pc/";
	}
	else
	{
		// master server
		strcpy((char*)0x6D9CBC, masterName);

		*(DWORD*)0x4D4800 = (DWORD)webName;
		*(DWORD*)0x4D481F = (DWORD)webName;
	}

	// increase maximum LSP packet size
	*(DWORD*)0x682614 = 3500;
	*(DWORD*)0x68262A = 3500;
}