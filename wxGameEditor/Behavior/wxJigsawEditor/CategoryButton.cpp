/////////////////////////////////////////////////////////////////////////////
// Name:        CategoryButton.cpp
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     30/11/2008 15:12:20
// RCS-ID:      
// Copyright:   Volodymir (T-Rex) Tryapichko, 2008
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "CategoryButton.h"

////@begin XPM images
////@end XPM images


/*!
 * CategoryButton type definition
 */

IMPLEMENT_DYNAMIC_CLASS( CategoryButton, wxGradientButton )


/*!
 * CategoryButton event table definition
 */

BEGIN_EVENT_TABLE( CategoryButton, wxGradientButton )

////@begin CategoryButton event table entries
////@end CategoryButton event table entries

END_EVENT_TABLE()


/*!
 * CategoryButton constructors
 */

CategoryButton::CategoryButton()
{
    Init();
}

CategoryButton::CategoryButton(wxWindow* parent, wxWindowID id, const wxString& label, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator)
{
    Init();
    Create(parent, id, label, pos, size, style, validator);
}


/*!
 * CategoryButton creator
 */

bool CategoryButton::Create(wxWindow* parent, wxWindowID id, const wxString& label, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator)
{
////@begin CategoryButton creation
    wxGradientButton::Create(parent, id, label, pos, size, style);
    CreateControls();
////@end CategoryButton creation
    return true;
}


/*!
 * CategoryButton destructor
 */

CategoryButton::~CategoryButton()
{
////@begin CategoryButton destruction
////@end CategoryButton destruction
}


/*!
 * Member initialisation
 */

void CategoryButton::Init()
{
////@begin CategoryButton member initialisation
    m_Shapes = NULL;
////@end CategoryButton member initialisation
}


/*!
 * Control creation for CategoryButton
 */

void CategoryButton::CreateControls()
{    
////@begin CategoryButton content construction
////@end CategoryButton content construction
}

