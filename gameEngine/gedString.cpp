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


// gedString.cpp: implementation of the gedString class.
//
//////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#ifndef _WIN32_WCE
#	include <wctype.h>
#endif

#include "gedString.h"
#include "SDL_endian.h"
#include "GameControl.h"

#define TEXT_NUMBER_DOUBLE_LIMIT	0.000000001


#include "dlmalloc.h"

U32 Crc32(void * seq, long size);

#define toLowerChar(c)       (isupper(c) ? (c)+32:c)
#define toLowerUnicode(c)       (iswupper(c) ? (c)+32:c)

//#define toUpper(c)       (iswlower(c) ? (c)-32:c)

Uint16 emptyString = 0;

wchar_t *charToUnicode(CHAR_TYPE *s)
{
	//Convert to system dependent wchar_t
	//To small strings conversion
	static wchar_t unicodeBuf[1024];

	int len = strlen(s);
	wchar_t *u = unicodeBuf;
	
	while(len > 0) 
	{
		*u = *s;
		++u; ++s; --len;
	}
	
	*u = 0;

	return unicodeBuf;
}

void wcsCpyWcharTo16(Uint16 *d, wchar_t *s)
{
	while(*s) 
	{
		*d++ = *s++;		
	}
	
	*d = 0;
}

void strCharToUnicodeCpy(Uint16 *d, const CHAR_TYPE *s)
{
	//To small strings conversion
	
	while(*s) 
	{
		*d++ = *s++;		
	}
	
	*d = 0;
}

void strCharToUnicodeNCpy(Uint16 *d, const CHAR_TYPE *s, int n)
{
	//To small strings conversion
	
	while(*s && n > 0) 
	{
		*d++ = *s++;
		n--;
	}
	
	*d = 0;
}

//Don't use wchar_t (may be 16 bits, may be 32bits...)
void wcsCpy(Uint16 *d, const Uint16 *s)
{
	while(*s) 
	{
		*d++ = *s++;		
	}
	
	*d = 0;
}



void wcsNcpy(Uint16 *d, const Uint16 *s, int n)
{
	while(*s && n > 0) 
	{
		*d++ = *s++;
		n--;
	}
	
	*d = 0;
}


int wcsLen(const Uint16 *s)
{
	int len = 0;
	
	while (s[len] != 0)
    {
		if (s[++len] == 0)
			return len;
		if (s[++len] == 0)
			return len;
		if (s[++len] == 0)
			return len;
		++len;
    }
	
	return len;
}

int strLenPoint(const CHAR_TYPE *s, bool &bHasPoint)
{
	const CHAR_TYPE *p = s;
	
	while(*p)
    {
		if(*p == '.')
		{
			bHasPoint = true;
		}

		++p;
    }
	
	return p - s;
}

int strLen(const CHAR_TYPE *s)
{
	const CHAR_TYPE *p = s;
	
	while(*p) ++p;
	
	return p - s;
}

Uint16 *wcsStr(const Uint16 *big, const Uint16 *little)
{
	const Uint16 *p;
	const Uint16 *q;
	const Uint16 *r;
	
	
	if (!*little) 
	{
		/* LINTED interface specification */
		return (Uint16 *)big;
	}
	if (wcsLen(big) < wcsLen(little))
		return NULL;
	
	p = big;
	q = little;
	
	while (*p) 
	{
		q = little;
		r = p;
		
		while (*q) 
		{
			if (*r != *q)
				break;
			q++;
			r++;
		}
		if (!*q) 
		{
			/* LINTED interface specification */
			return (Uint16 *)p;
		}
		p++;
	}
	
	return NULL;
}

Uint16 *wcsUnicodeChar(const Uint16 *big, const char *little)
{
	const Uint16 *p;
	const char *q;
	const Uint16 *r;
	
	
	if (!*little) 
	{
		/* LINTED interface specification */
		return (Uint16 *)big;
	}
	if (wcsLen(big) < (int)strlen(little))
		return NULL;
	
	p = big;
	q = little;
	
	while (*p) 
	{
		q = little;
		r = p;
		
		while (*q) 
		{
			if (*r != (Uint16)*q)
				break;

			q++;
			r++;
		}

		if (!*q) 
		{
			/* LINTED interface specification */
			return (Uint16 *)p;
		}
		p++;
	}
	
	return NULL;
}

char *wcsCharUnicode(const char *big, const Uint16 *little)
{
	const char *p;
	const Uint16 *q;
	const char *r;
	
	
	if (!*little) 
	{
		/* LINTED interface specification */
		return (char *)big;
	}

	if ((int)strlen(big) < wcsLen(little))
		return NULL;
	
	p = big;
	q = little;
	
	while (*p) 
	{
		q = little;
		r = p;
		
		while (*q) 
		{
			if ((Uint16)*r != *q)
				break;

			q++;
			r++;
		}

		if (!*q) 
		{
			/* LINTED interface specification */
			return (char *)p;
		}
		p++;
	}
	
	return NULL;
}

Uint16 *wcsChr(const Uint16 *s, Uint16 c)
{
	const Uint16 *p;
	
	
	p = s;
	while (*p) 
	{
		if (*p == c) 
		{
			/* LINTED interface specification */
			return (Uint16 *)p;
		}
		p++;
	}
	return NULL;
}

int wcsCmp(const Uint16 *s1, const Uint16 *s2)
{
	while (*s1 == *s2++)
	{
		if (*s1++ == 0)
			return (0);
	}
	
	return (*(const Uint16 *)s1 - *(const Uint16 *)(s2 - 1));
	
}

int cmpUnicodeChar(const Uint16 *s1, const CHAR_TYPE *s2)
{
	while (*s1 == *s2++)
		if (*s1++ == 0)
			return (0);
		
	return (*(const Uint16 *)s1 - *(const CHAR_TYPE *)(s2 - 1));	
}

int cmpCharUnicode(const CHAR_TYPE *s1, const Uint16 *s2)
{
	while (*s1 == *s2++)
		if (*s1++ == 0)
			return (0);
		
	return (*(const CHAR_TYPE *)s1 - *(const Uint16 *)(s2 - 1));	
}

int StriCmp(const char *string1, const char *string2) 
{
   while (*string1 && ((*string1 | 0x20) == (*string2 | 0x20))) {
      string1++;
      string2++;
   }
   return (*string1 - *string2);
}


int gedString::npos = -1;
bool gedString::bReadEncriptedString = false;


gedString::gedString()
{	
	charBuf = NULL;
	charBufSize = 0;
	unicodeBuf = NULL;
	
	clear();
}

gedString::gedString(const Uint16 *string, int n)
{		
	charBuf = NULL;
	charBufSize = 0;
	unicodeBuf = NULL;	
	len = 0;
	hash = 0;

	type = UNICODE_STRING;
	if(!string) return;
	
	
	if(n <= 0) 
	{
		len = wcsLen(string);
		unicodeBuf = (Uint16 *)malloc((len+1)*sizeof(Uint16));

		if(unicodeBuf)
		{
			wcsCpy(unicodeBuf, string);
			unicodeBuf[len] = 0;
		}
	}
	else
	{
		len = wcsLen(string);
		if(len > n) len = n;

		unicodeBuf = (Uint16 *)malloc((len+1)*sizeof(Uint16));

		if(unicodeBuf)
		{
			wcsNcpy(unicodeBuf, string, len);
			unicodeBuf[len] = 0;
		}
	}
	
	TestUnicode();
	updateHash();
}

gedString::gedString(const CHAR_TYPE *string, int n)
{		
	charBuf = NULL;
	charBufSize = 0;
	unicodeBuf = NULL;	
	len = 0;
	hash = 0;

	type = CHAR_STRING;
	if(!string) return;

	
	if(n <= 0) 
	{
		len = strlen(string);
		charBuf = (CHAR_TYPE *)ReallocChar(charBuf, (len<32)?32:(len+1));
		resetFirst32();

		strcpy(charBuf, string);
		charBuf[len] = 0;
	}
	else
	{
		len = strlen(string);
		if(len > n) len = n;

		charBuf = (CHAR_TYPE *)ReallocChar(charBuf, (len<32)?32:(len+1));
		resetFirst32();

		strncpy(charBuf, string, len);
		charBuf[len] = 0;
	}

	updateHash();
}

void gedString::resetFirst32()
{	
	if(charBuf)
	{
		U64 *w = (U64 *)charBuf;
		w[0] = 0;
		w[1] = 0;
		w[2] = 0;
		w[3] = 0;
	}
}


gedString::gedString(const CHAR_TYPE c)
{		
	charBuf = NULL;
	charBufSize = 0;
	unicodeBuf = NULL;
	
	clear();
	
	charBuf = (CHAR_TYPE *)ReallocChar(charBuf, 32);
	resetFirst32();

	charBuf[0] = c;
	charBuf[1] = 0;
	len = 1;
	
	type = CHAR_STRING;	
	updateHash();
}

gedString::gedString(const gedString& string)
{		
	charBuf = NULL;
	charBufSize = 0;
	unicodeBuf = NULL;
	len = 0;
	hash = 0;

	type = string.type;

	if(string.type == UNICODE_STRING)
	{
		if(!string.unicodeBuf) return;
		
		len = string.length();
		unicodeBuf = (Uint16 *)malloc((len+1)*sizeof(Uint16));
		
		if(unicodeBuf)
		{
			wcsCpy(unicodeBuf, string.unicodeBuf);
			unicodeBuf[len] = 0;	
			
			TestUnicode();
		}
	}
	else
	{
		if(!string.charBuf) return;
		
		len = string.length();		
		charBuf = (CHAR_TYPE *)ReallocChar(charBuf, (len<32)?32:(len+1));
		
		if(charBuf)
		{
			resetFirst32();
			
			strcpy(charBuf, string.charBuf);
			charBuf[len] = 0;
		}
	}	

	updateHash();
}

gedString::gedString(long n)
{		
	CHAR_TYPE nBuf[32];
	sprintf(nBuf, "%ld", n);

	charBuf = NULL;
	charBufSize = 0;

	len = strlen(nBuf);
	charBuf = (CHAR_TYPE *)ReallocChar(charBuf, 32);
	resetFirst32();

	strcpy(charBuf, nBuf);
	charBuf[len] = 0;

	unicodeBuf = NULL;	
	type = CHAR_STRING;
	updateHash();
}

gedString::gedString(unsigned long n)
{		
	CHAR_TYPE nBuf[32];
	sprintf(nBuf, "%u", n);

	charBuf = NULL;
	charBufSize = 0;

	len = strlen(nBuf);
	charBuf = (CHAR_TYPE *)ReallocChar(charBuf, 32);
	resetFirst32();

	strcpy(charBuf, nBuf);
	charBuf[len] = 0;

	unicodeBuf = NULL;	
	type = CHAR_STRING;
	updateHash();
}

gedString::gedString(double n, int nZeros)
{		
	CHAR_TYPE nBuf[64];
	charBuf = NULL;
	charBufSize = 0;
	

	//if(floor(scriptVars->textNumber) != scriptVars->textNumber) //bug in 1945.ged score if set initial textNumber != 0
	if(fabs(floor(n) - n) > TEXT_NUMBER_DOUBLE_LIMIT)
	{
		//Consider Real
		//Use g format to trunc trailing zeros
		
		//sprintf bug in release mode: -2 will show 00-2 for nZeros = 4
		if(n >= 0.0)
		{
			sprintf(nBuf, "%0*g", nZeros, n);
		}
		else
		{
			sprintf(nBuf, "-%0*g", nZeros-1, -n);
		}
	}
	else
	{
		//Consider Integer
		
		//Make integer
		n = (long)(n);
		
		//sprintf bug in release mode: -2 will show 00-2 for nZeros = 4
		if(n >= 0.0)
		{
			sprintf(nBuf, "%0*ld", nZeros, (long)n);
		}
		else
		{
			sprintf(nBuf, "-%0*ld", nZeros-1, (long)-n);
		}
	}


	//Set string
	len = strlen(nBuf);
	charBuf = (CHAR_TYPE *)ReallocChar(charBuf, 32);
	resetFirst32();

	strcpy(charBuf, nBuf);
	charBuf[len] = 0;

	unicodeBuf = NULL;	
	type = CHAR_STRING;
	updateHash();
}

gedString::~gedString()
{	
	clear();
}

void gedString::clear()
{
	invalidateUnicode();
	invalidateChar();

	len = 0;
	hash = 0;
	type = CHAR_STRING;
}

void gedString::invalidateChar()
{	
	if(charBuf)
	{
		FreeChar(charBuf);		
	}

	charBuf = NULL;
	charBufSize = 0;
}

void gedString::invalidateUnicode()
{	
	if(unicodeBuf)
	{
		free(unicodeBuf);		
	}

	unicodeBuf = NULL;		
}



void gedString::lower()
{	
	if(type == UNICODE_STRING && unicodeBuf)
	{
		Uint16 *s;
		for(s = unicodeBuf; *s; ++s)
		{
			*s = toLowerUnicode(*s);
		}

		invalidateChar();
	}
	else if(charBuf)	
	{
		CHAR_TYPE *s;
		for(s = charBuf; *s; ++s)
		{
			*s = toLowerChar((unsigned CHAR_TYPE)*s);
		}

		//Don't exists unicodeBuf in CHAR_TYPE strings
	}

	updateHash();
}

gedString& gedString::operator=(const CHAR_TYPE *string)
{		
	if(!string) 
	{
		clear();
		return *this;
	}
	
	if(string == charBuf) return *this;
	int newLen = strlen(string);
	
	if(len != newLen || !charBuf)
	{
		//Only allocate if is necessary
		len = newLen;
		charBuf = (CHAR_TYPE *)ReallocChar(charBuf, (len<32)?32:(len+1));
	}
	
	if(charBuf)
	{
		resetFirst32();
		
		strcpy(charBuf, string);
		charBuf[len] = 0;
		
		invalidateUnicode();
	}
	
	updateHash();
	
	type = CHAR_STRING;
	return *this;
}

gedString& gedString::operator=(const gedString& string)
{	
	if( &string != this )
	{
		//Only use this optimization after check all comparisons  
		//Causes bug in Add Variable (always edit)
		/*if(string.length() == 0)
		{
			clear();
			return *this;
		}*/

		type = string.type;
				
		//If string is NULL or empry, make this string empty too
		if(string.type == UNICODE_STRING)
		{
			if(!string.unicodeBuf)
			{
				clear();
				return *this;
			}
			
			if(string.unicodeBuf == unicodeBuf) return *this;
			
			if(len != string.length() || !unicodeBuf)
			{
				//Only allocate if is necessary							
				len = string.length();
				unicodeBuf = (Uint16 *)realloc(unicodeBuf, (len+1)*sizeof(Uint16));
			}

			if(unicodeBuf)
			{
				wcsCpy(unicodeBuf, string.unicodeBuf);
				unicodeBuf[len] = 0;
				
				if(TestUnicode())
				{
					invalidateChar();
				}
			}
		}
		else
		{
			if(!string.charBuf)
			{
				clear();
				return *this;
			}
			
			if(string.charBuf == charBuf) return *this;
			
			if(len != string.length() || !charBuf)
			{
				//Only allocate if is necessary				
				len = string.length();
				charBuf = (CHAR_TYPE *)ReallocChar(charBuf, (len<32)?32:(len+1));
			}
			
			if(charBuf)
			{
				resetFirst32();
				
				strcpy(charBuf, string.charBuf);
				charBuf[len] = 0;
				
				invalidateUnicode();
			}
		}
		
		updateHash();
	}

	return *this;
}


gedString& gedString::append(const gedString& string)
{	
	if(type == CHAR_STRING)
	{
		//Char
		if(string.type == CHAR_STRING)
		{
			//CHAR_TYPE + CHAR_TYPE
			if(!string.charBuf) return *this;
			
			int oldSize = len;
			len = oldSize + string.length();

			bool bEmpty = charBuf == NULL;
			charBuf = (CHAR_TYPE *)ReallocChar(charBuf, (len<32)?32:(len+1));

			if(bEmpty)
			{
				resetFirst32();
			}
			
			
			strcpy(charBuf + oldSize, string.charBuf);
			charBuf[len] = 0;

			invalidateUnicode();
		}
		else
		{
			//CHAR_TYPE + unicode

			//Convert this string to unicode
			updateUnicode();
		}		
	}


	//Don't use else (may be a conversion)
	if(type == UNICODE_STRING)
	{
		//Unicode
		if(string.type == CHAR_STRING)
		{
			//unicode + CHAR_TYPE

			//Convert the other string to unicode
			gedString aux(string);
			aux.updateUnicode();
			
			//unicode + unicode
			if(!aux.unicodeBuf) return *this;
			
			int oldSize = len;
			len = oldSize + aux.length();
			unicodeBuf = (Uint16 *)realloc(unicodeBuf, (len + 1)*sizeof(Uint16));
			
			wcsCpy((unicodeBuf + oldSize), aux.unicodeBuf);
			unicodeBuf[len] = 0;						
			
			
			invalidateChar();
			updateHash();
			return *this;
		}		
		
		
		//unicode + unicode
		if(!string.unicodeBuf) return *this;
		
		int oldSize = len;
		len = oldSize + string.length();
		unicodeBuf = (Uint16 *)realloc(unicodeBuf, (len + 1)*sizeof(Uint16));
		
		wcsCpy((unicodeBuf + oldSize), string.unicodeBuf);
		unicodeBuf[len] = 0;						
		
		
		invalidateChar();
	}

	updateHash();
	return *this;
}

gedString& gedString::append(const CHAR_TYPE *string, int n)
{	
	gedString ap(string , n);
	*this += ap;
	
	return *this;
}

gedString& gedString::operator+=(const gedString& string)
{	
	return append(string);
}



gedString& gedString::operator+=(const CHAR_TYPE c)
{	
	return append(c);
}

gedString& gedString::operator+=(const CHAR_TYPE *string)
{	
	return append(string);
}

gedString gedString::operator+(const CHAR_TYPE *string) const
{	
	gedString res(*this);
	res += gedString(string);

	return res;
}

gedString gedString::operator+(const CHAR_TYPE c) const
{	
	gedString res(*this);
	res += gedString(c);

	return res;
}

gedString gedString::operator+(const gedString& string) const
{	
	gedString res(*this);
	res += string;

	return res;
}




int gedString::find(const Uint16 *string, int ini) const
{	
	if(type == UNICODE_STRING)
	{		
		if(ini < 0 || ini >= len || !unicodeBuf || !string)
			return npos;
		
		Uint16 *substring, *str = unicodeBuf + ini;
		
		substring = (Uint16 *)wcsStr(str, string);
		if(!substring)
			return npos;
		
		return substring - unicodeBuf;
	}
	else
	{
		//Find unicode in a char string
		if(ini < 0 || ini >= len || !charBuf || !string)
			return npos;
		
		char *substring, *str = charBuf + ini;
		
		substring = wcsCharUnicode(str, string);
		if(!substring)
			return npos;
		
		return substring - charBuf;
	}
}

int gedString::find(const CHAR_TYPE *string, int ini) const
{	
	if(type == CHAR_STRING)
	{			
		if(ini < 0 || ini >= len || !charBuf || !string)
			return npos;
		
		CHAR_TYPE *substring, *str = charBuf + ini;
		
		substring = strstr(str, string);
		if(!substring)
			return npos;
		
		return substring - charBuf;
	}
	else
	{
		//Find char in a unicode string
		if(ini < 0 || ini >= len || !unicodeBuf || !string)
			return npos;
		
		Uint16 *substring, *str = unicodeBuf + ini;
		
		substring = wcsUnicodeChar(str, string);
		if(!substring)
			return npos;
		
		return substring - unicodeBuf;
	}
}

int gedString::replace(const CHAR_TYPE *oldString, const CHAR_TYPE *newString, int ini)
{	
	if(ini < 0 || ini >= len || !charBuf || !oldString || !newString)
		return npos;
	
	int i = find(oldString, ini);
	if(i == npos) return npos;


	gedString	first, last;

	if( i > 0)
	{
		first = substr(0, i);		
	}

	last = substr(i + strlen(oldString));

	*this = first + newString + last;

	return i;	
}

int gedString::replaceAll(const CHAR_TYPE *oldString, const CHAR_TYPE *newString)
{	
	if(!charBuf || !oldString || !newString)
		return npos;

	int i = 0, lastI;
	
	do
	{
		lastI = i;
		i = replace(oldString, newString, i);
	} while(i != npos);

	return lastI;	
}

int gedString::find(const CHAR_TYPE c, int ini) const
{	
	if(type == CHAR_STRING)
	{		
		if(ini < 0 || ini >= len || !charBuf)
			return npos;
		
		CHAR_TYPE *substring, *str = charBuf + ini;
		
		substring = strchr(str, c);
		if(!substring)
			return npos;
		
		return substring - charBuf;	
	}
	else
	{
		//Find char on unicode
		char s[2];
		s[0] = c;
		s[1] = 0;

		return find(s, ini);
	}
}




int gedString::find(const gedString& string, int ini) const
{	
	if(type == string.type )
	{
		if(type == CHAR_STRING)
		{
			return find(string.charBuf, ini);
		}
		else
		{
			return find(string.unicodeBuf, ini);
		}
	}
	else
	{
		if(type == CHAR_STRING)
		{
			return find(string.unicodeBuf, ini);
		}
		else
		{
			return find(string.charBuf, ini);
		}
	}
	
	return npos;
}

int gedString::find_no_case(gedString string, int ini) const
{	
	gedString s(*this);
	s.lower();
	string.lower();
	
	return s.find(string, ini);
}

int gedString::rfind(const Uint16 *string, int ini) const
{	
	/*if(type != UNICODE_STRING)
	{
		updateUnicode();
	}*/

	if(ini < 0 || ini >= len || !unicodeBuf || !string)
		return npos;

	Uint16 *substring, *str = unicodeBuf + ini;
	int pos;

	substring = (Uint16 *)wcsStr(str, string);
	if(!substring)
		return npos;

	pos = substring - str;

	while((substring = (Uint16 *)wcsStr((substring+1), string)) != NULL)
	{
		pos = substring - str;
	}

	return pos;
}

int gedString::rfind(const CHAR_TYPE *string, int ini) const
{	
	/*if(type != CHAR_STRING || !charBuf)
	{
		updateChar();
	}*/

	if(ini < 0 || ini >= len || !charBuf || !string)
		return npos;

	CHAR_TYPE *substring, *str = charBuf + ini;
	int pos;

	substring = strstr(str, string);
	if(!substring)
		return npos;

	pos = substring - str;

	while((substring = strstr(substring+1, string)) != NULL)
	{
		pos = substring - str;
	}

	return pos;
}


