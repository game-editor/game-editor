// ListBox.h: interface for the ListBox class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_LISTBOX_H__DCF343E1_36EE_430E_A06B_3814AB32DCEF__INCLUDED_)
#define AFX_LISTBOX_H__DCF343E1_36EE_430E_A06B_3814AB32DCEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "engine/kyra.h"
#include "Actor.h"

class ListBox  
{
public:
	int Count();
	gedString GetText(int id);
	void RemoveAll();
	void RemoveText(gedString text);
	void AddText(gedString text);

	int Left();
	int Width();
	int Top();
	int Right();
	int Down();
	int Height();

	Actor *getParent() {return parent;};
	int getListId() {return listId;};

	ListBox(int x = 0, int y = 0, int width = 100, int height = 50, int listId = -1, Actor *parent = NULL, gedString fontName = "maksfont.bmp");
	virtual ~ListBox();

	class ListBoxWidget : public KrListBox		
	{
	public:
		ListBoxWidget(KrFontResource* font,	int width, int height, 
			int scrollWidth,
			const KrScheme& scheme,
			ListBox *parent
			)
			: KrListBox(font, width,  height, scrollWidth, scheme)
		{
			this->parent = parent;
		}
		
		bool HandleWidgetEvent(	KrWidget* source, 
			U32 event, 
			U32 data, 
			const SDL_Event* sdlEvent,
			const char* command, 
			const char* arg );
		
		ListBox *parent;
	};

private:
	ListBoxWidget *list;
	KrFontResource *fontResource;
	int listId;
	Actor *parent;
};

#endif // !defined(AFX_LISTBOX_H__DCF343E1_36EE_430E_A06B_3814AB32DCEF__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME