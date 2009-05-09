/////////////////////////////////////////////////////////////////////////////
// Name:        wxJigsawShapeDesignerPaletteDialog.cpp
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     08/06/2008 02:14:15
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

#include "wxJigsawShapeDesignerPaletteDialog.h"
#include <wxJigsawShapePropertyIO.h>
#include <wxJigsawInputParameterPropertyIO.h>

////@begin XPM images
////@end XPM images


/*!
 * wxJigsawShapeDesignerPaletteDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( wxJigsawShapeDesignerPaletteDialog, wxDialog )


/*!
 * wxJigsawShapeDesignerPaletteDialog event table definition
 */

BEGIN_EVENT_TABLE( wxJigsawShapeDesignerPaletteDialog, wxDialog )

////@begin wxJigsawShapeDesignerPaletteDialog event table entries
    EVT_BUTTON( ID_LOAD_PALETTE, wxJigsawShapeDesignerPaletteDialog::OnLOADPALETTEClick )

    EVT_BUTTON( ID_SAVE_PALETTE, wxJigsawShapeDesignerPaletteDialog::OnSAVEPALETTEClick )

    EVT_BUTTON( ID_ADD_SHAPE, wxJigsawShapeDesignerPaletteDialog::OnADDSHAPEClick )

    EVT_BUTTON( ID_DELETE_SHAPE, wxJigsawShapeDesignerPaletteDialog::OnDELETESHAPEClick )
    EVT_UPDATE_UI( ID_DELETE_SHAPE, wxJigsawShapeDesignerPaletteDialog::OnDELETESHAPEUpdate )

    EVT_BUTTON( ID_CLEAR_PALETTE, wxJigsawShapeDesignerPaletteDialog::OnCLEARPALETTEClick )
    EVT_UPDATE_UI( ID_CLEAR_PALETTE, wxJigsawShapeDesignerPaletteDialog::OnCLEARPALETTEUpdate )

////@end wxJigsawShapeDesignerPaletteDialog event table entries

END_EVENT_TABLE()


/*!
 * wxJigsawShapeDesignerPaletteDialog constructors
 */

wxJigsawShapeDesignerPaletteDialog::wxJigsawShapeDesignerPaletteDialog()
{
    Init();
}

wxJigsawShapeDesignerPaletteDialog::wxJigsawShapeDesignerPaletteDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * wxJigsawShapeDesignerPaletteDialog creator
 */

bool wxJigsawShapeDesignerPaletteDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin wxJigsawShapeDesignerPaletteDialog creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end wxJigsawShapeDesignerPaletteDialog creation
    return true;
}


/*!
 * wxJigsawShapeDesignerPaletteDialog destructor
 */

wxJigsawShapeDesignerPaletteDialog::~wxJigsawShapeDesignerPaletteDialog()
{
	m_PaletteXmlIO.RemoveAll();
	m_XmlIO.RemoveAll();
////@begin wxJigsawShapeDesignerPaletteDialog destruction
////@end wxJigsawShapeDesignerPaletteDialog destruction
}


/*!
 * Member initialisation
 */

void wxJigsawShapeDesignerPaletteDialog::Init()
{
////@begin wxJigsawShapeDesignerPaletteDialog member initialisation
    m_LoadedShape = new wxJigsawShape(wxEmptyString);
    m_Palette = new wxJigsawPalette;
    m_ShapeList = NULL;
////@end wxJigsawShapeDesignerPaletteDialog member initialisation
}


/*!
 * Control creation for wxJigsawShapeDesignerPaletteDialog
 */

void wxJigsawShapeDesignerPaletteDialog::CreateControls()
{    
////@begin wxJigsawShapeDesignerPaletteDialog content construction
    wxJigsawShapeDesignerPaletteDialog* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Palette"));
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxHORIZONTAL);
    itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxButton* itemButton4 = new wxButton( itemDialog1, ID_LOAD_PALETTE, _("Load"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer3->Add(itemButton4, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxButton* itemButton5 = new wxButton( itemDialog1, ID_SAVE_PALETTE, _("Save"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer3->Add(itemButton5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer6Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Shapes"));
    wxStaticBoxSizer* itemStaticBoxSizer6 = new wxStaticBoxSizer(itemStaticBoxSizer6Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5);

    m_ShapeList = new wxListView( itemDialog1, ID_SHAPES_LIST, wxDefaultPosition, wxSize(400, 150), wxLC_REPORT );
    itemStaticBoxSizer6->Add(m_ShapeList, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer6->Add(itemBoxSizer8, 0, wxGROW, 0);

    wxButton* itemButton9 = new wxButton( itemDialog1, ID_ADD_SHAPE, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer8->Add(itemButton9, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxButton* itemButton10 = new wxButton( itemDialog1, ID_DELETE_SHAPE, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer8->Add(itemButton10, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxButton* itemButton11 = new wxButton( itemDialog1, ID_CLEAR_PALETTE, _("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer8->Add(itemButton11, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer12 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer12, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton13 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer12->AddButton(itemButton13);

    itemStdDialogButtonSizer12->Realize();

////@end wxJigsawShapeDesignerPaletteDialog content construction
	FillShapeList();

	XS_REGISTER_IO_HANDLER(wxT("jigsawshape"), xsJigsawShapePropIO);
	XS_REGISTER_IO_HANDLER(wxT("listjigsawshape"), xsListJigsawShapePropIO);
	XS_REGISTER_IO_HANDLER(wxT("jigsawinputparameter"), xsJigsawInputParameterPropIO);
	XS_REGISTER_IO_HANDLER(wxT("listjigsawinputparameter"), xsListJigsawInputParameterPropIO);

	m_XmlIO.SetSerializerOwner(wxT("wxJigsawShapeEngine"));
	m_XmlIO.SetSerializerRootName(wxT("wxJigsawShape"));
	m_XmlIO.SetSerializerVersion(wxT("1.0.0"));
	m_XmlIO.SetRootItem(m_LoadedShape);

	m_PaletteXmlIO.SetSerializerOwner(wxT("wxJigsawShapeEngine"));
	m_PaletteXmlIO.SetSerializerRootName(wxT("wxJigsawPalette"));
	m_PaletteXmlIO.SetSerializerVersion(wxT("1.0.0"));
	m_PaletteXmlIO.SetRootItem(m_Palette);
}


/*!
 * Should we show tooltips?
 */

bool wxJigsawShapeDesignerPaletteDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap wxJigsawShapeDesignerPaletteDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin wxJigsawShapeDesignerPaletteDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end wxJigsawShapeDesignerPaletteDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon wxJigsawShapeDesignerPaletteDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin wxJigsawShapeDesignerPaletteDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end wxJigsawShapeDesignerPaletteDialog icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ADD_SHAPE
 */

void wxJigsawShapeDesignerPaletteDialog::OnADDSHAPEClick( wxCommandEvent& event )
{
	wxFileDialog dlg(this, wxFileSelectorPromptStr, wxEmptyString,
		wxEmptyString, _("Shapes (*.jigshape)|*.jigshape"), wxFD_OPEN);
	if(dlg.ShowModal() == wxID_OK)
	{
		m_XmlIO.DeserializeFromXml(dlg.GetPath());
		m_Palette->GetShapes().Append(new wxJigsawShape(*m_LoadedShape));
		m_ShapeList->InsertItem(m_ShapeList->GetItemCount(),
			wxString::Format(_("Shape '%s'"), m_LoadedShape->GetName()));
	}
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DELETE_SHAPE
 */

void wxJigsawShapeDesignerPaletteDialog::OnDELETESHAPEClick( wxCommandEvent& event )
{
	do
	{
		long selection = m_ShapeList->GetFirstSelected();
		if(selection < 0) break;
		if(wxMessageBox(_("Do you really want to delete this item?"), wxMessageBoxCaptionStr,
			wxYES_NO|wxCENTER) != wxYES) break;
		wxJigsawShapeList::Node * node = m_Palette->GetShapes().Item(static_cast<size_t>(selection));
		if(!node) break;
		m_Palette->GetShapes().DeleteNode(node);
		m_ShapeList->DeleteItem(selection);
	}
	while(false);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_DELETE_SHAPE
 */

void wxJigsawShapeDesignerPaletteDialog::OnDELETESHAPEUpdate( wxUpdateUIEvent& event )
{
	event.Enable(m_ShapeList->GetFirstSelected() >= 0);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_CLEAR_PALETTE
 */

void wxJigsawShapeDesignerPaletteDialog::OnCLEARPALETTEClick( wxCommandEvent& event )
{
	do
	{
		if(m_ShapeList->GetItemCount() == 0) break;
		if(wxMessageBox(_("Do you really want to delete all items?"), wxMessageBoxCaptionStr,
			wxYES_NO|wxCENTER) != wxYES) break;
		m_Palette->GetShapes().Clear();
		m_ShapeList->DeleteAllItems();
	}
	while(false);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CLEAR_PALETTE
 */

void wxJigsawShapeDesignerPaletteDialog::OnCLEARPALETTEUpdate( wxUpdateUIEvent& event )
{
	event.Enable(m_ShapeList->GetItemCount() > 0);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_LOAD_PALETTE
 */

void wxJigsawShapeDesignerPaletteDialog::OnLOADPALETTEClick( wxCommandEvent& event )
{
	wxFileDialog dlg(this, wxFileSelectorPromptStr, wxEmptyString,
		wxEmptyString, _("Palettes (*.jigpalette)|*.jigpalette"), wxFD_OPEN);
	if(dlg.ShowModal() == wxID_OK)
	{
		m_PaletteXmlIO.DeserializeFromXml(dlg.GetPath());
		FillShapeList();
	}
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SAVE_PALETTE
 */

void wxJigsawShapeDesignerPaletteDialog::OnSAVEPALETTEClick( wxCommandEvent& event )
{
	wxFileDialog dlg(this, wxFileSelectorPromptStr, wxEmptyString,
		wxEmptyString, _("Palettes (*.jigpalette)|*.jigpalette"), wxFD_SAVE);
	if(dlg.ShowModal() == wxID_OK)
	{
		m_PaletteXmlIO.SerializeToXml(dlg.GetPath(), xsWITH_ROOT);
	}
}

void wxJigsawShapeDesignerPaletteDialog::FillShapeList()
{
	if(!m_ShapeList->GetColumnCount())
	{
		m_ShapeList->InsertColumn(0, _("Info"), wxLIST_FORMAT_LEFT, 150);
	}
	m_ShapeList->DeleteAllItems();
	for(wxJigsawShapeList::Node * node = m_Palette->GetShapes().GetFirst();
		node; node = node->GetNext())
	{
		wxJigsawShape * shape = node->GetData();
		if(!shape) continue;
		m_ShapeList->InsertItem(m_ShapeList->GetItemCount(),
			wxString::Format(_("Shape '%s'"), shape->GetName()));
	}
}