int gedString::rfind(const CHAR_TYPE c, int ini) const
{	
	/*if(type != CHAR_STRING || !charBuf)
	{
		updateChar();
	}*/

	if(ini < 0 || ini >= len || !charBuf)
		return npos;

	CHAR_TYPE *substring, *str = charBuf + ini;
	int pos;

	substring = strchr(str, c);
	if(!substring)
		return npos;

	pos = substring - str;

	while((substring = strchr(substring+1, c)) != NULL)
	{
		pos = substring - str;
	}

	return pos;
}

gedString gedString::substr(int ini, int n) const
{	
	//Don't need test ini + n > len due don't affect gedString(char, int)
	//Solve the but in ExtractSoundPath (only extract first sound)
	if(ini < 0) ini = 0;

	if(type == UNICODE_STRING)
	{
		if(ini >= len || !unicodeBuf)
			return "";
		
		if(n <= 0)
			return unicodeBuf + ini;
		
		return gedString(unicodeBuf + ini, n);
	}
	else
	{
		if(ini >= len || !charBuf)
			return "";
		
		if(n <= 0)
			return charBuf + ini;
		
		return gedString(charBuf + ini, n);
	}
}

bool gedString::compare(Uint16 *string) const
{	
	if(type != UNICODE_STRING) return false;

	if(!unicodeBuf)
	{
		if(!string)
			return true;
		else
			return false;
	}
	else if(!string)
	{
		return false;
	}

	return wcsCmp(unicodeBuf, string) == 0;
}

bool gedString::compare(CHAR_TYPE *string) const
{	
	if(type != CHAR_STRING && unicodeBuf)
	{
		return cmpUnicodeChar(unicodeBuf, string) == 0;
	}

	if(!charBuf)
	{
		if(!string)
			return true;
		else
			return false;
	}
	else if(!string)
	{
		return false;
	}

	return strcmp(charBuf, string) == 0;
}

int gedString::compare(const gedString &string, bool bCaseSensitive) const
{	
	if(type != CHAR_STRING && unicodeBuf)
	{
		return cmpUnicodeChar(unicodeBuf, string.charBuf);
	}

	if(!charBuf)
	{
		if(!string.charBuf)
			return 0;
		else
			return -1;
	}
	else if(!string.charBuf)
	{
		return +1;
	}

	if(bCaseSensitive)
	{
		return strcmp(charBuf, string.charBuf);
	}

#ifdef _WIN32_WCE
	return StriCmp(charBuf, string.charBuf);
#elif WIN32	
	return stricmp(charBuf, string.charBuf);	
#else
	return strcasecmp(charBuf, string.charBuf);
#endif
}




bool gedString::operator==(const Uint16 *string) const
{	
	return compare((Uint16 *)string);
}

bool gedString::operator==(const CHAR_TYPE *string) const
{	
	if(charBuf && *charBuf != *string)
	{
		//Speed up
		return false;
	}
	else
	{
		return compare((CHAR_TYPE *)string);
	}
}


bool gedString::operator==(const gedString& string) const
{	
	if(len <= 32 && string.len <= 32 && charBuf && string.charBuf)
	{
		U64 *w1 = (U64 *)charBuf, *w2 = (U64 *)string.charBuf;
		
		return ((w1[0] ^ w2[0]) | (w1[1] ^ w2[1]) | (w1[2] ^ w2[2]) | (w1[3] ^ w2[3])) == 0;
	}
	else if(len != string.len || (hash && string.hash && hash != string.hash))
	{
		return false;
	}
	/*else if(hash && hash == string.hash)
	{
		//It's safe?
		return true;
	}*/
	else if(type == string.type)
	{
		if(type == CHAR_STRING)
		{
			return compare(string.charBuf);
		}
		else
		{
			return compare(string.unicodeBuf);
		}
	}
	else
	{
		if(type == CHAR_STRING && string.type == UNICODE_STRING)
		{
			return cmpCharUnicode(charBuf, string.unicodeBuf);
		}
		else if(type == UNICODE_STRING && string.type == CHAR_STRING)
		{
			return cmpUnicodeChar(unicodeBuf, string.charBuf);
		}
	}


	return false;
}

bool gedString::operator>(const gedString& string) const
{	
	if(type == string.type)
	{
		if(type == UNICODE_STRING)
		{
			if(!unicodeBuf)
			{
				if(!string.unicodeBuf)
					return false;
				else
					return true;
			}
			else if(!string.unicodeBuf)
			{
				return true;
			}
			
			return wcsCmp(unicodeBuf, string.unicodeBuf) > 0;
		}
		else
		{
			if(!charBuf)
			{
				if(!string.charBuf)
					return false;
				else
					return true;
			}
			else if(!string.charBuf)
			{
				return true;
			}
			
			return strcmp(charBuf, string.charBuf) > 0;
		}
	}
	
	return false;
}

bool gedString::operator<(const gedString& string) const
{	
	if(type == string.type)
	{
		if(type == UNICODE_STRING)
		{
			if(!unicodeBuf)
			{
				if(!string.unicodeBuf)
					return true;
				else
					return false;
			}
			else if(!string.unicodeBuf)
			{
				return false;
			}
			
			return wcsCmp(unicodeBuf, string.unicodeBuf) < 0;
		}
		else
		{
			if(!charBuf)
			{
				if(!string.charBuf)
					return true;
				else
					return false;
			}
			else if(!string.charBuf)
			{
				return false;
			}
			
			return strcmp(charBuf, string.charBuf) < 0;
		}
	}

	return false;
}




int gedString::at(int pos) const
{	
	if(pos < 0 || pos >= len)
		return 0;

	if(type == CHAR_STRING)
	{
		if(charBuf)
		{
			return charBuf[pos];
		}		
	}
	else
	{
		if(unicodeBuf)
		{
			return unicodeBuf[pos];
		}
	}

	return 0;
}



void gedString::resize(int n)
{	
	if(n < 0) return;

	if(type == UNICODE_STRING)
	{		
		unicodeBuf = (Uint16 *)realloc(unicodeBuf, (n + 1)*sizeof(Uint16));
		unicodeBuf[n] = 0;
		
		invalidateChar();
	}
	else
	{
		bool bEmpty = charBuf == NULL;
		charBuf = (CHAR_TYPE *)ReallocChar(charBuf, (n<32)?32:(n+1));

		if(bEmpty)
		{
			resetFirst32();
		}

		charBuf[n] = 0;
	}

	len = n;
	updateHash();
}

void gedString::Read(SDL_RWops *stream, bool bForceNew)
{	
	//Create new string from file
	//lengh + string

	clear();

	type = -1;
	int len16 = SDL_ReadLE16(stream);

	if(len16 == 65535)
	{
		//Read the new 32 bit length
		len = SDL_ReadLE32(stream);
	}	
	else
	{
		//Old string
		len = len16;
	}
	

	if(len)
	{
		int allocLen = (len < 32)?32:(len + 1);

		if(GameControl::getLastGameFileVersion() >= 28 || //To read .ged
			KrResourceVault::getVersionDat() >= 4 ||//To read .dat
			bForceNew //avoid crash when load vars
			)
		{
			Uint8 stringType;
			if(GameControl::Read(stream, &stringType, sizeof(stringType), 1))
			{
				type = stringType;
				
				
				if(type == CHAR_STRING)
				{				
					charBuf = (CHAR_TYPE *)ReallocChar(charBuf, allocLen+1);
					memset(charBuf, 0, allocLen+1);
					SDL_RWread(stream, charBuf, len, 1);
					if(charBuf[len-1] == 0) len--; //Old read compatibility
					
					if(bReadEncriptedString) decript(true);
					
					charBuf[len] = 0;			
				}
				else
				{
					unicodeBuf = (Uint16 *)malloc(allocLen*sizeof(Uint16));
					memset(unicodeBuf, 0, allocLen*sizeof(Uint16));
					SDL_RWread(stream, unicodeBuf, len*sizeof(Uint16), 1);
					if(unicodeBuf[len-1] == 0) len--; //Old read compatibility
					
					if(bReadEncriptedString) decript();
					
					unicodeBuf[len] = 0;
					
					TestUnicode();
				}
			}
		}		
		else if(GameControl::getLastGameFileVersion() >= 26 || //To read .ged
			KrResourceVault::getVersionDat() >= 3//To read .dat
			) 
		{
			//Read unicode
			
			unicodeBuf = (Uint16 *)malloc(allocLen*sizeof(Uint16));
			memset(unicodeBuf, 0, allocLen*sizeof(Uint16));
			SDL_RWread(stream, unicodeBuf, len*sizeof(Uint16), 1);
			if(unicodeBuf[len-1] == 0) len--; //Old read compatibility
			
			if(bReadEncriptedString) decript();
			
			unicodeBuf[len] = 0;
			
			TestUnicode();
		}
		else
		{
			//Read old CHAR_TYPE

			unicodeBuf = (Uint16 *)malloc(allocLen*sizeof(Uint16));
			memset(unicodeBuf, 0, allocLen*sizeof(Uint16));

			charBuf = (CHAR_TYPE *)ReallocChar(charBuf, allocLen);
			memset(charBuf, 0, allocLen);

			SDL_RWread(stream, charBuf, len, 1);
			if(charBuf[len-1] == 0) len--; //Old read compatibility
			
			if(bReadEncriptedString) decript(true);
			 
			type = CHAR_STRING;
		}
	}

	updateHash();
}

void gedString::Write(SDL_RWops *stream) const
{	
	//16bit (old length) + 32bit length + string
	Uint8 stringType = type;

	SDL_WriteLE16(stream, 65535); //Tell to the reader to read the 32 bit length
	
	if(type == UNICODE_STRING)
	{
		encript();
		
		SDL_WriteLE32(stream, len);
		
		if(len) 
		{
			SDL_RWwrite( stream, &stringType, sizeof(stringType), 1);
			SDL_RWwrite( stream, unicodeBuf, len*sizeof(Uint16), 1 );
		}
		
		decript();
	}
	else
	{
		//Always save unicode
		/*gedString aux(charBuf);
		aux.updateUnicode();

		aux.encript();
		
		SDL_WriteLE16(stream, len);
		SDL_RWwrite(stream, &stringType, sizeof(stringType), 1);
		if(len) SDL_RWwrite( stream, aux.unicodeBuf, len*sizeof(Uint16), 1 );*/
		
		encript();

		SDL_WriteLE32(stream, len);
		if(len) 
		{
			SDL_RWwrite(stream, &stringType, sizeof(stringType), 1);
			SDL_RWwrite( stream, charBuf, len, 1 );
		}
	
		decript(true);
	}
}

int gedString::GetWriteLenght() const
{	
	//2 (old length) + 4 (length) + 1 (type) + len * (CHAR_TYPE or unicode)
	return 2 + 4 + 1 + len*(type == UNICODE_STRING?2:1);
}

