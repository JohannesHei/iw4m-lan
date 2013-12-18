#pragma once

#define BUILDNUMBER "1"
#define VERSION "1"
#define VERSIONSTRING "IW4LAN r" VERSION

#define masterName "127.0.0.1"

#define _CRT_SECURE_NO_WARNINGS

// Windows headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <map>
#include <vector>
#include <string>
#include <unordered_map>

// OSW headers
#define NO_STEAM // to reduce header amount needed
#include <osw/CCallback.h>
#include <osw/ISteamClient008.h>
#include <osw/ISteamContentServer002.h>
#include <osw/ISteamUser012.h>
#include <osw/ISteamFriends005.h>
#include <osw/ISteamGameServer009.h>
#include <osw/ISteamMasterServerUpdater001.h>
#include <osw/ISteamMatchmaking007.h>
#include <osw/ISteamNetworking003.h>
#include <osw/ISteamRemoteStorage002.h>
#include <osw/ISteamUtils005.h>
#include <osw/ISteamUserStats006.h>

// safe string functions
#define STRSAFE_NO_DEPRECATE
#define STRSAFE_NO_CCH_FUNCTIONS
#include <tchar.h>
#include <strsafe.h>

#undef sprintf_s
#define sprintf_s StringCbPrintf
//#define sprintf_s(buf, size, format, ...) StringCbPrintf(buf, size, format, __VA_ARGS__)

#undef strcat_s
#define strcat_s StringCbCat
//#define strcat_s(dst, size, src) StringCbCat(dst, size, src)

#undef strcpy_s
#define strcpy_s StringCbCopy

// custom headers
#include "SteamBase.h"
#include "Utils.h"
#include "IW4.h"
#include "IniReader.h"
#include "Flags.h"
#include "Hooking.h"

// more names
void PatchMW2();
void Sys_RunInit();