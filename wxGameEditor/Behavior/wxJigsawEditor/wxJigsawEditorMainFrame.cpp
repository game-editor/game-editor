/////////////////////////////////////////////////////////////////////////////
// Name:        wxJigsawEditorMainFrame.cpp
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     02/03/2008 19:34:55
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

#include "wxJigsawEditorMainFrame.h"
#include "wxJigsawEditorApp.h"
#include "DnDJigsawShapeDropTarget.h"

////@begin XPM images
////@end XPM images


/*!
 * wxJigsawEditorMainFrame type definition
 */

IMPLEMENT_CLASS( wxJigsawEditorMainFrame, wxDocParentFrame )


/*!
 * wxJigsawEditorMainFrame event table definition
 */

BEGIN_EVENT_TABLE( wxJigsawEditorMainFrame, wxDocParentFrame )

////@begin wxJigsawEditorMainFrame event table entries
    EVT_MENU( wxID_EXIT, wxJigsawEditorMainFrame::OnEXITClick )
    EVT_UPDATE_UI( wxID_EXIT, wxJigsawEditorMainFrame::OnEXITUpdate )

    EVT_MENU( wxID_ABOUT, wxJigsawEditorMainFrame::OnABOUTClick )

    EVT_SLIDER( ID_SCALE_SLIDER, wxJigsawEditorMainFrame::OnSCALESLIDERUpdated )

    EVT_TEXT_ENTER( ID_SEARCH_TEXTCTRL, wxJigsawEditorMainFrame::OnSEARCHClicked )

    EVT_BUTTON( wxID_FIND, wxJigsawEditorMainFrame::OnSEARCHClicked )

////@end wxJigsawEditorMainFrame event table entries

END_EVENT_TABLE()


/*!
 * wxJigsawEditorMainFrame constructors
 */

wxJigsawEditorMainFrame::wxJigsawEditorMainFrame( wxDocManager *manager, wxFrame *parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
    : wxDocParentFrame( manager, parent, id, caption, pos, size, style )
{
    Init();
    Create( manager, parent, id, caption, pos, size, style );
}


/*!
 * wxJigsawEditorMainFrame creator
 */

bool wxJigsawEditorMainFrame::Create( wxDocManager *manager, wxFrame *parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin wxJigsawEditorMainFrame creation
    SetParent(parent);
    CreateControls();
    Centre();
////@end wxJigsawEditorMainFrame creation
    return true;
}


/*!
 * wxJigsawEditorMainFrame destructor
 */

wxJigsawEditorMainFrame::~wxJigsawEditorMainFrame()
{
////@begin wxJigsawEditorMainFrame destruction
    GetAuiManager().UnInit();
////@end wxJigsawEditorMainFrame destruction
}


/*!
 * Member initialisation
 */

void wxJigsawEditorMainFrame::Init()
{
////@begin wxJigsawEditorMainFrame member initialisation
    m_ScaleValue = 100;
    m_ScaleSlider = NULL;
    m_SearchTextCtrl = NULL;
    m_CategoryList = NULL;
    m_Palette = NULL;
    m_Canvas = NULL;
////@end wxJigsawEditorMainFrame member initialisation
}


/*!
 * Control creation for wxJigsawEditorMainFrame
 */

void wxJigsawEditorMainFrame::CreateControls()
{    
	SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
////@begin wxJigsawEditorMainFrame content construction
    wxJigsawEditorMainFrame* itemDocParentFrame1 = this;

    GetAuiManager().SetManagedWindow(this);

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    itemMenu3->Append(wxID_NEW, _("New\tCtrl+N"), _T(""), wxITEM_NORMAL);
    itemMenu3->Append(wxID_OPEN, _("Open\tCtrl+O"), _T(""), wxITEM_NORMAL);
    itemMenu3->AppendSeparator();
    itemMenu3->Append(wxID_SAVE, _("Save\tCtrl+S"), _T(""), wxITEM_NORMAL);
    itemMenu3->Append(wxID_SAVEAS, _("Save as..."), _T(""), wxITEM_NORMAL);
    itemMenu3->AppendSeparator();
    itemMenu3->Append(wxID_EXIT, _("Exit\tAlt+F4"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu3, _("Menu"));
    wxMenu* itemMenu11 = new wxMenu;
    itemMenu11->Append(wxID_ABOUT, _("About..."), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu11, _("Help"));
    itemDocParentFrame1->SetMenuBar(menuBar);

    wxStatusBar* itemStatusBar13 = new wxStatusBar( itemDocParentFrame1, ID_JIGSAW_EDITOR_MAIN_STATUSBAR, wxST_SIZEGRIP|wxNO_BORDER );
    itemStatusBar13->SetFieldsCount(2);
    itemDocParentFrame1->SetStatusBar(itemStatusBar13);

    wxPanel* itemPanel14 = new wxPanel( itemDocParentFrame1, ID_PALETTE_CONTAINER, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    itemDocParentFrame1->GetAuiManager().AddPane(itemPanel14, wxAuiPaneInfo()
        .Name(_T("Palette")).MinSize(wxSize(200, -1)).BestSize(wxSize(220, -1)).CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(true).Floatable(false));

    wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxVERTICAL);
    itemPanel14->SetSizer(itemBoxSizer15);

    m_ScaleSlider = new wxSlider( itemPanel14, ID_SCALE_SLIDER, 0, 1, 500, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
    itemBoxSizer15->Add(m_ScaleSlider, 0, wxGROW, 5);

    wxArrayString itemChoice17Strings;
    wxChoice* itemChoice17 = new wxChoice( itemPanel14, ID_SHAPE_GROUP_CHOICE, wxDefaultPosition, wxDefaultSize, itemChoice17Strings, 0 );
    itemBoxSizer15->Add(itemChoice17, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText18 = new wxStaticText( itemPanel14, wxID_STATIC, _("Search:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer15->Add(itemStaticText18, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer15->Add(itemBoxSizer19, 0, wxGROW, 0);

    m_SearchTextCtrl = new wxTextCtrl( itemPanel14, ID_SEARCH_TEXTCTRL, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
    itemBoxSizer19->Add(m_SearchTextCtrl, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxButton* itemButton21 = new wxButton( itemPanel14, wxID_FIND, _("&Find"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer19->Add(itemButton21, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxSplitterWindow* itemSplitterWindow22 = new wxSplitterWindow( itemPanel14, ID_SPLITTERWINDOW, wxDefaultPosition, wxSize(100, 100), wxSP_3DBORDER|wxSP_3DSASH|wxNO_BORDER );
    itemSplitterWindow22->SetMinimumPaneSize(0);

    m_CategoryList = new CategoryList( itemSplitterWindow22, ID_CATEGORY_LIST, wxDefaultPosition, wxSize(100, 150), wxSUNKEN_BORDER );

    m_Palette = new wxJigsawShapeListBox( itemSplitterWindow22, ID_PALETTE, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

    itemSplitterWindow22->SplitHorizontally(m_CategoryList, m_Palette, 150);
    itemBoxSizer15->Add(itemSplitterWindow22, 1, wxGROW, 0);

    m_Canvas = new wxJigsawEditorCanvas( itemDocParentFrame1, ID_CANVAS, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
    m_Canvas->SetBackgroundColour(wxColour(128, 128, 128));
    itemDocParentFrame1->GetAuiManager().AddPane(m_Canvas, wxAuiPaneInfo()
        .Name(_T("Canvas")).Centre().CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(true).Floatable(false));

    GetAuiManager().Update();

    // Set validators
    m_ScaleSlider->SetValidator( wxGenericValidator(& m_ScaleValue) );
////@end wxJigsawEditorMainFrame content construction
	TransferDataToWindow();
	m_Canvas->SetDropTarget(new DnDJigsawShapeDropTarget(m_Canvas));
	m_Palette->SetAssociatedCanvas(m_Canvas);
	m_Palette->SetShapes(wxGetApp().GetShapeRegistry());
	m_Palette->Refresh();
	m_Palette->AdjustScrollBars();

	wxLogTrace(wxTraceMask(), _("Palettes = %i"), wxGetApp().GetPalettes().GetCount());
	for(wxJigsawPaletteList::Node * node = wxGetApp().GetPalettes().GetFirst();
		node; node = node->GetNext())
	{
		wxJigsawPalette * palette = node->GetData();
		if(!palette) continue;
		m_CategoryList->AddCategory(palette->GetPaletteName(), &palette->GetShapes());
	}
	Connect(m_CategoryList->GetId(), wxEVT_COMMAND_LISTBOX_SELECTED,
		wxCommandEventHandler(wxJigsawEditorMainFrame::OnCategorySelected));
}

void wxJigsawEditorMainFrame::OnCategorySelected(wxCommandEvent & event)
{
	wxJigsawShapeList * shapes = wxDynamicCast(event.GetClientData(), wxJigsawShapeList);
	if(shapes)
	{
		wxLogTrace(wxTraceMask(), _("%i shapes"), shapes->GetCount());
		m_Palette->SetShapes(shapes);
		m_Palette->Refresh();
		m_Palette->AdjustScrollBars();
	}
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT
 */

void wxJigsawEditorMainFrame::OnEXITClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT in wxJigsawEditorMainFrame.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT in wxJigsawEditorMainFrame. 
}


/*!
 * wxEVT_UPDATE_UI event handler for wxID_EXIT
 */

void wxJigsawEditorMainFrame::OnEXITUpdate( wxUpdateUIEvent& event )
{
////@begin wxEVT_UPDATE_UI event handler for wxID_EXIT in wxJigsawEditorMainFrame.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_UPDATE_UI event handler for wxID_EXIT in wxJigsawEditorMainFrame. 
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_ABOUT
 */

void wxJigsawEditorMainFrame::OnABOUTClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_MENU_SELECTED event handler for wxID_ABOUT in wxJigsawEditorMainFrame.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_MENU_SELECTED event handler for wxID_ABOUT in wxJigsawEditorMainFrame. 
}


/*!
 * Should we show tooltips?
 */

bool wxJigsawEditorMainFrame::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap wxJigsawEditorMainFrame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin wxJigsawEditorMainFrame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end wxJigsawEditorMainFrame bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon wxJigsawEditorMainFrame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin wxJigsawEditorMainFrame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end wxJigsawEditorMainFrame icon retrieval
}


/*!
 * wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SCALE_SLIDER
 */

void wxJigsawEditorMainFrame::OnSCALESLIDERUpdated( wxCommandEvent& event )
{
	if(m_Canvas->GetView())
	{
		m_Canvas->GetView()->SetScale(static_cast<double>(m_ScaleSlider->GetValue())/100.0);
		m_Canvas->Refresh();
	}
}




/*!
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_SEARCH_TEXTCTRL
 */

void wxJigsawEditorMainFrame::OnSEARCHClicked( wxCommandEvent& event )
{
	do 
	{
		m_SearchResults.DeleteContents(false);
		m_SearchResults.Clear();
		wxString searchText = m_SearchTextCtrl->GetValue();
		for(wxJigsawPaletteList::Node * node = wxGetApp().GetPalettes().GetFirst(); 
			node; node = node->GetNext())
		{
			wxJigsawPalette * palette = node->GetData();
			if(!palette) continue;
			for(wxJigsawShapeList::Node * shapeNode = palette->GetShapes().GetFirst();
				shapeNode; shapeNode = shapeNode->GetNext())
			{
				wxJigsawShape * shape = shapeNode->GetData();
				if(!shape) continue;
				if((shape->GetName().Lower().Find(searchText.Lower()) >= 0) ||
					searchText.IsEmpty())
				{
					m_SearchResults.Append(shape);
				}
			}
		}
		m_Palette->SetShapes(&m_SearchResults);
		m_Palette->Refresh();
		m_Palette->AdjustScrollBars();
	} 
	while (false);
}
