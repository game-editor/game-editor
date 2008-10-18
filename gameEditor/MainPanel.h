// MainPanel.h: interface for the MainPanel class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_MAINPANEL_H__508113F4_69EC_425D_91DA_4B6D4C530760__INCLUDED_)
#define AFX_MAINPANEL_H__508113F4_69EC_425D_91DA_4B6D4C530760__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

extern const int BT_GAME_MODE;

class TutorialReminder : public Panel  
{
public:
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message);
	TutorialReminder();
	virtual ~TutorialReminder();

	static bool getShow() {return bAlreadyShow;}

private:
	ListDirectory *listTutorialDir;
	static bool bAlreadyShow;
	Actor *logo;
};

class MainPanel : public Panel  
{
public:
	static void AddLastScript(gedString &text);
	void EditBoxContextMenu(KrTextWidget *textWidget, int x, int y);
	void HelpSetup();
	void FileSetup();
	void ScriptSetup();
	void ShowActorCoordinates(Actor *actor);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	bool OnListButtonDown(ListPop *list, int listId);
	void OnMouseMove(int x, int y);
	void OnKeyUp(SDLKey key);
	void OnKeyDown(SDLKey key, int repeatLoopCount = 0);
	void OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message);
	
	void OnButton(Button *button, int buttonId);
	
	virtual ~MainPanel();

	static MainPanel *mainPanel;
	static MainPanel *Get() {return mainPanel;}

	static MainPanel *Create();

	bool LoadGame(gedString file, bool bShowError);

private:
	MainPanel();
	void SetCoord(int x, int y);
	void UpdateCheck();

	Text *coord;
	Actor *iconCoord, *iconSnap1, *iconSnap2, *iconSnap3, *iconSnap4;
	ListPop *listHelp, *listConfig, *listFile;
	ListPop *listEditBox, *listScript;
	KrTextWidget *editBoxWidget;
	ListDirectory *listTutorialDir;
	gedString *pShowHideRegions;
};

#endif // !defined(AFX_MAINPANEL_H__508113F4_69EC_425D_91DA_4B6D4C530760__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME