/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues

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
<http://game-editor.com/License>, thats will help us to pay our developers 
<http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>.

***************************************************************************/


// ActorProperty.cpp: implementation of the ActorProperty class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "ActorProperty.h"
#include "ActorEdit.h"
#include "GameControl.h"
#include "gui.h"
#include "AddActionDialog.h"
#include "AddSequenceDialog.h"
#include "Path.h"
#include "ActorText.h"
#include "SetTile.h"
#include "SetTileSingle.h"
#include "EditSequenceDialog.h"
#include "Tutorial.h"
#include "PathPoint.h"
#include "Path.h"
#include "PathDialog.h"
#include "ExpressionEditor.h"
#include "Config.h"
#include "../gameEngine/ActivationEventsCanvas.h"

#ifdef wxUSE_GUI
#include "../wxGameEditor/PanelProperty.h"
#include "../wxGameEditor/PanelActors.h"
#endif

/*#if defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL)
#include "AdsShow.h"
#endif*/





enum
{
	BT_DUMMY,
	BT_ADD,
	//BT_CLOSE,
	BT_ADDACTION,
	BT_ADDSEQUENCE,
	BT_EDITSEQUENCE,
	BT_REMACTION,
	BT_CLONE,
	BT_REMACTOR,
	BT_TEXT,
	LS_PARENT,
	SL_TRANS,
	SL_ZDEPTH,
	LS_EVENT,
	LS_EDITEVENT,
	LS_EDITACTIONS,
	LS_REMOVEEVENT,
	LS_ACTION,
	LS_PATH,
	LS_EDITANIM,
	LS_CREATE,
	LS_NETACTOR,
	LS_NETOPTIMIZE,
	LS_INFINITE,
	LS_TILE,
	LS_ACTOR,
	LS_CLONE,
	LS_OUT_OF_VISION_OPTIMIZATION,
	LS_INHERIT_EVENTS_FROM
};

ActorProperty *ActorProperty::actorProperty = NULL;

#define WIDTH	472
#define HEIGHT	238
#define HEIGHT_BANNER	(HEIGHT + 62)

int actorPropertyX = 0, actorPropertyY = 0;
static int getActorPropertyX()
{
	if(!Tutorial::IsOff() || !actorPropertyX) 
	{
		actorPropertyX = (GameControl::Get()->Width() - WIDTH)/2;
	}

	return actorPropertyX;
}

static int getActorPropertyHeight()
{
	int height = HEIGHT;
	if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION)) height += 46;

	if(Tutorial::IsCompatible(VERSION_NETWORK) && GAME_EDITOR_VERSION >= VERSION_NETWORK) height += 32;

	return height;
}

static int getActorPropertyY()
{
	if(!Tutorial::IsOff() || !actorPropertyY) 
	{
		actorPropertyY = (GameControl::Get()->Height() - getActorPropertyHeight())/2;
	}

	return actorPropertyY;
}



ActorProperty::ActorProperty()
: Panel("ActorProperty", getActorPropertyX(), 
/*#if defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL)
		(GameControl::Get()->Height() - (AdsControl::Get()->HaveBanner()?HEIGHT_BANNER:HEIGHT))/2,
						  WIDTH, AdsControl::Get()->HaveBanner()?HEIGHT_BANNER:HEIGHT)
#else*/
		getActorPropertyY(), 
		WIDTH, 
		getActorPropertyHeight())
