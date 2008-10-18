// ToAnteriorPosition.cpp: implementation of the ToAnteriorPosition class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "ToAnteriorPosition.h"
#include "GameControl.h"
#include "ExpressionEditor.h"
#include "Tutorial.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_TO_ANTERIOR_POSITION			"\
Use this action to move the selected actor to the position in the previous frame"

#define TIP_TO_ANTERIOR_POSITION_ACTOR			"\
Select the actor to be moved\n"


//////////////////////////////////////////////////////////////////////////////

enum
{
	BT_CLOSE,
	BT_ADD,
	LS_ACTOR
};

#define WIDTH	220
#define HEIGHT	85

ToAnteriorPosition::ToAnteriorPosition(Actor *actor, bool bOnCollision)
	: BaseAction("To Anterior Position", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_TO_ANTERIOR_POSITION);
	this->eventActor = actionActor = actor;

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText(TO_ANTERIOR_POSITION, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("    Actor: ", 10, y);
	listActor = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ACTOR); listActor->SetToolTip(gedString(TIP_TO_ANTERIOR_POSITION_ACTOR) + TIP_ACTION_ACTOR);	
	
	//Close
	y = DrawHLine(listActor->Down() + 2);
	button = AddButton(Action::getEditAction()?"Ok":"Add", (WIDTH-135)/2, y, 0, 0, BT_ADD); button->SetToolTip(TIP_ACTION_ADD); SetConfirmButton(button);
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

	if(!ExpressionEditor::getExpressionEditor()) UpdateEdition();
}

void ToAnteriorPosition::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	listActor->SetItem(editAction->actorName);
}

ToAnteriorPosition::~ToAnteriorPosition()
{

}


void ToAnteriorPosition::OnButton(Button *button, int buttonId)
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
			if(listActor->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select an actor");
				return;
			}

			if(IsEditingScript())
			{
				//Auto complete script
				char buf[256];
				sprintf(buf, "ToAnteriorPosition(\"%s\");",
					listActor->GetText().c_str());
				InsertIntoScript(buf);
				delete this;
			}
			else
			{
				SelectActionType(eventActor, Action::Call(eventActor)->SetToAnteriorPosition(listActor->GetText()));
			}
		}
		break;
	}
}

#endif //#ifndef STAND_ALONE_GAME