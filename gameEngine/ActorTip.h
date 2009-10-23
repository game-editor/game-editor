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


// ActorTip.h: interface for the ActorTip class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_ACTORTIP_H__4B8EF10D_3478_463F_BD87_A4E290864054__INCLUDED_)
#define AFX_ACTORTIP_H__4B8EF10D_3478_463F_BD87_A4E290864054__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Actor.h"

class ActorTip : public Actor  
{
public:
	static void Release();
	static void Create(gedString *tip);
	static void Create(Actor *actor);
	static void ResetLast() {lastActor = NULL;}
	ActorTip(const gedString *tip);
	virtual ~ActorTip();

private:
	static ActorTip *actorTip;
	static Actor *lastActor;
};

#endif // !defined(AFX_ACTORTIP_H__4B8EF10D_3478_463F_BD87_A4E290864054__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME