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

#ifndef GLMAP_INCLUDED
#define GLMAP_INCLUDED

#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )


#include "../../gameEngine/gedString.h"


#include <ctype.h>
#include "glutil.h"
#include "gldebug.h"
#include "glprime.h"
#include "gltypes.h"

template < class KEY, class VALUE, class HASH >
class GlMapIterator;

// Standard Hash classes.
class GlHash
{
  public:
	U32 HashValue()	{ return val; }
  protected:
	U32 val;	
};

class GlStringHash : public GlHash
{
  public:
	GlStringHash( const gedString& str )
	{	
		val = str.getHash();
		if(!val)
		{			
			for ( int i=0; i<str.length() && i < 32; ++i )
			{
				val <<= 1;
				val |= str[i];
			}

			//val = str.getCrc32();
		}
	}

	GlStringHash( const char *str )
	{	
		val = 0;
		int len = strLen(str);
		for ( int i=0; i < len && i < 32; ++i )
		{
			val <<= 1;
			val |= str[i];
		}

		/*U32 Crc32(void * seq, long size);
		val = Crc32((void *)str, strlen(str));*/
	}
};

template < class T >
class GlNumberHash : public GlHash
{
  public:
	GlNumberHash( T num )	{ val = (T) num; }
};

template < class T >
class GlPointerHash : public GlHash
{
  public:
	GlPointerHash( T num )	{ val = (U32)num; }
};


// A map template class. It associates a KEY with a VALUE.
// It uses a HASH class to get a hash value from the KEY. 2 Hash classes are
// provided: GlStringHash and GlNumberHash.

template < class KEY, class VALUE, class HASH >
class GlMap
{
  public:
	GlMap( U32 startSize = 64, U32 grow = 60 );
	virtual ~GlMap() {Clear();}

	// Adds a key value pair. Will fail if the key is not unique.
	bool Add( const KEY& key, const VALUE& value );
	bool Add( const char *key, const VALUE& value );
	
	// Remove a key value pair. Will fail if the key is not found.
	bool Remove( const KEY& key, bool bReleaseMemory = true ); //maks
	bool Remove( const char *key, bool bReleaseMemory = true ); //maks
	
	// Returns true if the key is found, and writes the result to value.
	// Value will not be set if the key is not found.
	bool Find( const KEY& key, VALUE* value );

	VALUE* Find1( const KEY& key) const;
	VALUE* FindString( const char *key );
	VALUE* operator[]( const char *key ); //maks
	VALUE* operator[]( const KEY& key ); //maks
	VALUE* FindOrCreate(const KEY& key); //maks

	// PRIVATE! But silly template friends too hard.
	enum {
		EXPAND = 4
	};

	struct Item
	{
		KEY		key;
		VALUE	value;
		Item*	next;
	};

	// Grow the hash table. Dicey work, and slow.
	void Grow( int newsize );

	U32 size() {return numItems;} //maks
	bool empty() {return numItems == 0;} //maks

	void operator=(const GlMap< KEY, VALUE, HASH >& copy ) //maks	
	{ 
		Clear();
		Reinit();
		//if(copy.empty()) return;

		GlMapIterator< KEY, VALUE, HASH > it(copy);
		for(it.Begin(); !it.Done(); it.Next())
			Add(*it.Key(), *it.Value());		
	}

	void Reinit()
	{
		//Init
		buckets = new Item*[ numBuckets ];
		memset( buckets, 0, sizeof( Item* ) * numBuckets );
		numItems = 0;

		canAddArray = true;
		clearArray = false;
		keyArray.Clear();
		valueArray.Clear();
	}

	void Clear(bool bReleaseMemory = true); //maks
	KEY **GetKeyArray(); //maks
	VALUE **GetValueArray(); //maks

	Item** buckets;
	U32 numBuckets;

private:

	void *Alloc(); //maks
	void Free(Item *m, bool bReleaseMemory); //maks



		
	U32 grow;
	U32 numItems;

	//maks: to avoid iterators and sppedup
	GlDynArray<KEY *>	keyArray; 
	GlDynArray<VALUE *> valueArray; 

	//maks: to avoid allocations and sppedup
	GlDynArray<Item *> freeList;

	bool canAddArray, clearArray; //Solve crash in spriteBenchmark.ged	
};

template < class KEY, class VALUE, class HASH >
void *GlMap< KEY, VALUE, HASH>::Alloc() //maks
{
	if(freeList.Count())
	{
		Item *m = freeList.Back();
		freeList.PopBack();
		return m;
	}

	//No itens available in the free list, so allocate new
	return new Item;
}

template < class KEY, class VALUE, class HASH >
void GlMap< KEY, VALUE, HASH>::Free(Item *m, bool bReleaseMemory) //maks
{
	if(bReleaseMemory)
	{
		delete m;
	}
	else
	{
		//Put in the free list
		freeList.PushBack(m);
	}
}

template < class KEY, class VALUE, class HASH >
KEY **GlMap< KEY, VALUE, HASH>::GetKeyArray() //maks
{
	if(clearArray)
	{
		keyArray.Clear();
	}

	if(!keyArray.Memory())
	{
		//Create keyArray		
		int i = 0;
		keyArray.SetCount(numItems + 1);

		if(numItems)
		{
			GlMapIterator<KEY, VALUE, HASH> it(*this);
			
			for(it.Begin(); !it.Done(); it.Next())
			{
				keyArray.SetItem(i++, it.Key());
			}
		}

		//Always include the terminator
		keyArray.SetItem(i++, NULL); 
	}	

	canAddArray = false;
	clearArray = false;
	return keyArray.Memory();
}

template < class KEY, class VALUE, class HASH >
VALUE **GlMap< KEY, VALUE, HASH>::GetValueArray() //maks
{
	if(clearArray)
	{
		valueArray.Clear();
	}

	if(!valueArray.Memory())
	{
		//Create valueArray		
		int i = 0;
		valueArray.SetCount(numItems + 1);

		if(numItems)
		{
			GlMapIterator<KEY, VALUE, HASH> it(*this);
			
			for(it.Begin(); !it.Done(); it.Next())
			{
				valueArray.SetItem(i++, it.Value());
			}
		}

		//Always include the terminator
		valueArray.SetItem(i++, NULL); 
	}	

	canAddArray = false;
	clearArray = false;
	return valueArray.Memory();
}

template < class KEY, class VALUE, class HASH >
void GlMap< KEY, VALUE, HASH>::Clear(bool bReleaseMemory) //maks
{
	if(!buckets) return;

	for( U32 i=0; i<numBuckets; ++i )
	{	
		while ( buckets[i] )
		{
			Item* next = buckets[i]->next;
			Free(buckets[i], bReleaseMemory); //maks
			buckets[i] = next;
		}
	}
		
	// Everything is in the new list, create new buckets and put it back in.
	if(bReleaseMemory)
	{
		//Delete itens in the free list
		int n = freeList.Count();
		for(int i = 0; i < n; i++)
		{
			delete freeList[i];	
		}

		freeList.Clear();

		//Delete buckets
		delete [] buckets;	
		buckets = NULL;
	}

	numItems = 0;

	canAddArray = true;
	clearArray = false;

	keyArray.Clear();
	valueArray.Clear();
}

template < class KEY, class VALUE, class HASH >
void GlMap< KEY, VALUE, HASH>::Grow( int newsize )
{
	#ifdef DEBUG
	int itemcount = 0;
	#endif

	if(!buckets) Reinit(); //maks
	if(!buckets) return;

	// Yep. Unlink everything, put in a linked list, and re-insert.
	Item *root = 0;
	for( U32 i=0; i<numBuckets; ++i )
	{	
		while ( buckets[i] )
		{
			Item* next = buckets[i]->next;

			buckets[i]->next = root;
			root = buckets[i];

			buckets[i] = next;

			#ifdef DEBUG
			++itemcount;
			#endif
		}
	}
	#ifdef DEBUG
	int comparecount = 0;
	for( Item* it = root; it; it = it->next )
		++comparecount;
	GLASSERT( comparecount == itemcount );
	#endif
		
	// Everything is in the new list, create new buckets and put it back in.
	delete [] buckets;
	buckets = 0;

	#ifdef DEBUG
		//GLOUTPUT( "Rebuilding map (from %d)...", numBuckets );
	#endif

	numBuckets = GlPrime( newsize, 1 );
	#ifdef DEBUG
		//GLOUTPUT( "%d buckets\n", numBuckets );
	#endif

	buckets = new Item*[ numBuckets ];
	memset( buckets, 0, sizeof( Item* ) * numBuckets );

	while ( root )
	{
		Item* next = root->next;

		HASH hash( root->key );
		U32 which = hash.HashValue() % numBuckets;

		root->next = buckets[ which ];
		buckets[ which ] = root;

		root = next;
	}
}

template < class KEY, class VALUE, class HASH >
bool GlMap< KEY, VALUE, HASH>::Add( const char *key, const VALUE& value ) //maks
{
	//Assumes string keys
	VALUE *val = FindString(key);
	if(val) //maks: replace with new value
	{
		*val = value;
		return true;
	}


	if(!buckets) Reinit(); //maks
	if(!buckets) return false; //maks

	// Do we need to get bigger?
	if ( ( ( numItems + 1 ) * 100 / numBuckets ) > grow )
	{
		Grow( GlMax( numItems * EXPAND, numBuckets * EXPAND ) );
	}

	// Add the key in, if it is unique.
	HASH hash( key );
	U32 which = hash.HashValue() % numBuckets;

	Item* item	= (Item *)Alloc(); //maks
	item->key	= key;
	item->value = value;
	item->next	= buckets[ which ];

	buckets[ which ] = item;
	
	//maks dont't clear the arrays here
	if(canAddArray)
	{
		if(keyArray.Memory())
		{
			keyArray.SetCount(numItems + 2); 
			keyArray.SetItem(numItems, &item->key);
			keyArray.SetItem(numItems + 1, NULL);
		}
		
		if(valueArray.Memory())
		{
			valueArray.SetCount(numItems + 2); 
			valueArray.SetItem(numItems, &item->value);
			valueArray.SetItem(numItems + 1, NULL);
		}
	}
	else
	{
		clearArray = true;
	}

	++numItems;
	return true;
}

template < class KEY, class VALUE, class HASH >
bool GlMap< KEY, VALUE, HASH>::Add( const KEY& key, const VALUE& value )
{
	VALUE *val = Find1(key);
	if(val) //maks: replace with new value
	{
		*val = value;
		return true;
	}


	if(!buckets) Reinit(); //maks
	if(!buckets) return false; //maks

	// Do we need to get bigger?
	if ( ( ( numItems + 1 ) * 100 / numBuckets ) > grow )
	{
		Grow( GlMax( numItems * EXPAND, numBuckets * EXPAND ) );
	}

	// Add the key in, if it is unique.
	HASH hash( key );
	U32 which = hash.HashValue() % numBuckets;

	Item* item	= (Item *)Alloc(); //maks
	item->key	= key;
	item->value = value;
	item->next	= buckets[ which ];

	buckets[ which ] = item;
	
	//maks dont't clear the arrays here
	if(canAddArray)
	{
		if(keyArray.Memory())
		{
			keyArray.SetCount(numItems + 2); 
			keyArray.SetItem(numItems, &item->key);
			keyArray.SetItem(numItems + 1, NULL);
		}
		
		if(valueArray.Memory())
		{
			valueArray.SetCount(numItems + 2); 
			valueArray.SetItem(numItems, &item->value);
			valueArray.SetItem(numItems + 1, NULL);
		}
	}
	else
	{
		clearArray = true;
	}

	++numItems;
	return true;
}


template < class KEY, class VALUE, class HASH >
bool GlMap< KEY, VALUE, HASH>::Find( const KEY& key, VALUE* value )
{
	if(!buckets || !numItems) return false; //maks

	/*if(empty())
		return false;*/

	HASH hash( key );
	U32 which = hash.HashValue() % numBuckets;

	Item* item = buckets[which];
	
	while( item )
	{
		if ( item->key == key )
		{
			*value = item->value;
			return true;
		}
		item = item->next;
	}
	return false;	
}

template < class KEY, class VALUE, class HASH >
VALUE* GlMap< KEY, VALUE, HASH>::Find1( const KEY& key ) const
{
	if(!buckets || !numItems) return NULL;
	/*if(empty())
		return NULL;*/

	HASH hash( key );
	U32 which = hash.HashValue() % numBuckets;

	Item* item = buckets[which];
	
	while( item )
	{
		if ( item->key == key )
		{
			return &(item->value);
		}
		item = item->next;
	}
	return NULL;
}

template < class KEY, class VALUE, class HASH >
VALUE* GlMap< KEY, VALUE, HASH>::FindString( const char *key )
{
	//Assumes string keys
	if(!key || !buckets || !numItems) return NULL; //maks
	
	HASH hash( key );
	U32 which = hash.HashValue() % numBuckets;

	Item* item = buckets[which];
	
	while( item )
	{
		if ( item->key == key )
		{
			return &(item->value);
		}
		item = item->next;
	}
	return NULL;
}

template < class KEY, class VALUE, class HASH >
VALUE* GlMap< KEY, VALUE, HASH>::operator[]( const char *key ) //maks
{
	return FindString(key);
}

template < class KEY, class VALUE, class HASH >
VALUE* GlMap< KEY, VALUE, HASH>::operator[](const KEY& key) //maks
{
	if(!buckets || !numItems) return NULL;
	/*if(empty())
		return NULL;*/

	HASH hash( key );
	U32 which = hash.HashValue() % numBuckets;

	Item* item = buckets[which];
	
	while( item )
	{
		if ( item->key == key )
		{
			return &(item->value);
		}
		item = item->next;
	}
	return NULL;	
}

template < class KEY, class VALUE, class HASH >
VALUE* GlMap< KEY, VALUE, HASH>::FindOrCreate(const KEY& key) //maks
{
	while(1)
	{
		if(buckets)
		{
			HASH hash( key );
			U32 which = hash.HashValue() % numBuckets;
			
			Item* item = buckets[which];
			
			while( item )
			{
				if ( item->key == key )
				{
					return &(item->value);
				}
				item = item->next;
			}
		}
		
		//Not found, add
		VALUE newValue;
		Add(key, newValue);	
	}
}


template < class KEY, class VALUE, class HASH >
bool GlMap< KEY, VALUE, HASH>::Remove( const KEY& key, bool bReleaseMemory ) //maks
{
	if(!buckets || !numItems) return false; //maks

	HASH hash( key );
	U32 which = hash.HashValue() % numBuckets;

	Item* item = buckets[which];
	Item* prev = 0;
	
	while( item )
	{
		if ( item->key == key )
		{
			if ( prev )
				prev->next = item->next;
			else
				buckets[ which ] = item->next;

			Free(item, bReleaseMemory); //maks
			--numItems;

			keyArray.Clear(); //maks
			valueArray.Clear(); //maks
			return true;
		}
		prev = item;
		item = item->next;
	}

	return false;	
}

template < class KEY, class VALUE, class HASH >
bool GlMap< KEY, VALUE, HASH>::Remove( const char *key, bool bReleaseMemory ) //maks
{
	//Assumes string keys
	if(!buckets || !numItems) return false; //maks

	HASH hash( key );
	U32 which = hash.HashValue() % numBuckets;

	Item* item = buckets[which];
	Item* prev = 0;
	
	while( item )
	{
		if ( item->key == key )
		{
			if ( prev )
				prev->next = item->next;
			else
				buckets[ which ] = item->next;

			Free(item, bReleaseMemory); //maks
			--numItems;

			keyArray.Clear(); //maks
			valueArray.Clear(); //maks
			return true;
		}
		prev = item;
		item = item->next;
	}

	return false;	
}



template < class KEY, class VALUE, class HASH >
inline GlMap< KEY, VALUE, HASH>::GlMap( U32 startSize, U32 _grow )
{
	numBuckets = GlPrime( startSize, 1 );
	/*buckets = new Item*[ numBuckets ]; //maks
	memset( buckets, 0, sizeof( Item* ) * numBuckets );*/
	grow = _grow;
	numItems = 0;
	
	//maks
	buckets = NULL;
	canAddArray = true;
	clearArray = false;

	#ifdef DEBUG
		//GLOUTPUT( "Created Map: %d buckets\n", numBuckets );
	#endif
};







template < class KEY, class VALUE, class HASH >
class GlMapIterator
{	 
  public:
	  GlMapIterator(const GlMap< KEY, VALUE, HASH >& _map )	
	  { map = &_map; bucket=0; item=0; } //maks

	void Begin() //maks		
	{ 
		bucket=0; 
		if(map->buckets)
		{
			item=map->buckets[0]; 
			FindValid(); 
		}
		else
			bucket = -1;
	}

	void Next()			
	{ 
		if(item)
		{						
			item=item->next;  
		}

		FindValid();
	}
	bool Done()			{ return ( bucket < 0 ); } 

	void Current( KEY* key, VALUE* value )
	{
		if ( item )
		{
			*key = item->key;
			*value = item->value;
		}
	}

	KEY *Key() //maks
	{
		if(item)
			return &item->key;
		else 
			return NULL;
	}

	VALUE *Value() //maks
	{
		if(item)
			return &item->value;
		else 
			return NULL;
	}

  private:
	const GlMap< KEY, VALUE, HASH >* map;
	int bucket;
	typename GlMap< KEY, VALUE, HASH >::Item* item;

	void FindValid()
	{
		if (	bucket < 0				// we are done
			 || ( item && bucket >=0 ))	// we have a current value
		{
			return;
		}
		GLASSERT( item == 0 );			// else we should have current value
		for ( ++bucket; bucket < (int)map->numBuckets; ++bucket ) //maks
		{
			if ( map->buckets[bucket] )
			{
				item = map->buckets[ bucket ];
				return;
			}
		}
		bucket = -1;
		item = 0;
	}
};


#endif

