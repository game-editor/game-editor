// GameSettings.cpp: implementation of the GameSettings class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "GameSettings.h"
#include "GameControl.h"
#include "DlgPocketPCKey.h"
#include "Tutorial.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_GAMESETTINGS				"\
Change the general game settings\n\
Specify the game velocity in frames per second,\n\
maximum simultaneos sounds in game, game title and others.\n\
\n\
Use the safe margin to increase the view region used in Out of Vision events.\n\
\n\
Game ID:\n\
\n\
  The Game ID is used to identify your game over the network.\n\
  It should be unique, don't use a generic name that another developer might also use.\n\
  A value will be suggested for you for each new game.\n\
\n\
Network Port:\n\
\n\
  Port number to communicate with the remote machines."

#define TIP_GAMESETTINGS_RESOLUTION		"\
Select a machine detected resolution or enter a custom resolution\n\
On Pocket PC devices you can change the screen orientation"

#define TIP_GAMESETTINGS_MODE			"\
Specify if your game will run in Window or Full Screen mode\n\
On the PocketPc only the Full Screen mode is supported"

#define TIP_GAMESETTINGS_MOUSE			"Specify if mouse will be show or hide in the game"
#define TIP_GAMESETTINGS_SUSPEND		"\
Select \"Yes\" to pause the game when lost the window focus on desktop\n\
or receives a system message on PocketPC (battery alerts, phone calls, ...)\n\
\n\
On the PocketPC will show a game screenshot and a menu to continue or exit the game"

#define TIP_GAMESETTINGS_AUDIO_HZ		"Set the audio quality (frequency in Hz)"
#define TIP_GAMESETTINGS_AUDIO_STEREO	"Choose if your game must play audio in mono or stereo"
#define TIP_GAMESETTINGS_ICON			"\
Select the game icon file.\
To show the icon in Windows or PocketPC files,\
specify a .ico icon file"

#define TIP_GAMESETTINGS_POCKET_KEYS	"\
Use to remap Pocket PC or GP2X keys to desktop keys used in game.\n\
\n\
You can make your game using the normal desktop keyboard\n\
and define which device keys will be remaped to desktop keys\n\
\n\
The key position can be different in different equipments"


#define TIP_GAMESETTINGS_ESC	"\
Select \"Yes\" to exit game when use press the ESC key.\n\
If you don't want use ESC key to exit, select \"No\" and\n\
use the ExitGame() function in any action in your game to exit.\n\
\n\
This option will works only in your exported game."

#define TIP_GAMESETTINGS_MOTION_COMPENSATION	"\
Select \"Yes\" to correct actor velocity if real framerate is slow than game framerate."

#define TIP_GAMESETTINGS_GAME_SERVER	"\
Select \"Yes\" to use the Game Editor Server.\n\
The Game Editor Server will tell to the game where to find other machines running the same game."

#define TIP_GAMESETTINGS_AUTO_START_NETWORK	"\
Specify whether the network will be started when game starts.\n\
Always after connect to the remote machine the local game will be synchronized with the remote game.\n\
Global pointers and dynamic allocated memory in the scripts aren't synchronized."



//////////////////////////////////////////////////////////////////////////////

enum
{
	LS_RES,
	LS_FULL,
	LS_HZ,
	LS_STEREO,
	BT_CLOSE,
	BT_ADD,
	LS_MOUSE,
	LS_SUSPEND,
	LS_ESC,
	LS_MOTION,
	LS_POCKET_KEYS,
	LS_GAMESERVER,
	LS_AUTOSTART_NETWORK
};

#define WIDTH	380
#define HEIGHT	280

GameSettings *GameSettings::gameSettings = NULL;

static int getHeight()
{
	int height = HEIGHT;

	if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		height += 23;

		if(Tutorial::IsCompatible(VERSION_ESC_TO_EXIT))
		{
			height += 52;
		}
	}

	if(Tutorial::IsCompatible(VERSION_NETWORK) && GAME_EDITOR_VERSION >= VERSION_NETWORK) height += 98;

	return height;
}

GameSettings::GameSettings()
	: Panel("GameSettings", (GameControl::Get()->Width() - WIDTH)/2, 
	(GameControl::Get()->Height() - getHeight())/2,
						  WIDTH, getHeight())
{
	SetModal();
	SetToolTip(TIP_GAMESETTINGS);

	gameSettings = this;
	listSuspend = listESC = listMotion = NULL;
	editViewSafeMargin = NULL;
	gamePort = gameID = NULL;
	listAutoStartNetwork = listGameServer = NULL;

	Text *text;
	Button *button;	
	int y, editSize = 40, editStart = 22;

	if(!Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		editSize = 45;
		editStart = 8;
	}

	//Title
	text = AddText("Game Properties", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("Game resolution: ", 10, y);
	listRes = AddListPop(text->Right(), text->Top(), 128, 0, LS_RES); listRes->SetToolTip(TIP_GAMESETTINGS_RESOLUTION);

	editX = AddEditBox(listRes->Right() + editStart, listRes->Top(), editSize); editX->SetNumeric(2, 2000, 1);
	text = AddText("x", editX->Right()+1, editX->Top());
	editY = AddEditBox(text->Right() + 4, text->Top(), editSize); editY->SetNumeric(2, 2000, 1);

	text = AddText("Mode:            ", 10, editY->Down() + 4);
	listFull = AddListPop(text->Right(), text->Top(), 128, 0, LS_FULL); listFull->SetToolTip(TIP_GAMESETTINGS_MODE);

	if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		text = AddText(" Safe margin: ", listFull->Right(), listFull->Top());
		editViewSafeMargin = AddEditBox(editY->Left(), text->Top(), editSize); editViewSafeMargin->SetNumeric(0, 2000, 1);
	}

	text = AddText("Frame rate:      ", 10, listFull->Down() + 4);
	editFps = AddEditBox(text->Right(), text->Top(), 50); editFps->SetNumeric(1, 512, 1);
	text = AddText("fps", editFps->Right() + 2, editFps->Top());

	listMouse = AddListPop(text->Right() + 32, text->Top(), 128, 0, LS_MOUSE); listMouse->SetToolTip(TIP_GAMESETTINGS_MOUSE);

	if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		text = AddText("Suspend game if lost focus: ", 10, editFps->Down() + 6);
		listSuspend = AddListPop(text->Right() + 48, text->Top(), 32, 0, LS_SUSPEND); listSuspend->SetToolTip(TIP_GAMESETTINGS_SUSPEND);

		if(Tutorial::IsCompatible(VERSION_ESC_TO_EXIT))
		{
			text = AddText("  Use ESC key to exit game: ", 10, listSuspend->Down() + 6);
			listESC = AddListPop(text->Right() + 48, text->Top(), 32, 0, LS_ESC); listESC->SetToolTip(TIP_GAMESETTINGS_ESC);

			text = AddText("       Motion Compensation: ", 10, listESC->Down() + 6);
			listMotion = AddListPop(text->Right() + 48, text->Top(), 32, 0, LS_MOTION); listMotion->SetToolTip(TIP_GAMESETTINGS_MOTION_COMPENSATION);
		}
		
		
		y = DrawHLine(text->Down() + 8);
	}
	else
	{
		y = DrawHLine(editFps->Down() + 4);
	}
	
	text = AddText("Audio format: ", 10, y);
	listHz = AddListPop(text->Right(), text->Top(), 128, 0, LS_HZ); listHz->SetToolTip(TIP_GAMESETTINGS_AUDIO_HZ);
	listStereo = AddListPop(listHz->Right() + 4, listHz->Top(), 128, 0, LS_STEREO); listStereo->SetToolTip(TIP_GAMESETTINGS_AUDIO_STEREO);
	text = AddText("Maximum number of simultaneous sounds: ", 10, listStereo->Down()+4);
	editMaxSounds = AddEditBox(text->Right(), text->Top(), 50); editMaxSounds->SetNumeric(1, 512, 1);
	
	y = DrawHLine(editMaxSounds->Down() + 2);

	text = AddText("            Icon file:  ", 10, y + 2);
	iconPath = AddListDirectory(text->Right(), text->Top()); iconPath->SetToolTip(TIP_GAMESETTINGS_ICON);

	text = AddText("           Game title: ", 10, iconPath->Down() + 4);
	gameTitle = AddEditBox(text->Right(), text->Top(), 150);

	text = AddText("Game background color: ", 10, gameTitle->Down() + 4);
	backgroundColor = new ColorSample(this, text->Right(), text->Top(), 32); backgroundColor->SetToolTip(TIP_GAMESETTINGS_BACKGROUNDCOLOR);

	//Network
	if(Tutorial::IsCompatible(VERSION_NETWORK) && GAME_EDITOR_VERSION >= VERSION_NETWORK)
	{
		y = DrawHLine(backgroundColor->Down() + 2);

		text = AddText("     Game ID: ", 10, y);		
		gameID = AddEditBox(text->Right(), text->Top(), 256);

		text = AddText("Network Port: ", 10, gameID->Down() + 4);		
		gamePort = AddEditBox(text->Right(), text->Top(), 64);

		text = AddText("Use Game Editor Server: ", 10, gamePort->Down() + 4);
		listGameServer = AddListPop(text->Right() + 2, text->Top(), 32, 0, LS_GAMESERVER); listGameServer->SetToolTip(TIP_GAMESETTINGS_GAME_SERVER);

		text = AddText("    Auto Start Network: ", 10, listGameServer->Down() + 4);
		listAutoStartNetwork = AddListPop(text->Right() + 2, text->Top(), 32, 0, LS_AUTOSTART_NETWORK); listAutoStartNetwork->SetToolTip(TIP_GAMESETTINGS_AUTO_START_NETWORK);

		y = listAutoStartNetwork->Down() + 2;
	}
	else
	{
		y = backgroundColor->Down() + 2;
	}

	//PocketPC
	y = DrawHLine(y);
	listPocketKeys = AddListPop(10, y, 256, 0, LS_POCKET_KEYS, "Pocket PC and GP2X Keys"); listPocketKeys->SetToolTip(TIP_GAMESETTINGS_POCKET_KEYS);
	
	//Close
	y = DrawHLine(listPocketKeys->Down() + 2);
	button = AddButton("Ok", Width()/2 - 70, y, 0, 0, BT_ADD); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right()+8, y, 0, 0, BT_CLOSE); SetCancelButton(button);


	//Populate resolutions
	char buf[64];
	if(Tutorial::IsOff())
	{
		SDL_Rect **modes;
		const SDL_VideoInfo *vinfo = SDL_GetVideoInfo();
		
		/* Get available fullscreen/hardware modes */
		modes=SDL_ListModes(vinfo->vfmt, VIDEO_FLAGS | SDL_FULLSCREEN);
		
		if(modes != (SDL_Rect **)0 && modes != (SDL_Rect **)-1)
		{		
			for(int i=0;modes[i];++i)
			{
				sprintf(buf, "%d x %d", modes[i]->w, modes[i]->h);
				listRes->AddText(buf);
			}
		}
	}
	else
	{
		//Don't change in different machines
		listRes->AddText("1280 x 1024");
		listRes->AddText("1280 x 960");
		listRes->AddText("1280 x 768");
		listRes->AddText("1280 x 720");
		listRes->AddText("1152 x 864");
		listRes->AddText("1024 x 768");
		listRes->AddText("848 x 480");
		listRes->AddText("800 x 600");
		listRes->AddText("720 x 576");
		listRes->AddText("720 x 480");
		listRes->AddText("640 x 480");
		listRes->AddText("640 x 400");
		listRes->AddText("512 x 384");
		listRes->AddText("480 x 360");
		listRes->AddText("400 x 300");
		listRes->AddText("320 x 240");
		listRes->AddText("320 x 200");		
	}

	if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		listRes->AddText("240 x 320");
		listRes->AddText("240 x 320 Pocket PC");
		listRes->AddText("240 x 320 Pocket PC Upside down");
		listRes->AddText("320 x 240 Pocket PC Rotate left");
		listRes->AddText("320 x 240 Pocket PC Rotate right");

		listRes->AddText("480 x 640 Pocket PC");
		listRes->AddText("480 x 640 Pocket PC Upside down");
		listRes->AddText("640 x 480 Pocket PC Rotate left");
		listRes->AddText("640 x 480 Pocket PC Rotate right");

		//No changes in screen orientation now (don't have the w > h information)
		listRes->AddText("240 x 240 Pocket PC");
		listRes->AddText("480 x 480 Pocket PC");

		//Smartphone
		listRes->AddText("176 x 220 Smartphone");
		listRes->AddText("160 x 240 Smartphone");
		listRes->AddText("208 x 240 Smartphone");
		listRes->AddText("240 x 320 Smartphone");

		//HPC
		listRes->AddText("640 x 240 Handheld PC");
		listRes->AddText("640 x 480 Handheld PC");
		listRes->AddText("800 x 480 Handheld PC");
		listRes->AddText("800 x 600 Handheld PC");

		//GP2X
		listRes->AddText("320 x 240 GP2X");
		listRes->AddText("640 x 480 GP2X");
	}
	else
	{
		listRes->AddText("240 x 320 Pocket PC");
	}

	listFull->AddText("Full screen");
	listFull->AddText("Window");


	listHz->AddText("44100 Hz");
	listHz->AddText("22050 Hz");
	listHz->AddText("11025 Hz");


	listStereo->AddText("Stereo");
	listStereo->AddText("Mono");

	listMouse->AddText("Show mouse");
	listMouse->AddText("Hide mouse");

	if(listSuspend)
	{
		listSuspend->AddText("Yes");
		listSuspend->AddText("No");
		
		if(GameControl::Get()->getSuspendGameIfLostFocus()) listSuspend->SetItem("Yes");
		else listSuspend->SetItem("No");
	}

	if(listAutoStartNetwork)
	{
		listAutoStartNetwork->AddText("Yes");
		listAutoStartNetwork->AddText("No");
				
		if(GameControl::Get()->getAutoStartNetwork()) listAutoStartNetwork->SetItem("Yes");
		else listAutoStartNetwork->SetItem("No");
	}

	if(listGameServer)
	{
		listGameServer->AddText("Yes");
		listGameServer->AddText("No");
				
		if(GameControl::Get()->getUseGameEditorServer()) listGameServer->SetItem("Yes");
		else listGameServer->SetItem("No");
	}

	if(listESC)
	{
		listESC->AddText("Yes");
		listESC->AddText("No");
		
		if(GameControl::Get()->getUseESCKeyToExit()) listESC->SetItem("Yes");
		else listESC->SetItem("No");
	}

	if(listMotion)
	{
		listMotion->AddText("Yes");
		listMotion->AddText("No");
		
		if(GameControl::Get()->getUseMotionCorrection()) listMotion->SetItem("Yes");
		else listMotion->SetItem("No");
	}

	if(gameID)
	{		
		gameID->SetText(GameControl::Get()->getGameID());
	}

	if(gamePort)
	{		
		gamePort->SetText(GameControl::Get()->getGamePort());
	}


	if(GameControl::Get()->getFullScreen()) listFull->SetItem("Full screen");
	else listFull->SetItem("Window");

	if(GameControl::Get()->getStereo()) listStereo->SetItem("Stereo");
	else listStereo->SetItem("Mono");

	if(GameControl::Get()->getShowMouse()) listMouse->SetItem("Show mouse");
	else listMouse->SetItem("Hide mouse");

	if(editViewSafeMargin)
	{
		editViewSafeMargin->SetText((int)GameControl::Get()->getViewSafeMargin());
	}
	

	sprintf(buf, "%ld Hz", GameControl::Get()->getAudioSamplerRate()); 
	listHz->SetItem(buf);

	sprintf(buf, "%d x %d", GameControl::Get()->getGameWidth(), GameControl::Get()->getGameHeight());
	listRes->SetItem(buf);

	editX->SetText(GameControl::Get()->getGameWidth());
	editY->SetText(GameControl::Get()->getGameHeight());
	editFps->SetText(GameControl::Get()->getFrameRate());
	editMaxSounds->SetText(GameControl::Get()->getMaximumSounds());

	bFlipPocketPCScreen = GameControl::Get()->getFlipPocketPCScreen();

	gedString res(buf);
	if(res == "240 x 320")
	{
		if(bFlipPocketPCScreen)
		{
			listRes->SetText("240 x 320 Up. down");
		}
	}
	else if(res == "320 x 240")
	{
		if(bFlipPocketPCScreen)
		{
			listRes->SetText("320 x 240 Rot.right");
		}
		else
		{
			listRes->SetText("320 x 240 Rot. left");
		}
	}
	else if(res == "480 x 640")
	{
		if(bFlipPocketPCScreen)
		{
			listRes->SetText("480 x 640 Up. down");
		}
		else
		{
			listRes->SetText("480 x 640 Pocket PC");
		}
	}
	else if(res == "640 x 480")
	{
		if(bFlipPocketPCScreen)
		{
			listRes->SetText("640 x 480 Rot.right");
		}
		else
		{
			//listRes->SetText("640 x 480 Rot. left");
			listRes->SetText("640 x 480");
		}
	}
	else if(res == "240 x 240")
	{
		listRes->SetText("240 x 240 Pocket PC");
	}
	else if(res == "480 x 480")
	{
		listRes->SetText("480 x 480 Pocket PC");
	}
	else if(res == "176 x 220")
	{
		listRes->SetText("176 x 220 Smartphone");
	}
	else if(res == "160 x 240")
	{
		listRes->SetText("160 x 240 Smartphone");
	}
	else if(res == "208 x 240")
	{
		listRes->SetText("208 x 240 Smartphone");
	}



	iconPath->AddFilter("tga");
	iconPath->AddFilter("bmp");
	iconPath->AddFilter("pnm");
	iconPath->AddFilter("xpm");
	iconPath->AddFilter("xcf");
	iconPath->AddFilter("pcx");
	iconPath->AddFilter("gif");
	iconPath->AddFilter("jpg");
	iconPath->AddFilter("jpeg");
	iconPath->AddFilter("jpe");
	iconPath->AddFilter("jfif");
	iconPath->AddFilter("tif");
	iconPath->AddFilter("tiff");
	iconPath->AddFilter("iff");
	iconPath->AddFilter("lbm");
	iconPath->AddFilter("png");
	iconPath->AddFilter("ico");

	//iconPath->SetText(GameControl::Get()->getIconPath());

	if(!GameControl::Get()->getIconPath().empty())
	{
		gedString file, dir;
		SplitPath(GameControl::Get()->getIconPath(), file, dir);
		chdir(dir.c_str());

		iconPath->AddText(file);
		iconPath->SetItem(file);
	}

	gameTitle->SetText(GameControl::Get()->getGameTitle());

	backgroundColor->setColor(GameControl::Get()->getBackGroundColor());

	SetPocketKeys();
}

GameSettings::~GameSettings()
{
	delete backgroundColor;
	gameSettings = NULL;
}

void GameSettings::SetPocketKeys()
{
	GameControl::Get()->PopulatePocketPCKeys(listPocketKeys);
}

bool GameSettings::OnList(ListPop *list, int index, gedString &text, int listId)
{
	switch(listId)
	{
	case LS_RES:
		{
			int w, h;
			gedString sFlip(text.substr(text.length() - 4, 4));

			w = atol(text.substr(0, text.find(" ")).c_str());
			h = atol(text.substr(text.find("x ")+2, gedString::npos).c_str());
			editX->SetText(w);
			editY->SetText(h);

			if(sFlip == "down" || sFlip == "ight")
			{
				bFlipPocketPCScreen = true;
			}
			else
			{
				bFlipPocketPCScreen = false;
			}

			if(w == 240 || h == 240)
			{
				if(sFlip == "down") list->SetText("240 x 320 Up. down");
				else if(sFlip == "left") list->SetText("320 x 240 Rot. left");
				else if(sFlip == "ight") list->SetText("320 x 240 Rot.right");
			}
			else if(w == 480 || h == 480)
			{
				if(sFlip == "down") list->SetText("480 x 640 Up. down");
				else if(sFlip == "left") list->SetText("640 x 480 Rot. left");
				else if(sFlip == "ight") list->SetText("640 x 480 Rot.right");
			}
		}
		break;
	case LS_POCKET_KEYS:
		{
			new DlgPocketPCKey(*((SDLKey *)list->GetItemData(index)));
		}
		break;
	}

	return true;
}

