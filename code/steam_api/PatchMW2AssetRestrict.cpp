// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: Modern Warfare 2 patches: asset restrictions and
//          pre-load modifications.
//
// Initial author: NTAuthority
// Started: 2011-05-20
// ==========================================================

#include "StdInc.h"
#include <unordered_map>

StompHook dbAddXAssetHook;
DWORD dbAddXAssetHookLoc = 0x5BB650;
DWORD dbAddXAssetHookRet = 0x5BB657;

typedef const char* (__cdecl * DB_GetXAssetNameHandler_t)(void* asset);
DB_GetXAssetNameHandler_t* DB_GetXAssetNameHandlers = (DB_GetXAssetNameHandler_t*)0x799328;

char CanWeLoadAsset(assetType_t type, void* entry)
{
	const char* name = DB_GetXAssetNameHandlers[type](entry);

	if (type == ASSET_TYPE_WEAPON)
	{
		// somewhat-workaround for issue 'could not load weapon "destructible_car"' and cars not doing any damage
		if (strcmp(CURRENT_ZONE_NAME, "common_tc_mp") && (!strcmp(name, "none") || !strcmp(name, "destructible_car"))) // common_tc_mp also has these
		{
			return 1;
		}

		if (!strcmp(CURRENT_ZONE_NAME, "patch_mp"))
		{
			return 0;
		}
	}

	return 1;
}

void* LoadDefaultAsset(assetType_t atype)
{
	void* defaultAsset;

	__asm
	{
		push edi
		mov edi, atype
		mov eax, 5BB210h
		call eax
		pop edi

		mov defaultAsset, eax
	}

	static void* retStuff[2];
	retStuff[0] = 0;
	retStuff[1] = defaultAsset;

	return retStuff;
}

void __declspec(naked) DB_AddXAssetHookStub()
{
	__asm
	{
		mov eax, [esp + 4]
		mov ecx, [esp + 8]

		push ecx
		push eax
		call CanWeLoadAsset
		add esp, 08h

		cmp al, 2h
		je doDefault

		test al, al
		jz doNotLoad

		mov eax, [esp + 4]
		mov ecx, [esp + 8]
		push ecx
		push eax
		//call DoBeforeLoadAsset
		add esp, 08h

		mov eax, [esp + 8]
		sub esp, 14h
		jmp dbAddXAssetHookRet

doNotLoad:
		mov eax, [esp + 8]
		retn

doDefault:
		mov eax, [esp + 4]

		push eax
		call LoadDefaultAsset
		add esp, 4h

		retn
	}
}

StompHook weaponFileHook;
DWORD weaponFileHookLoc = 0x57B650;

void* WeaponFileHookFunc(const char* filename)
{
	//if (!_stricmp(filename, "m40a3") || !_stricmp(filename, "winchester1200") || !_stricmp(filename, "mg4_heartbeat_mp") || !_stricmp(filename, "famas_mp") || !_stricmp(filename, "ak47_mp") || !_stricmp(filename, "cheytac_mp"))
	/*if (!Legacy_IsLegacyMode())
	{
		if (!_stricmp(filename, "destructible_car") || !_stricmp(filename, "none"))
		{
			return DB_FindXAssetHeader(ASSET_TYPE_WEAPON, "defaultweapon_mp");
		}
	}*/

	if (FS_ReadFile(va("weapons/mp/%s", filename), NULL) > 0)
	{
		return ((void* (*)(const char*))0x57B5F0)(filename); // BG_LoadWeaponDef_LoadObj
	}

	char* file = (char*)DB_FindXAssetHeader(0x1C, filename);

	//TerminateProcess(GetCurrentProcess(), 0);

	return file;
}

void __declspec(naked) WeaponFileHookStub()
{
	__asm jmp WeaponFileHookFunc
}

typedef int (__cdecl * DB_GetXAssetSizeHandler_t)();

void** DB_XAssetPool = (void**)0x7998A8;
unsigned int* g_poolSize = (unsigned int*)0x7995E8;

DB_GetXAssetSizeHandler_t* DB_GetXAssetSizeHandlers = (DB_GetXAssetSizeHandler_t*)0x799488;

void* ReallocateAssetPool(assetType_t type, unsigned int newSize)
{
	int elSize = DB_GetXAssetSizeHandlers[type]();
	void* poolEntry = malloc(newSize * elSize);
	DB_XAssetPool[type] = poolEntry;
	g_poolSize[type] = newSize;
	return poolEntry;
}

void PatchMW2_AssetRestrict()
{
	dbAddXAssetHook.initialize(dbAddXAssetHookLoc, DB_AddXAssetHookStub, 7);
	dbAddXAssetHook.installHook();

	weaponFileHook.initialize(weaponFileHookLoc, WeaponFileHookStub);
	weaponFileHook.installHook();

	ReallocateAssetPool(ASSET_TYPE_IMAGE, 7168);
	
	/* ReallocateAssetPool(ASSET_TYPE_LOADED_SOUND, 2700);
	ReallocateAssetPool(ASSET_TYPE_FX, 1200);
	ReallocateAssetPool(ASSET_TYPE_LOCALIZE, 14000);
	ReallocateAssetPool(ASSET_TYPE_XANIM, 8192);
	ReallocateAssetPool(ASSET_TYPE_XMODEL, 3072);
	ReallocateAssetPool(ASSET_TYPE_PHYSPRESET, 128);
	ReallocateAssetPool(ASSET_TYPE_PIXELSHADER, 10000);
	//ReallocateAssetPool(ASSET_TYPE_ADDON_MAP_ENTS, 128);
	ReallocateAssetPool(ASSET_TYPE_VERTEXSHADER, 3072);
	//ReallocateAssetPool(ASSET_TYPE_TECHSET, 1024);
	//ReallocateAssetPool(ASSET_TYPE_MATERIAL, 8192);
	ReallocateAssetPool(ASSET_TYPE_VERTEXDECL, 196); */
}