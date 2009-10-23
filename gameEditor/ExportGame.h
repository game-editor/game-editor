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


// ExportGame.h: interface for the ExportGame class.
//
//////////////////////////////////////////////////////////////////////

#if /*defined(GAME_EDITOR_PROFESSIONAL) &&*/ defined(WIN32) && !defined(STAND_ALONE_GAME)

#if !defined(AFX_EXPORTGAME_H__2A2119ED_8FB7_4345_97CA_082E905FB445__INCLUDED_)
#define AFX_EXPORTGAME_H__2A2119ED_8FB7_4345_97CA_082E905FB445__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class ExportGame : public Panel  
{
public:
	void OnButton(Button *button, int buttonId);
	ExportGame();
	virtual ~ExportGame();
	
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnEditChar(EditBox *edit, SDLKey key, int line);

	static void ClearExportName() {exportName.clear();}


private:
	EditBox *name;
	ListPop *listSystem;
	ListDirectory *dir;	

	static gedString exportName;
};

#endif // !defined(AFX_EXPORTGAME_H__2A2119ED_8FB7_4345_97CA_082E905FB445__INCLUDED_)


#endif ///*!defined(GAME_EDITOR_PROFESSIONAL) &&*/ !defined(STAND_ALONE_GAME)
