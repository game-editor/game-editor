/////////////////////////////////////////////////////////////////////////////
// Name:        wxJigsawEditorCanvas.cpp
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     02/03/2008 19:26:49
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

#include "wxJigsawEditorCanvas.h"
#include "wxJigsawEditorDocument.h"
#include <wxJigsawShapeGroup.h>
#include "DnDJigsawShapeDataObject.h"

////@begin XPM images
////@end XPM images

int wxJigsawEditorCanvas::ScrollIncrement = 10;


/*!
 * wxJigsawEditorCanvas type definition
 */

IMPLEMENT_DYNAMIC_CLASS( wxJigsawEditorCanvas, wxWindow )


//Behavior events
DEFINE_EVENT_TYPE(wxEVT_BEHAVIOR_BLOCK_SELECTED)

/*!
 * wxJigsawEditorCanvas event table definition
 */

BEGIN_EVENT_TABLE( wxJigsawEditorCanvas, wxWindow )

////@begin wxJigsawEditorCanvas event table entries
    EVT_SIZE( wxJigsawEditorCanvas::OnSize )
    EVT_PAINT( wxJigsawEditorCanvas::OnPaint )
    EVT_IDLE( wxJigsawEditorCanvas::OnIdle )
    EVT_ERASE_BACKGROUND( wxJigsawEditorCanvas::OnEraseBackground )
    EVT_LEFT_DOWN( wxJigsawEditorCanvas::OnLeftDown )
    EVT_LEFT_UP( wxJigsawEditorCanvas::OnLeftUp )
    EVT_LEFT_DCLICK( wxJigsawEditorCanvas::OnLeftDClick )
    EVT_MOTION( wxJigsawEditorCanvas::OnMotion )

////@end wxJigsawEditorCanvas event table entries
	EVT_SCROLLWIN_LINEUP( wxJigsawEditorCanvas::OnScrollLineUp )
    EVT_SCROLLWIN_LINEDOWN( wxJigsawEditorCanvas::OnScrollLineDown )
    EVT_SCROLLWIN_PAGEUP( wxJigsawEditorCanvas::OnScrollPageUp )
    EVT_SCROLLWIN_PAGEDOWN( wxJigsawEditorCanvas::OnScrollPageDown )
    EVT_SCROLLWIN_THUMBTRACK( wxJigsawEditorCanvas::OnScrollThumbTrack )
    EVT_SCROLLWIN_THUMBRELEASE( wxJigsawEditorCanvas::OnScrollThumbRelease )

END_EVENT_TABLE()


/*!
 * wxJigsawEditorCanvas constructors
 */

wxJigsawEditorCanvas::wxJigsawEditorCanvas()
{
    Init();
}

wxJigsawEditorCanvas::wxJigsawEditorCanvas(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}


/*!
 * wxJigsawEditorCanvas creator
 */

bool wxJigsawEditorCanvas::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
////@begin wxJigsawEditorCanvas creation
    wxWindow::Create(parent, id, pos, size, style);
    CreateControls();
////@end wxJigsawEditorCanvas creation
    return true;
}


/*!
 * wxJigsawEditorCanvas destructor
 */

wxJigsawEditorCanvas::~wxJigsawEditorCanvas()
{
////@begin wxJigsawEditorCanvas destruction
////@end wxJigsawEditorCanvas destruction
	DestroyDragImage();
}


/*!
 * Member initialisation
 */

void wxJigsawEditorCanvas::Init()
{
////@begin wxJigsawEditorCanvas member initialisation
    m_View = NULL;
    m_SelectedObject = NULL;
    m_Mode = wxJSEC_MODE_NONE;
    m_SelectedObjectOffset = wxSize(0,0);
    m_DragImage = NULL;
    m_ActiveHotSpot = NULL;
    m_UpdateLayoutOnPaint = false;
////@end wxJigsawEditorCanvas member initialisation
}


/*!
 * Control creation for wxJigsawEditorCanvas
 */

void wxJigsawEditorCanvas::CreateControls()
{    
////@begin wxJigsawEditorCanvas content construction
////@end wxJigsawEditorCanvas content construction
}


/*!
 * Should we show tooltips?
 */

bool wxJigsawEditorCanvas::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap wxJigsawEditorCanvas::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin wxJigsawEditorCanvas bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end wxJigsawEditorCanvas bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon wxJigsawEditorCanvas::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin wxJigsawEditorCanvas icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end wxJigsawEditorCanvas icon retrieval
}

void wxJigsawEditorCanvas::AdjustScrollBars()
{
	// image sampled size
	wxSize imageSize = GetDiagramSize();
	// old scroll position
	wxPoint pt = GetScrollPosition();
	if(GetScrollRange(wxHORIZONTAL) != imageSize.x)
	{
		SetScrollbar(wxHORIZONTAL, 0, GetClientSize().x, imageSize.x);
	}
	if(GetScrollRange(wxVERTICAL) != imageSize.y)
	{
		SetScrollbar(wxVERTICAL, 0, GetClientSize().y, imageSize.y);
	}
	Scroll(pt);
	if(m_View)
	{
		//wxLogTrace(wxTraceMask(), _("View offset = %i, %i"), pt.x, pt.y);
		//m_View->SetViewOffset(wxSize(pt.x, pt.y));
	}
}


/*!
 * wxEVT_PAINT event handler for ID_WXJIGSAWEDITORCANVAS
 */

void wxJigsawEditorCanvas::OnPaint( wxPaintEvent& event )
{
	wxPaintDC dc(this);
	int width = wxMin(m_DoubleBufferDC.GetSize().GetWidth(), dc.GetSize().GetWidth());
	int height = wxMin(m_DoubleBufferDC.GetSize().GetHeight(), dc.GetSize().GetHeight());
	dc.Blit(0, 0, width, height, &m_DoubleBufferDC, 0, 0);
 //   // Before editing this code, remove the block markers.
 //   wxBufferedPaintDC dc(this);	
	//dc.SetBackground(wxBrush(GetBackgroundColour()));
	//dc.Clear();
	//wxPoint scrollPos = GetScrollPosition();
	//if(m_View)
	//{
	//	/*if(m_View->GetSelectedObject() && wxGetMouseState().LeftDown())
	//	{
	//		dc.SetBackground(wxBrush(wxColour(0, 0, 127)));
	//		dc.Clear();
	//	}*/
	//	wxFont font = GetFont();
	//	font.SetPointSize(font.GetPointSize() * m_View->GetScale());
	//	dc.SetFont(font);
	//	m_View->OnDraw(&dc);

	//	/*double scale = m_View->GetScale();
	//	dc.SetBrush(wxBrush(*wxWHITE, wxCROSSDIAG_HATCH ));
	//	dc.SetPen(*wxWHITE_PEN);
	//	for(size_t i = 0; i < m_HotSpots.Count(); i++)
	//	{
	//		dc.DrawRectangle(m_HotSpots[i].GetRect().GetLeft(),
	//			m_HotSpots[i].GetRect().GetTop(),
	//			m_HotSpots[i].GetRect().GetWidth(),
	//			m_HotSpots[i].GetRect().GetHeight());
	//	}
	//	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	//	dc.SetPen(*wxBLACK_PEN);
	//	dc.DrawRectangle(m_SelectionRect);*/
	//}
}

void wxJigsawEditorCanvas::DrawHotSpot(wxDC * dc, double scale)
{
	if((m_View->GetSelectedObject() || GetDragShape()) && m_ActiveHotSpot && m_HotSpotBitmap.IsOk())
	{
		wxPoint scrollPos = GetScrollPosition();
		/*wxLogTrace(wxTraceMask(), _("HotSpot bitmap: %i,%i"),
			m_HotSpotBitmap.GetWidth(), m_HotSpotBitmap.GetHeight());*/
		dc->DrawBitmap(m_HotSpotBitmap, 
			m_ActiveHotSpot->GetRect().GetLeft()-scrollPos.x,
			m_ActiveHotSpot->GetRect().GetTop()-scrollPos.y,
			true);
	}
}


/*!
 * wxEVT_IDLE event handler for ID_WXJIGSAWEDITORCANVAS
 */

void wxJigsawEditorCanvas::OnIdle( wxIdleEvent& event )
{
	do
	{
		if(!HasCapture()) break;

		// get scroll position
		wxPoint scrollPos = GetScrollPosition();
		// get mouse in client coordinates
		wxPoint currentPos = ScreenToClient(wxGetMousePosition());

		// auto scroll
		// check current drag position and update scroll regularly
		if(AutoScroll(currentPos, scrollPos))
		{
			event.RequestMore();
		}
		FixViewOffset();
	}
	while(false);
}


/*!
 * wxEVT_ERASE_BACKGROUND event handler for ID_WXJIGSAWEDITORCANVAS
 */

void wxJigsawEditorCanvas::OnEraseBackground( wxEraseEvent& event )
{
}


/*!
 * wxEVT_LEFT_DOWN event handler for ID_WXJIGSAWEDITORCANVAS
 */


