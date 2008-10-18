// TutorialTextEditor.h: interface for the TutorialTextEditor class.
//
//////////////////////////////////////////////////////////////////////
#ifndef STAND_ALONE_GAME

#if !defined(AFX_TUTORIALTEXTEDITOR_H__68431ADE_0647_4A96_BFDF_11982266095C__INCLUDED_)
#define AFX_TUTORIALTEXTEDITOR_H__68431ADE_0647_4A96_BFDF_11982266095C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class TutorialTextEditor : public Panel  
{
public:
	static void Call();
	
	virtual ~TutorialTextEditor();

	void OnButton(Button *button, int buttonId);

private:
	TutorialTextEditor();
	EditBox *edit;
	static TutorialTextEditor *tutorialTextEditor;
};

#endif // !defined(AFX_TUTORIALTEXTEDITOR_H__68431ADE_0647_4A96_BFDF_11982266095C__INCLUDED_)

#endif //STAND_ALONE_GAME
