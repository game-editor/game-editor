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


// RegionLoad.cpp: implementation of the RegionLoad class.
//
//////////////////////////////////////////////////////////////////////

#include "RegionLoad.h"
#include "GameControl.h"

/*
Tested with:

- Move to Region.ged
- Move to Region 2.ged
- 1945 reg.ged
- Houp/JItering/JItering9.15_2.ged
- activation-problem_view_parent.ged
- checkers.ged
- abuse.ged
- issue 57 (Test040.ged)
- Delta_V/20.ged
- zoiba/puzzle/puzzle.ged
- andysmiling/Ping-Pong/BumperGirls007.1.ged
*/


MapRegions RegionLoad::regions;
KrRect RegionLoad::viewPosAnt;
bool RegionLoad::bCreatingActors = false;
bool RegionLoad::bShowRegions = true;
RegionLoad *RegionLoad::defaultRegion = NULL;
RegionLoad *RegionLoad::regionTemp = NULL;


RegionLoadSetDefault::RegionLoadSetDefault(Actor *actor)
{
	//Set the default region equal to the first visible region in the actor
	oldDefaultRegion = NULL;

	if(actor && actor->getRegionCount())
	{
		MapRegionsIterator it(actor->GetOwnerRegions());
		it.Begin();

		while(!it.Done())
		{
			RegionLoad *pRegion = *it.Key();
			if(pRegion && pRegion->IsRegionInView())
			{
				oldDefaultRegion = RegionLoad::getDefaultRegion();
				RegionLoad::SetDefaultRegion(pRegion);
				break;
			}

			it.Next();
		}
	}
}

RegionLoadSetDefault::~RegionLoadSetDefault()
{
	//Reset old default region
	if(oldDefaultRegion)
	{
		RegionLoad::SetDefaultRegion(oldDefaultRegion);
	}
}

RegionLoad::RegionLoad()
	: Actor(REGION_NAME, NULL, REGION_ACTOR, 200, 200)
{
	viewPosAnt.Set(-123, 456, 0, 0);
	bRegionInView = false;
	flags.Set(FLAG_REGION);
	getImage()->setSlowCanvasCollision(false);

	regions.Add(this, 1);

#ifdef STAND_ALONE_GAME
	//Disable mouse button events
	setEventControl(EVENTMOUSEBUTTONDOWN, false);
	setEventControl(EVENTMOUSEBUTTONUP, false);
	setEventControl(EVENTMOUSEENTER, false);
	setEventControl(EVENTMOUSELEAVE, false);
	setEventControl(EVENTMOUSEMOVE, false);
#else
	list = AddListPop(0, 0, 1, 1);
	list->AddText("Remove Region");
	list->getImage()->SetVisible(false);
#endif //#ifdef STAND_ALONE_GAME

	setRegionColor(255, 255, 0);
	getImage()->SetZDepth(REGION_DEPTH);
}

RegionLoad::~RegionLoad()
{
	if(defaultRegion == this)
		InvalidateDefaultRegion();

	regions.Remove(this); 
}

void RegionLoad::SetDefaultRegion(RegionLoad *pRegion)
{
	if(pRegion != defaultRegion)
	{
		defaultRegion = pRegion;
	}
}

void RegionLoad::InvalidateDefaultRegion()
{
	defaultRegion = NULL;
}

void RegionLoad::DefineActors()
{
	//Creates a list with actors that intercept the region
	regionActors.Clear();

	KrVector2T< GlFixed > screen, axis;
	Axis *pAxis = GameControl::Get()->GetAxis();
	
	//Get region bounds (Solve the bug "Game behavior differ based on editor grid position when uses regions", ALPHA_1_1_4.ged)
	getImage()->CalcTransform();
	KrRect rectRegion = getImage()->Bounds();
	rectRegion.Translate(-rectRegion.xmin, -rectRegion.ymin);
	
	//To axis coordinate (Solve the bug "Game behavior differ based on editor grid position when uses regions", ALPHA_1_1_4.ged)
	getImage()->ObjectToScreen(0, 0, &screen);
	pAxis->getImage()->ScreenToObject( screen.x.ToInt(), screen.y.ToInt(), &axis );
	rectRegion.Translate(axis.x.ToInt(), axis.y.ToInt());


	MapActorIterator it(mapActors);
	for(it.Begin(); !it.Done(); it.Next())
	{
		ListActor *listActor = it.Value();
		for(int il = 0; il < listActor->Count(); il++)
		{
			Actor *actor = (*listActor)[il];			

			if( actor->EditMode() && 
				!actor->isRegion() &&
				!actor->isView() &&
				actor != pAxis
				)
			{
				//Get actor bounds (Solve the bug "Game behavior differ based on editor grid position when uses regions", ALPHA_1_1_4.ged, abuse2.ged)
				actor->getImage()->CalcCompositeTransform();
				KrRect rectActor = actor->Bounds();

				if(actor->getTextActor() && !rectActor.IsValid())
				{
					//Solve the bug "Text actors aren't load after use the LoadGame function without Activation Regions"
					engine->Tree()->Walk(actor->getImage(), true, true);
					rectActor = actor->Bounds();
				}

				rectActor.Translate(-rectActor.xmin, -rectActor.ymin);

				
				//To axis coordinate (Solve the bug "Game behavior differ based on editor grid position when uses regions", ALPHA_1_1_4.ged, abuse2.ged)
				actor->getImage()->ObjectToScreen(0, 0, &screen);
				pAxis->getImage()->ScreenToObject( screen.x.ToInt(), screen.y.ToInt(), &axis );
				rectActor.Translate(axis.x.ToInt(), axis.y.ToInt());

				if(rectActor.IsValid() && rectRegion.Intersect(rectActor))
				{
					regionActors.Add(actor->getCloneName(), 1); 
				}
			}
		}
	}

	viewPosAnt.Set(-123, 456, 0, 0);
	bRegionInView = false;
}

void RegionLoad::Load(SDL_RWops *src)
{
	regionActors.Clear();

	//Load region defination
	int	x = GameControl::Read32( src ), 
		y = GameControl::Read32( src ), 
		width = GameControl::Read32( src ), 
		height = GameControl::Read32( src );

	//Load default region in view
	Uint8 bDefaultRegion;
	GameControl::Read(src, &bDefaultRegion, sizeof(Uint8), 1);
	if(bDefaultRegion)
	{
		GLASSERT(defaultRegion == NULL);
		SetDefaultRegion(this);
	}


	//Load actors list
	Uint32 count = GameControl::Read32( src );
	gedString name;
	
	for(Uint32 i = 0; i < count; i++)
	{
		ReadString(src, &name);
		regionActors.Add(name, 1);
	}

	//Set position
	getImage()->SetDimensions(x, y, width, height);
	engine->InvalidateRectangle(getImage()->Bounds());
}


void RegionLoad::CreateActors()
{
	//Create regions actor, and necessary parent actors
	if(bRegionInView) return; //Actors are already created
	bRegionInView = true;

	bCreatingActors = true;

	createdActors.Clear();

#ifdef DEBUG
	GLOUTPUT( "Enter region: %X\n", this );
#endif

	
	MapRegionActorsIterator it(regionActors);
	for( it.Begin(); !it.Done(); it.Next() )
	{
		GameControl::Get()->GetActor(*it.Key());
	}
	
	CallOnCreate();

	bCreatingActors = false;
}

void RegionLoad::CallOnCreate()
{
	if(!bRegionInView) return;

	bCreatingActors = true;
	
	int nCreated;
	do
	{
		nCreated = createdActors.size();

		GlobalMapActorIterator itActor(createdActors);
		for( itActor.Begin(); !itActor.Done(); itActor.Next() )
		{
			Actor *actor = *itActor.Key();

			//This actor can be deleted in your old default region,
			//but is need now. So, don't delete
			actor->clearDeleteConfirmation();

			if( actor->getRunning() && actor->getAction() &&
				actor->getRegionCount() == 1) //Region count == 1 => new created actor
			{
				actor->getAction()->OnCreate(actor);

				if(nCreated != createdActors.size())
				{
					//Begin again, to avoid bug don't call all created actors
					break;
				}
			}
		}

	} while(nCreated != createdActors.size());

	bCreatingActors = false;
}

void RegionLoad::DestroyActors()
{
	//Destroys actors that don't belong more any region
	//and not intercept view

	if(!bRegionInView) return; //Actors are not created
	bRegionInView = false;

#ifdef DEBUG
	GLOUTPUT( "Leave region: %X\n", this );
#endif


	Actor *actor;
	GlobalMapActorIterator it(createdActors);
	for( it.Begin(); !it.Done(); it.Next() )
	{
		actor = *it.Key();
		if(IS_VALID_ACTOR(actor))
		{
			actor->RemoveRegionLoad(this);
			if(actor->getRegionCount() <= 0 && actor->getRunning() && !actor->IsActorInView(true))
			{
				//Don't delete now 
				//Solve Activation Region bug in Houp/JItering/JItering9.15_2.ged 
				//(don't destroy desk1.4 after change Activation Region)

				actor->PostMessage(actor, DELETE_ME, DELETE_ME);
			}
		}
	}

	createdActors.Clear();
}

void RegionLoad::UpdateView()
{
	/*
	Tests the intersection of all regions with the view (if moved) 
	Call CreateActors of the areas that began to intercept and    
	DestroyActors of the areas that are not more intercepting.
	*/
	
	//The coordinates changes solve the bug in activation-problem_view_parent.ged
	//(don't load left region)

	//Make sure view don't will shake
	KrImage *pImageView = GameControl::Get()->GetViewActor()->getImage();
	Axis *pAxis = GameControl::Get()->GetAxis();

	if(pImageView->IsInvalid(true)) 
	{
		//Solve the alpha14.ged bug
		pImageView->CalcCompositeTransform();
		engine->Tree()->Walk(pImageView, true, true);
	}
	else
	{
		pImageView->CalcTransform();
	}

	//Get view bounds
	KrRect rectView = pImageView->Bounds(), viewPos;

	//Get view in screen and axis coordinates
	KrVector2T< GlFixed > screen, axis;
	pImageView->ObjectToScreen(0, 0, &screen);
	pAxis->getImage()->ScreenToObject( screen.x.ToInt(), screen.y.ToInt(), &axis );

	//Make sure rect is (0, 0, ...)
	rectView.Translate(-rectView.xmin, -rectView.ymin);

	//Translate to correct position
	rectView.Translate(axis.x.ToInt(), axis.y.ToInt());


	//Solve the bug: "PocketPC don't load checkers.ged"
	viewPos.Set(rectView.xmin, rectView.ymin, 
				//zeroViewPos.x.ToInt(), zeroViewPos.y.ToInt(),
				0, 0);

	

	if(viewPos != viewPosAnt)
	{		
		MapRegions createdRegions, destroyedRegions;

/*#ifdef DEBUG
		GLOUTPUT( " View pos: (%ld, %ld)\n",  rectView.xmin, rectView.ymin);
#endif*/

		MapRegionsIterator it(regions);
		RegionLoad *pRegion;
		for( it.Begin(); !it.Done(); it.Next() )
		{
			pRegion = *it.Key();

			pRegion->getImage()->CalcTransform();
			KrRect rectRegion = pRegion->getImage()->Bounds();
			rectRegion.Translate(-rectRegion.xmin, -rectRegion.ymin);

			//To axis coordinate
			pRegion->getImage()->ObjectToScreen(0, 0, &screen);
			pAxis->getImage()->ScreenToObject( screen.x.ToInt(), screen.y.ToInt(), &axis );
			rectRegion.Translate(axis.x.ToInt(), axis.y.ToInt());

			//View and region in axis coordinates
			if(rectView.Intersect(rectRegion))
			{
				if(!pRegion->bRegionInView)
				{
					createdRegions.Add(pRegion, 1);
				}				
			}
			else
			{
				if(pRegion->bRegionInView)
				{
					destroyedRegions.Add(pRegion, 1);				
				}
			}
		}

		//Destroy actors from regions outside the view
		//Solve the Move to Region.ged when add the 'sad' actor to two default regions 
		//(some times)

		MapRegionsIterator it1(destroyedRegions);
		
		for(it1.Begin(); !it1.Done(); it1.Next())
		{
			pRegion = *it1.Key();
			
			pRegion->DestroyActors();
			if(defaultRegion == pRegion)
				InvalidateDefaultRegion();
		}

		//Call OnCreate after destroy old actors
		//Solve the Move to Region 2.ged actor move to a wrong position
		MapRegionsIterator it2(createdRegions);
		
		for(it2.Begin(); !it2.Done(); it2.Next())
		{
			pRegion = *it2.Key();			
			RegionLoad *pOldDefaultRegion = defaultRegion;
			SetDefaultRegion(pRegion);
			
			pRegion->CreateActors();
					
			RemoveCommonActorsFromOldDefaultRegion(pOldDefaultRegion);
		}
		
		viewPosAnt = viewPos;
	}






}

void RegionLoad::RemoveCommonActorsFromOldDefaultRegion(RegionLoad *pOldDefaultRegion)
{
	//Remove actors that are in the new default region, from old default region
	//Solve the Move to Region.ged when add the 'sad' actor to two default regions 
	//(after load, new game, load again)

	if(pOldDefaultRegion && defaultRegion && pOldDefaultRegion != defaultRegion)
	{		
		Actor *actor;
		GlobalMapActorIterator itNew(defaultRegion->createdActors);
		
		for(itNew.Begin(); !itNew.Done(); itNew.Next())
		{
			actor = *itNew.Key();
			if(IS_VALID_ACTOR(actor) && actor->getRegionCount() > 1)
			{
				if(actor->RemoveRegionLoad(pOldDefaultRegion))
				{
					pOldDefaultRegion->createdActors.Remove(actor);
				}
			}
		}
	}
}

void RegionLoad::LoadRegions(SDL_RWops *src)
{
	//Load all regions
	DestroyRegions();

	Uint32 count = GameControl::Read32( src );
		
	for(Uint32 i = 0; i < count; i++)
	{
		RegionLoad *newRegion = new RegionLoad();
		newRegion->Load(src);
	}
}



void RegionLoad::DestroyRegions()
{
	MapRegionsIterator it(regions);
	it.Begin();
	while(!it.Done())
	{
		RegionLoad *pRegion = *it.Key();
		delete pRegion;
		it.Begin();
	}
}

void RegionLoad::UpdateRegions()
{
	//Define all regions actors

	//Reset axis (Solve the bug "Game behavior differ based on editor grid position when uses regions", ALPHA_1_1_4.ged)
	int xAxisAnt, yAxisAnt;
	double scaleAnt;
	Axis *pAxis = GameControl::Get()->GetAxis();

	scaleAnt = pAxis->getScale();	
	xAxisAnt = pAxis->getImage()->X();
	yAxisAnt = pAxis->getImage()->Y();

	pAxis->SetScale(1.0);
	pAxis->SetPos(0, 0);
	pAxis->getImage()->Invalidate();

	//Define actors
	MapRegionsIterator it(regions);
	RegionLoad *pRegion;
	for( it.Begin(); !it.Done(); it.Next() )
	{
		pRegion = *it.Key();
		pRegion->DefineActors();
	}

	SetDefaultRegionView();

	//Restore axis (Solve the bug "Game behavior differ based on editor grid position when uses regions", ALPHA_1_1_4.ged)
	pAxis->SetScale(scaleAnt);
	pAxis->SetPos(xAxisAnt, yAxisAnt);	
	pAxis->getImage()->Invalidate();
	
}

void RegionLoad::SetDefaultRegionView()
{
	//Set default region (first region in view)
	//Must be used when view has a parent

	InvalidateDefaultRegion();
	Actor *view = GameControl::Get()->GetEditView();
	if(!view) return;

	KrRect rectView = view->getImage()->Bounds();
	

	MapRegionsIterator it(regions);
	RegionLoad *pRegion;
	for( it.Begin(); !it.Done(); it.Next() )
	{
		pRegion = *it.Key();
		if(rectView.Intersect(pRegion->getImage()->Bounds()))
		{
			SetDefaultRegion(pRegion);
			break;
		}
	}
}

void RegionLoad::ClearRegions()
{
	//Call when leave game mode

	MapRegionsIterator it(regions);
	for( it.Begin(); !it.Done(); it.Next() )
	{
		RegionLoad *pRegion = *it.Key();
		pRegion->RemoveActors();
	}
	
	defaultRegion = NULL;

	if(regionTemp)
	{
		delete regionTemp;
		regionTemp = NULL;
	}
}

void RegionLoad::RemoveActors()
{
	Actor *actor;
	GlobalMapActorIterator it(createdActors);
	for( it.Begin(); !it.Done(); it.Next() )
	{
		actor = *it.Key();

#ifdef DEBUG
	GLOUTPUT( "--- RegionLoad::RemoveActors: (%X) ",  actor);
#endif

		if(IS_VALID_ACTOR(actor))
		{
#ifdef DEBUG
			GLOUTPUT("ok");
#endif
			actor->RemoveRegionLoad(this);
		}

#ifdef DEBUG
	GLOUTPUT("\n");
#endif
	}

	createdActors.Clear();
}

void RegionLoad::RemoveActor(Actor *actor)
{
	createdActors.Remove(actor);
}

void RegionLoad::AddActor(Actor *actor)
{
	createdActors.Add(actor, 1);
}

void RegionLoad::ToggleVisibility()
{
	bShowRegions = !bShowRegions;

	MapRegionsIterator it(regions);
	for( it.Begin(); !it.Done(); it.Next() )
	{
		RegionLoad *pRegion = *it.Key();
		pRegion->getImage()->SetVisible(bShowRegions);
	}
}

bool RegionLoad::OnMouseButtonDown(int x, int y, Uint8 button)
{
	if(GameControl::Get()->getGameMode()) return false;

#ifndef STAND_ALONE_GAME
	if(button == SDL_BUTTON_RIGHT)
	{
		list->getImage()->SetVisible(true);
		list->OnMouseButtonDown(x, y, button);
		return false;
	}
#endif //#ifndef STAND_ALONE_GAME

	return Actor::OnMouseButtonDown(x, y, button);
}





void RegionLoad::CreateRegionTemp()
{
	if(regionTemp)
		delete regionTemp;

	regionTemp = new RegionLoad();

	if(regions.size() == 1)
	{
		//Solve the abuse.ged bug (don't show copTop in stand alone)
		SetDefaultRegion(regionTemp);
	}

	//Maximize region
	regionTemp->getImage()->SetDimensions(-256000, -256000, 512000, 512000);
}

void RegionLoad::AddActorInRegionTemp(const gedString &name)
{
	if(!regionTemp)
		return;

	regionTemp->regionActors.Add(name, 1);
}

#ifndef STAND_ALONE_GAME
bool RegionLoad::OnList(ListPop *list, int index, gedString &text, int listId)
{
	list->getImage()->SetVisible(false);

	if(index == 0)
	{
		PanelQuestion *panel = new PanelQuestion("Remove region?");
		if(panel->Wait() == OK_BUTTON)
		{
			delete this;
		}

		delete panel;
	}
	
	return true;
}

void RegionLoad::Save(SDL_RWops *src)
{
	//Save region defination
	SDL_WriteLE32(src, getImage()->X());
	SDL_WriteLE32(src, getImage()->Y());

	SDL_WriteLE32(src, ((SlowCanvas *)getImage())->Width());
	SDL_WriteLE32(src, ((SlowCanvas *)getImage())->Height());

	//Save default region in view
	Uint8 bDefaultRegion = (defaultRegion == this)?1:0;
	SDL_RWwrite(src, &bDefaultRegion, sizeof(Uint8), 1);

	//Save actors list
	SDL_WriteLE32(src, regionActors.size());
	
	MapRegionActorsIterator it(regionActors);
	for( it.Begin(); !it.Done(); it.Next() )
	{
		WriteString(src, *it.Key());		
	}
}

void RegionLoad::SaveRegions(SDL_RWops *src)
{
	//Save all regions
	SDL_WriteLE32(src, regions.size());

	if(regionTemp)
	{
		delete regionTemp;
		regionTemp = NULL;
	}

	MapRegionsIterator it(regions);
	RegionLoad *pRegion;
	for( it.Begin(); !it.Done(); it.Next() )
	{
		pRegion = *it.Key();
		pRegion->Save(src);
	}
}

void RegionLoad::DisableMouse()
{
	MapRegionsIterator it(regions);
	RegionLoad *pRegion;
	for( it.Begin(); !it.Done(); it.Next() )
	{
		pRegion = *it.Key();
		
		pRegion->setEventControl(EVENTMOUSEBUTTONDOWN, false);
		pRegion->setEventControl(EVENTMOUSEBUTTONUP, false);
		pRegion->setEventControl(EVENTMOUSEENTER, false);
		pRegion->setEventControl(EVENTMOUSELEAVE, false);
		pRegion->setEventControl(EVENTMOUSEMOVE, false);
	}
}

void RegionLoad::EnableMouse()
{
	MapRegionsIterator it(regions);
	RegionLoad *pRegion;
	for( it.Begin(); !it.Done(); it.Next() )
	{
		pRegion = *it.Key();
		
		pRegion->setEventControl(EVENTMOUSEBUTTONDOWN, true);
		pRegion->setEventControl(EVENTMOUSEBUTTONUP, true);
		pRegion->setEventControl(EVENTMOUSEENTER, true);
		pRegion->setEventControl(EVENTMOUSELEAVE, true);
		pRegion->setEventControl(EVENTMOUSEMOVE, true);
	}
}

#endif //#ifndef STAND_ALONE_GAME