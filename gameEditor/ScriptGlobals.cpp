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


// ScriptGlobals.cpp: implementation of the ScriptGlobals class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "ScriptGlobals.h"
#include "GameControl.h"
#include "LoadSaveScript.h"
#include "GenericScript.h"
#include "Tutorial.h"
#include "VariableEditor.h"
#include "ActorProperty.h"
#include "FunctionPanel.h"


//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_GLOBALCODE			"\
Use Global Code Editor to freely add any C code,\n\
like arrays, structs and functions,\n\
which can be accessed by any script\n\
\n\
After enter your code, enter a name for it and click in Add button.\n\
Edit a code by selecting it in Choose list, edit it and click in Add button\n\
\n\
Change any internal Actor variables has no effect in Global Code"

#define TIP_GLOBALCODE_CHOOSE			"\
Select an existing global code to view or edit\n\
Click in Add button after edition"

#define TIP_GLOBALCODE_ADD			"\
Add a new global code or edit an existing global code"

#define TIP_GLOBALCODE_REMOVE			"\
Remove an existing global code\n\
You can remove only the codes which don't affect other codes or scripts"

#define TIP_GLOBALCODE_FILE			"\
Load a code from a file or save the code to a file"

#define TIP_GLOBALCODE_CLOSE			"Close the Global Code editor"

//////////////////////////////////////////////////////////////////////////////


enum
{
	LS_CHOOSE,
	BT_REMOVE,
	BT_CLOSE,
	BT_ADD,
	LS_FILE,
	LS_ACTOR,
	LS_COMPLETE,
	BT_CREATEVAR
};

#define WIDTH	640
#define HEIGHT	520

ScriptGlobals *ScriptGlobals::scriptGlobals = NULL;

ScriptGlobals::ScriptGlobals()
	: Panel("ScriptGlobals", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	scriptGlobals = this;

	SetModal();
	SetToolTip(TIP_GLOBALCODE);

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText("Global Code Editor", CENTER_TEXT, 5);
	textLine = AddText("Ln 1    ", WIDTH-45, 5, ALIGN_LEFT);
	y = DrawHLine(text->Down() + 2);

	//Body
	editGlobal = AddEditBox(10, y, WIDTH-34, HEIGHT-110, true);
	
	listChooseGlobal = AddListPop(10, editGlobal->Down()+4, 80, 0, LS_CHOOSE, "Choose"); listChooseGlobal->SetToolTip(TIP_GLOBALCODE_CHOOSE);

	text = AddText(" Name: ", listChooseGlobal->Right() + 2, listChooseGlobal->Top());
	name = AddEditBox(text->Right() + 2, text->Top(), 234);

	button = AddButton("Add", name->Right() + 5, name->Top(), 0, 0, BT_ADD); button->SetToolTip(TIP_GLOBALCODE_ADD);
	button = AddButton("Remove", button->Right() + 2, button->Top(), 0, 0, BT_REMOVE); button->SetToolTip(TIP_GLOBALCODE_REMOVE);
	listFile = AddListPop(button->Right() + 2, button->Top(), 64, 0, LS_FILE, "File"); listFile->SetToolTip(TIP_GLOBALCODE_FILE);

	listActor = AddListPop(10, listChooseGlobal->Down()+4, 120, 0, LS_ACTOR, "actors"); listActor->SetToolTip(TIP_SCRIPTEDITOR_ACTORS);
	listAutoComplete = AddListPop(listActor->Right(), listActor->Top(), 215, 0, LS_COMPLETE, "variables/functions"); listAutoComplete->SetToolTip(TIP_SCRIPTEDITOR_VARIABLES_FUNCTIONS);
	button = AddButton("Variables", listAutoComplete->Right(), listAutoComplete->Top(), 0, 0, BT_CREATEVAR); button->SetToolTip(TIP_SCRIPTEDITOR_VARIABLES);

	//Close
	y = DrawHLine(listAutoComplete->Down() + 2);
	button = AddButton("Close", (WIDTH - 62) / 2, y, 0, 0, BT_CLOSE); SetCancelButton(button); button->SetToolTip(TIP_GLOBALCODE_CLOSE);
	

	listFile->AddText("Save");
	listFile->AddText("Load");

	editGlobal->SetSyntaxColorize(true);
	editGlobal->ShowScrollBar();

	bDelimit = false;

	GenericScript::Populate(listChooseGlobal);	
	
	//Populate actors
	GameControl::Get()->PopulateActors(listActor);

	UpdateVars();
}


ScriptGlobals::~ScriptGlobals()
{
	scriptGlobals = NULL;

	if(ExpressionEditor::getExpressionEditor())
	{
		//Solve issue 124
		ExpressionEditor::getExpressionEditor()->UpdateVars();
	}
}

void ScriptGlobals::OnEditChar(EditBox *edit, SDLKey key, int line)
{
	bDelimit = false;
}

void ScriptGlobals::UpdateVars()
{
	listAutoComplete->RemoveAll();

	//Populate list vars
	Script::PopulateActorVars(listAutoComplete);
	functionsIndex = listAutoComplete->AddText("--- User Vars ---");
	Script::PopulateVars(listAutoComplete);

	//Add functions
	listAutoComplete->AddText("--- Functions ---");
	Script::PopulateFunctions(listAutoComplete);
}

void ScriptGlobals::SetText(char *str)
{
	editGlobal->StartAction();

	editGlobal->SetText("");
	editGlobal->SetText(str, ALL_LINES);

	originalScript = editGlobal->GetText(ALL_LINES);

	editGlobal->EndAction();
}


