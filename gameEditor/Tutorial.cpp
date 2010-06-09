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


// Tutorial.cpp: implementation of the Tutorial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(STAND_ALONE_GAME)

#include "Tutorial.h"
#include "GameControl.h"
#include "PlaySong.h"
#include "LoadSaveScript.h"
#include "AddActionDialog.h"
#include "ActorText.h"
#include "ActorProperty.h"
#include "compression.h"
#include "TutorialTextEditor.h"
#include "TutorialControlPanel.h"
#include "TutorialPaint.h"
#include "MainPanel.h"
#include "../gameEngine/ListPopSelect.h"
#include "engine/../tinyxml/tinyxml.h"

#include "../gameEngine/dlmalloc.h"

#if defined(WIN32)
extern "C" HWND SDL_Window;
#endif

double distance(double x1, double y1, double x2, double y2);

Tutorial *Tutorial::tutorial = NULL;
char Tutorial::mode = TUTORIAL_NONE;

Tutorial::Tutorial()
{
	tutorial = this;
	memSlots = 1000;
	gameEditorTutorialVersion = 0;

	events = (stTutorialEvent *)realloc(NULL, memSlots*sizeof(stTutorialEvent));

	Reset();
}

Tutorial::~Tutorial()
{
	free(events);
	tutorial = NULL;	
}

void Tutorial::Reset()
{
	startTime = SDL_GetTicks();
	pauseDelay = initialPauseTime = totalPauseTime = lastExecutionTime = lastTimeStamp = 0;
	totalEvents = eventCount = 0;
	bPauseRequested = bPause = bUserPause = bActivatePause = bWaitPause = bPauseByExplanation = false;
	bIgnoreMouseMotionEvent = bMouseButtonDown = false;
	bPaintFinish = false;
	bCanPause = true;

	timeDelay = 1.0;
	
	xAnt = yAnt = -1;

	nList = 0;

	KrRGBA backgroundColor;
	backgroundColor.all = 0;

	//Reset editor
	GameControl::Get()->SetGameMode(false);
	GameControl::Get()->NewGame(true);
	GameControl::Get()->GetAxis()->setGridShow(true);
	GameControl::Get()->GetAxis()->setGridSnap(false);
	GameControl::Get()->GetAxis()->setGridX(20);
	GameControl::Get()->GetAxis()->setGridY(20);
	GameControl::Get()->SetGameProperties(640, 480, 30, false, 44100, true, 8, true, "", "", true, backgroundColor, true, (Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION)?20:0), true);
	GameControl::Get()->SwitchResolution(engine->Surface(), 640, 480);
	GameControl::Get()->GetAxis()->SetScale(1.0);
	GameControl::Get()->GetAxis()->SetPos(GameControl::Get()->Width()/2, GameControl::Get()->Height()/2);
	GameControl::Get()->GetAxis()->getImage()->Invalidate();

	ActorProperty::Destroy();

	ActorTextBitmap::ResetPath();
	ActorTextTrueType::ResetPath();
	AddActionDialog::ResetPath();
	LoadSaveScript::ResetPath();
	PlaySong::ResetPath();
}

bool Tutorial::IsCompatible(Uint32 version)
{
	if( IsOff() ||
		InRecMode() ||
		tutorial->gameEditorTutorialVersion >= version) 
	{
		return true;
	}


	return false;
}


Tutorial *Tutorial::Get()
{
	if(!tutorial)
	{
		new Tutorial();
	}

	return tutorial;
}

void Tutorial::Destroy()
{
	delete tutorial;
	tutorial = NULL;

	//Restore resolution and allow resize
	GameControl::Get()->SwitchResolution(engine->Surface(), 640, 480);

#ifdef __WIN32__	
	ShowWindow((HWND)GetMainWindow(), SW_RESTORE); //Don't maximize without restore message
	ShowWindow((HWND)GetMainWindow(), SW_SHOWMAXIMIZED);
#endif

	MainPanel::Create();
}

bool Tutorial::IsOff(SDL_Event &event)
{
	if(tutorial == NULL)
	{
#ifdef _DEBUG
		if(event.type == SDL_KEYDOWN)
		{
			switch(event.key.keysym.sym)
			{
			case SDLK_F4:
				Tutorial::Get()->Rec();
				return false;
			}		
		}
#endif
		
		return true;
	}
	
	return false;
}

bool Tutorial::InRecMode()
{
	return mode == TUTORIAL_REC;
}

bool Tutorial::InPlayMode()
{
	return mode == TUTORIAL_PLAY;
}

void Tutorial::Store(SDL_Event &event)
{
#ifdef _DEBUG
	//Store all mouse and keyboard events, and your time stamp
	//relative to tutorial start (subtracting rec pauses times)

	actualEvent = event;

	if(event.type == SDL_ACTIVEEVENT && (event.active.state & SDL_APPINPUTFOCUS/*SDL_APPMOUSEFOCUS*/))
	{
		Pause(PAUSED_BY_ACTIVATE);
		return;
	}

	if(event.type != SDL_KEYDOWN && 
	   event.type != SDL_KEYUP && 
	   event.type != SDL_MOUSEBUTTONDOWN && 
	   event.type != SDL_MOUSEBUTTONUP && 
	   event.type != SDL_MOUSEMOTION &&
	   event.type != SDL_VIDEORESIZE) return;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	F4 - Start Recording,
//	F5 - Tutorialpaint start (was F6 on older builds)
//	F6 - Tutorialpaint start & TutorialTextEditor start. Put a "newline" after your text to get recognized. (was F9 on older builds)
//	F7 - Tutorialpaint stop
//  F8 - Stop Recording and save as "tutorial.gedt"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	if(event.type == SDL_KEYDOWN)
	{
		switch(event.key.keysym.sym)
		{
		case SDLK_F5:
			TutorialPaint::Call();
			return;

		case SDLK_F7:
			TutorialPaint::Destroy();
			return;

		case SDLK_F8:
			TutorialPaint::Destroy();
			Tutorial::Get()->Save("tutorial.gedt");
			return;

		/*case SDLK_F9:
			Pause(PAUSED_BY_USER);
			return;*/

		case SDLK_F6://case SDLK_F10:
			if(!TutorialPaint::InPaintMode())
			{
				TutorialPaint::Call();
			}

			TutorialTextEditor::Call();
			return;
		}		
	}
	

	if(bPause && event.type != SDL_VIDEORESIZE) return; //Resize always must been stored

	SaveEvent(event);	
#endif
}

void Tutorial::SaveEvent(SDL_Event &event, Uint32 timeStamp)
{
	if(eventCount >= memSlots)
	{
		memSlots += 1000;
		events = (stTutorialEvent *)realloc(events, memSlots*sizeof(stTutorialEvent));

#ifdef DEBUG
		GLOUTPUT("Mem slots: %ld\n", memSlots);
#endif

		if(!events) return;
	}

	events[eventCount].event = event;
	
	if(timeStamp)
		events[eventCount].timeStamp = timeStamp - startTime - totalPauseTime;
	else
		events[eventCount].timeStamp = 0;

#ifdef _DEBUG
		if(event.type == SDL_KEYDOWN && events[eventCount].event.key.keysym.sym == 27)
		{
			GLOUTPUT("******************** ESC ******************\n");
		}
#endif

	eventCount++;
	totalEvents++;
}

void Tutorial::SaveActualEvent()
{
	//Save event (in pause mode too)
	SaveEvent(actualEvent, 0);
}

void Tutorial::ResetPauseDelay()
{
	bCanPause = false;
	pauseDelay = SDL_GetTicks();
}

void Tutorial::RemovePauseDelay()
{
	bCanPause = true;	
}

void Tutorial::ForceRedraw()
{
	WaitCursor wait(false);
	//engine->Draw();

	SDL_Event event;
	event.type = SDL_TIMER_EVENT;

	GameControl::Get()->GameTick(event);
}

