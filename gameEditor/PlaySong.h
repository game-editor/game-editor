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


// PlaySong.h: interface for the PlaySong class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_PLAYSONG_H__9BDFE49E_55A5_4934_97E7_6F610F10864F__INCLUDED_)
#define AFX_PLAYSONG_H__9BDFE49E_55A5_4934_97E7_6F610F10864F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseAction.h"
#include "SDL_mixer.h"

class PlaySong : public BaseAction  
{
public:
	void Activate();
	void UpdateEdition();
	void OnSlide(Slide *slide, int slideId, double pos);
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	PlaySong(Actor *actor, bool bOnCollision, bool bMusic);
	virtual ~PlaySong();
	static void ResetPath();

private:
	EditBox *loop;
	Slide *slide, *panSlide;
	ListDirectory *path;
	ListPop *musicMode;
	Actor *eventActor;
	bool bMusic;
	Mix_Music *music;
	Mix_Chunk *sound;
	int volume;
};

#endif // !defined(AFX_PLAYSONG_H__9BDFE49E_55A5_4934_97E7_6F610F10864F__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME