#include "StdInc.h"
/*
#define CURL_STATICLIB
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

int thisversion = 99;

void PatchMW2_Offline();

size_t UDataReceived(void *ptr, size_t size, size_t nmemb, void *data)
{
        size_t rsize = (size * nmemb);
        char* text = (char*)ptr;
        int version = atoi(text);

        if (version != thisversion)
        {
                Com_Error(1, "You are using an outdated version of iLabsIW4. Please delete caches.xml to update the game.", BUILDNUMBER, version);
        }

        return rsize;
}

void DisableOldVersions()
{
        //if (IsDebuggerPresent()) return;

        curl_global_init(CURL_GLOBAL_ALL);

        CURL* curl = curl_easy_init();

        if (curl)
        {
                char url[255];
                _snprintf(url, sizeof(url), "http://content.wlck.net/iLabsIW4.txt");

                curl_easy_setopt(curl, CURLOPT_URL, url);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, UDataReceived);
                //curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&steamID);
                curl_easy_setopt(curl, CURLOPT_USERAGENT, VERSIONSTRING);
                curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);

                CURLcode code = curl_easy_perform(curl);
                curl_easy_cleanup(curl);

                curl_global_cleanup();

                if (code == CURLE_OK)
                {
					strcpy((char*)0x7859C0, "iLabsIW4 ("VERSIONSTRING")");
					strcpy((char*)0x6EC9C4, "iLabsIW4 Console ");
					return;
                }
                else
                {
					strcpy((char*)0x7859C0, "iLabsIW4 ("VERSIONSTRING") (outdated)");
					strcpy((char*)0x6EC9C4, "iLabsIW4 Console (outdated)");
					// TODO: set some offline mode flag
					Warning("You are using an outdated version of iLabsIW4 client, but you can still play offline.\n\nOffline mode includes:\n- fully working anticheat\n- fully working private matches");
					PatchMW2_Offline();
                }
        }

        curl_global_cleanup();
}


#define CURL_STATICLIB
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

void PatchMW2_Offline();
bool serverOnline = false;

size_t UDataReceived(void *ptr, size_t size, size_t nmemb, void *data)
{
        size_t rsize = (size * nmemb);
        char* text = (char*)ptr;
        int datarecieved = atoi(text);

        if (datarecieved = 0)
        {
			Warning("Server offline.");
        }

        return rsize;
}

void CheckServerStatus()
{
        //if (IsDebuggerPresent()) return;

        curl_global_init(CURL_GLOBAL_ALL);

        CURL* curl = curl_easy_init();

        if (curl)
        {
                char url[255];
                _snprintf(url, sizeof(url), "http://" masterName ":13000/pc/mp_playlists.ff");

                curl_easy_setopt(curl, CURLOPT_URL, url);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, UDataReceived);
                //curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&steamID);
                curl_easy_setopt(curl, CURLOPT_USERAGENT, VERSIONSTRING);
                curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);

                CURLcode code = curl_easy_perform(curl);
                curl_easy_cleanup(curl);

                curl_global_cleanup();

                if (code == CURLE_OK)
                {
					serverOnline = true;
					return;
                }
                else
                {
					// warn out a message that we will go offline
					Info("Server is offline but you can still play in offline mode.");

					// go offline (patch offsets needed to play offline)
					PatchMW2_Offline();

					serverOnline = false;
                }
        }

        curl_global_cleanup();
}
*/

void HideCode_FindCreateFile();
HANDLE WINAPI HideCode_DoCreateFile(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

void SteamProxy_Init();
void ServerStatusCheck();

// Steam API code
extern "C"
{
    __declspec(dllexport) HSteamPipe /*__cdecl*/ __declspec(naked) GetHSteamPipe()
    {
        //return NULL;
		__asm jmp HideCode_DoCreateFile
    }
    
	__declspec(dllexport) HSteamUser __cdecl GetHSteamUser()
    {
        return NULL;
    }

    __declspec(dllexport) HSteamPipe __cdecl SteamAPI_GetHSteamPipe()
    {
        return NULL;
    }

    __declspec(dllexport) HSteamUser __cdecl SteamAPI_GetHSteamUser()
    {
        return NULL;
    }

    __declspec(dllexport) const char *__cdecl SteamAPI_GetSteamInstallPath()
    {
        return NULL;
    }

    __declspec(dllexport) bool __cdecl SteamAPI_Init()
    {
		//DisableOldVersions();
		//SteamProxy_Init();
        
		return true;
    }

    __declspec(dllexport) bool __cdecl SteamAPI_InitSafe()
    {
        return true;
    }

	__declspec(dllexport) char __cdecl SteamAPI_RestartApp()
    {
        return 1;
    }

	__declspec(dllexport) char __cdecl SteamAPI_RestartAppIfNecessary()
    {	
        return 0;
    }
    
	__declspec(dllexport) void __cdecl SteamAPI_RegisterCallResult( CCallbackBase* pResult, SteamAPICall_t APICall )
    {
		CSteamBase::RegisterCallResult(APICall, pResult);
    }

    __declspec(dllexport) void __cdecl SteamAPI_RegisterCallback( CCallbackBase *pCallback, int iCallback )
    {
		CSteamBase::RegisterCallback(pCallback, iCallback);
    }

    __declspec(dllexport) void __cdecl SteamAPI_RunCallbacks()
    {
		CSteamBase::RunCallbacks();
    }

    __declspec(dllexport) void __cdecl SteamAPI_SetMiniDumpComment( const char *pchMsg )
    {
		
    }

    __declspec(dllexport) bool __cdecl SteamAPI_SetTryCatchCallbacks( bool bUnknown )
    {
        return bUnknown;
    }

    __declspec(dllexport) void __cdecl SteamAPI_Shutdown()
    {

    }

    __declspec(dllexport) void __cdecl SteamAPI_UnregisterCallResult( CCallbackBase* pResult, SteamAPICall_t APICall )
    {
		
    }
    
	__declspec(dllexport) void __cdecl SteamAPI_UnregisterCallback( CCallbackBase *pCallback, int iCallback )
    {
	
    }

    __declspec(dllexport) void __cdecl SteamAPI_WriteMiniDump( uint32 uStructuredExceptionCode, void* pvExceptionInfo, uint32 uBuildID )
    {
		
    }

    __declspec(dllexport) ISteamApps* __cdecl SteamApps()
    {
		return NULL;
    }

    __declspec(dllexport) ISteamClient* __cdecl SteamClient()
    {
		return NULL;
    }

    __declspec(dllexport) ISteamContentServer* __cdecl SteamContentServer()
    {
        return NULL;
    }

    __declspec(dllexport) ISteamUtils* __cdecl SteamContentServerUtils()
    {
        return NULL;
    }

    __declspec(dllexport) bool __cdecl SteamContentServer_Init( unsigned int uLocalIP, unsigned short usPort )
    {
        return NULL;
    }

    __declspec(dllexport) void __cdecl SteamContentServer_RunCallbacks()
    {
		
    }

    __declspec(dllexport) void __cdecl SteamContentServer_Shutdown()
    {
		
    }

    __declspec(dllexport) ISteamFriends* __cdecl SteamFriends()
    {
		return (ISteamFriends*)CSteamBase::GetInterface(INTERFACE_STEAMFRIENDS005);
    }

    __declspec(dllexport) ISteamGameServer* __cdecl SteamGameServer()
    {
		return (ISteamGameServer*)CSteamBase::GetInterface(INTERFACE_STEAMGAMESERVER009);
    }

    __declspec(dllexport) ISteamUtils* __cdecl SteamGameServerUtils()
    {
        return NULL;
    }

    __declspec(dllexport) bool __cdecl SteamGameServer_BSecure()
    {
        return true;
    }

    __declspec(dllexport) HSteamPipe __cdecl SteamGameServer_GetHSteamPipe()
    {
        return NULL;
    }

    __declspec(dllexport) HSteamUser __cdecl SteamGameServer_GetHSteamUser()
    {
        return NULL;
    }

    __declspec(dllexport) int32 __cdecl SteamGameServer_GetIPCCallCount()
    {
        return NULL;
    }

    __declspec(dllexport) uint64 __cdecl SteamGameServer_GetSteamID()
    {
        return NULL;
    }

    __declspec(dllexport) bool __cdecl SteamGameServer_Init( uint32 unIP, uint16 usPort, uint16 usGamePort, EServerMode eServerMode, int nGameAppId, const char *pchGameDir, const char *pchVersionString )
    {
        return true;
    }

    __declspec(dllexport) bool __cdecl SteamGameServer_InitSafe( uint32 unIP, uint16 usPort, uint16 usGamePort, EServerMode eServerMode, int nGameAppId, const char *pchGameDir, const char *pchVersionString, unsigned long dongs )
    {
        return true;
    }

    __declspec(dllexport) void __cdecl SteamGameServer_RunCallbacks()
    {
		
    }

    __declspec(dllexport) void __cdecl SteamGameServer_Shutdown()
    {
		
    }

    __declspec(dllexport) ISteamMasterServerUpdater* __cdecl SteamMasterServerUpdater()
    {
		return (ISteamMasterServerUpdater*)CSteamBase::GetInterface(INTERFACE_STEAMMASTERSERVERUPDATER001);
    }

    __declspec(dllexport) ISteamMatchmaking* __cdecl SteamMatchmaking()
    {
		return (ISteamMatchmaking*)CSteamBase::GetInterface(INTERFACE_STEAMMATCHMAKING007);
    }

    __declspec(dllexport) ISteamMatchmakingServers* __cdecl SteamMatchmakingServers()
    {
        return NULL;
    }

    __declspec(dllexport) ISteamNetworking* __cdecl SteamNetworking()
    {
		return (ISteamNetworking*)CSteamBase::GetInterface(INTERFACE_STEAMNETWORKING003);
    }

    __declspec(dllexport) void* __cdecl SteamRemoteStorage()
    {
		return CSteamBase::GetInterface(INTERFACE_STEAMREMOTESTORAGE002);
    }

    __declspec(dllexport) ISteamUser* __cdecl SteamUser()
    {
		return (ISteamUser*)CSteamBase::GetInterface(INTERFACE_STEAMUSER012);
    }

    __declspec(dllexport) ISteamUserStats* __cdecl SteamUserStats()
    {
		return NULL;
    }

    __declspec(dllexport) ISteamUtils* __cdecl SteamUtils()
    {
		return (ISteamUtils*)CSteamBase::GetInterface(INTERFACE_STEAMUTILS005);
    }

    __declspec(dllexport) HSteamUser __cdecl Steam_GetHSteamUserCurrent()
    {
        return NULL;
    }

    __declspec(dllexport) void __cdecl Steam_RegisterInterfaceFuncs( void *hModule )
    {
		
	}

    __declspec(dllexport) void __cdecl Steam_RunCallbacks( HSteamPipe hSteamPipe, bool bGameServerCallbacks )
    {
		
    }

    __declspec(dllexport) void *g_pSteamClientGameServer = NULL;
}