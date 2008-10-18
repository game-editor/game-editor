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