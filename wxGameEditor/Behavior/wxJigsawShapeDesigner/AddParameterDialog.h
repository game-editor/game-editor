/////////////////////////////////////////////////////////////////////////////
// Name:        AddParameterDialog.h
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     26/03/2008 15:05:20
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _ADDPARAMETERDIALOG_H_
#define _ADDPARAMETERDIALOG_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/valgen.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_ADDPARAMETERDIALOG_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_ADDPARAMETERDIALOG_TITLE _("Add Parameter")
#define SYMBOL_ADDPARAMETERDIALOG_IDNAME ID_ADDPARAMETERDIALOG
#define SYMBOL_ADDPARAMETERDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_ADDPARAMETERDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * AddParameterDialog class declaration
 */

class AddParameterDialog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( AddParameterDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    AddParameterDialog();
    AddParameterDialog( wxWindow* parent, wxWindowID id = SYMBOL_ADDPARAMETERDIALOG_IDNAME, const wxString& caption = SYMBOL_ADDPARAMETERDIALOG_TITLE, const wxPoint& pos = SYMBOL_ADDPARAMETERDIALOG_POSITION, const wxSize& size = SYMBOL_ADDPARAMETERDIALOG_SIZE, long style = SYMBOL_ADDPARAMETERDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ADDPARAMETERDIALOG_IDNAME, const wxString& caption = SYMBOL_ADDPARAMETERDIALOG_TITLE, const wxPoint& pos = SYMBOL_ADDPARAMETERDIALOG_POSITION, const wxSize& size = SYMBOL_ADDPARAMETERDIALOG_SIZE, long style = SYMBOL_ADDPARAMETERDIALOG_STYLE );

    /// Destructor
    ~AddParameterDialog();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin AddParameterDialog event handler declarations

    /// wxEVT_UPDATE_UI event handler for wxID_OK
    void OnOKUpdate( wxUpdateUIEvent& event );

////@end AddParameterDialog event handler declarations

////@begin AddParameterDialog member function declarations

    int GetParamType() const { return m_ParamType ; }
    void SetParamType(int value) { m_ParamType = value ; }

    wxString GetParamLabel() const { return m_ParamLabel ; }
    void SetParamLabel(wxString value) { m_ParamLabel = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end AddParameterDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin AddParameterDialog member variables
    wxChoice* m_ParamTypeChoice;
    int m_ParamType;
    wxString m_ParamLabel;
    /// Control identifiers
    enum {
        ID_ADDPARAMETERDIALOG = 10008,
        ID_PARAM_TYPE_CHOICE = 10009,
        ID_PARAM_LABEL_TEXTCTRL = 10010
    };
////@end AddParameterDialog member variables
};

#endif
    // _ADDPARAMETERDIALOG_H_
