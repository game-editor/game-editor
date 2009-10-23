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
// Name:        CategoryList.h
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     30/11/2008 15:16:31
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _CATEGORYLIST_H_
#define _CATEGORYLIST_H_


/*!
 * Includes
 */

#include "wxJigsawPalette.h"

////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class CategoryList;
////@end forward declarations
class wxJigsawShapeList;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_CATEGORYLIST_STYLE wxSIMPLE_BORDER
#define SYMBOL_CATEGORYLIST_IDNAME ID_CATEGORYLIST
#define SYMBOL_CATEGORYLIST_SIZE wxSize(100, 100)
#define SYMBOL_CATEGORYLIST_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * CategoryList class declaration
 */

class CategoryList: public wxWindow
{    
    DECLARE_DYNAMIC_CLASS( CategoryList )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    CategoryList();
    CategoryList(wxWindow* parent, wxWindowID id = CategoryList::ID_CATEGORYLIST, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(100, 100), long style = wxSIMPLE_BORDER);

    /// Creation
    bool Create(wxWindow* parent, wxWindowID id = CategoryList::ID_CATEGORYLIST, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(100, 100), long style = wxSIMPLE_BORDER);

    /// Destructor
    ~CategoryList();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin CategoryList event handler declarations

    /// wxEVT_SIZE event handler for ID_CATEGORYLIST
    void OnSize( wxSizeEvent& event );

////@end CategoryList event handler declarations
	void OnCategoryButtonPressed(wxCommandEvent & event);

////@begin CategoryList member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end CategoryList member function declarations
	int ResizeButtons();
	void ReCreateScrollBars();
	void DestroyChildren();
	void AddCategory(wxJigsawPalette *palette);
	size_t GetCategoryCount();

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin CategoryList member variables
    /// Control identifiers
    enum {
        ID_CATEGORYLIST = 10009
    };
////@end CategoryList member variables
};

#endif
    // _CATEGORYLIST_H_
