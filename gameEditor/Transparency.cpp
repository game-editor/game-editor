// Transparency.cpp: implementation of the Transparency class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "Transparency.h"
#include "GameControl.h"
#include "ExpressionEditor.h"
#include "Tutorial.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_CHANGE_TRANSPARENCY			"\
Use this action to change the selected actor's transparency"

#define TIP_CHANGE_TRANSPARENCY_ACTOR			"\
Select the actor that will have its transparency changed\n"

#define TIP_CHANGE_TRANSPARENCY_AMOUNT			"\
Set transparency amount"

//////////////////////////////////////////////////////////////////////////////

enum
{
	LS_ACTOR,
	SL_TRANS,
	BT_CLOSE,
	BT_ADD
};

#define WIDTH	235
#define HEIGHT	105

Transparency::Transparency(Actor *actor, bool bOnCollision)
	: BaseAction("Set Transparency", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_CHANGE_TRANSPARENCY);
	eventActor = actionActor = actor;
	

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText(CHANGE_TRANSPARENCY, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("       Actor: ", 10, y);
	listActor = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ACTOR); listActor->SetToolTip(gedString(TIP_CHANGE_TRANSPARENCY_ACTOR) + TIP_ACTION_ACTOR);
	text = AddText("Transparency: ", 10, listActor->Down() + 2);
	slideTrans = AddSlide(text->Right(), text->Top(), 100, 14, SL_TRANS); slideTrans->SetToolTip(TIP_CHANGE_TRANSPARENCY_AMOUNT);
	
	
	//Close
	y = DrawHLine(slideTrans->Down() + 2);
	button = AddButton(Action::getEditAction()?"Ok":"Add", 55, y, 0, 0, BT_ADD); button->SetToolTip(TIP_ACTION_ADD); SetConfirmButton(button);
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

	oriTrans = actionActor->GetTransparency();

	if(!ExpressionEditor::getExpressionEditor()) UpdateEdition();
}

void Transparency::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	listActor->SetItem(editAction->actorName);	
	slideTrans->SetPos(editAction->data.transparency.data.fixed);	
	oriTrans = editAction->data.transparency.data.fixed;
}

Transparency::~Transparency()
{

}

bool Transparency::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(BaseAction::OnList(list, index, text, listId)) return true;

	switch(listId)
	{
	case LS_ACTOR:
		{
			if(actionActor) actionActor->SetTransparency(oriTrans);


			actionActor = GameControl::Get()->GetActor(listActor->GetText());
			if(actionActor == EVENT_ACTOR) actionActor = eventActor;
			else if(actionActor == PARENT_ACTOR && eventActor->getParent() && eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
			
		
			if(actionActor > COLLIDE_ACTOR) 
			{
				oriTrans = actionActor->GetTransparency();
			}
			else
			{
				actionActor = NULL;
			}
		}
		break;
	}

	return true;
}

void Transparency::OnSlide(Slide *slide, int slideId, double pos)
{
	if(actionActor) actionActor->SetTransparency(pos);
}

void Transparency::OnButton(Button *button, int buttonId)
{
	if(actionActor) actionActor->SetTransparency(oriTrans);

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
				sprintf(buf, "ChangeTransparency(\"%s\", %f);", 
					listActor->GetText().c_str(),
					slideTrans->GetPos());
				InsertIntoScript(buf);
				delete this;
			}
			else
			{
				SelectActionType(eventActor, Action::Call(eventActor)->SetTransparency(listActor->GetText(), slideTrans->GetPos()));
			}			
		}
		break;
	}
}


#endif //#ifndef STAND_ALONE_GAME