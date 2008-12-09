///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 26 2007)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx_pch.h"

#ifdef _DEBUG_MSVC
#define new DEBUG_NEW
#endif

#include "GUI.h"

///////////////////////////////////////////////////////////////////////////

_MainFrame::_MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxFlexGridSizer* mainSizer;
	mainSizer = new wxFlexGridSizer( 2, 1, 0, 0 );
	mainSizer->AddGrowableCol( 0 );
	mainSizer->AddGrowableRow( 0 );
	mainSizer->SetFlexibleDirection( wxBOTH );
	mainSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_pTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 640,400 ), wxTE_MULTILINE|wxTE_READONLY );
	mainSizer->Add( m_pTextCtrl, 1, wxEXPAND, 5 );

	m_sdbSizer = new wxStdDialogButtonSizer();
	m_sdbSizerOK = new wxButton( this, wxID_OK );
	m_sdbSizer->AddButton( m_sdbSizerOK );
	m_sdbSizer->Realize();
	mainSizer->Add( m_sdbSizer, 1, wxEXPAND|wxALL, 5 );

	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );

	// Connect Events
	m_sdbSizerOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( _MainFrame::OnOk ), NULL, this );
}
