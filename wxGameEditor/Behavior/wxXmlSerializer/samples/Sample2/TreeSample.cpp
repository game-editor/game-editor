/***************************************************************
 * Name:      TreeSample.cpp
 * Purpose:   DataTreeSample tutorial (classes implementation)
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-10-28
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "wx_pch.h"

#ifdef _DEBUG_MSVC
#define new DEBUG_NEW
#endif

#include "TreeSample.h"
#include "MainFrame.h"

int SerializableObject::m_nItemCounter = 0;

/////////////////////////////////////////////////////////////////////////////////////
// SerializableObject class /////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_IMPLEMENT_CLONABLE_CLASS(SerializableObject, xsSerializable);

SerializableObject::SerializableObject()
{
	// initialize member data
	m_sTreeItemName.Printf(wxT("Item %d"), ++m_nItemCounter);
	// mark the data members which should be serialized
	XS_SERIALIZE(m_sTreeItemName, wxT("item_name"));
}

SerializableObject::SerializableObject(SerializableObject &obj)
{
	// initialize member data
	m_sTreeItemName = obj.m_sTreeItemName;
	// mark the data members which should be serialized
	XS_SERIALIZE(m_sTreeItemName, wxT("item_name"));
}

SerializableObject::~SerializableObject()
{
}


/////////////////////////////////////////////////////////////////////////////////////
// TreeSampleApp class //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_APP(TreeSampleApp);

bool TreeSampleApp::OnInit()
{
	// load application settings if the configuration file exists, otherwise create new
	// settings class object with default values

	// initialize serializer
	m_XmlIO.SetSerializerOwner(wxT("TreeSampleApp"));
	m_XmlIO.SetSerializerRootName(wxT("tree"));
	m_XmlIO.SetSerializerVersion(wxT("1.0.0"));

	if( wxFileExists(wxT("data.xml")) )
	{
		// load tree data from configuration file (data class objects will be created at
		// run-time from the information stored in the configuration file)
		m_XmlIO.DeserializeFromXml(wxT("data.xml"));
	}

	// create and show main application frame
	MainFrame *frame = new MainFrame(NULL);
	SetTopWindow(frame);
	frame->Show();

	return true;
}

int TreeSampleApp::OnExit()
{
	// serialize tree data to given configuration file
	m_XmlIO.SerializeToXml(wxT("data.xml"));

	return 0;
}
