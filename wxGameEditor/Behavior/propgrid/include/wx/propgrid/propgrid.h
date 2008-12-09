/////////////////////////////////////////////////////////////////////////////
// Name:        propgrid.h
// Purpose:     wxPropertyGrid
// Author:      Jaakko Salli
// Modified by:
// Created:     Sep-25-2004
// RCS-ID:      $Id:
// Copyright:   (c) Jaakko Salli
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_PROPGRID_PROPGRID_H__
#define __WX_PROPGRID_PROPGRID_H__

#include <wx/dynarray.h>
#include <wx/hashmap.h>
#include <wx/variant.h>
#include <wx/longlong.h>
#include <wx/dcclient.h>
#include <wx/scrolwin.h>
#include <wx/tooltip.h>
#include <wx/datetime.h>

// NB: Do *NOT * remove this.
#if defined(SWIG) || defined(SWIG_TYPE_TABLE)
    #ifndef __WXPYTHON__
        #define __WXPYTHON__
    #endif
#endif

#if defined(SWIG)
    #define wxDEPRECATED(A) A
#endif

//
// In case DOXYGEN was not specified (should be, but just in case)
#if !defined(DOXYGEN) && !defined(_WX_WINDOW_H_BASE_) && !defined(SWIG)
    #define DOXYGEN
#endif


// Doxygen special includes
#ifdef DOXYGEN
    #include "pg_dox_mainpage.h"
    #include "propdev.h"
#endif


//
// Need to define some things for DOXYGEN
#ifdef DOXYGEN
    #define wxUSE_VALIDATORS        1
    #define wxUSE_DATETIME          1
    #define wxUSE_TOOLTIPS          1
    #define wxUSE_SPINBTN           1
    #define wxUSE_DATEPICKCTRL      1
#endif


//
// Check that some wxUSE_FOOs exist
// (we don't care if they are 1 or 0, just that they exist)
#ifndef wxUSE_VALIDATORS
    #error "wxUSE_VALIDATORS not defined"
#endif

#ifndef wxUSE_DATETIME
    #error "wxUSE_DATETIME not defined"
#endif

#ifndef wxUSE_DATEPICKCTRL
    #error "wxUSE_DATEPICKCTRL not defined"
#endif


#if defined(__WXPYTHON__)
    #include <Python.h>
#endif

// Defines for component version.
// The following symbols should be updated for each new component release
// since some kind of tests, like those of AM_WXCODE_CHECKFOR_COMPONENT_VERSION()
// for "configure" scripts under unix, use them.
#define wxPROPGRID_MAJOR          1
#define wxPROPGRID_MINOR          3
#define wxPROPGRID_RELEASE        0

// For non-Unix systems (i.e. when building without a configure script),
// users of this component can use the following macro to check if the
// current version is at least major.minor.release
#define wxCHECK_PROPGRID_VERSION(major,minor,release) \
    (wxPROPGRID_MAJOR > (major) || \
    (wxPROPGRID_MAJOR == (major) && wxPROPGRID_MINOR > (minor)) || \
    (wxPROPGRID_MAJOR == (major) && wxPROPGRID_MINOR == (minor) && wxPROPGRID_RELEASE >= (release)))


// -----------------------------------------------------------------------


//
// Here are some platform dependent defines
// NOTE: More in propertygrid.cpp
//

#ifndef SWIG

#if defined(__WXMSW__)
    // tested

    #define wxPG_XBEFORETEXT            4 // space between vertical line and value text
    #define wxPG_XBEFOREWIDGET          1 // space between vertical line and value editor control

    #define wxPG_TEXTCTRLXADJUST        3 // x position adjustment for wxTextCtrl (and like)

    #define wxPG_ICON_WIDTH             9 // comment to use bitmap buttons
    #define wxPG_USE_RENDERER_NATIVE    0 // 1 if wxRendererNative should be employed

    #define wxPG_DOUBLE_BUFFER          1 // 1 to use double-buffer that guarantees flicker-free painting

    #define wxPG_HEAVY_GFX              1 // 1 for prettier appearance

    #define wxPG_SUPPORT_TOOLTIPS       1  // Enable tooltips

#elif defined(__WXGTK__)
    // tested

    #define wxPG_XBEFORETEXT            5 // space between vertical line and value text
    #define wxPG_XBEFOREWIDGET          1 // space between vertical line and value editor control

    #define wxPG_TEXTCTRLXADJUST        3 // x position adjustment for wxTextCtrl (and like)

    #define wxPG_ICON_WIDTH             9 // comment to use bitmap buttons
    #define wxPG_USE_RENDERER_NATIVE    0 // 1 if wxRendererNative should be employed

    #define wxPG_DOUBLE_BUFFER          1 // 1 to use double-buffer that guarantees flicker-free painting

    #define wxPG_HEAVY_GFX              1 // 1 for prettier appearance

    #define wxPG_SUPPORT_TOOLTIPS       1  // Enable tooltips

#elif defined(__WXMAC__)
    // *not* tested

    #define wxPG_XBEFORETEXT            4 // space between vertical line and value text
    #define wxPG_XBEFOREWIDGET          1 // space between vertical line and value editor widget

    #define wxPG_TEXTCTRLXADJUST        3 // x position adjustment for wxTextCtrl (and like)

    #define wxPG_ICON_WIDTH             11  // comment to use bitmap buttons
    #define wxPG_USE_RENDERER_NATIVE    1 // 1 if wxRendererNative should be employed

    #define wxPG_DOUBLE_BUFFER          0 // 1 to use double-buffer that guarantees flicker-free painting

    #define wxPG_HEAVY_GFX              1 // 1 for prettier appearance

    #define wxPG_SUPPORT_TOOLTIPS       1  // Enable tooltips

#else
    // defaults
    // tested on: none.

    #define wxPG_XBEFORETEXT            5 // space between vertical line and value text
    #define wxPG_XBEFOREWIDGET          1 // space between vertical line and value editor widget

    #define wxPG_TEXTCTRLXADJUST        3 // x position adjustment for wxTextCtrl (and like)

    #define wxPG_ICON_WIDTH             9 // comment to use bitmap buttons
    #define wxPG_USE_RENDERER_NATIVE    0 // 1 if wxRendererNative should be employed

    #define wxPG_DOUBLE_BUFFER          0 // 1 to use double-buffer that guarantees flicker-free painting

    #define wxPG_HEAVY_GFX              1 // 1 for prettier appearance

    #define wxPG_SUPPORT_TOOLTIPS       0  // Enable tooltips

#endif // #if defined(__WXMSW__)


#define wxPG_CONTROL_MARGIN             0 // space between splitter and control

#define wxCC_CUSTOM_IMAGE_MARGIN1            4  // before image
#define wxCC_CUSTOM_IMAGE_MARGIN2            5  // after image

// Use this macro to generate standard custom image height from
#define wxPG_STD_CUST_IMAGE_HEIGHT(LINEHEIGHT)  (LINEHEIGHT-3)


#if defined(__WXWINCE__)
    #define wxPG_SMALL_SCREEN       1
    #undef wxPG_DOUBLE_BUFFER
    #define wxPG_DOUBLE_BUFFER      0
#else
    #define wxPG_SMALL_SCREEN       0
#endif

#endif // #ifndef SWIG

// Undefine wxPG_ICON_WIDTH to use supplied xpm bitmaps instead
// (for tree buttons)
//#undef wxPG_ICON_WIDTH

// Need to force disable tooltips?
#if !wxUSE_TOOLTIPS
    #undef wxPG_SUPPORT_TOOLTIPS
    #define wxPG_SUPPORT_TOOLTIPS       0
#endif

// Set 1 to include advanced properties (wxFontProperty, wxColourProperty, etc.)
#ifndef wxPG_INCLUDE_ADVPROPS
    #define wxPG_INCLUDE_ADVPROPS           1
#endif

// Set 1 include wxPropertyGridManager
#ifndef wxPG_INCLUDE_MANAGER
    #define wxPG_INCLUDE_MANAGER            1
#endif

// Set 1 to include checkbox editor class
#define wxPG_INCLUDE_CHECKBOX           1

// Set to 1 to use native STL constructs as much as possible
#ifndef wxPG_USE_STL
    #define wxPG_USE_STL    wxUSE_STL
#endif

#ifndef wxPG_USING_WXOWNERDRAWNCOMBOBOX
    #if wxCHECK_VERSION(2,8,0)
        #define wxPG_USING_WXOWNERDRAWNCOMBOBOX 1
    #else
        #define wxPG_USING_WXOWNERDRAWNCOMBOBOX 0
    #endif
#endif

#if wxPG_USING_WXOWNERDRAWNCOMBOBOX
    #define wxPGOwnerDrawnComboBox          wxOwnerDrawnComboBox
    #define wxPGCC_DCLICK_CYCLES            wxCC_SPECIAL_DCLICK
    #define wxPGCC_PAINTING_CONTROL         wxODCB_PAINTING_CONTROL
    #define wxPGCC_PAINTING_SELECTED        wxODCB_PAINTING_SELECTED
    #define wxPGCC_PROCESS_ENTER            wxTE_PROCESS_ENTER
    #define wxPGCC_ALT_KEYS                 0
#endif

// wxRect: Inside(<=2.7.0) or Contains(>2.7.0)?
#if !wxCHECK_VERSION(2,7,1)
    #define wxPGRectContains    Inside
#else
    #define wxPGRectContains    Contains
#endif

#if wxCHECK_VERSION(2,7,0)
    #define wxPGIndex           unsigned int
#else
    #define wxPGIndex           int
#endif

#ifdef __WXMSW__
  #if wxCHECK_VERSION(2,9,0)
    #define wxPG_GetHDCOf(DC) ((HDC)((const wxMSWDCImpl *)DC.GetImpl())->GetHDC())
  #else
    #define wxPG_GetHDCOf(DC) ((HDC)DC.GetHDC())
  #endif
#endif

// -----------------------------------------------------------------------

#ifndef wxPG_COMPATIBILITY_1_2_0
    #define wxPG_COMPATIBILITY_1_2_0    1
#endif


#if wxPG_COMPATIBILITY_1_2_0

    // Legacy wxPropertyGrid version macro
    #define wxPG_VERSION            ((wxPROPGRID_MAJOR*1000)+(wxPROPGRID_MINOR*100)+(wxPROPGRID_RELEASE*10))

#endif


#ifdef wxPG_COMPATIBILITY_1_0_0

    #define wxRECURSE               wxPG_RECURSE
    #define wxKEEP_STRUCTURE        wxPG_KEEP_STRUCTURE
    #define wxPGConstants           wxPGChoices
    #define wxPG_EX_CLASSIC_SPACING 0

    #define wxPGCtrlClass           wxWindow
    #define wxCCustomTextCtrl       wxTextCtrl
    #define wxCCustomComboBox       wxPGOwnerDrawnComboBox
    #define wxCCustomButton         wxButton

#endif // #ifdef wxPG_COMPATIBILITY_1_0_0

#ifdef __WXPYTHON__
    #define wxPG_VALUETYPE_IS_STRING    0  // 1
#else
    #define wxPG_VALUETYPE_IS_STRING    0
#endif

#ifndef SWIG
    #if !wxCHECK_VERSION(2,9,0)
        #if !defined(wxUniChar)
            #define wxUniChar   wxChar
        #endif
        //#define wxPGGetIterChar(str, i)  str[i]
        #define wxPGGetIterChar(str, i)  *i
    #else
        #define wxPGGetIterChar(str, i)  *i
    #endif
#endif

#if wxPG_COMPATIBILITY_1_2_0
    #define wxPGRegisterPropertyClass(A)
#endif

// -----------------------------------------------------------------------

//
// wxPython special considerations
//
// TODO: Using const yields some ugly function
//   names, so might as well make those GetPropertyValueAsXXX non-static
//   for regular C++ build as well.s
//

#ifndef __WXPYTHON__

// Some Strings are returned as const wxChar* in C++, and as wxString in wxPython
// (using just wxString for everything would've been better, but the current scheme
// is necessary for better backwards compatibility).
#define wxPG_CONST_WXCHAR_PTR       const wxChar*
#define wxPG_CONST_WXCHAR_DEFVAL    ((const wxChar*)NULL)
#define wxPG_TO_WXCHAR_PTR(A)       A

// In C++ we can stick with overloaded methods
#define SetPropertyValueLong        SetPropertyValue
#define SetPropertyValueDouble      SetPropertyValue
#define SetPropertyValueBool        SetPropertyValue
#define SetPropertyValueString      SetPropertyValue
#define SetPropertyValueWxObjectPtr SetPropertyValue
#define SetPropertyValuePoint       SetPropertyValue
#define SetPropertyValueSize        SetPropertyValue
#define SetPropertyValueArrint2     SetPropertyValue
#define SetPropertyValueArrstr2     SetPropertyValue
#define SetPropertyValueDatetime    SetPropertyValue
#define SetPropertyValueLongLong    SetPropertyValue
#define SetPropertyValueULongLong   SetPropertyValue

#define WX_PG_NOT_PURE_IN_WXPYTHON  =0

#else

// Some Strings are returned as const wxChar* in C++, and as wxString in wxPython
// (using just wxString for everything would've been better, but the current scheme
// is necessary for better backwards compatibility).
#define wxPG_CONST_WXCHAR_PTR       wxString
#define wxPG_CONST_WXCHAR_DEFVAL    wxEmptyString
#define wxPG_TO_WXCHAR_PTR(A)       A  //(A.c_str())

// Because SWIG has problems combining overloaded functions and
// Python object-to-wxXXX conversion, we need to use Python proxy
// functions for these value setters.
#define SetPropertyValueArrstr2     _SetPropertyValueArrstr
#define SetPropertyValueArrint2     _SetPropertyValueArrint

//
// wxPGVariantAndBool
//	
// Helper class that wraps wxVariant and bool. Need to use this class
// instead of writeback arguments in some virtual methods of custom
// property classes.
//
// WXDLLIMPEXP_PG
class wxPGVariantAndBool
{
public:

    wxPGVariantAndBool()
    {
        m_valueValid = false;
        m_result = false;
    }

    wxPGVariantAndBool( bool result, const wxVariant& variant )
    {
        m_valueValid = true;
        m_result = result;
        m_value = variant;
    }

    wxPGVariantAndBool( const wxVariant& variant )
    {
        m_valueValid = true;
        m_result = true;
        m_value = variant;
    }

    wxPGVariantAndBool( bool result )
    {
        Init(result);
    }

    ~wxPGVariantAndBool() { }

    void Init( bool result = false )
    {
        m_valueValid = false;
        m_result = result;
    }

    const wxVariant& GetValue() const
    {
        wxASSERT(m_valueValid);
        return m_value;
    }

public:
    wxVariant m_value;
    bool      m_valueValid;
    bool      m_result;
};

#define WX_PG_NOT_PURE_IN_WXPYTHON

#endif


// wxPG_CHECK_FOO_DBG - on Release and wxPython builds, show wxLogWarning instead
// (so that the program flow is not interrupted, but the message can still be seen).
#if !defined(__WXDEBUG__) || defined(__WXPYTHON__)
    #define wxPG_CHECK_RET_DBG(A,B) \
        if ( !(A) ) { wxLogWarning(B); return; }
    #define wxPG_CHECK_MSG_DBG(A,B,C) \
        if ( !(A) ) { wxLogWarning(C); return B; }
#else
    #define wxPG_CHECK_RET_DBG(A,B)     wxCHECK_RET(A,B)
    #define wxPG_CHECK_MSG_DBG(A,B,C)   wxCHECK_MSG(A,B,C)
#endif


#define wxPG_NO_DECL

#ifndef wxEMPTY_PARAMETER_VALUE
    #define wxEMPTY_PARAMETER_VALUE
#endif

// -----------------------------------------------------------------------

#ifdef WXMAKINGLIB_PROPGRID
    #define WXDLLIMPEXP_PG
#elif defined(WXMAKINGDLL_PROPGRID)
    #define WXDLLIMPEXP_PG WXEXPORT
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_PG WXIMPORT
#else // not making nor using DLL
    #define WXDLLIMPEXP_PG wxEMPTY_PARAMETER_VALUE
#endif

// -----------------------------------------------------------------------

#ifndef SWIG
class WXDLLIMPEXP_PG wxPGEditor;
class WXDLLIMPEXP_PG wxPGProperty;
class WXDLLIMPEXP_PG wxPropertyCategory;
class WXDLLIMPEXP_PG wxPGChoices;
class WXDLLIMPEXP_PG wxPropertyGridState;
class WXDLLIMPEXP_PG wxPGCell;
class WXDLLIMPEXP_PG wxPGChoiceEntry;
class WXDLLIMPEXP_PG wxPropertyContainerMethods;
class WXDLLIMPEXP_PG wxPropertyGrid;
class WXDLLIMPEXP_PG wxPropertyGridEvent;
class WXDLLIMPEXP_PG wxPropertyGridManager;
class WXDLLIMPEXP_PG wxPGOwnerDrawnComboBox;
class WXDLLIMPEXP_PG wxPGCustomComboControl;
class WXDLLIMPEXP_PG wxPGEditorDialogAdapter;

extern WXDLLIMPEXP_PG const wxChar *wxPropertyGridNameStr;

#endif // #ifndef SWIG


#ifdef __WXPYTHON__
    class wxPGPyEditor;
#endif // #ifndef __WXPYTHON__


/** \class wxPGPaintData
    \ingroup classes
    \brief Contains information relayed to property's OnCustomPaint.
*/
struct wxPGPaintData
{
    /** wxPropertyGrid. */
    const wxPropertyGrid*   m_parent;

    /** Normally -1, otherwise index to drop-down list item that has to be drawn. */
    int                     m_choiceItem;

    /** Set to drawn width in OnCustomPaint (optional). */
    int                     m_drawnWidth;

    /** In a measure item call, set this to the height of item at m_choiceItem index. */
    int                     m_drawnHeight;   
};


#ifndef SWIG

/** \class wxPGCellRenderer
    \ingroup classes
    \brief
    Base class for wxPropertyGrid cell renderers.
*/
class WXDLLIMPEXP_PG wxPGCellRenderer
{
public:

    wxPGCellRenderer( unsigned int refCount = 1 )
        : m_refCount(refCount) { }
    virtual ~wxPGCellRenderer() { }

    // Render flags
    enum
    {
        Selected        = 0x00010000,
        Control         = 0x00020000
    };

    virtual void Render( wxDC& dc,
                         const wxRect& rect,
                         const wxPropertyGrid* propertyGrid,
                         wxPGProperty* property,
                         int column,
                         int item,
                         int flags ) const = 0;

    /** Returns size of the image in front of the editable area.
        \remarks
        If property is NULL, then this call is for a custom value. In that case
        the item is index to wxPropertyGrid's custom values.
    */
    virtual wxSize GetImageSize( const wxPGProperty* property,
                                 int column,
                                 int item ) const;

    /** Paints property category selection rectangle.
    */
    virtual void DrawCaptionSelectionRect( wxDC& dc, int x, int y, int w, int h ) const;

    /** Utility to draw vertically centered text.
    */
    void DrawText( wxDC& dc,
                   const wxRect& rect,
                   int imageWidth,
                   const wxString& text ) const;

    /** Utility to draw editor's value, or vertically aligned text if editor is NULL.
    */
    void DrawEditorValue( wxDC& dc, const wxRect& rect,
                          int xOffset, const wxString& text,
                          wxPGProperty* property,
                          const wxPGEditor* editor ) const;

    /** Utility to render cell bitmap and set text colour plus bg brush colour.

        Returns image width that, for instance, can be passed to DrawText.
    */
    int PreDrawCell( wxDC& dc, const wxRect& rect, const wxPGCell& cell, int flags ) const;

    void IncRef()
    {
        m_refCount++;
    }

    void DecRef() 
    {
        m_refCount--;
        if ( !m_refCount )
            delete this;
    }
protected:

private:
    unsigned int    m_refCount;
};


/** \class wxPGCell
    \ingroup classes
    \brief
    Base class for simple wxPropertyGrid cell information.
*/
class WXDLLIMPEXP_PG wxPGCell
{
public:
    wxPGCell();
    wxPGCell( const wxString& text,
              const wxBitmap& bitmap = wxNullBitmap,
              const wxColour& fgCol = wxNullColour,
              const wxColour& bgCol = wxNullColour );

    virtual ~wxPGCell() { }

    void SetText( const wxString& text ) { m_text = text; }
    void SetBitmap( const wxBitmap& bitmap ) { m_bitmap = bitmap; }
    void SetFgCol( const wxColour& col ) { m_fgCol = col; }
    void SetBgCol( const wxColour& col ) { m_bgCol = col; }

    const wxString& GetText() const { return m_text; }
    const wxBitmap& GetBitmap() const { return m_bitmap; }
    const wxColour& GetFgCol() const { return m_fgCol; }
    const wxColour& GetBgCol() const { return m_bgCol; }

protected:
    wxString    m_text;
    wxBitmap    m_bitmap;
    wxColour    m_fgCol;
    wxColour    m_bgCol;
};


/** \class wxPGDefaultRenderer
    \ingroup classes
    \brief
    Default cell renderer, that can handles the common
    scenarios.
*/
class WXDLLIMPEXP_PG wxPGDefaultRenderer : public wxPGCellRenderer
{
public:
    virtual void Render( wxDC& dc,
                         const wxRect& rect,
                         const wxPropertyGrid* propertyGrid,
                         wxPGProperty* property,
                         int column,
                         int item,
                         int flags ) const;

    virtual wxSize GetImageSize( const wxPGProperty* property,
                                 int column,
                                 int item ) const;

protected:
};

#endif  // !SWIG

/** @defgroup miscellaneous wxPropertyGrid Miscellanous
    This section describes some miscellanous values, types and macros.
    @{
*/

#define wxPG_EMPTY_ARRAYINT     wxArrayInt()
#define wxPG_EMPTY_ARRAYSTRING  wxArrayString()

#if !defined(SWIG)
    #define wxPG_LABEL              (*((wxString*)NULL))  // Used to tell wxPGProperty to use label as name as well.
    #define wxPG_NULL_BITMAP        wxNullBitmap
    #define wxPG_COLOUR_BLACK       (*wxBLACK)
#else
    #define wxPG_LABEL              wxString_wxPG_LABEL
    #define wxPG_NULL_BITMAP        wxBitmap_NULL
    #define wxPG_COLOUR_BLACK       wxColour_BLACK
#endif // #ifndef SWIG


// Used to indicate wxPGChoices::Add etc that the value is actually not given
// by the caller.
#define wxPG_INVALID_VALUE      INT_MAX


/** Convert Red, Green and Blue to a single 32-bit value.
*/
#define wxPG_COLOUR(R,G,B) ((wxUint32)(R+(G<<8)+(B<<16)))


/** If property is supposed to have custom-painted image, then returning
    this in OnMeasureImage() will usually be enough.
*/
#define wxPG_DEFAULT_IMAGE_SIZE             wxSize(-1, -1)


/** This callback function is used by atleast wxCustomProperty
    to facilitiate easy custom action on button press.
    \param propGrid
    related wxPropertyGrid
    \param property
    related wxPGProperty
    \param ctrl
    If not NULL (for example, not selected), a wxWindow* or equivalent
    \param data
    Value depends on the context.
    \retval
    True if changed value of the property.
*/
typedef bool (*wxPropertyGridCallback)(wxPropertyGrid* propGrid,
                                       wxPGProperty* property,
                                       wxWindow* ctrl,
                                       int data);

/** This callback function is used by atleast wxCustomProperty
    to facilitiate drawing items in drop down list.

    Works very much like the old wxPGProperty::OnCustomPaint.
*/
typedef void (*wxPGPaintCallback)(wxPGProperty* property,
                                  wxDC& dc,
                                  const wxRect& rect,
                                  wxPGPaintData& paintdata);

/** Use this with wxPropertyGrid::IsPropertyKindOf. For example, as in
    \code
        pg->IsPropertyKindOf(WX_PG_CLASSINFO(wxStringProperty))
    \endcode
*/
#define WX_PG_CLASSINFO(NAME) (&NAME::ms_classInfo)

/** @}
*/

// -----------------------------------------------------------------------

/** @defgroup wndflags wxPropertyGrid Window Styles
    SetWindowStyleFlag method can be used to modify some of these at run-time.
    @{
*/
/** This will cause Sort() automatically after an item is added.
    When inserting a lot of items in this mode, it may make sense to
    use Freeze() before operations and Thaw() afterwards to increase
    performance.
*/
#define wxPG_AUTO_SORT              0x00000010

/** Categories are not initially shown (even if added).
    IMPORTANT NOTE: If you do not plan to use categories, then this
    style will waste resources.
    This flag can also be changed using wxPropertyGrid::EnableCategories method.
*/
#define wxPG_HIDE_CATEGORIES        0x00000020

/* This style combines non-categoric mode and automatic sorting.
*/
#define wxPG_ALPHABETIC_MODE        (wxPG_HIDE_CATEGORIES|wxPG_AUTO_SORT)

/** Modified values are shown in bold font. Changing this requires Refresh()
    to show changes.
*/
#define wxPG_BOLD_MODIFIED          0x00000040

/** When wxPropertyGrid is resized, splitter moves to the center. This
    behaviour stops once the user manually moves the splitter.
*/
#define wxPG_SPLITTER_AUTO_CENTER   0x00000080

/** Display tooltips for cell text that cannot be shown completely. If
    wxUSE_TOOLTIPS is 0, then this doesn't have any effect.
*/
#define wxPG_TOOLTIPS               0x00000100

/** Disables margin and hides all expand/collapse buttons that would appear
    outside the margin (for sub-properties). Toggling this style automatically
    expands all collapsed items.
*/
#define wxPG_HIDE_MARGIN            0x00000200

/** This style prevents user from moving the splitter.
*/
#define wxPG_STATIC_SPLITTER        0x00000400

/** Combination of other styles that make it impossible for user to modify
    the layout.
*/
#define wxPG_STATIC_LAYOUT          (wxPG_HIDE_MARGIN|wxPG_STATIC_SPLITTER)

/** Disables wxTextCtrl based editors for properties which
    can be edited in another way. Equals calling wxPropertyGrid::LimitPropertyEditing
    for all added properties.
*/
#define wxPG_LIMITED_EDITING        0x00000800

#ifdef DOXYGEN

/** wxTAB_TRAVERSAL allows using Tab/Shift-Tab to travel between properties
    in grid. Travelling forwards from last property will navigate to the
    next control, and backwards from first will navigate to the previous one.
*/
    #define wxTAB_TRAVERSAL         0x00080000

#endif

/** wxPropertyGridManager only: Show toolbar for mode and page selection. */
#define wxPG_TOOLBAR                0x00001000

/** wxPropertyGridManager only: Show adjustable text box showing description
    or help text, if available, for currently selected property.
*/
#define wxPG_DESCRIPTION            0x00002000

/**
    NOTE: wxPG_EX_xxx are extra window styles and must be set using SetExtraStyle()
    member function.

    Speeds up switching to wxPG_HIDE_CATEGORIES mode. Initially, if wxPG_HIDE_CATEGORIES
    is not defined, the non-categorized data storage is not activated, and switching
    the mode first time becomes somewhat slower. wxPG_EX_INIT_NOCAT activates the
    non-categorized data storage right away. IMPORTANT NOTE: If you do plan not
    switching to non-categoric mode, or if you don't plan to use categories at
    all, then using this style will result in waste of resources.

*/
#define wxPG_EX_INIT_NOCAT          0x00001000

/** Extended window style that sets wxPropertyGridManager toolbar to not
    use flat style.
*/
#define wxPG_EX_NO_FLAT_TOOLBAR     0x00002000

/** This extra style allows editing more similar to some Microsoft/Mono
    provided property sheet controls. Currently this includes (but more may be
    added later, incase I missed something):
    * Pressing ENTER in control, in addition to confirming changes, will
      unfocus it.
    * Pressing ESC doesn't cancel edit (but still unfocuses the editor).

    Note that ESC and ENTER events in editor controls are relayed to the
    wxPropertyGrid itself, so that they can be detected by the application.
*/
//#define wxPG_EX_ALTERNATE_KEYS      0x00004000

/** Shows alphabetic/categoric mode buttons from toolbar.
*/
#define wxPG_EX_MODE_BUTTONS        0x00008000

/** Show property help strings as tool tips instead as text on the status bar.
    You can set the help strings using SetPropertyHelpString member function.
*/
#define wxPG_EX_HELP_AS_TOOLTIPS    0x00010000

/** Prevent TAB from focusing to wxButtons. This behaviour was default
    in version 1.2.0 and earlier.
    NOTE! Tabbing to button doesn't work yet. Problem seems to be that on wxMSW
      atleast the button doesn't properly propagate key events (yes, I'm using
      wxWANTS_CHARS).
*/
//#define wxPG_EX_NO_TAB_TO_BUTTON    0x00020000

#if wxPG_COMPATIBILITY_1_2_0
/* No longer used, defined for compatibility.
*/
    #define wxPG_EX_GREY_LABEL_WHEN_DISABLED    0x00000000
#endif

/** Allows relying on native double-buffering.
*/
#define wxPG_EX_NATIVE_DOUBLE_BUFFERING     0x00080000

#if wxPG_COMPATIBILITY_1_2_0
/** Since all events are now automatically processed immediately, this
    flag is zero and only provided for backwards compatibility.
*/
    #define wxPG_EX_PROCESS_EVENTS_IMMEDIATELY  0x00000000
#endif

/** Set this style to let user have ability to set values of properties to
    unspecified state. Currently, this applies to following properties:
    - wxIntProperty, wxUIntProperty, and wxFloatProperty: Clear the
      text field.
*/
#define wxPG_EX_AUTO_UNSPECIFIED_VALUES     0x00200000

