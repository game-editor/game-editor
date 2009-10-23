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


// EventKeyDown.cpp: implementation of the EventKeyDown class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "EventKeyDown.h"
#include "GameControl.h"
#include "Action.h"
#include "ActorEdit.h"
#include "Tutorial.h"






enum
{
	BT_CANCEL,
	BT_CLEAR,
	LS_KEY_DOWN_MODE,
	LS_ACTION
};

#define WIDTH	250
#define HEIGHT	120

#define WIDTH_KEY_DOWN	405
#define HEIGHT_KEY_DOWN	190



DlgEventKeyDown::DlgEventKeyDown(Actor *actor, bool bUp)
	: Panel("EventKeyDown", (GameControl::Get()->Width() - ((bUp || !Tutorial::IsCompatible(VERSION_GET_ANIMINDEX))?WIDTH:WIDTH_KEY_DOWN))/2, 
				          (GameControl::Get()->Height() - ((bUp || !Tutorial::IsCompatible(VERSION_GET_ANIMINDEX))?HEIGHT:HEIGHT_KEY_DOWN))/2,
						  ((bUp || !Tutorial::IsCompatible(VERSION_GET_ANIMINDEX))?WIDTH:WIDTH_KEY_DOWN), 
						  ((bUp || !Tutorial::IsCompatible(VERSION_GET_ANIMINDEX))?HEIGHT:HEIGHT_KEY_DOWN))
{
	SetModal();
	eventActor = actor;
	this->bUp = bUp;
	mode = SDLK_UNKNOWN;

	Text *text;
	Button *button;	
	Actor *add;
	int y;
	listMode = listRepeat = NULL;
	textPanel = NULL;

	//Title
	text = AddText(bUp?"Key Up Event":"Key Down Event", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);
	

	if(bUp || !Tutorial::IsCompatible(VERSION_GET_ANIMINDEX))
	{
		//Body
		text = AddText("Press event key or\nright click for 'any' key: ", 10, y);	
		textKey = AddText("              ", 170, text->Down() - 13);
		
		y = textKey->Down() + 5;
		if(!bUp)
		{
			SetToolTip(TIP_DLG_KEYDOWN);
			
			text = AddText("Repeat: ", 10, y+2);
			listRepeat = AddListPop(text->Right(), text->Top(), 128); listRepeat->SetToolTip(TIP_DLG_KEYDOWN_REPEAT);			
			y = listRepeat->Down();
		}
		else
		{
			SetToolTip(TIP_DLG_KEYUP);
			y += 20;
		}
	}
	else
	{
		//New Key Down intreface
		SetToolTip(TIP_DLG_KEYDOWN);

		text =    AddText("Press the key or key sequence (right click for 'any' key)", 10, y);	
		text =    AddText("Keys: ", 10, text->Down() + 4);	

		textKey = AddText("                                                          \n                                                          "
			, text->Right(), text->Top() + 2);

		textPanel = new Panel("textKeyPanel", textKey->Left() - 5, textKey->Top() - 2, textKey->Width() + 5, textKey->Height() + 4,
				  this, true, false, true, false);

		//Transparent background
		KrRGBA *pixels = textPanel->getCanvasResource()->Pixels();	
		if(pixels)
		{
			KrRGBA colorBack;
			colorBack.c.alpha = 0;
			
			for(int i = 2; i < textKey->Width() + 3; i++)
			{
				for(int j = 2; j < textKey->Height() + 2; j++)
				{
					pixels[ j*textPanel->Width() + i ] = colorBack;
				}
			}
		}

		GetFocus(this);

		textKey->getImage()->SetZDepth(textPanel->getImage()->ZDepth() + 1000);

		button = AddButton("Clear", textPanel->Right() - 45, textPanel->Down() + 3, 45, 0, BT_CLEAR); button->SetToolTip(TIP_DLG_KEYDOWN_CLEAR);			

		y = DrawHLine(button->Down() + 3);		
		
		text = AddText("Execute when: ", 10, y+2);
		listMode = AddListPop(text->Right(), text->Top(), 230); listMode->SetToolTip(TIP_DLG_KEYDOWN_MODE);			

		text = AddText("      Repeat: ", 10, listMode->Down() + 2);
		listRepeat = AddListPop(text->Right(), text->Top(), 128); listRepeat->SetToolTip(TIP_DLG_KEYDOWN_REPEAT);

		listMode->AddText("At least one key is pressed");
		listMode->AddText("All keys are pressed");
		listMode->AddText("Keys are pressed in order");
		listMode->SetItem("At least one key is pressed");
		
		y = listRepeat->Down();
	}
	
	//Close
	y = DrawHLine(y + 5);
	if(Action::inNewActivationEvent())
	{
		add = AddButton("Choose Actor", (((bUp || !Tutorial::IsCompatible(VERSION_GET_ANIMINDEX))?WIDTH:WIDTH_KEY_DOWN)-135)/2 - 15, y, 0, 0, LS_ACTION); ((Button *)add)->SetToolTip(TIP_ACTIVATIONEVENT_CHOOSE);	
	}
	else if(Action::getEditAction())
	{
		add = AddButton("Edit Action", (((bUp || !Tutorial::IsCompatible(VERSION_GET_ANIMINDEX))?WIDTH:WIDTH_KEY_DOWN)-135)/2 - 12, y, 0, 0, LS_ACTION); ((Button *)add)->SetToolTip(TIP_ACTION_EDIT);
	}
	else
	{
		add = AddListPop((((bUp || !Tutorial::IsCompatible(VERSION_GET_ANIMINDEX))?WIDTH:WIDTH_KEY_DOWN)-135)/2 - 10, y, 85, 0, LS_ACTION, "Add Action"); ((ListPop *)add)->SetToolTip(TIP_ADDACTION);	
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

	if(listRepeat)
	{
		listRepeat->AddText("Enable");
		listRepeat->AddText("Disable");
		listRepeat->SetItem("Enable");
	}

	Clear();

	UpdateEdition();
}

void DlgEventKeyDown::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	

	mode = editAction->eventData.data.keyEvent.key;
	if(mode <= SDLK_ANY_KEY)
	{
		keySequence[0] = mode;
	}
	else
	{
		memcpy(keySequence, editAction->eventData.data.keyEvent.keySequence, MAX_KEY_SEQUENCE*sizeof(unsigned short));
	}

	while(nKeys < MAX_KEY_SEQUENCE && keySequence[nKeys]) nKeys++;

	UpdateKeyText();

	if(editAction->eventData.eventType == EVENT_KEYDOWN)
	{
		if(editAction->eventData.data.keyEvent.bDisableRepeat && listRepeat)
		{
			listRepeat->SetItem("Disable");
		}

		if(listMode)
		{
			switch(mode)
			{
			case SDLK_ALL_KEYS_PRESSED:
				listMode->SetItem("All keys are pressed");
				break;

			case SDLK_KEYS_PRESSED_IN_ORDER:
				listMode->SetItem("Keys are pressed in order");
				break;

			default:
				listMode->SetItem("At least one key is pressed");
				break;
			}
		}
	}
}

DlgEventKeyDown::~DlgEventKeyDown()
{
	delete textPanel;
}

void DlgEventKeyDown::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_CANCEL:
		{
			Action::clearEditAction();
			delete this;
		}
		break;

	case BT_CLEAR:
		{
			Clear();
			UpdateKeyText();
		}
		break;

	case LS_ACTION:
		{
			//Add activation event
			if(!DoAction()) return;	
			
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

void DlgEventKeyDown::Clear()
{
	nKeys = 0;
	mode = SDLK_AT_LEAST_ONE_KEY_IS_PRESSED;
	memset(keySequence, 0, MAX_KEY_SEQUENCE*sizeof(unsigned short));
}

void DlgEventKeyDown::UpdateKeyText()
{
	gedString keyText;

	for(int i = 0; i < nKeys; i++)
	{
		gedString tmp(keyText + SDL_GetKeyName((SDLKey)keySequence[i]));

		if(textKey->GetDimensionsFont(tmp).Width() >= textKey->Width())
		{
			keyText += "\n";
		}

		keyText += SDL_GetKeyName((SDLKey)keySequence[i]);		
		keyText += " ";
	}

	textKey->SetText(keyText);
}

void DlgEventKeyDown::OnKeyDown(SDLKey key, int repeatLoopCount)
{
	if(keySequence[0] == SDLK_ANY_KEY || bUp || !Tutorial::IsCompatible(VERSION_GET_ANIMINDEX))
	{
		nKeys = 0;
	}

	if(nKeys >= MAX_KEY_SEQUENCE)
	{
		new PanelInfo(gedString("You can use a sequence with no more than ") + gedString((long)MAX_KEY_SEQUENCE) + " keys\nPlease, click on Clear button and enter a new key sequence");
		return;
	}

	keySequence[nKeys++] = key;
	
	UpdateKeyText();
}

bool DlgEventKeyDown::OnMouseButtonDown(int x, int y, Uint8 button)
{
	if(button == SDL_BUTTON_RIGHT) 
	{
		keySequence[0] = SDLK_ANY_KEY;
		nKeys = 1;

		textKey->SetText(SDL_GetKeyName(SDLK_ANY_KEY));
	}
	
	return Panel::OnMouseButtonDown(x, y, button);
}

bool DlgEventKeyDown::OnList(ListPop *list, int index, gedString &text, int listId)
{
	switch(listId)
	{
	case LS_ACTION:
		{
			if(!DoAction()) return true;

			Action::DoAction(eventActor, text);	
		}
		break;
	}

	return true;
}

bool DlgEventKeyDown::DoAction()
{
	if(mode == SDLK_UNKNOWN || !nKeys)
	{
		new PanelInfo("Please, press a key before add an action");
		return false;
	}	
	
	
	if(bUp) Action::EventKeyUp((SDLKey)keySequence[0]);
	else
	{
		bool bDisableRepeat = false;
		if(listRepeat && listRepeat->GetText() == "Disable") bDisableRepeat = true;

		if(nKeys == 1)
		{
			mode = (SDLKey)keySequence[0];
		}
		else
		{
			mode = SDLK_AT_LEAST_ONE_KEY_IS_PRESSED;

			if(listMode)
			{
				if(listMode->GetText() == "All keys are pressed") mode = SDLK_ALL_KEYS_PRESSED;
				else if(listMode->GetText() == "Keys are pressed in order") mode = SDLK_KEYS_PRESSED_IN_ORDER;
			}
		}
		
		Action::EventKeyDown(mode, bDisableRepeat, keySequence);
	}

	return true;
}


#endif //#ifndef STAND_ALONE_GAME