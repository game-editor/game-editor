#ifndef MAKS_KYRA_INCLUDE
#define MAKS_KYRA_INCLUDE

#include <stdlib.h>
#include "system.h"
#include "SDL_mixer.h"
#include "gedString.h"

#define	MAX_ACTOR_DEPTH	60000
#define	REGION_DEPTH	(MAX_ACTOR_DEPTH + 500)
#define	PATH_DEPTH		(REGION_DEPTH + 500)
#define	GUI_DEPTH		(PATH_DEPTH + 500)
#define	CURSOR_DEPTH	65535



//Wait Cursor 
#define WAIT_CWIDTH		13
#define WAIT_CHEIGHT	22
#define WAIT_CHOTX		6
#define WAIT_CHOTY		11

extern unsigned char wait_cdata[];
extern unsigned char wait_cmask[];
extern SDL_Cursor *SDL_waitcursor;


extern gedString getRandomName();
gedString AbbreviateDirName(gedString lpszCanon, int cchMax, bool bAtLeastName = true);

////////////////////////////////////////////////////////////////////////
//Directory name convertion
void correctDirectoryName(const char *oldName, char *newName);
SDL_RWops *ged_SDL_RWFromFile(const char *file, const char *mode);
SDL_Surface *ged_IMG_Load(const char *file);
Mix_Music *ged_Mix_LoadMUS(const char *file);
Mix_Chunk *ged_Mix_LoadWAV(const char *file);
////////////////////////////////////////////////////////////////////////

SDL_RWops *ged_SDL_RWMemoryMapFile(const char *file, const char *mode);

class Actor;

class stCursor
{
public:
	stCursor(gedString _imgName = "",
		Sint16 _nFramesH = 0,
		Sint16 _nFramesV = 0,
		Sint16 _hotSpotX = 0,
		Sint16 _hotSpotY = 0,
		bool _bAnimate = false) 
	{
		imgName = _imgName;
		nFramesH = _nFramesH;
		nFramesV = _nFramesV;
		hotSpotX = _hotSpotX;
		hotSpotY = _hotSpotY;
		bAnimate = _bAnimate;
	}

	bool operator==( const stCursor& other ) const
	{ 
		if(
			imgName == other.imgName &&
			nFramesH == other.nFramesH &&
			nFramesV == other.nFramesV &&
			hotSpotX == other.hotSpotX &&
			hotSpotY == other.hotSpotY &&
			bAnimate == other.bAnimate
		  ) 
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	bool operator!=( const stCursor& other ) const
	{ 
		return !(*this == other);
	}

	gedString imgName;
	Sint16 nFramesH;
	Sint16 nFramesV;
	Sint16 hotSpotX;
	Sint16 hotSpotY;
	bool bAnimate;
#ifndef STAND_ALONE_GAME
	void Save(SDL_RWops *src);
#endif //#ifndef STAND_ALONE_GAME

	void Load(SDL_RWops *src);
};

class stSoundFile
{
public:
	stSoundFile() {bMusic = false;}
	stSoundFile(gedString _path, bool _bMusic) {path = _path; bMusic = _bMusic;}
	void operator=( const stSoundFile& copy ) 
	{ 
		if( &copy != this )
		{
			path = copy.path;
			bMusic = copy.bMusic;
		}
	}

	gedString path;
	bool bMusic;
};

typedef GlSList<stSoundFile> ListSounds;  
typedef GlSListIterator<stSoundFile> ListSoundsIterator;

class stAudioIndex
{
public:
	stAudioIndex() {offset = size = 0;}
	stAudioIndex(Uint32 _offset, Uint32 _size) {offset = _offset; size = _size;}
	void operator=( const stAudioIndex& copy ) 
	{ 
		if( &copy != this )
		{
			offset = copy.offset;
			size = copy.size;
		}
	}

	Uint32 offset;
	Uint32 size;
};

typedef GlMap< gedString, stAudioIndex, GlStringHash >		MapAudioFileIndex;
typedef GlMapIterator< gedString, stAudioIndex, GlStringHash >		MapAudioFileIndexIterator;



SDL_Surface* ToSupportedSurface(SDL_Surface* surface, bool bRemoveOld = true);
SDL_Surface* ToSurface32(SDL_Surface* surface);
SDL_Surface* LoadSurface(	const char* filename, bool bTryEditor = true/*, KrRGBA &transRgba*/);
SDL_Surface* LoadSurfaceEditor(const char *fileName, KrResourceVault *vault = NULL);

void EnableCursor();
void DisableCursor();
void DrawCursor(KrEngine *engine, const stCursor *cursor, int x, int y, bool bImediateDraw = false);

enum
{
	VAR_NAME_OK = 0,
	VAR_NAME_INVALID_INTIAL_CHAR,
	VAR_NAME_INVALID_CHAR,
	VAR_NAME_IS_MACRO,
	VAR_NAME_IS_FUNCTION_OR_DECLARED_VAR,
	VAR_NAME_IS_RESERVED_WORD
};

int isValidVarName(const char *name);
void *isSymbolInEic(const char *id);
int IMG_isAnimatedGIF(const char *fileName);
SDL_Surface *IMG_LoadAnimatedGIF_RW(SDL_RWops *src, int imageNumber, double &gifFps, int &left, int &top, int &disposal, SDL_Color &transpColor);
void SplitPath(const gedString& fullPath, gedString& file, gedString& path);
U32 Crc32(void * seq, long size);
U32 FileCrc32(const char *filename, int startOffSet = 0);
void SetCaption(const char *str, const char *icon);

#ifndef STAND_ALONE_GAME

KrSpriteResource *LoadTiledSprite(gedString imgName, gedString spriteName, int nx, int ny, KrSpriteResource **sr = NULL, char *action = NULL, int hotSpotX = -1, int hotSpotY = -1);
bool copyfile(const char *oldFrom, const char *oldTo, const char *oldFPath, bool overwrite = false);
bool copy(SDL_RWops *from, SDL_RWops *to);
bool copy(const char *fromFileName, SDL_RWops *to);
bool copy(const char *fromFileName, const char *toFileName);
bool concatenateFile(const char *oldFirst, const char *oldSecond);
SDL_RWops *LoadToMemoryFile(gedString fileName);
bool IsSolidImage(SDL_Surface *surface, KrRGBA& transColor);

int SDL_SavePNG(SDL_Surface *surface, const char *file);
long GetFileWriteTime(gedString file);

int SDL_SavePNG_RW(SDL_Surface *surface, SDL_RWops *dst, int freedst);
SDL_Surface *GetSDLSurface(Actor *actor, int w = -1, int h = -1, int frame = 0, U32 colorKey = 0);

void ExtractSoundPath(gedString code, gedString function, ListSounds& list);
void SaveSoundList(const ListSounds &list, SDL_RWops *exeFile, MapAudioFileIndex &audioIndex);

bool CanShowString(gedString s);

#ifdef WIN32
extern "C" int CanShowStringW(TCHAR *s);
#endif

#endif

#endif //MAKS_KYRA_INCLUDE