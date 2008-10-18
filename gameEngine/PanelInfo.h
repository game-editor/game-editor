// PanelInfo.h: interface for the PanelInfo class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_PANELINFO_H__039CF0BB_F2BE_46E8_AD1F_F060BEBDF914__INCLUDED_)
#define AFX_PANELINFO_H__039CF0BB_F2BE_46E8_AD1F_F060BEBDF914__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class PanelInfo : public Panel
{
public:
	PanelInfo(gedString text, gedString title = "Error", int align = ALIGN_CENTER, bool bCreateButton = true);
	virtual ~PanelInfo();
	void OnButton(Button *button, int buttonId);

	static PanelInfo *getLastPanel() {return lastPanel;}

protected:
	int lastLine;

	static PanelInfo *lastPanel;
};

#endif // !defined(AFX_PANELINFO_H__039CF0BB_F2BE_46E8_AD1F_F060BEBDF914__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME