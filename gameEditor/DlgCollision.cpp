// DlgCollision.cpp: implementation of the DlgCollision class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "DlgCollision.h"
#include "GameControl.h"
#include "ActorEdit.h"
#include "Tutorial.h"



enum
{
	LS_ACTOR,
	LS_ANIMATION,
	BT_CLOSE,
	LS_ACTION,
	LS_TYPE,
	LS_SIDE
};

#define WIDTH	220
#define HEIGHT	82

DlgCollision::DlgCollision(Actor *actor, bool bCollision)
	: Panel("DlgCollision", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - (bCollision?(HEIGHT + 55):HEIGHT))/2,
						  WIDTH, bCollision?(HEIGHT + 55):HEIGHT)
{
	SetModal();

	eventActor = actionActor = actor;
	this->bCollision = bCollision;
	

	Text *text;
	Button *button;
	Actor *add;
	int y;

	//Title
	text = AddText(bCollision?"Collision Event":"Collision Finish Event", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	if(bCollision)
	{
		SetToolTip(TIP_DLG_COLLISION);

		text = AddText("       On: ", 10, y);
		listSide = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_SIDE); listSide->SetToolTip(TIP_DLG_COLLISION_SIDE);

		text = AddText(" Of Actor: ", 10, listSide->Down() + 2);
		listActor = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ACTOR); listActor->SetToolTip(TIP_DLG_COLLISION_ACTOR);

		text = AddText("        Repeat this event\nwhile actor is colliding: ", 10, listActor->Down() + 2);
		listType = AddListPop(listActor->Right() - 40, text->Top() + 10, 40, 0, LS_TYPE); listType->SetToolTip(TIP_DLG_COLLISION_REPEAT);

		listType->AddText("Yes");
		listType->AddText("No");
		listType->SetItem("No");

		listSide->AddText("Any Side");
		listSide->AddText("Top Side");
		listSide->AddText("Bottom Side");
		listSide->AddText("Left Side");
		listSide->AddText("Right Side");
		listSide->AddText("Top or Bottom Side");
		listSide->AddText("Left or Right Side");

		listSide->SetItem("Any Side");
		side = ANY_SIDE;


		y = listType->Down() + 2;
	}
	else
	{
		SetToolTip(TIP_DLG_COLLISION_FINISH);

		text = AddText("    Actor: ", 10, y);
		listActor = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ACTOR); listActor->SetToolTip(TIP_DLG_COLLISION_ACTOR);

		y = listActor->Down() + 2;
	}
		
	//Close
	y = DrawHLine(y);
	if(Action::inNewActivationEvent())
	{
		add = AddButton("Choose Actor", 25, y, 0, 0, LS_ACTION); ((Button *)add)->SetToolTip(TIP_ACTIVATIONEVENT_CHOOSE);
	}
	else if(Action::getEditAction())
	{
		add = AddButton("Edit Action", (WIDTH-135)/2 - 12, y, 0, 0, LS_ACTION); ((Button *)add)->SetToolTip(TIP_ACTION_EDIT);
	}
	else
	{
		add = AddListPop(30, y, 85, 0, LS_ACTION, "Add Action"); ((ListPop *)add)->SetToolTip(TIP_ADDACTION);	
		Action::Populate((ListPop *)add);
	}

	if(Action::getEditAction())
	{
		button = AddButton("Cancel", add->Right() + 10, y, 0, 0, BT_CLOSE); if(!Action::inNewActivationEvent()) button->SetToolTip(TIP_ACTION_CANCEL);	
	}
	else
	{
		button = AddButton("Close", add->Right() + 10, y, 0, 0, BT_CLOSE); if(!Action::inNewActivationEvent()) button->SetToolTip(TIP_ACTION_CLOSE);	
	}

	

	 

	GameControl::Get()->PopulateActors(listActor);

	if(!Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		//Set default only in old tutorials
		listActor->SetItem(actionActor->getCloneIndex()?actionActor->getCloneName():actionActor->getActorName());	
	}

	listActor->AddText(ANY_ACTOR);
	listActor->RemoveText(VIEW_NAME);

	UpdateEdition();
}

void DlgCollision::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	if(editAction->eventData.eventType == EVENT_COLLISION)
	{
		listActor->SetItem(editAction->eventData.data.name);

		//Type
		if(editAction->eventData.data.name[NAME_LIMIT-1])
		{
			listType->SetItem("No");
		}
		else
		{
			listType->SetItem("Yes");
		}

		//Side
		switch(editAction->eventData.data.name[NAME_LIMIT-2])
		{
		case TOP_SIDE:
			listSide->SetItem("Top Side");
			break;

		case BOTTOM_SIDE:
			listSide->SetItem("Bottom Side");
			break;

		case LEFT_SIDE:
			listSide->SetItem("Left Side");
			break;

		case RIGHT_SIDE:
			listSide->SetItem("Right Side");
			break;

		case TOP_OR_BOTTOM_SIDE:
			listSide->SetItem("Top or Bottom Side");
			break;

		case LEFT_OR_RIGHT_SIDE:
			listSide->SetItem("Left or Right Side");
			break;
		}

		side = editAction->eventData.data.name[NAME_LIMIT-2];
	}
	else
	{
		listActor->SetItem(editAction->eventData.data.name);
	}
}

DlgCollision::~DlgCollision()
{

}

bool DlgCollision::OnList(ListPop *list, int index, gedString &text, int listId)
{
	switch(listId)
	{
	case LS_ACTION:
		{
			if(listActor->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select an actor");
				return true;
			}

			//Don't add collisions with itself
			if(listActor->GetText() == eventActor->getCloneName())
			{
				new PanelInfo("An actor can't collide with itself");
				return true;
			}

			if(bCollision) Action::EventCollision(listActor->GetText(), listType->GetText() == "Yes", side);
			else Action::EventCollisionFinish(listActor->GetText());

			Action::DoAction(eventActor, text, true);	
		}
		break;
	case LS_SIDE:
		{
			if(text == "Top Side") side = TOP_SIDE;	
			else if(text == "Bottom Side") side = BOTTOM_SIDE;
			else if(text == "Left Side") side = LEFT_SIDE;
			else if(text == "Right Side") side = RIGHT_SIDE;
			else if(text == "Top or Bottom Side") side = TOP_OR_BOTTOM_SIDE;
			else if(text == "Left or Right Side") side = LEFT_OR_RIGHT_SIDE;
			else side = ANY_SIDE;
		}
		break;
	}

	return true;
}

void DlgCollision::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_CLOSE:
		{
			Action::clearEditAction();
			delete this;
		}
		break;
	case LS_ACTION:
		{
			if(listActor->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select an actor");
				return;
			}

			//Don't add collisions with itself
			if(listActor->GetText() == eventActor->getCloneName())
			{
				new PanelInfo("An actor can't collide with itself");
				return;
			}

			

			
			if(bCollision) Action::EventCollision(listActor->GetText(), listType->GetText() == "Yes", side);
			else Action::EventCollisionFinish(listActor->GetText());

			if(Action::getEditAction())
			{
				//Edit current action
				Action::Call(eventActor)->Action::UpdateEvent(); 
				Action::DoAction(eventActor, Action::GetActionName(Action::getEditAction()), true);
			}
			else
			{
				//Add activation event
				ActorEdit::DoActivationEvent(true);
			}

			delete this;
		}
		break;
	}
}


#endif //#ifndef STAND_ALONE_GAME