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

#ifndef KYRA_DYNARRAY_INCLUDED
#define KYRA_DYNARRAY_INCLUDED

#include <limits.h>
#include "glutil.h"
#include "gldebug.h"
#include <memory.h> 


//#include "../../gameEngine/dlmalloc.h" //maks

//maks: if use free and malloc will crash Purify

/**	A dynamic array class.
	Has a notion of both Size (the allocated
	memory) and Count (the number of items.) Generally, as a user
	of the array you want to be sure you are using Count.
	
	Left to its own devices, GlDynArray will use power of 2 
	memory allocation when it expands.
*/
template <class T>
class GlDynArray
{
  public:
	GlDynArray( int _size )	{ size1 = 0; count = 0; data = 0; bAutoExpand = false; bZeroMemory = false; Resize( _size ); } //maks
	GlDynArray()			
	{ 
		size1 = 0; 
		count = 0; 
		data = 0; 
		bAutoExpand = bZeroMemory = false; //maks
	}

	~GlDynArray()			
	{ 
		delete [] data; /*if(data) free(data);*/ 
	}

	GlDynArray( const GlDynArray<T>& copy )			{ size1 = 0; count = 0; data = 0; bAutoExpand = false; bZeroMemory = false; this->SetEqual( copy ); }
	void operator=( const GlDynArray<T>& copy )		{ this->SetEqual( copy ); }

	unsigned NotFound()	const { return UINT_MAX; }		///< Returned by failed search.
	bool     Empty()    const { return count == 0; }	///< Returns true if this contains nothing.

	void setAutoExpand(bool value) {bAutoExpand = value;} //maks
	void setZeroMemory(bool value) {bZeroMemory = value;} //maks

	void Clear() ///< delete the contents of this array
	{ 
		if(data) //maks
		{
			delete [] data;
			//free(data);
		} 
		size1  = 0; 
		count = 0; 
		data = 0; 
	} 
	void clear() {Clear();} //maks: vector compatibility
	void ClearCount() {count = 0;} //maks: don't erase data

	/// Array access.
	T& operator[] ( int i )	
	{	
		if(i < 0) i = 0;
		if(i >= count && bAutoExpand) //maks
		{
			if(i >= size1 )
			{
				//Icrease the allocation size
				ResizePower2(i + 1);
			}

			count = i + 1;
		}

		GLASSERT( i<count );
		
		return data[i];		
	}
 	/// Access to the base of array memory.
	T* Memory()		{ return data; }

	void SetEqual( const GlDynArray<T>& rhs )
	{
		if(rhs.size1 > 0)
		{
			delete [] data;
			data = new T[ rhs.size1 ];
			//if(data) free(data);
			//data = (T*)malloc(rhs.size1*sizeof(T));

			for ( int i=0; i<rhs.count; i++ )
			{
				data[i] = rhs.data[i];
			}
			size1 = rhs.size1;
			count = rhs.count;
		}
		else
		{
			Clear();
		}

		bAutoExpand = rhs.bAutoExpand; //maks
		bZeroMemory = rhs.bZeroMemory; //maks
	}

	/// Fetch an item, by reference, at the index given. 
	T Item( int i ) const	{ GLASSERT( i<count );
								  return data[i]; }
	/// Fetch an item, by pointer, at the index given.
	T* ItemPointer( int i ) const	{ GLASSERT( i<count );
										  return &data[i]; }
	/// Set the item at the given index.
	void SetItem( int i, const T& t )
	{ 
		if(data && i<count)
		{
			data[i] = t; 
		}
	}

	/** The number of items in the array. (Managed by the user.)
	*/
	int Count() const 	{ return count; }
	int size() {return Count();} //maks: vector compatibility

	/** The allocated memory, in total number of items.
	*/
	int AllocatedSize()	const	{ return size1; }

	/** Set the number of items in the array. This will also
		re-allocate memory.
	*/
	void SetCount( int _count )	
	{	
		if ( count != _count )										
		{
			//if ( _count != size1 )
			if(_count >= size1 || _count <= (size1 >> 1)) //maks: increase buffer only if need more space, or need to release less than half
			{
				ResizePower2( _count );
			}

			count = _count;
		}
	}

	/** Push T to the last element of the array. Will never
		reduce the size1, only increase it. (Useful for setting the
		size1 to something reasonable, and then pushing items
		to the array.

		Will increment count.
	*/				
	int PushBack( const T& t)			
	{
		if ( count+1 > size1 )
			ResizePower2( count+1 );

		if(data) data[count] = t; //maks
		count++;
		return count -1;
	}

	void push_back(const T& t) {PushBack(t);} //maks: vector compatibility

	/** Get the last element. */
	T Back()
	{
		GLASSERT( count > 0 );
		return data[ count - 1 ];
	}

	/** Pop the last element of the array. Will never
		change the size1 of allocated memory, but will
		update the count.
	*/				
	T PopBack()			
	{
		GLASSERT( count > 0 );
		count--;
		return data[count];
	}

	void PushFront(T t) //maks
	{
		Insert(t, 0);
	}

	/** Insert an element into this array at index 'n'.
		Will resource and adjust count as necessary.
	*/
	void Insert( T t, int n )
	{
		if ( n > count ) 
			n = count;
		if ( count + 1 > size1 )
			ResizePower2( count + 1 );
		if ( count > 0 )
		{
			for ( int i = count; i > n; i-- )
				data[i] = data[i-1];
		}
		data[n] = t;
		++count;
	}

	/** Remove the element at index 'n'.
		Adjusts count as necessary. Won't reduce memory.
	*/
	void Remove( int n )
	{
		if ( n >= count ) 
			return;

		GLASSERT( count > 0 );
		if ( count > 0 )
		{
			for ( int i = n; i < count-1; i++ )
				data[i] = data[i+1];
			--count;
		}
	}

	/** Trys to find the selected item (criteria being t == data[i])
		returns an index if successful, NOTFOUND if not.
	*/
	int Find( const T& t ) const
	{	
		for ( int i=0; i<count; i++ )
			if ( data[i] == t )
				return i;
		return NotFound();
	}

	/** Trys to find the selected item (criteria being t == data[i])
		and delete it.
	*/
	bool FindAndDelete( const T& t )
	{	
		for ( int i=0; i<count; i++ )
		{
			if ( data[i] == t )
			{
				Remove( i );
				return true;
			}
		}
		return false;
	}

	/** Resize the array. Will take no action of the array
		size1 does not change. Will not increase count.

	*/
	void Resize( int _size )	
	{
		if (_size > 0 && _size != size1 ) //maks 
		{ 
			T* newData = new T[_size];			
			//T* newData = (T*)malloc(_size*sizeof(T));

			if(bZeroMemory)
			{
				memset(newData, 0, _size*sizeof(T));
			}

			int copy = GlMin( _size, count );
			
			if(data) //maks
			{
				for( int i=0; i<copy; i++ )
					newData[i] = data[i];
				
				delete [] data;
				//free(data);
			}

			data = newData;

			size1 = _size;
			count = GlMin( count, _size );
		}
	}

	/** A more useful and efficient resize. Note resize normally
		does not need to be called, since it will be called by other functions.
	*/
	void ResizePower2( int _size )
	{
		int newSize = 1;
		while ( newSize < _size )
			newSize <<= 1;
		Resize( newSize );
	}

	/** Appends another DynArray (of the same type) on to this
		one. The appended array is unchanged. Uses PowerOf2 allocation
		in hopes of optimizing multiple appends.
	*/
	void Append( const GlDynArray<T>& rhs )
	{
		int addAt = count; //maks
		SetCount(count + rhs.count);//ResizePower2( size1 + rhs.size1 );

		for ( int i=0; i<rhs.count; i++ )
			data[i+addAt] = rhs.data[i]; //maks
	}

	/** Perform a shellsort on the array. Note that "Find" is not
		cordinated with the sort, so Find will still be in linear 
		time.
	*/
	void Sort(bool bOldSort = false) //maks: original algorithm does not sort first element
	{
		// Shellsort. Sedgewick, Algorithms in C++. p109
		// Note N = count-1
		/*int i, j, h;
		T value;
		int N = count-1;

		for( h = 1; h <= N/9; h = 3*h+1 )	
		{}

		for( ; h > 0; h /= 3 )
		{
			for( i = h+1; i <= N; ++i )
			{
				value = data[i];
				j = i;
				while( j>h && data[j-h]>value )
				{
					data[j] = data[j-h];
					j -= h;
				}
				data[j] = value;
				}
		}*/
		
		
		int inc = count-1, ant;
		T val;

		if(inc == 1 && !bOldSort)
		{
			//maks: 2 element array
			if(data[1] < data[0])
			{
				val = data[0];
				data[0] = data[1];
				data[1] = val;
			}
		}
		else
		{
			//maks: don't sort 2 element array			
			while ( inc > 1 )
			{
				inc = 2*( inc/4 ) + 1;
				for ( int a=0; a <= inc-1; a++ )
				{
					for ( int j = a+inc; j <= count-1; j=j+inc )
					{
						ant = j - inc;
						val = data[j];
						while( ant >= 0 && val < data[ant] )
						{
							data[ant+inc] = data[ant];
							ant = ant - inc;
						}
						data[ant + inc] = val;
					}
				}
			}
		}

	}

  private:
	int count;
	int size1;
	T*		 data;
	bool bAutoExpand, bZeroMemory; //maks
};


#endif