void gedString::encript() const
{	
	//Very simple encription
	if(!len) return;

#if defined(GAME_EDITOR_PROFESSIONAL)
	Uint16 c = 0x80; //Save game or stand alone in Professional version
#elif defined(STAND_ALONE_GAME)
	Uint16 c = 0xD2; //Save stand alone in Demo version (0x72 don't encode "runleft"string)
#else
	Uint16 c = 0x80; //Save game in Demo version
	if(GameControl::Get()->getExportMode())
	{
		c = 0xD2; //Save stand alone in Demo version
	}
#endif

	//Encripy only the second byte (unicode string) 
	//to improve compression don't works

	if(type == UNICODE_STRING && unicodeBuf)
	{
		for(int i = 0; i < len; i++)
		{
			unicodeBuf[i] ^= c;
			c = unicodeBuf[i];
		}
	}
	else if(charBuf)
	{
		for(int i = 0; i < len; i++)
		{
			charBuf[i] ^= c;
			c = charBuf[i];
		}
	}
}

void gedString::decript(bool bDecriptChar) const
{	
	if(!len) return;

	if(bDecriptChar && charBuf)
	{
		for(int i = len-1; i > 0; i--)
		{
			charBuf[i] ^= charBuf[i-1];		
		}
		
		
#if defined(GAME_EDITOR_PROFESSIONAL)
		charBuf[0] ^= 0x80; //Read game or stand alone from Professional version
#elif defined(STAND_ALONE_GAME)
		charBuf[0] ^= 0xD2; //Read stand alone from Demo version
#else
		if(GameControl::Get()->getExportMode())
		{
			charBuf[0] ^= 0xD2; //Load stand alone in Demo version (to encrypt decript write)
		}
		else
		{
			charBuf[0] ^= 0x80; //Load game in Demo version
		}
#endif
	}
	else if(unicodeBuf)
	{
		for(int i = len-1; i > 0; i--)
		{
			unicodeBuf[i] ^= unicodeBuf[i-1];		
		}
		
		
#if defined(GAME_EDITOR_PROFESSIONAL)
		unicodeBuf[0] ^= 0x80; //Read game or stand alone from Professional version
#elif defined(STAND_ALONE_GAME)
		unicodeBuf[0] ^= 0xD2; //Read stand alone from Demo version
#else
		if(GameControl::Get()->getExportMode())
		{
			unicodeBuf[0] ^= 0xD2; //Load stand alone in Demo version (to encrypt decript write)
		}
		else
		{
			unicodeBuf[0] ^= 0x80; //Load game in Demo version
		}
#endif
	}
}

void gedString::trimRight()
{	
	int newLen = len;

	if(type == UNICODE_STRING && unicodeBuf)
	{		
		for(int i = len-1; i > 0; i--)
		{
			if( unicodeBuf[i] != ' ' && 
				unicodeBuf[i] != '\t' &&
				unicodeBuf[i] != '\r' &&
				unicodeBuf[i] != '\n')
				break;
			
			newLen--;
		}
		
		if(newLen != len)
		{
			unicodeBuf = (Uint16 *)realloc(unicodeBuf, (newLen + 1)*sizeof(Uint16));
			
			unicodeBuf[newLen] = 0;
			len = newLen;
		}

		invalidateChar();
	}
	else if(charBuf)
	{
		for(int i = len-1; i > 0; i--)
		{
			if( charBuf[i] != ' ' && 
				charBuf[i] != '\t' &&
				charBuf[i] != '\r' &&
				charBuf[i] != '\n')
				break;
			
			newLen--;
		}
		
		if(newLen != len)
		{
			charBuf = (CHAR_TYPE *)ReallocChar(charBuf, (newLen<32)?32:(newLen+1));
			charBuf[newLen] = 0;
			len = newLen;
		}
	}

	updateHash();
}

void gedString::trimLeft()
{	
	int i;

	if(type == UNICODE_STRING && unicodeBuf)
	{
		for(i = 0; i < len; i++)
		{
			if( unicodeBuf[i] != ' ' && 
				unicodeBuf[i] != '\t' &&
				unicodeBuf[i] != '\r' &&
				unicodeBuf[i] != '\n')
				break;
		}
		
		if(i > 0)
		{
			int newLen = len - i;
			Uint16 *tmp = (Uint16 *)malloc((newLen + 1)*sizeof(Uint16));
			
			wcsCpy(tmp, (unicodeBuf + i));
			
			free(unicodeBuf);
			unicodeBuf = tmp;
			
			unicodeBuf[newLen] = 0;
			len = newLen;
		}

		invalidateChar();
	}
	else if(charBuf)
	{
		for(i = 0; i < len; i++)
		{
			if( charBuf[i] != ' ' && 
				charBuf[i] != '\t' &&
				charBuf[i] != '\r' &&
				charBuf[i] != '\n')
				break;
		}
		
		if(i > 0)
		{
			int newLen = len - i;
			CHAR_TYPE *tmp = (CHAR_TYPE *)malloc((newLen<32)?32:(newLen+1));
			
			strcpy(tmp, charBuf + i);
			
			FreeChar(charBuf);
			charBuf = tmp;
			
			charBuf[newLen] = 0;
			len = newLen;
		}
	}

	updateHash();
}

void gedString::erase(int pos, int n)
{	
	if(pos < 0 || n < 0) return;

	if(type == UNICODE_STRING && unicodeBuf)
	{
		Uint16 *tmp = (Uint16 *)malloc((len + 1)*sizeof(Uint16));
		
		int i0 = 0;
		for(int i = 0; i < len; i++)
		{
			if(i < pos || i >= pos + n)
			{
				tmp[i0] = unicodeBuf[i];
				i0++;
			}
		}
		
		free(unicodeBuf);
		
		unicodeBuf = (Uint16 *)realloc(tmp, (i0 + 1)*sizeof(Uint16));
		unicodeBuf[i0] = 0;
		len = i0;
		
		invalidateChar();
	}
	else if(charBuf)
	{
		CHAR_TYPE *tmp = (CHAR_TYPE *)malloc((len<32)?32:(len+1));
		memset(tmp, 0, (len<32)?32:(len+1));
		
		int i0 = 0;
		for(int i = 0; i < len; i++)
		{
			if(i < pos || i >= pos + n)
			{
				tmp[i0] = charBuf[i];
				i0++;
			}
		}
		
		FreeChar(charBuf);
		
		charBuf = (CHAR_TYPE *)ReallocChar(tmp, (i0<32)?32:(i0+1));
		charBuf[i0] = 0;
		len = i0;
	}

	updateHash();
}



