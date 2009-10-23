/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com

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
More info at http://game-editor.com/License
Be a Game Editor developer: http://game-editor.com/Sharing_Software_Revenues_in_Open_Source

***************************************************************************/


// PanelGenericProperty.h: interface for the PanelGenericProperty class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PANELGENERICPROPERTY_H__AE04F836_AFB9_4ED6_BED1_11903F986F11__INCLUDED_)
#define AFX_PANELGENERICPROPERTY_H__AE04F836_AFB9_4ED6_BED1_11903F986F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "wx/wxprec.h"
#include "GameControl.h"
#include "wxGed/wxGedControls.h"
#include "wx/ifm/ifm.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

// Main propertygrid header.
#include <wx/propgrid/propgrid.h>

// Needed for implementing custom properties.
#include <wx/propgrid/propdev.h>

// Extra property classes.
#include <wx/propgrid/advprops.h>

// This defines wxPropertyGridManager.
#include <wx/propgrid/manager.h>

#include <wx/spinctrl.h>

#define PANEL_PROPERTY_ID	1
#define PANEL_EVENTS_ID		2

class PanelGenericProperty : public wxPanel  
{
	DECLARE_CLASS(PanelGenericProperty)
    DECLARE_EVENT_TABLE()

public:
	PanelGenericProperty(wxWindow *parent, int _pgId, unsigned int _flags = 0, bool bCreateManager = false);
	virtual ~PanelGenericProperty();

	virtual void OnSize(wxSizeEvent & event);
	virtual void OnPropertyGridItemCollapse ( wxPropertyGridEvent& event );
    virtual void OnPropertyGridItemExpand ( wxPropertyGridEvent& event );

	int GetMaxHeight() {return pg?pg->GetBottomY() + 20:-1;}

	virtual void OnFocus(wxFocusEvent &event);
	virtual void OnPropertyGridHighlight( wxPropertyGridEvent& event );
	virtual void OnPropertyGridSelected ( wxPropertyGridEvent& event );
	
	void SaveState(wxDataOutputStream& stream);
	void LoadState(wxDataInputStream& stream);
	void ResetState();

	wxPropertyGrid* GetPropertyGrid() {return pg;}

protected:
	void UpdateSizeLimits();
	void SavePropertyState(wxPGId id, wxDataOutputStream& stream);

	wxColour cellColor;

	int rightMargin, initialSplitterPos;

	wxPropertyGridManager* pgManager;
	wxPropertyGrid* pg;
};


// NOTE: Regardless that this class inherits from a working editor, it has
//   all necessary methods to work independently. wxTextCtrl stuff is only
//   used for event handling here.
class wxPGSpinCtrlEditor : public wxPGTextCtrlEditor
{
    WX_PG_DECLARE_EDITOR_CLASS()
public:
    virtual ~wxPGSpinCtrlEditor();

    // See below for short explanations of what these are suppposed to do.
    virtual wxWindow* CreateControls ( wxPropertyGrid* propgrid, wxPGProperty* property, const wxPoint& pos, const wxSize& sz, wxWindow** psecondary ) const;
    virtual void UpdateControl ( wxPGProperty* property, wxWindow* wnd ) const;
    virtual bool OnEvent ( wxPropertyGrid* propgrid, wxPGProperty* property, wxWindow* wnd, wxEvent& event ) const;
    virtual bool CopyValueFromControl ( wxPGProperty* property, wxWindow* wnd ) const;
    virtual void SetValueToUnspecified ( wxWindow* wnd ) const;
    virtual void SetControlStringValue ( wxWindow* wnd, const wxString& txt ) const;
    virtual void OnFocus ( wxPGProperty* property, wxWindow* wnd ) const;
};


WX_PG_DECLARE_EDITOR_WITH_DECL(SpinCtrl,WXDLLIMPEXP_PG)


class wxPGKeyCtrlEditor : public wxPGTextCtrlEditor
{
    WX_PG_DECLARE_EDITOR_CLASS()
public:
    virtual ~wxPGKeyCtrlEditor();

    // See below for short explanations of what these are suppposed to do.
    //virtual wxWindow* CreateControls ( wxPropertyGrid* propgrid, wxPGProperty* property, const wxPoint& pos, const wxSize& sz, wxWindow** psecondary ) const;
    virtual void UpdateControl ( wxPGProperty* property, wxWindow* wnd ) const;
    virtual bool OnEvent ( wxPropertyGrid* propgrid, wxPGProperty* property, wxWindow* wnd, wxEvent& event ) const;
    virtual bool CopyValueFromControl ( wxPGProperty* property, wxWindow* wnd ) const;
    virtual void SetValueToUnspecified ( wxWindow* wnd ) const;
    virtual void SetControlStringValue ( wxWindow* wnd, const wxString& txt ) const;
};


WX_PG_DECLARE_EDITOR_WITH_DECL(KeyCtrl,WXDLLIMPEXP_PG)

#endif // !defined(AFX_PANELGENERICPROPERTY_H__AE04F836_AFB9_4ED6_BED1_11903F986F11__INCLUDED_)
