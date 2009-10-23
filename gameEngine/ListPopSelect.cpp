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


// ListPopSelect.cpp: implementation of the ListPopSelect class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "ListPopSelect.h"
#include <math.h>
#include "../gameEditor/ActorText.h"


bool ListPopSelect::bHasListSelect = false;
ListPopSelect::ListPopSelect(ListString* itens, int x, int y, int width, int height, 
							 int widthItem, int heightItem, 
							 int listpopId, ListPop *listPop,
							 int leftMargin, int topMargin,
							 bool bHideOnClickOutList)
	: Panel(getRandomName(), x, y, width, height, NULL, false, true, true, true)
{
	SetModal();

	bHasListSelect = true;

	this->itens = itens;
	this->width = width;
	this->height = height;
	this->heightItem = heightItem;
	this->widthItem = widthItem;
	this->listPop = listPop;
	this->listpopId = listpopId;
	this->bHideOnClickOutList = bHideOnClickOutList;
	bLButtonDown = false;
	bDrawAnt = true;
	bOnExpandItem = bOnExpander = false;

	lines = (height-2) / heightItem;

	this->leftMargin	= leftMargin;
	this->topMargin		= topMargin;

	//Populate
	gedString s;
	

	int xi = leftMargin, yi;

	if(itens->Count() > 20)
	{
		yi = 0;
	}
	else
	{
		yi = topMargin;
	}

	for(int i = 0; i < itens->Count(); i++)
	{
		if(yi+heightItem > height-2)
		{
			xi += widthItem;
			yi = 0;
		}

		int r = 0, g = 0, b = 0, ic;
		s = itens->Item(i);

		if((ic = s.find(" RGB:")) != gedString::npos)
		{
			//Get color value
			int n;
			ic += 6;

			n = s.find(',', ic) - ic;
			r = atol(s.substr(ic, n).c_str());
			
			ic += n + 2;
			n = s.find(',', ic) - ic;
			g = atol(s.substr(ic, n).c_str());

			ic += n + 2;
			n = s.find(')', ic) - ic;
			b = atol(s.substr(ic, n).c_str());
		}

		if(s != LIST_SEPARATOR)
		{
			AddText(s, xi, yi+1, 0, r, g, b);
		}
		else
		{
			//Separator
			DrawHLine(yi + heightItem/2, xi, widthItem);
		}

		yi += heightItem;		
	}

	nItens = itens->Count();
	index = -1;
}

ListPopSelect::~ListPopSelect()
{
	bHasListSelect = false;
}

bool ListPopSelect::OnMouseButtonDown(int x, int y, Uint8 button)
{
	bLButtonDown = true;
	return false;
}

void ListPopSelect::OnMouseButtonUp(int x, int y, Uint8 button)
{
	KrRect rect;
	KrVector2T< GlFixed > object;
	bool bClose = false;

	getImage()->ScreenToObject( x, y, &object );
	getImage()->QueryBoundingBox(&rect);
	if(rect.Intersect(x, y))
	{
		if(!bLButtonDown) return;
		bLButtonDown = false;

		OnMouseMove(x,  y);
		
		if(index >=0 && index < nItens)
		{		
			gedString text;
			for(int i = 0; i < itens->Count(); i++)
			{
				if(i == index)
				{
					text = itens->Item(i);
					break;
				}		
			}
			
			if(text != LIST_SEPARATOR)
			{
				ListPop *list = listPop;
				int ind = index, id = listpopId;
				if(bOnExpandItem && !bOnExpander)
				{
					text = text.substr(2);
				}
				
				delete this; //Close list when call font panels
				list->OnList(NULL, ind, text, id);
			}
		}
		else if(bHideOnClickOutList) 
		{
			bClose = true;
		}
	}
	else if(bHideOnClickOutList) 
	{
		bClose = true;
	}	

	if(bClose)
	{
		listPop->PostMessage(listPop->getParent(), LISTPOPSELECT_HIDED, (U32)listPop);
		listPop->ResetListRefresh();
		delete this;
	}
}

void ListPopSelect::OnMouseMove(int x, int y)
{
	static int nyAnt = -1, nxAnt = -1;

	KrVector2T< GlFixed > object;
	getImage()->ScreenToObject(x, y, &object);

	int x1 = object.x.ToInt();
	int y1 = object.y.ToInt();
	int ny = (y1 - topMargin)/heightItem,
		nx = (x1 - leftMargin)/widthItem;

	Draw();

	//Refresh separators
	int xi = leftMargin, yi;
	gedString s;

	if(itens->Count() > 20)
	{
		yi = 0;
	}
	else
	{
		yi = topMargin;
	}

	for(int i = 0; i < itens->Count(); i++)
	{
		if(yi+heightItem > height-2)
		{
			xi += widthItem;
			yi = 0;
		}

		s = itens->Item(i);

		if(s == LIST_SEPARATOR)
		{
			//Separator
			DrawHLine(yi + heightItem/2, xi, widthItem);
		}

		yi += heightItem;		
	}

	//Draw selection
	if(!bDrawAnt || DrawSelect(nxAnt, nyAnt, colorBack))
	{
		index = nx*lines + ny;

		if(index >= 0 && index < nItens)
		{
			gedString text(itens->Item(index));
			if(text != LIST_SEPARATOR)
			{
				if(text[0] == '+')
				{
					//Sub list item
					int xi = (x1 - leftMargin) - nx*widthItem;
					if(xi > 9)
					{
						//On item
						bOnExpander = false;
					}
					else
					{
						//On expander
						bOnExpander = true;
					}

					bOnExpandItem = true;
				}
				else
				{
					bOnExpandItem = false;
				}

				DrawSelect(nx, ny, colorDown);
				bDrawAnt = true;
			}
			else
			{
				bDrawAnt = false;
				bOnExpandItem = bOnExpander = false;
			}
		}
		else
		{
			bOnExpandItem = bOnExpander = false;
		}
	}
	else
	{
		bOnExpandItem = bOnExpander = false;
	}

	getImage()->Invalidate();

	nxAnt = nx;
	nyAnt = ny;
}


bool ListPopSelect::DrawSelect(int nx, int ny, KrRGBA &color)
{
	if(nx < 0 || ny < 0)
		return false;

	int ymin = ny*heightItem + 1 + topMargin,
		ymax = ymin + heightItem - 1,
		xmin = nx*widthItem /*+ 1*/ + leftMargin - 2,
		xmax = xmin + widthItem - 1;

	if(ymax > height) return false;

	if(ymin < 2) ymin = 2;
	else if(ymax >= height - 2) ymax = height - 2;

	if(xmin < 2) xmin = 2;
	else if(xmax >= width - 4) xmax = width - 4;

	double ammout = .8;
	if(color == colorBack)
	{
		ammout = 0;
	}

		
	//Fill background

	
	if(bOnExpandItem)
	{
		if(bOnExpander)
		{
			xmax = xmin + 9;
		}
		else
		{
			xmin += 9;
		}
	}

	/*if(!(engine->Surface()->flags | SDL_OPENGL))
	{*/
		KrRGBA *pixels = getCanvasResource()->Pixels();
		
		for(int i = xmin; i < xmax; i++)
		{
			for(int j = ymin; j < ymax; j++)
			{
				pixels[ j*Width() + i ] = SlowCanvas::MixColors(pixels[ j*Width() + i ], Panel::getColorHighLight(), ammout);
			}
		}
		
	/*}
	else
	{
		KrRect rect = getImage()->Bounds();

		xmin += rect.xmin;
		xmax += rect.xmin;
		ymin += rect.ymin;
		ymax += rect.ymin;

		KrPainter painter( engine->Surface() );
		painter.DrawFilledBox(xmin, ymin, xmax - xmin + 1, ymax - ymin + 1,
			color.c.red, color.c.green, color.c.blue, getImage()->TreeDepth());
	}*/

	getCanvasResource()->Refresh();
	return true;
}

const stCursor * ListPopSelect::OnGetCursor()
{
	return NULL;
}

void ListPopSelect::OnKeyDown(SDLKey key, int repeatLoopCount)
{
	/*if(key == SDLK_ESCAPE)
		delete this;*/
}


#endif //#ifndef STAND_ALONE_GAME