bool Tutorial::ExecuteMoves(SDL_Event &event)
{
	//Execute all moves from last call to now, if in your time 
	
	if(event.type == SDL_KEYDOWN)
	{
		switch(event.key.keysym.sym)
		{
		case SDLK_F4:
			Rec();
			return true;
		}		
	}
	else if(event.type == SDL_MOUSEMOTION)
	{
		if(xAnt != -1 && !bIgnoreMouseMotionEvent && bCanPause && distance(event.motion.x, event.motion.y, xAnt, yAnt) > 5) 
		{
			if(!bPause && !TutorialPaint::InPaintMode())
			{
				if(!bMouseButtonDown) 
				{
#ifdef _DEBUG					
					GLOUTPUT("Mouse distance: %f\n", distance(event.motion.x, event.motion.y, xAnt, yAnt));
#endif
					Pause(PAUSED_BY_USER);
				}
				else
				{
					bPauseRequested = true;
				}

				//bCanPause = false;
				//pauseDelay = SDL_GetTicks();
			}
			else if(bPaintFinish)
			{
				//End paint info
				Pause(PAUSED_BY_USER);
				TutorialPaint::Destroy();
				bPaintFinish = false;				

				bCanPause = false;
				pauseDelay = SDL_GetTicks();
			}
		}

		xAnt = event.motion.x;
		yAnt = event.motion.y;

		bIgnoreMouseMotionEvent = false;
		return true;
	}
	else if(!bPause && event.type == SDL_ACTIVEEVENT && (event.active.state & SDL_APPINPUTFOCUS/*SDL_APPMOUSEFOCUS*/) && !event.active.gain)
	{
		//Don't use SDL_APPMOUSEFOCUS (Pause after switch resolution)

		//Pause(PAUSED_BY_USER/*PAUSED_BY_ACTIVATE*/);
		if(!bMouseButtonDown) 
		{
			Pause(PAUSED_BY_USER);
			return true;
		}
		else
		{
			bPauseRequested = true;
		}		
	}

	if(!bCanPause && (SDL_GetTicks() - pauseDelay) > 4000)
	{
		//Enable puse after 4s
		pauseDelay = 0;
		bCanPause = true;
	}

	if(bPause) return true;

	
	//Execution
	double timeDelayNow = 1.0;
	

	int fps = GameControl::Get()->getRealFrameRate();
	if(fps > 0) timeDelayNow = 25.0/fps;
	else timeDelayNow = 4.0;

	if(timeDelayNow < 1.0) timeDelayNow = 1;
	//else if(timeDelayNow > 1) timeDelayNow *= 2.0;
	
	if(timeDelayNow > timeDelay) timeDelay = timeDelayNow;
	else timeDelay = (9*timeDelay + timeDelayNow)/10.0;
	
#ifdef _DEBUG
	if(timeDelay > 1.0) GLOUTPUT("timeDelay: %f\n", timeDelay);
#endif

	while(eventCount < totalEvents && (timeDelay*(events[eventCount].timeStamp - lastTimeStamp) + lastExecutionTime) <= (SDL_GetTicks() - startTime - totalPauseTime))
	{
	

#ifdef _DEBUG
		if(events[eventCount].event.key.keysym.sym == 27)
		{
			GLOUTPUT("******************** ESC ******************\n");
		}
#endif

		if(events[eventCount].event.type == SDL_TUTORIAL_PAINT_START_EVENT)
		{
			TutorialPaint::Call();
			bCanPause = true;
			//eventCount++;

			do
			{
				eventCount++;
				TutorialPaint::Get()->Execute(events[eventCount].event);
			} while(events[eventCount].event.type != SDL_TUTORIAL_PAINT_END_EVENT);

			bPaintFinish = true;
			eventCount++;

			Pause(PAUSED_BY_USER);
			GameControl::Get()->UpdateToolTip("Move Mouse to Continue");
			continue;
		}

		if(events[eventCount].event.type == SDL_MOUSEMOTION)
		{
			SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
			SDL_WarpMouse(events[eventCount].event.motion.x, events[eventCount].event.motion.y);
			SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);

			bIgnoreMouseMotionEvent = true;
			//bCanPause = true;

			if(bMouseButtonDown || ListPopSelect::getHasListSelect())
			{
				ForceRedraw();
			}
		}
		
		if(events[eventCount].event.type == SDL_MOUSEBUTTONDOWN) 
		{
			bMouseButtonDown = true;
			ForceRedraw();
		}
		else if(events[eventCount].event.type == SDL_MOUSEBUTTONUP) 
		{
			bMouseButtonDown = false;
			ForceRedraw();
		}

		if(!GameControl::Get()->GameTick(events[eventCount].event))
			return false;

		if( events[eventCount].event.type == SDL_MOUSEBUTTONDOWN ||
			events[eventCount].event.type == SDL_MOUSEBUTTONUP ||
			events[eventCount].event.type == SDL_KEYDOWN) 
		{
			ForceRedraw();
		}

		//Control tips after execution
		if( events[eventCount].event.type == SDL_KEYDOWN && 
			(events[eventCount].event.key.keysym.sym < SDLK_F1 || events[eventCount].event.key.keysym.sym > SDLK_F15) &&
			events[eventCount].event.key.keysym.sym != SDLK_LALT &&
			events[eventCount].event.key.keysym.sym != SDLK_TAB)
		{
			//Don't show the function keys
			GameControl::Get()->UpdateToolTip(SDL_GetKeyName(events[eventCount].event.key.keysym.sym));
		}
		else if(events[eventCount].event.type == SDL_MOUSEBUTTONDOWN)
		{
			gedString tip;

			if(events[eventCount].event.button.button == SDL_BUTTON_RIGHT) tip = "Right Mouse Button";
			else if(events[eventCount].event.button.button == SDL_BUTTON_LEFT) tip = "Left Mouse Button";
			else if(events[eventCount].event.button.button == SDL_BUTTON_MIDDLE) tip = "Middle Mouse Button";

			GameControl::Get()->UpdateToolTip(tip);
		}
	
		lastExecutionTime = timeDelay*(events[eventCount].timeStamp - lastTimeStamp) + lastExecutionTime;

		if(events[eventCount].timeStamp) lastTimeStamp = events[eventCount].timeStamp;

		eventCount++;
	}

	if(eventCount >= totalEvents)
	{
		bPauseRequested = false;
		if(!TutorialPaint::InPaintMode()) TutorialPlayControl::Call(true);
	}

	if(bPauseRequested && !bMouseButtonDown)
	{
		bPauseRequested = false;
		Pause(PAUSED_BY_USER);		
	}

	return true;
}

bool Tutorial::CanExecute(SDL_Event &event)
{
	//Return true for all events other then mouse and keyboard
	//or in pause mode

	if((event.type != SDL_KEYDOWN && 
	   event.type != SDL_KEYUP && 
	   event.type != SDL_MOUSEBUTTONDOWN && 
	   event.type != SDL_MOUSEBUTTONUP && 
	   event.type != SDL_MOUSEMOTION &&
	   event.type != SDL_ACTIVEEVENT) || bPause) return true;

	return false;
}

void Tutorial::Pause(int type)
{
	switch(type)
	{
	case PAUSED_BY_USER:
		bUserPause = !bUserPause;
		break;
	case PAUSED_BY_ACTIVATE:
		bActivatePause = !bActivatePause;
		break;
	case PAUSED_BY_WAIT:
		bWaitPause = !bWaitPause;
		break;
	case PAUSED_BY_EXPLANATION_EDITOR:
		bPauseByExplanation = !bPauseByExplanation;
		break;
	}


	if((bUserPause || bActivatePause || bWaitPause || bPauseByExplanation) && !bPause)
	{
		//Enter in pause mode
		initialPauseTime = SDL_GetTicks();
		
#ifdef DEBUG
		GLOUTPUT("Pause in (type = %ld)\n", type);
#endif
	}
	else if(!bUserPause && !bActivatePause && !bWaitPause && !bPauseByExplanation)
	{
		//Leave pause mode
		totalPauseTime += SDL_GetTicks() - initialPauseTime;

#ifdef DEBUG
		GLOUTPUT("Pause out (type = %ld)\n", type);
#endif
	}
		
	if(bUserPause && mode == TUTORIAL_PLAY && !TutorialPaint::InPaintMode())
	{
		TutorialPlayControl::Call(eventCount >= totalEvents);
	}
	
	bPause = bUserPause || bActivatePause || bWaitPause || bPauseByExplanation;
}

void Tutorial::Play()
{
	Uint32 n = totalEvents;
	Reset();
	totalEvents = n;

	//Remove modal panels
	Actor *modal = GameControl::Get()->GetModal();
	while(modal)
	{
		delete modal;
		modal = GameControl::Get()->GetModal();
	}

	mode = TUTORIAL_PLAY;		
}

void Tutorial::Rec()
{
#ifdef _DEBUG
	if(eventCount > 0) 
	{
		if(bPause) Pause(PAUSED_BY_USER); //Don't reset if continues rec
		totalEvents = eventCount;
		GLOUTPUT("Rec: continue tutorial\n");
	}
	else 
	{
		//New rec
		ged_getcwd(tutorialPath, PATH_LIMIT);
		Reset();
		GLOUTPUT("Rec: new tutorial\n");
	}
	
	mode = TUTORIAL_REC;	
#endif
}

bool Tutorial::Load(const gedString& fileName)
{
	GED_TRY
	{
	WaitCursor wait;
	texts.Clear();
	lists.Clear();
	nList = 0;

	SDL_ClearError();

	gedString aux(fileName);
	SDL_RWops *src = ged_SDL_RWFromFile(aux.c_str(), "rb");
	if(!src)
	{
		return false;
	}

	gedString::SetReadEncriptString(true);

	//Signature
	Uint32 magic = GameControl::Read32(src);
	
	if(memcmp(&magic, "GEDT", 4) != 0)
	{
		new PanelInfo("This file is not a valid tutorial");

		SDL_RWclose(src);
		return false;
	}

	//Load file version
	Uint32 fileVersion = GameControl::Read32(src);

	//Load Game Editor version
	gameEditorTutorialVersion = GameControl::Read32(src);

	if(gameEditorTutorialVersion > GAME_EDITOR_VERSION)
	{
		new PanelInfo("This tutorial require the newest Game Editor version\nPlease, download last version in http://game-editor.com");
		
		SDL_RWclose(src);
		return false;
	}

	//Load total events
	totalEvents = GameControl::Read32(src);
	
	//Alloc events mem
	Uint32 size = totalEvents*sizeof(stTutorialEvent);
	events = (stTutorialEvent *)malloc(size);
	if(!events)
	{
		SDL_RWclose(src);
		return false;
	}

	//Load compressed events
	Uint32 len = GameControl::Read32(src);
	char *tmp = new char[len];
	if(!tmp)
	{
		SDL_RWclose(src);

		free(events);
		events = NULL;
		return false;
	}

	memset(events, 0, size);
	memset(tmp, 0, len);


	//Load events	
	GameControl::Read(src, tmp, len, 1);

	

	Compression comp;			
	comp.Decompression(tmp, len, events, size, false);

	delete [] tmp;

	if(fileVersion > 1)
	{
		Uint32 n = GameControl::Read32(src);
		if(n > 0)
		{									
			for(int iList = 0; iList < n; iList++)
			{
				ListString sList;
				Uint32 nTexts = GameControl::Read32(src);
		
				for(int iText = 0; iText < nTexts; iText++)
				{
					gedString s;
					s.Read(src);
					sList.PushBack(s);
				}

				lists.PushBack(sList);
			}
			
			
		}
	}

	SDL_RWclose(src);

	//Load texts in ASCII format
	src = ged_SDL_RWFromFile((fileName + ".txt").c_str(), "rb");
	if(src)
	{	
		//Read all file and store in a big buf (for speed)
		SDL_RWseek( src, 0, SEEK_END );
		int length = SDL_RWtell( src );
		SDL_RWseek( src, 0, SEEK_SET );
		
		char *buf = new char[length + 1];
		
		if(buf)
		{
			memset(buf, 0, length+1);
			SDL_RWread( src, buf, length, 1 );
			
			int ini = 0;
			for(int i = 0; i < length; i++)
			{
				if(buf[i] == '\n' || buf[i] == '\r')
				{
					buf[i] = 0;

					if(buf[i+1] == '\n' || buf[i+1] == '\r')
					{
						buf[i + 1] = 0;
						i++;
					}

					//Replace | by /n
					gedString s(&buf[ini]);
					for(int i0 = 0; i0 < s.length(); i0++)
					{
						if(s[i0] == '|')
						{
							s.set(i0, '\n');
						}
					}

					texts.PushBack(s);
					ini = i+1;					
				}
			}
			
			if(buf[ini])
			{
				//Replace | by /n
				gedString s(&buf[ini]);
				for(int i0 = 0; i0 < s.length(); i0++)
				{
					if(s[i0] == '|')
					{
						s.set(i0, '\n');
					}
				}
				
				texts.PushBack(s);
			}

			delete [] buf;
		}

		SDL_RWclose( src );
	}

	} 
	GED_CATCH
	{
		
	}

	if(*SDL_GetError() != 0)
	{
		new PanelInfo(SDL_GetError());
		return false;
	}

	//Set memSlots for save after this load
	memSlots = totalEvents;

	//Store the turorial path
	ged_getcwd(tutorialPath, PATH_LIMIT);

	return true;
}

bool Tutorial::Save(const gedString& fileName)
{
	WaitCursor wait;

	SDL_ClearError();

	gedString aux(fileName);
	SDL_RWops *src = ged_SDL_RWFromFile(aux.c_str(), "wb");
	if(!src)
	{
		return false;
	}

	//Signature
	Uint32 magic;
	memcpy(&magic, "GEDT", 4);

	SDL_WriteLE32(src, magic);

	//Save file version
	SDL_WriteLE32(src, TUTORIAL_FILE_VERSION);

	//Save Game Editor version
	SDL_WriteLE32(src, GAME_EDITOR_VERSION);

	//Save total events
	SDL_WriteLE32(src, totalEvents);

	//Save events
	Uint32 size = totalEvents*sizeof(stTutorialEvent);
	char *tmp = new char[size + size / 8 + 256];
	if(!tmp)
	{
		SDL_RWclose(src);
		return false;
	}

	Compression comp;
	Uint32 len = comp.DoCompression(events, size, tmp, size + size / 8 + 256);

	SDL_WriteLE32(src, len);
	SDL_RWwrite(src, tmp, len, 1);
	delete tmp;

	//Save text lists
	SDL_WriteLE32(src, lists.Count());

	int i;
	for(i = 0; i < lists.Count(); i++)
	{
		//Save the list count
		SDL_WriteLE32(src, lists[i].Count());

		for(int i1 = 0; i1 < lists[i].Count(); i1++)
		{
			lists[i].Item(i1).Write(src);
		}
	}

	SDL_RWclose(src);

	if(*SDL_GetError() != 0)
	{
		new PanelInfo(SDL_GetError());
		return false;
	}

	//Save texts in ASCII format
	char *end = "\r\n"; //Handle this on linux too
	src = ged_SDL_RWFromFile((fileName + ".txt").c_str(), "wb");
	if(!src)
	{
		return false;
	}

	for(i = 0; i < texts.Count(); i++)
	{
		gedString s(texts[i]);

		//Replace /n by |

		for(int i0 = 0; i0 < s.length(); i0++)
		{
			if(s[i0] == '\n')
			{
				s.set(i0, '|');
			}
		}

		//Save
		SDL_RWwrite(src, s.c_str(), s.length(), 1);
		SDL_RWwrite(src, end, 2, 1);
	}

	SDL_RWclose(src);

	if(*SDL_GetError() != 0)
	{
		new PanelInfo(SDL_GetError());
		return false;
	}

	return true;
}

bool Tutorial::HasTutorial(char *dirName)
{
	//Return true if the directory has a .gedt file
	if(chdir(dirName) == 0)
	{
		long handle = ged_findfirst("*.gedt", A_ANY);
		if(handle != GED_NOTFOUND)
		{	
			ged_findclose(handle);
			chdir("..");
			return true;
		}

		chdir("..");
	}

	return false;
}

