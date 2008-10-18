// ToMousePosition.cpp: implementation of the ToMousePosition class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "ToMousePosition.h"
#include "GameControl.h"
#include "ExpressionEditor.h"
#include "Tutorial.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_FOLLOW_MOUSE			"\
Use this action to make the selected actor follow the mouse"

#define TIP_FOLLOW_MOUSE_ACTOR			"\
Select the actor that will follow the mouse\n"

#define TIP_FOLLOW_MOUSE_AXIS			"\
Specify if the mouse affects x axis, y axis, both axis or none (end following)"
//////////////////////////////////////////////////////////////////////////////

enum
{
	LS_ACTOR,
	LS_PATH,
	BT_CLOSE,
	BT_ADD
};

#define WIDTH	220
#define HEIGHT	105

ToMousePosition::ToMousePosition(Actor *actor, bool bOnCollision)
	: BaseAction("To Mouse Position", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_FOLLOW_MOUSE);
	this->eventActor = actionActor = actor;

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText(FOLLOW_MOUSE, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("Actor: ", 10, y);
	listActor = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ACTOR); listActor->SetToolTip(gedString(TIP_FOLLOW_MOUSE_ACTOR) + TIP_ACTION_ACTOR);
	text = AddText(" Axis: ", 10, listActor->Down());
	listAxis = AddListPop(text->Right() + 2, text->Top(), 128); listAxis->SetToolTip(TIP_FOLLOW_MOUSE_AXIS);
	
	
	//Close
	y = DrawHLine(listAxis->Down() + 2);
	button = AddButton(Action::getEditAction()?"Ok":"Add", 45, y, 0, 0, BT_ADD); button->SetToolTip(TIP_ACTION_ADD); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right()+8, y, 0, 0, BT_CLOSE); button->SetToolTip(TIP_ACTION_CANCEL); SetCancelButton(button);


	GameControl::Get()->PopulateActors(listActor);
	listActor->AddText(S_EVENT_ACTOR);

	if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		listActor->AddText(S_PARENT_ACTOR);
		listActor->AddText(S_CREATOR_ACTOR);
	}

	if(bOnCollision) listActor->AddText(S_COLLIDE_ACTOR);
	listActor->SetItem(S_EVENT_ACTOR);


	listAxis->AddText("Both");
	listAxis->AddText("x");
	listAxis->AddText("y");
	listAxis->AddText("none");
	listAxis->SetItem("Both");

	if(!ExpressionEditor::getExpressionEditor()) UpdateEdition();
}

void ToMousePosition::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	listActor->SetItem(editAction->actorName);	

	if(editAction->data.path.axis == X_AXIS) listAxis->SetItem("x");
	else if(editAction->data.path.axis == Y_AXIS) listAxis->SetItem("y");
	else if(editAction->data.path.axis == NONE_AXIS) listAxis->SetItem("none");
	else listAxis->SetItem("Both");
}

ToMousePosition::~ToMousePosition()
{

}


void ToMousePosition::OnButton(Button *button, int buttonId)
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
			else if(listAxis->GetText() == "none") 
			{
				axis = NONE_AXIS;
				axisStr = "NONE_AXIS";
			}

			if(IsEditingScript())
			{
				//Auto complete script

				char buf[256];
				sprintf(buf, "FollowMouse(\"%s\", %s);",
					listActor->GetText().c_str(),
					axisStr.c_str());
				InsertIntoScript(buf);
				delete this;
			}
			else
			{
				SelectActionType(eventActor, Action::Call(eventActor)->SetFollowMouse(listActor->GetText(), axis));
			}			
		}
		break;
	}
}

#endif //#ifndef STAND_ALONE_GAME