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





#ifdef __SYMBIAN32__
#  include "system_symbian.c"
#elif _WIN32_WCE
# include "system_pocketpc.c"
#elif WIN32
# include "system_windows.c" 
#else
#  include "system_linux.c"
#endif
