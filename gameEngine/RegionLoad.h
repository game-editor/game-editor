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


// RegionLoad.h: interface for the RegionLoad class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGIONLOAD_H__A5B57EF1_487C_413D_B7A2_DDAA61E17BFF__INCLUDED_)
#define AFX_REGIONLOAD_H__A5B57EF1_487C_413D_B7A2_DDAA61E17BFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Actor.h"
#include "GameControl.h"


typedef GlMap< gedString, char, GlStringHash >		MapRegionActors;
typedef GlMapIterator< gedString, char, GlStringHash >		MapRegionActorsIterator;


class RegionLoadSetDefault;
class RegionLoad : public Actor  
{
friend class RegionLoadSetDefault;  
public:

#ifndef ACTOR_USES_VIRTUAL_FUNCTIONS
	//Overload the memory alocators to avoid crash when delete the class derived from Actor
	void *operator new(size_t size)	{return dlmalloc(size);}
	void operator delete(void * mem) {dlfree(mem);}
#endif

#ifndef STAND_ALONE_GAME
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void Save(SDL_RWops *src);
	static void SaveRegions(SDL_RWops *src);
	static bool getShowRegions() {return bShowRegions;}
	static void DisableMouse();
	static void EnableMouse();
#endif //#ifndef STAND_ALONE_GAME

	static void AddActorInRegionTemp(const gedString& name);
	static void CreateRegionTemp();
	static int Count() {return regions.size();}
	
	bool OnMouseButtonDown(int x, int y, Uint8 button);
	static void ToggleVisibility();
	static void ClearRegions();
	static RegionLoad *getDefaultRegion() {return defaultRegion;}
	static bool getCreatingActors() {return bCreatingActors;}
	static void UpdateRegions();
	static void DestroyRegions();
	
	static void LoadRegions(SDL_RWops *src);
	bool HaveActor(const gedString &name) {return regionActors[name] != NULL;}
	static void AddActorToDefaultRegion(Actor *actor) {if(!actor->isView() && defaultRegion) defaultRegion->createdActors.Add(actor, 1);}
	static void UpdateView();

	void RemoveActor(Actor *actor);
	void AddActor(Actor *actor);

	bool IsRegionInView() {return bRegionInView;}
	
	
	void Load(SDL_RWops *src);	
	RegionLoad();
	/*virtual*/ ~RegionLoad();

private:
	void DefineActors();

	static void SetDefaultRegionView();
	void RemoveActors();

	void DestroyActors();
	void CreateActors();
	void CallOnCreate();

	static void SetDefaultRegion(RegionLoad *pRegion);
	static void InvalidateDefaultRegion();
	static void RemoveCommonActorsFromOldDefaultRegion(RegionLoad *pOldDefaultRegion);
	

#ifndef STAND_ALONE_GAME
	ListPop *list;
#endif //#ifndef STAND_ALONE_GAME

	MapRegionActors		regionActors;
	GlobalMapActor		createdActors;

	bool bRegionInView;

	static MapRegions regions;			//Contains all regions
	static KrRect viewPosAnt;			//Update view optimization
	static bool bCreatingActors;		//In CreateActor funcion indicator
	static bool bShowRegions;
	static RegionLoad *defaultRegion;	//Any region in view (To actors created at run time without regions, like script reference actors...)
	static RegionLoad *regionTemp;		//Used when no regions has been defined
};

class RegionLoadSetDefault
{
public:
	RegionLoadSetDefault(Actor *actor);
	~RegionLoadSetDefault();

private:
	RegionLoad *oldDefaultRegion;
};

#endif // !defined(AFX_REGIONLOAD_H__A5B57EF1_487C_413D_B7A2_DDAA61E17BFF__INCLUDED_)
