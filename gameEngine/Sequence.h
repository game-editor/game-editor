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


// Sequence.h: interface for the Sequence class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEQUENCE_H__A5E67279_A4A5_4729_9B8F_492D1D683795__INCLUDED_)
#define AFX_SEQUENCE_H__A5E67279_A4A5_4729_9B8F_492D1D683795__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Actor.h"

enum
{
	ANIMATION_SEQUENCE = 0,
	DEFINED_SEQUENCE
};

class Sequence  
{
public:
	void setFrameRate(int fps);
	bool CanAnim();
	int getFrameRate();
	gedString GetFrameString();
	static bool IsValideFrameSequence(Actor *actor, const gedString& animationName, const gedString& frameSequence, bool bAllowSequence = false, int maxSequenceFrames = -1);
	void Load(SDL_RWops *src, Uint32 version);

#ifndef STAND_ALONE_GAME
	void Save(SDL_RWops *src);
#endif //#ifndef STAND_ALONE_GAME

	void resetFrame() {frame = -1;}
	int GetFrame(int index);
	bool DoStep(bool bForward, int &actualFrame);
	Sequence(int type);
	virtual ~Sequence();

	int getType() {return sequenceType;}
	const gedString& getAnimationName() {return animationName;}
	const gedString& getSequenceName() {return sequenceName;}
	void setAnimationName(const gedString& name) {animationName = name;}
	void setSequenceName(const gedString& name) {sequenceName = name;}

	void SetFrameSequence(const gedString& frameSequence, int fps);
	void SetFrame(int i) {frame = i;};

	int getNFrames() {return nFrames;}
	Uint16 *getFrames() {return frames;}
	int getAnimIndex() {return animIndex;}
	void setAnimIndex(int value) {animIndex = value;}

private:
	gedString animationName, sequenceName;
	Uint16 nFrames, fps;
	Uint16 *frames;
	double frameInc;

	//Don't save
	int sequenceType; //Animation or defined sequence
	int frame;

	//For speedup game anim index search
	int animIndex;
};

#endif // !defined(AFX_SEQUENCE_H__A5E67279_A4A5_4729_9B8F_492D1D683795__INCLUDED_)
