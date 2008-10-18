// DlgAnimationFinish.cpp: implementation of the DlgAnimationFinish class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "DlgAnimationFinish.h"
#include "GameControl.h"
#include "ActorEdit.h"



enum
{
	LS_ANIMATION,
	BT_CLOSE,
	LS_ACTION
};



#define WIDTH	220
#define HEIGHT	85

DlgAnimationFinish::DlgAnimationFinish(Actor *actor)
	: Panel("DlgAnimationFinish", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_DLG_ANIMATIONFINISH);
	eventActor = actionActor = actor;

	Text *text;
	Button *button;
	Actor *add;
	int y;

	//Title
	text = AddText("Animation Finish Event", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("Animation: ", 10, y);
	listAnimation = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ANIMATION); listAnimation->SetToolTip(TIP_DLG_ANIMATIONFINISH_ANIMATION);
		
	//Close
	y = DrawHLine(listAnimation->Down() + 5);
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


	GameControl::Get()->PopulateAnimations(actionActor, listAnimation);	
	listAnimation->AddText(ANY_ANIMATION);
	listAnimation->SetItem(ANY_ANIMATION);

	UpdateEdition();
}

void DlgAnimationFinish::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	listAnimation->SetItem(editAction->eventData.data.name);	
}

DlgAnimationFinish::~DlgAnimationFinish()
{

}

bool DlgAnimationFinish::OnList(ListPop *list, int index, gedString &text, int listId)
{
	switch(listId)
	{
	case LS_ACTION:
		{
			Action::EventAnimationFinish(listAnimation->GetText());
			Action::DoAction(eventActor, text);	
		}
		break;
	}

	return true;
}

void DlgAnimationFinish::OnButton(Button *button, int buttonId)
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
			//Add activation event
			Action::EventAnimationFinish(listAnimation->GetText());

			if(Action::getEditAction())
			{
				//Edit current action
				Action::Call(eventActor)->Action::UpdateEvent(); 
				Action::DoAction(eventActor, Action::GetActionName(Action::getEditAction()));
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


DlgPathFinish::DlgPathFinish(Actor *actor)
	: Panel("DlgAnimationFinish", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_DLG_PATH_FINISH);
	eventActor = actionActor = actor;

	Text *text;
	Button *button;
	Actor *add;
	int y;

	//Title
	text = AddText("Path Finish Event", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("Path: ", 10, y);
	listPath = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ANIMATION); listPath->SetToolTip(TIP_DLG_PATHFINISH_PATH);
		
	//Close
	y = DrawHLine(listPath->Down() + 5);
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


	GameControl::Get()->PopulatePaths(listPath, false);

	listPath->AddText(ANY_PATH);
	listPath->SetItem(ANY_PATH);

	UpdateEdition();
}

void DlgPathFinish::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	listPath->SetItem(editAction->eventData.data.name);
}

DlgPathFinish::~DlgPathFinish()
{

}

bool DlgPathFinish::OnList(ListPop *list, int index, gedString &text, int listId)
{
	switch(listId)
	{
	case LS_ACTION:
		{
			Action::EventPathFinish(listPath->GetText());
			Action::DoAction(eventActor, text);	
		}
		break;
	}

	return true;
}

void DlgPathFinish::OnButton(Button *button, int buttonId)
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
			//Add activation event
			Action::EventPathFinish(listPath->GetText());
			
			if(Action::getEditAction())
			{
				//Edit current action
				Action::Call(eventActor)->Action::UpdateEvent(); 
				Action::DoAction(eventActor, Action::GetActionName(Action::getEditAction()));
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