// MainPanel.cpp: implementation of the MainPanel class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#pragma warning(disable:4786)

#include "MainPanel.h"
#include "PathDialog.h"
#include "GameControl.h"
#include "AddActor.h"
#include "LoadSaveGame.h"
#include "ActorProperty.h"
#include "ActorEdit.h"
#include "GridSettings.h"
#include "GameSettings.h"
#include "AboutDlg.h"
#include "RegionLoad.h"
#include "ExportGame.h"
#include "GenericScript.h"
#include "ScriptGlobals.h"
#include "Config.h"
#include "UndoControl.h"

#include "TutorialControlPanel.h"
#include "Tutorial.h"

#ifdef wxUSE_GUI
#include "../wxGameEditor/PanelProperty.h"
#endif

#if defined(WIN32)
extern "C" HWND SDL_Window;
#endif


#if defined(GAME_EDITOR_PROFESSIONAL) && defined(WIN32) && !defined(STAND_ALONE_GAME)
#	include "SecuredSections.h"
#	include <time.h>
#	include "AboutDlg.h"
char *date_mac_conv(char *pszDate);
#endif


#define MAX_SCRIPTS 5




//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_MAIN_MENU				"\
The main menu show the main controls and the mouse coordinates relative to game center or\n\
actor's coordinate relative to parent when click on actor\n\
(Actor icon shown here indicates this)\n\
\n\
Editor Elements:\n\
\n\
   Red grid lines: Actual game resolution\n\
   Orange grid lines: Actual grid size\n\
   White rectangle: View actor\n\
   Yellow rectangle: Activation Region\n\
   Green rectangle: Wire Frame Region\n\
   Blue rectangle: Filled Region\n\
   Cyan rectangle: Canvas Actor\n\
   Gradient Line: Activation Event\n\
   White cross: Game center\n\
   Pacman: Initial actor icon\n\
\n\
Useful controls:\n\
   Zoom: +/- or >/< to increase or decrease zoom\n\
   Undo: Ctrl+z\n\
   Redo: Ctrl+y or Ctrl+r\n\
   Mouse click + left Ctrl select overlapped actors\n\
   Mouse click + right Ctrl move the grid\n\
   Left shift toggle the actor snap on/off\n\
     (Magnetic effect between actors or tiles,\n\
     for more precision when positioning)"

#define TIP_MAIN_MENU_FILE			"\
New Game: Clear current game\n\
    Load: Load a new game\n\
   Merge: Merge current game with a newly loaded game\n\
    Save: Save current game\n\
  Export: Create a game executable file for Windows, Linux or PocketPC\n\
    Exit: Close the Game Editor"

#define TIP_MAIN_MENU_ACTOR			"Add a new character to your game"
#define TIP_MAIN_MENU_PATH			"Add a new path or select an existent path to view and edit"

#define TIP_MAIN_MENU_REGIONS		"\
Activation Regions: Use to build a multilevel game in a single file\n\
 Show/Hide Regions: Show or hide Activation Regions and Region Actors\n\
\n\
Activation Regions are screen areas that are defined by a bounding box\n\
and include game Actors. A Region is loaded into view when the View Actor\n\
intersects it. The Region and all actors within the Region are destroyed\n\
when the View Actor no longer intersects it.\n\
\n\
Activation Regions allow a game to have multiple gaming levels.\n\
All levels defined within a game are saved in a single file.\n\
\n\
Numerous Activation Regions and overlapped Activation Regions can be constructed.\n\
All of a Region’s previously defined Actors automatically load inside the Activation Region.\n\
\n\
How it works:\n\
\n\
   If an actor is out of the Activation Region, it will be destroyed only when the\n\
   Activation Region doesn’t intersect with a View Actor. An Actor can be destroyed\n\
   explicitly at any time with the Destroy Actor Action.\n\
\n\
   Actors created with Create Actor will be assigned to a visible Activation Region.\n\
   If the view leaves an region and later it comes back, all actors will be reloaded,\n\
   except those destroyed explicitly with the Destroy Actor action.\n\
   Any actor can be loaded with Create Actor, even if has been destroyed explicitly.\n\
\n\
   An Actor will be destroyed by the Activation Region when it and your ascendants are outside of the \"View\".\n\
\n\
   If no Activation Regions have been defined, all actors will be loaded when the game starts.\n\
\n\
Activation Regions are denoted by a yellow bounding box. The Activation Region’s\n\
rectangular box can be moved or resized freely. The box is only visible in the work area.\n\
It disappears when the game is running in game mode.\n\
\n\
To remove an Activation Region, right click on the bounding box, and select Remove Region."

#define TIP_MAIN_MENU_GAMEMODE		"Run the game"

#define TIP_MAIN_MENU_CONFIG		"\
     Set Game Properties: Resolution, Mode, Game Title, etc.\n\
  Set Editor Preferences: (Grid settings, undo levels, etc.)"

#define TIP_MAIN_MENU_HELP		"\
Game Editor's help center\n\
Click here to find the documentation and tutorials"

#define TIP_MAIN_MENU_SCRIPT "\
Quick access to Global code and all actor's scripts"

//////////////////////////////////////////////////////////////////////////////


enum
{
	BT_ADDACTOR,
	BT_PATH,
	//BT_GAME_MODE,
	LS_FILE,
	LS_SETTINGS,
	LS_REGIONS,
	LS_HELP,
	LS_TUTORIALDIR,

	BT_DONTSHOW,
	BT_UPDATE,
	BT_DEMOS,
	BT_DOCS,
	BT_FORUM,
	BT_NEWGAME,
	LS_TUTORIAL,
	LS_EDIT_MENU,
	LS_SCRIPT
};

const int BT_GAME_MODE = LS_SCRIPT + 1;

#define WIDTH_REMINDER	320
#define HEIGHT_REMINDER	389

bool TutorialReminder::bAlreadyShow = false;
TutorialReminder::TutorialReminder()
	: Panel("TutorialReminder", (GameControl::Get()->Width() - (WIDTH_REMINDER - 85))/2, 
				          (GameControl::Get()->Height() - HEIGHT_REMINDER)/2,
						  WIDTH_REMINDER, HEIGHT_REMINDER)
{
	SetModal();

	bAlreadyShow = true;
	chdir((GameControl::getEditorPath() + DIR_SEP + "Tutorials" + DIR_SEP + "Getting Started").c_str());


	Text *text;
	Button *button;
	
	int y;

	//Title
	text = AddText("Welcome", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);
	
	//Body
	text = AddText("Thank you for choosing Game Editor\nFor a quick start, select a option below:", 30, y);


	//Create a new game
	y = DrawHLine(text->Down() + 10);

	text = AddText("Create a new game:", 30, y);
	button = AddButton("Create now", 58, text->Down() + 10, 0, 0, BT_NEWGAME);


	//Demos
	y = DrawHLine(button->Down() + 5);

	text = AddText("Get game demos:", 30, y);
	button = AddButton("Go to demo page", 58, text->Down() + 10, 0, 0, BT_DEMOS);

	//Tutorials
	y = DrawHLine(button->Down() + 5);

	text = AddText("See the tutorials:", 30, y);

	listTutorialDir = AddListDirectory(10, text->Down() + 10, 0, 0, LS_TUTORIAL, " Click here for the tutorials ");
	listTutorialDir->setCanUseSystemFileDialog(false);

#if defined(WIN32) && !defined(GAME_EDITOR_HOME_EDITION)
	button = AddButton("Get the newest tutorials", 58, listTutorialDir->Down() + 5, 0, 0, BT_UPDATE);
	text = AddText("For more tutorials, select from Help Menu above", text->Left(), button->Down() + 5);
#else
	text = AddText("For more tutorials, select from Help Menu above", text->Left(), listTutorialDir->Down() + 25);
#endif

	listTutorialDir->SetPos((WIDTH_REMINDER - listTutorialDir->Width())/2, listTutorialDir->getImage()->Y());
	listTutorialDir->AddFilter("gedt");
	listTutorialDir->setShowDir(false);
	listTutorialDir->setShowExtension(false);


	


	//Help
	y = DrawHLine(text->Down() + 5);

	text = AddText("Get help:", 30, y);
	button = AddButton("Read the documentation", 58, text->Down() + 10, 0, 0, BT_DOCS);
	button = AddButton("Go to Game Editor's forum", 58, button->Down() + 5, 0, 0, BT_FORUM);
	
	
		
	//Close
	y = DrawHLine(button->Down() + 5);

	button = AddButton("Close", (WIDTH_REMINDER-135)/2 - 30, y, 0, 0, BT_CLOSE);
	button = AddButton(" Don't show again ", button->Right() + 10, y, 0, 0, BT_DONTSHOW);

	//Logo
	{
		EditorDirectory dir;

		logo = new Actor("ged_TutorialReminder_logo", this);
		logo->AddAnimation("logo", "logo.png", 1, 1, 0, 0);
		logo->SetPos(-logo->Width(), 0);
	}
}

TutorialReminder::~TutorialReminder()
{
	delete logo;
}

bool TutorialReminder::OnList(ListPop *list, int index, gedString &text, int listId)
{
	PostMessage(this, 0);
	return true;
}

void TutorialReminder::OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message)
{
	if(messageId == 0)
	{
		if(Tutorial::Get()->Load(listTutorialDir->GetFullPath()))
		{
			Tutorial::Get()->Play();
		}
		
		PostMessage(this, DELETE_ME, DELETE_ME);
	}
}

void TutorialReminder::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_UPDATE:
		TutorialUpdateDlg::Call();
		break;

	case BT_NEWGAME:
		new AddActor(true);
		delete this;
		break;

	case BT_DEMOS:
		openUrl("http://game-editor.com/demos.html");
		break;

	case BT_DOCS:
		{
			EditorDirectory editDir;
			
			//Firefox don't open "Docs/index.html" in windows
			//So, use \ on windows and / on linux
			openUrl((gedString("Docs") + DIR_SEP + "index.html").c_str());
		}
		break;

	case BT_FORUM:
		openUrl("http://game-editor.com/forum");
		break;

	case BT_CLOSE:
		ActorProperty::Call(GameControl::Get()->GetActor("view"));
		delete this;
		break;

	case BT_DONTSHOW:
		Config::Get()->setShowTutorialReminder(false);
		ActorProperty::Call(GameControl::Get()->GetActor("view"));
		delete this;
		break;
	}

	chdir((GameControl::getEditorPath() + DIR_SEP + "Tutorials" + DIR_SEP + "Getting Started").c_str());
}

