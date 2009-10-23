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


// PanelActors.cpp: implementation of the PanelActors class.
//
//////////////////////////////////////////////////////////////////////

#include "PanelActors.h"
#include "PanelProperty.h"
#include "wxgameeditor.h"
#include "../gameEngine/ActorEdit.h"
#include "../gameEditor/ActorProperty.h"
#include "../gameEditor/UndoControl.h"
#include <wx/listimpl.cpp> 
#include <wx/tooltip.h>   

WX_DEFINE_LIST(ListEventIcon);

#ifdef WIN32
#include <commctrl.h>
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#endif

#define PANEL_ACTORS_ID		3
#define MENU_CENTER_ACTOR_ID		1000
#define MENU_REMOVE_ACTOR_ID		1001



BEGIN_EVENT_TABLE(PanelActors, wxTreeCtrl)
    EVT_KEY_DOWN(PanelActors::OnKey)
	EVT_LEFT_DOWN(PanelActors::OnLeftClickDown)
	EVT_RIGHT_DOWN(PanelActors::OnRightClickDown)
	EVT_LEFT_DCLICK(PanelActors::OnLeftDoubleClick)
	EVT_MOTION(PanelActors::OnMouseMove)
	EVT_TREE_SEL_CHANGING(PANEL_ACTORS_ID, PanelActors::OnSeletionChanging)
	EVT_TREE_SEL_CHANGED(PANEL_ACTORS_ID, PanelActors::OnSeletionChanged)
	EVT_TREE_ITEM_COLLAPSED(PANEL_ACTORS_ID, PanelActors::OnItemCollapsed)
	EVT_TREE_ITEM_EXPANDED(PANEL_ACTORS_ID, PanelActors::OnItemExpanded)
	EVT_PAINT(PanelActors::OnPaint)
	EVT_BUTTON(wxID_ANY, PanelActors::OnButtonClick)
	EVT_TREE_ITEM_GETTOOLTIP(PANEL_ACTORS_ID, PanelActors::OnGetToolTip)
	EVT_MENU( wxID_ANY, PanelActors::OnMenuClick )
END_EVENT_TABLE()

extern int (*OnGetItemImage)(wxTreeCtrl *pTree, wxTreeItemId item);

int PanelActorsGetItemImage(wxTreeCtrl *pTree, wxTreeItemId item)
{
	//Give the images for tree popups
	if(PanelActors::Get() && pTree)
	{
		if(!pTree->GetImageList())
		{
			pTree->SetImageList(PanelActors::Get()->GetImageList());
		}

		return PanelActors::Get()->GetImageIndex(pTree->GetItemText(item));
	}

	return 0;
}

void AddToPanelActors(Actor *_actor, bool bSendMesage)
{	
	if(PanelActors::Get()) 
	{
		if(bSendMesage)	_actor->PostMessage(_actor, ADD_TO_PANEL_ACTORS, ADD_TO_PANEL_ACTORS); //Don't have right name
		else PanelActors::Get()->AddItem(_actor); //Name it's ok, can add
	}
}

void RemoveFromPanelActors(Actor *_actor)
{
	if(PanelActors::Get()) PanelActors::Get()->RemoveItem(_actor);
}

void RemoveAllPanelActors()
{
	if(PanelActors::Get()) PanelActors::Get()->RemoveAll();
}

void RemoveSelectedActor()
{
	if(PanelActors::Get()) PanelActors::Get()->RemoveSelectedActor();
}

void UpdateActorAnim(Actor *_actor)
{
	if(PanelActors::Get()) PanelActors::Get()->UpdateActorAnim(_actor);
}

PanelActors *PanelActors::panelActors = NULL;
#define ICON_SIZE 18

