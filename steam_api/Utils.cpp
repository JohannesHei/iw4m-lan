#include "StdInc.h"
#include <ShellAPI.h>
#include <sys/stat.h>
#include <direct.h>
#include <io.h>

#define VA_BUFFER_COUNT		4
#define VA_BUFFER_SIZE		32768

static char g_vaBuffer[VA_BUFFER_COUNT][VA_BUFFER_SIZE];
static int g_vaNextBufferIndex = 0;

const char *va( const char *fmt, ... )
{
	va_list ap;
	char *dest = &g_vaBuffer[g_vaNextBufferIndex][0];
	g_vaNextBufferIndex = (g_vaNextBufferIndex + 1) % VA_BUFFER_COUNT;
	va_start(ap, fmt);
	int res = _vsnprintf( dest, VA_BUFFER_SIZE, fmt, ap );
	dest[VA_BUFFER_SIZE - 1] = '\0';
	va_end(ap);

	if (res < 0 || res >= VA_BUFFER_SIZE)
	{
		//Com_Error(1, "Attempted to overrun string in call to va() - return address 0x%x", _ReturnAddress());
	}

	return dest;
}

size_t Com_AddToString(const char* string, char* buffer, size_t current, size_t length, bool escapeSpaces)
{
	const char* i = string;
	size_t margin = (escapeSpaces) ? 2 : 0;
	bool hadSpaces = false;

	if (length - current <= 0)
	{
		return current;
	}

	if (escapeSpaces)
	{
		if ((length - current) > margin)
		{
			while (*i != 0)
			{
				if (*i == ' ')
				{
					buffer[current++] = '"';
					hadSpaces = true;
					break;
				}

				i++;
			}
		}
	}

	i = string;
	while (*i != '\0')
	{
		if (length - current > margin)
		{
			buffer[current++] = *i;
		}

		i++;
	}

	if (hadSpaces)
	{
		buffer[current++] = '"';
	}

	return current;
}

/*
===================
Info_RemoveKey
===================
*/
void Info_RemoveKey( char *s, const char *key ) {
	char	*start;
	char	pkey[MAX_INFO_KEY];
	char	value[MAX_INFO_VALUE];
	char	*o;

	if (strchr (key, '\\')) {
		return;
	}

	while (1)
	{
		start = s;
		if (*s == '\\')
			s++;
		o = pkey;
		while (*s != '\\')
		{
			if (!*s)
				return;
			*o++ = *s++;
		}
		*o = 0;
		s++;

		o = value;
		while (*s != '\\' && *s)
		{
			if (!*s)
				return;
			*o++ = *s++;
		}
		*o = 0;

		if (!strcmp (key, pkey) )
		{
			strcpy (start, s);	// remove this part
			return;
		}

		if (!*s)
			return;
	}

}

/*
==================
Info_SetValueForKey

Changes or adds a key/value pair
==================
*/
void Info_SetValueForKey( char *s, const char *key, const char *value ) {
	char	newi[MAX_INFO_STRING];

	Info_RemoveKey (s, key);
	if (!value || !strlen(value))
		return;

	_snprintf (newi, sizeof(newi), "\\%s\\%s", key, value);

	strcat (newi, s);
	strcpy (s, newi);
}

#define	BIG_INFO_STRING		8192  // used for system info key only
#define	BIG_INFO_KEY		  8192
#define	BIG_INFO_VALUE		8192

/*
===============
Info_ValueForKey

Searches the string for the given
key and returns the associated value, or an empty string.
FIXME: overflow check?
===============
*/
char *Info_ValueForKey( const char *s, const char *key )
{
	char	pkey[BIG_INFO_KEY];
	static	char value[2][BIG_INFO_VALUE];	// use two buffers so compares
	// work without stomping on each other
	static	int	valueindex = 0;
	char	*o;

	if ( !s || !key ) {
		return "";
	}

	if ( strlen( s ) >= BIG_INFO_STRING ) {
		//Com_Error( ERR_DROP, "Info_ValueForKey: oversize infostring" );
		return "";
	}

	valueindex ^= 1;
	if (*s == '\\')
		s++;
	while (1)
	{
		o = pkey;
		while (*s != '\\')
		{
			if (!*s)
				return "";
			*o++ = *s++;
		}
		*o = 0;
		s++;

		o = value[valueindex];

		while (*s != '\\' && *s)
		{
			*o++ = *s++;
		}
		*o = 0;

		if (!_stricmp (key, pkey) )
			return value[valueindex];

		if (!*s)
			break;
		s++;
	}

	return "";
}

unsigned int jenkins_one_at_a_time_hash(char *key, size_t len)
{
	unsigned int hash, i;
	for(hash = i = 0; i < len; ++i)
	{
		hash += key[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}

bool FileExists(const char* file)
{
	struct stat st;

	// note that this doesn't count any of the other ways stat could fail, but that'd be more error checking elsewhere
	if (stat(file, &st) >= 0)
	{
		return true;
	}

	return false;
}

bool FolderExists(const char* folder)
{
	struct stat st;

	// note that this doesn't count any of the other ways stat could fail, but that'd be more error checking elsewhere
	if (stat(folder, &st) >= 0)
	{
		return true;
	}

	return false;
}

void Info(char* message)
{
	MessageBoxA(NULL, message, "iLabsIW4 - Info",  MB_OK | MB_ICONINFORMATION);
}

void Warning(char* message)
{
	MessageBoxA(NULL, message, "iLabsIW4 - Warning",  MB_OK | MB_ICONWARNING);
}

void Error(char* message)
{
	MessageBoxA(NULL, message, "iLabsIW4 - Error", MB_OK | MB_ICONSTOP /*ERROR*/);
}