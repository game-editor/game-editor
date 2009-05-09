/////////////////////////////////////////////////////////////////////////////
// Name:        wxJigsawShapeDesignerCanvas.h
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     02/03/2008 18:32:59
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXJIGSAWSHAPEDESIGNERCANVAS_H_
#define _WXJIGSAWSHAPEDESIGNERCANVAS_H_


/*!
 * Includes
 */

////@begin includes
////@end includes
#include "wxJigsawShapeDesignerDocument.h"
#include "wxJigsawShapeDesignerView.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxJigsawShapeDesignerCanvas;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_WXJIGSAWSHAPEDESIGNERCANVAS_STYLE wxSIMPLE_BORDER
#define SYMBOL_WXJIGSAWSHAPEDESIGNERCANVAS_IDNAME ID_WXJIGSAWSHAPEDESIGNERCANVAS
#define SYMBOL_WXJIGSAWSHAPEDESIGNERCANVAS_SIZE wxSize(100, 100)
#define SYMBOL_WXJIGSAWSHAPEDESIGNERCANVAS_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * wxJigsawShapeDesignerCanvas class declaration
 */

class wxJigsawShapeDesignerCanvas: public wxWindow
{    
    DECLARE_DYNAMIC_CLASS( wxJigsawShapeDesignerCanvas )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    wxJigsawShapeDesignerCanvas();
    wxJigsawShapeDesignerCanvas(wxWindow* parent, wxWindowID id = ID_WXJIGSAWSHAPEDESIGNERCANVAS, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(100, 100), long style = wxSIMPLE_BORDER);

    /// Creation
    bool Create(wxWindow* parent, wxWindowID id = ID_WXJIGSAWSHAPEDESIGNERCANVAS, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(100, 100), long style = wxSIMPLE_BORDER);

    /// Destructor
    ~wxJigsawShapeDesignerCanvas();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin wxJigsawShapeDesignerCanvas event handler declarations

    /// wxEVT_PAINT event handler for ID_WXJIGSAWSHAPEDESIGNERCANVAS
    void OnPaint( wxPaintEvent& event );

    /// wxEVT_IDLE event handler for ID_WXJIGSAWSHAPEDESIGNERCANVAS
    void OnIdle( wxIdleEvent& event );

    /// wxEVT_ERASE_BACKGROUND event handler for ID_WXJIGSAWSHAPEDESIGNERCANVAS
    void OnEraseBackground( wxEraseEvent& event );

    /// wxEVT_LEFT_DOWN event handler for ID_WXJIGSAWSHAPEDESIGNERCANVAS
    void OnLeftDown( wxMouseEvent& event );

    /// wxEVT_LEFT_UP event handler for ID_WXJIGSAWSHAPEDESIGNERCANVAS
    void OnLeftUp( wxMouseEvent& event );

////@end wxJigsawShapeDesignerCanvas event handler declarations

////@begin wxJigsawShapeDesignerCanvas member function declarations

    wxJigsawShapeDesignerView * GetView() const { return m_View ; }
    void SetView(wxJigsawShapeDesignerView * value) { m_View = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end wxJigsawShapeDesignerCanvas member function declarations
	wxJigsawShapeDesignerDocument * GetDocument();
	wxJigsawShape * GetShape();

    /// Should we show tooltips?
    static bool ShowToolTips();
    
    void AdjustScrollBars();

////@begin wxJigsawShapeDesignerCanvas member variables
    wxJigsawShapeDesignerView * m_View;
    /// Control identifiers
    enum {
        ID_WXJIGSAWSHAPEDESIGNERCANVAS = 10001
    };
////@end wxJigsawShapeDesignerCanvas member variables
};

#endif
    // _WXJIGSAWSHAPEDESIGNERCANVAS_H_