PanelActors::PanelActors(wxWindow *parent)
              : wxTreeCtrl(parent, PANEL_ACTORS_ID, wxPoint(0, 0), wxSize(-1, -1),
			wxTR_HAS_BUTTONS | wxTR_SINGLE | wxTR_HIDE_ROOT | wxTR_LINES_AT_ROOT /*| wxTR_EDIT_LABELS*/ | wxBORDER_NONE | wxCLIP_CHILDREN ) 
{
	panelActors = this;
	bCheckSelection = true;
	OnGetItemImage = PanelActorsGetItemImage;


	AddRoot(wxT(""));
    
	SetSizeHints(0,0);
	SetBackgroundColour(colorBgContent);

	imageList.Create(ICON_SIZE, ICON_SIZE, true);
	SetImageList(&imageList);


#ifdef WIN32
	SendMessage(      
		// returns LRESULT in lResult     
		(HWND) GetHandle(),      // handle to destination control     
		(UINT) TVM_SETLINECOLOR,      // message ID     
		(WPARAM) 0,      // = 0; not used, must be zero    
		(LPARAM) RGB(0, 0, 0));
#endif

	
	//Alloc space for the icon lines (array it's more fast)
	nMaxUsedIconLines = 0;
	nMaxIconLines = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) /ICON_SIZE;
	eventIcon = (EventIcons **)malloc(nMaxIconLines * sizeof(EventIcons *));
	memset(eventIcon, 0, nMaxIconLines * sizeof(EventIcons *));

	//Load bitmaps
	bmActivationEvent.LoadFile(wxMainFrame::Get()->getApplicationDir() + wxT("img_event_activation_event.gif"), wxBITMAP_TYPE_GIF);
	bmAnimationFinish.LoadFile(wxMainFrame::Get()->getApplicationDir() + wxT("img_event_animation_finish.gif"), wxBITMAP_TYPE_GIF);
	bmCreateActor.LoadFile(wxMainFrame::Get()->getApplicationDir() + wxT("img_event_create_actor.gif"), wxBITMAP_TYPE_GIF);
	bmDestroyActor.LoadFile(wxMainFrame::Get()->getApplicationDir() + wxT("img_event_destroy_actor.gif"), wxBITMAP_TYPE_GIF);
	bmKeyboard.LoadFile(wxMainFrame::Get()->getApplicationDir() + wxT("img_event_keyboard.gif"), wxBITMAP_TYPE_GIF);
	bmMouse.LoadFile(wxMainFrame::Get()->getApplicationDir() + wxT("img_event_mouse.gif"), wxBITMAP_TYPE_GIF);
	bmTimer.LoadFile(wxMainFrame::Get()->getApplicationDir() + wxT("img_event_timer.gif"), wxBITMAP_TYPE_GIF);
	bmCollision.LoadFile(wxMainFrame::Get()->getApplicationDir() + wxT("img_event_collision.gif"), wxBITMAP_TYPE_GIF);
	bmDrawActor.LoadFile(wxMainFrame::Get()->getApplicationDir() + wxT("img_event_draw_actor.gif"), wxBITMAP_TYPE_GIF);
	bmMoveFinish.LoadFile(wxMainFrame::Get()->getApplicationDir() + wxT("img_event_move_finish.gif"), wxBITMAP_TYPE_GIF);
	bmPathFinish.LoadFile(wxMainFrame::Get()->getApplicationDir() + wxT("img_event_path_finish.gif"), wxBITMAP_TYPE_GIF);
	bmOutOfVision.LoadFile(wxMainFrame::Get()->getApplicationDir() + wxT("img_event_out_of_vision.gif"), wxBITMAP_TYPE_GIF);

	//Menu setup
	m_menu = NULL;
	CreateMenu();
}

PanelActors::~PanelActors()
{
	panelActors = NULL;
	OnGetItemImage = NULL;
	delete m_menu;

	RemoveEventIcons();

	free(eventIcon);
}

void PanelActors::CreateMenu()
{
	if(!m_menu)
	{
		m_menu = new wxMenu;

		//Center Actor (default action)
		wxMenuItem *item = m_menu->Append(MENU_CENTER_ACTOR_ID, wxT("Center Actor"));
		wxFont &font = item->GetFont();
		font.SetWeight(wxFONTWEIGHT_BOLD);
		item->SetFont(font);

		m_menu->Append(MENU_REMOVE_ACTOR_ID, wxT("Remove Actor"));
	}
	
}

wxTreeItemId PanelActors::AppendItem(const wxTreeItemId& parent, const wxString& text, int image, int selImage, wxTreeItemData* data)
{
	//image = GetImage(text); //Very slow, add in OnPaint event (on demmand)

	wxTreeItemId item = wxTreeCtrl::AppendItem(parent, text, image, image, data);
	map[text] = item;	

	return item;
}

void PanelActors::Delete(const wxTreeItemId& item)
{
	int imageIndex = GetItemImage(item);
	if(imageIndex >= 0)
	{
		imageList.Remove(imageIndex);
	}

	//Remove all event icons to avoid ghost icons when remove an actor
	RemoveEventIcons();

	wxTreeCtrl::Delete(item);
}

int PanelActors::GetImage(const wxString &cloneName)
{
	int imageIndex = -1;
	wxString imgName;
	wxBitmap *pBitmap = NULL;
	wxColour colorKey(255, 0, 255);		
	
	Actor *actor = GameControl::Get()->GetActor(cloneName.GetData());	
	if(actor)
	{
		if(actor->getType() == SPRITE && !actor->getTextActor())
		{
			//Generate from actor
			colorKey = colorBgContent;
			
			SDL_Surface* surface = GetSDLSurface(actor, ICON_SIZE, ICON_SIZE, 0, SDL_MapRGB( engine->Surface()->format, colorKey.Red(), colorKey.Green(), colorKey.Blue()));
						
			if(surface)
			{				
				pBitmap = new wxBitmap(
					static_cast<const char *>(surface->pixels), surface->w, surface->h, surface->format->BitsPerPixel
				//wxImage(surface->w, surface->h, static_cast<unsigned char *>(surface->pixels), true)
				);

				//SDL_SaveBMP(surface, (wxMainFrame::Get()->getApplicationDir() + "outs.bmp").GetData());
				//pBitmap->SaveFile(wxMainFrame::Get()->getApplicationDir() + "out.bmp", wxBITMAP_TYPE_BMP);
				
				SDL_FreeSurface( surface );				
			}
		}
		else
		{
			switch(actor->getType())
			{
			case SPRITE: //text actor
				imgName = wxT("img_text.gif");
				break;

			case CANVAS:
				imgName = wxT("img_canvas.gif");
				break;	
				
			case REGION_ACTOR_FILLED:
				imgName = wxT("img_region_filled.gif");
				break;
				
			case REGION_ACTOR:
				imgName = wxT("img_region_wire.gif");
				break;
				
			case VIEW_ACTOR:
				imgName = wxT("img_view.gif");
				break;
			}			
		}		
	}
	else if(cloneName == "Global Code")
	{
		//Special case for the script icon
		imgName = wxT("img_global.gif");
	}

	if(!pBitmap && !imgName.IsEmpty())
	{
		pBitmap = new wxBitmap();
		if(!pBitmap->LoadFile(wxMainFrame::Get()->getApplicationDir() + imgName, wxBITMAP_TYPE_GIF))
		{
			delete pBitmap;
			pBitmap = NULL;
		}
	}

	if(pBitmap)
	{			
		if(!imageList.GetImageCount())
		{
			//There is a bug in wxWidgets that return 0 even if item has no image
			//So, add a blank image to index 0
			imageIndex = imageList.Add(wxBitmap(ICON_SIZE, ICON_SIZE), wxColour(0, 0, 0));
		}

		imageIndex = imageList.Add(*pBitmap, colorKey);

		delete pBitmap;
	}

	return imageIndex;
}

void PanelActors::OnPaint(wxPaintEvent& event)
{
	wxTreeItemId h;	
	int index = -1;

    for (h=GetFirstVisibleItem(); h.IsOk();	h=GetNextVisible(h))
    {
        if (!IsVisible(h))
            break;

		int image = GetItemImage(h);
		wxString cloneName(GetItemText(h));

		if(image == -1) //wxWidgets return 0 even if there is no image in the item
		{
			image = GetImage(cloneName);

			if(image >= 0)
			{
				SetItemImage(h, image, wxTreeItemIcon_Normal);
				SetItemImage(h, image, wxTreeItemIcon_Selected);
			}
		}

		index = UpdateEventStatus(h, GameControl::Get()->GetActor(cloneName.GetData()));		
    }

	//Clear the last line, if not visible
	//May slow down the scroll
	if(index > 0 && index < nMaxUsedIconLines)
	{
		if(eventIcon[nMaxUsedIconLines])
		{
			delete eventIcon[nMaxUsedIconLines];
			eventIcon[nMaxUsedIconLines] = NULL;
			nMaxUsedIconLines--;
		}
	}

	event.Skip();
}


void PanelActors::SendSelectedEvent()
{
	if(!GetSelection().IsOk()) return;

    Actor *actor = GameControl::Get()->GetActor(GetItemText(GetSelection()).GetData());	
	if(actor) 
	{	
		//Only update (center when click)
		bCheckSelection = false; //allow select actors with clones
		ActorProperty::Update(actor);
		bCheckSelection = true;
	}
}


void PanelActors::OnSeletionChanged(wxTreeEvent& event)
{
	if( event.GetItem().IsOk() 
		//&& GetChildrenCount(event.GetItem()) == 0 //Comment to allow select actors with clones
		) 
	{
		if(bCheckSelection)
		{
			PanelActors::SendSelectedEvent();
		}
	}

	event.Skip();
}

void PanelActors::OnSeletionChanging(wxTreeEvent& event)
{
	/*if(!(event.GetItem().IsOk() && GetChildrenCount(event.GetItem()) == 0))
	{
		event.Veto();
	}*/

	event.Skip();
}

void PanelActors::OnItemCollapsed(wxTreeEvent& event)
{
	RemoveEventIcons();
	event.Skip();
}

void PanelActors::OnItemExpanded(wxTreeEvent& event)
{
	RemoveEventIcons();
	event.Skip();
}

void PanelActors::OnGetToolTip(wxTreeEvent& event)
{
	wxTreeItemId item = event.GetItem();

	if(item.IsOk())
	{
		Actor *actor = GameControl::Get()->GetActor(GetItemText(item).GetData());	
		if(IS_VALID_ACTOR(actor)) 
		{				
			wxString tip;
			int n = GetChildrenCount(item);

			if(!n)
			{
				//Actor tip

				tip += wxT("Name: ");
				tip += actor->getCloneName();
				tip += wxT("\n");
				
				if(!actor->getDescription().empty())
				{
					tip += wxT("Description: ");
					tip += actor->getDescription().c_str();				
					tip += wxT("\n");
				}			
				
				
				tip += wxT("Position: ");
				tip += wxString::Format(wxT("%ld, %ld"), (long)actor->getX(), (long)actor->getY());
				tip += wxT("\n");
				
				if(actor->getParent() && actor->getParent() != GameControl::Get()->GetAxis())
				{
					tip += wxT("Parent: ");
					tip += actor->getParent()->getCloneName();
				}
			}
			else
			{
				//Clone group
				tip = tip.Format(wxT("Name: %s\nClones: %ld"), actor->getActorName(), n);
			}

			event.SetToolTip(tip);
		}
	}
	else
	{
		event.SetToolTip(wxT(""));
	}
}

void PanelActors::OnMouseMove(wxMouseEvent& event)
{
	//Remove the tool tip
	int flags = 0;
	if(!HitTest(event.GetPosition(), flags).IsOk())
	{
		if(!(flags & wxTREE_HITTEST_ONITEMLABEL) && !(flags & wxTREE_HITTEST_ONITEMICON))
		{
			SetToolTip(wxT(""));	
		}
	}

	event.Skip();
}


void PanelActors::CenterActor()
{
	Actor *actor = GameControl::Get()->GetActor(GetItemText(GetSelection()).GetData());	
	if(actor) 
	{
		//Move view
		int x, y;
		
		if(actor->getParent() == GameControl::Get()->GetAxis())
		{
			x = actor->getImage()->X();
			y = actor->getImage()->Y();
		}
		else
		{
			//To axis coordinates
			x = actor->getImage()->CompositeXForm().x.ToInt();
			y = actor->getImage()->CompositeXForm().y.ToInt();
			
			KrVector2T< GlFixed > object;
			GameControl::Get()->GetAxis()->getImage()->ScreenToObject( x, y, &object );
			
			x = object.x.ToIntRound();
			y = object.y.ToIntRound();
		}
		
		int dx = (int)(GameControl::Get()->GetAxis()->getScreenWidth()/2.0);
		int dy = (int)(GameControl::Get()->GetAxis()->getScreenHeight()/2.0);
		
		x = (int)(x*GameControl::Get()->GetAxis()->getScale());
		y = (int)(y*GameControl::Get()->GetAxis()->getScale());
		
		x -= dx;
		y -= dy;				
		
		GameControl::Get()->GetAxis()->SetPos(-x, -y);
	}
}


void PanelActors::OnLeftClickDown(wxMouseEvent& event)
{
    wxPoint pt = event.GetPosition();
	
	wxRect rect;
	if(GetSelection().IsOk() && GetBoundingRect(GetSelection(), rect, true) && rect.Inside(pt))
	{	
		CenterActor();
	}

	event.Skip();
}

void PanelActors::OnRightClickDown(wxMouseEvent& event)
{
    wxPoint pt = event.GetPosition();

	int flags = 0;
	if(HitTest(event.GetPosition(), flags).IsOk())
	{
		if(flags & wxTREE_HITTEST_ONITEMLABEL)
		{
			PopupMenu(m_menu, pt);	
		}
	}

	event.Skip();
}

void PanelActors::OnLeftDoubleClick(wxMouseEvent& event)
{
	wxPoint pt = event.GetPosition();
	
	wxRect rect;
	if(GetSelection().IsOk() && GetBoundingRect(GetSelection(), rect, true) && rect.Inside(pt))
	{	
		CenterActor();
	}

	event.Skip();
}

void PanelActors::OnMenuClick(wxCommandEvent& event)
{	
	switch(event.GetId())
	{
	case MENU_CENTER_ACTOR_ID:
		CenterActor();
		break;

	case MENU_REMOVE_ACTOR_ID:
		RemoveActor(GetItemText(GetSelection()));
		break;
	}
}

void PanelActors::OnKey(wxKeyEvent& event)
{
	if( event.GetKeyCode() == WXK_DELETE && GetSelection().IsOk())
	{		
		RemoveActor(GetItemText(GetSelection()));		
	}
	else
	{
		event.Skip();	
	}
}

void PanelActors::OnButtonClick(wxCommandEvent& event)
{
	event.Skip();
}

void PanelActors::RemoveSelectedActor()
{
	wxWindow *focus = wxWindow::FindFocus();

	if (GetSelection().IsOk() && 
	   (focus == PanelActors::Get() || focus == SDLPanel::Get()) )
	{
		RemoveActor(GetItemText(GetSelection()));
	}
}

void PanelActors::RemoveActor(wxString name)
{
	bool bSingle = true;
	ListActor *pList = mapActors.FindString(name.GetData());
	
	if(pList)
	{
		if(pList->Count() > 1)
		{
			bSingle = false;
		}
	}
	else
	{
		if(name.Find('.') < 0)
		{
			bSingle = false;
		}
	}
	
	//Confirm?
	if(bSingle)
	{		
		Actor *actor = GameControl::Get()->GetActor(name.GetData());
		wxString msg(wxT("Do you want remove the actor '" + name + "'?"));

		if(actor->isView())
		{
			wxMessageBox(wxT("You can't remove the view"), wxT("Error"));
			return;
		}

		//Test children event actors 
		bool bHaveHeirs = false;
		if(ActorProperty::GiveInheritedEvents(actor))
		{
			msg = "The events in the actor '" + name + "' are inherited by other actors.\nRemove anyway?";
			bHaveHeirs = true;
		}

		int answer = wxMessageBox(msg, wxT("Confirm"), wxYES_NO | wxCENTRE);
		if(answer != wxYES) return;
		
		//Update undo
		UndoControl::Get()->PushCurrentState();

		//Remove
		gedString actorName(actor->getActorName());
		RemoveItem(actor);
		ActivationEventsCanvas::ActorRemoved(actor->getCloneName());
		GameControl::Get()->RemoveActor(actor, true);
		delete actor;
		
		if(bHaveHeirs)
		{
			Action::RemoveInheritedEvents(actorName);
		}
	}
	else
	{
		//Multiple remotion
		wxString msg(wxT("Do you want remove all the actors '" + name + "'?"));
		Actor *actor = GameControl::Get()->GetActor(name.GetData());

		//Test children event actors 
		bool bHaveHeirs = false;
		if(Action::GiveInheritedEvents(actor->getActorName()))
		{
			msg = "The events in the actor '" + name + "' are inherited by other actors.\nRemove anyway?";
			bHaveHeirs = true;
		}

		int answer = wxMessageBox(msg, wxT("Confirm"), wxYES_NO | wxCENTRE);
		if(answer != wxYES) return;

		//Update undo
		UndoControl::Get()->PushCurrentState();

		//Remove
		if(pList)
		{
			ListActor *listActor = mapActors[name.GetData()];	
			for(int il = 0; il < listActor->Count(); il++)
			{
				Actor *actor = (*listActor)[il];
				
				gedString actorName(actor->getActorName());
				RemoveItem(actor);
				ActivationEventsCanvas::ActorRemoved(actor->getCloneName());
				GameControl::Get()->RemoveActor(actor, true);
				delete actor;				
			}
		}

		if(bHaveHeirs)
		{
			Action::RemoveInheritedEvents(name.GetData());
		}
	}
	
	SetFocus();
}

void PanelActors::AddItem(Actor *_actor)
{
	wxString cloneName(_actor->getCloneName()), name(_actor->getActorName());
	wxTreeItemId parentNode;
	wxStringTreeMap::iterator it = map.find(name);


	if(it != map.end())
	{
		parentNode = it->second;

		if(mapActors.FindString(_actor->getActorName())->Count() == 2)
		{
			//Expand the tree
			//Add the first and second child now

			ListActor *listActor = mapActors[_actor->getActorName()];	
			for(int il = 0; il < listActor->Count(); il++)
			{
				Actor *pActor = (*listActor)[il];
			
				wxTreeItemId item = AppendItem(parentNode, pActor->getCloneName());
				UpdateEventStatus(item, pActor);

				if(selectedText == pActor->getCloneName())
				{
					SelectItem(item);
				}
			}			

			return;
		}
	}
	else
	{
		parentNode = GetRootItem();
		cloneName = name;
	}

	wxTreeItemId item = AppendItem(parentNode, cloneName);
	UpdateEventStatus(item, _actor);

	if(selectedText == _actor->getCloneName())
	{
		SelectItem(item);
	}

	SortChildren(parentNode);
	Refresh();
}

void PanelActors::RemoveItem(Actor *_actor)
{
	if(!GetCount()) return;

	wxString cloneName(_actor->getCloneName()), name(_actor->getActorName());
	wxTreeItemId parentNode;
	wxStringTreeMap::iterator it = map.find(cloneName);


	if(it != map.end())
	{
		Delete(it->second);
		map.erase(it);
	}

	//Is the last child?
	ListActor *pList = mapActors.FindString(_actor->getActorName());
	if(!pList)
	{
		//Remove name
		it = map.find(name);		
		
		if(it != map.end())
		{
			Delete(it->second);
			map.erase(it);
		}
	}
	else if(pList->Count() == 1)
	{
		//Remove the unique clone
		it = map.find(((*pList)[0])->getCloneName());		
		
		if(it != map.end())
		{
			Delete(it->second);
			map.erase(it);
		}
	}

	Refresh();
}

void PanelActors::RemoveAll()
{
	DeleteAllItems();
	map.clear();
	imageList.RemoveAll();
	selectedText.Clear();

	RemoveEventIcons();

	AddRoot(wxT(""));
}

void PanelActors::RemoveEventIcons()
{
	for(int i = 0; i <= nMaxUsedIconLines; i++)
	{
		delete eventIcon[i];
		eventIcon[i] = NULL;
	}

	nMaxUsedIconLines = 0;
}

void PanelActors::Update(Actor *_actor)
{
	if(panelActors && panelActors->bCheckSelection)
	{
		if(IS_VALID_ACTOR(_actor))
		{
			wxStringTreeMap::iterator it = panelActors->map.find(_actor->getCloneName());
			wxTreeItemId item;
			
			panelActors->bCheckSelection = false;
			panelActors->selectedText = _actor->getCloneName();
			if(it != panelActors->map.end())
			{
				//Select clone
				item = it->second;
				panelActors->SelectItem(item);							
			}
			else
			{
				it = panelActors->map.find(_actor->getActorName());
				
				if(it != panelActors->map.end())
				{
					//Select name
					item = it->second;
					panelActors->SelectItem(item);					
				}
				
			}
			
			panelActors->UpdateEventStatus(item, _actor);
			

			panelActors->bCheckSelection = true;
		}
		else
		{
			//Deselect
			/*if(panelActors->GetSelection().IsOk())
			{
				panelActors->SelectItem(panelActors->GetSelection(), false);
			}*/
		}
	}
}

int PanelActors::UpdateEventStatus(wxTreeItemId& item, Actor *_actor)
{
	int index = -1;

	if(item.IsOk() && IS_VALID_ACTOR(_actor))
	{
		//Show event icons only on actors
		//Show in clone groups if not expanded

		wxRect itemRect;		
		if(GetBoundingRect(item, itemRect, true) && (!GetChildrenCount(item) || !IsExpanded(item))) 
        {
			
            int x = GetClientSize().GetWidth() - ICON_SIZE, y = itemRect.GetTop();
			index = y/ICON_SIZE; 
			
			if(index < nMaxIconLines)
			{				
				//Allocate line
				if(!eventIcon[index])
				{
					eventIcon[index] = new EventIcons(this);
				}
				
				if(index > nMaxUsedIconLines) nMaxUsedIconLines = index;

				x -= ICON_SIZE*eventIcon[index]->IconCount(_actor);
				eventIcon[index]->SetIcons(this, _actor, x, y);					
			}
        }
	}

	return index;
}

void PanelActors::UpdateActorAnim(Actor *_actor)
{
	//Remove the current actor image
	//So, will be updated in the next repaint
	
	wxStringTreeMap::iterator it = map.find(_actor->getCloneName());
	wxTreeItemId item;

	if(it != map.end())
	{
		//Select clone
		item = it->second;							
	}
	else
	{
		it = map.find(_actor->getActorName());
		
		if(it != map.end())
		{
			//Select name
			item = it->second;				
		}
		
	}

	if(item.IsOk())
	{
		SetItemImage(item, 0, wxTreeItemIcon_Normal);
		SetItemImage(item, 0, wxTreeItemIcon_Selected);
	}
}

int PanelActors::GetImageIndex(const wxString &name)
{
	int image = 0;

	wxStringTreeMap::iterator it = map.find(name);
	wxTreeItemId item;
	

	if(it != map.end())
	{
		wxTreeItemId item = it->second;
		image =  GetItemImage(item);

		if(image == -1) //wxWidgets return 0 even if there is no image in the item
		{
			image = GetImage(GetItemText(item));

			if(image >= 0)
			{
				SetItemImage(item, image, wxTreeItemIcon_Normal);
				SetItemImage(item, image, wxTreeItemIcon_Selected);
			}
		}
	}
	else if(name == "Global Code")
	{
		image = GetImage(name);
	}
	

	return image;
}





IMPLEMENT_DYNAMIC_CLASS( EventIcon, wxStaticBitmap )

BEGIN_EVENT_TABLE(EventIcon, wxStaticBitmap)
	EVT_MOTION(EventIcon::OnMouseMove)
	EVT_LEFT_DOWN(EventIcon::OnButtonClick)
END_EVENT_TABLE()

void EventIcon::OnMouseMove(wxMouseEvent& event)
{
	if(actor && actor->getAction() /*&& !GetToolTip()*/)
	{
		wxString newTip(actor->getAction()->GetDescriptions(events).c_str());

		//if(newTip != tip)
		if(posAnt != event.GetPosition()) //Comment this line to keep showing the tip without timeout
		{
			tip = newTip;
			SetToolTip(tip);
			GetToolTip()->SetDelay(100);

			posAnt = event.GetPosition();
		}
	}

	event.Skip();
}

void EventIcon::OnButtonClick(wxMouseEvent& event)
{
	PanelActors::Update(actor);
	PanelProperty::Update(actor);

	event.Skip();
}


EventIcons::EventIcons(wxWindow *_parent)
{
	actor = NULL;
	xIni = 0;
	events = 0;
	parent = _parent;
	list.DeleteContents(true);
}

EventIcons::~EventIcons()
{
	Clear();
}

void EventIcons::Add(int id, wxBitmap& bitmap, int x, int y, Uint32 events)
{
	EventIcon *p = new EventIcon(actor, events, parent, id, bitmap, wxPoint(x, y), wxDefaultSize);
	list.Append(p);
}

void EventIcons::Clear()
{
	actor = NULL;
	xIni = 0;
	events = 0;
	list.Clear();
}

