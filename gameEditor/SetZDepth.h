// SetZDepth.h: interface for the SetZDepth class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_SETZDEPTH_H__F1A52FEB_2757_4437_9995_959F8E8AC52F__INCLUDED_)
#define AFX_SETZDEPTH_H__F1A52FEB_2757_4437_9995_959F8E8AC52F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseAction.h"

class SetZDepth : public BaseAction  
{
public:
	void UpdateEdition();
	void OnSlide(Slide *slide, int slideId, double pos);
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	SetZDepth(Actor *actor, bool bOnCollision);
	virtual ~SetZDepth();

private:
	Slide *slide;
	ListPop *listActor;
	Actor *eventActor, *actionActor;
	int		oriZDepth;
};

#endif // !defined(AFX_SETZDEPTH_H__F1A52FEB_2757_4437_9995_959F8E8AC52F__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME