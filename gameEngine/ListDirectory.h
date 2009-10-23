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


// ListDirectory.h: interface for the ListDirectory class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_LISTDIRECTORY_H__F7B6D562_1523_4A86_86A9_7D9C847DE250__INCLUDED_)
#define AFX_LISTDIRECTORY_H__F7B6D562_1523_4A86_86A9_7D9C847DE250__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ListPop.h"
#include "system.h"


#ifdef WIN32
#define USE_SYSTEM_FILE_DIALOG
#endif


class ListDirectory : public ListPop  
{
public:
	static gedString GetMultipleFileName(const gedString& file);
	void ReadDrives();
	gedString GetFullPath();
	void Populate(bool bForward = true);
	ListDirectory(int x = 0, int y = 0,
			   int width = 0, int height = 0,
			   int listpopId = -1,
			   Actor *parent = NULL,
			   gedString buttonText = "");
	virtual ~ListDirectory();

	
	bool OnMouseButtonDown(int x, int y, Uint8 button);
	bool OnList(ListPop *list, int index, gedString &text, int listId);

	void SetFilter(const gedString &filter);
	void AddFilter(const gedString &filter);
	bool Filter(gedString file);
	void setShowDir(bool value) {bShowDirs = value;}
	void setShowExtension(bool value) {bShowExtension = value;}

	void setCanUseSystemFileDialog(bool value) {bCanUseSystemFileDialog = value;}
	void SetSave() {bSave = true;}
	void SetSaveName(gedString name) {saveName = name;}

private:
	long handle;


	gedString filter, saveName;
	static char drives[27];
	static bool bReadDrives;
	ListString multiFilter;

	bool bShowDirs, bShowExtension, bSave, bCanUseSystemFileDialog;
};


#endif // !defined(AFX_LISTDIRECTORY_H__F7B6D562_1523_4A86_86A9_7D9C847DE250__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME