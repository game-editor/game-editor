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
	this->Centre( wxBOTH );

	wxFlexGridSizer* mainSizer;
	mainSizer = new wxFlexGridSizer( 2, 1, 0, 0 );
	mainSizer->AddGrowableCol( 0 );
	mainSizer->AddGrowableRow( 0 );
	mainSizer->SetFlexibleDirection( wxBOTH );
	mainSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* treeSizer;
	treeSizer = new wxFlexGridSizer( 2, 1, 0, 0 );
	treeSizer->AddGrowableCol( 0 );
	treeSizer->AddGrowableRow( 1 );
	treeSizer->SetFlexibleDirection( wxBOTH );
	treeSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxBoxSizer* treeBtnSizer;
	treeBtnSizer = new wxBoxSizer( wxHORIZONTAL );

	btnAddNode = new wxButton( this, IDC_TREE_ADD, wxT("Add item"), wxDefaultPosition, wxDefaultSize, 0 );
	treeBtnSizer->Add( btnAddNode, 0, wxALL, 5 );

	btnRemoveNode = new wxButton( this, IDC_TREE_REMOVE, wxT("Remove item"), wxDefaultPosition, wxDefaultSize, 0 );
	treeBtnSizer->Add( btnRemoveNode, 0, wxALL, 5 );

	btnAbout = new wxButton( this, wxID_ABOUT, wxT("About sample..."), wxDefaultPosition, wxDefaultSize, 0 );
	treeBtnSizer->Add( btnAbout, 0, wxALL, 5 );

	treeSizer->Add( treeBtnSizer, 1, wxEXPAND, 5 );

	treeCtrl = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( 400,300 ), wxTR_DEFAULT_STYLE );
	treeSizer->Add( treeCtrl, 1, wxEXPAND, 5 );

	mainSizer->Add( treeSizer, 1, wxEXPAND, 5 );

	m_sdbSizer = new wxStdDialogButtonSizer();
	m_sdbSizerOK = new wxButton( this, wxID_OK );
	m_sdbSizer->AddButton( m_sdbSizerOK );
	m_sdbSizer->Realize();
	mainSizer->Add( m_sdbSizer, 1, wxEXPAND|wxALL, 5 );

	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );

	// Connect Events
	btnAddNode->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( _MainFrame::OnAddClick ), NULL, this );
	btnAddNode->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( _MainFrame::OnUpdateAdd ), NULL, this );
	btnRemoveNode->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( _MainFrame::OnRemoveClick ), NULL, this );
	btnRemoveNode->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( _MainFrame::OnUpdateRemove ), NULL, this );
	btnAbout->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( _MainFrame::OnAboutClick ), NULL, this );
	m_sdbSizerOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( _MainFrame::OnOk ), NULL, this );
}
