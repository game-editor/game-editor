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


// PanelActors.h: interface for the PanelActors class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PANELACTORS_H__537F8062_E783_46C9_A685_C07069AB1356__INCLUDED_)
#define AFX_PANELACTORS_H__537F8062_E783_46C9_A685_C07069AB1356__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "wx/wxprec.h"
#include "GameControl.h"
#include "wxGed/wxGedControls.h"
#include "wx/ifm/ifm.h"
#include "wx/treectrl.h" //maks
#include "wx/tokenzr.h" //maks


#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif


class EventIcon : public wxStaticBitmap
{
	DECLARE_DYNAMIC_CLASS( EventIcon )
    DECLARE_EVENT_TABLE()

public:

	EventIcon()
	{
		actor = NULL;
	}

	EventIcon(Actor *_actor, Uint32 _events, wxWindow *parent,
                   wxWindowID id,
                   const wxBitmap& bitmap,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = wxBU_EXACTFIT,                   
                   const wxString& name = wxButtonNameStr) 
		: wxStaticBitmap(parent, id, bitmap, pos, size, style, name)
    {
		actor = _actor;
		events = _events;        
    }

	void SetActor(Actor *_actor) {actor = _actor;}
	

protected:

	void OnButtonClick(wxMouseEvent& event);

	Actor *actor;
	Uint32 events;
	wxString tip;
	wxPoint posAnt;

	void OnMouseMove(wxMouseEvent& event);

};

WX_DECLARE_HASH_MAP(wxString, wxTreeItemId, wxStringHash, wxStringEqual, wxStringTreeMap);
WX_DECLARE_LIST(EventIcon, ListEventIcon);




class PanelActors;
class EventIcons
{
public:
	EventIcons(wxWindow *_parent);
    virtual ~EventIcons();
	
	void SetIcons(PanelActors *panelActors, Actor *actor, int x, int y);
	int IconCount(Actor *_actor);
	void Clear();

private:
	void Add(int id, wxBitmap& bitmap, int x, int y, Uint32 events);

	ListEventIcon list;
	wxWindow *parent;

	Actor *actor;
	Uint32 events;
	int xIni;
};

class PanelActors : public wxTreeCtrl  
{
	friend EventIcons;

public:

	void RemoveSelectedActor();
	void RemoveItem(Actor *_actor);
	void RemoveAll();
	void AddItem(Actor *_actor);

	PanelActors(wxWindow* parent);
    virtual ~PanelActors();

	wxTreeItemId AppendItem(const wxTreeItemId& parent, const wxString& text, int image = -1, int selImage = -1, wxTreeItemData* data = NULL);
	void Delete(const wxTreeItemId& item);
   	//void SelectItem(wxString value);
	
	static PanelActors *Get() {return panelActors;}
	static void Update(Actor *_actor);
	void UpdateActorAnim(Actor *_actor);

	int GetImageIndex(const wxString &name);

protected:

	void CreateMenu();

	void RemoveActor(wxString name);
	void RemoveEventIcons();
	void CenterActor();
	int UpdateEventStatus(wxTreeItemId& item, Actor *_actor);
 
    void OnKey(wxKeyEvent& event);	
	void OnSeletionChanging(wxTreeEvent& event);
	void OnSeletionChanged(wxTreeEvent& event);
	void OnItemCollapsed(wxTreeEvent& event);
	void OnItemExpanded(wxTreeEvent& event);
	void OnLeftClickDown(wxMouseEvent& event);
	void OnRightClickDown(wxMouseEvent& event);
	void OnLeftDoubleClick(wxMouseEvent& event);
	void OnMouseMove(wxMouseEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnButtonClick(wxCommandEvent& event);
	void OnGetToolTip(wxTreeEvent& event);
	void OnMenuClick(wxCommandEvent& event);

	void SendSelectedEvent();

	int GetImage(const wxString &cloneName);

	bool bCheckSelection;


	static PanelActors *panelActors;

	wxStringTreeMap map; //For fast item find
	wxImageList imageList;
	wxString selectedText;

	wxMenu *m_menu;

	//Event icons
	int nMaxIconLines, nMaxUsedIconLines;
	EventIcons **eventIcon;

	wxBitmap	bmCreateActor, bmDestroyActor, bmActivationEvent, bmAnimationFinish, 
				bmKeyboard, bmMouse, bmTimer, bmCollision, bmDrawActor,
				bmMoveFinish, bmPathFinish, bmOutOfVision;

    DECLARE_EVENT_TABLE()

};

#endif // !defined(AFX_PANELACTORS_H__537F8062_E783_46C9_A685_C07069AB1356__INCLUDED_)
