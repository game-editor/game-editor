/***************************************************************
 * Name:      CustomDataSample.cpp
 * Purpose:   CustomDataSample tutorial (classes implementation)
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

#include "CustomDataSample.h"
#include "MainFrame.h"

/////////////////////////////////////////////////////////////////////////////////////
// xsColourDataPropIO class /////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

// define class encapsulating custom data format handler
XS_DEFINE_IO_HANDLER(wxColourData, xsColourDataPropIO);

// two following static member functions of the data handler class MUST be defined manualy:

// wxString xsPropIO::ToString(T value) -> creates a string representation of the given value
wxString xsColourDataPropIO::ToString(wxColourData value)
{
	wxString out;

	out << xsColourPropIO::ToString(value.GetColour());
	for(int i = 0; i < 16; i++)
	{
		out << wxT("|") << xsColourPropIO::ToString(value.GetCustomColour(i));
	}

    return out;
}

// T xsPropIO::FromString(const wxString& value) -> converts data from given string representation to its relevant value
wxColourData xsColourDataPropIO::FromString(const wxString& value)
{
	wxColourData data;

	if(!value.IsEmpty())
	{
		int i = 0;
		wxStringTokenizer tokens(value, wxT("|"), wxTOKEN_STRTOK);

		data.SetColour(xsColourPropIO::FromString(tokens.GetNextToken()));
		while(tokens.HasMoreTokens())
		{
			data.SetCustomColour(i, xsColourPropIO::FromString(tokens.GetNextToken()));
			i++;
		}
	}

	return data;
}

/////////////////////////////////////////////////////////////////////////////////////
// Settings class ///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_IMPLEMENT_CLONABLE_CLASS(Settings, xsSerializable);

Settings::Settings()
{
	// set default values of application properties:
	m_colourData.SetColour(*wxBLUE);
	for(int i = 0; i < 16; i++)m_colourData.SetCustomColour(i, wxColour(i*16, i*16, i*16));

	// serialize colour data everytime
	XS_SERIALIZE_COLOURDATA(m_colourData, wxT("colordlg_content"));

	// this version of mark macro causes the data will be serialized only if its value
	// differs from the default one (the last macro parameter)
	//XS_SERIALIZE_COLOURDATA_EX(m_colourData, wxT("colordlg_content"), m_colourData);
}

Settings::Settings(Settings &obj)
{
	// set default values of application properties:
	m_colourData = obj.m_colourData;

	// serialize colour data everytime
	XS_SERIALIZE_COLOURDATA(m_colourData, wxT("colordlg_content"));

	// this version of mark macro causes the data will be serialized only if its value
	// differs from the default one (the last macro parameter)
	//XS_SERIALIZE_COLOURDATA_EX(m_colourData, wxT("colordlg_content"), m_colourData);
}

Settings::~Settings()
{
	// data clean up
}

/////////////////////////////////////////////////////////////////////////////////////
// CustomDataSampleApp class ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_APP(CustomDataSampleApp);

bool CustomDataSampleApp::OnInit()
{
	// load application settings if the configuration file exists, otherwise create new
	// settings class object with default values

	// initialize serializer
	m_XmlIO.SetSerializerOwner(wxT("CustomDataSampleApp"));
	m_XmlIO.SetSerializerRootName(wxT("settings"));
	m_XmlIO.SetSerializerVersion(wxT("1.0.0"));

	// register new property I/O handler 'xsColourDataPropIO' for data type with name 'colourdata'
	XS_REGISTER_IO_HANDLER(wxT("colourdata"), xsColourDataPropIO);

    // create serialized settings class object manualy with default values
    m_pSettings = new Settings();
    // insert settings class object into serializer as its root node
    m_XmlIO.SetRootItem(m_pSettings);

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

int CustomDataSampleApp::OnExit()
{
	// write application settings
	if( m_pSettings )
	{
		// serialize settings to XML file
		m_XmlIO.SerializeToXml(wxT("settings.xml"), xsWITH_ROOT);
	}

	return 0;
}
