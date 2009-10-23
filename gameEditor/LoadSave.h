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


// LoadSave.h: interface for the LoadSave class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_LOADSAVE_H__F69676C5_0661_4778_A547_7AB8155C4608__INCLUDED_)
#define AFX_LOADSAVE_H__F69676C5_0661_4778_A547_7AB8155C4608__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"


enum
{
	LOAD_DIALOG,
	MERGE_DIALOG,
	SAVE_DIALOG,
	LOADSAVE_OK,
	LOADSAVE_CLOSE
};

class LoadSave : public Panel  
{
public:
	void Init(int command, gedString title, gedString confirmButtonText);
	void OnButton(Button *button, int buttonId);
	LoadSave(bool bExtendHeight = false);
	virtual ~LoadSave();
	
	bool OnList(ListPop *list, int index, gedString &text, int listId);

	void SetFilter(const gedString &filter);
	void AddFilter(const gedString &filter);
	void SetText(gedString str);
	gedString GetText();
	gedString GetDirectory();
	bool IsOk() {return bOk;}

private:
	EditBox *name;		
	bool bOk;

protected:
	int command;
	ListDirectory *dir;
};

#endif // !defined(AFX_LOADSAVE_H__F69676C5_0661_4778_A547_7AB8155C4608__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME