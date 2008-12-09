/////////////////////////////////////////////////////////////////////////////
// Name:        propertygridsample.h
// Purpose:     wxPropertyGrid sample
// Author:      Jaakko Salli
// Modified by:
// Created:     Sep-25-2004
// RCS-ID:      $Id:
// Copyright:   (c) Jaakko Salli
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef _PROPGRIDSAMPLE_H_
#define _PROPGRIDSAMPLE_H_

// -----------------------------------------------------------------------
// Declare custom user properties.
// -----------------------------------------------------------------------

WX_PG_DECLARE_CUSTOM_FLAGS_PROPERTY(wxTestCustomFlagsProperty)

WX_PG_DECLARE_CUSTOM_ENUM_PROPERTY(wxTestCustomEnumProperty)

// -----------------------------------------------------------------------

class wxAdvImageFileProperty : public wxFileProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxAdvImageFileProperty)
public:

    wxAdvImageFileProperty( const wxString& label = wxPG_LABEL,
                            const wxString& name = wxPG_LABEL,
                            const wxString& value = wxEmptyString );
    virtual ~wxAdvImageFileProperty ();

    virtual void OnSetValue();  // Override to allow image loading.

    WX_PG_DECLARE_CHOICE_METHODS()
    WX_PG_DECLARE_EVENT_METHODS()
    WX_PG_DECLARE_CUSTOM_PAINT_METHODS()

    void LoadThumbnails( size_t n );

protected:
    wxImage*    m_pImage; // Temporary thumbnail data.

    static wxPGChoices ms_choices;

    int m_index; // Index required for choice behaviour.
};

// -----------------------------------------------------------------------

class wxVector3f
{
public:
    wxVector3f()
    {
        x = y = z = 0.0;
    }
    wxVector3f( double x, double y, double z )
    {
        x = x; y = y; z = z;
    }

    double x, y, z;
};

inline bool operator == (const wxVector3f& a, const wxVector3f& b)
{
    return (a.x == b.x && a.y == b.y && a.z == b.z);
}

WX_PG_DECLARE_VARIANT_DATA(wxVector3fVariantData, wxVector3f, wxPG_NO_DECL)

class wxVectorProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxVectorProperty)
public:

    wxVectorProperty( const wxString& label = wxPG_LABEL,
                      const wxString& name = wxPG_LABEL,
                      const wxVector3f& value = wxVector3f() );
    virtual ~wxVectorProperty();

    WX_PG_DECLARE_PARENTAL_METHODS()

protected:
};

// -----------------------------------------------------------------------

class wxTriangle
{
public:
    wxVector3f a, b, c;
};

inline bool operator == (const wxTriangle& a, const wxTriangle& b)
{
    return (a.a == b.a && a.b == b.b && a.c == b.c);
}

WX_PG_DECLARE_VARIANT_DATA(wxTriangleVariantData, wxTriangle, wxPG_NO_DECL)

class wxTriangleProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxTriangleProperty)
public:

    wxTriangleProperty( const wxString& label = wxPG_LABEL,
                        const wxString& name = wxPG_LABEL,
                        const wxTriangle& value = wxTriangle() );
    virtual ~wxTriangleProperty();

    WX_PG_DECLARE_PARENTAL_METHODS()

protected:
};

// -----------------------------------------------------------------------

#if wxPG_INCLUDE_MANAGER
    #define SIMPLE_DEMO                     0
#else
    #define SIMPLE_DEMO                     1
#endif

#if !SIMPLE_DEMO
    // Change this to 0 to see it without the wxPropertyGridManager stuff.
    #define TESTING_WXPROPERTYGRIDADV       1
#else
    #define TESTING_WXPROPERTYGRIDADV       0
#endif

// -----------------------------------------------------------------------

// Note that outside Msw we use radiobox instead of tabctrl
#if defined(__WXMSW__) && wxMINOR_VERSION < 7
# define wxPGS_USE_TABCTRL
#endif

#ifdef wxPGS_USE_TABCTRL
# include <wx/tabctrl.h>
#else
# include <wx/radiobox.h>
#endif

//#undef wxPGS_USE_TABCTRL

// -----------------------------------------------------------------------

enum
{
    ID_COLOURSCHEME4 = 100
};

// -----------------------------------------------------------------------

class FormMain : public wxFrame
{
public:
    FormMain(const wxString& title, const wxPoint& pos, const wxSize& size );
	~FormMain();

#ifdef wxPGS_USE_TABCTRL
    wxTabCtrl*      m_tabctrl;
#else
    wxRadioBox*     m_radiobox;
#endif

#if TESTING_WXPROPERTYGRIDADV
    wxPropertyGridManager*  m_pPropGridMan;
#else
    #define m_pPropGridMan m_propGrid
#endif
    wxPropertyGrid*     m_propGrid;

