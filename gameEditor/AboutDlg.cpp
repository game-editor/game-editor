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


// AboutDlg.cpp: implementation of the AboutDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "AboutDlg.h"
#include "GameControl.h"

#if defined(GAME_EDITOR_PROFESSIONAL) && defined(WIN32)
#	include <time.h>
#	ifdef USE_ACTIVATION
#		include "SecuredSections.h"
#	endif
#endif


extern Uint32 startKyraSplash;

enum
{
	BT_OK,
	BT_CANCEL,
	LS_ANIMATION
};



#ifdef WIN32
typedef bool (__stdcall *ShowEnterKeyDialogFn)(HWND parent); 
extern "C" HWND SDL_Window;

void ShowReg(bool bShowThanks)
{
	HINSTANCE libInst = LoadLibrary("ArmAccess.DLL");	
	
	if(libInst)
	{
		ShowEnterKeyDialogFn ShowEnterKeyDialog=(ShowEnterKeyDialogFn)GetProcAddress(libInst, "ShowEnterKeyDialog");
		if(ShowEnterKeyDialog)
		{
			if(ShowEnterKeyDialog((HWND)GetMainWindow()) && bShowThanks)
			{
				new PanelInfo(
				   "Thank you for your update!\n\
					To correct execution, please, close Game Editor and execute again.", "Info");		
			}
		}		
		
		FreeLibrary(libInst);
	}
}
#else
void ShowReg(bool bShowThanks) {}
#endif

#if defined(GAME_EDITOR_PROFESSIONAL)
void Inf1(gedString &userName, gedString &updateDays)
{
#if defined(GAME_EDITOR_PROFESSIONAL) && defined(WIN32) && !defined(STAND_ALONE_GAME)
	NANOBEGIN
#endif

	char buf[256];

	

#if !defined(USE_ACTIVATION)
	userName = "Game Editor";
	updateDays = "Life time";
	return;
#endif

#ifdef WIN32
	if(GetEnvironmentVariable("ALTUSERNAME",  buf, 255))
	{
		userName = buf;
	}

	if(GetEnvironmentVariable("EXTRAINFO",  buf, 255))
	{
		long purchasedDays = atol(buf), remainingDays = 0;

		if(purchasedDays == 0)
		{
			updateDays = "Life time";
		}
		else if(purchasedDays == 1)
		{
			updateDays = "no updates";
		}		
		else
		{
			//Calc the remaining update days
			if(GetEnvironmentVariable("KEYCREATED",  buf, 255))
			{
				//KEYCREATED = YYYY.MM.DD

				struct tm tmKeyCreated;
				time_t tCurrentTime, tKeyCreated;

				memset(&tmKeyCreated, 0, sizeof(struct tm));

				//Get key creation date
				sscanf(buf, "%4ld.%2ld.%2ld", &tmKeyCreated.tm_year, &tmKeyCreated.tm_mon, &tmKeyCreated.tm_mday);

				/////////////////////////////////////////
				//Keep this until release the new interface
				if(purchasedDays >= 30 && tmKeyCreated.tm_year >= 2006)
				{
					//New interface promotion
					updateDays = purchasedDays;
					return;
				}
				/////////////////////////////////////////

				tmKeyCreated.tm_year -= 1900;	//tm_year = year - 1900
				tmKeyCreated.tm_mon -= 1;		//January = 0

				tKeyCreated = mktime(&tmKeyCreated);

				//Get current time
				time(&tCurrentTime);

				//Calc diff
				double seconds = difftime(tCurrentTime, tKeyCreated);
				remainingDays = purchasedDays - (long)((seconds/3600.0)/24.0);

				

				if(remainingDays > 0)
				{
					updateDays = remainingDays;
				}
			}
		}
	}
#endif

#if defined(GAME_EDITOR_PROFESSIONAL) && defined(WIN32) && !defined(STAND_ALONE_GAME)
	NANOEND
#endif
}
#endif

#define WIDTH	300
#define HEIGHT	365

gedString collaboration("\
  Lassi Lehtinen (lcl)\n\
  Andreas Sturmer (StarFishDesign)\n\
  Anthony Artmann (jazz_e_bob)\n\
  Bjorn Halldor Bjornsson (DarkYeti)\n\
  Carlos Varela (CarlosVarela)\n\
  Christopher Hill (trajecto)\n\
  Ingrid Sanasse (ingsan)\n\
  John Lane (HillBilly)\n\
  Kelly Wilson (kellyrey)\n\
  Odilon Nelson Grisi Dantas (odilon)\n\
  Rachel Loyd (Just4Fun)\n\
  Raphael Salgado (BeyondtheTech)\n\
  Robin van der Kuijl (robin.deman)\n\
  Silvio Toledo (silviotoledo)\n\
  Steve Dunning (Kodo)\n\
  Victor Bissonnette (victor1242)\n\
");



static int getHeight()
{
	return HEIGHT + 70;
}

gedString getVersion()
{
	gedString version;

#if !defined(GAME_EDITOR_HOME_EDITION)
	version = "v";
	
	version += gedString((long)((GAME_EDITOR_VERSION & 0xFF000000) >> 24));
	version += ".";

	version += gedString((long)((GAME_EDITOR_VERSION & 0x00FF0000) >> 16));
	version += ".";

	version += gedString((long)((GAME_EDITOR_VERSION & 0x0000FF00) >> 8));

#else
	version = "v1.0";
#endif

	return version;
}

AboutDlg::AboutDlg()
	: Panel("AboutDlg", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - getHeight())/2,
						  WIDTH, getHeight())
{
	Text *text;
	int y;

	//Title
	text = AddText("About Game Editor", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	logo = new Actor("logo", this);
	logo->AddAnimation("maksiconActor.gif", "maksiconActor.gif", 1, 1, 0, 0);
	logo->SetPos(10, y);
	gedString type;

#if defined(GAME_EDITOR_HOME_EDITION)
	type = " Home Edition";
#elif defined(GAME_EDITOR_PROFESSIONAL)
#if defined(GAME_EDITOR_BETA)
	type = " Beta";
#else
	type = " Professional";
#endif
#endif



	gedString date(__DATE__), month, year;

	month = date.substr(0, date.find(' '));
	year = date.substr(date.rfind(' ') + 1);
	month.lower();

	text = AddText(gedString("Game Editor") + type + " " + getVersion() + "\n" + month + "/" + year, logo->Right() + 20, logo->Top() + 20);


	text = AddText("game-editor.com", text->Left(), text->Down());

	y = DrawHLine(logo->Down() + 8);

	text = AddText("Developed by Makslane Araujo Rodrigues", 10, y);
	text = AddText("makslane@game-editor.com", text->Left(), text->Down());	
	text = AddText("Co-developer: Andreas Kindlbacher (akr)", text->Left(), text->Down() + 10);	
	text = AddText("andreas@game-editor.com", text->Left(), text->Down());	
	text = AddText("Co-developer: Leland Dawson (skydereign)", text->Left(), text->Down() + 10);	
	text = AddText("leland@game-editor.com", text->Left(), text->Down());	


	text = AddText("Collaboration:", text->Left(), text->Down()+10);

	EditBox *edit = AddEditBox(text->Left(),  text->Down() + 10, WIDTH-34, 100, true);
	edit->ShowScrollBar();
	edit->SetText(collaboration, ALL_LINES);
	
	//Register info
	y = DrawHLine(edit->Down() + 8);


	gedString userName("Demo Version"), updateDays("no updates");

#if defined(GAME_EDITOR_PROFESSIONAL)
	Inf1(userName, updateDays);
#endif

	//Show info
/*#ifndef GAME_EDITOR_HOME_EDITION	
	text = AddText("Registered to: ", 10, y + 2);
	text = AddText(userName, text->Right(), text->Top());	

	text = AddText("Remaining update days: ", 10, text->Down() + 2);
	text = AddText(updateDays, text->Right(), text->Top());

	y = DrawHLine(y + 8);
#endif*/




	//Library logos
	EditorDirectory editDir;


	text = AddText("This software uses:", 10, y);

	text = AddText("  PocketHAL: pockethal.droneship.com", text->Left(), text->Down() + 10);
#ifdef USE_RAKNET
	text = AddText("  RakNet: www.rakkarsoft.com", text->Left(), text->Down() + 2);
#endif
	//text = AddText("  Graphviz: graphviz.org", text->Left(), text->Down() + 2);
	//text = AddText("  Scintilla: www.scintilla.org", text->Left(), text->Down() + 2);
	text = AddText("  Kyra: www.grinninglizard.com/kyra", text->Left(), text->Down() + 2);
	text = AddText("  SDL: www.libsdl.org", text->Left(), text->Down() + 2);


	//Kyra LGPL liscence
	//GameControl::Get()->StartKyraSplash();
}

AboutDlg::~AboutDlg()
{
	delete logo;
	/*delete kyraLogo;
	delete sdlLogo;*/
}

bool AboutDlg::OnMouseButtonDown(int x, int y, Uint8 button)
{
	PostMessage(this, DELETE_ME, DELETE_ME);
	return false;
}








#define REGISTER_WIDTH 450
#define REGISTER_HEIGHT 135

enum
{
	BT_BUY = 0,
	BT_REGISTER,
	BT_CLOSE
};


RegisterPanel::RegisterPanel(gedString _keyCreationDate, int _purchasedDays, bool bUpdate)
	: Panel("RegisterPanel", (GameControl::Get()->Width() - REGISTER_WIDTH)/2, 
	(GameControl::Get()->Height() - (bUpdate?REGISTER_HEIGHT:REGISTER_HEIGHT+25))/2,
						  REGISTER_WIDTH, (bUpdate?REGISTER_HEIGHT:REGISTER_HEIGHT+25))
{
	SetModal();

	Text *text;
	Button *button;
	int y;

	//Title
	text = AddText("Update Error", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	gedString updateDays((long)_purchasedDays);
	if(_purchasedDays <= 5) //Five days of tolerance
	{
		updateDays = "no update versions";
	}
	else
	{
		updateDays += " days of updates";
	}

	gedString text1("run"), text2("\n\nTo use your old working version, please reinstall it.");

	if(bUpdate) 
	{
		text1 = "update";
		text2 = "";
	}

	text = AddText(gedString("Dear user, you have purchased Game Editor in ") + _keyCreationDate + gedString("\nwith ") +
				   updateDays + gedString(". Your time for update is over.\n\nIf you want to ") + text1 + gedString(" this version, you must buy a new package and\nenter your new registration info.") + text2, 10, y);
	

	y = DrawHLine(text->Down() + 8);

	button = AddButton("Buy new package", bUpdate?120:90, y, 0, 0, BT_BUY); SetConfirmButton(button);


	if(bUpdate)
	{
		button = AddButton("Close", button->Right() + 15, button->Top(), 0, 0, BT_CLOSE);
	}
	else
	{
		button = AddButton("Enter registration info", button->Right() + 15, button->Top(), 0, 0, BT_REGISTER);
	}

	
}

RegisterPanel::~RegisterPanel()
{

}

void RegisterPanel::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_BUY:
		{
			openUrl("http://game-editor.com/orders.html?day=update");
		}
		break;

	case BT_REGISTER:
		{
			ShowReg(true);
		}
		break;

	case BT_CLOSE:
		delete this;
		break;
	}
}

#endif //STAND_ALONE_GAME
