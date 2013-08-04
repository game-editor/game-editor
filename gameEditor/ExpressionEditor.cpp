/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com, http://game-editor.com

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
More info at http://game-editor.com/License
Be a Game Editor developer: Be a Game Editor developer: <http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>  .

***************************************************************************/


// ExpressionEditor.cpp: implementation of the ExpressionEditor class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "ExpressionEditor.h"
#include "GameControl.h"
#include "VariableEditor.h"
#include "LoadSaveScript.h"
#include "ScriptGlobals.h"
#include "Tutorial.h"
#include "MainPanel.h"
#include "FunctionPanel.h"
#include "Config.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_SCRIPTEDITOR			"\
Use this action to execute a script code in C programming language,\n\
that allows a high development power and a high level of flexibility\n\
\n\
All actions can be activated through scripts.\n\
When an action is selected in the function list, its corresponding dialog box will pop-up"

#define TIP_SCRIPTEDITOR_GLOBALCODE			"\
Use Global Code Editor to freely add any C code,\n\
like arrays, structs and functions,\n\
which can be accessed by any script"

#define TIP_SCRIPTEDITOR_FILE			"\
Load a script from a file or save the script to a file"

#define TIP_SCRIPTEDITOR_WEB			"Click here to get more about C language on the Internet"

//////////////////////////////////////////////////////////////////////////////


enum
{
	LS_COMPLETE,
	LS_ACTOR,
	BT_CREATEVAR,
	BT_CLOSE,
	BT_ADD,
	LS_FILE,
	BT_GLOBALS,
	BT_WEB
};

#define WIDTH	Config::Get()->getEditorSizeX()
#define HEIGHT	Config::Get()->getEditorSizeY()

bool IsEditingScript()
{
	return ExpressionEditor::getExpressionEditor() != NULL ||
		   ScriptGlobals::getScriptGlobals() != NULL;
}

void InsertIntoScript(char *str)
{
	if(ScriptGlobals::getScriptGlobals())
	{
		//Priority to Global code
		ScriptGlobals::getScriptGlobals()->Insert(str);
	}
	else if(ExpressionEditor::getExpressionEditor())
	{
		ExpressionEditor::getExpressionEditor()->Insert(str);
	}
}

gedString GetEditingScriptTextUntilCursor()
{
	gedString code;

	if(ScriptGlobals::getScriptGlobals())
	{
		//Priority to Global code
		code = ScriptGlobals::getScriptGlobals()->GetTextUntilCursor();
	}
	else if(ExpressionEditor::getExpressionEditor())
	{
		code = gedString("void ex_GedFunction_Test_() {\n") + ExpressionEditor::getExpressionEditor()->GetTextUntilCursor();
	}

	return code;
}

int GetNewVariableInsertionPosition()
{
	int pos = 0;

	gedString code;

	if(ScriptGlobals::getScriptGlobals())
	{
		//Priority to Global code
		code = ScriptGlobals::getScriptGlobals()->GetTextUntilCursor();
	}
	else if(ExpressionEditor::getExpressionEditor())
	{
		code = ExpressionEditor::getExpressionEditor()->GetTextUntilCursor();
	}

	//Search the first opened {
	int i = code.length() - 1;
	bool bClosed = false;

	while(i >= 0)
	{
		char c = code.at(pos);

		if(c == '}')
		{
			bClosed = true;
		}
		else if(c == '{' && !bClosed)
		{
			pos = i;
			bClosed = false;
		}

		i--;
	}

	return pos;
}


ExpressionEditor *ExpressionEditor::expressionEditor = NULL;

ExpressionEditor::ExpressionEditor(Actor *actor, bool bOnCollision)
	: BaseAction("Expression Editor", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	expressionEditor = this;
	SetModal();
	SetToolTip(TIP_SCRIPTEDITOR);

	eventActor = actionActor = actor;
	this->bOnCollision = bOnCollision;

	Text *text;
	Button *button;	
	int y;

	//Title: Script Editor: oi -> Key Down (left)
	gedString title(EXPRESSION_EDITOR), actionDesc;
	title += ": ";
	actionDesc = actor->getActorName();
	actionDesc += " -> ";

	editAction = Action::getEditAction();
	if(editAction)
	{		
		actionDesc += Action::GetDescription(editAction, false);

		MainPanel::AddLastScript(actionDesc);
	}
	else
	{
		stAction tmp;
		tmp.eventData = Action::getActualEvent();

		actionDesc += Action::GetDescription(&tmp, false);
	}

	title += actionDesc;

	text = AddText(title, CENTER_TEXT, 5);
	textLine = AddText("Ln 1    ", WIDTH-45, 5, ALIGN_LEFT);
	y = DrawHLine(text->Down() + 2);

	//Body
	editExp = AddEditBox(10, y, WIDTH-34, HEIGHT-85, true);
	listActor = AddListPop(10, editExp->Down()+4, 120, 0, LS_ACTOR, "actors"); listActor->SetToolTip(TIP_SCRIPTEDITOR_ACTORS);
	listAutoComplete = AddListPop(listActor->Right(), listActor->Top(), 215, 0, LS_COMPLETE, "variables/functions"); listAutoComplete->SetToolTip(TIP_SCRIPTEDITOR_VARIABLES_FUNCTIONS);
	button = AddButton("Variables", listAutoComplete->Right(), listAutoComplete->Top(), 0, 0, BT_CREATEVAR); button->SetToolTip(TIP_SCRIPTEDITOR_VARIABLES);
	button = AddButton("Global code", button->Right(), button->Top(), 0, 0, BT_GLOBALS); button->SetToolTip(TIP_SCRIPTEDITOR_GLOBALCODE);
	listFile = AddListPop(button->Right() + 2, button->Top(), 64, 0, LS_FILE, "File"); listFile->SetToolTip(TIP_SCRIPTEDITOR_FILE);

	//button = AddButton("Web", listFile->Right() + 2, listFile->Top(), 35, 0, BT_WEB); button->SetToolTip(TIP_SCRIPTEDITOR_WEB);

	
	
	//Close
	y = DrawHLine(listAutoComplete->Down() + 2);
	button = AddButton(Action::getEditAction()?"Ok":"Add", WIDTH/2 - 65, y, 0, 0, BT_ADD); button->SetToolTip(TIP_ACTION_ADD);
	button = AddButton("Cancel", button->Right()+8, y, 0, 0, BT_CLOSE); button->SetToolTip(TIP_ACTION_CANCEL); //Don't exit on ESC (crash the editor)


	//Populate actors
	GameControl::Get()->PopulateActors(listActor);
	if(bOnCollision) listActor->AddText("collide");

	UpdateVars();

	listFile->AddText("Save");
	listFile->AddText("Load");

	bDelimit = false;

	editExp->SetSyntaxColorize(true);
	editExp->ShowScrollBar();

	pScript = NULL;
	UpdateEdition();
	if(!pScript) pScript = new Script();
}

void ExpressionEditor::NotifyDeletedScript(Script *p)
{
  // Called when a script is removed to let the Script Editor update the pScript object
  if(p && pScript && pScript==p)
  {
    // the current pScript is invalid, create a new one
    pScript = new Script();
  }
}

void ExpressionEditor::UpdateEdition()
{
	editAction = Action::getEditAction();
	if(!editAction) return;

	if((pScript = GameControl::Get()->GetExpression(editAction->data.name)) != NULL)
	{
		editExp->SetText(pScript->getCode(), ALL_LINES);
		originalScript = editExp->GetText(ALL_LINES);
	}
}

ExpressionEditor::~ExpressionEditor()
{
	expressionEditor = NULL;
}



void ExpressionEditor::OnEditChar(EditBox *edit, SDLKey key, int line)
{
	bDelimit = false;
}

void ExpressionEditor::OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message)
{
	BaseAction::OnMessage(fromActor, messageId, message);

	if(messageId == LINE_CHANGED)
	{
		char buf[128];
		sprintf(buf, "Ln %ld", message+1);
		textLine->SetText(buf);
	}
}