ListString lastScripts;
MainPanel *MainPanel::mainPanel = NULL;
MainPanel::MainPanel()
: Panel("MainPanel", 0, 0, 
#if !defined(GAME_EDITOR_HOME_EDITION)
		Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION)?700:
#endif
		640,
		24, NULL)
{
	mainPanel = this;

	Button *button;
	ListPop *listRegion;
	int x;
	listScript = NULL;

	SetToolTip(TIP_MAIN_MENU);

	listFile = AddListPop(10, 2, 64, 0, LS_FILE, "File"); listFile->SetToolTip(TIP_MAIN_MENU_FILE);
	button = AddButton("Add actor", listFile->Right()+2, 2, 0, 0, BT_ADDACTOR); button->SetToolTip(TIP_MAIN_MENU_ACTOR);
	button = AddButton("Path", button->Right() + 2, 2, 0, 0, BT_PATH);	button->SetToolTip(TIP_MAIN_MENU_PATH);
	listRegion = AddListPop(button->Right() + 2, 2, 64, 0, LS_REGIONS, "Regions"); listRegion->SetToolTip(TIP_MAIN_MENU_REGIONS);

#if !defined(GAME_EDITOR_HOME_EDITION)
	if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		listScript = AddListPop(listRegion->Right() + 2, 2, 64, 0, LS_SCRIPT, "Script"); listScript->SetToolTip(TIP_MAIN_MENU_SCRIPT);

		ScriptSetup();

		x = listScript->Right() + 2;
	}
	else
#endif
	{
		x = listRegion->Right() + 2;
	}

	button = AddButton("Game Mode", x, 2, 0, 0, BT_GAME_MODE); button->SetToolTip(TIP_MAIN_MENU_GAMEMODE);
	listConfig = AddListPop(button->Right() + 2, 2, 64, 0, LS_SETTINGS, "Config"); listConfig->SetToolTip(TIP_MAIN_MENU_CONFIG);
	listHelp = AddListPop(listConfig->Right() + 2, 2, 64, 0, LS_HELP, "Help"); listHelp->SetToolTip(TIP_MAIN_MENU_HELP);

	listTutorialDir = AddListDirectory(listHelp->Right() + 4, listHelp->Top(), 1, 1, LS_TUTORIALDIR);
	listTutorialDir->getImage()->SetVisible(false);
	listTutorialDir->AddFilter("gedt");
	listTutorialDir->setShowDir(false);
	listTutorialDir->setShowExtension(false);
	listTutorialDir->setCanUseSystemFileDialog(false);
	
	


	listConfig->AddText("Game Properties");
	listConfig->AddText("Preferences");

	if(!Tutorial::IsCompatible(VERSION_PREFERENCES))
	{
		if(Config::Get()->getEnableToolTips()) listConfig->AddText("Disable Tool Tips");	
		else listConfig->AddText("Enable Tool Tips");
	}

	listRegion->AddText("Add Activation Region");	
	pShowHideRegions = listRegion->GetTextPtr(listRegion->AddText("Hide Regions"));

	
	
	if(!GameControl::Get()->GetViewActor())
	{
		new ActorEdit(VIEW_NAME, NULL, true);
	}
	
	//Coordinate
	iconCoord = new Actor("gedIconCoordinate", this);
	iconCoord->AddAnimation(ACTOR_GED_ICON, "maksiconActor.gif", 1, 1, 0, 0);
	iconCoord->SetActorScale(.25);
	iconCoord->SetPos(listHelp->Right() + 21, 2);

	coord = AddText("(0000000,0000000)", iconCoord->Right() + 2, 5, ALIGN_RIGHT);
	iconCoord->getImage()->SetVisible(false);

	//Snap
	iconSnap1 = new Actor("gedIconCoordinate", this);
	iconSnap1->AddAnimation(ACTOR_GED_ICON, "maksiconActor.gif", 1, 1, 0, 0);
	iconSnap1->SetActorScale(.125);
	iconSnap1->SetPos(listHelp->Right() + 5, 2);
	iconSnap1->getImage()->CalcTransform();

	iconSnap2 = new Actor("gedIconCoordinate", this);
	iconSnap2->AddAnimation(ACTOR_GED_ICON, "maksiconActor.gif", 1, 1, 0, 0);
	iconSnap2->SetActorScale(.125);
	iconSnap2->SetPos(iconSnap1->getImage()->X() + 8, iconSnap1->getImage()->Y());

	iconSnap3 = new Actor("gedIconCoordinate", this);
	iconSnap3->AddAnimation(ACTOR_GED_ICON, "maksiconActor.gif", 1, 1, 0, 0);
	iconSnap3->SetActorScale(.125);
	iconSnap3->SetPos(iconSnap1->getImage()->X() + 8, iconSnap1->getImage()->Y() + 8);

	iconSnap4 = new Actor("gedIconCoordinate", this);
	iconSnap4->AddAnimation(ACTOR_GED_ICON, "maksiconActor.gif", 1, 1, 0, 0);
	iconSnap4->SetActorScale(.125);
	iconSnap4->SetPos(iconSnap1->getImage()->X(), iconSnap1->getImage()->Y() + 8);

	iconSnap1->getImage()->SetVisible(false);
	iconSnap2->getImage()->SetVisible(false);
	iconSnap3->getImage()->SetVisible(false);
	iconSnap4->getImage()->SetVisible(false);


	FileSetup();
	HelpSetup();




	//Only show reminder on linux when can open the pages
/*#if !defined(__WIN32__) && !defined(GAME_EDITOR_HOME_EDITION)
	//Start message	linux
	if(Config::Get()->getShowTutorialReminder())
	{
		new TutorialReminder();
	}			
#endif*/

	listEditBox = NULL;
}

MainPanel *MainPanel::Create()
{
	delete mainPanel;
	mainPanel = new MainPanel();

	GameControl::Get()->SetMainActor(mainPanel);

	if(Tutorial::IsOff() && !GameControl::Get()->GetModal() && !Config::Get()->getShowTutorialReminder())
	{
		ActorProperty::Call(GameControl::Get()->GetActor("view"));
	}

	return mainPanel;
}

void MainPanel::ScriptSetup()
{
	if(!listScript) return;

	listScript->RemoveAll();
	

	Action::PopulateScriptActions(listScript);
	listScript->Sort();
	int nScripts = listScript->Count();

	listScript->AddText("Global code", false, 0);
	if(listScript->Count() > 1) listScript->AddSeparator(1);
	
	if(lastScripts.Count())
	{
		//Put the last used scripts in front
		int i;
		U32 data[MAX_SCRIPTS];
		

		//Get data
		for(i = 0; i < lastScripts.Count(); i++)
		{
			data[i] = (U32)listScript->GetItemData(lastScripts[i]);
			if(!data[i])
			{
				//Remove scripts if not exists
				lastScripts.Remove(i);
				i = -1;
			}
		}

		if(lastScripts.Count()) //Have any last scripts yet?
		{
			
			//Remove the texts already exists in last scripts
			for(i = 0; i < lastScripts.Count(); i++)
			{
				listScript->RemoveText(lastScripts[i]);
			}
			
			//Add last scripts
			for(i = 0; i < lastScripts.Count(); i++)
			{
				listScript->AddText(lastScripts[i], true, i + 2);
				listScript->SetItemData(i + 2, (void *)data[i]);
			}
			
			if(nScripts > lastScripts.Count())
			{
				listScript->AddSeparator(i + 2);
			}
		}
	}
}

void MainPanel::AddLastScript(gedString &text)
{
	
	int i;
	bool bFound = false;
	
	for(i = 0; i < lastScripts.Count(); i++)
	{
		if(text == lastScripts[i])
		{
			lastScripts.Remove(i);
			break;
		}
	}
	
	lastScripts.PushFront(text);
	
	if(lastScripts.Count() > MAX_SCRIPTS)
	{
		lastScripts.PopBack();
	}
}

void MainPanel::FileSetup()
{
	gedString oi("ok");
	oi = "Actor -> Collision (Any side of LeftWire)";

	for(int i = 0; i < listFile->Count(); i++)
	{
		gedString *s = (gedString *)listFile->GetItemData(i);
		delete s;
	}

	listFile->RemoveAll();

	listFile->AddText("New Game");
	listFile->AddText("Load");
	listFile->AddText("Merge");
	listFile->AddText("Save");

#ifdef USE_SYSTEM_FILE_DIALOG
	if(Tutorial::IsOff())
	{
		listFile->AddText("Save As...");
	}
#endif

	listFile->AddText("Export");

	if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		RecentFileList mru;
		bool bFirstSep = false;

		
		
		for(int i = 0; i < mru.GetSize(); i++)
		{
			if(!mru.GetFullPath(i).empty())
			{
				if(!bFirstSep)
				{
					listFile->AddSeparator();
					bFirstSep = true;
				}

				int iData = listFile->AddText(mru.GetDisplayName(i));
				listFile->SetItemData(iData, new gedString(mru.GetFullPath(i)));
			}
		}

		if(bFirstSep)
		{
			listFile->AddSeparator();
		}
	}

	listFile->AddText("Exit");

#if defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL) && !defined(STAND_ALONE_GAME) && !defined(GAME_EDITOR_HOME_EDITION)
	listFile->AddText(BUY_NOW); //Must be the last item to avoid turorial bugs
#endif
}

void MainPanel::HelpSetup()
{
	listHelp->RemoveAll();

#if defined(__WIN32__) //No download functions in Linux now
	listHelp->AddText("Documentation");
#endif
	
	//Tutorial section
	Tutorial::PopulateList(listHelp);
	//listHelp->AddText("Create New Tutorial");

#if defined(__WIN32__) && !defined(GAME_EDITOR_HOME_EDITION) //No download functions in Linux now
	listHelp->AddText("Get Newest Tutorials...");

#ifdef GAME_EDITOR_PROFESSIONAL
	listHelp->AddText("Check for Updates...");
#endif

	listHelp->AddText("Game Demos...");
	listHelp->AddText("User Forums...");
#endif

	listHelp->AddText("About...");

#if defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL) && !defined(STAND_ALONE_GAME) && !defined(GAME_EDITOR_HOME_EDITION)
	listHelp->AddText(BUY_NOW); //Must be the last item to avoid turorial bugs
#endif
}

MainPanel::~MainPanel()
{
	mainPanel = NULL;

	delete iconCoord;
	delete iconSnap1;
	delete iconSnap2;
	delete iconSnap3;
	delete iconSnap4;

	delete listEditBox;

	for(int i = 0; i < listFile->Count(); i++)
	{
		gedString *s = (gedString *)listFile->GetItemData(i);
		delete s;
	}
}

void MainPanel::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_ADDACTOR:
		{
#if !defined(GAME_EDITOR_HOME_EDITION)
			new AddActor();
#else
			if(GameControl::Get()->NumActors() <= 30)
			{
				new AddActor();
			}
			else
			{
				new PanelInfo("Game Editor Home Edition only support 30 actors");
			}
#endif
		}
		break;
	case BT_PATH:
		{
			PathDialog::Call();
		}
		break;
	case BT_GAME_MODE:
		{
			//Save copy
			//GameControl::Get()->SaveEmergencyRecovery();

			if(GenericScript::ParserAll())
			{			
				UndoControl::Get()->PushCurrentState();
				RegionLoad::UpdateRegions();
				GameControl::Get()->StoreScreenSize();
				GameControl::Get()->SetGameMode(true);
			}
			else
			{
				/*if(Config::Get()->getEmergencySave().length())
				{
					remove(Config::Get()->getEmergencySave().c_str());
					Config::Get()->setEmergencySave("");
				}

				Config::Destroy();*/

				new PanelInfo(GenericScript::GetError(), "Error", ALIGN_LEFT);
			}
		}
		break;
	}
}

bool MainPanel::OnListButtonDown(ListPop *list, int listId)
{
	switch(listId)
	{
		case LS_SCRIPT:
		ScriptSetup();
		break;
	}

	return true;
}

void MainPanel::UpdateCheck()
{
#if defined(GAME_EDITOR_PROFESSIONAL) && defined(WIN32)

	//Check if can run this version
	NANOBEGIN

	char buf[256], keyCreated[20];
	bool bCanUpdate = false;


	if(GetEnvironmentVariable("EXTRAINFO",  buf, 255))
	{
		long purchasedDays = atol(buf);

		if(purchasedDays == 0)
		{
			bCanUpdate = true;
		}
		else
		{
			//Calc the remaining update days
			if(GetEnvironmentVariable("KEYCREATED",  buf, 255))
			{
				//KEYCREATED = YYYY.MM.DD
				strcpy(keyCreated, buf);

				struct tm tmKeyCreated, tmCompilationTime;
				time_t tCompilationTime, tKeyCreated;

				memset(&tmCompilationTime, 0, sizeof(struct tm));
				memset(&tmKeyCreated, 0, sizeof(struct tm));

				//Get key creation date
				sscanf(buf, "%4ld.%2ld.%2ld", &tmKeyCreated.tm_year, &tmKeyCreated.tm_mon, &tmKeyCreated.tm_mday);

				/////////////////////////////////////////
				//maks:teste Keep this until release the new interface
				if(purchasedDays >= 30 && tmKeyCreated.tm_year >= 2006)
				{
					//New interface promotion
					bCanUpdate = true;
				} else {
				/////////////////////////////////////////

				tmKeyCreated.tm_year -= 1900;	//tm_year = year - 1900
				tmKeyCreated.tm_mon -= 1;		//January = 0

				tKeyCreated = mktime(&tmKeyCreated);

				//Get compilation time
				sscanf(date_mac_conv(__DATE__), "%4ld-%2ld-%2ld", &tmCompilationTime.tm_year, &tmCompilationTime.tm_mon, &tmCompilationTime.tm_mday);

				tmCompilationTime.tm_year -= 1900;	//tm_year = year - 1900
				tmCompilationTime.tm_mon -= 1;		//January = 0

				tCompilationTime = mktime(&tmCompilationTime);
				 

				//Calc diff
				double seconds = difftime(tCompilationTime, tKeyCreated);

				if((long)((seconds/3600.0)/24.0) <= purchasedDays)
				{
					bCanUpdate = true;
				}
				else
				{
					new RegisterPanel(keyCreated, purchasedDays, true);
				}

				} //maks:teste Keep this until release the new interface
			}
		}
	}

	if(bCanUpdate)
	{
		WaitCursor wait;

		char buf[128], date1[64], date2[64];
		gedString hash;
		
		//Get current time (GMC)
		struct tm *newtime;
		time_t ltime;
		 
		time(&ltime );		
		newtime = gmtime(&ltime );

		sprintf(date1, "%02d/%02d/%04d", newtime->tm_mday, newtime->tm_mon + 1, newtime->tm_year + 1900);
		sprintf(date2, "%02d %02d %04d", newtime->tm_mon + 1, newtime->tm_mday, newtime->tm_year + 1900);

		sprintf(buf, "%X%X", Crc32(date1, strlen(date1)), Crc32(date2, strlen(date2)));

		hash = buf;

		openUrl((gedString("http://game-editor.com/gameeditorpro?version=") + getVersion() + "&hash=" + hash + "&ext=.exe").c_str());
	}

	NANOEND

#endif
}

bool MainPanel::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(listEditBox) 
	{
		delete listEditBox;
		listEditBox = NULL;
	}

	switch(listId)
	{
	case LS_FILE:
		{			
			if(text == "New Game")
			{
				/*<Odilon>
				  Verifica se nao foi feito nada, ainda, para
				  pedir confirmacao apenas se necessario.
				*/
				
				if (GameControl::Get()->Modified())
				{
					//<Odilon> Pequena correcao de Ingles...
					PanelQuestion *panel = new PanelQuestion("This will discard the current game.\nProceed anyway?");
					///PanelQuestion *panel = new PanelQuestion("This action will erase the current game.\nTo proceed?");
					
					if(panel->Wait() == OK_BUTTON)
					{
						GameControl::Get()->NewGame();
						UndoControl::Get()->Clear();
						lastScripts.Clear();

						#ifdef WIN32
						ExportGame::ClearExportName();
						#endif

						/*Close actor dialog*/
						ActorProperty::Destroy();
					}
					
					delete panel;
					
				}
				else  //nao eh necessario confirmar; nada foi feito.
				{
					/* Aqui estou repetindo o mesmo codigo acima,...
					   talvez seja melhor encontrar outra maneira de fazer isso,
					   ou talvez nao... */
			
					GameControl::Get()->NewGame();
					UndoControl::Get()->Clear();
					lastScripts.Clear();

					/*Close actor dialog*/
					ActorProperty::Destroy();
				}
				
			}
			else if(text == "Load")
			{
				new LoadSaveGame(LOAD_GAME);

				#ifdef WIN32
				ExportGame::ClearExportName();
				#endif
			}
			else if(text == "Merge")
			{
				new LoadSaveGame(MERGE_GAME);
			}
#ifdef USE_SYSTEM_FILE_DIALOG
			else if(text == "Save As...")
			{
				new LoadSaveGame(SAVE_GAME);
				ExportGame::ClearExportName();
			}
			else if(text == "Save")
			{
				if(Tutorial::IsOff())
				{
					gedString gamePath(GameControl::Get()->getGamePath());

					if(!gamePath.empty())
					{
						LoadSaveGame::Save(gamePath + DIR_SEP + GameControl::Get()->getGameName());
					}
					else
					{
						new LoadSaveGame(SAVE_GAME);
					}
				}
				else
				{
					new LoadSaveGame(SAVE_GAME);
				}
			}
#else
			else if(text == "Save")
			{
				new LoadSaveGame(SAVE_GAME);
			}
#endif			
			else if(text == "Export")
			{
				#ifdef WIN32//#ifdef GAME_EDITOR_PROFESSIONAL

				if(GenericScript::ParserAll())
				{
					new ExportGame();
				}
				else
				{
					new PanelInfo(GenericScript::GetError(), "Error", ALIGN_LEFT);
				}
				
				#else 
				//Don't export on Linux
				//<Odilon> Pequena correcao de Ingles...
				new PanelInfo(GAME_EDITOR_VERSION_ERROR);
				#endif
			}
			else if(text == "Exit")
			{
				//Exit
				SDL_Event event;
				memset(&event, 0, sizeof(SDL_Event));
				event.quit.type = SDL_QUIT;
				SDL_PushEvent(&event);
			}
#if defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL) && !defined(STAND_ALONE_GAME) && !defined(GAME_EDITOR_HOME_EDITION)
			else if(text == BUY_NOW)
			{
				openBuySite();
			}
#endif
			else
			{
				//Recent file list
				gedString *sFile = (gedString *)listFile->GetItemData(index);
				if(sFile)
				{
					LoadGame(*sFile, true);
				}
			}		
		}
		break;
	case LS_SETTINGS:
		{
			if(text == "Game Properties")
			{
				new GameSettings();
			}
			else if(text == "Preferences")
			{
				new Preferences();
			}
			else if(text == "Disable Tool Tips")
			{
				Config::Get()->setEnableToolTips(false);
				
				gedString *pEnableDisableToolTips = listConfig->GetTextPtr("Disable Tool Tips");
				if(pEnableDisableToolTips)
				{
					*pEnableDisableToolTips = "Enable Tool Tips";
				}
			}
			else if(text == "Enable Tool Tips")
			{
				Config::Get()->setEnableToolTips(true);
				
				gedString *pEnableDisableToolTips = listConfig->GetTextPtr("Enable Tool Tips");
				if(pEnableDisableToolTips)
				{
					*pEnableDisableToolTips = "Disable Tool Tips";
				}
			}
		}
		break;
	case LS_REGIONS:
		{
			switch(index)
			{
			case 0: //Add Activation Region
#if !defined(GAME_EDITOR_HOME_EDITION)
				new RegionLoad();	
#else
				new PanelInfo(GAME_EDITOR_VERSION_ERROR);
				return true;
#endif
				break;
			case 1: //show/hide
				
				if(RegionLoad::getShowRegions())
				{
					*pShowHideRegions = "Show Regions";
				}
				else
				{
					*pShowHideRegions = "Hide Regions";
				}

				RegionLoad::ToggleVisibility();
				Actor::RegionActorToggleVisibility();
				break;
			}
		}
		break;
	case LS_HELP:
		{
			if(text == "Documentation")
			{
				EditorDirectory editDir;
				
				//Firefox don't open "Docs/index.html" in windows
				//So, use \ on windows and / on linux
				openUrl((gedString("Docs") + DIR_SEP + "index.html").c_str());
			}
			else if(text == "About...")
			{
				new AboutDlg();
			}
			else if(text == "User Forums...")
			{
				openUrl("http://game-editor.com/forum");
			}			
			else if(text == "Get Newest Tutorials...")
			{
				TutorialUpdateDlg::Call();
			}
			else if(text == "Check for Updates...")
			{
				UpdateCheck();
			}
			else if(text == "Game Demos...")
			{
				openUrl("http://game-editor.com/demos.html");				
			}
			#if defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL) && !defined(STAND_ALONE_GAME) && !defined(GAME_EDITOR_HOME_EDITION)
			else if(text == BUY_NOW)
			{
				openBuySite();
			}
			#endif
			else
			{
				//Tutorials
				if(chdir((GameControl::getEditorPath() + DIR_SEP + "Tutorials" + DIR_SEP + text).c_str()) == 0)
				{				
					listTutorialDir->RemoveAll();
					listTutorialDir->getImage()->SetVisible(true);				
					PostMessage(this, 0);
				}
			}
		}
		break;
	case LS_TUTORIALDIR:
		{		
			PanelQuestion *panel = new PanelQuestion("This will discard the current game.\nProceed anyway?", "Tutorial Execution");
			if(panel->Wait() == OK_BUTTON)
			{
				PostMessage(this, 1);					
			}

			delete panel;	
			
		}
		break;

	case LS_EDIT_MENU:
		{
			switch(index)
			{
			case 0:
				editBoxWidget->Cut();
				break;

			case 1:
				editBoxWidget->Copy();
				break;

			case 2:
				editBoxWidget->Paste();
				break;
			}			
		}
		break;

	case LS_SCRIPT:
		{
			if(text == "Global code")
			{
				new ScriptGlobals();
			}
			else
			{
				stAction *action = (stAction *)list->GetItemData(index);
				gedString actorName(text.substr(0, text.find(' ')));

				Actor *actor = GameControl::Get()->GetActor(actorName);								

				if(actor && action)
				{
					AddLastScript(text);
					Action::ShowActionDialog(action, actor);			
				}
			}
		}
		break;
	}

	return true;
}

bool MainPanel::LoadGame(gedString file, bool bShowError)
{
	lastScripts.Clear();
	gedString s(file);
	s.lower();

	if(s.substr(s.length() - 4, 4) == ".ged" && LoadSaveGame::Load(file, true))
	{
		if(Tutorial::IsOff())
		{
			RecentFileList mru;
			mru.Add(file);
		}
	}
	else
	{
		if(Tutorial::IsOff())
		{
			RecentFileList mru;
			mru.Remove(file);
		}
		
		if(bShowError) new PanelInfo("Can't load this project");
		GameControl::Get()->NewGame();
		UndoControl::Get()->Clear();

		return false;
	}
	
	MainPanel::Get()->FileSetup();

	return true;
}

void MainPanel::OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message)
{
	switch(messageId)
	{
	case 0:
		listTutorialDir->OnMouseButtonDown(GameControl::Get()->getMouseX(), GameControl::Get()->getMouseY(), 1);
		break;
	case 1:
		if(Tutorial::Get()->Load(listTutorialDir->GetFullPath()))
		{
			Tutorial::Get()->Play();
			MainPanel::Create()->FileSetup();
		}
		else
		{
			new PanelInfo("Tutorial can't be oppen.\nPlease, update your tutorials.");
			Tutorial::Destroy();
		}

		break;
	}
}

