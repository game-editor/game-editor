/////////////////////////////////////////////////////////////////////////////
// Name:        wxJigsawShapeDesignerPaletteDialog.h
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     08/06/2008 02:14:15
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXJIGSAWSHAPEDESIGNERPALETTEDIALOG_H_
#define _WXJIGSAWSHAPEDESIGNERPALETTEDIALOG_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/listctrl.h"
////@end includes
#include <wxJigsawPalette.h>
#include <wx/wxxmlserializer/XmlSerializer.h>


/*!
 * Forward declarations
 */

////@begin forward declarations
class wxListView;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_WXJIGSAWSHAPEDESIGNERPALETTEDIALOG_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_WXJIGSAWSHAPEDESIGNERPALETTEDIALOG_TITLE _("Palette")
#define SYMBOL_WXJIGSAWSHAPEDESIGNERPALETTEDIALOG_IDNAME ID_WXJIGSAWSHAPEDESIGNERPALETTEDIALOG
#define SYMBOL_WXJIGSAWSHAPEDESIGNERPALETTEDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_WXJIGSAWSHAPEDESIGNERPALETTEDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * wxJigsawShapeDesignerPaletteDialog class declaration
 */

class wxJigsawShapeDesignerPaletteDialog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( wxJigsawShapeDesignerPaletteDialog )
    DECLARE_EVENT_TABLE()

protected:
	wxXmlSerializer m_XmlIO;
	wxXmlSerializer m_PaletteXmlIO;
public:
    /// Constructors
    wxJigsawShapeDesignerPaletteDialog();
    wxJigsawShapeDesignerPaletteDialog( wxWindow* parent, wxWindowID id = SYMBOL_WXJIGSAWSHAPEDESIGNERPALETTEDIALOG_IDNAME, const wxString& caption = SYMBOL_WXJIGSAWSHAPEDESIGNERPALETTEDIALOG_TITLE, const wxPoint& pos = SYMBOL_WXJIGSAWSHAPEDESIGNERPALETTEDIALOG_POSITION, const wxSize& size = SYMBOL_WXJIGSAWSHAPEDESIGNERPALETTEDIALOG_SIZE, long style = SYMBOL_WXJIGSAWSHAPEDESIGNERPALETTEDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WXJIGSAWSHAPEDESIGNERPALETTEDIALOG_IDNAME, const wxString& caption = SYMBOL_WXJIGSAWSHAPEDESIGNERPALETTEDIALOG_TITLE, const wxPoint& pos = SYMBOL_WXJIGSAWSHAPEDESIGNERPALETTEDIALOG_POSITION, const wxSize& size = SYMBOL_WXJIGSAWSHAPEDESIGNERPALETTEDIALOG_SIZE, long style = SYMBOL_WXJIGSAWSHAPEDESIGNERPALETTEDIALOG_STYLE );

    /// Destructor
    ~wxJigsawShapeDesignerPaletteDialog();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin wxJigsawShapeDesignerPaletteDialog event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_LOAD_PALETTE
    void OnLOADPALETTEClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SAVE_PALETTE
    void OnSAVEPALETTEClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ADD_SHAPE
    void OnADDSHAPEClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DELETE_SHAPE
    void OnDELETESHAPEClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_DELETE_SHAPE
    void OnDELETESHAPEUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_CLEAR_PALETTE
    void OnCLEARPALETTEClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_CLEAR_PALETTE
    void OnCLEARPALETTEUpdate( wxUpdateUIEvent& event );

////@end wxJigsawShapeDesignerPaletteDialog event handler declarations

////@begin wxJigsawShapeDesignerPaletteDialog member function declarations

    wxJigsawShape * GetLoadedShape() const { return m_LoadedShape ; }
    void SetLoadedShape(wxJigsawShape * value) { m_LoadedShape = value ; }

    wxJigsawPalette * GetPalette() const { return m_Palette ; }
    void SetPalette(wxJigsawPalette * value) { m_Palette = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end wxJigsawShapeDesignerPaletteDialog member function declarations
	void FillShapeList();

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin wxJigsawShapeDesignerPaletteDialog member variables
    wxListView* m_ShapeList;
    wxJigsawShape * m_LoadedShape;
    wxJigsawPalette * m_Palette;
    /// Control identifiers
    enum {
        ID_WXJIGSAWSHAPEDESIGNERPALETTEDIALOG = 10012,
        ID_LOAD_PALETTE = 10000,
        ID_SAVE_PALETTE = 10001,
        ID_SHAPES_LIST = 10013,
        ID_ADD_SHAPE = 10014,
        ID_DELETE_SHAPE = 10015,
        ID_CLEAR_PALETTE = 10016
    };
////@end wxJigsawShapeDesignerPaletteDialog member variables
};

#endif
    // _WXJIGSAWSHAPEDESIGNERPALETTEDIALOG_H_
