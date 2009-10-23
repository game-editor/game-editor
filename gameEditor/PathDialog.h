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


// PathDialog.h: interface for the PathDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_PATHDIALOG_H__66571D81_A6EE_4390_9175_9416A5FCEB98__INCLUDED_)
#define AFX_PATHDIALOG_H__66571D81_A6EE_4390_9175_9416A5FCEB98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class PathPoint;

enum
{
	BT_CLOSE,
	BT_ADDPATH,
	BT_REMPATH
};

class PathDialog : public Panel  
{
public:
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void SetPaths();
	static void Call();
	void OnButton(Button *button, int buttonId);
	PathDialog();
	virtual ~PathDialog();

	static PathDialog *getPathDialog() {return pathDialog;}
	static void setActualPath(Path *path) {actualPath = path;}
	static Path *getActualPath() {return actualPath;}
	PathPoint *getPathPoint();

private:
	static PathDialog *pathDialog;
	ListPop *listPath;
	PathPoint *pathDraw;
	static Path *actualPath;
};

#endif // !defined(AFX_PATHDIALOG_H__66571D81_A6EE_4390_9175_9416A5FCEB98__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME