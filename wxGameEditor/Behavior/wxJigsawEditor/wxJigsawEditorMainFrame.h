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
// Name:        wxJigsawEditorMainFrame.h
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     02/03/2008 19:34:55
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXJIGSAWEDITORMAINFRAME_H_
#define _WXJIGSAWEDITORMAINFRAME_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/aui/framemanager.h"
#include "wx/docview.h"
#include "wx/statusbr.h"
#include "wx/valgen.h"
#include "wx/splitter.h"
////@end includes
#include "wxJigsawEditorCanvas.h"
#include "wxJigsawShapeListBox.h"
#include "CategoryList.h"

#include <wx/wxxmlserializer/XmlSerializer.h>
#include <wxJigsawPalette.h>

/*!
 * Forward declarations
 */

////@begin forward declarations
class CategoryList;
class wxJigsawShapeListBox;
class wxJigsawEditorCanvas;
class wxJigsawEditorConfig;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_WXJIGSAWEDITORMAINFRAME_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxCLOSE_BOX
#define SYMBOL_WXJIGSAWEDITORMAINFRAME_TITLE _("wxJigsaw Editor")
#define SYMBOL_WXJIGSAWEDITORMAINFRAME_IDNAME ID_WXJIGSAWEDITORMAINFRAME
#define SYMBOL_WXJIGSAWEDITORMAINFRAME_SIZE wxSize(750, 450)
#define SYMBOL_WXJIGSAWEDITORMAINFRAME_POSITION wxDefaultPosition
////@end control identifiers

#define USE_WXPANEL_FOR_FRAME

#ifdef USE_WXPANEL_FOR_FRAME
#define wxBehaviorFrame wxPanel
#else
#define wxBehaviorFrame wxDocParentFrame
#endif

/*!
 * wxJigsawEditorMainFrame class declaration
 */

class wxJigsawEditorMainFrame: public wxBehaviorFrame
{    
    DECLARE_CLASS( wxJigsawEditorMainFrame )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    wxJigsawEditorMainFrame( wxDocManager *manager, wxFrame *parent, wxWindowID id = SYMBOL_WXJIGSAWEDITORMAINFRAME_IDNAME, const wxString& caption = SYMBOL_WXJIGSAWEDITORMAINFRAME_TITLE, const wxPoint& pos = SYMBOL_WXJIGSAWEDITORMAINFRAME_POSITION, const wxSize& size = SYMBOL_WXJIGSAWEDITORMAINFRAME_SIZE, long style = SYMBOL_WXJIGSAWEDITORMAINFRAME_STYLE );

    bool Create( wxDocManager *manager, wxFrame *parent, wxWindowID id = SYMBOL_WXJIGSAWEDITORMAINFRAME_IDNAME, const wxString& caption = SYMBOL_WXJIGSAWEDITORMAINFRAME_TITLE, const wxPoint& pos = SYMBOL_WXJIGSAWEDITORMAINFRAME_POSITION, const wxSize& size = SYMBOL_WXJIGSAWEDITORMAINFRAME_SIZE, long style = SYMBOL_WXJIGSAWEDITORMAINFRAME_STYLE );

    /// Destructor
    ~wxJigsawEditorMainFrame();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin wxJigsawEditorMainFrame event handler declarations

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT
    void OnEXITClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for wxID_EXIT
    void OnEXITUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_ABOUT
    void OnABOUTClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SCALE_SLIDER
    void OnSCALESLIDERUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_ENTER event handler for ID_SEARCH_TEXTCTRL
    void OnSEARCHClicked( wxCommandEvent& event );

	void SearchStyle(wxJigsawShapeStyle style);

	

////@end wxJigsawEditorMainFrame event handler declarations
	void OnCategorySelected(wxCommandEvent & event);

////@begin wxJigsawEditorMainFrame member function declarations

    /// Returns the AUI manager object
    wxAuiManager& GetAuiManager() { return m_auiManager; }

    int GetScaleValue() const { return m_ScaleValue ; }
    void SetScaleValue(int value) { m_ScaleValue = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end wxJigsawEditorMainFrame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

	wxJigsawShape * LoadShape(const wxString & shapeFileName);
	void LoadConfig();
	void SaveConfig();
	void LoadPalettes();
	bool LoadShapeRegistry();
	void InitConfigSerialization();

	wxString GetCode();

	wxJigsawShapeList * GetShapeRegistry() const { return m_ShapeRegistry ; }
	void SetShapeRegistry(wxJigsawShapeList * value) { m_ShapeRegistry = value ; }

	wxJigsawEditorConfig * GetConfig() const { return m_Config ; }
	void SetConfig(wxJigsawEditorConfig * value) { m_Config = value ; }


	wxJigsawPaletteList & GetPalettes() {return m_Palettes;}

	wxJigsawShapeListBox* GetPalette() {return m_Palette;}

	static wxJigsawEditorMainFrame *Get() {return pFrame;}

	void RefreshCanvas();
	wxJigsawEditorCanvas *GetCanvas() const {return m_Canvas;}

	bool IsSearchStyleSelected() {return m_Palette && (m_Palette->GetShapes() == &m_SearchStyleResults);}

////@begin wxJigsawEditorMainFrame member variables
    wxAuiManager m_auiManager;
    wxSlider* m_ScaleSlider;
    wxTextCtrl* m_SearchTextCtrl;
    CategoryList* m_CategoryList;
    wxJigsawShapeListBox* m_Palette;
    wxJigsawEditorCanvas* m_Canvas;
    int m_ScaleValue;
    /// Control identifiers
    enum {
        ID_WXJIGSAWEDITORMAINFRAME = 10000,
        ID_JIGSAW_EDITOR_MAIN_STATUSBAR = 10002,
        ID_PALETTE_CONTAINER = 10004,
        ID_SCALE_SLIDER = 10007,
        ID_SHAPE_GROUP_CHOICE = 10005,
        ID_SEARCH_TEXTCTRL = 10012,
        ID_SPLITTERWINDOW = 10011,
        ID_CATEGORY_LIST = 10010,
        ID_PALETTE = 10006,
        ID_CANVAS = 10003
    };
////@end wxJigsawEditorMainFrame member variables
	wxJigsawShapeList m_SearchResults;
	wxJigsawShapeList m_SearchStyleResults;

	wxJigsawShapeList * m_ShapeRegistry;
	wxJigsawEditorConfig * m_Config;
////@end wxJigsawEditorApp member variables
private:
	wxXmlSerializer m_XmlIO;
	wxJigsawPaletteList m_Palettes;
	static wxJigsawEditorMainFrame *pFrame;
};

#endif
    // _WXJIGSAWEDITORMAINFRAME_H_
