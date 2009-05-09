/////////////////////////////////////////////////////////////////////////////
// Name:        wxJigsawShapeDesignerCanvas.cpp
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     02/03/2008 18:32:59
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

#include "wxJigsawShapeDesignerCanvas.h"

////@begin XPM images
////@end XPM images


/*!
 * wxJigsawShapeDesignerCanvas type definition
 */

IMPLEMENT_DYNAMIC_CLASS( wxJigsawShapeDesignerCanvas, wxWindow )


/*!
 * wxJigsawShapeDesignerCanvas event table definition
 */

BEGIN_EVENT_TABLE( wxJigsawShapeDesignerCanvas, wxWindow )

////@begin wxJigsawShapeDesignerCanvas event table entries
    EVT_PAINT( wxJigsawShapeDesignerCanvas::OnPaint )
    EVT_IDLE( wxJigsawShapeDesignerCanvas::OnIdle )
    EVT_ERASE_BACKGROUND( wxJigsawShapeDesignerCanvas::OnEraseBackground )
    EVT_LEFT_DOWN( wxJigsawShapeDesignerCanvas::OnLeftDown )
    EVT_LEFT_UP( wxJigsawShapeDesignerCanvas::OnLeftUp )

////@end wxJigsawShapeDesignerCanvas event table entries

END_EVENT_TABLE()


/*!
 * wxJigsawShapeDesignerCanvas constructors
 */

wxJigsawShapeDesignerCanvas::wxJigsawShapeDesignerCanvas()
{
    Init();
}

wxJigsawShapeDesignerCanvas::wxJigsawShapeDesignerCanvas(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}


/*!
 * wxJigsawShapeDesignerCanvas creator
 */

bool wxJigsawShapeDesignerCanvas::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
////@begin wxJigsawShapeDesignerCanvas creation
    wxWindow::Create(parent, id, pos, size, style);
    CreateControls();
////@end wxJigsawShapeDesignerCanvas creation
    return true;
}


/*!
 * wxJigsawShapeDesignerCanvas destructor
 */

wxJigsawShapeDesignerCanvas::~wxJigsawShapeDesignerCanvas()
{
////@begin wxJigsawShapeDesignerCanvas destruction
////@end wxJigsawShapeDesignerCanvas destruction
}


/*!
 * Member initialisation
 */

void wxJigsawShapeDesignerCanvas::Init()
{
////@begin wxJigsawShapeDesignerCanvas member initialisation
    m_View = NULL;
////@end wxJigsawShapeDesignerCanvas member initialisation
}


/*!
 * Control creation for wxJigsawShapeDesignerCanvas
 */

void wxJigsawShapeDesignerCanvas::CreateControls()
{    
////@begin wxJigsawShapeDesignerCanvas content construction
////@end wxJigsawShapeDesignerCanvas content construction
}


/*!
 * Should we show tooltips?
 */

bool wxJigsawShapeDesignerCanvas::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap wxJigsawShapeDesignerCanvas::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin wxJigsawShapeDesignerCanvas bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end wxJigsawShapeDesignerCanvas bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon wxJigsawShapeDesignerCanvas::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin wxJigsawShapeDesignerCanvas icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end wxJigsawShapeDesignerCanvas icon retrieval
}


/*!
 * wxEVT_PAINT event handler for ID_WXJIGSAWSHAPEDESIGNERCANVAS
 */

void wxJigsawShapeDesignerCanvas::OnPaint( wxPaintEvent& )
{
////@begin wxEVT_PAINT event handler for ID_WXJIGSAWSHAPEDESIGNERCANVAS in wxJigsawShapeDesignerCanvas.
    // Before editing this code, remove the block markers.
    wxPaintDC dc(this);
////@end wxEVT_PAINT event handler for ID_WXJIGSAWSHAPEDESIGNERCANVAS in wxJigsawShapeDesignerCanvas. 
	dc.SetFont(GetFont());
	dc.SetTextForeground(GetForegroundColour());
	dc.SetBackground(wxBrush(GetBackgroundColour()));
	dc.Clear();
	if(m_View)
	{
		m_View->OnDraw(&dc);
	}
}


/*!
 * wxEVT_IDLE event handler for ID_WXJIGSAWSHAPEDESIGNERCANVAS
 */

void wxJigsawShapeDesignerCanvas::OnIdle( wxIdleEvent& event )
{
////@begin wxEVT_IDLE event handler for ID_WXJIGSAWSHAPEDESIGNERCANVAS in wxJigsawShapeDesignerCanvas.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_IDLE event handler for ID_WXJIGSAWSHAPEDESIGNERCANVAS in wxJigsawShapeDesignerCanvas. 
}


/*!
 * wxEVT_ERASE_BACKGROUND event handler for ID_WXJIGSAWSHAPEDESIGNERCANVAS
 */

void wxJigsawShapeDesignerCanvas::OnEraseBackground( wxEraseEvent& event )
{
////@begin wxEVT_ERASE_BACKGROUND event handler for ID_WXJIGSAWSHAPEDESIGNERCANVAS in wxJigsawShapeDesignerCanvas.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_ERASE_BACKGROUND event handler for ID_WXJIGSAWSHAPEDESIGNERCANVAS in wxJigsawShapeDesignerCanvas. 
}


/*!
 * wxEVT_LEFT_DOWN event handler for ID_WXJIGSAWSHAPEDESIGNERCANVAS
 */

void wxJigsawShapeDesignerCanvas::OnLeftDown( wxMouseEvent& event )
{
////@begin wxEVT_LEFT_DOWN event handler for ID_WXJIGSAWSHAPEDESIGNERCANVAS in wxJigsawShapeDesignerCanvas.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_LEFT_DOWN event handler for ID_WXJIGSAWSHAPEDESIGNERCANVAS in wxJigsawShapeDesignerCanvas. 
}


/*!
 * wxEVT_LEFT_UP event handler for ID_WXJIGSAWSHAPEDESIGNERCANVAS
 */

void wxJigsawShapeDesignerCanvas::OnLeftUp( wxMouseEvent& event )
{
////@begin wxEVT_LEFT_UP event handler for ID_WXJIGSAWSHAPEDESIGNERCANVAS in wxJigsawShapeDesignerCanvas.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_LEFT_UP event handler for ID_WXJIGSAWSHAPEDESIGNERCANVAS in wxJigsawShapeDesignerCanvas. 
}

void wxJigsawShapeDesignerCanvas::AdjustScrollBars()
{
}

wxJigsawShapeDesignerDocument * wxJigsawShapeDesignerCanvas::GetDocument()
{
	do
	{
		if(!m_View) break;
		return wxDynamicCast(m_View->GetDocument(), wxJigsawShapeDesignerDocument);
	}
	while(false);
	return NULL;
}

wxJigsawShape * wxJigsawShapeDesignerCanvas::GetShape()
{
	do
	{
		wxJigsawShapeDesignerDocument * document = GetDocument();
		if(!document) break;
		return document->GetShape();
	}
	while(false);
	return NULL;
}
