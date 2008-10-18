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