gedString ScriptGlobals::GetText()
{
	return editGlobal->GetText(ALL_LINES);
}

gedString ScriptGlobals::GetTextUntilCursor()
{
	return editGlobal->GetTextUntilCursor();
}

void ScriptGlobals::OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message)
{
	if(messageId == LINE_CHANGED)
	{
		char buf[128];
		sprintf(buf, "Ln %ld", message+1);
		textLine->SetText(buf);
	}
}

bool ScriptGlobals::OnList(ListPop *list, int index, gedString &text, int listId)
{
	switch(listId)
	{
	case LS_CHOOSE:
		{
			if(originalScript != editGlobal->GetText(ALL_LINES))
			{
				PanelQuestion *panel = new PanelQuestion("Your code has been altered\nDo you want select other code anyway?");
				if(panel->Wait() == OK_BUTTON)
				{
					SetText((char *)GenericScript::Get(list->GetText()).c_str());
					name->SetText(list->GetText());
				}
				
				delete panel;			
			}
			else
			{
				SetText((char *)GenericScript::Get(list->GetText()).c_str());
				name->SetText(list->GetText());
			}			
		}
		break;
	case LS_FILE:
		{
			if(text == "Save")
			{
				new LoadSaveScript(SAVE_DIALOG, NULL, this);
			}
			else
			{
				new LoadSaveScript(LOAD_DIALOG, NULL, this);
			}
		}
		break;

	case LS_ACTOR:
		{
			editGlobal->InsertText(text);
			bDelimit = true;			
		}
		break;

	case LS_COMPLETE:
		{
			if(text[0] != '-')
			{
				gedString delimit;
				if(bDelimit && index < functionsIndex) delimit = VAR_DELIMIT;

				gedString actionName = Action::MapScriptFunctionToAction(text);
				if(actionName.length() > 0)
				{
					Action::DoAction(ActorProperty::GetCurrentEventActor(), actionName, false);
				}
				else if(!Tutorial::IsCompatible(VERSION_PREFERENCES) || !FunctionCompletion::Get()->GetCallText(text))
				{
					editGlobal->InsertText(delimit + text);
				}

				bDelimit = false;
			}
		}
		break;
	}

	SetFocus(0);	
	return true;
}

void ScriptGlobals::Insert(char *str)
{
	editGlobal->InsertText(str);
}



void ScriptGlobals::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_CLOSE:
		{
			if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
			{
				if(originalScript != editGlobal->GetText(ALL_LINES))
				{
					PanelQuestion *panel = new PanelQuestion("Your code has been altered\nDo you want close anyway?");
					if(panel->Wait() == OK_BUTTON)
					{
						delete this;
					}
					
					delete panel;			
				}
				else
				{
					delete this;
				}
			}
			else
			{
				delete this;
			}
		}
		break;
	case BT_ADD:
		{
			if(editGlobal->GetText(ALL_LINES).length() < 1)
			{
				new PanelInfo("Can't add an empty script");
				return;
			}

			if(name->GetText().length() < 1)
			{
				new PanelInfo("Please, enter valid name");
				return;
			}

			GenericScript script;
			if(!script.Add(editGlobal->GetText(ALL_LINES), name->GetText()) || script.GetError().length())
			{
				if(script.GetError().find("Error") == gedString::npos)
				{
					//Warnings
					
					PanelQuestion *panel = new PanelQuestion(script.GetError() + "\n\nThere is some errors but are not fatal.\nProceed anyway?", "Confirm", "Yes", "No", ALIGN_LEFT);
					
					if(panel->Wait() != OK_BUTTON)
					{
						delete panel;						
						return;
					}
					
					//Add with errors and check again in game mode
					delete panel;
					script.Add(editGlobal->GetText(ALL_LINES), name->GetText(), true);
				}
				else
				{
					PanelQuestion *panel = new PanelQuestion(script.GetError() + "\n\nThere are script code errors that need to be corrected before your program will execute.\nProceed anyway?", "Error", "Yes", "No", ALIGN_LEFT);
					if(panel->Wait() != OK_BUTTON)
					{
						delete panel;						
						return;
					}
					
					//Add with errors and check again in game mode
					delete panel;
					script.Add(editGlobal->GetText(ALL_LINES), name->GetText(), true);	
				}							
			}

			SetText("");
			name->SetText("");

			//Don't call here (restore codes without update new symbos)
			//script.ParserAll();

			GenericScript::Populate(listChooseGlobal);
			//delete this;			
		}
		break;
	case BT_REMOVE:
		{
			PanelQuestion *panel = new PanelQuestion(" Remove code? ");
			if(panel->Wait() == OK_BUTTON)
			{
				if(!GenericScript::RemoveAndVerify(listChooseGlobal->GetText()))
				{
					delete panel;

					panel = new PanelQuestion(GenericScript::GetError() + "\n\nThere are script code errors that need to be corrected before your program will execute.\nProceed anyway?", "Error", "Yes", "No", ALIGN_LEFT);
					if(panel->Wait() != OK_BUTTON)
					{
						delete panel;						
						return;
					}

					delete panel;
					panel = NULL;
	
					//Remove with errors and check again in game mode
					GenericScript::RemoveAndVerify(listChooseGlobal->GetText(), true);
				}

				SetText("");
				name->SetText("");
				listChooseGlobal->RemoveText(listChooseGlobal->GetText());
			}

			delete panel;
		}
		break;

	case BT_CREATEVAR:
		{
			new VariableEditor(NULL, this);
		}
		break;
	}
}



#endif //#ifndef STAND_ALONE_GAME