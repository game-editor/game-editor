// LoadSaveGame.cpp: implementation of the LoadSaveGame class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "LoadSaveGame.h"
#include "GameControl.h"
#include "ActorProperty.h"
#include "Tutorial.h"
#include "MainPanel.h"
#include "Config.h"
#include "UndoControl.h"

enum
{
	LS_SAVECACHE = 0
};

#define TIP_SAVECACHE "\
If you game has many animations and take a long time to load,\n\
select yes to save your game data in the cache.\n\
With cache file your game will load faster on the next loading.\n\
This cache file can be deleted"

LoadSaveGame *LoadSaveGame::loadSaveGame = NULL;
LoadSaveGame::LoadSaveGame(int command)
: LoadSave(/*command == SAVE_GAME*/false)
{
	this->command = command;
	int dialogCommand;
	loadSaveGame = this;
	
	listSaveCache = NULL;

	//Title
	gedString title = "Save Game";
	if(command == LOAD_GAME) title = "Load Game";
	else if(command == MERGE_GAME)  title = "Merge Game";

	//Command
	if(command == SAVE_GAME)
	{
		dialogCommand = SAVE_DIALOG;	
	}
	else if(command == LOAD_GAME || command == MERGE_GAME)
	{
		dialogCommand = LOAD_DIALOG;
	}

	//Button
	gedString buttontext = "Save";
	if(command == LOAD_GAME || command == MERGE_GAME) buttontext = "Load";
	

	//Init
	Init(command == MERGE_GAME?MERGE_DIALOG:dialogCommand, title, buttontext);

	if(GameControl::Get()->getGamePath().empty())
	{
		chdir(Config::Get()->getPathGame().c_str());
	}
	else
	{
		chdir(GameControl::Get()->getGamePath().c_str());
	}

	SetFilter("*.ged");
	if(command == SAVE_GAME) 
	{
		SetText(GameControl::Get()->getGameName());

		/*Text *text = AddText("Save animations in the cache file: ", 10, dir->Down() + 5);;
		listSaveCache = AddListPop(dir->Left(), text->Top() - 2, 35, 0, LS_SAVECACHE); listSaveCache->SetToolTip(TIP_SAVECACHE);

		listSaveCache->AddText("Yes");
		listSaveCache->AddText("No");

		if(GameControl::Get()->getSaveCache())
		{
			listSaveCache->SetItem("Yes");
		}
		else
		{
			listSaveCache->SetItem("No");
		}*/
	}

	if(!Tutorial::IsOff())
	{
		chdir(Tutorial::Get()->getTutorialPath());
	}
#ifdef USE_SYSTEM_FILE_DIALOG
	else
	{
		if(command == LOAD_GAME || command == SAVE_GAME)
		{
			getImage()->SetVisible(DISABLE);
			dir->OnMouseButtonDown(0, 0, 0);
			
			PostMessage(this, DELETE_ME, DELETE_ME);
		}
	}
#endif
}

LoadSaveGame::~LoadSaveGame()
{

}

bool LoadSaveGame::OnList(ListPop *list, int index, gedString &text, int listId)
{
	LoadSave::OnList(list, index, text, listId);

	if(list == listSaveCache)
	{
		if(text == "Yes")
		{
			GameControl::Get()->setSaveCache(true);
		}
		else
		{
			GameControl::Get()->setSaveCache(false);
		}
	}

#ifdef USE_SYSTEM_FILE_DIALOG
	if(Tutorial::IsOff() && (command == LOAD_GAME || command == SAVE_GAME))
	{
		OnButton(NULL, LOADSAVE_OK);
	}
#endif

	return true;
}

void LoadSaveGame::OnButton(Button *button, int buttonId)
{
	LoadSave::OnButton(button, buttonId);
	if(!IsOk()) return;

	switch(buttonId)
	{
	case LOADSAVE_OK:
		{
			if(command == SAVE_GAME)
			{
				gedString file = GetText();				
				gedString s(file);
				s.lower();
				
				char *buf = (char *)s.c_str();
				int iExt = strlen(buf) - 4;
				if(iExt < 0 || strcmp(&buf[iExt], ".ged") != 0)
				{
					file += ".ged";
				}

				ged_mkdir("data"); //To copy files to a new directory

				Save(file);				
			}
			else if(command == LOAD_GAME || command == MERGE_GAME)
			{
				if(!Load(GetDirectory(), command == LOAD_GAME))
				{
					return;
				}

				//Add to recent file list
				if(Tutorial::IsOff() && command == LOAD_GAME)
				{
					RecentFileList mru;
					mru.Add(GameControl::Get()->getGamePath() + DIR_SEP + GetDirectory());
					Config::Get()->setPathGame(GameControl::Get()->getGamePath());
				}
				MainPanel::Get()->FileSetup();
			}

			PostMessage(this, DELETE_ME, DELETE_ME);
		}	
		break;
	case LOADSAVE_CLOSE:
		{
			PostMessage(this, DELETE_ME, DELETE_ME);
		}
		break;
	}
}


void LoadSaveGame::SaveCache(gedString file)
{
	WaitCursor wait;

	//Remove old caches
	long hnd = ged_findfirst((file+"*.cache").c_str(), A_ANY);
	if(hnd != GED_NOTFOUND)
	{	
		do
		{
			remove(ged_filename(hnd));
			
		} while((hnd = ged_findnext(hnd)) != GED_NOTFOUND);
		
		ged_findclose(hnd);
	}

	//Calculate check sum of all data files
	if(chdir("data") == 0)
	{
		U32 chk = 0;
		hnd = ged_findfirst("*.*", A_ANY);
		if(hnd != GED_NOTFOUND)
		{	
			do
			{
				chk += FileCrc32(ged_filename(hnd));
				
			} while((hnd = ged_findnext(hnd)) != GED_NOTFOUND);
			
			ged_findclose(hnd);
		}
		
		chdir("..");

		char buf[4096];
		sprintf(buf, "%s%8X.cache", file.c_str(), chk);

		SDL_ClearError();
		engine->Vault()->SaveDatFile(buf, false);
	}
}

bool LoadSaveGame::Load(gedString filePath, bool bLoad)
{
	//Close actor dialog
	ActorProperty::Destroy();	

	UndoControl::Get()->Clear();
	GameControl::Get()->setFastFile(false);
	
	chdir(filePath.GetFilePath().c_str());

	KrResourceVault::resetVersionDat();

	if(bLoad) 
	{
		//Clear before load the cache to avoid cache inflaction
		GameControl::Get()->ClearGame();
		LoadCache(filePath);
	}
	else
	{
		//Copy the data files from merge to current game path
		if(!GameControl::Get()->getGamePath().empty())
		{
			copyfile((gedString("data") + DIR_SEP + "*.*").c_str(), (GameControl::Get()->getGamePath() + DIR_SEP + "data").c_str(), "data");
		}
	}

	
	if(!GameControl::Get()->Load(filePath.GetFileName(), true, !bLoad))
	{
		ActorProperty::Destroy();
		return false;
	}

	if(Tutorial::IsOff()) 
	{
		ActorProperty::Call(GameControl::Get()->GetActor("view"));
	}

	return true;
}

bool LoadSaveGame::Save(gedString file)
{
	if(!file.GetFilePath().empty())
	{
		//Has a path, change the dir
		chdir(file.GetFilePath().c_str());
		file = file.GetFileName();
	}

	GameControl::Get()->setFastFile(false);

	if(!GameControl::Get()->Save(file))
	{
		char buf[1024];
		sprintf(buf, "Please, verify if '%s' file is not read only.", file.c_str());
		new PanelInfo(buf);
		
		return false;
	}
	
	UndoControl::Get()->Save();
	
	/*if(GameControl::Get()->getSaveCache())
	{
		SaveCache(file);
	}
	else
	{
		//Delete old cache
		//Remove old caches
		long hnd = ged_findfirst((file+"*.cache").c_str(), A_ANY);
		if(hnd != GED_NOTFOUND)
		{	
			do
			{
				remove(ged_filename(hnd));
				
			} while((hnd = ged_findnext(hnd)) != GED_NOTFOUND);
			
			ged_findclose(hnd);
		}
	}*/
	
	
	//Add to recent file list
	if(Tutorial::IsOff())
	{
		RecentFileList mru;
		mru.Add(GameControl::Get()->getGamePath() + DIR_SEP + file);
		Config::Get()->setPathGame(GameControl::Get()->getGamePath());
		
/*#if !defined(GAME_EDITOR_PROFESSIONAL) && !defined(STAND_ALONE_GAME) && defined(WIN32)
		new PanelInfo("You are using the Game Editor Demo. The games saved with\nthis version can be read/played only on this computer.\n\nVisit www.game-editor.com to purchase the full version", "Info");
#endif*/
		
	}
	MainPanel::Get()->FileSetup();

	return true;
}

void LoadSaveGame::LoadCache(gedString file)
{
	WaitCursor wait;

	//Has cache?
	bool bHasCache = false;
	long hnd = ged_findfirst((file+"*.cache").c_str(), A_ANY);
	if(hnd != GED_NOTFOUND)
	{	
		do
		{
			bHasCache = true;
			break;
			
		} while((hnd = ged_findnext(hnd)) != GED_NOTFOUND);
		
		ged_findclose(hnd);
	}

	if(!bHasCache) return;

	//Load cache only if still valid

	U32 chk = 0;

	if(chdir("data") == 0)
	{
		long hnd = ged_findfirst("*.*", A_ANY);
		if(hnd != GED_NOTFOUND)
		{	
			do
			{
				chk += FileCrc32(ged_filename(hnd));
				
			} while((hnd = ged_findnext(hnd)) != GED_NOTFOUND);
			
			ged_findclose(hnd);
		}
		
		chdir("..");		
	}	

	char buf[4096];
	sprintf(buf, "%s%8X.cache", file.c_str(), chk);

	SDL_ClearError();
	engine->Vault()->LoadDatFile(buf, true);
}


#endif //#ifndef STAND_ALONE_GAME




