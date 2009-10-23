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


// ConditionalAction.cpp: implementation of the ConditionalAction class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "ConditionalAction.h"
#include "GameControl.h"


enum
{
	LS_COMPLETE1,
	LS_ACTOR1,
	LS_COMPLETE2,
	LS_ACTOR2,
	LS_ACTION,
	BT_CLOSE
};

#define S_EQUAL					"Equal"
#define S_GREATER_THAN			"Greater Than"
#define S_SMALLER_THAN			"Less Than"
#define S_GREATER_OR_EQUAL_THAN	"Greater or Equal To"
#define S_SMALLER_OR_EQUAL_THAN	"Less or Equal To"

#define WIDTH	450
#define HEIGHT	175

stAction *ConditionalAction::firstAction = NULL;
ListConditionalActionDlg ConditionalAction::listConditionalActionDlg;

ConditionalAction::ConditionalAction(Actor *actor, bool bOnCollision)
	: BaseAction("Conditional Action", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	eventActor = actor;
	bConditionAdd = false;

	Text *text;
	Button *button;
	int y;

	//Title
	text = AddText(CONDITIONAL_ACTION, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("Expression 1: ", 10, y);
	editExp1 = AddEditBox(text->Right(), text->Top(), 350);
	listActor1 = AddListPop(10, editExp1->Down()+4, 215, 0, LS_ACTOR1, "actors");
	listAutoComplete1 = AddListPop(listActor1->Right(), listActor1->Top(), 215, 0, LS_COMPLETE1, "variables");

	text = AddText("Condition: ", 10, listAutoComplete1->Down() + 4);
	listCondition = AddListPop((450-128)/2, text->Top());

	text = AddText("Expression 2: ", 10, listCondition->Down() + 4);
	editExp2 = AddEditBox(text->Right(), text->Top(), 350);
	listActor2 = AddListPop(10, editExp2->Down()+4, 215, 0, LS_ACTOR2, "actors");
	listAutoComplete2 = AddListPop(listActor2->Right(), listActor2->Top(), 215, 0, LS_COMPLETE2, "variables");
		
	//Close
	y = DrawHLine(listAutoComplete2->Down() + 2);
	listActions = AddListPop(WIDTH/2 - 75, y, 85, 0, LS_ACTION, Action::getEditAction()?"Edit Action":"Add Action");
	Action::Populate(listActions);
	button = AddButton("Cancel", listActions->Right() + 10, y, 0, 0, BT_CLOSE);


	listCondition->AddText(S_EQUAL);
	listCondition->AddText(S_GREATER_THAN);
	listCondition->AddText(S_SMALLER_THAN);
	listCondition->AddText(S_GREATER_OR_EQUAL_THAN);
	listCondition->AddText(S_SMALLER_OR_EQUAL_THAN);
	listCondition->SetItem(S_EQUAL);


	//Populate actors
	GameControl::Get()->PopulateActors(listActor1);
	if(bOnCollision) listActor1->AddText("collide");

	GameControl::Get()->PopulateActors(listActor2);
	if(bOnCollision) listActor2->AddText("collide");

	//Populate list vars
	Script::PopulateActorVars(listAutoComplete1);
	Script::PopulateActorVars(listAutoComplete2);

	userVarsIndex = listAutoComplete1->AddText("--- User Vars ---");
	userVarsIndex = listAutoComplete2->AddText("--- User Vars ---");

	Script::PopulateVars(listAutoComplete1);
	Script::PopulateVars(listAutoComplete2);

	bDelimit1 = bDelimit2 = false;
	listConditionalActionDlg.PushBack(this);

	pScript = NULL;
	UpdateEdition();
	if(!pScript) pScript = new Script();
}

void ConditionalAction::UpdateEdition()
{
	editAction = Action::getEditAction();
	if(!editAction) return;

	if((pScript = GameControl::Get()->GetExpression(editAction->data.conditionalaction.expression1)) != NULL)
	{
		int i = 0, f = 0, condition;
		if((i = pScript->getCode().find(">")) == gedString::npos)
		{
			if((i = pScript->getCode().find("<")) == gedString::npos)
			{
				i = pScript->getCode().find("=");
				condition = EQUAL;
			}
			else condition = SMALLER_THAN;
		}
		else condition = GREATER_THAN;

		f = i+1;
		if(pScript->getCode()[i+1] == '=')
		{
			if(condition == GREATER_THAN) condition = GREATER_OR_EQUAL_THAN;
			else if(condition == SMALLER_THAN) condition = SMALLER_OR_EQUAL_THAN;
			f++;
		}
		
		if(condition == GREATER_THAN) listCondition->SetItem(S_GREATER_THAN);
		else if(condition == SMALLER_THAN) listCondition->SetItem(S_SMALLER_THAN);
		else if(condition == GREATER_OR_EQUAL_THAN) listCondition->SetItem(S_GREATER_OR_EQUAL_THAN);
		else if(condition == SMALLER_OR_EQUAL_THAN) listCondition->SetItem(S_SMALLER_OR_EQUAL_THAN);
		else listCondition->SetItem(S_EQUAL);
		
		editExp1->SetText(pScript->getCode().substr(0, i));
		editExp2->SetText(pScript->getCode().substr(f, pScript->getCode().length()-f-1));
	}

	//Populate actions
	eventActor->getAction()->PopulateConditionalActions(listActions, editAction);
}

ConditionalAction::~ConditionalAction()
{

}




void ConditionalAction::OnEditChar(EditBox *edit, SDLKey key, int line)
{
	bDelimit1 = bDelimit2 = false;
}






bool ConditionalAction::OnList(ListPop *list, int index, gedString &text, int listId)
{
	switch(listId)
	{
	case LS_ACTOR1:
		{
			editExp1->InsertText(text);
			bDelimit1 = true;
			SetFocus(0);
		}
		break;
	case LS_ACTOR2:
		{
			editExp2->InsertText(text);
			bDelimit2 = true;
			SetFocus(1);
		}
		break;
	case LS_COMPLETE1:
		{
			if(text[0] != '-')
			{
				gedString delimit;
				if(bDelimit1 && index < userVarsIndex) delimit = VAR_DELIMIT;
				editExp1->InsertText(delimit + text);
				bDelimit1 = false;
				SetFocus(0);
			}
		}
		break;
	case LS_COMPLETE2:
		{
			if(text[0] != '-')
			{
				gedString delimit;
				if(bDelimit2 && index < userVarsIndex) delimit = VAR_DELIMIT;
				editExp2->InsertText(delimit + text);
				bDelimit2 = false;
				SetFocus(1);
			}
		}
		break;
	case LS_ACTION:
		{
			gedString exp = editExp1->GetText();

			if(listCondition->GetText() == S_GREATER_THAN) exp += ">";
			else if(listCondition->GetText() == S_SMALLER_THAN) exp += "<";
			else if(listCondition->GetText() == S_GREATER_OR_EQUAL_THAN) exp += ">=";
			else if(listCondition->GetText() == S_SMALLER_OR_EQUAL_THAN) exp += "<=";
			else  exp += "==";

			exp += editExp2->GetText() + ";";
			
			if(pScript && !pScript->Parser(exp, true))
			{
				new PanelInfo(pScript->GetError(), "Error", ALIGN_LEFT);
				return true;
			}

			

			/*
			Possible conditional actions (boolean and conditions):
			condition test 1 -> condition test 2 ->... condition test n -> 
			conditional action 1 -> conditional action 2 -> ... conditional action n
			*/

			stAction *action = NULL;
			Action::setEditAction(editAction);
			if(!bConditionAdd)
			{				
				action = Action::Call(eventActor)->SetConditionalAction(pScript);

				bConditionAdd = true;
			}

			Disable(); //Disable equation edit
			if(!Action::getEditAction())
			{
				if(!firstAction) firstAction = action;				
				Action::DoAction(eventActor, text);
			}
			else
			{
				Action::ShowActionDialog((stAction *)list->GetItemData(index), eventActor);
			}
		}
		break;
	}	

	return true;
}

void ConditionalAction::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_CLOSE:
		{
			if(firstAction)
			{
				if(eventActor->getAction()->getLasAction()->actionType == SET_CONDITIONAL_ACTION)
				{
					eventActor->getAction()->DeleteConditionalAction(firstAction);
				}

				firstAction = NULL;
			}

			Action::SetConditionalAction(false);
			
			//Close all conditional action dialogs
			while(!listConditionalActionDlg.Empty())
			{
				ConditionalAction *dlg = listConditionalActionDlg.Back();
				listConditionalActionDlg.PopBack();
				
				delete dlg;
			}			
		}
		break;
	}
}


void ConditionalAction::Disable()
{
	editExp1->Disable();
	editExp2->Disable();
	listActor1->Disable();
	listAutoComplete1->Disable();
	listActor2->Disable();
	listAutoComplete2->Disable();
	listCondition->Disable();
}

#endif //#ifndef STAND_ALONE_GAME