// Behavior.h: interface for the Behavior class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PANELEVENTS_H__721C4D92_A22B_4720_8A41_2FF2AAAD9C06_ASDFFSE___INCLUDED_)
#define AFX_PANELEVENTS_H__721C4D92_A22B_4720_8A41_2FF2AAAD9C06_ASDFFSE___INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*!
 * Includes
 */

////@begin includes
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/aui/framemanager.h"
#include "wx/docview.h"
#include "wx/statusbr.h"
#include "wx/valgen.h"
#include "wx/splitter.h"
////@end includes
#include "wxJigsawEditor/wxJigsawEditorCanvas.h"
#include "wxJigsawEditor/wxJigsawShapeListBox.h"
#include "wxJigsawEditor/CategoryList.h"

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

class Behavior : public wxPanel  
{
	DECLARE_CLASS(Behavior)
    DECLARE_EVENT_TABLE()

public:
	Behavior(wxWindow *parent);
	virtual ~Behavior();
	
	static Behavior *Get() {return pBehavior;}
	
	bool Create( wxDocManager *manager, wxFrame *parent, wxWindowID id = SYMBOL_WXJIGSAWEDITORMAINFRAME_IDNAME, const wxString& caption = SYMBOL_WXJIGSAWEDITORMAINFRAME_TITLE, const wxPoint& pos = SYMBOL_WXJIGSAWEDITORMAINFRAME_POSITION, const wxSize& size = SYMBOL_WXJIGSAWEDITORMAINFRAME_SIZE, long style = SYMBOL_WXJIGSAWEDITORMAINFRAME_STYLE );

  

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin Behavior event handler declarations

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

////@end Behavior event handler declarations
	void OnCategorySelected(wxCommandEvent & event);

////@begin Behavior member function declarations

    /// Returns the AUI manager object
    wxAuiManager& GetAuiManager() { return m_auiManager; }

    int GetScaleValue() const { return m_ScaleValue ; }
    void SetScaleValue(int value) { m_ScaleValue = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end Behavior member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

	wxJigsawShape * LoadShape(const wxString & shapeFileName);
	void LoadConfig();
	void SaveConfig();
	void LoadPalettes();
	bool LoadShapeRegistry();
	void InitConfigSerialization();

	wxJigsawShapeList * GetShapeRegistry() const { return m_ShapeRegistry ; }
	void SetShapeRegistry(wxJigsawShapeList * value) { m_ShapeRegistry = value ; }

	wxJigsawEditorConfig * GetConfig() const { return m_Config ; }
	void SetConfig(wxJigsawEditorConfig * value) { m_Config = value ; }


	wxJigsawPaletteList & GetPalettes() {return m_Palettes;}

////@begin Behavior member variables
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
////@end Behavior member variables
	wxJigsawShapeList m_SearchResults;

	wxJigsawShapeList * m_ShapeRegistry;
	wxJigsawEditorConfig * m_Config;
////@end wxJigsawEditorApp member variables
private:
	wxXmlSerializer m_XmlIO;
	wxJigsawPaletteList m_Palettes;

private:
	static Behavior *pBehavior;
};


#endif // !defined(AFX_PANELEVENTS_H__721C4D92_A22B_4720_8A41_2FF2AAAD9C06_ASDFFSE___INCLUDED_)
