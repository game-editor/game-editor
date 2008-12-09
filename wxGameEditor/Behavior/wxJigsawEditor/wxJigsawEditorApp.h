/////////////////////////////////////////////////////////////////////////////
// Name:        wxJigsawEditorApp.h
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     02/03/2008 19:32:23
// RCS-ID:      
// Copyright:   Volodymir (T-Rex) Tryapichko, 2008
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXJIGSAWEDITORAPP_H_
#define _WXJIGSAWEDITORAPP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "wxJigsawEditorMainFrame.h"
////@end includes
#include <wx/wxxmlserializer/XmlSerializer.h>
#include <wxJigsawPalette.h>

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations
class wxJigsawEditorConfig;

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

/*!
 * wxJigsawEditorApp class declaration
 */

class wxJigsawEditorApp: public wxApp
{    
    DECLARE_CLASS( wxJigsawEditorApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    wxJigsawEditorApp();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin wxJigsawEditorApp event handler declarations

////@end wxJigsawEditorApp event handler declarations

////@begin wxJigsawEditorApp member function declarations

	wxDocManager * GetDocManager() const { return m_DocManager ; }
	void SetDocManager(wxDocManager * value) { m_DocManager = value ; }

	wxJigsawShapeList * GetShapeRegistry() const { return m_ShapeRegistry ; }
	void SetShapeRegistry(wxJigsawShapeList * value) { m_ShapeRegistry = value ; }

	wxJigsawEditorConfig * GetConfig() const { return m_Config ; }
	void SetConfig(wxJigsawEditorConfig * value) { m_Config = value ; }

////@end wxJigsawEditorApp member function declarations
	wxJigsawPaletteList & GetPalettes() {return m_Palettes;}

	bool LoadShapeRegistry();
	void InitConfigSerialization();

	wxJigsawShape * LoadShape(const wxString & shapeFileName);
	void LoadConfig();
	void SaveConfig();
	void LoadPalettes();
////@begin wxJigsawEditorApp member variables
	wxDocManager * m_DocManager;
	wxJigsawShapeList * m_ShapeRegistry;
	wxJigsawEditorConfig * m_Config;
////@end wxJigsawEditorApp member variables
private:
	wxXmlSerializer m_XmlIO;
	wxJigsawPaletteList m_Palettes;
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(wxJigsawEditorApp)
////@end declare app

#endif
    // _WXJIGSAWEDITORAPP_H_
