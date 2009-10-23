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


// Sequence.cpp: implementation of the Sequence class.
//
//////////////////////////////////////////////////////////////////////

#include "Sequence.h"
#include "GameControl.h"
#include <stdlib.h>

#include "dlmalloc.h"


Sequence::Sequence(int type)
{
	sequenceType = type;
	frames = NULL;
	frameInc = 0.0;
	animIndex = -1;
	resetFrame();
}

Sequence::~Sequence()
{
	if(frames)
		delete [] frames;
}

void Sequence::SetFrameSequence(const gedString& frameSequence, int fps)
{
	//Sequence frames separated for ';'
	if(frames)
		delete [] frames;

	nFrames = 0;

	//Count
	gedString aux(frameSequence);
	const char *buf1 = aux.c_str();
	for(int i = 0; i < strlen(buf1); i++)
	{
		if(buf1[i] == ';') 
			nFrames++;
	}
	nFrames++;

	//Store
	frames = new Uint16[nFrames];

	char *buf = (char *)malloc(frameSequence.size()+1);
	strcpy(buf, (char *)aux.c_str());

	char *pToken, sep[] = ";";
	
	
	nFrames = 0;
	pToken = strtok(buf, sep);
	
	while(pToken) 
	{		
		frames[nFrames] = atol(pToken);
		nFrames++;
		pToken = strtok (NULL, sep);
	}
	
	free(buf);
	setFrameRate(fps);
}

bool Sequence::DoStep(bool bForward, int &actualFrame)
{
	actualFrame = frame;
	if(!CanAnim()) return false;

	bool bAnimationFinish = false;

	if(bForward)
	{											
		if(frame+1 == nFrames-1) bAnimationFinish = true;
		frame = (frame + 1) % nFrames;
	}
	else
	{	
		if(frame == -1) frame = nFrames;
		if(frame-1 == 0) bAnimationFinish = true;
		else if(frame == 0) frame = nFrames;
		frame--;
	}

	actualFrame = frame;
	return bAnimationFinish;
}

int Sequence::GetFrame(int index)
{
	if(index < 0) index = 0;
	else if(index >= nFrames) index = nFrames-1;

	if(frames) return frames[index];
	else return 0;
}

#ifndef STAND_ALONE_GAME
void Sequence::Save(SDL_RWops *src)
{
	animationName.Write(src);
	sequenceName.Write(src);


	SDL_WriteLE16(src, nFrames);
	SDL_RWwrite(src, frames, nFrames*sizeof(Uint16), 1);;

	SDL_WriteLE16(src, fps);
}
#endif //#ifndef STAND_ALONE_GAME

void Sequence::Load(SDL_RWops *src, Uint32 version)
{
	animationName.Read(src);
	sequenceName.Read(src);
	
	
	nFrames = GameControl::Read16(src);

	delete [] frames;
	frames = new Uint16[nFrames];
	GameControl::Read(src, frames, nFrames*sizeof(Uint16), 1);
	
	if(version > 7)
	{
		setFrameRate(GameControl::Read16(src));
	}
	else fps = 0;
}

bool Sequence::IsValideFrameSequence(Actor *actor, const gedString& animationName, const gedString& frameSequence, bool bAllowSequence, int maxSequenceFrames)
{
	if(!actor || !actor->getSpriteResource()) return false;

	int maxFrames = 0, n = 0;
	

	if(animationName == ANY_ANIMATION)
	{
		int nAnimations = actor->getSpriteResource()->NumActions();
		for(int i = 0; i < nAnimations; i++)
		{
			KrAction *action = actor->getSpriteResource()->GetActionByIndex(i);
			if(action) 
			{
				n = action->NumFrames();
				maxFrames = max(maxFrames, n);
			}
		}
	}
	else
	{
		if(bAllowSequence)
		{
			Sequence *seq = actor->GetSequence(animationName);
			if(!seq) return false;
			
			
			KrAction *action = actor->getSpriteResource()->GetAction(seq->getAnimationName());
			if(!action) return false;
			
			maxFrames = action->NumFrames();
		}
		else
		{
			KrAction *action = actor->getSpriteResource()->GetAction(animationName);
			if(!action) return false;
			
			maxFrames = action->NumFrames();
		}
	}

	//Verify data
	gedString aux(frameSequence);
	char *buf = (char *)malloc(frameSequence.size()+1);
	strcpy(buf, (char *)aux.c_str());

	for(int i = 0; i < strlen(buf); i++)
	{
		if((buf[i] < '0' || buf[i] > '9') && buf[i] != ';')
		{
			return false;
		}
	}

	char *pToken, sep[] = ";";	

	pToken = strtok(buf, sep);
	
	int count = 0;
	while(pToken) 
	{		
		int n = atol(pToken);
		if(n < 0 || n >= maxFrames)
		{
			free(buf);
			return false;
		}

		pToken = strtok (NULL, sep);
		count++;
	}

	free(buf);
	if(!count) return false;

	if(maxSequenceFrames != -1 && count > maxSequenceFrames)
		return false;

	return true;
}

gedString Sequence::GetFrameString()
{
	gedString str;
	char buf[64];
	
	if(frames)
	{
		for(int i = 0; i < nFrames; i++)
		{
			sprintf(buf, "%ld;", frames[i]);
			str += buf;
		}
	}

	return str;
}

int Sequence::getFrameRate() 
{
	if(fps == 0) setFrameRate(GameControl::Get()->getFrameRate()); 
	return fps;
}

void Sequence::setFrameRate(int fps) 
{
	this->fps = fps;
	frameInc = 0.0;	
}

bool Sequence::CanAnim()
{
	//Verifies if it can animate (depending game frame rate, and animation frame rate)
	//For animation frame rate <= game frame rate

	bool canAnim = false;
	if(fps == 0) setFrameRate(GameControl::Get()->getFrameRate()); 

	if(frameInc == 0.0) canAnim = true;
	
	frameInc += fps/(double)GameControl::Get()->getFrameRate();
	if(frameInc >= 1.0) frameInc = 0.0;

	return canAnim;
}
