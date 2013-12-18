#include "StdInc.h"

// flag settings
#define GAME_FLAG_OFFLINE	(1 << 0)
#define GAME_FLAG_CONSOLE	(1 << 1)

#define GAME_FLAG(x)			((_gameFlags & x) == x)

extern unsigned int _gameFlags;
void DetermineGameFlags();