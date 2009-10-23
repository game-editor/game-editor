/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com

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
More info at http://game-editor.com/License
Be a Game Editor developer: http://game-editor.com/Sharing_Software_Revenues_in_Open_Source

***************************************************************************/


// ListPop.cpp: implementation of the ListPop class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "ListPop.h"
#include "GameControl.h"
#include "ListPopSelect.h"
#include "../gameEditor/Tutorial.h"


ListPop::ListPop(int x, int y,
			   int width, int height,
			   int listpopId,
			   Actor *parent,
			   gedString buttonText)
: Button(buttonText, x, y, width, height, -1, parent,
			0, 0, 0, (width < 200)?"makslistPop.bmp":"makslistPopBig.bmp", "maksfont.bmp", true)
{
	textBox->SetPos(px, py);
	this->listpopId = listpopId;
	heightItem = Text::GetDimensions("CALIBRATION").Height() + 2;

	bListDirectory = false;
	
	RemoveAll();

	if(buttonText.size()) bButtonText = true;
	else bButtonText = false;	

	bHideOnClickOutList = true;
	//listName = "ListPopSelect";

	bCenterListOnMouse = true;

	Reset();
	pCurrentList = &allItens;
	pCurrentStack = &stackItens;
}

ListPop::~ListPop()
{
	
}


bool ListPop::OnMouseButtonDown(int x, int y, Uint8 button)
{
	if(!bListDirectory && button != 255) 
	{
		if(!getParent()->OnListButtonDown(this, listpopId))
		{
			//Don't execute
			return false;
		}
	}

	if(pCurrentList->Count() == 0 || !bEnable)
		return false;

	WaitCursor wait;

	xMouse = x;
	yMouse = y;

	if(firstListItem == -1)
	{
		//First click		
		firstListItem = 0;
		lastListItem = 0;		
		
		ResetWork();
		PopulateWorkList(true);
	}

	CalcDimensions();
	
	int height = heightList + 2*topMargin;

	//Calc position
	int xmin = x, ymin = y;

	if(bCenterListOnMouse)
	{
		xmin = x - widthList/2,
		ymin = y - height/2;
	}

	if(xmin < 2) xmin = 2;
	if(ymin < 2) ymin = 2;

	if(xmin + widthList > GameControl::Get()->Width())   xmin = GameControl::Get()->Width() - widthList;
	if(ymin + height > GameControl::Get()->Height()) ymin = GameControl::Get()->Height() - height;

	new ListPopSelect(&itens, xmin, ymin, widthList, height, widthItem, heightItem, listpopId, this, leftMargin, topMargin, bHideOnClickOutList/*, listName*/);

	return false;
}

int ListPop::AddTextWork(const gedString& text, bool bDuplicate)
{
	if(nCols >= MAX_COLS-1 || (nCols+2)*widthItem >= GameControl::Get()->Width())
	{
		CalcDimensions();
		bGrow = false;
		
		if((itensCols[nCols]+5)*heightItem >= (GameControl::Get()->Height()-16))
		{
			return -1;
		}		
	}

	if(!bDuplicate)
	{
		for(int i = 0; i < itens.Count(); i++)
		{
			if(text == itens[i]) 
			{
				return i;
			}
		}
	}

	if(bGrow && itensCols[nCols]*heightItem >= (GameControl::Get()->Height()-16))
	{
		nCols++;
		heightList = GameControl::Get()->Height() - 16;
	}

	itensCols[nCols]++;
	

	//Don't grow width, but can grow height
	//Solve the bug: Key Down em asteroids.ged (show only two actions when use 640x480)
	//   http://game-editor.com/forum/viewtopic.php?t=901
	//   http://game-editor.com/forum/viewtopic.php?t=1077

	//if(bGrow)
	{
		widthItem = max(widthItem, Text::GetDimensions(text).Width() + TEXT_MARGIN);
		//if(nCols) 
		//	widthItem = min(widthItem, GameControl::Get()->Width()/MAX_COLS);
		heightList = max(heightList, itensCols[nCols]*heightItem);
	}

	itens.PushBack(text);
	return itens.Count() - 1;
}

int ListPop::AddText(const gedString& text, bool bDuplicate, int insertAt, void *data)
{
	if(!bDuplicate)
	{
		for(int i = 0; i < pCurrentList->Count(); i++)
		{
			if(text == (*pCurrentList)[i].text) 
			{
				return i;
			}
		}
	}

	firstListItem = -1;

	if(insertAt < 0) 
	{
		pCurrentList->PushBack(stListData(text, data));
		return pCurrentList->Count() - 1;
	}
	else
	{
		if(insertAt > pCurrentList->Count())
		{
			insertAt = pCurrentList->Count();
		}

		pCurrentList->Insert(stListData(text, data), insertAt);
		return insertAt;
	}	
}

void ListPop::AddSeparator(int insertAt)
{
	AddText(LIST_SEPARATOR, true, insertAt);
}


bool ListPop::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(text == LIST_SEPARATOR) return true;

	int realFps = GameControl::Get()->getRealFrameRate();
	if(realFps <= 0) realFps = 1;

	int interval = (int)(1000.0/realFps);
	selectedText.clear();

	if(text[0] == '+')
	{
		//Expand clone names
		PopulateCloneList(&text.c_str()[2]);
		OnMouseButtonDown(GameControl::Get()->getMouseX(), GameControl::Get()->getMouseY(), 255);
	}
	else if(text == MORE_ITENS)
	{
		pCurrentStack->PushFront(firstListItem);
		ResetWork();
		AddTextWork(LESS_ITENS);
		PopulateWorkList(true);
		
		
		OnMouseButtonDown(xMouse, yMouse, 255);
	}
	else if(text == LESS_ITENS)
	{
		firstListItem = pCurrentStack->Front();
		pCurrentStack->PopFront();			
		
		ResetWork();
		if(pCurrentStack->size()) AddTextWork(LESS_ITENS);
		
		PopulateWorkList(false);
		
		
		OnMouseButtonDown(xMouse, yMouse, 255);
	}
	else
	{
		if(!bButtonText) SetListText(text);
		selectedText = text;

		if(listId < 0) listId = listpopId;
		if(index < 0) 
		{
			//Get index from text
			UseMainList();
			
			for(int i = 0; i < pCurrentList->Count(); i++)
			{
				if(text == (*pCurrentList)[i].text)
				{
					index = i;
					break;
				}
			}
			
			if(index >= 0) this->index = index;
		}
		else
		{
			if(firstListItem > 0) index = firstListItem + index - 1; //Work index to all index
			UseMainList();

			//Put after UseMainList() to get the
			//correct clone in the panel of getclone function
			this->index = index;
		}

		
		firstListItem = -1;
		getParent()->OnList(this, index, text, listId);
	}	

	return true;
}

void ListPop::RemoveAll()
{
	heightList = widthItem = nCols = 0;
	memset(itensCols, 0, MAX_COLS*sizeof(int));
	
	itens.Clear();
	allItens.Clear();	

	lastListItem = firstListItem = -1;
	stackItens.Clear();
}

gedString ListPop::GetText(int index)
{
	if(index == -1)
	{
		if(!selectedText.empty()) return selectedText;
		else index = GetSelectedIndex();
	}
	else if(index < 0) index = 0;
	else if(index > pCurrentList->Count() - 1) index = pCurrentList->Count() - 1;


	if(index >= 0)
		return (*pCurrentList)[index].text;

	return "";
}



int ListPop::Count()
{
	return pCurrentList->Count();
}

bool ListPop::RemoveText(const gedString& text)
{
	int s1 = pCurrentList->Count();
	pCurrentList->FindAndDelete(stListData(text, 0));
	int s2 = pCurrentList->Count();

	return s2 < s1;
}

void ListPop::SetItem(int index)
{
	for(int i = 0; i < pCurrentList->Count(); i++)
	{
		if(i == index)
		{
			SetListText((*pCurrentList)[i].text);
			selectedText = (*pCurrentList)[i].text;
			this->index = index;
			return;
		}
	}

	this->index = -1;
	selectedText.clear();
	SetListText("");
}

int ListPop::SetItem(const gedString& text)
{
	index = 0;

	gedString textCmp(text);
	if(pCurrentList->Count() && (*pCurrentList)[0].text[0] == '+')
	{
		//Has sub lists
		//Remove clone index
		int i = textCmp.find('.');

		if(i != gedString::npos)
		{
			textCmp = textCmp.substr(0, i);
		}
	}

	for(int i = 0; i < pCurrentList->Count(); i++)
	{
		gedString itemText((*pCurrentList)[i].text);
		if(itemText[0] == '+')
		{
			//Remove expander
			itemText = itemText.substr(2);
		}

		if(textCmp == itemText)
		{
			SetListText(text);
			selectedText = text;
			return index;
		}
		index++;
	}

	index = -1;
	selectedText.clear();
	SetListText("");
	return -1;
}

void ListPop::Sort()
{
	if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		pCurrentList->Sort();
	}
	else
	{
		//Solve the any key tutorial (don't sort two elements list)
		pCurrentList->Sort(true);
	}
	
}

void ListPop::CalcDimensions()
{
	leftMargin = TEXT_MARGIN/2;
	topMargin = 2;

	widthList = (nCols+1)*widthItem;

	if(widthList < 90)
	{
		leftMargin = (90 - widthList)/2;
		widthList = 90;
	}


	if(heightList < 45)
	{
		topMargin = (45 - heightList)/2;
		heightList = 45;
	}
}

void ListPop::Reset()
{
	index = -1;
	bGrow = true;
	RemoveAll();
}

void ListPop::ResetWork()
{
	bGrow = true;
	heightList = widthItem = nCols = 0;
	memset(itensCols, 0, MAX_COLS*sizeof(int));	
	itens.Clear();
}

void ListPop::SetItemData(int _i, void *data)
{
	for(int i = 0; i < pCurrentList->Count(); i++)
	{
		if(i == _i)
		{
			(*pCurrentList)[i].data = data;
			break;
		}
	}		
}

void *ListPop::GetItemData(int _i)
{
	for(int i = 0; i < pCurrentList->Count(); i++)
	{
		if(i == _i)
		{
			return (*pCurrentList)[i].data;			
		}
	}
	
	return NULL;
}

void *ListPop::GetItemData(const gedString& text)
{
	for(int i = 0; i < pCurrentList->Count(); i++)
	{
		if(text == (*pCurrentList)[i].text)
		{
			return (*pCurrentList)[i].data;			
		}
	}
	
	return NULL;
}

gedString *ListPop::GetTextPtr(int _i)
{
	for(int i = 0; i < pCurrentList->Count(); i++)
	{
		if(i == _i)
		{
			return &((*pCurrentList)[i].text);			
		}
	}
	
	return NULL;
}

gedString *ListPop::GetTextPtr(const gedString& text)
{
	for(int i = 0; i < pCurrentList->Count(); i++)
	{
		if(text == (*pCurrentList)[i].text)
		{
			return &((*pCurrentList)[i].text);
		}
	}
	
	return NULL;
}

void ListPop::UseSubList()
{
	if(pCurrentList == &allItens)
	{
		pCurrentList = &subItens;
		pCurrentStack = &stackSubItens;
		
		//Save main list properties
		main_heightItem = heightItem;
		main_widthItem = widthItem;	
		main_widthList = widthList;
		main_heightList = heightList;
		main_nCols = nCols;
		main_index = index;
		main_leftMargin = leftMargin;
		main_topMargin = topMargin;
		main_bGrow = bGrow;
		main_firstListItem = firstListItem;
		main_lastListItem = lastListItem;
		
		memcpy(main_itensCols, itensCols, MAX_COLS*sizeof(int));
		
		//Reset properties
		heightList = widthItem = nCols = 0;
		memset(itensCols, 0, MAX_COLS*sizeof(int));
		lastListItem = firstListItem = -1;
	}
}

bool ListPop::UseMainList()
{
	if(pCurrentList == &subItens)
	{
		pCurrentList = &allItens;
		pCurrentStack = &stackItens;
		
		//Restore main list properties
		heightItem = main_heightItem;
		widthItem = main_widthItem;	
		widthList = main_widthList;
		heightList = main_heightList;
		nCols = main_nCols;
		index = main_index;
		leftMargin = main_leftMargin;
		topMargin = main_topMargin;
		bGrow = main_bGrow;
		firstListItem = main_firstListItem;
		lastListItem = main_lastListItem;
		
		memcpy(itensCols, main_itensCols, MAX_COLS*sizeof(int));

		return true;
	}

	return false;
}

void ListPop::PopulateCloneList(gedString actorName)
{
	subItens.Clear();
	stackItens.Clear();
	UseSubList();

	ListActor *listActor = mapActors[actorName];
	for(int il = 0; listActor && il < listActor->Count(); il++)
	{
		Actor *actor = (*listActor)[il];
		if(actor->EditMode())
		{
			AddText(actor->getCloneName());
		}
	}

	Sort();
	
	ResetWork();
	PopulateWorkList(true);
}

void ListPop::PopulateWorkList(bool bForward)
{
	//Populate
	int i;

	if(bForward) i = lastListItem;
	else i = firstListItem;

	firstListItem = i;

	for(; i < pCurrentList->Count(); i++)
	{
#if defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL)
		if(!Tutorial::IsOff() && (*pCurrentList)[i].text == BUY_NOW)
			continue; //Don't show to avoid turorial bugs
#endif

		if(i >= 0 && AddTextWork((*pCurrentList)[i].text, true) == -1)
		{
			itens.PopBack();
			itens.PushBack(MORE_ITENS);
			i--;
			break;
		}
	}

	lastListItem = i;
}

void ListPop::SetListText(gedString text)
{
	int width = Width() - 20, n;
	KrRect rect;
	gedString s(text);

	rect.Set(0, 0, 0, 0);
	
	if(s.length() > 5)
	{
		for(n = 1; n <= text.length() && rect.Width() < width; n++)
		{
			s = text.substr(0, n);
			rect = Text::GetDimensions(s);		
		}
		
		if(s.length() != text.length() && n > 3)
		{
			s = s.substr(0, s.length() - 3) + "...";
			SetToolTip(text);
		}
	}
	
	SetText(s);
	selectedText = text;
}


#endif //#ifndef STAND_ALONE_GAME






