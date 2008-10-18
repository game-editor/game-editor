// AddPathDialog.h: interface for the AddPathDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_ADDPATHDIALOG_H__325305E9_508C_472F_98AF_07C62A841C26__INCLUDED_)
#define AFX_ADDPATHDIALOG_H__325305E9_508C_472F_98AF_07C62A841C26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class PathDialog;

class AddPathDialog : public Panel  
{
public:
	void OnButton(Button *button, int buttonId);
	static void Call();
	AddPathDialog();
	virtual ~AddPathDialog();
	

private:
	static AddPathDialog *addPathDialog;
	EditBox *name, *frames;
	bool bClose;
	PathDialog *pathDialog;
};

#endif // !defined(AFX_ADDPATHDIALOG_H__325305E9_508C_472F_98AF_07C62A841C26__INCLUDED_)


#endif //STAND_ALONE_GAME
