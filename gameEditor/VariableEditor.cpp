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


// VariableEditor.cpp: implementation of the VariableEditor class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "VariableEditor.h"
#include "CreateVar.h"
#include "GameControl.h"
#include "ExpressionEditor.h"
#include "GenericScript.h"
#include "ScriptGlobals.h"



//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_VARIABLEEDITOR			"\
Use this panel to add, edit or remove your variables.\n\
You can edit all variables attributes, except your name"

#define TIP_VARIABLEEDITOR_VARIABLES			"\
Select a variable to edit or remove"

#define TIP_VARIABLEEDITOR_ADD			"\
Add a new variable in your game"

#define TIP_VARIABLEEDITOR_EDIT			"\
Edit an existing variable"

#define TIP_VARIABLEEDITOR_REMOVE			"\
Remove an existing variable"
//////////////////////////////////////////////////////////////////////////////



enum
{
	BT_ADD,
	BT_REMOVE,
	BT_EDIT,
	BT_CLOSE
};

#define WIDTH	400
#define HEIGHT	107


VariableEditor::VariableEditor(ExpressionEditor *_editor, ScriptGlobals *_globals)
	: Panel("VariableEditor", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_VARIABLEEDITOR);

	editor = _editor;
	globals = _globals;
	

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText("User Variables", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("Variables: ", 10, y);
	listVars = AddListPop(text->Right() + 2, text->Top(), 300); listVars->SetToolTip(TIP_VARIABLEEDITOR_VARIABLES);
	button = AddButton("Add", listVars->Left(), listVars->Down() + 2, 0, 0, BT_ADD); button->SetToolTip(TIP_VARIABLEEDITOR_ADD);
	button = AddButton("Edit", button->Right(), button->Top(), 0, 0, BT_EDIT); button->SetToolTip(TIP_VARIABLEEDITOR_EDIT);
	button = AddButton("Remove", button->Right(), button->Top(), 0, 0, BT_REMOVE); button->SetToolTip(TIP_VARIABLEEDITOR_REMOVE);
	
	//Close
	y = DrawHLine(button->Down() + 4);
	button = AddButton("Close", (WIDTH - 60)/2, y, 0, 0, BT_CLOSE);

	UpdateVars();
}

VariableEditor::~VariableEditor()
{
	if(editor) editor->UpdateVars();
	else if(globals) globals->UpdateVars();	
}

void VariableEditor::UpdateVars(const gedString item)
{
	listVars->RemoveAll();

	Script::PopulateLocalUserVars(listVars);
	GameControl::Get()->PopulateGlobalUserVars(listVars);
	
	listVars->Sort();

	if(item.length())
	{
		for(int index = 0; index < listVars->Count(); index++)
		{
			gedString variable(listVars->GetText(index));
			int i = variable.find('[');
			if(i == gedString::npos) i = variable.find(' ');
			
			variable = variable.substr(0, i);
			
			if(variable == item)
			{
				listVars->SetItem(index);
				break;
			}
		}
	}
}

bool VariableEditor::OnList(ListPop *list, int index, gedString &text, int listId)
{

	return true;
}


void VariableEditor::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_CLOSE:
		delete this;
		break;

	case BT_ADD:
		new CreateVar(this);
		break;

	case BT_EDIT:
		{
			if(listVars->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select a variable");
				return;
			}

			gedString variable(listVars->GetText());
			int i = variable.find('[');
			if(i == gedString::npos) i = variable.find(' ');

			variable = variable.substr(0, i);

			new CreateVar(this, variable, (int)listVars->GetItemData(listVars->GetSelectedIndex()));
		}
		break;

	case BT_REMOVE:	
		{
			if(listVars->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select a variable");
				return;
			}

			PanelQuestion *panel = new PanelQuestion("Remove variable?");
			if(panel->Wait() == OK_BUTTON)
			{
				gedString variable(listVars->GetText());
				int i = variable.find('[');
				if(i == gedString::npos) i = variable.find(' ');

				variable = variable.substr(0, i);

				int next = listVars->GetSelectedIndex();

				if(!GenericScript::RemoveAndVerifyVariable(variable, (int)listVars->GetItemData(listVars->GetSelectedIndex())))
				{
					delete panel;

					panel = new PanelQuestion(GenericScript::GetError() + "\nProceed anyway?", "Error");
					if(panel->Wait() != OK_BUTTON)
					{
						delete panel;						
						return;
					}
					
					delete panel;
					
					//Force remotion
					GenericScript::RemoveAndVerifyVariable(variable, (int)listVars->GetItemData(listVars->GetSelectedIndex()), NULL, -1, true);
					panel = NULL;
					
				}

				if(next >= listVars->Count() - 1)
				{
					next = 0;
				}

				UpdateVars();
				listVars->SetItem(next);
			}

			if(panel) delete panel;
		}
		break;
	}
}


#endif //#ifndef STAND_ALONE_GAME