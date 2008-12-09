/***************************************************************
 * Name:      MainFrame.h
 * Purpose:   DynamicSettingsSample tutorial (application main frame)
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-10-28
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef __MainFrame__
#define __MainFrame__

#include "GUI.h"
#include "SettingsSample.h"

class MainFrame : public _MainFrame
{
protected:
	void DumpSerializableObject(xsSerializable *obj, wxTextCtrl *memo);

	// Handlers for _MainFrame events.
	void OnOk( wxCommandEvent& event );

public:
	MainFrame( wxWindow* parent );
};

#endif // __MainFrame__
