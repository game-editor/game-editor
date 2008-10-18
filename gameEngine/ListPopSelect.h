// ListPopSelect.h: interface for the ListPopSelect class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_LISTPOPSELECT_H__0D52572A_E1E8_4D74_B1B5_37E00238DD19__INCLUDED_)
#define AFX_LISTPOPSELECT_H__0D52572A_E1E8_4D74_B1B5_37E00238DD19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"
#include "ListPop.h"

class ListPopSelect : public Panel  
{
public:
	void OnKeyDown(SDLKey key, int repeatLoopCount = 0);
	const stCursor * OnGetCursor();
	bool DrawSelect(int nx, int ny, KrRGBA &color);
	void OnMouseMove(int x, int y);
	void OnMouseButtonUp(int x, int y, Uint8 button);
	bool OnMouseButtonDown(int x, int y, Uint8 button);
	ListPopSelect(ListString* itens, int x, int y, int width, int height, 
		          int widthItem, int heightItem, int listpopId, ListPop *listPop,
				  int leftMargin, int topMargin,
				  bool bHideOnClickOutList);
	virtual ~ListPopSelect();

	static bool getHasListSelect() {return bHasListSelect;}

private:
	ListPop *listPop;
	int listpopId;
	ListString *itens;
	int widthItem, heightItem;
	int width, height;
	int nItens, lines;
	int index;
	int leftMargin, topMargin;
	bool bHideOnClickOutList, bLButtonDown;
	bool bDrawAnt;
	bool bOnExpander, bOnExpandItem;

	static bool bHasListSelect;

};

#endif // !defined(AFX_LISTPOPSELECT_H__0D52572A_E1E8_4D74_B1B5_37E00238DD19__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME