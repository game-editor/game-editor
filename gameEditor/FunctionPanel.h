/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

If GPL v3 does not work for you, we offer a "non-GPL" license 
<http://game-editor.com/License>, thats will help us to pay our developers 
<http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>.

***************************************************************************/


// FunctionCompletion.h: interface for the FunctionCompletion class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_FUNCIONPANEL_H__99AEBAC9_1A98_4CD9_8A42_EA0127925C5A__INCLUDED_)
#define AFX_FUNCIONPANEL_H__99AEBAC9_1A98_4CD9_8A42_EA0127925C5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../gameEngine/Panel.h"
#include "engine/../tinyxml/tinyxml.h"
#include "../gameEngine/ListPop.h"
#include "../gameEngine/EditBox.h"
#include "ColorPicker.h"

class stFunctionInputOption
{
public:
	stFunctionInputOption() {}

	stFunctionInputOption(gedString _name, gedString _value)
	{
		name = _name;
		value = _value;
	}

	void operator=( const stFunctionInputOption& copy ) 
	{ 
		name = copy.name;
		value = copy.value;
	}

	/*bool operator==( const stFunctionInputOption& copy ) 
	{ 
		return name == copy.name;			
	}*/

	gedString name, value;
};

typedef GlDynArray<stFunctionInputOption> ListOptions;

class stFuncionArgument
{
public:

	stFuncionArgument() 
	{
		variable_list = false; 
		min = max = size = 0;
		step = 1.0;
		editBox = NULL; 
		listPop = NULL; 
		color = NULL;
		controlId = 0;
		bNeedQuotationMarks = false;
		bNeedCast = false;
	}

	void operator=( const stFuncionArgument& copy ) 
	{ 
		title = copy.title;
		c_type = copy.c_type;
		tip = copy.tip;
		errorMsg = copy.errorMsg;
		inputType = copy.inputType;
		size = copy.size;
		min = copy.min;
		max = copy.max;
		step = copy.step;
		variable_list = copy.variable_list;
		populateCmd = copy.populateCmd;
		selectedOption = copy.selectedOption;
		listOptions = copy.listOptions;
		editBox = copy.editBox;
		listPop = copy.listPop;
		color = copy.color;
		controlId = copy.controlId;
	}

	//Argument properties
	gedString title, c_type, tip, errorMsg;
	int controlId;

	//Input
	gedString inputType;
	int size;

	//String type
	bool bNeedQuotationMarks;

	//Const argument
	bool bNeedCast;

	//Edit
	double min, max, step;
	EditBox *editBox;
	bool variable_list;

	ColorSample *color;

	//List
	gedString populateCmd, selectedOption;
	ListOptions listOptions;
	ListPop *listPop;
};

typedef GlDynArray<stFuncionArgument> ListArguments;


class FunctionCompletion  
{
public:
	
	virtual ~FunctionCompletion();
	bool GetCallText(gedString functionName);

	static FunctionCompletion* Get();

	ListArguments *getListArguments() {return &listArguments;}
	gedString &getReturnType() {return return_type;}
	//gedString &getReturnStorein() {return returnStorein;}
	gedString &getFunctionName() {return functionName;}

private:

	FunctionCompletion();

	void GetArguments(TiXmlElement* root);
	//void GetReturn(TiXmlElement* root);
	void GetCurrentCode();

	static FunctionCompletion *functionCompletion;


	ListArguments listArguments;
	gedString return_type, /*returnStorein,*/ functionName;

};


class FunctionPanel : public Panel  
{
public:

	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnButton(Button *button, int buttonId);
	void OnEditChar(EditBox *edit, SDLKey key, int line);
	void OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message);
	bool OnListButtonDown(ListPop *list, int listId);
	
	FunctionPanel(gedString _panelTitle, gedString _panelTip, FunctionCompletion *_functionCompletion);
	virtual ~FunctionPanel();



private:

	bool VerifyData();
	void BuidFunctionCall();

	gedString VerifyArgument(stFuncionArgument *pArgument, EditBox *edit = NULL);
	void GetCurrentCode();
	static void VisibleVariableCallBack(char *name, int bPointer);
	ListData GetVariableList(gedString c_type);
	void PopulateListByCommand(ListPop *listPop, gedString populateCmd);
	bool NeedReference(ListPop *list, gedString text = "");

	FunctionCompletion *functionCompletion;
	static FunctionPanel *functionPanel;

	bool bHasActorVariable, bVerfyErrors;

	gedString scriptCodeStart, currentSelectedActor;
	ListData variableList;

	ListPop *listReturn, *listActor, *lastListControl, *listActorVariables;
	VariableSelector *lastVariableSelector;
};


#endif // !defined(AFX_FUNCIONPANEL_H__99AEBAC9_1A98_4CD9_8A42_EA0127925C5A__INCLUDED_)


#endif