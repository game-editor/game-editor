/***************************************************************
 * Name:      MainFrame.h
 * Purpose:   DataTreeSample tutorial (application main frame)
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-10-28
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef __MainFrame__
#define __MainFrame__

#include "GUI.h"

// a user data container for tree control items
class TreeData : public wxTreeItemData
{
public:
	TreeData(long id){m_nDataId = id;}
	long m_nDataId;
};

// Implementing _MainFrame
class MainFrame : public _MainFrame
{
protected:
	// Handlers for _MainFrame events.
	void OnAddClick( wxCommandEvent& event );
	void OnUpdateAdd( wxUpdateUIEvent& event );
	void OnRemoveClick( wxCommandEvent& event );
	void OnUpdateRemove( wxUpdateUIEvent& event );
	void OnAboutClick( wxCommandEvent& event );
	void OnOk( wxCommandEvent& event );

	void CreateTreeRoot();
	void BuildTreeFromData();
	void _BuildChildren(wxTreeItemId parent);

public:
	MainFrame( wxWindow* parent );
};

#endif // __MainFrame__