void Tutorial::PopulateList(ListPop *list)
{
	//Populate the tutorial category
	EditorDirectory editDir;
	ListString listDirs;

	chdir("Tutorials");

	//Read dirs
	long handle = ged_findfirst("*.*", A_DIREC | A_HIDDEN);
	if(handle != GED_NOTFOUND)
	{	
		do
		{
			if(strcmp(ged_filename(handle), ".") != 0 && strcmp(ged_filename(handle), ".."))
			{
				char *name = ged_filename(handle);
				if(HasTutorial(name))
				{
					listDirs.PushBack(name);				
				}				
			}
			
		} while((handle = ged_findnext(handle)) != GED_NOTFOUND);
		
		ged_findclose(handle);
	}	

	//Put in the list
	if(listDirs.Count())
	{
		int i;
		
		list->AddSeparator();
		
		for(i = 0; i < listDirs.Count(); i++)
		{
			list->AddText(listDirs[i]);
		}
		
		list->AddSeparator();
	}
}

void Tutorial::AddList(ListPop *list)
{
	//Add the list content to tutorial
	if(!InRecMode()) return;

	
	ListString sList;
	for(int i = 0; i < list->getItens().Count(); i++)
	{
		sList.PushBack(list->getItens().Item(i).text);
	}
	
	lists.PushBack(sList);
}

void Tutorial::GetList(ListPop *list)
{
	//Get the list content from tutorial
	if(!InPlayMode()) return;

	if(nList < lists.Count())
	{
		list->RemoveAll();
		
		for(int i = 0; i < lists[nList].Count(); i++)
		{
			list->AddText(lists[nList].Item(i));
		}
	}

	nList++;
}

void Tutorial::StoreText(int x, int y, const gedString &text)
{
	//Add event
	SDL_Event event;
	event.type = SDL_TUTORIAL_TEXT_EVENT;
	event.user.code = texts.Count();
	event.user.data1 = (void *)x;
	event.user.data2 = (void *)y;
	
	texts.PushBack(text); //Flush on tutorial save
	Tutorial::Get()->SaveEvent(event, 0);
}

gedString Tutorial::GetText(int index)
{
	if(index < texts.Count())
	{
		return texts[index];
	}

	return "";
}


TutorialUpdate *TutorialUpdate::tutorialUpdate = NULL;
TutorialUpdate::TutorialUpdate()
{
	tutorialUpdate = this;
	thread = NULL;
	totalDownloadSize = 0;

	BuildXMLFromCurrentTutorials();	
	
	if(!Update())
	{
		new PanelInfo("A Internet connection can't be established\nPlease, try again after connection");
		TutorialUpdateDlg::Destroy();
	}
}

TutorialUpdate::~TutorialUpdate()
{
	tutorialUpdate = NULL;

	if(thread)
	{
		SDL_WaitThread(thread, NULL);
	}
}

void TutorialUpdate::Call()
{
	if(!tutorialUpdate)
	{
		new TutorialUpdate();
	}
}

void TutorialUpdate::Destroy()
{
	delete tutorialUpdate;
}

void TutorialUpdate::ProcessFile(TiXmlElement &root, char *fileName, int fileSize)
{
	//Skip compressed files
	gedString s(fileName), s1(fileName);
	s1.lower();

	if(s1.substr(s1.length() - 5, 5) == ".gedc" || s1.find(".tmp") != gedString::npos)
	{
		return;
	}

	//Get path
	char buf[PATH_LIMIT];
	ged_getcwd(buf, PATH_LIMIT);
		
	//Use the / as directory separator
	for(int i = 0; i < strlen(buf); i++)
	{
		if(buf[i] == '\\')
		{
			buf[i] = '/';
		}
	}

	//Remove last /
	if(buf[strlen(buf)-1] == '/')
	{
		buf[strlen(buf)-1] = 0;
	}

	//Save only after Tutorials dir
	gedString path(buf), sub(GameControl::getEditorPath() + "/Tutorials");
	path = path.substr(sub.length(), path.length() - sub.length());
	
	if(path[0] != '/') path = gedString("/") + path;

	//Compress
	gedString fileCompName(gedString(fileName) + ".gedc");
	Compression comp;
	int lenghComp = comp.DoCompression(fileName, fileCompName.c_str());
	
	//Calc hash before compression (for update check)
	gedString crc((unsigned long)FileCrc32(fileName));

	//Size
	gedString sSize((long)lenghComp); //Size after compression (for download integrity check)
	
	//Create the element
	TiXmlElement file( "file" );

	file.SetAttribute( "name", fileCompName);	rawField += fileCompName;
	file.SetAttribute( "path", path);	rawField += path;
	file.SetAttribute( "size", sSize);		rawField += sSize;
	file.SetAttribute( "hash", crc);		rawField += crc;
	
	root.InsertEndChild( file );
}

void TutorialUpdate::ProcessDir(TiXmlElement &root)
{
	//Read files
	long handle = ged_findfirst("*.*", A_ANY);
	if(handle != GED_NOTFOUND)
	{	
		do
		{
			char *name = ged_filename(handle);

			if(ged_attrib(handle) & A_DIREC)
			{
				//Directory
				if(strcmp(name, ".") != 0 && strcmp(name, ".."))
				{
					chdir(name);
					ProcessDir(root);
					chdir("..");
				}
			}
			else
			{
				//File
				ProcessFile(root, name, ged_filesize(handle));
			}			
			
		} while((handle = ged_findnext(handle)) != GED_NOTFOUND);
		
		ged_findclose(handle);
	}
}

void TutorialUpdate::BuildXMLFromCurrentTutorials()
{
	//Create the XML
	//For developer use only
	//Send the files to ftp server with binary option to .txt files

#ifdef _DEBUG

	EditorDirectory editDir;
	if(chdir("Tutorials") == 0)
	{
		
		TiXmlDocument doc("tutorials.xml");	
		
		TiXmlElement root( "TutorialUpdate" );
		root.SetAttribute( "version", "1");
		
		
		rawField.clear();
		
		ProcessDir(root);
		chdir("..");
		
		gedString crc((unsigned long)Crc32((void *)rawField.c_str(), rawField.length()));
		root.SetAttribute( "hash", crc);
		
		doc.InsertEndChild( root );
		doc.SaveFile();

		exit(0);
	}

#endif
}

bool TutorialUpdate::Update()
{
	//Update the tutorials, if new
	WaitCursor wait;
	totalDownloadSize = 0;

	
	//Load the XML from site
	long size = 0;
	char *xml = download("game-editor.com/tutorials.xml", &size);
	if(!xml) return false;

	xml[size] = 0;
	updatedTutorials.clear();


	//Build the tree
	TiXmlDocument doc;
	if(!doc.LoadFromBuffer(xml, size)) return false;

	free(xml);

	
	//Check tree
	TiXmlElement* root = NULL;
	TiXmlElement* child = NULL;
	
	root = doc.FirstChildElement("TutorialUpdate");
	if(!root) return false;

	if(!root->Attribute("hash")) return false;

	gedString aux(*root->Attribute("hash"));
	U32 hashXml = atol(aux.c_str());
	rawField.clear();
	listFiles.Clear();
	
	for(child = root->FirstChildElement();	child; child = child->NextSiblingElement())
	{
		 if(child->Value() == "file")
		 {
			 const gedString *name, *path, *size, *hash;
			 
			 name = child->Attribute("name");
			 path = child->Attribute("path");
			 size = child->Attribute("size");
			 hash = child->Attribute("hash");

			 if(name && path && size && hash)
			 {
				rawField += *name;
				rawField += *path;
				rawField += *size;
				rawField += *hash;

				gedString aux1(*size);
				gedString aux2(*hash);

				U32 fileSize = (U32)atol(aux1.c_str());
				totalDownloadSize += fileSize;
				listFiles.PushBack(stTutorialFileInfo(*name, *path, fileSize, (U32)atol(aux2.c_str())));
			 }
		 }
	}

	if(Crc32((void *)rawField.c_str(), rawField.length()) != hashXml) return false;
	if(listFiles.Count() == 0) return false;

	thread = SDL_CreateThread(CheckFilesThread, NULL);
	return true;
}


int TutorialUpdate::CheckFilesThread( void *pParam )
{
	//Check the list of site files with local files

	int nTotal = tutorialUpdate->listFiles.Count(), sizeUpdated = 0, nRetry = 0;
	TutorialUpdate *p = tutorialUpdate;
	bool bRepeat;

	do
	{
		bRepeat = false;

		for(int i = 0; i < nTotal && tutorialUpdate; i++)
		{
			if(!p->listFiles[i].bUpdated)
			{
				if(p->CheckFile(p->listFiles[i]))
				{
					sizeUpdated += p->listFiles[i].size;	
					TutorialUpdateDlg::SetProgress(100.0*sizeUpdated/(double)p->totalDownloadSize, p->updatedTutorials);
				}
				else
				{
					bRepeat = true;

					if(!isInternetConnected())
					{
						//Sleep for 5s
						SDL_Delay(5*1000);
						nRetry++;

						if(nRetry > 5)
						{
							new PanelInfo("A Internet connection can't be established\nPlease, try again after connection");
							TutorialUpdateDlg::Destroy();

							p->thread = NULL;
							return 0;
						}
					}
				}
			}
		}

	} while(bRepeat && tutorialUpdate);


	p->thread = NULL;
	return 0;
}


bool TutorialUpdate::CheckFile(stTutorialFileInfo& info)
{
	EditorDirectory editDir;

	if(chdir("Tutorials") != 0)
	{
		ged_mkdir("Tutorials");
	}

	gedString path(info.path), tutorialInfo;
	char *pToken, sep[] = "/";	

	pToken = strtok((char *)path.c_str(), sep);
	
	//Create dirs
	while(pToken) 
	{	
		tutorialInfo += gedString(pToken) + " -> ";

		if(chdir(pToken) != 0)
		{
			ged_mkdir(pToken);
		}

		pToken = strtok (NULL, sep);
	}

	//Remove .gedc
	gedString realFileName(info.name);
	realFileName = realFileName.substr(0, realFileName.length() - 5);

	//Check file
	if(FileCrc32(realFileName.c_str()) != info.hash)
	{
		//File don't exists or is different, so get a new file
		gedString file("/tutorials"), name(info.name);
		
		name.lower(); //The ftp server make an lower on the file names
		file += info.path + "/" + name;
		
		//Get from ftp server
		long bytesRead = 0;
		char *compBuf = download((gedString("game-editor.com") +  file).c_str(), &bytesRead);
		if(!compBuf) return false;

		if(bytesRead != info.size)
		{
			free(compBuf);
			return false;
		}

		//Is compressed?
		int dataOffSet = 2*sizeof(Uint32); //skip header if not compressed
		char *fileBuf = compBuf;
		Uint32 realSize = ((Uint32 *)compBuf)[0], compSize = ((Uint32 *)compBuf)[1];
		
		
		if(realSize > compSize)
		{
			//Decompress
			fileBuf = (char *)malloc(realSize);
			if(!fileBuf)
			{
				free(compBuf);
				return false;
			}
			
			
			Compression comp;			
			comp.Decompression(compBuf + dataOffSet, compSize, fileBuf, realSize, false);
			dataOffSet = 0;
			free(compBuf);		
		}


		//Save
		gedString fullPath(GameControl::getEditorPath() + "/Tutorials" + info.path + "/" + realFileName);
		SDL_RWops *arq = ged_SDL_RWFromFile(fullPath.c_str(), "wb");
		if(!arq) 
		{
			free(fileBuf);
			return false;		
		}
		
		SDL_RWwrite(arq, fileBuf + dataOffSet, realSize, 1);
		SDL_RWclose(arq);		

		free(fileBuf);

		//Updated
		gedString s(realFileName);
		s.lower();
		if(s.substr(s.length() - 5, 5) == ".gedt")
		{
			tutorialInfo += realFileName.substr(0, realFileName.length() - 5);
			updatedTutorials += tutorialInfo + "\n";
		}
	}

	info.bUpdated = true;
	return true;
}

enum
{
	BT_OK,
	BT_CANCEL	
};

#define WIDTH	215
#define HEIGHT	80

TutorialUpdateDlg *TutorialUpdateDlg::tutorialUpdateDlg = NULL;
TutorialUpdateDlg::TutorialUpdateDlg()
	: Panel("TutorialUpdateDlg", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	ged_getcwd(oldpath, PATH_LIMIT);
	tutorialUpdateDlg = this;

	SetModal();

	bFinish = false;

	Text *text;
	int y;

	

	//Title
	text = AddText("Tutorial Update", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("Update progress: ", 40, y);	
	progress = AddText("  0%", text->Right(), y);
	
	//Close
	y = DrawHLine(text->Down() + 5);

	cancelButton = AddButton("Cancel", (WIDTH - 64)/2, y, 0, 0, BT_CANCEL); 

	TutorialUpdate::Call();
}

TutorialUpdateDlg::~TutorialUpdateDlg()
{
	TutorialUpdate::Destroy();
	tutorialUpdateDlg = NULL;

	chdir(oldpath);
}

void TutorialUpdateDlg::Call()
{
	if(!tutorialUpdateDlg)
	{
		new TutorialUpdateDlg();
	}
}

void TutorialUpdateDlg::Destroy()
{
	delete tutorialUpdateDlg;
}

void TutorialUpdateDlg::SetProgress(double pg, gedString &updatedTutorials)
{
	if(tutorialUpdateDlg)
	{
		char buf[64];
		sprintf(buf, "%3.0f%%", pg);
		
		tutorialUpdateDlg->progress->SetText(buf);

		if(pg >= 100.0)
		{
			//Update finished
			tutorialUpdateDlg->cancelButton->SetText("Close");
			tutorialUpdateDlg->bFinish = true;

			tutorialUpdateDlg->PostMessage(tutorialUpdateDlg, DELETE_ME, DELETE_ME);

			if(updatedTutorials.length())
			{
				new PanelInfo(gedString("Updated tutorials:\n\n") + updatedTutorials, "Update successful", ALIGN_LEFT);
				MainPanel::Get()->HelpSetup();
			}
			else
			{
				new PanelInfo("Your tutorials are up-to-date and an\nupdate is not necessary at this moment", "Info");
			}
		}
	}
}

void TutorialUpdateDlg::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_CANCEL:
		{
			if(bFinish)
			{
				delete this;
			}
			else
			{
				PanelQuestion *panel = new PanelQuestion("The update is not completed. Some tutorials won't work correctly.\nProceed anyway?");
				if(panel->Wait() == OK_BUTTON)
				{
					delete this;
				}
				
				delete panel;
			}
		}
		break;
	}
}



bool TutorialUpdateDlg::OnList(ListPop *list, int index, gedString &text, int listId)
{	
	
	
	return true;
}


#endif //STAND_ALONE_GAME



