/** If this style is used, built-in attributes (such as wxPG_FLOAT_PRECISION and wxPG_STRING_PASSWORD)
    are not stored into property's attribute storage (thus they are not readable).

    Note that this option is global, and applies to all wxPG property containers.
*/
#define wxPG_EX_WRITEONLY_BUILTIN_ATTRIBUTES    0x00400000

/** With this tyle Validators on properties will work basicly work just they
    work in wxWidgets.
*/
#define wxPG_EX_TRADITIONAL_VALIDATORS          0x00800000

/** Combines various styles.
*/
#define wxPG_DEFAULT_STYLE	        (0)

/** Combines various styles.
*/
#define wxPGMAN_DEFAULT_STYLE	    (0)

/** @}
*/

/** Flags for wxPropertyGrid::GetPropertyValues and wxPropertyGridManager::GetPropertyValues. */
#define wxPG_KEEP_STRUCTURE               0x00000010

/** Flags for wxPropertyGrid::SetPropertyAttribute etc */
#define wxPG_RECURSE                      0x00000020

/** Include attributes for GetPropertyValues. */
#define wxPG_INC_ATTRIBUTES               0x00000040

/** Used when first starting recursion. */
#define wxPG_RECURSE_STARTS               0x00000080

/** Force value change. */
#define wxPG_FORCE                        0x00000100

// -----------------------------------------------------------------------

// Misc argument flags.
#define wxPG_FULL_VALUE             0x00000001 // Get/Store full value instead of displayed value.
#define wxPG_REPORT_ERROR           0x00000002
#define wxPG_PROPERTY_SPECIFIC      0x00000004
#define wxPG_EDITABLE_VALUE         0x00000008 // Get/Store editable value instead of displayed one
                                               // (should only be different in the case of common values)

// -----------------------------------------------------------------------


#define wxNullProperty  ((wxPGProperty*)NULL)
#define wxPGIdGen(PTR)  (PTR)
#define wxPGIdToPtr(ID) ((wxPGProperty*)ID)
#define wxPGIdIsOk(ID)  ( ID != ((wxPGProperty*)NULL) )

typedef wxPGProperty* wxPGId;

#ifndef SWIG

/** \class wxPGPropArgCls
    \ingroup classes
    \brief
    Most property grid functions have this type as their argument, as it can
    convey a property by either a pointer or name.
*/
class WXDLLIMPEXP_PG wxPGPropArgCls
{
public:
    wxPGPropArgCls() { }
    wxPGPropArgCls( const wxPGProperty* property )
    {
        m_ptr.property = (wxPGProperty*) property;
        m_isName = false;
    }
    wxPGPropArgCls( const wxString& str )
    {
        m_ptr.name = &str;
        m_isName = 1;
    }
    wxPGPropArgCls( const wxPGPropArgCls& id )
    {
        m_ptr = id.m_ptr;
        m_isName = id.m_isName;
    }
#ifdef __WXPYTHON__
    wxPGPropArgCls( wxString* str, bool WXUNUSED(deallocPtr) )
    {
        m_ptr.name = str;
        m_isName = 3;
    }
    ~wxPGPropArgCls()
    {
        if ( m_isName == 3 )
            delete m_ptr.name;
    }
#endif
    wxPGProperty* GetPtr() const { wxCHECK( !m_isName, NULL ); return m_ptr.property; }
    wxPGPropArgCls( const wxChar* str )
    {
        m_ptr.rawname = str;
        m_isName = 2;
    }
    /** This constructor is required for NULL. */
    wxPGPropArgCls( int )
    {
        m_ptr.property = (wxPGProperty*) NULL;
        m_isName = false;
    }
    wxPGProperty* GetPtr( wxPropertyContainerMethods* methods ) const;
    wxPGProperty* GetPtr( const wxPropertyContainerMethods* methods ) const
    {
        return GetPtr((wxPropertyContainerMethods*)methods);
    }
    wxPGProperty* GetPtr0() const { return m_ptr.property; }
    unsigned char HasName() const { return m_isName; }
    const wxString& GetName() const { return *m_ptr.name; }
private:
    union
    {
        wxPGProperty* property;
        const wxChar* rawname;
        const wxString* name;
    } m_ptr;
    unsigned char m_isName;
};

#else

class WXDLLIMPEXP_PG wxPGPropArgCls;

#endif

typedef const wxPGPropArgCls& wxPGPropArg;

// -----------------------------------------------------------------------

WXDLLIMPEXP_PG void wxPGTypeOperationFailed( const wxPGProperty* p, const wxChar* typestr, const wxChar* op );
WXDLLIMPEXP_PG void wxPGGetFailed( const wxPGProperty* p, const wxChar* typestr );

// -----------------------------------------------------------------------

/** @defgroup propflags wxPGProperty Flags
    @{
*/

// NOTE: Do not change order of these, and if you add
//   any, remember also to update gs_property_flag_to_string
//   in propgrid.cpp.

/** Indicates bold font.
*/
#define wxPG_PROP_MODIFIED          0x0001

/** Disables ('greyed' text and editor does not activate) property.
*/
#define wxPG_PROP_DISABLED          0x0002

/** Hider button will hide this property.
*/
#define wxPG_PROP_HIDDEN            0x0004

/** This property has custom paint image just in front of its value.
    If property only draws custom images into a popup list, then this
    flag should not be set.
*/
#define wxPG_PROP_CUSTOMIMAGE       0x0008

/** Do not create text based editor for this property (but button-triggered
    dialog and choice are ok).
*/
#define wxPG_PROP_NOEDITOR          0x0010

/** Property has its children collapsed.
*/
#define wxPG_PROP_COLLAPSED         0x0020

/** Indicates the bit useable by derived properties.
*/
#define wxPG_PROP_CLASS_SPECIFIC_1  0x0040

/** Indicates the bit useable by derived properties.
*/
#define wxPG_PROP_CLASS_SPECIFIC_2  0x0080

/** A common value cannot be selected by user.
*/
#define wxPG_PROP_DENY_COMMON_VALUES    0x0100

/** Switched via SetWasModified(). Temporary flag - only used when
    setting/changing property value.
*/
#define wxPG_PROP_WAS_MODIFIED          0x0200

/** If set, then child properties (if any) should be "invisible" to the
    application.
*/
#define wxPG_PROP_AGGREGATE             0x0400

/** If set, then child properties (if any) are copies and should not
    be deleted in dtor.
*/
#define wxPG_PROP_CHILDREN_ARE_COPIES   0x0800

/** Classifies this item as a non-category. Used for faster item type identification.
*/
#define wxPG_PROP_PROPERTY              0x1000

/** Classifies this item as a category. Used for faster item type identification.
*/
#define wxPG_PROP_CATEGORY              0x2000

/** Classifies this item as a property that has children, but is not aggregate
    (ie. children are not private).
*/
#define wxPG_PROP_MISC_PARENT           0x4000

/** Property is read-only. Editor is still created.
*/
#define wxPG_PROP_READONLY              0x8000

/** Topmost flag.
*/
#define wxPG_PROP_MAX                   wxPG_PROP_READONLY

/** Property with children *must* have one of these set, otherwise iterators
    will not work correctly. Code should automatically take care of this, however.
*/
#define wxPG_PROP_PARENTAL_FLAGS        (wxPG_PROP_AGGREGATE|wxPG_PROP_CATEGORY|wxPG_PROP_MISC_PARENT)

//
// NB: DO NOT USE FLAGS ABOVE 0x8000
//     (or if do, please see the iterator flags section for
//      why it might be dangerous)
//

/** @}
*/

// Amalgam of flags that should be inherited by sub-properties
#define wxPG_INHERITED_PROPFLAGS        (wxPG_PROP_HIDDEN|wxPG_PROP_NOEDITOR)

// Combination of flags that can be stored by GetFlagsAsString
#define wxPG_STRING_STORED_FLAGS        (wxPG_PROP_DISABLED|wxPG_PROP_HIDDEN|wxPG_PROP_NOEDITOR|wxPG_PROP_COLLAPSED|wxPG_PROP_DENY_COMMON_VALUES)

// -----------------------------------------------------------------------

/** @defgroup attrids wxPropertyGrid Property Attribute Identifiers
    wxPropertyGrid::SetPropertyAttribute accepts one of these as
    attrid argument when used with one of the built-in property classes.
    @{
*/

/** wxBoolProperty specific, int, default 0. When 1 sets bool property to
    use checkbox instead of choice.
*/
#define wxPG_BOOL_USE_CHECKBOX              wxT("UseCheckbox")

/** wxBoolProperty specific, int, default 0. When 1 sets bool property value
    to cycle on double click (instead of showing the popup listbox).
*/
#define wxPG_BOOL_USE_DOUBLE_CLICK_CYCLING  wxT("UseDClickCycling")

/** wxFloatProperty (and similar) specific, int, default -1. Sets the (max) precision
    used when floating point value is rendered as text. The default -1 means infinite
    precision.
*/
#define wxPG_FLOAT_PRECISION                wxT("Precision")

/** The text will be echoed as asterisks (wxTE_PASSWORD will be passed to textctrl etc).
*/
#define wxPG_STRING_PASSWORD                wxT("Password")

/** Define base used by a wxUIntProperty. Valid constants are
    wxPG_BASE_OCT, wxPG_BASE_DEC, wxPG_BASE_HEX and wxPG_BASE_HEXL
    (lowercase characters).
*/
#define wxPG_UINT_BASE                      wxT("Base")

/** Define prefix rendered to wxUIntProperty. Accepted constants
    wxPG_PREFIX_NONE, wxPG_PREFIX_0x, and wxPG_PREFIX_DOLLAR_SIGN.
    <b>Note:</b> Only wxPG_PREFIX_NONE works with Decimal and Octal
    numbers.
*/
#define wxPG_UINT_PREFIX                    wxT("Prefix")

/** wxFileProperty/wxImageFileProperty specific, wxChar*, default is detected/varies.
    Sets the wildcard used in the triggered wxFileDialog. Format is the
    same.
*/
#define wxPG_FILE_WILDCARD                  wxT("Wildcard")

/** wxFileProperty/wxImageFileProperty specific, int, default 1.
    When 0, only the file name is shown (i.e. drive and directory are hidden).
*/
#define wxPG_FILE_SHOW_FULL_PATH            wxT("ShowFullPath")

/** Specific to wxFileProperty and derived properties, wxString, default empty.
    If set, then the filename is shown relative to the given path string.
*/
#define wxPG_FILE_SHOW_RELATIVE_PATH        wxT("ShowRelativePath")

/** Specific to wxFileProperty and derived properties, wxString, default is empty.
    Sets the initial path of where to look for files.
*/
#define wxPG_FILE_INITIAL_PATH              wxT("InitialPath")

/** Specific to wxFileProperty and derivatives, wxString, default is empty.
    Sets a specific title for the dir dialog.
*/
#define wxPG_FILE_DIALOG_TITLE              wxT("DialogTitle")

/** Specific to wxDirProperty, wxString, default is empty.
    Sets a specific message for the dir dialog.
*/
#define wxPG_DIR_DIALOG_MESSAGE             wxT("DialogMessage")

/** Sets displayed date format for wxDateProperty.
*/
#define wxPG_DATE_FORMAT                    wxT("DateFormat")

/** Sets wxDatePickerCtrl window style used with wxDateProperty. Default
    is wxDP_DEFAULT | wxDP_SHOWCENTURY.
*/
#define wxPG_DATE_PICKER_STYLE              wxT("PickerStyle")


#ifdef wxPG_COMPATIBILITY_1_0_0

/** wxCustomProperty specific, wxPGEditor*. Set editor control. Editor pointer is stored
    in variable named wxPGEditor_EDITORNAME. So the basic built-in editors are at
    wxPGEditor_TextCtrl, wxPGEditor_Choice, wxPGEditor_ComboBox, wxPGEditor_CheckBox,
    wxPGEditor_TextCtrlAndButton, and wxPGEditor_ChoiceAndButton.
*/
#define wxPG_CUSTOM_EDITOR                  wxT("128")

/** wxCustomProperty specific, wxBitmap*. Sets a small bitmap. Value must be given as
    pointer and it is then copied. If you give it wxNullBitmap, then the current
    image (if any) is deleted.
*/
#define wxPG_CUSTOM_IMAGE                   wxT("129")

/** wxCustomProperty specific, void*. Sets callback function (of type wxPropertyGridCallback)
    that is called whenever button is pressed.
*/
#define wxPG_CUSTOM_CALLBACK                wxT("130")

#endif // wxPG_COMPATIBILITY_1_0_0

/** wxCustomProperty specific, void*. Sets callback function (of type wxPGPaintCallback)
    that is called whenever image in front of property needs to be repainted. This attribute
    takes precedence over bitmap set with wxPG_CUSTOM_IMAGE, and it is only proper way
    to draw images to wxCustomProperty's drop down choices list.
    \remarks
    Callback must handle measure calls (i.e. when rect.x < 0, set paintdata.m_drawnHeight to
    height of item in question).
*/
#define wxPG_CUSTOM_PAINT_CALLBACK          wxT("PaintCallback")


/** wxCustomProperty specific, int, default 0. Setting to 1 makes children private (ie. sets
    wxPG_PROP_AGGREGATE flag), similar to other properties with fixed children.
    \remarks
    - Children must be added <b>when this attribute has value 0</b>. Otherwise
      there will be an assertion failure.
    - Changed event occurs on the parent only.
*/
#define wxPG_CUSTOM_PRIVATE_CHILDREN        wxT("PrivateChildren")


/** wxColourProperty and its kind, int, default 1. Setting this attribute to 0 hides custom
    colour from property's list of choices.
*/
#define wxPG_COLOUR_ALLOW_CUSTOM            wxT("AllowCustom")


/** First attribute id that is guaranteed not to be used built-in
    properties.
*/
//#define wxPG_USER_ATTRIBUTE                 192

/** @}
*/

//
// Valid constants for wxPG_UINT_BASE attribute
// (long because of wxVariant constructor)
#define wxPG_BASE_OCT                       (long)8
#define wxPG_BASE_DEC                       (long)10
#define wxPG_BASE_HEX                       (long)16
#define wxPG_BASE_HEXL                      (long)32

//
// Valid constants for wxPG_UINT_PREFIX attribute
#define wxPG_PREFIX_NONE                    (long)0
#define wxPG_PREFIX_0x                      (long)1
#define wxPG_PREFIX_DOLLAR_SIGN             (long)2


// -----------------------------------------------------------------------
// Editor class.

// Editor accessor.
#define wxPG_EDITOR(T)          wxPGEditor_##T

// Declare editor class, with optional part.
#define WX_PG_DECLARE_EDITOR_WITH_DECL(EDITOR,DECL) \
extern DECL wxPGEditor* wxPGEditor_##EDITOR; \
extern DECL wxPGEditor* wxPGConstruct##EDITOR##EditorClass();

// Declare editor class.
#define WX_PG_DECLARE_EDITOR(EDITOR) \
extern wxPGEditor* wxPGEditor_##EDITOR; \
extern wxPGEditor* wxPGConstruct##EDITOR##EditorClass();

// Declare builtin editor classes.
WX_PG_DECLARE_EDITOR_WITH_DECL(TextCtrl,WXDLLIMPEXP_PG)
WX_PG_DECLARE_EDITOR_WITH_DECL(Choice,WXDLLIMPEXP_PG)
WX_PG_DECLARE_EDITOR_WITH_DECL(ComboBox,WXDLLIMPEXP_PG)
WX_PG_DECLARE_EDITOR_WITH_DECL(TextCtrlAndButton,WXDLLIMPEXP_PG)
#if wxPG_INCLUDE_CHECKBOX
WX_PG_DECLARE_EDITOR_WITH_DECL(CheckBox,WXDLLIMPEXP_PG)
#endif
WX_PG_DECLARE_EDITOR_WITH_DECL(ChoiceAndButton,WXDLLIMPEXP_PG)

// -----------------------------------------------------------------------


#ifdef SWIG
    #define wxVARIANT_REFCOUNTED 1
#endif

//
// For compability with wxWidgets 2.6 patched with refcounted wxVariant
#ifndef wxVARIANT_REFCOUNTED
  #if wxCHECK_VERSION(2,8,0)
    #define wxVARIANT_REFCOUNTED 1
  #else
    #define wxVARIANT_REFCOUNTED 0
    #error "Only supports wxWidgets 2.6 that has been patched with refcounted wxVariant"
  #endif
#endif


#if !defined(SWIG)

/** \class wxPGVariantData
	\ingroup classes
    \brief wxVariantData with additional functionality.

    It is usually enough to use supplied to macros to automatically generate
    variant data class. Like so:

    \code

    // In header
    WX_PG_DECLARE_VARIANT_DATA(wxPGVariantDataMyClass,  // Name of the class
                               MyClass,  // Name of the data type
                               wxEMPTY_PARAMETER_VALUE)  // Declaration

    // In source
    WX_PG_IMPLEMENT_VARIANT_DATA(wxPGVariantDataMyClass, MyClass)

    \endcode

    If your class is derived from wxObject, it is recommended that you use
    wxObject-versions of the macros (WX_PG_DECLARE_WXOBJECT_VARIANT_DATA and
    WX_PG_IMPLEMENT_WXOBJECT_VARIANT_DATA).

*/
class WXDLLIMPEXP_PG wxPGVariantData : public wxVariantData
{
#if !wxCHECK_VERSION(2,9,0)
    DECLARE_ABSTRACT_CLASS(wxPGVariantData)
#endif
public:
    virtual void* GetValuePtr() = 0;
    virtual wxVariant GetDefaultValue() const = 0;
protected:
};


//
// With wxWidgets 2.9 and later we demand native C++ RTTI support
#if wxCHECK_VERSION(2,9,0)
    #ifdef wxNO_RTTI
        #error "You need to enable compiler RTTI support when using wxWidgets 2.9.0 or later"
    #endif
    #define WX_PG_DECLARE_DYNAMIC_CLASS_VARIANTDATA(A)
    #define WX_PG_IMPLEMENT_DYNAMIC_CLASS_VARIANTDATA(A, B)
    typedef const std::type_info* wxPGVariantDataClassInfo;
    #define wxPGVariantDataGetClassInfo(A) (&typeid(*A))
    #define wxPG_VARIANT_EQ(A, B) ( typeid(*(A).GetData()) == typeid(*(B).GetData()) && (A == B) )
#else
    #define WX_PG_DECLARE_DYNAMIC_CLASS_VARIANTDATA DECLARE_DYNAMIC_CLASS
    #define WX_PG_IMPLEMENT_DYNAMIC_CLASS_VARIANTDATA IMPLEMENT_DYNAMIC_CLASS
    typedef wxList wxVariantList;
    typedef wxClassInfo* wxPGVariantDataClassInfo;
    #define wxPGVariantDataGetClassInfo(A) (A->GetClassInfo())
    #define wxPG_VARIANT_EQ(A, B) ( (A).GetData()->GetClassInfo() == (B).GetData()->GetClassInfo() && (A == B) )
#endif

#ifndef wxDynamicCastVariantData
    #define wxDynamicCastVariantData wxDynamicCast
#endif


#define _WX_PG_IMPLEMENT_VARIANT_DATA_CLASS(CLASSNAME, DATATYPE, AS_ARG, AS_ARG_CONST, CTOR_CODE, DEFAULT_VALUE, SET_CODE) \
    WX_PG_DECLARE_DYNAMIC_CLASS_VARIANTDATA(CLASSNAME) \
protected: \
    DATATYPE m_value; \
public: \
    CLASSNAME() { CTOR_CODE; } \
    CLASSNAME(AS_ARG_CONST value) { CTOR_CODE; SET_CODE; m_value = value; } \
    inline DATATYPE GetValue() const { return m_value; } \
    inline AS_ARG_CONST GetValueRef() const { return m_value; } \
    inline AS_ARG GetValueRef() { return m_value; } \
    inline void SetValue(AS_ARG_CONST value) { SET_CODE; m_value = value; } \
    virtual bool Eq(wxVariantData&) const { return false; } \
    virtual wxString GetType() const { return wxT(#DATATYPE); } \
    virtual wxVariantData* Clone() { return new CLASSNAME; } \
    virtual bool Read(wxString &) { return false; } \
    virtual bool Write(wxString &) const { return true; } \
    virtual wxVariant GetDefaultValue() const { return DEFAULT_VALUE; }

//
// Macro WXVARIANT allows creation of wxVariant from any type supported by wxWidgets internally,
// and of all types created using WX_PG_DECLARE_VARIANT_DATA.
template<class T>
wxVariant WXVARIANT( const T& value )
{
    return wxVariant((void*)&value);
}

template<> inline wxVariant WXVARIANT( const int& value ) { return wxVariant((long)value); }
template<> inline wxVariant WXVARIANT( const long& value ) { return wxVariant(value); }
template<> inline wxVariant WXVARIANT( const bool& value ) { return wxVariant(value); }
template<> inline wxVariant WXVARIANT( const double& value ) { return wxVariant(value); }
//template<> inline wxVariant WXVARIANT( const wxChar* value ) { return wxVariant(wxString(value)); }
//#if wxCHECK_VERSION(2,9,0)
//    template<> inline wxVariant WXVARIANT( const char* value ) { return wxVariant(wxString(value)); }
//#endif
template<> inline wxVariant WXVARIANT( const wxArrayString& value ) { return wxVariant(value); }
template<> inline wxVariant WXVARIANT( const wxString& value ) { return wxVariant(value); }
#if wxUSE_DATETIME
    template<> inline wxVariant WXVARIANT( const wxDateTime& value ) { return wxVariant(value); }
#endif

#if wxCHECK_VERSION(2,9,0)
    #define _WX_PG_VARIANT_DATA_CLASSINFO_CONTAINER_DECL(CLASSNAME) \
        extern int CLASSNAME##_d_;
    #define _WX_PG_VARIANT_DATA_CLASSINFO_CONTAINER(CLASSNAME) \
        int CLASSNAME##_d_;
#else
    #define _WX_PG_VARIANT_DATA_CLASSINFO_CONTAINER_DECL(CLASSNAME) \
        extern wxPGVariantDataClassInfo CLASSNAME##_ClassInfo
    #define _WX_PG_VARIANT_DATA_CLASSINFO_CONTAINER(CLASSNAME) \
        wxPGVariantDataClassInfo CLASSNAME##_ClassInfo = &CLASSNAME::ms_classInfo;
#endif

#define _WX_PG_IMPLEMENT_VARIANT_DATA(CLASSNAME, DATATYPE, AS_ARG, AS_CONST_ARG, NULLVAL, BASECLASS) \
_WX_PG_VARIANT_DATA_CLASSINFO_CONTAINER(CLASSNAME) \
WX_PG_IMPLEMENT_DYNAMIC_CLASS_VARIANTDATA(CLASSNAME, BASECLASS) \
AS_ARG operator <<( AS_ARG value, const wxVariant &variant ) \
{ \
	CLASSNAME *data = wxDynamicCastVariantData( variant.GetData(), CLASSNAME ); \
    wxASSERT( data ); \
    value = data->GetValue(); \
    return value; \
} \
wxVariant& operator <<( wxVariant &variant, AS_CONST_ARG value ) \
{ \
    CLASSNAME *data = new CLASSNAME( value ); \
    variant.SetData( data ); \
    return variant; \
} \
AS_ARG DATATYPE##FromVariant( const wxVariant& v ) \
{ \
	CLASSNAME *data = wxDynamicCastVariantData( v.GetData(), CLASSNAME ); \
    if ( !data ) \
        return NULLVAL; \
    return data->GetValueRef(); \
} \
wxVariant DATATYPE##ToVariant( AS_CONST_ARG value ) \
{ \
    wxVariant variant( new CLASSNAME( value ) ); \
    return variant; \
}

#define WX_PG_IMPLEMENT_VARIANT_DATA(CLASSNAME, DATATYPE) \
class CLASSNAME : public wxPGVariantData \
{ \
_WX_PG_IMPLEMENT_VARIANT_DATA_CLASS(CLASSNAME, DATATYPE, DATATYPE&, \
                                    const DATATYPE&, 1, \
                                    wxVariant(new CLASSNAME(DATATYPE())), 1) \
public: \
    virtual void* GetValuePtr() { return (void*)&m_value; } \
}; \
_WX_PG_IMPLEMENT_VARIANT_DATA(CLASSNAME, DATATYPE, DATATYPE&, const DATATYPE&, (DATATYPE&)*((DATATYPE*)NULL), wxPGVariantData)

#define WX_PG_IMPLEMENT_WXOBJECT_VARIANT_DATA(CLASSNAME, DATATYPE) \
class CLASSNAME : public wxPGVariantData \
{ \
_WX_PG_IMPLEMENT_VARIANT_DATA_CLASS(CLASSNAME, DATATYPE, DATATYPE&, \
                                    const DATATYPE&, 1, \
                                    wxVariant(new CLASSNAME(DATATYPE())), 1) \
public: \
    virtual void* GetValuePtr() { return (void*)&m_value; } \
    virtual wxClassInfo* GetValueClassInfo() { return m_value.GetClassInfo(); } \
}; \
_WX_PG_IMPLEMENT_VARIANT_DATA(CLASSNAME, DATATYPE, DATATYPE&, const DATATYPE&, (DATATYPE&)*((DATATYPE*)NULL), wxPGVariantData)


#define WX_PG_DECLARE_VARIANT_DATA(CLASSNAME, DATATYPE, DECL) \
DECL DATATYPE& operator <<( DATATYPE& value, const wxVariant &variant ); \
DECL wxVariant& operator <<( wxVariant &variant, const DATATYPE& value ); \
DECL DATATYPE& DATATYPE##FromVariant( const wxVariant& variant ); \
DECL wxVariant DATATYPE##ToVariant( const DATATYPE& value ); \
template<> inline wxVariant WXVARIANT( const DATATYPE& value ) { return DATATYPE##ToVariant(value); } \
DECL _WX_PG_VARIANT_DATA_CLASSINFO_CONTAINER_DECL(CLASSNAME);

#define WX_PG_DECLARE_WXOBJECT_VARIANT_DATA WX_PG_DECLARE_VARIANT_DATA

#define WX_PG_DECLARE_PTR_VARIANT_DATA(CLASSNAME, DATATYPE, DECL) \
DECL DATATYPE* operator <<( DATATYPE* value, const wxVariant &variant ); \
DECL wxVariant& operator <<( wxVariant &variant, DATATYPE* value ); \
DECL DATATYPE* DATATYPE##FromVariant( const wxVariant& variant ); \
DECL wxVariant DATATYPE##ToVariant( DATATYPE* value ); \
DECL _WX_PG_VARIANT_DATA_CLASSINFO_CONTAINER_DECL(CLASSNAME);


#define WX_PG_IMPLEMENT_PTR_VARIANT_DATA(CLASSNAME, DATATYPE, DEFAULT) \
class CLASSNAME : public wxPGVariantData \
{ \
_WX_PG_IMPLEMENT_VARIANT_DATA_CLASS(CLASSNAME, DATATYPE*, DATATYPE*, \
                                    DATATYPE*, m_value = NULL, \
                                    DEFAULT, if (m_value) Py_DECREF(m_value); if (!value) value = Py_None; Py_INCREF(value) ) \
    ~CLASSNAME() { if (m_value) Py_DECREF(m_value); } \
public: \
    virtual void* GetValuePtr() { return (void*)m_value; } \
}; \
_WX_PG_IMPLEMENT_VARIANT_DATA(CLASSNAME, DATATYPE, DATATYPE*, DATATYPE*, NULL, wxPGVariantData)


WX_PG_DECLARE_VARIANT_DATA(wxPGVariantDataPoint, wxPoint, WXDLLIMPEXP_PG)
WX_PG_DECLARE_VARIANT_DATA(wxPGVariantDataSize, wxSize, WXDLLIMPEXP_PG)
WX_PG_DECLARE_VARIANT_DATA(wxPGVariantDataArrayInt, wxArrayInt, WXDLLIMPEXP_PG)
WX_PG_DECLARE_VARIANT_DATA(wxPGVariantDataLongLong, wxLongLong, WXDLLIMPEXP_PG)
WX_PG_DECLARE_VARIANT_DATA(wxPGVariantDataULongLong, wxULongLong, WXDLLIMPEXP_PG)
#ifdef __WXPYTHON__
    WX_PG_DECLARE_PTR_VARIANT_DATA(wxPGVariantDataPyObject, PyObject, WXDLLIMPEXP_PG)
#endif

WX_PG_DECLARE_WXOBJECT_VARIANT_DATA(wxPGVariantDataFont, wxFont, WXDLLIMPEXP_PG)
#if !wxCHECK_VERSION(2,8,0)
    WX_PG_DECLARE_WXOBJECT_VARIANT_DATA(wxPGVariantDataColour, wxColour, WXDLLIMPEXP_PG)
#else
    template<> inline wxVariant WXVARIANT( const wxColour& value )
    {
        wxVariant variant;
        variant << value;
        return variant;
    }
#endif

#endif // !SWIG

#define WX_PG_VARIANT_GETVALUEREF(P, T)   (*((T*)((wxPGVariantData*)(P.GetData()))->GetValuePtr()))

// Safely converts a wxVariant to (long) int. Supports converting from string as well.
WXDLLIMPEXP_PG long wxPGVariantToInt( const wxVariant& variant );

// -----------------------------------------------------------------------


#if !defined(SWIG)

#if wxPG_USE_STL
typedef WXDLLIMPEXP_PG std::vector<wxPGProperty*>  wxArrayPGProperty;
#else
WX_DEFINE_ARRAY_PTR(wxPGProperty*, wxArrayPGProperty);
#endif


// Always use wxString based hashmap with unicode, stl, swig and GCC 4.0+
WX_DECLARE_STRING_HASH_MAP_WITH_DECL(void*,
                                     wxPGHashMapS2P,
                                     class WXDLLIMPEXP_PG);

#else

class WXDLLIMPEXP_PG wxArrayPGProperty;
class WXDLLIMPEXP_PG wxPGHashMapS2P;

#endif


// -----------------------------------------------------------------------

//
// Property class declaration helper macros
// (wxPGRootPropertyClass and wxPropertyCategory require this).
//

// We don't want to create SWIG interface for DoGetEditorClass (we'll use GetEditor instead)
#ifndef SWIG
    #define WX_PG_DECLARE_DOGETEDITORCLASS  virtual const wxPGEditor* DoGetEditorClass() const;
#else
    #define WX_PG_DECLARE_DOGETEDITORCLASS
#endif

#ifndef SWIG
    #define WX_PG_DECLARE_PROPERTY_CLASS(CLASSNAME) \
        public: \
            DECLARE_DYNAMIC_CLASS(CLASSNAME) \
            WX_PG_DECLARE_DOGETEDITORCLASS \
        private:
#else
    #define WX_PG_DECLARE_PROPERTY_CLASS(CLASSNAME)
#endif

// Implements sans constructor function. Also, first arg is class name, not property name.
#define WX_PG_IMPLEMENT_PROPERTY_CLASS_PLAIN(PROPNAME,T,EDITOR) \
const wxPGEditor* PROPNAME::DoGetEditorClass() const \
{ \
    return wxPGEditor_##EDITOR; \
}


#ifndef SWIG

// -----------------------------------------------------------------------

// Structure for relaying choice/list info.
struct wxPGChoiceInfo
{
    wxPGChoices*    m_choices;
};

// -----------------------------------------------------------------------

//
// Ids for sub-controls
// NB: It should not matter what these are.
#define wxPG_SUBID1                     2
#define wxPG_SUBID2                     3
#define wxPG_SUBID_TEMP1                4


// -----------------------------------------------------------------------

#ifndef SWIG

/** \class wxPGAttributeStorage
	\ingroup classes
    \brief wxPGAttributeStorage is somewhat optimized storage for
      key=variant pairs (ie. a map).
*/
class WXDLLIMPEXP_PG wxPGAttributeStorage
{
public:
    wxPGAttributeStorage();
    ~wxPGAttributeStorage();

    void Set( const wxString& name, const wxVariant& value );
    size_t GetCount() const { return m_map.size(); }
    wxVariant FindValue( const wxString& name ) const
    {
        wxPGHashMapS2P::const_iterator it = m_map.find(name);
        if ( it != m_map.end() )
        {
            wxVariantData* data = (wxVariantData*) it->second;
            data->IncRef();
            return wxVariant(data, it->first);
        }
        return wxVariant();
    }

    typedef wxPGHashMapS2P::const_iterator const_iterator;
    const_iterator StartIteration() const
    {
        return m_map.begin();
    }
    bool GetNext( const_iterator& it, wxVariant& variant ) const
    {
        if ( it == m_map.end() )
            return false;

        wxVariantData* data = (wxVariantData*) it->second;
        data->IncRef();
        variant.SetData(data);
        variant.SetName(it->first);
        it++;
        return true;
    }

protected:
    wxPGHashMapS2P  m_map;
};

#endif  // !SWIG

// -----------------------------------------------------------------------

/** \class wxPGPropertyDataExt
	\ingroup classes
    \brief wxPGPropertyDataExt is data extension class for wxPGProperty.
*/
class WXDLLIMPEXP_PG wxPGPropertyDataExt
{
public:

    wxPGPropertyDataExt()
    {
#if wxUSE_VALIDATORS
        m_validator = (wxValidator*) NULL;
#endif
        m_customEditor = (wxPGEditor*) NULL;
        m_valueBitmap = (wxBitmap*) NULL;
    }

    ~wxPGPropertyDataExt()
    {
        // Do not delete m_customEditor
#if wxUSE_VALIDATORS
        delete m_validator;
#endif
        delete m_valueBitmap;
    }

    // For conviency, declare all members public.

    wxString                    m_helpString; // Help shown in statusbar or help box.
    const wxPGEditor*           m_customEditor; // Overrides editor returned by property class

#if wxUSE_VALIDATORS
    // NOTE: This is candidate for hash mapping.
    wxValidator*                m_validator; // Editor is going to get this validator
#endif

    wxBitmap*                   m_valueBitmap; // Show this in front of the value
};

#endif // SWIG

// -----------------------------------------------------------------------

/** \class wxPGProperty
	\ingroup classes
    \brief wxPGProperty, alias wxBasePropertyClass, is base class for properties.
    Information here is provided primarily for anyone who creates new properties,
    since <b>all operations on properties should be done via wxPropertyGrid's or
    wxPropertyGridManager's methods</b>.

    \remarks
    - When changing name of a property, it is essential to use wxPropertyGrid::SetPropertyName
      (that's why there is no SetName method).
*/
class WXDLLIMPEXP_PG wxPGProperty : public wxObject
{
#ifndef SWIG
    friend class wxPropertyGrid;
    friend class wxPropertyContainerMethods;
    friend class wxPropertyGridState;
    friend class wxPropertyGridPopulator;
    DECLARE_ABSTRACT_CLASS(wxPGProperty)
#endif
public:
    typedef unsigned short FlagType;
#ifndef __WXPYTHON__
    typedef void* ClientDataType;
#else
    typedef PyObject* ClientDataType;
#endif

// PYSWIG is a special symbol used by my custom scripts. Code to remove it
// automatically should be added in future.
#ifndef PYSWIG
    /** Basic constructor. Should not be necessary to override.
    */
    wxPGProperty();
#endif

    /** Constructor.
        Real used property classes should have constructor of this style:

        \code

        // If T is a class, then it should be a constant reference
        // (e.g. const T& ) instead.
        MyProperty( const wxString& label, const wxString& name, T value )
            : wxPGProperty ()
        {
            // Generally recommended way to set the initial value
            // (as it should work in pretty much 100% of cases).
            wxVariant variant;
            variant << value;
            SetValue(variant);

            // If has child properties then create them here. For example:
            //     AddChild( new wxStringProperty( wxT("Subprop 1"), wxPG_LABEL, value.GetSubProp1() ) );
        }

        \endcode

        Of course, in this example, wxPGProperty could actually any other property class.
    */
    wxPGProperty( const wxString& label, const wxString& name );

    /** Virtual destructor. It is customary for derived properties to implement this. */
    virtual ~wxPGProperty();

    /** This virtual function is called after m_value has been validated and set.

        \remarks
        - If m_value was set to Null variant (ie. unspecified value), OnSetValue
          will not be called.
        - Default implementation does nothing.
    */
    virtual void OnSetValue();

    /** Override this to return something else than m_value as the value.
    */
    virtual wxVariant DoGetValue() const { return m_value; }

#ifndef DOXYGEN
private:
#else
public:
#endif
    // These methods are private. Use ActualXXX versions in code.
    //

    /** Implement this function in derived class to check and/or convert the
        value. Return true if it is ok.

        Default implementation does nothing except returns true.
    */
    virtual bool ValidateValue( wxVariant& value ) const;

    /** Converts 'text' into proper value 'variant'. Returns true if new (different than
        m_value) value could be interpreted from the text.
        \param argFlags
        If wxPG_FULL_VALUE is set, returns complete, storable value instead of displayable
        one (they may be different).

        \remarks
        Default implementation converts semicolon delimited tokens into child values. Only
        works for properties with children.
    */
    virtual bool StringToValue( wxVariant& variant, const wxString& text, int argFlags = 0 ) const;

    /** Converts 'number' (including choice selection) into proper value 'variant'.
        Returns true if new (different than m_value) value could be interpreted from the integer.
        \param argFlags
        If wxPG_FULL_VALUE is set, returns complete, storable value instead of displayable
        \remarks
        - Default implementation may work correctly if used value type is int.
        - If property is not supposed to use choice or spinctrl or other editor
          with int-based value, it is not necessary to implement this method.
        - If property uses choice control, and displays a dialog on some choice items,
          then it is preferred to display that dialog in IntToValue instead of OnEvent.
    */
    virtual bool IntToValue( wxVariant& value, int number, int argFlags = 0 ) const;
public:

    /** Returns text representation of property's value.

        \remarks
        Default implementation returns string composed from text representations of
        child properties.
    */
    virtual wxString GetValueAsString( int argFlags = 0 ) const;

    /** Converts string to a value, and if succesfull, calls SetValue() on it.
        Default behaviour is to do nothing.
        \param text
        String to get the value from.
        \param report_error
        If true, invalid string will be reported (prefer to use wxLogError).
        \retval
        true if value was changed.
    */
#if wxPG_COMPATIBILITY_1_2_0
    virtual
#endif
    bool SetValueFromString( const wxString& text, int flags = 0 );

    /** Converts integer to a value, and if succesfull, calls SetValue() on it.
        Default behaviour is to do nothing.
        \param value
        Int to get the value from.
        \param flags
        If has wxPG_FULL_VALUE, then the value given is a actual value and not an index.
        \retval
        true if value was changed.
    */
#if wxPG_COMPATIBILITY_1_2_0
    virtual
#endif
    bool SetValueFromInt( long value, int flags = 0 );

#if wxPG_COMPATIBILITY_1_2_0
    /** Deprecated, overriding this will cause run-time assertion failure.
    */
    virtual wxSize GetImageSize() const;
#endif

    /** Returns size of the custom painted image in front of property. This method
        must be overridden to return non-default value if OnCustomPaint is to be
        called.
        \param item
        Normally -1, but can be an index to the property's list of items.
        \remarks
        - Default behaviour is to return wxSize(0,0), which means no image.
        - Default image width or height is indicated with dimension -1.
        - You can also return wxPG_DEFAULT_IMAGE_SIZE which equals wxSize(-1, -1).
    */
    virtual wxSize OnMeasureImage( int item = -1 ) const;

    /** Events received by editor widgets are processed here. Note that editor class
        usually processes most events. Some, such as button press events of
        TextCtrlAndButton class, should be handled here. Also, if custom handling
        for regular events is desired, then that can also be done (for example,
        wxSystemColourProperty custom handles wxEVT_COMMAND_CHOICE_SELECTED).

        If the event causes value to be changed, wxPropertyGrid::ValueChangeInEvent(value)
        should be used.

        \param event
        Associated wxEvent.
        \retval
        Should return true if any changes in value should be reported.
        \remarks
        - If property uses choice control, and displays a dialog on some choice items,
          then it is preferred to display that dialog in IntToValue instead of OnEvent.
    */
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event );

    /** Called after value of a child property has been altered. Note that this function is
        usually called at the time that value of this property, or given child property, is
        still pending for change.

        Sample pseudo-code implementation:

        \code
        void MyProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
        {
            // Acquire reference to actual type of data stored in variant
            // (TFromVariant only exists if wxPropertyGrid's wxVariant-macros were used to create
            // the variant class).
            T& data = TFromVariant(thisValue);

            // Copy childValue into data.
            switch ( childIndex )
            {
                case 0:
                    data.SetSubProp1( childvalue.GetLong() );
                    break;
                case 1:
                    data.SetSubProp2( childvalue.GetString() );
                    break;
                ...
            }
        }
        \endcode

        \param thisValue
        Value of this property, that should be altered.
        \param childIndex
        Index of child changed (you can use Item(childIndex) to get).
        \param childValue
        Value of the child property.
    */
    virtual void ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;

#ifdef __WXPYTHON__
    // Because SWIG doesn't generate code well for writeback variables,
    // we need to declare custom versions of some functions.
    virtual wxPGVariantAndBool PyValidateValue( const wxVariant& value ) const;
    virtual wxPGVariantAndBool PyStringToValue( const wxString& text, int argFlags = 0 ) const;
    virtual wxPGVariantAndBool PyIntToValue( int number, int argFlags = 0 ) const;
#endif

    bool ActualValidateValue( wxVariant& value ) const
    {
#ifdef __WXPYTHON__
        if ( m_scriptObject )
        {
            wxPGVariantAndBool vab = PyValidateValue(value);
            if ( vab.m_valueValid )
                value = vab.m_value;
            return vab.m_result;
        }
#endif

        return ValidateValue(value);
    }

    bool ActualStringToValue( wxVariant& variant, const wxString& text, int argFlags = 0 ) const
    {
#ifdef __WXPYTHON__
        if ( m_scriptObject )
        {
            wxPGVariantAndBool vab = PyStringToValue(text, argFlags);
            if ( vab.m_valueValid )
                variant = vab.m_value;
            return vab.m_result;
        }
#endif

        return StringToValue(variant, text, argFlags);
    }

    bool ActualIntToValue( wxVariant& variant, int number, int argFlags = 0 ) const
    {
#ifdef __WXPYTHON__
        if ( m_scriptObject )
        {
            wxPGVariantAndBool vab = PyIntToValue(number, argFlags);
            if ( vab.m_valueValid )
                variant = vab.m_value;
            return vab.m_result;
        }
#endif

        return IntToValue(variant, number, argFlags);
    }


    bool IsValueType( const wxChar* str )
    {
        return ((m_value.GetType() == str) == 0);
    }

#ifndef SWIG
    /** Returns pointer to an instance of editor class.
    */
    virtual const wxPGEditor* DoGetEditorClass() const;
#endif

#ifdef __WXPYTHON__
    /** Returns name of editor used. Takes precendence in the wxPython bindings.
    */
    virtual wxString GetEditor() const;
#endif

    /** Returns pointer to the wxValidator that should be used
        with the editor of this property (NULL for no validator).
        Setting validator explicitly via SetPropertyValidator
        will override this.

        In most situations, code like this should work well
        (macros are used to maintain one actual validator instance,
        so on the second call the function exits within the first
        macro):

        \code

        wxValidator* wxMyPropertyClass::DoGetValidator () const
        {
            WX_PG_DOGETVALIDATOR_ENTRY()

            wxMyValidator* validator = new wxMyValidator(...);

            ... prepare validator...

            WX_PG_DOGETVALIDATOR_EXIT(validator)
        }

        \endcode

        \remarks
        You can get common filename validator by returning
        wxFileProperty::GetClassValidator(). wxDirProperty,
        for example, uses it.
    */
    virtual wxValidator* DoGetValidator () const;

    /** Returns current value's index to the choice control. May also return,
        through pointer arguments, strings that should be inserted to that control.
        Irrelevant to classes which do not employ wxPG_EDITOR(Choice) or similar.
        \remarks
        - If returns NULL in choices, then this class must be
          derived from wxBaseEnumProperty.
        - Must not crash even if property's set of choices is uninitialized
          (i.e. it points to wxPGGlobalVars->m_emptyConstants).
    */
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );

    /** Override to paint an image in front of the property value text or drop-down
        list item (but only if wxPGProperty::OnMeasureImage is overridden as well).

        If property's OnMeasureImage() returns size that has height != 0 but less than
        row height ( < 0 has special meanings), wxPropertyGrid calls this method to
        draw a custom image in a limited area in front of the editor control or
        value text/graphics, and if control has drop-down list, then the image is
        drawn there as well (even in the case OnMeasureImage() returned higher height
        than row height).

        NOTE: Following applies when OnMeasureImage() returns a "flexible" height (
        using wxPG_FLEXIBLE_SIZE(W,H) macro), which implies variable height items:
        If rect.x is < 0, then this is a measure item call, which means that
        dc is invalid and only thing that should be done is to set paintdata.m_drawnHeight
        to the height of the image of item at index paintdata.m_choiceItem. This call
        may be done even as often as once every drop-down popup show.

        \param dc
        wxDC to paint on.
        \param rect
        Box reserved for custom graphics. Includes surrounding rectangle, if any.
        If x is < 0, then this is a measure item call (see above).
        \param paintdata
        wxPGPaintData structure with much useful data.

        \remarks
            - You can actually exceed rect width, but if you do so then paintdata.m_drawnWidth
              must be set to the full width drawn in pixels.
            - Due to technical reasons, rect's height will be default even if custom height
              was reported during measure call.
            - Brush is guaranteed to be default background colour. It has been already used to
              clear the background of area being painted. It can be modified.
            - Pen is guaranteed to be 1-wide 'black' (or whatever is the proper colour) pen for
              drawing framing rectangle. It can be changed as well.
        \sa @link GetValueAsString @endlink
    */
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );

    /** Returns used wxPGCellRenderer instance for given property column (label=0, value=1).

        Default implementation returns editor's renderer for all columns.
    */
    virtual wxPGCellRenderer* GetCellRenderer( int column ) const;

    /** Refresh values of child properties. Automatically called after value is set.
    */
    virtual void RefreshChildren();

    /** Special handling for attributes of this property.

        If returns false, then the attribute will be automatically stored in
        m_attributes.

        Default implementation simply returns false.
    */
    virtual bool DoSetAttribute( const wxString& name, wxVariant& value );

    /** Returns value of an attribute.

        Override if custom habdling of attributes is needed.

        Default implementation simply return NULL variant.
    */
    virtual wxVariant DoGetAttribute( const wxString& name ) const;

    /** Returns instance of a new wxPGEditorDialogAdapter instance, which is
        used when user presses the (optional) button next to the editor control;

        Default implementation returns NULL (ie. no action is generated when
        button is pressed).
    */
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;

    /** Adds entry to property's wxPGChoices and editor control (if it is active).
        Returns index of item added.
    */
    inline int AppendChoice( const wxString& label, int value = wxPG_INVALID_VALUE )
    {
        return InsertChoice(label,-1,value);
    }

    /** Removes entry from property's wxPGChoices and editor control (if it is active).

        If selected item is deleted, then the value is set to unspecified.
    */
    void DeleteChoice( int index );

    /** Returns property's label. */
    inline const wxString& GetLabel() const { return m_label; }

    /** Returns property's name with all (non-category, non-root) parents. */
    wxString GetName() const;

    /** Returns property's base name (ie. parent's name is not added in any case) */
    const wxString& GetBaseName() const { return m_name; }

    /** If property did not have data extension, one is created now
        (returns true in that case).
    */
    bool EnsureDataExt();

    wxPGChoices& GetChoices();

    const wxPGChoices& GetChoices() const;

    const wxPGChoiceEntry* GetCurrentChoice() const;

    /** Returns coordinate to the top y of the property. Note that the
        position of scrollbars is not taken into account.
    */
    int GetY() const;

    wxVariant GetValue() const
    {
    #ifdef __WXPYTHON__
        wxVariant val = DoGetValue();
        if ( val.IsNull() )
            return PyObjectToVariant( Py_None );
        return val;
    #else
        return DoGetValue();
    #endif
    }

    /** Returns reference to the internal stored value. GetValue is preferred
        way to get the actual value, since GetValueRef ignores DoGetValue,
        which may override stored value.
    */
    wxVariant& GetValueRef()
    {
        return m_value;
    }

    const wxVariant& GetValueRef() const
    {
        return m_value;
    }

    /** Same as GetValueAsString, except takes common value into account.
    */
    wxString GetValueString( int argFlags = 0 ) const;

    void UpdateControl( wxWindow* primary );

    wxPGCell* GetCell( unsigned int column ) const
    {
        if ( column >= m_cells.size() )
            return NULL;

        return (wxPGCell*) m_cells[column];
    }

    const wxChar* GetClassName() const
    {
        return GetClassInfo()->GetClassName();
    }

    unsigned int GetChoiceCount() const;

    wxString GetChoiceString( unsigned int index );

    /** Return number of displayed common values for this property.
    */
    int GetDisplayedCommonValueCount() const;

    inline wxString GetDisplayedString() const
    {
        return GetValueString(0);
    }

#if wxPG_COMPATIBILITY_1_2_0
    /** Returns property id. */
    inline wxPGId GetId() { return wxPGIdGen(this); }
#endif

    /** Returns property grid where property lies. */
    wxPropertyGrid* GetGrid() const;

    /** Returns owner wxPropertyGrid, but only if one is currently on a page
        displaying this property. */
    wxPropertyGrid* GetGridIfDisplayed() const;

    /** Returns highest level non-category, non-root parent. Useful when you
        have nested wxCustomProperties/wxParentProperties.
        \remarks
        Thus, if immediate parent is root or category, this will return the
        property itself.
    */
    wxPGProperty* GetMainParent() const;

    /** Return parent of property */
    wxPGProperty* GetParent() const { return m_parent; }

    inline bool IsFlagSet( FlagType flag ) const
    {
        return ( m_flags & flag ) ? true : false;
    }

    inline bool IsValueUnspecified() const
    {
        return m_value.IsNull();
    }

    inline FlagType HasFlag( FlagType flag ) const
    {
        return ( m_flags & flag );
    }

    /** Returns true if extra children can be added for this property
        (i.e. it is wxPropertyCategory or wxCustomProperty)
    */
    bool CanHaveExtraChildren() const
    {
        return !IsFlagSet(wxPG_PROP_AGGREGATE);
    }

    /** Returns comma-delimited string of property attributes.
    */
    const wxPGAttributeStorage& GetAttributes() const
    {
        return m_attributes;
    }

    /** Returns m_attributes as list wxVariant.
    */
    wxVariant GetAttributesAsList() const;

    /** Returns property's data extension (NULL if none). */
    inline wxPGPropertyDataExt* GetDataExt() { return m_dataExt; }

    inline unsigned int GetFlags() const
    {
        return (unsigned int)m_flags;
    }

    const wxPGEditor* GetEditorClass() const;

    /** Returns editor used for given column. NULL for no editor.
    */
    const wxPGEditor* GetColumnEditor( int column ) const
    {
        if ( column == 1 )
            return GetEditorClass();

        return NULL;
    }

    /** Returns common value selected for this property. -1 for none.
    */
    int GetCommonValue() const
    {
        return m_commonValue;
    }

    /** Returns type name of property that is compatible with CreatePropertyByType.
        and wxVariant::GetType.
    */
    wxString GetType() const
    {
        return m_value.GetType();
    }

    /** Adds entry to property's wxPGChoices and editor control (if it is active).
        Returns index of item added.
    */
    int InsertChoice( const wxString& label, int index, int value = wxPG_INVALID_VALUE );

    /** Returns true if this property is actually a wxPropertyCategory.
    */
    bool IsCategory() const { return IsFlagSet(wxPG_PROP_CATEGORY); }

    /** Returns true if this property is actually a wxRootProperty.
    */
    bool IsRoot() const { return (m_parent == NULL); }

    /** Returns true if this is a sub-property. */
    inline bool IsSubProperty() const
    {
        wxPGProperty* parent = (wxPGProperty*)m_parent;
        if ( parent && !parent->IsCategory() )
            return true;
        return false;
    }

    /** Returns last visible sub-property, recursively.
    */
    const wxPGProperty* GetLastVisibleSubItem() const;

    wxVariant GetDefaultValue() const;

    inline int GetMaxLength() const
    {
        return (int) m_maxLen;
    }

#ifdef SWIG
    %pythoncode {
        def GetValue(self):
            return self.GetGrid().GetPropertyValue(self)
    }
#else

    template<class T>
    T& GetValueAs() const
    {
        wxPGVariantData* data = (wxPGVariantData*) GetValue().GetData();
        wxASSERT_MSG( data && data->IsKindOf(CLASSINFO(wxPGVariantData)), wxT("Use GetValueAs only if value's variant data is wxPGVariantData-based") );
        return *((T*)data->GetValuePtr());
    }

  #if wxPG_COMPATIBILITY_1_2_0
    /** Returns value as wxVariant.
        DEPRECATED: Use GetValue() instead.
    */
    wxVariant GetValueAsVariant() const
    {
        return GetValue();
    }
  #endif
#endif

    /** Returns true if containing grid uses wxPG_EX_AUTO_UNSPECIFIED_VALUES.
    */
    bool UsesAutoUnspecified() const;

    inline wxBitmap* GetValueImage() const
    {
        if ( m_dataExt )
            return m_dataExt->m_valueBitmap;
        return (wxBitmap*) NULL;
    }

    wxVariant GetAttribute( const wxString& name ) const;

    /** Returns named attribute, as string, if found. Otherwise defVal is returned.
    */
    wxString GetAttribute( const wxString& name, const wxString& defVal );

    /** Returns named attribute, as long, if found. Otherwise defVal is returned.
    */
    long GetAttribute( const wxString& name, long defVal );

    inline unsigned int GetArrIndex() const { return m_arrIndex; }

    inline unsigned int GetDepth() const { return (unsigned int)m_depth; }

    /** Gets flags as a'|' delimited string. Note that flag names are not
        prepended with 'wxPG_PROP_'.
        \param flagsMask
        String will only be made to include flags combined by this parameter.
    */
    wxString GetFlagsAsString( FlagType flagsMask ) const;

    /** Returns position in parent's array. */
    inline unsigned int GetIndexInParent() const
    {
        return (unsigned int)m_arrIndex;
    }

    /** Hides or reveals the property.
        \param hide
        true for hide, false for reveal.
    */
    inline bool Hide( bool hide );

    bool IsExpanded() const { return (!(m_flags & wxPG_PROP_COLLAPSED) && GetChildCount()); }

    /** Returns true if all parents expanded.
    */
    bool IsVisible() const;

    inline bool IsEnabled() const
    {
        return ( m_flags & wxPG_PROP_DISABLED ) ? false : true;
    }

    /** If property's editor is created this forces its recreation. Useful
        in SetAttribute etc. Returns true if actually did anything.
    */
    bool RecreateEditor();

    /** If property's editor is active, then update it's value.
    */
    void RefreshEditor();

    /** Sets an attribute for this property.  There are both common attributes and
        property class specific ones. For list of common attributes, see
        wxPropertyContainerMethods::SetPropertyAttribute().
    */
    void SetAttribute( const wxString& name, wxVariant value );

    /** Sets attributes from a comma-delimited string.
    */
    void SetAttributes( const wxPGAttributeStorage& attributes );

    /** Sets editor for a property. */
#ifndef SWIG
    inline void SetEditor( const wxPGEditor* editor )
    {
        EnsureDataExt();
        m_dataExt->m_customEditor = editor;
    }
#endif

    /** Sets editor for a property. */
    inline void SetEditor( const wxString& editorName );

    /** Sets cell information for given column.

        Note that the property takes ownership of given wxPGCell instance.
    */
    void SetCell( int column, wxPGCell* cellObj );

    /** Changes value of a property with choices, but only
        works if the value type is long or string. */
    void SetChoiceSelection( int newValue, const wxPGChoiceInfo& choiceInfo );

    /** Sets common value selected for this property. -1 for none.
    */
    void SetCommonValue( int commonValue )
    {
        m_commonValue = commonValue;
    }

    /** Sets flags from a '|' delimited string. Note that flag names are not
        prepended with 'wxPG_PROP_'.
    */
    void SetFlagsFromString( const wxString& str );

    /** Call this to set value of the property. Unlike methods in wxPropertyGrid,
        this does not automatically update the display.
    */
    void SetValue( wxVariant value );

    /** Like SetValue, but can accept value as list of child values
        (only if wxPG_PROP_AGGREGATE flag is set).
        \param pList
        Pointer to list variant that contains child values. Used to indicate
        which children should be marked as modified.
        \param refreshEditor
        If true, then editor control is refreshed (if active).
    */
    void SetValue0( wxVariant& value, wxVariant* pList = NULL, bool refreshEditor = true );

    /** Set wxBitmap in front of the value. This bitmap may be ignored
        by custom cell renderers.
    */
    void SetValueImage( wxBitmap& bmp );

    /** If property has choices and they are not yet exclusive, new such copy
        of them will be created.
    */
    void SetChoicesExclusive();

    void SetExpanded( bool expanded )
    {
        if ( !expanded ) m_flags |= wxPG_PROP_COLLAPSED;
        else m_flags &= ~wxPG_PROP_COLLAPSED;
    }

    inline void SetFlag( FlagType flag ) { m_flags |= flag; }

    inline void SetHelpString( const wxString& helpString )
    {
        EnsureDataExt();
        m_dataExt->m_helpString = helpString;
    }

    void SetLabel( const wxString& label ) { m_label = label; }

    inline void SetValueToUnspecified()
    {
        m_value.MakeNull();
    }

#if wxUSE_VALIDATORS
    /** Sets wxValidator for a property*/
    inline void SetValidator( const wxValidator& validator )
    {
        EnsureDataExt();
        m_dataExt->m_validator = wxDynamicCast(validator.Clone(),wxValidator);
    }

    /** Gets assignable version of property's validator. */
    inline wxValidator* GetValidator() const
    {
        if ( m_dataExt )
            return m_dataExt->m_validator;
        return DoGetValidator();
    }
#endif // #if wxUSE_VALIDATORS

    /** Updates property value in case there were last minute
        changes. If value was unspecified, it will be set to default.
        Use only for properties that have TextCtrl-based editor.
        \remarks
        If you have code similar to
        \code
            // Update the value in case of last minute changes
            if ( primary && propgrid->IsEditorsValueModified() )
                 GetEditorClass()->CopyValueFromControl( this, primary );
        \endcode
        in wxPGProperty::OnEvent wxEVT_COMMAND_BUTTON_CLICKED handler,
        then replace it with call to this method.
        \retval
        True if value changed.
    */
    bool PrepareValueForDialogEditing( wxPropertyGrid* propgrid );

    inline ClientDataType GetClientData() const { return m_clientData; }

    /** Sets client data (void*) of a property.
        \remarks
        This untyped client data has to be deleted manually.
    */
    inline void SetClientData( ClientDataType clientData )
    {
#ifdef __WXPYTHON__
        if ( m_clientData )
            Py_DECREF( m_clientData );
        Py_INCREF( clientData );
#endif
        m_clientData = clientData;
    }

    /** Sets new set of choices for property.
    */
    bool SetChoices( wxPGChoices& choices );

    /** Sets new set of choices for property.
    */
    inline bool SetChoices( const wxArrayString& labels,
                            const wxArrayInt& values = wxPG_EMPTY_ARRAYINT );

    /** Set max length of text editor.
    */
    inline bool SetMaxLength( int maxLen );

    /** Call with 'false' in OnSetValue to cancel value changes after all
        (ie. cancel 'true' returned by StringToValue() or IntToValue()).
    */
    void SetWasModified( bool set = true )
    {
        if ( set ) m_flags |= wxPG_PROP_WAS_MODIFIED;
        else m_flags &= ~wxPG_PROP_WAS_MODIFIED;
    }

    inline wxString GetHelpString() const
    {
        if (m_dataExt)
            return m_dataExt->m_helpString;
        return wxEmptyString;
    }

    inline void ClearFlag( FlagType flag ) { m_flags &= ~(flag); }

    // Use, for example, to detect if item is inside collapsed section.
    bool IsSomeParent( wxPGProperty* candidate_parent ) const;

    // Shows error as a tooltip or something similar (depends on platform).
    void ShowError( const wxString& msg ) const;

#if defined(__WXPYTHON__) && !defined(SWIG)
    // This is the python object that contains and owns the C++ representation.
    PyObject*                   m_scriptObject;
#endif

    /** Adapts list variant into proper value using consequtive ChildChanged-calls.
    */
    void AdaptListToValue( wxVariant& list, wxVariant* value ) const;

    /** This is used by properties that have fixed sub-properties. */
    void AddChild( wxPGProperty* prop );

    /** Returns height of children, recursively, and
        by taking expanded/collapsed status into account.
        
        iMax is used when finding property y-positions.
    */
    int GetChildrenHeight( int lh, int iMax = -1 ) const;

    /** Returns number of child properties */
    unsigned int GetChildCount() const { return m_children.GetCount(); }

    /** Returns number of child properties */
    unsigned GetCount() const { return GetChildCount(); }

    /** Returns sub-property at index i. */
    inline wxPGProperty* Item( size_t i ) const { return (wxPGProperty*)m_children.Item(i); }

    /** Returns last sub-property.
    */
    wxPGProperty* Last() const { return (wxPGProperty*)m_children.Last(); }

    /** Returns index of given sub-property. */
    inline int Index( const wxPGProperty* p ) const { return m_children.Index((wxPGProperty*)p); }

    /** Deletes all sub-properties. */
    void Empty();

    // Puts correct indexes to children
    void FixIndexesOfChildren( size_t starthere = 0 );

#ifndef SWIG
    // Returns wxPropertyGridState in which this property resides.
    wxPropertyGridState* GetParentState() const { return m_parentState; }
#endif

    wxPGProperty* GetItemAtY( unsigned int y, unsigned int lh, unsigned int* nextItemY ) const;

    /** Returns (direct) child property with given name (or NULL if not found).
    */
    wxPGProperty* GetPropertyByName( const wxString& name ) const;

protected:

#ifndef SWIG

    /** This is used by Insert etc. */
    void AddChild2( wxPGProperty* prop, int index = -1, bool correct_mode = true );

    void DoSetName(const wxString& str) { m_name = str; }

    // Call for after sub-properties added with AddChild
    void PrepareSubProperties();

    void SetParentalType( int flag )
    {
        m_flags = (m_flags&(~(wxPG_PROP_PROPERTY|wxPG_PROP_PARENTAL_FLAGS)))|flag;
    }

    void SetParentState( wxPropertyGridState* pstate ) { m_parentState = pstate; }

    // Call after fixed sub-properties added/removed after creation.
    // if oldSelInd >= 0 and < new max items, then selection is
    // moved to it.
    void SubPropsChanged( int oldSelInd = -1 );

    int GetY2( int lh ) const;

    wxString                    m_label;
    wxString                    m_name;
    wxPGProperty*               m_parent;
    wxPropertyGridState*        m_parentState;
    ClientDataType              m_clientData;
    wxPGPropertyDataExt*        m_dataExt; // Optional data extension.
    wxVariant                   m_value;
    wxPGAttributeStorage        m_attributes;
    wxArrayPtrVoid              m_children;
    wxArrayPtrVoid              m_cells;  // Extended cell information
    unsigned int                m_arrIndex; // Index in parent's property array.
    int                         m_commonValue; // If not -1, then overrides m_value

    short                       m_maxLen; // Maximum length (mainly for string properties).
                                          // Could be in some sort of wxBaseStringProperty, but currently,
                                          // for maximum flexibility and compatibility, we'll stick it here.
                                          // Anyway, we had 3 excess bytes to use so short int will fit in
                                          // just fine.

    FlagType                    m_flags;

    unsigned char               m_depth; // Root has 0, categories etc. at that level 1, etc.

    // m_depthBgCol indicates width of background colour between margin and item
    // (essentially this is category's depth, if none then equals m_depth).
    unsigned char               m_depthBgCol;

    unsigned char               m_bgColIndex; // Background brush index.
    unsigned char               m_fgColIndex; // Foreground colour index.

private:
    // Called in constructors.
    void Init();
    void Init( const wxString& label, const wxString& name );
#endif // #ifndef SWIG
};


#if wxPG_COMPATIBILITY_1_2_0
    typedef wxPGProperty wxPGPropertyWithChildren;
#endif

#define wxPGPropNameStr            const wxString&
#define wxPGNameConv(STR)      STR

// -----------------------------------------------------------------------


#ifndef SWIG

WX_DECLARE_VOIDPTR_HASH_MAP_WITH_DECL(void*,
                                      wxPGHashMapP2P,
                                      class WXDLLIMPEXP_PG);

#else
class WXDLLIMPEXP_PG wxPGHashMapP2P;
#endif // #ifndef SWIG

// -----------------------------------------------------------------------

#ifndef SWIG

/** \class wxPGRootProperty
    \ingroup classes
    \brief Root parent property.
*/
class WXDLLIMPEXP_PG wxPGRootProperty : public wxPGProperty
{
public:
    WX_PG_DECLARE_PROPERTY_CLASS(wxPGRootProperty)
public:

    /** Constructor. */
    wxPGRootProperty();
    virtual ~wxPGRootProperty();

    virtual bool StringToValue( wxVariant&, const wxString&, int ) const
    {
        return false;
    }

    virtual void ChildChanged( wxVariant&, int, wxVariant& ) const
    {
    }

protected:
};

// -----------------------------------------------------------------------

/** \class wxPropertyCategory
    \ingroup classes
    \brief Category (caption) property.
*/
class WXDLLIMPEXP_PG wxPropertyCategory : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxPropertyCategory)
public:

    /** Special constructor only used in special cases. */
    wxPropertyCategory();

    /** Construct.
        \param label
        Label for the category.
        \remarks
        All non-category properties appended will have most recently
        added category.
    */
    wxPropertyCategory( const wxString& label, const wxString& name = wxPG_LABEL );
    ~wxPropertyCategory();

    /** Must be overridden with function that doesn't do anything. */
    virtual wxString GetValueAsString( int argFlags ) const;

    virtual void ChildChanged( wxVariant&, int, wxVariant& ) const
    {
    }

    int GetTextExtent( const wxWindow* wnd, const wxFont& font ) const;

    void CalculateTextExtent( wxWindow* wnd, const wxFont& font );

    void SetTextColIndex( unsigned int colInd ) { m_capFgColIndex = (wxByte) colInd; }
    unsigned int GetTextColIndex() const { return (unsigned int) m_capFgColIndex; }

protected:
    int     m_textExtent;  // pre-calculated length of text
    wxByte  m_capFgColIndex;  // caption text colour index

private:
    void Init();
};

#endif  // !SWIG

// -----------------------------------------------------------------------

/** \class wxPGCommonValue
    \ingroup classes
    \brief wxPropertyGrid stores information about common values in these
    records.
*/
class WXDLLIMPEXP_PG wxPGCommonValue
{
public:

    wxPGCommonValue( const wxString& label, wxPGCellRenderer* renderer )
    {
        m_label = label;
        m_renderer = renderer;
        renderer->IncRef();
    }
    virtual ~wxPGCommonValue()
    {
        m_renderer->DecRef();
    }

    virtual wxString GetEditableText() const { return m_label; }
    const wxString& GetLabel() const { return m_label; }
    wxPGCellRenderer* GetRenderer() const { return m_renderer; }

protected:
    wxString            m_label;
    wxPGCellRenderer*   m_renderer;
};

// -----------------------------------------------------------------------


#ifndef SWIG

/** \class wxPGChoiceEntry
    \ingroup classes
    \brief Attributes of a single choice.
*/
class WXDLLIMPEXP_PG wxPGChoiceEntry : public wxPGCell
{
public:
    wxPGChoiceEntry();
    wxPGChoiceEntry( const wxPGChoiceEntry& entry );
    wxPGChoiceEntry( const wxString& label,
                     int value = wxPG_INVALID_VALUE )
        : wxPGCell(), m_value(value)
    {
        m_text = label;
    }

    wxPGChoiceEntry( const wxString& label,
                     int value,
                     const wxBitmap& bitmap,
                     const wxColour& fgCol = wxNullColour,
                     const wxColour& bgCol = wxNullColour )
        : wxPGCell(label, bitmap, fgCol, bgCol), m_value(value)
    {
    }

    virtual ~wxPGChoiceEntry()
    {
    }

    void SetValue( int value ) { m_value = value; }

    int GetValue() const { return m_value; }

    bool HasValue() const { return (m_value != wxPG_INVALID_VALUE); }

protected:
    int m_value;
};


typedef void* wxPGChoicesId;

class WXDLLIMPEXP_PG wxPGChoicesData
{
    friend class wxPGChoices;
public:
    // Constructor sets m_refCount to 1.
    wxPGChoicesData();

    void CopyDataFrom( wxPGChoicesData* data );

    // Takes ownership of object
    void Insert( int index, wxPGChoiceEntry* item )
    {
        wxArrayPtrVoid::iterator it;
        if ( index == -1 )
            it = m_items.end();
        else
            it = m_items.begin() + index;
        m_items.insert(it, item);
    }

    size_t GetCount() const
    {
        return m_items.size();
    }

    wxPGChoiceEntry* Item( unsigned int i ) const
    {
        return (wxPGChoiceEntry*) m_items[i];
    }

    void DecRef()
    {
        m_refCount--;
        wxASSERT( m_refCount >= 0 );
        if ( m_refCount == 0 )
            delete this;
    }

private:
    wxArrayPtrVoid  m_items;

    // So that multiple properties can use the same set
    int             m_refCount;

    virtual ~wxPGChoicesData();
};

#define wxPGChoicesEmptyData    ((wxPGChoicesData*)NULL)

#endif // SWIG


/** \class wxPGChoices
    \ingroup classes
    \brief Helper class for managing choices for properties.
    Each entry can have label, value, bitmap, text colour, and background colour.
*/
class WXDLLIMPEXP_PG wxPGChoices
{
public:

    /** Basic constructor. */
    wxPGChoices()
    {
        Init();
    }

    /** Copy constructor. */
    wxPGChoices( const wxPGChoices& a )
    {
        if ( a.m_data != wxPGChoicesEmptyData )
        {
            m_data = a.m_data;
            m_data->m_refCount++;
        }
    }

    /** Constructor. */
    wxPGChoices( const wxChar** labels, const long* values = NULL )
    {
        Init();
        Set(labels,values);
    }

    /** Constructor. */
    wxPGChoices( const wxArrayString& labels, const wxArrayInt& values = wxPG_EMPTY_ARRAYINT )
    {
        Init();
        Set(labels,values);
    }

    /** Simple interface constructor. */
    inline wxPGChoices( wxPGChoicesData* data )
    {
        wxASSERT(data);
        m_data = data;
        data->m_refCount++;
    }

    /** Destructor. */
    ~wxPGChoices()
    {
        Free();
    }

    void AssignData( wxPGChoicesData* data );

    inline void Assign( const wxPGChoices& a )
    {
        AssignData(a.m_data);
    }

#ifndef SWIG
    void operator= (const wxPGChoices& a)
    {
        AssignData(a.m_data);
    }
#endif

    typedef long ValArrItem;

    /** Adds to current. If did not have own copies, creates them now. If was empty,
        identical to set except that creates copies.
    */
    void Add( const wxChar** labels, const ValArrItem* values = NULL );

    /** Version that works with wxArrayString. */
    void Add( const wxArrayString& arr, const ValArrItem* values = NULL );

    /** Version that works with wxArrayString and wxArrayInt. */
    void Add( const wxArrayString& arr, const wxArrayInt& arrint );

    /** Adds single item. */
    wxPGChoiceEntry& Add( const wxString& label, int value = wxPG_INVALID_VALUE );

    /** Adds single item. */
    wxPGChoiceEntry& AddAsSorted( const wxString& label, int value = wxPG_INVALID_VALUE );

    inline void EnsureData()
    {
        if ( m_data == wxPGChoicesEmptyData )
            m_data = new wxPGChoicesData();
    }

    const wxPGChoiceEntry& Item( unsigned int i ) const
    {
        wxASSERT( IsOk() );
        return *m_data->Item(i);
    }

    wxPGChoiceEntry& Item( unsigned int i )
    {
        wxASSERT( IsOk() );
        return *m_data->Item(i);
    }

    wxPGChoiceEntry& operator[](unsigned int i)
    {
        return Item(i);
    }

    const wxPGChoiceEntry& operator[](unsigned int i) const
    {
        return Item(i);
    }

    /** Returns false if this is a constant empty set of choices,
        which should not be modified.
    */
    bool IsOk() const
    {
        return ( m_data != wxPGChoicesEmptyData );
    }

    /** Gets a unsigned number identifying this list. */
    wxPGChoicesId GetId() const { return (wxPGChoicesId) m_data; };

    /** Removes count items starting at position nIndex. */
    void RemoveAt(size_t nIndex, size_t count = 1);

#ifndef SWIG
    /** Does not create copies for itself. */
    void Set( const wxChar** labels, const long* values = NULL )
    {
        Free();
        Add(labels,values);
    }

    /** Version that works with wxArrayString.
        TODO: Deprecate this.
    */
    void Set( wxArrayString& arr, const long* values = (const long*) NULL )
    {
        Free();
        Add(arr,values);
    }
#endif // SWIG

    /** Version that works with wxArrayString and wxArrayInt. */
    void Set( const wxArrayString& labels, const wxArrayInt& values = wxPG_EMPTY_ARRAYINT )
    {
        Free();
        if ( &values )
            Add(labels,values);
        else
            Add(labels);
    }

    // Creates exclusive copy of current choices
    void SetExclusive()
    {
        if ( m_data->m_refCount != 1 )
        {
            wxPGChoicesData* data = new wxPGChoicesData();
            data->CopyDataFrom(m_data);
            Free();
            m_data = data;
        }
    }

    const wxString& GetLabel( size_t ind ) const
    {
        wxASSERT( ind >= 0 && ind < GetCount() );
        return Item(ind).GetText();
    }

    size_t GetCount () const
    {
        wxASSERT_MSG( m_data,
            wxT("When checking if wxPGChoices is valid, use IsOk() instead of GetCount()") );
        return m_data->GetCount();
    }

    /** Returns true if choices in general are likely to have values
        (depens on that all entries have values or none has)
    */
    bool HasValues() const;

    bool HasValue( unsigned int i ) const { return (m_data->GetCount() > i && m_data->Item(i)->HasValue()); }

    int GetValue( size_t ind ) const { return Item(ind).GetValue(); }

    int Index( const wxString& str ) const;
    int Index( int val ) const;

    /** Inserts single item. */
    wxPGChoiceEntry& Insert( const wxString& label, int index, int value = wxPG_INVALID_VALUE );

    // Returns data, increases refcount.
    inline wxPGChoicesData* GetData()
    {
        wxASSERT( m_data->m_refCount != 0xFFFFFFF );
        m_data->m_refCount++;
        return m_data;
    }

    // Returns plain data ptr - no refcounting stuff is done.
    inline wxPGChoicesData* GetDataPtr() const { return m_data; }

    // Changes ownership of data to you.
    inline wxPGChoicesData* ExtractData()
    {
        wxPGChoicesData* data = m_data;
        m_data = wxPGChoicesEmptyData;
        return data;
    }

    wxArrayString GetLabels() const;

#ifndef SWIG

protected:
    wxPGChoicesData*    m_data;

    void Init();
    void Free();
#endif
};


// -----------------------------------------------------------------------

#ifndef SWIG
  #if wxMINOR_VERSION < 7 || ( wxMINOR_VERSION == 7 && wxRELEASE_NUMBER < 1 )
    // NOTE: Since wxWidgets at this point doesn't have wxArrayDouble, we have
    //   to create it ourself, using wxObjArray model.
    WX_DECLARE_OBJARRAY_WITH_DECL(double, wxArrayDouble, class WXDLLIMPEXP_PG);
  #endif
#endif // !SWIG

// -----------------------------------------------------------------------
// Property declaration.

// Doxygen will only generate warnings here
#define wxPG_CONSTFUNC(PROP) PROP##Create
#define wxPG_PROPCLASS(PROP) PROP

// Macro based constructor.
#define wxPG_NEWPROPERTY(PROP,LABEL,NAME,VALUE) wx##PROP##Property(LABEL,NAME,VALUE)


WXDLLIMPEXP_PG wxObject* wxPG_VariantToWxObject( const wxVariant& variant, wxClassInfo* classInfo );

//
// Redefine wxGetVariantCast to also take propertygrid variantdata
// classes into account.
// TODO: Remove after persistent wxObject classes added (i.e.
//   GetWxObjectPtr works for all).
//
#undef wxGetVariantCast
#define wxGetVariantCast(var,classname) (classname*)wxPG_VariantToWxObject(var,&classname::ms_classInfo)

// TODO: After a while, remove this.
#define WX_PG_VARIANT_TO_WXOBJECT(VARIANT,CLASSNAME) (CLASSNAME*)wxPG_VariantToWxObject(VARIANT,&CLASSNAME::ms_classInfo)

// -----------------------------------------------------------------------

#define wxPG_IT_CHILDREN(A)         (A<<16)

/** @defgroup iteratorflags wxPropertyGridIterator Flags
    @{

    NOTES: At lower 16-bits, there are flags to check if item will be included. At higher
      16-bits, there are same flags, but to instead check if children will be included.
*/

/** Iterate through 'normal' property items (does not include children of aggregate or hidden items by default).
*/
#define wxPG_ITERATE_PROPERTIES     (wxPG_PROP_PROPERTY|wxPG_PROP_MISC_PARENT|wxPG_PROP_AGGREGATE| \
                                     wxPG_PROP_COLLAPSED|((wxPG_PROP_MISC_PARENT|wxPG_PROP_CATEGORY)<<16))

/** Iterate children of collapsed parents, and individual items that are hidden.
*/
#define wxPG_ITERATE_HIDDEN         (wxPG_PROP_HIDDEN|wxPG_IT_CHILDREN(wxPG_PROP_COLLAPSED))

/** Iterate children of parent that is an aggregate property (ie. has fixed children).
*/
#define wxPG_ITERATE_FIXED_CHILDREN (wxPG_IT_CHILDREN(wxPG_PROP_AGGREGATE)|wxPG_ITERATE_PROPERTIES)

/** Iterate categories. Note that even without this flag, children of categories
    are still iterated through.
*/
#define wxPG_ITERATE_CATEGORIES      (wxPG_PROP_CATEGORY|wxPG_IT_CHILDREN(wxPG_PROP_CATEGORY)|wxPG_PROP_COLLAPSED)

#define wxPG_ITERATE_ALL_PARENTS        (wxPG_PROP_MISC_PARENT|wxPG_PROP_AGGREGATE|wxPG_PROP_CATEGORY)

#define wxPG_ITERATE_ALL_PARENTS_RECURSIVELY        (wxPG_ITERATE_ALL_PARENTS|wxPG_IT_CHILDREN(wxPG_ITERATE_ALL_PARENTS))

#define wxPG_ITERATOR_FLAGS_ALL     (wxPG_PROP_PROPERTY|wxPG_PROP_MISC_PARENT|wxPG_PROP_AGGREGATE| \
                                     wxPG_PROP_HIDDEN|wxPG_PROP_CATEGORY|wxPG_PROP_COLLAPSED)

#define wxPG_ITERATOR_MASK_OP_ITEM      wxPG_ITERATOR_FLAGS_ALL

#define wxPG_ITERATOR_MASK_OP_PARENT    wxPG_ITERATOR_FLAGS_ALL // (wxPG_PROP_MISC_PARENT|wxPG_PROP_AGGREGATE|wxPG_PROP_CATEGORY)

/** Combines all flags needed to iterate through visible properties
    (ie. hidden properties and children of collapsed parents are skipped).
*/
#define wxPG_ITERATE_VISIBLE        (wxPG_ITERATE_PROPERTIES|wxPG_PROP_CATEGORY|wxPG_IT_CHILDREN(wxPG_PROP_AGGREGATE))

/** Iterate all items.
*/
#define wxPG_ITERATE_ALL            (wxPG_ITERATE_VISIBLE|wxPG_ITERATE_HIDDEN)

/** Iterate through individual properties (ie. categories and children of
    aggregate properties are skipped).
*/
#define wxPG_ITERATE_NORMAL         (wxPG_ITERATE_PROPERTIES|wxPG_ITERATE_HIDDEN)

/** Default iterator flags.
*/
#define wxPG_ITERATE_DEFAULT        wxPG_ITERATE_NORMAL

/** @}
*/


#define wxPG_ITERATOR_CREATE_MASKS(FLAGS, A, B) \
    A = (FLAGS ^ wxPG_ITERATOR_MASK_OP_ITEM) & wxPG_ITERATOR_MASK_OP_ITEM & 0xFFFF; \
    B = ((FLAGS>>16) ^ wxPG_ITERATOR_MASK_OP_PARENT) & wxPG_ITERATOR_MASK_OP_PARENT & 0xFFFF;


// Macro to test if children of PWC should be iterated through
#define wxPG_ITERATOR_PARENTEXMASK_TEST(PWC, PARENTMASK) \
        ( \
        !(PWC->GetFlags() & PARENTMASK) && \
        PWC->GetChildCount() \
        )


/** \class wxPropertyGridIteratorBase
    \ingroup classes
    \brief Base for wxPropertyGridIterator classes.
*/
class WXDLLIMPEXP_PG wxPropertyGridIteratorBase
{
public:
    wxPropertyGridIteratorBase()
    {
    }

    void Assign( const wxPropertyGridIteratorBase& it );

    bool AtEnd() const { return m_property == NULL; }

    /** Get current property.
    */
    wxPGProperty* GetProperty() const { return m_property; }

    void Init( wxPropertyGridState* state, int flags, wxPGProperty* property, int dir = 1 );

    void Init( wxPropertyGridState* state, int flags, int startPos = wxTOP, int dir = 0 );

    /** Iterate to the next property.
    */
    void Next( bool iterateChildren = true );

    /** Iterate to the previous property.
    */
    void Prev();

    /** Set base parent, ie. a property when, in which iteration returns, it ends.

        Default base parent is the root of the used wxPropertyGridState.
    */
    void SetBaseParent( wxPGProperty* baseParent ) { m_baseParent = baseParent; }

protected:

    wxPGProperty*               m_property;

private:
    wxPropertyGridState*        m_state;
    wxPGProperty*               m_baseParent;

    // Masks are used to quickly exclude items
    int                         m_itemExMask;
    int                         m_parentExMask;
};


#define wxPG_IMPLEMENT_ITERATOR(CLASS, PROPERTY, STATE) \
    CLASS( STATE* state, int flags = wxPG_ITERATE_DEFAULT, PROPERTY* property = NULL, int dir = 1 ) \
        : wxPropertyGridIteratorBase() \
        { Init( (wxPropertyGridState*)state, flags, (wxPGProperty*)property, dir ); } \
    CLASS( STATE* state, int flags, int startPos, int dir = 0 ) \
        : wxPropertyGridIteratorBase() \
        { Init( (wxPropertyGridState*)state, flags, startPos, dir ); } \
    CLASS() \
        : wxPropertyGridIteratorBase() \
    { \
        m_property = NULL; \
    } \
    CLASS( const CLASS& it ) \
        : wxPropertyGridIteratorBase( ) \
    { \
        Assign(it); \
    } \
    const CLASS& operator=( const CLASS& it ) \
    { \
        Assign(it); \
        return *this; \
    } \
    CLASS& operator++() { Next(); return *this; } \
    CLASS operator++(int) { CLASS it=*this;Next();return it; } \
    CLASS& operator--() { Prev(); return *this; } \
    CLASS operator--(int) { CLASS it=*this;Prev();return it; } \
    PROPERTY* operator *() const { return (PROPERTY*)m_property; } \
    static PROPERTY* OneStep( STATE* state, int flags = wxPG_ITERATE_DEFAULT, PROPERTY* property = NULL, int dir = 1 ) \
    { \
        CLASS it( state, flags, property, dir ); \
        if ( property ) \
        { \
            if ( dir == 1 ) it.Next(); \
            else it.Prev(); \
        } \
        return *it; \
    }


/** \class wxPropertyGridIterator
    \ingroup classes
    \brief Preferable way to iterate through contents of wxPropertyGrid,
    wxPropertyGridManager, and wxPropertyGridPage.

    See wxPropertyContainerMethods::GetIterator() for more information about usage.
*/
class WXDLLIMPEXP_PG wxPropertyGridIterator : public wxPropertyGridIteratorBase
{
public:

    wxPG_IMPLEMENT_ITERATOR(wxPropertyGridIterator,
                            wxPGProperty,
                            wxPropertyGridState)

protected:
};


/** \class wxPropertyGridConstIterator
    \ingroup classes
    \brief Const version of wxPropertyGridIterator.
*/
class WXDLLIMPEXP_PG wxPropertyGridConstIterator : public wxPropertyGridIteratorBase
{
public:
    wxPG_IMPLEMENT_ITERATOR(wxPropertyGridConstIterator,
                            const wxPGProperty,
                            const wxPropertyGridState)

protected:
};

// -----------------------------------------------------------------------

/** Base class to derive new viterators.
*/
class WXDLLIMPEXP_PG wxPGVIteratorBase
{
    friend class wxPGVIterator;
public:
    wxPGVIteratorBase() { m_refCount = 1; }
    virtual void Next() = 0;
    void IncRef()
    {
        m_refCount++;
    }
    void DecRef()
    {
        m_refCount--;
        if ( m_refCount <= 0 )
            delete this;
    }
protected:
    virtual ~wxPGVIteratorBase() { }

    wxPropertyGridIterator  m_it;
private:
    int     m_refCount;
};

/** \class wxPGVIterator
    \ingroup classes
    \brief Abstract implementation of a simple iterator. Can only be used
    to iterate in forward order, and only through the entire container.
    Used to have functions dealing with all properties work with both
    wxPropertyGrid and wxPropertyGridManager.
*/
class WXDLLIMPEXP_PG wxPGVIterator
{
public:
    wxPGVIterator() { m_pIt = NULL; }
    wxPGVIterator( wxPGVIteratorBase* obj ) { m_pIt = obj; }
    ~wxPGVIterator() { UnRef(); }
    void UnRef() { if (m_pIt) m_pIt->DecRef(); }
    wxPGVIterator( const wxPGVIterator& it )
    {
        UnRef();
        m_pIt = it.m_pIt;
        m_pIt->IncRef();
    }
#ifndef SWIG
    const wxPGVIterator& operator=( const wxPGVIterator& it )
    {
        UnRef();
        m_pIt = it.m_pIt;
        m_pIt->IncRef();
        return *this;
    }
#endif
    void Next() { m_pIt->Next(); }
    bool AtEnd() const { return m_pIt->m_it.AtEnd(); }
    wxPGProperty* GetProperty() const { return m_pIt->m_it.GetProperty(); }
protected:
    wxPGVIteratorBase*  m_pIt;
};

// -----------------------------------------------------------------------

/** \class wxPropertyGridHitTestResult
    \ingroup classes
    \brief wxPropertyGridHitTestResult, a return value from wxPropertyGrid::HitTest(),
    contains all you need to know about an arbitrary location on the grid.
*/
struct WXDLLIMPEXP_PG wxPropertyGridHitTestResult
{
#ifdef SWIG
private:
#endif
    /** Property. NULL if empty space below properties was hit */
    wxPGProperty*   property;
#ifdef SWIG
public:
#endif

    wxPGProperty* GetProperty() const { return property; }

    /** Column. -1 for margin. */
    int             column;

    /** Index of splitter hit, -1 for none. */
    int             splitter;

    /** If splitter hit, offset to that */
    int             splitterHitOffset;
};

// -----------------------------------------------------------------------

#ifndef SWIG
// We won't need this class from wxPython

/** \class wxPropertyGridState
	\ingroup classes
    \brief
    Contains information of a single wxPropertyGrid page.
*/
// BM_STATE
class WXDLLIMPEXP_PG wxPropertyGridState
{
    friend class wxPGProperty;
    friend class wxPropertyGrid;
    friend class wxPropertyGridManager;
    friend class wxPGCanvas;
public:

    /** Constructor. */
    wxPropertyGridState();

    /** Destructor. */
    virtual ~wxPropertyGridState();

    /** Base append. */
    wxPGProperty* DoAppend( wxPGProperty* property );

    /** Returns property by its name. */
    wxPGProperty* BaseGetPropertyByName( wxPGPropNameStr name ) const;

    /** Called in, for example, wxPropertyGrid::Clear. */
    void Clear();

    void ClearModifiedStatus( wxPGProperty* p );

    static void ClearPropertyAndChildrenFlags( wxPGProperty* p, long flags );
    static void SetPropertyAndChildrenFlags( wxPGProperty* p, long flags );

    inline bool ClearSelection()
    {
        return DoSelectProperty(NULL);
    }

    bool DoCollapse( wxPGProperty* p, int* pHeightAdd = NULL );

    bool DoExpand( wxPGProperty* p, int* pHeightAdd = NULL );

    /** Makes sure all columns have minimum width.
    */
    void CheckColumnWidths();

    /** Override this member function to add custom behaviour on property deletion.
    */
    virtual void DoDelete( wxPGProperty* item );

    wxPGProperty* DoGetItemAtY( int y ) const;

    /** Override this member function to add custom behaviour on property insertion.
    */
    virtual wxPGProperty* DoInsert( wxPGProperty* parent, int index, wxPGProperty* property );

    /** This needs to be overridden in grid used the manager so that splitter changes
        can be propagated to other pages.
    */
    virtual void DoSetSplitterPosition( int pos, int splitterColumn = 0, bool allPages = true );

    bool EnableCategories( bool enable );

    /** Enables or disables given property and its subproperties. */
    bool DoEnableProperty( wxPGProperty* p, bool enable );

    unsigned int GetColumnCount() const
    {
        return m_colWidths.size();
    }

    wxPGProperty* GetSelection() const
    {
        return m_selected;
    }

    int GetColumnMinWidth( int column ) const;

    wxPropertyGrid* GetGrid() const { return m_pPropGrid; }

    /** Returns last item which could be iterated using given flags.
        \param flags
        @link iteratorflags List of iterator flags@endlink
    */
    wxPGProperty* GetLastItem( int flags = wxPG_ITERATE_DEFAULT );

    const wxPGProperty* GetLastItem( int flags = wxPG_ITERATE_DEFAULT ) const
    {
        return ((wxPropertyGridState*)this)->GetLastItem(flags);
    }

#if wxPG_COMPATIBILITY_1_2_0
    wxPGProperty* GetNextCategory( wxPGProperty* id ) const;

    wxPGProperty* GetNextProperty( wxPGProperty* id );

    wxPGProperty* GetNextSibling( wxPGProperty* id );

    wxPGProperty* GetPrevSibling( wxPGProperty* id );

    wxPGProperty* GetPrevProperty( wxPGProperty* id );
#endif // wxPG_COMPATIBILITY_1_2_0

    wxPropertyCategory* GetPropertyCategory( const wxPGProperty* p ) const;

    wxPGProperty* GetPropertyByLabel( const wxString& name, wxPGProperty* parent = NULL ) const;

    wxVariant DoGetPropertyValues( const wxString& listname, wxPGProperty* baseparent, long flags ) const;

    wxPGProperty* GetRoot() const { return m_properties; }

    int GetSplitterPosition( int splitterIndex ) const;

    // Returns combined width of margin and all the columns
    int GetVirtualWidth() const
    {
        return m_width;
    }

    /** Used by SetSplitterLeft. */
    int GetLeftSplitterPos( wxClientDC& dc, wxPGProperty* pwc, bool subProps );

    /** Returns information about arbitrary position in the grid.
    */
    wxPropertyGridHitTestResult HitTest( const wxPoint& pt ) const;

    inline bool IsDisplayed() const;

    inline bool IsInNonCatMode() const { return (bool)(m_properties == m_abcArray); }

    /** Only inits arrays, doesn't migrate things or such. */
    void InitNonCatMode ();

    void DoLimitPropertyEditing ( wxPGProperty* p, bool limit = true )
    {
        if ( limit )
            p->m_flags |= wxPG_PROP_NOEDITOR;
        else
            p->m_flags &= ~(wxPG_PROP_NOEDITOR);
    }

    bool DoSelectProperty( wxPGProperty* p, unsigned int flags = 0 );

    /** widthChange is non-client.
    */
    void OnClientWidthChange( int newWidth, int widthChange );

    void SetColumnCount( int colCount );

    void PropagateColSizeDec( int column, int decrease, int dir );

    bool HideProperty( wxPGProperty* p, bool hide );

    bool DoSetPropertyValueString( wxPGProperty* p, const wxString& value );

    bool DoSetPropertyValue( wxPGProperty* p, wxVariant& value );

    bool DoSetPropertyValueWxObjectPtr( wxPGProperty* p, wxObject* value );
    void DoSetPropertyValues( const wxVariantList& list, wxPGProperty* default_category );

    void DoSetPropertyUnspecified( wxPGProperty* p );

#ifdef wxPG_COMPATIBILITY_1_0_0
    #define SetPropertyValueUnspecified SetPropertyUnspecified
#endif

    void SetSplitterLeft( bool subProps = false );

    /** Set virtual width for this particular page. */
    void SetVirtualWidth( int width );

    void Sort( wxPGProperty* p );
    void Sort();

    void SetSelection( wxPGProperty* p ) { m_selected = p; }

protected:

    /** Returns column at x coordinate (in GetGrid()->GetPanel()).
        \param pSplitterHit
        Give pointer to int that receives index to splitter that is at x.
        \param pSplitterOffset
        Distance from said splitter.
    */
    int HitTestH( int x, int* pSplitterHit, int* pSplitterHitOffset ) const;

#ifndef DOXYGEN
    int PrepareToAddItem ( wxPGProperty* property, wxPGProperty* scheduledParent );

    /** If visible, then this is pointer to wxPropertyGrid.
        This shall *never* be NULL to indicate that this state is not visible.
    */
    wxPropertyGrid*             m_pPropGrid;

    /** Pointer to currently used array. */
    wxPGProperty*               m_properties;

    /** Array for categoric mode. */
    wxPGRootProperty            m_regularArray;

    /** Array for root of non-categoric mode. */
    wxPGRootProperty*           m_abcArray;

    /** Dictionary for name-based access. */
    wxPGHashMapS2P              m_dictName;

    /** List of column widths (first column does not include margin). */
    wxArrayInt                  m_colWidths;

    double                      m_fSplitterX;

    /** Most recently added category. */
    wxPropertyCategory*         m_currentCategory;

    /** Pointer to selected property. */
    wxPGProperty*               m_selected;

    /** Virtual width. */
    int                         m_width;

    /** Indicates total virtual height of visible properties. */
    unsigned int                m_bottomY;

    /** 1 if m_lastCaption is also the bottommost caption. */
    unsigned char               m_lastCaptionBottomnest;

    /** 1 items appended/inserted, so stuff needs to be done before drawing;
        If m_bottomy == 0, then calcylatey's must be done.
        Otherwise just sort.
    */
    unsigned char               m_itemsAdded;

    /** 1 if any value is modified. */
    unsigned char               m_anyModified;

#endif // #ifndef DOXYGEN

};

#endif // #ifndef SWIG

inline bool wxPGProperty::SetChoices( const wxArrayString& labels,
                                      const wxArrayInt& values )
{
    wxPGChoices chs(labels,values);
    return SetChoices(chs);
}

// -----------------------------------------------------------------------

// GetPropertyName version used internally. Use GetPropertyName for slight speed advantage,
// or GetPropertyNameA for nice assertion (essential for wxPython bindings).
#define GetPropertyByNameI          GetPropertyByNameA

// FOR BACKWARDS COMPATIBILITY
#define GetPropertyByNameWithAssert GetPropertyByNameA


// Helper macro that does necessary preparations when calling
// some wxPGProperty's member function.
#define wxPG_PROP_ARG_CALL_PROLOG_0(PROPERTY) \
    PROPERTY *p = (PROPERTY*)id.GetPtr(this); \
    if ( !p ) return;

#define wxPG_PROP_ARG_CALL_PROLOG_RETVAL_0(PROPERTY, RETVAL) \
    PROPERTY *p = (PROPERTY*)id.GetPtr(this); \
    if ( !p ) return RETVAL;

#define wxPG_PROP_ARG_CALL_PROLOG() \
    wxPG_PROP_ARG_CALL_PROLOG_0(wxPGProperty)

#define wxPG_PROP_ARG_CALL_PROLOG_RETVAL(RVAL) \
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL_0(wxPGProperty, RVAL)

#define wxPG_PROP_ID_CONST_CALL_PROLOG() \
    wxPG_PROP_ARG_CALL_PROLOG_0(const wxPGProperty)

#define wxPG_PROP_ID_CONST_CALL_PROLOG_RETVAL(RVAL) \
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL_0(const wxPGProperty, RVAL)


/** \class wxPropertyContainerMethods
    \ingroup classes
    \brief In order to have most same base methods, both wxPropertyGrid and
    wxPropertyGridManager must derive from this.
*/
class WXDLLIMPEXP_PG wxPropertyContainerMethods
// BM_METHODS
{
public:

    /** Destructor */
    virtual ~wxPropertyContainerMethods() { };

    /** Adds choice to a property that can accept one.
        \remarks
        - If you need to make sure that you modify only the set of choices of
          a single property (and not also choices of other properties with initially
          identical set), call wxPropertyGrid::SetPropertyChoicesPrivate.
        - This usually only works for wxEnumProperty and derivatives (wxFlagsProperty
          can get accept new items but its items may not get updated).
    */
    void AddPropertyChoice( wxPGPropArg id, const wxString& label, int value = wxPG_INVALID_VALUE );

    /** Appends property to the list. wxPropertyGrid assumes ownership of the object.
        Becomes child of most recently added category.
        \remarks
        - wxPropertyGrid takes the ownership of the property pointer.
        - If appending a category with name identical to a category already in the
          wxPropertyGrid, then newly created category is deleted, and most recently
          added category (under which properties are appended) is set to the one with
          same name. This allows easier adding of items to same categories in multiple
          passes.
        - Does not automatically redraw the control, so you may need to call Refresh
          when calling this function after control has been shown for the first time.
    */
    wxPGProperty* Append( wxPGProperty* property );

#if wxPG_COMPATIBILITY_1_2_0
    wxDEPRECATED( wxPGProperty* AppendCategory( const wxString& label, const wxString& name = wxPG_LABEL ) );
#endif

    wxPGProperty* AppendIn( wxPGPropArg id, wxPGProperty* newproperty );

    /** Inorder to add new items into a property with fixed children (for instance, wxFlagsProperty),
        you need to call this method. After populating has been finished, you need to call EndAddChildren.
    */
    void BeginAddChildren( wxPGPropArg id );

    /** Called after population of property with fixed children has finished.
    */
    void EndAddChildren( wxPGPropArg id );

    /** Inserts choice to a property that can accept one.

        See AddPropertyChoice for more details.
    */
    void InsertPropertyChoice( wxPGPropArg id, const wxString& label, int index, int value = wxPG_INVALID_VALUE );

    /** Deselect current selection, if any. Returns true if success
        (ie. validator did not intercept). */
    bool ClearSelection();

    /** Collapses given category or property with children.
        Returns true if actually collapses.
    */
    bool Collapse( wxPGPropArg id );

#if wxPG_COMPATIBILITY_1_2_0
    /** Collapses all items that can be collapsed.
        DEPRECATED: Removed due to rare use case. Please instead do a small loop
        that iterates through the property container using GetVIterator(wxPG_ITERATE_ALL).
        \retval
        Return false if failed (may fail if editor value cannot be validated).
    */
    bool CollapseAll() { return ExpandAll(false); }
#endif

    /** Deletes choice from a property.

        If selected item is deleted, then the value is set to unspecified.

        See AddPropertyChoice for more details.
    */
    void DeletePropertyChoice( wxPGPropArg id, int index );

    /** Resets value of a property to its default. */
    bool ClearPropertyValue( wxPGPropArg id )
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(false)
        p->SetValue(p->GetDefaultValue());
        RefreshProperty(p);
        return true;
    }

    /** Constructs a property. Class used is given as the first
        string argument. It may be either normal property class
        name, such as "wxIntProperty" or a short one such as
        "Int".
    */
    static wxPGProperty* CreatePropertyByClass(const wxString &classname,
                                               const wxString &label,
                                               const wxString &name);

    /** Constructs a property. Value type name used is given as the first
        string argument. It may be "string", "long", etc. Any value returned
        by wxVariant::GetType fits there.

        Otherwise, this is similar as CreatePropertyByClass.
        \remarks
        <b>Cannot</b> generate property category.
    */
    static wxPGProperty* CreatePropertyByType(const wxString &valuetype,
                                              const wxString &label,
                                              const wxString &name);

    /** Deletes a property by id. If category is deleted, all children are automatically deleted as well. */
    void DeleteProperty( wxPGPropArg id );

#if wxPG_COMPATIBILITY_1_2_0
    /** Deletes a property by id. If category is deleted, all children are automatically deleted as well.
        DEPRECATED: Use DeleteProperty() instead.
    */
    wxDEPRECATED( void Delete( wxPGPropArg id ) );
#endif

#if wxPG_COMPATIBILITY_1_2_0
    /** Disables property. */
    wxDEPRECATED( bool Disable( wxPGPropArg id ) );
#endif

    /** Disables property. */
    bool DisableProperty( wxPGPropArg id ) { return EnableProperty(id,false); }

    /** Enables or disables property, depending on whether enable is true or false. */
    bool EnableProperty( wxPGPropArg id, bool enable = true );

    /** Expands given category or property with children.
        Returns true if actually expands.
    */
    bool Expand( wxPGPropArg id );

#if wxPG_COMPATIBILITY_1_2_0
    /** Expands all items that can be expanded.
        DEPRECATED: Removed due to rare use case. Please instead do a small loop
        that iterates through the property container using GetVIterator(wxPG_ITERATE_ALL).
    */
    bool ExpandAll( bool expand = true );
#endif

    /** Returns list of expanded properties.
        \remarks
        If wxPG_USE_STL is 1, then wxArrayPGProperty is typedef of
        std::vector<wxPGProperty*>.
    */
    wxArrayPGProperty GetExpandedProperties() const
    {
        wxArrayPGProperty array;
        GetPropertiesWithFlag(&array, wxPG_PROP_COLLAPSED, true,
            wxPG_ITERATE_ALL_PARENTS_RECURSIVELY|wxPG_ITERATE_HIDDEN);
        return array;
    }

    /** Returns id of first child of given property.
        \remarks
        Does not return sub-properties!
    */
    inline wxPGId GetFirstChild( wxPGPropArg id )
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(wxNullProperty)

        if ( !p->GetChildCount() || p->IsFlagSet(wxPG_PROP_AGGREGATE) )
            return wxNullProperty;

        return wxPGIdGen(p->Item(0));
    }

    /** Returns iterator class instance.
        \param flags
        wxPropertyGridIterator flags. If default wxPG_ITERATE_DEFAULT supplied,
        everything except fixed subproperties are iterated.
        @link iteratorflags List of iterator flags@endlink
        \param firstProp
        Property to start iteration from. If NULL, then first child of root is used.
    */
    wxPropertyGridIterator GetIterator( int flags = wxPG_ITERATE_DEFAULT, wxPGProperty* firstProp = NULL )
    {
        return wxPropertyGridIterator( m_pState, flags, firstProp );
    }

    wxPropertyGridConstIterator GetIterator( int flags = wxPG_ITERATE_DEFAULT, wxPGProperty* firstProp = NULL ) const
    {
        return wxPropertyGridConstIterator( m_pState, flags, firstProp );
    }

    /** Returns iterator class instance.
        \param flags
        wxPropertyGridIterator flags. If default wxPG_ITERATE_DEFAULT supplied,
        everything except fixed subproperties are iterated.
        @link iteratorflags List of iterator flags@endlink
        \param startPos
        Either wxTOP or wxBOTTOM. wxTOP will indicate that iterations start from
        the first property from the top, and wxBOTTOM means that the iteration will
        instead begin from bottommost valid item.
    */
    wxPropertyGridIterator GetIterator( int flags, int startPos )
    {
        return wxPropertyGridIterator( m_pState, flags, startPos );
    }

    wxPropertyGridConstIterator GetIterator( int flags, int startPos ) const
    {
        return wxPropertyGridConstIterator( m_pState, flags, startPos );
    }

    /** Similar to GetIterator, but instead returns wxPGVIterator instance,
        which can be useful for forward-iterating through arbitrary property
        containers.
    */
    virtual wxPGVIterator GetVIterator( int flags ) const;

    /** Returns id of first item, whether it is a category or property.
        \param flags
        @link iteratorflags List of iterator flags@endlink
    */
    wxPGProperty* GetFirst( int flags = wxPG_ITERATE_ALL )
    {
        wxPropertyGridIterator it( m_pState, flags, wxNullProperty, 1 );
        return *it;
    }

    const wxPGProperty* GetFirst( int flags = wxPG_ITERATE_ALL ) const
    {
        return ((wxPropertyContainerMethods*)this)->GetFirst(flags);
    }

#if wxPG_COMPATIBILITY_1_2_0
    /** Returns next item under the same parent. */
    inline wxPGId GetNextSibling( wxPGPropArg id )
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(wxNullProperty)
        return m_pState->GetNextSibling(p);
    }
#endif

    /** Returns map-like storage of property's attributes.
        \remarks
        Note that if extra style wxPG_EX_WRITEONLY_BUILTIN_ATTRIBUTES is set,
        then builtin-attributes are not included in the storage.
    */
    const wxPGAttributeStorage& GetPropertyAttributes( wxPGPropArg id ) const
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(*((const wxPGAttributeStorage*)NULL));
        return p->GetAttributes();
    }

    /** Sets attributes from a wxPGAttributeStorage.
    */
    void SetPropertyAttributes( wxPGPropArg id, const wxPGAttributeStorage& attributes )
    {
        wxPG_PROP_ARG_CALL_PROLOG()
        p->SetAttributes(attributes);
    }

    /** Adds to 'targetArr' pointers to properties that have given
        flags 'flags' set. However, if 'inverse' is set to true, then
        only properties without given flags are stored.
        \param flags
        Property flags to use.
        \param iterflags
        Iterator flags to use. Default is everything expect private children.
        \remarks
        If wxPG_USE_STL is 1, then wxArrayPGProperty is typedef of
        std::vector<wxPGProperty*>.
    */
    void GetPropertiesWithFlag( wxArrayPGProperty* targetArr,
                                wxPGProperty::FlagType flags,
                                bool inverse = false,
                                int iterFlags = (wxPG_ITERATE_PROPERTIES|wxPG_ITERATE_HIDDEN|wxPG_ITERATE_CATEGORIES) ) const;

    /** Returns value of given attribute. If none found, returns NULL-variant.
    */
    wxVariant GetPropertyAttribute( wxPGPropArg id, const wxString& attrName ) const
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(wxNullVariant)
        return p->GetAttribute(attrName);
    }

    /** Returns id of property with given name (case-sensitive). If there is no
        property with such name, returned property id is invalid ( i.e. it will return
        false with IsOk method).
        \remarks
        - Sub-properties (i.e. properties which have parent that is not category or
          root) can not be accessed globally by their name. Instead, use
          "<property>.<subproperty>" in place of "<subproperty>".
    */
    wxPGProperty* GetPropertyByName( wxPGPropNameStr name ) const;

    /** Returns id of a sub-property 'subname' of property 'name'. Same as
        calling GetPropertyByNameI(wxT("name.subname")), albeit slightly faster.
    */
    wxPGProperty* GetPropertyByName( wxPGPropNameStr name, wxPGPropNameStr subname ) const;

    /** Returns writable reference to property's list of choices (and relevant
        values). If property does not have any choices, will return reference
        to an invalid set of choices that will return false on IsOk call.
    */
    wxPGChoices& GetPropertyChoices( wxPGPropArg id );

    /** Gets name of property's constructor function. */
    inline wxPG_CONST_WXCHAR_PTR GetPropertyClassName( wxPGPropArg id ) const
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(wxPG_CONST_WXCHAR_DEFVAL)
        return p->GetClassName();
    }

    /** Returns client data (void*) of a property. */
    inline wxPGProperty::ClientDataType GetPropertyClientData( wxPGPropArg id ) const
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(NULL)
        return p->GetClientData();
    }
    /** Returns property's editor. */
    inline const wxPGEditor* GetPropertyEditor( wxPGPropArg id ) const
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(NULL)
        return p->GetEditorClass();
    }

    /** Returns property's custom value image (NULL of none). */
    inline wxBitmap* GetPropertyImage( wxPGPropArg id ) const
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(NULL)
        if ( p->GetDataExt() )
            return p->GetDataExt()->m_valueBitmap;
        return (wxBitmap*) NULL;
    }

    /** Returns property's position under its parent. */
    inline unsigned int GetPropertyIndex( wxPGPropArg id )
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(INT_MAX)
        return p->GetIndexInParent();
    }

    /** Returns label of a property. */
    inline const wxString& GetPropertyLabel( wxPGPropArg id )
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(m_emptyString)
        return p->GetLabel();
    }
    /** Returns name of a property, by which it is globally accessible. */
    inline wxString GetPropertyName( wxPGPropArg id )
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(m_emptyString)
        return p->GetName();
    }

    /** Returns parent item of a property. */
    inline wxPGId GetPropertyParent( wxPGPropArg id )
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(wxNullProperty)
        return p->GetParent();
    }

#if wxPG_COMPATIBILITY_1_2_0
    /** Returns pointer to a property.
    */
    wxPGProperty* GetPropertyPtr( wxPGPropArg id ) const
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(NULL)
        return wxPGIdToPtr(p);
    }
#endif

    /** Returns help string associated with a property. */
    inline wxString GetPropertyHelpString( wxPGPropArg id ) const
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(m_emptyString)
        return p->GetHelpString();
    }

    /** Returns short name for property's class. For example,
        "wxPropertyCategory" translates to "Category" and "wxIntProperty"
        to "Int".
    */
    wxString GetPropertyShortClassName( wxPGPropArg id );

#if wxUSE_VALIDATORS
    /** Returns validator of a property as a reference, which you
        can pass to any number of SetPropertyValidator.
    */
    inline wxValidator* GetPropertyValidator( wxPGPropArg id )
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(NULL)
        return p->GetValidator();
    }
#endif

#ifndef SWIG
    /** Returns value as wxVariant. To get wxObject pointer from it,
        you will have to use WX_PG_VARIANT_TO_WXOBJECT(VARIANT,CLASSNAME) macro.

        If property value is unspecified, Null variant is returned.
    */
    inline wxVariant GetPropertyValue( wxPGPropArg id )
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(wxVariant())
        return p->GetValue();
    }
#endif

    wxString GetPropertyValueAsString( wxPGPropArg id ) const;
    long GetPropertyValueAsLong( wxPGPropArg id ) const;
    unsigned long GetPropertyValueAsULong( wxPGPropArg id ) const
    {
        return (unsigned long) GetPropertyValueAsLong(id);
    }
#ifndef SWIG
    inline int GetPropertyValueAsInt( wxPGPropArg id ) const { return (int)GetPropertyValueAsLong(id); }
#endif
    bool GetPropertyValueAsBool( wxPGPropArg id ) const;
    double GetPropertyValueAsDouble( wxPGPropArg id ) const;
    wxObject* GetPropertyValueAsWxObjectPtr( wxPGPropArg id ) const;
    void* GetPropertyValueAsVoidPtr( wxPGPropArg id ) const;

#define wxPG_PROP_ID_GETPROPVAL_CALL_PROLOG_RETVAL(TYPENAME, DEFVAL) \
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(DEFVAL) \
    if ( p->m_value.GetType() != TYPENAME ) \
    { \
        wxPGGetFailed(p,TYPENAME); \
        return DEFVAL; \
    }

#define wxPG_PROP_ID_GETPROPVAL_CALL_PROLOG_RETVAL_WFALLBACK(TYPENAME, DEFVAL) \
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(DEFVAL) \
    if ( p->m_value.GetType() != TYPENAME ) \
        return DEFVAL; \

    inline wxArrayString GetPropertyValueAsArrayString( wxPGPropArg id ) const
    {
        wxPG_PROP_ID_GETPROPVAL_CALL_PROLOG_RETVAL(wxT("arrstring"), wxArrayString())
        return p->m_value.GetArrayString();
    }

    inline wxPoint GetPropertyValueAsPoint( wxPGPropArg id ) const
    {
        wxPG_PROP_ID_GETPROPVAL_CALL_PROLOG_RETVAL(wxT("wxPoint"), wxPoint())
        return WX_PG_VARIANT_GETVALUEREF(p->GetValue(), wxPoint);
    }

    inline wxSize GetPropertyValueAsSize( wxPGPropArg id ) const
    {
        wxPG_PROP_ID_GETPROPVAL_CALL_PROLOG_RETVAL(wxT("wxSize"), wxSize())
        return WX_PG_VARIANT_GETVALUEREF(p->GetValue(), wxSize);
    }

    wxLongLong_t GetPropertyValueAsLongLong( wxPGPropArg id ) const
    {
        wxPG_PROP_ID_GETPROPVAL_CALL_PROLOG_RETVAL_WFALLBACK(wxT("wxLongLong"), (long) GetPropertyValueAsLong(id))
        return WX_PG_VARIANT_GETVALUEREF(p->GetValue(), wxLongLong).GetValue();
    }

    wxULongLong_t GetPropertyValueAsULongLong( wxPGPropArg id ) const
    {
        wxPG_PROP_ID_GETPROPVAL_CALL_PROLOG_RETVAL_WFALLBACK(wxT("wxULongLong"), (unsigned long) GetPropertyValueAsULong(id))
        return WX_PG_VARIANT_GETVALUEREF(p->GetValue(), wxULongLong).GetValue();
    }

    inline wxArrayInt GetPropertyValueAsArrayInt( wxPGPropArg id ) const
    {
        wxPG_PROP_ID_GETPROPVAL_CALL_PROLOG_RETVAL(wxT("wxArrayInt"), wxArrayInt())
        wxArrayInt arr = WX_PG_VARIANT_GETVALUEREF(p->GetValue(), wxArrayInt);
        return arr;
    }

#if wxUSE_DATETIME
    inline wxDateTime GetPropertyValueAsDateTime( wxPGPropArg id ) const
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(wxDateTime())

        if ( wxStrcmp(p->m_value.GetType(),wxT("datetime")) != 0 )
        {
            wxPGGetFailed(p,wxT("datetime"));
            return wxDateTime();
        }
        return p->m_value.GetDateTime();
    }
#endif

#ifdef __WXPYTHON__
    PyObject* GetPropertyValueAsPyObject( wxPGPropArg id ) const;
#endif

#ifndef SWIG
    /** Returns a wxVariant list containing wxVariant versions of all
        property values. Order is not guaranteed.
        \param flags
        Use wxPG_KEEP_STRUCTURE to retain category structure; each sub
        category will be its own wxVariantList of wxVariant.
        Use wxPG_INC_ATTRIBUTES to include property attributes as well.
        Each attribute will be stored as list variant named @<propname>@attr.
        \remarks
    */
    wxVariant GetPropertyValues( const wxString& listname = wxEmptyString,
        wxPGId baseparent = wxPGIdGen((wxPGProperty*)NULL), long flags = 0 ) const
    {
        return m_pState->DoGetPropertyValues(listname,wxPGIdToPtr(baseparent),flags);
    }
#endif

    wxString GetPropertyValueType( wxPGPropArg id )
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(m_emptyString)
        return p->GetType();
    }

#ifdef __WXPYTHON__
    size_t GetPVTI( wxPGPropArg id )
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(1)
        wxVariantData* data = p->GetValue().GetData();
        if ( !data )
            return 0;
        return (size_t)wxPGVariantDataGetClassInfo(data);;
    }
#endif

    /** Returns currently selected property. */
    wxPGProperty* GetSelection() const
    {
        return m_pState->GetSelection();
    }

#ifndef SWIG
    inline wxPropertyGridState* GetState() const { return m_pState; }
#endif

    /** Hides or reveals a property.
        \param hide
        If true, hides property, otherwise reveals it.
    */
    bool HideProperty( wxPGPropArg id, bool hide = true );

#if wxPG_INCLUDE_ADVPROPS
    /** Initializes additional property editors (SpinCtrl etc.). Causes references
        to most object files in the library, so calling this may cause significant increase
        in executable size when linking with static library.
    */
    static void RegisterAdditionalEditors();
#else
    static inline void RegisterAdditionalEditors() { }
#endif

#if wxPG_INCLUDE_ADVPROPS
    /** Initializes *all* property types. Causes references to most object
        files in the library, so calling this may cause significant increase
        in executable size when linking with static library.
    */
    static void InitAllTypeHandlers();
#else
    static inline void InitAllTypeHandlers() { }
#endif

    /** Inserts property to the property container.

        \param priorthis
        New property is inserted just prior to this. Available only
        in the first variant. There are two versions of this function
        to allow this parameter to be either an id or name to
        a property.

        \param parent
        New property is inserted under this category. Available only
        in the second variant. There are two versions of this function
        to allow this parameter to be either an id or name to
        a property.

        \param index
        Index under category. Available only in the second variant.
        If index is < 0, property is appended in category.

        \param newproperty
        Pointer to the inserted property. wxPropertyGrid will take
        ownership of this object.

        \return
        Returns id for the property,

        \remarks

        - wxPropertyGrid takes the ownership of the property pointer.

        While Append may be faster way to add items, make note that when
        both data storages (categoric and
        non-categoric) are active, Insert becomes even more slow. This is
        especially true if current mode is non-categoric.

        Example of use:

        \code

            // append category
            wxPGId my_cat_id = propertygrid->Append( new wxPropertyCategory (wxT("My Category")) );

            ...

            // insert into category - using second variant
            wxPGId my_item_id_1 = propertygrid->Insert( my_cat_id, 0, new wxStringProperty(wxT("My String 1")) );

            // insert before to first item - using first variant
            wxPGId my_item_id_2 = propertygrid->Insert( my_item_id, new wxStringProperty(wxT("My String 2")) );

        \endcode

    */
    wxPGProperty* Insert( wxPGPropArg priorThis, wxPGProperty* newproperty );

    /** @link wxPropertyGrid::Insert Insert @endlink */
    wxPGProperty* Insert( wxPGPropArg parent, int index, wxPGProperty* newproperty );

#if wxPG_COMPATIBILITY_1_2_0
    wxPGProperty* InsertCategory( wxPGPropArg id, int index, const wxString& label, const wxString& name = wxPG_LABEL )
    {
        return Insert( id, index, new wxPropertyCategory(label,name) );
    }
#endif

    /** Returns true if property is enabled. */
    inline bool IsPropertyEnabled( wxPGPropArg id ) const
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(false)
        return (!(p->GetFlags() & wxPG_PROP_DISABLED))?true:false;
    }

    /** Returns true if property is shown (ie. hideproperty with true not called for it). */
    inline bool IsPropertyShown( wxPGPropArg id ) const
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(false)
        return (!(p->GetFlags() & wxPG_PROP_HIDDEN))?true:false;
    }

#if wxPG_COMPATIBILITY_1_2_0
    /** Returns true if property's value type has name typestr.
        DEPRECATED: Use GetPropertyValueType() and wxString comparison instead.
    */
    wxDEPRECATED( bool IsPropertyValueType( wxPGPropArg id, const wxChar* typestr ) const );
#endif

    /** Returns true if given property is expanded. Naturally, always returns false
        for properties that cannot be expanded.
    */
    bool IsPropertyExpanded( wxPGPropArg id ) const;

    /** Returns true if property is of certain type.
        \param info
        Preferably use WX_PG_CLASSINFO(PROPERTYNAME). Alternative is
        PROPERTYNAMEClassInfo.
    */
    inline bool IsPropertyKindOf( wxPGPropArg id, wxClassInfo* info ) const
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(false)
        return p->IsKindOf(info);
    }

#if wxPG_COMPATIBILITY_1_2_0
    /** Returns true if property has been modified after value set or modify flag
        clear by software.

        NOTE: This function is deprecated. Use IsPropertyModified instead.
    */
    wxDEPRECATED( bool IsModified( wxPGPropArg id ) const );
#endif

    /** Returns true if property is a category. */
    inline bool IsPropertyCategory( wxPGPropArg id ) const
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(false)
        return p->IsCategory();
    }

    /** Returns true if property has been modified after value set or modify flag
        clear by software.
    */
    inline bool IsPropertyModified( wxPGPropArg id ) const
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(false)
        return ( (p->GetFlags() & wxPG_PROP_MODIFIED) ? true : false );
    }

    /** Returns true if property value is set to unspecified.
    */
#ifdef wxPG_COMPATIBILITY_1_0_0
    inline bool IsPropertyValueUnspecified( wxPGPropArg id ) const
#else
    inline bool IsPropertyUnspecified( wxPGPropArg id ) const
#endif
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(false)
        return p->IsValueUnspecified();
    }

    /** Disables (limit = true) or enables (limit = false) wxTextCtrl editor of a property,
        if it is not the sole mean to edit the value.
    */
    void LimitPropertyEditing( wxPGPropArg id, bool limit = true );

    /** Converts array of properties to array of names.
        \remarks
        If wxPG_USE_STL is 1, then wxArrayPGProperty is typedef of
        std::vector<wxPGProperty*>.
    */
    void PropertiesToNames( wxArrayString* names, const wxArrayPGProperty& properties ) const;

    /** Converts array of names to array of property pointers.
        \remarks
        If wxPG_USE_STL is 1, then wxArrayPGProperty is typedef of
        std::vector<wxPGProperty*>.
    */
    void NamesToProperties( wxArrayPGProperty* properties, const wxArrayString& names ) const;

    /** If state is shown in it's grid, refresh it now.
    */
    virtual void RefreshGrid( wxPropertyGridState* state = NULL );

    /** Replaces property with id with newly created property. For example,
        this code replaces existing property named "Flags" with one that
        will have different set of items:
        \code
            pg->ReplaceProperty(wxT("Flags"),
                wxFlagsProperty(wxT("Flags"),wxPG_LABEL,newItems))
        \endcode
        For more info, see wxPropertyGrid::Insert.
    */
    wxPGId ReplaceProperty( wxPGPropArg id, wxPGProperty* property );

    /** Lets user to set the strings listed in the choice dropdown of a wxBoolProperty.
        Defaults are "True" and "False", so changing them to, say, "Yes" and "No" may
        be useful in some less technical applications.
    */
    static void SetBoolChoices( const wxString& true_choice, const wxString& false_choice );

    /** Sets all properties in given array as expanded.
        \param expand
        False if you want to collapse properties instead.
        \remarks
        If wxPG_USE_STL is 1, then wxArrayPGProperty is typedef of
        std::vector<wxPGProperty*>.
    */
    void SetExpandedProperties( wxArrayPGProperty& array, bool expand = true )
    {
        SetPropertiesFlag(array, wxPG_PROP_COLLAPSED, expand);
    }

    /** Sets or clears flag(s) of all properties in given array.
        \param flags
        Property flags to set or clear.
        \param inverse
        Set to true if you want to clear flag instead of setting them.
        \remarks
        If wxPG_USE_STL is 1, then wxArrayPGProperty is typedef of
        std::vector<wxPGProperty*>.
    */
    void SetPropertiesFlag( wxArrayPGProperty& srcArr, wxPGProperty::FlagType flags,
                            bool inverse = false );

    /** Sets text, bitmap, and colours for given column's cell.

        \remarks
        - You can set label cell by setting column to 0.
        - You can use wxPG_LABEL as text to use default text for column.
    */
    void SetPropertyCell( wxPGPropArg id,
                          int column,
                          const wxString& text = wxEmptyString,
                          const wxBitmap& bitmap = wxNullBitmap,
                          const wxColour& fgCol = wxNullColour,
                          const wxColour& bgCol = wxNullColour )
    {
        wxPG_PROP_ARG_CALL_PROLOG()
        p->SetCell( column, new wxPGCell(text, bitmap, fgCol, bgCol) );
    }

    /** Set choices of a property to specified set of labels and values.
    */
    inline void SetPropertyChoices( wxPGPropArg id, wxPGChoices& choices)
    {
        wxPG_PROP_ARG_CALL_PROLOG()
        p->SetChoices(choices);
    }


    /** If property's set of choices is shared, then calling this method converts
        it to private.
    */
    inline void SetPropertyChoicesExclusive( wxPGPropArg id )
    {
        wxPG_PROP_ARG_CALL_PROLOG()
        p->SetChoicesExclusive();
    }

    /** Sets an attribute of a property. Ids and relevants values are totally
        specific to property classes and may affect either the given instance
        or all instances of that class. See \ref attrids for list of built-in
        attributes.
        \param argFlags
        Optional. Use wxPG_RECURSE to set the attribute to child properties
        as well.
        \remarks
        wxVariant doesn't have int constructor (as of 2.5.4), so <b>you will
        need to cast int values (including most numeral constants) to long</b>.
    */
    inline void SetPropertyAttribute( wxPGPropArg id, const wxString& attrName, wxVariant value, long argFlags = 0 )
    {
        DoSetPropertyAttribute(id,attrName,value,argFlags);
    }

#ifndef SWIG
    /** Sets editor control of a property. As editor argument, use
        wxPG_EDITOR(EditorName), where basic built-in editor names are TextCtrl, Choice,
        ComboBox, CheckBox, TextCtrlAndButton, and ChoiceAndButton. Additional editors
        include SpinCtrl and DatePickerCtrl, which also require wxPropertyGrid::RegisterAdditionalEditors()
        call prior using.
    */
    inline void SetPropertyEditor( wxPGPropArg id, const wxPGEditor* editor )
    {
        wxPG_PROP_ARG_CALL_PROLOG()
        wxCHECK_RET( editor, wxT("unknown/NULL editor") );
        p->SetEditor(editor);
        RefreshProperty(p);
    }
#endif // #ifndef SWIG

    /** Sets editor control of a property. As editor argument, use
        editor name string, such as wxT("TextCtrl") or wxT("Choice").
    */
    inline void SetPropertyEditor( wxPGPropArg id, const wxString& editorName )
    {
        SetPropertyEditor(id,GetEditorByName(editorName));
    }

    /** Sets label of a property.
        \remarks
        This is the only way to set property's name. There is not
        wxPGProperty::SetLabel() method.
    */
    void SetPropertyLabel( wxPGPropArg id, const wxString& newproplabel );

    /** Sets client data (void*) of a property.
        \remarks
        This untyped client data has to be deleted manually.
    */
    inline void SetPropertyClientData( wxPGPropArg id, wxPGProperty::ClientDataType clientData )
    {
        wxPG_PROP_ARG_CALL_PROLOG()
        p->SetClientData(clientData);
    }

    /** Sets property's value to unspecified. If it has children (it may be category),
        then the same thing is done to them.
    */
    void SetPropertyUnspecified( wxPGPropArg id );

    /** Sets property to have read-only value. In other words, user cannot change
        the value in the editor, but they can still copy it.
        \remarks
        This is mainly for use with textctrl editor. Not all other editors fully
        support it.
    */
    void SetPropertyReadOnly( wxPGPropArg id, bool set = true )
    {
        wxPG_PROP_ARG_CALL_PROLOG()
        if ( set )
            p->SetFlag(wxPG_PROP_READONLY);
        else
            p->ClearFlag(wxPG_PROP_READONLY);
    }

#ifndef SWIG
    /** Sets various property values from a list of wxVariants. If property with
        name is missing from the grid, new property is created under given default
        category (or root if omitted).
    */
    void SetPropertyValues( const wxVariantList& list, wxPGPropArg defaultCategory = wxNullProperty )
    {
        wxPGProperty *p;
        if ( defaultCategory.HasName() ) p = defaultCategory.GetPtr(this);
        else p = defaultCategory.GetPtr0();
        m_pState->DoSetPropertyValues(list, p);
    }

    void SetPropertyValues( const wxVariant& list, wxPGPropArg defaultCategory = wxNullProperty )
    {
        SetPropertyValues(list.GetList(),defaultCategory);
    }
#endif

    /** Returns pointer of property's nearest parent category. If no category
        found, returns NULL.
    */
    wxPropertyCategory* GetPropertyCategory( wxPGPropArg id ) const
    {
        wxPG_PROP_ID_CONST_CALL_PROLOG_RETVAL(NULL)
        return m_pState->GetPropertyCategory(p);
    }

    /** Associates the help string with property.
        \remarks
        By default, text is shown either in the manager's "description"
        text box or in the status bar. If extra window style wxPG_EX_HELP_AS_TOOLTIPS
        is used, then the text will appear as a tooltip.
    */
    inline void SetPropertyHelpString( wxPGPropArg id, const wxString& helpString )
    {
        wxPG_PROP_ARG_CALL_PROLOG()
        p->SetHelpString(helpString);
    }

    /** Set wxBitmap in front of the value.
        \remarks
        - Bitmap will be scaled to a size returned by wxPropertyGrid::GetImageSize();
    */
    inline void SetPropertyImage( wxPGPropArg id, wxBitmap& bmp )
    {
        wxPG_PROP_ARG_CALL_PROLOG()
        p->SetValueImage(bmp);
        RefreshProperty(p);
    }

    /** Sets max length of property's text.
    */
    bool SetPropertyMaxLength( wxPGPropArg id, int maxLen );

#if wxUSE_VALIDATORS
    /** Sets validator of a property. For example
        \code
          // Allow property's value range from -100 to 100
          wxIntPropertyValidator validator(-100,100);
          wxPGId id = pg->Append( wxIntProperty(wxT("Value 1",wxPG_LABEL,0)) );
          pg->SetPropertyValidator( id, validator );
        \endcode
    */
    inline void SetPropertyValidator( wxPGPropArg id, const wxValidator& validator )
    {
        wxPG_PROP_ARG_CALL_PROLOG()
        p->SetValidator(validator);
    }
#endif

    /** Sets value (long integer) of a property.
    */
    inline void SetPropertyValueLong( wxPGPropArg id, long value )
    {
        wxVariant v(value);
        SetPropVal( id, v );
    }

#ifndef __WXPYTHON__
    /** Sets value (integer) of a property.
    */
    inline void SetPropertyValue( wxPGPropArg id, int value )
    {
        wxVariant v((long)value);
        SetPropVal( id, v );
    }
#endif
    /** Sets value (floating point) of a property.
    */
    inline void SetPropertyValueDouble( wxPGPropArg id, double value )
    {
        wxVariant v(value);
        SetPropVal( id, v );
    }
    /** Sets value (bool) of a property.
    */
    inline void SetPropertyValueBool( wxPGPropArg id, bool value )
    {
        wxVariant v(value);
        SetPropVal( id, v );
    }
#ifndef __WXPYTHON__
    inline void SetPropertyValue( wxPGPropArg id, const wxChar* value )
    {
        SetPropertyValueString( id, wxString(value) );
    }
#endif

    /** Sets value (wxArrayString) of a property.
    */
    inline void SetPropertyValueArrstr2( wxPGPropArg id, const wxArrayString& value )
    {
        wxVariant v(value);
        SetPropVal( id, v );
    }


#if wxUSE_DATETIME
    inline void SetPropertyValueDatetime( wxPGPropArg id, const wxDateTime& value )
    {
        wxVariant v(value);
        SetPropVal( id, v );
    }
#endif

    /** Sets value (wxString) of a property.
        \remarks
        This method uses wxPGProperty::SetValueFromString, which all properties
        should implement. This means that there should not be a type error,
        and instead the string is converted to property's actual value type.
    */
    void SetPropertyValueString( wxPGPropArg id, const wxString& value );

    /** Sets value (wxObject*) of a property.
    */
    inline void SetPropertyValueWxObjectPtr( wxPGPropArg id, wxObject* value )
    {
        wxVariant v(value);
        SetPropVal( id, v );
    }

#ifndef __WXPYTHON__
    /** Sets value (void*) of a property. */
    inline void SetPropertyValue( wxPGPropArg id, void* value )
    {
        wxVariant v(value);
        SetPropVal( id, v );
    }
    inline void SetPropertyValue ( wxPGPropArg id, wxObject& value )
    {
        wxVariant v(&value);
        SetPropVal( id, v );
    }

    /** Sets value (wxVariant&) of a property. */
    void SetPropertyValue( wxPGPropArg id, wxVariant value )
    {
        SetPropVal( id, value );
    }
#endif

    /** Sets value (wxPoint&) of a property.
    */
    inline void SetPropertyValuePoint( wxPGPropArg id, const wxPoint& value )
    {
        wxVariant v = WXVARIANT(value);
        SetPropVal( id, v );
    }
    /** Sets value (wxSize&) of a property.
    */
    inline void SetPropertyValueSize( wxPGPropArg id, const wxSize& value )
    {
        wxVariant v = WXVARIANT(value);
        SetPropVal( id, v );
    }
    /** Sets value (wxLongLong&) of a property.
    */
    inline void SetPropertyValueLongLong( wxPGPropArg id, wxLongLong_t value )
    {
        wxVariant v = WXVARIANT(wxLongLong(value));
        SetPropVal( id, v );
    }
    /** Sets value (wxULongLong&) of a property.
    */
    inline void SetPropertyValueULongLong( wxPGPropArg id, wxULongLong_t value )
    {
        wxVariant v = WXVARIANT(wxULongLong(value));
        SetPropVal( id, v );
    }
    /** Sets value (wxArrayInt&) of a property.
    */
    inline void SetPropertyValueArrint2( wxPGPropArg id, const wxArrayInt& value )
    {
        wxVariant v = WXVARIANT(value);
        SetPropVal( id, v );
    }
    /** Sets value (wxDateTime&) of a property.
    */
#ifdef __WXPYTHON__
    inline void SetPropertyValuePyObject( wxPGPropArg id, PyObject* value )
    {
        wxVariant v = WXVARIANT(value);
        SetPropVal( id, v );
    }
#endif

#ifndef SWIG
    /** Sets value (wxVariant&) of a property. Same as SetPropertyValue, but
        accepts reference. */
    void SetPropVal( wxPGPropArg id, wxVariant& value );
#endif

#ifdef SWIG
    %pythoncode {
        def MapType(class_,factory):
            "Registers Python type/class to property mapping.\n\nfactory: Property builder function/class."
            global _type2property
            try:
                mappings = _type2property
            except NameError:
                raise AssertionError("call only after a propertygrid or manager instance constructed")

            mappings[class_] = factory


        def DoDefaultTypeMappings(self):
            "Map built-in properties."
            global _type2property
            try:
                mappings = _type2property

                return
            except NameError:
                mappings = {}
                _type2property = mappings

            mappings[str] = StringProperty
            mappings[unicode] = StringProperty
            mappings[int] = IntProperty
            mappings[float] = FloatProperty
            mappings[bool] = BoolProperty
            mappings[list] = ArrayStringProperty
            mappings[tuple] = ArrayStringProperty
            mappings[wx.Font] = FontProperty
            mappings[wx.Colour] = ColourProperty
            mappings[wx.Size] = SizeProperty
            mappings[wx.Point] = PointProperty
            mappings[wx.FontData] = FontDataProperty


        def GetPropertyValue(self,p):
            "Returns Python object value for property.\n\nCaches getters on value type id basis for performance purposes."
            global _vt2getter
            vtid = self.GetPVTI(p)
            if not vtid:
                return None
            if vtid == 1:
                raise TypeError("Property '%s' doesn't have valid value type"%(p.GetName()))
            try:
                getter = _vt2getter[vtid]
            except KeyError:

                cls = PropertyContainerMethods
                vtn = self.GetPropertyValueType(p)

                if vtn == 'long':
                    getter = cls.GetPropertyValueAsLong
                elif vtn == 'string':
                    getter = cls.GetPropertyValueAsString
                elif vtn == 'double':
                    getter = cls.GetPropertyValueAsDouble
                elif vtn == 'bool':
                    getter = cls.GetPropertyValueAsBool
                elif vtn == 'arrstring':
                    getter = cls.GetPropertyValueAsArrayString
                elif vtn == 'wxArrayInt':
                    getter = cls.GetPropertyValueAsArrayInt
                elif vtn == 'PyObject*':
                    getter = cls.GetPropertyValueAsPyObject
                elif vtn == 'datetime':
                    getter = cls.GetPropertyValueAsDateTime
                elif vtn == 'wxPoint':
                    getter = cls.GetPropertyValueAsPoint
                elif vtn == 'wxSize':
                    getter = cls.GetPropertyValueAsSize
                elif vtn.startswith('wx'):
                    getter = cls.GetPropertyValueAsWxObjectPtr
                elif not vtn:
                    if p:
                        raise ValueError("no property with name '%s'"%p)
                    else:
                        raise ValueError("NULL property")
                else:
                    raise AssertionError("Unregistered property grid value type '%s'"%vtn)
                _vt2getter[vtid] = getter
            return getter(self,p)


        def SetPropertyValueArrstr(self,p,v):
            "NB: We must implement this in Python because SWIG has problems combining"
            "    conversion of list to wxArrayXXX and overloaded arguments."
            if not isinstance(p,basestring):
                self._SetPropertyValueArrstr(p,v)
            else:
                self._SetPropertyValueArrstr(self.GetPropertyByNameI(p),v)


        def SetPropertyValueArrint(self,p,v):
            "NB: We must implement this in Python because SWIG has problems combining"
            "    conversion of list to wxArrayXXX and overloaded arguments."
            if not isinstance(p,basestring):
                self._SetPropertyValueArrint(p,v)
            else:
                self._SetPropertyValueArrint(self.GetPropertyByNameI(p),v)


        def DoDefaultValueTypeMappings(self):
            "Map pg value type ids to getter methods."
            global _vt2getter
            try:
                vt2getter = _vt2getter

                return
            except NameError:
                vt2getter = {}
                _vt2getter = vt2getter


        def _GetValues(self,parent,fc,dict_,getter,inc_attributes):
            p = fc

            while p:
                pfc = self.GetFirstChild(p)
                if pfc:
                    self._GetValues(p,pfc,dict_,getter,inc_attributes)
                else:
                    dict_[p.GetName()] = getter(p)

                if inc_attributes:
                    attrs = p.GetAttributes()
                    if attrs and len(attrs):
                        dict_['@%s@attr'%(p.GetName())] = attrs

                p = self.GetNextSibling(p)


        def GetPropertyValues(self,dict_=None, as_strings=False, inc_attributes=False):
            "Returns values in the grid."
            ""
            "dict_: if not given, then a new one is created. dict_ can be"
            "  object as well, in which case it's __dict__ is used."
            "as_strings: if True, then string representations of values"
            "  are fetched instead of native types. Useful for config and such."
            "inc_attributes: if True, then property attributes are added"
            "  as @<propname>@<attr>."
            ""
            "Return value: dictionary with values. It is always a dictionary,"
            "so if dict_ was object with __dict__ attribute, then that attribute"
            "is returned."

            if dict_ is None:
                dict_ = {}
            elif hasattr(dict_,'__dict__'):
                dict_ = dict_.__dict__

            if not as_strings:
                getter = self.GetPropertyValue
            else:
                getter = self.GetPropertyValueAsString

            root = self.GetGrid().GetRoot()
            self._GetValues(root,self.GetFirstChild(root),dict_,getter,inc_attributes)

            return dict_

        GetValues = GetPropertyValues


        def SetPropertyValues(self,dict_):
            "Sets property values from dict_, which can be either\ndictionary or an object with __dict__ attribute."
            ""
            "autofill: If true, keys with not relevant properties"
            "  are auto-created. For more info, see AutoFill."
            ""
            "Notes:"
            "  * Keys starting with underscore are ignored."
            "  * Attributes can be set with entries named @<propname>@<attr>."
            ""

            autofill = False

            if dict_ is None:
                dict_ = {}
            elif hasattr(dict_,'__dict__'):
                dict_ = dict_.__dict__

            attr_dicts = []

            def set_sub_obj(k0,dict_):
                for k,v in dict_.iteritems():
                    if k[0] != '_':
                        if k.endswith('@attr'):
                            attr_dicts.append((k[1:-5],v))
                        else:
                            try:
                                self.SetPropertyValue(k,v)
                            except:
                                try:
                                    if autofill:
                                        self._AutoFillOne(k0,k,v)
                                        continue
                                except:
                                    if isinstance(v,dict):
                                        set_sub_obj(k,v)
                                    elif hasattr(v,'__dict__'):
                                        set_sub_obj(k,v.__dict__)


            for k,v in attr_dicts:
                p = GetPropertyByName(k)
                if not p:
                    raise AssertionError("No such property: '%s'"%k)
                for an,av in v.iteritems():
                    p.SetAttribute(an, av)


            cur_page = False
            is_manager = isinstance(self,PropertyGridManager)

            try:
                set_sub_obj(self.GetGrid().GetRoot(),dict_)
            except:
                import traceback
                traceback.print_exc()

            self.Refresh()


        SetValues = SetPropertyValues


        def _AutoFillMany(self,cat,dict_):
            for k,v in dict_.iteritems():
                self._AutoFillOne(cat,k,v)


        def _AutoFillOne(self,cat,k,v):
            global _type2property

            factory = _type2property.get(v.__class__,None)

            if factory:
                self.AppendIn( cat, factory(k,k,v) )
            elif hasattr(v,'__dict__'):
                cat2 = self.AppendIn( cat, PropertyCategory(k) )
                self._AutoFillMany(cat2,v.__dict__)
            elif isinstance(v,dict):
                cat2 = self.AppendIn( cat, PropertyCategory(k) )
                self._AutoFillMany(cat2,v)
            elif not k.startswith('_'):
                raise AssertionError("member '%s' is of unregisted type/class '%s'"%(k,v.__class__))


        def AutoFill(self,obj,parent=None):
            "Clears properties and re-fills to match members and\nvalues of given object or dictionary obj."

            self.edited_objects[parent] = obj

            cur_page = False
            is_manager = isinstance(self,PropertyGridManager)

            if not parent:
                if is_manager:
                    page = self.GetTargetPage()
                    self.ClearPage(page)
                    parent = self.GetPageRoot(page)
                else:
                    self.Clear()
                    parent = self.GetGrid().GetRoot()
            else:
                p = self.GetFirstChild(parent)
                while p:
                    self.Delete(p)
                    p = self.GetNextSibling(p)

            if not is_manager or page == self.GetSelectedPage():
                self.Freeze()
                cur_page = True

            try:
                self._AutoFillMany(parent,obj.__dict__)
            except:
                import traceback
                traceback.print_exc()

            if cur_page:
                self.Thaw()


        def RegisterEditor(self, editor, editorName=None):
            "Transform class into instance, if necessary."
            if not isinstance(editor, PGEditor):
                editor = editor()
            if not editorName:
                editorName = editor.__class__.__name__
            try:
                self._editor_instances.append(editor)
            except:
                self._editor_instances = [editor]
            RegisterEditor(editor, editorName)

    }
#endif

    // GetPropertyByNameI With nice assertion error message.
    wxPGProperty* GetPropertyByNameA( wxPGPropNameStr name ) const;

    static wxPGEditor* GetEditorByName( const wxString& editorName );
    
protected:

    // Default call's m_pState's BaseGetPropertyByName
    virtual wxPGProperty* DoGetPropertyByName( wxPGPropNameStr name ) const;

    virtual void RefreshProperty( wxPGProperty* p ) = 0;

#ifndef SWIG
    // Deriving classes must set this (it must be only or current page).
    wxPropertyGridState*         m_pState;

    // Intermediate version needed due to wxVariant copying inefficiency
    void DoSetPropertyAttribute( wxPGPropArg id, const wxString& name, wxVariant& value, long argFlags );

    // Empty string object to return from member functions returning const wxString&.
    wxString                    m_emptyString;

#endif // #ifndef SWIG
};

// -----------------------------------------------------------------------


// wxPropertyGrid::DoSelectProperty flags (selFlags)
#define wxPG_SEL_FOCUS      0x01 // Focuses to created editor
#define wxPG_SEL_FORCE      0x02 // Forces deletion and recreation of editor
#define wxPG_SEL_NONVISIBLE 0x04 // For example, doesn't cause EnsureVisible
#define wxPG_SEL_NOVALIDATE 0x08 // Do not validate editor's value before selecting
#define wxPG_SEL_DELETING   0x10 // Property being deselected is about to be deleted
#define wxPG_SEL_SETUNSPEC  0x20 // Property's values was set to unspecified by the user
#define wxPG_SEL_DIALOGVAL  0x40 // Property's event handler changed the value


// -----------------------------------------------------------------------

#ifndef SWIG

// Internal flags
#define wxPG_FL_INITIALIZED                 0x0001
#define wxPG_FL_ACTIVATION_BY_CLICK         0x0002 // Set when creating editor controls if it was clicked on.
#define wxPG_FL_DONT_CENTER_SPLITTER        0x0004
#define wxPG_FL_FOCUSED                     0x0008
#define wxPG_FL_MOUSE_CAPTURED              0x0010
#define wxPG_FL_MOUSE_INSIDE                0x0020
#define wxPG_FL_VALUE_MODIFIED              0x0040
#define wxPG_FL_PRIMARY_FILLS_ENTIRE        0x0080 // don't clear background of m_wndPrimary
#define wxPG_FL_CUR_USES_CUSTOM_IMAGE       0x0100 // currently active editor uses custom image
//#define wxPG_FL_HIDE_STATE                  0x0200 // set when hideable properties should be hidden
#define wxPG_FL_SCROLLED                    0x0400
#define wxPG_FL_ADDING_HIDEABLES            0x0800 // set when all added/inserted properties get hideable flag
#define wxPG_FL_NOSTATUSBARHELP             0x1000 // Disables showing help strings on statusbar.
#define wxPG_FL_CREATEDSTATE                0x2000 // Marks that we created the state, so we have to destroy it too.
#define wxPG_FL_SCROLLBAR_DETECTED          0x4000 // Set if scrollbar's existence was detected in last onresize.
#define wxPG_FL_DESC_REFRESH_REQUIRED       0x8000 // Set if wxPGMan requires redrawing of description text box.
#define wxPG_FL_IN_MANAGER                  0x00020000 // Set if contained in wxPropertyGridManager
#define wxPG_FL_GOOD_SIZE_SET               0x00040000 // Set after wxPropertyGrid is shown in its initial good size
#define wxPG_FL_IGNORE_NEXT_NAVKEY          0x00080000 // Next navigation key event will get ignored
#define wxPG_FL_IN_SELECT_PROPERTY          0x00100000 // Set when in SelectProperty.
#define wxPG_FL_STRING_IN_STATUSBAR         0x00200000 // Set when help string is shown in status bar
#define wxPG_FL_SPLITTER_PRE_SET            0x00400000 // Splitter position has been custom-set by the user
#define wxPG_FL_VALIDATION_FAILED           0x00800000 // Validation failed. Clear on modify event.
//#define wxPG_FL_SELECTED_IS_FULL_PAINT      0x01000000 // Set if selected is fully painted (ie. both image and text)
#define wxPG_MAN_FL_PAGE_INSERTED           0x02000000 // Set after page has been inserted to manager
#define wxPG_FL_ABNORMAL_EDITOR             0x04000000 // Active editor control is abnormally large
#define wxPG_FL_USE_COMMON_VALUES           0x08000000
#define wxPG_FL_VALUE_CHANGE_IN_EVENT       0x10000000
#define wxPG_FL_FIXED_WIDTH_EDITOR          0x20000000 // Editor control width should not change on resize
#define wxPG_FL_HAS_VIRTUAL_WIDTH           0x40000000 // Width of panel can be different than width of grid
#define wxPG_FL_RECALCULATING_VIRTUAL_SIZE  0x80000000 // Prevents RecalculateVirtualSize re-entrancy

#endif // #ifndef SWIG

#if !defined(__wxPG_SOURCE_FILE__) && !defined(SWIG)
    // Reduce compile time, but still include in user app
    #include "props.h"
#endif

// -----------------------------------------------------------------------

#define wxPG_USE_STATE  m_pState

/** \class wxPropertyGrid
	\ingroup classes
    \brief
    wxPropertyGrid is a specialized two-column grid for editing properties
    such as strings, numbers, flagsets, fonts, and colours. wxPropertySheet
    used to do the very same thing, but it hasn't been updated for a while
    and it is currently deprecated.

    wxPropertyGrid is modeled after .NET propertygrid (hence the name),
    and thus features are similar. However, inorder to keep the widget lightweight,
    it does not (and will not) have toolbar for mode and page selection, nor the help
    text box. wxAdvancedPropertyGrid (or something similarly named) is planned to have
    these features in some distant future.

    <h4>Derived from</h4>

    wxPropertyContainerMethods\n
    wxScrolledWindow\n
    wxPanel\n
    wxWindow\n
    wxEvtHandler\n
    wxObject\n

    <h4>Include files</h4>

    <wx/propertygrid/propertygrid.h>

    <h4>Window styles</h4>

    @link wndflags Additional Window Styles@endlink

    <h4>Event handling</h4>

    To process input from a propertygrid control, use these event handler macros to
    direct input to member functions that take a wxPropertyGridEvent argument.

    <table>
    <tr><td>EVT_PG_SELECTED (id, func)</td><td>Property is selected.</td></tr>
    <tr><td>EVT_PG_CHANGED (id, func)</td><td>Property value is modified.</td></tr>
    <tr><td>EVT_PG_CHANGING (id, func)</td><td>Property value is about to be changed. Use wxPropertyGridEvent::GetValue() to take a peek at the pending value.</td></tr>
    <tr><td>EVT_PG_HIGHLIGHTED (id, func)</td><td>Mouse moves over property. Event's property is NULL if hovered on area that is not a property.</td></tr>
    <tr><td>EVT_PG_RIGHT_CLICK (id, func)</td><td>Mouse right-clicked on a property.</td></tr>
    <tr><td>EVT_PG_DOUBLE_CLICK (id, func)</td><td>Mouse double-clicked on a property.</td></tr>
    <tr><td>EVT_PG_ITEM_COLLAPSED (id, func)</td><td>User collapses a property or category.</td></tr>
    <tr><td>EVT_PG_ITEM_EXPANDED (id, func)</td><td>User expands a property or category.</td></tr>
    <tr><td>EVT_BUTTON (id, func)</td><td>Button in a property editor was clicked. Only occurs if the property doesn't handle button clicks itself.</td></tr>
    <tr><td>EVT_TEXT (id, func)</td><td>wxTextCtrl based editor was updated (but property value was not yet modified)</td></tr>
    </table>

    \sa @link wxPropertyGridEvent wxPropertyGridEvent@endlink

    \remarks

    - Following functions do not automatically update the screen: Append. You
      probably need to explicitly call Refresh() <b>if</b> you called one of these
      functions outside parent window constructor.

    - Use Freeze() and Thaw() respectively to disable and enable drawing. This
      will also delay sorting etc. miscellaneous calculations to the last possible
      moment.

    - Most methods have two versions - one which accepts property id (faster) and
      another that accepts property name (which is a bit slower since it does a hashmap
      lookup).

    For code examples, see the main page.

*/
// BM_GRID
class WXDLLIMPEXP_PG wxPropertyGrid : public wxScrolledWindow, public wxPropertyContainerMethods
{
#ifndef SWIG
    friend class wxPropertyGridState;
    friend class wxPropertyContainerMethods;
    friend class wxPropertyGridManager;
    friend class wxPGCanvas;

    DECLARE_DYNAMIC_CLASS(wxPropertyGrid)
#endif

public:
	/** Two step constructor. Call Create when this constructor is called to build up the
	    wxPropertyGrid
	*/

#ifdef SWIG
    %pythonAppend wxPropertyGrid {
        self._setOORInfo(self)
        self.DoDefaultTypeMappings()
        self.edited_objects = {}
        self.DoDefaultValueTypeMappings()
        if not hasattr(self.__class__,'_vt2setter'):
            self.__class__._vt2setter = {}
    }
    %pythonAppend wxPropertyGrid() ""

    wxPropertyGrid( wxWindow *parent, wxWindowID id = wxID_ANY,
               	    const wxPoint& pos = wxDefaultPosition,
               	    const wxSize& size = wxDefaultSize,
               	    long style = wxPG_DEFAULT_STYLE,
               	    const wxChar* name = wxPyPropertyGridNameStr );
    %RenameCtor(PrePropertyGrid,  wxPropertyGrid());

#else

    wxPropertyGrid();

    /** The default constructor. The styles to be used are styles valid for
        the wxWindow and wxScrolledWindow.
        \sa @link wndflags Additional Window Styles@endlink
    */
    wxPropertyGrid( wxWindow *parent, wxWindowID id = wxID_ANY,
               	    const wxPoint& pos = wxDefaultPosition,
               	    const wxSize& size = wxDefaultSize,
               	    long style = wxPG_DEFAULT_STYLE,
               	    const wxChar* name = wxPropertyGridNameStr );

    /** Destructor */
    virtual ~wxPropertyGrid();
#endif

    /** This static function enables or disables automatic use of wxGetTranslation for
        following strings: wxEnumProperty list labels, wxFlagsProperty sub-property
        labels.
        Default is false.
    */
    static void AutoGetTranslation( bool enable );

    /** Returns true if all property grid data changes have been committed. Usually
        only returns false if value in active editor has been invalidated by a
        wxValidator.
    */
    inline bool CanClose()
    {
        return DoEditorValidate();
    }

    /** Returns true if all property grid data changes have been committed. Usually
        only returns false if value in active editor has been invalidated by a
        wxValidator.
    */
    inline bool EditorValidate()
    {
        return DoEditorValidate();
    }

    /** Centers the splitter. If argument is true, automatic splitter centering is
        enabled (only applicapple if style wxPG_SPLITTER_AUTO_CENTER was defined).
    */
    void CenterSplitter( bool enable_auto_centering = false );

    /** Two step creation. Whenever the control is created without any parameters, use Create to actually
        create it. Don't access the control's public methods before this is called
        \sa @link wndflags Additional Window Styles@endlink
    */
    bool Create( wxWindow *parent, wxWindowID id = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxPG_DEFAULT_STYLE,
                 const wxChar* name = wxPropertyGridNameStr );

    /** Deletes all properties. Does not free memory allocated for arrays etc.
        This should *not* be called in wxPropertyGridManager.
    */
    void Clear();

    /** Resets modified status of a property and all sub-properties.
    */
    void ClearModifiedStatus( wxPGPropArg id )
    {
        wxPG_PROP_ARG_CALL_PROLOG()
        m_pState->ClearModifiedStatus(p);
    }

    /** Resets modified status of all properties.
    */
    inline void ClearModifiedStatus()
    {
        m_pState->ClearModifiedStatus(m_pState->m_properties);
        m_pState->m_anyModified = false;
    }

    /** Deselect current selection, if any. Returns true if success
        (ie. validator did not intercept). */
    bool ClearSelection();

#if wxPG_COMPATIBILITY_1_2_0
    /** Synonymous to Clear.
    */
    wxDEPRECATED( void ClearTargetPage() );
#endif

    /** Enables or disables (shows/hides) categories according to parameter enable. */
    bool EnableCategories( bool enable );

    /** Call to enable or disable usage of common values (values that can be selected for
        properties instead of their normal values).

        Common values are disabled by the default.
    */
    void EnableCommonValues( bool enable = true );

    /** Returns wxWindow that the properties are painted on, and which should be used
        as the parent for editor controls.
    */
    wxWindow* GetPanel() const
    {
        return m_canvas;
    }

    const wxPGCommonValue* GetCommonValue( unsigned int i ) const
    {
        return (wxPGCommonValue*) m_commonValues[i];
    }

    /** Returns label of given common value.
    */
    wxString GetCommonValueLabel( unsigned int i ) const
    {
        wxASSERT( GetCommonValue(i) );
        return GetCommonValue(i)->GetLabel();
    }

    /** Return number of displayed common values
    */
    int GetDisplayedCommonValueCount() const
    {
        if ( m_iFlags & wxPG_FL_USE_COMMON_VALUES )
            return m_commonValues.size();
        return 0;
    }

    /** Scrolls and/or expands items to ensure that the given item is visible.
        Returns true if something was actually done.
    */
    bool EnsureVisible( wxPGPropArg id );

    wxFont& GetCaptionFont() { return m_captionFont; }

#ifndef SWIG
    const wxFont& GetCaptionFont() const { return m_captionFont; }
#endif

    /** Returns current category caption background colour. */
    inline wxColour GetCaptionBackgroundColour() const { return m_colCapBack; }

    /** Returns current category caption text colour. */
    inline wxColour GetCaptionForegroundColour() const { return m_colCapFore; }

    /** Returns current cell background colour. */
    inline wxColour GetCellBackgroundColour() const { return m_colPropBack; }

    /** Returns current cell text colour when disabled. */
    inline wxColour GetCellDisabledTextColour() const { return m_colDisPropFore; }

    /** Returns current cell text colour. */
    inline wxColour GetCellTextColour() const { return m_colPropFore; }

    /** Returns number of children of the root property.
    */
    inline size_t GetChildrenCount()
    {
        return GetChildrenCount( wxPGIdGen(m_pState->m_properties) );
    }

    /** Returns number of children for the property.

        NB: Cannot be in container methods class due to name hiding.
    */
    inline size_t GetChildrenCount( wxPGPropArg id ) const
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(0)
        return p->GetChildCount();
    }

    int GetColumnCount() const
    {
        return m_pState->m_colWidths.size();
    }

#if wxPG_COMPATIBILITY_1_2_0
    /** Returns id of first visible item, whether it is a category or property.
        Note that visible item means category, property, or sub-property which
        user can see when control is scrolled properly. It does not only mean
        items that are actually painted on the screen.
        DEPRECATED: Use wxPropertyContainerMethods::GetIterator() instead.
    */
    const wxPGProperty* GetFirstVisible() const
    {
        return wxPropertyContainerMethods::GetFirst( wxPG_ITERATE_VISIBLE );
    }

    wxPGProperty* GetFirstVisible()
    {
        return wxPropertyContainerMethods::GetFirst( wxPG_ITERATE_VISIBLE );
    }
#endif

    /** Returns height of highest characters of used font. */
    int GetFontHeight() const { return m_fontHeight; }

    /** Returns pointer to itself. Dummy function that enables same kind
        of code to use wxPropertyGrid and wxPropertyGridManager.
    */
    wxPropertyGrid* GetGrid() { return this; }

#if wxPG_COMPATIBILITY_1_2_0
    /** Returns id of first category (from target page).
        DEPRECATED: Use wxPropertyContainerMethods::GetIterator() instead.
    */
    wxPGProperty* GetFirstCategory() const
    {
        return (wxPGProperty*) wxPropertyContainerMethods::GetFirst( wxPG_ITERATE_CATEGORIES );
    }

    /** Returns id of first property that is not a category.
        DEPRECATED: Use wxPropertyContainerMethods::GetIterator() instead.
    */
    wxPGProperty* GetFirstProperty()
    {
        return (wxPGProperty*) wxPropertyContainerMethods::GetFirst( wxPG_ITERATE_DEFAULT );
    }
#endif

    /** Returns size of the custom paint image in front of property.
        If no argument is given, returns preferred size.
    */
    wxSize GetImageSize( wxPGProperty* p = NULL, int item = -1 ) const;

    wxRect GetImageRect( wxPGProperty* p, int item ) const;

    /** Returns GetCustomPaintWidth for odcombo, GetImageSize(m_selected) otherwise.
    */
    //wxSize GetEditorImageSize() const;

    /** Returns property (or category) at given y coordinate (relative to control's
        top left).
    */
    wxPGId GetItemAtY( int y ) const { return wxPGIdGen(DoGetItemAtY(y)); }

    /** Returns last item which could be iterated using given flags.
        \param flags
        @link iteratorflags List of iterator flags@endlink
    */
    wxPGProperty* GetLastItem( int flags = wxPG_ITERATE_DEFAULT )
    {
        return m_pState->GetLastItem(flags);
    }

    const wxPGProperty* GetLastItem( int flags = wxPG_ITERATE_DEFAULT ) const
    {
        return m_pState->GetLastItem(flags);
    }

#if wxPG_COMPATIBILITY_1_2_0
    /** Returns id of last item. Ignores categories and sub-properties.
        DEPRECATED: Use wxPropertyContainerMethods::GetIterator() instead.
    */
    wxPGId GetLastProperty()
    {
        if ( !m_pState->m_properties->GetCount() ) return wxPGIdGen((wxPGProperty*)NULL);
        wxPGProperty* p = GetLastItem(wxPG_ITERATE_DEFAULT);
        if ( p->IsCategory() )
            return GetPrevProperty( wxPGIdGen(p) );
        return wxPGIdGen(p);
    }

    /** Returns id of last child of given property.
        \remarks
        Returns even sub-properties.
        DEPRECATED: Use wxPropertyContainerMethods::GetIterator() instead.
    */
    wxPGId GetLastChild( wxPGPropArg id )
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(wxNullProperty)

        if ( !p->GetChildCount() ) return wxNullProperty;

        return wxPGIdGen(p->Last());
    }
    /** Returns id of last visible item. Does <b>not</b> ignore categories sub-properties.
        DEPRECATED: Use wxPropertyContainerMethods::GetIterator() instead.
    */
    wxPGId GetLastVisible()
    {
        return GetLastItem(wxPG_ITERATE_VISIBLE);
    }
#endif // wxPG_COMPATIBILITY_1_2_0

    /** Returns colour of lines between cells. */
    inline wxColour GetLineColour() const { return m_colLine; }

    /** Returns background colour of margin. */
    inline wxColour GetMarginColour() const { return m_colMargin; }

#if wxPG_COMPATIBILITY_1_2_0
    /** Returns id of next property. This does <b>not</b> iterate to sub-properties
        or categories, unlike GetNextVisible.
        DEPRECATED: Use wxPropertyContainerMethods::GetIterator() instead.
    */
    wxPGProperty* GetNextProperty( wxPGPropArg id )
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(wxNullProperty)
        return m_pState->GetNextProperty(p);
    }

    /** Returns id of next category after a given property (which does not have to be category).
        DEPRECATED: Use wxPropertyContainerMethods::GetIterator() instead.
    */
    wxPGProperty* GetNextCategory( wxPGPropArg id ) const
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(wxNullProperty)
        return m_pState->GetNextCategory(p);
    }

    /** Returns id of next visible item.
        Note that visible item means category, property, or sub-property which
        user can see when control is scrolled properly. It does not only mean
        items that are actually painted on the screen.
        DEPRECATED: Use wxPropertyContainerMethods::GetIterator() instead.
    */
    inline wxPGProperty* GetNextVisible( wxPGPropArg id ) const
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(wxNullProperty)
        return wxPropertyGridIterator::OneStep(m_pState,wxPG_ITERATE_VISIBLE,p,1);
    }

    /** Returns id of previous property. Unlike GetPrevVisible, this skips categories
        and sub-properties.
        DEPRECATED: Use wxPropertyContainerMethods::GetIterator() instead.
    */
    inline wxPGProperty* GetPrevProperty( wxPGPropArg id )
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(NULL)
        return m_pState->GetPrevProperty(p);
    }
    /** Returns id of previous item under the same parent.
        DEPRECATED: Use wxPropertyContainerMethods::GetIterator() instead.
    */
    inline wxPGProperty* GetPrevSibling( wxPGPropArg id )
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(wxNullProperty)
        return m_pState->GetPrevSibling(p);
    }
    /** Returns id of previous visible property.
        DEPRECATED: Use wxPropertyContainerMethods::GetIterator() instead.
    */
    inline wxPGProperty* GetPrevVisible( wxPGPropArg id )
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(wxNullProperty)
        return wxPropertyGridIterator::OneStep(m_pState,wxPG_ITERATE_VISIBLE,p,-1);
    }
#endif // wxPG_COMPATIBILITY_1_2_0

    /** Returns cell background colour of a property. */
    wxColour GetPropertyBackgroundColour( wxPGPropArg id ) const;

    /** Returns cell background colour of a property. */
    inline wxColour GetPropertyColour( wxPGPropArg id ) const
    {
        return GetPropertyBackgroundColour( id );
    }

    /** Returns cell background colour of a property. */
    wxColour GetPropertyTextColour( wxPGPropArg id ) const;

    /** Returns id of property with given label (case-sensitive). If there is no
        property with such label, returned property id is invalid ( i.e. it will return
        false with IsOk method). If there are multiple properties with identical name,
        most recent added is returned.
    */
    inline wxPGId GetPropertyByLabel( const wxString& name ) const
    {
        return m_pState->GetPropertyByLabel(name);
    }

    /** Returns "root property". It does not have name, etc. and it is not
        visible. It is only useful for accessing its children.
    */
    wxPGProperty* GetRoot() const { return m_pState->m_properties; }

    /** Returns height of a single grid row (in pixels). */
    int GetRowHeight() const { return m_lineHeight; }

    wxPGProperty* GetSelectedProperty () const { return GetSelection(); }

    /** Returns currently selected property. */
    wxPGProperty* GetSelection() const
    {
        return m_selected;
    }

    /** Returns current selection background colour. */
    inline wxColour GetSelectionBackgroundColour() const { return m_colSelBack; }

    /** Returns current selection text colour. */
    inline wxColour GetSelectionForegroundColour() const { return m_colSelFore; }

    /** Returns current splitter x position. */
    inline int GetSplitterPosition() const { return m_pState->GetSplitterPosition(0); }

    /** Returns index of common value that will truly change value to unspecified.
    */
    int GetUnspecifiedCommonValue() const { return m_cvUnspecified; }

    /** Returns current vertical spacing. */
    inline int GetVerticalSpacing() const { return (int)m_vspacing; }

    /** Returns true if a property is selected. */
    inline bool HasSelection() const { return ((m_selected!=(wxPGProperty*)NULL)?true:false); }

    bool HasVirtualWidth() const { return (m_iFlags & wxPG_FL_HAS_VIRTUAL_WIDTH)?true:false; }

    /** Returns information about arbitrary position in the grid.
    */
    wxPropertyGridHitTestResult HitTest( const wxPoint& pt ) const;

    /** Returns true if any property has been modified by the user. */
    inline bool IsAnyModified() const { return (m_pState->m_anyModified>0); }

    /** Returns true if updating is frozen (ie. Freeze() called but not yet Thaw() ). */
    inline bool IsFrozen() const { return (m_frozen>0)?true:false; }

#if wxPG_COMPATIBILITY_1_2_0
    /** Returns true if given property is selected. */
    wxDEPRECATED( bool IsPropertySelected( wxPGPropArg id ) const );
#endif

    /** Set number of columns (2+).
    */
    void SetColumnCount( int colCount )
    {
        m_pState->SetColumnCount(colCount);
        Refresh();
    }

    /** Moves splitter as left as possible, while still allowing all
        labels to be shown in full.
        \param subProps
        If false, will still allow sub-properties (ie. properties which
        parent is not root or category) to be cropped.
    */
    void SetSplitterLeft( bool subProps = false )
    {
        m_pState->SetSplitterLeft(subProps);
    }

#ifndef SWIG
    /** Registers a new editor class.
        \retval
        Pointer to the editor class instance that should be used.
    */
    static wxPGEditor* RegisterEditorClass( wxPGEditor* editor, const wxString& name,
                                            bool noDefCheck = false );
#endif

    /** Resets all colours to the original system values.
    */
    void ResetColours();

    /** Changes keyboard shortcut to push the editor button.
        \remarks
        You can set default with keycode 0. Good value for the platform is guessed,
        but don't expect it to be very accurate.
    */
    void SetButtonShortcut( int keycode, bool ctrlDown = false, bool altDown = false );

    /** Sets text colour of a category caption (but not it's children).
    */
    void SetCaptionTextColour( wxPGPropArg id, const wxColour& col );

    /** Sets the current category - Append will add non-categories under this one.
    */
    inline void SetCurrentCategory( wxPGPropArg id )
    {
        wxPG_PROP_ARG_CALL_PROLOG()
        wxPropertyCategory* pc = (wxPropertyCategory*)p;
#ifdef __WXDEBUG__
        if ( pc ) wxASSERT( pc->IsCategory() );
#endif
        m_pState->m_currentCategory = pc;
    }

    /** Sets property attribute for all applicapple properties.
        Be sure to use this method after all properties have been
        added to the grid.
    */
    void SetPropertyAttributeAll( const wxString& attrName, wxVariant value );

    /** Sets background colour of property and all its children. Colours of
        captions are not affected. Background brush cache is optimized for often
        set colours to be set last.
    */
    void SetPropertyBackgroundColour( wxPGPropArg id, const wxColour& col );

#if wxPG_COMPATIBILITY_1_2_0
    /** Sets background colour of property and all its children. Colours of
        captions are not affected. Background brush cache is optimized for often
        set colours to be set last.

        NOTE: This function is deprecated. Use SetPropertyBackgroundColour instead.
    */
    wxDEPRECATED( void SetPropertyColour( wxPGPropArg id, const wxColour& col ) );
#endif

    /** Sets text colour of property and all its children.
    */
    void SetPropertyTextColour( wxPGPropArg id, const wxColour& col );

    /** Sets background and text colour of property and all its children to the default. */
    void SetPropertyColourToDefault( wxPGPropArg id );

    /** Sets category caption background colour. */
    void SetCaptionBackgroundColour(const wxColour& col);

    /** Sets category caption text colour. */
    void SetCaptionForegroundColour(const wxColour& col);

    /** Sets default cell background colour - applies to property cells.
        Note that appearance of editor widgets may not be affected.
    */
    void SetCellBackgroundColour(const wxColour& col);

    /** Sets cell text colour for disabled properties.
    */
    void SetCellDisabledTextColour(const wxColour& col);

    /** Sets default cell text colour - applies to property name and value text.
        Note that appearance of editor widgets may not be affected.
    */
    void SetCellTextColour(const wxColour& col);

    /** Sets colour of lines between cells. */
    void SetLineColour(const wxColour& col);

    /** Sets background colour of margin. */
    void SetMarginColour(const wxColour& col);

    /** Sets selection background colour - applies to selected property name background. */
    void SetSelectionBackground(const wxColour& col);

    /** Sets selection foreground colour - applies to selected property name text. */
    void SetSelectionForeground(const wxColour& col);

    /** Sets x coordinate of the splitter. */
    inline void SetSplitterPosition( int newxpos, bool refresh = true )
    {
        DoSetSplitterPosition(newxpos,refresh,0);
        m_iFlags |= wxPG_FL_SPLITTER_PRE_SET;
    }

    /** Set index of common value that will truly change value to unspecified.
        Using -1 will set none to have such effect.
        Default is 0.
    */
    void SetUnspecifiedCommonValue( int index ) { m_cvUnspecified = index; }

    /** Set virtual width for this particular page. Width -1 indicates that the
        virtual width should be disabled. */
    void SetVirtualWidth( int width );

    /** Selects a property. Editor widget is automatically created, but
        not focused unless focus is true. This will generate wxEVT_PG_SELECT event.
        \param id
        Id to property to select.
        \retval
        True if selection finished succesfully. Usually only fails if current
        value in editor is not valid.
        \sa wxPropertyGrid::Unselect
    */
    inline bool SelectProperty( wxPGPropArg id, bool focus = false )
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(false)
        return DoSelectProperty(p,focus?wxPG_SEL_FOCUS:0);
    }

    /** Mostly useful for page switching.
    */
#ifndef SWIG
    void SwitchState( wxPropertyGridState* pNewState );
#endif

    /** Sets name of a property.
        \param id
        Id of a property.
        \param newname
        New name.
        \remarks
        This is the only way to set property's name. There is not
        wxPGProperty::SetName() method.
    */
    void SetPropertyName( wxPGPropArg id, const wxString& newname )
    {
        wxPG_PROP_ARG_CALL_PROLOG()
        DoSetPropertyName( p, newname );
    }

    /** Sets vertical spacing. Can be 1, 2, or 3 - a value relative to font
        height. Value of 2 should be default on most platforms.
        \remarks
        On wxMSW, wxComboBox, when used as property editor widget, will spill
        out with anything less than 3.
    */
    inline void SetVerticalSpacing( int vspacing )
    {
        m_vspacing = (unsigned char)vspacing;
        CalculateFontAndBitmapStuff( vspacing );
        if ( !m_pState->m_itemsAdded ) Refresh();
    }

    /** Shows an brief error message that is related to a property. */
    inline void ShowPropertyError( wxPGPropArg id, const wxString& msg )
    {
        wxPG_PROP_ARG_CALL_PROLOG()
        p->ShowError(msg);
    }

    /** Sorts all items at all levels (except sub-properties). */
    void Sort();

    /** Sorts children of a category.
    */
    void Sort( wxPGPropArg id );

    /** Overridden function.
        \sa @link wndflags Additional Window Styles@endlink
    */
    virtual void SetWindowStyleFlag( long style );

    /** Property editor widget helper methods. */
    //@{
    /** Call when editor widget's contents is modified. For example, this is called
        when changes text in wxTextCtrl (used in wxStringProperty and wxIntProperty).
        \remarks
        This should only be called by properties.
        \sa @link wxPGProperty::OnEvent @endlink
    */
    inline void EditorsValueWasModified() { m_iFlags |= wxPG_FL_VALUE_MODIFIED; }
    /** Reverse of EditorsValueWasModified(). */
    inline void EditorsValueWasNotModified()
    {
        m_iFlags &= ~(wxPG_FL_VALUE_MODIFIED);
    }

    /** Returns true if editor's value was marked modified. */
    inline bool IsEditorsValueModified() const { return  ( m_iFlags & wxPG_FL_VALUE_MODIFIED ) ? true : false; }

    /** Shortcut for creating dialog-caller button. Used, for example, by wxFontProperty.
        \remarks
        This should only be called by properties.
    */
    wxWindow* GenerateEditorButton( const wxPoint& pos, const wxSize& sz );

    /** Fixes position of wxTextCtrl-like control (wxSpinCtrl usually
        fits as one). Call after control has been created (but before
        shown).
    */
    void FixPosForTextCtrl( wxWindow* ctrl, const wxPoint& offset = wxPoint(0, 0) );

    /** Shortcut for creating text editor widget.
        \param pos
        Same as pos given for CreateEditor.
        \param sz
        Same as sz given for CreateEditor.
        \param value
        Initial text for wxTextCtrl.
        \param secondary
        If right-side control, such as button, also created, then create it first
        and pass it as this parameter.
        \param extraStyle
        Extra style flags to pass for wxTextCtrl.
        \remarks
        Note that this should generally be called only by new classes derived
        from wxPGProperty.
    */
    wxWindow* GenerateEditorTextCtrl( const wxPoint& pos,
                                      const wxSize& sz,
                                      const wxString& value,
                                      wxWindow* secondary,
                                      int extraStyle = 0,
                                      int maxLen = 0 );

    /* Generates both textctrl and button.
    */
    wxWindow* GenerateEditorTextCtrlAndButton( const wxPoint& pos,
        const wxSize& sz, wxWindow** psecondary, int limited_editing,
        wxPGProperty* property );

    /** Generates position for a widget editor dialog box.
        \param p
        Property for which dialog is positioned.
        \param sz
        Known or over-approximated size of the dialog.
        \retval
        Position for dialog.
    */
    wxPoint GetGoodEditorDialogPosition( wxPGProperty* p,
                                         const wxSize& sz );

    // Converts escape sequences in src_str to newlines,
    // tabs, etc. and copies result to dst_str.
    static wxString& ExpandEscapeSequences( wxString& dst_str, wxString& src_str );

    // Converts newlines, tabs, etc. in src_str to escape
    // sequences, and copies result to dst_str.
    static wxString& CreateEscapeSequences( wxString& dst_str, wxString& src_str );

    /** Returns rectangle that fully contains properties between and including p1 and p2.
    */
    wxRect GetPropertyRect( const wxPGProperty* p1, const wxPGProperty* p2 ) const;

    /** Returns pointer to current active primary editor control (NULL if none).

        If editor uses clipper window, pointer is returned to the actual editor, not the clipper.
    */
    wxWindow* GetEditorControl() const;

    /** Adjusts given position if topCtrlWnd is child of clipper window.
    */
    bool AdjustPosForClipperWindow( wxWindow* topCtrlWnd, int* x, int* y );

    inline wxWindow* GetPrimaryEditor() const
    {
        return GetEditorControl();
    }

    /** Returns pointer to current active secondary editor control (NULL if none).
    */
    inline wxWindow* GetEditorControlSecondary() const
    {
        return m_wndSecondary;
    }

    inline int IsNextEventIgnored() const
    {
        return m_ignoredEvents;
    }

    inline void IgnoreNextEvent()
    {
        m_ignoredEvents++;
    }

    inline void IgnoredEventPasses()
    {
        wxASSERT( m_ignoredEvents > 0 );
        m_ignoredEvents--;
    }

#ifdef __WXPYTHON__
    // Dummy method to put wxRect type info into the wrapper
    wxRect DummywxRectTypeInit() const { return wxRect(1,2,3,4); }
#endif

