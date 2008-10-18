// TutorialControlPanel.h: interface for the TutorialControlPanel class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_TUTORIALCONTROLPANEL_H__433593F1_856B_459A_93BA_F1C230E1931D__INCLUDED_)
#define AFX_TUTORIALCONTROLPANEL_H__433593F1_856B_459A_93BA_F1C230E1931D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Panel.h"

class TutorialPlayControl : public Panel  
{
public:
	void OnButton(Button *button, int buttonId);
	TutorialPlayControl(bool bTutorialEnd);
	virtual ~TutorialPlayControl();

	static void Call(bool bTutorialEnd);

private:
	static TutorialPlayControl *tutorialPlayControl;
	int keyFocus;

};

#endif // !defined(AFX_TUTORIALCONTROLPANEL_H__433593F1_856B_459A_93BA_F1C230E1931D__INCLUDED_)

#endif //STAND_ALONE_GAME
