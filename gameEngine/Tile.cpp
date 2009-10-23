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


// Tile.cpp: implementation of the Tile class.
//
//////////////////////////////////////////////////////////////////////

#pragma warning(disable:4786)

#include "Tile.h"
#include "GameControl.h"

#define ERASE_COLOR 200
#define DRAW_COLOR  255

Tile::Tile(Actor *actor)
{
	this->actor = actor;
	bInit = false;
	inix = iniy = 0;
	iMouse = jMouse = 0;
	bEnableAdd = false;
	bVisibleState = true;
	bCursorCreated = false;
	nAnimations = 0;
	color = DRAW_COLOR;
	bSingle = false;
	bRandom = false;
	area = width = height = 0;

	//Create the parent sprite
	//Need to avoid check in other actor children when have tiles
	//Must check collisions with children of the parentSprite
	//Solve bug in changeParentDemo.ged when slide out of platform

	parentSprite = new KrSprite( actor->getSpriteResource(), actor );
	parentSprite->SetNodeId((int)parentSprite);
	engine->Tree()->AddNode(actor->getImage(), parentSprite);
	parentSprite->SetAction(ACTOR_GED_TEXT);

	//parentSprite = actor->getSprite();

	actor->SetAnimation(ACTOR_GED_TEXT);
}

Tile::~Tile()
{
	MapTileIterator itTile(mapTile);	
	for(itTile.Begin(); !itTile.Done(); itTile.Next())
	{
		TileElement *element = *itTile.Value();
		delete element;
	}
	mapTile.Clear();


	if(engine->Tree()->FindNodeById((int)parentSprite))
	{
		engine->Tree()->DeleteNode(parentSprite);
	}	
}

TileElement::TileElement()
{
	sprite = NULL;
	tileSlot = 0;
	animationIndex = 0;
}

TileElement::~TileElement()
{
	if(sprite)
	{
		if(engine->Tree()->FindNodeById((int)sprite))
		{
			engine->Tree()->DeleteNode(sprite);
		}
	}
}

void TileElement::SetTile(int index)
{
	if(sprite) sprite->SetFrame(index);	
	tileSlot = index;
}


void TileElement::CreateSprite(Actor *actor, KrSprite *parent)
{

	sprite = new KrSprite( actor->getSpriteResource(), actor );
	sprite->SetNodeId((int)sprite);
	engine->Tree()->AddNode(parent, sprite);
}


void Tile::CreateTileCursor(const gedString &animationName)
{
	//Create 3x3 tiles

	int index = nAnimations;

	MapAnimationsIterator itAnim(mapAnimations);	
	for(itAnim.Begin(); !itAnim.Done(); itAnim.Next())
	{
		if(*itAnim.Value() == animationName)
		{
			index = *itAnim.Key();
			break;
		}
	}

	for(int i = 0; i < 9; i++)
	{
		if(!bCursorCreated) tileCursor[i].CreateSprite(actor, parentSprite);
		tileCursor[i].getSprite()->SetAction(animationName);
		tileCursor[i].SetTile(i);
		tileCursor[i].setAnimationIndex(index);
		tileCursor[i].getSprite()->SetZDepth(MAX_ACTOR_DEPTH);
	}

	bCursorCreated = true;
}








void Tile::SetVisibleCursor(bool bVisible)
{
	if(bVisible == bVisibleState) return;
	
	int n = bSingle?1:9;
	
	for(int i = 0; i < n; i++)
	{
		KrSprite *sprite = tileCursor[i].getSprite();
		if(sprite)
		{
			sprite->SetVisible(bVisible);
		}
	}

	bVisibleState = bVisible;

	if(!bVisibleState)
	{
		if(!NumTiles()) actor->SetAnimation(animationName, true);
	}
	else
	{
		actor->SetAnimation(ACTOR_GED_TEXT);
	}
}


void Tile::SetColorCursor(int color)
{
	if(color == this->color) return;
	
	KrColorTransform colorTrans;
	colorTrans.Set(color, 0, color, 0, color, 0, 255);	
	for(int i = 0; i < 9; i++)
	{
		tileCursor[i].getSprite()->SetColor(colorTrans);
	}

	this->color = color;
}

void Tile::SetDrawMode(bool bDraw)
{
	if(bDraw)
	{
		SetColorCursor(DRAW_COLOR);
	}
	else
	{
		SetColorCursor(ERASE_COLOR);
	}
}

void Tile::Equal(Tile *copyTile)
{
	bInit = true;
	area = width = height = 0;

	tileSize = copyTile->tileSize;	
	inix = copyTile->inix;
	iniy = copyTile->iniy;	
	actualx = copyTile->actualx;
	actualy = copyTile->actualy;
	iMouse = copyTile->iMouse;
	jMouse = copyTile->jMouse;
	animationName = copyTile->animationName;

	CreateTileCursor(animationName);

	MapAnimationsIterator itAnim(copyTile->mapAnimations);	
	for(itAnim.Begin(); !itAnim.Done(); itAnim.Next())
	{
		mapAnimations.Add(nAnimations, *itAnim.Value());
		nAnimations++;
	}	

	MapTileIterator it(copyTile->mapTile);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		Uint32 coord = *it.Key();
		TileElement *copyElement = *it.Value(), *newElement;

		newElement = new TileElement();
		newElement->CreateSprite(actor, parentSprite);
		newElement->getSprite()->SetAction(copyTile->AnimationName(copyElement->getAnimationIndex()));
		newElement->setAnimationIndex(copyElement->getAnimationIndex());
		newElement->SetTile(copyElement->getSlot());
		
		newElement->getSprite()->SetPos(copyElement->getSprite()->X(), copyElement->getSprite()->Y());
		mapTile.Add(coord, newElement);		
	}

	SetVisibleCursor(false);
}

#ifndef STAND_ALONE_GAME
void Tile::Save(SDL_RWops *src)
{
	animationName.Write(src);

	SDL_WriteLE32(src, tileSize);
	SDL_WriteLE32(src, inix);
	SDL_WriteLE32(src, iniy);
	SDL_WriteLE32(src, actualx);
	SDL_WriteLE32(src, actualy);
	SDL_WriteLE32(src, iMouse);
	SDL_WriteLE32(src, jMouse);

	//Animation names
	SDL_WriteLE32(src, nAnimations);

	MapAnimationsIterator itAnim(mapAnimations);	
	for(itAnim.Begin(); !itAnim.Done(); itAnim.Next())
	{
		itAnim.Value()->Write(src);
	}

	//Tiles
	SDL_WriteLE32(src, mapTile.size());

	MapTileIterator it(mapTile);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		TileElement *element = *it.Value();

		Uint32 coord = *it.Key(), x = element->getSprite()->X(), y = element->getSprite()->Y();
		Uint16 animIndex = element->getAnimationIndex();
		Uint8  slot = element->getSlot();
		

		SDL_WriteLE32(src, coord);
		SDL_WriteLE16(src, animIndex);
		SDL_RWwrite(src, &slot, sizeof(slot), 1);
		SDL_WriteLE32(src, x);
		SDL_WriteLE32(src, y);
	}

	//Final expansion indicator
	Uint8 expansion = 0;
	SDL_RWwrite(src, &expansion, sizeof(expansion), 1);
}
#endif //#ifndef STAND_ALONE_GAME

void Tile::Load(SDL_RWops *src)
{
	area = width = height = 0;
	bInit = true;

	animationName.Read(src);


	tileSize = GameControl::Read32(src);
	inix = GameControl::Read32(src);
	iniy = GameControl::Read32(src);
	actualx = GameControl::Read32(src);
	actualy = GameControl::Read32(src);
	iMouse = GameControl::Read32(src);
	jMouse = GameControl::Read32(src);

	//Animation names
	nAnimations = GameControl::Read32(src);
	int i;
	for(i = 0; i < nAnimations; i++)
	{
		gedString animName;
		animName.Read(src);

		mapAnimations.Add(i, animName);		
	}

	//Tiles
	Uint32 nTiles = GameControl::Read32(src);

	for(i = 0; i < nTiles; i++)
	{
		Uint32 coord, x, y;
		Uint16 animIndex;
		Uint8  slot;

		coord = GameControl::Read32(src);
		animIndex = GameControl::Read16(src);
		GameControl::Read(src, &slot, sizeof(slot), 1);
		x = GameControl::Read32(src);
		y = GameControl::Read32(src);

		TileElement *newElement = new TileElement();
		newElement->CreateSprite(actor, parentSprite);
		newElement->getSprite()->SetAction(AnimationName(animIndex));
		newElement->setAnimationIndex(animIndex);
		newElement->SetTile(slot);
		
		newElement->getSprite()->SetPos(x, y);
		mapTile.Add(coord, newElement);
	}

	//Final expansion indicator
	Uint8 expansion = 0;
	GameControl::Read(src, &expansion, sizeof(expansion), 1);
}


const gedString Tile::AnimationName(int i)
{
	gedString *s = mapAnimations[i];
	if(s)
	{
		return *s;
	}

	return "";
}




void TileElement::RandomTile()
{
	GlRandom random(SDL_GetTicks());
	SetTile(random.Rand(getSprite()->NumFrames()));
}

int Tile::GetArea()
{
	if(!area)
	{
		CalcDimensions();
	}

	return area;
}

int Tile::Width()
{
	if(!width)
	{
		CalcDimensions();
	}

	return width;
}

int Tile::Height()
{
	if(!height)
	{
		CalcDimensions();
	}

	return height;
}

void Tile::CalcDimensions()
{
	KrRect rect;
	rect.Set(0, 0, 0, 0);
	area = 0;
	MapTileIterator it(mapTile);


	for(it.Begin(); !it.Done(); it.Next())
	{
		TileElement *element = *it.Value();
		if(element->getSprite()->IsVisible())
		{
			KrRect r;
			KrRle *rle = element->getSprite()->GetAction()->GetFrame(element->getSprite()->Frame());

			rle->CalculateBounds(element->getSprite()->XTransform(), &r);

			rect.DoUnion(r);
			area += rle->getArea();			
		}
	}
	
	width = rect.Width();
	height = rect.Height();
}

#ifndef STAND_ALONE_GAME
bool Tile::AddTile()
{
	if(!bVisibleState) return false;
	area = width = height = 0;
	
	int x = actualx,
		y = actualy;
	if(!bInit)
	{
		inix = x;
		iniy = y;
		bInit = true;
	}
	
	if(!bSingle)
	{
		//Create 3x3 tiles
		int i = (x - inix)/ tileSize, j = (y - iniy) / tileSize;
		
		/*SDLMod keyMod = SDL_GetModState(); //Don't works in tutorial mode
		if(keyMod & KMOD_RSHIFT)*/
		if(color == ERASE_COLOR)
		{
			//Clear
			int i1, j1, j0;
			TileElement *element;
			for(j0 = 0; j0 < 3; j0++)
			{
				for(int i0 = 0; i0 < 3; i0++)
				{
					i1 = i + i0;
					j1 = j + j0;
					element = GetTile(i1, j1, false);
					if(element)
					{
						mapTile.Remove(Coord(i1, j1));
						delete element;
					}
				}
			}
			
			//Adjust
			for(j0 = -1; j0 < 4; j0++)
			{
				i1 = i - 1;
				j1 = j + j0;
				element = GetTile(i1, j1, false);
				if(element)
				{
					AdjustTileSlot(element, i1, j1, -1, false);
				}
				
				i1 = i + 3;
				element = GetTile(i1, j1, false);
				if(element)
				{
					AdjustTileSlot(element, i1, j1, -1, false);
				}
			}
			
			for(int i0 = -1; i0 < 4; i0++)
			{
				i1 = i + i0;
				j1 = j  - 1;
				element = GetTile(i1, j1, false);
				if(element)
				{
					AdjustTileSlot(element, i1, j1, -1, false);
				}
				
				j1 = j + 3;
				element = GetTile(i1, j1, false);
				if(element)
				{
					AdjustTileSlot(element, i1, j1, -1, false);
				}
			}
			
			//if(!NumTiles()) actor->SetAnimation(animationName);
			return true;
		}
		
		
		//Add
		for(int n = 0; n < 9; n++)
		{
			TileElement *element = NULL;
			switch(n)
			{
			case 0:
				if((element = GetTile(i, j, false)) == NULL)
				{
					element = new TileElement();
					element->CreateSprite(actor, parentSprite);
					element->getSprite()->SetAction(animationName);
					
					element->getSprite()->SetPos(x, y);
					mapTile.Add(Coord(i, j), element);
				}			
				break;
			case 1:
				if((element = GetTile(i+1, j, false)) == NULL)
				{
					element = new TileElement();
					element->CreateSprite(actor, parentSprite);
					element->getSprite()->SetAction(animationName);
					
					element->getSprite()->SetPos(x+tileSize, y);
					mapTile.Add(Coord(i+1, j), element);
				}
				break;
			case 2:
				if((element = GetTile(i+2, j, false)) == NULL)
				{
					element = new TileElement();
					element->CreateSprite(actor, parentSprite);
					element->getSprite()->SetAction(animationName);
					
					element->getSprite()->SetPos(x+2*tileSize, y);
					mapTile.Add(Coord(i+2, j), element);
				}
				break;
			case 3:
				if((element = GetTile(i, j+1, false)) == NULL)
				{
					element = new TileElement();
					element->CreateSprite(actor, parentSprite);
					element->getSprite()->SetAction(animationName);
					
					element->getSprite()->SetPos(x, y+tileSize);
					mapTile.Add(Coord(i, j+1), element);
				}
				break;
			case 4:
				if((element = GetTile(i+1, j+1, false)) == NULL)
				{
					element = new TileElement();
					element->CreateSprite(actor, parentSprite);
					element->getSprite()->SetAction(animationName);
					
					element->getSprite()->SetPos(x+tileSize, y+tileSize);
					mapTile.Add(Coord(i+1, j+1), element);
				}
				break;
			case 5:
				if((element = GetTile(i+2, j+1, false)) == NULL)
				{
					element = new TileElement();
					element->CreateSprite(actor, parentSprite);
					element->getSprite()->SetAction(animationName);
					
					element->getSprite()->SetPos(x+2*tileSize, y+tileSize);
					mapTile.Add(Coord(i+2, j+1), element);
				}
				break;
			case 6:
				if((element = GetTile(i, j+2, false)) == NULL)
				{
					element = new TileElement();
					element->CreateSprite(actor, parentSprite);
					element->getSprite()->SetAction(animationName);
					
					element->getSprite()->SetPos(x, y+2*tileSize);
					mapTile.Add(Coord(i, j+2), element);
				}
				break;
			case 7:
				if((element = GetTile(i+1, j+2, false)) == NULL)
				{
					element = new TileElement();
					element->CreateSprite(actor, parentSprite);
					element->getSprite()->SetAction(animationName);
					
					element->getSprite()->SetPos(x+tileSize, y+2*tileSize);
					mapTile.Add(Coord(i+1, j+2), element);
				}
				break;
			case 8:
				if((element = GetTile(i+2, j+2, false)) == NULL)
				{
					element = new TileElement();
					element->CreateSprite(actor, parentSprite);
					element->getSprite()->SetAction(animationName);
					
					element->getSprite()->SetPos(x+2*tileSize, y+2*tileSize);
					mapTile.Add(Coord(i+2, j+2), element);
				}
				break;		
			}
			
			if(element)
			{
				element->SetTile(tileCursor[n].getSlot());
				element->setAnimationIndex(tileCursor[n].getAnimationIndex());
			}
		}
	}
	else
	{
		//Single tile

		/*SDLMod keyMod = SDL_GetModState(); //Don't works in tutorial mode
		if(keyMod & KMOD_RSHIFT)*/
		if(color == ERASE_COLOR)
		{
			//Clear
			GlDynArray<KrImage*> hittest;
			engine->Tree()->HitTest(xscreen, yscreen, KrImageTree::GET_ALL_HITS, &hittest);
			
			int n = hittest.Count();
			for(int i = 0; i < n; i++)
			{						
				KrImage *p = hittest.Item(i);
				Actor *actorHit = (Actor *)p->getParentActor();
				if(actor == actorHit && actor->getImage() != p)
				{
					Uint32 coord = Coord(p->X(), p->Y());
					TileElement **p = mapTile[coord];
					if(p)
					{
						TileElement *element = *p;
						mapTile.Remove(coord);
						delete element;
					}
				}
			}			
			
			//if(!NumTiles()) actor->SetAnimation(animationName);
			return true;						
		}
		else
		{
			//Add
			Uint32 coord = Coord(x, y);
			if(!mapTile[coord])
			{
				TileElement *element = new TileElement();
				element->CreateSprite(actor, parentSprite);
				element->getSprite()->SetAction(animationName);
				
				element->getSprite()->SetPos(x, y);
				mapTile.Add(coord, element);
				
				element->SetTile(tileCursor[0].getSlot());
				element->setAnimationIndex(tileCursor[0].getAnimationIndex());
			}
		}
	}

	return true;
}

void Tile::OnMouseMove(int x, int y)
{
	//To parent coordinates
	xscreen = x;
	yscreen = y;

	KrVector2T< GlFixed > object;									
	actor->getImage()->ScreenToObject( x, y, &object );
	x = object.x.ToInt();
	y = object.y.ToInt();

	if(!bSingle)
	{
		x -= (int)(3*tileSize*actor->getImage()->XScale().ToDouble()/2.0);
		y -= (int)(3*tileSize*actor->getImage()->XScale().ToDouble()/2.0);
	}

	//Snap
	GridSnap(x, y);
	actualx = x;
	actualy = y;

	if(!bSingle)
	{
		int i = (x - inix)/ tileSize, j = (y - iniy) / tileSize;
		iMouse = i;
		jMouse = j;
		
		tileCursor[0].getSprite()->SetPos(x, y);
		tileCursor[1].getSprite()->SetPos(x+tileSize, y);
		tileCursor[2].getSprite()->SetPos(x+2*tileSize, y);
		
		tileCursor[3].getSprite()->SetPos(x, y+tileSize);
		tileCursor[4].getSprite()->SetPos(x+tileSize, y+tileSize);
		tileCursor[5].getSprite()->SetPos(x+2*tileSize, y+tileSize);
		
		tileCursor[6].getSprite()->SetPos(x, y+2*tileSize);
		tileCursor[7].getSprite()->SetPos(x+tileSize, y+2*tileSize);
		tileCursor[8].getSprite()->SetPos(x+2*tileSize, y+2*tileSize);
		
		//Set tile slot
		int i1, j1, n = 0;	
		for(int j0 = 0; j0 < 3; j0++)
		{
			for(int i0 = 0; i0 < 3; i0++)
			{
				i1 = i + i0;
				j1 = j + j0;
				AdjustTileSlot(&tileCursor[n], i1, j1, n);			
				n++;
			}
		}
	}
	else
	{
		//Single tile
		if(bRandom) tileCursor[0].RandomTile();
		tileCursor[0].getSprite()->SetPos(x, y);

		int movex, movey;
		if(GameControl::Get()->ImageSnap(tileCursor[0].getSprite(), GameControl::Get()->GetAxis()->getImage(), &movex, &movey))
		{
			actualx += movex;
			actualy += movey;
		}
	}

	if(bEnableAdd) AddTile();
}

Uint32 Tile::Coord(int i, int j)
{
	//Uint32 coord = 0;	
	//coord = i | (j << 16); //Fail if i < 0

	char coord[4];
	memcpy(&coord[0], &i, 2);
	memcpy(&coord[2], &j, 2);

	Uint32 res;
	memcpy(&res, coord, 4);
	return res;
}

void Tile::GridSnap(int &x, int &y)
{
	if(!bInit) return;
	
	if(!bSingle)
	{
		int scale = 1, gridX, gridY;
		gridX = gridY = tileSize;
		
		int gridXs = gridX*scale,
			gridYs = gridY*scale;
		
		if(gridXs >= 1 && gridYs >= 1)
		{
			
			int x1 = (x/gridXs)*gridXs + ((inix) % gridXs),
				y1 = (y/gridYs)*gridYs + ((iniy) % gridYs);
			
			if(x < x1 + gridXs/2.0) x = x1;
			else x = x1 + gridXs;
			
			if(y < y1 + gridYs/2.0) y = y1;
			else y = y1 + gridYs;
		}
	}
	else
	{
	}
}

TileElement *Tile::GetTile(int i, int j, bool bGetMouse)
{
	Uint32 coord = Coord(i, j);
	TileElement **element = mapTile[coord];
	if(element)
	{
		return *element;
	}
	else if(bGetMouse)
	{
		if(iMouse == i && jMouse == j) return &tileCursor[0];
		else if(iMouse+1 == i && jMouse == j) return &tileCursor[1];
		else if(iMouse+2 == i && jMouse == j) return &tileCursor[2];
		else if(iMouse == i && jMouse+1 == j) return &tileCursor[3];
		else if(iMouse+1 == i && jMouse+1 == j) return &tileCursor[4];
		else if(iMouse+2 == i && jMouse+1 == j) return &tileCursor[5];
		else if(iMouse == i && jMouse+2 == j) return &tileCursor[6];
		else if(iMouse+1 == i && jMouse+2 == j) return &tileCursor[7];
		else if(iMouse+2 == i && jMouse+2 == j) return &tileCursor[8];		
	}

	return NULL;
}

void Tile::AdjustTileSlot(TileElement *element, int i1, int j1, int n, bool bGetMouse)
{
	//Set tile slot	
	element->getSprite()->SetAction(tileCursor[0].getSprite()->GetAction()->Name());
	
	//Is slot 0?
	if( GetTile(i1-1, j1-1, bGetMouse) == NULL &&
		GetTile(i1, j1-1, bGetMouse)   == NULL &&
		//GetTile(i1+1, j1-1, bGetMouse) == NULL && //Ok 1
		
		GetTile(i1-1, j1, bGetMouse)   == NULL &&
		GetTile(i1, j1, bGetMouse)     != NULL &&
		GetTile(i1+1, j1, bGetMouse)   != NULL &&
		
		//GetTile(i1-1, j1+1, bGetMouse) == NULL && //Ok 1	   
		GetTile(i1, j1+1, bGetMouse)   != NULL &&
		GetTile(i1+1, j1+1, bGetMouse) != NULL)
	{
		element->SetTile(0);
	} 
	else
	
	//Is slot 1?
	if( //GetTile(i1-1, j1-1, bGetMouse) == NULL && //ok
		GetTile(i1, j1-1, bGetMouse)   == NULL &&
		//GetTile(i1+1, j1-1, bGetMouse) == NULL && //ok
		
		GetTile(i1-1, j1, bGetMouse)   != NULL &&
		GetTile(i1, j1, bGetMouse)     != NULL &&
		GetTile(i1+1, j1, bGetMouse)   != NULL &&
		
		GetTile(i1-1, j1+1, bGetMouse) != NULL &&	   
		GetTile(i1, j1+1, bGetMouse)   != NULL &&
		GetTile(i1+1, j1+1, bGetMouse) != NULL)
	{
		element->SetTile(1);
	}
	else
	
	//Is slot 2?
	if( //GetTile(i1-1, j1-1, bGetMouse) == NULL && //Ok 1
		GetTile(i1, j1-1, bGetMouse)   == NULL &&
		GetTile(i1+1, j1-1, bGetMouse) == NULL &&
		
		GetTile(i1-1, j1, bGetMouse)   != NULL &&
		GetTile(i1, j1, bGetMouse)     != NULL &&
		GetTile(i1+1, j1, bGetMouse)   == NULL &&
		
		GetTile(i1-1, j1+1, bGetMouse) != NULL &&	   
		GetTile(i1, j1+1, bGetMouse)   != NULL /*&& //Ok 1
		GetTile(i1+1, j1+1, bGetMouse) == NULL*/)
	{
		element->SetTile(2);
	}
	else
	
	//Is slot 3?
	if( //GetTile(i1-1, j1-1, bGetMouse) == NULL && //ok
		GetTile(i1, j1-1, bGetMouse)   != NULL &&
		GetTile(i1+1, j1-1, bGetMouse) != NULL &&
		
		GetTile(i1-1, j1, bGetMouse)   == NULL &&
		GetTile(i1, j1, bGetMouse)     != NULL &&
		GetTile(i1+1, j1, bGetMouse)   != NULL &&
		
		//GetTile(i1-1, j1+1, bGetMouse) == NULL &&	//ok   
		GetTile(i1, j1+1, bGetMouse)   != NULL &&
		GetTile(i1+1, j1+1, bGetMouse) != NULL)
	{
		element->SetTile(3);
	}
	else
	
	//Is slot 4 (center)?
	if( GetTile(i1-1, j1-1, bGetMouse) != NULL &&
		GetTile(i1, j1-1, bGetMouse)   != NULL &&
		GetTile(i1+1, j1-1, bGetMouse) != NULL &&
		
		GetTile(i1-1, j1, bGetMouse)   != NULL &&
		GetTile(i1, j1, bGetMouse)     != NULL &&
		GetTile(i1+1, j1, bGetMouse)   != NULL &&
		
		GetTile(i1-1, j1+1, bGetMouse) != NULL &&	   
		GetTile(i1, j1+1, bGetMouse)   != NULL &&
		GetTile(i1+1, j1+1, bGetMouse) != NULL)
	{
		element->SetTile(4);
	}
	else
	
	//Is slot 5?
	if( GetTile(i1-1, j1-1, bGetMouse) != NULL &&
		GetTile(i1, j1-1, bGetMouse)   != NULL &&
		//GetTile(i1+1, j1-1, bGetMouse) == NULL && //ok
		
		GetTile(i1-1, j1, bGetMouse)   != NULL &&
		GetTile(i1, j1, bGetMouse)     != NULL &&
		GetTile(i1+1, j1, bGetMouse)   == NULL &&
		
		GetTile(i1-1, j1+1, bGetMouse) != NULL &&	   
		GetTile(i1, j1+1, bGetMouse)   != NULL /*&& 
		GetTile(i1+1, j1+1, bGetMouse) == NULL*/) //ok
	{
		element->SetTile(5);
	}
	else
	
	//Is slot 6?
	if( //GetTile(i1-1, j1-1, bGetMouse) == NULL && //Ok 1
		GetTile(i1, j1-1, bGetMouse)   != NULL &&
		GetTile(i1+1, j1-1, bGetMouse) != NULL &&
		
		GetTile(i1-1, j1, bGetMouse)   == NULL &&
		GetTile(i1, j1, bGetMouse)     != NULL &&
		GetTile(i1+1, j1, bGetMouse)   != NULL &&
		
		GetTile(i1-1, j1+1, bGetMouse) == NULL &&	   
		GetTile(i1, j1+1, bGetMouse)   == NULL /*&& //Ok 1
		GetTile(i1+1, j1+1, bGetMouse) == NULL*/)
	{
		element->SetTile(6);
	}
	else
	
	//Is slot 7?
	if( GetTile(i1-1, j1-1, bGetMouse) != NULL &&
		GetTile(i1, j1-1, bGetMouse)   != NULL &&
		GetTile(i1+1, j1-1, bGetMouse) != NULL &&
		
		GetTile(i1-1, j1, bGetMouse)   != NULL &&
		GetTile(i1, j1, bGetMouse)     != NULL &&
		GetTile(i1+1, j1, bGetMouse)   != NULL &&
		
		//GetTile(i1-1, j1+1, bGetMouse) == NULL &&	//ok   
		GetTile(i1, j1+1, bGetMouse)   == NULL /*&& //ok
		GetTile(i1+1, j1+1, bGetMouse) == NULL*/)
	{
		element->SetTile(7);
	}
	else
	
	//Is slot 8?
	if( GetTile(i1-1, j1-1, bGetMouse) != NULL &&
		GetTile(i1, j1-1, bGetMouse)   != NULL &&
		//GetTile(i1+1, j1-1, bGetMouse) == NULL && //Ok 1
		
		GetTile(i1-1, j1, bGetMouse)   != NULL &&
		GetTile(i1, j1, bGetMouse)     != NULL &&
		GetTile(i1+1, j1, bGetMouse)   == NULL &&
		
		//GetTile(i1-1, j1+1, bGetMouse) == NULL && //Ok 1	   
		GetTile(i1, j1+1, bGetMouse)   == NULL &&
		GetTile(i1+1, j1+1, bGetMouse) == NULL)
	{
		element->SetTile(8);
	}
	else
	
	//Is slot 9?
	if( GetTile(i1-1, j1-1, bGetMouse) == NULL &&
		GetTile(i1, j1-1, bGetMouse)   != NULL &&
		GetTile(i1+1, j1-1, bGetMouse) != NULL &&
		
		GetTile(i1-1, j1, bGetMouse)   != NULL &&
		GetTile(i1, j1, bGetMouse)     != NULL &&
		GetTile(i1+1, j1, bGetMouse)   != NULL &&
		
		GetTile(i1-1, j1+1, bGetMouse) != NULL &&	   
		GetTile(i1, j1+1, bGetMouse)   != NULL &&
		GetTile(i1+1, j1+1, bGetMouse) != NULL)
	{
		element->SetTile(9);
	}
	else
	
	//Is slot 10?
	if( GetTile(i1-1, j1-1, bGetMouse) != NULL &&
		GetTile(i1, j1-1, bGetMouse)   != NULL &&
		GetTile(i1+1, j1-1, bGetMouse) == NULL &&
		
		GetTile(i1-1, j1, bGetMouse)   != NULL &&
		GetTile(i1, j1, bGetMouse)     != NULL &&
		GetTile(i1+1, j1, bGetMouse)   != NULL &&
		
		GetTile(i1-1, j1+1, bGetMouse) != NULL &&	   
		GetTile(i1, j1+1, bGetMouse)   != NULL &&
		GetTile(i1+1, j1+1, bGetMouse) != NULL)
	{
		element->SetTile(10);
	}
	else
	
	//Is slot 11?
	if( GetTile(i1-1, j1-1, bGetMouse) != NULL &&
		GetTile(i1, j1-1, bGetMouse)   != NULL &&
		GetTile(i1+1, j1-1, bGetMouse) != NULL &&
		
		GetTile(i1-1, j1, bGetMouse)   != NULL &&
		GetTile(i1, j1, bGetMouse)     != NULL &&
		GetTile(i1+1, j1, bGetMouse)   != NULL &&
		
		GetTile(i1-1, j1+1, bGetMouse) == NULL &&	   
		GetTile(i1, j1+1, bGetMouse)   != NULL &&
		GetTile(i1+1, j1+1, bGetMouse) != NULL)
	{
		element->SetTile(11);
	}
	else
	
	//Is slot 12?
	if( GetTile(i1-1, j1-1, bGetMouse) != NULL &&
		GetTile(i1, j1-1, bGetMouse)   != NULL &&
		GetTile(i1+1, j1-1, bGetMouse) != NULL &&
		
		GetTile(i1-1, j1, bGetMouse)   != NULL &&
		GetTile(i1, j1, bGetMouse)     != NULL &&
		GetTile(i1+1, j1, bGetMouse)   != NULL &&
		
		GetTile(i1-1, j1+1, bGetMouse) != NULL &&	   
		GetTile(i1, j1+1, bGetMouse)   != NULL &&
		GetTile(i1+1, j1+1, bGetMouse) == NULL)
	{
		element->SetTile(12);
	}
	else
	
	//Is slot 13?
	if( GetTile(i1-1, j1-1, bGetMouse) != NULL &&
		GetTile(i1, j1-1, bGetMouse)   != NULL &&
		GetTile(i1+1, j1-1, bGetMouse) == NULL &&
		
		GetTile(i1-1, j1, bGetMouse)   != NULL &&
		GetTile(i1, j1, bGetMouse)     != NULL &&
		GetTile(i1+1, j1, bGetMouse)   != NULL &&
		
		GetTile(i1-1, j1+1, bGetMouse) == NULL &&	   
		GetTile(i1, j1+1, bGetMouse)   != NULL &&
		GetTile(i1+1, j1+1, bGetMouse) != NULL)
	{
		element->SetTile(13);
	}
	else
	
	//Is slot 14?
	if( GetTile(i1-1, j1-1, bGetMouse) == NULL &&
		GetTile(i1, j1-1, bGetMouse)   != NULL &&
		GetTile(i1+1, j1-1, bGetMouse) != NULL &&
		
		GetTile(i1-1, j1, bGetMouse)   != NULL &&
		GetTile(i1, j1, bGetMouse)     != NULL &&
		GetTile(i1+1, j1, bGetMouse)   != NULL &&
		
		GetTile(i1-1, j1+1, bGetMouse) != NULL &&	   
		GetTile(i1, j1+1, bGetMouse)   != NULL &&
		GetTile(i1+1, j1+1, bGetMouse) == NULL)
	{
		element->SetTile(14);
	}
	else
	{
		if(n > 0) element->SetTile(n);
	}
														
}

void Tile::SetAnimation(const gedString &animationName)
{
	CreateTileCursor(animationName);
	this->animationName = animationName;
	tileSize = tileCursor[0].getSprite()->GetAction()->getFrameWidth();

	if(!HasAnimation(animationName))
	{
		mapAnimations.Add(nAnimations, animationName);
		nAnimations++;
	}
}

bool Tile::HasAnimation(const gedString &animationName)
{
	MapAnimationsIterator itAnim(mapAnimations);	
	for(itAnim.Begin(); !itAnim.Done(); itAnim.Next())
	{
		if(animationName == *itAnim.Value())
		{
			return true;
		}
	}

	return false;
}

void Tile::RemoveAnimation(const gedString &removeAnimationName)
{
	MapTileIterator it(mapTile);
	bool bRemoved;
	
	//Without external while don't remove all tiles
	do
	{
		bRemoved = false;
		for(it.Begin(); !it.Done(); it.Next())
		{
			Uint32 coord = *it.Key();
			TileElement *element = *it.Value();
			
			if(AnimationName(element->getAnimationIndex()) == removeAnimationName)
			{
				
				mapTile.Remove(coord);
				delete element;
				bRemoved = true;
				break;
			}	
		}

	} while (bRemoved);


	
	MapAnimationsIterator itAnim(mapAnimations);	
	for(itAnim.Begin(); !itAnim.Done(); itAnim.Next())
	{
		if(*itAnim.Value() == removeAnimationName)
		{
			*itAnim.Value() = "ged Invalid Anim";
			//Don't break due repetid animations (old files?)
		}
	}

	if(animationName == removeAnimationName)
	{
		//Get first valid animation
		for(itAnim.Begin(); !itAnim.Done(); itAnim.Next())
		{
			if(*itAnim.Value() != "ged Invalid Anim")
			{
				SetAnimation(*itAnim.Value());
				break;
			}
		}
	}

	SetVisibleCursor(false);
}

void Tile::SetSingleMode(bool bSingle)
{
	for(int i = 1; i < 9; i++)
	{
		tileCursor[i].getSprite()->SetVisible(!bSingle);
	}

	this->bSingle = bSingle;
}

void Tile::OnKeyDown(SDLKey key)
{
	if(bSingle)
	{
		if(key == SDLK_LEFT)
		{
			int frame = tileCursor[0].getSprite()->Frame() - 1;
			if(frame < 0) 
			{
				frame = tileCursor[0].getSprite()->NumFrames() - 1;
			}

			tileCursor[0].SetTile(frame);
		}
		else if(key == SDLK_RIGHT)
		{
			int frame = (tileCursor[0].getSprite()->Frame() + 1) % tileCursor[0].getSprite()->NumFrames();
			tileCursor[0].SetTile(frame);
		}
	}
}

#endif //#ifndef STAND_ALONE_GAME