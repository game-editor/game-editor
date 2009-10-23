/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues

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
<http://game-editor.com/License>, thats will help us to pay our developers 
<http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>.

***************************************************************************/


// GenericScript.h: interface for the GenericScript class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GENERICSCRIPT_H__11135726_6474_4989_81F7_9335A2EB6B9A__INCLUDED_)
#define AFX_GENERICSCRIPT_H__11135726_6474_4989_81F7_9335A2EB6B9A__INCLUDED_

#include "gedString.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Actor.h"

class stVarInfo;
class stGenericCode
{
public:
	stGenericCode() {bAddedWithErrors = false;}
	stGenericCode(const gedString& _code, bool _bAddedWithErrors)
	{
		code = _code;
		bAddedWithErrors = _bAddedWithErrors;
	}

	void operator=(const stGenericCode &copy)
	{
		if( &copy != this )
		{
			code = copy.code;
			mapFunctionPrototypes = copy.mapFunctionPrototypes;
			bAddedWithErrors = copy.bAddedWithErrors;
		}
	}

	virtual ~stGenericCode() {}
	

	gedString code;
	MapString mapFunctionPrototypes;
	bool bAddedWithErrors;
};

class stNameCode
{
public:
	stNameCode() {bAddedWithErrors = false;}
	stNameCode(const gedString& _name, const gedString& _code, bool _bAddedWithErrors = false)
	{
		code = _code;
		name = _name;
		bAddedWithErrors = _bAddedWithErrors;
	}

	void operator=(const stNameCode &copy)
	{
		if( &copy != this )
		{
			name = copy.name;
			code = copy.code;		
			bAddedWithErrors = copy.bAddedWithErrors;
		}
	}

	bool operator==(const stNameCode &copy)
	{
		if( name == copy.name &&
			code == copy.code)
			return true;

		return false;
	}

	virtual ~stNameCode() {}
	

	gedString name;
	gedString code;	
	bool bAddedWithErrors;
};

typedef GlSList<stNameCode> ListCode;  
typedef GlSListIterator<stNameCode> ListCodeIterator;

typedef GlMap< gedString, stGenericCode, GlStringHash >		MapCode;
typedef GlMapIterator< gedString, stGenericCode, GlStringHash >		MapCodeIterator;

class GenericScript  
{
public:
	

#ifndef STAND_ALONE_GAME
	static bool ParserGenericScripts(gedString &scriptError, ListCode &parserOrder);
	static bool ParserAll();
	static void Populate(ListPop *list);
	static void Populate(ListString &list);
	static void Save(SDL_RWops *src);
	static bool RemoveAndVerify(const gedString& name, bool bIgnoreErrors = false);	
	static bool RemoveAndVerifyVariable(const gedString& variable, int scope, stVarInfo *newVar = NULL, int newScope = -1, bool bIgnoreErrors = false);
	static void SaveSoundTo(SDL_RWops *exeFile, MapAudioFileIndex &audioIndex);
#endif //#ifndef STAND_ALONE_GAME

	static void ClearGenericScripts();
	static void ParserLoadedCode();
	
	static void Remove(const gedString& name);
	
	
	static gedString Get(const gedString& name);
	static bool GetCodeError(const gedString &name);
	static void Load(SDL_RWops *src, bool bMergeGames);

	bool Add(const gedString& code,  const gedString& name, bool bIgnoreErrors = false);
	GenericScript();
	virtual ~GenericScript();

	static const gedString& GetError() {return error;};

private:
#ifndef STAND_ALONE_GAME
	static int RemoveLocalVar(const gedString& variable);
	static stVarInfo RemoveGlobalVar(const gedString& variable);
	static void RestoreVar(const gedString &variable, int type);
	static void RestoreVar(const stVarInfo& varInfo);
#endif //#ifndef STAND_ALONE_GAME

	static void ParserAllFunctionPrototypes();


	void AddCode(const gedString &code, const gedString &name, bool bAddedWithErrors);
	static void RestoreCode(const gedString& name);
	static void RemoveCode(const gedString& name, bool bRemovePrototypes = true);

	static gedString error;

	static MapCode mapCode;
	static ListCode listLoadedCodes;
	static ListString listLoadedPrototypes;
};

#endif // !defined(AFX_GENERICSCRIPT_H__11135726_6474_4989_81F7_9335A2EB6B9A__INCLUDED_)