bool ExpressionEditor::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(BaseAction::OnList(list, index, text, listId)) return true;

	switch(listId)
	{
	case LS_ACTOR:
		{
			editExp->InsertText(text);
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
					Action::DoAction(eventActor, actionName, bOnCollision);
				}
				else if(!Tutorial::IsCompatible(VERSION_PREFERENCES) || !FunctionCompletion::Get()->GetCallText(text))
				{
					editExp->InsertText(delimit + text);
				}

				bDelimit = false;
			}
		}
		break;
	case LS_FILE:
		{
			if(text == "Save")
			{
				new LoadSaveScript(SAVE_DIALOG, this);
			}
			else
			{
				new LoadSaveScript(LOAD_DIALOG, this);
			}
		}
		break;
	}

	SetFocus(0);

	return true;
}



void ExpressionEditor::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_CLOSE:
		{
			if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
			{
				if(originalScript != editExp->GetText(ALL_LINES))
				{
					PanelQuestion *panel = new PanelQuestion("Your code has been altered.\nIf you close now all changes will be lost.\nDo you want close anyway?");
					if(panel->Wait() == OK_BUTTON)
					{
						//Restore code
						if(pScript) pScript->Parser(originalScript, false, false, true);
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
	case BT_WEB:
		{
			openUrl("http://game-editor.com/programming.html");
		}
		break;
	case BT_ADD:
		{
			gedString code(editExp->GetText(ALL_LINES));

			if(code.empty())
			{
				new PanelInfo("Can't add an empty script\nPlease, enter your script or close", "Error");
				return;
			}

			if(pScript)
			{
				if(!pScript->Parser(code, false, false, true) || pScript->GetError().length())
				{
					//Error or only warnings?
					
					if(pScript->GetError().find("Error") == gedString::npos)
					{
						//Warnings
						
						PanelQuestion *panel = new PanelQuestion(pScript->GetError() + "\n\nThere is some errors but are not fatal.\nProceed anyway?", "Confirm", "Yes", "No", ALIGN_LEFT);
						
						if(panel->Wait() != OK_BUTTON)
						{
							delete panel;						
							return;
						}
						
						delete panel;
					}
					else
					{
						//Errors
						PanelQuestion *panel = new PanelQuestion(pScript->GetError() + "\n\nThere are script code errors that need to be corrected before your program will execute.\nProceed anyway?", "Error", "Yes", "No", ALIGN_LEFT);
						if(panel->Wait() != OK_BUTTON)
						{
							delete panel;						
							return;
						}
						
						//Add with errors and check again in game mode
						pScript->setAddedWithError(true);
						delete panel;
					}				
				}
				else
				{
					pScript->setAddedWithError(false);
				}
			}
			
			
			if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
			{
				SelectActionType(eventActor, Action::Call(eventActor)->SetExpression(pScript));
			}
			else
			{
				//Old code
				//Don't allow edit action type
				if(!editAction)
				{
					//Add
					SelectActionType(eventActor, Action::Call(eventActor)->SetExpression(pScript));
				}
				else
				{
					delete this;
				}
			}
		}
		break;
	case BT_CREATEVAR:
		{
			new VariableEditor(this);
		}
		break;
	case BT_GLOBALS:

#if !defined(GAME_EDITOR_HOME_EDITION)
		new ScriptGlobals();

		//Check the erros only if add this script
		/*if(pScript->Parser(editExp->GetText(ALL_LINES), false, false, true))
		{
			new ScriptGlobals();
		}
		else
		{
			gedString error(pScript->GetError());
			error += "\n\nFix all errors in this script before enter global codes";

			new PanelInfo(error, "Error", ALIGN_LEFT);
			return;
		}*/
#else
		new PanelInfo(GAME_EDITOR_VERSION_ERROR);
#endif
		break;
	}
}



void ExpressionEditor::UpdateVars()
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

void ExpressionEditor::Insert(char *str)
{
	editExp->InsertText(str);
}

void ExpressionEditor::SetText(char *str)
{
	editExp->StartAction();

	editExp->SetText("");
	editExp->SetText(str, ALL_LINES);

	editExp->EndAction();
}


gedString ExpressionEditor::GetText()
{
	return editExp->GetText(ALL_LINES);
}

gedString ExpressionEditor::GetTextUntilCursor()
{
	return editExp->GetTextUntilCursor();
}


#endif //#ifndef STAND_ALONE_GAME
