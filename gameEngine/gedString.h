/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

If GPL v3 does not work for you, we offer a "non-GPL" license 
More info at http://game-editor.com/License
Be a Game Editor developer: http://game-editor.com/Sharing_Software_Revenues_in_Open_Source

***************************************************************************/


// gedString.h: interface for the gedString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEDSTRING_H__04EDE424_4FE9_4F64_B70D_09F578568BB8__INCLUDED_)
#define AFX_GEDSTRING_H__04EDE424_4FE9_4F64_B70D_09F578568BB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>
#include <string.h>

#ifndef _WIN32_WCE
#	include <wchar.h>
#endif

#include "SDL.h"
#include "SDL.h"
#include "../kyra/util/gldynarray.h"

#define CHAR_TYPE char

int wcsLen(const Uint16 *s);
int strLen(const CHAR_TYPE *s);

enum
{
	CHAR_STRING,
	UNICODE_STRING
};

#define MAX_STACK_BUF_SIZE 32 //Keeps gedString size to 64 bytes

class gedString  
{
public:

	int GetWriteLenght() const;

	bool hasPoint() const 
	{
		if(charBuf)
		{
			return strchr(charBuf, '.') != NULL;
		}

		return false;
	}

	bool IsUnicode() const {return type == UNICODE_STRING;}

	bool onlyBlank();
	bool isNumber();

	CHAR_TYPE *getCharBuf() const {return charBuf;}
	Uint16 *getUnicodeBuf() const {return unicodeBuf;}
	Uint32 getHash() const {return hash;};
	//Uint32 getCrc32() const;
	gedString GetFileName() const;
	gedString GetFilePath() const;
	void trimRight();
	void trimLeft();
	void erase(int pos, int n);
	static void SetReadEncriptString(bool enc) {bReadEncriptedString = enc;}
	void decript(bool bDecriptChar = false) const;
	void encript() const;
	void Write(SDL_RWops *stream) const;
	void Read(SDL_RWops* stream, bool bForceNew = false);
	void set(int pos, CHAR_TYPE c) {if(charBuf){charBuf[pos] = c; charBuf[pos] = (CHAR_TYPE)c; invalidateUnicode();}};
	static int npos;
	void clear();
	void resize(int n);
	//gedString& append(const Uint16 *string, int n = 0);
	bool empty() const {return len == 0;}
	gedString substr(int ini = 0, int n = 0) const;

	int rfind(const Uint16 *string, int ini = 0) const;
	//int rfind(const Uint16 c, int ini = 0) const;
	int rfind(const CHAR_TYPE *string, int ini = 0) const;
	int rfind(const CHAR_TYPE c, int ini = 0) const;

	int find(const gedString& string, int ini = 0) const;
	int find_no_case(gedString string, int ini = 0) const;
	int find(const Uint16 *string, int ini = 0) const;
	//int find(const Uint16 c, int ini = 0) const;
	int find(const CHAR_TYPE *string, int ini = 0) const;
	int find(const CHAR_TYPE c, int ini = 0) const;

	int replace(const CHAR_TYPE *oldString, const CHAR_TYPE *newString, int ini = 0);
	int replaceAll(const CHAR_TYPE *oldString, const CHAR_TYPE *newString);

	void lower();

	Uint16 operator[](int pos) const
	{
		if(pos >= len)
			return 0;

		if(charBuf)
		{
			return charBuf[pos];
		}
		
		if(unicodeBuf)
		{
			return unicodeBuf[pos];
		}		

		return 0;
	}

	int at(int pos) const;
	const Uint16 *c_str16();


	bool operator>(const gedString& string) const;
	bool operator<(const gedString& string) const;

	bool operator==(const gedString& string) const;
	bool operator==(const Uint16 * string) const;
	bool operator==(const CHAR_TYPE *string) const;

	bool compare(Uint16 * string) const;
	bool compare(CHAR_TYPE *string) const;
	int compare(const gedString &string, bool bCaseSensitive = true) const;

	bool operator!=(const gedString& string) const {return !(*this == string);}
	bool operator!=(Uint16 * string) const {return !(*this == string);}

	gedString& operator=(const gedString& string);
	gedString& operator=(const CHAR_TYPE *string);
	//gedString& operator=(const Uint16 *string);
	//gedString& operator=(const Uint16 c);

	gedString& append(const CHAR_TYPE *string, int n);
	gedString& append(const gedString& string);

	gedString& operator+=(const gedString& string);
	gedString& operator+=(const CHAR_TYPE c);
	gedString& operator+=(const CHAR_TYPE *string);
	//gedString& operator+=(const Uint16 *string);
	//gedString& operator+=(const Uint16 c);

	gedString operator+(const gedString& string) const;
	gedString operator+(const CHAR_TYPE *string) const;
	gedString operator+(const CHAR_TYPE c) const;
	//gedString operator+(const Uint16 *string) const;
	//gedString operator+(const Uint16 c) const;

	int length() const {return len;}
	int size() const {return len;} 

	gedString(const Uint16 *string, int n = -1);
	gedString(const CHAR_TYPE *string, int n = -1);
	gedString(const gedString& string);
	//gedString(const Uint16 c);
	gedString(const CHAR_TYPE c);
	gedString(long n);
	gedString(unsigned long n);
	gedString(double n, int nZeros = 1);
	gedString();

	virtual ~gedString();

	
		
	const CHAR_TYPE *c_str();
		

private:
	bool TestUnicode();
	void updateHash();
	void updateChar();
	void updateUnicode();
	void invalidateChar();
	void invalidateUnicode();

	inline void resetFirst32();

	CHAR_TYPE *ReallocChar(CHAR_TYPE *m, int size);
	void FreeChar(CHAR_TYPE *m);

	CHAR_TYPE stackBuf[MAX_STACK_BUF_SIZE]; //Don't alloc in the heap if size is less than 40 and is not unicode
	int charBufSize;
	CHAR_TYPE *charBuf;
	

	Uint16 * unicodeBuf;
	

	int len;

	static bool bReadEncriptedString;
	

	int type;
	Uint32 hash; //Cached hash for little strings
};

typedef GlDynArray<gedString> ListString;

#ifndef STAND_ALONE_GAME
class Token
{
public:
	Token() {initialPosition = finalPosition = tokenId = 0;}

	Token(gedString _text, int _initialPosition, int _finalPosition, int _tokenId)
	{
		text = _text;
		initialPosition = _initialPosition;
		finalPosition = _finalPosition;
		tokenId = _tokenId;
	}

	gedString text;
	int initialPosition;
	int finalPosition;
	int tokenId;
};
#endif

#endif // !defined(AFX_GEDSTRING_H__04EDE424_4FE9_4F64_B70D_09F578568BB8__INCLUDED_)
