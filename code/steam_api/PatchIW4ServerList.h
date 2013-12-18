#pragma once

#define MAX_GLOBAL_SERVERS 8192
#define	MAX_OTHER_SERVERS 128
#define	MAX_STRING_CHARS 1024

#define	PORT_SERVER	28960
#define	NUM_SERVER_PORTS 4		// broadcast scan this many ports after
#define PORT_MASTER	20810
#define MAX_MASTER_SERVERS 5
// stuff for 'lan party' list
/*
#define SORT_HOST 0
#define SORT_MAP 1
#define SORT_CLIENTS 2
#define SORT_GAME 3
#define SORT_PING 4
*/

#define SORT_HOST 0
#define SORT_MAP 1
#define SORT_CLIENTS 2
#define SORT_GAME 3
//#define SORT_HARDCORE 7
//#define SORT_MOD 8
//#define SORT_UPD 9
#define SORT_PING 4

typedef struct {
	netadr_t	adr;
	int			start;
	int			time;
	char		info[8192];
} ping_t;

typedef struct {
	netadr_t	adr;
	int			visible;
	char	  	hostName[1024];
	char	  	mapName[1024];
	char	  	game[1024];
	int			netType;
	char		gameType[1024];
	char		rgameType[1024];
	char		mod[1024];
	char		version[128];
	int			hardcore;
	int		  	clients;
	int		  	maxClients;
	int			minPing;
	int			maxPing;
	int			ping;
	int			punkbuster;
} serverInfo_t;

typedef struct {
	unsigned char ip[4];
	unsigned short	port;
} serverAddress_t;

typedef struct {
	int			numlocalservers;
	serverInfo_t	localServers[MAX_OTHER_SERVERS];

	int			numglobalservers;
	serverInfo_t  globalServers[MAX_GLOBAL_SERVERS];
	// additional global servers
	int			numGlobalServerAddresses;
	serverAddress_t		globalServerAddresses[MAX_GLOBAL_SERVERS];

	int			numfavoriteservers;
	serverInfo_t	favoriteServers[MAX_OTHER_SERVERS];

	int pingUpdateSource;		// source currently pinging or updating

	int masterNum;
} clientStatic_t;

typedef struct {
	char	adrstr[16];
	int		start;
} pinglist_t;


typedef struct serverStatus_s {
	pinglist_t pingList[32];
	int		numqueriedservers;
	int		currentping;
	int		nextpingtime;
	int		maxservers;
	int		refreshtime;
	int		numServers;
	int		sortKey;
	int		sortDir;
	int		lastCount;
	qboolean refreshActive;
	int		currentServer;
	int		displayServers[MAX_GLOBAL_SERVERS];
	int		numDisplayServers;
	int		numPlayersOnServers;
	int		nextDisplayRefresh;
	int		nextSortTime;
} serverStatus_t;

void CL_FetchFavoritesList();
void UI_DoServerRefresh();
