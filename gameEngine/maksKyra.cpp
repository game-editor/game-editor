#pragma warning(disable:4786)

#include "SDL.h"
#include "SDL_image.h"
#include "engine/kyra.h"
#include "GameControl.h"
#include "compression.h"
#include "SDL_endian.h"
#if __APPLE__
#include "strings.h"
#endif


#if __APPLE__
#include "SDL_rwops.h"
#endif

#include "eic.h"
#include "modules.h"
#include "preproc.h"

#ifndef STAND_ALONE_GAME
#include "png.h"
#endif


extern "C"
{
#include "global.h"
#include "symbol.h"
}

#undef malloc
#undef free
#include <stdio.h>
//#include <stdlib.h>

#include "dlmalloc.h" //maks

extern "C" int EiC_ismacroid(char *id);
#if __APPLE__
int equal(const char a, const char b)
{
	return strcasecmp(&a, &b);
}
#else
extern "C" int equal(const char a, const char b);
#endif



// Global (makes a wrong error in Purify)
//Use the enable/disable to avoid create from heap and destroy from dlmalloc in PocketHAL (infinite loop in dlfree)


int bUse_dlmalloc = 1;
void disable_dlmalloc()
{
	bUse_dlmalloc = 0;
}

void enable_dlmalloc()
{
	bUse_dlmalloc = 1;
}

void *operator new(size_t size)
{
	if(bUse_dlmalloc)
	{		
		return dlmalloc(size);
	}
	else
	{
		return malloc(size);
	}
}

void operator delete(void * mem)
{
	if(mem) 
	{		
		if(bUse_dlmalloc)
		{			
			dlfree(mem);
		}
		else
		{
			free(mem);
		}		
	}
}

void *operator new[](size_t size)
{
	if(bUse_dlmalloc)
	{		
		return dlmalloc(size);
	}
	else
	{
		return malloc(size);
	}
}

void operator delete[](void * mem)
{
	if(mem) 
	{		
		if(bUse_dlmalloc)
		{			
			dlfree(mem);
		}
		else
		{
			free(mem);
		}		
	}
}

#if defined(_WIN32_WCE) && !defined(DISABLE_DLMALLOC)
//It's ok avoid the malloc system functions?
extern "C" void *malloc(size_t bytes)
{	
	return dlmalloc(bytes);
}

extern "C" void free(void *m)
{	
	dlfree(m);
}

extern "C" void* realloc(void* m, size_t bytes) //maks
{	
	return dlrealloc(m, bytes);
}

extern "C" void *calloc(size_t n, size_t elem_size) //maks
{
	return dlcalloc(n, elem_size);
}
#endif


/* Cursor format
Data / Mask Resulting pixel on screen 
0 / 1 White 
1 / 1 Black 
0 / 0 Transparent 
1 / 0 Inverted color if possible, black if not.
*/


/*
//Wait Cursor 
Dimensions: 13x22
Hotspot:	6, 11

Data:

1111111111111	= 0xFF, 0x1F
1100000000011	= 0x03, 0x18
1111111111111	= 0xFF, 0x1F
0100000000010	= 0x02, 0x08
0100000000010	= 0x02, 0x08
0100101010010	= 0x52, 0x09
0100010100010	= 0xA2, 0x08
0110001000110	= 0x46, 0x0C
0011000001100	= 0x0C, 0x06
0001101011000	= 0x58, 0x03
0000110110000	= 0xB0, 0x01
0000110110000	= 0xB0, 0x01
0001100011000	= 0x18, 0x03
0011001001100	= 0x4C, 0x06
0110000000110	= 0x06, 0x0C
0100001000010	= 0x42, 0x08
0100010100010	= 0xA2, 0x08
0100101010010	= 0x52, 0x09
0101010101010	= 0xAA, 0x0A
1111111111111	= 0xFF, 0x1F
1100000000011	= 0X03, 0X18
1111111111111	= 0xFF, 0x1F


Mask:

1111111111111	= 0xFF, 0x1F
1111111111111	= 0xFF, 0x1F
1111111111111	= 0xFF, 0x1F
0111111111110	= 0xFE, 0x0F
0111111111110	= 0xFE, 0x0F
0111111111110	= 0xFE, 0x0F
0111111111110	= 0xFE, 0x0F
0111111111110	= 0xFE, 0x0F
0011111111100	= 0xFC, 0x07
0001111111000	= 0xF8, 0x03
0000111110000	= 0xF0, 0x01
0000111110000	= 0xF0, 0x01
0001111111000	= 0xF8, 0x03
0011111111100	= 0xFC, 0x07
0111111111110	= 0xFE, 0x0F
0111111111110	= 0xFE, 0x0F
0111111111110	= 0xFE, 0x0F
0111111111110	= 0xFE, 0x0F
0111111111110	= 0xFE, 0x0F
1111111111111	= 0xFF, 0x1F
1111111111111	= 0xFF, 0x1F
1111111111111	= 0xFF, 0x1F
*/



/* Added a real MacOS cursor, at the request of Luc-Olivier de Charrière */
unsigned char wait_cdata[] =
{
		0x1F, 0xFF,
		0x18, 0x03,
		0x1F, 0xFF,
		0x08, 0x02,
		0x08, 0x02,
		0x09, 0x52,
		0x08, 0xA2,
		0x0C, 0x46,
		0x06, 0x0C,
		0x03, 0x58,
		0x01, 0xB0,
		0x01, 0xB0,
		0x03, 0x18,
		0x06, 0x4C,
		0x0C, 0x06,
		0x08, 0x42,
		0x08, 0xA2,
		0x09, 0x52,
		0x0A, 0xAA,
		0x1F, 0xFF,
		0X18, 0X03,
		0x1F, 0xFF 
};

unsigned char wait_cmask[] =
{
		0x1F, 0xFF,
		0x1F, 0xFF,
		0x1F, 0xFF,
		0x0F, 0xFE,
		0x0F, 0xFE,
		0x0F, 0xFE,
		0x0F, 0xFE,
		0x0F, 0xFE,
		0x07, 0xFC,
		0x03, 0xF8,
		0x01, 0xF0,
		0x01, 0xF0,
		0x03, 0xF8,
		0x07, 0xFC,
		0x0F, 0xFE,
		0x0F, 0xFE,
		0x0F, 0xFE,
		0x0F, 0xFE,
		0x0F, 0xFE,
		0x1F, 0xFF,
		0x1F, 0xFF,
		0x1F, 0xFF
};


SDL_Cursor *SDL_waitcursor = NULL;






gedString getRandomName()
{
	char s[128];
	static long n = 0;

	sprintf(s, "ged_control%ld", n);

	n++;
	return s;
}

void SetMainWindowCaption(const char *str, const char *icon);

#if defined(WIN32) && !defined(STAND_ALONE_GAME) && !defined(GAME_EDITOR_PROFESSIONAL)
void SetCaption(const char *str, const char *icon)
{
	char daysleft[64], buf[512];

	if(GetEnvironmentVariable("DAYSLEFT",  daysleft, 64))
	{
		sprintf(buf, "%s [Demo Version, %s days left to use]", str, daysleft);

		SetMainWindowCaption(buf, icon);
	}
	else
	{
		SetMainWindowCaption(str, icon);
	}
}
#else
void SetCaption(const char *str, const char *icon)
{
	SetMainWindowCaption(str, icon);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// lpszCanon = C:\MYAPP\DEBUGS\C\TESWIN.C
//
// cchMax   b   Result
// ------   -   ---------
//  1- 7    F   <empty>
//  1- 7    T   TESWIN.C
//  8-14    x   TESWIN.C
// 15-16    x   C:\...\TESWIN.C
// 17-23    x   C:\...\C\TESWIN.C
// 24-25    x   C:\...\DEBUGS\C\TESWIN.C
// 26+      x   C:\MYAPP\DEBUGS\C\TESWIN.C

gedString AbbreviateDirName(gedString lpszCanon, int cchMax, bool bAtLeastName)
{
	int cchFullPath, cchFileName, cchVolName;
	const char *lpszCur;
	const char *lpszBase;
	const char *lpszFileName;
	gedString fileName(lpszCanon.GetFileName()), aux(lpszCanon);

	lpszBase = aux.c_str();
	if(strlen(lpszBase) < 5) return lpszCanon;

	cchFullPath = lpszCanon.length();

	
	lpszFileName = fileName.c_str();
	cchFileName = fileName.length();

	// If cchMax is more than enough to hold the full path name, we're done.
	// This is probably a pretty common case, so we'll put it first.
	if (cchMax >= cchFullPath)
		return lpszCanon;

	// If cchMax isn't enough to hold at least the basename, we're done
	if (cchMax < cchFileName)
	{
		if(bAtLeastName)
		{
			lpszCanon = lpszFileName;
		}
		else
		{
			lpszCanon.clear();
		}

		return lpszCanon;
	}

	// Calculate the length of the volume name.  Normally, this is two characters
	// (e.g., "C:", "D:", etc.), but for a UNC name, it could be more (e.g.,
	// "\\server\share").
	//
	// If cchMax isn't enough to hold at least <volume_name>\...\<base_name>, the
	// result is the base filename.

#ifdef WIN32
	lpszCur = lpszBase + 2;                 // Skip "C:" or leading "\\"
#else
	lpszCur = lpszBase;
#endif

	if (equal(lpszBase[0], '\\') && equal(lpszBase[1], '\\')) // UNC pathname
	{
		// First skip to the '\' between the server name and the share name,
		while (!equal(*lpszCur, '\\'))
		{
			lpszCur++;
			GLASSERT(*lpszCur != '\0');
		}
	}

	// if a UNC get the share name, if a drive get at least one directory
	GLASSERT(equal(*lpszCur, '\\'));

	// make sure there is another directory, not just c:\filename.ext
	if (cchFullPath - cchFileName > 3)
	{
		lpszCur++;
		while (!equal(*lpszCur, '\\'))
		{
			lpszCur++;
			GLASSERT(*lpszCur != '\0');
		}
	}

	GLASSERT(equal(*lpszCur, '\\'));

	cchVolName = lpszCur - lpszBase;
	if (cchMax < cchVolName + 5 + cchFileName)
	{
		lpszCanon = lpszFileName;
		return lpszCanon;
	}

	// Now loop through the remaining directory components until something
	// of the form <volume_name>\...\<one_or_more_dirs>\<base_name> fits.
	//
	// Assert that the whole filename doesn't fit -- this should have been
	// handled earlier.

	GLASSERT(cchVolName + (int)strlen(lpszCur) > cchMax);
	while (cchVolName + 4 + (int)strlen(lpszCur) > cchMax)
	{
		do
		{
			lpszCur++;
			GLASSERT(*lpszCur != '\0');

		} while (!equal(*lpszCur, '\\'));
	}

	// Form the resultant string and we're done.
	// Must use aux string
	lpszCanon = lpszCanon.substr(0, cchVolName);
	lpszCanon += DIR_SEP;
	lpszCanon += "...";
	lpszCanon += lpszCur;


	return lpszCanon;
}

////////////////////////////////////////////////////////////////////////

SDL_RWops *ged_SDL_RWMemoryMapFile(const char *file, const char *mode)
{
	char *newName = (char *)dlmalloc(strlen(file) + 1);
	correctDirectoryName(file, newName);

	SDL_RWops *arq = SDL_RWMemoryMapFile(newName, mode);
	dlfree(newName);
	return arq;

}

SDL_RWops *ged_SDL_RWFromFile(const char *file, const char *mode)
{
	if(!file || strlen(file) <= 0) 
		return NULL;

	char *newName = (char *)dlmalloc(strlen(file) + 1);
	correctDirectoryName(file, newName);

	SDL_RWops *arq = SDL_RWFromFile(newName, mode);
	dlfree(newName);

	return arq;
}

SDL_Surface *ged_IMG_Load(const char *file)
{
	if(!file || strlen(file) <= 0) 
		return NULL;

	char *newName = (char *)dlmalloc(strlen(file) + 1);
	correctDirectoryName(file, newName);

	SDL_Surface *surf = NULL;

#ifndef STAND_ALONE_GAME
	surf = IMG_Load(newName);
#endif //#ifndef STAND_ALONE_GAME

	dlfree(newName);

	return surf;
}

Mix_Music *ged_Mix_LoadMUS(const char *file)
{
	if(!file || strlen(file) <= 0) 
		return NULL;

	char *newName = (char *)dlmalloc(strlen(file) + 1);
	correctDirectoryName(file, newName);

	Mix_Music *mus = Mix_LoadMUS(newName);

#ifdef _DEBUG
	if(mus) GLOUTPUT("Music %s opened\n", newName);
	else GLOUTPUT("Can't open music: %s (%s)\n", newName, SDL_GetError());
#endif

	dlfree(newName);



	SDL_ClearError();
	return mus;
}

Mix_Chunk *ged_Mix_LoadWAV(const char *file)
{
	if(!file || strlen(file) <= 0) 
		return NULL;

	char *newName = (char *)dlmalloc(strlen(file) + 1);
	correctDirectoryName(file, newName);

	Mix_Chunk *chunk = NULL;
	
	if(GameControl::Get()->getStandAloneMode())
	{
		//Decompress sound
		SDL_RWops *src = SDL_RWFromFile(file, "rb"), *srcMem;
		if(src)
		{			
			U32 size = SDL_ReadLE32( src );
			U32 lenghtComp   = SDL_ReadLE32( src );
			
			if(lenghtComp < size)
			{
				U8 *mem = new U8[size];
				if(mem)
				{
					//Decompress
					U8 *in = new U8[lenghtComp];
					if(in)
					{					
						SDL_RWread(src, in, lenghtComp, 1);	
						
						Compression comp;			
						comp.Decompression(in, lenghtComp, mem, size);
						
						delete [] in;					
						
						srcMem = SDL_RWFromMem(mem, size);
						
						//Read sound
						chunk = Mix_LoadWAV_RW(srcMem, 1);
					}
					
					delete [] mem;
				}
			}
			else
			{
				//No compression
				chunk = Mix_LoadWAV_RW(src, 0);
			}

			SDL_RWclose(src);						
		}
	}
	else
	{
		chunk = Mix_LoadWAV(newName);
	}

	dlfree(newName);

	SDL_ClearError();
	return chunk;
}



////////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME
void stCursor::Save(SDL_RWops *src)
{
	if(imgName == "makscursorMao.bmp")
	{
		//Don't save editor cursor
		nFramesH = 0;
	}

	imgName.Write(src);

	SDL_WriteLE16(src, nFramesH);
	SDL_WriteLE16(src, nFramesV);
	SDL_WriteLE16(src, hotSpotX);
	SDL_WriteLE16(src, hotSpotY);
	SDL_RWwrite(src, &bAnimate, sizeof(Uint8), 1);
	
	//Final expansion indicator
	Uint8 expansion = 0;
	SDL_RWwrite(src, &expansion, sizeof(expansion), 1);
}
#endif //#ifndef STAND_ALONE_GAME

void stCursor::Load(SDL_RWops *src)
{
	imgName.Read(src);
	
	nFramesH = GameControl::Read16(src);
	nFramesV = GameControl::Read16(src);
	hotSpotX = GameControl::Read16(src);
	hotSpotY = GameControl::Read16(src);
	GameControl::Read(src, &bAnimate, sizeof(Uint8), 1);
	
	//Final expansion indicator
	Uint8 expansion = 0;
	GameControl::Read(src, &expansion, sizeof(expansion), 1);
}

SDL_Surface* LoadSurfaceEditor(const char *fileName, KrResourceVault *vault)
{
	//Load surface from internal editor images

	SDL_Surface* surface = NULL;	
	SDL_ClearError();
	if(!vault) vault = engine->Vault();
	KrBinaryDataResource* binData = vault->GetBinaryDataResource(fileName);
	if(!binData)
		return NULL;
	
	SDL_RWops *src = SDL_RWFromMem((void *)binData->Data(), binData->Length());
	
	char *ext = (char *)strrchr(fileName, '.');
	if(ext)	ext++;
	
	surface = IMG_LoadTyped_RW(src, 1, ext);
				
	return surface;
}

SDL_Surface* ToSurface32(SDL_Surface* surface)
{   
	Uint8 a;
	
	SDL_PixelFormat format;
	memset(&format, 0, sizeof(SDL_PixelFormat));
	
	format.BitsPerPixel = 32;
	format.BytesPerPixel = 4;
	format.Rmask = 0x00ff0000;
	format.Gmask = 0x0000ff00;
	format.Bmask = 0x000000ff;
	format.Amask = 0xff000000;
#ifndef __APPLE__
	format.alpha = surface->format->alpha;
#else
#ifndef __MACOSX__ //AKR
	SDL_GetSurfaceAlphaMod(surface, &a); 
#endif
#endif

	SDL_Surface *surface32 = SDL_ConvertSurface(surface, &format, SDL_SWSURFACE);
	if(surface32)
	{
		SDL_FreeSurface( surface ); 
		return surface32;
	}
	
	return NULL;
}

SDL_Surface* ToSupportedSurface(SDL_Surface* surface, bool bRemoveOld)
{
	if(surface)
	{
		SDL_Color  colorkey = {0, 0, 0, 0};
		bool bHasColorKey = false;

		if(surface->flags & SDL_SRCCOLORKEY)
		{
			bHasColorKey = true;
#if SDL_VERSION_ATLEAST(1, 3, 0) //TODO: remove this when all projects use the SDL13
			Uint32 k;
			SDL_GetColorKey(surface, &k);
			SDL_GetRGB(k, surface->format, &colorkey.r, &colorkey.g, &colorkey.b);			
#else
			SDL_GetRGB(surface->format->colorkey, surface->format, &colorkey.r, &colorkey.g, &colorkey.b);
#endif
		}
		if(surface->format->BytesPerPixel == 4) 
		{
			//Put any 32 bit image in same mask format
			
			SDL_Surface* sNew = SDL_CreateRGBSurface(SDL_SWSURFACE,
				surface->w,
				surface->h,
				32,
				0x00ff0000,	
				0x0000ff00,
				0x000000ff,
				0xff000000 );
			
			// Now copy one surface to the other, 
			
			//SDL_BlitSurface only works if amask = 0
			for(int x = 0; x < surface->w; x++)
			{
				for(int y = 0; y < surface->h; y++)
				{
					U8* src = (U8*) surface->pixels 
						+ y * surface->pitch
						+ x * surface->format->BytesPerPixel;
					
					U8* dst = (U8*) sNew->pixels 
						+ y * sNew->pitch
						+ x * sNew->format->BytesPerPixel;
					

					//Convert
					U8 r, g, b, a;

					SDL_GetRGBA(*((U32 *)src), surface->format, &r, &g, &b, &a);
					
					*((U32 *)dst) = SDL_MapRGBA( sNew->format, r, g, b, a );
				}
			}
			
			if(bRemoveOld) SDL_FreeSurface( surface );
			surface = sNew;
		}
#ifndef USE_COMPILED_SPRITES
		else if(surface->format->BytesPerPixel == 3) 
		{
			//Put any 24 bit image in same mask format
			
			SDL_Surface* sNew = SDL_CreateRGBSurface(SDL_SWSURFACE,
				surface->w,
				surface->h,
				24,
				0x00ff0000,	
				0x0000ff00,
				0x000000ff,
				0 );
			
			// Now copy one surface to the other, 
			SDL_BlitSurface( surface, 0, sNew, 0 );		
			
			if(bRemoveOld) SDL_FreeSurface( surface );
			surface = sNew;
		}
		else if(surface->format->BytesPerPixel <= 2) 
#else
		else if(surface->format->BytesPerPixel <= 3) 
#endif
		{
			//Convert all surfaces <= 2 bytes per pixel in 5-6-5 16bit images
			
			//Create 16 bit surface
			SDL_Surface* sNew = SDL_CreateRGBSurface(SDL_SWSURFACE,
				surface->w,
				surface->h,
				16,
				0x0000f800,	//5-6-5
				0x000007e0,
				0x0000001f,
				0 );
			
			//Create 24 bit surface
			/*SDL_Surface* sNew = SDL_CreateRGBSurface(SDL_SWSURFACE,
			surface->w,
			surface->h,
			24,
			0x00ff0000,	
			0x0000ff00,
			0x000000ff,
			0 );*/
			
			// Now copy one surface to the other, 
			SDL_BlitSurface( surface, 0, sNew, 0 );		
			
			if(bRemoveOld) SDL_FreeSurface( surface );
			surface = sNew;			
		}
		
		
		/*surface = SDL_DisplayFormat(surface);

		if(surface->format->BytesPerPixel == 4)
		{	

			//Create 32 bit surface
			KrPainter painter( surface );
			KrRGBA transRgba;
			painter.BreakPixel( 0, 0, &transRgba );	
					
			Uint32 colorKey = SDL_MapRGB(surface->format, transRgba.c.red, transRgba.c.green, transRgba.c.blue);

			//SDL_BlitSurface only works if amask = 0
			for(int x = 0; x < surface->w; x++)
			{
				for(int y = 0; y < surface->h; y++)
				{
					U8* src = (U8*) surface->pixels 
						+ y * surface->pitch
						+ x * surface->format->BytesPerPixel;
						

					//Convert
					U8 r, g, b, a;

					SDL_GetRGBA(*((U32 *)src), surface->format, &r, &g, &b, &a);
					
					if( r == transRgba.c.red &&
						g == transRgba.c.green &&
						b == transRgba.c.blue)
					{
						a = 0;
						*((U32 *)src) = SDL_MapRGBA( surface->format, r, g, b, a );
					}
					
				}
			}
		
		}	*/

		
		
		if(bHasColorKey)
		{
			SDL_SetColorKey(surface, SDL_SRCCOLORKEY, SDL_MapRGB(surface->format, colorkey.r, colorkey.g, colorkey.b));
		}
	}

	return surface;
}


SDL_Surface* LoadSurface(	const char* filename, bool bTryEditor/*, KrRGBA &transRgba*/)
{
	// Try to load the file
	if(!filename || strlen(filename) <= 0) return NULL;

	SDL_Surface* surface = ged_IMG_Load( filename );
	
	if(!surface)
	{
		if(bTryEditor)
		{
			//Try internal editor images
			surface = LoadSurfaceEditor(filename);				
			if ( !surface )
				return NULL;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		if(surface->w >= 8192 || surface->h >= 8192)
		{
			SDL_FreeSurface( surface );
			return NULL;
		}
	}

	// The image can be 32 bits or less. A NON-32 bit image has
	// color(s) that are marked transparent. A 32 bit image
	// simply uses the alpha channel. To simplify things,
	// images are converted to 32 bit before they
	// are returned.
	// 
	// Oddly, SDL_Image will sometimes return a 32 bit image,
	// when it wasn't. This will *really* screw up kyra.
	

	return ToSupportedSurface(surface);
}



static bool bEnableCursor = true;
void EnableCursor()
{
	SDL_ShowCursor(SDL_ENABLE);
	bEnableCursor = true;
}

void DisableCursor()
{
	bEnableCursor = false;
	SDL_ShowCursor(SDL_DISABLE);
}

const stCursor *cursorAnt = NULL;
gedString	cursorAntName;
void DrawCursor(KrEngine *engine, const stCursor *cursor, int x, int y, bool bImediateDraw)
{
	//Emulate animated cursor (sprite)
	

	//static stCursor *cursorAnt = NULL;
	//static gedString	cursorAntName;

	if(!cursor)
	{
		//if(!cursorAnt)
		{
			//Enable default cursor
			if(bEnableCursor) 
			{
				KrRect rect;
				rect.Set(x, y, x, y);
				rect.EdgeAdd(8);

				//Solve the bud "only update mouse in full screen if the background is moving"
				engine->InvalidateRectangle(rect); 
				SDL_ShowCursor(SDL_ENABLE);
			}
			
			if(cursorAnt)
			{
				KrImNode *cursorNode = engine->Tree()->FindNodeById(cursorAntName.GetFileName().getHash());
				if(cursorNode) cursorNode->SetVisible(false);
			}

			cursorAnt = NULL;
			return;
		}

		cursor = cursorAnt;
	}

	KrSpriteResource *resource;
	KrSprite *sprite;

#ifndef STAND_ALONE_GAME
	//Remove the old resource with the same name, if any, to avoid problem when edit the cursos attributes
	KrImNode *p = engine->Tree()->FindNodeById(cursor->imgName.GetFileName().getHash());
	if(p)
	{
		engine->Tree()->DeleteNode(p);
	}

	KrSpriteResource *res = engine->Vault()->GetSpriteResource(cursor->imgName);
	if(res)
	{
		engine->Vault()->RemoveSpriteResource(res);
	}	
	
#endif

	if((resource = engine->Vault()->GetSpriteResource(cursor->imgName)) == NULL)
	{	
#ifndef STAND_ALONE_GAME
		resource = LoadTiledSprite(cursor->imgName, cursor->imgName, cursor->nFramesH, cursor->nFramesV, NULL, NULL, cursor->hotSpotX, cursor->hotSpotY);
		if(!resource)
		{
			SDL_ClearError(); //Ignore error
			return;
		}

		engine->Vault()->AddResource(resource);
#else
		return;
#endif
	}


	SDL_ShowCursor(SDL_DISABLE);


	sprite = (KrSprite *)engine->Tree()->FindNodeById(cursor->imgName.GetFileName().getHash());	
	if(!sprite)
	{
		sprite = new KrSprite( resource );	
		sprite->SetNodeId(cursor->imgName.GetFileName().getHash());
		engine->Tree()->AddNode( 0, sprite );		
		sprite->SetZDepth(CURSOR_DEPTH);
		sprite->SetHitTest(false);
	}

	sprite->SetPos(x, y);

	if(cursor->imgName != cursorAntName) 
	{
		KrImNode *p = engine->Tree()->FindNodeById(cursorAntName.GetFileName().getHash());
		if(p) p->SetVisible(false);		
	}

	if(bEnableCursor) sprite->SetVisible(true);
	else sprite->SetVisible(false);


	if(cursor->bAnimate)
		sprite->DoStep();

	if(bImediateDraw)
	{
		GlDynArray<KrRect> rect;

		rect.Insert(sprite->Bounds(), 0);
		engine->Draw(true, &rect);
		engine->InvalidateScreen(); //Really ?
	}

	cursorAnt =  cursor;
	cursorAntName = cursorAnt->imgName;
}




void *isSymbolInEic(const char *id)
{
    symentry_t *sym;
    for(sym = EiC_HTAB[hashsmc((char *)id,HSIZE)]; sym != NULL; sym = sym->next)
	{
		if(sym->id && strcmp(id,sym->id) == 0) 
		{
			return sym;
		}
	}
		
	return NULL;
}

int isValidVarName(const char *name)
{
	int lenght = strlen(name);

	if(!((name[0] >= 'a' && name[0] <= 'z') ||
	   (name[0] >= 'A' && name[0] <= 'Z')))
	   return VAR_NAME_INVALID_INTIAL_CHAR;

    int i;
	for(i = 0; i < lenght; i++)
	{
		if(!((name[i] >= 'a' && name[i] <= 'z') ||
			(name[i] >= 'A' && name[i] <= 'Z') ||
			(name[i] >= '0' && name[i] <= '9') ||
			name[i] == '_'))
			 return VAR_NAME_INVALID_CHAR;
	}

	
	if(EiC_isReservedWord(name)) 
		return VAR_NAME_IS_RESERVED_WORD;

	//Symbols
	if(isSymbolInEic(name))
		return VAR_NAME_IS_FUNCTION_OR_DECLARED_VAR;

	//Macros
	if(EiC_ismacroid((char *)name) != -1)
		return VAR_NAME_IS_MACRO;

	return VAR_NAME_OK;
}

void SplitPath(const gedString& fullPath, gedString& file, gedString& path)
{
	int i = fullPath.rfind('\\');
	if(i == gedString::npos)
		i = fullPath.rfind('/');

	file = fullPath.substr(i+1);
	if(fullPath.substr(0, 4) == "data")
	{
		//Relative to old game directory
		gedString gamePath(GameControl::Get()->getOldGamePath());

		if(gamePath.empty())
		{
			//Try from new game directory
			gamePath = GameControl::Get()->getGamePath();
		}

		if(!gamePath.empty())
		{
			path = gamePath + DIR_SEP + "data";
		}
		else
		{
			path = "data";
		}
	}
	else
	{
		if(i > 0)
		{
			path = fullPath.substr(0, i);
		}
		else
		{
			path.clear();
		}
	}
}

static unsigned long crc_table[256];
void gen_table()
{
	static bool bGen = false;
	if(bGen) return;

	unsigned long crc, poly;
    int i, j;

    poly = 0xEDB88320L;
    for (i = 0; i < 256; i++)
    {
	    crc = i;
	    for (j = 8; j > 0; j--)
	    {
	        if (crc & 1)
	            crc = (crc >> 1) ^ poly;
	        else
	            crc >>= 1;
	    }
	    crc_table[i] = crc;
    }

	bGen = true;
}

U32 Crc32(void * seq, long size)
{
	//CRC32
	gen_table();

	register U32 crc;

    crc = 0xFFFFFFFF;
	for(long i=0; i < size; i++)
		crc = ((crc>>8) & 0x00FFFFFF) ^ crc_table[ (crc^(((unsigned char *)(seq))[i])) & 0xFF ];
	
	register U32 res = crc^0xFFFFFFFF;

	return res;	
}

U32 FileCrc32(const char *filename, int startOffSet)
{
	U32 crc = 0;

	SDL_RWops *arq = ged_SDL_RWFromFile(filename, "rb");

	if(arq)
	{
		SDL_RWseek( arq, 0, SEEK_END ); //tell error without this
		U32 size = SDL_RWtell( arq );
		
		if(size > startOffSet)
		{
			SDL_RWseek( arq, startOffSet, SEEK_SET );
			size -= startOffSet;
			
			char *buf = new char[size];
			if(buf)
			{
				SDL_RWread(arq, buf, size, 1);
				crc = Crc32(buf, size);
				
				delete [] buf;
			}
		}

		SDL_RWclose(arq);
	}

	return crc;
}

#ifndef STAND_ALONE_GAME
KrSpriteResource *LoadTiledSprite(gedString imgName, gedString spriteName, int nx, int ny, KrSpriteResource **sr, char *action, int hotSpotX, int hotSpotY)
{
	/*
	Load multiple sprites (nx*ny same size sprites)
	If sr and action specified, add new action to sprite
	*/

	KrRGBA rgba;
	rgba.c.red = 0;
	rgba.c.green = 0;
	rgba.c.blue = 0;

	SDL_Surface *surface = LoadSurface( imgName.c_str()/*, rgba*/ );
	if(!surface)
		return NULL;

	KrSpriteResource *spriteResource;
	
	if(sr && *sr)
		spriteResource = *sr;
	else
		spriteResource = new KrSpriteResource(spriteName);
	
	if ( !spriteResource->GetAction( action?action:"NONE" ) )
	{
		spriteResource->AddAction( new KrAction( action?action:"NONE" ) );
	}

	KrAction* actionRes = spriteResource->GetAction( action?action:"NONE" );

	if(nx < 1) nx = 1;
	if(ny < 1) ny = 1;

	int dx = surface->w / nx;
	int dy = surface->h / ny;
	
	for(int j = 0; j < ny; j++)
	{
		for(int i = 0; i < nx; i++)
		{
			int hx = (hotSpotX != -1)?hotSpotX:(dx/2);
			int hy = (hotSpotY != -1)?hotSpotY:(dy/2);
			int index = actionRes->NumFrames();
			actionRes->AddFrame();
			
			KrPaintInfo info( surface );

			//Set transparent color
			KrPainter painter( surface );
			KrRGBA transRgba;
			painter.BreakPixel( 0, 0, &transRgba );	
			info.SetTranparentColor(transRgba);

			KrRle* rle = actionRes->GetFrame( index );			
			
			rle->Create( &info, i*dx, j*dy, dx, dy, i*dx + hx, j*dy + hy, 0, 0);
		}
	}

	SDL_FreeSurface( surface );

	if(sr)
		*sr = spriteResource;

	return spriteResource;
}

bool copy(SDL_RWops *from, SDL_RWops *to)
{
	int n;
	char *buf = (char *)dlmalloc(10*1024);
	
	while((n = SDL_RWread(from, buf, 1, 10*1024)) > 0)
	{
		if(SDL_RWwrite(to, buf, 1, n) != n)
		{
			dlfree(buf);
			return false;
		}
	}

	dlfree(buf);
	return true;
}

bool copy(const char *fromFileName, SDL_RWops *to)
{
	SDL_RWops *from = ged_SDL_RWFromFile(fromFileName, "rb");

	if(!from)
		return false;

	bool res = copy(from, to);
	SDL_RWclose(from);

	return res;
}

bool copy(const char *fromFileName, const char *toFileName)
{
	SDL_RWops *to = ged_SDL_RWFromFile(toFileName, "wb");

	if(!to)
		return false;

	bool res = copy(fromFileName, to);
	SDL_RWclose(to);

	if(!res)
	{
		//Remove to file
		remove(toFileName);
	}

	return res;
}

bool copyfile(const char *oldFrom, const char *oldTo, const char *oldFPath, bool overwrite)
{
#ifndef __iPhone__
	//Copy multiple files

	char *from = (char *)dlmalloc(strlen(oldFrom) + 1);
	correctDirectoryName(oldFrom, from);

	char *to = (char *)dlmalloc(strlen(oldTo) + 1);
	correctDirectoryName(oldTo, to);

	char *fPath = (char *)dlmalloc(strlen(oldFPath) + 1);
	correctDirectoryName(oldFPath, fPath);

	char *buf = (char *)dlmalloc(10*1024);	

	//Multiple files
	//If copy image s*.bmp don't copy image sw*.bmp
	gedString imgName(from);
	int numberPos = -1;
	if(imgName.find("*") != gedString::npos && imgName.find("*.*") == gedString::npos)
	{
		//Find *
		numberPos = imgName.GetFileName().find('*');
	}
	
	
	long hnd = ged_findfirst(from, A_ANY);
	if(hnd != GED_NOTFOUND)
	{	
		do
		{
			if(!(ged_attrib(hnd) & A_DIREC))
			{
				gedString fromPath = fPath;
				gedString toPath = to;
				char *fileName = ged_filename(hnd), *name = NULL;

				if(numberPos >= 0)
				{
					name = fileName + numberPos;
				}

				//Add only the files in same sequence
				if(name && !isdigit(*name))
				{
					continue;			
				}
				
				fromPath = fromPath + DIR_SEP + fileName;
				toPath = toPath + DIR_SEP + fileName;
				if(!overwrite)
				{
					int temp = open(toPath.c_str(),  O_RDONLY | O_BINARY);
					if(temp != -1)
					{
						close(temp);
						continue;
					}
				}
				
				int src = open(fromPath.c_str(),
#ifndef __iPhone__
				 O_RDONLY |
#endif
				 O_BINARY);
				if(src == -1) 
				{
#ifdef _DEBUG
					GLOUTPUT("copyfile error: can't open '%s'\n", fromPath.c_str());
#endif

					dlfree(from);
					dlfree(to);
					dlfree(fPath);
					dlfree(buf);
					continue;
				}
				
#ifdef WIN32
				int dst = open(toPath.c_str(),  O_CREAT | O_WRONLY | O_BINARY);
#else
				int dst = open(toPath.c_str(),  O_CREAT | O_WRONLY | O_BINARY, S_IRWXU | S_IRWXG | S_IRWXO);
#endif
				if(dst == -1)
				{
#ifdef _DEBUG
					GLOUTPUT("copyfile error: can't open '%s'\n", toPath.c_str());
#endif
					close(src);
					
					dlfree(from);
					dlfree(to);
					dlfree(fPath);
					dlfree(buf);
					continue;
				}
				
				int n;
				
				while((n = read(src, buf, 10*1024)) > 0)
				{
					if(write(dst, buf, n) != n)
					{
#ifdef _DEBUG
						GLOUTPUT("copyfile error: write error '%s'\n", toPath.c_str());
#endif
						close(src);
						close(dst);
						remove(toPath.c_str());
						
						dlfree(from);
						dlfree(to);
						dlfree(fPath);
						dlfree(buf);
						return false;
					}
				}
				
				close(src);
				close(dst);
			}
			
		} while((hnd = ged_findnext(hnd)) != GED_NOTFOUND);
		
		ged_findclose(hnd);
	}
#ifdef _DEBUG
	else
	{
		GLOUTPUT("copyfile error: file not found '%s'\n", from);
	}
#endif
	
	dlfree(from);
	dlfree(to);
	dlfree(fPath);
	dlfree(buf);
#endif
	return true;
}

bool concatenateFile(const char *oldFirst, const char *oldSecond)
{
#ifndef __APPLE__
	char *first = (char *)dlmalloc(strlen(oldFirst) + 1);
	correctDirectoryName(oldFirst, first);

	char *second = (char *)dlmalloc(strlen(oldSecond) + 1);
	correctDirectoryName(oldSecond, second);


	int secondFile = open(second,  O_RDONLY | O_BINARY);
	if(secondFile == -1) 
	{
		dlfree(first);
		dlfree(second);
		return false;
	}
	
	int firstFile = open(first,  O_WRONLY | O_APPEND | O_BINARY);
	if(firstFile == -1)
	{		
		close(secondFile);

		dlfree(first);
		dlfree(second);
		return false;
	}
	
	char *buf = (char *)dlmalloc(10*1024);
	int n;
	
	while((n = read(secondFile, buf, 10*1024)) > 0)
	{
		if(write(firstFile, buf, n) != n)
		{
			close(firstFile);
			close(secondFile);

			dlfree(first);
			dlfree(second);
			dlfree(buf);
			return false;
		}
	}
	
	close(firstFile);
	close(secondFile);		
	
	dlfree(first);
	dlfree(second);
	dlfree(buf);
#endif
	return true;
}

SDL_RWops *LoadToMemoryFile(gedString fileName)
{
	SDL_RWops *src = ged_SDL_RWFromFile(fileName.c_str(), "rb"), *memFile = NULL;

	if(src)
	{
		memFile = SDL_RWFromMem(NULL, 0);

		if(memFile)
		{
			if(!copy(src, memFile))
			{
				dlfree(memFile->hidden.mem.base);
				SDL_RWclose(memFile);
			}
		}

		SDL_RWclose(src);
	}

	return memFile;
}

bool IsSolidImage(SDL_Surface *surface, KrRGBA& transColor)
{
	if(surface->format->BytesPerPixel == 4)
	{
		//Get transparent from alpha on 32 bits image
		return false;
	}

	KrPainter painter( surface );
	KrRGBA firstColor;
	painter.BreakPixel( 0, 0, &firstColor );

	for(int x = 0; x < surface->w; x++)
	{
		for(int y = 0; y < surface->h; y++)
		{
			KrRGBA color;
			painter.BreakPixel(x, y, &color);

			if(color != firstColor)
			{
				return false;
			}
		}
	}

	//Set a transparent color for the solid image
	transColor = firstColor;
	transColor.c.red = 255 - transColor.c.red;
	return true;
}

static Uint32 getpixel(SDL_Surface * surface, int x, int y)
{
  int bpp;
  Uint8 * p;
  Uint32 pixel;

  pixel = 0;


  /* Assuming the X/Y values are within the bounds of this surface... */

  if (x >= 0 && y >= 0 && x < surface -> w && y < surface -> h)
    {
      /* SDL_LockSurface(surface); */


      /* Determine bytes-per-pixel for the surface in question: */

      bpp = surface->format->BytesPerPixel;


      /* Set a pointer to the exact location in memory of the pixel
         in question: */

      p = (Uint8 *) (((Uint8 *)surface->pixels) +  /* Start at top of RAM */
		     (y * surface->pitch) +  /* Go down Y lines */
		     (x * bpp));             /* Go in X pixels */


      /* Return the correctly-sized piece of data containing the
       * pixel's value (an 8-bit palette value, or a 16-, 24- or 32-bit
       * RGB value) */

      if (bpp == 1)         /* 8-bit display */
        pixel = *p;
      else if (bpp == 2)    /* 16-bit display */
        pixel = *(Uint16 *)p;
      else if (bpp == 3)    /* 24-bit display */
        {
          /* Depending on the byte-order, it could be stored RGB or BGR! */

          if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            pixel = p[0] << 16 | p[1] << 8 | p[2];
          else
            pixel = p[0] | p[1] << 8 | p[2] << 16;
        }
      else if (bpp == 4)    /* 32-bit display */
        pixel = *(Uint32 *)p;

      /* SDL_UnlockSurface(surface); */
    }

  return pixel;
}

/* Write PNG image data to an SDL rwops */
static void sgWritePngData(png_structp ctx, png_bytep area, png_size_t size)
{
  SDL_RWops *rwops;
  
  rwops = (SDL_RWops *)png_get_io_ptr(ctx);
  SDL_RWwrite(rwops, area, size, 1);
}

int SDL_SavePNG_RW(SDL_Surface *surface, SDL_RWops *dst, int freedst)
{
	//Save 24 oe 32 bit images

	png_structp png_ptr;
	png_infop info_ptr;
	png_text text_ptr[4];
	unsigned char ** png_rows;
	Uint8 r, g, b, a;
	int x, y, count;

	if(!dst || !surface || surface->format->BitsPerPixel < 24)
	{
		if(freedst && dst) SDL_RWclose(dst);
		return -1;
	}
	
	
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
    {
		png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
		
		SDL_SetError("Couldn't save the image!");      
    }
	else
    {
		png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == NULL)
		{
			png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
			
			SDL_SetError("Couldn't save the image!");
		}
		else
		{
			if (setjmp(png_jmpbuf(png_ptr)))
			{
				png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
				
				SDL_SetError("Couldn't save the image!");
				
				if(freedst) SDL_RWclose(dst);
				return -1;
			}
			else
			{
				/* Set up the output control */
				png_set_write_fn(png_ptr, dst, sgWritePngData, NULL);
				
				info_ptr->width = surface->w;
				info_ptr->height = surface->h;
				info_ptr->bit_depth = 8;
				info_ptr->color_type = (surface->format->BitsPerPixel == 24)?PNG_COLOR_TYPE_RGB:PNG_COLOR_TYPE_RGBA;
				info_ptr->interlace_type = 0;
				info_ptr->valid = 0;
				
				
				/* Set headers */
				
				count = 0;
				
				/*
				if (title != NULL && strlen(title) > 0)
				{
				text_ptr[count].key = "Title";
				text_ptr[count].text = title;
				text_ptr[count].compression = PNG_TEXT_COMPRESSION_NONE;
				count++;
				}
				*/
				
				text_ptr[count].key = "Game Editor";
				text_ptr[count].text = "";
				text_ptr[count].compression = PNG_TEXT_COMPRESSION_NONE;
				count++;
				
				
				png_set_text(png_ptr, info_ptr, text_ptr, count);
				
				png_write_info(png_ptr, info_ptr);
				
				
				
				/* Save the picture: */
				
				png_rows = (unsigned char **)malloc(sizeof(char *) * surface->h);
				
				if(surface->format->BytesPerPixel == 3)
				{
					//Save 24 bits
					for (y = 0; y < surface->h; y++)
					{
						png_rows[y] = (unsigned char *)malloc(sizeof(char) * 3 * surface->w);
						
						for (x = 0; x < surface->w; x++)
						{
							SDL_GetRGB(getpixel(surface, x, y), surface->format, &r, &g, &b);
							
							png_rows[y][x * 3 + 0] = r;
							png_rows[y][x * 3 + 1] = g;
							png_rows[y][x * 3 + 2] = b;
						}
					}
				}
				else
				{
					//Save 32 bits
					for (y = 0; y < surface->h; y++)
					{
						png_rows[y] = (unsigned char *)malloc(sizeof(char) * 4 * surface->w);
						
						for (x = 0; x < surface->w; x++)
						{
							SDL_GetRGBA(getpixel(surface, x, y), surface->format, &r, &g, &b, &a);
							
							png_rows[y][x * 4 + 0] = r;
							png_rows[y][x * 4 + 1] = g;
							png_rows[y][x * 4 + 2] = b;
							png_rows[y][x * 4 + 3] = a;
						}
					}
				}

				//Error: RGB = BGR
				/*for(y = 0; y < surface->h; y++)
				{
					png_rows[y] = (png_bytep)
					(Uint8 *)surface->pixels + y*surface->pitch;
				}*/
				
				png_write_image(png_ptr, png_rows);
				
				for (y = 0; y < surface->h; y++)
					free(png_rows[y]);
				
				free(png_rows);
				
				
				png_write_end(png_ptr, NULL);
				
				png_destroy_write_struct(&png_ptr, &info_ptr);
				
				if(freedst) SDL_RWclose(dst);
				return 0;
			}
		}
    }
	
	if(freedst) SDL_RWclose(dst);
	return -1;
}

SDL_Surface *GetSDLSurface(Actor *actor, int w, int h, int frame, U32 colorKey)
{
	KrRle &rle = actor->getSprite()->GetAction()->Frame(frame);

	int width = rle.Width(),
		height = rle.Height();

	if(w == -1) w = width;
	if(h == -1) h = height;

	//Same format of video device
	SDL_Surface* surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
		w,
		h,
		engine->Surface()->format->BitsPerPixel,
		engine->Surface()->format->Rmask,	
		engine->Surface()->format->Gmask,
		engine->Surface()->format->Bmask,
		engine->Surface()->format->Amask );
	
	
	if(surface)
	{
		double scale = 1.0;
		
		if(width != -1 || height != -1)
		{
			scale = surface->w/(double)((width>height)?width:height);
		}

		SDL_FillRect(surface, 0, colorKey); 
		
		KrPaintInfo paint(surface);
		KrMatrix2 matrix;
		KrColorTransform cForm;
		KrRect clip;		
		KrVector2 hotSpot(rle.Hotspot());
		
		matrix.Set(hotSpot.x*scale, hotSpot.y*scale, scale, scale);
		clip.Set(0, 0, surface->w-1, surface->h-1);
		
		rle.Draw(&paint, matrix, cForm, clip, 0, true, true);			
	}
	
	return surface;
}



int SDL_SavePNG(SDL_Surface *surface, const char *file)
{
	return SDL_SavePNG_RW(surface, ged_SDL_RWFromFile(file, "wb"), 1);
}

long GetFileWriteTime(gedString file)
{
	long handle = ged_findfirst(file.c_str(), A_ANY), writeTime = 0;

	if(handle != GED_NOTFOUND)
	{	
		writeTime = ged_writetime(handle);

		ged_findclose(handle);
	}

	return writeTime;
}

void ExtractSoundPath(gedString code, gedString function, ListSounds& list)
{
	int i;
	while((i = code.find(function)) != gedString::npos)
	{
		int i1 = code.find('\"', i);
		if(i1 != gedString::npos)
		{
			//Have strings?
			//Skip (function(variable); function1("string"))
			int i0 = code.find(')', i);
			if(i0 != gedString::npos &&  i1 < i0)
			{				
				int i2 = code.find('\"', i1+1) - 1;
				
				if(i2 > i1)
				{
					gedString path(code.substr(i1+1, i2 - i1));
					
					char *newName = (char *)dlmalloc(path.length() + 1);
					correctDirectoryName(path.c_str(), newName);
					
					list.PushBack(stSoundFile(newName, function == "PlayMusic"));
					dlfree(newName);			
				}
			}
		}
		
		code = code.substr(i + function.length(), code.length() - i + 1);
	}
}


void SaveSoundList(const ListSounds &list, SDL_RWops *exeFile, MapAudioFileIndex &audioIndex)
{
	ListSoundsIterator it(list);

	for(it.Begin(); !it.Done(); it.Next())
	{
		if(!audioIndex.FindString(it.Current().path.c_str()))
		{
			//Get start audio position
			SDL_RWseek( exeFile, 0, SEEK_END ); //tell error without this
			Uint32 startAudioPos = SDL_RWtell( exeFile ), size = 0;
			
			if(it.Current().bMusic)
			{
				//Try in relative gamepath
				if(!copy((GameControl::Get()->getGamePath() + DIR_SEP + it.Current().path).c_str(), exeFile))
				{
					//Try in absolute path
					copy(it.Current().path.c_str(), exeFile);
				}
			}
			else
			{
				//Compress data
				Compression comp;
				comp.DoCompression((GameControl::Get()->getGamePath() + DIR_SEP + it.Current().path).c_str(), exeFile);
			}
			
			//Get audio size
			SDL_RWseek( exeFile, 0, SEEK_END ); //tell error without this
			size = SDL_RWtell( exeFile ) - startAudioPos;
			
			if(size > 0)
			{
				audioIndex.Add(it.Current().path.c_str(), stAudioIndex(startAudioPos, size));
			}
		}
	}
}

bool CanShowString(gedString s)
{
	/*  Valid string for a fixed font
		from a BMP file in a buffer. It assumes
		ascii: start 32, length 95.

		Update this function when support Unicode strings
	*/
#ifdef WIN32

	if(s.IsUnicode()) return false;

	for(int i = 0; i < s.length(); i++)
	{
		if(s.at(i) < 32 || s.at(i) > (32 + 95))
		{
			return false;
		}
	}

#endif
	

	return true;
}

#ifdef WIN32
extern "C" int CanShowStringW(TCHAR *s)
{
	/*  Valid string for a fixed font
		from a BMP file in a buffer. It assumes
		ascii: start 32, length 95.

		Update this function when support Unicode strings
	*/

	const TCHAR *p = s;
	
	while(*p) 
	{
		if(*p < 32 || *p > (32 + 95))
		{
			return 0;
		}

		++p;
	}	

	return 1;
}

#endif

#ifdef DEBUG
extern "C" void dPrintf(char *s) //Para imprimir debug na SDL
{
	GLOUTPUT(s);
}
#endif

#else //#ifndef STAND_ALONE_GAME //maks

#ifdef WIN32
extern "C" int CanShowStringW(TCHAR *s)
{
	//Don't need test in stand alone version
	return 1;
}
#endif

#endif //#ifndef STAND_ALONE_GAME //maks
















