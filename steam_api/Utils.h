bool FileExists(const char* file);
size_t FileSize(const char* file);
bool DirectoryExists(const char* file);
char *str_replace(char *orig, char *rep, char *with);
void CreateDirectoryAnyDepth(const char *path);

const char* va(const char* format, ...);
unsigned int jenkins_one_at_a_time_hash(char *key, size_t len);

size_t Com_AddToString(const char* source, char* buffer, size_t current, size_t length, bool escapeSpaces);

#define	MAX_INFO_STRING		1024
#define	MAX_INFO_KEY		1024
#define	MAX_INFO_VALUE		1024

void Info_RemoveKey( char *s, const char *key );
void Info_SetValueForKey( char *s, const char *key, const char *value );
char *Info_ValueForKey( const char *s, const char *key );

void Info(char* message);
void Warning(char* message);
void Error(char* message);