//#endif
{
	actorProperty = this;
	bRemove = false;
	SetToolTip(TIP_ACTORCONTROL);

/*#if defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL)
	adsShow = NULL;
#endif*/

	Text *text;
	Button *button, *buttonRemove, *buttonText;	
	Actor *cloneControl;
	ListPop *list, *listTile;
	int y, dw = Tutorial::IsCompatible(VERSION_CLONE_ARRAY)?5:0;

	listOutOfVisionOptimization = NULL;
	listInheritEventsFrom = NULL;
	listNetActor = NULL;
	listNetOptimize = NULL;

	text = AddText("Actor Control", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	text = AddText("Name: ", 10, y);
	listActor = AddListPop(text->Right(), text->Top(), 226, 0, LS_ACTOR); listActor->SetToolTip(TIP_ACTORCONTROL_NAME);

	if(Tutorial::IsCompatible(VERSION_CLONE_ARRAY))
	{
		cloneControl = AddListPop(listActor->Right() + 5, listActor->Top(), 60, 0, LS_CLONE, "Clone"); ((ListPop*)cloneControl)->SetToolTip(TIP_ACTORCONTROL_CLONE);

		((ListPop*)cloneControl)->AddText("Single");
		((ListPop*)cloneControl)->AddText("Array");
		((ListPop*)cloneControl)->AddText("Along Path");
	}
	else
	{
		//For old tutorials compatibility, only
		cloneControl = AddButton("Clone", listActor->Right() + 5, listActor->Top(), 0, 0, BT_CLONE); ((Button*)cloneControl)->SetToolTip(TIP_ACTORCONTROL_CLONE);
	}

	button = buttonRemove = AddButton("Remove", cloneControl->Right() + 1, listActor->Top(), cloneControl->Width(), 0, BT_REMACTOR); buttonRemove->SetToolTip(TIP_ACTORCONTROL_REMOVE);
	buttonText = AddButton(" Text ", buttonRemove->Left(), buttonRemove->Down(), cloneControl->Width(), 0, BT_TEXT); buttonText->SetToolTip(TIP_ACTORCONTROL_TEXT);
	listTile = AddListPop(cloneControl->Left(), buttonText->Top(), cloneControl->Width(), 0, LS_TILE, "Tile"); listTile->SetToolTip(TIP_ACTORCONTROL_TILE);
	listAnim = AddListPop(buttonRemove->Right() + 2, buttonRemove->Top(), cloneControl->Width() + dw, 0, LS_EDITANIM); listAnim->SetToolTip(TIP_ACTORCONTROL_MOVE);
	listInfinite = AddListPop(listAnim->Left(), listAnim->Down(), cloneControl->Width() + dw, 0, LS_INFINITE); listInfinite->SetToolTip(TIP_ACTORCONTROL_INFINITE);
	
	text = AddText("Transparency:  ", 10, button->Down() + 2);
	slideTrans = AddSlide(text->Right(), text->Top(), 100, 14, SL_TRANS); slideTrans->SetToolTip(TIP_ACTORCONTROL_TRANSPARENCY);

	text = AddText("     Z Depth:  ", 10, slideTrans->Down() + 2);
	slideZDepth = AddSlide(text->Right(), text->Top(), 100, 14, SL_ZDEPTH); slideZDepth->SetToolTip(TIP_ACTORCONTROL_ZDEPTH);

	text = AddText("Create at startup: ", listTile->Left() + 10, listTile->Down()+2);
	listCreate = AddListPop(text->Right(), text->Top(), buttonRemove->Width() + 2, 0, LS_CREATE); listCreate->SetToolTip(TIP_ACTORCONTROL_CREATE);
	
	y = listCreate->Down() + 2;
	

	if(Tutorial::IsCompatible(VERSION_NETWORK) && GAME_EDITOR_VERSION >= VERSION_NETWORK)
	{
		y = DrawHLine(y);

		text = AddText("Net Actor: ", 10, y);
		listNetActor = AddListPop(text->Right() + 3, text->Top(), buttonRemove->Width() + 2, 0, LS_NETACTOR); listNetActor->SetToolTip(TIP_ACTORCONTROL_NETACTOR);

		text = AddText("Optimized for: ", listNetActor->Right() + 20, listNetActor->Top());
		listNetOptimize = AddListPop(text->Right(), text->Top(), 82, 0, LS_NETOPTIMIZE); listNetOptimize->SetToolTip(TIP_ACTORCONTROL_NETOPTIMIZE);

		y = listNetOptimize->Down() + 3;
	}

	y = DrawHLine(y);

	//Action
	text = AddText("Animation: ", 10, y);
	listAction = AddListPop(text->Right() + 2, text->Top(), 258, 0, LS_ACTION); listAction->SetToolTip(TIP_ACTORCONTROL_ANIMATION_LIST);
	button = AddButton("Add Animation", text->Left(), listAction->Down() + 2, 0, 0, BT_ADDACTION); button->SetToolTip(TIP_ACTORCONTROL_ANIMATION_ADD);
	button = AddButton("Add Sequence", button->Right()+2, button->Top(), 0, 0, BT_ADDSEQUENCE); button->SetToolTip(TIP_ACTORCONTROL_ANIMATION_SEQUENCE);
	button = AddButton("Edit", button->Right()+2, button->Top(), 0, 0, BT_EDITSEQUENCE); button->SetToolTip(TIP_ACTORCONTROL_ANIMATION_EDIT);
	button = AddButton("Remove", button->Right()+2, button->Top(), 0, 0, BT_REMACTION); button->SetToolTip(TIP_ACTORCONTROL_ANIMATION_REMOVE);
	y = DrawHLine(button->Down() + 2);

	//Parent
	text =   AddText("Parent: ", 10, y);
	listParent = AddListPop(text->Right() + 2, y, 225, 0, LS_PARENT); listParent->SetToolTip(TIP_ACTORCONTROL_PARENT);
	
	//Path
	text =   AddText("  Path: ", listParent->Right(), listParent->Top());
	listPath = AddListPop(text->Right() + 2, y, 128, 0, LS_PATH); listPath->SetToolTip(TIP_ACTORCONTROL_PATH);
	y = DrawHLine(listPath->Down() + 2);

	//Events
	text =   AddText("Events: ", 10, y);
	list = AddListPop(text->Right(), text->Top(), 70, 0, LS_EVENT, "Add"); list->SetToolTip(TIP_ACTORCONTROL_EVENTS_ADD);
	Action::PopulateEvents(list);	

	listEditEvents = AddListPop(list->Right() + 4, list->Top(), 70, 0, LS_EDITEVENT, "Edit"); listEditEvents->SetToolTip(TIP_ACTORCONTROL_EVENTS_EDIT);
	listRemoveEvents = AddListPop(listEditEvents->Right() + 4, listEditEvents->Top(), 70, 0, LS_REMOVEEVENT, "Remove"); listRemoveEvents->SetToolTip(TIP_ACTORCONTROL_EVENTS_REMOVE);
	listEditActions = AddListPop(list->Right() + 4, list->Top(), 1, 1, LS_EDITACTIONS);
	listEditActions->getImage()->SetVisible(false);

	if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		y = list->Down() + 5;

		if(1)
		{
			text =   AddText("Inherit events from: ", 10, y);
			listInheritEventsFrom = AddListPop(text->Right() - 4, text->Top(), 144, 0, LS_INHERIT_EVENTS_FROM); listInheritEventsFrom->SetToolTip(TIP_INHERIT_EVENTS_FROM);
			
			y = listInheritEventsFrom->Down() + 5;
		}



		text =   AddText("Receive events even if out of vision: ", 10, y);
		listOutOfVisionOptimization = AddListPop(text->Right(), text->Top(), 38, 0, LS_OUT_OF_VISION_OPTIMIZATION); listOutOfVisionOptimization->SetToolTip(TIP_OUT_OF_VISION_OPTIMIZATION);

		listOutOfVisionOptimization->AddText("Yes");
		listOutOfVisionOptimization->AddText("No");
		

		
		

		y = text->Down() + 10;
		
	}
	else
	{
		y = list->Down() + 5;
	}
	
	
	//Ads
	y = DrawHLine(y);
/*#if defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL)
	if(AdsControl::Get()->HaveBanner())
	{
		adsShow = new AdsShow(this, 236, y + 26);		
		
		y = DrawHLine(y + 56);
	}
#endif*/

	//Close
	button = AddButton("Close", (WIDTH - 60)/2, y - 2, 0, 0, BT_CLOSE);

	listAnim->AddText("Move");
	listAnim->AddText("Stop");
	listAnim->SetItem("Stop");

	listCreate->AddText("Yes");
	listCreate->AddText("No");

	listInfinite->AddText("Infinite");
	listInfinite->AddText("x Infinite");
	listInfinite->AddText("y Infinite");
	listInfinite->AddText("Normal");
	listInfinite->SetItem("Normal");

	listTile->AddText("Single tile");
	listTile->AddText("Tile Matching");

	if(listNetActor)
	{
		listNetActor->AddText("Yes");
		listNetActor->AddText("No");
	}

	if(listNetOptimize)
	{
		listNetOptimize->AddText("Speed");
		listNetOptimize->AddText("Precision");
	}

	actor = NULL;
	SetPaths();
	SetParent();	
}

