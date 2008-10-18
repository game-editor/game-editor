// AddActor.h: interface for the AddActor class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_ADDACTOR_H__ED9C630B_CC5B_4634_BB19_4ADFF0AC3091__INCLUDED_)
#define AFX_ADDACTOR_H__ED9C630B_CC5B_4634_BB19_4ADFF0AC3091__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class AddActor : public Panel  
{
public:
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);

	AddActor(bool _bCallAddAnimation = false);
	virtual ~AddActor();	

private:
	EditBox *name;
	ActorType type;
	bool bCallAddAnimation;
};

#endif // !defined(AFX_ADDACTOR_H__ED9C630B_CC5B_4634_BB19_4ADFF0AC3091__INCLUDED_)

#endif //STAND_ALONE_GAME
