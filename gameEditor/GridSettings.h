// GridSettings.h: interface for the GridSettings class.
//
//////////////////////////////////////////////////////////////////////


#ifndef STAND_ALONE_GAME

#if !defined(AFX_GRIDSETTINGS_H__A4819BBB_AFDE_4F8A_A95E_ABFE856FAD3F__INCLUDED_)
#define AFX_GRIDSETTINGS_H__A4819BBB_AFDE_4F8A_A95E_ABFE856FAD3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"
#include "ColorPicker.h"

class Preferences : public Panel  
{
public:
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message);
	Preferences();
	virtual ~Preferences();

private:
	EditBox *editX, *editY, *editMaxUndoLevels;
	ListPop *listShow, *listSnap, *listToolTip, *listPersistentUndo;

	ColorSample *mainColor, *resolutionColor;
	ListDirectory *imageEditorPath;
};


#endif // !defined(AFX_GRIDSETTINGS_H__A4819BBB_AFDE_4F8A_A95E_ABFE856FAD3F__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME