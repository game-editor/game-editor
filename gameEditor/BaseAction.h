// BaseAction.h: interface for the BaseAction class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_BASEACTION_H__8FC213C8_ACD3_4E85_AD7A_CA39B063AFF7__INCLUDED_)
#define AFX_BASEACTION_H__8FC213C8_ACD3_4E85_AD7A_CA39B063AFF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"
#include "../gameEngine/Action.h"

//class stAction;
class BaseAction : public Panel  
{
public:
	void SelectActionType(Actor *eventActor, stAction *action);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message);

	BaseAction(const gedString spriteName, int x, int y, int width, int height,
		   Actor *parent = NULL, bool bInvertBevel = false, bool bDrag = true, bool bPanel = true);

	virtual ~BaseAction();

private:
	ListPop *listSelect;
	stAction *action, copyEditAction;
	Actor *eventActor;
};

#endif // !defined(AFX_BASEACTION_H__8FC213C8_ACD3_4E85_AD7A_CA39B063AFF7__INCLUDED_)

#endif //STAND_ALONE_GAME