    wxTextCtrl*     m_tcPropLabel;
    wxWindow*       m_panel;

    wxPGChoices     m_combinedFlags;

    wxMenuItem*     m_itemCatColours;
    wxMenuItem*     m_itemFreeze;
    wxMenuItem*     m_itemEnable;

    wxVariant       m_storedValues;

    void CreateGrid( int style, int extraStyle );

    void PopulateWithStandardItems();
    void PopulateWithExamples();
    void PopulateWithLibraryConfig();

    void OnCloseClick( wxCommandEvent& event );
    void OnLabelTextChange( wxCommandEvent& event );

    void OnColourScheme( wxCommandEvent& event );

    void OnInsertPropClick( wxCommandEvent& event );
    void OnAppendPropClick( wxCommandEvent& event );
    void OnClearClick( wxCommandEvent& event );
    void OnAppendCatClick( wxCommandEvent& event );
    void OnInsertCatClick( wxCommandEvent& event );
    void OnDelPropClick( wxCommandEvent& event );
    void OnDelPropRClick( wxCommandEvent& event );

    void OnContextMenu( wxContextMenuEvent& event );

    void OnEnableDisable( wxCommandEvent& event );
    void OnHideShow( wxCommandEvent& event );
    void OnClearModifyStatusClick( wxCommandEvent& event );
    void OnFreezeClick( wxCommandEvent& event );
    void OnDumpList( wxCommandEvent& event );
    void OnCatColours( wxCommandEvent& event );
    void OnSetColumns( wxCommandEvent& event );
    void OnMisc( wxCommandEvent& event );
    void OnPopulateClick( wxCommandEvent& event );
    void OnSetSpinCtrlEditorClick( wxCommandEvent& event );
    void OnTestReplaceClick( wxCommandEvent& event );
    void OnTestXRC( wxCommandEvent& event );
    void OnEnableCommonValues( wxCommandEvent& event );
    void OnSelectStyle( wxCommandEvent& event );

    void OnChangeFlagsPropItemsClick( wxCommandEvent& event );

    void OnSaveToFileClick( wxCommandEvent& event );
    void OnLoadFromFileClick( wxCommandEvent& event );

    void OnSetPropertyValue( wxCommandEvent& event );
    void OnInsertChoice( wxCommandEvent& event );
    void OnDeleteChoice( wxCommandEvent& event );
    void OnInsertPage( wxCommandEvent& event );
    void OnRemovePage( wxCommandEvent& event );

#ifdef wxPGS_USE_TABCTRL
    void OnTabChange( wxTabEvent& event );
#else
    void OnRadioBoxChange( wxCommandEvent& event );
#endif

    void OnIterate1Click( wxCommandEvent& event );
    void OnIterate2Click( wxCommandEvent& event );
    void OnIterate3Click( wxCommandEvent& event );
    void OnIterate4Click( wxCommandEvent& event );

    void OnPropertyGridChange( wxPropertyGridEvent& event );
    void OnPropertyGridChanging( wxPropertyGridEvent& event );
    void OnPropertyGridSelect( wxPropertyGridEvent& event );
    void OnPropertyGridHighlight( wxPropertyGridEvent& event );
    void OnPropertyGridItemRightClick( wxPropertyGridEvent& event );
    void OnPropertyGridItemDoubleClick( wxPropertyGridEvent& event );
    void OnPropertyGridPageChange( wxPropertyGridEvent& event );
    void OnPropertyGridButtonClick( wxCommandEvent& event );
    void OnPropertyGridTextUpdate( wxCommandEvent& event );
    void OnPropertyGridKeyEvent( wxKeyEvent& event );
    void OnPropertyGridItemCollapse( wxPropertyGridEvent& event );
    void OnPropertyGridItemExpand( wxPropertyGridEvent& event );

    void OnAbout( wxCommandEvent& event );

    void OnMove( wxMoveEvent& event );
    void OnResize( wxSizeEvent& event );
    void OnPaint( wxPaintEvent& event );
    void OnCloseEvent( wxCloseEvent& event );

    void OnIdle( wxIdleEvent& event );

    void RunTests();

private:
    DECLARE_EVENT_TABLE()
};

// -----------------------------------------------------------------------

class cxApplication : public wxApp
{
public:

    virtual bool OnInit();

private:
    FormMain    *Form1;
};

DECLARE_APP(cxApplication)

// -----------------------------------------------------------------------

#endif // _PROPGRIDSAMPLE_H_
