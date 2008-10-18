// PathDialog.h: interface for the PathDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_PATHDIALOG_H__66571D81_A6EE_4390_9175_9416A5FCEB98__INCLUDED_)
#define AFX_PATHDIALOG_H__66571D81_A6EE_4390_9175_9416A5FCEB98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class PathPoint;

enum
{
	BT_CLOSE,
	BT_ADDPATH,
	BT_REMPATH
};

class PathDialog : public Panel  
{
public:
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void SetPaths();
	static void Call();
	void OnButton(Button *button, int buttonId);
	PathDialog();
	virtual ~PathDialog();

	static PathDialog *getPathDialog() {return pathDialog;}
	static void setActualPath(Path *path) {actualPath = path;}
	static Path *getActualPath() {return actualPath;}
	PathPoint *getPathPoint();

private:
	static PathDialog *pathDialog;
	ListPop *listPath;
	PathPoint *pathDraw;
	static Path *actualPath;
};

#endif // !defined(AFX_PATHDIALOG_H__66571D81_A6EE_4390_9175_9416A5FCEB98__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME