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