void GameSettings::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_CLOSE:
		{
			delete this;
		}
		break;
	case BT_ADD:
		{	
			int w = atol(editX->GetText().c_str());
			int h = atol(editY->GetText().c_str());
			int fps = atol(editFps->GetText().c_str());
			int smax = atol(editMaxSounds->GetText().c_str());
			int viewSafeArea = 0, port = 0;

			if(gamePort) port = atol(gamePort->GetText().c_str());

			if(editViewSafeMargin)
			{
				viewSafeArea = atol(editViewSafeMargin->GetText().c_str());

				if(viewSafeArea < 0) viewSafeArea = 0;
				else if(viewSafeArea > 2048) viewSafeArea = 2048;
			}

			if(w < 2 || w > 2000 || h < 2 || h > 2000)
			{
				new PanelInfo("Game resolution is invalid");
				return;
			}

			if(fps < 1 || fps > 512)
			{
				new PanelInfo("Frame rate is invalid");
				return;
			}

			if(smax < 1 || smax > 512)
			{
				new PanelInfo("Maximum number of simultaneous sounds is invalid");
				return;
			}

			if(gameID && gameID->GetText().length() > MAX_GAME_ID)
			{
				char s[32];
				sprintf(s, "Maximum game id length is %ld", MAX_GAME_ID);
				new PanelInfo(s);
				return;
			}

			if(gamePort && (port < 0 || port > 65535))
			{
				new PanelInfo("Port number must be a value between 1024 and 65535");
				return;
			}

			

			gedString icon(GameControl::Get()->getIconPath());
			if(iconPath->GetSelectedIndex() >= 0)
			{
				icon = iconPath->GetFullPath();
				
				SDL_RWops *src = ged_SDL_RWFromFile(icon.c_str(), "rb");
				if(src)
				{
					SDL_RWclose(src);
					
					gedString iconExt(icon);
					iconExt.lower();
					if(iconExt.substr(iconExt.length() - 4, 4) != ".ico")
					{
						//Validate only non .ico files
						SDL_Surface* surface = ged_IMG_Load(icon.c_str());
						if(!surface)
						{
							new PanelInfo("This format is not supported or file is corrupted\nPlease, select other file");
							return;
						}
					}
				}
				else
				{
					new PanelInfo("Please, select a file");
					return;
				}
			}
			
			if(gameID)
			{
				if(gameID->GetText() != GameControl::Get()->getGameID())
				{
					PanelQuestion *panel = new PanelQuestion("If you change the ID, this game will be recognized as a different game in the network.\nProceed anyway?");
					if(panel->Wait() != OK_BUTTON)
					{
						delete panel;
						return;
					}

					delete panel;
				}				
			}
			
			bool bStereo = false, bFull = false, bShowMouse = false, bSuspendGameIfLostFocus = false, bUseESCKeyToExit = false, bUseMotionCorrection = false, bUseGameEditorServer = false, bAutoStartNetwork = false;
			if(listStereo->GetText() == "Stereo") bStereo = true;			
			if(listFull->GetText() == "Full screen") bFull = true;
			if(listMouse->GetText() == "Show mouse") bShowMouse = true;
			if(listSuspend && listSuspend->GetText() == "Yes") bSuspendGameIfLostFocus = true;
			if(listAutoStartNetwork && listAutoStartNetwork->GetText() == "Yes") bAutoStartNetwork = true;
			if(listGameServer && listGameServer->GetText() == "Yes") bUseGameEditorServer = true;
			if(listESC && listESC->GetText() == "Yes") bUseESCKeyToExit = true;
			if(listMotion && listMotion->GetText() == "Yes") bUseMotionCorrection = true;

			gedString hz = listHz->GetText();

			if(bFull && (w > 1024 || h > 768))
			{
				PanelQuestion *panel = new PanelQuestion("The selected resolution will be able not to work in full screen in some old computers.\nIf your game must work in old machines, make sure select resolutions below 1024x768.\nUse this resolution anyway?");
				if(panel && panel->Wait() != OK_BUTTON)
				{
					delete panel;
					return;
				}			
				
				delete panel;
			}

			GameControl::Get()->SetGameProperties(w, h, fps, bFull, atol(hz.substr(0, hz.find(" ")).c_str()), bStereo, smax, bShowMouse, icon, gameTitle->GetText(), false, backgroundColor->getColor(), bSuspendGameIfLostFocus, viewSafeArea, bUseESCKeyToExit);
			GameControl::Get()->setFlipPocketPCScreen(bFlipPocketPCScreen);
			GameControl::Get()->setUseMotionCorrection(bUseMotionCorrection);
			GameControl::Get()->setUseGameEditorServer(bUseGameEditorServer);
			GameControl::Get()->setAutoStartNetwork(bAutoStartNetwork);

			if(gameID)
			{				
				GameControl::Get()->setGameID(gameID->GetText());
			}

			if(gamePort)
			{				
				GameControl::Get()->setGamePort(port);
			}

			delete this;
		}
		break;
	}
}



#endif //#ifndef STAND_ALONE_GAME