ActorProperty::~ActorProperty()
{
/*#if defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL)
	delete adsShow;
#endif*/

	actorProperty = NULL;

	actorPropertyX = getImage()->X();
	actorPropertyY = getImage()->Y();
}

void ActorProperty::Call(Actor *actor, bool bCreate)
{
	if(!actorProperty && bCreate)
	{
		new ActorProperty();
	}

	Update(actor);
}

void ActorProperty::Update()
{
	if(actorProperty)
	{
		if(IS_VALID_ACTOR(actorProperty->actor))
		{
			actorProperty->Update(actorProperty->actor);
		}
		else
		{
			//Select view
			actorProperty->Update(GameControl::Get()->GetActor("view"));
		}
	}
}

void ActorProperty::Update(Actor *actor)
{
	if(actorProperty)
	{
		actorProperty->SetParent();
		actorProperty->SetActor(actor);
	}

#ifdef wxUSE_GUI
	PanelProperty::Update(actor);
	PanelActors::Update(actor);
#endif
}

void ActorProperty::SetActor(Actor *actor)
{
	GameControl::Get()->PopulateActors(listActor);

	if(!actor)
	{
		actor = GameControl::Get()->GetActor(listActor->GetText(0));
	}

	this->actor = actor;

	//Name
	if(mapActors.FindString(actor->getActorName())->Count() > 1)
	{
		listActor->SetItem(actor->getCloneName()/*actor->getCloneIndex()?actor->getCloneName():actor->getActorName()*/);
		if(listActor->GetSelectedIndex() == -1)
		{
			//Try only name
			listActor->SetItem(actor->getActorName());
		}
	}
	else
	{
		listActor->SetItem(actor->getActorName());
	}

	//Parent name
	if(actor->getParent() != GameControl::Get()->GetAxis())
	{
		if(mapActors.FindString(actor->getParent()->getActorName())->Count() > 1)
		{
			int i = listParent->SetItem(actor->getParent()->getCloneName()); //solve the issue 119
			if(i == -1) listParent->SetItem(actor->getParent()->getActorName());
		}
		else
		{
			listParent->SetItem(actor->getParent()->getActorName());
		}
	}
	else
	{
		listParent->SetItem(NO_PARENT);
	}

	//Slide
	slideTrans->SetPos((255 - actor->getImage()->CTransform().Alpha())/255.0);
	slideZDepth->SetPos(actor->getImage()->ZDepth()/(double)MAX_ACTOR_DEPTH);

	//Actions
	GameControl::Get()->PopulateAnimations(actor, listAction);	
	
	//Path
	if(actor->getPathX())
	{
		listPath->SetItem(actor->getPathX()->getName());
	}
	else if(actor->getPathY())
	{
		listPath->SetItem(actor->getPathY()->getName());
	}
	else
	{
		listPath->SetItem(-1);
	}

	//Move
	if(actor->editAnim())
	{
		listAnim->SetItem("Move");
	}
	else
	{
		listAnim->SetItem("Stop");
	}

	//Create
	if(actor->getCreateAtStartup())
	{
		listCreate->SetItem("Yes");
	}
	else
	{
		listCreate->SetItem("No");
	}

	//Net Actor
	if(listNetActor)
	{
		if(actor->getNetOptimizedPrecision())
		{
			listNetOptimize->SetItem("Precision");
		}
		else
		{
			listNetOptimize->SetItem("Speed");
		}

		if(actor->getNetActor())
		{
			listNetActor->SetItem("Yes");
			listNetOptimize->Enable();
		}
		else
		{
			listNetActor->SetItem("No");
			listNetOptimize->Disable();
		}
	}

	//Infinite
	if(actor->getInfinite() == BOTH_INFINITE)
	{
		listInfinite->SetItem("Infinite");
	}
	else if(actor->getInfinite() == X_INFINITE)
	{
		listInfinite->SetItem("x Infinite");
	}
	else if(actor->getInfinite() == Y_INFINITE)
	{
		listInfinite->SetItem("y Infinite");
	}
	else
	{
		listInfinite->SetItem("Normal");
	}

	//Out of Vision Optimization
	if(listOutOfVisionOptimization)
	{
		if(actor->getOutOfVisionOptimization())
		{
			listOutOfVisionOptimization->SetItem("No");
		}
		else
		{
			listOutOfVisionOptimization->SetItem("Yes");
		}
	}

	if(listInheritEventsFrom)
	{
		GameControl::Get()->PopulateActors(listInheritEventsFrom, false);
		listInheritEventsFrom->AddText("No Inheritance");

		if(actor->getAction() && !actor->getAction()->getInheritedEventsActorName().empty())
		{
			listInheritEventsFrom->SetItem(actor->getAction()->getInheritedEventsActorName());
		}
		else
		{
			listInheritEventsFrom->SetItem("No Inheritance");
		}
	}

	//Edit Events
	SetEditEvents();

	SetParent();
}

bool ActorProperty::GiveInheritedEvents(Actor *testActor)
{
	if(mapActors.FindString(testActor->getActorName())->Count() == 1)
	{
		//Don't have clones
		//Test actions

		return Action::GiveInheritedEvents(testActor->getActorName());
	}

	return false;
}

void ActorProperty::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_CLOSE:
		{
			Destroy();
			
			//Show first close reminder
			if(Config::Get()->getShowActorPropertyReminder())
			{
				new PanelInfo("Right mouse click on any actor will show the actor menu.\nYou can select the \"Actor Control\" option to open the Actor Control Panel again", "Info", ALIGN_LEFT);
				Config::Get()->setShowActorPropertyReminder(false);
			}
			
			Config::Destroy();		
			
		}
		break;
	case BT_CLONE:
		{
			//For old tutorials compatibility, only
			if(!actor->isView() && actor->getType() != REGION_ACTOR && actor->getType() != REGION_ACTOR_FILLED) //Can clone CANVAS
			{
				Update(actor->NewActor(true));
			}
			else
			{
				new PanelInfo("Invalid option for this actor");
				return;
			}
		}
		break;
	case BT_ADDACTION:
		{
			if(actor->getTextActor())
			{
				new PanelInfo("You can't add animations in text actors");
				return;
			}

			if(!actor->isView() && actor->getType() != REGION_ACTOR && actor->getType() != REGION_ACTOR_FILLED && actor->getType() != CANVAS) new AddActionDialog(this, actor);
			else
			{
				new PanelInfo("Invalid option for this actor");
				return;
			}
		}
		break;
	case BT_ADDSEQUENCE:
		{
			if(actor->getTextActor())
			{
				new PanelInfo("You can't add sequences in text actors");
				return;
			}

			if(!actor->isView() && actor->getType() != REGION_ACTOR && actor->getType() != REGION_ACTOR_FILLED && actor->getType() != CANVAS) new AddSequenceDialog(this, actor);
			else
			{
				new PanelInfo("Invalid option for this actor");
				return;
			}
		}
		break;
	case BT_EDITSEQUENCE:
		{
			if(!actor->isView() && actor->getType() != REGION_ACTOR && actor->getType() != REGION_ACTOR_FILLED && actor->getType() != CANVAS)
			{
				if(listAction->GetSelectedIndex() != -1)
				{
					Sequence *seq = actor->GetSequence(listAction->GetText());
					if(!seq)
					{
						new PanelInfo("Please, select a valid animation");
						return;
					}

					if(seq->getType() == ANIMATION_SEQUENCE)
					{
						//Replace animation file
						new AddActionDialog(this, actor, actor->GetAnimationInfo(listAction->GetText()));
					}
					else
					{		
						new EditSequenceDialog(actor, seq);
					}
				}
			}
			else
			{
				new PanelInfo("Invalid option for this actor");
				return;
			}
		}
		break;
	case BT_REMACTION:
		{
			if(!actor->isView() && actor->getType() != REGION_ACTOR && actor->getType() != REGION_ACTOR_FILLED && actor->getType() != CANVAS)
			{
				if(listAction->GetSelectedIndex() != -1)
				{
					gedString error("Remove animation?");
					//Check if this animation is used by tiles
					bool bHasTile = false;
					if(actor->getTile() && actor->getTile()->HasAnimation(listAction->GetText()))
					{
						bHasTile = true;
						error = "Some tiles in this actor use this animation\nDo you want remove the animation and the tiles?";
					}

					PanelQuestion *panel = new PanelQuestion(error);
					if(panel->Wait() == OK_BUTTON)
					{
						gedString removedAnimation(listAction->GetText());
						
						int next = (listAction->GetSelectedIndex() + 1) % listAction->Count();

						actor->RemoveAnimation(removedAnimation);
						listAction->RemoveText(removedAnimation);
						
						
						gedString animationName;
						if(listAction->Count())
						{
							//Select next action
							next = next % listAction->Count();
							animationName = listAction->GetText(next);
							listAction->SetItem(listAction->GetText(next));
						}
						else
						{
							animationName = ACTOR_GED_ICON;
							listAction->SetItem(-1);
						}
						
						//Set all clones
						ListActor *listActor = mapActors[actor->getActorName()];
						if(listActor)
						{
							for(int il = 0; listActor && il < listActor->Count(); il++)
							{
								Actor *clone = (*listActor)[il];
								if(clone->GetAnimation() == removedAnimation)
								{
									clone->SetAnimation(animationName);
								}

								if(bHasTile && clone->getTile())
								{
									clone->RemoveTileAnimation(removedAnimation);
									if(clone->getTile() && clone->getTile()->NumTiles())
									{
										//Don't show the icon if has tiles
										clone->SetAnimation(ACTOR_GED_TEXT);
									}
								}
							}
						}						
					}
					
					delete panel;				
				}
			}
			else
			{
				new PanelInfo("Invalid option for this actor");
				return;
			}
		}
		break;
	case BT_REMACTOR:
		{
			if(!actor->isView())
			{
				gedString panelText("  Remove actor?  ");

				//Test children event actors 
				bool bHaveHeirs = false;
				if(GiveInheritedEvents(actor))
				{
					panelText = "The events in this actor are inherited by other actors.\nRemove actor anyway?";
					bHaveHeirs = true;
				}

				//Remove
				PanelQuestion *panel = new PanelQuestion(panelText);
				if(panel->Wait() == OK_BUTTON)
				{
					//Remove
					gedString actorName(actor->getActorName());
					ActivationEventsCanvas::ActorRemoved(actor->getCloneName());
					GameControl::Get()->RemoveActor(actor, true);
					delete actor;

					if(bHaveHeirs)
					{
						Action::RemoveInheritedEvents(actorName);
					}
					
					//Adjust dialog
					if(GameControl::Get()->NumActors())
					{
						//First actor
						//SetActor(NULL);					

						//Next selection
						gedString text(actorName);
						Actor *nexActor = NULL;

						//Try some clone first
						if(!GameControl::Get()->GetActor(text))
						{
							//No clones, try the next
							int next = (listActor->GetSelectedIndex() + 1) % listActor->Count();
							text = listActor->GetText(next);
							
							if(text == "view" && GameControl::Get()->NumActors() > 1)
							{
								//Only select the view if the only actor
								next = (next + 1) % listActor->Count();
								text = listActor->GetText(next);
							}
							
							
							text = listActor->GetText(next);
							if(text[0] == '+')
							{
								text = text.substr(2);
							}
						}

						nexActor = GameControl::Get()->GetActor(text);
						if(!nexActor)
						{
							nexActor = GameControl::Get()->GetActor("view");
						}

						Update(nexActor);						
					}
					else
					{
						Destroy();
					}
				}
				
				delete panel;				
			}
			else
			{
				new PanelInfo("Invalid option for this actor");
				return;
			}
		}
		break;
	case BT_TEXT:
		{
			if(actor->NumAnimations())
			{
				new PanelInfo("You can't add text to an actor with animations");
				return;
			}
			else if(!actor->isView() && actor->getType() != REGION_ACTOR && actor->getType() != REGION_ACTOR_FILLED && actor->getType() != CANVAS) new ActorText(actor);
			else
			{
				new PanelInfo("Invalid option for this actor");
				return;
			}
		}
		break;
	}
}

void ActorProperty::AddAction(const gedString &action)
{
	listAction->AddText(action);
	listAction->SetItem(action);
}

void ActorProperty::OnSlide(Slide *slide, int slideId, double pos)
{
	switch(slideId)
	{
	case SL_TRANS:
		{
			actor->SetTransparency(pos);
		}
		break;
	case SL_ZDEPTH:
		{
			actor->getImage()->SetZDepth((int)(pos*MAX_ACTOR_DEPTH));
		}
		break;
	}
}

bool ActorProperty::OnList(ListPop *list, int index, gedString &text, int listId)
{
	switch(listId)
	{
	case LS_ACTION:
		{
			if(!actor->getTile())
			{
				actor->SetAnimation(text);
			}

			//Allow selection, so the user can remove the animation tiles
			/*else
			{
				new PanelInfo("You can't select an animation if actor has tiles");
				listAction->SetItem(-1);
			}*/
		}
		break;
	case LS_PATH:
		{
			if(text != NO_PATH)
				actor->SetPath(text, BOTH_AXIS);	
			else
				actor->SetPath("", BOTH_AXIS);
		}
		break;
	case LS_PARENT:
		{			
			int res;

			if(actor->getCreateAtStartup())
			{
				if((res = actor->SetParent(text)) != PARENT_OK)
				{
					if(res != PARENT_NOT_FOUND)
					{
						new PanelInfo("Actors can't be parented their own descendants");
						actor->SetParent(NO_PARENT);
						listParent->SetItem(NO_PARENT);					
					}
				}
			}
			else
			{
				new PanelInfo(
"This actor have the \"Create at startup\" option set to \"No\"\n\
and will have the parent set in a Create Actor action.\n\
If you need set the parent now, please set the \"Create at startup\" option to \"Yes\"");
				listParent->SetItem(NO_PARENT);
			}
			
		}
		break;
	case LS_EVENT:
		{
			if( (actor->isView() &&
				(text == EV_COLLISION ||
				 text == EV_COLLISION_FINISH ||
				 text == EV_OUT_OF_VISION)))
			{
				new PanelInfo("Invalid option for this actor");
				return true;
			}

			if( (actor->getTextActor() &&
				(text == EV_COLLISION ||
				 text == EV_COLLISION_FINISH))
				)
			{
				new PanelInfo("Actor Text can't get collision events");
				return true;
			}

			Action::DoEvent(actor, text, false);			
		}
		break;
	case LS_EDITEVENT:
		{
			//Show actions for choosed event
			bRemove = false;
			listEditActions->RemoveAll();
			if(actor->getAction())
			{
				actor->getAction()->PopulateEventActions(listEditActions, text);

				if(listEditActions->Count())
				{
					listEditActions->getImage()->SetVisible(true);
					PostMessage(this, 0);
				}
				else
				{
					new PanelInfo("This event only generates Activation Events.\nTo remove an Activation Events click on your activation line");
					return true;
				}
			}
		}
		break;
	case LS_REMOVEEVENT:
		{
			//Show actions for choosed event
			bRemove = true;
			listEditActions->RemoveAll();
			if(actor->getAction())
			{
				actor->getAction()->PopulateEventActions(listEditActions, text);

				if(listEditActions->Count())
				{
					listEditActions->getImage()->SetVisible(true);
					PostMessage(this, 0);
				}
				else
				{
					new PanelInfo("This event only generates Activation Events.\nTo remove an Activation Events click on your activation line");
					return true;
				}				
			}
		}
		break;
	case LS_EDITACTIONS:
		{
			//Show action to edit
			listEditActions->getImage()->SetVisible(false);
			if(bRemove)
			{
				if(actor->getAction())
				{
					PanelQuestion *panel = new PanelQuestion("Remove action?");
					if(panel->Wait() == OK_BUTTON)
					{
						stAction *action = (stAction *)list->GetItemData(index);
						if(action->actionType == SET_CONDITIONAL_ACTION) actor->getAction()->DeleteConditionalAction(action);
						else 
						{
							actor->getAction()->RemoveAction(action);
							delete action;
						}	
					}
					
					delete panel;				
				}
				SetEditEvents();
			}
			else
			{
				stAction *action = (stAction *)list->GetItemData(index);

				if( !Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION) ||
					action->eventData.eventType == EVENT_ANIMATION ||
					action->eventData.eventType == EVENT_CREATE ||
					action->eventData.eventType == EVENT_DESTROY_ACTOR ||
					action->eventData.eventType == EVENT_MOUSEENTER ||
					action->eventData.eventType == EVENT_MOUSELEAVE ||
					action->eventData.eventType == EVENT_DESTROY_ACTOR ||
					action->eventData.eventType == EVENT_MOVEFINISH ||
					action->eventData.eventType == EVENT_OUT_OF_VISION)
				{
					//On old tutorials and general events
					Action::ShowActionDialog(action, actor);	
				}
				else
				{					
					Action::ShowEventDialog(action, actor);
				}
				
			}
		}
		break;
	case LS_EDITANIM:
		{
			if(text == "Move") actor->setEditAnim(true);		
			else
			{
				actor->setEditAnim(false);	
				actor->setFramePathX(0);
				actor->setFramePathY(0);
				actor->getImage()->SetPos(actor->getPathIniX(), actor->getPathIniY());
				if(actor->getSprite()) actor->getSprite()->SetFrame(0);
			}
		}
		break;
	case LS_CREATE:
		{
			if(text == "Yes") actor->setCreateAtStartup(true);
			else 
			{
				if(actor->getParent() == GameControl::Get()->GetAxis())
				{
					actor->setCreateAtStartup(false);
				}
				else
				{
					new PanelInfo(
"To set the \"Create at startup\" option to \"No\" the actor must have no parent.\n\
If you need set this option to \"No\", remove the parent actor.\n\
You can set the parent in a Create Actor action.");
					listCreate->SetItem("Yes");
				}
			}
		}
		break;
	case LS_NETACTOR:
		{
			if(text == "Yes") 
			{
				actor->setNetActor(true);
				listNetOptimize->Enable();
			}
			else
			{
				actor->setNetActor(false);
				actor->setNetOptimizedPrecision(false);
				listNetOptimize->SetItem("Speed");
				listNetOptimize->Disable();
			}
		}
		break;
	case LS_NETOPTIMIZE:
		{
			if(text == "Precision") actor->setNetOptimizedPrecision(true);
			else actor->setNetOptimizedPrecision(false);
		}
		break;
	case LS_INFINITE:
		{
			if(!actor->isView() && actor->getType() != REGION_ACTOR && actor->getType() != REGION_ACTOR_FILLED && actor->getType() != CANVAS)
			{
				if(!actor->getTile())
				{
					if(text == "Infinite") actor->SetInfinite(BOTH_INFINITE);
					else if(text == "x Infinite") actor->SetInfinite(X_INFINITE);
					else if(text == "y Infinite") actor->SetInfinite(Y_INFINITE);
					else actor->SetInfinite(NO_INFINITE);
				}
				else
				{
					new PanelInfo("Infinite options can't be used with tiles");
					list->SetItem(3); //Normal
				}
			}
			else
			{
				new PanelInfo("Invalid option for this actor");
				list->SetItem(3); //Normal
				return true;
			}
		}
		break;
	case LS_TILE:
		{
#if !defined(GAME_EDITOR_HOME_EDITION)
			if(!actor->isView() && actor->getType() != REGION_ACTOR && actor->getType() != REGION_ACTOR_FILLED && actor->getType() != CANVAS)
			{
				if(actor->getTextActor())
				{
					new PanelInfo("Tile can't be used with text actors");
					return true;
				}

				if(actor->getInfinite() == NO_INFINITE)
				{
					if(text == "Tile Matching") SetTile::Call(actor);
					else SetTileSingle::Call(actor);			
				}
				else
				{
					new PanelInfo("Tile can't be used with infinite actors");
				}
			}
			else
			{
				new PanelInfo("Invalid option for this actor");
				return true;
			}
#else
			new PanelInfo(GAME_EDITOR_VERSION_ERROR);
#endif
		}
		break;
	case LS_ACTOR:
		{			
			Actor *actor = GameControl::Get()->GetActor(text);	
			if(actor) 
			{
				//Move view
				int x, y;
				
				if(actor->getParent() == GameControl::Get()->GetAxis())
				{
					x = actor->getImage()->X();
					y = actor->getImage()->Y();
				}
				else
				{
					//To axis coordinates
					x = actor->getImage()->CompositeXForm().x.ToInt();
					y = actor->getImage()->CompositeXForm().y.ToInt();
					
					KrVector2T< GlFixed > object;
					GameControl::Get()->GetAxis()->getImage()->ScreenToObject( x, y, &object );
					
					x = object.x.ToIntRound();
					y = object.y.ToIntRound();
				}
				
				int dx = (int)(GameControl::Get()->GetAxis()->getScreenWidth()/2.0);
				int dy = (int)(GameControl::Get()->GetAxis()->getScreenHeight()/2.0);

				x = (int)(x*GameControl::Get()->GetAxis()->getScale());
				y = (int)(y*GameControl::Get()->GetAxis()->getScale());

				x -= dx;
				y -= dy;				
				
				GameControl::Get()->GetAxis()->SetPos(-x, -y);
				Update(actor);
				listActor->SetItem(text);
			}
		}
		break;

	case LS_CLONE:
		{
#if !defined(GAME_EDITOR_HOME_EDITION)
			if(!actor->isView() /*&& actor->getType() != REGION_ACTOR && actor->getType() != REGION_ACTOR_FILLED*/) //Can clone CANVAS
			{
				if(text == "Single")
				{
					Actor *newActor = actor->NewActor(true);
					if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION)) newActor->CenterOnScreen();
					Update(newActor);					
				}
				else if(text == "Array")
				{
					new CloneArray(actor);
				}
				else
				{
					if(!GameControl::Get()->NumPaths())
					{
						new PanelInfo("Add a new path first");
					}
					else
					{
						new ClonePath(actor);
					}
				}
			}
			else
			{
				new PanelInfo("Invalid option for this actor");
			}
#else
			new PanelInfo(GAME_EDITOR_VERSION_ERROR);
#endif
		}
		break;

	case LS_OUT_OF_VISION_OPTIMIZATION:
		{
			if(listOutOfVisionOptimization)
			{
				if(!actor->isView())
				{
					if(!actor->getAction())
					{
						//Add an empty action
						Action::Call(actor);
					}
					
					if(text == "Yes") actor->setOutOfVisionOptimization(false);
					else actor->setOutOfVisionOptimization(true);					
				}
				else
				{
					listOutOfVisionOptimization->SetItem("No");			
					new PanelInfo("Invalid option for this actor");
					return true;
				}
			}
		}
		break;

	case LS_INHERIT_EVENTS_FROM:
		{
			if(listInheritEventsFrom)
			{				
				//Child
				if(!actor->getAction())
				{
					//Add an empty action
					Action::Call(actor);
				}
				
				
				if(text == "No Inheritance") actor->getAction()->setInheritedEvents("");
				else 
				{
					//Parent
					Actor *parentEvent = GameControl::Get()->GetActor(text);
					if(!parentEvent->getAction())
					{
						//Add an empty action
						Action::Call(parentEvent);
					}

					
					if(!actor->getAction()->setInheritedEvents(text))
					{
						new PanelInfo("Actors can't inherit events from your own descendants");
					}

					if(actor->getAction()->getInheritedEventsActorName().empty())
					{
						listInheritEventsFrom->SetItem("No Inheritance");
					}
					else
					{
						listInheritEventsFrom->SetItem(actor->getAction()->getInheritedEventsActorName());
					}
				}
			}
		}
		break;
	}
	
	return true;
}

void ActorProperty::SetPaths()
{
	if(actorProperty)
	{
		GameControl::Get()->PopulatePaths(actorProperty->listPath);
	}
	
#ifdef wxUSE_GUI
	PanelProperty::UpdatePaths();
#endif
}

void ActorProperty::SetParent()
{
	if(!IS_VALID_ACTOR(actor)) return;

	GameControl::Get()->PopulateActors(listParent);

	listParent->AddText(NO_PARENT);

	if(actor && actor->getParent() != GameControl::Get()->GetAxis())
	{
		if(mapActors.FindString(actor->getParent()->getActorName())->Count() > 1)
		{
			int i = listParent->SetItem(actor->getParent()->getCloneName()); //solve the issue 119
			if(i == -1) listParent->SetItem(actor->getParent()->getActorName());
		}
		else
		{
			listParent->SetItem(actor->getParent()->getActorName());
		}
	}
	else
	{
		listParent->SetItem(NO_PARENT);
	}
}

void ActorProperty::Destroy()
{	
	if(actorProperty)
	{
		delete actorProperty;
		actorProperty = NULL;
	}

#ifdef wxUSE_GUI
	PanelProperty::Update(NULL);
#endif
}

void ActorProperty::SetEditEvents()
{
	if(actorProperty)
	{
		Action::PopulateEvents(actorProperty->listEditEvents, actorProperty->actor);
		Action::PopulateEvents(actorProperty->listRemoveEvents, actorProperty->actor);
	}
}

void ActorProperty::OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message)
{
	if(messageId == 0)
	{
		listEditActions->OnMouseButtonDown(GameControl::Get()->getMouseX(), GameControl::Get()->getMouseY(), 1);
	}
}











enum
{
	LS_CLONE_DISTANCE
};

#define WIDTH_CLONE		235
#define HEIGHT_CLONE	170

CloneArray::CloneArray(Actor *actor)
	: Panel("CloneArray", (GameControl::Get()->Width() - WIDTH_CLONE)/2, 
				          (GameControl::Get()->Height() - HEIGHT_CLONE)/2,
						  WIDTH_CLONE, HEIGHT_CLONE)
{
	SetModal();
	SetToolTip(TIP_CLONEARRAY);
	clonedActor = actor;

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText("Clone array", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("       Horizontal count: ", 10, y);
	horizontalCount = AddEditBox(text->Right(), text->Top(), 47); horizontalCount->SetNumeric(1, 128, 1);

	text = AddText("         Vertical count: ", 10, horizontalCount->Down() + 2);
	verticalCount = AddEditBox(text->Right(), text->Top(), 47); verticalCount->SetNumeric(1, 128, 1);

	y = DrawHLine(verticalCount->Down() + 2);
	text = AddText("Distance between clones: ", 10, y);
	listMode = AddListPop(text->Right(), text->Top(), 72, 0, LS_CLONE_DISTANCE); listMode->SetToolTip(TIP_CLONEARRAY_DISTANCE);

	text = AddText("    Horizontal distance: ", 10, listMode->Down() + 2);
	horizontalDistance = AddEditBox(text->Right(), text->Top(), 47); horizontalDistance->SetNumeric(1, 4096, 1);

	text = AddText("      Vertical distance: ", 10, horizontalDistance->Down() + 2);
	verticalDistance = AddEditBox(text->Right(), text->Top(), 47); verticalDistance->SetNumeric(1, 4096, 1);

	//Close
	y = DrawHLine(verticalDistance->Down() + 3);
	button = AddButton("Ok", (WIDTH_CLONE-135)/2, y, 0, 0, BT_ADD); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right()+8, y, 0, 0, BT_CLOSE); SetCancelButton(button);


	//Default
	listMode->AddText("Automatic");
	listMode->AddText("Manual");
	listMode->SetItem("Automatic");
	

	horizontalCount->SetText(1);
	verticalCount->SetText(1);

	horizontalDistance->SetText("(auto)");
	verticalDistance->SetText("(auto)");
}


CloneArray::~CloneArray()
{

}

bool CloneArray::OnList(ListPop *list, int index, gedString &text, int listId)
{
	switch(listId)
	{
	case LS_CLONE_DISTANCE:
		{
			if(text == "Automatic")
			{
				horizontalDistance->SetText("(auto)");
				verticalDistance->SetText("(auto)");
			}
			else
			{
				double scale = GameControl::Get()->GetAxis()->getScale();

				horizontalDistance->SetText((int)(clonedActor->Width()/scale));
				verticalDistance->SetText((int)(clonedActor->Height()/scale));
			}

			UpdatePreview();
		}
		break;
	}

	return true;
}

void CloneArray::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_CLOSE:
		{
			RemovePreviewClones();			
		}
		break;
	case BT_ADD:
		{	
			ActorProperty::Update(clonedActor);	
		}
		break;
	}

	delete this;
}

void CloneArray::OnEditChar(EditBox *edit, SDLKey key, int line)
{
	UpdatePreview();
}

void CloneArray::RemovePreviewClones()
{
	for(int i = 0; i < createdClones.Count(); i++)
	{
		//Don't use DELETE_ME (will generate clones with big count)
		//createdClones[i]->PostMessage(createdClones[i], DELETE_ME, DELETE_ME);

		Actor *actor = createdClones[i];
		//GameControl::Get()->RemoveActor(actor, true); //Don't call this: the previe actors don't have childs
		delete actor;
	}

	createdClones.Clear();
}

