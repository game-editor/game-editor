/***************************************************************
 * Name:      TreeSample.h
 * Purpose:   DataTreeSample tutorial (classes declaration)
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-10-28
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef __TreeSampleApp__
#define __TreeSampleApp__

#include "wx/wxxmlserializer/XmlSerializer.h"
#include <wx/arrstr.h>

// serializable class
class SerializableObject : public xsSerializable
{
public:
	// RTTI and xsSerializable::Clone() function must be provided
	XS_DECLARE_CLONABLE_CLASS(SerializableObject);

	// constructor
	SerializableObject();
	// copy onstructor needed by default implementation of xsSerializable::Clone() function
	SerializableObject(SerializableObject &obj);
	// destructor
	virtual ~SerializableObject();

	// protected data members
	wxString m_sTreeItemName;

private:
	// private data members
	static int m_nItemCounter;
};

// application class
class TreeSampleApp : public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();

	// main serializer object
	wxXmlSerializer m_XmlIO;
};

DECLARE_APP(TreeSampleApp);

#endif //__TreeSampleApp__
