/////////////////////////////////////////////////////////////////////////////
// Name:        AddParameterDialog.cpp
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     26/03/2008 15:05:20
// RCS-ID:      
// Copyright:   Volodymir (T-Rex) Tryapichko, 2008
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

#include "AddParameterDialog.h"

////@begin XPM images
////@end XPM images


/*!
 * AddParameterDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( AddParameterDialog, wxDialog )


/*!
 * AddParameterDialog event table definition
 */

BEGIN_EVENT_TABLE( AddParameterDialog, wxDialog )

////@begin AddParameterDialog event table entries
    EVT_UPDATE_UI( wxID_OK, AddParameterDialog::OnOKUpdate )

////@end AddParameterDialog event table entries

END_EVENT_TABLE()


/*!
 * AddParameterDialog constructors
 */

AddParameterDialog::AddParameterDialog()
{
    Init();
}

AddParameterDialog::AddParameterDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * AddParameterDialog creator
 */

bool AddParameterDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin AddParameterDialog creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end AddParameterDialog creation
    return true;
}


/*!
 * AddParameterDialog destructor
 */

AddParameterDialog::~AddParameterDialog()
{
////@begin AddParameterDialog destruction
////@end AddParameterDialog destruction
}


/*!
 * Member initialisation
 */

void AddParameterDialog::Init()
{
////@begin AddParameterDialog member initialisation
    m_ParamType = 0;
    m_ParamTypeChoice = NULL;
////@end AddParameterDialog member initialisation
}


/*!
 * Control creation for AddParameterDialog
 */

void AddParameterDialog::CreateControls()
{    
////@begin AddParameterDialog content construction
    AddParameterDialog* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxFlexGridSizer* itemFlexGridSizer3 = new wxFlexGridSizer(2, 2, 0, 0);
    itemFlexGridSizer3->AddGrowableCol(1);
    itemBoxSizer2->Add(itemFlexGridSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemStaticText4, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_ParamTypeChoiceStrings;
    m_ParamTypeChoiceStrings.Add(_("None"));
    m_ParamTypeChoiceStrings.Add(_("Numeric"));
    m_ParamTypeChoiceStrings.Add(_("Boolean"));
    m_ParamTypeChoice = new wxChoice( itemDialog1, ID_PARAM_TYPE_CHOICE, wxDefaultPosition, wxDefaultSize, m_ParamTypeChoiceStrings, 0 );
    itemFlexGridSizer3->Add(m_ParamTypeChoice, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("Label:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemStaticText6, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl7 = new wxTextCtrl( itemDialog1, ID_PARAM_LABEL_TEXTCTRL, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    itemFlexGridSizer3->Add(itemTextCtrl7, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer8 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton9 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer8->AddButton(itemButton9);

    wxButton* itemButton10 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer8->AddButton(itemButton10);

    itemStdDialogButtonSizer8->Realize();

    // Set validators
    m_ParamTypeChoice->SetValidator( wxGenericValidator(& m_ParamType) );
    itemTextCtrl7->SetValidator( wxGenericValidator(& m_ParamLabel) );
////@end AddParameterDialog content construction
    TransferDataToWindow();
}


/*!
 * Should we show tooltips?
 */

bool AddParameterDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap AddParameterDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin AddParameterDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end AddParameterDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon AddParameterDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin AddParameterDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end AddParameterDialog icon retrieval
}


/*!
 * wxEVT_UPDATE_UI event handler for wxID_OK
 */

void AddParameterDialog::OnOKUpdate( wxUpdateUIEvent& event )
{
    event.Enable(m_ParamTypeChoice->GetSelection() != 0);
}

