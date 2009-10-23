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


// EditorDirectory.cpp: implementation of the EditorDirectory class.
//
//////////////////////////////////////////////////////////////////////

#include "GameControl.h"


EditorDirectory::EditorDirectory(gedString newPath)
{	
	//Change path to newPath or editor path
	//Restore current path when destroyed

	oldpath[0] = 0;

#ifndef STAND_ALONE_GAME
	ged_getcwd(oldpath, PATH_LIMIT);

	gedString aux;
	
	if(newPath.empty()) aux = GameControl::getEditorPath();
	else aux = newPath;

	chdir(aux.c_str());
#endif
}

EditorDirectory::~EditorDirectory()
{
#ifndef STAND_ALONE_GAME
	chdir(oldpath);
#endif
}
