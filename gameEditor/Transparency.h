// Transparency.h: interface for the Transparency class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_TRANSPARENCY_H__35A9AE31_2650_4EB3_A728_9AF64622E949__INCLUDED_)
#define AFX_TRANSPARENCY_H__35A9AE31_2650_4EB3_A728_9AF64622E949__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseAction.h"

class Transparency : public BaseAction  
{
public:
	void UpdateEdition();
	void OnSlide(Slide *slide, int slideId, double pos);
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	Transparency(Actor *actor, bool bOnCollision);
	virtual ~Transparency();

private:
	Slide *slideTrans;
	ListPop *listActor;
	Actor *eventActor, *actionActor;
	double oriTrans;
};

#endif // !defined(AFX_TRANSPARENCY_H__35A9AE31_2650_4EB3_A728_9AF64622E949__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME