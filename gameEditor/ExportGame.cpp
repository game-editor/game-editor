/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com, http://game-editor.com

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
Be a Game Editor developer: Be a Game Editor developer: <http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>  .

***************************************************************************/


// ExportGame.cpp: implementation of the ExportGame class.
//
//////////////////////////////////////////////////////////////////////

#if /*defined(GAME_EDITOR_PROFESSIONAL) &&*/ !defined(STAND_ALONE_GAME)

#include "ExportGame.h"
#include "GameControl.h"
#include "Config.h"


//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_EXPORT			"\
You can export your game in a single executable file with all game data, animations and sounds.\n\
Available formats:\n\
\n\
Windows Executable: Windows executable file (95, 98, Me, NT, 2000, XP)\n\
Mac OSX Executable: Mac OSX 10.5 and above\n\
Pocket PC / Handheld PC Executable: Executable file compatible with Pocket PC and Handheld PC\n\
    Supports:\n\
      - Windows CE OS 3.0 (Pocket PC 2002, Windows Mobile 2003, 2005) and later\n\
      - ARM processors (ARM, StrongARM, XScale, OMAP, PXA, ...)\n\
      - Full screen mode only\n\
	  - VGA and QVGA devices\n\
      - Ogg Vorbis, wav, mod, s3m, it and xm music files\n\
\n\
Windows Mobile-based Smartphone Executable: Smartphone executable file\n\
    Supports:\n\
      - Windows CE OS 3.0 (Pocket PC 2002, Windows Mobile 2003, 2005) and later\n\
      - ARM processors (ARM, StrongARM, Scale, OMAP, PXA...)\n\
	  - MMC/SD expansion card is recommended\n\
      - Full screen mode only\n\
      - Ogg Vorbis, wav, mod, s3m, it and xm music files\n\
GP2X Executable\n\
\n\
Linux Executable\n\
Game Data Only:\n\
    Use this format if your game has been distributed in many game files\n\
    Use the LoadGame function to load the game data\n\
    Save with same name used in LoadGame function and in same directory of your executable game file\
\n\
Game Data Only for Pocket PC, Handheld PC, Smartphone or GP2X:\
    Game data optimized for Pocket PC, Handheld PC, Smartphones and GP2X devices"

#define TIP_EXPORT_FILE		"Select the directory where the game will be exported"

#define TIP_EXPORT_SYSTEM	"Select the export format"


//////////////////////////////////////////////////////////////////////////////

#define WIDTH	465

#if defined(GAME_EDITOR_PROFESSIONAL) || defined(GAME_EDITOR_HOME_EDITION)
#	define HEIGHT	106
#else
#	define HEIGHT	128
#endif

enum
{
	BT_OK,
	BT_CLOSE,
	BT_BUYNOW,
	LS_SYSTEM
};

char exportPath[PATH_LIMIT];
gedString ExportGame::exportName;

ExportGame::ExportGame()
	: Panel("ExportGame", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	static bool bInitPath = false;

	SetModal();
	SetToolTip(TIP_EXPORT);

	Text *text;
	Button *button;		
	int y;	

	//Title
	text = AddText("Export Game", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("       File: ", 10, y);
	name = AddEditBox(text->Right(), text->Top(), 200);
	dir = AddListDirectory(name->Right() + 2, y, 80, 0, 0, "select"); dir->SetToolTip(TIP_EXPORT_FILE);
	dir->SetSave();

	text = AddText("Export Type: ", 10, dir->Down() + 2);
	listSystem = AddListPop(text->Right(), text->Top(), 282, 0, LS_SYSTEM); listSystem->SetToolTip(TIP_EXPORT_SYSTEM);
	
#if defined(GAME_EDITOR_PROFESSIONAL) || defined(GAME_EDITOR_HOME_EDITION)
	y = listSystem->Down() + 2;
#else
	button = AddButton(BUY_NOW, listSystem->Left(), listSystem->Down() + 2, 0, 0, BT_BUYNOW);
	text = AddText(" to remove Game Editor information from your game\n and other limitations", button->Right() + 2, button->Top() - 2);
	y = button->Down() + 2;
#endif
	

	
	//Close
	y = DrawHLine(y + 2);

	button = AddButton("Export", (WIDTH-135)/2, y, 0, 0, BT_OK); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right()+8, y, 0, 0, BT_CLOSE); SetCancelButton(button);

	dir->AddFilter("exe");
	dir->AddFilter("dat");
	//dir->AddFilter("*"); //Can add an empty filter to linux files?

	listSystem->AddText("Windows Executable");
	listSystem->AddText("MAC OSX Executable");
	listSystem->AddText("Pocket PC / Handheld PC Executable");
	listSystem->AddText("Windows Mobile-based Smartphone Executable");
	listSystem->AddText("GP2X Executable");
	listSystem->AddText("Linux Executable");
	listSystem->AddText("iPhone Executable");
	

	listSystem->AddText("Game Data Only");
	listSystem->AddText("Game Data Only for Pocket PC, Handheld PC, Smartphone or GP2X");

	if(!exportName.length() /*|| actualGameName != GameControl::Get()->getGameName()*/)
	{
		exportName = GameControl::Get()->getGameName();
		//actualGameName = actualGameName;

		int i;
		if((i = exportName.find_no_case(".ged")) != gedString::npos)
		{
			exportName = exportName.substr(0, i);				
		}
	}

	name->SetText(exportName);
	dir->SetSaveName(exportName);

	if(!bInitPath)
	{
		exportPath[0] = 0;
		bInitPath = true;
		
		if(!Config::Get()->getPathExport().empty())
		{
			chdir(Config::Get()->getPathExport().c_str());
		}
		else
		{
			chdir(GameControl::Get()->getGamePath().c_str());
		}
	}
	else chdir(exportPath);
}

ExportGame::~ExportGame()
{

}

void ExportGame::OnEditChar(EditBox *edit, SDLKey key, int line)
{
	if(edit == name)
	{
		dir->SetSaveName(edit->GetText());
	}
}

bool ExportGame::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(listId != LS_SYSTEM)
	{
		name->SetText(text);
	}

	return true;
}


void ExportGame::OnButton(Button *button, int buttonId)
{
	SDL_ClearError();

	switch(buttonId)
	{
	case BT_OK:
		{
			if(GameControl::Get()->getGameName().length() == 0)
			{
				new PanelInfo("Please, save game first");
				return;
			}
			
			/*char actualpath[PATH_LIMIT];
			ged_getcwd(actualpath, PATH_LIMIT);
			if(GameControl::Get()->getGamePath() == actualpath)
			{
				new PanelInfo("Please, select a different game path");
				return;
			}*/
			
			if(!GameControl::Get()->Save(true)) 
			{
				new PanelInfo("Please, save game in a different game path first\nor verify if your .ged game file is not read only.");
				return;
			}

			if(name->GetText().size() <= 0 || name->GetText().size() > PATH_LIMIT)
			{
				new PanelInfo("Please, enter valid file name");
				return;
			}

			if(listSystem->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select a Export Type");
				return;
			}

			int exportType = GAME_DATA_ONLY;
			if(listSystem->GetText() == "Windows Executable") exportType = WINDOWS_EXECUTABLE;
			else if(listSystem->GetText() == "Pocket PC / Handheld PC Executable") 
			{
				if(!(
					(GameControl::Get()->getGameWidth() == 240 && GameControl::Get()->getGameHeight() == 320) ||
					(GameControl::Get()->getGameWidth() == 320 && GameControl::Get()->getGameHeight() == 240) ||
					(GameControl::Get()->getGameWidth() == 240 && GameControl::Get()->getGameHeight() == 240) ||

					(GameControl::Get()->getGameWidth() == 480 && GameControl::Get()->getGameHeight() == 640) ||
					(GameControl::Get()->getGameWidth() == 640 && GameControl::Get()->getGameHeight() == 480) ||
					(GameControl::Get()->getGameWidth() == 480 && GameControl::Get()->getGameHeight() == 800) ||
					(GameControl::Get()->getGameWidth() == 800 && GameControl::Get()->getGameHeight() == 480) ||

					(GameControl::Get()->getGameWidth() == 800 && GameControl::Get()->getGameHeight() == 600) ||
					(GameControl::Get()->getGameWidth() == 640 && GameControl::Get()->getGameHeight() == 240) ||

					(GameControl::Get()->getGameWidth() == 480 && GameControl::Get()->getGameHeight() == 480)
					)
				  )
				{
					
					PanelQuestion *panel = new PanelQuestion("To export for Pocket PC, change the game resolution to:\n  240x320,\n  320x240,\n  240x240,\n  480x640,\n  640x480,\n  640x240\n  480x800,\n  800x480\n  800x600\n  480x480\n\nProceed anyway?", "Warning", "Yes", "No", ALIGN_LEFT);
					if(panel->Wait() == CANCEL_BUTTON)
					{
						delete panel;
						return;
					}
					delete panel;
				}


				exportType = POCKETPC_EXECUTABLE;
			}
			else if(listSystem->GetText() == "Windows Mobile-based Smartphone Executable") 
			{
				if(!(
					(GameControl::Get()->getGameWidth() == 240 && GameControl::Get()->getGameHeight() == 320) ||
					(GameControl::Get()->getGameWidth() == 320 && GameControl::Get()->getGameHeight() == 240) ||								

					(GameControl::Get()->getGameWidth() == 176 && GameControl::Get()->getGameHeight() == 220) ||
					(GameControl::Get()->getGameWidth() == 220 && GameControl::Get()->getGameHeight() == 176) ||

					(GameControl::Get()->getGameWidth() == 160 && GameControl::Get()->getGameHeight() == 240) ||
					(GameControl::Get()->getGameWidth() == 240 && GameControl::Get()->getGameHeight() == 160) ||

					(GameControl::Get()->getGameWidth() == 208 && GameControl::Get()->getGameHeight() == 240) ||
					(GameControl::Get()->getGameWidth() == 240 && GameControl::Get()->getGameHeight() == 208)
					)
				  )
				{					
					PanelQuestion *panel = new PanelQuestion("To export for Windows Mobile-based Smartphone, change the game resolution to:\n  176x220,\n  160x240,\n  208x240,\n  240x320\n\nProceed anyway?", "Warning", "Yes", "No", ALIGN_LEFT);
					if(panel->Wait() == CANCEL_BUTTON)
					{
						delete panel;
						return;
					}
					delete panel;
				}


				exportType = POCKETPC_EXECUTABLE;
			}
			else if(listSystem->GetText() == "Game Data Only for Pocket PC, Handheld PC, Smartphone or GP2X") 
			{
				if(!(
					(GameControl::Get()->getGameWidth() == 240 && GameControl::Get()->getGameHeight() == 320) ||
					(GameControl::Get()->getGameWidth() == 320 && GameControl::Get()->getGameHeight() == 240) ||
					(GameControl::Get()->getGameWidth() == 240 && GameControl::Get()->getGameHeight() == 240) ||

					(GameControl::Get()->getGameWidth() == 480 && GameControl::Get()->getGameHeight() == 640) ||
					(GameControl::Get()->getGameWidth() == 640 && GameControl::Get()->getGameHeight() == 480) ||
					(GameControl::Get()->getGameWidth() == 480 && GameControl::Get()->getGameHeight() == 480) ||

					(GameControl::Get()->getGameWidth() == 480 && GameControl::Get()->getGameHeight() == 800) ||
					(GameControl::Get()->getGameWidth() == 800 && GameControl::Get()->getGameHeight() == 480) ||

					(GameControl::Get()->getGameWidth() == 800 && GameControl::Get()->getGameHeight() == 600) ||
					(GameControl::Get()->getGameWidth() == 640 && GameControl::Get()->getGameHeight() == 240) ||

					//Smartphone
					(GameControl::Get()->getGameWidth() == 176 && GameControl::Get()->getGameHeight() == 220) ||
					(GameControl::Get()->getGameWidth() == 220 && GameControl::Get()->getGameHeight() == 176) ||

					(GameControl::Get()->getGameWidth() == 160 && GameControl::Get()->getGameHeight() == 240) ||
					(GameControl::Get()->getGameWidth() == 240 && GameControl::Get()->getGameHeight() == 160) ||

					(GameControl::Get()->getGameWidth() == 208 && GameControl::Get()->getGameHeight() == 240) ||
					(GameControl::Get()->getGameWidth() == 240 && GameControl::Get()->getGameHeight() == 208)
					)
				  )
				{					
					PanelQuestion *panel = new PanelQuestion("To export for Pocket PC, change the game resolution to:\n  240x320,\n  320x240,\n  240x240,\n  480x640,\n  640x480,\n  640x240\n  480x800,\n  800x480\n  800x600\n  480x480\n\n\nTo export for Windows Mobile-based Smartphone, change the game resolution to:\n  176x220,\n  160x240,\n  208x240,\n  240x320\nTo export for GP2X, change the game resolution to:\n  320x240,\n  640x480\n\nProceed anyway?", "Warning", "Yes", "No", ALIGN_LEFT);
					if(panel->Wait() == CANCEL_BUTTON)
					{
						delete panel;
						return;
					}
					delete panel;
				}


				exportType = POCKETPC_DATA;
			}
			else if(listSystem->GetText() == "Linux Executable") exportType = LINUX_EXECUTABLE;
			else if(listSystem->GetText() == "MAC OSX Executable") exportType = MACOSX_EXECUTABLE;
			else if(listSystem->GetText() == "iPhone Executable") exportType = IPHONE_EXECUTABLE;
			else if(listSystem->GetText() == "GP2X Executable") 
			{
				if(!(					
					(GameControl::Get()->getGameWidth() == 320 && GameControl::Get()->getGameHeight() == 240) ||
					(GameControl::Get()->getGameWidth() == 640 && GameControl::Get()->getGameHeight() == 480)					
					)
				  )
				{
					
					PanelQuestion *panel = new PanelQuestion("To export for GP2X, change the game resolution to:\n  320x240,\n  640x480\n\nProceed anyway?", "Warning", "Yes", "No", ALIGN_LEFT);
					if(panel->Wait() == CANCEL_BUTTON)
					{
						delete panel;
						return;
					}
					delete panel;
				}

				exportType = GP2X_EXECUTABLE;
			}

#ifndef GAME_EDITOR_PROFESSIONAL
			if(exportType != WINDOWS_EXECUTABLE)
			{
				new PanelInfo(GAME_EDITOR_VERSION_ERROR);
				return;
			}
#endif

			exportName = name->GetText();
			if(!GameControl::Get()->ExportGame(exportName, exportType)) return;

			//Store current path
			ged_getcwd(exportPath, PATH_LIMIT);
			Config::Get()->setPathExport(exportPath);
		}	
		break;
#ifndef GAME_EDITOR_PROFESSIONAL
	case BT_BUYNOW:
		openBuySite();
		return;
#endif
	}

	delete this;
}






#endif ///*!defined(GAME_EDITOR_PROFESSIONAL) &&*/ !defined(STAND_ALONE_GAME)