#ifndef SWIG

    /** Generates contents for string dst based on the convetents of wxArrayString
        src. Format will be <preDelim>str1<postDelim> <preDelim>str2<postDelim>
        and so on. Set flags to 1 inorder to convert backslashes to double-back-
        slashes and "<preDelims>"'s to "\<preDelims>".
    */
    static void ArrayStringToString( wxString& dst, const wxArrayString& src,
                                     wxChar preDelim, wxChar postDelim,
                                     int flags );

    /** Pass this function to Connect calls in propertyclass::CreateEditor.
    */
    void OnCustomEditorEvent( wxCommandEvent &event );
    //@}

    inline long GetInternalFlags() const { return m_iFlags; }
    inline void SetInternalFlag( long flag ) { m_iFlags |= flag; }
    inline void ClearInternalFlag( long flag ) { m_iFlags &= ~(flag); }
    inline void IncFrozen() { m_frozen++; }
    inline void DecFrozen() { m_frozen--; }

    void OnComboItemPaint( wxPGCustomComboControl* pCb,int item,wxDC& dc,
                           wxRect& rect,int flags );

    // Used by simple check box for keyboard navigation
    void SendNavigationKeyEvent( int dir );

    /** Standardized double-to-string conversion.
    */
    static void DoubleToString( wxString& target,
                                double value,
                                int precision,
                                bool removeZeroes,
                                wxString* precTemplate );

    /** Call this from wxPGProperty::OnEvent() to cause property value to be changed
        after the function returns (with true as return value). ValueChangeInEvent()
        must be used if you wish the application to be able to use wxEVT_PG_CHANGING
        to potentially veto the given value.
    */
    void ValueChangeInEvent( wxVariant variant )
    {
        m_changeInEventValue = variant;
        m_iFlags |= wxPG_FL_VALUE_CHANGE_IN_EVENT;
    }

    /** Returns true if given event is from first of an array of buttons
        (as can be in case when wxPGMultiButton is used).
    */
    bool IsMainButtonEvent( const wxEvent& event )
    {
        return (event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED) && (m_wndSecId == event.GetId());
    }

    /** Forces updating the value of property from the editor control.
        Returns true if DoPropertyChanged was actually called.
    */
    bool CommitChangesFromEditor( wxUint32 flags = 0 );

    bool DoPropertyChanged( wxPGProperty* p, wxVariant* pPendingValue, unsigned int selFlags = 0 );

    int GetSpacingY() const { return m_spacingy; }

protected:

    /** wxPropertyGridState used by the grid is created here. If grid is used
        in wxPropertyGridManager, there is no point overriding this - instead,
        set custom wxPropertyGridPage classes.
    */
    virtual wxPropertyGridState* CreateState() const;

#ifndef DOXYGEN
public:

    // Control font changer helper.
    void SetCurControlBoldFont();

    //
    // Public methods for semi-public use
    // (not protected for optimization)
    //
    bool DoSelectProperty( wxPGProperty* p, unsigned int flags = 0 );

    // Overridden functions.
    virtual bool Destroy();
    virtual wxSize DoGetBestSize() const;
    virtual void Refresh( bool eraseBackground = true,
                          const wxRect *rect = (const wxRect *) NULL );
    virtual bool SetFont( const wxFont& font );
#if wxPG_SUPPORT_TOOLTIPS
    void SetToolTip( const wxString& tipString );
#endif
    virtual void Freeze();
    virtual void SetExtraStyle( long exStyle );
    virtual void Thaw();


protected:

    /** 1 if in DoPropertyChanged. */
    unsigned char       m_propValChanging;

#ifndef wxPG_ICON_WIDTH
	wxBitmap            *m_expandbmp, *m_collbmp;
#endif

    wxCursor            *m_cursorSizeWE;

    /** wxWindow pointers to editor control(s). */
    wxWindow            *m_wndPrimary;
    wxWindow            *m_wndSecondary;

    /** wxPGCanvas instance. */
    wxWindow            *m_canvas;

#if wxPG_DOUBLE_BUFFER
    wxBitmap            *m_doubleBuffer;
#endif

    wxArrayPtrVoid      *m_windowsToDelete;

    /** Local time ms when control was created. */
    wxLongLong          m_timeCreated;

    /** wxPGProperty::OnEvent can change value by setting this. */
    wxVariant           m_changeInEventValue;

    /** Id of m_wndSecondary, or its first child, if any. */
    int                 m_wndSecId;

	/** Extra Y spacing between the items. */
	int                 m_spacingy;

    /** Control client area width; updated on resize. */
    int                 m_width;

    /** Control client area height; updated on resize. */
    int                 m_height;

    /** Current non-client width (needed when auto-centering). */
    int                 m_ncWidth;

    /** Non-client width (auto-centering helper). */
    //int                 m_fWidth;

    /** Previously recorded scroll start position. */
    int                 m_prevVY;

	/** The gutter spacing in front and back of the image. This determines the amount of spacing in front
	    of each item */
	int                 m_gutterWidth;

    /** Includes separator line. */
    int                 m_lineHeight;

    /** Gutter*2 + image width. */
    int                 m_marginWidth;

    int                 m_buttonSpacingY; // y spacing for expand/collapse button.

    /** Extra margin for expanded sub-group items. */
    int                 m_subgroup_extramargin;

	/** The image width of the [+] icon. This is also calculated in the gutter */
	int                 m_iconWidth;

#ifndef wxPG_ICON_WIDTH

	/** The image height of the [+] icon. This is calculated as minimal size and to align */
	int                 m_iconHeight;
#endif

    /** Current cursor id. */
    int                 m_curcursor;

	/** This captionFont is made equal to the font of the wxScrolledWindow. As extra the bold face
	    is set on it when this is wanted by the user (see flags) */
	wxFont              m_captionFont;

#if !wxPG_HEAVY_GFX
    int                 m_splitterprevdrawnx;

    /** Pen used to draw splitter column when it is being dragged. */
    wxPen               m_splitterpen;

#endif

	int                 m_fontHeight;  // Height of the font.

    int                 m_pushButKeyCode;  // Base keycode for triggering push button.

    /** m_splitterx when drag began. */
    int                 m_startingSplitterX;

    /** Index to splitter currently being dragged (0=one after the first column). */
    int                 m_draggedSplitter;

    //
    // Temporary values
    //

    /** Bits are used to indicate which colours are customized. */
    unsigned short      m_coloursCustomized;

    /** x - m_splitterx. */
    signed char                 m_dragOffset;

    /** 0 = not dragging, 1 = drag just started, 2 = drag in progress */
    unsigned char       m_dragStatus;

    /** 0 = margin, 1 = label, 2 = value. */
    unsigned char       m_mouseSide;

    /** True when editor control is focused. */
    unsigned char       m_editorFocused;

    /** 1 if m_latsCaption is also the bottommost caption. */
    //unsigned char       m_lastCaptionBottomnest;

    /** Set to 1 when graphics frozen. */
    unsigned char       m_frozen;

    unsigned char       m_vspacing;

    unsigned char       m_pushButKeyCodeNeedsAlt;  // Does triggering push button need Alt down?

    unsigned char       m_pushButKeyCodeNeedsCtrl;  // Does triggering push button need Ctrl down?

    unsigned char       m_keyComboConsumed;  // Used to track when Alt/Ctrl+Key was consumed.

    unsigned char       m_ignoredEvents;  // Number of EVT_TEXT-style events to ignore.

    /** Internal flags - see wxPG_FL_XXX constants. */
    wxUint32            m_iFlags;

    /** When drawing next time, clear this many item slots at the end. */
    int                 m_clearThisMany;

    /** Pointer to selected property. Note that this is duplicated in
        m_state for better transiency between pages so that the selected
        item can be retained.
    */
    wxPGProperty*       m_selected;

    wxPGProperty*       m_propHover;  // pointer to property that has mouse hovering

    wxWindow*           m_eventObject;  // EventObject for wxPropertyGridEvents

    wxWindow*           m_curFocused;  // What (global) window is currently focused
                                       // (needed to resolve event handling mess).

    wxEvtHandler*       m_tlwHandler;  // wxPGTLWHandler

    wxWindow*           m_tlp;  // Top level parent

    int                 m_propHoverY;   // y coordinate of property that mouse hovering
    int                 m_selColumn;  // Which column's editor is selected (usually 1)?

    int                 m_ctrlXAdjust; // x relative to splitter (needed for resize).

    wxColour            m_colLine;     // lines between cells
    wxColour            m_colPropFore; // property labels and values are written in this colour
    wxColour            m_colDisPropFore;  // or with this colour when disabled
    wxColour            m_colPropBack; // background for m_colPropFore
    wxColour            m_colCapFore;  // text color for captions
    wxColour            m_colCapBack;  // background color for captions
    wxColour            m_colSelFore;  // foreground for selected property
    wxColour            m_colSelBack;  // background for selected property (actually use background color when control out-of-focus)
    wxColour            m_colMargin;   // background colour for margin

    wxArrayPtrVoid      m_visPropArray;  // temp property array used in DoDrawItems

    // NB: These *cannot* be moved to globals.
    wxArrayPtrVoid      m_arrBgBrushes; // Array of background colour brushes.
    wxArrayPtrVoid      m_arrFgCols; // Array of foreground colours.

    wxArrayPtrVoid      m_commonValues;  // labels when properties use common values
    int                 m_cvUnspecified;  // Which cv selection really sets value to unspecified?

protected:

    // Sets some members to defaults (called constructors).
	void Init1();

    // Initializes some members (called by Create and complex constructor).
	void Init2();

	void OnPaint(wxPaintEvent &event );

    // main event receivers
    void OnMouseMove( wxMouseEvent &event );
    void OnMouseMoveBottom( wxMouseEvent &event );
    void OnMouseClick( wxMouseEvent &event );
    void OnMouseRightClick( wxMouseEvent &event );
    void OnMouseDoubleClick( wxMouseEvent &event );
    void OnMouseUp( wxMouseEvent &event );
    void OnKey( wxKeyEvent &event );
    void OnKeyUp( wxKeyEvent &event );
    void OnNavigationKey( wxNavigationKeyEvent& event );
    void OnResize( wxSizeEvent &event );

    // event handlers
    bool HandleMouseMove( int x, unsigned int y, wxMouseEvent &event );
    bool HandleMouseClick( int x, unsigned int y, wxMouseEvent &event );
    bool HandleMouseRightClick( int x, unsigned int y, wxMouseEvent &event );
    bool HandleMouseDoubleClick( int x, unsigned int y, wxMouseEvent &event );
    bool HandleMouseUp( int x, unsigned int y, wxMouseEvent &event );
    void HandleKeyEvent( wxKeyEvent &event );
    bool HandleChildKey( wxKeyEvent& event, bool canDestroy ); // Handle TAB and ESCAPE in control

    void OnMouseEntry( wxMouseEvent &event );

    void OnIdle( wxIdleEvent &event );
    void OnFocusEvent( wxFocusEvent &event );
    void OnChildFocusEvent( wxChildFocusEvent& event );

    bool OnMouseCommon( wxMouseEvent &event, int* px, int *py );
    bool OnMouseChildCommon( wxMouseEvent &event, int* px, int *py );

    // sub-control event handlers
    void OnMouseClickChild( wxMouseEvent &event );
    void OnMouseRightClickChild( wxMouseEvent &event );
    void OnMouseMoveChild( wxMouseEvent &event );
    void OnMouseUpChild( wxMouseEvent &event );
    void OnChildKeyDown( wxKeyEvent &event );
    void OnChildKeyUp( wxKeyEvent &event );

    void OnCaptureChange( wxMouseCaptureChangedEvent &event );

    void OnScrollEvent( wxScrollWinEvent &event );

    void OnSysColourChanged( wxSysColourChangedEvent &event );

protected:

	/** Adjust the centering of the bitmap icons (collapse / expand) when the caption font changes. They need to
	    be centered in the middle of the font, so a bit of deltaY adjustment is needed.
        On entry, m_captionFont must be set to window font. It will be modified properly.
    */
	void CalculateFontAndBitmapStuff( int vspacing );

    wxRect GetEditorWidgetRect( wxPGProperty* p, int column ) const;

    void CorrectEditorWidgetSizeX();

    /** Called usually on expand/collapse to fix editor control position.
        isBelowthis is the expanded/collapsed property.
    */
    void CorrectEditorWidgetPosY( wxPGProperty* isBelowThis, int heightAdd );

#ifdef __WXDEBUG__
    void _log_items();
    void OnScreenNote( const wxChar* format, ... );
#endif

    int DoDrawItems( wxDC& dc,
                     const wxPGProperty* first_item,
                     const wxPGProperty* last_item,
                     const wxRect* clip_rect ) const;

    void DoSetPropertyUnspecified( wxPGProperty* p );

    /** Draws an expand/collapse (ie. +/-) button.
    */
    virtual void DrawExpanderButton( wxDC& dc, const wxRect& rect,
                                     wxPGProperty* property ) const;

    virtual void RefreshProperty( wxPGProperty* p );

    /** Draws items from topitemy to bottomitemy */
    void DrawItems( wxDC& dc, unsigned int topitemy, unsigned int bottomitemy,
                    const wxRect* clip_rect = (const wxRect*) NULL );

    void DrawItems( const wxPGProperty* p1, const wxPGProperty* p2 );

    // In addition to calling DoDrawItems directly, this is the
    // only alternative for using wxClientDC - others just call
    // RefreshRect.
    void DrawItem( wxDC& dc, wxPGProperty* p );

    inline void DrawItem( wxPGProperty* p )
    {
        DrawItems(p,p);
    }

    virtual void DrawItemAndChildren( wxPGProperty* p );

    /** Draws item, children, and consequtive parents as long as category is not met. */
    void DrawItemAndValueRelated( wxPGProperty* p );

#if wxPG_COMPATIBILITY_1_2_0
    /** Returns property reference for given property id. */
    wxPGProperty& GetPropertyById( wxPGPropArg id )
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(*((wxPGProperty*)NULL))
        return *p;
    }
#endif

    void ImprovedClientToScreen( int* px, int* py );

    // Called by focus event handlers. newFocused is the window that becomes focused.
    void HandleFocusChange( wxWindow* newFocused );

    /** Reloads all non-customized colours from system settings. */
    void RegainColours();

    bool DoEditorValidate();

    wxPGProperty* DoGetItemAtY( int y ) const;

    void DoSetSplitterPosition( int newxpos, bool refresh = true, int splitterIndex = 0, bool allPages = true );

    void FreeEditors();

    bool DoExpand( wxPGProperty* p, bool sendEvent = false );

    bool DoCollapse( wxPGProperty* p, bool sendEvent = false );

    // Returns nearest paint visible property (such that will be painted unless
    // window is scrolled or resized). If given property is paint visible, then
    // it itself will be returned.
    wxPGProperty* GetNearestPaintVisible( wxPGProperty* p ) const;

    static void RegisterDefaultEditors();

    // Sets m_bgColIndex to this property and all its children.
    void SetBackgroundColourIndex( wxPGProperty* p, int index );

    // Sets m_fgColIndex to this property and all its children.
    void SetTextColourIndex( wxPGProperty* p, int index, int flags );

    int CacheColour( const wxColour& colour );

    void DoSetPropertyName( wxPGProperty* p, const wxString& newname );

    // Setups event handling for child control
    void SetupEventHandling( wxWindow* wnd, int id );

    void CustomSetCursor( int type, bool override = false );

    virtual bool ProcessEvent(wxEvent& event);

    void RecalculateVirtualSize( int forceXPos = -1 );

    inline bool UsesAutoUnspecified() const
    {
        return ( GetExtraStyle() & wxPG_EX_AUTO_UNSPECIFIED_VALUES ) ? true : false;
    }

    /** When splitter is dragged to a new position, this is drawn. */
    void DrawSplitterDragColumn( wxDC& dc, int x );

    void PrepareAfterItemsAdded();

    bool SendEvent( int eventType, wxPGProperty* p, wxVariant* pValue = NULL, unsigned int selFlags = 0 );

    bool DoHideProperty( wxPGProperty* p, bool hide );

private:

    bool ButtonTriggerKeyTest( wxKeyEvent &event );

#endif // DOXYGEN_SHOULD_SKIP_THIS

private:
    DECLARE_EVENT_TABLE()
#endif // #ifndef SWIG
};

#undef wxPG_USE_STATE

// -----------------------------------------------------------------------
//
// Bunch of inlines that need to resolved after all classes have been defined.
//

#ifndef SWIG
inline bool wxPropertyGridState::IsDisplayed() const
{
    return ( this == m_pPropGrid->GetState() );
}
#endif

inline void wxPGProperty::SetEditor( const wxString& editorName )
{
    EnsureDataExt();
    m_dataExt->m_customEditor = wxPropertyContainerMethods::GetEditorByName(editorName);
}

inline bool wxPGProperty::Hide( bool hide )
{
    return GetGrid()->HideProperty(wxPGIdGen(this),hide);
}

inline bool wxPGProperty::SetMaxLength( int maxLen )
{
    return GetGrid()->SetPropertyMaxLength(wxPGIdGen(this),maxLen);
}

// -----------------------------------------------------------------------

/** \class wxPropertyGridEvent
	\ingroup classes
    \brief A propertygrid event holds information about events associated with
    wxPropertyGrid objects.

    <h4>Derived from</h4>

    wxNotifyEvent\n
    wxCommandEvent\n
    wxEvent\n
    wxObject\n

    <h4>Include files</h4>

    <wx/propertygrid/propertygrid.h>
*/
class WXDLLIMPEXP_PG wxPropertyGridEvent : public wxCommandEvent
{
public:

    /** Constructor. */
    wxPropertyGridEvent(wxEventType commandType=0, int id=0);
#ifndef SWIG
    /** Copy constructor. */
    wxPropertyGridEvent(const wxPropertyGridEvent& event);
#endif
    /** Destructor. */
    ~wxPropertyGridEvent();

    /** Copyer. */
    virtual wxEvent* Clone() const;

    /** Enables property. */
    inline void EnableProperty( bool enable = true )
    {
        m_pg->EnableProperty(wxPGIdGen(m_property),enable);
    }

    /** Disables property. */
    inline void DisableProperty()
    {
        m_pg->EnableProperty(wxPGIdGen(m_property),false);
    }

    inline wxPGProperty* GetMainParent() const
    {
        wxASSERT(m_property);
        return m_property->GetMainParent();
    }

    /** Returns id of associated property. */
    wxPGProperty* GetProperty() const
    {
        return m_property;
    }

#if wxPG_COMPATIBILITY_1_2_0
  #ifndef SWIG
    /** Returns pointer to associated property.
        DEPRECATED: Use GetProperty() instead.
    */
    wxPGProperty* GetPropertyPtr() const
    {
        return m_property;
    }
  #endif
#endif

    /** Returns label of associated property. */
    const wxString& GetPropertyLabel() const
    {
        wxASSERT( m_property );
        return m_property->GetLabel();
    }

    /** Returns global name of associated property. */
    wxString GetPropertyName() const
    {
        wxASSERT( m_property );
        return m_property->GetName();
    }

    /** Returns client data of relevant property. */
    wxPGProperty::ClientDataType GetPropertyClientData() const
    {
        wxASSERT( m_property );
        return m_property->GetClientData();
    }

#ifndef SWIG
    /** Returns value of relevant property. */
    wxVariant GetPropertyValue() const
    {
        wxASSERT( m_property );
        return m_property->GetValue();
    }

    inline wxString GetPropertyValueAsString() const
    {
        return m_pg->GetPropertyValueAsString( wxPGIdGen(m_property) );
    }
    inline long GetPropertyValueAsLong() const
    {
        return m_pg->GetPropertyValueAsLong( wxPGIdGen(m_property) );
    }
    inline int GetPropertyValueAsInt() const
    {
        return (int)GetPropertyValueAsLong();
    }
    inline long GetPropertyValueAsBool() const
    {
        return m_pg->GetPropertyValueAsBool( wxPGIdGen(m_property) );
    }
    inline double GetPropertyValueAsDouble() const
    {
        return m_pg->GetPropertyValueAsDouble( wxPGIdGen(m_property) );
    }
    inline const wxObject* GetPropertyValueAsWxObjectPtr() const
    {
        return m_pg->GetPropertyValueAsWxObjectPtr( wxPGIdGen(m_property) );
    }
    inline void* GetPropertyValueAsVoidPtr() const
    {
        return m_pg->GetPropertyValueAsVoidPtr( wxPGIdGen(m_property) );
    }
    inline wxArrayString GetPropertyValueAsArrayString() const
    {
        return m_pg->GetPropertyValueAsArrayString( wxPGIdGen(m_property) );
    }
    inline wxPoint GetPropertyValueAsPoint() const
    {
        return m_pg->GetPropertyValueAsPoint( wxPGIdGen(m_property) );
    }
    inline wxSize GetPropertyValueAsSize() const
    {
        return m_pg->GetPropertyValueAsSize( wxPGIdGen(m_property) );
    }
    inline wxArrayInt GetPropertyValueAsArrayInt() const
    {
        return m_pg->GetPropertyValueAsArrayInt( wxPGIdGen(m_property) );
    }

#else
    %pythoncode {
        def GetPropertyValue(self):
            return self.GetProperty().GetValue()
    }
#endif

    /** Returns true if event has associated property. */
    inline bool HasProperty() const { return ( m_property != (wxPGProperty*) NULL ); }

    inline bool IsPropertyEnabled() const
    {
        return m_pg->IsPropertyEnabled(wxPGIdGen(m_property));
    }

    /** Returns true if you can veto the action that the event is signaling.
    */
    bool CanVeto() const { return m_canVeto; }

    /** Call this from your event handler to veto action that the event is signaling.
        You can only veto a shutdown if wxPropertyGridEvent::CanVeto returns true.
    */
    void Veto( bool veto = true ) { m_wasVetoed = veto; }

    /** Returns value that is about to be set for wxEVT_PG_CHANGING.
    */
    const wxVariant& GetValue() const
    {
        wxASSERT_MSG( m_value,
                      wxT("Only call GetValue from a handler of event type that supports it") );
        return *m_value;
    }

#ifndef SWIG

    void SetValue( const wxVariant& value ) { m_value = &value; }
    void SetCanVeto( bool canVeto ) { m_canVeto = canVeto; }
    bool WasVetoed() const { return m_wasVetoed; }

    /** Changes the associated property. */
    void SetProperty( wxPGProperty* p ) { m_property = p; }

    void SetPropertyGrid( wxPropertyGrid* pg ) { m_pg = pg; }

private:
    void Init();
    DECLARE_DYNAMIC_CLASS(wxPropertyGridEvent)

    wxPGProperty*       m_property;
    wxPropertyGrid*     m_pg;
    const wxVariant*    m_value;
    bool                m_canVeto;
    bool                m_wasVetoed;
#endif
};


#define wxPG_BASE_EVT_PRE_ID     1775

#ifndef SWIG
BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_PG, wxEVT_PG_SELECTED,           wxPG_BASE_EVT_PRE_ID)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_PG, wxEVT_PG_CHANGING,           wxPG_BASE_EVT_PRE_ID+1)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_PG, wxEVT_PG_CHANGED,            wxPG_BASE_EVT_PRE_ID+2)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_PG, wxEVT_PG_HIGHLIGHTED,        wxPG_BASE_EVT_PRE_ID+3)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_PG, wxEVT_PG_RIGHT_CLICK,        wxPG_BASE_EVT_PRE_ID+4)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_PG, wxEVT_PG_PAGE_CHANGED,       wxPG_BASE_EVT_PRE_ID+5)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_PG, wxEVT_PG_ITEM_COLLAPSED,     wxPG_BASE_EVT_PRE_ID+6)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_PG, wxEVT_PG_ITEM_EXPANDED,      wxPG_BASE_EVT_PRE_ID+7)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_PG, wxEVT_PG_DOUBLE_CLICK,       wxPG_BASE_EVT_PRE_ID+8)
END_DECLARE_EVENT_TYPES()
#else
    enum {
        wxEVT_PG_SELECTED = wxPG_BASE_EVT_PRE_ID,
        wxEVT_PG_CHANGING,
        wxEVT_PG_CHANGED,
        wxEVT_PG_HIGHLIGHTED,
        wxEVT_PG_RIGHT_CLICK,
        wxEVT_PG_PAGE_CHANGED,
        wxEVT_PG_ITEM_COLLAPSED,
        wxEVT_PG_ITEM_EXPANDED,
        wxEVT_PG_DOUBLE_CLICK
    };
#endif


#define wxPG_BASE_EVT_TYPE       wxEVT_PG_SELECTED
#define wxPG_MAX_EVT_TYPE        (wxPG_BASE_EVT_TYPE+30)


#ifndef SWIG
typedef void (wxEvtHandler::*wxPropertyGridEventFunction)(wxPropertyGridEvent&);

#define EVT_PG_SELECTED(id, fn)              DECLARE_EVENT_TABLE_ENTRY( wxEVT_PG_SELECTED, id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxPropertyGridEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_PG_CHANGING(id, fn)              DECLARE_EVENT_TABLE_ENTRY( wxEVT_PG_CHANGING, id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxPropertyGridEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_PG_CHANGED(id, fn)               DECLARE_EVENT_TABLE_ENTRY( wxEVT_PG_CHANGED, id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxPropertyGridEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_PG_HIGHLIGHTED(id, fn)           DECLARE_EVENT_TABLE_ENTRY( wxEVT_PG_HIGHLIGHTED, id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxPropertyGridEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_PG_RIGHT_CLICK(id, fn)           DECLARE_EVENT_TABLE_ENTRY( wxEVT_PG_RIGHT_CLICK, id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxPropertyGridEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_PG_DOUBLE_CLICK(id, fn)          DECLARE_EVENT_TABLE_ENTRY( wxEVT_PG_DOUBLE_CLICK, id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxPropertyGridEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_PG_PAGE_CHANGED(id, fn)          DECLARE_EVENT_TABLE_ENTRY( wxEVT_PG_PAGE_CHANGED, id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxPropertyGridEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_PG_ITEM_COLLAPSED(id, fn)        DECLARE_EVENT_TABLE_ENTRY( wxEVT_PG_ITEM_COLLAPSED, id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxPropertyGridEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_PG_ITEM_EXPANDED(id, fn)         DECLARE_EVENT_TABLE_ENTRY( wxEVT_PG_ITEM_EXPANDED, id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxPropertyGridEventFunction, & fn ), (wxObject *) NULL ),

#define wxPropertyGridEventHandler(A) ((wxObjectEventFunction)(wxEventFunction)(wxPropertyGridEventFunction)&A)

#endif


// -----------------------------------------------------------------------

/** \class wxPropertyGridPopulator
    \ingroup classes
    \brief Allows populating wxPropertyGrid from arbitrary text source.
*/
class WXDLLIMPEXP_PG wxPropertyGridPopulator
{
public:
    /** Default constructor.
    */
    wxPropertyGridPopulator();

    /** Destructor. */
    virtual ~wxPropertyGridPopulator();

    void SetState( wxPropertyGridState* state );

    void SetGrid( wxPropertyGrid* pg );

    /** Appends a new property under bottommost parent.
        \param propClass
        Property class as string.
    */
    wxPGProperty* Add( const wxString& propClass,
                       const wxString& propLabel,
                       const wxString& propName,
                       const wxString* propValue,
                       wxPGChoices* pChoices = NULL );

    /** Pushes property to the back of parent array (ie. it becomes bottommost parent),
        and starts scanning/adding children for it. When finished, parent array is
        returned to the original state.
    */
    void AddChildren( wxPGProperty* property );

    /** Adds attribute to the bottommost property.
        \param type
        Allowed values: "string", (same as string), "int", "bool". Empty string
          mean autodetect.
    */
    bool AddAttribute( const wxString& name,
                       const wxString& type,
                       const wxString& value );

    /** Called once in AddChildren.
    */
    virtual void DoScanForChildren() = 0;

    /** Returns id of parent property for which children can currently be added. */
    wxPGProperty* GetCurParent() const
    {
        return (wxPGProperty*) m_propHierarchy[m_propHierarchy.size()-1];
    }

    wxPropertyGridState* GetState() { return m_state; }
    const wxPropertyGridState* GetState() const { return m_state; }

    /** Like wxString::ToLong, except allows N% in addition of N.
    */
    static bool ToLongPCT( const wxString& s, long* pval, long max );

    /** Parses strings of format "choice1"[=value1] ... "choiceN"[=valueN] into
        wxPGChoices. Registers parsed result using idString (if not empty). Also,
        if choices with given id already registered, then don't parse but return
        those choices instead.
    */
    wxPGChoices ParseChoices( const wxString& choicesString,
                              const wxString& idString );

    /** Implement in derived class to do custom process when an error occurs.
        Default implementation uses wxLogError.
    */
    virtual void ProcessError( const wxString& msg );

protected:

    /** Used property grid. */
    wxPropertyGrid*         m_pg;

    /** Used property grid state. */
    wxPropertyGridState*    m_state;

    /** Tree-hierarchy of added properties (that can have children). */
    wxArrayPGProperty       m_propHierarchy;

    /** Hashmap for string-id to wxPGChoicesData mapping. */
    wxPGHashMapS2P          m_dictIdChoices;
};

// -----------------------------------------------------------------------

//
// Undefine macros that are not needed outside propertygrid sources
//
#ifndef __wxPG_SOURCE_FILE__
    #undef wxPG_FL_DESC_REFRESH_REQUIRED
    #undef wxPG_FL_SCROLLBAR_DETECTED
    #undef wxPG_FL_CREATEDSTATE
    #undef wxPG_FL_NOSTATUSBARHELP
    #undef wxPG_FL_SCROLLED
    #undef wxPG_FL_FOCUS_INSIDE_CHILD
    #undef wxPG_FL_FOCUS_INSIDE
    #undef wxPG_FL_MOUSE_INSIDE_CHILD
    #undef wxPG_FL_CUR_USES_CUSTOM_IMAGE
    #undef wxPG_FL_PRIMARY_FILLS_ENTIRE
    #undef wxPG_FL_VALUE_MODIFIED
    #undef wxPG_FL_MOUSE_INSIDE
    #undef wxPG_FL_FOCUSED
    #undef wxPG_FL_MOUSE_CAPTURED
    #undef wxPG_FL_INITIALIZED
    #undef wxPG_FL_ACTIVATION_BY_CLICK
    #undef wxPG_FL_DONT_CENTER_SPLITTER
    #undef wxPG_SUPPORT_TOOLTIPS
    #undef wxPG_DOUBLE_BUFFER
    #undef wxPG_HEAVY_GFX
    #undef wxPG_ICON_WIDTH
    #undef wxPG_USE_RENDERER_NATIVE
// Following are needed by the manager headers
//    #undef wxPGIdGen
//    #undef wxPGPropNameStr
//    #undef wxPGIdToPtr
#endif

// Doxygen special
#ifdef DOXYGEN
    #include "editor.h"
    #include "manager.h"
#endif

// -----------------------------------------------------------------------

#endif // __WX_PROPGRID_PROPGRID_H__

