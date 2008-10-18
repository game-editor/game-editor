/*--License:
	Kyra Sprite Engine
	Copyright Lee Thomason (Grinning Lizard Software) 2001-2002
	www.grinninglizard.com/kyra
	www.sourceforge.net/projects/kyra

	Kyra is provided under 2 licenses:

	- The GPL, with no additional restrictions.
	- The LGPL, provided you display the Kyra splash screen, described below.


--- GPL License --
	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

	The full text of the license can be found in license.txt


--- LGPL License --
  **Provided you kindly display the Kyra splash screen (details below), 
	you	may use the LGPL license:**

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	The full text of the license can be found in lgpl.txt


--- Kyra Splash Screen.

	It would be appreciate if you display the Kyra splash screen when using
	either license, however it is only required for the LGPL. All the
	resources for the splash are compiled into the library, and it can be
	accessed through the following API:

		KrEngine::StartSplash
		KrEngine::UpdateSplash
		KrEngine::EndSplash

	Full documentation is provided with the KrEngine class. The splash screen
	should be displayed for 2 seconds.

	Thank you.
*/


#ifndef IO_COMPRESSOR_INCLUDED
#define IO_COMPRESSOR_INCLUDED

#define RLE_WRITE

#include "SDL.h"
#include "SDL_rwops.h"

#include "../util/glutil.h"
#include "../util/gllist.h"
#include "../util/gldynarray.h"
#include "../util/glstring.h"
#include "../util/glmap.h" //maks

#include "rle.h"
#include "kyraresource.h"


typedef GlMap< gedString, char, GlStringHash >	MapIgnoredResources; //maks

class KrResource;
class KrSpriteResource;
class KrTileResource;
class KrFontResource;

/**	The vault is a container for resources. The sequence of
	steps generally followed is:
	
	- A vault is created and resources are read from a stream. 
	- Nhe engine is created, used, and destroyed.
	- The vault is deleted.

	It would be bad to delete resources the engine is using.

	Note that the engine has a "default vault" Engine()->Vault() 
	that it manages for you. It is usually simpler to use that
	Vault rather than manage your own.
*/

//Maks
class stResourceInfo
{
public:
	stResourceInfo() {type = -1;}
		
	stResourceInfo(const gedString& _name, int _type) 
	{ 
		name = _name;
		type = _type;
	}


	void operator=( const stResourceInfo& copy ) 
	{ 
		name = copy.name;
		type = copy.type;		
	}

	bool operator==( const stResourceInfo& copy ) 
	{ 
		return	name == copy.name && 
				type == copy.type;
	}
	

	gedString name;
	int type;
};

class stResourceInfoHash : public GlHash
{
  public:
	stResourceInfoHash( const stResourceInfo& col )
	{	
		val = 0;
		for ( int i=0; i<col.name.length() && i < 32; ++i )
		{
			val <<= 1;
			val |= col.name[i];
		}

		val ^= col.type;
	}
};

typedef GlMap< stResourceInfo, KrResource *, stResourceInfoHash >		MapResNamePointer;
typedef GlMapIterator< stResourceInfo, KrResource *, stResourceInfoHash >		MapResNamePointerIterator;

typedef GlMap< KrResource *, gedString, GlPointerHash<KrResource *> >		MapResPointerName;
typedef GlMapIterator< KrResource *, gedString, GlPointerHash<KrResource *> >		MapResPointerNameIterator;



class KrResourceVault
{
  public:
	static void IgnoreResource(const gedString& resourceName); //maks
	bool isIndexed() {return indexedData != NULL;} //maks
	SDL_RWops *getIndexedData() {return indexedData;} //maks
	static Uint32 getVersionDat() {return versionDat;} //maks
	static void resetVersionDat() {versionDat = 0;} //maks
	KrResource *LoadIndexedResource(const gedString& resourceName); //maks
	void RemoveSpriteResource(KrSpriteResource *res); //maks
	void RemoveCanvasResource(KrCanvasResource *res); //maks


	void RemoveBinaryDataResource(KrBinaryDataResource *res); //maks
	KrCanvasResource*	GetCanvasResource( const gedString& name ); //maks
	bool SaveDatFile( const char* fileName, bool bSaveIndexed = true ); //maks
	bool SaveDatFile( SDL_RWops* src, bool bSaveIndexed = true, bool bCloseFile = true); //maks

	KrResourceVault()			{ memoryPoolRGBA = 0; memoryPoolLine = 0; memoryPoolSegment = 0; indexedData = 0; versionDat = 0;} //maks
	virtual ~KrResourceVault();

	/**	Load in a .dat file from disk. Only one dat files can be
		loaded into a vault; but the engine can use multiple vaults.
	*/
	bool LoadDatFile( const char* fileName, bool bFailIfDifferentVersion = false );

	/**	Load in a .dat file from disk. Only one dat files can be
		loaded into a vault; but the engine can use multiple vaults.
	*/
	bool LoadDatFileFromMemory( void* memory, int size );

	/// How many resources are in the vault?
	int ResourceCount();
	/** Get an interator that can walk all the resources in this vault.
		Useful if you want to list resources, for instance if making
		an editor or allowing the user to choose between available fonts.
	*/
	MapResNamePointer& GetResourceMap(); //maks

	/// Add user created resources - generally canvases. Will be owned and deleted by the vault.
	void AddResource( KrResource* resource );

	/// Return a pointer to a resource. Null if the resource isn't found.
	//KrResource* GetResource( const gedString& type, gedString name ); //maks
	/// Return a pointer to a resource. Null if the resource isn't found.
	KrResource* GetResource( U32 type, const gedString &name ); //maks
	/// Return a pointer to a resource. Null if the resource isn't found.
	//KrResource* GetResource( U32 type, U32 resourceId );

	/// Return a pointer to a resource. Null if the resource isn't found.
	KrSpriteResource*	GetSpriteResource( const gedString& name );
	/// Return a pointer to a resource. Null if the resource isn't found.
	//KrSpriteResource*	GetSpriteResource( U32 resourceId );

	/// Return a pointer to a resource. Null if the resource isn't found.
	KrTileResource*		GetTileResource( const gedString& name );
	/// Return a pointer to a resource. Null if the resource isn't found.
	//KrTileResource*		GetTileResource( U32 resourceId );

	/// Return a pointer to a resource. Null if the resource isn't found.
	KrFontResource*		GetFontResource( const gedString& name );
	/// Return a pointer to a resource. Null if the resource isn't found.
	//KrFontResource*		GetFontResource( U32 resourceId );

	/// Return a pointer to a resource. Null if the resource isn't found.
	KrTextDataResource*		GetTextDataResource( const gedString& name );
	/// Return a pointer to a resource. Null if the resource isn't found.
	//KrTextDataResource*		GetTextDataResource( U32 resourceId );

	/// Return a pointer to a resource. Null if the resource isn't found.
	KrBinaryDataResource*	GetBinaryDataResource( const gedString& name );
	/// Return a pointer to a resource. Null if the resource isn't found.
	//KrBinaryDataResource*	GetBinaryDataResource( U32 resourceId );

	/// For cached scaling, cache all the resources in this vault.
	virtual void CacheScale( GlFixed xScale, GlFixed yScale );
	/// For cached scaling, free all the scale caches in this vault.
	virtual void FreeScaleCache();

	bool LoadDatFile( SDL_RWops* stream, bool bFailIfDifferentVersion = false ); //maks
	bool ExistsResource(KrResource *res); //maks
  private:
	//bool LoadDatFile( SDL_RWops* stream );

	
	GlMap< gedString, Uint32, GlStringHash >	resourceMap; //maks
	SDL_RWops* indexedData; //maks

	// The vault is the owner of these memory allocators:
	GlLinearMemoryPool*	memoryPoolRGBA;
	GlLinearMemoryPool*	memoryPoolLine;
	GlLinearMemoryPool*	memoryPoolSegment;

	static Uint32 versionDat; //maks
	static MapIgnoredResources mapIgnoredResources; //maks


	//Maks: this maps must be syncronized
	//GlSList< KrResource* > resList; //maks: inflate the cache size. Use the maps below

	MapResNamePointer	resMapName; //maks
	MapResPointerName	resMapPointer; //maks
};
	
	

#endif