void MainPanel::OnKeyDown(SDLKey key, int repeatLoopCount)
{
	if(GameControl::Get()->getStandAloneMode() || GameControl::Get()->getGameMode()) return;
	
	//Don't works in some systems if called in key up (don't works on Ingrid computer)
	//May be ctrl is up when other key is up
	SDLMod keyMod = SDL_GetModState();
	
	if((keyMod & KMOD_RCTRL) || (keyMod & KMOD_LCTRL))
	{
		switch(key)
		{
			
		case SDLK_z:
			if(PathDialog::getPathDialog())
			{
				new PanelInfo("Undo is not available while editing paths");
			}
			else if(!UndoControl::Get()->Undo())
			{
				new PanelInfo("No previous operation to undo");
			}
			break;
			
		case SDLK_y:
		case SDLK_r:
			if(PathDialog::getPathDialog())
			{
				new PanelInfo("Redo is not available while editing paths");
			}
			else if(!UndoControl::Get()->Redo())
			{
				new PanelInfo("No next operation to redo");
			}
			break;
		}
	}
}

void MainPanel::OnKeyUp(SDLKey key)
{
	if(GameControl::Get()->getStandAloneMode()) return;
	
	switch(key)
	{
	case SDLK_ESCAPE:
		GameControl::Get()->SetGameMode(false);	
		ActorEdit::DoActivationEvent(false);
		GameControl::Get()->RestoreGame();
		break;	
		
	case SDLK_LSHIFT:
		GameControl::Get()->ToggleSnap();	
		
		if(GameControl::Get()->getSnapMode())
		{
			iconSnap1->getImage()->SetVisible(true);
			iconSnap2->getImage()->SetVisible(true);
			iconSnap3->getImage()->SetVisible(true);
			iconSnap4->getImage()->SetVisible(true);
		}
		else
		{
			iconSnap1->getImage()->SetVisible(false);
			iconSnap2->getImage()->SetVisible(false);
			iconSnap3->getImage()->SetVisible(false);
			iconSnap4->getImage()->SetVisible(false);
		}
		
		break;
	}
	
}

void MainPanel::OnMouseMove(int x, int y)
{
	GameControl::Get()->GetAxis()->GridSnap(x, y);

	KrVector2T< GlFixed > object;
	GameControl::Get()->GetAxis()->getImage()->ScreenToObject(x, y, &object);

	SetCoord(object.x.ToInt(), object.y.ToInt());
}

void MainPanel::ShowActorCoordinates(Actor *actor)
{
	if(actor && (actor->EditMode() || actor->isRegion())) 
	{
		iconCoord->getImage()->SetVisible(true);
		SetCoord(actor->getImage()->X(), actor->getImage()->Y());
	}
	else 
		iconCoord->getImage()->SetVisible(false);

#ifdef wxUSE_GUI
	PanelProperty::UpdateActorPosition();		
#endif
}

void MainPanel::SetCoord(int x, int y)
{
	char buf[64];
	sprintf(buf, "(%ld, %ld)\0", x, y);
	coord->SetText(buf);
}

void MainPanel::EditBoxContextMenu(KrTextWidget *textWidget, int x, int y)
{
	editBoxWidget = textWidget;

	if(!listEditBox)
	{
		listEditBox = new ListPop(0, 0, 1, 1, LS_EDIT_MENU, this);
		listEditBox->setCenterListOnMouse(false);

		listEditBox->AddText("Cut     Ctrl+X");
		listEditBox->AddText("Copy    Ctrl+C");
		listEditBox->AddText("Paste   Ctrl+V");				
	}	

	listEditBox->SetParent(this);
	listEditBox->getImage()->SetVisible(true);
	listEditBox->OnMouseButtonDown(x, y, SDL_BUTTON_RIGHT);
}

#endif //#ifndef STAND_ALONE_GAME


#ifndef wxUSE_GUI
#include "GameControl.h"

/*int AddInGameGraphControl::n = 0;
Actor *AddInGameGraphControl::to = NULL;
int AddInGameGraphControl::interation = 0;*/

#ifndef STAND_ALONE_GAME

#ifdef WIN32
extern "C" HWND SDL_Window;
void *GetSDLPanelWindow()
{
	return SDL_Window;
}

void *GetMainWindow()
{
	return SDL_Window;
}
#else
void *GetSDLPanelWindow()
{
	return NULL;
}

void *GetMainWindow()
{
	return NULL;
}
#endif

#endif

void SetMainWindowGameMode(bool bGameMode)
{
}

void ShowMainWindow(bool bShow) 
{
}

void SetMainWindowCaption(const char *str, const char *icon)
{
	//Need to work in stand alone too to set the game title
#if defined(WIN32)
	SDL_WM_SetCaption(str, icon);
#endif
}

void AddToPanelActors(Actor *_actor, bool bSendMesage)
{

}

void RemoveFromPanelActors(Actor *_actor)
{
}

void RemoveAllPanelActors()
{
}

void RemoveSelectedActor()
{
}

void GridSelected(Actor *_actor)
{
}

void UpdateActorSize()
{
}

void UpdateActorAnim(Actor *_actor)
{
}

void UpdatePanelEvents()
{
}

void AddToGameGraph(Actor *to, int interation, Actor *from = NULL)
{
}

void ClearGameGraph()
{
}

void GenerateGameGraph()
{
}

void UpdatePanelScript()
{
}

#endif


