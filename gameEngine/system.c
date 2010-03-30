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


/*******************************************************************************
  system.c: OS-independent functions (implementation)
            Author: Odilon Nelson
            Last modified: 02/05/03
*******************************************************************************/

#define GEDSYSTEM_C

#include "system.h"


//Directory name convertion
void correctDirectoryName(const char *oldName, char *newName)
{
	int ant = 0;
	while(*oldName)
	{
		if(ant != DIR_SEP || (*oldName != '/' && *oldName != '\\'))
		{
			*newName = *oldName;
			
			if(*newName == '/' || *newName == '\\')	
				*newName = DIR_SEP;
			
			ant = *newName;
			newName++;
		}
		
		oldName++;
	}

	*newName = *oldName;
}


#if __iPhone__
void openUrl(const char * b) { return 0; };
#endif


#ifdef __SYMBIAN32__
#  include "system_symbian.c"
#elif _WIN32_WCE
# include "system_pocketpc.c"
#elif __APPLE__
# include "system_iphone.c"
#elif WIN32
# include "system_windows.c" 
#else
#  include "system_linux.c"
#endif
