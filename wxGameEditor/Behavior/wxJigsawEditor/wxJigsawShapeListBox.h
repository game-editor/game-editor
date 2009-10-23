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
// Name:        wxJigsawShapeListBox.h
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     24/03/2008 23:19:47
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXJIGSAWSHAPELISTBOX_H_
#define _WXJIGSAWSHAPELISTBOX_H_


/*!
 * Includes
 */

////@begin includes
////@end includes
#include "wxJigsawShape.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxJigsawShapeListBox;
////@end forward declarations
class wxJigsawEditorCanvas;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_WXJIGSAWSHAPELISTBOX 10000
#define SYMBOL_WXJIGSAWSHAPELISTBOX_STYLE wxSIMPLE_BORDER
#define SYMBOL_WXJIGSAWSHAPELISTBOX_IDNAME ID_WXJIGSAWSHAPELISTBOX
#define SYMBOL_WXJIGSAWSHAPELISTBOX_SIZE wxSize(150, 300)
#define SYMBOL_WXJIGSAWSHAPELISTBOX_POSITION wxDefaultPosition
////@end control identifiers


/// \brief wxJigsawShape palette list box
 class wxJigsawShapeListBox: public wxWindow
{    
    DECLARE_DYNAMIC_CLASS( wxJigsawShapeListBox )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    wxJigsawShapeListBox();
	/// Constructor
    wxJigsawShapeListBox(wxWindow* parent, wxWindowID id = ID_WXJIGSAWSHAPELISTBOX, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(100, 100), long style = wxSIMPLE_BORDER);

    /// Creation
    bool Create(wxWindow* parent, wxWindowID id = ID_WXJIGSAWSHAPELISTBOX, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(100, 100), long style = wxSIMPLE_BORDER);

    /// Destructor
    ~wxJigsawShapeListBox();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin wxJigsawShapeListBox event handler declarations

    /// wxEVT_PAINT event handler for ID_WXJIGSAWSHAPELISTBOX
    void OnPaint( wxPaintEvent& event );

    /// wxEVT_IDLE event handler for ID_WXJIGSAWSHAPELISTBOX
    void OnIdle( wxIdleEvent& event );

    /// wxEVT_ERASE_BACKGROUND event handler for ID_WXJIGSAWSHAPELISTBOX
    void OnEraseBackground( wxEraseEvent& event );

    /// wxEVT_LEFT_DOWN event handler for ID_WXJIGSAWSHAPELISTBOX
    void OnLeftDown( wxMouseEvent& event );

    /// wxEVT_MOTION event handler for ID_WXJIGSAWSHAPELISTBOX
    void OnMotion( wxMouseEvent& event );

////@end wxJigsawShapeListBox event handler declarations
	/// wxEVT_SCROLL_LINEUP event handler for ID_WXJIGSAWSHAPELISTBOX
	void OnScrollLineUp(wxScrollEvent & event);
	/// wxEVT_SCROLL_LINEDOWN event handler for ID_WXJIGSAWSHAPELISTBOX
	void OnScrollLineDown(wxScrollEvent & event);
	/// wxEVT_SCROLL_PAGEUP event handler for ID_WXJIGSAWSHAPELISTBOX
	void OnScrollPageUp(wxScrollEvent & event);
	/// wxEVT_SCROLL_PAGEDOWN event handler for ID_WXJIGSAWSHAPELISTBOX
	void OnScrollPageDown(wxScrollEvent & event);
	/// wxEVT_SCROLL_THUMBTRACK event handler for ID_WXJIGSAWSHAPELISTBOX
	void OnScrollThumbTrack(wxScrollEvent & event);
	/// wxEVT_SCROLL_THUMBRELEASE event handler for ID_WXJIGSAWSHAPELISTBOX
	void OnScrollThumbRelease(wxScrollEvent & event);

	void OnTimer(wxTimerEvent &event);

////@begin wxJigsawShapeListBox member function declarations

    wxJigsawShapeList * GetShapes() const { return m_Shapes ; }
    void SetShapes(wxJigsawShapeList * value) { m_Shapes = value ; }

    wxJigsawEditorCanvas * GetAssociatedCanvas() const { return m_AssociatedCanvas ; }
    void SetAssociatedCanvas(wxJigsawEditorCanvas * value) { m_AssociatedCanvas = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end wxJigsawShapeListBox member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

	/// Returns the scroll position
	wxPoint GetScrollPosition();
	/// Converts client point to logical point
	wxPoint PointToViewPoint(const wxPoint & pos);

	/// Returns the shape under specified point
	wxJigsawShape * GetShapeByPoint(const wxPoint & pos);

	/// Returns the logical size of shape list
	wxSize GetViewSize();
	/// Recreates the scroll bars
	void AdjustScrollBars();
	/// Scrolls to specified point
	void Scroll(const wxPoint & pos);


////@begin wxJigsawShapeListBox member variables
    wxJigsawShapeList * m_Shapes; // List of shapes
    wxJigsawEditorCanvas * m_AssociatedCanvas;
	wxTimer m_TimerMove;
	wxPoint startPosition, endPosition;
	wxJigsawShape *selectedShape;
	double f, df;
////@end wxJigsawShapeListBox member variables
};

#endif
    // _WXJIGSAWSHAPELISTBOX_H_
