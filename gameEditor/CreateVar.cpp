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


// CreateVar.cpp: implementation of the CreateVar class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "CreateVar.h"
#include "GameControl.h"
#include "VariableEditor.h"
#include "GenericScript.h"


//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_CREATEVAR			"\
variable\n\
\n\
         Name: Variable names must begin with characters and must be followed by characters,\n\
               numbers or '_'. Script functions and var names are not allowed\n\
\n\
   Array size: If you select Yes on array list, specify how many elements the array will store.\n\
               The array index will be between zero and size\n\
\n\
   Save Group: If you want save or load this var with saveVars and loadVars functions,\n\
               set the group were this variable will be saved. Save goups are like folders\n\
               where you can save or load a group of variables"

#define TIP_CREATEVAR_TYPE			"\
Select the variable type:\n\
\n\
   Integer: non-decimal numbers\n\
      Real: decimal numbers\n\
    String: text"

#define TIP_CREATEVAR_SCOPE			"\
Global or Actor variable\n\
\n\
Use the Global Vars to control general aspects of your game like gravity or highestScore, for example\n\
\n\
Actor variables extends the Actor struct.\n\
Thus, a actor variable can be accessed in the same way that the internal Actor variables.\n\
Use local actors variables to make things like player power or health, for example"

#define TIP_CREATEVAR_ARRAY			"\
If you want store many elements in the variable, choose yes\n\
and type the how many elements you want in the size field"

#define TIP_CREATEVAR_SELECT_GROUP			"Select an existing save group"

//////////////////////////////////////////////////////////////////////////////


enum
{
	BT_ADD,
	BT_CLOSE,
	LS_ARRAY,
	LS_SCOPE,
	LS_SELECT
};

#define WIDTH	260
#define HEIGHT	265


