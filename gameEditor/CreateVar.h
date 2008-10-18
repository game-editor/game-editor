// CreateVar.h: interface for the CreateVar class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_CREATEVAR_H__8D9AC552_1F59_4B04_87EC_1D7248CF6D38__INCLUDED_)
#define AFX_CREATEVAR_H__8D9AC552_1F59_4B04_87EC_1D7248CF6D38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class VariableEditor;
class CreateVar : public Panel  
{
public:
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnButton(Button *button, int buttonId);
	CreateVar(VariableEditor *editor, gedString _editVariable = "", int _editScope = -1);
	virtual ~CreateVar();	

private:
	ListPop *listType, *listLocalGlobal, *listArray;
	EditBox *name, *arrayElements, *saveGroup;
	VariableEditor *editor;

	//For edition
	gedString editVariable;
	int editScope;
};

#endif // !defined(AFX_CREATEVAR_H__8D9AC552_1F59_4B04_87EC_1D7248CF6D38__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME