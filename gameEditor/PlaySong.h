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