gedString gedString::GetFileName() const
{	
	//Returns the file name, if is a path

	int i = rfind('\\');
	if(i == gedString::npos)
		i = rfind('/');

	return substr(i+1);
}

gedString gedString::GetFilePath() const
{	
	//Returns the file path, if is a path

	int i = rfind('\\');
	if(i == gedString::npos)
		i = rfind('/');

	if(i != gedString::npos)
	{
		return substr(0, i);
	}

	return "";
}

void gedString::updateChar()
{	
	if(charBuf)
	{
		FreeChar(charBuf);
		charBuf = NULL;
		charBufSize = 0;
	}

	if(unicodeBuf)
	{
		charBuf = (CHAR_TYPE *)ReallocChar(charBuf, (len<32)?32:(len+1));
		resetFirst32();

		int i;		
		for(i = 0; i < len; i++)
		{
			charBuf[i] = (CHAR_TYPE)unicodeBuf[i];			
		}
		
		charBuf[i] = 0;
	}

	updateHash();
}

void gedString::updateUnicode()
{	
	if(unicodeBuf)
	{
		free(unicodeBuf);
		unicodeBuf = NULL;
	}

	if(charBuf)
	{
		unicodeBuf = (Uint16 *)malloc((len+1)*sizeof(Uint16));
		int i;
		for(i = 0; i < len; i++)
		{
			unicodeBuf[i] = (U16)charBuf[i];
		}
		
		unicodeBuf[i] = 0;

		//Type upgrade
		type = UNICODE_STRING; 
	}

	
	updateHash();
}

const Uint16 *gedString::c_str16() 
{	
	if(!unicodeBuf && type == CHAR_STRING)
	{
		updateUnicode();
	}

	return unicodeBuf!=NULL?unicodeBuf:&emptyString;
}

const CHAR_TYPE *gedString::c_str()
{	
	if(!charBuf && type == UNICODE_STRING)
	{
		updateChar();
	}

	return charBuf!=NULL?charBuf:"";
}

void gedString::updateHash()
{	
	//Clear hash here
	//Sove bug:
	//gedString oi("ok");
	//oi = "Actor -> Collision (Any side of LeftWire)";
	//oi.hash = 225 (hash of "ok" string)
	hash = 0;

	if(len && len < 32)
	{
		GlStringHash sHash(*this);
		hash = sHash.HashValue();
	}
}


bool gedString::TestUnicode()
{	
	bool bNeedUnicode = false;
	
	if(unicodeBuf)
	{
		for(int i = 0; i < len; i++)
		{
			if(unicodeBuf[i] > 255)
			{
				bNeedUnicode = true;
				break;
			}
		}
	}
	
	if(bNeedUnicode)
	{
		type = UNICODE_STRING;
	}
	else
	{
		updateChar();
		invalidateUnicode();
		type = CHAR_STRING;
	}

	return bNeedUnicode;
}

bool gedString::onlyBlank()
{	
	if(type == UNICODE_STRING && unicodeBuf)
	{
		for(int i = 0; i < len; i++)
		{
			if( unicodeBuf[i] != ' ' && 
				unicodeBuf[i] != '\t' &&
				unicodeBuf[i] != '\r' &&
				unicodeBuf[i] != '\n')
			{
				return false;
			}
		}
	}
	else if(charBuf)
	{
		for(int i = 0; i < len; i++)
		{
			if( charBuf[i] != ' ' && 
				charBuf[i] != '\t' &&
				charBuf[i] != '\r' &&
				charBuf[i] != '\n')
			{
				return false;
			}
		}
	}

	return true;
}

bool gedString::isNumber()
{		
	if(type == UNICODE_STRING && unicodeBuf)
	{
		for(int i = 0; i < len; i++)
		{
			if( (unicodeBuf[i] < '0' || unicodeBuf[i] > '9') &&
				unicodeBuf[i] != '.')
			{
				return false;
			}
		}
	}
	else if(charBuf)
	{
		for(int i = 0; i < len; i++)
		{
			if( (charBuf[i] < '0' || charBuf[i] > '9') &&
				charBuf[i] != '.')
			{
				return false;
			}
		}
	}


	return true;
}

CHAR_TYPE *gedString::ReallocChar(CHAR_TYPE *m, int size)
{		
	if(size > charBufSize || !m)
	{
		if(size <= MAX_STACK_BUF_SIZE) 
		{
			if(m != stackBuf && m)
			{
				memcpy(stackBuf, m, size);
			}

			m = stackBuf;
			charBufSize = MAX_STACK_BUF_SIZE;
		}
		else
		{
			if(m == stackBuf)
			{
				m = (CHAR_TYPE *)malloc(size);
				memcpy(m, stackBuf, charBufSize);
			}
			else
			{
				m = (CHAR_TYPE *)realloc(m, size);
			}

			charBufSize = size;
		}
	}	
	
	return m;
}

void gedString::FreeChar(CHAR_TYPE *m)
{	
	//Don't compare m with stackBuf
	//Will be invalid on ARM (stackBuf address may be different from creation)
	if(charBufSize > MAX_STACK_BUF_SIZE)
	{
		GLASSERT(m != stackBuf); //Valid on x86 only
		if(m) free(m);		
	}

	charBufSize = 0;
}