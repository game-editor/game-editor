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


/////////////////////////////////////////////////////////////////////////////
// Name:        wxJigsawShapeListBox.cpp
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     24/03/2008 23:19:47
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include <wx/dcbuffer.h>
#include "wxJigsawShapeListBox.h"
#include "wxJigsawEditorCanvas.h"
#include "DnDJigsawShapeDataObject.h"
#include "DnDJigsawShapeDropTarget.h"

////@begin XPM images
////@end XPM images


/*!
 * wxJigsawShapeListBox type definition
 */

IMPLEMENT_DYNAMIC_CLASS( wxJigsawShapeListBox, wxWindow )


/*!
 * wxJigsawShapeListBox event table definition
 */

BEGIN_EVENT_TABLE( wxJigsawShapeListBox, wxWindow )

////@begin wxJigsawShapeListBox event table entries
    EVT_PAINT( wxJigsawShapeListBox::OnPaint )
    EVT_IDLE( wxJigsawShapeListBox::OnIdle )
    EVT_ERASE_BACKGROUND( wxJigsawShapeListBox::OnEraseBackground )
    EVT_LEFT_DOWN( wxJigsawShapeListBox::OnLeftDown )
    EVT_MOTION( wxJigsawShapeListBox::OnMotion )

////@end wxJigsawShapeListBox event table entries
EVT_SCROLL_LINEUP(wxJigsawShapeListBox::OnScrollLineUp)
EVT_SCROLL_LINEDOWN(wxJigsawShapeListBox::OnScrollLineUp)
EVT_SCROLL_PAGEUP(wxJigsawShapeListBox::OnScrollPageUp)
EVT_SCROLL_PAGEDOWN(wxJigsawShapeListBox::OnScrollPageDown)
EVT_SCROLL_THUMBTRACK(wxJigsawShapeListBox::OnScrollThumbTrack)
EVT_SCROLL_THUMBRELEASE(wxJigsawShapeListBox::OnScrollThumbRelease)

EVT_TIMER(-1,wxJigsawShapeListBox::OnTimer)

END_EVENT_TABLE()


/*!
 * wxJigsawShapeListBox constructors
 */

wxJigsawShapeListBox::wxJigsawShapeListBox()
{
    Init();
}

wxJigsawShapeListBox::wxJigsawShapeListBox(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}


/*!
 * wxJigsawShapeListBox creator
 */

bool wxJigsawShapeListBox::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
////@begin wxJigsawShapeListBox creation
    wxWindow::Create(parent, id, pos, size, style);
    CreateControls();
////@end wxJigsawShapeListBox creation
    return true;
}


/*!
 * wxJigsawShapeListBox destructor
 */

wxJigsawShapeListBox::~wxJigsawShapeListBox()
{
////@begin wxJigsawShapeListBox destruction
////@end wxJigsawShapeListBox destruction
}


/*!
 * Member initialisation
 */

void wxJigsawShapeListBox::Init()
{
////@begin wxJigsawShapeListBox member initialisation
    m_Shapes = NULL;
    m_AssociatedCanvas = NULL;
	selectedShape = NULL;
////@end wxJigsawShapeListBox member initialisation
}


/*!
 * Control creation for wxJigsawShapeListBox
 */

void wxJigsawShapeListBox::CreateControls()
{    
////@begin wxJigsawShapeListBox content construction
////@end wxJigsawShapeListBox content construction
}


/*!
 * Should we show tooltips?
 */

bool wxJigsawShapeListBox::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap wxJigsawShapeListBox::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin wxJigsawShapeListBox bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end wxJigsawShapeListBox bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon wxJigsawShapeListBox::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin wxJigsawShapeListBox icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end wxJigsawShapeListBox icon retrieval
}

wxPoint wxJigsawShapeListBox::GetScrollPosition()
{
	return wxPoint(GetScrollPos(wxHORIZONTAL),GetScrollPos(wxVERTICAL));
}

/*!
 * wxEVT_PAINT event handler for ID_WXJIGSAWSHAPELISTBOX
 */

void wxJigsawShapeListBox::OnPaint( wxPaintEvent& event )
{
	wxBufferedPaintDC dc(this);
	dc.SetBackground(wxBrush(GetBackgroundColour()));
	dc.Clear();
	dc.SetFont(GetFont());
	do
	{
		if(!m_Shapes) break;
		wxPoint scrollPos = GetScrollPosition();
		wxSize offset(-scrollPos.x, -scrollPos.y);
		wxPoint shapePos(10, 10);

		for(wxJigsawShapeList::Node * node = m_Shapes->GetFirst(); node; node = node->GetNext())
		{
			wxJigsawShape * shape = node->GetData();
			wxSize shapeSize = shape->GetSize(dc, 1.0);
			shape->SetPosition(shapePos);
			shape->Draw(dc, offset, 1.0);
			shapePos.y += shapeSize.GetHeight();
			shapePos.y += 10;
		}
	}
	while(false);
}


/*!
 * wxEVT_IDLE event handler for ID_WXJIGSAWSHAPELISTBOX
 */

void wxJigsawShapeListBox::OnIdle( wxIdleEvent& event )
{
////@begin wxEVT_IDLE event handler for ID_WXJIGSAWSHAPELISTBOX in wxJigsawShapeListBox.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_IDLE event handler for ID_WXJIGSAWSHAPELISTBOX in wxJigsawShapeListBox. 
}


/*!
 * wxEVT_ERASE_BACKGROUND event handler for ID_WXJIGSAWSHAPELISTBOX
 */

void wxJigsawShapeListBox::OnEraseBackground( wxEraseEvent& event )
{
}

wxPoint wxJigsawShapeListBox::PointToViewPoint(const wxPoint & pos)
{
	wxPoint scrollPos = GetScrollPosition();
	return wxPoint(pos.x + scrollPos.x, pos.y + scrollPos.y);
}

wxJigsawShape * wxJigsawShapeListBox::GetShapeByPoint(const wxPoint & pos)
{
	do
	{
		if(!m_Shapes) break;
		for(wxJigsawShapeList::Node * node = m_Shapes->GetFirst(); node; node = node->GetNext())
		{
			wxJigsawShape * shape = node->GetData();
			wxRect shapeRect(shape->GetPosition(), shape->GetSize());
			if(shapeRect.Contains(pos))
			{
				return shape;
			}
		}
	}
	while(false);
	return NULL;
}

wxSize wxJigsawShapeListBox::GetViewSize()
{
	wxSize res(0,0);
	do
	{
		if(!m_Shapes) break;
		for(wxJigsawShapeList::Node * node = m_Shapes->GetFirst(); node; node = node->GetNext())
		{
			wxJigsawShape * shape = node->GetData();
			wxSize shapeSize = shape->GetSize();
			res.SetWidth(wxMax(res.GetWidth(), shapeSize.GetWidth()));
			res.SetHeight(res.GetHeight() + shapeSize.GetHeight() + 10);
		}
	}
	while(false);
	res.IncBy(10, 10);
	wxSize clientSize = GetClientSize();
	res.SetWidth(wxMax(res.GetWidth(), clientSize.GetWidth()));
	res.SetHeight(wxMax(res.GetHeight(), clientSize.GetHeight()));
	return res;
}

void wxJigsawShapeListBox::AdjustScrollBars()
{
	// image sampled size
	wxSize imageSize = GetViewSize();
	// old scroll position
	wxPoint pt = GetScrollPosition();
	SetScrollbar(wxHORIZONTAL, 0, GetClientSize().x, imageSize.x);
	SetScrollbar(wxVERTICAL, 0, GetClientSize().y, imageSize.y);
	Scroll(pt);
}

void wxJigsawShapeListBox::Scroll(const wxPoint & pos)
{
	if(pos.x >= 0)
		SetScrollPos(wxHORIZONTAL, pos.x);
	if(pos.y >= 0)
		SetScrollPos(wxVERTICAL, pos.y);
	Refresh();
}

/*!
 * wxEVT_LEFT_DOWN event handler for ID_WXJIGSAWSHAPELISTBOX
 */

void wxJigsawShapeListBox::OnLeftDown( wxMouseEvent& event )
{
	wxPoint realPosition = PointToViewPoint(event.GetPosition());
	wxJigsawShape * shape = GetShapeByPoint(realPosition);
	m_TimerMove.Stop();

	if(m_AssociatedCanvas->GetDragImage())
	{
		m_AssociatedCanvas->GetDragImage()->Hide();
		m_AssociatedCanvas->GetDragImage()->EndDrag();
		m_AssociatedCanvas->DestroyDragImage();
	}

	if(shape)
	{
		//wxMessageBox(_("Shape"));
		DND_JigsawShapeInfo * shapeInfo = new DND_JigsawShapeInfo;
		shapeInfo->m_shape = shape;
		shapeInfo->m_offset = wxSize(
			realPosition.x-shape->GetPosition().x,
			realPosition.y-shape->GetPosition().y);
		DnDJigsawShapeDataObject * dataObject = new DnDJigsawShapeDataObject(shapeInfo);
		m_AssociatedCanvas->GetDropTarget()->SetDataObject(dataObject);
		DnDJigsawShapeSource dropSource(*dataObject, this, m_AssociatedCanvas);/*, NULL,
			wxDROP_ICON(dnd_copy),
			wxDROP_ICON(dnd_move),
			wxDROP_ICON(dnd_none));*/
		if(m_AssociatedCanvas->CreateDragImage(shape))
		{
			m_AssociatedCanvas->GetDragImage()->BeginDrag(
				wxPoint(shapeInfo->m_offset.x,shapeInfo->m_offset.y),
				//m_AssociatedCanvas->ScreenToClient(ClientToScreen(event.GetPosition())),
				this, true);

			m_AssociatedCanvas->GetDragImage()->Show();
		}
		
		m_AssociatedCanvas->SetSelectedObject(NULL);
		m_AssociatedCanvas->ReCreateHotSpots();
		wxPoint mousePosBefore(::wxGetMousePosition());
		wxDragResult dragResult = dropSource.DoDragDrop(wxDrag_DefaultMove);
		if(dragResult != wxDragResult::wxDragMove) 
		{
			//No drop happens
			//Check for auto drop for slots
			wxJigsawShape::wxJigsawShapeHitTestInfo info(m_AssociatedCanvas->GetSelectedShapeInfo());
			if(mousePosBefore == ::wxGetMousePosition() && info.GetResult() == wxJigsawShape::wxJS_HITTEST_SLOT && info.GetInputParameterStyle() == shape->GetStyle())
			{
				//There are a slot selected, so, the selected shape in the list have the same type of the slot
				startPosition = ScreenToClient(mousePosBefore);
				endPosition = ScreenToClient(m_AssociatedCanvas->ClientToScreen(info.GetMousePos()));
				m_AssociatedCanvas->GetDragImage()->Show();
				selectedShape = shape;

				double duration = 500, timer = 30;
				f = 0.0;
				df = timer/duration;
				m_TimerMove.SetOwner(this);
				m_TimerMove.Start(timer);
			}

			//Deselect the shape
			m_AssociatedCanvas->SetSelectedObject(NULL);
		}

		if(!m_TimerMove.IsRunning() && m_AssociatedCanvas->GetDragImage())
		{
			m_AssociatedCanvas->GetDragImage()->Hide();
			m_AssociatedCanvas->GetDragImage()->EndDrag();
			m_AssociatedCanvas->DestroyDragImage();
		}
	}
}

void wxJigsawShapeListBox::OnTimer(wxTimerEvent &event)
{
	wxPoint pos(startPosition);

	pos.x = (1 - f)*startPosition.x + f*endPosition.x;
	pos.y = (1 - f)*startPosition.y + f*endPosition.y;

	m_AssociatedCanvas->GetDragImage()->Move(pos);
	f += df;

	if(f >= 1.0)
	{
		m_AssociatedCanvas->ProcessDrop(m_AssociatedCanvas->GetSelectedShapeInfo().GetMousePos(), selectedShape, wxSize(0,0));
		m_AssociatedCanvas->GetSelectedShapeInfo().Clear();
		m_AssociatedCanvas->GetDragImage()->Hide();
		m_AssociatedCanvas->GetDragImage()->EndDrag();
		m_AssociatedCanvas->DestroyDragImage();
		m_TimerMove.Stop();
	}
}


/*!
 * wxEVT_MOTION event handler for ID_WXJIGSAWSHAPELISTBOX
 */

void wxJigsawShapeListBox::OnMotion( wxMouseEvent& event )
{

////@begin wxEVT_MOTION event handler for ID_WXJIGSAWSHAPELISTBOX in wxJigsawShapeListBox.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_MOTION event handler for ID_WXJIGSAWSHAPELISTBOX in wxJigsawShapeListBox. 
}

void wxJigsawShapeListBox::OnScrollLineUp(wxScrollEvent & event)
{
}

void wxJigsawShapeListBox::OnScrollLineDown(wxScrollEvent & event)
{
}

void wxJigsawShapeListBox::OnScrollPageUp(wxScrollEvent & event)
{
}

void wxJigsawShapeListBox::OnScrollPageDown(wxScrollEvent & event)
{
}

void wxJigsawShapeListBox::OnScrollThumbTrack(wxScrollEvent & event)
{
}

void wxJigsawShapeListBox::OnScrollThumbRelease(wxScrollEvent & event)
{
}
