// PhysicalBounce.cpp: implementation of the PhysicalBounce class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "PhysicalBounce.h"
#include "GameControl.h"
#include "ExpressionEditor.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_PHYSICALRESPONSE			"\
Use this action to move physically a actor after a collision event\n\
You can adjust the final velocity by changing the final velocity multiplier for\n\
the actors in collision"

#define TIP_TIP_PHYSICALRESPONSE_MOVE			"\
Select if after a collision, should move \"Event Actor Only\", \"Collide Actor Only\" or \"Both Actors\""

#define TIP_TIP_PHYSICALRESPONSE_MASS			"\
Select \"Use Calculated\" to use internally calculated mass\n\
Select \"Specify\" to manually enter actor mass\n\
\n\
     Event Actor: Enter Event Actor mass in \"Specify\" mode or Event Actor mass multiplier in \"Use Calculated\" mode\n\
   Collide Actor: Enter Collide Actor mass in \"Specify\" mode or Event Actor mass multiplier in \"Use Calculated\" mode"
//////////////////////////////////////////////////////////////////////////////

enum
{
	LS_MOVE_TYPE,
	LS_MASS_TYPE,
	LS_PATH,
	BT_CLOSE,
	BT_ADD
};

#define WIDTH	200
#define HEIGHT	215

PhysicalBounce::PhysicalBounce(Actor *actor)
	: BaseAction(PHYSICAL_BOUNCE, (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_PHYSICALRESPONSE);
	this->eventActor = actionActor = actor;

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText(PHYSICAL_BOUNCE, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("Move: ", 10, y);
	listMoveType = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_MOVE_TYPE); listMoveType->SetToolTip(TIP_TIP_PHYSICALRESPONSE_MOVE);
	
	y = DrawHLine(listMoveType->Down() + 2);

	text = AddText("Mass: ", 10, y);
	listMassType = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_MASS_TYPE);  listMassType->SetToolTip(TIP_TIP_PHYSICALRESPONSE_MASS);
	
	textEventActor =   AddText("  Event multiplier: ", 10, listMassType->Down() + 2);
	massEventActor = AddEditBox(textEventActor->Right(), textEventActor->Top(), 47); massEventActor->SetNumeric(1, 16000, .1);

	textCollideActor = AddText("Collide multiplier: ", 10, massEventActor->Down() + 2);
	massCollideActor = AddEditBox(textCollideActor->Right(), textCollideActor->Top(), 47); massCollideActor->SetNumeric(1, 16000, .1);

	y = DrawHLine(massCollideActor->Down() + 2);
	text = AddText("Final velocity multiplier: ", 10, y);

	text =   AddText("       Event Actor: ", 10, text->Down() + 2);
	eventVelocityMultiplier = AddEditBox(text->Right(), text->Top(), 47); eventVelocityMultiplier->SetNumeric(.01, 32, .01);

	text =   AddText("     Collide Actor: ", 10, eventVelocityMultiplier->Down() + 2);
	collideVelocityMultiplier = AddEditBox(text->Right(), text->Top(), 47); collideVelocityMultiplier->SetNumeric(.01, 32, .01);
	
	//Close
	y = DrawHLine(collideVelocityMultiplier->Down() + 3);
	button = AddButton(Action::getEditAction()?"Ok":"Add", (WIDTH-135)/2, y, 0, 0, BT_ADD); button->SetToolTip(TIP_ACTION_ADD); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right()+8, y, 0, 0, BT_CLOSE); button->SetToolTip(TIP_ACTION_CANCEL); SetCancelButton(button);


	//Default
	listMoveType->AddText("Event Actor Only");
	listMoveType->AddText("Collide Actor Only");
	listMoveType->AddText("Both Actors");
	listMoveType->SetItem("Event Actor Only");

	listMassType->AddText("Use Calculated");
	listMassType->AddText("Specify");
	listMassType->SetItem("Use Calculated");

	massEventActor->SetText("1.0");
	massCollideActor->SetText("1.0");

	eventVelocityMultiplier->SetText("1.0");
	collideVelocityMultiplier->SetText("1.0");

	if(!ExpressionEditor::getExpressionEditor()) UpdateEdition();
}

void PhysicalBounce::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	if(editAction->data.physicalbounce.moveType == MOVE_EVENT_ACTOR_ONLY) listMoveType->SetItem("Event Actor Only");
	else if(editAction->data.physicalbounce.moveType == MOVE_COLLIDE_ACTOR_ONLY) listMoveType->SetItem("Collide Actor Only");
	else listMoveType->SetItem("Both Actors");

	if(editAction->data.physicalbounce.massType == USE_CALCULATED_MASS)
	{
		listMassType->SetItem("Use Calculated");
		textEventActor->SetText("  Event multiplier: ");				
		textCollideActor->SetText("Collide multiplier: ");
	}
	else 
	{
		listMassType->SetItem("Specify");
		textEventActor->SetText("       Event Actor: ");				
		textCollideActor->SetText("     Collide Actor: ");
	}

	massEventActor->SetText(editAction->data.physicalbounce.massEventActor);
	massCollideActor->SetText(editAction->data.physicalbounce.massCollideActor);	

	eventVelocityMultiplier->SetText(editAction->data.physicalbounce.eventVelocityMultiplier);
	collideVelocityMultiplier->SetText(editAction->data.physicalbounce.collideVelocityMultiplier);
}

PhysicalBounce::~PhysicalBounce()
{

}

bool PhysicalBounce::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(BaseAction::OnList(list, index, text, listId)) return true;

	switch(listId)
	{
	case LS_MASS_TYPE:
		{
			if(text == 	"Use Calculated")
			{
				  textEventActor->SetText("  Event multiplier: ");				
				textCollideActor->SetText("Collide multiplier: ");
			}
			else
			{
				  textEventActor->SetText("       Event Actor: ");				
				textCollideActor->SetText("     Collide Actor: ");
			}
		}
		break;
	}

	return true;
}

void PhysicalBounce::OnButton(Button *button, int buttonId)
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
			int moveType = MOVE_BOTH_ACTORS;
			gedString moveTypeStr = "MOVE_BOTH_ACTORS";

			int massType = USE_CALCULATED_MASS;
			gedString massTypeStr = "USE_CALCULATED_MASS";

			double	massEvent = atof(massEventActor->GetText().c_str()),
					massCollide = atof(massCollideActor->GetText().c_str()),
					velocityEvent = atof(eventVelocityMultiplier->GetText().c_str()),
					velocityCollide = atof(collideVelocityMultiplier->GetText().c_str());

			if(listMoveType->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select a move type");
				return;
			}

			if(listMassType->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select a mass type");
				return;
			}

			
			
			if(listMoveType->GetText() == "Event Actor Only")
			{
				moveType = MOVE_EVENT_ACTOR_ONLY;
				moveTypeStr = "MOVE_EVENT_ACTOR_ONLY";
			}
			else if(listMoveType->GetText() == "Collide Actor Only")
			{
				moveType = MOVE_COLLIDE_ACTOR_ONLY;
				moveTypeStr = "MOVE_COLLIDE_ACTOR_ONLY";
			}			

			if(listMassType->GetText() == "Specify")
			{
				massType = USE_SPECIFIED_MASS;
				massTypeStr = "USE_SPECIFIED_MASS";
			}


			if(!ExpressionEditor::getExpressionEditor() && (massEvent <= 0.0 || massCollide <= 0.0))
			{
				if(massType == USE_SPECIFIED_MASS)
					new PanelInfo("Mass must be greater than 0.0");	
				else
					new PanelInfo("Mass multiplier must be greater than 0.0");	

				return;
			}

			if(IsEditingScript())
			{
				//Auto complete script

				char buf[256];
				sprintf(buf, "PhysicalResponse(%s, %s, %f, %f, %f, %f);",
					moveTypeStr.c_str(),
					massTypeStr.c_str(),
					massEvent,
					massCollide,
					velocityEvent,
					velocityCollide);
				InsertIntoScript(buf);
				delete this;
			}
			else
			{
				SelectActionType(eventActor, Action::Call(eventActor)->SetPhysicalBounce(
					moveType, massType, massEvent, massCollide,
					velocityEvent, velocityCollide));
			}			
		}
		break;
	}
}



#endif //#ifndef STAND_ALONE_GAME