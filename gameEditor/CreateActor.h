// CreateActor.h: interface for the CreateActor class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_CREATEACTOR_H__E82F5FF1_0C62_4B2E_A592_42BABCA5DF48__INCLUDED_)
#define AFX_CREATEACTOR_H__E82F5FF1_0C62_4B2E_A592_42BABCA5DF48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseAction.h"

class CreateActor : public BaseAction  
{
public:
	void UpdateEdition();
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	CreateActor(Actor *actor, bool bOnCollision);
	virtual ~CreateActor();

private:
	ListPop *listActor, *listParent, *listPath, *listAnimation, *listRelativePos;
	Actor *eventActor, *actionActor;
	EditBox *posX, *posY;
};

#endif // !defined(AFX_CREATEACTOR_H__E82F5FF1_0C62_4B2E_A592_42BABCA5DF48__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME