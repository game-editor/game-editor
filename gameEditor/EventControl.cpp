// EventControl.cpp: implementation of the EventControl class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "EventControl.h"
#include "GameControl.h"
#include "NewTimer.h"
#include "ExpressionEditor.h"
#include "Tutorial.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_EVENTCONTROL_ENABLE			"\
Use this action to enable the specified event on the selected actor"

#define TIP_TIP_EVENTCONTROL_ENABLE_ACTOR			"\
Select the actor that will have its event enable\n"

#define TIP_TIP_EVENTCONTROL_ENABLE_EVENT			"\
Select the event to be enable or \"All Events\""



#define TIP_TIP_EVENTCONTROL_DISABLE		"\
Use this action to disable the specified event on the selected actor\n\
\n\
If an actor actors has a \"Mouse Button Down\" or \"Mouse Button Up\" events disables\n\
The click will be passed to next actor behind it"

#define TIP_EVENTCONTROL_DISABLE_ACTOR			"\
Select the actor that will have its event disable\n"

#define TIP_EVENTCONTROL_DISABLE_EVENT			"\
Select the event to be disable or \"All Events\""

//////////////////////////////////////////////////////////////////////////////

enum
{
	LS_ACTOR,
	LS_EVENT,
	BT_CLOSE,
	BT_ADD
};

#define WIDTH	220
#define HEIGHT	105

EventControl::EventControl(Actor *actor, bool bOnCollision, bool bEventEnable)
	: BaseAction("EventControl", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	eventActor = actionActor = actor;
	this->bEventEnable = bEventEnable;

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText(bEventEnable?EVENT_ENABLE:EVENT_DISABLE, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("   Actor: ", 10, y);
	listActor = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ACTOR);
	text = AddText("   Event: ", 10, listActor->Down());
	listEvent = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_EVENT);
	
	
	//Close
	y = DrawHLine(listEvent->Down() + 2);
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

	Action::PopulateEvents(listEvent);
	listEvent->AddText("All Events");
	listEvent->SetItem("All Events");	

	if(bEventEnable)
	{
		SetToolTip(TIP_EVENTCONTROL_ENABLE);
		listActor->SetToolTip(gedString(TIP_TIP_EVENTCONTROL_ENABLE_ACTOR) + TIP_ACTION_ACTOR);
		listEvent->SetToolTip(TIP_TIP_EVENTCONTROL_ENABLE_EVENT);
	}
	else
	{
		SetToolTip(TIP_TIP_EVENTCONTROL_DISABLE);
		listActor->SetToolTip(gedString(TIP_EVENTCONTROL_DISABLE_ACTOR) + TIP_ACTION_ACTOR);
		listEvent->SetToolTip(TIP_EVENTCONTROL_DISABLE_EVENT);
	}

	if(!ExpressionEditor::getExpressionEditor()) UpdateEdition();
}

void EventControl::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	listActor->SetItem(editAction->actorName);
	
	Uint32 events = editAction->data.eventcontrol.events;
	
	if(events == EVENTMOUSEBUTTONDOWN) listEvent->SetItem(EV_MOUSE_BUTTON_DOWN);
	else if(events == EVENTMOUSEBUTTONUP) listEvent->SetItem(EV_MOUSE_BUTTON_UP);
	else if(events == EVENTANIMATION) listEvent->SetItem(EV_ANIMATION);
	else if(events == EVENTANIMATIONFINISH) listEvent->SetItem(EV_ANIMATION_FINISH);
	else if(events == EVENTPATHFINISH) listEvent->SetItem(EV_PATH_FINISH);
	else if(events == EVENTKEYDOWN) listEvent->SetItem(EV_KEY_DOWN);
	else if(events == EVENTKEYUP) listEvent->SetItem(EV_KEY_UP);
	else if(events == EVENTTIMER) listEvent->SetItem(EV_TIMER);
	else if(events == EVENTCOLLISION) listEvent->SetItem(EV_COLLISION);
	else if(events == EVENTCOLLISIONFINISH) listEvent->SetItem(EV_COLLISION_FINISH);
	else if(events == EVENTCREATE) listEvent->SetItem(EV_CREATE);
	else if(events == EVENTDESTROYACTOR) listEvent->SetItem(EV_DESTROY_ACTOR);
	else if(events == EVENTOUTOFVISION) listEvent->SetItem(EV_OUT_OF_VISION);
	else if(events == EVENTACTIVATIONEVENT) listEvent->SetItem(EV_ACTIVATION_EVENT);
	else if(events == EVENTMOUSEENTER) listEvent->SetItem(EV_MOUSE_ENTER);
	else if(events == EVENTMOUSELEAVE) listEvent->SetItem(EV_MOUSE_LEAVE);
	else if(events == EVENTMOVEFINISH) listEvent->SetItem(EV_MOVE_FINISH);
	else if(events == EVENTALL) listEvent->SetItem("All Events");
}

EventControl::~EventControl()
{

}



void EventControl::OnButton(Button *button, int buttonId)
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
			gedString sevent = listEvent->GetText(), eventStr;

			if(listActor->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select an actor");
				return;
			}

			Uint32 events = 0;

			if(sevent == EV_MOUSE_BUTTON_DOWN)
			{
				events |= EVENTMOUSEBUTTONDOWN;
				eventStr = "EVENTMOUSEBUTTONDOWN";
			}
			if(sevent == EV_MOUSE_BUTTON_UP)
			{
				events |= EVENTMOUSEBUTTONUP;
				eventStr = "EVENTMOUSEBUTTONUP";
			}
			if(sevent == EV_ANIMATION)	
			{
				events |= EVENTANIMATION;
				eventStr = "EVENTANIMATION";
			}
			if(sevent == EV_ANIMATION_FINISH)
			{
				events |= EVENTANIMATIONFINISH;
				eventStr = "EVENTANIMATIONFINISH";
			}
			if(sevent == EV_PATH_FINISH)
			{
				events |= EVENTPATHFINISH;
				eventStr = "EVENTPATHFINISH";
			}
			if(sevent == EV_KEY_DOWN)	
			{
				events |= EVENTKEYDOWN;
				eventStr = "EVENTKEYDOWN";
			}
			if(sevent == EV_KEY_UP)	
			{
				events |= EVENTKEYUP;
				eventStr = "EVENTKEYUP";
			}
			if(sevent == EV_TIMER)
			{
				events |= EVENTTIMER;
				eventStr = "EVENTTIMER";
			}
			if(sevent == EV_COLLISION)
			{
				events |= EVENTCOLLISION;
				eventStr = "EVENTCOLLISION";
			}
			if(sevent == EV_COLLISION_FINISH)
			{
				events |= EVENTCOLLISIONFINISH;
				eventStr = "EVENTCOLLISIONFINISH";
			}
			if(sevent == EV_CREATE)
			{
				events |= EVENTCREATE;
				eventStr = "EVENTCREATE";
			}
			if(sevent == EV_DESTROY_ACTOR)
			{
				events |= EVENTDESTROYACTOR;
				eventStr = "EVENTDESTROYACTOR";
			}
			if(sevent == EV_OUT_OF_VISION)
			{
				events |= EVENTOUTOFVISION;
				eventStr = "EVENTOUTOFVISION";
			}
			if(sevent == EV_ACTIVATION_EVENT)
			{
				events |= EVENTACTIVATIONEVENT;
				eventStr = "EVENTACTIVATIONEVENT";
			}
			if(sevent == EV_MOUSE_ENTER)
			{
				events |= EVENTMOUSEENTER;
				eventStr = "EVENTMOUSEENTER";
			}
			if(sevent == EV_MOUSE_LEAVE)
			{
				events |= EVENTMOUSELEAVE;
				eventStr = "EVENTMOUSELEAVE";
			}
			if(sevent == EV_MOVE_FINISH)
			{
				events |= EVENTMOVEFINISH;
				eventStr = "EVENTMOVEFINISH";
			}
			if(sevent == "All Events")	
			{
				events |= EVENTALL;
				eventStr = "EVENTALL";
			}
			
			if(IsEditingScript())
			{
				gedString function = "EventDisable";
				if(bEventEnable) function = "EventEnable";

				//Auto complete script
				char buf[256];
				sprintf(buf, "%s(\"%s\", %s);",
					function.c_str(),
					listActor->GetText().c_str(),
					eventStr.c_str());
				InsertIntoScript(buf);
				delete this;
			}
			else
			{
				SelectActionType(eventActor, Action::Call(eventActor)->SetEventControl(listActor->GetText(), events, bEventEnable));
			}
			
		}
		break;
	}
}


#endif //#ifndef STAND_ALONE_GAME