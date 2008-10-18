// ChangePath.cpp: implementation of the ChangePath class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "ChangePath.h"
#include "GameControl.h"
#include "ExpressionEditor.h"
#include "Tutorial.h"
#include "../gameEngine/Path.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_CHANGEPATH			"\
Use this action to change the selected actor's path\n\
The path will begin at the current actor position"

#define TIP_CHANGEPATH_ACTOR			"\
Select the actor that will have its path changed\n"

#define TIP_CHANGEPATH_PATH			"\
Select the path\n\
\n\
You can choose:\n\
   a created path\n\
   no path (choose this to remove the current path)\n\
   random path (selected randomly in the created paths)"

#define TIP_CHANGEPATH_AXIS			"\
Specify whether this path affects x axis, y axis or both axis (horizontal and vertical)"

//////////////////////////////////////////////////////////////////////////////

enum
{
	LS_ACTOR,
	LS_PATH,
	BT_CLOSE,
	BT_ADD
};

#define WIDTH	220
#define HEIGHT	125

ChangePath::ChangePath(Actor *actor, bool bOnCollision)
	: BaseAction("Change Path", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_CHANGEPATH);
	this->eventActor = actionActor = actor;

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText(CHANGE_PATH, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText(" Actor: ", 10, y);
	listActor = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ACTOR); listActor->SetToolTip(gedString(TIP_CHANGEPATH_ACTOR) + TIP_ACTION_ACTOR);
	text = AddText("	 Path: ", 10, listActor->Down());
	listPath = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_PATH); listPath->SetToolTip(TIP_CHANGEPATH_PATH);
	text = AddText("	 Axis: ", 10, listPath->Down());
	listAxis = AddListPop(text->Right() + 2, text->Top(), 128); listAxis->SetToolTip(TIP_CHANGEPATH_AXIS);
	
	
	//Close
	y = DrawHLine(listAxis->Down() + 2);
	button = AddButton(Action::getEditAction()?"Ok":"Add", 45, y, 0, 0, BT_ADD); button->SetToolTip(TIP_ACTION_ADD); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right()+8, y, 0, 0, BT_CLOSE); button->SetToolTip(TIP_ACTION_CANCEL); SetCancelButton(button);


	GameControl::Get()->PopulatePaths(listPath);
	GameControl::Get()->PopulateActors(listActor);
	listActor->AddText(S_EVENT_ACTOR);

	if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		listActor->AddText(S_PARENT_ACTOR);
		listActor->AddText(S_CREATOR_ACTOR);
	}

	if(bOnCollision) listActor->AddText(S_COLLIDE_ACTOR);
	listActor->SetItem(S_EVENT_ACTOR);

	if(actionActor->getPathX())
	{
		listPath->SetItem(actionActor->getPathX()->getName());
	}
	else if(actionActor->getPathY())
	{
		listPath->SetItem(actionActor->getPathY()->getName());
	}

	listAxis->AddText("Both");
	listAxis->AddText("x");
	listAxis->AddText("y");
	listAxis->SetItem("Both");

	if(!ExpressionEditor::getExpressionEditor()) UpdateEdition();
}

void ChangePath::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	listActor->SetItem(editAction->actorName);
	if(listPath->SetItem(editAction->data.path.name) == -1)
	{
		listPath->SetItem(NO_PATH);
	}

	if(editAction->data.path.axis == X_AXIS) listAxis->SetItem("x");
	else if(editAction->data.path.axis == Y_AXIS) listAxis->SetItem("y");
	else listAxis->SetItem("Both");
}

ChangePath::~ChangePath()
{

}

bool ChangePath::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(BaseAction::OnList(list, index, text, listId)) return true;

	switch(listId)
	{
	case LS_ACTOR:
		{
			actionActor = GameControl::Get()->GetActor(text);
			if(actionActor == EVENT_ACTOR)
			{
				actionActor = eventActor;
			}
			else if(actionActor == PARENT_ACTOR)
			{
				if(eventActor->getParent() != GameControl::Get()->GetAxis())
				{
					actionActor = eventActor->getParent();
				}
				else
				{
					new PanelInfo("This actor has no parent");
					listActor->SetItem(S_EVENT_ACTOR);
					actionActor = eventActor;
				}
			}
			else if(actionActor == CREATOR_ACTOR)
			{
				actionActor = eventActor->getCreator();
			}
			
			if(actionActor > COLLIDE_ACTOR)
			{
				if(actionActor->getPathX())
				{
					listPath->SetItem(actionActor->getPathX()->getName());
				}
				else if(actionActor->getPathY())
				{
					listPath->SetItem(actionActor->getPathY()->getName());
				}
			}
		}
		break;
	}

	return true;
}

void ChangePath::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_CLOSE:
		{
			delete this;
		}
		break;
	case BT_ADD:
		{
			int axis = BOTH_AXIS;
			gedString axisStr = "BOTH_AXIS";

			if(listActor->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select an actor");
				return;
			}

			if(listPath->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select a path");
				return;
			}

			if(listAxis->GetText() == "x")
			{
				axis = X_AXIS;
				axisStr = "X_AXIS";
			}
			else if(listAxis->GetText() == "y")
			{
				axis = Y_AXIS;
				axisStr = "Y_AXIS";
			}

			if(IsEditingScript())
			{
				//Auto complete script

				char buf[256];
				sprintf(buf, "ChangePath(\"%s\", \"%s\", %s);",
					listActor->GetText().c_str(),
					listPath->GetText().c_str(),
					axisStr.c_str());
				InsertIntoScript(buf);
				delete this;
			}
			else
			{
				SelectActionType(eventActor, Action::Call(eventActor)->SetPath(listActor->GetText(), listPath->GetText(), axis));
			}			
		}
		break;
	}
}

#endif //#ifndef STAND_ALONE_GAME