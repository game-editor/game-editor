// DlgGeneralEvent.cpp: implementation of the DlgGeneralEvent class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "DlgGeneralEvent.h"
#include "GameControl.h"
#include "ActorEdit.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_DLG_DRAW_ACTOR			"\
Use the Draw Actor Event to execute actions in every game frame.\n\
This event occurs at specified game frame rate"

#define TIP_DLG_CREATE_ACTOR			"\
Use the Create Actor Event to execute actions when the actor is created"

#define TIP_DLG_DESTROY_ACTOR			"\
Use the Destroy Actor Event to execute actions when this actor is destroyed"

#define TIP_DLG_OUT_OF_VISION			"\
Use the Out of Vision Event to execute actions when this actor leaves the vision field"

#define TIP_DLG_MOUSE_ENTER			"\
Use the Mouse Enter Event to execute actions when mouse enter in this actor"

#define TIP_DLG_MOUSE_LEAVE			"\
Use the Mouse Leave Event to execute actions when mouse leaves this actor"

#define TIP_DLG_MOVE_FINISH			"\
Use the Move Finish Event to execute actions when a\n\
move started with Move To action reaches the specified position"


//////////////////////////////////////////////////////////////////////////////

enum
{
	BT_CANCEL,
	LS_ACTION
};

#define WIDTH	215
#define HEIGHT	80

DlgGeneralEvent::DlgGeneralEvent(Actor *actor, const gedString& eventName)
	: Panel("DlgGeneralEvent", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	eventActor = actor;
	this->eventName = eventName;

	Text *text;
	Button *button;	
	Actor *add;
	int y;

	if(eventName == EV_ANIMATION) SetToolTip(TIP_DLG_DRAW_ACTOR);
	else if(eventName == EV_CREATE) SetToolTip(TIP_DLG_CREATE_ACTOR);
	else if(eventName == EV_DESTROY_ACTOR) SetToolTip(TIP_DLG_DESTROY_ACTOR);
	else if(eventName == EV_OUT_OF_VISION) SetToolTip(TIP_DLG_OUT_OF_VISION);
	else if(eventName == EV_MOUSE_ENTER) SetToolTip(TIP_DLG_MOUSE_ENTER);
	else if(eventName == EV_MOUSE_LEAVE) SetToolTip(TIP_DLG_MOUSE_LEAVE);
	else if(eventName == EV_MOVE_FINISH) SetToolTip(TIP_DLG_MOVE_FINISH);
	

	//Title
	text = AddText(eventName + " Event", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("Select action for this event", CENTER_TEXT, y);	
	
	//Close
	y = DrawHLine(text->Down() + 5);
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
		button = AddButton("Cancel", add->Right() + 10, y, 0, 0, BT_CANCEL); if(!Action::inNewActivationEvent()) button->SetToolTip(TIP_ACTION_CANCEL);	
	}
	else
	{
		button = AddButton("Close", add->Right() + 10, y, 0, 0, BT_CANCEL); if(!Action::inNewActivationEvent()) button->SetToolTip(TIP_ACTION_CLOSE);	
	}

	UpdateEdition();
}

void DlgGeneralEvent::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	//No data to edit
}

DlgGeneralEvent::~DlgGeneralEvent()
{

}

void DlgGeneralEvent::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_CANCEL:
		{
			Action::clearEditAction();
			delete this;
		}
		break;
	case LS_ACTION:
		{
			//Add activation event
			DoAction();
			
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
	}
}



bool DlgGeneralEvent::OnList(ListPop *list, int index, gedString &text, int listId)
{	
	DoAction();
	
	Action::DoAction(eventActor, text);
	
	return true;
}

void DlgGeneralEvent::DoAction()
{
	if(eventName == EV_ANIMATION) Action::EventAnimation();
	else if(eventName == EV_CREATE) Action::EventCreate();
	else if(eventName == EV_DESTROY_ACTOR) Action::EventDestroyActor();
	else if(eventName == EV_OUT_OF_VISION) Action::EventOutOfVision();
	else if(eventName == EV_MOUSE_ENTER) Action::EventMouseEnter(0, 0);
	else if(eventName == EV_MOUSE_LEAVE) Action::EventMouseLeave(0, 0);
	else if(eventName == EV_MOVE_FINISH) Action::EventMoveFinish();
}

#endif //#ifndef STAND_ALONE_GAME