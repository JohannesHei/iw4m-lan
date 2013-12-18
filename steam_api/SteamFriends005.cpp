#include "StdInc.h"
#include "SteamFriends005.h"

unsigned int GetPlayerSteamID();
const char* personaName = NULL;

const char *CSteamFriends005::GetPersonaName()
{
	/*if (FileExists("iLabsIW4.ini"))
	{
		CIniReader reader(".\\iLabsIW4.ini");
		personaName = reader.ReadString("Configuration", "Nickname", "Unknown Soldier");
		
		return personaName;
	}
	else
	{
		return "Unknown Soldier";
	}

	if (personaName = 0)
	{
		return "Unknown Soldier";
	}

	if (personaName = "")
	{
		return "Unknown Soldier";
	}*/

	//return personaName;
	return "UnknownPlayer";
}

void CSteamFriends005::SetPersonaName( const char *pchPersonaName )
{

}

EPersonaState CSteamFriends005::GetPersonaState()
{
	return k_EPersonaStateOnline;
}

int CSteamFriends005::GetFriendCount( EFriendFlags iFriendFlags )
{
	return 0;
}

CSteamID CSteamFriends005::GetFriendByIndex( int iFriend, int iFriendFlags )
{
	return CSteamID(248938, k_EUniverseBeta, k_EAccountTypeAnonGameServer);
}

EFriendRelationship CSteamFriends005::GetFriendRelationship( CSteamID steamIDFriend )
{
	return k_EFriendRelationshipNone;
}

EPersonaState CSteamFriends005::GetFriendPersonaState( CSteamID steamIDFriend )
{
	return k_EPersonaStateOffline;
}

const char *CSteamFriends005::GetFriendPersonaName( CSteamID steamIDFriend )
{
	return "Unknown Soldier";
}

int CSteamFriends005::GetFriendAvatar( CSteamID steamIDFriend, int eAvatarSize )
{
	return 0;
}

bool CSteamFriends005::GetFriendGamePlayed( CSteamID steamIDFriend, FriendGameInfo_t *pFriendGameInfo )
{
	return false;
}

const char *CSteamFriends005::GetFriendPersonaNameHistory( CSteamID steamIDFriend, int iPersonaName )
{
	return "";
}

bool CSteamFriends005::HasFriend( CSteamID steamIDFriend, EFriendFlags iFriendFlags )
{
	return false;
}

int CSteamFriends005::GetClanCount()
{
	return 0;
}

CSteamID CSteamFriends005::GetClanByIndex( int iClan )
{
	return CSteamID(1337, k_EUniverseBeta, k_EAccountTypeAnonGameServer);
}

const char *CSteamFriends005::GetClanName( CSteamID steamIDClan )
{
	return "IW4";
}

int CSteamFriends005::GetFriendCountFromSource( CSteamID steamIDSource )
{
	return 0;
}

CSteamID CSteamFriends005::GetFriendFromSourceByIndex( CSteamID steamIDSource, int iFriend )
{
	return CSteamID(1337, k_EUniverseBeta, k_EAccountTypeAnonGameServer);
}

bool CSteamFriends005::IsUserInSource( CSteamID steamIDUser, CSteamID steamIDSource )
{
	return false;
}

void CSteamFriends005::SetInGameVoiceSpeaking( CSteamID steamIDUser, bool bSpeaking )
{
	
}

void CSteamFriends005::ActivateGameOverlay( const char *pchDialog )
{
	if (!_stricmp(pchDialog, "LobbyInvite"))
	{
		Cmd_ExecuteSingleCommand(0, 0, "openmenu pc_join_unranked");
		//Com_Error(2, "This feature is not available.");
	}
}

void CSteamFriends005::ActivateGameOverlayToUser( const char *pchDialog, CSteamID steamID )
{
	
}

void CSteamFriends005::ActivateGameOverlayToWebPage( const char *pchURL )
{
	
}

void CSteamFriends005::ActivateGameOverlayToStore( AppId_t nAppID )
{
	
}

void CSteamFriends005::SetPlayedWith( CSteamID steamIDUserPlayedWith )
{
	
}