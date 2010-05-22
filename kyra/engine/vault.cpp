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

#ifndef _WIN32_WCE //maks
#	include <time.h>
#endif

#include "SDL.h"
#include "vault.h"
#include "SDL_rwops.h"
#include "SDL_endian.h"
#include "sdlutil.h"
#include "pixelblock.h"
#include "../util/glperformance.h"
#include "../engine/spriteresource.h" //maks
#include "../engine/canvasresource.h" //maks
#include "../engine/dataresource.h" //maks
#include "encoder.h" //maks
#include "../../gameEngine/compression.h" //maks


Uint32 KrResourceVault::versionDat = 0; //maks
MapIgnoredResources KrResourceVault::mapIgnoredResources; //maks

KrResourceVault::~KrResourceVault()
{
	/*while( !resList.Empty() )
	{
		KrResource* resource = resList.Front();
		delete resource;
		resList.PopFront();
	}*/

	MapResNamePointerIterator it(resMapName);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		delete *it.Value();
	}
	resMapName.Clear();
	resMapPointer.Clear();

	if(indexedData)
		SDL_RWclose(indexedData);

	delete memoryPoolLine;
	delete memoryPoolRGBA;
	delete memoryPoolSegment;

	mapIgnoredResources.Clear();
}


bool KrResourceVault::LoadDatFile( const char* fileName, bool bFailIfDifferentVersion )
{
	//GLOUTPUT( "LoadDatFile(filename) called\n" );
	SDL_RWops* data = SDL_RWFromFile( fileName, "rb" );
	if ( !data )
	{
		#ifdef DEBUG
			GLOUTPUT(	"KrResourceVault::LoadDatFile failed to open file '%s'\n",
						fileName );
		#endif
		return false;
	}
	bool success = LoadDatFile( data, bFailIfDifferentVersion );
	if(!indexedData) SDL_RWclose( data ); //maks
	//SDL_FreeRW( data );;
	return success;
}


bool KrResourceVault::LoadDatFileFromMemory( void* memory, int size )
{
	SDL_RWops* data = SDL_RWFromMem( memory, size );
	if ( !data )
	{
		#ifdef DEBUG
			GLOUTPUT(( "KrResourceVault::LoadDatFileFromMemory failed.\n" ));
		#endif
		return false;
	}
	bool success = LoadDatFile( data );
	if(!indexedData) SDL_RWclose( data );
	//SDL_FreeRW( data );;
	return success;
}

bool KrResourceVault::LoadDatFile( SDL_RWops* data, bool bFailIfDifferentVersion )
{
#ifdef COUNT_PERFORMANCE //maks
	GlPerformance perf( "LoadDatFileFromMemory" );
#endif

 	char magic[ 5 ];
	gedString version;
	bool ret = false;

	//GLOUTPUT( "LoadDatFile called\n" );
	// Can only load 1 dat file.
	/*GLASSERT( memoryPoolRGBA == 0 );
	GLASSERT( memoryPoolLine == 0 );
	GLASSERT( memoryPoolSegment == 0 );*/

	KrResourceFactory* factory = KrResourceFactory::Instance();
	GLASSERT( factory );

	if(indexedData) //maks
	{
		SDL_RWclose(indexedData);
		indexedData = NULL;
		resourceMap.Clear();
	}

	if ( data && factory )
	{
		// Read the magic and the version.
		SDL_RWread( data, magic, 4, 1 );
		magic[4] = 0;

		/*GLASSERT( magic[0] == 'K' ); //maks
		GLASSERT( magic[1] == 'Y' );
		GLASSERT( magic[2] == 'R' );
		GLASSERT( magic[3] == 'A' );*/

		bool bIndexed = false; //maks
		if(strcmp(magic, "GEDI") == 0)
			bIndexed = true;
		else if(strcmp(magic, "KYRA") != 0) 
			return false;

		SetNewReadString(false);
		ReadString( data, &version );
		versionDat = 0;

		if(bFailIfDifferentVersion && version != "2.1.5")
		{
			return false;
		}

		//Set new style ReadString ?
		if(version == "2.1.1" || version == "2.1.2" || version == "2.1.3" || version == "2.1.4" || version == "2.1.5")
		{
			SetNewReadString(true);
			gedString::SetReadEncriptString(true);

			if(version == "2.1.2") versionDat = 2;
			else if(version == "2.1.3") versionDat = 3;
			else if(version == "2.1.4") versionDat = 4;
			else if(version == "2.1.5") versionDat = 5;
		}
		else 
		{
			gedString::SetReadEncriptString(false);
			if(version != "2.0.4" && version != "2.0.2") SetNewReadString(true);
			else SetNewReadString(false);
		}
		

		#ifdef DEBUG
			GLOUTPUT( "File magic=%s version=%s\n", magic, version.c_str() );
		#endif

		U32 numRGBA    = SDL_ReadLE32( data );
		U32 numLine    = SDL_ReadLE32( data );
		U32 numSegment = SDL_ReadLE32( data );

		#ifdef DEBUG
			GLOUTPUT( "Tally count: rgba=%d line=%d segment=%d\n", numRGBA, numLine, numSegment  );
		#endif

		if(bIndexed) //maks: Don't use memory pool in on demand sprite load
		{
			//Load index
			
			Uint32	datLenght = SDL_ReadLE32( data ),
					nResources = SDL_ReadLE32( data ), 
					index;

			gedString resourceName;
			for(Uint32 i = 0; i < nResources; i++)
			{
				ReadString(data, &resourceName);
				index = SDL_ReadLE32( data );
				resourceMap.Add(resourceName, index);
			}

			SDL_RWseek( data, datLenght, SEEK_SET ); //To .ged load
			indexedData = data;			
		}
#ifndef STAND_ALONE_GAME //maks
		else
		{
			//If already has a memory pool, will use memoryPool->Alloc
			if(!memoryPoolRGBA) memoryPoolRGBA    = new GlLinearMemoryPool( sizeof( KrRGBA ) * numRGBA );
			if(!memoryPoolLine) memoryPoolLine    = new GlLinearMemoryPool( sizeof( KrRleLine ) * numLine );
			if(!memoryPoolSegment) memoryPoolSegment = new GlLinearMemoryPool( sizeof( KrRleSegment ) * numSegment );
			
			KrRle::SetMemoryPools( memoryPoolRGBA, memoryPoolLine, memoryPoolSegment );
			
			U32 id = 0;
			U32 size = 0;
			U32 next = 0;
			
			for ( ;; )
			{
#ifdef DEBUG
				GLOUTPUT( "here=%d ", SDL_RWtell( data ) );
#endif
				id   = SDL_ReadLE32( data );
				size = SDL_ReadLE32( data );
				next = size + SDL_RWtell( data );
#ifdef DEBUG
				GLOUTPUT( "id=%d size=%d next=%d\n", id, size, next );
#endif
				
				if ( id == KYRATAG_END )
				{
					break;
				}
				else
				{
					KrResource* resource;
					resource = factory->Create( id, size, data );
					
					if ( resource )
					{
						//resList.PushBack( resource );

						stResourceInfo info(resource->ResourceName(), resource->Type());
						if(!resMapName.Find1(info))
						{
							resMapName.Add(info, resource);
							resMapPointer.Add(resource, resource->ResourceName());
						}
						else
						{
							//Don't duplicate resources
							delete resource;							
						}
					}
					else
					{
						break;
					}
					
				}
			}
			
			// We should be out of memory, since we calculated how much we need:
#ifdef DEBUG 
			GLOUTPUT( "Num RGBA: RLE: %d Block: %d total=%d\n",
				KrRleSegment::numRGBA, KrPixelBlock::numRGBA,
				KrRleSegment::numRGBA + KrPixelBlock::numRGBA );
			GLASSERT( KrRle::PoolOut() );
#endif
			
			//	KrRleSegment::SetMemoryPool( 0 );
			//	KrPixelBlock::SetMemoryPool( 0 );
			KrRle::SetMemoryPools( 0, 0, 0 );
		}
#endif

		
		ret = true;
	}
	delete factory;	

	return ret;
}


MapResNamePointer& KrResourceVault::GetResourceMap() //maks
{
	return resMapName;
}


int KrResourceVault::ResourceCount()
{
	return resMapName.size();
}


void KrResourceVault::AddResource( KrResource* resource )
{
	//resList.PushBack( resource );

	stResourceInfo info(resource->ResourceName(), resource->Type());
	resMapName.Add(info, resource);
	resMapPointer.Add(resource, resource->ResourceName());
}

#ifdef __0__
KrResource* KrResourceVault::GetResource( const gedString& type, gedString name )
{
	name = name.GetFileName();

	// OPT linear search
	/*GlSListIterator< KrResource* > it( resList );

	for( it.Begin(); !it.Done(); it.Next() )
	{
		if (	it.Current()->TypeName() == type
			 && it.Current()->ResourceName() == name )
		{
			return it.Current();
		}
	}*/

	KrResource **res = resMapName.Find1(name);
	if(res && *res)
		return *res;

	return LoadIndexedResource(name); //maks
}
#endif


KrResource* KrResourceVault::GetResource( U32 type, const gedString &name ) //maks
{

	// OPT linear search
	/*GlSListIterator< KrResource* > it( resList );

	for( it.Begin(); !it.Done(); it.Next() )
	{
		if (	it.Current()->Type() == type
			 && it.Current()->ResourceName() == name )
		{
			return it.Current();
		}
	}*/

	
	stResourceInfo info(name.GetFileName(), type);
	KrResource **res = resMapName.Find1(info);
	
	if(res && *res)
		return *res;

	
	return LoadIndexedResource(info.name); //maks
}



/*KrResource* KrResourceVault::GetResource( U32 type, U32 resid ) //maks
{
	// OPT linear search
	MapResNamePointerIterator it( resMapName );

	for( it.Begin(); !it.Done(); it.Next() )
	{
		if (	(*it.Value())->Type() == type
			 && (*it.Value())->ResourceId() == resid )
		{
			return *it.Value();
		}
	}

	return 0;
}*/


KrSpriteResource* KrResourceVault::GetSpriteResource( const gedString& name )
{
	KrResource* resource = GetResource( KYRATAG_SPRITE, name );
	if ( resource )
	{	
		GLASSERT( resource->ToSpriteResource() );
		return resource->ToSpriteResource();
	}
	return 0;
}

KrCanvasResource* KrResourceVault::GetCanvasResource( const gedString& name ) //maks
{
	KrResource* resource = GetResource( KYRATAG_CANVAS, name );
	if ( resource )
	{	
		GLASSERT( resource->ToCanvasResource() );
		return resource->ToCanvasResource();
	}
	return 0;
}

/*KrSpriteResource* KrResourceVault::GetSpriteResource( U32 id )
{
	KrResource* resource = GetResource( KYRATAG_SPRITE, id );
	if ( resource )
	{	
		GLASSERT( resource->ToSpriteResource() );
		return resource->ToSpriteResource();
	}
	return 0;
}*/


KrTileResource*	KrResourceVault::GetTileResource( const gedString& name )
{
	KrResource* resource = GetResource( KYRATAG_TILE, name );
	if ( resource )
	{	
		GLASSERT( resource->ToTileResource() );
		return resource->ToTileResource();
	}
	return 0;
}


/*KrTileResource*	KrResourceVault::GetTileResource( U32 id )
{
	KrResource* resource = GetResource( KYRATAG_TILE, id );
	if ( resource )
	{	
		GLASSERT( resource->ToTileResource() );
		return resource->ToTileResource();
	}
	return 0;
}*/

KrFontResource*	KrResourceVault::GetFontResource( const gedString& name )
{	
	KrResource* resource = GetResource( KYRATAG_FONT, name );
	
	if ( resource )
	{	
		GLASSERT( resource->ToFontResource() );
		return resource->ToFontResource();
	}
	return 0;
}


/*KrFontResource*	KrResourceVault::GetFontResource( U32 id )
{
	KrResource* resource = GetResource( KYRATAG_FONT, id );
	if ( resource )
	{	
		GLASSERT( resource->ToFontResource() );
		return resource->ToFontResource();
	}
	return 0;
}*/


/*KrTextDataResource*	KrResourceVault::GetTextDataResource( U32 id )
{
	KrResource* resource = GetResource( KYRATAG_TEXTDATA, id );
	if ( resource )
	{	
		GLASSERT( resource->ToTextDataResource() );
		return resource->ToTextDataResource();
	}
	return 0;
}*/

KrTextDataResource*	KrResourceVault::GetTextDataResource( const gedString& name )
{
	KrResource* resource = GetResource( KYRATAG_TEXTDATA, name );
	if ( resource )
	{	
		GLASSERT( resource->ToTextDataResource() );
		return resource->ToTextDataResource();
	}
	return 0;
}

/*KrBinaryDataResource* KrResourceVault::GetBinaryDataResource( U32 id )
{
	KrResource* resource = GetResource( KYRATAG_BINARYDATA, id );
	if ( resource )
	{	
		GLASSERT( resource->ToBinaryDataResource() );
		return resource->ToBinaryDataResource();
	}
	return 0;
}*/

KrBinaryDataResource* KrResourceVault::GetBinaryDataResource( const gedString& name )
{
	KrResource* resource = GetResource( KYRATAG_BINARYDATA, name );
	if ( resource )
	{	
		GLASSERT( resource->ToBinaryDataResource() );
		return resource->ToBinaryDataResource();
	}
	return 0;
}


void KrResourceVault::CacheScale( GlFixed xScale, GlFixed yScale ) //maks
{
	MapResNamePointerIterator it( resMapName );

	for( it.Begin(); !it.Done(); it.Next() )
	{
		(*it.Value())->CacheScale( xScale, yScale );
	}
}


void KrResourceVault::FreeScaleCache()
{
	MapResNamePointerIterator it( resMapName );

	for( it.Begin(); !it.Done(); it.Next() ) //maks
	{
		(*it.Value())->FreeScaleCache();
	}
}

bool KrResourceVault::ExistsResource(KrResource *res) //maks
{
	return resMapPointer.Find1(res) != NULL;	
}

void KrResourceVault::RemoveSpriteResource(KrSpriteResource *res) //maks
{
	if(resMapPointer.Find1((KrResource *)res))
	{
		stResourceInfo info(res->ResourceName(), res->Type());
		resMapName.Remove(info);
		resMapPointer.Remove((KrResource *)res);
		delete res;
	}
}


void KrResourceVault::RemoveCanvasResource(KrCanvasResource *res) //maks
{
	if(resMapPointer.Find1((KrResource *)res))
	{
		stResourceInfo info(res->ResourceName(), res->Type());
		resMapName.Remove(info);
		resMapPointer.Remove((KrResource *)res);
		delete res;
	}
}


void KrResourceVault::RemoveBinaryDataResource(KrBinaryDataResource *res) //maks
{
	if(resMapPointer.Find1((KrResource *)res))
	{
		stResourceInfo info(res->ResourceName(), res->Type());
		resMapName.Remove(info);
		resMapPointer.Remove((KrResource *)res);
		delete res;
	}
}

bool KrResourceVault::SaveDatFile( const char* fileName, bool bSaveIndexed ) //maks
{
	SDL_RWops *src = SDL_RWFromFile(fileName, "wb");
	if(!src)
	{
		return false;
	}

	return SaveDatFile( src, bSaveIndexed );
}

bool KrResourceVault::SaveDatFile( SDL_RWops* src, bool bSaveIndexed, bool bCloseFile ) //maks
{
#if !defined(STAND_ALONE_GAME)

	MapResNamePointerIterator it( resMapName );
	KrEncoder encoder(src, bSaveIndexed);

	encoder.StartDat(bSaveIndexed?"GEDI":"KYRA");
	
	Uint32 indexPos = 0, *index = NULL, n = 0;
	if(bSaveIndexed)
	{
		//Count resources
		int lenght = 8; //Fisrt, save dat file lenght and number of resources
		for( it.Begin(); !it.Done(); it.Next() )
		{
			if((*it.Value())->Type() == KYRATAG_CANVAS ||
				(*it.Value())->Type() == KYRATAG_BOX)
			{
				continue;
			}

			gedString resName((*it.Value())->ResourceName());
			if(mapIgnoredResources[resName])
				continue;
			
			//Resource name + lenght + 32 bit index (newstyle string)
			lenght += 2*resName.length() + 2 + 4; 
			n++;
		}

		//Reserve index space
		SDL_RWseek( src, 0, SEEK_END ); //tell error without this
		indexPos = SDL_RWtell( src );
		index = new Uint32[n];

		//SDL_RWseek( src, lenght, SEEK_CUR ); //Don't works good
		//Write something
		U8 *tmp = new U8[lenght];
		SDL_RWwrite(src, tmp, lenght, 1);
		delete [] tmp;

#ifdef _DEBUG
		int fin = SDL_RWtell( src );
		GLASSERT(fin == indexPos + lenght);
#endif
	}
	
	//Save resources
	n = 0;
	for( it.Begin(); !it.Done(); it.Next() )
	{
		if((*it.Value())->Type() == KYRATAG_CANVAS ||
		   (*it.Value())->Type() == KYRATAG_BOX)
		{
			continue;
		}

		if(mapIgnoredResources[(*it.Value())->ResourceName()])
			continue;

		if(bSaveIndexed)
		{
			SDL_RWseek( src, 0, SEEK_END ); //tell error without this
			index[n++] = SDL_RWtell( src );

			/*#ifdef DEBUG
			GLOUTPUT( "*** %s, %X\n", (*it.Value())->ResourceName().c_str(), index[n-1]);
			#endif*/
		}

		(*it.Value())->Save(&encoder);
	}

	encoder.EndDat();

	//Save index
	if(bSaveIndexed)
	{
		SDL_RWseek( src, 0, SEEK_END ); //tell error without this
		Uint32 datLenght = SDL_RWtell( src );

		SDL_RWseek( src, indexPos, SEEK_SET );
		SDL_WriteLE32(src, datLenght);
		SDL_WriteLE32(src, n);
		
		n = 0;
		for( it.Begin(); !it.Done(); it.Next() )
		{
			if((*it.Value())->Type() == KYRATAG_CANVAS ||
				(*it.Value())->Type() == KYRATAG_BOX)
			{
				continue;
			}

			if(mapIgnoredResources[(*it.Value())->ResourceName()])
				continue;
			
			WriteString(src, (*it.Value())->ResourceName());
			SDL_WriteLE32(src, index[n++]);			
		}

		delete index;
	}

	SDL_RWseek( src, 0, SEEK_END );

	if(bCloseFile) SDL_RWclose(src);
	return *SDL_GetError() == 0;
#else
	return false;
#endif
}

KrResource *KrResourceVault::LoadIndexedResource(const gedString& resourceName) //maks
{
	#ifdef COUNT_PERFORMANCE //maks
	GlPerformance perf( "LoadIndexedResource" );
	#endif 

	
	
	KrResource* resource = NULL;
	if(!indexedData) return resource;

	Uint32 *index = resourceMap[resourceName];	
	if(!index) return NULL;

	Uint32 pos = SDL_RWtell( indexedData ); //Save actual position
	SDL_RWseek( indexedData, *index, SEEK_SET );

	U32 id   = SDL_ReadLE32( indexedData );
	U32 lenghtComp   = SDL_ReadLE32( indexedData );
	U32 size = SDL_ReadLE32( indexedData );

		
	if ( id != KYRATAG_END )
	{
		//Optimized memory read
		SDL_RWops *src = indexedData;
		U8 *mem = NULL;

				
		if(lenghtComp != size)
		{
			mem = new U8[size];

			
			if(mem)
			{
				//Decompress
				U8 *in = new U8[lenghtComp];
				if(!in) return NULL;

								
				SDL_RWread(src, in, lenghtComp, 1);	

				
				
				Compression comp;			
				if(!comp.Decompression(in, lenghtComp, mem, size))
				{
					delete [] in;
					
					SDL_RWclose(src);
					delete [] mem;

					return NULL;
				}

								
				delete [] in;
				
				src = SDL_RWFromMem(mem, size);				
			}
		}

		KrResourceFactory* factory = KrResourceFactory::Instance();
		resource = factory->Create( id, size, src );
				
		if ( resource )
		{
			//resList.PushBack( resource );
			stResourceInfo info(resource->ResourceName(), resource->Type());
			
			if(!resMapName.Find1(info))
			{				
				resMapName.Add(info, resource);
				resMapPointer.Add(resource, resource->ResourceName());				
			}
			else
			{
				//Don't duplicate resources				
				delete resource;
			}
		}
		
		
		delete factory;
		

		if(mem)
		{
			SDL_RWclose(src);
			delete [] mem;
		}
	}	
	
	SDL_RWseek( indexedData, pos, SEEK_SET ); //Restore position	
	return resource;
}

void KrResourceVault::IgnoreResource(const gedString& resourceName)
{
	//Don't save resource
	mapIgnoredResources.Add(resourceName, 1);
}