void EventIcons::SetIcons(PanelActors *panelActors, Actor *_actor, int x, int y)
{
	//Test actions
	Action *action = _actor->getAction();
	if(action && action->getActions())
	{
		//Set event icons
		Uint32 newEvents = action->GetEventBits(false);
		x = (int)ceil((double)x / ICON_SIZE) * ICON_SIZE;
		parent = panelActors;
		actor = _actor;

		
		if(newEvents != events)
		{
			Clear();
			events = newEvents;
			actor = _actor;
			
		
			//If need context menus or other control based on button click,
			//change the button ids
			static int id = 0; 
			
			xIni = x;			
			
			
			//Create icons
			if(events & EVENTACTIVATIONEVENT)
			{						
				Add(id++, panelActors->bmActivationEvent, x, y, EVENTACTIVATIONEVENT);
				x += ICON_SIZE;
			}
			
			if(events & EVENTANIMATIONFINISH)
			{						
				Add(id++, panelActors->bmAnimationFinish, x, y, EVENTANIMATIONFINISH);
				x += ICON_SIZE;
			}
			
			if((events & EVENTCOLLISION) || (events & EVENTCOLLISIONFINISH))
			{						
				Add(id++, panelActors->bmCollision, x, y, EVENTCOLLISION | EVENTCOLLISIONFINISH);
				x += ICON_SIZE;
			}
			
			if(events & EVENTCREATE)
			{						
				Add(id++, panelActors->bmCreateActor, x, y, EVENTCREATE);
				x += ICON_SIZE;
			}
			
			if(events & EVENTDESTROYACTOR)
			{						
				Add(id++, panelActors->bmDestroyActor, x, y, EVENTDESTROYACTOR);
				x += ICON_SIZE;
			}
			
			if(events & EVENTANIMATION)
			{
				Add(id++, panelActors->bmDrawActor, x, y, EVENTANIMATION);
				x += ICON_SIZE;
			}
			
			if((events & EVENTKEYDOWN) || (events & EVENTKEYUP))
			{						
				Add(id++, panelActors->bmKeyboard, x, y, EVENTKEYDOWN | EVENTKEYUP);
				x += ICON_SIZE;
			}
			
			if((events & EVENTMOUSEBUTTONDOWN) || (events & EVENTMOUSEBUTTONUP) || (events & EVENTMOUSEENTER) || (events & EVENTMOUSELEAVE))
			{
				Add(id++, panelActors->bmMouse, x, y, EVENTMOUSEBUTTONDOWN | EVENTMOUSEBUTTONUP | EVENTMOUSEENTER | EVENTMOUSELEAVE);
				x += ICON_SIZE;
			}					
			
			if(events & EVENTMOVEFINISH)
			{
				Add(id++, panelActors->bmMoveFinish, x, y, EVENTMOVEFINISH);
				x += ICON_SIZE;
			}
			
			if(events & EVENTOUTOFVISION)
			{
				Add(id++, panelActors->bmOutOfVision, x, y, EVENTOUTOFVISION);
				x += ICON_SIZE;
			}
			
			if(events & EVENTPATHFINISH)
			{
				Add(id++, panelActors->bmPathFinish, x, y, EVENTPATHFINISH);
				x += ICON_SIZE;
			}					
			
			if(events & EVENTTIMER)
			{						
				Add(id++, panelActors->bmTimer, x, y, EVENTTIMER);
				x += ICON_SIZE;
			}
		}
		else if(xIni != x)
		{
			//Move to new x position
			xIni = x;			

			for (ListEventIcon::Node *node = list.GetFirst(); node; node = node->GetNext())
			{
				EventIcon *current = node->GetData();
				
				current->Move(x, y);
				current->SetActor(actor);
				current->Refresh();
				current->SetToolTip(NULL);
				x += ICON_SIZE;
			}

		}
	}
	else
	{
		Clear();
	}
}
int EventIcons::IconCount(Actor *_actor)
{
	int nIcons = 0;

	Action *action = _actor->getAction();
	if(action && action->getActions())
	{
		Uint32 events = action->GetEventBits(false);		

		if(events & EVENTACTIVATIONEVENT) nIcons++;			
		if(events & EVENTANIMATIONFINISH) nIcons++;			
		if((events & EVENTCOLLISION) || (events & EVENTCOLLISIONFINISH)) nIcons++;			
		if(events & EVENTCREATE) nIcons++;			
		if(events & EVENTDESTROYACTOR) nIcons++;			
		if(events & EVENTANIMATION) nIcons++;			
		if((events & EVENTKEYDOWN) || (events & EVENTKEYUP)) nIcons++;			
		if((events & EVENTMOUSEBUTTONDOWN) || (events & EVENTMOUSEBUTTONUP) || (events & EVENTMOUSEENTER) || (events & EVENTMOUSELEAVE)) nIcons++;			
		if(events & EVENTMOVEFINISH) nIcons++;			
		if(events & EVENTOUTOFVISION) nIcons++;			
		if(events & EVENTPATHFINISH) nIcons++;			
		if(events & EVENTTIMER) nIcons++;

	}

	return nIcons;
}
