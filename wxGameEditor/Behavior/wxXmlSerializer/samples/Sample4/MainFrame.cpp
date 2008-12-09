/*******************************************************************
 * Name:      MainFrame.cpp
 * Purpose:   StaticSettingsSample tutorial (application main frame)
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-10-28
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 *******************************************************************/

#include "wx_pch.h"

#ifdef _DEBUG_MSVC
#define new DEBUG_NEW
#endif

#include "MainFrame.h"

MainFrame::MainFrame( wxWindow* parent )
: _MainFrame( parent )
{
    #ifdef __WXMSW__
	SetIcon(wxICON(amain));
	#endif

	m_pTextCtrl->SetValue(wxT("This sample demonstrates how to load/save an application settings \
via the wxXmlSerializer class.\n\nThe text bellow \
shows serialized properties of 'Settings' class object stored in \
'settings.xml' file (the class object is static in this sample - see the source code). \
So called 'Properties' (xsProperty class objects) encapsulate marked class members \
of the 'Settings' class (derived from 'xsSerializable' class ) which can be accessed \
in the standard. Class data members which should be serialized must be marked by \
'XS_SERIALIZE' macro or by similar ones.\n\n"));

	// Dump content of the Setting class. Of course, you can also use the wxGetApp().m_pSettings
	// class members in the standard way...
	DumpSerializableObject(&wxGetApp().m_Settings, m_pTextCtrl);

	Center();
}

void MainFrame::DumpSerializableObject(xsSerializable *obj, wxTextCtrl *memo)
{
	// dump info about serializable object
	memo->AppendText(wxString::Format(wxT("Serializable object of type '%s' with ID:%d at address 0x%x :\n"), obj->GetClassInfo()->GetClassName(), obj->GetId(), obj));

	// get serialized properties maintained by the Setting class
	xsProperty *prop;
	xsPropertyIO* ioHandler;

	PropertyList::compatibility_iterator node = obj->GetProperties().GetFirst();
	while( node )
	{
		prop = node->GetData();
		// get suitable I/O handler for current property (depends on its data type)
		ioHandler = wxXmlSerializer::m_mapPropertyIOHandlers[prop->m_sDataType];

		memo->AppendText(wxString::Format(wxT("Property name: %s, Type: %s, Value: %s\n"), prop->m_sFieldName.c_str(), prop->m_sDataType.c_str(), ioHandler->GetValueStr(prop).c_str()));

		node = node->GetNext();
	}

	memo->AppendText(wxT("\n"));
}

void MainFrame::OnOk( wxCommandEvent& event )
{
	Destroy();
}
