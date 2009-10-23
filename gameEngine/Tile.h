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


// Tile.h: interface for the Tile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILE_H__4355234C_DFC5_4D9B_B420_5032AE1E33F2__INCLUDED_)
#define AFX_TILE_H__4355234C_DFC5_4D9B_B420_5032AE1E33F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Actor.h"

typedef  Uint32 TilePoint; //Two bytes per coordinate

class TileElement
{
public:
	void RandomTile();
	void setAnimationIndex(int index) {animationIndex = index;}
	Uint16 getAnimationIndex() {return animationIndex;}
	Uint8 getSlot() {return tileSlot;}
	void CreateSprite(Actor *actor, KrSprite *parent);
	void SetTile(int index);
	TileElement();
	virtual ~TileElement();

	KrSprite *getSprite() {return sprite;}

private:
	KrSprite *sprite;
	Uint8 tileSlot;
	Uint16 animationIndex;
};

typedef GlMap< TilePoint, TileElement *, GlNumberHash<TilePoint> >		MapTile;
typedef GlMapIterator< TilePoint, TileElement *, GlNumberHash<TilePoint> >		MapTileIterator;

typedef GlMap< int, gedString, GlNumberHash<int> >		MapAnimations;
typedef GlMapIterator< int, gedString, GlNumberHash<int> >		MapAnimationsIterator;


class Tile  
{
public:
	int GetArea();
	int Width();
	int Height();

	KrImage *getTileImage() {return parentSprite;}
	
	void Load(SDL_RWops *src);
	void Equal(Tile *copyTile);
	void SetVisibleCursor(bool bVisible);
	int NumTiles() {return mapTile.size();}
	void SetColorCursor(int color);
	void SetDrawMode(bool bDraw);
	const gedString AnimationName(int i);

#ifndef STAND_ALONE_GAME
	void SetRandom(bool bRand) {bRandom = bRand;}
	bool IsSingle() {return bSingle;}
	void OnKeyDown(SDLKey key);
	void SetSingleMode(bool bSingle);
	int getTileSize() {return tileSize;}
	bool HasAnimation(const gedString &animationName);
	void RemoveAnimation(const gedString &removeAnimationName);
	
	

	void Save(SDL_RWops *src);
	
	
	void SetAnimation(const gedString &animationName);
	
	void setAdd(bool bEnable) {bEnableAdd = bEnable;}
	void AdjustTileSlot(TileElement *element, int i1, int j1, int n = -1, bool bGetMouse = true);
	TileElement *GetTile(int i, int j, bool bGetMouse = true);
	void GridSnap(int &x, int &y);
	Uint32 Coord(int i, int j);
	void OnMouseMove(int x, int y);
	bool AddTile();

	int GetFrame() {return tileCursor[0].getSprite()->Frame();}
	bool IsVisible() {return bVisibleState;}
	Actor *getParentActor() {return actor;}
#endif //#ifndef STAND_ALONE_GAME

	
	Tile(Actor *actor);
	virtual ~Tile();

	const gedString& getAnimationName() {return animationName;}
	const MapTile& getMapTile() {return mapTile;}
	

private:
	void CreateTileCursor(const gedString &animationName);
	void CalcDimensions();
	
	MapTile mapTile;
	MapAnimations mapAnimations;

	Uint32 nAnimations;
	Uint32 tileSize;		//Size of tile square
	Uint32 inix, iniy;		//Initial tile coordinates
	Uint32 actualx, actualy;
	Uint32 iMouse, jMouse;
	gedString animationName;

	int xscreen, yscreen;

	//Dont save
	Uint32 color;
	bool bInit, bCursorCreated, bSingle, bRandom;
	bool bEnableAdd, bVisibleState;
	Actor *actor;
	KrSprite *parentSprite;

	TileElement tileCursor[9]; //3x3 tile cursor
	int area, width, height;
};

#endif // !defined(AFX_TILE_H__4355234C_DFC5_4D9B_B420_5032AE1E33F2__INCLUDED_)
