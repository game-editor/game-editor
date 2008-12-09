/*********************************************************************
 * Name:      SettingsSample.cpp
 * Purpose:   StaticSettingsTutorial tutorial (classes implementation)
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-10-28
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 *********************************************************************/

#include "wx_pch.h"

#ifdef _DEBUG_MSVC
#define new DEBUG_NEW
#endif

#include "StaticSettingsSample.h"
#include "MainFrame.h"

/////////////////////////////////////////////////////////////////////////////////////
// SerializableObject class /////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(SerializableObject, xsSerializable);

SerializableObject::SerializableObject()
{
	// initialize member data
	m_sTextData = wxT("Textual data encapsulated by 'SerializableObject' class object");
	// mark the data members which should be serialized
	XS_SERIALIZE(m_sTextData, wxT("text"));
}

SerializableObject::SerializableObject(SerializableObject &obj)
: xsSerializable(obj)
{
	// initialize member data
	m_sTextData = obj.m_sTextData;
	// mark the data members which should be serialized
	XS_SERIALIZE(m_sTextData, wxT("text"));
}

SerializableObject::~SerializableObject()
{
}

/////////////////////////////////////////////////////////////////////////////////////
// Settings class ///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_IMPLEMENT_CLONABLE_CLASS(Settings, xsSerializable);

Settings::Settings()
{
	// set default values of application properties:

	// int
	m_nIntData = 1024;
	// long
	m_nLongData = 123456789;
	// double
	m_nDoubleData = 3.14159267;
	// double
	m_nFloatData = 3.14f;
	// bool
	m_fBoolData = true;
	// wxChar
	m_nCharData = 'A';
	// wxString
	m_sTextData = wxT("Textual data");
	// wxPoint
	m_nPointData = wxPoint(0, 0);
	// wxSize
	m_nSizeData = wxSize(100, 200);
	// wxRealPoint
	m_nRealPointData = wxRealPoint(1.23, 4.56);
	// wxPen
	m_PenData = *wxBLACK_PEN;
	// wxBrush
	m_BrushData = *wxBLACK_BRUSH;
	// wxFont
	m_FontData = *wxSWISS_FONT;
	// wxColour
	m_nColourData = *wxWHITE;
	// wxArrayString
	m_arrStringData.Add(wxT("First string item"));
	m_arrStringData.Add(wxT("Second string item"));
	// RealPointArray - array of wxRealPoint values
	m_arrRealPointData.Add(wxRealPoint(1.2, 3.4));
	m_arrRealPointData.Add(wxRealPoint(5.6, 7.8));
	// RealPointList - list of wxRealPoint values
	m_lstRealPointData.Append(new wxRealPoint(1.2, 3.4));
	m_lstRealPointData.Append(new wxRealPoint(5.6, 7.8));
	// dynamic instances of xsSerializable class or other derived classes
	m_pDynamicSerializableObject = new SerializableObject();
	// static instances of xsSerializable class or other derived classes
	m_StaticSerializableObject.m_sTextData = wxT("Modified textual data encapsulated by 'SerializableObject' class object");

	// mark class data members which should be serialized
	MarkDataMembers();
}

// Copy constructor needed by the xml serializer class copy constructor
// or by default implementation of the xsSerializable::Clone() function.
// You haven't to define it if you don't plan to use the Clone() function
// or serializer class copy constructor.
Settings::Settings(Settings &obj) : xsSerializable(obj)
{
	// copy values from source object

	m_nIntData = obj.m_nIntData;
	m_nLongData = obj.m_nLongData;
	m_nDoubleData = obj.m_nDoubleData;
	m_nFloatData = obj.m_nFloatData;
	m_fBoolData = obj.m_fBoolData;
	m_nCharData = obj.m_nCharData;
	m_sTextData = obj.m_sTextData;
	m_nPointData = obj.m_nPointData;
	m_nSizeData = obj.m_nSizeData;
	m_nRealPointData = obj.m_nRealPointData;
	m_PenData = obj.m_PenData;
	m_BrushData = obj.m_BrushData;
	m_FontData = obj.m_FontData;
	m_nColourData = obj.m_nColourData;

	// copy array items
	WX_APPEND_ARRAY(m_arrStringData, obj.m_arrStringData);
	WX_APPEND_ARRAY(m_arrRealPointData, obj.m_arrRealPointData);

	// copy list item
	wxRealPointListNode *node = obj.m_lstRealPointData.GetFirst();
	while(node)
	{
		m_lstRealPointData.Append(new wxRealPoint(*node->GetData()));
		node = node->GetNext();
	}

	// copy dynamic instances of xsSerializable class or other derived classes
	m_pDynamicSerializableObject = (SerializableObject*)obj.m_pDynamicSerializableObject->Clone();

	// copy static instances of xsSerializable class or other derived classes
	m_StaticSerializableObject.m_sTextData = obj.m_StaticSerializableObject.m_sTextData;

	// mark class data members which should be serialized
	MarkDataMembers();
}

Settings::~Settings()
{
	// data clean up
	if( m_pDynamicSerializableObject )delete m_pDynamicSerializableObject;

	m_lstRealPointData.DeleteContents(true);
	m_lstRealPointData.Clear();
}

void Settings::MarkDataMembers()
{
	// mark class data members which should be serialized
	XS_SERIALIZE(m_nIntData, wxT("integer_data"));
	XS_SERIALIZE(m_nLongData, wxT("long_int_data"));
	XS_SERIALIZE(m_nFloatData, wxT("float_data"));
	XS_SERIALIZE(m_nDoubleData, wxT("double_data"));
	XS_SERIALIZE(m_fBoolData, wxT("boolean_data"));
	XS_SERIALIZE(m_nCharData, wxT("char_data"));
	XS_SERIALIZE(m_sTextData, wxT("string_data"));
	XS_SERIALIZE(m_nPointData, wxT("point_data"));
	XS_SERIALIZE(m_nSizeData, wxT("size_data"));
	XS_SERIALIZE(m_nRealPointData, wxT("realpoint_data"));
	XS_SERIALIZE(m_nColourData, wxT("color_data"));
	XS_SERIALIZE(m_PenData, wxT("pen_data"));
	XS_SERIALIZE(m_BrushData, wxT("brush_data"));
	XS_SERIALIZE(m_FontData, wxT("font_data"));
	XS_SERIALIZE(m_arrStringData, wxT("stringarray_data"));
	XS_SERIALIZE(m_arrRealPointData, wxT("realpointarray_data"));
	XS_SERIALIZE(m_lstRealPointData, wxT("realpointlist_data"));
	XS_SERIALIZE_DYNAMIC_OBJECT_NO_CREATE(m_pDynamicSerializableObject, wxT("dynamicobject_data"));
	XS_SERIALIZE_STATIC_OBJECT(m_StaticSerializableObject, wxT("staticobject_data"));
}

/////////////////////////////////////////////////////////////////////////////////////
// SettingsSampleApp class //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_APP(SettingsSampleApp);

bool SettingsSampleApp::OnInit()
{
	// load application settings if the configuration file exists, otherwise default
	// values are used

	// initialize serializer
	m_XmlIO.SetSerializerOwner(wxT("StaticSettingsSampleApp"));
	m_XmlIO.SetSerializerRootName(wxT("settings"));
	m_XmlIO.SetSerializerVersion(wxT("1.0.0"));

    // tell the serializer's root node it should serialize static 'Settings' class instance as a
    // standard property (the 'Settings' class instance is not created at the runtime, only its
    // properties are serialized).
    m_XmlIO.GetRootItem()->AddProperty(new xsProperty(&m_Settings, wxT("serializablestatic"), wxT("app_settings")));

	if( wxFileExists(wxT("settings.xml")) )
	{		// load settings from configuration file
		m_XmlIO.DeserializeFromXml(wxT("settings.xml"));
	}

    // create and show main application frame
    MainFrame *frame = new MainFrame(NULL);
    SetTopWindow(frame);
    frame->Show();

    return true;
}

int SettingsSampleApp::OnExit()
{
	// Write application settings. We must tell the serializer the root's properties
	// added manually in the OnInit() function should be serialzied as well. Note that
	// the root node isn't serialized like any other nodes: only its properties can
	// be serialized into special xml node called 'NAME_properties' where NAME is the root
	// node name set by function SetSerializerRootName().
    m_XmlIO.SerializeToXml(wxT("settings.xml"), xsWITH_ROOT );

	return 0;
}
