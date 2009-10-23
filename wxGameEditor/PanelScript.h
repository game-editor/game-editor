/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

If GPL v3 does not work for you, we offer a "non-GPL" license 
<http://game-editor.com/License>, thats will help us to pay our developers 
<http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>.

***************************************************************************/


#pragma once

#include "wx/wxprec.h"
#include "GameControl.h"
#include "wxGed/wxGedControls.h"
#include "wx/ifm/ifm.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/treectrl.h"
#include "wx/listctrl.h"
#include "wx/stedit/stedit.h"

#include "wx/propgrid/odcombo.h"

// Main propertygrid header.
#include <wx/propgrid/propgrid.h>

// Needed for implementing custom properties.
#include <wx/propgrid/propdev.h>

// Extra property classes.
#include <wx/propgrid/advprops.h>

// This defines wxPropertyGridManager.
#include <wx/propgrid/manager.h>

#define ID_ADD_SCRIPT				12
#define ID_REMOVE_SCRIPT			13
#define ID_SHOW_FUNCTIONS			14
#define ID_ADD_VAR					15
#define ID_ACTOR_CODE_SELECTOR		16
#define ID_CODE_FUNCTION_SELECTOR	17
#define ID_APPLY_CHANGES			18

typedef GlMap< gedString, stAction *, GlStringHash >		MatstAction;
typedef GlMapIterator< gedString, stAction *, GlStringHash >		MatstActionIterator;

class PanelScript :	public wxPanel
{
	DECLARE_CLASS(PanelGenericProperty)
    DECLARE_EVENT_TABLE()

public:
	PanelScript(wxWindow *parent);
	virtual ~PanelScript(void);

	virtual void OnSize(wxSizeEvent & event);
	virtual void OnShow(wxShowEvent &event);
	virtual void OnSelect ( wxCommandEvent& event );
	virtual void OnRemove( wxCommandEvent& event );	
	virtual void OnApplyChanges( wxCommandEvent& event );	
	virtual void OnPageChanged(wxNotebookEvent &event);
	void OnSTEState(wxSTEditorEvent &event);

	static void Update();

private:
	
	void CreateToolBar();
	void CreateNotebook();
	void RemoveEditor(gedString &name);
	void VerifyChanges();

	wxSTEditorNotebook *script;
	wxToolBar *toolBar;
	wxPGOwnerDrawnComboBox *actorCodeSelector;
	wxListCtrl *codeFunctionSelector;

	MatstAction mapAction;

	static PanelScript *panelScript;
};
