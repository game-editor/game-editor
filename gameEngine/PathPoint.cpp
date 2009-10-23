/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com

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
Be a Game Editor developer: http://game-editor.com/Sharing_Software_Revenues_in_Open_Source

***************************************************************************/


// PathPoint.cpp: implementation of the PathPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "PathPoint.h"
#include "GameControl.h"
#include "Path.h"


int PathPoint::nPaths = 0;

PathPoint::PathPoint(int x, int y, PathPoint *prev, bool bEdit, Path *path, int iKey)
:	Actor("PathPoint", prev?prev->getRootPath():NULL)
{
	flags.Set(FLAG_PANEL); //to hide in game mode

	if(!prev)
	{
		getImage()->SetZDepth(PATH_DEPTH);
	}

	this->prev = prev;
	this->path = path;
	this->iKey = iKey;
	next = NULL;

	EditorDirectory editDir;
	AddAnimation("iconPath", "maksiconPath.bmp", 2, 1);

	getImage()->SetPos(x, y);

	if(prev)
	{
		KrSprite *pSprite = getSprite();
		if(pSprite) pSprite->SetFrame(1);
	}
	else
	{
		nPaths++;
	}
	

	framesToAnim = DISABLE_ANIMATION;
	flags.Clear(FLAG_ENABLECOLLISIONDETECTION);

	#ifndef STAND_ALONE_GAME
	list = AddListPop(0, 0, 1, 1);
	list->AddText("Linear");
	list->AddText("Curved");
	list->AddText("Delete");
	list->getImage()->SetVisible(false);
	#endif //#ifndef STAND_ALONE_GAME
	

	this->bEdit = bEdit;
	bLinear = false;
	

	SetTransparency(.2);
}

PathPoint::~PathPoint()
{
	if(getRootPath() == this) nPaths--;
	if(next)
		delete next;
}

PathPoint * PathPoint::getRootPath()
{
	PathPoint *rootPath = this;

	while(rootPath->prev)
	{
		rootPath = rootPath->prev;
	}
	
	return rootPath;
}

int PathPoint::TotalKeys()
{
	int n = 1;
	PathPoint *node = getRootPath();

	while(node->next)
	{
		n++;
		node = node->next;
	}
	
	return n;
}

void PathPoint::OnMouseMove(int x, int y)
{
	if(path && GameControl::Get()->getDragActor() == this)
	{
		if(iKey > 0)
		{
			path->SetKey(iKey, getImage()->X(), getImage()->Y());
		}
		else
		{
			path->SetOri(getImage()->X(), getImage()->Y());
		}

		path->ConstantVelocity();
		GameControl::Get()->GetAxis()->getImage()->Invalidate();
	}
}

bool PathPoint::OnMouseButtonDown(int x, int y, Uint8 button)
{
#ifndef STAND_ALONE_GAME
	if(button == SDL_BUTTON_RIGHT)
	{
		list->getImage()->SetVisible(true);
		list->OnMouseButtonDown(x, y, button);
		return false;
	}
#endif //#ifndef STAND_ALONE_GAME

	return bEdit;
}



const stCursor * PathPoint::OnGetCursor()
{
	if(bEdit)
	{
		cursor.imgName	= "makscursorMao.bmp";
		cursor.nFramesH	= 1;
		cursor.nFramesV	= 1;
		cursor.hotSpotX = 7;
		cursor.hotSpotY = 0;
		cursor.bAnimate = false;
		
		return &cursor;
	}
	else
	{
		return NULL;
	}
}

PathPoint *PathPoint::AddPoint(int x, int y, bool bEdit, Path *path, int iKey)
{
	next = new PathPoint(x,y, this, bEdit, path, iKey);
	return next;
}


void PathPoint::Remove()
{
	if(prev)
	{
		prev->next = next;
		if(next) next->prev = prev;
		next = NULL;

		PathPoint *aux = prev->next;
		while(aux)
		{
			aux->iKey = aux->prev->iKey + 1;
			aux = aux->next;
		}

		delete this;
	}
}

#ifndef STAND_ALONE_GAME

bool PathPoint::OnList(ListPop *list, int index, gedString &text, int listId)
{
	list->getImage()->SetVisible(false);

	if(text == "Linear")
	{
		bLinear = true;
		if(path) path->SetLinear(iKey, bLinear);
	}
	else if(text == "Curved")
	{
		bLinear = false;
		if(path) path->SetLinear(iKey, bLinear);
	}
	else
	{
		if(path && getRootPath() != this)
		{
			if(path->RemoveKey(iKey))
			{
				path->ConstantVelocity();
				Remove();
			}
		}
		else
		{
			if(!next) //Last point
			{
				GameControl::Get()->ChangePathPoint(prev);
			}
			Remove();
		}
	}


	GameControl::Get()->GetAxis()->getImage()->Invalidate();

	return true;
}

#endif //#ifndef STAND_ALONE_GAME
