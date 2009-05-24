// Behavior.cpp: implementation of the Behavior class.
//
//////////////////////////////////////////////////////////////////////

#include "Behavior.h"
#include "wxJigsawEditor/DnDJigsawShapeDropTarget.h"

#include "wxJigsawEditor/wxJigsawEditorView.h"
#include "wxJigsawEditor/wxJigsawShapeCategory.h"
#include "wxJigsawEditor/wxJigsawEditorConfig.h"
#include "wxJigsawEditor/xsColourDataPropIO.h"
#include <wxJigsawShape.h>
#include <wxJigsawShapePropertyIO.h>
#include <wxJigsawInputParameterPropertyIO.h>


IMPLEMENT_CLASS(Behavior, wxPanel)

BEGIN_EVENT_TABLE(Behavior, wxPanel)
    EVT_MENU( wxID_EXIT, Behavior::OnEXITClick )
    EVT_UPDATE_UI( wxID_EXIT, Behavior::OnEXITUpdate )

    EVT_MENU( wxID_ABOUT, Behavior::OnABOUTClick )

    EVT_SLIDER( ID_SCALE_SLIDER, Behavior::OnSCALESLIDERUpdated )

    EVT_TEXT_ENTER( ID_SEARCH_TEXTCTRL, Behavior::OnSEARCHClicked )

    EVT_BUTTON( wxID_FIND, Behavior::OnSEARCHClicked )
END_EVENT_TABLE()


Behavior *Behavior::pBehavior = NULL;

Behavior::Behavior(wxWindow *parent)   : wxPanel(parent)
{
	Init();	
    CreateControls();
    Centre();

	pBehavior = this;
}

Behavior::~Behavior()
{
	wxDELETE(m_ShapeRegistry);
	pBehavior = NULL;	
}


void Behavior::Init()
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

////@begin Behavior member initialisation
    m_ScaleValue = 100;
    m_ScaleSlider = NULL;
    m_SearchTextCtrl = NULL;
    m_CategoryList = NULL;
    m_Palette = NULL;
    m_Canvas = NULL;
////@end Behavior member initialisation
}


/*!
 * Control creation for Behavior
 */

void Behavior::CreateControls()
{    
	SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
////@begin Behavior content construction
    Behavior* itemDocParentFrame1 = this;

    GetAuiManager().SetManagedWindow(this);

    /*wxMenuBar* menuBar = new wxMenuBar;
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
    itemDocParentFrame1->SetMenuBar(menuBar);*/

    /*wxStatusBar* itemStatusBar13 = new wxStatusBar( itemDocParentFrame1, ID_JIGSAW_EDITOR_MAIN_STATUSBAR, wxST_SIZEGRIP|wxNO_BORDER );
    itemStatusBar13->SetFieldsCount(2);
    itemDocParentFrame1->SetStatusBar(itemStatusBar13);*/

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
////@end Behavior content construction
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
		wxCommandEventHandler(Behavior::OnCategorySelected));
}

void Behavior::OnCategorySelected(wxCommandEvent & event)
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

void Behavior::OnEXITClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT in Behavior.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT in Behavior. 
}


/*!
 * wxEVT_UPDATE_UI event handler for wxID_EXIT
 */

void Behavior::OnEXITUpdate( wxUpdateUIEvent& event )
{
////@begin wxEVT_UPDATE_UI event handler for wxID_EXIT in Behavior.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_UPDATE_UI event handler for wxID_EXIT in Behavior. 
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_ABOUT
 */

void Behavior::OnABOUTClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_MENU_SELECTED event handler for wxID_ABOUT in Behavior.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_MENU_SELECTED event handler for wxID_ABOUT in Behavior. 
}


/*!
 * Should we show tooltips?
 */

bool Behavior::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap Behavior::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin Behavior bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end Behavior bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon Behavior::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin Behavior icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end Behavior icon retrieval
}


/*!
 * wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SCALE_SLIDER
 */

void Behavior::OnSCALESLIDERUpdated( wxCommandEvent& event )
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

void Behavior::OnSEARCHClicked( wxCommandEvent& event )
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

bool Behavior::LoadShapeRegistry()
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

void Behavior::InitConfigSerialization()
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

	wxJigsawShapeCategory * category(NULL);
	category = new wxJigsawShapeCategory;
	category->SetCategoryName(_("First"));
	category->GetShapeFileNames().Add(wxT("test1.jigshape"));
	category->GetShapeFileNames().Add(wxT("test2.jigshape"));
	category->GetShapeFileNames().Add(wxT("test3.jigshape"));
	m_Config->GetCategories().Append(category);
	category = new wxJigsawShapeCategory;
	category->SetCategoryName(_("Second"));
	category->GetShapeFileNames().Add(wxT("test4.jigshape"));
	category->GetShapeFileNames().Add(wxT("test5.jigshape"));
	m_Config->GetCategories().Append(category);
}

void Behavior::LoadConfig()
{
	wxString configFileName = wxT("config.xml");
	if(wxFileExists(configFileName))
	{
		m_XmlIO.DeserializeFromXml(configFileName);
	}
}

void Behavior::SaveConfig()
{
	wxString configFileName = wxT("config.xml");
	m_XmlIO.SerializeToXml(configFileName, true);
}

wxJigsawShape * Behavior::LoadShape(const wxString & shapeFileName)
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
		return (wxJigsawShape*)shape->Clone();
	}
	serializer.RemoveAll();
	return NULL;
}

void Behavior::LoadPalettes()
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








