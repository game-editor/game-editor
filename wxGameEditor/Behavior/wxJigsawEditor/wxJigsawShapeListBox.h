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
////@end wxJigsawShapeListBox member variables
};

#endif
    // _WXJIGSAWSHAPELISTBOX_H_
