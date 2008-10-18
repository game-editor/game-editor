// LoadSaveGame.h: interface for the LoadSaveGame class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_LOADSAVEGAME_H__ADA53450_D475_4BF3_BDF8_4160C3005A37__INCLUDED_)
#define AFX_LOADSAVEGAME_H__ADA53450_D475_4BF3_BDF8_4160C3005A37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LoadSave.h"

enum
{
	LOAD_GAME,
	SAVE_GAME,
	MERGE_GAME
};

class LoadSaveGame : public LoadSave  
{
public:
	static void LoadCache(gedString file);
	void SaveCache(gedString file);

	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnButton(Button *button, int buttonId);

	LoadSaveGame(int command);
	virtual ~LoadSaveGame();

	static bool Load(gedString filePath, bool bLoad);
	static bool Save(gedString file);
	static LoadSaveGame *getLastPanel() {return loadSaveGame;}

private:
	int command;

	ListPop *listSaveCache;
	static LoadSaveGame *loadSaveGame;
};

#endif // !defined(AFX_LOADSAVEGAME_H__ADA53450_D475_4BF3_BDF8_4160C3005A37__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME