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


// Tutorial.h: interface for the Tutorial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(STAND_ALONE_GAME)

#if !defined(AFX_TUTORIAL_H__ED79A932_C42D_4A12_98DA_B3A59AA2A6C7__INCLUDED_)
#define AFX_TUTORIAL_H__ED79A932_C42D_4A12_98DA_B3A59AA2A6C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SDL.h"
#include "gedString.h"
#include "ListPop.h"
#include "SDL_thread.h"
#include "Panel.h"


#define TUTORIAL_FILE_VERSION 2

enum
{
	TUTORIAL_NONE,
	TUTORIAL_REC,
	TUTORIAL_PLAY,
	TUTORIAL_PAUSE
};

enum
{
	PAUSED_BY_USER,
	PAUSED_BY_ACTIVATE,
	PAUSED_BY_WAIT,
	PAUSED_BY_EXPLANATION_EDITOR
};


class stTutorialEvent
{
public:
	SDL_Event event;
	Uint32 timeStamp;
};

typedef GlDynArray<ListString> ListListsString; 

class Tutorial  
{
public:
	void GetList(ListPop *list);
	void AddList(ListPop *list);
	void ResetPauseDelay();
	void RemovePauseDelay();

	void ForceRedraw();

	gedString GetText(int index);
	void StoreText(int x, int y, const gedString& text);
	static void PopulateList(ListPop *list);
	void SaveActualEvent();

	bool Save(const gedString& fileName);
	bool Load(const gedString& fileName);
	void Store(SDL_Event &event);
	bool ExecuteMoves(SDL_Event &event);
	bool CanExecute(SDL_Event &event);
	
	static bool IsCompatible(Uint32 version);
	

	static Tutorial *Get();
	static bool InPlayMode();
	static bool InRecMode();
	static bool IsOff(SDL_Event &event);
	static bool IsOff() {return tutorial == NULL;}
	static void Destroy();
	virtual ~Tutorial();

	void Pause(int type);
	void Play();
	void Rec();
	void SaveEvent(SDL_Event &event, Uint32 timeStamp = SDL_GetTicks());

	char *getTutorialPath() {return tutorialPath;}

protected:
	
	Tutorial();
	static Tutorial *tutorial;
	static char mode;

	double timeDelay;
	Uint32 startTime, initialPauseTime, totalPauseTime, pauseDelay, lastExecutionTime, lastTimeStamp;
	Uint32 eventCount, totalEvents, memSlots;
	bool bPause, bUserPause, bActivatePause, bWaitPause, bPauseByExplanation, bPauseRequested;
	bool bMouseButtonDown, bIgnoreMouseMotionEvent;
	bool bPaintFinish;
	bool bCanPause;


	ListString			texts;
	ListListsString		lists;
	int nList;

	stTutorialEvent *events;
	SDL_Event actualEvent;

	void Reset();

	int xAnt, yAnt;
	
	
	
private:
	static bool HasTutorial(char *dirName);
	char tutorialPath[PATH_LIMIT];

	Uint32 gameEditorTutorialVersion;
};


class stTutorialFileInfo
{
public:
	stTutorialFileInfo() {size = 0; hash = 0; bUpdated = false;}
	stTutorialFileInfo(const gedString& _name, const gedString& _path, U32 _size, U32 _hash)
	{
		name = _name;
		path = _path;
		size = _size;
		hash = _hash;

		bUpdated = false;
	}

	gedString name, path;
	U32 size, hash;
	bool bUpdated;
};

typedef GlDynArray<stTutorialFileInfo> ListTutorialFiles; 



class TutorialUpdateDlg : public Panel  
{
public:
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnButton(Button *button, int buttonId);

	virtual ~TutorialUpdateDlg();
	static void Call();
	static void Destroy();

	static void SetProgress(double pg, gedString &updatedTutorials);

private:
	TutorialUpdateDlg();

	static TutorialUpdateDlg *tutorialUpdateDlg;
	Text *progress;
	Button *cancelButton;

	bool bFinish;
	char oldpath[PATH_LIMIT];
};

class TutorialUpdate
{
public:
	static void Call();
	static void Destroy();
	virtual ~TutorialUpdate();

private:
	TutorialUpdate();

	void BuildXMLFromCurrentTutorials();
	bool Update();
	

	void ProcessDir(TiXmlElement &root);
	void ProcessFile(TiXmlElement &root, char *fileName, int fileSize);

	bool CheckFile(stTutorialFileInfo& info);
	static int CheckFilesThread( void *pParam );
	static TutorialUpdate *tutorialUpdate;
	SDL_Thread *thread;

	gedString rawField; //Used in hash calc
	gedString updatedTutorials;
	ListTutorialFiles listFiles;
	U32 totalDownloadSize;
};


#endif // !defined(AFX_TUTORIAL_H__ED79A932_C42D_4A12_98DA_B3A59AA2A6C7__INCLUDED_)

#endif //STAND_ALONE_GAME