void wxJigsawEditorCanvas::OnLeftDown( wxMouseEvent& event )
{
	SetFocus();
	wxRect displayRect = GetDisplayRect(GetScrollPosition());
	do
	{
		if(!m_View) break;
		wxJigsawEditorDocument * document = GetDocument();
		if(!document) break;
		document->ReCreateHotSpots(m_DoubleBufferDC, m_HotSpots, NULL, m_View->GetScale());
		wxPoint diagramPoint = PointToViewPoint(event.GetPosition());
		wxLogTrace(wxTraceMask(), 
			_("wxJigsawEditorCanvas::OnLeftDown: Client Pos (%i, %i); View offset (%i, %i); Point (%i, %i)"),
			event.GetPosition().x, event.GetPosition().y,
			m_View->GetViewOffset().GetWidth(), m_View->GetViewOffset().GetHeight(),
			diagramPoint.x, diagramPoint.y);
		wxJigsawShape * shape(NULL);
		wxJigsawEditorCanvasHitTest hitTest = HitTest(event.GetPosition(), NULL);
		switch(hitTest)
		{
		case wxJSEC_HITTEST_SHAPE:
			shape = m_View->GetShapeFromPoint(m_DoubleBufferDC, diagramPoint, NULL);
			if(shape)
			{
				wxJigsawShapeGroup * group(NULL);
				group = document->GetShapeGroup(shape);
				if(!group)
				{
					group = document->CreateGroupByShape(m_DoubleBufferDC, shape);
				}
				else
				{
					// If it is not the first shape in a group
					if(group->GetShapes().IndexOf(shape) > 0)
					{
						// Then extract the shape and all shapes after it into separate group
						group = document->CreateGroupByShape(m_DoubleBufferDC, shape);
					}
				}
				if(!group) 
				{
					wxLogTrace(wxTraceMask(), _("Error obtaining group of shapes"));
					break;
				}
				else
				{
					m_View->BringToFront(group);
					wxPoint groupPosition = m_View->GetRealGroupPosition(group);
					m_SelectedObjectOffset = wxSize(
						diagramPoint.x - groupPosition.x,
						diagramPoint.y - groupPosition.y);
				}
				
				SetSelectedObject(group);
				document->ReCreateHotSpots(m_DoubleBufferDC, m_HotSpots, m_View->GetSelectedObject(), m_View->GetScale());
				FixActiveHotSpot(diagramPoint);
				m_Mode = wxJSEC_MODE_DRAGGING;

				CaptureMouse();
				RefreshBuffer();
			}
			break;
		case wxJSEC_HITTEST_NONE:
		default:
			break;
		}
		//if(displayRect.Contains(event.GetPosition())) break;
		//CaptureMouse();

		RefreshBuffer();
		m_TR = m_BL = event.GetPosition();
		m_SelectionRect.SetPosition(event.GetPosition());
		m_SelectionRect.SetSize(wxSize(0,0));
	}
	while(false);
}


/*!
 * wxEVT_LEFT_UP event handler for ID_WXJIGSAWEDITORCANVAS
 */

void wxJigsawEditorCanvas::OnLeftUp( wxMouseEvent& event )
{
	if(HasCapture())
	{
		ReleaseMouse();

		do
		{
			if(!m_View) break;
			wxJigsawEditorDocument * document = GetDocument();
			if(!document) break;
			wxPoint realPosition = PointToViewPoint(event.GetPosition());
			if(m_View->GetSelectedObject())
			{
				if(document->ProcessDrop(m_DoubleBufferDC, realPosition, 
					m_View->GetSelectedObject(), m_SelectedObjectOffset, m_View->GetScale()))
				{
					SetSelectedObject(NULL);
				}
				document->RequestSizeRecalculation();
				/*wxWindowDC dc(this);
				dc.SetFont(GetFont());
				m_View->OnDraw(&dc);*/
				document->UpdateLayout(m_DoubleBufferDC, m_View->GetScale());
			}
		}
		while(false);

		RefreshBuffer();
		m_TR = m_BL = wxPoint(0,0);
		m_SelectionRect.SetPosition(m_TR);
		m_SelectionRect.SetSize(wxSize(0,0));
		m_ActiveHotSpot = NULL;
		m_HotSpots.Clear();
	}
}


/*!
 * wxEVT_LEFT_DCLICK event handler for ID_WXJIGSAWEDITORCANVAS
 */

void wxJigsawEditorCanvas::OnLeftDClick( wxMouseEvent& event )
{
////@begin wxEVT_LEFT_DCLICK event handler for ID_WXJIGSAWEDITORCANVAS in wxJigsawEditorCanvas.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_LEFT_DCLICK event handler for ID_WXJIGSAWEDITORCANVAS in wxJigsawEditorCanvas. 
}


/*!
 * wxEVT_MOTION event handler for ID_WXJIGSAWEDITORCANVAS
 */

void wxJigsawEditorCanvas::OnMotion( wxMouseEvent& event )
{
	UpdateCursor(event.GetPosition());
	if(HasCapture())
	{
		MotionUpdate(event.GetPosition());
	}
}

void wxJigsawEditorCanvas::MotionUpdate(wxPoint currentPos)
{		
	// get scroll position
	wxPoint scrollPos = GetScrollPosition();
	wxPoint viewPos = PointToViewPoint(currentPos);
	AutoScroll(currentPos, scrollPos);
	FixViewOffset();
	FixActiveHotSpot(viewPos);
	RefreshBuffer();
}

wxJigsawEditorDocument * wxJigsawEditorCanvas::GetDocument()
{
	do
	{
		if(!m_View) break;
		return wxDynamicCast(m_View->GetDocument(), wxJigsawEditorDocument);
	}
	while(false);
	return NULL;
}

wxPoint wxJigsawEditorCanvas::PointToViewPoint(const wxPoint & pos)
{
	do
	{
		if(!m_View) break;
		return wxPoint(pos.x - m_View->GetViewOffset().GetWidth(), 
			pos.y - m_View->GetViewOffset().GetHeight());
	}
	while(false);
	return wxDefaultPosition;
}

wxJigsawShape * wxJigsawEditorCanvas::GetShapeFromPoint(const wxPoint & pos,
		wxJigsawShapeGroup * ignoreGroup)
{
	do
	{
		if(!m_View) break;
		wxPoint viewPoint = PointToViewPoint(pos);
		return m_View->GetShapeFromPoint(m_DoubleBufferDC, viewPoint, ignoreGroup);
	}
	while(false);
	return NULL;
}

wxJigsawEditorCanvas::wxJigsawEditorCanvasHitTest wxJigsawEditorCanvas::HitTest(const wxPoint & pos,
		wxJigsawShapeGroup * ignoreGroup)
{
	do
	{
		wxPoint realPoint = PointToViewPoint(pos);
		wxLogTrace(wxTraceMask(), _("wxJigsawEditorCanvas::HitTest - (%i,%i)"),
			realPoint.x, realPoint.y);
		wxJigsawShape * shape = m_View->GetShapeFromPoint(m_DoubleBufferDC, realPoint, ignoreGroup);
		if(!shape) break;
		return wxJSEC_HITTEST_SHAPE;
	}
	while(false);
	return wxJSEC_HITTEST_NONE;
}


wxPoint wxJigsawEditorCanvas::GetScrollPosition()
{
	return wxPoint(GetScrollPos(wxHORIZONTAL),GetScrollPos(wxVERTICAL));
}

wxSize wxJigsawEditorCanvas::GetDiagramSize()
{
	if(!m_View)
	{
		return wxSize(0,0);
	}
	wxSize diagramSize = m_View->GetDiagramSize();
	return wxSize(
		wxMax(GetClientSize().GetWidth(), diagramSize.GetWidth()),
		wxMax(GetClientSize().GetHeight(), diagramSize.GetHeight()));
}

void wxJigsawEditorCanvas::Scroll(const wxPoint & pt)
{
	if((pt.x) >= 0 && (GetScrollPos(wxHORIZONTAL) != pt.x))
		SetScrollPos(wxHORIZONTAL, pt.x);
	if((pt.y) >= 0 && (GetScrollPos(wxVERTICAL) != pt.y))
		SetScrollPos(wxVERTICAL, pt.y);
	RefreshBuffer();
}

wxRect wxJigsawEditorCanvas::GetDisplayRect(const wxPoint & scrollPos)
{
	return wxRect(wxPoint(-scrollPos.x, -scrollPos.y), GetDiagramSize());
}

void wxJigsawEditorCanvas::FixViewOffset()
{
	if(m_View)
	{
		wxPoint pt = GetScrollPosition();
		wxSize offset(-pt.x, -pt.y);
		m_View->SetViewOffset(offset);
	}
}

bool wxJigsawEditorCanvas::AutoScroll(const wxPoint & currentPos, const wxPoint & scrollPos)
{
	bool res = false;

	// If mouse is captured set rect coords if inside the image and autoscroll if posible
	// get image display rectangle
	wxRect displayRect = GetDisplayRect(scrollPos);

	wxSize oldSelection(m_TR.x-m_BL.x, m_TR.y-m_BL.y);

	// check if the current drag position is inside the image - do not allow to draw rectangle out of the image
	m_TR.x = 
		wxMin(wxMax(currentPos.x, displayRect.GetLeft()), displayRect.GetRight());
	m_TR.y = 
		wxMin(wxMax(currentPos.y, displayRect.GetTop()), displayRect.GetBottom());
	
	// Check current drag position and update scroll regularly
	if(currentPos.x <= 0) 
	{
		Scroll(wxPoint(wxMax(scrollPos.x - wxJigsawEditorCanvas::ScrollIncrement, 0), -1));
		m_BL.x += (scrollPos.x - GetScrollPosition().x);
		res = true;
	}
	if(currentPos.y <= 0) 
	{
		Scroll(wxPoint(-1,wxMax(scrollPos.y - wxJigsawEditorCanvas::ScrollIncrement, 0)));
		m_BL.y += (scrollPos.y - GetScrollPosition().y);
		res = true;
	}
	if(currentPos.x >= GetClientSize().GetWidth()) 
	{
		Scroll(wxPoint(scrollPos.x + wxJigsawEditorCanvas::ScrollIncrement, -1));
		m_BL.x -= (GetScrollPosition().x - scrollPos.x);
		res = true;
	}
	if(currentPos.y >= GetClientSize().y) 
	{
		Scroll(wxPoint(-1, scrollPos.y + wxJigsawEditorCanvas::ScrollIncrement));
		m_BL.y -= (GetScrollPosition().y - scrollPos.y);
		res = true;
	}
	m_SelectionRect = wxRect(wxMin(m_TR.x, m_BL.x), wxMin(m_TR.y, m_BL.y),
		abs(m_TR.x-m_BL.x), abs(m_TR.y-m_BL.y));
	if(HasCapture())
	{
		if(m_View && m_View->GetSelectedObject() && (m_Mode == wxJSEC_MODE_DRAGGING))
		{
			/*wxSize offset(m_TR.x-m_BL.x-oldSelection.GetWidth(),
				m_TR.y-m_BL.y-oldSelection.GetHeight());*/
			wxPoint cursorPosOnDiagram = currentPos;
			
			cursorPosOnDiagram.x /= m_View->GetScale();
			cursorPosOnDiagram.y /= m_View->GetScale();
			cursorPosOnDiagram.x -= m_View->GetViewOffset().x;
			cursorPosOnDiagram.y -= m_View->GetViewOffset().y;

			m_SelectedObjectOffset.x /= m_View->GetScale();
			m_SelectedObjectOffset.y /= m_View->GetScale();
			m_View->GetSelectedObject()->Drag(m_DoubleBufferDC, cursorPosOnDiagram, m_SelectedObjectOffset,
				m_View->GetScale());
			GetDocument()->Modify(true);
			AdjustScrollBars();
		}
	}
	return res;
}

void wxJigsawEditorCanvas::UpdateCursor(const wxPoint & pos)
{
	do
	{
		if(!m_View) break;
		wxPoint cursorPosOnImage = PointToViewPoint(pos);
		if(HasCapture())
		{
			//SetCursor(GetCursorByHitTest(m_DragAction));
		}
		else
		{
			/*GraphicObject * object = m_View->GetObjectFromPoint(cursorPosOnImage);
			if(!object) break;		
			SetCursor(GetCursorByHitTest(object->HitTest(cursorPosOnImage, m_HitTestClue)));*/
		}
		return;
	}
	while(false);
	SetCursor(wxNullCursor);
}

void wxJigsawEditorCanvas::OnScrollLineUp(wxScrollWinEvent & event)
{
	wxLogTrace(wxTraceMask(), _("wxJigsawEditorCanvas::OnScrollLineUp;"));
	int increment = wxJigsawEditorCanvas::ScrollIncrement;
	SetScrollPos(event.GetOrientation(), GetScrollPos(event.GetOrientation()) - increment);
	FixViewOffset();
	RefreshBuffer();
}

void wxJigsawEditorCanvas::OnScrollLineDown(wxScrollWinEvent & event)
{
	wxLogTrace(wxTraceMask(), _("wxJigsawEditorCanvas::OnScrollLineDown;"));
	int increment = wxJigsawEditorCanvas::ScrollIncrement;
	SetScrollPos(event.GetOrientation(), GetScrollPos(event.GetOrientation()) + increment);
	FixViewOffset();
	RefreshBuffer();
}

void wxJigsawEditorCanvas::OnScrollPageUp(wxScrollWinEvent & event)
{
	wxLogTrace(wxTraceMask(), _("wxJigsawEditorCanvas::OnScrollPageUp;"));
	SetScrollPos(event.GetOrientation(), 
		GetScrollPos(event.GetOrientation()) - GetScrollThumb(event.GetOrientation()));
	FixViewOffset();
	RefreshBuffer();
}

void wxJigsawEditorCanvas::OnScrollPageDown(wxScrollWinEvent & event)
{
	wxLogTrace(wxTraceMask(), _("wxJigsawEditorCanvas::OnScrollPageDown;"));
	SetScrollPos(event.GetOrientation(), 
		GetScrollPos(event.GetOrientation()) + GetScrollThumb(event.GetOrientation()));
	FixViewOffset();
	RefreshBuffer();
}

void wxJigsawEditorCanvas::OnScrollThumbTrack(wxScrollWinEvent & event)
{
	wxLogTrace(wxTraceMask(), _("wxJigsawEditorCanvas::OnScrollThumbTrack; ScrollPos = %i"),
		event.GetPosition());
	SetScrollPos(event.GetOrientation(), event.GetPosition());
	FixViewOffset();
	RefreshBuffer();
}

void wxJigsawEditorCanvas::OnScrollThumbRelease(wxScrollWinEvent & event)
{
	wxLogTrace(wxTraceMask(), _("wxJigsawEditorCanvas::OnScrollThumbRelease; ScrollPos = %i"),
		event.GetPosition());
	SetScrollPos(event.GetOrientation(), event.GetPosition());
	FixViewOffset();
	RefreshBuffer();
}

wxJigsawShape *wxJigsawEditorCanvas::GetDragShape() const
{
	if(GetDropTarget() && GetDropTarget()->GetDataObject())
	{
		DnDJigsawShapeDataObject *pData = (DnDJigsawShapeDataObject *)GetDropTarget()->GetDataObject();
		if(pData && pData->ReadShapeInfo())
		{
			return pData->ReadShapeInfo()->m_shape;
		}
	}

	return NULL;
}

bool wxJigsawEditorCanvas::CreateDragImage(wxJigsawShape * shape)
{
	DestroyDragImage();
	do
	{
		if(!m_View || !shape) break;
		double scale = m_View->GetScale();
		wxSize shapeSize = shape->GetSize();
		wxBitmap bmp(shapeSize.GetWidth() * scale, shapeSize.GetHeight() * scale);
		wxMemoryDC mdc(bmp);
		wxFont font = GetFont();
		font.SetPointSize(font.GetPointSize()*m_View->GetScale());
		mdc.SetFont(font);
		mdc.SetBackground(wxBrush(wxColour(255, 0, 255)));
		mdc.Clear();
		mdc.SetPen(*wxBLACK_PEN);
		//mdc.DrawRectangle(0, 0, bmp.GetWidth(), bmp.GetHeight());
		wxSize offset(-shape->GetPosition().x*scale, -shape->GetPosition().y*scale);
		wxLogTrace(wxTraceMask(), _("DragShape offset = (%i,%i)"), 
			offset.GetWidth(), offset.GetHeight());
		wxPoint oldPos = shape->GetPosition();
		shape->SetPosition(wxPoint(0,0));
		shape->RequestSizeRecalculation();
		shape->Draw(mdc, wxSize(0,0), m_View->GetScale());
		shape->RequestSizeRecalculation();
		shape->SetPosition(oldPos);
		mdc.SelectObject(wxNullBitmap);
		bmp.SetMask(new wxMask(bmp, wxColour(255, 0, 255)));
		m_DragImage = new wxDragImage(bmp);
		return true;
	}
	while(false);
	return false;
}

void wxJigsawEditorCanvas::DestroyDragImage()
{
	wxDELETE(m_DragImage);
}

void wxJigsawEditorCanvas::ProcessDrop(const wxPoint & pos, 
		const wxJigsawShape * shape, 
		const wxSize & hotSpotOffset)
{
	do
	{
		if(!shape) break;
		wxJigsawEditorDocument * document = GetDocument();
		if(!document) break;
		wxPoint realPosition = PointToViewPoint(pos);
		wxJigsawShape * newShape = new wxJigsawShape(*shape);
		newShape->SetPosition(	realPosition.x - hotSpotOffset.GetWidth(),
			realPosition.y - hotSpotOffset.GetHeight());
		wxJigsawShapeGroup * newGroup = document->CreateGroupByShape(m_DoubleBufferDC, newShape);
		if(!newGroup) break;
		document->ProcessDrop(m_DoubleBufferDC, realPosition, newGroup, hotSpotOffset, m_View->GetScale());
		m_View->BringToFront(newGroup);
		RefreshBuffer();
	}
	while(false);
}

wxJigsawHotSpot * wxJigsawEditorCanvas::GetHotSpotByPoint(
	const wxPoint & pos, wxJigsawShapeGroup * targetGroup) const
{
	wxJigsawHotSpot * result(NULL);
	wxJigsawShape * singleShape(NULL);
	wxJigsawShape * firstShape(NULL);
	wxJigsawShape * lastShape(NULL);
	do
	{
		if(targetGroup)
		{
			wxJigsawShapeList::Node * node;
			node = targetGroup->GetShapes().GetFirst();
			if(!node) break;
			if(targetGroup->GetShapes().GetCount() == 1)
			{
				singleShape = node->GetData();
			}
			firstShape = node->GetData();
			node = targetGroup->GetShapes().GetLast();
			if(!node) break;
			lastShape = node->GetData();
		}
		else 
		{
			singleShape = firstShape = lastShape = GetDragShape();		
		}
	}
	while(false);

	if(!firstShape || !lastShape) return NULL;
	for(size_t i = 0; i < m_HotSpots.Count(); i++)
	{
		if(singleShape)
		{
			if((singleShape->GetStyle() == wxJigsawShapeStyle::wxJS_TYPE_DEFAULT) &&
				(m_HotSpots[i].GetHotSpotType() == wxJigsawHotSpotType::wxJS_HOTSPOT_INPUT_PARAMETER))
			{
				continue;
			}
			else if((singleShape->GetStyle() != wxJigsawShapeStyle::wxJS_TYPE_DEFAULT) &&
				(m_HotSpots[i].GetHotSpotType() != wxJigsawHotSpotType::wxJS_HOTSPOT_INPUT_PARAMETER))
			{
				continue;
			}
			else if((singleShape->GetStyle() == wxJigsawShapeStyle::wxJS_TYPE_DEFAULT) &&
				(m_HotSpots[i].GetHotSpotType() != wxJigsawHotSpotType::wxJS_HOTSPOT_INPUT_PARAMETER))
			{
				if(m_HotSpots[i].GetHotSpotType() == wxJigsawHotSpotType::wxJS_HOTSPOT_INNER_NOTCH)
				{
					if(!singleShape->GetHasBump()) continue;
				}
				if(m_HotSpots[i].GetHotSpotType() == wxJigsawHotSpotType::wxJS_HOTSPOT_BUMP)
				{
					if(!singleShape->GetHasBump()) continue;
				}
				if(m_HotSpots[i].GetHotSpotType() == wxJigsawHotSpotType::wxJS_HOTSPOT_NOTCH)
				{
					if(!singleShape->GetHasNotch()) continue;
				}
			}
		}
		else
		{
			if(m_HotSpots[i].GetHotSpotType() == wxJigsawHotSpotType::wxJS_HOTSPOT_INPUT_PARAMETER)
			{
				continue;
			}
			if(m_HotSpots[i].GetHotSpotType() == wxJigsawHotSpotType::wxJS_HOTSPOT_INNER_NOTCH)
			{
				if(!firstShape->GetHasBump() || !lastShape->GetHasNotch()) continue;
			}
			if(m_HotSpots[i].GetHotSpotType() == wxJigsawHotSpotType::wxJS_HOTSPOT_BUMP)
			{
				if(!firstShape->GetHasBump()) continue;
			}
			if(m_HotSpots[i].GetHotSpotType() == wxJigsawHotSpotType::wxJS_HOTSPOT_NOTCH)
			{
				if(!firstShape->GetHasNotch()) continue;
			}			
		}
		if(m_HotSpots[i].GetRect().Contains(pos))
		{
			if(!result || result->GetRect().Contains(pos))
			{
				result = &m_HotSpots[i];
			}
			//return &m_HotSpots[i];
		}
	}
	return result;
}

double wxJigsawEditorCanvas::GetScale() const
{
	if(m_View)
	{
		return m_View->GetScale();
	}
	return 1.0;
}

void wxJigsawEditorCanvas::SetScale(double value)
{
	do
	{
		if(!m_View) break;
		m_View->SetScale(value);
		RefreshBuffer();
		wxJigsawEditorDocument * document = GetDocument();
		if(!document) break;
		document->ReCreateHotSpots(m_DoubleBufferDC, m_HotSpots, m_View->GetSelectedObject(), m_View->GetScale());
	}
	while(false);
}

void wxJigsawEditorCanvas::ReCreateHotSpots()
{
	if(m_View)
	{
		wxJigsawEditorDocument * document = GetDocument();
		if(document) document->ReCreateHotSpots(m_DoubleBufferDC, m_HotSpots, NULL, m_View->GetScale());
	}
}

void wxJigsawEditorCanvas::FixActiveHotSpot(const wxPoint & currentPos)
{	
	do
	{
		if(!m_View) break;
		do
		{
			if(!m_ActiveHotSpot) break;
			if(!m_ActiveHotSpot->GetRect().Contains(currentPos)) break;
			return;
		}
		while(false);
		wxJigsawHotSpot * hotSpot = GetHotSpotByPoint(currentPos, m_View->GetSelectedObject());
		m_ActiveHotSpot = NULL;
		if(!hotSpot) break;


		bool bInput = false;
		wxJigsawShapeStyle style = wxJigsawShapeStyle::wxJS_TYPE_NONE;

		if(m_View->GetSelectedObject())
		{
			//Moving group
			bInput = m_View->GetSelectedObject()->IsInputParameterShapeGroup();
			style = m_View->GetSelectedObject()->GetFirstShape()->GetStyle();
		}
		else
		{
			//Drag over
			if(GetDragShape())
			{	
				style = GetDragShape()->GetStyle();
				bInput = style != wxJigsawShapeStyle::wxJS_TYPE_NONE;				
			}
		}

		if(bInput)
		{
			bool matchStyle = false;
			do 
			{
				if(hotSpot->GetHotSpotType() != wxJigsawHotSpotType::wxJS_HOTSPOT_INPUT_PARAMETER) break;
				if(!hotSpot->GetShape()) break;
				wxJigsawInputParameter * param = 
					hotSpot->GetShape()->GetInputParameters()[hotSpot->GetIndex()];
				if(param->GetStyle() != style) break;
				matchStyle = true;
			} 
			while (false);
			if(!matchStyle) break;
			m_ActiveHotSpot = hotSpot;
			break;
		}		

		m_ActiveHotSpot = hotSpot;
	}
	while(false);
	/*if(m_ActiveHotSpot)
	{
		wxLogTrace(wxTraceMask(), _("Active HotSpot: (%i,%i,%i,%i)"),
			m_ActiveHotSpot->GetLeft(), m_ActiveHotSpot->GetTop(),
			m_ActiveHotSpot->GetRight(), m_ActiveHotSpot->GetBottom());
	}*/
	ReCreateHotSpotBitmap();
}

void wxJigsawEditorCanvas::ReCreateHotSpotBitmap()
{
	do
	{
		if(!m_ActiveHotSpot || !m_View) break;
		m_HotSpotBitmap = wxBitmap(
			m_ActiveHotSpot->GetRect().GetWidth(),
			m_ActiveHotSpot->GetRect().GetHeight());
		wxMemoryDC mdc(m_HotSpotBitmap);
		mdc.SetBackground(*wxBLACK_BRUSH);
		mdc.Clear();
		int w(0), h(0);
		mdc.GetSize(&w, &h);
		mdc.SetBrush(*wxWHITE_BRUSH);
		mdc.DrawRoundedRectangle(0, 0, w, h, wxMin(3, wxMin(h/2, w/2)));
		mdc.SelectObject(wxNullBitmap);
		wxImage img = m_HotSpotBitmap.ConvertToImage();
		if(!img.HasAlpha())
		{
			img.InitAlpha();
		}
		mdc.SelectObject(m_HotSpotBitmap);
		wxColour clr;
		for(size_t x = 0; x < img.GetWidth(); x++)
		{
			for(size_t y = 0; y < img.GetHeight(); y++)
			{
				if(!mdc.GetPixel(x, y, &clr)) continue;
				if(clr == *wxWHITE)
				{
					img.SetAlpha(x, y, 240);
				}
				else
				{
					img.SetAlpha(x, y, 0);
				}
			}
		}
		mdc.SelectObject(wxNullBitmap);
		m_HotSpotBitmap = wxBitmap(img);
		return;
	}
	while(false);
	m_HotSpotBitmap = wxNullBitmap;
}

void wxJigsawEditorCanvas::RefreshBuffer()
{
	do
	{
		if(!m_DoubleBufferDC.IsOk()) break;
		m_DoubleBufferDC.SetBackground(wxBrush(GetBackgroundColour()));
		m_DoubleBufferDC.Clear();
		if(!m_View) break;
		m_DoubleBufferDC.SetFont(GetScaledFont());
		m_View->OnDraw(&m_DoubleBufferDC);
	}
	while(false);
	Refresh();
}

wxFont wxJigsawEditorCanvas::GetScaledFont()
{
	wxFont font = GetFont();
	if(m_View)
	{
		font.SetPointSize(font.GetPointSize() * m_View->GetScale());
	}
	return font;
}


/*!
 * wxEVT_SIZE event handler for ID_WXJIGSAWEDITORCANVAS
 */

void wxJigsawEditorCanvas::OnSize( wxSizeEvent& event )
{
	m_DoubleBufferDC.SelectObject(wxNullBitmap);
	m_DoubleBufferBitmap = wxBitmap(event.GetSize().GetWidth(), event.GetSize().GetHeight());
	m_DoubleBufferDC.SelectObject(m_DoubleBufferBitmap);
	RefreshBuffer();
}

void wxJigsawEditorCanvas::SetSelectedObject(wxJigsawShapeGroup * value) 
{ 	
	m_SelectedObject = value ; 
	m_View->SetSelectedObject(m_SelectedObject);

	//Sed the select event to other windows
	wxCommandEvent event( wxEVT_BEHAVIOR_BLOCK_SELECTED, GetId() );
    event.SetEventObject( this );
	event.SetClientData(m_SelectedObject);

	if(m_SelectedObject) event.SetString(m_SelectedObject->GetFirstShape()->GetName());

    // Send it	
    GetEventHandler()->ProcessEvent( event );
}