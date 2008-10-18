// CreateTimer.cpp: implementation of the CreateTimer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "CreateTimer.h"
#include "GameControl.h"
#include "NewTimer.h"
#include "ExpressionEditor.h"
#include "Tutorial.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_CREATETIMER			"\
Use this action to create a timer for selected actor\n\
The created timer can be used to fire actions in the timer event"

#define TIP_CREATETIMER_ACTOR			"\
Select the actor that will have this timer\n"

#define TIP_CREATETIMER_TIMER			"\
Select the timer to be created\n\
You can define a new timer by choosing the \"New Timer\" option"
//////////////////////////////////////////////////////////////////////////////

enum
{
	LS_ACTOR,
	LS_TIMER,
	BT_CLOSE,
	BT_ADD
};

#define WIDTH	400
#define HEIGHT	105

CreateTimer::CreateTimer(Actor *actor, bool bOnCollision)
	: BaseAction("Create Timer", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_CREATETIMER);
	eventActor = actionActor = actor;

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText(CREATE_TIMER, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("   Actor: ", 10, y);
	listActor = AddListPop(text->Right() + 2, text->Top(), 300, 0, LS_ACTOR); listActor->SetToolTip(gedString(TIP_CREATETIMER_ACTOR) + TIP_ACTION_ACTOR);
	text = AddText("   Timer: ", 10, listActor->Down());
	listTimer = AddListPop(text->Right() + 2, text->Top(), 300, 0, LS_TIMER); listTimer->SetToolTip(TIP_CREATETIMER_TIMER);
	
	
	//Close
	y = DrawHLine(listTimer->Down() + 2);
	button = AddButton(Action::getEditAction()?"Ok":"Add", (WIDTH-135)/2, y, 0, 0, BT_ADD); button->SetToolTip(TIP_ACTION_ADD); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right()+8, y, 0, 0, BT_CLOSE); button->SetToolTip(TIP_ACTION_CANCEL); SetCancelButton(button);


	GameControl::Get()->PopulateTimers(listTimer);	
	GameControl::Get()->PopulateActors(listActor);

	listActor->AddText(S_EVENT_ACTOR);

	if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		listActor->AddText(S_PARENT_ACTOR);
		listActor->AddText(S_CREATOR_ACTOR);
	}

	if(bOnCollision) listActor->AddText(S_COLLIDE_ACTOR);

	listTimer->AddText("New Timer");
	//listTimer->SetItem(listTimer->GetText(0));

	listActor->SetItem(S_EVENT_ACTOR);

	if(!ExpressionEditor::getExpressionEditor()) UpdateEdition();
}

void CreateTimer::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	listActor->SetItem(editAction->actorName);

	char timerName[256];
	sprintf(timerName, "%s (", editAction->data.timer.name);
	int lenght = strlen(timerName);
	for(int i = 0; i < listTimer->Count(); i++)
	{
		if(listTimer->GetText(i).substr(0, lenght) == timerName)
		{
			listTimer->SetItem(i);
			break;
		}
	}
}

CreateTimer::~CreateTimer()
{

}

void CreateTimer::AddTimerDefinition(const gedString& timerName, Uint32 interval, int count, int type, Uint32 minRandomInterval)
{
	if(GameControl::Get()->AddTimerDefinition(timerName, interval, count, type, minRandomInterval))
	{
		listTimer->AddText(timerName);
		listTimer->SetItem(timerName);
	}
}

bool CreateTimer::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(BaseAction::OnList(list, index, text, listId)) return true;

	switch(listId)
	{
	case LS_TIMER:
		{
			if(text == "New Timer")
			{
				new NewTimer(this);
			}
		}
		break;
	}

	return true;
}

void CreateTimer::OnButton(Button *button, int buttonId)
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

			if(listTimer->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select a timer");
				return;
			}

			if(listTimer->GetText() == "New Timer")
			{
				new PanelInfo("Please, select or create a timer");
				return;
			}

			int milliseconds = GameControl::Get()->GetTimer(listTimer->GetText())->interval;
			if(IsEditingScript())
			{
				//Auto complete script
				char timerName[256];
				strcpy(timerName, listTimer->GetText().c_str());
				int pos = listTimer->GetText().find(" (");
				if(pos >= 0) timerName[pos] = 0;

				char buf[256];
				sprintf(buf, "CreateTimer(\"%s\", \"%s\", %d);",
					listActor->GetText().c_str(),
					timerName,
					milliseconds
					);
				InsertIntoScript(buf);
				delete this;
			}
			else
			{
				SelectActionType(eventActor, Action::Call(eventActor)->SetCreateTimer(listActor->GetText(), 
					listTimer->GetText(), 
					milliseconds));
			}
		}
		break;
	}
}

#endif //#ifndef STAND_ALONE_GAME