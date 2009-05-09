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
	m_ShapeRegistry = new wxJigsawShapeList;
	m_Config = NULL;
////@end wxJigsawEditorApp member initialisation
	wxDocTemplate * docTemplate = new wxDocTemplate(m_DocManager, _("Jigsaw Scene"),
		wxT("*.jigscene;*.xml"), wxEmptyString, wxT("jigscene"), 
		wxT("wxJigsawEditorDoc"), wxT("wxJigsawEditorView"),
		CLASSINFO(wxJigsawEditorDocument), CLASSINFO(wxJigsawEditorView));
	wxUnusedVar(docTemplate);
	m_DocManager->SetMaxDocsOpen(1);
	m_ShapeRegistry->DeleteContents(true);
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
	InitConfigSerialization();
	/*m_Config->GetShapesLookupDirectories().Add(wxT("."));
	m_Config->GetShapesLookupDirectories().Add(wxT("shapes"));
	SaveConfig();*/
	LoadConfig();
	LoadPalettes();

	LoadShapeRegistry();

	wxJigsawEditorMainFrame* mainWindow = 
		new wxJigsawEditorMainFrame( GetDocManager(), NULL, wxJigsawEditorMainFrame::ID_WXJIGSAWEDITORMAINFRAME );
	mainWindow->Maximize();
	mainWindow->Show(true);

    return true;
}


/*!
 * Cleanup for wxJigsawEditorApp
 */

int wxJigsawEditorApp::OnExit()
{    
	wxDELETE(m_ShapeRegistry);
	wxDELETE(m_DocManager);
////@begin wxJigsawEditorApp cleanup
	return wxApp::OnExit();
////@end wxJigsawEditorApp cleanup
}

bool wxJigsawEditorApp::LoadShapeRegistry()
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

void wxJigsawEditorApp::InitConfigSerialization()
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

void wxJigsawEditorApp::LoadConfig()
{
	wxString configFileName = wxT("config.xml");
	if(wxFileExists(configFileName))
	{
		m_XmlIO.DeserializeFromXml(configFileName);
	}
}

void wxJigsawEditorApp::SaveConfig()
{
	wxString configFileName = wxT("config.xml");
	m_XmlIO.SerializeToXml(configFileName, true);
}

wxJigsawShape * wxJigsawEditorApp::LoadShape(const wxString & shapeFileName)
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

void wxJigsawEditorApp::LoadPalettes()
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
