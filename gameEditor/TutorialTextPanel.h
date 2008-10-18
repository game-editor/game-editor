// TutorialTextPanel.h: interface for the TutorialTextPanel class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_TUTORIALTEXTPANEL_H__EF1DADB6_04D4_4911_8536_19FC4B87DB68__INCLUDED_)
#define AFX_TUTORIALTEXTPANEL_H__EF1DADB6_04D4_4911_8536_19FC4B87DB68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Actor.h"
#include "GameControl.h"



class TutorialTextPanel : public Actor  
{
public:
	void Save();
	static void DestroyAll();
	static int Count();
	static void Hide();
	static void Show();
	TutorialTextPanel(const gedString& _explanationText);
	virtual ~TutorialTextPanel();

	bool OnMouseButtonDown(int x, int y, Uint8 button);
	const stCursor *OnGetCursor();
	void OnButton(Button *button, int buttonId);
	

private:
	gedString explanationText;	

	static MapPointer mapTextPanel;
	static long depth;
};

#endif // !defined(AFX_TUTORIALTEXTPANEL_H__EF1DADB6_04D4_4911_8536_19FC4B87DB68__INCLUDED_)

#endif //STAND_ALONE_GAME
