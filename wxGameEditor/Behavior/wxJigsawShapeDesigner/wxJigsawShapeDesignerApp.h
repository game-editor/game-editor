/////////////////////////////////////////////////////////////////////////////
// Name:        wxJigsawShapeDesignerApp.h
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     02/03/2008 18:44:02
// RCS-ID:      
// Copyright:   Volodymir (T-Rex) Tryapichko, 2008
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXJIGSAWSHAPEDESIGNERAPP_H_
#define _WXJIGSAWSHAPEDESIGNERAPP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "wxJigsawShapeDesignerMainFrame.h"
////@end includes
#include "wxJigsawShape.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

/*!
 * wxJigsawShapeDesignerApp class declaration
 */

class wxJigsawShapeDesignerApp: public wxApp
{    
    DECLARE_CLASS( wxJigsawShapeDesignerApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    wxJigsawShapeDesignerApp();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin wxJigsawShapeDesignerApp event handler declarations

////@end wxJigsawShapeDesignerApp event handler declarations

////@begin wxJigsawShapeDesignerApp member function declarations

	wxDocManager * GetDocManager() const { return m_DocManager ; }
	void SetDocManager(wxDocManager * value) { m_DocManager = value ; }

	wxJigsawShapeList GetShapeRegistry() const { return m_ShapeRegistry ; }
	void SetShapeRegistry(wxJigsawShapeList value) { m_ShapeRegistry = value ; }

////@end wxJigsawShapeDesignerApp member function declarations
	wxJigsawShapeList & GetShapeRegistry();
	bool LoadShapeRegistry(const wxString & filename);
	bool SaveShapeRegistry(const wxString & filename);

////@begin wxJigsawShapeDesignerApp member variables
	wxDocManager * m_DocManager;
	wxJigsawShapeList m_ShapeRegistry;
////@end wxJigsawShapeDesignerApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(wxJigsawShapeDesignerApp)
////@end declare app

#endif
    // _WXJIGSAWSHAPEDESIGNERAPP_H_
