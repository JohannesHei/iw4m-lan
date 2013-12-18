#include "StdInc.h"
#include <google/dense_hash_map>

struct localizedEntry_s
{
	const char* value;
	const char* name;
};

StompHook loadLocalizeHook;
DWORD loadLocalizeHookLoc = 0x629B90;

static google::dense_hash_map<std::string, std::string> _localizedStrings;

const char* SEH_GetLocalizedString(const char* key)
{
	google::dense_hash_map<std::string, std::string>::const_iterator iter = _localizedStrings.find(key);

	if (iter != _localizedStrings.end())
	{
		return (*iter).second.c_str();
	}

	localizedEntry_s* entry = (localizedEntry_s*)DB_FindXAssetHeader(ASSET_TYPE_LOCALIZE, key);

	if (entry)
	{
		return entry->value;
	}
	
	return NULL;
}

void PatchMW2_LocalizedStrings()
{
	_localizedStrings.set_empty_key("");

	_localizedStrings["PLATFORM_PLAY_ONLINE_CAPS"] = "PLAY OFFLINE";
	_localizedStrings["XBOXLIVE_SERVICENAME"] = "IW4LAN";
	_localizedStrings["EXE_AWAITINGGAMESTATE"] = "Awaiting gamestate";

	loadLocalizeHook.initialize(loadLocalizeHookLoc, SEH_GetLocalizedString);
	loadLocalizeHook.installHook();
}