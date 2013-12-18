// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: branding for IW4M
//
// Initial author: NTAuthority
// Started: 2011-06-09
// ==========================================================

#include "StdInc.h"

typedef void (__cdecl * R_AddCmdDrawText_t)(const char* text, int, void* font, float screenX, float screenY, float, float, float rotation, float* color, int);
R_AddCmdDrawText_t R_AddCmdDrawText = (R_AddCmdDrawText_t)0x509D80;

CallHook drawDevStuffHook;
DWORD drawDevStuffHookLoc = 0x5ACB99;

void DrawDemoWarning()
{
	void* font = DB_FindXAssetHeader(ASSET_TYPE_FONT, "fonts/consolefont");
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	R_AddCmdDrawText(VERSIONSTRING, 0x7FFFFFFF, font, 5, 20, 1.0f, 1.0f, 0.0f, color, 0);
}

void __declspec(naked) DrawDevStuffHookStub()
{
	__asm
	{
		call DrawDemoWarning
		jmp drawDevStuffHook.pOriginal
	}
}

void PatchMW2_Branding()
{
	// displayed build tag in UI code
	*(DWORD*)0x43F73B = (DWORD)VERSIONSTRING;

	// console '%s: %s> ' string
	*(DWORD*)0x5A44B4 = (DWORD)(VERSIONSTRING "> ");

	// console version string
	*(DWORD*)0x4B12BB = (DWORD)(VERSIONSTRING " (built " __DATE__ " " __TIME__ ")");
	//*(DWORD*)0x4B12BB = (DWORD)(VERSIONSTRING " " BUILDHOST " (built " __DATE__ " " __TIME__ ")");

	// version string
	*(DWORD*)0x60BD56 = (DWORD)(VERSIONSTRING " (built " __DATE__ " " __TIME__ ")");

	// increase font sizes for chat on higher resolutions
	static float float13 = 13.0f;
	static float float10 = 10.0f;
	*(float**)0x5814AE = &float13;
	*(float**)0x5814C8 = &float10;

	// always play the intro video
	//nop(0x60BECF, 2);

	//*(DWORD*)0x60BED2 = (DWORD)"unskippablecinematic IW_logo\n";

	drawDevStuffHook.initialize(drawDevStuffHookLoc, DrawDevStuffHookStub);
	drawDevStuffHook.installHook();
}