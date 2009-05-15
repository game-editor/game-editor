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

#include "wxJigsawEditorView.h"
#include "wxJigsawShapeCategory.h"
#include "wxJigsawEditorConfig.h"
#include "xsColourDataPropIO.h"
#include <wxJigsawShape.h>
#include <wxJigsawShapePropertyIO.h>
#include <wxJigsawInputParameterPropertyIO.h>
#include "wx/ifm/ifmdefs.h"
#include "../../wxGed/wxGedControls.h"

////@begin XPM images
////@end XPM images


/*!
 * wxJigsawEditorMainFrame type definition
 */

IMPLEMENT_CLASS( wxJigsawEditorMainFrame, wxBehaviorFrame )


/*!
 * wxJigsawEditorMainFrame event table definition
 */

BEGIN_EVENT_TABLE( wxJigsawEditorMainFrame, wxBehaviorFrame )

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
wxJigsawEditorMainFrame *wxJigsawEditorMainFrame::pFrame = NULL;

wxJigsawEditorMainFrame::wxJigsawEditorMainFrame( wxDocManager *manager, wxFrame *parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
#ifdef USE_WXPANEL_FOR_FRAME
    : wxPanel(parent)
#else
	: wxBehaviorFrame( manager, parent, id, caption, pos, size, style )
#endif
{
	pFrame = this;
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
	pFrame = NULL;
	wxDELETE(m_ShapeRegistry);
////@begin wxJigsawEditorMainFrame destruction
    GetAuiManager().UnInit();
////@end wxJigsawEditorMainFrame destruction
}


/*!
 * Member initialisation
 */

void wxJigsawEditorMainFrame::Init()
{
	m_ShapeRegistry = new wxJigsawShapeList;
	m_Config = NULL;
	m_ShapeRegistry->DeleteContents(true);

	InitConfigSerialization();
	/*m_Config->GetShapesLookupDirectories().Add(wxT("."));
	m_Config->GetShapesLookupDirectories().Add(wxT("shapes"));
	SaveConfig();*/
	LoadConfig();
	LoadPalettes();

	LoadShapeRegistry();

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

#ifndef USE_WXPANEL_FOR_FRAME
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
#endif

    wxPanel* itemPanel14 = new wxPanel( itemDocParentFrame1, ID_PALETTE_CONTAINER, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    itemDocParentFrame1->GetAuiManager().AddPane(itemPanel14, wxAuiPaneInfo()
        .Name(_T("Palette")).MinSize(wxSize(250, -1)).BestSize(wxSize(220, -1)).CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(true).Floatable(false));

    wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxVERTICAL);
    itemPanel14->SetSizer(itemBoxSizer15);
	itemPanel14->SetBackgroundColour(wxBackground_Pen);

	//Zoom is not working very well. So, doesn't use it now.
	m_ScaleSlider = NULL;
    /*m_ScaleSlider = new wxSlider( itemPanel14, ID_SCALE_SLIDER, 0, 1, 500, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
    itemBoxSizer15->Add(m_ScaleSlider, 0, wxGROW, 5);*/

    /*wxArrayString itemChoice17Strings;
    wxChoice* itemChoice17 = new wxChoice( itemPanel14, ID_SHAPE_GROUP_CHOICE, wxDefaultPosition, wxDefaultSize, itemChoice17Strings, 0 );
    itemBoxSizer15->Add(itemChoice17, 0, wxGROW|wxALL, 5);*/

    wxStaticText* itemStaticText18 = new wxStaticText( itemPanel14, wxID_STATIC, _(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer15->Add(itemStaticText18, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer15->Add(itemBoxSizer19, 0, wxGROW, 0);

    m_SearchTextCtrl = new wxTextCtrl( itemPanel14, ID_SEARCH_TEXTCTRL, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
    itemBoxSizer19->Add(m_SearchTextCtrl, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);


    wxButton* itemButton21 = new wxButton( itemPanel14, wxID_FIND, _("&Search"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer19->Add(itemButton21, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	

    wxSplitterWindow* itemSplitterWindow22 = new wxSplitterWindow( itemPanel14, ID_SPLITTERWINDOW, wxDefaultPosition, wxSize(100, 200), wxSP_3DBORDER|wxSP_3DSASH|wxNO_BORDER );
    itemSplitterWindow22->SetMinimumPaneSize(0);

    m_CategoryList = new CategoryList( itemSplitterWindow22, ID_CATEGORY_LIST, wxDefaultPosition, wxSize(100, 150), wxSUNKEN_BORDER );
	m_CategoryList->SetBackgroundColour(wxBackground_Pen);

    m_Palette = new wxJigsawShapeListBox( itemSplitterWindow22, ID_PALETTE, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	m_Palette->SetBackgroundColour(wxBackground_Pen);

    itemSplitterWindow22->SplitHorizontally(m_CategoryList, m_Palette, 150);
    itemBoxSizer15->Add(itemSplitterWindow22, 1, wxGROW, 0);

    m_Canvas = new wxJigsawEditorCanvas( itemDocParentFrame1, ID_CANVAS, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
    m_Canvas->SetBackgroundColour(wxColour(128, 128, 128));
    itemDocParentFrame1->GetAuiManager().AddPane(m_Canvas, wxAuiPaneInfo()
        .Name(_T("Canvas")).Centre().CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(true).Floatable(false));

    GetAuiManager().Update();

    // Set validators
	if(m_ScaleSlider)     m_ScaleSlider->SetValidator( wxGenericValidator(& m_ScaleValue) );
////@end wxJigsawEditorMainFrame content construction
	TransferDataToWindow();
	m_Canvas->SetDropTarget(new DnDJigsawShapeDropTarget(m_Canvas));
	m_Palette->SetAssociatedCanvas(m_Canvas);
	m_Palette->SetShapes(GetShapeRegistry());
	m_Palette->Refresh();
	m_Palette->AdjustScrollBars();

	wxLogTrace(wxTraceMask(), _("Palettes = %i"), GetPalettes().GetCount());
	for(wxJigsawPaletteList::Node * node = GetPalettes().GetFirst();
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
		for(wxJigsawPaletteList::Node * node = GetPalettes().GetFirst(); 
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

bool wxJigsawEditorMainFrame::LoadShapeRegistry()
{
	do
	{
		if(!m_ShapeRegistry) break;

		/*wxJigsawShape * shape;

		shape = new wxJigsawShape(wxT("Test 1"), wxEmptyString, wxColour(100, 100, 255));
		shape->SetHasBump(true);
		shape->SetHasNotch(true);
		m_ShapeRegistry->Append(shape);
		shape = new wxJigsawShape(wxT("Test 2"), wxEmptyString, wxColour(0, 180, 0));
		shape->SetStyle(wxJigsawShapeStyle::wxJS_TYPE_NUMERIC);
		shape->GetInputParameters().Append(
			new wxJigsawInputParameter(_(" IF"), wxJigsawShapeStyle::wxJS_TYPE_BOOLEAN, NULL));
		shape->GetInputParameters().Append(
			new wxJigsawInputParameter(_("AND"), wxJigsawShapeStyle::wxJS_TYPE_NUMERIC, NULL));
		m_ShapeRegistry->Append(shape);
		shape = new wxJigsawShape(wxT("Test 3"), wxEmptyString, wxColour(255, 180, 0));
		shape->SetHasBump(true);
		shape->SetHasNotch(true);
		shape->SetHasCShape(true);
		shape->GetInputParameters().Append(
			new wxJigsawInputParameter(_("Param1"), wxJigsawShapeStyle::wxJS_TYPE_NUMERIC, NULL));
		m_ShapeRegistry->Append(shape);

		shape = new wxJigsawShape(wxT("Test 4"), wxEmptyString, wxColour(0, 0, 180));
		shape->SetStyle(wxJigsawShapeStyle::wxJS_TYPE_BOOLEAN);
		shape->GetInputParameters().Append(
			new wxJigsawInputParameter(_(" IF"), wxJigsawShapeStyle::wxJS_TYPE_NUMERIC, NULL));
		shape->GetInputParameters().Append(
			new wxJigsawInputParameter(_("AND"), wxJigsawShapeStyle::wxJS_TYPE_BOOLEAN, NULL));
		m_ShapeRegistry->Append(shape);*/
		return true;
	}
	while(false);
	return false;
}

void wxJigsawEditorMainFrame::InitConfigSerialization()
{
	m_XmlIO.SetSerializerOwner(wxT("wxJigsawEditor"));
	m_XmlIO.SetSerializerRootName(wxT("settings"));
	m_XmlIO.SetSerializerVersion(wxT("1.0.0"));

	XS_REGISTER_IO_HANDLER(wxT("jigsawshape"), xsJigsawShapePropIO);
	XS_REGISTER_IO_HANDLER(wxT("listjigsawshape"), xsListJigsawShapePropIO);
	XS_REGISTER_IO_HANDLER(wxT("jigsawinputparameter"), xsJigsawInputParameterPropIO);
	XS_REGISTER_IO_HANDLER(wxT("listjigsawinputparameter"), xsListJigsawInputParameterPropIO);

	XS_REGISTER_IO_HANDLER(wxT("colourdata"), xsColourDataPropIO);
	XS_REGISTER_IO_HANDLER(wxT("jigsawshapecategory"), xsJigsawShapeCategoryPropIO);
	XS_REGISTER_IO_HANDLER(wxT("listjigsawshapecategory"), xsListJigsawShapeCategoryPropIO);

	m_Config = new wxJigsawEditorConfig;
	m_XmlIO.SetRootItem(m_Config);
}

void wxJigsawEditorMainFrame::LoadConfig()
{
	wxString configFileName = wxT("config.xml");
	if(wxFileExists(configFileName))
	{
		m_XmlIO.DeserializeFromXml(configFileName);
	}
}

void wxJigsawEditorMainFrame::SaveConfig()
{
	wxString configFileName = wxT("config.xml");
	m_XmlIO.SerializeToXml(configFileName, true);
}

wxJigsawShape * wxJigsawEditorMainFrame::LoadShape(const wxString & shapeFileName)
{
	wxXmlSerializer serializer;
	wxJigsawShape * shape = new wxJigsawShape;

	serializer.SetSerializerOwner(wxT("wxJigsawShapeEngine"));
	serializer.SetSerializerRootName(wxT("wxJigsawShape"));
	serializer.SetSerializerVersion(wxT("1.0.0"));
	serializer.SetRootItem(shape);

	for(size_t i = 0; i < m_Config->GetShapesLookupDirectories().Count(); i++)
	{
		wxString fileName = m_Config->GetShapesLookupDirectories()[i] + 
			wxFILE_SEP_PATH + shapeFileName;
		if(!wxFileExists(fileName)) continue;
		serializer.DeserializeFromXml(fileName);
		if(shape->GetBitmapFileName() != "") shape->SetBitmapFileName(shape->GetBitmapFileName());
		return (wxJigsawShape*)shape->Clone();
	}
	serializer.RemoveAll();
	return NULL;
}

void wxJigsawEditorMainFrame::LoadPalettes()
{
	m_Palettes.DeleteContents(true);
	for(wxJigsawShapeCategoryList::Node * node = m_Config->GetCategories().GetFirst();
		node; node = node->GetNext())
	{
		wxJigsawShapeCategory * category = node->GetData();
		if(!category) continue;
		wxJigsawPalette * palette = new wxJigsawPalette;
		palette->SetPaletteName(category->GetCategoryName());
		palette->SetColours(category->GetColourData());
		for(size_t i = 0; i < category->GetShapeFileNames().Count(); i++)
		{
			wxJigsawShape * shape = LoadShape(category->GetShapeFileNames()[i]);
			if(!shape) continue;
			palette->GetShapes().Append(shape);
		}
		m_Palettes.Append(palette);
	}
}
