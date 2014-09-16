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


// ChangeResolution.cpp: implementation of the ChangeResolution class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "ChangeResolution.h"
#include "GameControl.h"
#include "ExpressionEditor.h"
#include "Tutorial.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_CHANGERESOLUTION			"\
Use this action to change the resolution settings"

#define TIP_CHANGERESOLUTION_FULLSCREEN			"\
Choose yes to set the game window to use fullscreen"

//////////////////////////////////////////////////////////////////////////////

enum
{
  LS_FULLSCREEN,
  BT_CLOSE,
  BT_ADD
};

#define WIDTH	200
#define HEIGHT	120

ChangeResolution::ChangeResolution(Actor *actor, bool bOnCollision)
  : BaseAction("Change Resolution", (GameControl::Get()->Width() - WIDTH)/2, 
	       (GameControl::Get()->Height() - HEIGHT)/2,
	       WIDTH, HEIGHT)
{
  SetModal();
  SetToolTip(TIP_CHANGERESOLUTION);
  this->eventActor = actionActor = actor;

  Text *text;
  Button *button;	
  int y;

  //Title
  text = AddText(CHANGE_RESOLUTION, CENTER_TEXT, 5);
  y = DrawHLine(text->Down() + 2);

  text = AddText("X Resolution: ", 10, y);
  posX = AddEditBox(text->Right() + 2, text->Top(), 80); posX->SetNumeric(1, 650000, 1);

  text = AddText("Y Resolution: ", 10, posX->Down() + 2);
  posY = AddEditBox(text->Right() + 2, text->Top(), 80); posY->SetNumeric(1, 650000, 1);

  text = AddText("Fullscreen: ", 10, posY->Down() + 2);
  listFullscreen = AddListPop(text->Right() + 2, text->Top(), 40, 0, LS_FULLSCREEN); listFullscreen->SetToolTip(TIP_CHANGERESOLUTION_FULLSCREEN);

	
  //Close
  y = DrawHLine(listFullscreen->Down() + 2);
  button = AddButton(Action::getEditAction()?"Ok":"Add", (WIDTH-135)/2, y, 0, 0, BT_ADD); button->SetToolTip(TIP_ACTION_ADD); SetConfirmButton(button);
  button = AddButton("Cancel", button->Right()+8, y, 0, 0, BT_CLOSE); button->SetToolTip(TIP_ACTION_CANCEL); SetCancelButton(button);

  posX->SetText(GameControl::Get()->getGameWidth());
  posY->SetText(GameControl::Get()->getGameHeight());

  listFullscreen->AddText("Yes");
  listFullscreen->AddText("No");
  listFullscreen->SetText(GameControl::Get()->getFullScreen() ? "Yes" : "No");
  //if(!ExpressionEditor::getExpressionEditor()) UpdateEdition();
}

//void ChangeResolution::UpdateEdition()
//{
//  stAction *editAction = Action::getEditAction();
//  if(!editAction) return;
//
//  Actor *actor = GameControl::Get()->GetActor(editAction->data.createActor.name);
//	
//  if(actor == EVENT_ACTOR) actor = eventActor;
//  else if(actor == PARENT_ACTOR && eventActor->getParent() && eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
//
//  if(actor > COLLIDE_ACTOR)
//  {
//    GameControl::Get()->PopulateAnimations(actor, listAnimation);
//  }
//
//
//  listActor->SetItem(editAction->data.createActor.name);
//  listAnimation->SetItem(editAction->data.createActor.animationName);
//  if(listParent->SetItem(editAction->data.createActor.parentName) == -1)
//  {
//    listParent->SetItem(NO_PARENT);
//  }
//
//  if(listPath->SetItem(editAction->data.createActor.pathName) == -1)
//  {
//    listPath->SetItem(NO_PATH);
//  }
//
//  if(editAction->bAbsolutePosition) listRelativePos->SetItem("No");
//  else listRelativePos->SetItem("Yes");
//
//  posX->SetText(editAction->x);
//  posY->SetText(editAction->y);
//}

ChangeResolution::~ChangeResolution()
{

}

bool ChangeResolution::OnList(ListPop *list, int index, gedString &text, int listId)
{
//if(BaseAction::OnList(list, index, text, listId)) return true;
//
//switch(listId)
//{
//  case LS_ACTOR:
//  {
//    actionActor = GameControl::Get()->GetActor(text);
//			
//    if(actionActor == EVENT_ACTOR) actionActor = eventActor;
//    else if(actionActor == PARENT_ACTOR && eventActor->getParent() && eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
//			
//    if(actionActor > COLLIDE_ACTOR)
//    {
//	GameControl::Get()->PopulateAnimations(actionActor, listAnimation);
//    }	
//  }
//  break;
//}
//
  return true;
}

void ChangeResolution::OnButton(Button *button, int buttonId)
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
      int x = atol(posX->GetText().c_str()), y = atol(posY->GetText().c_str());			
      bool bFullscreen = true;
      if(listFullscreen->GetText() == "Yes") bFullscreen = false;

      if(IsEditingScript())
      {
	//Auto complete script

	char buf[256];
	sprintf(buf, "ChangeResolution(%ld, %ld, %s);",	x, y, bFullscreen?"true":"false");
	InsertIntoScript(buf);
	delete this;
      }
      else
      {
	//skyde SelectActionType(eventActor, Action::Call(eventActor)->SetChangeResolution(x, y, bFullscreen));
      }			
    }
    break;
  }
}

#endif //#ifndef STAND_ALONE_GAME
