/////////////////////////////////////////////////////////////////////////////
// Name:        wxJigsawShapeDesignerMainFrame.h
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     02/03/2008 18:53:58
// RCS-ID:      
// Copyright:   Volodymir (T-Rex) Tryapichko, 2008
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXJIGSAWSHAPEDESIGNERMAINFRAME_H_
#define _WXJIGSAWSHAPEDESIGNERMAINFRAME_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/aui/framemanager.h"
#include "wx/docview.h"
#include "wx/treectrl.h"
#include "wx/statusbr.h"
#include "wx/propgrid/propgrid.h"
#include "wx/toolbar.h"
////@end includes
#include "wxJigsawShapeDesignerCanvas.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxTreeCtrl;
class wxPropertyGrid;
class wxJigsawShapeDesignerCanvas;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_WXJIGSAWSHAPEDESIGNERMAINFRAME_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_WXJIGSAWSHAPEDESIGNERMAINFRAME_TITLE _("wxJigsawShape Designer")
#define SYMBOL_WXJIGSAWSHAPEDESIGNERMAINFRAME_IDNAME ID_WXJIGSAWSHAPEDESIGNERMAINFRAME
#define SYMBOL_WXJIGSAWSHAPEDESIGNERMAINFRAME_SIZE wxSize(750, 500)
#define SYMBOL_WXJIGSAWSHAPEDESIGNERMAINFRAME_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * wxJigsawShapeDesignerMainFrame class declaration
 */

class wxJigsawShapeDesignerMainFrame: public wxDocParentFrame
{    
    DECLARE_CLASS( wxJigsawShapeDesignerMainFrame )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    wxJigsawShapeDesignerMainFrame( wxDocManager *manager, wxFrame *parent, wxWindowID id = SYMBOL_WXJIGSAWSHAPEDESIGNERMAINFRAME_IDNAME, const wxString& caption = SYMBOL_WXJIGSAWSHAPEDESIGNERMAINFRAME_TITLE, const wxPoint& pos = SYMBOL_WXJIGSAWSHAPEDESIGNERMAINFRAME_POSITION, const wxSize& size = SYMBOL_WXJIGSAWSHAPEDESIGNERMAINFRAME_SIZE, long style = SYMBOL_WXJIGSAWSHAPEDESIGNERMAINFRAME_STYLE );

    bool Create( wxDocManager *manager, wxFrame *parent, wxWindowID id = SYMBOL_WXJIGSAWSHAPEDESIGNERMAINFRAME_IDNAME, const wxString& caption = SYMBOL_WXJIGSAWSHAPEDESIGNERMAINFRAME_TITLE, const wxPoint& pos = SYMBOL_WXJIGSAWSHAPEDESIGNERMAINFRAME_POSITION, const wxSize& size = SYMBOL_WXJIGSAWSHAPEDESIGNERMAINFRAME_SIZE, long style = SYMBOL_WXJIGSAWSHAPEDESIGNERMAINFRAME_STYLE );

    /// Destructor
    ~wxJigsawShapeDesignerMainFrame();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin wxJigsawShapeDesignerMainFrame event handler declarations

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_PALETTE_EDITOR
    void OnPALETTEEDITORClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT
    void OnExitClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_TREE_SEL_CHANGED event handler for ID_SHAPE_STRUCTURE_TREE
    void OnSHAPESTRUCTURETREESelChanged( wxTreeEvent& event );

    /// wxEVT_PG_CHANGED event handler for ID_PROPERTYGRID
    void OnPROPERTYGRIDPropertyChanged( wxPropertyGridEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_ADD_PARAMETER
    void OnADDPARAMETERClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_ADD_PARAMETER
    void OnADDPARAMETERUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_DEL_PARAMETER
    void OnDELPARAMETERClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_DEL_PARAMETER
    void OnDELPARAMETERUpdate( wxUpdateUIEvent& event );

////@end wxJigsawShapeDesignerMainFrame event handler declarations

////@begin wxJigsawShapeDesignerMainFrame member function declarations

    /// Returns the AUI manager object
    wxAuiManager& GetAuiManager() { return m_auiManager; }

    wxTreeItemId GetInputParametersNode() const { return m_InputParametersNode ; }
    void SetInputParametersNode(wxTreeItemId value) { m_InputParametersNode = value ; }

    wxImageList GetParamTreeImageList() const { return m_ParamTreeImageList ; }
    void SetParamTreeImageList(wxImageList value) { m_ParamTreeImageList = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end wxJigsawShapeDesignerMainFrame member function declarations
	void FillDocumentTree();
	void FixDocumentTreeRoot();

	void FillPropertyGrid();
	void FillPropertyGridByShape();
	void FillPropertyGridByParameter(int index = wxID_ANY);

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin wxJigsawShapeDesignerMainFrame member variables
    wxAuiManager m_auiManager;
    wxTreeCtrl* m_DocumentTree;
    wxPropertyGrid* m_PropertyGrid;
    wxJigsawShapeDesignerCanvas* m_Canvas;
    wxTreeItemId m_InputParametersNode;
    wxImageList m_ParamTreeImageList;
    /// Control identifiers
    enum {
        ID_WXJIGSAWSHAPEDESIGNERMAINFRAME = 10000,
        ID_PALETTE_EDITOR = 10017,
        ID_SHAPE_STRUCTURE_TREE = 10004,
        ID_MAIN_STATUSBAR = 10002,
        ID_PROPERTYGRID = 10005,
        ID_CANVAS = 10003,
        ID_TOOLBAR1 = 10006,
        ID_ADD_PARAMETER = 10007,
        ID_DEL_PARAMETER = 10011
    };
////@end wxJigsawShapeDesignerMainFrame member variables
};

#endif
    // _WXJIGSAWSHAPEDESIGNERMAINFRAME_H_
