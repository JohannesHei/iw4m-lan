#include "StdInc.h"
#include <time.h>
#include <winsock.h>
#include <dbghelp.h>
#include <shellapi.h>

void InstallCrashHandler();

CallHook winMainInitHook;
DWORD winMainInitHookLoc = 0x4513D0;

void __declspec(naked) WinMainInitHookStub()
{
	InstallCrashHandler();

	__asm
	{
		jmp winMainInitHook.pOriginal
	}
}

void CrashHandlerInit()
{
	winMainInitHook.initialize(winMainInitHookLoc, WinMainInitHookStub);
	winMainInitHook.installHook();
}

LONG WINAPI CustomUnhandledExceptionFilter(LPEXCEPTION_POINTERS ExceptionInfo)
{
	// step 1: write minidump
	char error[1024];
	char filename[MAX_PATH];
	__time64_t time;
	tm* ltime;

	_time64(&time);
	ltime = _localtime64(&time);
	strftime(filename, sizeof(filename) - 1, "iLabsIW4-%Y%m%d%H%M%S.dmp", ltime);
	_snprintf(error, sizeof(error) - 1, "A minidump has been written to %s.", filename);

	HANDLE hFile = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION ex;
		memset(&ex, 0, sizeof(ex));
		ex.ThreadId = GetCurrentThreadId();
		ex.ExceptionPointers = ExceptionInfo;
		ex.ClientPointers = FALSE;

		if (FAILED(MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ex, NULL, NULL)))
		{
			_snprintf(error, sizeof(error) - 1, "An error (0x%x) occurred during writing %s.", GetLastError(), filename);
		}

		CloseHandle(hFile);
	}
	else
	{
		_snprintf(error, sizeof(error) - 1, "An error (0x%x) occurred during creating %s.", GetLastError(), filename);
	}

	// step 2: exit the application
	Com_Error(0, "Fatal error (0x%08x) at 0x%08x.\n%s", ExceptionInfo->ExceptionRecord->ExceptionCode, ExceptionInfo->ExceptionRecord->ExceptionAddress, error);

	return 0;
}

void InstallCrashHandler()
{
	SetUnhandledExceptionFilter(&CustomUnhandledExceptionFilter);
}