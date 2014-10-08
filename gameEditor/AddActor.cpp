/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com, http://game-editor.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

If GPL v3 does not work for you, we offer a "non-GPL" license 
More info at http://game-editor.com/License
Be a Game Editor developer: Be a Game Editor developer: <http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>  .

***************************************************************************/


// AddActor.cpp: implementation of the AddActor class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "AddActor.h"
#include "GameControl.h"
#include "ActorEdit.h"
#include "ActorProperty.h"
#include "Tutorial.h"
#include "AddActionDialog.h"


//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_ADDACTOR			"\
Actors are your objects, the characters of your game.\n\
Each actor can have a group of animations linked to it,\n\
can be parented by another actors, follow a path,\n\
be transparent, be a text, among others.\n\
\n\
Right click on an actor to open the actor menu:\n\
   Actor Control: make your actors live\n\
   New Activation Event: send Activation Events to other actors\n\
   Show/Hide Activation Events\n\
   Lock Actor: Lock actors to avoid accidental moves in editor\n\
\n\
Activation Events are events send to other actors when a specified event happens.\n\
Imagine a game which some actors will be move when the main actor is at a certain place.\n\
That can be made easily with Activation Events.\n\
\n\
Create a Activation Event by right click in actor, select \"New Activation Event\" and define\n\
an event which will generate the Activation Event."

#define TIP_ADDACTOR_TYPE			"\
Actor Type:\n\
\n\
Normal: An actor that can receive animations\n\
\n\
Canvas: Draw on this actor using the Drawing Functions\n\
     Are shown with cyan rectangle, which don't showed\n\
     in the game mode, and can moved or resized freely.\n\
\n\
Wire Frame Region: Are helper actors which can be used\n\
     like sensors to make actions like \"Open the door when\n\
     player is into Actor Region\" and don't catch clicks on it.\n\
     Are shown with green rectangle, which don't showed\n\
     in the game mode, and can moved or resized freely.\n\
\n\
Filled Region: Are helper actors which can be used\n\
     like sensors to catch mouse clicks.\n\
     Are shown with blue rectangle, which don't showed\n\
     in the game mode, and can moved or resized freely."
//////////////////////////////////////////////////////////////////////////////


enum
{
	BT_ADD,
	BT_CLOSE,
	LS_TYPE
};

#define WIDTH	220
#define HEIGHT	100


AddActor::AddActor(bool _bCallAddAnimation)
	: Panel("AddActor", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_ADDACTOR);

	bCallAddAnimation = _bCallAddAnimation;
	

	Text *text;
	Button *button;	
	ListPop *list;
	int y;

	text = AddText("Add Actor", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	text = AddText("Name: ", 10, text->Down() + 8);
	name = AddEditBox(text->Right() + 1, text->Top(), 128);

	text = AddText("Type: ", 10, name->Down() + 2);
	list = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_TYPE); list->SetToolTip(TIP_ADDACTOR_TYPE);


	list->AddText("Normal");
	list->AddText("Canvas");
	list->AddText("Wire Frame Region");
	list->AddText("Filled Region");

	list->SetItem("Normal");
	type = SPRITE;
	actorTypeList = list;
	

	
	//Close
	y = DrawHLine(list->Down() + 2);
	button = AddButton("Add", (WIDTH-130)/2, y, 0, 0, BT_ADD); SetConfirmButton(button);
	button = AddButton("Close", button->Right()+8, y, 0, 0, BT_CLOSE); SetCancelButton(button);
}

AddActor::~AddActor()
{

}


bool AddActor::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(text == "Normal") type = SPRITE;
	else if(text == "Canvas") type = CANVAS;
	else if(text == "Wire Frame Region") type = REGION_ACTOR;
	else if(text == "Filled Region") type = REGION_ACTOR_FILLED;

	return true;
}

void AddActor::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_ADD:
		{
			//-6 to allow a clone name .0000 in actions
			if(name->GetText().size() <= 0)
			{
				new PanelInfo("Please, enter valid actor name");
				return;
			}
			else if(name->GetText().size() > NAME_LIMIT-7)
			{
				new PanelInfo("The actor name must have less than 26 characters");
				return;
			}

			if(GameControl::Get()->GetActor(name->GetText()))
			{
				new PanelInfo("Actor name already exists\nEnter another actor name");
				return;
			}

			bool bVerifyInternalNames = true; //Solve the Parent tutorial bug
			if( name->GetText() == "parent" ||
				name->GetText() == "creator" ||
				name->GetText() == "collide" ||
				name->GetText() == "Axis" )
			{
				if(Tutorial::IsCompatible(VERSION_ESC_TO_EXIT))
				{
					new PanelInfo("Actor name already used in internal actors\nEnter another actor name");
					return;
				}
				else
				{
					bVerifyInternalNames = false;
				}
			}

			if( name->GetText() == "Axis")
			{

			}
			int res;
			if((res = isValidVarName(name->GetText().c_str())) != VAR_NAME_OK && bVerifyInternalNames)
			{
				new PanelInfo("Actor names must begin with characters\nand must be followed with characters, numbers or '_'.\nScript functions and vars names are not allowed.");	
				return;
			}
			
			
			

			ActorEdit *actor = new ActorEdit(name->GetText(), NULL, false, type, 200, 200);
			actor->CenterOnScreen();
			
			//Notify actor dialog
			ActorProperty::Update(actor);	
			

			if(bCallAddAnimation && type == SPRITE)
			{
				new AddActionDialog(ActorProperty::getActorProperty(), actor, NULL, true);
			}


			delete this;
			
		}	
		break;
	case BT_CLOSE:
		{
			delete this;
		}
		break;
	}
}

void AddActor::OnKeyDown(SDLKey key, int repeatLoopCount)
{
	Panel::OnKeyDown(key, repeatLoopCount);
	if(GameControl::Get()->InModal(this) && !repeatLoopCount)
	{
		if(key == SDLK_UP || key == SDLK_DOWN)
		{
			int index = (actorTypeList->GetSelectedIndex() + (key == SDLK_DOWN) - (key == SDLK_UP));
			index = (index + actorTypeList->Count()) % actorTypeList->Count();
			actorTypeList->SetItem(index);
		}
	}
}

#endif //STAND_ALONE_GAME
