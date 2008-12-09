/****************************************************************
 * Name:      SettingsSample.h
 * Purpose:   StaticSettingsSample tutorial (classes declaration)
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-10-28
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 ****************************************************************/

#ifndef __SettingsSample__
#define __SettingsSample__

#include "wx/wxxmlserializer/XmlSerializer.h"
#include <wx/arrstr.h>

// some serializable class
class SerializableObject : public xsSerializable
{
public:
	// RTTI must be provided
	DECLARE_DYNAMIC_CLASS(SerializableObject);

	// constructor
	SerializableObject();
	// copy constructor needed by the xml serializer class copy constructor
	// or by default implementation of the xsSerializable::Clone() function.
	// You haven't to define it if you don't plan to use the Clone() function
	// or serializer class copy constructor.
	SerializableObject(SerializableObject &obj);
	// destructor
	virtual ~SerializableObject();

	// Clone function and copy constructor must be implemented for proper
	// functionality of the serializer class copy constructor and
	// wxXmlSerializer::CopyItems() function. The Clone function can
	// be defined automatically if you use XS_DECLARE_CLONABLE_CLASS and
	// XS_IMPLEMENT_CLONABLE_CLASS instead of the DECLARE_DYNAMIC_CLASS
	// and IMPLEMENT_DYNAMIC_CLASS (see Settings class bellow)
	virtual xsSerializable* Clone(){return new SerializableObject(*this);}

	// protected data members
	wxString m_sTextData;
};

// main settings class ( also serializable class... )
class Settings : public xsSerializable
{
public:
	// RTTI and xsSerializable::Clone() function must be provided
	XS_DECLARE_CLONABLE_CLASS(Settings);

	// constructor
	Settings();
	// copy constructor needed by the xml serializer class copy constructor
	// or by default implementation of the xsSerializable::Clone() function.
	// You haven't to define it if you don't plan to use the Clone() function
	// or serializer class copy constructor.
	Settings(Settings &obj);
	// destructor
	virtual ~Settings();

protected:
	// protected data members

	// wxXmlSerializer currently supports these data types:
	// int
	int m_nIntData;
	// long
	long m_nLongData;
	// double
	float m_nFloatData;
	// double
	double m_nDoubleData;
	// bool
	bool m_fBoolData;
	// wxChar
	wxChar m_nCharData;
	// wxString
	wxString m_sTextData;
	// wxPoint
	wxPoint m_nPointData;
	// wxSize
	wxSize m_nSizeData;
	// wxRealPoint
	wxRealPoint m_nRealPointData;
	// wxColour
	wxColour m_nColourData;
	// wxPen
	wxPen m_PenData;
	// wxBrush
	wxBrush m_BrushData;
	// wxFont
	wxFont m_FontData;
	// wxArrayString
	wxArrayString m_arrStringData;
	// RealPointArray - array of wxRealPoint values
	RealPointArray m_arrRealPointData;
	// RealPointList - list of wxRealPoint values
	RealPointList m_lstRealPointData;
	// dynamic instances of xsSerializable class or other derived classes
	SerializableObject *m_pDynamicSerializableObject;
	// static instances of xsSerializable class or other derived classes
	SerializableObject m_StaticSerializableObject;

private:
	// private auxiliary functions
	void MarkDataMembers();
};

// application class
class SettingsSampleApp : public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();

	// public data members

	// settings class
	Settings m_Settings;

protected:
	// protected data members

	// main serializer object
	wxXmlSerializer m_XmlIO;
};

DECLARE_APP(SettingsSampleApp);

#endif //__SettingsSample__
