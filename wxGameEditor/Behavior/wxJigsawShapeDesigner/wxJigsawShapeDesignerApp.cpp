/////////////////////////////////////////////////////////////////////////////
// Name:        wxJigsawShapeDesignerApp.cpp
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     02/03/2008 18:44:02
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

#include "wxJigsawShapeDesignerApp.h"
#include "wxJigsawShapeDesignerDocument.h"
#include "wxJigsawShapeDesignerView.h"

////@begin XPM images
////@end XPM images


/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( wxJigsawShapeDesignerApp )
////@end implement app


/*!
 * wxJigsawShapeDesignerApp type definition
 */

IMPLEMENT_CLASS( wxJigsawShapeDesignerApp, wxApp )


/*!
 * wxJigsawShapeDesignerApp event table definition
 */

BEGIN_EVENT_TABLE( wxJigsawShapeDesignerApp, wxApp )

////@begin wxJigsawShapeDesignerApp event table entries
////@end wxJigsawShapeDesignerApp event table entries

END_EVENT_TABLE()


/*!
 * Constructor for wxJigsawShapeDesignerApp
 */

wxJigsawShapeDesignerApp::wxJigsawShapeDesignerApp()
{
    Init();
}


/*!
 * Member initialisation
 */

void wxJigsawShapeDesignerApp::Init()
{
////@begin wxJigsawShapeDesignerApp member initialisation
	m_DocManager = new wxDocManager;
////@end wxJigsawShapeDesignerApp member initialisation
	wxDocTemplate * docTemplate = new wxDocTemplate(m_DocManager, _("Jigsaw Shape"),
		wxT("*.jigshape;*.xml"), wxEmptyString, wxT("jigshape"), 
		wxT("wxJigsawShapeDesignerDoc"), wxT("wxJigsawShapeDesignerView"),
		CLASSINFO(wxJigsawShapeDesignerDocument), CLASSINFO(wxJigsawShapeDesignerView));
	wxUnusedVar(docTemplate);
	m_DocManager->SetMaxDocsOpen(1);
}

/*!
 * Initialisation for wxJigsawShapeDesignerApp
 */

bool wxJigsawShapeDesignerApp::OnInit()
{    
#if wxUSE_XPM
	wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
	wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
	wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
	wxImage::AddHandler(new wxGIFHandler);
#endif
	wxJigsawShapeDesignerMainFrame* mainWindow = 
		new wxJigsawShapeDesignerMainFrame( GetDocManager(), NULL, wxJigsawShapeDesignerMainFrame::ID_WXJIGSAWSHAPEDESIGNERMAINFRAME );
	mainWindow->Show(true);

    return true;
}


/*!
 * Cleanup for wxJigsawShapeDesignerApp
 */

int wxJigsawShapeDesignerApp::OnExit()
{    
	wxDELETE(m_DocManager);
////@begin wxJigsawShapeDesignerApp cleanup
	return wxApp::OnExit();
////@end wxJigsawShapeDesignerApp cleanup
}

bool wxJigsawShapeDesignerApp::LoadShapeRegistry(const wxString & filename)
{
	//TODO: Add loading of shape registry
	wxMessageBox(_("Not implemented"));
	return false;
}

bool wxJigsawShapeDesignerApp::SaveShapeRegistry(const wxString & filename)
{
	//TODO: Add saving of shape registry
	wxMessageBox(_("Not implemented"));
	return false;
}
