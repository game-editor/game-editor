/////////////////////////////////////////////////////////////////////////////
// Name:        CategoryList.h
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     30/11/2008 15:16:31
// RCS-ID:      
// Copyright:   Volodymir (T-Rex) Tryapichko, 2008
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _CATEGORYLIST_H_
#define _CATEGORYLIST_H_


/*!
 * Includes
 */

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
	void ResizeButtons();
	void ReCreateScrollBars();
	void DestroyChildren();
	void AddCategory(const wxString & name, wxJigsawShapeList * shapes);
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