void CloneArray::UpdatePreview()
{
	WaitCursor wait;

	//Get new params
	int nh = atol(horizontalCount->GetText().c_str()),
		nv = atol(verticalCount->GetText().c_str()),
		hd, vd;

	if(listMode->GetText() == "Automatic")
	{
		double scale = 1.0;
		
		if(clonedActor->getType() != SLOW_CANVAS && clonedActor->getType() != REGION_ACTOR && clonedActor->getType() != REGION_ACTOR_FILLED)
		{
			scale = GameControl::Get()->GetAxis()->getScale();
		}

		hd = (int)(clonedActor->Width()/scale);
		vd = (int)(clonedActor->Height()/scale);
	}
	else
	{
		hd = atol(horizontalDistance->GetText().c_str()),
		vd = atol(verticalDistance->GetText().c_str());

		if(hd < 1) hd = 1;
		else if(hd > hd) hd = 4096;

		if(vd < 1) vd = 1;
		else if(vd > vd) vd = 4096;
	}
	
	if(nh < 1) nh = 1;
	else if(nh > 128) nh = 128;
	
	if(nv < 1) nv = 1;
	else if(nv > 128) nv = 128;


	//Remove old preview clones
	RemovePreviewClones();
	

	//Add new preview clones
	int nx, ny = clonedActor->getImage()->Y();
	
	int i, j;
	for(i = 0; i < nv; i++)
	{
		nx = clonedActor->getImage()->X();

		for(j = 0; j < nh; j++)
		{
			if(i != 0 || j != 0) //Don't clone first actor
			{
				Actor *newClone = clonedActor->NewActor(true, true, nx, ny);
				if(newClone)
				{
					createdClones.PushBack(newClone);
				}
			}

			nx += hd;			
		}

		ny += vd;
	}
}




#define WIDTH_CLONE_PATH		230
#define HEIGHT_CLONE_PATH	105

ClonePath::ClonePath(Actor *actor)
	: Panel("ClonePath", (GameControl::Get()->Width() - WIDTH_CLONE_PATH)/2, 
				          (GameControl::Get()->Height() - HEIGHT_CLONE_PATH)/2,
						  WIDTH_CLONE_PATH, HEIGHT_CLONE_PATH)
{
	SetModal();
	SetToolTip(TIP_CLONEPATH);
	clonedActor = actor;

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText("Clone Along Path", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body	
	text = AddText(" Path: ", 10, y);
	listPath = AddListPop(text->Right(), text->Top());

	text = AddText("Count: ", 10, listPath->Down() + 2);
	count = AddEditBox(text->Right(), text->Top(), 47); count->SetNumeric(1, 4096, 1);


	//Close
	y = DrawHLine(count->Down() + 3);
	button = AddButton("Ok", (WIDTH_CLONE_PATH-135)/2, y, 0, 0, BT_ADD); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right()+8, y, 0, 0, BT_CLOSE); SetCancelButton(button);


	//Default
	count->SetText(1);

	GameControl::Get()->PopulatePaths(listPath, false);
	actualPath = NULL;

	bControlPathShow = (PathDialog::getActualPath() == NULL);
}


ClonePath::~ClonePath()
{
	GameControl::Get()->GetAxis()->getImage()->Invalidate();

	if(actualPath) actualPath->SetOri(xPathOri, yPathOri);

	
	if(bControlPathShow) PathDialog::setActualPath(NULL);
}

bool ClonePath::OnList(ListPop *list, int index, gedString &text, int listId)
{
	Path *path = GameControl::Get()->GetPath(text);

	//Delete anterior path draw
	if(actualPath) actualPath->SetOri(xPathOri, yPathOri);
	actualPath = NULL;
	

	//Draw path
	if(path)
	{
		actualPath = path;	
		if(bControlPathShow) PathDialog::setActualPath(actualPath);

		xPathOri = actualPath->getXOri();
		yPathOri = actualPath->getYOri();

		actualPath->SetOri(clonedActor->getImage()->X(), clonedActor->getImage()->Y());

		UpdatePreview();
	}	

	GameControl::Get()->GetAxis()->getImage()->Invalidate();	

	return true;
}

void ClonePath::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_CLOSE:
		{
			RemovePreviewClones();			
		}
		break;
	case BT_ADD:
		{	
			ActorProperty::Update(clonedActor);
		}
		break;
	}

	delete this;
}

void ClonePath::OnEditChar(EditBox *edit, SDLKey key, int line)
{
	UpdatePreview();
}

void ClonePath::RemovePreviewClones()
{
	for(int i = 0; i < createdClones.Count(); i++)
	{
		//Don't use DELETE_ME (will generate clones with big count)
		//createdClones[i]->PostMessage(createdClones[i], DELETE_ME, DELETE_ME);

		Actor *actor = createdClones[i];
		//GameControl::Get()->RemoveActor(actor, true); //Don't call this: the previe actors don't have childs
		delete actor;
	}

	createdClones.Clear();
}

void ClonePath::UpdatePreview()
{
	if(!actualPath) return;

	WaitCursor wait;
	

	//Get new params
	int nClones = atol(count->GetText().c_str());
	
	
	if(nClones < 1) nClones = 1;
	else if(nClones > 4096) nClones = 4096;


	//Remove old preview clones
	RemovePreviewClones();
	

	//Add new preview clones
	
	int x, y;
	double df = actualPath->GetTotalFrames() / (double)nClones;
	double frame = 0.0;

	do
	{
		actualPath->GetXY(x, y, (int)frame);
		
		Actor *newClone = clonedActor->NewActor(true, true, x + clonedActor->getImage()->X(), y + clonedActor->getImage()->Y());
		if(newClone)
		{
			createdClones.PushBack(newClone);
		}
		
		frame += df;
	} while(frame < actualPath->GetTotalFrames());


}

Actor *ActorProperty::GetCurrentEventActor()
{
	//Returns the current selected actor in editor
	Actor *eventActor = NULL;
	
	if(ExpressionEditor::getExpressionEditor())
	{
		eventActor = ExpressionEditor::getExpressionEditor()->getEventActor();
	}
	else if(ActorProperty::getActorProperty())
	{
		eventActor = ActorProperty::getActorProperty()->getCurrentActor();
	}
	else if(GameControl::Get()->NumActors() > 1)
	{
		eventActor = GameControl::Get()->GetActor(GameControl::Get()->ActorName(1));
	}
	
	if(!eventActor)
	{
		//Get the view
		eventActor = GameControl::Get()->GetViewActor();
	}

	return eventActor;
}


#endif //#STAND_ALONE_GAME




