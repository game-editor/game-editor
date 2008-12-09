/***************************************************************
 * Name:      CustomDataSample.h
 * Purpose:   CustomDataSample tutorial (classes declaration)
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-10-28
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef __SettingsSample__
#define __SettingsSample__

#include "wx/wxxmlserializer/XmlSerializer.h"
#include <wx/arrstr.h>
#include <wx/colordlg.h>

// declaration of a class 'xsColourDataPropIO' encapsulating custom property I/O handler
// for 'wxColouData' data type
XS_DECLARE_IO_HANDLER(wxColourData, xsColourDataPropIO);

// definition macros which create new serialized wxColourData property (without/with defined default value). data type's assertion is not compulsory, but usefull...
#define XS_SERIALIZE_COLOURDATA(x, name) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxColourData)), wxT("Object is not wxColourData"));XS_SERIALIZE_PROPERTY(x, wxT("colourdata"), name);
#define XS_SERIALIZE_COLOURDATA_EX(x, name, def) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxColourData)), wxT("Object is not wxColourData"));XS_SERIALIZE_PROPERTY_EX(x, wxT("colourdata"), name, xsColourDataPropIO::ToString(def));

// serializable class
class Settings : public xsSerializable
{
public:
	// RTTI and xsSerializable::Clone() function must be provided
	XS_DECLARE_CLONABLE_CLASS(Settings);

	// constructor
	Settings();
	// copy onstructor needed by default implementation of xsSerializable::Clone() function
	Settings(Settings &obj);
	// destructor
	virtual ~Settings();

	// public data members
	wxColourData m_colourData;
};

// application class
class CustomDataSampleApp : public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();

	// public data members

	// settings class
	Settings *m_pSettings;

protected:
	// protected data members

	// main serializer object
	wxXmlSerializer m_XmlIO;
};

DECLARE_APP(CustomDataSampleApp);

#endif //__SettingsSample__
