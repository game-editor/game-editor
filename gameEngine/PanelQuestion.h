// PanelQuestion.h: interface for the PanelQuestion class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_PANELQUESTION_H__8AA781F5_C1BE_4AC2_BCA4_C7DC3F5672ED__INCLUDED_)
#define AFX_PANELQUESTION_H__8AA781F5_C1BE_4AC2_BCA4_C7DC3F5672ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PanelInfo.h"

#define NO_BUTTON "NO_BUTTON"

class PanelQuestion : public PanelInfo  
{
public:
	
	void OnButton(Button *button, int buttonId);
	PanelQuestion(gedString text, gedString title = "Confirm", gedString confirmButton = "Yes", gedString cancelButton = "No", int align = ALIGN_CENTER);
	virtual ~PanelQuestion();
};

#endif // !defined(AFX_PANELQUESTION_H__8AA781F5_C1BE_4AC2_BCA4_C7DC3F5672ED__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME