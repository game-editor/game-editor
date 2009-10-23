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


// AddActionDialog.h: interface for the AddActionDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_ADDACTIONDIALOG_H__BF3693E2_EA9E_4F7E_8874_9AA9F3432E49__INCLUDED_)
#define AFX_ADDACTIONDIALOG_H__BF3693E2_EA9E_4F7E_8874_9AA9F3432E49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class stFrameData
{
public:
	stFrameData() {frame = 0; writeTime = 0; bChanged = false;};

	stFrameData(gedString _animationName, int _frame, long _writeTime) 
	{
		animationName = _animationName;
		frame = _frame;
		writeTime = _writeTime;
		bChanged = false;
	}

	void operator=( const stFrameData& copy )
	{ 
		if( &copy != this )
		{
			animationName = copy.animationName;
			frame = copy.frame;
			writeTime = copy.writeTime;
			bChanged = copy.bChanged;
		}
	}

	gedString animationName;
	int frame;
	long writeTime;
	bool bChanged;
};

typedef GlMap< gedString, stFrameData, GlStringHash >		MapFrameData;
typedef GlMapIterator< gedString, stFrameData, GlStringHash >		MapFrameDataIterator;


class ActorProperty;

class AddActionDialog : public Panel  
{
public:
	void UpdateSample(bool bChangeFile);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	bool OnListButtonDown(ListPop *list, int listId);
	void OnButton(Button *button, int buttonId);
	void OnEditChar(EditBox *edit, SDLKey key, int line);
	void OnEditSelection(EditBox *edit);
	void OnAnimation();
	AddActionDialog(ActorProperty *actorProperty, Actor *actor, KrAction *replaceAnimInfo = NULL, bool _bFromWizard = false);
	virtual ~AddActionDialog();
	static void ResetPath();

private:
	void CheckFrameChanges();
	void RemoveEditedFrames();
	bool HasEditedFrames();
	void ConfirmEditedFrames();
	void ContinueWizard(Actor *actor);

	Actor *actor, *animationSample;
	ActorProperty *actorProperty;
	ListDirectory *path;
	ListPop *fileType, *listPlay;
	EditBox *name, *hframes, *vframes, *editFps, *editFrame;
	bool bReplaceFile, bAutopopulateName;
	bool bFromWizard;

	int sampleAdition;

	MapFrameData mapEditedImages;
};

#endif // !defined(AFX_ADDACTIONDIALOG_H__BF3693E2_EA9E_4F7E_8874_9AA9F3432E49__INCLUDED_)

#endif //STAND_ALONE_GAME
