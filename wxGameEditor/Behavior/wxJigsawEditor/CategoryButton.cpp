/////////////////////////////////////////////////////////////////////////////
// Name:        CategoryButton.cpp
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     30/11/2008 15:12:20
// RCS-ID:      
// Copyright:   
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
#include "../../color.h"

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


void CategoryButton::SetColor(const wxColour &color) 
{ 
	unsigned char newColor[3], colorA[3], colorB[3];

	colorA[0] = color.Red();
	colorA[1] = color.Green();
	colorA[2] = color.Blue();

	COLOUR_TO_ARRAY(m_GradientTopStartColour, colorB)
	ColorBlend_Overlay(newColor, colorA, colorB);
	m_GradientTopStartColour = wxColour(newColor[0], newColor[1], newColor[2]);

	COLOUR_TO_ARRAY(m_GradientBottomStartColour, colorB)
	ColorBlend_Overlay(newColor, colorA, colorB);
	m_GradientBottomStartColour = wxColour(newColor[0], newColor[1], newColor[2]);

	COLOUR_TO_ARRAY(m_GradientBottomEndColour, colorB)
	ColorBlend_Overlay(newColor, colorA, colorB);
	m_GradientBottomEndColour = wxColour(newColor[0], newColor[1], newColor[2]);

	COLOUR_TO_ARRAY(m_PressedColourTop, colorB)
	ColorBlend_Overlay(newColor, colorA, colorB);
	m_PressedColourTop = wxColour(newColor[0], newColor[1], newColor[2]);

	COLOUR_TO_ARRAY(m_PressedColourBottom, colorB)
	ColorBlend_Overlay(newColor, colorA, colorB);
	m_PressedColourBottom = wxColour(newColor[0], newColor[1], newColor[2]);
}