CreateVar::CreateVar(VariableEditor *editor, gedString _editVariable, int _editScope)
	: Panel("CreateVar", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	this->editor = editor;
	editVariable = _editVariable;
	editScope = _editScope;
	bool bEditMode = editVariable != "";

	SetModal();
	SetToolTip(bEditMode?(gedString("Edit a ") + TIP_CREATEVAR):(gedString("Add a new ") + TIP_CREATEVAR));

	

	

	Text *text;
	Button *button;
	ListPop *list;
	int y;

	text = AddText(bEditMode?"Edit Variable":"Add New Variable", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	text = AddText("Name: ", 10, text->Down() + 8);
	name = AddEditBox(text->Right(), text->Top(), 128);
	listType = AddListPop(text->Right(), name->Down()+2); listType->SetToolTip(TIP_CREATEVAR_TYPE);
	listLocalGlobal = AddListPop(listType->Left(), listType->Down()+2, 128, 0, LS_SCOPE); listLocalGlobal->SetToolTip(TIP_CREATEVAR_SCOPE);

	y = DrawHLine(listLocalGlobal->Down() + 4);
	text = AddText("Array: ", 10, y);
	listArray = AddListPop(text->Right(), text->Top() - 2, 40, 0, LS_ARRAY); listArray->SetToolTip(TIP_CREATEVAR_ARRAY);
	text = AddText(" Size: ", listArray->Right() + 6, text->Top());
	arrayElements = AddEditBox(text->Right(), text->Top(), 32); arrayElements->SetNumeric(2, 32000, 1);

	y = DrawHLine(arrayElements->Down() + 4);
	text = AddText("Save group: ", 10, y);
	saveGroup = AddEditBox(text->Right(), text->Top(), 92);
	list = AddListPop(saveGroup->Right() + 2, saveGroup->Top(), 60, 0, LS_SELECT, "select"); list->SetToolTip(TIP_CREATEVAR_SELECT_GROUP);
	text = AddText("Use this field to put a variable\nin the specified group.\nThe variable will be saved or\nloaded with your group.\nLeave this field blank\nif you don't want save the variable", 40, saveGroup->Down() + 6);
	
	//Close
	y = DrawHLine(text->Down() + 4);
	button = AddButton(bEditMode?"Ok":"Add", (WIDTH - 135)/2, y, 0, 0, BT_ADD); SetConfirmButton(button);
	button = AddButton("Close", button->Right()+8, y, 0, 0, BT_CLOSE); button->SetToolTip(TIP_ACTION_CANCEL); SetCancelButton(button);

	listType->AddText("Integer");
	listType->AddText("Real");
	listType->AddText("String");
	listType->SetItem("Integer");

	listLocalGlobal->AddText("Actor variable");
	listLocalGlobal->AddText("Global");
	listLocalGlobal->SetItem("Global");

	listArray->AddText("Yes");
	listArray->AddText("No");
	listArray->SetItem("No");

	GameControl::Get()->PopulateGlobalUserVarsGroups(list);
	list->Sort();

	if(bEditMode)
	{
		int type;
		
		if(editScope == LOCAL_VAR) 
		{
			char *pType = Script::getLocalVars().FindString(editVariable.c_str());
			if(pType) 
			{
				type = *pType;
			}

			listLocalGlobal->SetItem("Actor variable");
		}
		else
		{
			stVarInfo *p;
			p = GameControl::Get()->getVars().FindString(editVariable.c_str());
			if(p)
			{
				type = p->type;
				if(p->arrayLenght)
				{
					arrayElements->SetText(p->arrayLenght);
					listArray->SetItem("Yes");
				}

				saveGroup->SetText(p->saveGroup);
			}
		}

		switch(type)
		{
		case INTEGER_VAR:
			listType->SetItem("Integer");
			break;

		case REAL_VAR:
			listType->SetItem("Real");
			break;

		case STRING_VAR:
			listType->SetItem("String");
			break;

		}

		name->SetText(editVariable);
		name->Disable();
	}
}

CreateVar::~CreateVar()
{

}

bool CreateVar::OnList(ListPop *list, int index, gedString &text, int listId)
{
	switch(listId)
	{
	case LS_ARRAY:
		if(text == "No")
		{
			arrayElements->SetText("");
		}
		break;

	case LS_SCOPE:
		if(text != "Global")
		{
			listArray->SetItem("No");
			arrayElements->SetText("");
			saveGroup->SetText("");
		}
		break;

	case LS_SELECT:
		saveGroup->SetText(text);
		break;
	}

	return true;
}

void CreateVar::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_ADD:
		{
			bool bEditMode = editVariable != "";

			int arrayLenght = 0;
			gedString save(saveGroup->GetText());

			if(listArray->GetText() == "Yes")
			{
				arrayLenght = atol(arrayElements->GetText().c_str());
				if(arrayLenght < 2)
				{
					new PanelInfo("Min array size must be 2");
					return;
				}
			}

			if(!bEditMode)
			{
				if(name->GetText().size() <= 0 || name->GetText().size() > NAME_LIMIT)
				{
					new PanelInfo("Please, enter valid variable name");
					return;
				}

				if(GameControl::Get()->GetActor(name->GetText()))
				{
					new PanelInfo("There is an Actor with this name\nPlease, enter another variable name");
					return;
				}
				
				if(GameControl::Get()->existsVar(name->GetText()))
				{
					new PanelInfo("Variable already exists\nPlease, enter another variable name");
					return;
				}
				
				int res;
				if((res = isValidVarName(name->GetText().c_str())) != VAR_NAME_OK)
				{
					switch(res)
					{
					case VAR_NAME_INVALID_INTIAL_CHAR:
						new PanelInfo("Variable names must begin with a letter");	
						break;

					case VAR_NAME_INVALID_CHAR:
						new PanelInfo("Variable names must be made of letters, numbers, and underscores");	
						break;

					case VAR_NAME_IS_RESERVED_WORD:
						new PanelInfo("Reserved words can't be variable names");	
						break;

					case VAR_NAME_IS_FUNCTION_OR_DECLARED_VAR:
						new PanelInfo("Script functions and already declared variable names are not allowed");	
						break;

					case VAR_NAME_IS_MACRO:
						new PanelInfo("Already declared macro names are not allowed");	
						break;
					}
					
					return;
				}
			}
			
			int type = INTEGER_VAR, newScope;
			stVarInfo newVar;

			if(listType->GetText() == "Real") type = REAL_VAR;
			else if(listType->GetText() == "String") type = STRING_VAR;

			newVar.name = name->GetText();
			newVar.type = type;

			if(listLocalGlobal->GetText() == "Global")
			{
				newScope = GLOBAL_VAR;

				if(!bEditMode)
				{
					if(!GameControl::Get()->CreateUserVar(name->GetText(), type, arrayLenght, save))
					{
						new PanelInfo("Variable's name is a reserved word\nEnter another variable name");
						return;
					}
				}
				else
				{	
					if(editScope == LOCAL_VAR && GameControl::Get()->getVars().FindString(name->GetText().c_str()))
					{
						new PanelInfo("A global variable already exists with this name.\nYou can't change this variable to global");
						return;
					}
					
					newVar.arrayLenght = arrayLenght;
					newVar.saveGroup = save;
				}
			}
			else 
			{
				newScope = LOCAL_VAR;
				if(arrayLenght)
				{
					new PanelInfo("You can create array only for global variables");
					return;
				}

				if(save.length())
				{
					new PanelInfo("You can use save group only for global variables");
					return;
				}

				if(!bEditMode)
				{
					if(!Script::CreateLocalUserVar(name->GetText(), type))
					{
						new PanelInfo("Variable's name is a reserved word\nEnter another variable name");
						return;
					}
				}
				else
				{
					if(editScope == GLOBAL_VAR && Script::getLocalVars().FindString(name->GetText().c_str()))
					{
						new PanelInfo("A actor variable already exists with this name.\nYou can't change this variable to be an actor variable");
						return;
					}
				}
			}

			if(bEditMode)
			{
				if(!GenericScript::RemoveAndVerifyVariable(editVariable, editScope, &newVar, newScope))
				{
					PanelQuestion *panel = new PanelQuestion(GenericScript::GetError() + "\nProceed anyway?", "Error");
					if(panel->Wait() != OK_BUTTON)
					{
						delete panel;						
						return;
					}
					
					delete panel;
					
					//Force modification
					GenericScript::RemoveAndVerifyVariable(editVariable, editScope, &newVar, newScope, true);					
				}
			}

			
			editor->UpdateVars(name->GetText());
			delete this;
			
		}	
		break;
	case BT_CLOSE:
		{
			delete this;
		}
		break;
	}
}

#endif //#ifndef STAND_ALONE_GAME