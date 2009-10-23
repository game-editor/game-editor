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


// PanelGenericProperty.cpp: implementation of the PanelGenericProperty class.
//
//////////////////////////////////////////////////////////////////////

#include "PanelGenericProperty.h"


IMPLEMENT_CLASS(PanelGenericProperty, wxPanel)

BEGIN_EVENT_TABLE(PanelGenericProperty, wxPanel)
	EVT_SIZE(PanelGenericProperty::OnSize)
	EVT_PG_ITEM_COLLAPSED( wxID_ANY, PanelGenericProperty::OnPropertyGridItemCollapse )
    EVT_PG_ITEM_EXPANDED( wxID_ANY, PanelGenericProperty::OnPropertyGridItemExpand )
	EVT_SET_FOCUS(PanelGenericProperty::OnFocus) 
	EVT_PG_SELECTED( wxID_ANY, PanelGenericProperty::OnPropertyGridSelected )    
    EVT_PG_HIGHLIGHTED( wxID_ANY, PanelGenericProperty::OnPropertyGridHighlight )
END_EVENT_TABLE()


//
// Implement custom editor control that allows using wxSpinCtrl to
// edit value of wxIntProperty (and similar).
//
// Note that this class has to be registered using wxPGRegisterEditorClass
// (see code below) before it can be used (but the registeration can be
// performed even in a constructor of a property that is known to require
// it).
//
// KNOWN ISSUES
// * Settings value to unspecified doesn't work properly: When such value
//   is edited manually (ie. not by spinbutton), it is not stored. This is
//   because we can't interprete textctrl update events here because they
//   occur even when value is set by program. For wxTextCtrl this is handled
//   by checking the string length (GetLocation() method), but there is
//   no suitable API for wxSpinCtrl.
//
// * Limited editing mode doesn't really work because wxSpinCtrl can't
//   be set to read-only.
//


// This macro also defined global wxPGEditor_SpinCtrl for storing
// the singleton class instance.
WX_PG_IMPLEMENT_EDITOR_CLASS(SpinCtrl,wxPGSpinCtrlEditor,wxPGEditor)


// Trivial destructor.
wxPGSpinCtrlEditor::~wxPGSpinCtrlEditor() { }


// Create controls and initialize event handling.
wxWindow* wxPGSpinCtrlEditor::CreateControls ( wxPropertyGrid* propgrid, wxPGProperty* property,
        const wxPoint& pos, const wxSize& sz, wxWindow** ) const
{

    // Get initial value (may be none if value is 'unspecified')
    wxString text;
    if ( !(property->GetFlags() & wxPG_PROP_UNSPECIFIED) )
        text = property->GetValueAsString(0);

        // If it can't translate value to int, this will fail at run-time.
    double value = propgrid->GetPropertyValueAsDouble(property), min, max, increment;

	GetFloatConstraints(property, min, max, increment);	

    // Use two stage creation to allow cleaner display on wxMSW
    wxGedSpin* ctrl = new wxGedSpin();
#ifdef __WXMSW__
    ctrl->Hide();
#endif
    ctrl->Create(propgrid,wxPG_SUBID1,
                 text,pos,sz,
                 wxNO_BORDER |
				 wxSP_ARROW_KEYS |
				 wxSP_HORIZONTAL,
                 // In limited editing mode, use read-only textctrl
                 // (if possible - but its not so this is merely
                 // an example code)
                 //|((property->GetFlags() & wxPG_PROP_NOEDITOR)?wxTE_READONLY:0)
                 min,max,value, increment);

    // Connect all required events to grid's OnCustomEditorEvent
    // (all relevenat wxTextCtrl, wxComboBox and wxButton events are
    // already connected)
    propgrid->Connect( wxPG_SUBID1, wxEVT_COMMAND_SPINCTRL_UPDATED,
                       (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)
                       &wxPropertyGrid::OnCustomEditorEvent );

    // This centers the control in a platform dependent manner
    propgrid->FixPosForTextCtrl( ctrl );

#ifdef __WXMSW__
    ctrl->Show();
#endif

    return ctrl;
}

// Copies value from property to control
void wxPGSpinCtrlEditor::UpdateControl ( wxPGProperty* property, wxWindow* wnd ) const
{
    wxGedSpin* ctrl = (wxGedSpin*) wnd;
    wxASSERT( ctrl && ctrl->IsKindOf(CLASSINFO(wxGedSpin)) );

    // We assume that property's data type is 'int' (or something similar),
    // thus allowing us to get raw, unchecked value via DoGetValue.
    ctrl->SetValue( property->DoGetValue().GetLong() );
}

// Control's events are redirected here
bool wxPGSpinCtrlEditor::OnEvent ( wxPropertyGrid* propgrid, wxPGProperty* property,
    wxWindow* wnd, wxEvent& event ) const
{
    if ( event.GetEventType() == wxEVT_COMMAND_SPINCTRL_UPDATED )
    {
        if ( CopyValueFromControl( property, wnd ) )
            return true;

        propgrid->EditorsValueWasNotModified();
    }
    else
        return wxPGTextCtrlEditor::OnEvent(propgrid,property,wnd,event);

    return false;
}

// Copies value from control to property
bool wxPGSpinCtrlEditor::CopyValueFromControl ( wxPGProperty* property, wxWindow* wnd ) const
{
    wxGedSpin* ctrl = (wxGedSpin*) wnd;
    wxASSERT( ctrl && ctrl->IsKindOf(CLASSINFO(wxGedSpin)) );

    return property->SetValueFromDouble(ctrl->GetValue(),wxPG_FULL_VALUE);
}

// Makes control look like it has unspecified value
void wxPGSpinCtrlEditor::SetValueToUnspecified ( wxWindow* wnd ) const
{
    wxGedSpin* ctrl = (wxGedSpin*) wnd;
    wxASSERT( ctrl && ctrl->IsKindOf(CLASSINFO(wxGedSpin)) );

    ctrl->SetValue(wxEmptyString);
}

// Used when control's value is wanted to set from string source
// (obviously, not all controls can implement this properly,
//  but wxSpinCtrl can)
void wxPGSpinCtrlEditor::SetControlStringValue ( wxWindow* wnd, const wxString& txt ) const
{
    wxGedSpin* ctrl = (wxGedSpin*) wnd;
    wxASSERT( ctrl && ctrl->IsKindOf(CLASSINFO(wxGedSpin)) );

    ctrl->SetValue(txt);
}

void wxPGSpinCtrlEditor::OnFocus ( wxPGProperty*, wxWindow* wnd ) const
{
    wxGedSpin* ctrl = (wxGedSpin*) wnd;
    //wxASSERT( ctrl && ctrl->IsKindOf(CLASSINFO(wxGedSpin)) );

    ctrl->SetSelection(-1,-1);
}



/////////////////////////////////////////////
//KeyCtrl
WX_PG_IMPLEMENT_EDITOR_CLASS(KeyCtrl,wxPGKeyCtrlEditor,wxPGEditor)


// Trivial destructor.
wxPGKeyCtrlEditor::~wxPGKeyCtrlEditor() { }



// Copies value from property to control
void wxPGKeyCtrlEditor::UpdateControl ( wxPGProperty* property, wxWindow* wnd ) const
{
    /*wxGedSpin* ctrl = (wxGedSpin*) wnd;
    wxASSERT( ctrl && ctrl->IsKindOf(CLASSINFO(wxGedSpin)) );

    ctrl->SetValue( property->DoGetValue().GetLong() );*/
}

// Control's events are redirected here
bool wxPGKeyCtrlEditor::OnEvent ( wxPropertyGrid* propgrid, wxPGProperty* property,
    wxWindow* wnd, wxEvent& event ) const
{
    /*if ( event.GetEventType() == wxEVT_COMMAND_SPINCTRL_UPDATED )
    {
        if ( CopyValueFromControl( property, wnd ) )
            return true;

        propgrid->EditorsValueWasNotModified();
    }
    else
        return wxPGTextCtrlEditor::OnEvent(propgrid,property,wnd,event);*/

    return false;
}

// Copies value from control to property
bool wxPGKeyCtrlEditor::CopyValueFromControl ( wxPGProperty* property, wxWindow* wnd ) const
{
    /*wxGedSpin* ctrl = (wxGedSpin*) wnd;
    wxASSERT( ctrl && ctrl->IsKindOf(CLASSINFO(wxGedSpin)) );

    return property->SetValueFromDouble(ctrl->GetValue(),wxPG_FULL_VALUE);*/

	return true;
}

// Makes control look like it has unspecified value
void wxPGKeyCtrlEditor::SetValueToUnspecified ( wxWindow* wnd ) const
{
    /*wxGedSpin* ctrl = (wxGedSpin*) wnd;
    wxASSERT( ctrl && ctrl->IsKindOf(CLASSINFO(wxGedSpin)) );

    ctrl->SetValue(wxEmptyString);*/
}


void wxPGKeyCtrlEditor::SetControlStringValue ( wxWindow* wnd, const wxString& txt ) const
{
    /*wxGedSpin* ctrl = (wxGedSpin*) wnd;
    wxASSERT( ctrl && ctrl->IsKindOf(CLASSINFO(wxGedSpin)) );

    ctrl->SetValue(txt);*/
}

/////////////////////////////////////////////





PanelGenericProperty::PanelGenericProperty(wxWindow *parent, int _pgId, unsigned int _flags, bool bCreateManager)  : wxPanel(parent)
{
	SetSizeHints(0,0);	

	unsigned int flags = 
		//wxPG_BOLD_MODIFIED |
        //wxPG_SPLITTER_AUTO_CENTER |
        //wxPG_AUTO_SORT |
        //wxPG_HIDE_MARGIN | 
		//wxPG_STATIC_SPLITTER |
        wxPG_TOOLTIPS | 
        //wxPG_NOCATEGORIES |
		//wxSUNKEN_BORDER |		
		wxNO_BORDER |
		wxPG_LIMITED_EDITING |
        wxTAB_TRAVERSAL |
        _flags;
	
	pgManager = NULL;
	pg = NULL;

	if(bCreateManager)
	{
		flags |= wxPG_TOOLBAR;
		pgManager = new wxPropertyGridManager(this, _pgId, wxDefaultPosition, wxDefaultSize, flags);
		pg = pgManager->GetGrid();
		pgManager->RecreateControls();
		
		if(pgManager->GetToolBar())
		{
			pgManager->GetToolBar()->SetBackgroundColour(wxBackground_Pen);
		}
	}
	else
	{
		pg = new wxPropertyGrid(this, _pgId, wxDefaultPosition, wxDefaultSize, flags);
	}
	
	// Register SpinCtrl editor before use
    wxPGRegisterEditorClass( SpinCtrl );
	wxPGRegisterEditorClass( KeyCtrl );

	//Font (in user configuration don't allow < 7)
	wxFont font(7, wxFONTFAMILY_SWISS, wxNORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial"));
	pg->SetFont(font);

	//Colors
	cellColor.Set(colorBgContent.Red() + 30, colorBgContent.Green() + 30, colorBgContent.Blue() + 30);

	SetBackgroundColour ( colorBgContent );
	pg->SetBackgroundColour ( colorBgContent );	

	pg->Freeze();
	pg->SetMarginColour ( colorBgContent );
	pg->SetCaptionBackgroundColour ( colorBgContent );
	pg->SetCaptionForegroundColour(*wxBLACK);
	pg->SetCellBackgroundColour ( cellColor );
	pg->SetCellTextColour ( *wxBLACK );
	pg->SetLineColour ( *wxBLACK ); 
	pg->Thaw();
	
	initialSplitterPos = pg->GetSplitterPosition();
	rightMargin = 5;
}

PanelGenericProperty::~PanelGenericProperty()
{
	if(pgManager)
	{
		delete pgManager;
		pgManager = NULL;
	}
	else
	{
		delete pg;
		pg = NULL;
	}
}

void PanelGenericProperty::OnSize(wxSizeEvent & event)
{
	wxSize size(event.GetSize().GetWidth() - rightMargin, event.GetSize().GetHeight());
	
	if(pgManager) pgManager->SetSize(size);
	else if(pg) pg->SetSize(size);
}

void PanelGenericProperty::OnPropertyGridItemCollapse( wxPropertyGridEvent& )
{
	//UpdateSizeLimits(); 
}

void PanelGenericProperty::OnPropertyGridItemExpand( wxPropertyGridEvent& )
{
   //UpdateSizeLimits(); 
}

void PanelGenericProperty::UpdateSizeLimits()
{
	if(wxInterfaceManager::GetManager())
	{
		wxInterfaceManager::GetManager()->SetChildSize(
			this, 
			IFM_USE_CURRENT_SIZE, 
			IFM_USE_CURRENT_SIZE, 
			wxSize(IFM_USE_CURRENT_VALUE, pg->GetBottomY())
			);
	}
}

void PanelGenericProperty::OnFocus(wxFocusEvent &event)
{
	if(pgManager) pgManager->SetFocus();
	else if(pg) pg->SetFocus();
}

void PanelGenericProperty::OnPropertyGridHighlight( wxPropertyGridEvent& event )
{

}

void PanelGenericProperty::OnPropertyGridSelected ( wxPropertyGridEvent& event )
{
    
}

void PanelGenericProperty::ResetState()
{
	pg->SetSplitterPosition(initialSplitterPos);
}

void PanelGenericProperty::LoadState(wxDataInputStream& stream)
{
	wxString propertyName;
	int pos = 0;

	pg->CollapseAll();

	//Load splitter position	
	stream >> pos;
	pg->SetSplitterPosition(pos);

	//Load scroll position
	stream >> pos;
	pg->SetScrollPos(wxVERTICAL, pos, false);
	

	do
	{
		stream >> propertyName;
		pg->Expand(propertyName);

	} while(propertyName != wxT("end of prop"));
}

void PanelGenericProperty::SaveState(wxDataOutputStream& stream)
{
	if(pg)
	{
		//Save splitter position
		stream << pg->GetSplitterPosition();

		//Save scroll position
		stream << pg->GetScrollPos(wxVERTICAL);

		wxPGId first_child = pg->GetFirstChild(pg->GetRoot());
		if ( first_child.IsOk() )
			SavePropertyState(first_child, stream);
	}

	//Save end
	stream << wxT("end of prop");	
}

void PanelGenericProperty::SavePropertyState(wxPGId id, wxDataOutputStream& stream)
{
    while ( id.IsOk() )
    {
		bool bExpanded = wxPropertyContainerMethods::IsPropertyExpanded(id);
        
		if(bExpanded)
		{
			stream << id.GetProperty().GetName();
		}

        // Write children, if any
        wxPGId first_child = pg->GetFirstChild(id);
        if ( first_child.IsOk() )
        {
            SavePropertyState(first_child, stream);
        }

        id = ((wxPropertyContainerMethods *)pg)->GetNextSibling(id);
    }
}