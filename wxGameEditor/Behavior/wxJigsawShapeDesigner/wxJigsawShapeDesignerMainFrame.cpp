/////////////////////////////////////////////////////////////////////////////
// Name:        wxJigsawShapeDesignerMainFrame.cpp
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     02/03/2008 18:53:58
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
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/numdlg.h>

#include "wxJigsawShapeDesignerMainFrame.h"
#include "wxPropGridAdvancedProperties.h"
#include "wxIndexTreeItemData.h"
#include "AddParameterDialog.h"
#include "wxJigsawShapeDesignerPaletteDialog.h"

////@begin XPM images
#include "add.xpm"
#include "del.xpm"
////@end XPM images
#include "param.xpm"
#include "shape.xpm"


/*!
 * wxJigsawShapeDesignerMainFrame type definition
 */

IMPLEMENT_CLASS( wxJigsawShapeDesignerMainFrame, wxDocParentFrame )


/*!
 * wxJigsawShapeDesignerMainFrame event table definition
 */

BEGIN_EVENT_TABLE( wxJigsawShapeDesignerMainFrame, wxDocParentFrame )

////@begin wxJigsawShapeDesignerMainFrame event table entries
    EVT_MENU( ID_PALETTE_EDITOR, wxJigsawShapeDesignerMainFrame::OnPALETTEEDITORClick )

    EVT_MENU( wxID_EXIT, wxJigsawShapeDesignerMainFrame::OnExitClick )

    EVT_TREE_SEL_CHANGED( ID_SHAPE_STRUCTURE_TREE, wxJigsawShapeDesignerMainFrame::OnSHAPESTRUCTURETREESelChanged )

    EVT_PG_CHANGED(ID_PROPERTYGRID, wxJigsawShapeDesignerMainFrame::OnPROPERTYGRIDPropertyChanged)

    EVT_MENU( ID_ADD_PARAMETER, wxJigsawShapeDesignerMainFrame::OnADDPARAMETERClick )
    EVT_UPDATE_UI( ID_ADD_PARAMETER, wxJigsawShapeDesignerMainFrame::OnADDPARAMETERUpdate )

    EVT_MENU( ID_DEL_PARAMETER, wxJigsawShapeDesignerMainFrame::OnDELPARAMETERClick )
    EVT_UPDATE_UI( ID_DEL_PARAMETER, wxJigsawShapeDesignerMainFrame::OnDELPARAMETERUpdate )

////@end wxJigsawShapeDesignerMainFrame event table entries

END_EVENT_TABLE()


/*!
 * wxJigsawShapeDesignerMainFrame constructors
 */

wxJigsawShapeDesignerMainFrame::wxJigsawShapeDesignerMainFrame( wxDocManager *manager, wxFrame *parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
    : wxDocParentFrame( manager, parent, id, caption, pos, size, style )
{
    Init();
    Create( manager, parent, id, caption, pos, size, style );
}


/*!
 * wxJigsawShapeDesignerMainFrame creator
 */

bool wxJigsawShapeDesignerMainFrame::Create( wxDocManager *, wxFrame *parent, wxWindowID, const wxString&, const wxPoint&, const wxSize&, long)
{
////@begin wxJigsawShapeDesignerMainFrame creation
    SetParent(parent);
    CreateControls();
    Centre();
////@end wxJigsawShapeDesignerMainFrame creation
    return true;
}


/*!
 * wxJigsawShapeDesignerMainFrame destructor
 */

wxJigsawShapeDesignerMainFrame::~wxJigsawShapeDesignerMainFrame()
{
////@begin wxJigsawShapeDesignerMainFrame destruction
    GetAuiManager().UnInit();
////@end wxJigsawShapeDesignerMainFrame destruction
}


/*!
 * Member initialisation
 */

void wxJigsawShapeDesignerMainFrame::Init()
{
////@begin wxJigsawShapeDesignerMainFrame member initialisation
    m_DocumentTree = NULL;
    m_PropertyGrid = NULL;
    m_Canvas = NULL;
////@end wxJigsawShapeDesignerMainFrame member initialisation
}


/*!
 * Control creation for wxJigsawShapeDesignerMainFrame
 */

void wxJigsawShapeDesignerMainFrame::CreateControls()
{    
////@begin wxJigsawShapeDesignerMainFrame content construction
    wxJigsawShapeDesignerMainFrame* itemDocParentFrame1 = this;

    GetAuiManager().SetManagedWindow(this);

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    itemMenu3->Append(wxID_NEW, _("New\tCtrl+N"), _T(""), wxITEM_NORMAL);
    itemMenu3->Append(wxID_OPEN, _("Open\tCtrl+O"), _T(""), wxITEM_NORMAL);
    itemMenu3->Append(wxID_SAVE, _("Save\tCtrl+S"), _T(""), wxITEM_NORMAL);
    itemMenu3->Append(wxID_SAVEAS, _("Save as..."), _T(""), wxITEM_NORMAL);
    itemMenu3->AppendSeparator();
    itemMenu3->Append(ID_PALETTE_EDITOR, _("Palette editor"), _T(""), wxITEM_NORMAL);
    itemMenu3->AppendSeparator();
    itemMenu3->Append(wxID_EXIT, _("Exit\tAlt+F4"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu3, _("File"));
    itemDocParentFrame1->SetMenuBar(menuBar);

    m_DocumentTree = new wxTreeCtrl( itemDocParentFrame1, ID_SHAPE_STRUCTURE_TREE, wxDefaultPosition, wxSize(250, 100), wxTR_HAS_BUTTONS |wxTR_LINES_AT_ROOT|wxTR_SINGLE );
    itemDocParentFrame1->GetAuiManager().AddPane(m_DocumentTree, wxAuiPaneInfo()
        .Name(_T("ShapeStructureTree")).Caption(_T("Shape structure")).MinSize(wxSize(100, -1)).BestSize(wxSize(250, -1)).CloseButton(false).DestroyOnClose(false).Resizable(true).Floatable(false));

    wxStatusBar* itemStatusBar13 = new wxStatusBar( itemDocParentFrame1, ID_MAIN_STATUSBAR, wxST_SIZEGRIP|wxNO_BORDER );
    itemStatusBar13->SetFieldsCount(2);
    itemDocParentFrame1->SetStatusBar(itemStatusBar13);

    m_PropertyGrid = new wxPropertyGrid( itemDocParentFrame1, ID_PROPERTYGRID, wxDefaultPosition, wxSize(250, 100), wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL|wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER|wxPG_AUTO_SORT );
    itemDocParentFrame1->GetAuiManager().AddPane(m_PropertyGrid, wxAuiPaneInfo()
        .Name(_T("PropertyGrid")).Caption(_T("Properties")).MinSize(wxSize(150, 50)).BestSize(wxSize(250, 300)).CloseButton(false).DestroyOnClose(false).Resizable(true).Floatable(false));
    m_PropertyGrid->SetScrollbars(1, 1, 0, 0);

    m_Canvas = new wxJigsawShapeDesignerCanvas( itemDocParentFrame1, ID_CANVAS, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
    m_Canvas->SetBackgroundColour(wxColour(128, 128, 128));
    itemDocParentFrame1->GetAuiManager().AddPane(m_Canvas, wxAuiPaneInfo()
        .Name(_T("Canvas")).Centre().CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(true).Floatable(false));

    wxToolBar* itemToolBar16 = new wxToolBar( itemDocParentFrame1, ID_TOOLBAR1, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_HORIZONTAL|wxTB_NODIVIDER );
    itemToolBar16->SetToolBitmapSize(wxSize(24, 24));
    wxBitmap itemtool17Bitmap(itemDocParentFrame1->GetBitmapResource(wxT("add.xpm")));
    wxBitmap itemtool17BitmapDisabled;
    itemToolBar16->AddTool(ID_ADD_PARAMETER, _("Add parameter"), itemtool17Bitmap, itemtool17BitmapDisabled, wxITEM_NORMAL, _("Add new parameter to shape"), _("Add new parameter to shape"));
    wxBitmap itemtool18Bitmap(itemDocParentFrame1->GetBitmapResource(wxT("del.xpm")));
    wxBitmap itemtool18BitmapDisabled;
    itemToolBar16->AddTool(ID_DEL_PARAMETER, _("Delete parameter"), itemtool18Bitmap, itemtool18BitmapDisabled, wxITEM_NORMAL, _("Delete parameter from shape"), _("Delete parameter from shape"));
    itemToolBar16->Realize();
    itemDocParentFrame1->GetAuiManager().AddPane(itemToolBar16, wxAuiPaneInfo()
        .Name(_T("Pane1")).Top().LeftDockable(false).RightDockable(false).BottomDockable(false).CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(true).Floatable(false).Movable(false));

    GetAuiManager().Update();

////@end wxJigsawShapeDesignerMainFrame content construction
	m_ParamTreeImageList.Create(16, 16);
	m_ParamTreeImageList.Add(wxBitmap(param_xpm));
	m_ParamTreeImageList.Add(wxBitmap(shape_xpm));
	m_DocumentTree->SetImageList(&m_ParamTreeImageList);
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT
 */

void wxJigsawShapeDesignerMainFrame::OnExitClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT in wxJigsawShapeDesignerMainFrame.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT in wxJigsawShapeDesignerMainFrame. 
}


/*!
 * Should we show tooltips?
 */

bool wxJigsawShapeDesignerMainFrame::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap wxJigsawShapeDesignerMainFrame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin wxJigsawShapeDesignerMainFrame bitmap retrieval
    wxUnusedVar(name);
    if (name == _T("add.xpm"))
    {
        wxBitmap bitmap(add_xpm);
        return bitmap;
    }
    else if (name == _T("del.xpm"))
    {
        wxBitmap bitmap(del_xpm);
        return bitmap;
    }
    return wxNullBitmap;
////@end wxJigsawShapeDesignerMainFrame bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon wxJigsawShapeDesignerMainFrame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin wxJigsawShapeDesignerMainFrame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end wxJigsawShapeDesignerMainFrame icon retrieval
}

void wxJigsawShapeDesignerMainFrame::FillDocumentTree()
{
	m_DocumentTree->DeleteAllItems();
	wxJigsawShape * shape = m_Canvas->GetShape();
	wxString label = wxString::Format(_("Shape: %s"),
		shape != NULL ? shape->GetName() : _("<none>"));
	wxTreeItemId root = m_DocumentTree->AddRoot(label, 1, 1);
	if(shape)
	{
		//m_InputParametersNode = m_DocumentTree->AppendItem(root, _("Input parameters"));
		int index(0);
		for(wxJigsawInputParameters::Node * node = shape->GetInputParameters().GetFirst(); 
			node; node = node->GetNext(), index++)
		{
			wxJigsawInputParameter * param = node->GetData();
			wxTreeItemId paramNode = m_DocumentTree->AppendItem(root,
				wxString::Format(_("Parameter: %s"), JigsawShapeStyleToStr(param->GetStyle())),
				0, 0, new wxIndexTreeItemData(index));
		}
	}
	m_DocumentTree->ExpandAll();
}

void wxJigsawShapeDesignerMainFrame::FixDocumentTreeRoot()
{
	wxTreeItemId id = m_DocumentTree->GetRootItem();
	if(id.IsOk())
	{
		wxJigsawShape * shape = m_Canvas->GetShape();
		m_DocumentTree->SetItemText(id, wxString::Format(_("Shape: %s"),
			shape != NULL ? shape->GetName() : _("<none>")));
	}
}

void wxJigsawShapeDesignerMainFrame::FillPropertyGrid()
{
	do
	{
		wxTreeItemId selectedNode = m_DocumentTree->GetSelection();
		if(!selectedNode.IsOk()) break;
		if(selectedNode == m_DocumentTree->GetRootItem())
		{
			FillPropertyGridByShape();
		}
		else if(m_DocumentTree->GetItemParent(selectedNode) == m_InputParametersNode)
		{
			// TODO: Add TreeItemData and determine an index of selected parameter
			FillPropertyGridByParameter(wxID_ANY);
		}
	}
	while(false);
	m_PropertyGrid->Refresh();
}

void wxJigsawShapeDesignerMainFrame::FillPropertyGridByShape()
{
	do
	{
		wxJigsawShape * shape = m_Canvas->GetShape();
		if(!shape) break;
		wxPGProperty * prop(NULL);
		m_PropertyGrid->Clear();
		m_PropertyGrid->Append(new wxPropertyCategory(_("General")));
		m_PropertyGrid->Append(new wxStringProperty(_("Name"), wxPG_LABEL, shape->GetName()));
		m_PropertyGrid->Append(new wxImageFileProperty(_("Bitmap"), wxPG_LABEL, shape->GetBitmapFileName()));
		m_PropertyGrid->Append(new wxSystemColourProperty(_("Colour"), wxT("Colour"), shape->GetColour()));
		m_PropertyGrid->Append(new wxSizeProperty(_("Minimal size"), wxT("MinSize"), shape->GetMinSize()));
		m_PropertyGrid->Append(new wxBoolProperty(_("Has C-Shape"), wxT("HasCShape"), shape->GetHasCShape()));
		prop = m_PropertyGrid->Append(new wxBoolProperty(_("Has Bump"), wxT("HasBump"), shape->GetHasBump()));
		m_PropertyGrid->SetPropertyReadOnly(prop->GetId(), !shape->GetHasCShape());
		prop = m_PropertyGrid->Append(new wxBoolProperty(_("Has Notch"), wxT("HasNotch"), shape->GetHasNotch()));
		m_PropertyGrid->SetPropertyReadOnly(prop->GetId(), !shape->GetHasCShape());
		wxArrayString shapeTypeNames;
		shapeTypeNames.Add(_("None"));
		shapeTypeNames.Add(_("Numeric"));
		shapeTypeNames.Add(_("Boolean"));
		wxArrayInt ids;
		ids.Add(wxJigsawShapeStyle::wxJS_TYPE_NONE);
		ids.Add(wxJigsawShapeStyle::wxJS_TYPE_NUMERIC);
		ids.Add(wxJigsawShapeStyle::wxJS_TYPE_BOOLEAN);
		prop = m_PropertyGrid->Append(new wxEnumProperty(_("Style"), wxT("Style"), shapeTypeNames));
		prop->SetChoices(shapeTypeNames, ids);
	}
	while(false);
}

void wxJigsawShapeDesignerMainFrame::FillPropertyGridByParameter(int)
{
	do
	{
		wxJigsawShape * shape = m_Canvas->GetShape();
		if(!shape) break;
		m_PropertyGrid->Clear();
		m_PropertyGrid->Append(new wxPropertyCategory(_("Parameter")));
		m_PropertyGrid->Append(new wxStringProperty(_("Label"), wxPG_LABEL, shape->GetName()));
	}
	while(false);
}


/*!
 * wxEVT_COMMAND_TREE_SEL_CHANGED event handler for ID_SHAPE_STRUCTURE_TREE
 */

void wxJigsawShapeDesignerMainFrame::OnSHAPESTRUCTURETREESelChanged( wxTreeEvent& )
{
	FillPropertyGrid();
}


/*!
 * wxEVT_PG_CHANGED event handler for ID_PROPERTYGRID
 */

void wxJigsawShapeDesignerMainFrame::OnPROPERTYGRIDPropertyChanged( wxPropertyGridEvent& event )
{
	wxPGProperty * prop = event.GetProperty();
	do
	{
		wxJigsawShape * shape = m_Canvas->GetShape();
		if(!shape) break;
		if(prop->GetName() == wxT("HasCShape"))
		{
			shape->SetHasCShape(prop->GetValue());
		}
		else if(prop->GetName() == wxT("Name"))
		{
			shape->SetName(prop->GetValueAsString());
			wxString label = wxString::Format(_("Shape: %s"),
				shape != NULL ? shape->GetName() : _("<none>"));
			FixDocumentTreeRoot();
		}
		else if(prop->GetName() == wxT("Colour"))
		{
			wxColourPropertyValue * cpv = wxGetVariantCast(event.GetPropertyValue(), wxColourPropertyValue);
			if(cpv)
			{
				shape->SetColour(cpv->m_colour);
			}			
		}
		else if(prop->GetName() == wxT("Bitmap"))
		{
			shape->SetBitmapFileName(prop->GetValueAsString());
		}
		else if(prop->GetName() == wxT("HasBump"))
		{
			shape->SetHasBump(prop->GetValue());
		}
		else if(prop->GetName() == wxT("HasNotch"))
		{
			shape->SetHasNotch(prop->GetValue());
		}
		else if(prop->GetName() == wxT("MinSize"))
		{
			shape->SetMinSize(m_PropertyGrid->GetPropertyValueAsSize(prop->GetId()));
		}
		else if(prop->GetName() == wxT("Style"))
		{
			bool hasConnectors = (event.GetPropertyValueAsString() == _("None"));
			shape->SetStyle(IntToJigsawShapeStyle(event.GetPropertyValueAsInt()));
			if(!hasConnectors)
			{
				shape->SetHasCShape(false);
				shape->SetHasBump(false);
				shape->SetHasNotch(false);
			}
			m_PropertyGrid->EnableProperty(wxT("HasBump"), hasConnectors);
			m_PropertyGrid->EnableProperty(wxT("HasNotch"), hasConnectors);
			m_PropertyGrid->EnableProperty(wxT("HasCShape"), hasConnectors);
			m_PropertyGrid->SetPropertyValue(wxT("HasBump"), shape->GetHasBump());
			m_PropertyGrid->SetPropertyValue(wxT("HasNotch"), shape->GetHasNotch());
			m_PropertyGrid->SetPropertyValue(wxT("HasCShape"), shape->GetHasCShape());
		}
		m_PropertyGrid->Refresh();
		m_Canvas->Refresh();
		m_Canvas->GetDocument()->Modify(true);
	}
	while(false);
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_ADD_PARAMETER
 */

void wxJigsawShapeDesignerMainFrame::OnADDPARAMETERClick( wxCommandEvent& )
{
	do
	{
		wxJigsawShape * shape = m_Canvas->GetShape();
		if(!shape) break;
		AddParameterDialog * dlg = new AddParameterDialog(this);
		if(dlg->ShowModal() == wxID_OK)
		{
			int paramType = dlg->GetParamType();
			wxString paramLabel = dlg->GetParamLabel();
			//wxMessageBox(paramType + wxT(" - ") + paramLabel);
			shape->GetInputParameters().Append(
				new wxJigsawInputParameter(paramLabel, IntToJigsawShapeStyle(paramType), NULL));
			wxTreeItemId root = m_DocumentTree->GetRootItem();
			wxTreeItemId id = m_DocumentTree->AppendItem(root,
				wxString::Format(_("Parameter: %s"), 
				JigsawShapeStyleToStr(IntToJigsawShapeStyle(paramType)).GetData()), 0, 0);
			m_DocumentTree->Expand(root);
			m_Canvas->Refresh();
			m_Canvas->GetDocument()->Modify(true);
		}
		dlg->Destroy();
	}
	while(false);
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_DEL_PARAMETER
 */

void wxJigsawShapeDesignerMainFrame::OnDELPARAMETERClick( wxCommandEvent& )
{
	do
	{
		wxJigsawShape * shape = m_Canvas->GetShape();
		if(!shape) break;
		long index = wxGetNumberFromUser(_("Delete input parameter"), 
			_("Select the parameter which you want to delete"),
			wxEmptyString, 1, 1, shape->GetInputParameters().GetCount());
		if(index <= 0) break;
		index--;
		shape->GetInputParameters().DeleteNode(
			shape->GetInputParameters().Item(static_cast<size_t>(index)));
		FillDocumentTree();
		m_Canvas->Refresh();
	}
	while(false);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_DEL_PARAMETER
 */

void wxJigsawShapeDesignerMainFrame::OnDELPARAMETERUpdate( wxUpdateUIEvent& event )
{
	event.Enable(m_Canvas->GetView() != NULL);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_ADD_PARAMETER
 */

void wxJigsawShapeDesignerMainFrame::OnADDPARAMETERUpdate( wxUpdateUIEvent& event )
{
	event.Enable(m_Canvas->GetView() != NULL);
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_PALETTE_EDITOR
 */

void wxJigsawShapeDesignerMainFrame::OnPALETTEEDITORClick( wxCommandEvent& event )
{
	wxJigsawShapeDesignerPaletteDialog * dlg = new wxJigsawShapeDesignerPaletteDialog(this);
	dlg->ShowModal();
	dlg->Destroy();
}

