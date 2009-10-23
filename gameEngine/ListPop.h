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


// ListPop.h: interface for the ListPop class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_LISTPOP_H__0DBF21CF_82E7_461B_BC1F_0D398987904C__INCLUDED_)
#define AFX_LISTPOP_H__0DBF21CF_82E7_461B_BC1F_0D398987904C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Button.h"

class WaitCursor;

class stListData
{
public:
	stListData() {data = NULL;}
		
	stListData(const gedString& _text, void *_data) 
	{ 
		text = _text;
		data = _data;
	}

	void operator=( const stListData& copy ) 
	{ 
		text = copy.text;
		data = copy.data;
	}

	bool operator==( const stListData& copy ) 
	{ 
		return text == copy.text;
	}

	bool operator<( const stListData& copy ) 
	{
		/*
		Considerations:
		
		Put drivers first
		(..) always < anything and > driver letter
		Use case insensitive comparation
		*/
		
		if(text.length() == 4)
		{
			if( text.at(0) == '(' && text.at(3) == ')' && 
				(copy.text.at(0) != '(' || copy.text.at(3) != ')') )
			{
				return true;
			}
		}
		
		if(copy.text.length() == 4)
		{
			if( copy.text.at(0) == '(' && copy.text.at(3) == ')' && 
				(text.at(0) != '(' || text.at(3) != ')') )
			{
				return false;
			}
		}

		if(text.length() == 4 && copy.text.length() == 4)
		{
			if( text.at(0) == '(' && text.at(3) == ')' &&
				copy.text.at(0) == '(' && copy.text.at(3) == ')')
			{
				if(text.at(1) == '.') return false;
				if(copy.text.at(1) == '.') return true;
			}
		}

		//return text < copy.text;
		return text.compare(copy.text, false) < 0;
	}

	gedString text;
	void *data;
	
};

typedef GlDynArray<stListData> ListData;  
 

typedef GlMap< int, void *, GlNumberHash<int> >		MapItemData;
typedef GlMapIterator< int, void *, GlNumberHash<int> >		MapItemDataIterator;

typedef GlSList<gedString> StackItens;  //push = PushFront, pop = PopFront, top = Front
typedef GlSList<int> StackIndex;


#define MAX_COLS		5
#define TEXT_MARGIN		10
#define LESS_ITENS		"<<< less"
#define MORE_ITENS		"more >>>"

#define LIST_SEPARATOR	"##"

class ListPopSelect;
class ListPop : public Button  
{
public:
	void setCenterListOnMouse(bool value) {bCenterListOnMouse = value;}
	void AddSeparator(int insertAt = -1);

	//void setListName(const gedString& name) {listName = name;}
	void DisableHideOnClickOutList() {bHideOnClickOutList = false;};
	void SetItemData(int _i, void *data);
	void *GetItemData(int _i);
	void *GetItemData(const gedString& text);
	gedString *GetTextPtr(int _i);
	gedString *GetTextPtr(const gedString& text);
	void Reset();
	void Sort();
	int SetItem(const gedString& text);
	void SetItem(int index);
	bool RemoveText(const gedString& text);
	int Count();
	gedString GetText(int index = -1);
	void RemoveAll();
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	int AddText(const gedString& text, bool bDuplicate = false, int insertAt = -1, void *data = NULL);
	bool OnMouseButtonDown(int x, int y, Uint8 button);	
	ListPop(int x = 0, int y = 0,
			   int width = 0, int height = 0,
			   int listpopId = -1,
			   Actor *parent = NULL,
			   gedString buttonText = "");
	virtual ~ListPop();

	int GetSelectedIndex() {return index;}

	ListData &getItens() {return *pCurrentList;}

	void ResetListRefresh() {UseMainList(); firstListItem = -1;} //Force repopulate

	void SetListText(gedString text);

private:
	void ResetWork();
	void PopulateWorkList(bool bForward);
	void PopulateCloneList(gedString actorName);
	void CalcDimensions();
	

	void UseSubList();
	bool UseMainList();
	

	bool bButtonText;
	

	int heightItem;
	int widthItem;
	int itensCols[MAX_COLS+1];
	int widthList;
	int heightList;
	int nCols;
	int index;
	int leftMargin, topMargin;
	bool bGrow, bHideOnClickOutList;

	StackIndex stackItens, stackSubItens, *pCurrentStack;

	//Must save when use sub lists
	int main_heightItem;
	int main_widthItem;
	int main_itensCols[MAX_COLS+1];
	int main_widthList;
	int main_heightList;
	int main_nCols;
	int main_index;
	int main_leftMargin, main_topMargin;
	bool main_bGrow;
	int main_firstListItem, main_lastListItem;

	gedString selectedText;
	

protected:
	int AddTextWork(const gedString& text, bool bDuplicate = false);
		
	int listpopId;
	int xMouse, yMouse;
	int firstListItem, lastListItem;

	ListData allItens, subItens, *pCurrentList;
	ListString itens;

	bool bCenterListOnMouse;
	bool bListDirectory;

	
	//gedString listName;
};

#endif // !defined(AFX_LISTPOP_H__0DBF21CF_82E7_461B_BC1F_0D398987904C__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME