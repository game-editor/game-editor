// ListDirectory.cpp: implementation of the ListDirectory class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "ListDirectory.h"
#include "GameControl.h"
#include "ListPopSelect.h"


#include "../gameEditor/Tutorial.h"

#ifdef USE_SYSTEM_FILE_DIALOG
extern "C" HWND SDL_Window;
extern "C" void SDL_ResetMouse(void);
extern "C" int SDL_PrivateMouseButton(Uint8 state, Uint8 button,Sint16 x,Sint16 y);

/*UINT CALLBACK OFNHookProc(
  HWND hdlg,      // handle to child dialog window
  UINT uiMsg,     // message identifier
  WPARAM wParam,  // message parameter
  LPARAM lParam   // message parameter
)
{
	return 0;
}*/

extern "C" void SDL_FlipToGDISurface();
extern "C" void *SDL_GetModuleHandle();
extern "C" LPSTR SDL_Appname;

gedString FileDialog(ListString& multiFilter, bool bOpen, gedString saveName = "")
{
	gedString filter;
	OPENFILENAME ofn;
	char filebuf[512];

	memset(filebuf, 0, 512);
	memset(&ofn, 0, sizeof(OPENFILENAME));

	// Set the correct size of the struct
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	GetVersionEx( &osvi );

	if ( osvi.dwMajorVersion >= 5 )
	{
		// We are running Win2000 or WinXP or Win .Net Server
		ofn.lStructSize = sizeof( OPENFILENAME );
	}
	else
	{
		// We are running Win98 or WinNT
		ofn.lStructSize = 76;
	}

	//Flush message queue (solve the bug "Don't open the dialog at second time")
	MSG  msg;
	while ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) {
		if ( msg.message == WM_QUIT ) break;
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	//File dialog behind the draw window? Use FlipToGDISurface
	SDL_FlipToGDISurface();

	//Use this window to make sure the dialog doesn't will appear behind the main window
	HWND hPlace = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW, SDL_Appname, NULL, WS_VISIBLE, -30, -30, 1, 1, NULL, NULL, (HINSTANCE)SDL_GetModuleHandle(), NULL);

	ofn.hwndOwner = hPlace;//(HWND)GetMainWindow(); //GetMainWindow don't works in some machines
	WaitCursor wait; //Use if hwndOwner is not GetMainWindow()

	//Change the windowproc of the SDL window to avoid click in the editor and lost the file dialog focus
	WNDPROC sdlWindowProc = (WNDPROC)GetWindowLong(SDL_Window, GWL_WNDPROC);
	SetWindowLong(SDL_Window, GWL_WNDPROC, (LONG)DefWindowProc);
	
	if(multiFilter.Count())
	{
		//Make filter
		int i0;

		//Description		
		for(i0 = 0; i0 < multiFilter.Count(); i0++)
		{			
			//Type
			filter += ".";
			filter += multiFilter[i0];
			if(i0 < multiFilter.Count() - 1) filter += "; ";
		}

		filter += (char)0;

		//Type
		for(i0 = 0; i0 < multiFilter.Count(); i0++)
		{			
			filter += "*.";
			filter += multiFilter[i0];
			filter += ';';
		}
		
		if(!filter.empty())
		{
			//Double NULL terminator
			filter += (char)0;
			
			ofn.lpstrFilter = filter.c_str();
		}
	}

	//File buf
	ofn.lpstrFile = filebuf;
	ofn.nMaxFile = 511;

	//Current directory
	char currentPath[PATH_LIMIT];
	if(ged_getcwd(currentPath, PATH_LIMIT))
	{
		ofn.lpstrInitialDir = currentPath;
	}

	//Flags
	int res = 0;
	DWORD error = 0;
	

	if(bOpen)
	{
		//Open
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;


		//ofn.Flags |= OFN_ENABLEHOOK | OFN_EXPLORER;
		//ofn.lpfnHook = OFNHookProc;

		res = GetOpenFileName(&ofn);
	}
	else
	{
		//Save
		if(!saveName.empty()) strcpy(filebuf, saveName.c_str());
		else filebuf[0] = 0;

		ofn.lpstrFile = filebuf;
		ofn.nMaxFile  = 511;

		ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
		res = GetSaveFileName(&ofn);
	}

	if(hPlace) DestroyWindow(hPlace);

	//Restore sdl windowproc
	SetWindowLong(SDL_Window, GWL_WNDPROC, (LONG)sdlWindowProc);

	//Without this can't enter text anymore
	SendMessage(SDL_Window, WM_SETFOCUS, (WPARAM)SDL_Window, 0); 
	

	//Solve the redraw problem in some machines
	engine->InvalidateScreen();

	if(res)
	{
		gedString file(filebuf);
		file = file.GetFileName(); //Solve bug when select: C:\Downloads\Jogos ó\Mario\super mario.ged

		if(!CanShowString(file))
		{
			//Unicode
			//Get the file in old 8.3 format
			char shortPath[512];
			if(GetShortPathName(filebuf, shortPath, 511))
			{
				file = shortPath;
				file = file.GetFileName();
			}
		}


		//return file.GetFileName();
		return file;
	}


	return "";
}

#endif


bool ListDirectory::bReadDrives = false;
char ListDirectory::drives[27];

ListDirectory::ListDirectory(int x, int y,
			   int width, int height,
			   int listpopId,
			   Actor *parent,
			   gedString buttonText)
	: ListPop(x, y, width, height, listpopId, parent, buttonText)
{
	handle = 0;
	bSave = false;
	bCanUseSystemFileDialog = true;
	bListDirectory = true;


	filter = "*.*";
	bShowExtension = bShowDirs = true;
	
	if(Tutorial::IsOff())
	{
		ReadDrives();
	}
	else
	{
		bReadDrives = false;
		memset(drives, 0, 27);
	}
}

ListDirectory::~ListDirectory()
{
	ged_findclose(handle);
}


gedString ListDirectory::GetFullPath()
{
	if(GetSelectedIndex() == -1) return "";

	char buf[PATH_LIMIT];
	buf[0] = 0;

	if(ged_getcwd(buf, PATH_LIMIT))
	{
		gedString path = buf;
		if(buf[strlen(buf)-1] == DIR_SEP)
		{
			path += GetText();
		}
		else
		{
			path += DIR_SEP;
			path += GetText();
		}

		if(bShowExtension)
			return path;
		else
			return path + "." + multiFilter.Back();
	}

	return "";
}


bool ListDirectory::OnMouseButtonDown(int x, int y, Uint8 button)
{
	if(button != 255) 
	{
		if(!getParent()->OnListButtonDown(this, listpopId))
		{
			//Don't execute
			return false;
		}
	}

#ifdef USE_SYSTEM_FILE_DIALOG
	if(Tutorial::IsOff() && bCanUseSystemFileDialog)
	{
		gedString fileName(FileDialog(multiFilter, !bSave, saveName)); 
		if(!fileName.empty())
		{
			Populate();

			if(!bShowExtension)
			{			
				gedString s(fileName);
				int i = s.find('.');
				if(i != gedString::npos)
				{
					s = s.substr(0, i);
				}
				
				fileName = s;
			}

			ListPop::OnList(this, -1, fileName, -1);
		}

		//Need this to release the mouse capture
		//Solve the bug "lost cursor when open merge, open file dialog, close, click on grid"
		//SDL_ResetMouse(); //Don't works
		::PostMessage(SDL_Window, WM_LBUTTONUP, 0, 0);
		return false;
	}
#endif

	if(firstListItem == -1) Populate();

	return ListPop::OnMouseButtonDown(x, y, button);
}

bool ListDirectory::OnList(ListPop *list, int index, gedString &text, int listId)
{
	int realFps = GameControl::Get()->getRealFrameRate();
	if(realFps <= 0) realFps = 1;
	if(realFps > 30) realFps = 30;

	int interval = (int)(1000.0/realFps);

	if(text.c_str()[0] == '(' && text.c_str()[text.length() - 1] == ')')
	{
		text.set(text.length() - 1, 0);
		chdir(&text.c_str()[1]);

		//Don't use timers (error in tutorial execution)
		ged_findclose(handle);
		handle = 0;
		
		Reset();
		Populate();
		
		ListPop::OnMouseButtonDown(xMouse, yMouse, 1);
	}
	else
	{
		ListPop::OnList(list, index, text, listId);
	}

	return true;
}

void ListDirectory::SetFilter(const gedString &filter)
{
	this->filter = filter;	

	//Avoid *.ged returns *.gedt...

	if(filter.find('.') != gedString::npos)
	{
		gedString ext(filter.substr(filter.find('.') + 1, filter.length() - filter.find('.') - 1));

		if(ext.find('*') == gedString::npos)
		{
			multiFilter.PushBack(ext);
		}
	}
}

void ListDirectory::AddFilter(const gedString &filter)
{
	this->filter = "*.*";
	multiFilter.PushBack(filter);	
}


bool ListDirectory::Filter(gedString file)
{
	if(!multiFilter.Count()) return true;

	file.lower();

	
	for(int i0 = 0; i0 < multiFilter.Count(); i0++)
	{
		if(file.length() > multiFilter[i0].length() && file.substr(file.length() - multiFilter[i0].length()) == multiFilter[i0])
		{
			return true;
		}
	}

	return false;
}

void ListDirectory::Populate(bool bForward)
{
	WaitCursor wait;

	if(!handle)
	{	
		if(bShowDirs)
		{
			char buf[PATH_LIMIT];		
			
			//Add drives
			if(bReadDrives && bShowDirs)
			{
				for(int drive = 1; drive <= 26; drive++)
				{
					if(drives[drive])
					{
						sprintf(buf, "(%c:)\0", drive + 'A' - 1);
						AddText(buf);
					}
				}
			}			
			
			//Read dirs
			handle = ged_findfirst("*.*", A_DIREC | A_HIDDEN);
			if(handle != GED_NOTFOUND)
			{	
				do
				{
					if(strcmp(ged_filename(handle), ".") != 0)
					{
						sprintf(buf, "(%s)\0", ged_filename(handle));
						AddText(buf);
					}
					
				} while((handle = ged_findnext(handle)) != GED_NOTFOUND);
				
				ged_findclose(handle);
				handle = ged_findfirst(filter.c_str(), A_ANY);
			}	
		}
		else
		{
			handle = ged_findfirst(filter.c_str(), A_ANY);
		}
	}

	if(handle != GED_NOTFOUND)
	{	
		do
		{
			if(!(ged_attrib(handle) & A_DIREC) && Filter(ged_filename(handle)))
			{
				char *fileName = ged_filename(handle);
				
				if(bShowExtension)
				{
					AddText(fileName);				
				}
				else
				{
					gedString s(fileName);
					int i = s.find('.');
					if(i != gedString::npos)
					{
						s = s.substr(0, i);
					}

					AddText(s);
				}
			}
			

		} while((handle = ged_findnext(handle)) != GED_NOTFOUND);

		if(handle == GED_NOTFOUND)
		{
			ged_findclose(handle);
			handle = 0;
		}
	}

	if(!Tutorial::IsOff())
	{
		if(Tutorial::Get()->InRecMode())
		{
			Tutorial::Get()->AddList(this);
		}
		else if(Tutorial::Get()->InPlayMode())
		{
			Tutorial::Get()->GetList(this);
		}
	}
	else
	{
		//Solve the bug "The files don't always seem to sort alphabetically"
		Sort();
	}
}


void ListDirectory::ReadDrives()
{
#ifdef ___WIN32___
	if(!bReadDrives)
	{
		memset(drives, 0, 27);
			
		//Don't causes error on removable devices
		unsigned long   dwDrives;
		int     i;
		dwDrives = GetLogicalDrives();

		if(dwDrives)
		{				
			for ( i = 2; i <= 25; i++ ) //Start from drive C
			{
				if ( dwDrives & (1 << i) )
				{
					drives[i + 1] = 1;
				}
			}
		}
		else
		{
			//Use the old method
			//Causes error when boot with a CD on drive and remove it
			//"There is no disk in the drive. Please insert a disk into drive..."

			
			int curdrive = getdrive();
			
			char path[PATH_LIMIT];
			ged_getcwd(path, PATH_LIMIT);
			
			//Don't use this: causes error when boot with a CD on drive and remove it
			for(int drive = 3; drive <= 26; drive++)
			{
				if( !chdrive( drive ) )
				{
					drives[drive] = 1;
				}
			}
			
			chdrive( curdrive );
			chdir(path);
		}
		
		
		bReadDrives = true;
	}
#endif
}

gedString ListDirectory::GetMultipleFileName(const gedString& file)
{
	/*
	If the file name goes of the type lettersnumbers...
	Substitutes numbers for *
	*/

	gedString newName(file);
	gedString aux(file);
	const char *buf = aux.c_str();
	
	int ini = -1, fin = -1;
	for(int i = file.length() - 1; i >= 0; i--)
	{
		if(fin == -1 && isdigit(buf[i]))
		{
			fin = i;
		}
		else if(fin != -1 && !isdigit(buf[i]))
		{
			ini = i+1;
			break;
		}
	}

	if(ini != -1 && fin != -1 && fin >= ini)
	{
		newName = file.substr(0, ini) + "*" + file.substr(fin+1);

		//Verify if they are files really multiple
		int nFiles = 0;
		long h = ged_findfirst(newName.c_str(), A_ANY);
		if(h != -1)
		{	
			do
			{
				nFiles++;						
			} while((h = ged_findnext(h)) != GED_NOTFOUND);
			
			ged_findclose(h);
		}	

		if(nFiles <= 1) return file; //Not multiple
	}	

	return newName;
}


#endif //#ifndef STAND_ALONE_GAME