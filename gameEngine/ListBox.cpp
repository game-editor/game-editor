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


// ListBox.cpp: implementation of the ListBox class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "ListBox.h"
#include "GameControl.h"



ListBox::ListBox(int x, int y, int width, int height, int listId, Actor *parent, gedString fontName)
{
	this->listId = listId;
	this->parent = parent;

	list = NULL;
	fontResource = GameControl::Get()->GetFont(fontName.c_str());
	if(fontResource)
	{
		KrScheme scheme(fontResource);
		
		list = new ListBoxWidget(fontResource, width, height, 20, scheme, this);
				
		

		list->SetNodeId((int)list);
		engine->Tree()->AddNode( parent->getImage(), list );
		list->SetPos( x, y );

		fontResource->refCount++;

		list->AddListener(list); //To message process
	}
}

ListBox::~ListBox()
{
	if(engine->Vault()->ExistsResource(fontResource))
	{
		fontResource->refCount--;
		if(fontResource->refCount == 0)
			engine->Vault()->RemoveSpriteResource(fontResource);
	}

	if(engine->Tree()->FindNodeById((int)list))
	{
		engine->Tree()->DeleteNode(list);
	}
}

bool ListBox::ListBoxWidget::HandleWidgetEvent(	KrWidget* source, 
							   U32 event, 
							   U32 data, 
							   const SDL_Event* sdlEvent,
							   const char* command, 
							   const char* arg )
{	
	if(KrListBox::HandleWidgetEvent(source, event, data, sdlEvent, command, arg))
	{
		if(parent->getParent())
		{
			gedString text;
			GetTextChar(SelectedItem(), &text);
			if(text.length() > 0)
				parent->getParent()->OnList((ListPop *)parent, SelectedItem(), text, parent->getListId());
		}
	}
	
	return false;
}

int ListBox::Width()
{
	return list->Width();
}

int ListBox::Height()
{
	return list->Height();
}

int ListBox::Top()
{
	return list->Y();
}

int ListBox::Down()
{
	return list->Y() + list->Height();
}

int ListBox::Left()
{
	return list->X();
}

int ListBox::Right()
{
	return list->X() + list->Width();
}

void ListBox::AddText(gedString text)
{
	list->AddTextChar(text);
}

void ListBox::RemoveText(gedString text)
{
	list->RemoveText(text);
}

void ListBox::RemoveAll()
{
	list->RemoveAll();
}

gedString ListBox::GetText(int id)
{
	gedString text;
	list->GetTextChar(id, &text);
	return text;
}

int ListBox::Count()
{
	return list->NumItems();
}


#endif //#ifndef STAND_ALONE_GAME