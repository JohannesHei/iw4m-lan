#include "StdInc.h"

char* GetHardwareSerialNumber()
{
	HW_PROFILE_INFO hwProfileInfo;
	char* serial = hwProfileInfo.szHwProfileGuid;

	return serial;
}