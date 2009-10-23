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


/////////////////////////////////////////////////////////////////////////////
// Name:        CategoryButton.h
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     30/11/2008 15:12:20
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _CATEGORYBUTTON_H_
#define _CATEGORYBUTTON_H_


/*!
 * Includes
 */

////@begin includes
////@end includes
#include "wxGradientButton.h"
#include <wxJigsawShape.h>

#define COLOUR_TO_ARRAY(A,B)\
	B[0] = A.Red();\
	B[1] = A.Green();\
	B[2] = A.Blue();\

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_CATEGORYBUTTON_STYLE 0
#define SYMBOL_CATEGORYBUTTON_IDNAME ID_CATEGORYBUTTON
#define SYMBOL_CATEGORYBUTTON_SIZE wxDefaultSize
#define SYMBOL_CATEGORYBUTTON_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * CategoryButton class declaration
 */

class CategoryButton: public wxGradientButton
{    
    DECLARE_DYNAMIC_CLASS( CategoryButton )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    CategoryButton();
    CategoryButton(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& label = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, const wxValidator& validator = wxDefaultValidator);

    /// Creation
    bool Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& label = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, const wxValidator& validator = wxDefaultValidator);

    /// Destructor
    ~CategoryButton();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin CategoryButton event handler declarations

////@end CategoryButton event handler declarations

////@begin CategoryButton member function declarations

    wxJigsawShapeList * GetShapes() const { return m_Shapes ; }
    void SetShapes(wxJigsawShapeList * value) { m_Shapes = value ; }
	void SetColor(const wxColour &color);

////@end CategoryButton member function declarations

////@begin CategoryButton member variables
    wxJigsawShapeList * m_Shapes;
    /// Control identifiers
    enum {
        ID_CATEGORYBUTTON = 10008
    };
////@end CategoryButton member variables
};

#endif
    // _CATEGORYBUTTON_H_
