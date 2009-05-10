/////////////////////////////////////////////////////////////////////////////
// Name:        wxJigsawEditorApp.cpp
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     02/03/2008 19:32:23
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

#include "wxJigsawEditorApp.h"
#include "wxJigsawEditorDocument.h"
#include "wxJigsawEditorView.h"
#include "wxJigsawShapeCategory.h"
#include "wxJigsawEditorConfig.h"
#include "xsColourDataPropIO.h"
#include <wxJigsawShape.h>
#include <wxJigsawShapePropertyIO.h>
#include <wxJigsawInputParameterPropertyIO.h>

////@begin XPM images
////@end XPM images


/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( wxJigsawEditorApp )
////@end implement app


/*!
 * wxJigsawEditorApp type definition
 */

IMPLEMENT_CLASS( wxJigsawEditorApp, wxApp )


/*!
 * wxJigsawEditorApp event table definition
 */

BEGIN_EVENT_TABLE( wxJigsawEditorApp, wxApp )

////@begin wxJigsawEditorApp event table entries
////@end wxJigsawEditorApp event table entries

END_EVENT_TABLE()


/*!
 * Constructor for wxJigsawEditorApp
 */

wxJigsawEditorApp::wxJigsawEditorApp()
{
    Init();
}


/*!
 * Member initialisation
 */

void wxJigsawEditorApp::Init()
{
////@begin wxJigsawEditorApp member initialisation
	m_DocManager = new wxDocManager;
	
////@end wxJigsawEditorApp member initialisation
	wxDocTemplate * docTemplate = new wxDocTemplate(m_DocManager, _("Jigsaw Scene"),
		wxT("*.jigscene;*.xml"), wxEmptyString, wxT("jigscene"), 
		wxT("wxJigsawEditorDoc"), wxT("wxJigsawEditorView"),
		CLASSINFO(wxJigsawEditorDocument), CLASSINFO(wxJigsawEditorView));
	wxUnusedVar(docTemplate);
	m_DocManager->SetMaxDocsOpen(1);
	
		
}

/*!
 * Initialisation for wxJigsawEditorApp
 */

bool wxJigsawEditorApp::OnInit()
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
	

	wxJigsawEditorMainFrame* mainWindow = 
		new wxJigsawEditorMainFrame( GetDocManager(), NULL, wxJigsawEditorMainFrame::ID_WXJIGSAWEDITORMAINFRAME );
//	mainWindow->Maximize();
	mainWindow->Show(true);

	m_DocManager->CreateDocument( wxEmptyString, wxDOC_NEW );

    return true;
}


/*!
 * Cleanup for wxJigsawEditorApp
 */

int wxJigsawEditorApp::OnExit()
{    
	
	wxDELETE(m_DocManager);
////@begin wxJigsawEditorApp cleanup
	return wxApp::OnExit();
////@end wxJigsawEditorApp cleanup
}

