// PlaySong.cpp: implementation of the PlaySong class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "PlaySong.h"
#include "GameControl.h"
#include "ExpressionEditor.h"
#include "Tutorial.h"
#include "Config.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_PLAY_MUSIC			"\
Use this action to play the selected music\n\
You can repeat the music by changing the loop value (1 to 65000 or 0 to infinite loop)\n\
\n\
Music files are not entirely loaded into memory, so, big musics don't will consume big resources\n\
Only one music will be played at a time"

#define TIP_PLAY_MUSIC_FILE			"\
Select a music file\n\
Supports wav, mid, ogg, mod, s3m, it and xm music files on the desktop\n\
and wav, ogg, mod, s3m, it and xm music files on the PocketPC"

#define TIP_PLAY_MUSIC_WEB			"Click here to get more musics on the Internet"

#define TIP_PLAY_MUSIC_VOLUME		"Set the music volume"

#define TIP_PLAY_MUSIC_PRIORITY		"\
Startup Priority:\n\
\n\
In slow machines, like PDA devices, the music can be loaded slowly\n\
To fix this, set the best option for your game:\n\
\n\
     High: the game stops while load the music, but is the fastest load mode\n\
   Medium: the game don't stop, but can be slow\n\
      Low: the game will play in normal velocity, but the music load will be slow"




#define TIP_PLAY_SOUND			"\
Use this action to play the selected sound\n\
You can repeat the sound by changing the loop value (1 to 65000 or 0 to infinite loop)\n\
\n\
The maximum simultaneous sound must be adjusted in \"Game Properties\" panel"

#define TIP_PLAY_SOUND_FILE			"\
Select a sound file\n\
Supports wav, voc, iff and 8svx sound files"

#define TIP_PLAY_SOUND_WEB			"Click here to get more sounds on the Internet"

#define TIP_PLAY_SOUND_VOLUME		"Set the sound volume"

#define TIP_PLAY_SOUND_PAN			"Set the left/right sound balance"

//////////////////////////////////////////////////////////////////////////////

enum
{
	LS_MUSIC_MODE,
	SL_VOLUME,
	SL_PAN,
	BT_CLOSE,
	BT_ADD,
	BT_WEB
};

#define WIDTH	235
#define HEIGHT	135

char soundPath[PATH_LIMIT];
static bool bInitPath = false;

PlaySong::PlaySong(Actor *actor, bool bOnCollision, bool bMusic)
	: BaseAction("Play Song", (GameControl::Get()->Width() - WIDTH)/2, 
	(GameControl::Get()->Height() - (bMusic?HEIGHT+35:HEIGHT))/2,
						  WIDTH, (bMusic?HEIGHT+35:HEIGHT))
{
	SetModal();
	eventActor = actor;
	this->bMusic = bMusic;
	

	Text *text;
	Button *button, *webButton;	
	int y;

	//Title
	text = AddText(bMusic?PLAY_MUSIC:PLAY_SOUND, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("  File: ", 10, y);
	path = AddListDirectory(text->Right(), text->Top());
	if(bMusic)
	{
		path->AddFilter("wav");
		path->AddFilter("mid");
		path->AddFilter("midi");
		path->AddFilter("ogg");
		//path->AddFilter("mp3");
		path->AddFilter("mod");
		path->AddFilter("s3m");
		path->AddFilter("it");
		path->AddFilter("xm");
	}
	else
	{
		path->AddFilter("wav");
		//path->AddFilter("ogg");
		path->AddFilter("voc");
		path->AddFilter("iff");
		path->AddFilter("aiff");
		path->AddFilter("riff");
		path->AddFilter("8svx");
	}

	//webButton = AddButton("Web", path->Right() + 2, path->Top(), 35, 0, BT_WEB);


	text = AddText("Volume: ", 10, path->Down() + 2);
	slide = AddSlide(text->Right(), text->Top(), 100, 14, SL_VOLUME);
	slide->SetPos(1.0);

	if(!bMusic)
	{
		text = AddText("   Pan: ", 10, slide->Down() + 2);
		panSlide = AddSlide(text->Right(), text->Top(), 100, 14, SL_PAN);
		panSlide->SetPos(0.5);

		y = panSlide->Down() + 2;
	}
	else
	{
		y = slide->Down() + 2;
		panSlide = NULL;
	}

	text = AddText("  Loop: ", 10, y);
	loop = AddEditBox(text->Right(), text->Top(), 64); loop->SetNumeric(0, 65000, 1);
	loop->SetText(1);
	
	if(bMusic)
	{
		text = AddText("Startup Priority: ", 10, loop->Down() + 2);
		musicMode = AddListPop(text->Right() + 2, text->Top(), 64, 0, LS_MUSIC_MODE);
		
		musicMode->AddText("High");
		musicMode->AddText("Medium");
		musicMode->AddText("Low");
		musicMode->SetItem("High");

		text = AddText("Note: Priority has effect on\n      PocketPC and slow machines.", text->Left(), musicMode->Down() + 4);	
		
		y = text->Down() + 2;
	}
	else
		y = loop->Down() + 2;
	
	//Close
	y = DrawHLine(y);
	button = AddButton(Action::getEditAction()?"Ok":"Add", 55, y, 0, 0, BT_ADD); button->SetToolTip(TIP_ACTION_ADD); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right()+8, y, 0, 0, BT_CLOSE); button->SetToolTip(TIP_ACTION_CANCEL); SetCancelButton(button);	

	music = NULL;
	sound = NULL;
	volume = SDL_MIX_MAXVOLUME;

	if(!Tutorial::IsOff())
	{
		chdir(Tutorial::Get()->getTutorialPath());
	}
	else
	{
		if(!bInitPath)
		{
			soundPath[0] = 0;
			bInitPath = true;
			
			if(!Config::Get()->getPathSounds().empty())
			{
				chdir(Config::Get()->getPathSounds().c_str());
			}
			
		}
		else chdir(soundPath);
	}

	if(bMusic)
	{
		SetToolTip(TIP_PLAY_MUSIC);
		path->SetToolTip(TIP_PLAY_MUSIC_FILE);
		//webButton->SetToolTip(TIP_PLAY_MUSIC_WEB);
		slide->SetToolTip(TIP_PLAY_MUSIC_VOLUME);
		musicMode->SetToolTip(TIP_PLAY_MUSIC_PRIORITY);
	}
	else
	{
		SetToolTip(TIP_PLAY_SOUND);
		path->SetToolTip(TIP_PLAY_SOUND_FILE);
		//webButton->SetToolTip(TIP_PLAY_SOUND_WEB);
		slide->SetToolTip(TIP_PLAY_SOUND_VOLUME);
		if(panSlide) panSlide->SetToolTip(TIP_PLAY_SOUND_PAN);
	}

	if(!ExpressionEditor::getExpressionEditor()) UpdateEdition();
}

void PlaySong::ResetPath()
{
	bInitPath = false;
}

void PlaySong::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	slide->SetPos(editAction->data.sound.volume/(double)SDL_MIX_MAXVOLUME);
	if(panSlide) panSlide->SetPos(editAction->data.sound.pan/510.0 + 0.5); //[-255, 255] -> [0.0, 1.0]

	if(editAction->data.sound.loop < 0) editAction->data.sound.loop = 0;
	loop->SetText(editAction->data.sound.loop);

	if(bMusic)
	{
		switch(editAction->data.sound.priority)
		{
		case HIGH_PRIORITY_MUSIC:
			musicMode->SetItem("High");
			break;
		case MEDIUM_PRIORITY_MUSIC:
			musicMode->SetItem("Medium");
			break;
		case LOW_PRIORITY_MUSIC:
			musicMode->SetItem("Low");
			break;
		}
	}

	//Update file and directory
	gedString file, dir;
	SplitPath(editAction->data.sound.path, file, dir);
	path->AddText(file);
	path->SetItem(file);
	chdir(dir.c_str());

	Activate();
}

PlaySong::~PlaySong()
{
	if(sound) Mix_FreeChunk(sound);
	if(music) Mix_FreeMusic(music);
}

bool PlaySong::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(BaseAction::OnList(list, index, text, listId)) return true;

	if(listId != LS_MUSIC_MODE) Activate();

	return true;
}

void PlaySong::OnSlide(Slide *slide, int slideId, double pos)
{
	if(slideId == SL_VOLUME) volume = (int)(SDL_MIX_MAXVOLUME*pos);

	if(music)
	{
		Mix_VolumeMusic(volume);
	}
	else if(sound && slideId == SL_VOLUME)
	{
		Mix_VolumeChunk(sound, volume);
	}

	Activate();
}

void PlaySong::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_CLOSE:
		{
			delete this;
		}
		break;
	case BT_WEB:
		{
			openUrl(bMusic?"http://game-editor.com/musics.html":"http://game-editor.com/sounds.html");
		}
		break;
	case BT_ADD:
		{
			
			if(path->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select a file");
				return;
			}	

			if(!sound && !music)				
			{
				gedString format(path->GetFullPath().GetFileName());
				format.lower();

				if(format.substr(format.length() - 4, 4) == ".wav")
				{
					//May be an encode error
					new PanelInfo("This wav file is not supported.\nPlease, convert this file to a PCM encoded wav file or select other file");
					return;	
				}

				new PanelInfo("This format is not supported or file is corrupted\nPlease, select other file");
				return;
			}

			int loopCount = atol(loop->GetText().c_str());
			if(!ExpressionEditor::getExpressionEditor() && (loopCount < 1 || loopCount > 65000))
			{
				new PanelInfo("Invalide loop count");
				return;
			}

			int priority = HIGH_PRIORITY_MUSIC;
			gedString sPriority = "HIGH_PRIORITY_MUSIC";

			if(bMusic)
			{
				if(musicMode->GetText() == "Medium")
				{
					priority = MEDIUM_PRIORITY_MUSIC;
					sPriority = "MEDIUM_PRIORITY_MUSIC";
				}
				else if(musicMode->GetText() == "Low")
				{
					priority = LOW_PRIORITY_MUSIC;
					sPriority = "LOW_PRIORITY_MUSIC";
				}
			}

			//Store current path
			ged_getcwd(soundPath, PATH_LIMIT);
			Config::Get()->setPathSounds(soundPath);

			if(IsEditingScript())
			{
				//Auto complete script
				gedString path1 = path->GetFullPath();
				const char *pathOri = path1.c_str();
				gedString pathStr;
				for(int i = 0; i < strlen(pathOri); i++)
				{
					pathStr += pathOri[i];
					if(pathOri[i] == '\\') pathStr += "\\";
				}

				char buf[256];
				double vol = slide->GetPos();
				if(vol > 1.0) vol = 1.0;
				
				if(bMusic) 
				{
					sprintf(buf, "PlayMusic2(\"%s\", %f, %ld, %s);",
						pathStr.c_str(),
						vol,
						loopCount,
						sPriority.c_str());
				}
				else if(panSlide)
				{
					//[0.0, 1.0] -> [-1.0, 1.0]
					double dPan = (panSlide->GetPos() - 0.5)*2.0;

					sprintf(buf, "PlaySound2(\"%s\", %f, %ld, %f);",
						pathStr.c_str(),
						vol,
						loopCount,
						dPan);
				}				
				
				InsertIntoScript(buf);
				delete this;
			}
			else
			{
				Sint16 pan = 0;
				if(panSlide) pan = (Sint16)((panSlide->GetPos() - 0.5)*510);
				SelectActionType(eventActor, Action::Call(eventActor)->SetSound(bMusic, path->GetFullPath(), volume, loopCount, priority, pan));
			}					
		}
		break;
	}
}


void PlaySong::Activate()
{
	gedString pathStr = path->GetFullPath();
	int loopCount = atol(loop->GetText().c_str());
	if(loopCount <= 0) loopCount = 1000000; //infinite
	else if(loopCount > 65000) loopCount = 65000;

	if(bMusic)
	{		
		if(music) Mix_FreeMusic(music);
		music = ged_Mix_LoadMUS(pathStr.c_str());
		if(music) Mix_PlayMusic(music, loopCount-1);
	}
	else
	{
		if(sound) Mix_FreeChunk(sound);
		sound = ged_Mix_LoadWAV(pathStr.c_str());
		if(sound)
		{
			Mix_VolumeChunk(sound, volume);
			int channel = Mix_PlayChannel(-1, sound, loopCount-1);

			if(panSlide)
			{
				//[0.0, 1.0] -> [-255, 255]
				Sint16 pan = (Sint16)((panSlide->GetPos() - 0.5)*510);

				Mix_SetPanning(channel, (pan <= 0)?255:(255-pan), (pan < 0)?(255+pan):255); 
			}
		}
	}
}


#endif //#ifndef STAND_ALONE_GAME