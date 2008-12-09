/////////////////////////////////////////////////////////////////////////////
// Name:        propgrid.cpp
// Purpose:     wxPropertyGrid
// Author:      Jaakko Salli
// Modified by:
// Created:     Sep-25-2004
// RCS-ID:      $Id:
// Copyright:   (c) Jaakko Salli
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/defs.h"
    #include "wx/object.h"
    #include "wx/hash.h"
    #include "wx/string.h"
    #include "wx/log.h"
    #include "wx/event.h"
    #include "wx/window.h"
    #include "wx/panel.h"
    #include "wx/dc.h"
    #include "wx/dcmemory.h"
    #include "wx/button.h"
    #include "wx/pen.h"
    #include "wx/brush.h"
    #include "wx/cursor.h"
    #include "wx/dialog.h"
    #include "wx/settings.h"
    #include "wx/msgdlg.h"
    #include "wx/choice.h"
    #include "wx/stattext.h"
    #include "wx/scrolwin.h"
    #include "wx/dirdlg.h"
    //#include "wx/combobox.h"
    #include "wx/layout.h"
    #include "wx/sizer.h"
    #include "wx/textdlg.h"
    #include "wx/filedlg.h"
    #include "wx/statusbr.h"
    #include "wx/intl.h"
    #include "wx/frame.h"
#endif


// This define is necessary to prevent macro clearing
#define __wxPG_SOURCE_FILE__

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/propdev.h>
#include <wx/propgrid/editors.h>

#ifdef __WXPYTHON__
    #include <wx/propgrid/advprops.h>
    #include <wx/propgrid/extras.h>
#endif

#if wxPG_USE_RENDERER_NATIVE
    #include <wx/renderer.h>
#endif

#if wxPG_USING_WXOWNERDRAWNCOMBOBOX
    #include <wx/odcombo.h>
#else
    #include <wx/propgrid/odcombo.h>
#endif

#include "wx/timer.h"
#include "wx/dcbuffer.h"

#ifdef __WXMSW__
    #include <wx/msw/private.h>
#endif


// Two pics for the expand / collapse buttons.
// Files are not supplied with this project (since it is
// recommended to use either custom or native rendering).
// If you want them, get wxTreeMultiCtrl by Jorgen Bodde,
// and copy xpm files from archive to wxPropertyGrid src directory
// (and also comment/undef wxPG_ICON_WIDTH in propGrid.h
// and set wxPG_USE_RENDERER_NATIVE to 0).
#ifndef wxPG_ICON_WIDTH
  #if defined(__WXMAC__)
    #include "mac_collapse.xpm"
    #include "mac_expand.xpm"
  #elif defined(__WXGTK__)
    #include "linux_collapse.xpm"
    #include "linux_expand.xpm"
  #else
    #include "default_collapse.xpm"
    #include "default_expand.xpm"
  #endif
#endif


//#define wxPG_TEXT_INDENT                4 // For the wxComboControl
#define wxPG_ALLOW_CLIPPING             1 // If 1, GetUpdateRegion() in OnPaint event handler is not ignored
#define wxPG_GUTTER_DIV                 3 // gutter is max(iconwidth/gutter_div,gutter_min)
#define wxPG_GUTTER_MIN                 3 // gutter before and after image of [+] or [-]
#define wxPG_YSPACING_MIN               1
#define wxPG_DEFAULT_VSPACING           2 // This matches .NET propertygrid's value,
                                          // but causes normal combobox to spill out under MSW

#define wxPG_OPTIMAL_WIDTH              200 // Arbitrary

#define wxPG_CAPRECTXMARGIN             2 // space between caption and selection rectangle,
#define wxPG_CAPRECTYMARGIN             1 // horizontally and vertically


#define PWC_CHILD_SUMMARY_LIMIT         16 // Maximum number of children summarized in a parent property's
                                           // value field.

#define PWC_CHILD_SUMMARY_CHAR_LIMIT    64 // Character limit of summary field when not editing

#define wxPG_MIN_SCROLLBAR_WIDTH        10 // Smallest scrollbar width on any platform
                                           // Must be larger than largest control border
                                           // width * 2.


#define wxPG_DEFAULT_CURSOR             wxNullCursor
#define RedrawAllVisible                Refresh


//
// Here are some extra platform dependent defines.
//

#if defined(__WXMSW__)
    // tested

    #define wxPG_DEFAULT_SPLITTERX      110 // default splitter position

    #define wxPG_CREATE_CONTROLS_HIDDEN 0 // 1 to create controls out of sight, hide them, and then move them into correct position

    #define wxPG_NO_CHILD_EVT_MOTION    0 // 1 if splitter drag detect margin and control cannot overlap

    #define wxPG_CUSTOM_IMAGE_WIDTH     20 // for wxColourProperty etc.

    #define wxPG_ALLOW_EMPTY_TOOLTIPS   1  // If 1, then setting empty tooltip actually hides it

    #define wxPG_NAT_BUTTON_BORDER_ANY          1
    #define wxPG_NAT_BUTTON_BORDER_X            1
    #define wxPG_NAT_BUTTON_BORDER_Y            1

    #define wxPG_REFRESH_CONTROLS_AFTER_REPAINT 0 // If 1 then controls are refreshed after selected was drawn.

#elif defined(__WXGTK__)
    // tested

    #define wxPG_DEFAULT_SPLITTERX      110

    #define wxPG_CREATE_CONTROLS_HIDDEN 0 // 1 to create controls out of sight, hide them, and then move them into correct position

    #define wxPG_NO_CHILD_EVT_MOTION    1 // 1 if splitter drag detect margin and control cannot overlap

    #define wxPG_CUSTOM_IMAGE_WIDTH     20 // for wxColourProperty etc.

    #define wxPG_ALLOW_EMPTY_TOOLTIPS   0  // If 1, then setting empty tooltip actually hides it

    #define wxPG_NAT_BUTTON_BORDER_ANY      1
    #define wxPG_NAT_BUTTON_BORDER_X        1
    #define wxPG_NAT_BUTTON_BORDER_Y        1

    #define wxPG_REFRESH_CONTROLS_AFTER_REPAINT 1 // If 1 then controls are refreshed after selected was drawn.

#elif defined(__WXMAC__)
    // *not* tested

    #define wxPG_DEFAULT_SPLITTERX      110

    #define wxPG_CREATE_CONTROLS_HIDDEN 0 // 1 to create controls out of sight, hide them, and then move them into correct position

    #define wxPG_NO_CHILD_EVT_MOTION    0 // 1 if splitter drag detect margin and control cannot overlap

    #define wxPG_CUSTOM_IMAGE_WIDTH     20 // for wxColourProperty etc.

    #define wxPG_ALLOW_EMPTY_TOOLTIPS   1  // If 1, then setting empty tooltip actually hides it

    #define wxPG_NAT_BUTTON_BORDER_ANY      0
    #define wxPG_NAT_BUTTON_BORDER_X        0
    #define wxPG_NAT_BUTTON_BORDER_Y        0

    #define wxPG_REFRESH_CONTROLS_AFTER_REPAINT 0 // If 1 then controls are refreshed after selected was drawn.

#else
    // defaults

    #define wxPG_DEFAULT_SPLITTERX      110

    #define wxPG_CREATE_CONTROLS_HIDDEN 0 // 1 to create controls out of sight, hide them, and then move them into correct position

    #define wxPG_NO_CHILD_EVT_MOTION    1 // 1 if splitter drag detect margin and control cannot overlap

    #define wxPG_CUSTOM_IMAGE_WIDTH     20 // for wxColourProperty etc.

    #define wxPG_ALLOW_EMPTY_TOOLTIPS   0  // If 1, then setting empty tooltip actually hides it

    #define wxPG_NAT_BUTTON_BORDER_ANY      0
    #define wxPG_NAT_BUTTON_BORDER_X        0
    #define wxPG_NAT_BUTTON_BORDER_Y        0

    #define wxPG_REFRESH_CONTROLS_AFTER_REPAINT 1 // If 1 then controls are refreshed after selected was drawn.

#endif


#if wxPG_NO_CHILD_EVT_MOTION

    #define wxPG_SPLITTERX_DETECTMARGIN1    3 // this much on left
    #define wxPG_SPLITTERX_DETECTMARGIN2    2 // this much on right

#else

    #define wxPG_SPLITTERX_DETECTMARGIN1    3 // this much on left
    #define wxPG_SPLITTERX_DETECTMARGIN2    2 // this much on right

#endif



//#define wxPG_NAT_CHOICE_BORDER_ANY   0

#define wxPG_DRAG_MARGIN                30

#define wxPG_CUSTOM_IMAGE_SPACINGY      1 // space between vertical sides of a custom image

#define wxPG_HIDER_BUTTON_HEIGHT        25

#define wxPG_PIXELS_PER_UNIT            m_lineHeight

#ifdef wxPG_ICON_WIDTH
  #define m_iconHeight m_iconWidth
#endif

#define wxPG_TOOLTIP_DELAY              1000

// Colour for the empty but visible space below last property.
#define wxPG_SLACK_BACKROUND        m_colPropBack


// Helper to decide which way is better (ie. first macro clears
// "unspecified" state of siblings of child properties as well, while the latter is
// more precise).
#define CLEAR_PROPERTY_UNSPECIFIED_FLAG(p) if ( p->GetValueRef().IsNull() ) p->SetValue(p->GetDefaultValue())

#define __INTENSE_DEBUGGING__       0
#define __PAINT_DEBUGGING__         0
#define __MOUSE_DEBUGGING__         0


#if wxMINOR_VERSION < 7 || ( wxMINOR_VERSION == 7 && wxRELEASE_NUMBER < 1 )
// NOTE: Since wxWidgets at this point doesn't have wxArrayDouble, we have
//   to create it ourself, using wxObjArray model.
#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(wxArrayDouble);
#endif

// -----------------------------------------------------------------------

#if wxUSE_INTL
void wxPropertyGrid::AutoGetTranslation ( bool enable )
{
    WX_PG_GLOBALS_LOCKER()

    wxPGGlobalVars->m_autoGetTranslation = enable;
}
#else
void wxPropertyGrid::AutoGetTranslation ( bool ) { }
#endif

// -----------------------------------------------------------------------

#if !wxCHECK_VERSION(2, 7, 1)

#if defined(__WXMSW__)

#ifndef WS_EX_COMPOSITED
    #define WS_EX_COMPOSITED        0x02000000L
#endif
static bool wxPGIsWindowBuffered( const wxWindow* wnd )
{
    while ( wnd )
    {
        if ( GetWindowLong((HWND)wnd->GetHWND(), GWL_EXSTYLE) & WS_EX_COMPOSITED )
            return true;
        if ( wnd->IsTopLevel() )
            break;
        wnd = wnd->GetParent();
    }

    return false;
}

#elif defined(__WXGTK20__)

#include <gtk/gtk.h>
static bool wxPGIsWindowBuffered( const wxWindow* wnd )
{
    return GTK_WIDGET_DOUBLE_BUFFERED(wnd->GetHandle());
}

#elif defined(__WXMAC_OSX__) || defined(__WXCOCOA__) || defined(__WXDFB__)

static bool wxPGIsWindowBuffered( const wxWindow* WXUNUSED(wnd) )
{
    return true;
}

#else

static bool wxPGIsWindowBuffered( const wxWindow* WXUNUSED(wnd) )
{
    return false;
}

#endif

#else

static bool wxPGIsWindowBuffered( const wxWindow* wnd )
{
    return wnd->IsDoubleBuffered();
}

#endif


// -----------------------------------------------------------------------

// DeviceContext Init Macros.

#define wxPG_CLIENT_DC_INIT() \
    wxClientDC dc(m_canvas);

#define wxPG_CLIENT_DC_INIT_R(RETVAL) \
    wxClientDC dc(m_canvas);

// -----------------------------------------------------------------------

// For wxMSW cursor consistency, we must do mouse capturing even
// when using custom controls.

#define BEGIN_MOUSE_CAPTURE \
    if ( !(m_iFlags & wxPG_FL_MOUSE_CAPTURED) ) \
    { \
        m_canvas->CaptureMouse(); \
        m_iFlags |= wxPG_FL_MOUSE_CAPTURED; \
    }

#define END_MOUSE_CAPTURE \
    if ( m_iFlags & wxPG_FL_MOUSE_CAPTURED ) \
    { \
        m_canvas->ReleaseMouse(); \
        m_iFlags &= ~(wxPG_FL_MOUSE_CAPTURED); \
    }

// -----------------------------------------------------------------------

const wxChar *wxPropertyGridNameStr = wxT("wxPropertyGrid");

const wxChar *wxPGTypeName_long = wxT("long");
const wxChar *wxPGTypeName_bool = wxT("bool");
const wxChar *wxPGTypeName_double = wxT("double");
const wxChar *wxPGTypeName_wxString = wxT("string");
const wxChar *wxPGTypeName_void = wxT("void*");
const wxChar *wxPGTypeName_wxArrayString = wxT("arrstring");

#ifdef __WXPYTHON__
const wxChar *wxPGTypeName_PyObject = wxT("PyObject");
#endif

// -----------------------------------------------------------------------

static void wxPGDrawFocusRect( wxDC& dc, const wxRect& rect )
{
#if defined(__WXMSW__) && !defined(__WXWINCE__)
    // FIXME: Use DrawFocusRect code above (currently it draws solid line
    //   for caption focus but works ok for other stuff).
    //   Also, it seems that this code may not work in future wx versions.
    dc.SetLogicalFunction(wxINVERT);

    wxPen pen(*wxBLACK,1,wxDOT);
    pen.SetCap(wxCAP_BUTT);
    dc.SetPen(pen);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    dc.DrawRectangle(rect);

    dc.SetLogicalFunction(wxCOPY);
#else
    dc.SetLogicalFunction(wxINVERT);

    dc.SetPen(wxPen(*wxBLACK,1,wxDOT));
    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    dc.DrawRectangle(rect);

    dc.SetLogicalFunction(wxCOPY);
#endif
}


// -----------------------------------------------------------------------
// Choice related methods from various classes
// -----------------------------------------------------------------------

void wxPropertyContainerMethods::AddPropertyChoice( wxPGPropArg id,
                                                    const wxString& label,
                                                    int value )
{
    wxPG_PROP_ARG_CALL_PROLOG()

    p->InsertChoice(label,-1,value);
}


void wxPropertyContainerMethods::InsertPropertyChoice( wxPGPropArg id,
                                                       const wxString& label,
                                                       int index,
                                                       int value )
{
    wxPG_PROP_ARG_CALL_PROLOG()

    p->InsertChoice(label,index,value);
}


void wxPropertyContainerMethods::DeletePropertyChoice( wxPGPropArg id,
                                                       int index )
{
    wxPG_PROP_ARG_CALL_PROLOG()

    p->DeleteChoice(index);
}


// -----------------------------------------------------------------------
// Statics in one class for easy destruction.
// NB: We prefer to use wxModule, as it offers more consistent behaviour
//     across platforms. However, for those rare problem situations, we
//     also need to offer option to use simpler approach.
// -----------------------------------------------------------------------

#ifndef wxPG_USE_WXMODULE
    #define wxPG_USE_WXMODULE 1
#endif

#if wxPG_USE_WXMODULE

#include <wx/module.h>

class wxPGGlobalVarsClassManager : public wxModule
{
    DECLARE_DYNAMIC_CLASS(wxPGGlobalVarsClassManager)
public:
    wxPGGlobalVarsClassManager() {}
    virtual bool OnInit() { wxPGGlobalVars = new wxPGGlobalVarsClass(); return true; }
    virtual void OnExit() { delete wxPGGlobalVars; wxPGGlobalVars = NULL; }
};

IMPLEMENT_DYNAMIC_CLASS(wxPGGlobalVarsClassManager, wxModule)

#else // !wxPG_USE_WXMODULE

class wxPGGlobalVarsClassManager
{
public:
    wxPGGlobalVarsClassManager() {}
    ~wxPGGlobalVarsClassManager() { delete wxPGGlobalVars; }
};

static wxPGGlobalVarsClassManager gs_pgGlobalVarsClassManager;

#endif


wxPGGlobalVarsClass* wxPGGlobalVars = (wxPGGlobalVarsClass*) NULL;


wxPGGlobalVarsClass::wxPGGlobalVarsClass()
{
    m_boolChoices.Add(_("False"));
    m_boolChoices.Add(_("True"));

    m_fontFamilyChoices = (wxPGChoices*) NULL;

    m_defaultRenderer = new wxPGDefaultRenderer();

    m_autoGetTranslation = false;

    m_offline = 0;

    m_extraStyle = 0;

    wxVariant v;

    v = (long)0;
    wxVariantClassInfo_long = wxPGVariantDataGetClassInfo(v.GetData());

    v = wxString();
    wxVariantClassInfo_string = wxPGVariantDataGetClassInfo(v.GetData());

    v = (double)0.0;
    wxVariantClassInfo_double = wxPGVariantDataGetClassInfo(v.GetData());

    v = (bool)false;
    wxVariantClassInfo_bool = wxPGVariantDataGetClassInfo(v.GetData());

    v = wxArrayString();
    wxVariantClassInfo_arrstring = wxPGVariantDataGetClassInfo(v.GetData());

    wxColour col;
    wxVariant v2((wxObject*)&col);
    wxVariantClassInfo_wxobject = wxPGVariantDataGetClassInfo(v2.GetData());

    wxVariantList list;
    v = wxVariant(list);
    wxVariantClassInfo_list = wxPGVariantDataGetClassInfo(v.GetData());

#if wxCHECK_VERSION(2,8,0)
    v << *wxRED;
    wxVariantClassInfo_wxColour = wxPGVariantDataGetClassInfo(v.GetData());
#endif

#if wxUSE_DATETIME
    v = wxVariant(wxDateTime::Now());
    wxVariantClassInfo_datetime = wxPGVariantDataGetClassInfo(v.GetData());
#endif

	// Prepare some shared variants
    m_vEmptyString = wxString();
    m_vZero = (long) 0;
    m_vMinusOne = (long) -1;
    m_vTrue = true;
    m_vFalse = false;

#ifdef __WXDEBUG__
    m_warnings = 0;
#endif
}


wxPGGlobalVarsClass::~wxPGGlobalVarsClass()
{
    size_t i;

    delete m_defaultRenderer;

    // This will always have one ref
    delete m_fontFamilyChoices;

#if wxUSE_VALIDATORS
    for ( i=0; i<m_arrValidators.GetCount(); i++ )
        delete ((wxValidator*)m_arrValidators[i]);
#endif

    //
    // Destroy value type class instances.
    wxPGHashMapS2P::iterator vt_it;

    // Destroy editor class instances.
    // iterate over all the elements in the class
    for( vt_it = m_mapEditorClasses.begin(); vt_it != m_mapEditorClasses.end(); ++vt_it )
    {
        delete ((wxPGEditor*)vt_it->second);
    }
}

// -----------------------------------------------------------------------
// wxPGProperty
// -----------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxPGProperty, wxObject)


void wxPGProperty::Init()
{
#ifdef __WXPYTHON__
    m_scriptObject = NULL;
#endif

    m_commonValue = -1;
    m_arrIndex = 0xFFFF;
    m_parent = NULL;

    m_parentState = (wxPropertyGridState*) NULL;

    m_clientData = NULL;

    m_dataExt = (wxPGPropertyDataExt*) NULL;

    m_maxLen = 0; // infinite maximum length

    m_flags = wxPG_PROP_PROPERTY;

    m_depth = 1;
    m_bgColIndex = 0;
    m_fgColIndex = 0;

    SetExpanded(true);
}


void wxPGProperty::Init( const wxString& label, const wxString& name )
{
    if ( &label != ((wxString*)NULL) )
        m_label = label;

#ifndef __WXPYTHON__
    if ( &name != ((wxString*)NULL) )
#else
    if ( (&name != ((wxString*)NULL)) && name != wxT("_LABEL_AS_NAME") )
#endif
        DoSetName( name );
    else
        DoSetName( m_label );

    Init();
}

wxPGProperty::wxPGProperty()
    : wxObject()
{
    Init();
}


wxPGProperty::wxPGProperty( const wxString& label, const wxString& name )
    : wxObject()
{
    Init( label, name );
}


wxPGProperty::~wxPGProperty()
{
#ifdef __WXPYTHON__
    if ( m_clientData )
        Py_DECREF( m_clientData );
#endif

    Empty();  // this deletes items

    unsigned int i;

    for ( i=0; i<m_cells.size(); i++ )
        delete (wxPGCell*) m_cells[i];

    delete m_dataExt;

    // This makes it easier for us to detect dangling pointers
    m_parent = NULL;
}


bool wxPGProperty::IsSomeParent( wxPGProperty* candidate ) const
{
    wxPGProperty* parent = m_parent;
    do
    {
        if ( parent == candidate )
            return true;
        parent = parent->m_parent;
    } while ( parent );
    return false;
}


wxString wxPGProperty::GetName() const
{
    wxPGProperty* parent = GetParent();

    if ( !m_name.length() || !parent || parent->IsCategory() || parent->IsRoot() )
        return m_name;

    return m_parent->GetName() + wxT(".") + m_name;
}


void wxPGProperty::ShowError( const wxString& msg ) const
{
    if ( !msg.length() )
        return;

#if wxUSE_STATUSBAR
    if ( !wxPGGlobalVars->m_offline )
    {
        wxPropertyGrid* pg = GetGrid();
        wxASSERT(pg);
        wxWindow* topWnd = ::wxGetTopLevelParent(pg);
        if ( topWnd )
        {
            wxFrame* pFrame = wxDynamicCast(topWnd,wxFrame);
            if ( pFrame )
            {
                wxStatusBar* pStatusBar = pFrame->GetStatusBar();
                if ( pStatusBar )
                {
                    pStatusBar->SetStatusText(msg);
                    return;
                }
            }
        }
    }
#endif
    ::wxLogError(msg);
}


wxPropertyGrid* wxPGProperty::GetGrid() const
{
    wxASSERT(m_parentState);
    return m_parentState->GetGrid();
}


void wxPGProperty::UpdateControl( wxWindow* primary )
{
    if ( primary )
        GetEditorClass()->UpdateControl(this, primary);
}

bool wxPGProperty::ValidateValue( wxVariant& WXUNUSED(value) ) const
{
    return true;
}

void wxPGProperty::OnSetValue()
{
}

void wxPGProperty::RefreshChildren ()
{
}


wxString wxPGProperty::GetValueAsString( int argFlags ) const
{
    wxCHECK_MSG( GetCount() > 0,
                 wxString(),
                 wxT("If user property does not have any children, it must override GetValueAsString") );

    wxString text;

    int i;
    int iMax = m_children.GetCount();

    if ( iMax > PWC_CHILD_SUMMARY_LIMIT &&
         !(argFlags & wxPG_FULL_VALUE) )
        iMax = PWC_CHILD_SUMMARY_LIMIT;

    int iMaxMinusOne = iMax-1;

    wxPGProperty* curChild = (wxPGProperty*) m_children.Item(0);

    for ( i = 0; i < iMax; i++ )
    {
        wxString s;
        if ( !curChild->IsValueUnspecified() )
            s = curChild->GetValueString(argFlags);

        if ( !curChild->GetChildCount() )
            text += s;
        else
            text += wxT("[") + s + wxT("]");

        if ( i < iMaxMinusOne )
        {
            if ( text.length() > PWC_CHILD_SUMMARY_CHAR_LIMIT &&
                 !(argFlags & wxPG_EDITABLE_VALUE) &&
                 !(argFlags & wxPG_FULL_VALUE) )
                break;

            curChild = (wxPGProperty*) m_children.Item(i+1);

            if ( !curChild->GetChildCount() )
                text += wxT("; ");
            else
                text += wxT(" ");
        }
    }

    if ( (unsigned int)i < m_children.GetCount() )
        text += wxT("; ...");

    return text;
}

wxString wxPGProperty::GetValueString( int argFlags ) const
{
    if ( IsValueUnspecified() )
        return wxEmptyString;

    if ( m_commonValue == -1 )
        return GetValueAsString(argFlags);

    //
    // Return common value's string representation
    wxPropertyGrid* pg = GetGrid();
    const wxPGCommonValue* cv = pg->GetCommonValue(m_commonValue);

    if ( argFlags & wxPG_FULL_VALUE )
    {
        return cv->GetLabel();
    }
    else if ( argFlags & wxPG_EDITABLE_VALUE )
    {
        return cv->GetEditableText();
    }
    else
    {
        return cv->GetLabel();
    }
}

bool wxPGProperty::IntToValue( wxVariant& variant, int number, int WXUNUSED(argFlags) ) const
{
    variant = (long)number;
    return true;
}

// Convert semicolon delimited tokens into child values.
bool wxPGProperty::StringToValue( wxVariant& variant, const wxString& text, int argFlags ) const
{
    if ( !GetCount() )
        return false;

    unsigned int curChild = 0;

    unsigned int iMax = m_children.GetCount();

    if ( iMax > PWC_CHILD_SUMMARY_LIMIT &&
         !(argFlags & wxPG_FULL_VALUE) )
        iMax = PWC_CHILD_SUMMARY_LIMIT;

    bool changed = false;

    wxString token;
    size_t pos = 0;

    // Its best only to add non-empty group items
    bool addOnlyIfNotEmpty = false;
    const wxChar delimeter = wxT(';');
    wxChar a;

    size_t lastPos = text.length();
    size_t tokenStart = 0xFFFFFF;

    wxVariantList temp_list;
    wxVariant list(temp_list);

    do
    {
        a = text[pos];

        if ( tokenStart != 0xFFFFFF )
        {
            // Token is running
            if ( a == delimeter || a == 0 )
            {
                token = text.substr(tokenStart,pos-tokenStart);
                token.Trim(true);
                size_t len = token.length();

                if ( !addOnlyIfNotEmpty || len > 0 )
                {
                    const wxPGProperty* child = Item(curChild);

                    if ( len > 0 )
                    {
                        bool wasUnspecified = child->IsValueUnspecified();

                        wxVariant variant(child->GetValueRef());
                        if ( child->ActualStringToValue(variant, token, wxPG_REPORT_ERROR) )
                        {
                            // Use label instead of name, as name can be empty string, but
                            // label in practice never is.
                            variant.SetName(child->GetLabel());

                            // Clear unspecified flag only if OnSetValue() didn't
                            // affect it.
                            if ( child->IsValueUnspecified() &&
                                 (wasUnspecified || !UsesAutoUnspecified()) )
                            {
                                variant = child->GetDefaultValue();
                            }

                            list.Append(variant);

                            changed = true;
                        }
                    }
                    else
                    {
                        wxVariant variant2;
                        variant2.SetName(child->GetLabel());
                        list.Append(variant2);
                        changed = true;
                    }

                    curChild++;
                    if ( curChild >= iMax )
                        break;
                }

                tokenStart = 0xFFFFFF;
            }
        }
        else
        {
            // Token is not running
            if ( a != wxT(' ') )
            {

                addOnlyIfNotEmpty = false;

                // Is this a group of tokens?
                if ( a == wxT('[') )
                {
                    int depth = 1;

                    pos++;
                    size_t startPos = pos;

                    // Group item - find end
                    do
                    {
                        a = text[pos];
                        pos++;

                        if ( a == wxT(']') )
                            depth--;
                        else if ( a == wxT('[') )
                            depth++;

                    } while ( depth > 0 && a );

                    token = text.substr(startPos,pos-startPos-1);

                    if ( !token.length() )
                        break;

                    const wxPGProperty* child = Item(curChild);

                    wxVariant variant(child->GetValueRef());
                    if ( child->ActualStringToValue( variant, token, wxPG_REPORT_ERROR ) )
                    {
                        // Use label instead of name, as name can be empty string, but
                        // label in practice never is.
                        variant.SetName(child->GetLabel());
                        list.Append(variant);
                        changed = true;
                    }
                    else
                    {
                        wxVariant variant2;
                        variant2.SetName(child->GetLabel());
                        list.Append(variant2);
                        changed = true;
                    }

                    curChild++;
                    if ( curChild >= iMax )
                        break;

                    addOnlyIfNotEmpty = true;

                    tokenStart = 0xFFFFFF;
                }
                else
                {
                    tokenStart = pos;

                    if ( a == delimeter )
                    {
                        pos--;
                    }
                }
            }

        }
        pos++;

    }
    while ( pos <= lastPos );

    // This ensures that the last item is set unspecified even
    // if the blank had no terminating delimiter.
    if ( curChild < iMax )
    {
        list.Append(wxVariant());
        changed = true;
    }

    if ( changed )
        variant = list;

    return changed;
}

#ifdef __WXPYTHON__
wxPGVariantAndBool wxPGProperty::PyValidateValue( const wxVariant& value ) const
{
    wxPGVariantAndBool vab;
    vab.m_value = value;
    vab.m_valueValid = true;
    vab.m_result = ValidateValue(vab.m_value);
    return vab;
}

wxPGVariantAndBool wxPGProperty::PyStringToValue( const wxString& text, int argFlags ) const
{
    wxPGVariantAndBool vab;
    vab.m_result = StringToValue(vab.m_value, text, argFlags);
    if ( vab.m_result )
        vab.m_valueValid = true;
    return vab;
}

wxPGVariantAndBool wxPGProperty::PyIntToValue( int number, int argFlags ) const
{
    wxPGVariantAndBool vab;
    vab.m_result = IntToValue(vab.m_value, number, argFlags);
    if ( vab.m_result )
        vab.m_valueValid = true;
    return vab;
}
#endif

bool wxPGProperty::SetValueFromString( const wxString& text, int argFlags )
{
#if wxPG_COMPATIBILITY_1_2_0
    if ( argFlags == 0xFFFF )
    {
        // Do not override! (for backwards compliancy)
        m_commonValue = -1234;
        return true;
    }
#endif

    wxVariant variant(m_value);
    bool res = ActualStringToValue(variant, text, argFlags);
    if ( res )
        SetValue(variant);
    return res;
}

bool wxPGProperty::SetValueFromInt( long number, int argFlags )
{
#if wxPG_COMPATIBILITY_1_2_0
    if ( argFlags == 0xFFFF )
    {
        // Do not override! (for backwards compliancy)
        m_commonValue = -1234;
        return true;
    }
#endif

    wxVariant variant(m_value);
    bool res = ActualIntToValue(variant, number, argFlags);
    if ( res )
        SetValue(variant);
    return res;
}

wxSize wxPGProperty::OnMeasureImage( int WXUNUSED(item) ) const
{
    if ( m_dataExt && m_dataExt->m_valueBitmap )
        return wxSize(m_dataExt->m_valueBitmap->GetWidth(),-1);

    return wxSize(0,0);
}

#if wxPG_COMPATIBILITY_1_2_0
wxSize wxPGProperty::GetImageSize() const
{
    return wxSize(-1234,-1234);
}
#endif

wxPGCellRenderer* wxPGProperty::GetCellRenderer( int WXUNUSED(column) ) const
{
    return wxPGGlobalVars->m_defaultRenderer;
}


void wxPGProperty::OnCustomPaint( wxDC& dc,
                                  const wxRect& rect,
                                  wxPGPaintData& )
{
    wxCHECK_RET( m_dataExt, wxT("m_dataExt is mandatory") );

    wxBitmap* bmp = m_dataExt->m_valueBitmap;

    wxCHECK_RET( bmp && bmp->Ok(), wxT("invalid bitmap") );

    wxCHECK_RET( rect.x >= 0, wxT("unexpected measure call") );

    dc.DrawBitmap(*bmp,rect.x,rect.y);
}

const wxPGEditor* wxPGProperty::DoGetEditorClass() const
{
    return wxPG_EDITOR(TextCtrl);
}

#ifdef __WXPYTHON__
wxString wxPGProperty::GetEditor() const
{
    return wxEmptyString;
}
#endif


// Default extra property event handling - that is, none at all.
bool wxPGProperty::OnEvent( wxPropertyGrid*, wxWindow*, wxEvent& )
{
    return false;
}


void wxPGProperty::SetValue0( wxVariant& value, wxVariant* pList, bool refreshEditor )
{
    SetCommonValue(-1);
#ifdef __WXPYTHON__
    // Translate Py_None to Null wxVariant
    PyObject* obj = PyObjectFromVariant(value);
    if ( obj )
    {
        if ( obj == Py_None )
            value.MakeNull();
    }
#endif

    if ( !value.IsNull() )
    {
        // List variants are reserved a special purpose
        // as intermediate containers for child values
        // of properties with children.
        if ( wxPGIsVariantType(value, list) )
        {
            wxVariant newValue;
            AdaptListToValue(value, &newValue);
            value = newValue;
        }

        if ( pList )
        {
            wxASSERT( wxPGIsVariantType(*pList, list) );
            wxASSERT( GetChildCount() );
            wxASSERT( !IsCategory() );

            wxPGIndex i;
            unsigned int n = 0;

            for ( i=0; i<(wxPGIndex)pList->GetCount(); i++ )
            {
                wxVariant childValue = (*pList)[i];
                wxPGProperty* child = Item(n);
                if ( childValue.GetName() == child->GetLabel() )
                {
                    if ( !wxPG_VARIANT_EQ(child->GetValue(), childValue) )
                        // This flag is not normally set when setting value programmatically.
                        // However, this loop is usually only executed when called from
                        // DoPropertyChanged, which should set this flag.
                        child->SetFlag(wxPG_PROP_MODIFIED);
                    n++;
                }
            }
        }

        if ( ActualValidateValue(value) )
        {
            if ( !value.IsNull() )
            {
                wxPGVariantAssign(m_value, value);
                OnSetValue();
            }
            RefreshChildren();
        }
    }
    else
    {
        m_value = value;
        // TODO: Refresh Children to NULL
    }

    //
    // Update editor control
    //

    // We need to check for these, otherwise GetGrid() may fail.
    if ( refreshEditor )
        RefreshEditor();
}


void wxPGProperty::SetValue( wxVariant value )
{
    SetValue0(value);
}


void wxPGProperty::RefreshEditor()
{
    if ( m_parent && GetParentState() )
    {
        wxPropertyGrid* pg = GetParentState()->GetGrid();
        if ( pg->GetSelectedProperty() == this )
        {
            wxWindow* editor = pg->GetEditorControl();
            if ( editor )
                GetEditorClass()->UpdateControl( this, editor );
        }
    }
}


wxVariant wxPGProperty::GetDefaultValue() const
{
    wxVariant defVal = GetAttribute(wxT("DefaultValue"));
    if ( !defVal.IsNull() )
        return defVal;

    wxVariant value = GetValue();

    if ( !value.IsNull() )
    {
        wxPGVariantDataClassInfo classInfo = wxPGVariantDataGetClassInfo(value.GetData());
        if ( wxPGIsVariantClassInfo(classInfo, long) )
            return wxPGVariant_Zero;
        if ( wxPGIsVariantClassInfo(classInfo, string) )
            return wxPGVariant_EmptyString;
        if ( wxPGIsVariantClassInfo(classInfo, bool) )
            return wxPGVariant_False;
        if ( wxPGIsVariantClassInfo(classInfo, double) )
            return wxVariant(0.0);

        wxPGVariantData* pgvdata = wxDynamicCastVariantData(m_value.GetData(), wxPGVariantData);
        if ( pgvdata )
            return pgvdata->GetDefaultValue();

        if ( wxPGIsVariantClassInfo(classInfo, arrstring) )
            return wxVariant(wxArrayString());
#if wxCHECK_VERSION(2,8,0)
        if ( wxPGIsVariantClassInfo(classInfo, wxColour) )
            return WXVARIANT(*wxRED);
#endif
#if wxUSE_DATETIME
        if ( wxPGIsVariantClassInfo(classInfo, datetime) )
            return wxVariant(wxDateTime::Now());
#endif

        wxFAIL_MSG(
            wxString::Format(wxT("Inorder for value to have default value, it must be added to")
                             wxT("wxPGProperty::GetDefaultValue or it's variantdata must inherit")
                             wxT("from wxPGVariantData (unrecognized type was '%s')"),m_value.GetType().c_str())
                  );
    }

    return wxVariant();
}

int wxPGProperty::GetDisplayedCommonValueCount() const
{
    if ( m_flags & wxPG_PROP_DENY_COMMON_VALUES )
        return 0;

    return GetGrid()->GetDisplayedCommonValueCount();
}

void wxPGProperty::SetCell( int column, wxPGCell* cellObj )
{
    if ( column >= (int)m_cells.size() )
        m_cells.SetCount(column+1, NULL);

    delete (wxPGCell*) m_cells[column];
    m_cells[column] = cellObj;
}

void wxPGProperty::SetChoiceSelection( int newValue, const wxPGChoiceInfo& choiceInfo )
{
    // Changes value of a property with choices, but only
    // works if the value type is long or string.
    wxString ts = GetValue().GetType();

    wxCHECK_RET( choiceInfo.m_choices, wxT("invalid choiceinfo") );

    if ( ts == wxT("long") )
    {
        SetValue( (long) newValue );
    }
    else if ( ts == wxT("string") )
    {
        SetValue( choiceInfo.m_choices->GetLabel(newValue) );
    }
}


wxString wxPGProperty::GetChoiceString( unsigned int index )
{
    wxPGChoiceInfo ci;
    GetChoiceInfo(&ci);
    wxASSERT(ci.m_choices);
    return ci.m_choices->GetLabel(index);
}

int wxPGProperty::InsertChoice( const wxString& label, int index, int value )
{
    wxPropertyGrid* pg = GetGrid();

    wxPGChoiceInfo ci;
    ci.m_choices = (wxPGChoices*) NULL;
    int sel = GetChoiceInfo(&ci);

    if ( ci.m_choices )
    {
        int newSel = sel;

        if ( index < 0 )
            index = ci.m_choices->GetCount();

        if ( index <= sel )
            newSel++;

        ci.m_choices->Insert(label, index, value);

        if ( sel != newSel )
            SetChoiceSelection(newSel, ci);

        if ( this == pg->GetSelection() )
            GetEditorClass()->InsertItem(pg->GetEditorControl(),label,index);

        return index;
    }

    return -1;
}


void wxPGProperty::DeleteChoice( int index )
{
    wxPropertyGrid* pg = GetGrid();

    wxPGChoiceInfo ci;
    ci.m_choices = (wxPGChoices*) NULL;
    int sel = GetChoiceInfo(&ci);

    if ( ci.m_choices )
    {
        int newSel = sel;

        // Adjust current value
        if ( sel == index )
        {
            SetValueToUnspecified();
            newSel = 0;
        }
        else if ( index < sel )
        {
            newSel--;
        }

        ci.m_choices->RemoveAt(index);

        if ( sel != newSel )
            SetChoiceSelection(newSel, ci);

        if ( this == pg->GetSelection() )
            GetEditorClass()->DeleteItem(pg->GetEditorControl(), index);
    }
}

int wxPGProperty::GetChoiceInfo( wxPGChoiceInfo* WXUNUSED(info) )
{
    return -1;
}

wxPGEditorDialogAdapter* wxPGProperty::GetEditorDialog() const
{
    return NULL;
}

bool wxPGProperty::DoSetAttribute( const wxString& WXUNUSED(name), wxVariant& WXUNUSED(value) )
{
    return false;
}

void wxPGProperty::SetAttribute( const wxString& name, wxVariant value )
{
    if ( DoSetAttribute( name, value ) )
    {
        // Support working without grid, when possible
        if ( wxPGGlobalVars->HasExtraStyle( wxPG_EX_WRITEONLY_BUILTIN_ATTRIBUTES ) )
            return;
    }

    m_attributes.Set( name, value );
}

wxVariant wxPGProperty::DoGetAttribute( const wxString& WXUNUSED(name) ) const
{
    return wxVariant();
}


wxVariant wxPGProperty::GetAttribute( const wxString& name ) const
{
    return m_attributes.FindValue(name);
}

wxString wxPGProperty::GetAttribute( const wxString& name, const wxString& defVal )
{
    wxVariant variant = m_attributes.FindValue(name);

    if ( !variant.IsNull() )
        return variant.GetString();

    return defVal;
}

long wxPGProperty::GetAttribute( const wxString& name, long defVal )
{
    wxVariant variant = m_attributes.FindValue(name);

    if ( !variant.IsNull() )
        return variant.GetLong();

    return defVal;
}

wxVariant wxPGProperty::GetAttributesAsList() const
{
    wxVariantList tempList;
    wxVariant v( tempList, wxString::Format(wxT("@%s@attr"),m_name.c_str()) );

    wxPGAttributeStorage::const_iterator it = m_attributes.StartIteration();
    wxVariant variant;

    while ( m_attributes.GetNext(it, variant) )
        v.Append(variant);

    return v;
}

// Slots of utility flags are NULL
const int gs_propFlagToStringSize = 14;

static const wxChar* gs_propFlagToString[gs_propFlagToStringSize] = {
    NULL,
    wxT("DISABLED"),
    wxT("HIDDEN"),
    NULL,
    wxT("NOEDITOR"),
    wxT("COLLAPSED"),
    NULL,
    NULL,
    wxT("DENY_COMMON_VALUES"),
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

wxString wxPGProperty::GetFlagsAsString( FlagType flagsMask ) const
{
    wxString s;
    int relevantFlags = m_flags & flagsMask & wxPG_STRING_STORED_FLAGS;
    FlagType a = 1;

    unsigned int i = 0;
    for ( i=0; i<gs_propFlagToStringSize; i++ )
    {
        if ( relevantFlags & a )
        {
            const wxChar* fs = gs_propFlagToString[i];
            wxASSERT(fs);
            if ( s.length() )
                s << wxT("|");
            s << fs;
        }
        a = a << 1;
    }

    return s;
}

void wxPGProperty::SetFlagsFromString( const wxString& str )
{
    FlagType flags = 0;

    WX_PG_TOKENIZER1_BEGIN(str, wxT('|'))
        unsigned int i;
        for ( i=0; i<gs_propFlagToStringSize; i++ )
        {
            const wxChar* fs = gs_propFlagToString[i];
            if ( fs && str == fs )
            {
                flags |= (1<<i);
                break;
            }
        }
    WX_PG_TOKENIZER1_END()

    m_flags = (m_flags & ~wxPG_STRING_STORED_FLAGS) | flags;
}

wxValidator* wxPGProperty::DoGetValidator() const
{
    return (wxValidator*) NULL;
}

wxPGChoices& wxPGProperty::GetChoices()
{
    wxPGChoiceInfo choiceInfo;
    choiceInfo.m_choices = NULL;
    GetChoiceInfo(&choiceInfo);
    return *choiceInfo.m_choices;
}

const wxPGChoices& wxPGProperty::GetChoices() const
{
    return (const wxPGChoices&) ((wxPGProperty*)this)->GetChoices();
}

unsigned int wxPGProperty::GetChoiceCount() const
{
    const wxPGChoices& choices = GetChoices();
    if ( &choices && choices.IsOk() )
        return choices.GetCount();
    return 0;
}

const wxPGChoiceEntry* wxPGProperty::GetCurrentChoice() const
{
    wxPGChoiceInfo ci;
    ci.m_choices = (wxPGChoices*) NULL;
    int index = ((wxPGProperty*)this)->GetChoiceInfo(&ci);
    if ( index == -1 || !ci.m_choices )
        return NULL;

    return &(*ci.m_choices)[index];
}

bool wxPGProperty::SetChoices( wxPGChoices& choices )
{
    wxPGChoiceInfo ci;
    ci.m_choices = (wxPGChoices*) NULL;

    // Unref existing
    GetChoiceInfo(&ci);
    if ( ci.m_choices )
    {
        ci.m_choices->Assign(choices);

        //if ( m_parent )
        {
            // This may be needed to trigger some initialization
            // (but don't do it if property is somewhat uninitialized)
            wxVariant defVal = GetDefaultValue();
            if ( defVal.IsNull() )
                return false;

            SetValue(defVal);

            return true;
        }
    }
    return false;
}


const wxPGEditor* wxPGProperty::GetEditorClass() const
{
    const wxPGEditor* editor;

    if ( !m_dataExt || !m_dataExt->m_customEditor )
    {
#ifdef __WXPYTHON__
        wxString editorName = GetEditor();
        if ( editorName.length() )
            editor = wxPropertyContainerMethods::GetEditorByName(editorName);
        else
#endif
            editor = DoGetEditorClass();
    }
    else
        editor = m_dataExt->m_customEditor;

    //
    // Maybe override editor if common value specified
    if ( GetDisplayedCommonValueCount() )
    {
        // TextCtrlAndButton -> ComboBoxAndButton
        if ( editor->IsKindOf(CLASSINFO(wxPGTextCtrlAndButtonEditor)) )
            editor = wxPG_EDITOR(ChoiceAndButton);

        // TextCtrl -> ComboBox
        else if ( editor->IsKindOf(CLASSINFO(wxPGTextCtrlEditor)) )
            editor = wxPG_EDITOR(ComboBox);
    }

    return editor;
}


// Privatizes set of choices
void wxPGProperty::SetChoicesExclusive()
{
    wxPGChoiceInfo ci;
    ci.m_choices = (wxPGChoices*) NULL;

    GetChoiceInfo(&ci);
    if ( ci.m_choices )
        ci.m_choices->SetExclusive();
}


bool wxPGProperty::PrepareValueForDialogEditing( wxPropertyGrid* propGrid )
{
    return propGrid->EditorValidate();
}


bool wxPGProperty::RecreateEditor()
{
    wxPropertyGrid* pg = GetGrid();
    wxASSERT(pg);

    wxPGProperty* selected = pg->GetSelection();
    if ( this == selected )
    {
        pg->DoSelectProperty(this, wxPG_SEL_FORCE);
        return true;
    }
    return false;
}


bool wxPGProperty::EnsureDataExt()
{
    if ( !m_dataExt )
    {
        m_dataExt = new wxPGPropertyDataExt();
        return true;
    }
    return false;
}


void wxPGProperty::SetValueImage( wxBitmap& bmp )
{
    EnsureDataExt();

    delete m_dataExt->m_valueBitmap;

    if ( &bmp && bmp.Ok() )
    {
        // Resize the image
        wxSize maxSz = GetGrid()->GetImageSize();
        wxSize imSz(bmp.GetWidth(),bmp.GetHeight());

        if ( imSz.x != maxSz.x || imSz.y != maxSz.y )
        {
            // Create a memory DC
            wxBitmap* bmpNew = new wxBitmap(maxSz.x,maxSz.y,bmp.GetDepth());

            wxMemoryDC dc;
            dc.SelectObject(*bmpNew);

            // Scale
            // FIXME: This is ugly - use image or wait for scaling patch.
            double scaleX = (double)maxSz.x / (double)imSz.x;
            double scaleY = (double)maxSz.y / (double)imSz.y;

            dc.SetUserScale(scaleX,scaleY);

            dc.DrawBitmap( bmp, 0, 0 );

            m_dataExt->m_valueBitmap = bmpNew;
        }
        else
            m_dataExt->m_valueBitmap = new wxBitmap(bmp);

        m_flags |= wxPG_PROP_CUSTOMIMAGE;
    }
    else
    {
        m_dataExt->m_valueBitmap = (wxBitmap*) NULL;
        m_flags &= ~(wxPG_PROP_CUSTOMIMAGE);
    }
}


wxPGProperty* wxPGProperty::GetMainParent() const
{
    const wxPGProperty* curChild = this;
    const wxPGProperty* curParent = m_parent;

    while ( curParent && !curParent->IsCategory() )
    {
        curChild = curParent;
        curParent = curParent->m_parent;
    }

    return (wxPGProperty*) curChild;
}


const wxPGProperty* wxPGProperty::GetLastVisibleSubItem() const
{
    //
    // Returns last visible sub-item, recursively.
    if ( !IsExpanded() || !GetChildCount() )
        return this;

    return Last()->GetLastVisibleSubItem();
}


bool wxPGProperty::IsVisible() const
{
    const wxPGProperty* parent;

    for ( parent = GetParent(); parent != NULL; parent = parent->GetParent() )
    {
        if ( !parent->IsExpanded() )
            return false;
    }

    return true;
}

bool wxPGProperty::UsesAutoUnspecified() const
{
    if ( GetGrid()->GetExtraStyle() & wxPG_EX_AUTO_UNSPECIFIED_VALUES )
        return true;

    return false;
}


wxPropertyGrid* wxPGProperty::GetGridIfDisplayed() const
{
    wxPropertyGridState* state = GetParentState();
    wxPropertyGrid* propGrid = state->GetGrid();
    if ( state == propGrid->GetState() )
        return propGrid;
    return NULL;
}


int wxPGProperty::GetY2( int lh ) const
{
    const wxPGProperty* parent;
    const wxPGProperty* child = this;

    int y = 0;

    for ( parent = GetParent(); parent != NULL; parent = child->GetParent() )
    {
        if ( !parent->IsExpanded() )
            return -1;
        y += parent->GetChildrenHeight(lh, child->GetIndexInParent());
        y += lh;
        child = parent;
    }

    y -= lh;  // need to reduce one level

    return y;
}


int wxPGProperty::GetY() const
{
    return GetY2(GetGrid()->GetRowHeight());
}


wxPGProperty* wxPGPropArgCls::GetPtr( wxPropertyContainerMethods* methods ) const
{
    if ( !m_isName )
    {
        wxASSERT_MSG( m_ptr.property, wxT("invalid property ptr") );
        return m_ptr.property;
    }
    else if ( m_isName == 1 )
        return methods->GetPropertyByNameI(*m_ptr.name);
    else if ( m_isName == 2 )
        return methods->GetPropertyByNameI(m_ptr.rawname);
#ifdef __WXPYTHON__
    else if ( m_isName == 3 )
        return methods->GetPropertyByNameI(*m_ptr.name);
#endif

    wxASSERT( m_isName <= 3 );
    return NULL;
}

// This is used by Insert etc.
void wxPGProperty::AddChild2( wxPGProperty* prop, int index, bool correct_mode )
{
    if ( index < 0 || (size_t)index >= m_children.GetCount() )
    {
        if ( correct_mode ) prop->m_arrIndex = m_children.GetCount();
        m_children.Add( prop );
    }
    else
    {
        m_children.Insert( prop, index );
        if ( correct_mode ) FixIndexesOfChildren( index );
    }

    prop->m_parent = this;
}

// This is used by properties that have fixed sub-properties
void wxPGProperty::AddChild( wxPGProperty* prop )
{
    prop->m_arrIndex = m_children.GetCount();
    m_children.Add( prop );

    int custImgHeight = prop->OnMeasureImage().y;
    if ( custImgHeight < 0 /*|| custImgHeight > 1*/ )
        prop->m_flags |= wxPG_PROP_CUSTOMIMAGE;

    prop->m_parent = this;
}


void wxPGProperty::AdaptListToValue( wxVariant& list, wxVariant* value ) const
{
    wxASSERT_MSG( GetChildCount() && !IsCategory(),
                  wxT("list variants can only be used as intermediate containers for child values") );

    *value = GetValue();

    if ( !list.GetCount() )
        return;

    wxASSERT( GetCount() >= (unsigned int)list.GetCount() );

    wxVariant childValue = list[0];
    unsigned int i;
    unsigned int n = 0;

    for ( i=0; i<GetCount(); i++ )
    {
        const wxPGProperty* child = Item(i);

        if ( childValue.GetName() == child->GetLabel() )
        {
            if ( wxPGIsVariantType(childValue, list) )
            {
                wxVariant cv2(child->GetValue());
                AdaptListToValue(childValue, &cv2);
                childValue = cv2;
            }

            ChildChanged(*value, i, childValue);
            n++;
            if ( n == (unsigned int)list.GetCount() )
                break;
            childValue = list[n];
        }
    }
}


void wxPGProperty::FixIndexesOfChildren( size_t starthere )
{
    size_t i;
    for ( i=starthere;i<GetCount();i++)
        Item(i)->m_arrIndex = i;
}


// Returns (direct) child property with given name (or NULL if not found)
wxPGProperty* wxPGProperty::GetPropertyByName( const wxString& name ) const
{
    size_t i;

    for ( i=0; i<GetCount(); i++ )
    {
        wxPGProperty* p = Item(i);
        if ( p->m_name == name )
            return p;
    }

    // Does it have point, then?
    int pos = name.Find(wxT('.'));
    if ( pos <= 0 )
        return (wxPGProperty*) NULL;

    wxPGProperty* p = GetPropertyByName(name. substr(0,pos));

    if ( !p || !p->GetChildCount() )
        return NULL;

    return p->GetPropertyByName(name.substr(pos+1,name.length()-pos-1));
}

int wxPGProperty::GetChildrenHeight( int lh, int iMax_ ) const
{
    // Returns height of children, recursively, and
    // by taking expanded/collapsed status into account.
    //
    // iMax is used when finding property y-positions.
    //
    unsigned int i = 0;
    int h = 0;

    if ( iMax_ == -1 )
        iMax_ = GetChildCount();

    unsigned int iMax = iMax_;

    wxASSERT( iMax <= GetChildCount() );

    while ( i < iMax )
    {
        wxPGProperty* pwc = (wxPGProperty*) Item(i);

        if ( !pwc->HasFlag(wxPG_PROP_HIDDEN) )
        {
            if ( !pwc->IsExpanded() ||
                 pwc->GetChildCount() == 0 )
                h += lh;
            else
                h += pwc->GetChildrenHeight(lh) + lh;
        }

        i++;
    }

    return h;
}

wxPGProperty* wxPGProperty::GetItemAtY( unsigned int y, unsigned int lh, unsigned int* nextItemY ) const
{
    wxASSERT( nextItemY );

    // Linear search at the moment
    //
    // nextItemY = y of next visible property, final value will be written back.
    wxPGProperty* result = NULL;
    wxPGProperty* current = NULL;
    unsigned int iy = *nextItemY;
    unsigned int i = 0;
    unsigned int iMax = GetCount();

    while ( i < iMax )
    {
        wxPGProperty* pwc = Item(i);

        if ( !pwc->HasFlag(wxPG_PROP_HIDDEN) )
        {
            // Found?
            if ( y < iy )
            {
                result = current;
                break;
            }

            iy += lh;

            if ( pwc->IsExpanded() &&
                 pwc->GetChildCount() > 0 )
            {
                result = (wxPGProperty*) pwc->GetItemAtY( y, lh, &iy );
                if ( result )
                    break;
            }

            current = pwc;
        }

        i++;
    }

    // Found?
    if ( !result && y < iy )
        result = current;

    *nextItemY = iy;

    /*
    if ( current )
        wxLogDebug(wxT("%s::GetItemAtY(%i) -> %s"),this->GetLabel().c_str(),y,current->GetLabel().c_str());
    else
        wxLogDebug(wxT("%s::GetItemAtY(%i) -> NULL"),this->GetLabel().c_str(),y);
    */

    return (wxPGProperty*) result;
}

void wxPGProperty::Empty()
{
    size_t i;
    if ( !HasFlag(wxPG_PROP_CHILDREN_ARE_COPIES) )
    {
        for ( i=0; i<GetCount(); i++ )
        {
            wxPGProperty* p = (wxPGProperty*) Item(i);
            delete p;
        }
    }

    m_children.Empty();
}

void wxPGProperty::ChildChanged( wxVariant& WXUNUSED(thisValue),
                                 int WXUNUSED(childIndex),
                                 wxVariant& WXUNUSED(childValue) ) const
{
}

// -----------------------------------------------------------------------
// wxPGRootProperty
// -----------------------------------------------------------------------

WX_PG_IMPLEMENT_PROPERTY_CLASS_PLAIN(wxPGRootProperty,none,TextCtrl)
IMPLEMENT_DYNAMIC_CLASS(wxPGRootProperty, wxPGProperty)


wxPGRootProperty::wxPGRootProperty()
    : wxPGProperty()
{
#ifdef __WXDEBUG__
    m_name = wxT("<root>");
#endif
    SetParentalType(0);
    m_depth = 0;
}


wxPGRootProperty::~wxPGRootProperty()
{
}


// -----------------------------------------------------------------------
// wxPropertyCategory
// -----------------------------------------------------------------------

WX_PG_IMPLEMENT_PROPERTY_CLASS_PLAIN(wxPropertyCategory,none,TextCtrl)
IMPLEMENT_DYNAMIC_CLASS(wxPropertyCategory, wxPGProperty)

void wxPropertyCategory::Init()
{
    // don't set colour - prepareadditem method should do this
    SetParentalType(wxPG_PROP_CATEGORY);
    m_capFgColIndex = 1;
    m_textExtent = -1;
}

wxPropertyCategory::wxPropertyCategory()
    : wxPGProperty()
{
    Init();
}


wxPropertyCategory::wxPropertyCategory( const wxString &label, const wxString& name )
    : wxPGProperty(label,name)
{
    Init();
}


wxPropertyCategory::~wxPropertyCategory()
{
}


wxString wxPropertyCategory::GetValueAsString( int ) const
{
    return wxEmptyString;
}

int wxPropertyCategory::GetTextExtent( const wxWindow* wnd, const wxFont& font ) const
{
    if ( m_textExtent > 0 )
        return m_textExtent;
    int x = 0, y = 0;
	((wxWindow*)wnd)->GetTextExtent( m_label, &x, &y, 0, 0, &font );
    return x;
}

void wxPropertyCategory::CalculateTextExtent( wxWindow* wnd, const wxFont& font )
{
    int x = 0, y = 0;
	wnd->GetTextExtent( m_label, &x, &y, 0, 0, &font );
    m_textExtent = x;
}

// -----------------------------------------------------------------------
// wxPGCellRenderer
// -----------------------------------------------------------------------

wxSize wxPGCellRenderer::GetImageSize( const wxPGProperty* WXUNUSED(property),
                                       int WXUNUSED(column),
                                       int WXUNUSED(item) ) const
{
     return wxSize(0, 0);
}

void wxPGCellRenderer::DrawText( wxDC& dc, const wxRect& rect,
                                 int xOffset, const wxString& text ) const
{
    if ( xOffset )
        xOffset += wxCC_CUSTOM_IMAGE_MARGIN1 + wxCC_CUSTOM_IMAGE_MARGIN2;
    dc.DrawText( text,
                 rect.x+xOffset+wxPG_XBEFORETEXT,
                 rect.y+((rect.height-dc.GetCharHeight())/2) );
}

void wxPGCellRenderer::DrawEditorValue( wxDC& dc, const wxRect& rect,
                                        int xOffset, const wxString& text,
                                        wxPGProperty* property,
                                        const wxPGEditor* editor ) const
{
    if ( xOffset )
        xOffset += wxCC_CUSTOM_IMAGE_MARGIN1 + wxCC_CUSTOM_IMAGE_MARGIN2;

    int yOffset = ((rect.height-dc.GetCharHeight())/2);

    if ( editor )
    {
        wxRect rect2(rect); 
        rect2.x += xOffset;
        rect2.y += yOffset;
        rect2.height -= yOffset;
        editor->DrawValue( dc, rect2, property, text );
    }
    else
    {
        dc.DrawText( text,
                     rect.x+xOffset+wxPG_XBEFORETEXT,
                     rect.y+yOffset );
    }
}

void wxPGCellRenderer::DrawCaptionSelectionRect( wxDC& dc, int x, int y, int w, int h ) const
{
    wxRect focusRect(x,y+((h-dc.GetCharHeight())/2),w,h);
    wxPGDrawFocusRect(dc,focusRect);
}

int wxPGCellRenderer::PreDrawCell( wxDC& dc, const wxRect& rect, const wxPGCell& cell, int flags ) const
{
    int imageOffset = 0;

    if ( !(flags & Selected) )
    {
        // Draw using wxPGCell information, if available
        wxColour fgCol = cell.GetFgCol();
        if ( fgCol.Ok() )
            dc.SetTextForeground(fgCol);

        wxColour bgCol = cell.GetBgCol();
        if ( bgCol.Ok() )
        {
            dc.SetPen(bgCol);
            dc.SetBrush(bgCol);
            dc.DrawRectangle(rect);
        }
    }

    const wxBitmap& bmp = cell.GetBitmap();
    if ( bmp.Ok() )
    {
        dc.DrawBitmap( bmp,
                       rect.x + wxPG_CONTROL_MARGIN + wxCC_CUSTOM_IMAGE_MARGIN1,
                       rect.y + wxPG_CUSTOM_IMAGE_SPACINGY,
                       true );
        imageOffset = bmp.GetWidth();
    }

    return imageOffset;
}

// -----------------------------------------------------------------------
// wxPGDefaultRenderer
// -----------------------------------------------------------------------

void wxPGDefaultRenderer::Render( wxDC& dc, const wxRect& rect,
                                  const wxPropertyGrid* propertyGrid, wxPGProperty* property,
                                  int column, int item, int flags ) const
{
    bool isUnspecified = property->IsValueUnspecified();

    if ( column == 1 && item == -1 )
    {
        int cmnVal = property->GetCommonValue();
        if ( cmnVal >= 0 )
        {
            // Common Value
            if ( !isUnspecified )
                DrawText( dc, rect, 0, propertyGrid->GetCommonValueLabel(cmnVal) );
            return;
        }
    }

    const wxPGEditor* editor = NULL;
    const wxPGCell* cell = property->GetCell(column);

    wxString text;
    int imageOffset = 0;

    // Use choice cell?
    if ( column == 1 && (flags & Control) )
    {
        const wxPGCell* ccell = property->GetCurrentChoice();
        if ( ccell &&
#if wxCHECK_VERSION(2,8,0)
             ( ccell->GetBitmap().IsOk() || ccell->GetFgCol().IsOk() || ccell->GetBgCol().IsOk() )
#else
             ( ccell->GetBitmap().Ok() || ccell->GetFgCol().Ok() || ccell->GetBgCol().Ok() )
#endif
           )
            cell = ccell;
    }

    if ( cell )
    {
        imageOffset = PreDrawCell( dc, rect, *cell, flags );
        text = cell->GetText();
        if ( text == wxT("@!") )
        {
            if ( column == 0 )
                text = property->GetLabel();
            else if ( column == 1 )
                text = property->GetValueString();
            else
                text = wxEmptyString;
        }
    }
    else if ( column == 0 )
    {
        // Caption
        DrawText( dc, rect, 0, property->GetLabel() );
    }
    else if ( column == 1 && !isUnspecified )
    {
        editor = property->GetColumnEditor(column);

        // Regular property value

        wxSize imageSize = propertyGrid->GetImageSize(property, item);

        wxPGPaintData paintdata;
        paintdata.m_parent = propertyGrid;
        paintdata.m_choiceItem = item;

        if ( imageSize.x > 0 )
        {
            wxRect imageRect(rect.x + wxPG_CONTROL_MARGIN + wxCC_CUSTOM_IMAGE_MARGIN1,
                             rect.y+wxPG_CUSTOM_IMAGE_SPACINGY,
                             wxPG_CUSTOM_IMAGE_WIDTH,
                             rect.height-(wxPG_CUSTOM_IMAGE_SPACINGY*2));

            /*if ( imageSize.x == wxPG_FULL_CUSTOM_PAINT_WIDTH )
            {
                imageRect.width = m_width - imageRect.x;
            }*/

            dc.SetPen( wxPen(propertyGrid->GetCellTextColour(), 1, wxSOLID) );

            paintdata.m_drawnWidth = imageSize.x;
            paintdata.m_drawnHeight = imageSize.y;

            if ( !isUnspecified )
            {
                property->OnCustomPaint( dc, imageRect, paintdata );
            }
            else
            {
                dc.SetBrush(*wxWHITE_BRUSH);
                dc.DrawRectangle(imageRect);
            }

            imageOffset = paintdata.m_drawnWidth;
        }

        text = property->GetValueString();
    }

    DrawEditorValue( dc, rect, imageOffset, text, property, editor );

    // active caption gets nice dotted rectangle
    if ( property->IsCategory() /*&& column == 0*/ )
    {
        if ( flags & Selected )
        {
            if ( imageOffset > 0 )
                imageOffset += wxCC_CUSTOM_IMAGE_MARGIN2 + 4;

            DrawCaptionSelectionRect( dc,
                                      rect.x+wxPG_XBEFORETEXT-wxPG_CAPRECTXMARGIN+imageOffset,
                                      rect.y-wxPG_CAPRECTYMARGIN+1,
                                      ((wxPropertyCategory*)property)->GetTextExtent(propertyGrid,
                                                                                     propertyGrid->GetCaptionFont())
                                      +(wxPG_CAPRECTXMARGIN*2),
                                      propertyGrid->GetFontHeight()+(wxPG_CAPRECTYMARGIN*2) );
        }
    }
}

wxSize wxPGDefaultRenderer::GetImageSize( const wxPGProperty* property,
                                          int column,
                                          int item ) const
{
    if ( property && column == 1 )
    {
        if ( item == -1 )
        {
            wxBitmap* bmp = property->GetValueImage();

            if ( bmp && bmp->Ok() )
                return wxSize(bmp->GetWidth(),bmp->GetHeight());
        }
    }
    return wxSize(0,0);
}

// -----------------------------------------------------------------------
// wxPGCell
// -----------------------------------------------------------------------

wxPGCell::wxPGCell()
{
}

wxPGCell::wxPGCell( const wxString& text,
                    const wxBitmap& bitmap,
                    const wxColour& fgCol,
                    const wxColour& bgCol )
    : m_bitmap(bitmap), m_fgCol(fgCol), m_bgCol(bgCol)
{
#ifndef __WXPYTHON__
    if ( &text != ((wxString*)NULL) )
#else
    if ( (&text != ((wxString*)NULL)) && text != wxT("_LABEL_AS_NAME") )
#endif
    {
        wxASSERT_MSG( m_text != wxT("@!"), wxT("\"@!\" is not an allowed as a cell text.") );
        m_text = text;
    }
    else
    {
        m_text = wxT("@!");
    }
}

// -----------------------------------------------------------------------
// wxPGBrush
// -----------------------------------------------------------------------

//
// This class is a wxBrush derivative used in the background colour
// brush cache. It adds wxPG-type colour-in-long to the class.
// JMS: Yes I know wxBrush doesn't actually hold the value (refcounted
//   object does), but this is simpler implementation and equally
//   effective.
//

class wxPGBrush : public wxBrush
{
public:
    wxPGBrush( const wxColour& colour );
    wxPGBrush();
    virtual ~wxPGBrush() { }
    void SetColour2( const wxColour& colour );
    inline long GetColourAsLong() const { return m_colAsLong; }
private:
    long    m_colAsLong;
};


void wxPGBrush::SetColour2( const wxColour& colour )
{
    wxBrush::SetColour(colour);
    m_colAsLong = wxPG_COLOUR(colour.Red(),colour.Green(),colour.Blue());
}


wxPGBrush::wxPGBrush() : wxBrush()
{
    m_colAsLong = 0;
}


wxPGBrush::wxPGBrush( const wxColour& colour ) : wxBrush(colour)
{
    m_colAsLong = wxPG_COLOUR(colour.Red(),colour.Green(),colour.Blue());
}


// -----------------------------------------------------------------------
// wxPGColour
// -----------------------------------------------------------------------

//
// Same as wxPGBrush, but for wxColour instead.
//

class wxPGColour : public wxColour
{
public:
    wxPGColour( const wxColour& colour );
    wxPGColour();
    virtual ~wxPGColour() { }
    void SetColour2( const wxColour& colour );
    inline long GetColourAsLong() const { return m_colAsLong; }
private:
    long    m_colAsLong;
};


void wxPGColour::SetColour2( const wxColour& colour )
{
    *this = colour;
    m_colAsLong = wxPG_COLOUR(colour.Red(),colour.Green(),colour.Blue());
}


wxPGColour::wxPGColour() : wxColour()
{
    m_colAsLong = 0;
}


wxPGColour::wxPGColour( const wxColour& colour ) : wxColour(colour)
{
    m_colAsLong = wxPG_COLOUR(colour.Red(),colour.Green(),colour.Blue());
}


// -----------------------------------------------------------------------
// wxPGTLWHandler
//   Intercepts Close-events sent to wxPropertyGrid's top-level parent,
//   and tries to commit property value.
// -----------------------------------------------------------------------

class wxPGTLWHandler : public wxEvtHandler
{
public:

    wxPGTLWHandler( wxPropertyGrid* pg )
        : wxEvtHandler()
    {
        m_pg = pg;
    }

protected:

    void OnClose( wxCloseEvent& event )
    {
        // ClearSelection forces value validation/commit.
        if ( event.CanVeto() && !m_pg->ClearSelection() )
        {
            event.Veto();
            return;
        }

        event.Skip();
    }

private:
    wxPropertyGrid*     m_pg;

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(wxPGTLWHandler, wxEvtHandler)
    EVT_CLOSE(wxPGTLWHandler::OnClose)
END_EVENT_TABLE()

// -----------------------------------------------------------------------
// wxPGCanvas
// -----------------------------------------------------------------------

//
// wxPGCanvas acts as a graphics sub-window of the
// wxScrolledWindow that wxPropertyGrid is.
//
class wxPGCanvas : public wxPanel
{
public:
    wxPGCanvas() : wxPanel()
    {
        /*m_draggedSplitter = -1;
        m_dragStatus = 0;
        m_dragOffset = 0;*/
    }
    virtual ~wxPGCanvas() { }

protected:
    void OnMouseMove( wxMouseEvent &event )
    {
        wxPropertyGrid* pg = wxStaticCast(GetParent(), wxPropertyGrid);
        pg->OnMouseMove( event );
    }

    void OnMouseClick( wxMouseEvent &event )
    {
        wxPropertyGrid* pg = wxStaticCast(GetParent(), wxPropertyGrid);
        pg->OnMouseClick( event );
    }

    void OnMouseUp( wxMouseEvent &event )
    {
        wxPropertyGrid* pg = wxStaticCast(GetParent(), wxPropertyGrid);
        pg->OnMouseUp( event );
    }

    void OnMouseRightClick( wxMouseEvent &event )
    {
        wxPropertyGrid* pg = wxStaticCast(GetParent(), wxPropertyGrid);
        pg->OnMouseRightClick( event );
    }

    void OnMouseDoubleClick( wxMouseEvent &event )
    {
        wxPropertyGrid* pg = wxStaticCast(GetParent(), wxPropertyGrid);
        pg->OnMouseDoubleClick( event );
    }

    void OnKey( wxKeyEvent& event )
    {
        wxPropertyGrid* pg = wxStaticCast(GetParent(), wxPropertyGrid);
        pg->OnKey( event );
    }

    void OnKeyUp( wxKeyEvent& event )
    {
        wxPropertyGrid* pg = wxStaticCast(GetParent(), wxPropertyGrid);
        pg->OnKeyUp( event );
    }

    void OnNavigationKey( wxNavigationKeyEvent& event )
    {
        wxPropertyGrid* pg = wxStaticCast(GetParent(), wxPropertyGrid);
        pg->OnNavigationKey( event );
    }

    void OnPaint( wxPaintEvent& event );

private:
    DECLARE_EVENT_TABLE()
};


BEGIN_EVENT_TABLE(wxPGCanvas, wxPanel)
    EVT_MOTION(wxPGCanvas::OnMouseMove)
    EVT_PAINT(wxPGCanvas::OnPaint)
    EVT_LEFT_DOWN(wxPGCanvas::OnMouseClick)
    EVT_LEFT_UP(wxPGCanvas::OnMouseUp)
    EVT_RIGHT_UP(wxPGCanvas::OnMouseRightClick)
    EVT_LEFT_DCLICK(wxPGCanvas::OnMouseDoubleClick)
    EVT_KEY_DOWN(wxPGCanvas::OnKey)
    EVT_KEY_UP(wxPGCanvas::OnKeyUp)
    EVT_CHAR(wxPGCanvas::OnKey)
    EVT_NAVIGATION_KEY(wxPGCanvas::OnNavigationKey)
END_EVENT_TABLE()


void wxPGCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
    wxPropertyGrid* pg = wxStaticCast(GetParent(), wxPropertyGrid);
    wxASSERT( pg->IsKindOf(CLASSINFO(wxPropertyGrid)) );

    wxPaintDC dc(this);

    // Don't paint after destruction has begun
    if ( !(pg->GetInternalFlags() & wxPG_FL_INITIALIZED) )
        return;

#if __PAINT_DEBUGGING__
    wxLogDebug( wxT("wxPGCanvas::OnPaint()") );
#endif

    // Update everything inside the box
    wxRect r = GetUpdateRegion().GetBox();

    // Repaint this rectangle
    pg->DrawItems( dc, r.y, r.y + r.height, &r );

    // We assume that the size set when grid is shown
    // is what is desired.
    pg->SetInternalFlag(wxPG_FL_GOOD_SIZE_SET);
}

// -----------------------------------------------------------------------
// wxPropertyGrid
// -----------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxPropertyGrid, wxScrolledWindow)

BEGIN_EVENT_TABLE(wxPropertyGrid, wxScrolledWindow)
  EVT_IDLE(wxPropertyGrid::OnIdle)
  EVT_MOTION(wxPropertyGrid::OnMouseMoveBottom)
  EVT_PAINT(wxPropertyGrid::OnPaint)
  EVT_SIZE(wxPropertyGrid::OnResize)
  EVT_ENTER_WINDOW(wxPropertyGrid::OnMouseEntry)
  EVT_LEAVE_WINDOW(wxPropertyGrid::OnMouseEntry)
  EVT_MOUSE_CAPTURE_CHANGED(wxPropertyGrid::OnCaptureChange)
  EVT_SCROLLWIN(wxPropertyGrid::OnScrollEvent)
  //EVT_TEXT(wxPG_SUBID1,wxPropertyGrid::OnCustomEditorEvent)
  //EVT_COMBOBOX(wxPG_SUBID1,wxPropertyGrid::OnCustomEditorEvent)
  //EVT_BUTTON(wxPG_SUBID2,wxPropertyGrid::OnCustomEditorEvent)
  EVT_CHILD_FOCUS(wxPropertyGrid::OnChildFocusEvent)
  EVT_SET_FOCUS(wxPropertyGrid::OnFocusEvent)
  EVT_KILL_FOCUS(wxPropertyGrid::OnFocusEvent)
  EVT_TEXT_ENTER(wxPG_SUBID1,wxPropertyGrid::OnCustomEditorEvent)
  EVT_SYS_COLOUR_CHANGED(wxPropertyGrid::OnSysColourChanged)
END_EVENT_TABLE()


// -----------------------------------------------------------------------

wxPropertyGrid::wxPropertyGrid()
    : wxScrolledWindow()
{
    Init1();
}

// -----------------------------------------------------------------------

wxPropertyGrid::wxPropertyGrid( wxWindow *parent,
                                wxWindowID id,
                                const wxPoint& pos,
                                const wxSize& size,
                                long style,
                                const wxChar* name )
    : wxScrolledWindow()
{
    Init1();
    Create(parent,id,pos,size,style,name);
}

// -----------------------------------------------------------------------

bool wxPropertyGrid::Create( wxWindow *parent,
                             wxWindowID id,
                             const wxPoint& pos,
                             const wxSize& size,
                             long style,
                             const wxChar* name )
{

    if ( !(style&wxBORDER_MASK) )
        style |= wxSIMPLE_BORDER;

    style |= wxVSCROLL;

#ifdef __WXMSW__
    // This prevents crash under Win2K, but still
    // enables keyboard navigation
    if ( style & wxTAB_TRAVERSAL )
    {
        style &= ~(wxTAB_TRAVERSAL);
        style |= wxWANTS_CHARS;
    }
#else
    if ( style & wxTAB_TRAVERSAL )
        style |= wxWANTS_CHARS;
#endif

    wxScrolledWindow::Create(parent,id,pos,size,style,name);

    Init2();

    return true;
}

// -----------------------------------------------------------------------

//
// Initialize values to defaults
//
void wxPropertyGrid::Init1()
{
    WX_PG_GLOBALS_LOCKER()

#if !wxPG_USE_WXMODULE
    if ( !wxPGGlobalVars )
        wxPGGlobalVars = new wxPGGlobalVarsClass();
#endif

    // Register editor classes, if necessary.
    if ( wxPGGlobalVars->m_mapEditorClasses.empty() )
        RegisterDefaultEditors();

    m_iFlags = 0;
    m_pState = (wxPropertyGridState*) NULL;
    m_wndPrimary = m_wndSecondary = (wxWindow*) NULL;
    m_selected = (wxPGProperty*) NULL;
    m_selColumn = -1;
    m_propHover = (wxPGProperty*) NULL;
    m_eventObject = this;
    m_curFocused = (wxWindow*) NULL;
    m_tlwHandler = NULL;
    m_propValChanging = 0;
    m_dragStatus = 0;
    m_mouseSide = 16;
    m_editorFocused = 0;
    m_coloursCustomized = 0;
    m_frozen = 0;

    m_canvas = NULL;

#if wxPG_DOUBLE_BUFFER
    m_doubleBuffer = (wxBitmap*) NULL;
#endif

    m_windowsToDelete = NULL;

#ifndef wxPG_ICON_WIDTH
	m_expandbmp = NULL;
	m_collbmp = NULL;
	m_iconWidth = 11;
	m_iconHeight = 11;
#else
    m_iconWidth = wxPG_ICON_WIDTH;
#endif

    m_prevVY = -1;

    m_gutterWidth = wxPG_GUTTER_MIN;
    m_subgroup_extramargin = 10;

    m_lineHeight = 0;

    m_width = m_height = 0;

#if !wxPG_HEAVY_GFX
    m_splitterpen.SetColour( *wxBLACK );
    m_splitterpen.SetWidth( 4 );

    m_splitterprevdrawnx = -1;
#endif

    SetButtonShortcut(0);

    m_keyComboConsumed = 0;

    m_ignoredEvents = 0;

    m_commonValues.push_back(new wxPGCommonValue(_("Unspecified"), wxPGGlobalVars->m_defaultRenderer) );
    m_cvUnspecified = 0;
}

// -----------------------------------------------------------------------

//
// Initialize after parent etc. set
//
void wxPropertyGrid::Init2()
{
    wxASSERT( !(m_iFlags & wxPG_FL_INITIALIZED ) );

#ifdef __WXMAC__
   // Smaller controls on Mac
   SetWindowVariant(wxWINDOW_VARIANT_SMALL);
#endif 

    // Now create state, if one didn't exist already
    // (wxPropertyGridManager might have created it for us).
    if ( !m_pState )
    {
        m_pState = CreateState();
        m_pState->m_pPropGrid = this;
        m_iFlags |= wxPG_FL_CREATEDSTATE;
    }

    if ( !(m_windowStyle & wxPG_SPLITTER_AUTO_CENTER) )
        m_iFlags |= wxPG_FL_DONT_CENTER_SPLITTER;

    if ( m_windowStyle & wxPG_HIDE_CATEGORIES )
    {
        m_pState->InitNonCatMode();

        m_pState->m_properties = m_pState->m_abcArray;
    }

    GetClientSize(&m_width,&m_height);

#if !wxPG_HEAVY_GFX
    m_splitterpen.SetColour( *wxBLACK );
    m_splitterpen.SetWidth( 4 );
#endif

#ifndef wxPG_ICON_WIDTH
    // create two bitmap nodes for drawing
	m_expandbmp = new wxBitmap(expand_xpm);
	m_collbmp = new wxBitmap(collapse_xpm);

	// calculate average font height for bitmap centering

	m_iconWidth = m_expandbmp->GetWidth();
	m_iconHeight = m_expandbmp->GetHeight();
#endif

    m_curcursor = wxCURSOR_ARROW;
    m_cursorSizeWE = new wxCursor( wxCURSOR_SIZEWE );

	// adjust bitmap icon y position so they are centered
    m_vspacing = wxPG_DEFAULT_VSPACING;

    if ( !m_font.Ok() )
    {
        wxFont useFont = wxScrolledWindow::GetFont();
        wxScrolledWindow::SetOwnFont( useFont );
    }
    else
        // This should be otherwise called by SetOwnFont
	    CalculateFontAndBitmapStuff( wxPG_DEFAULT_VSPACING );

    // Add base brush item
    m_arrBgBrushes.Add((void*)new wxPGBrush());

    // Add base colour items
    m_arrFgCols.Add((void*)new wxPGColour());
    m_arrFgCols.Add((void*)new wxPGColour());

    RegainColours();

    // This helps with flicker
    SetBackgroundStyle( wxBG_STYLE_CUSTOM );

    // Hook the TLW
    wxPGTLWHandler* handler = new wxPGTLWHandler(this);
    m_tlp = ::wxGetTopLevelParent(this);
    m_tlwHandler = handler;
    m_tlp->PushEventHandler(handler);

	// set virtual size to this window size
    wxSize wndsize = GetSize();
	SetVirtualSize(wndsize.GetWidth(), wndsize.GetWidth());

    m_timeCreated = ::wxGetLocalTimeMillis();

    m_canvas = new wxPGCanvas();
    m_canvas->Create(this, 1, wxPoint(0, 0), GetClientSize(),
                     (GetWindowStyle() & wxTAB_TRAVERSAL) | wxWANTS_CHARS);
    m_canvas->SetBackgroundStyle( wxBG_STYLE_CUSTOM );

    m_iFlags |= wxPG_FL_INITIALIZED;

    m_ncWidth = wndsize.GetWidth();

    // Need to call OnResize handler or size given in constructor/Create
    // will never work.
    wxSizeEvent sizeEvent(wndsize,0);
    OnResize(sizeEvent);
}

// -----------------------------------------------------------------------

wxPropertyGrid::~wxPropertyGrid()
{
    size_t i;

    DoSelectProperty(NULL);

    // This should do prevent things from going too badly wrong
    m_iFlags &= ~(wxPG_FL_INITIALIZED);

    END_MOUSE_CAPTURE

    wxPGTLWHandler* handler = (wxPGTLWHandler*) m_tlwHandler;
    m_tlp->RemoveEventHandler(handler);
    delete handler;

#ifdef __WXDEBUG__
    if ( IsEditorsValueModified() )
        ::wxMessageBox(wxT("Most recent change in property editor was lost!!!\n\n(if you don't want this to happen, close your frames and dialogs using Close(false).)"),
                       wxT("wxPropertyGrid Debug Warning") );
#endif

#if wxPG_DOUBLE_BUFFER
    if ( m_doubleBuffer )
        delete m_doubleBuffer;
#endif

    delete m_windowsToDelete;

    //m_selected = (wxPGProperty*) NULL;

    if ( m_iFlags & wxPG_FL_CREATEDSTATE )
        delete m_pState;

    delete m_cursorSizeWE;

#ifndef wxPG_ICON_WIDTH
	delete m_expandbmp;
	delete m_collbmp;
#endif

    // Delete cached text colours.
    for ( i=0; i<m_arrFgCols.GetCount(); i++ )
    {
        delete (wxPGColour*)m_arrFgCols.Item(i);
    }

    // Delete cached brushes.
    for ( i=0; i<m_arrBgBrushes.GetCount(); i++ )
    {
        delete (wxPGBrush*)m_arrBgBrushes.Item(i);
    }

    // Delete common value records
    for ( i=0; i<m_commonValues.size(); i++ )
    {
        delete GetCommonValue(i);
    }
}

// -----------------------------------------------------------------------

bool wxPropertyGrid::Destroy()
{
    END_MOUSE_CAPTURE
    return wxScrolledWindow::Destroy();
}

// -----------------------------------------------------------------------

wxPropertyGridState* wxPropertyGrid::CreateState() const
{
    return new wxPropertyGridState();
}

// -----------------------------------------------------------------------
// wxPropertyGrid overridden wxWindow methods
// -----------------------------------------------------------------------

void wxPropertyGrid::SetWindowStyleFlag( long style )
{
    long old_style = m_windowStyle;

    if ( m_iFlags & wxPG_FL_INITIALIZED )
    {
        wxASSERT( m_pState );

        if ( !(style & wxPG_HIDE_CATEGORIES) && (old_style & wxPG_HIDE_CATEGORIES) )
        {
        // Enable categories
            EnableCategories( true );
        }
        else if ( (style & wxPG_HIDE_CATEGORIES) && !(old_style & wxPG_HIDE_CATEGORIES) )
        {
        // Disable categories
            EnableCategories( false );
        }
        if ( !(old_style & wxPG_AUTO_SORT) && (style & wxPG_AUTO_SORT) )
        {
            //
            // Autosort enabled
            //
            if ( !m_frozen )
                PrepareAfterItemsAdded();
            else
                m_pState->m_itemsAdded = 1;
        }
    #if wxPG_SUPPORT_TOOLTIPS
        if ( !(old_style & wxPG_TOOLTIPS) && (style & wxPG_TOOLTIPS) )
        {
            //
            // Tooltips enabled
            //
            /*wxToolTip* tooltip = new wxToolTip ( wxEmptyString );
            SetToolTip ( tooltip );
            tooltip->SetDelay ( wxPG_TOOLTIP_DELAY );*/
        }
        else if ( (old_style & wxPG_TOOLTIPS) && !(style & wxPG_TOOLTIPS) )
        {
            //
            // Tooltips disabled
            //
            m_canvas->SetToolTip( (wxToolTip*) NULL );
        }
    #endif
    }

    wxScrolledWindow::SetWindowStyleFlag ( style );

    if ( m_iFlags & wxPG_FL_INITIALIZED )
    {
        if ( (old_style & wxPG_HIDE_MARGIN) != (style & wxPG_HIDE_MARGIN) )
        {
            CalculateFontAndBitmapStuff( m_vspacing );
            RedrawAllVisible();
        }
    }
}

// -----------------------------------------------------------------------

void wxPropertyGrid::Freeze()
{
    m_frozen++;
    wxScrolledWindow::Freeze();
}

// -----------------------------------------------------------------------

void wxPropertyGrid::Thaw()
{
    m_frozen--;
    wxScrolledWindow::Thaw();
    RecalculateVirtualSize();
#if wxPG_REFRESH_CONTROLS_AFTER_REPAINT
    m_canvas->Refresh();
#endif

    // Force property re-selection
    if ( m_selected )
        DoSelectProperty(m_selected, wxPG_SEL_FORCE);
}

// -----------------------------------------------------------------------

void wxPropertyGrid::SetExtraStyle( long exStyle )
{
    if ( exStyle & wxPG_EX_NATIVE_DOUBLE_BUFFERING )
    {
#if defined(__WXMSW__)

        /*
        // Don't use WS_EX_COMPOSITED just now.
        HWND hWnd;

        if ( m_iFlags & wxPG_FL_IN_MANAGER )
            hWnd = (HWND)GetParent()->GetHWND();
        else
            hWnd = (HWND)GetHWND();

        ::SetWindowLong( hWnd, GWL_EXSTYLE,
                         ::GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_COMPOSITED );
        */

//#elif defined(__WXGTK20__)
#endif
        // Only apply wxPG_EX_NATIVE_DOUBLE_BUFFERING if the window
        // truly was double-buffered.
        if ( !wxPGIsWindowBuffered(this) )
        {
            exStyle &= ~(wxPG_EX_NATIVE_DOUBLE_BUFFERING);
        }
        else
        {
        #if wxPG_DOUBLE_BUFFER
            delete m_doubleBuffer;
            m_doubleBuffer = NULL;
        #endif
        }
    }

    wxScrolledWindow::SetExtraStyle( exStyle );

    if ( exStyle & wxPG_EX_INIT_NOCAT )
        m_pState->InitNonCatMode();

    if ( exStyle & wxPG_EX_HELP_AS_TOOLTIPS )
        m_windowStyle |= wxPG_TOOLTIPS;

    // Set global style
    wxPGGlobalVars->m_extraStyle = exStyle;
}

// -----------------------------------------------------------------------

void wxPropertyGrid::EnableCommonValues( bool enable )
{
    if ( enable ) m_iFlags |= wxPG_FL_USE_COMMON_VALUES;
    else m_iFlags &= ~wxPG_FL_USE_COMMON_VALUES;
}

// -----------------------------------------------------------------------

// returns the best acceptable minimal size
wxSize wxPropertyGrid::DoGetBestSize() const
{
    int hei = 15;
    if ( m_lineHeight > hei )
        hei = m_lineHeight;
    wxSize sz = wxSize( 60, hei+40 );

    CacheBestSize(sz);
    return sz;
}

// -----------------------------------------------------------------------
// wxPropertyGrid Font and Colour Methods
// -----------------------------------------------------------------------

void wxPropertyGrid::CalculateFontAndBitmapStuff( int vspacing )
{
	int x = 0, y = 0;

    m_captionFont = wxScrolledWindow::GetFont();

	GetTextExtent(wxT("jG"), &x, &y, 0, 0, &m_captionFont);
    m_subgroup_extramargin = x + (x/2);
	m_fontHeight = y;

#if wxPG_USE_RENDERER_NATIVE
    m_iconWidth = wxPG_ICON_WIDTH;
#elif wxPG_ICON_WIDTH
    // scale icon
    m_iconWidth = (m_fontHeight * wxPG_ICON_WIDTH) / 13;
    if ( m_iconWidth < 5 ) m_iconWidth = 5;
    else if ( !(m_iconWidth & 0x01) ) m_iconWidth++; // must be odd

#endif

    m_gutterWidth = m_iconWidth / wxPG_GUTTER_DIV;
    if ( m_gutterWidth < wxPG_GUTTER_MIN )
        m_gutterWidth = wxPG_GUTTER_MIN;

    int vdiv = 6;
    if ( vspacing <= 1 ) vdiv = 12;
    else if ( vspacing >= 3 ) vdiv = 3;

    m_spacingy = m_fontHeight / vdiv;
    if ( m_spacingy < wxPG_YSPACING_MIN )
        m_spacingy = wxPG_YSPACING_MIN;

    m_marginWidth = 0;
    if ( !(m_windowStyle & wxPG_HIDE_MARGIN) )
        m_marginWidth = m_gutterWidth*2 + m_iconWidth;

    m_captionFont.SetWeight(wxBOLD);
	GetTextExtent(wxT("jG"), &x, &y, 0, 0, &m_captionFont);

    m_lineHeight = m_fontHeight+(2*m_spacingy)+1;
    m_visPropArray.SetCount((m_height/m_lineHeight)+10);

    // button spacing
    m_buttonSpacingY = (m_lineHeight - m_iconHeight) / 2;
    if ( m_buttonSpacingY < 0 ) m_buttonSpacingY = 0;

    if ( m_iFlags & wxPG_FL_INITIALIZED )
        RecalculateVirtualSize();

    InvalidateBestSize();
}

// -----------------------------------------------------------------------

void wxPropertyGrid::OnSysColourChanged( wxSysColourChangedEvent &WXUNUSED(event) )
{
    RegainColours();
    m_canvas->Refresh();
}

// -----------------------------------------------------------------------

static wxColour wxPGAdjustColour(const wxColour& src, int ra,
                                 int ga = 1000, int ba = 1000,
                                 bool forceDifferent = false)
{
    if ( ga >= 1000 )
        ga = ra;
    if ( ba >= 1000 )
        ba = ra;

    // Recursion guard (allow 2 max)
    static int isinside = 0;
    isinside++;
    wxCHECK_MSG( isinside < 3,
                 *wxBLACK,
                 wxT("wxPGAdjustColour should not be recursively called more than once") );

    wxColour dst;

    int r = src.Red();
    int g = src.Green();
    int b = src.Blue();
    int r2 = r + ra;
    if ( r2>255 ) r2 = 255;
    else if ( r2<0) r2 = 0;
    int g2 = g + ga;
    if ( g2>255 ) g2 = 255;
    else if ( g2<0) g2 = 0;
    int b2 = b + ba;
    if ( b2>255 ) b2 = 255;
    else if ( b2<0) b2 = 0;

    // Make sure they are somewhat different
    if ( forceDifferent && (abs((r+g+b)-(r2+g2+b2)) < abs(ra/2)) )
        dst = wxPGAdjustColour(src,-(ra*2));
    else
        dst = wxColour(r2,g2,b2);

    // Recursion guard (allow 2 max)
    isinside--;

    return dst;
}


static int wxPGGetColAvg( const wxColour& col )
{
    return (col.Red() + col.Green() + col.Blue()) / 3;
}


void wxPropertyGrid::RegainColours()
{
    wxColour def_bgcol = wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW );

    if ( !(m_coloursCustomized & 0x0002) )
    {
        wxColour col = wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE );

        // Make sure colour is dark enough
    #ifdef __WXGTK__
        int colDec = wxPGGetColAvg(col) - 230;
    #else
        int colDec = wxPGGetColAvg(col) - 200;
    #endif
        if ( colDec > 0 )
            m_colCapBack = wxPGAdjustColour(col,-colDec);
        else
            m_colCapBack = col;
    }

    if ( !(m_coloursCustomized & 0x0001) )
        m_colMargin = m_colCapBack;

    if ( !(m_coloursCustomized & 0x0004) )
    {
    #ifdef __WXGTK__
        int colDec = -90;
    #else
        int colDec = -72;
    #endif
        wxColour capForeCol = wxPGAdjustColour(m_colCapBack,colDec,5000,5000,true);
        m_colCapFore = capForeCol;

        // Set the cached colour as well.
        ((wxPGColour*)m_arrFgCols.Item(1))->SetColour2(capForeCol);
    }

    if ( !(m_coloursCustomized & 0x0008) )
    {
        wxColour bgCol = wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW );
        m_colPropBack = bgCol;

        // Set the cached brush as well.
        ((wxPGBrush*)m_arrBgBrushes.Item(0))->SetColour2(bgCol);
    }

    if ( !(m_coloursCustomized & 0x0010) )
    {
        wxColour fgCol = wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT );
        m_colPropFore = fgCol;

        // Set the cached colour as well.
        ((wxPGColour*)m_arrFgCols.Item(0))->SetColour2(fgCol);
    }

    if ( !(m_coloursCustomized & 0x0020) )
        m_colSelBack = wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT );

    if ( !(m_coloursCustomized & 0x0040) )
        m_colSelFore = wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT );

    if ( !(m_coloursCustomized & 0x0080) )
        m_colLine = m_colCapBack;

    if ( !(m_coloursCustomized & 0x0100) )
        m_colDisPropFore = m_colCapFore;
}

// -----------------------------------------------------------------------

void wxPropertyGrid::ResetColours()
{
    m_coloursCustomized = 0;

    RegainColours();

    m_canvas->Refresh();
}

// -----------------------------------------------------------------------

bool wxPropertyGrid::SetFont( const wxFont& font )
{

#if __INTENSE_DEBUGGING__
    wxLogDebug( wxT("wxPropertyGrid::SetFont") );
#endif

    // Must disable active editor.
    if ( m_selected )
    {
        bool selRes = ClearSelection();
        wxPG_CHECK_MSG_DBG( selRes,
                            false,
                            wxT("failed to deselect a property (editor probably had invalid value)") );
    }

    // TODO: Following code is disabled with wxMac because
    //   it is reported to fail. I (JMS) cannot debug it
    //   personally right now.
#if !defined(__WXMAC__)
    bool res = wxScrolledWindow::SetFont( font );
    if ( res )
    {
        CalculateFontAndBitmapStuff( m_vspacing );

        if ( m_pState )
        {
            // Recalculate caption text extents.
            // TODO: This should also be done to other pages of manager
            //   (so add wxPropertyGridManager::SetFont), but since font
            //   is usually set before categories are added, this is
            //   quite low priority.
            size_t i;

            for ( i=0;i<m_pState->m_regularArray.GetCount();i++ )
            {
                wxPGProperty* p = m_pState->m_regularArray.Item(i);

                if ( p->IsCategory() )
                    ((wxPropertyCategory*)p)->CalculateTextExtent(this,m_captionFont);
            }
        }

        Refresh();
    }

    return res;
#else
    // ** wxMAC Only **
    // TODO: Remove after SetFont crash fixed.
    if ( m_iFlags & wxPG_FL_INITIALIZED )
    {
        wxLogDebug(wxT("WARNING: propGrid.cpp: wxPropertyGrid::SetFont has been disabled on wxMac since there has been crash reported in it. If you are willing to debug the cause, replace line '#if !defined(__WXMAC__)' with line '#if 1' in wxPropertyGrid::SetFont."));
    }
    return false;
#endif
}

// -----------------------------------------------------------------------

void wxPropertyGrid::SetLineColour( const wxColour& col )
{
    m_colLine = col;
    m_coloursCustomized |= 0x80;
    Refresh();
}

// -----------------------------------------------------------------------

void wxPropertyGrid::SetMarginColour( const wxColour& col )
{
    m_colMargin = col;
    m_coloursCustomized |= 0x01;
    Refresh();
}

// -----------------------------------------------------------------------

void wxPropertyGrid::SetCellBackgroundColour( const wxColour& col )
{
    m_colPropBack = col;
    m_coloursCustomized |= 0x08;

    // Set the cached brush as well.
    ((wxPGBrush*)m_arrBgBrushes.Item(0))->SetColour2(col);

    Refresh();
}

// -----------------------------------------------------------------------

void wxPropertyGrid::SetCellTextColour( const wxColour& col )
{
    m_colPropFore = col;
    m_coloursCustomized |= 0x10;

    // Set the cached colour as well.
    ((wxPGColour*)m_arrFgCols.Item(0))->SetColour2(col);

    Refresh();
}

// -----------------------------------------------------------------------

void wxPropertyGrid::SetCellDisabledTextColour( const wxColour& col )
{
    m_colDisPropFore = col;
    m_coloursCustomized |= 0x100;
    Refresh();
}

// -----------------------------------------------------------------------

void wxPropertyGrid::SetSelectionBackground( const wxColour& col )
{
    m_colSelBack = col;
    m_coloursCustomized |= 0x20;
    Refresh();
}

// -----------------------------------------------------------------------

void wxPropertyGrid::SetSelectionForeground( const wxColour& col )
{
    m_colSelFore = col;
    m_coloursCustomized |= 0x40;
    Refresh();
}

// -----------------------------------------------------------------------

void wxPropertyGrid::SetCaptionBackgroundColour( const wxColour& col )
{
    m_colCapBack = col;
    m_coloursCustomized |= 0x02;
    Refresh();
}

// -----------------------------------------------------------------------

void wxPropertyGrid::SetCaptionForegroundColour( const wxColour& col )
{
    m_colCapFore = col;
    m_coloursCustomized |= 0x04;

    // Set the cached colour as well.
    ((wxPGColour*)m_arrFgCols.Item(1))->SetColour2(col);

    Refresh();
}

// -----------------------------------------------------------------------

void wxPropertyGrid::SetBackgroundColourIndex( wxPGProperty* p, int index )
{
    unsigned char ind = index;

    p->m_bgColIndex = ind;

    unsigned int i;
    for ( i=0; i<p->GetChildCount(); i++ )
        SetBackgroundColourIndex(p->Item(i),index);
}

// -----------------------------------------------------------------------

void wxPropertyGrid::SetPropertyBackgroundColour( wxPGPropArg id, const wxColour& colour )
{
    wxPG_PROP_ARG_CALL_PROLOG()

    size_t i;
    int colInd = -1;

    long colAsLong = wxPG_COLOUR(colour.Red(),colour.Green(),colour.Blue());

    // As it is most likely that the previous colour is used, start comparison
    // from the end.
    for ( i=(m_arrBgBrushes.GetCount()-1); i>0; i-- )
    {
        if ( ((wxPGBrush*)m_arrBgBrushes.Item(i))->GetColourAsLong() == colAsLong )
        {
            colInd = i;
            break;
        }
    }

    if ( colInd < 0 )
    {
        colInd = m_arrBgBrushes.GetCount();
        wxCHECK_RET( colInd < 256, wxT("wxPropertyGrid: Warning - Only 255 different property background colours allowed.") );
        m_arrBgBrushes.Add( (void*)new wxPGBrush(colour) );
    }

    // Set indexes
    SetBackgroundColourIndex(p,colInd);

    // If this was on a visible grid, then draw it.
    DrawItemAndChildren(p);
}

// -----------------------------------------------------------------------

wxColour wxPropertyGrid::GetPropertyBackgroundColour( wxPGPropArg id ) const
{
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(wxColour())

    return ((wxPGBrush*)m_arrBgBrushes.Item(p->m_bgColIndex))->GetColour();
}

// -----------------------------------------------------------------------

void wxPropertyGrid::SetTextColourIndex( wxPGProperty* p, int index, int flags )
{
    unsigned char ind = index;

    p->m_fgColIndex = ind;

    if ( p->GetChildCount() && (flags & wxPG_RECURSE) )
    {
        unsigned int i;
        for ( i=0; i<p->GetChildCount(); i++ )
            SetTextColourIndex( p->Item(i), index, flags );
    }
}

// -----------------------------------------------------------------------

int wxPropertyGrid::CacheColour( const wxColour& colour )
{
    unsigned int i;
    int colInd = -1;

    long colAsLong = wxPG_COLOUR(colour.Red(),colour.Green(),colour.Blue());

    // As it is most likely that the previous colour is used, start comparison
    // from the end.
    for ( i=(m_arrFgCols.GetCount()-1); i>0; i-- )
    {
        if ( ((wxPGColour*)m_arrFgCols.Item(i))->GetColourAsLong() == colAsLong )
        {
            colInd = i;
            break;
        }
    }

    if ( colInd < 0 )
    {
        colInd = m_arrFgCols.GetCount();
        wxCHECK_MSG( colInd < 256, 0, wxT("wxPropertyGrid: Warning - Only 255 different property foreground colours allowed.") );
        m_arrFgCols.Add( (void*)new wxPGColour(colour) );
    }

    return colInd;
}

// -----------------------------------------------------------------------

void wxPropertyGrid::SetPropertyTextColour( wxPGPropArg id, const wxColour& colour )
{
    wxPG_PROP_ARG_CALL_PROLOG()

    // Set indexes
    SetTextColourIndex(p, CacheColour(colour), wxPG_RECURSE);

    // If this was on a visible grid, then draw it.
    DrawItemAndChildren(p);
}

// -----------------------------------------------------------------------

void wxPropertyGrid::SetCaptionTextColour( wxPGPropArg id, const wxColour& colour )
{
    wxPG_PROP_ARG_CALL_PROLOG()

    wxCHECK_RET( p->IsCategory(),
                 wxT("Only call SetCaptionTextColour for caption properties") );

    // Set indexes
    wxPropertyCategory* cat = (wxPropertyCategory*) p;
    cat->SetTextColIndex(CacheColour(colour));

    // If this was on a visible grid, then draw it.
    DrawItemAndChildren(p);
}

// -----------------------------------------------------------------------

wxColour wxPropertyGrid::GetPropertyTextColour( wxPGPropArg id ) const
{
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(wxColour())

    return wxColour(*((wxPGColour*)m_arrFgCols.Item(p->m_fgColIndex)));
}

// -----------------------------------------------------------------------

void wxPropertyGrid::SetPropertyColourToDefault( wxPGPropArg id )
{
    wxPG_PROP_ARG_CALL_PROLOG()

    SetBackgroundColourIndex( p, 0 );
    SetTextColourIndex( p, 0, wxPG_RECURSE );

    if ( p->IsCategory() )
    {
        wxPropertyCategory* cat = (wxPropertyCategory*) p;
        cat->SetTextColIndex(1);
    }
}

// -----------------------------------------------------------------------
// wxPropertyGrid property adding and removal
// -----------------------------------------------------------------------

void wxPropertyContainerMethods::RefreshGrid( wxPropertyGridState* state )
{
    if ( !state )
        state = m_pState;

    wxPropertyGrid* grid = state->GetGrid();
    if ( grid->GetState() == state && !grid->IsFrozen() )
    {
        grid->Refresh();
    }
}

// -----------------------------------------------------------------------

wxPGProperty* wxPropertyContainerMethods::Append( wxPGProperty* property )
{
    return m_pState->DoAppend(property);
}

// -----------------------------------------------------------------------

wxPGProperty* wxPropertyContainerMethods::AppendIn( wxPGPropArg id, wxPGProperty* newproperty )
{
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(wxNullProperty)
    wxPGProperty* pwc = (wxPGProperty*) p;
    wxPGProperty* retp = m_pState->DoInsert(pwc, pwc->GetChildCount(), newproperty);
    return retp;
}

// -----------------------------------------------------------------------

wxPGProperty* wxPropertyContainerMethods::Insert( wxPGPropArg id, wxPGProperty* property )
{
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(wxNullProperty)
    wxPGProperty* retp = m_pState->DoInsert(p->GetParent(), p->GetArrIndex(), property);
    RefreshGrid();
    return retp;
}

// -----------------------------------------------------------------------

wxPGProperty* wxPropertyContainerMethods::Insert( wxPGPropArg id, int index, wxPGProperty* newproperty )
{
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(wxNullProperty)
    wxPGProperty* retp = m_pState->DoInsert((wxPGProperty*)p,index,newproperty);
    RefreshGrid();
    return retp;
}

// -----------------------------------------------------------------------

void wxPropertyContainerMethods::DeleteProperty( wxPGPropArg id )
{
    wxPG_PROP_ARG_CALL_PROLOG()

    wxPropertyGridState* state = p->GetParentState();
    wxPropertyGrid* grid = state->GetGrid();

    if ( grid->GetState() == state )
    {
        bool selRes = grid->DoSelectProperty(NULL, wxPG_SEL_DELETING);
        wxPG_CHECK_RET_DBG( selRes,
                            wxT("failed to deselect a property (editor probably had invalid value)") );
    }

    state->DoDelete( p );

    RefreshGrid(state);
}

// -----------------------------------------------------------------------

wxPGId wxPropertyContainerMethods::ReplaceProperty( wxPGPropArg id, wxPGProperty* property )
{
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(wxNullProperty)

    wxPGProperty* replaced = p;
    wxCHECK_MSG( replaced && property,
                 wxNullProperty,
                 wxT("NULL property") );
    wxCHECK_MSG( !replaced->IsCategory(),
                 wxNullProperty,
                 wxT("cannot replace this type of property") );
    wxCHECK_MSG( !m_pState->IsInNonCatMode(),
                 wxNullProperty,
                 wxT("cannot replace properties in alphabetic mode") );

    // Get address to the slot
    wxPGProperty* parent = replaced->GetParent();
    int ind = replaced->GetIndexInParent();

    wxPropertyGridState* state = replaced->GetParentState();
    DeleteProperty(replaced); // Must use generic Delete
    state->DoInsert(parent,ind,property);

    return wxPGIdGen(property);
}

// -----------------------------------------------------------------------

void wxPropertyGrid::PrepareAfterItemsAdded()
{
    if ( !m_pState || !m_pState->m_itemsAdded ) return;

#if __INTENSE_DEBUGGING__
    wxLogDebug(wxT("PrepareAfterItemsAdded( in thread 0x%lX )"),
        (unsigned long)wxThread::GetCurrentId());
#endif

    m_pState->m_itemsAdded = 0;

    wxPGProperty* p = GetLastItem(wxPG_ITERATE_VISIBLE);
    if ( p )
        m_pState->m_bottomY = p->GetY() + GetRowHeight();
    else
        m_pState->m_bottomY = 0;

    if ( m_windowStyle & wxPG_AUTO_SORT )
        Sort();

    RecalculateVirtualSize();
}

// -----------------------------------------------------------------------
// wxPropertyContainerMethods property operations
// -----------------------------------------------------------------------

bool wxPropertyContainerMethods::ClearSelection()
{
    wxPropertyGridState* state = m_pState;
    wxPropertyGrid* pg = state->GetGrid();
    if ( pg->GetState() == state )
        return pg->ClearSelection();
    else
        state->SetSelection(NULL);
    return true;
}

// -----------------------------------------------------------------------

void wxPropertyContainerMethods::LimitPropertyEditing( wxPGPropArg id, bool limit )
{
    wxPG_PROP_ARG_CALL_PROLOG()

    m_pState->DoLimitPropertyEditing(p, limit);
    RefreshProperty(p);
}

// -----------------------------------------------------------------------

bool wxPropertyContainerMethods::EnableProperty( wxPGPropArg id, bool enable )
{
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(false)

    wxPropertyGridState* state = p->GetParentState();
    wxPropertyGrid* grid = state->GetGrid();

    if ( enable )
    {
        if ( !(p->m_flags & wxPG_PROP_DISABLED) )
            return false;

        // If active, Set active Editor.
        if ( grid->GetState() == state && p == grid->GetSelection() )
            grid->DoSelectProperty( p, wxPG_SEL_FORCE );
    }
    else
    {
        if ( p->m_flags & wxPG_PROP_DISABLED )
            return false;

        // If active, Disable as active Editor.
        if ( grid->GetState() == state && p == grid->GetSelection() )
            grid->DoSelectProperty( p, wxPG_SEL_FORCE );
    }

    state->DoEnableProperty(p, enable);

    RefreshProperty( p );

    return true;
}

// -----------------------------------------------------------------------

#if wxPG_COMPATIBILITY_1_2_0
bool wxPropertyContainerMethods::ExpandAll( bool doExpand )
{
    wxPropertyGridState* state = m_pState;

    if ( !state->GetRoot()->GetChildCount() )
        return true;

    wxPropertyGrid* pg = state->GetGrid();

    if ( GetSelection() && GetSelection() != state->GetRoot() &&
         !doExpand )
    {
        if ( !pg->ClearSelection() )
            return false;
    }

    wxPGVIterator it;

    for ( it = GetVIterator( wxPG_ITERATE_ALL ); !it.AtEnd(); it.Next() )
    {
        wxPGProperty* p = (wxPGProperty*) it.GetProperty();
        if ( p->GetChildCount() )
            p->SetExpanded( doExpand );
    }

    RefreshGrid();

    return true;
}
#endif

// -----------------------------------------------------------------------

void wxPropertyContainerMethods::SetPropertyUnspecified( wxPGPropArg id )
{
    wxPG_PROP_ARG_CALL_PROLOG()
    wxPropertyGrid* propGrid = p->GetGridIfDisplayed();
    if ( propGrid )
        propGrid->DoSetPropertyUnspecified(p);
    else
        p->GetParentState()->DoSetPropertyUnspecified(p);
}

// -----------------------------------------------------------------------
// wxPropertyGrid property value setting and getting
// -----------------------------------------------------------------------

void wxPGGetFailed( const wxPGProperty* p, const wxChar* typestr )
{
    wxPGTypeOperationFailed(p,typestr,wxT("Get"));
}

// -----------------------------------------------------------------------

void wxPGTypeOperationFailed( const wxPGProperty* p, const wxChar* typestr,
    const wxChar* op )
{
    wxASSERT( p != NULL );
    wxLogError( _("Type operation \"%s\" failed: Property labeled \"%s\" is of type \"%s\", NOT \"%s\"."),
        op,p->GetLabel().c_str(),p->GetValue().GetType().c_str(),typestr );
}

// -----------------------------------------------------------------------

void wxPropertyContainerMethods::SetPropVal( wxPGPropArg id, wxVariant& value )
{
    wxPG_PROP_ARG_CALL_PROLOG()

    if ( p )
    {
        p->SetValue(value);
        wxPropertyGrid* propGrid = p->GetGridIfDisplayed();
        if ( propGrid )
            propGrid->DrawItemAndValueRelated( p );

    }
}

// -----------------------------------------------------------------------

void wxPropertyContainerMethods::SetPropertyValueString( wxPGPropArg id, const wxString& value )
{
    wxPG_PROP_ARG_CALL_PROLOG()

    if ( m_pState->DoSetPropertyValueString(p,value) )
    {
        wxPropertyGrid* propGrid = p->GetGridIfDisplayed();
        if ( propGrid )
            propGrid->DrawItemAndValueRelated( p );
    }
}

// -----------------------------------------------------------------------

void wxPropertyGrid::DoSetPropertyUnspecified( wxPGProperty* p )
{
    m_pState->DoSetPropertyUnspecified(p);
    DrawItemAndChildren(p);

    wxPGProperty* parent = p->GetParent();
    while ( (parent->GetFlags() & wxPG_PROP_PARENTAL_FLAGS) == wxPG_PROP_MISC_PARENT )
    {
        DrawItem(parent);
        parent = parent->GetParent();
    }
}

// -----------------------------------------------------------------------
// wxPropertyGrid property operations
// -----------------------------------------------------------------------

void wxPropertyGrid::DoSetPropertyName( wxPGProperty* p, const wxString& newname )
{
    wxCHECK_RET( p, wxT("invalid property id") );

#if __INTENSE_DEBUGGING__
    wxLogDebug( wxT("wxPropertyGrid::SetPropertyName( %s -> %s )"),
        p->GetName().c_str(), newname.c_str() );
#endif

    if ( p->GetBaseName().Len() ) m_pState->m_dictName.erase( wxPGNameConv(p->GetBaseName()) );
    if ( newname.Len() ) m_pState->m_dictName[newname] = (void*) p;

    p->DoSetName(newname);
}

// -----------------------------------------------------------------------

bool wxPropertyGrid::EnsureVisible( wxPGPropArg id )
{
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(false)

    Update();

    bool changed = false;

    // Is it inside collapsed section?
    if ( !p->IsVisible() )
    {
        // expand parents
        wxPGProperty* parent = p->GetParent();
        wxPGProperty* grandparent = parent->GetParent();

        if ( grandparent && grandparent != m_pState->m_properties )
            Expand( grandparent );

        Expand( parent );
        changed = true;
    }

    // Need to scroll?
    int vx, vy;
    GetViewStart(&vx,&vy);
    vy*=wxPG_PIXELS_PER_UNIT;

    int y = p->GetY();

    if ( y < vy )
    {
        Scroll(vx, y/wxPG_PIXELS_PER_UNIT );
        m_iFlags |= wxPG_FL_SCROLLED;
        changed = true;
    }
    else if ( (y+m_lineHeight) > (vy+m_height) )
    {
        Scroll(vx, (y-m_height+(m_lineHeight*2))/wxPG_PIXELS_PER_UNIT );
        m_iFlags |= wxPG_FL_SCROLLED;
        changed = true;
    }

    if ( changed )
        DrawItems( p, p );

    return changed;
}

// -----------------------------------------------------------------------
// wxPropertyGrid helper methods called by properties
// -----------------------------------------------------------------------

// Control font changer helper.
void wxPropertyGrid::SetCurControlBoldFont()
{
    wxASSERT( m_wndPrimary );
    m_wndPrimary->SetFont( m_captionFont );
}

// -----------------------------------------------------------------------

wxPoint wxPropertyGrid::GetGoodEditorDialogPosition( wxPGProperty* p,
                                                     const wxSize& sz )
{
#if wxPG_SMALL_SCREEN
    // On small-screen devices, always show dialogs with default position and size.
    return wxDefaultPosition;
#else
    int splitterX = GetSplitterPosition();
    int x = splitterX;
    int y = p->GetY();

    wxCHECK_MSG( y >= 0, wxPoint(-1,-1), wxT("invalid y?") );

    ImprovedClientToScreen( &x, &y );

    int sw = wxSystemSettings::GetMetric( ::wxSYS_SCREEN_X );
    int sh = wxSystemSettings::GetMetric( ::wxSYS_SCREEN_Y );

    int new_x;
    int new_y;

    if ( x > (sw/2) )
        // left
        new_x = x + (m_width-splitterX) - sz.x;
    else
        // right
        new_x = x;

    if ( y > (sh/2) )
        // above
        new_y = y - sz.y;
    else
        // below
        new_y = y + m_lineHeight;

    return wxPoint(new_x,new_y);
#endif
}

// -----------------------------------------------------------------------

wxString& wxPropertyGrid::ExpandEscapeSequences( wxString& dst_str, wxString& src_str )
{
    if ( src_str.length() == 0 )
    {
        dst_str = src_str;
        return src_str;
    }

    bool prev_is_slash = false;

    wxString::const_iterator i = src_str.begin();

    dst_str.clear();

    for ( ; i != src_str.end(); i++ )
    {
        wxUniChar a = wxPGGetIterChar(src_str, i);

        if ( a != wxT('\\') )
        {
            if ( !prev_is_slash )
            {
                dst_str << a;
            }
            else
            {
                if ( a == wxT('n') )
                {
            #ifdef __WXMSW__
                    dst_str << wxT('\n');
                    //dst_str << wxT('\10');
            #else
                    dst_str << wxT('\n');
                    //dst_str << 10;
            #endif
                }
                else if ( a == wxT('t') )
                    dst_str << wxT('\t');
                else
                    dst_str << a;
            }
            prev_is_slash = false;
        }
        else
        {
            if ( prev_is_slash )
            {
                dst_str << wxT('\\');
                prev_is_slash = false;
            }
            else
            {
                prev_is_slash = true;
            }
        }
    }
    return dst_str;
}

// -----------------------------------------------------------------------

wxString& wxPropertyGrid::CreateEscapeSequences( wxString& dst_str, wxString& src_str )
{
    if ( src_str.length() == 0 )
    {
        dst_str = src_str;
        return src_str;
    }

    wxString::const_iterator i = src_str.begin();
    wxUniChar prev_a = wxT('\0');

    dst_str.clear();

    for ( ; i != src_str.end(); i++ )
    {
        wxChar a = wxPGGetIterChar(src_str, i);

        if ( a >= wxT(' ') )
        {
            // This surely is not something that requires an escape sequence.
            dst_str << a;
        }
        else
        {
            // This might need...
            if ( a == wxT('\r')  )
            {
                // DOS style line end.
                // Already taken care below
                //dst_str = wxT("\\n");
                //src++;
            }
            else if ( a == wxT('\n') )
                // UNIX style line end.
                dst_str << wxT("\\n");
            else if ( a == wxT('\t') )
                // Tab.
                dst_str << wxT('\t');
            else
            {
                //wxLogDebug(wxT("WARNING: Could not create escape sequence for character #%i"),(int)a);
                dst_str << a;
            }
        }

        prev_a = a;
    }
    return dst_str;
}

// -----------------------------------------------------------------------
// Item iteration macros
//   NB: Only needed for alphabetic/categoric mode switching.
// -----------------------------------------------------------------------

#define II_INVALID_I    0x00FFFFFF

#define ITEM_ITERATION_VARIABLES \
    wxPGProperty* parent; \
    unsigned int i; \
    unsigned int iMax;

#define ITEM_ITERATION_INIT_FROM_THE_TOP \
    parent = m_properties; \
    i = 0;

#define ITEM_ITERATION_INIT(startparent, startindex, state) \
    parent = startparent; \
    i = (unsigned int)startindex; \
    if ( parent == (wxPGProperty*) NULL ) \
    { \
        parent = state->m_properties; \
        i = 0; \
    }

#define ITEM_ITERATION_LOOP_BEGIN \
    do \
    { \
        iMax = parent->GetCount(); \
        while ( i < iMax ) \
        {  \
            wxPGProperty* p = parent->Item(i);

#define ITEM_ITERATION_LOOP_END \
            if ( p->GetChildCount() ) \
            { \
                i = 0; \
                parent = (wxPGProperty*)p; \
                iMax = parent->GetCount(); \
            } \
            else \
                i++; \
        } \
        i = parent->m_arrIndex + 1; \
        parent = parent->m_parent; \
    } \
    while ( parent != NULL );


// -----------------------------------------------------------------------

wxPGProperty* wxPropertyGrid::DoGetItemAtY( int y ) const
{
    // Outside?
    if ( y >= (int)m_pState->m_bottomY || y < 0 )
        return (wxPGProperty*) NULL;

    unsigned int a = 0;
    return m_pState->m_properties->GetItemAtY(y, m_lineHeight, &a);
}

// -----------------------------------------------------------------------

wxPropertyGridHitTestResult wxPropertyGrid::HitTest( const wxPoint& pt ) const
{
    wxPoint pt2;
    GetViewStart(&pt2.x,&pt2.y);
    pt2.x *= wxPG_PIXELS_PER_UNIT;
    pt2.y *= wxPG_PIXELS_PER_UNIT;
    pt2.x += pt.x;
    pt2.y += pt.y;

    return m_pState->HitTest(pt2);
}

// -----------------------------------------------------------------------
// wxPropertyGrid graphics related methods
// -----------------------------------------------------------------------

void wxPropertyGrid::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
    wxPaintDC dc(this);

    // Update everything inside the box
    wxRect r = GetUpdateRegion().GetBox();

    dc.SetPen(wxPG_SLACK_BACKROUND);
    dc.SetBrush(wxPG_SLACK_BACKROUND);
    dc.DrawRectangle(r);
}

// -----------------------------------------------------------------------

void wxPropertyGrid::DrawExpanderButton( wxDC& dc, const wxRect& rect,
                                         wxPGProperty* property ) const
{
    // Prepare rectangle to be used
    wxRect r(rect);
    r.x += m_gutterWidth; r.y += m_buttonSpacingY;
    r.width = m_iconWidth; r.height = m_iconHeight;

#if (wxPG_USE_RENDERER_NATIVE)
    //
#elif wxPG_ICON_WIDTH
    // Drawing expand/collapse button manually
    dc.SetPen(m_colPropFore);
    if ( property->IsCategory() )
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
    else
        dc.SetBrush(m_colPropBack);

    dc.DrawRectangle( r );
    int _y = r.y+(m_iconWidth/2);
    dc.DrawLine(r.x+2,_y,r.x+m_iconWidth-2,_y);
#else
    wxBitmap* bmp;
#endif

    if ( property->IsExpanded() )
    {
    // wxRenderer functions are non-mutating in nature, so it
    // should be safe to cast "const wxPropertyGrid*" to "wxWindow*".
    // Hopefully this does not cause problems.
    #if (wxPG_USE_RENDERER_NATIVE)
        wxRendererNative::Get().DrawTreeItemButton(
                (wxWindow*)this,
                dc,
                r,
                wxCONTROL_EXPANDED
            );
    #elif wxPG_ICON_WIDTH
        //
    #else
        bmp = m_collbmp;
    #endif

    }
    else
    {
    #if (wxPG_USE_RENDERER_NATIVE)
        wxRendererNative::Get().DrawTreeItemButton(
                (wxWindow*)this,
                dc,
                r,
                0
            );
    #elif wxPG_ICON_WIDTH
        int _x = r.x+(m_iconWidth/2);
        dc.DrawLine(_x,r.y+2,_x,r.y+m_iconWidth-2);
    #else
        bmp = m_expandbmp;
    #endif
    }

#if (wxPG_USE_RENDERER_NATIVE)
    //
#elif wxPG_ICON_WIDTH
    //
#else
    dc.DrawBitmap( *bmp, r.x, r.y, true );
#endif
}

// -----------------------------------------------------------------------

//
// This is the one called by OnPaint event handler and others.
// topy and bottomy are already unscrolled (ie. physical)
//
void wxPropertyGrid::DrawItems( wxDC& dc,
                                unsigned int topy,
                                unsigned int bottomy,
                                const wxRect* clipRect )
{
    if ( m_frozen || m_height < 1 || bottomy < topy || !m_pState ) return;

    wxRect tempClipRect;
    if ( !clipRect )
    {
        tempClipRect = wxRect(0,topy,m_pState->m_width,bottomy);
        clipRect = &tempClipRect;
    }

#if __PAINT_DEBUGGING__
    wxLogDebug(wxT("wxPropertyGrid::DrawItems ( %i -> %i, clipRect = 0x%X )"),topy,bottomy,
        (unsigned int)clipRect);
#endif

    // items added check
    if ( m_pState->m_itemsAdded ) PrepareAfterItemsAdded();

    int paintFinishY = 0;

    if ( m_pState->m_properties->GetCount() > 0 )
    {
        paintFinishY = DoDrawItems( dc, NULL, NULL, clipRect );
    }

    // Clear area beyond m_bottomy?
    if ( paintFinishY < (clipRect->y+clipRect->height) )
    {
        wxColour& bgc = wxPG_SLACK_BACKROUND;
        dc.SetPen( wxPen(bgc) );
        dc.SetBrush( wxBrush(bgc) );
        dc.DrawRectangle( 0, paintFinishY, m_width, (clipRect->y+clipRect->height) );
    }
}

// -----------------------------------------------------------------------

int wxPropertyGrid::DoDrawItems( wxDC& dcMain,
                                 const wxPGProperty* firstItem,
                                 const wxPGProperty* lastItem,
                                 const wxRect* clipRect ) const
{
    // TODO: This should somehow be eliminated.
    wxRect tempClipRect;
    if ( !clipRect )
    {
        wxASSERT(firstItem);
        wxASSERT(lastItem);
        tempClipRect = GetPropertyRect(firstItem, lastItem);
        clipRect = &tempClipRect;
    }

    if ( !firstItem )
        firstItem = DoGetItemAtY(clipRect->y);

    if ( !lastItem )
    {
        lastItem = DoGetItemAtY(clipRect->y+clipRect->height-1);
        if ( !lastItem )
            lastItem = GetLastItem( wxPG_ITERATE_VISIBLE );
    }

    if ( m_frozen || m_height < 1 )
        return clipRect->y;

    wxCHECK_MSG( !m_pState->m_itemsAdded, clipRect->y, wxT("no items added") );
    wxCHECK_MSG( firstItem != NULL, clipRect->y, wxT("invalid first item") );
    wxASSERT( m_pState->m_properties->GetCount() );

    int lh = m_lineHeight;

    int firstItemTopY;
    int lastItemBottomY;

    firstItemTopY = clipRect->y;
    lastItemBottomY = clipRect->y + clipRect->height;

    // Align y coordinates to item boundaries
    firstItemTopY -= firstItemTopY % lh;
    lastItemBottomY += lh - (lastItemBottomY % lh);
    lastItemBottomY -= 1;

    // Entire range outside scrolled, visible area?
    if ( firstItemTopY >= (int)m_pState->m_bottomY || lastItemBottomY <= 0 )
        return clipRect->y;

    wxCHECK_MSG( firstItemTopY < lastItemBottomY, clipRect->y, wxT("invalid y values") );

    wxDC* dcPtr;

#if wxPG_DOUBLE_BUFFER
    wxMemoryDC* bufferDC = NULL;
    const wxRect* blitClipRect = NULL;

    if ( !(GetExtraStyle() & wxPG_EX_NATIVE_DOUBLE_BUFFERING) )
    {
        if ( !m_doubleBuffer )
            return clipRect->y;

        // Must fit to double-buffer
    #ifdef __WXDEBUG__
        /*if ( (lastItemBottomY - firstItemTopY) > m_doubleBuffer->GetHeight() )
        {
            wxString msg;
            msg.Printf( wxT("wxPropertyGrid: DOUBLE-BUFFER TOO SMALL ( drawn %i vs db height %i vs client_height %i)!"),
                (int)(lastItemBottomY - firstItemTopY),
                (int)(m_doubleBuffer->GetHeight()),
                (int)m_height );
            wxLogError(msg);
            wxLogDebug(msg);
        }*/
    #endif

        bufferDC = new wxMemoryDC();
        bufferDC->SelectObject( *m_doubleBuffer );
        dcPtr = bufferDC;

        blitClipRect = clipRect;
    }
    else
#endif
    {
        dcPtr = &dcMain;
    }

    wxDC& dc = *dcPtr;

#if __PAINT_DEBUGGING__
    wxLogDebug(wxT("  -> DoDrawItems ( \"%s\" -> \"%s\", height=%i (ch=%i), clipRect = 0x%lX )"),
        firstItem->GetLabel().c_str(),
        lastItem->GetLabel().c_str(),
        (int)(lastItemBottomY - firstItemTopY),
        (int)m_height,
        (unsigned long)clipRect );
#endif

    wxRect r;

    long windowStyle = m_windowStyle;

    int xRelMod = 0;
    int yRelMod = 0;

    //
    // With wxPG_DOUBLE_BUFFER, do double buffering
    // - buffer's y = 0, so align cliprect and coordinates to that
    //
#if wxPG_DOUBLE_BUFFER

    wxRect cr2;

    if ( bufferDC )
    {
        xRelMod = clipRect->x;
        yRelMod = clipRect->y;

        //
        // clipRect conversion
        if ( clipRect )
        {
            cr2 = *clipRect;
            cr2.x -= xRelMod;
            cr2.y -= yRelMod;
            clipRect = &cr2;
        }
        firstItemTopY -= yRelMod;
        lastItemBottomY -= yRelMod;
    }
#endif

    int x = m_marginWidth - xRelMod;

    const wxFont& normalfont = m_font;

    bool reallyFocused = (m_iFlags & wxPG_FL_FOCUSED) ? true : false;

    bool isEnabled = IsEnabled();

    //
    // Prepare some pens and brushes that are often changed to.
    //

    wxBrush marginBrush(m_colMargin);
    wxPen marginPen(m_colMargin);
    wxBrush capbgbrush(m_colCapBack,wxSOLID);
    wxPen linepen(m_colLine,1,wxSOLID);

    // pen that has same colour as text
    wxPen outlinepen(m_colPropFore,1,wxSOLID);

    dc.SetClippingRegion( *clipRect );

    //
    // Clear margin with background colour
    //
    dc.SetBrush( marginBrush );
    if ( !(windowStyle & wxPG_HIDE_MARGIN) )
    {
        dc.SetPen( *wxTRANSPARENT_PEN );
        dc.DrawRectangle(-1-xRelMod,firstItemTopY-1,x+2,lastItemBottomY-firstItemTopY+2);
    }

    const wxPGProperty* selected = m_selected;
    const wxPropertyGridState* state = m_pState;

#if wxPG_REFRESH_CONTROLS_AFTER_REPAINT
    bool wasSelectedPainted = false;
#endif

    // TODO: Only render columns that are within clipping region.

    dc.SetFont(normalfont);

    wxPropertyGridConstIterator it( state, wxPG_ITERATE_VISIBLE, firstItem );
    int endScanBottomY = lastItemBottomY + lh;
    int y = firstItemTopY;
    unsigned int arrInd = 0;

    for ( ; !it.AtEnd(); it.Next() )
    {
        const wxPGProperty* p = *it;
        //wxLogDebug(wxT("%s, %i < %i"),p.GetName().c_str(), y, endScanBottomY);

        if ( !p->HasFlag(wxPG_PROP_HIDDEN) )
        {
            m_visPropArray[arrInd] = (wxPGProperty*)p;
            arrInd++;

            if ( y > endScanBottomY )
                break;

            y += lh;
        }
    }

    m_visPropArray[arrInd] = NULL;

    int gridWidth = state->m_width;

    y = firstItemTopY;
    for ( arrInd=0;
          m_visPropArray[arrInd] != NULL && y <= lastItemBottomY;
          arrInd++ )
    {
        wxPGProperty* p =(wxPGProperty*) m_visPropArray[arrInd];
        wxPGProperty* nextP = (wxPGProperty*) m_visPropArray[arrInd+1];

        int rowHeight = m_fontHeight+(m_spacingy*2)+1;
        int textMarginHere = x;
        int renderFlags = wxPGCellRenderer::Control;

        int greyDepth = m_marginWidth;
        if ( !(windowStyle & wxPG_HIDE_CATEGORIES) )
            greyDepth = (((int)p->m_depthBgCol)-1) * m_subgroup_extramargin + m_marginWidth;

        int greyDepth0 = greyDepth;
        greyDepth -= xRelMod;

        // Use basic depth if in non-categoric mode and parent is base array.
        if ( !(windowStyle & wxPG_HIDE_CATEGORIES) || p->GetParent() != m_pState->m_properties )
        {
            textMarginHere += ((unsigned int)((p->m_depth-1)*m_subgroup_extramargin));
        }

        // Paint margin area
        dc.SetBrush(marginBrush);
        dc.SetPen(marginPen);
        dc.DrawRectangle( -xRelMod, y, greyDepth, lh );

        dc.SetPen( linepen );

        int y2 = y + lh;

        // Margin Edge
        dc.DrawLine( greyDepth, y, greyDepth, y2 );

        // Splitters
        unsigned int si;
        int sx = x;

        for ( si=0; si<state->m_colWidths.size(); si++ )
        {
            sx += state->m_colWidths[si];
            dc.DrawLine( sx, y, sx, y2 );
        }

        // Horizontal Line, below
        //   (not if both this and next is category caption)
        if ( p->IsCategory() &&
             nextP && nextP->IsCategory() )
            dc.SetPen(m_colCapBack);

        dc.DrawLine( greyDepth, y2-1, gridWidth, y2-1 );

        if ( p == selected )
        {
            renderFlags |= wxPGCellRenderer::Selected;
#if wxPG_REFRESH_CONTROLS_AFTER_REPAINT
            wasSelectedPainted = true;
#endif
        }

        wxColour rowBgCol;
        wxColour rowFgCol;
        wxBrush rowBgBrush;

        if ( p->IsCategory() )
        {
            if ( p->m_fgColIndex == 0 )
                rowFgCol = m_colCapFore;
            else
                rowFgCol = *(wxPGColour*)m_arrFgCols[p->m_fgColIndex];
            rowBgBrush = wxBrush(m_colCapBack);
        }
        else if ( p != selected )
        {
            // Disabled may get different colour.
            if ( !p->IsEnabled() )
                rowFgCol = m_colDisPropFore;
            else
                rowFgCol = m_colPropFore;

            rowBgBrush = *(wxPGBrush*)m_arrBgBrushes[p->m_bgColIndex];
        }
        else
        {
            // Selected gets different colour.
            if ( reallyFocused )
            {
                rowFgCol = m_colSelFore;
                rowBgBrush = wxBrush(m_colSelBack);
            }
            else if ( isEnabled )
            {
                rowFgCol = *(wxPGColour*)m_arrFgCols[p->m_fgColIndex];
                rowBgBrush = marginBrush;
            }
            else
            {
                rowFgCol = m_colDisPropFore;
                rowBgBrush = wxBrush(m_colSelBack);
            }
        }

        bool fontChanged = false;

        wxRect butRect( ((p->m_depth - 1) * m_subgroup_extramargin) - xRelMod,
                        y,
                        m_marginWidth,
                        lh );

        if ( p->IsCategory() )
        {
            // Captions are all cells merged as one
            dc.SetFont(m_captionFont);
            fontChanged = true;
            wxRect cellRect(greyDepth, y, gridWidth - greyDepth0 + 2, rowHeight-1 );

            dc.SetBrush(rowBgBrush);
            dc.SetPen(rowBgBrush.GetColour());
            dc.SetTextForeground(rowFgCol);

            dc.DrawRectangle(cellRect);

            // Foreground
            wxPGCellRenderer* renderer = p->GetCellRenderer(0);
            renderer->Render( dc, cellRect, this, p, 0, -1, renderFlags );

            // Tree Item Button
            if ( !HasFlag(wxPG_HIDE_MARGIN) )
                DrawExpanderButton( dc, butRect, p );
        }
        else
        {
            if ( p->m_flags & wxPG_PROP_MODIFIED && (windowStyle & wxPG_BOLD_MODIFIED) )
            {
                dc.SetFont(m_captionFont);
                fontChanged = true;
            }

            unsigned int ci;
            int cellX = x + 1;
            int nextCellWidth = state->m_colWidths[0] - greyDepth + m_marginWidth;
            wxRect cellRect(greyDepth+1, y, 0, rowHeight-1);
            int textXAdd = textMarginHere - greyDepth;

            for ( ci=0; ci<state->m_colWidths.size(); ci++ )
            {
                cellRect.width = nextCellWidth - 1;

                bool ctrlCell = false;

                // Background
                if ( p == selected && m_wndPrimary && ci == 1 )
                {
                    dc.SetBrush(m_colPropBack);
                    dc.SetPen(m_colPropBack);
                    dc.SetTextForeground(m_colPropFore);

                    if ( m_dragStatus == 0 && !(m_iFlags & wxPG_FL_CUR_USES_CUSTOM_IMAGE) )
                        ctrlCell = true;
                }
                else
                {
                    dc.SetBrush(rowBgBrush);
                    dc.SetPen(rowBgBrush.GetColour());
                    dc.SetTextForeground(rowFgCol);
                }

                dc.DrawRectangle(cellRect);

                // Tree Item Button
                if ( ci == 0 && !HasFlag(wxPG_HIDE_MARGIN) && p->GetChildCount() )
                    DrawExpanderButton( dc, butRect, p );

                dc.SetClippingRegion(cellRect);

                cellRect.x += textXAdd;
                cellRect.width -= textXAdd;

                // Foreground
                if ( !ctrlCell )
                {
                    wxPGCellRenderer* renderer;
                    int cmnVal = p->GetCommonValue();
                    if ( cmnVal == -1 || ci != 1 )
                    {
                        renderer = p->GetCellRenderer(ci);
                        renderer->Render( dc, cellRect, this, p, ci, -1, renderFlags );
                    }
                    else
                    {
                        renderer = GetCommonValue(cmnVal)->GetRenderer();
                        renderer->Render( dc, cellRect, this, p, ci, -1, renderFlags );
                    }
                }

                cellX += state->m_colWidths[ci];
                if ( ci < (state->m_colWidths.size()-1) )
                    nextCellWidth = state->m_colWidths[ci+1];
                cellRect.x = cellX; 
                dc.DestroyClippingRegion(); // Is this really necessary?
                textXAdd = 0;
            }
        }

        if ( fontChanged )
            dc.SetFont(normalfont);

        y += rowHeight;
    }

#if wxPG_DOUBLE_BUFFER
    if ( bufferDC )
    {
        //wxLogDebug(wxT("%i, %i, %i"),yRelMod,blitClipRect->height,y);
        dcMain.SetClippingRegion( *blitClipRect );
        dcMain.Blit( blitClipRect->x, blitClipRect->y, blitClipRect->width, blitClipRect->height,
            &dc, 0, 0, wxCOPY );
        dcMain.DestroyClippingRegion(); // Is this really necessary?
        delete bufferDC;
    }
#endif

#if __PAINT_DEBUGGING__
    wxLogDebug(wxT("  \\--> ends..."));
#endif

    // Refresh editor controls (seems not needed on msw)
    // NOTE: This code is mandatory for GTK!
#if wxPG_REFRESH_CONTROLS_AFTER_REPAINT
    if ( wasSelectedPainted )
    {
        if ( m_wndPrimary )
            m_wndPrimary->Refresh();
        if ( m_wndSecondary )
            m_wndSecondary->Refresh();
    }
#endif

    return y + yRelMod;
}

// -----------------------------------------------------------------------

wxRect wxPropertyGrid::GetPropertyRect( const wxPGProperty* p1, const wxPGProperty* p2 ) const
{
    wxRect r;

    if ( m_width < 10 || m_height < 10 ||
         !m_pState->m_properties->GetCount() ||
         p1 == (wxPGProperty*) NULL )
        return wxRect(0,0,0,0);

    int vy = 0;

    //
    // Return rect which encloses the given property range

    int visTop = p1->GetY();
    int visBottom;
    if ( p2 )
        visBottom = p2->GetY() + m_lineHeight;
    else
        visBottom = m_height + visTop;

    // If seleced property is inside the range, we'll extend the range to include
    // control's size.
    wxPGProperty* selected = m_selected;
    if ( selected )
    {
        int selectedY = selected->GetY();
        if ( selectedY >= visTop && selectedY < visBottom )
        {
            wxWindow* editor = GetEditorControl();
            if ( editor )
            {
                int visBottom2 = selectedY + editor->GetSize().y;
                if ( visBottom2 > visBottom )
                    visBottom = visBottom2;
            }
        }
    }

    return wxRect(0,visTop-vy,m_pState->m_width,visBottom-visTop);
}

// -----------------------------------------------------------------------

void wxPropertyGrid::DrawItems( const wxPGProperty* p1, const wxPGProperty* p2 )
{
    if ( m_frozen )
        return;

    if ( m_pState->m_itemsAdded )
        PrepareAfterItemsAdded();

    wxRect r = GetPropertyRect(p1, p2);
    if ( r.width > 0 )
        m_canvas->RefreshRect(r);
}

// -----------------------------------------------------------------------

// In addition to calling DoDrawItems directly, this is the
// only alternative for using wxClientDC - others just call
// RefreshRect.
void wxPropertyGrid::DrawItem( wxDC& dc, wxPGProperty* p )
{
    wxCHECK_RET( p, wxT("invalid property id") );

    // do not draw a single item if multiple pending
    if ( m_pState->m_itemsAdded )
        return;

#if __PAINT_DEBUGGING__
    wxLogDebug(wxT("wxPropertyGrid::DrawItem( %s )"), p->GetLabel().c_str() );
#endif

    wxRect r(GetPropertyRect(p, p));
    DoDrawItems( dc, p, p, &r );
}

// -----------------------------------------------------------------------

void wxPropertyGrid::RefreshProperty( wxPGProperty* p )
{
    if ( p == m_selected )
        DoSelectProperty(p, wxPG_SEL_FORCE);

    DrawItemAndChildren(p);
}

// -----------------------------------------------------------------------

void wxPropertyGrid::DrawItemAndValueRelated( wxPGProperty* p )
{
    if ( m_frozen )
        return;

    // Draw item, children, and parent too, if it is not category
    wxPGProperty* parent = p->GetParent();

    while ( parent &&
            !parent->IsCategory() &&
            parent->GetParent() )
    {
         DrawItem(parent);
         parent = parent->GetParent();
    }

    DrawItemAndChildren(p);
}

void wxPropertyGrid::DrawItemAndChildren( wxPGProperty* p )
{
    wxCHECK_RET( p, wxT("invalid property id") );

    // Do not draw if in non-visible page
    if ( p->GetParentState() != m_pState )
        return;

    // do not draw a single item if multiple pending
    if ( m_pState->m_itemsAdded || m_frozen )
        return;

#if __PAINT_DEBUGGING__
    wxLogDebug(wxT("wxPropertyGrid::DrawItemAndChildren( %s )"), p->GetLabel().c_str() );
#endif

    wxWindow* wndPrimary = GetEditorControl();

    // Update child control.
    if ( m_selected && m_selected->GetParent() == p )
        m_selected->UpdateControl(wndPrimary);

    const wxPGProperty* lastDrawn = p->GetLastVisibleSubItem();

    DrawItems(p, lastDrawn);
}

// -----------------------------------------------------------------------

void wxPropertyGrid::Refresh( bool WXUNUSED(eraseBackground),
                              const wxRect *rect )
{
    PrepareAfterItemsAdded();

    wxWindow::Refresh(false);
    if ( m_canvas )
        // TODO: Coordinate translation
        m_canvas->Refresh(false, rect);
#if wxPG_REFRESH_CONTROLS_AFTER_REPAINT
    // I think this really helps only GTK+1.2
    if ( m_wndPrimary ) m_wndPrimary->Refresh();
    if ( m_wndSecondary ) m_wndSecondary->Refresh();
#endif
}

// -----------------------------------------------------------------------

#if wxPG_HEAVY_GFX
void wxPropertyGrid::DrawSplitterDragColumn( wxDC&, int ) { }
#else
void wxPropertyGrid::DrawSplitterDragColumn( wxDC& dc, int x )
{
    int vx, vy;
    GetViewStart(&vx,&vy);
    vy *= wxPG_PIXELS_PER_UNIT;

    dc.SetLogicalFunction(wxINVERT);
    dc.DestroyClippingRegion();

    dc.SetPen( m_splitterpen );
    dc.DrawLine(x,vy,x,vy+m_height);

}
#endif

// -----------------------------------------------------------------------
// wxPropertyGrid global operations
// -----------------------------------------------------------------------

void wxPropertyGrid::Clear()
{
    if ( m_selected )
    {
        bool selRes = DoSelectProperty(NULL, wxPG_SEL_DELETING);  // This must be before state clear
        wxPG_CHECK_RET_DBG( selRes,
                            wxT("failed to deselect a property (editor probably had invalid value)") );
    }

    m_pState->Clear();

    m_propHover = NULL;

    m_prevVY = 0;

    RecalculateVirtualSize();

    // Need to clear some area at the end
    if ( !m_frozen )
        RefreshRect(wxRect(0, 0, m_width, m_height));
}

// -----------------------------------------------------------------------

bool wxPropertyGrid::EnableCategories( bool enable )
{
    if ( !ClearSelection() )
        return false;

    if ( enable )
    {
        //
        // Enable categories
        //

        m_windowStyle &= ~(wxPG_HIDE_CATEGORIES);
    }
    else
    {
        //
        // Disable categories
        //
        m_windowStyle |= wxPG_HIDE_CATEGORIES;
    }

    if ( !m_pState->EnableCategories(enable) )
        return false;

    if ( !m_frozen )
    {
        if ( m_windowStyle & wxPG_AUTO_SORT )
        {
            m_pState->m_itemsAdded = 1; // force
            PrepareAfterItemsAdded();
        }
    }
    else
        m_pState->m_itemsAdded = 1;

    RecalculateVirtualSize();
    m_canvas->Refresh();

    return true;
}

// -----------------------------------------------------------------------

void wxPropertyGrid::SwitchState( wxPropertyGridState* pNewState )
{
    wxASSERT( pNewState );
    wxASSERT( pNewState->GetGrid() );

    wxPGProperty* oldSelection = m_selected;

    // Deselect
    if ( m_selected )
    {
        bool selRes = ClearSelection();
        wxPG_CHECK_RET_DBG( selRes,
                            wxT("failed to deselect a property (editor probably had invalid value)") );
    }

    m_pState->m_selected = oldSelection;

    bool orig_mode = m_pState->IsInNonCatMode();
    bool new_state_mode = pNewState->IsInNonCatMode();

    m_pState = pNewState;

    // Validate width
    int pgWidth = GetClientSize().x;
    if ( HasVirtualWidth() )
    {
        int minWidth = pgWidth;
        if ( pNewState->m_width < minWidth )
        {
            pNewState->m_width = minWidth;
            pNewState->CheckColumnWidths();
        }
    }
    else
    {
        pNewState->OnClientWidthChange( pgWidth, pgWidth - pNewState->m_width );
    }

    m_propHover = (wxPGProperty*) NULL;

    // If necessary, convert state to correct mode.
    if ( orig_mode != new_state_mode )
    {
        // This should refresh as well.
        EnableCategories( orig_mode?false:true );
    }
    else if ( !m_frozen )
    {
        // Refresh, if not frozen.
        if ( m_pState->m_itemsAdded )
            PrepareAfterItemsAdded();

        // Reselect
        if ( m_pState->m_selected )
            DoSelectProperty( m_pState->m_selected );

        RecalculateVirtualSize(0);
        m_canvas->Refresh();
    }
    else
        m_pState->m_itemsAdded = 1;
}

// -----------------------------------------------------------------------

void wxPropertyGrid::Sort( wxPGPropArg id )
{
    wxPG_PROP_ARG_CALL_PROLOG()

    m_pState->Sort( p );
}

// -----------------------------------------------------------------------

void wxPropertyGrid::Sort()
{
    bool selRes = ClearSelection();  // This must be before state clear
    wxPG_CHECK_RET_DBG( selRes,
                        wxT("failed to deselect a property (editor probably had invalid value)") );

    m_pState->Sort();
}

// -----------------------------------------------------------------------

// Call to SetSplitterPosition will always disable splitter auto-centering
// if parent window is shown.
void wxPropertyGrid::DoSetSplitterPosition( int newxpos, bool refresh, int splitterIndex, bool allPages )
{
    if ( ( newxpos < wxPG_DRAG_MARGIN ) )
        return;
    // ( m_width > wxPG_DRAG_MARGIN && newxpos > (m_width-wxPG_DRAG_MARGIN) )

#if __INTENSE_DEBUGGING__
    wxLogDebug( wxT("wxPropertyGrid::DoSetSplitterPosition ( %i )"), newxpos );
#endif

    wxPropertyGridState* state = m_pState;

#if wxPG_HEAVY_GFX
    state->DoSetSplitterPosition( newxpos, splitterIndex, allPages );

    if ( refresh )
    {
        if ( m_selected )
            CorrectEditorWidgetSizeX();

        m_canvas->Refresh();
    }
#else
    #error "Not currently supported"
    if ( !m_dragStatus )
    {
        // Only do this if this was not a call from HandleMouseUp
        m_startingSplitterX = splitterX;
        splitterX = newxpos;
        m_fSplitterX = (float) newxpos;
    }

    // Clear old
    if ( m_splitterprevdrawnx != -1 )
    {
        wxPG_CLIENT_DC_INIT()

        DrawSplitterDragColumn( dc, m_splitterprevdrawnx );
        m_splitterprevdrawnx = -1;
    }

    // Redraw only if drag really moved
    if ( splitterX != m_startingSplitterX && refresh )
    {
        if ( m_selected)
            CorrectEditorWidgetSizeX();

        Update(); // This fixes a graphics-mess in wxMSW

        m_canvas->Refresh();
    }
#endif
}

// -----------------------------------------------------------------------

void wxPropertyGrid::CenterSplitter( bool enableAutoCentering )
{
    SetSplitterPosition( m_width/2, true );
    if ( enableAutoCentering && ( m_windowStyle & wxPG_SPLITTER_AUTO_CENTER ) )
        m_iFlags &= ~(wxPG_FL_DONT_CENTER_SPLITTER);
}

// -----------------------------------------------------------------------
// wxPropertyGrid item iteration (GetNextProperty etc.) methods
// -----------------------------------------------------------------------

// Returns nearest paint visible property (such that will be painted unless
// window is scrolled or resized). If given property is paint visible, then
// it itself will be returned
wxPGProperty* wxPropertyGrid::GetNearestPaintVisible( wxPGProperty* p ) const
{
    int vx,vy1;// Top left corner of client
    GetViewStart(&vx,&vy1);
    vy1 *= wxPG_PIXELS_PER_UNIT;

    int vy2 = vy1 + m_height;
    int propY = p->GetY2(m_lineHeight);

    if ( (propY + m_lineHeight) < vy1 )
    {
    // Too high
        return DoGetItemAtY( vy1 );
    }
    else if ( propY > vy2 )
    {
    // Too low
        return DoGetItemAtY( vy2 );
    }

    // Itself paint visible
    return p;

}

// -----------------------------------------------------------------------

void wxPropertyGrid::SetButtonShortcut( int keycode, bool ctrlDown, bool altDown )
{
    if ( keycode )
    {
        m_pushButKeyCode = keycode;
        m_pushButKeyCodeNeedsCtrl = ctrlDown ? 1 : 0;
        m_pushButKeyCodeNeedsAlt = altDown ? 1 : 0;
    }
    else
    {
        m_pushButKeyCode = WXK_DOWN;
        m_pushButKeyCodeNeedsCtrl = 0;
        m_pushButKeyCodeNeedsAlt = 1;
    }
}

// -----------------------------------------------------------------------
// Methods related to change in value, value modification and sending events
// -----------------------------------------------------------------------

// commits any changes in editor of selected property
// return true if validation did not fail
// flags are same as with DoSelectProperty
bool wxPropertyGrid::CommitChangesFromEditor( wxUint32 flags )
{
    if ( m_wndPrimary && IsEditorsValueModified() &&
         (m_iFlags & wxPG_FL_INITIALIZED) )
    {
        bool wasUnspecified = m_selected->IsValueUnspecified();

        // JACS - necessary to avoid new focus being found spuriously within OnIdle
        // due to another window getting focus
        wxWindow* oldFocus = m_curFocused;

        if ( !(flags & (wxPG_SEL_NOVALIDATE|wxPG_SEL_FORCE)) &&
             !DoEditorValidate() )
        {
            if (oldFocus)
            {
                oldFocus->SetFocus();
                m_curFocused = oldFocus;
            }

            return false;
        }

        // If truly modified, schedule value as pending.
        wxVariant variant(m_selected->GetValueRef());
        wxVariant* pvariant = NULL;
        if ( m_selected->GetEditorClass()->ActualGetValueFromControl( variant, m_selected, GetEditorControl() ) )
        {
            if ( wxPGIsVariantType(variant, list) ||
                 m_selected->ActualValidateValue(variant) )
                pvariant = &variant;
        }
        else
            EditorsValueWasNotModified();

        if ( m_selected->IsValueUnspecified() && !wasUnspecified && UsesAutoUnspecified() )
            flags |= wxPG_SEL_SETUNSPEC;

        DoPropertyChanged( m_selected, pvariant, flags );

        return true;
    }

    return true;
}

// -----------------------------------------------------------------------

// flags are same as with DoSelectProperty
bool wxPropertyGrid::DoPropertyChanged( wxPGProperty* p, wxVariant* pPendingValue, unsigned int selFlags )
{
    if ( m_propValChanging )
        return true;

#if __INTENSE_DEBUGGING__
    wxLogDebug(wxT("wxPropertyGrid::DoPropertyChanged( %s )"),p->GetLabel().c_str());
#endif

    m_pState->m_anyModified = 1;

    m_propValChanging = 1;

    // Maybe need to update control
    if ( m_iFlags & wxPG_FL_VALUE_MODIFIED )
    {
        //
        // Adapt list to child values, if necessary
        wxVariant value;
        wxVariant listValue;
        wxVariant* pList = NULL;
        bool allowModify = true;

        // If parent has wxPG_PROP_AGGREGATE flag,
        // then we need treat as it was changed instead.
        // This includes creating list variant for child
        // value.

        wxPGProperty* pwc = p->GetParent();
        wxPGProperty* changedProperty = p;

        if ( pPendingValue )
        {
            listValue = *pPendingValue;
            listValue.SetName(p->GetLabel());
        }

        while ( pwc &&
                pwc->HasFlag(wxPG_PROP_AGGREGATE) )
        {
            if ( pPendingValue )
            {
                wxVariantList tempList;
                wxVariant lv(tempList, pwc->GetLabel());
                lv.Append(listValue);
                listValue = lv;
                pPendingValue = &listValue;
            }

            changedProperty = pwc;
            pwc = pwc->GetParent();
        }

        wxPGProperty* topPaintedProperty = changedProperty;

        while ( pwc &&
                !pwc->IsCategory() &&
                !pwc->IsRoot() )
        {
            topPaintedProperty = pwc;
            pwc = pwc->GetParent();
        }

        if ( pPendingValue )
        {
            if ( !wxPGIsVariantType(*pPendingValue, list) )
            {
                value = *pPendingValue;
            }
            else
            {
                // Convert list to child values
                pList = pPendingValue;
                changedProperty->AdaptListToValue( *pPendingValue, &value );
            }
        }

        //
        // Does the application allow property to be modified?
        if ( pPendingValue )
        {
            // SendEvent returns true if event was vetoed
            if ( SendEvent( wxEVT_PG_CHANGING, changedProperty, &value, selFlags ) )
                allowModify = false;
        }

        m_iFlags &= ~(wxPG_FL_VALUE_MODIFIED);

        if ( allowModify )
        {
            if ( pPendingValue )
                changedProperty->SetValue0(value, pList, false);

            // Set as Modified (not if dragging just began)
            if ( !(p->m_flags & wxPG_PROP_MODIFIED) )
            {
                p->m_flags |= wxPG_PROP_MODIFIED;
                if ( p == m_selected && (m_windowStyle & wxPG_BOLD_MODIFIED) )
                {
                    if ( m_wndPrimary )
                        SetCurControlBoldFont();
                }
            }

            pwc = p;
            wxPGProperty* prevPwc = NULL;

            // Propagate updates to parent(s)
            while ( prevPwc != topPaintedProperty )
            {
                pwc->m_flags |= wxPG_PROP_MODIFIED;
                if ( pwc == m_selected && (m_windowStyle & wxPG_BOLD_MODIFIED) )
                {
                    if ( m_wndPrimary )
                        SetCurControlBoldFont();
                }

                prevPwc = pwc;
                pwc = pwc->GetParent();
            }

            // Draw the actual property
            DrawItemAndChildren( topPaintedProperty );

            //
            // If value was set by wxPGProperty::OnEvent, then update the editor
            // control.
            if ( selFlags & wxPG_SEL_DIALOGVAL )
            {
                if ( m_wndPrimary )
                    p->GetEditorClass()->UpdateControl(p, m_wndPrimary);
            }
            else
            {
        #if wxPG_REFRESH_CONTROLS_AFTER_REPAINT
                if ( m_wndPrimary ) m_wndPrimary->Refresh();
                if ( m_wndSecondary ) m_wndSecondary->Refresh();
        #endif
            }

            SendEvent( wxEVT_PG_CHANGED, changedProperty, NULL, selFlags );
        }
    }

    m_propValChanging = 0;

    return true;
}

// -----------------------------------------------------------------------

// Runs wxValidator for the selected property
bool wxPropertyGrid::DoEditorValidate()
{
#if wxUSE_VALIDATORS
    // With traditional validator style, we dont need to more
    if ( GetExtraStyle() & wxPG_EX_TRADITIONAL_VALIDATORS )
        return true;

    if ( m_iFlags & wxPG_FL_VALIDATION_FAILED )
    {
        return false;
    }

    wxWindow* wnd = GetEditorControl();

    wxValidator* validator = m_selected->GetValidator();
    if ( validator && wnd )
    {
        // Use TextCtrl of ODComboBox instead
        if ( wnd->IsKindOf(CLASSINFO(wxPGOwnerDrawnComboBox)) )
        {
            wnd = ((wxPGOwnerDrawnComboBox*)wnd)->GetTextCtrl();

            if ( !wnd )
                return true;
        }

        validator->SetWindow(wnd);

        // Instead setting the flag after the failure, we set
        // it before checking and then clear afterwards if things
        // went fine. This trick is necessary since focus events
        // may be triggered while in Validate.
        m_iFlags |= wxPG_FL_VALIDATION_FAILED;
        if ( !validator->Validate(this) )
        {
            // If you dpm't want to display message multiple times per change,
            // comment the following line.
            m_iFlags &= ~(wxPG_FL_VALIDATION_FAILED);
            return false;
        }
        m_iFlags &= ~(wxPG_FL_VALIDATION_FAILED);
    }
#endif
    return true;
}

// -----------------------------------------------------------------------

bool wxPropertyGrid::ProcessEvent(wxEvent& event)
{
    wxWindow* wnd = (wxWindow*) event.GetEventObject();
    if ( wnd )
    {
        wxASSERT( wnd->IsKindOf(CLASSINFO(wxWindow)) );
        wxWindow* parent = wnd->GetParent();

        if ( parent &&
             (parent == m_canvas ||
              parent->GetParent() == m_canvas) )
        {
            OnCustomEditorEvent((wxCommandEvent&)event);
            return true;
        }
    }
    return wxPanel::ProcessEvent(event);
}

// -----------------------------------------------------------------------

// NB: It may really not be wxCommandEvent - must check if necessary
//     (usually not).
void wxPropertyGrid::OnCustomEditorEvent( wxCommandEvent &event )
{
    wxPGProperty* selected = m_selected;

    //
    // Somehow, event is handled after property has been deselected.
    // Possibly, but very rare.
    if ( !selected )
        return;

    wxVariant variant(selected->GetValueRef());
    wxWindow* wnd = GetEditorControl();
    int selFlags = 0;
    bool wasUnspecified = selected->IsValueUnspecified();
    bool usesAutoUnspecified = UsesAutoUnspecified();
    bool valuePending = false;
    bool res0 = false;
    bool res1 = false;
    bool res2 = false;

    m_iFlags &= ~(wxPG_FL_VALIDATION_FAILED|wxPG_FL_VALUE_CHANGE_IN_EVENT);

    // Try common button handling
    if ( m_wndSecondary && event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED )
    {
        wxPGEditorDialogAdapter* adapter = selected->GetEditorDialog();
        if ( adapter )
        {
            res0 = true;
            // Store as res2, as previously (and still currently alternatively)
            // dialogs can be shown by handling wxEVT_COMMAND_BUTTON_CLICKED
            // in wxPGProperty::OnEvent().
            res2 = adapter->ShowDialog( this, selected );
            delete adapter;
        }
    }

    if ( !res0 )
    {
        // First call editor class' event handler.
        const wxPGEditor* editor = selected->GetEditorClass();
        res1 = editor->OnEvent( this, selected, wnd, event );

        if ( res1 )
        {
            // If changes, validate them
            if ( DoEditorValidate() )
            {
                if ( editor->ActualGetValueFromControl( variant, selected, wnd ) &&
                     (wxPGIsVariantType(variant, list) || selected->ActualValidateValue( variant )))
                {
                    valuePending = true;
                }
                else
                {
                    // False alarm
                    res1 = false;

                    EditorsValueWasNotModified();

                    // However, even moot editing will clear the unspecified status
                    //if ( wasUnspecified || !usesAutoUnspecified )
                    //    CLEAR_PROPERTY_UNSPECIFIED_FLAG(selected);
                }
            }
            else
            {
                res1 = false;
                EditorsValueWasNotModified();
                if ( wasUnspecified || !usesAutoUnspecified )
                    CLEAR_PROPERTY_UNSPECIFIED_FLAG(selected);
                return;
            }
        }

        // Then the property's custom handler (must be always called).
        res2 = selected->OnEvent( this, wnd, event );
    }

    if ( m_iFlags & wxPG_FL_VALUE_CHANGE_IN_EVENT )
    {
        valuePending = true;
        variant = m_changeInEventValue;
        selFlags |= wxPG_SEL_DIALOGVAL;
        res2 = true;
    }

    if ( res1 || res2 )
    {
        wxVariant* pvariant = NULL;
        if ( valuePending )
            pvariant = &variant;

        // Setting this is not required if res was true, so we do it now.
        m_iFlags |= wxPG_FL_VALUE_MODIFIED;

        selFlags |= ( !wasUnspecified && selected->IsValueUnspecified() && usesAutoUnspecified ) ? wxPG_SEL_SETUNSPEC : 0;

        DoPropertyChanged(selected, pvariant, selFlags);
    }
    else
        // Let unhandled button click events go to the parent
        if ( event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED )
        {
            wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED,GetId());
            GetEventHandler()->AddPendingEvent(evt);
        }
}

// -----------------------------------------------------------------------
// wxPropertyGrid editor control helper methods
// -----------------------------------------------------------------------

wxRect wxPropertyGrid::GetEditorWidgetRect( wxPGProperty* p, int column ) const
{
    int itemy = p->GetY2(m_lineHeight);
    int vy = 0;
    int cust_img_space = 0;
    int splitterX = m_pState->GetSplitterPosition(column-1);
    int colEnd = splitterX + m_pState->m_colWidths[column];

    //m_iFlags &= ~(wxPG_FL_CUR_USES_CUSTOM_IMAGE);

    // TODO: If custom image detection changes from current, change this.
    if ( m_iFlags & wxPG_FL_CUR_USES_CUSTOM_IMAGE /*p->m_flags & wxPG_PROP_CUSTOMIMAGE*/ )
    {
        //m_iFlags |= wxPG_FL_CUR_USES_CUSTOM_IMAGE;
        int imwid = p->OnMeasureImage().x;
        if ( imwid < 1 ) imwid = wxPG_CUSTOM_IMAGE_WIDTH;
        cust_img_space = imwid + wxCC_CUSTOM_IMAGE_MARGIN1 + wxCC_CUSTOM_IMAGE_MARGIN2;
    }

    return wxRect
      (
        splitterX+cust_img_space+wxPG_XBEFOREWIDGET+wxPG_CONTROL_MARGIN+1,
        itemy-vy,
        colEnd-splitterX-wxPG_XBEFOREWIDGET-wxPG_CONTROL_MARGIN-cust_img_space-1,
        m_lineHeight-1
      );
}

// -----------------------------------------------------------------------

wxRect wxPropertyGrid::GetImageRect( wxPGProperty* p, int item ) const
{
    wxSize sz = GetImageSize(p, item);
    return wxRect(wxPG_CONTROL_MARGIN + wxCC_CUSTOM_IMAGE_MARGIN1,
                  wxPG_CUSTOM_IMAGE_SPACINGY,
                  sz.x,
                  sz.y);
}

// return size of custom paint image
wxSize wxPropertyGrid::GetImageSize( wxPGProperty* p, int item ) const
{
    // If called with NULL property, then return default image
    // size for properties that use image.
    if ( !p )
        return wxSize(wxPG_CUSTOM_IMAGE_WIDTH,wxPG_STD_CUST_IMAGE_HEIGHT(m_lineHeight));

    wxSize cis = p->OnMeasureImage(item);

    int choiceCount = p->GetChoiceCount();
    int comVals = p->GetDisplayedCommonValueCount();
    if ( item >= choiceCount && comVals > 0 )
    {
        unsigned int cvi = item-choiceCount;
        cis = GetCommonValue(cvi)->GetRenderer()->GetImageSize(NULL, 1, cvi);
    }
    else if ( item >= 0 && choiceCount == 0 )
        return wxSize(0, 0);

    if ( cis.x < 0 )
    {
        if ( cis.x <= -1 )
            cis.x = wxPG_CUSTOM_IMAGE_WIDTH;
    }
    if ( cis.y <= 0 )
    {
        if ( cis.y >= -1 )
            cis.y = wxPG_STD_CUST_IMAGE_HEIGHT(m_lineHeight);
        else
            cis.y = -cis.y;
    }
    return cis;
}

// -----------------------------------------------------------------------

// takes scrolling into account
void wxPropertyGrid::ImprovedClientToScreen( int* px, int* py )
{
    int vx, vy;
    GetViewStart(&vx,&vy);
    vy*=wxPG_PIXELS_PER_UNIT;
    vx*=wxPG_PIXELS_PER_UNIT;
    *px -= vx;
    *py -= vy;
    ClientToScreen( px, py );
}

// -----------------------------------------------------------------------

// custom set cursor
void wxPropertyGrid::CustomSetCursor( int type, bool override )
{
    if ( type == m_curcursor && !override ) return;

    wxCursor* cursor = &wxPG_DEFAULT_CURSOR;

    if ( type == wxCURSOR_SIZEWE )
        cursor = m_cursorSizeWE;

    m_canvas->SetCursor( *cursor );

    //if ( m_wndPrimary ) m_wndPrimary->SetCursor(wxNullCursor);

    m_curcursor = type;
}

// -----------------------------------------------------------------------
// wxPropertyGrid property selection
// -----------------------------------------------------------------------

#define CONNECT_CHILD(EVT,FUNCTYPE,FUNC) \
    wnd->Connect(id, EVT, \
        (wxObjectEventFunction) (wxEventFunction)  \
        FUNCTYPE (&wxPropertyGrid::FUNC), \
        NULL, this );

// Setups event handling for child control
void wxPropertyGrid::SetupEventHandling( wxWindow* argWnd, int id )
{
    wxWindow* wnd = argWnd;

    if ( argWnd == m_wndPrimary )
    {
        CONNECT_CHILD(wxEVT_MOTION,(wxMouseEventFunction),OnMouseMoveChild)
        CONNECT_CHILD(wxEVT_LEFT_UP,(wxMouseEventFunction),OnMouseUpChild)
        CONNECT_CHILD(wxEVT_LEFT_DOWN,(wxMouseEventFunction),OnMouseClickChild)
        CONNECT_CHILD(wxEVT_RIGHT_UP,(wxMouseEventFunction),OnMouseRightClickChild)
        CONNECT_CHILD(wxEVT_ENTER_WINDOW,(wxMouseEventFunction),OnMouseEntry)
        CONNECT_CHILD(wxEVT_LEAVE_WINDOW,(wxMouseEventFunction),OnMouseEntry)
    }
    else
    {
        CONNECT_CHILD(wxEVT_NAVIGATION_KEY,(wxNavigationKeyEventFunction),OnNavigationKey)
    }
    CONNECT_CHILD(wxEVT_KEY_DOWN,(wxCharEventFunction),OnChildKeyDown)
    CONNECT_CHILD(wxEVT_KEY_UP,(wxCharEventFunction),OnChildKeyUp)
    CONNECT_CHILD(wxEVT_KILL_FOCUS,(wxFocusEventFunction),OnFocusEvent)
}

void wxPropertyGrid::FreeEditors()
{
    // Do not free editors immediately if processing events
    if ( !m_windowsToDelete )
        m_windowsToDelete = new wxArrayPtrVoid;

    if ( m_wndSecondary )
    {
        m_windowsToDelete->push_back(m_wndSecondary);
        m_wndSecondary->Hide();
        m_wndSecondary = (wxWindow*) NULL;
    }

    if ( m_wndPrimary )
    {
        m_windowsToDelete->push_back(m_wndPrimary);
        m_wndPrimary->Hide();
        m_wndPrimary = (wxWindow*) NULL;
    }
}

// Call with NULL to de-select property
bool wxPropertyGrid::DoSelectProperty( wxPGProperty* p, unsigned int flags )
{

#if __INTENSE_DEBUGGING__
    if (p)
        wxLogDebug(wxT("SelectProperty( %s (%s[%i]) )"),p->m_label.c_str(),
            p->m_parent->m_label.c_str(),p->GetIndexInParent());
    else
        wxLogDebug(wxT("SelectProperty( NULL, -1 )"));
#endif

    wxPGProperty* prev = m_selected;

    //
    // Delete windows pending for deletion
    if ( m_windowsToDelete && !m_propValChanging && m_windowsToDelete->size() )
    {
        unsigned int i;

        for ( i=0; i<m_windowsToDelete->size(); i++ )
            delete ((wxWindow*)((*m_windowsToDelete)[i]));

        m_windowsToDelete->clear();
    }

    if ( !m_pState )
        return false;

    //
    // If we are frozen, then just set the values.
    if ( m_frozen )
    {
        m_iFlags &= ~(wxPG_FL_ABNORMAL_EDITOR);
        m_editorFocused = 0;
        m_selected = p;
        m_selColumn = 1;
        m_pState->m_selected = p;

        // If frozen, always free controls. But don't worry, as Thaw will
        // recall SelectProperty to recreate them.
        FreeEditors();

        // Prevent any further selection measures in this call
        p = (wxPGProperty*) NULL;
    }
    else
    {
        // Is it the same?
        if ( m_selected == p && !(flags & wxPG_SEL_FORCE) )
        {
            // Only set focus if not deselecting
            if ( p )
            {
                if ( flags & wxPG_SEL_FOCUS )
                {
                    if ( m_wndPrimary )
                    {
                        m_wndPrimary->SetFocus();
                        m_editorFocused = 1;
                    }
                }
                else
                {
                    wxScrolledWindow::SetFocus();
                    m_editorFocused = 0;
                }
            }

            return true;
        }

        wxClientDC dc(m_canvas);

        // Don't put this earlier, due to return statements
        m_iFlags |= wxPG_FL_IN_SELECT_PROPERTY;

        //
        // First, deactivate previous
        if ( m_selected )
        {

    #if __INTENSE_DEBUGGING__
            wxLogDebug(wxT("  (closing previous (%s))"), m_selected->m_label.c_str() );
    #endif

            // Must double-check if this is an selected in case of forceswitch
            if ( p != prev )
            {
                if ( !CommitChangesFromEditor(flags) )
                {
                    // Validation has failed, so we can't exit the previous editor
                    //::wxMessageBox(_("Please correct the value or press ESC to cancel the edit."),
                    //               _("Invalid Value"),wxOK|wxICON_ERROR);
                    return false;
                }
            }

            FreeEditors();
            m_selColumn = -1;

            m_selected = (wxPGProperty*) NULL;
            m_pState->m_selected = (wxPGProperty*) NULL;

            // We need to always fully refresh the grid here
            m_canvas->Refresh(false);

            m_iFlags &= ~(wxPG_FL_VALUE_MODIFIED|wxPG_FL_ABNORMAL_EDITOR);
        }

        //
        // Then, activate the one given.
        if ( p )
        {
            int propY = p->GetY2(m_lineHeight);

            int splitterX = GetSplitterPosition();
            m_editorFocused = 0;
            m_selected = p;
            m_pState->m_selected = p;
            m_iFlags |= wxPG_FL_PRIMARY_FILLS_ENTIRE;
            if ( p != prev )
                m_iFlags &= ~(wxPG_FL_VALIDATION_FAILED);

            wxASSERT( m_wndPrimary == (wxWindow*) NULL );

            // Do we need OnMeasureCalls?
            wxSize imsz = p->OnMeasureImage();

            //
            // Only create editor for non-disabled non-caption
            if ( !p->IsCategory() && !(p->m_flags & wxPG_PROP_DISABLED) )
            {
            // do this for non-caption items

                m_selColumn = 1;

                // Do we need to paint the custom image, if any?
                m_iFlags &= ~(wxPG_FL_CUR_USES_CUSTOM_IMAGE);
                if ( (p->m_flags & wxPG_PROP_CUSTOMIMAGE) &&
                     !p->GetEditorClass()->CanContainCustomImage()
                   )
                    m_iFlags |= wxPG_FL_CUR_USES_CUSTOM_IMAGE;

                wxRect grect = GetEditorWidgetRect(p, m_selColumn);
                wxPoint goodPos = grect.GetPosition();
            #if wxPG_CREATE_CONTROLS_HIDDEN
                int coord_adjust = m_height - goodPos.y;
                goodPos.y += coord_adjust;
            #endif

                const wxPGEditor* editor = p->GetEditorClass();
                wxCHECK_MSG(editor, false,
                    wxT("NULL editor class not allowed"));

                m_iFlags &= ~wxPG_FL_FIXED_WIDTH_EDITOR;

                wxPGWindowList wndList = editor->CreateControls(this,
                                                                p,
                                                                goodPos,
                                                                grect.GetSize());

                m_wndPrimary = wndList.m_primary;
                m_wndSecondary = wndList.m_secondary;

                // NOTE: It is allowed for m_wndPrimary to be NULL - in this case
                //       value is drawn as normal, and m_wndSecondary is assumed
                //       to be a right-aligned button that triggers a separate editor
                //       window.

                if ( m_wndPrimary )
                {
                    wxASSERT_MSG( m_wndPrimary->GetParent() == m_canvas,
                                  wxT("CreateControls must use result of wxPropertyGrid::GetPanel() as parent of controls.") );

                    // Set validator, if any
                #if wxUSE_VALIDATORS
                    if ( GetExtraStyle() & wxPG_EX_TRADITIONAL_VALIDATORS )
                    {
                        wxValidator* validator = p->GetValidator();
                        if ( validator )
                            m_wndPrimary->SetValidator(*validator);
                    }
                #endif

                    if ( m_wndPrimary->GetSize().y > (m_lineHeight+6) )
                        m_iFlags |= wxPG_FL_ABNORMAL_EDITOR;

                    // If it has modified status, use bold font
                    // (must be done before capturing m_ctrlXAdjust)
                    if ( (p->m_flags & wxPG_PROP_MODIFIED) && (m_windowStyle & wxPG_BOLD_MODIFIED) )
                        SetCurControlBoldFont();

                    //
                    // Fix TextCtrl indentation
                #if defined(__WXMSW__) && !defined(__WXWINCE__)
                    wxTextCtrl* tc = NULL;
                    if ( m_wndPrimary->IsKindOf(CLASSINFO(wxPGOwnerDrawnComboBox)) )
                        tc = ((wxPGOwnerDrawnComboBox*)m_wndPrimary)->GetTextCtrl();
                    else
                        tc = wxDynamicCast(m_wndPrimary, wxTextCtrl);
                    if ( tc )
                        ::SendMessage(GetHwndOf(tc), EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELONG(0, 0));
                #endif

                    // Store x relative to splitter (we'll need it).
                    m_ctrlXAdjust = m_wndPrimary->GetPosition().x - splitterX;

                    // Check if background clear is not necessary
                    wxPoint pos = m_wndPrimary->GetPosition();
                    if ( pos.x > (splitterX+1) || pos.y > propY )
                    {
                        m_iFlags &= ~(wxPG_FL_PRIMARY_FILLS_ENTIRE);
                    }

                    m_wndPrimary->SetSizeHints(3, 3);

                #if wxPG_CREATE_CONTROLS_HIDDEN
                    m_wndPrimary->Show(false);
                    m_wndPrimary->Freeze();

                    goodPos = m_wndPrimary->GetPosition();
                    goodPos.y -= coord_adjust;
                    m_wndPrimary->Move( goodPos );
                #endif

                    wxWindow* primaryCtrl = GetEditorControl();
                    SetupEventHandling(primaryCtrl, wxPG_SUBID1);

                    // Focus and select all (wxTextCtrl, wxComboBox etc)
                    if ( flags & wxPG_SEL_FOCUS )
                    {
                        primaryCtrl->SetFocus();

                        p->GetEditorClass()->OnFocus(p, primaryCtrl);
                    }
                }

                if ( m_wndSecondary )
                {
                    wxASSERT_MSG( m_wndSecondary->GetParent() == m_canvas,
                                  wxT("CreateControls must use result of wxPropertyGrid::GetPanel() as parent of controls.") );

                    // Get proper id for wndSecondary
                    m_wndSecId = m_wndSecondary->GetId();
                    wxWindowList children = m_wndSecondary->GetChildren();
                    wxWindowList::iterator node = children.begin();
                    if ( node != children.end() )
                        m_wndSecId = ((wxWindow*)*node)->GetId();

                    m_wndSecondary->SetSizeHints(3,3);

                #if wxPG_CREATE_CONTROLS_HIDDEN
                    wxRect sec_rect = m_wndSecondary->GetRect();
                    sec_rect.y -= coord_adjust;

                    // Fine tuning required to fix "oversized"
                    // button disappearance bug.
                    if ( sec_rect.y < 0 )
                    {
                        sec_rect.height += sec_rect.y;
                        sec_rect.y = 0;
                    }
                    m_wndSecondary->SetSize( sec_rect );
                #endif
                    m_wndSecondary->Show();

                    SetupEventHandling(m_wndSecondary,wxPG_SUBID2);

                    // If no primary editor, focus to button to allow
                    // it to interprete ENTER etc.
                    // NOTE: Due to problems focusing away from it, this
                    //       has been disabled.
                    /*
                    if ( (flags & wxPG_SEL_FOCUS) && !m_wndPrimary )
                        m_wndSecondary->SetFocus();
                    */
                }

                if ( flags & wxPG_SEL_FOCUS )
                    m_editorFocused = 1;

            }
            else
            {
                // wxGTK atleast seems to need this (wxMSW not)
                SetFocus();
            }

            m_iFlags &= ~(wxPG_FL_VALUE_MODIFIED);

            // If it's inside collapsed section, expand parent, scroll, etc.
            // Also, if it was partially visible, scroll it into view.
            if ( !(flags & wxPG_SEL_NONVISIBLE) )
                EnsureVisible( p );

            if ( m_wndPrimary )
            {
                // Clear its background
                // (why can't this be optimized by some other drawing?)
                if ( !(m_iFlags & wxPG_FL_PRIMARY_FILLS_ENTIRE) )
                {
                    dc.SetPen(*wxTRANSPARENT_PEN);
                    dc.SetBrush( wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW) );
                    dc.DrawRectangle(splitterX+1,propY,
                                     m_width-splitterX,m_lineHeight-1);
                }

            #if wxPG_CREATE_CONTROLS_HIDDEN
                m_wndPrimary->Thaw();
            #endif
                m_wndPrimary->Show(true);
            }

            DoDrawItems( dc, p, p, NULL );
        }
    }

#if wxUSE_STATUSBAR

    //
    // Show help text in status bar.
    //   (if found and grid not embedded in manager with help box and
    //    style wxPG_EX_HELP_AS_TOOLTIPS is not used).
    //

    if ( !(GetExtraStyle() & wxPG_EX_HELP_AS_TOOLTIPS) )
    {
        wxStatusBar* statusbar = (wxStatusBar*) NULL;
        if ( !(m_iFlags & wxPG_FL_NOSTATUSBARHELP) )
        {
            wxFrame* frame = wxDynamicCast(::wxGetTopLevelParent(this),wxFrame);
            if ( frame )
                statusbar = frame->GetStatusBar();
        }

        if ( statusbar )
        {
            const wxString* pHelpString = (const wxString*) NULL;

            if ( p && p->m_dataExt )
            {
                pHelpString = &p->m_dataExt->m_helpString;
                if ( pHelpString->length() )
                {
                    // Set help box text.
                    statusbar->SetStatusText( *pHelpString );
                    m_iFlags |= wxPG_FL_STRING_IN_STATUSBAR;
                }
            }

            if ( (!pHelpString || !pHelpString->length()) &&
                 (m_iFlags & wxPG_FL_STRING_IN_STATUSBAR) )
            {
                // Clear help box - but only if it was written
                // by us at previous time.
                statusbar->SetStatusText( m_emptyString );
                m_iFlags &= ~(wxPG_FL_STRING_IN_STATUSBAR);
            }
        }
    }
#endif

    m_iFlags &= ~(wxPG_FL_IN_SELECT_PROPERTY);

    // call wx event handler (here so that it also occurs on deselection)
    SendEvent( wxEVT_PG_SELECTED, m_selected, NULL, flags );

    return true;
}

// -----------------------------------------------------------------------

// This method is not inline because it called dozens of times
// (i.e. two-arg function calls create smaller code size).
bool wxPropertyGrid::ClearSelection()
{
    return DoSelectProperty((wxPGProperty*)NULL);
}

// -----------------------------------------------------------------------
// wxPropertyGrid expand/collapse state
// -----------------------------------------------------------------------

bool wxPropertyGrid::DoCollapse( wxPGProperty* p, bool sendEvents )
{
    wxPGProperty* pwc = wxStaticCast(p, wxPGProperty);

    // If active editor was inside collapsed section, then disable it
    if ( m_selected && m_selected->IsSomeParent (p) )
    {
        if ( !ClearSelection() )
            return false;
    }

    // Store dont-center-splitter flag 'cause we need to temporarily set it
    wxUint32 old_flag = m_iFlags & wxPG_FL_DONT_CENTER_SPLITTER;
    m_iFlags |= wxPG_FL_DONT_CENTER_SPLITTER;

    int heightAdd = 0;
    bool res = m_pState->DoCollapse(pwc, &heightAdd);

    if ( res )
    {
        if ( sendEvents )
            SendEvent( wxEVT_PG_ITEM_COLLAPSED, p );

        RecalculateVirtualSize();

        CorrectEditorWidgetPosY( p, heightAdd );

        // Redraw etc. only if collapsed was visible.
        if (pwc->IsVisible() &&
            !m_frozen &&
            ( !pwc->IsCategory() || !(m_windowStyle & wxPG_HIDE_CATEGORIES) ) )
        {
            // When item is collapsed so that scrollbar would move,
            // graphics mess is about (unless we redraw everything).
            m_canvas->Refresh();
        }
    }

    // Clear dont-center-splitter flag if it wasn't set
    m_iFlags = m_iFlags & ~(wxPG_FL_DONT_CENTER_SPLITTER) | old_flag;

    return res;
}

// -----------------------------------------------------------------------

bool wxPropertyGrid::DoExpand( wxPGProperty* p, bool sendEvents )
{
    wxCHECK_MSG( p, false, wxT("invalid property id") );

    wxPGProperty* pwc = (wxPGProperty*)p;

    // Store dont-center-splitter flag 'cause we need to temporarily set it
    wxUint32 old_flag = m_iFlags & wxPG_FL_DONT_CENTER_SPLITTER;
    m_iFlags |= wxPG_FL_DONT_CENTER_SPLITTER;

    int heightAdd = 0;
    bool res = m_pState->DoExpand(pwc, &heightAdd);

    if ( res )
    {
        if ( sendEvents )
            SendEvent( wxEVT_PG_ITEM_EXPANDED, p );

        RecalculateVirtualSize();

        CorrectEditorWidgetPosY( p, heightAdd );

        // Redraw etc. only if expanded was visible.
        if ( pwc->IsVisible() && !m_frozen &&
             ( !pwc->IsCategory() || !(m_windowStyle & wxPG_HIDE_CATEGORIES) )
           )
        {
            // Redraw
        #if wxPG_REFRESH_CONTROLS_AFTER_REPAINT
            m_canvas->Refresh();
        #else
            DrawItems(pwc, NULL);
        #endif
        }
    }

    // Clear dont-center-splitter flag if it wasn't set
    m_iFlags = m_iFlags & ~(wxPG_FL_DONT_CENTER_SPLITTER) | old_flag;

    return res;
}

// -----------------------------------------------------------------------

bool wxPropertyGrid::DoHideProperty( wxPGProperty* p, bool hide )
{
    if ( m_frozen )
        return m_pState->HideProperty(p, hide);

    if ( m_selected &&
         ( m_selected == p || m_selected->IsSomeParent(p) )
       )
        {
            if ( !ClearSelection() )
                return false;
        }

    m_pState->HideProperty(p, hide);

    RedrawAllVisible();

    return true;
}


// -----------------------------------------------------------------------
// wxPropertyGrid size related methods
// -----------------------------------------------------------------------

// This is called by CalculateYs (so those calling it won't need to call this)
void wxPropertyGrid::RecalculateVirtualSize( int forceXPos )
{
    if ( m_iFlags & wxPG_FL_RECALCULATING_VIRTUAL_SIZE )
        return;

    m_iFlags |= wxPG_FL_RECALCULATING_VIRTUAL_SIZE;

    int x = m_pState->m_width;
    int y = m_pState->m_bottomY;

    int width, height;
    GetClientSize(&width,&height);

    // Now adjust virtual size.
	SetVirtualSize(x, y);

    int xAmount = 0;
    int xPos = 0;

    //
    // Adjust scrollbars
    if ( HasVirtualWidth() )
    {
        xAmount = x/wxPG_PIXELS_PER_UNIT;
        xPos = GetScrollPos( wxHORIZONTAL );
    }

    if ( forceXPos != -1 )
        xPos = forceXPos;
    // xPos too high?
    else if ( xPos > (xAmount-(width/wxPG_PIXELS_PER_UNIT)) )
        xPos = 0;

    int yAmount = (y+wxPG_PIXELS_PER_UNIT+2)/wxPG_PIXELS_PER_UNIT;
    int yPos = GetScrollPos( wxVERTICAL );

    SetScrollbars( wxPG_PIXELS_PER_UNIT, wxPG_PIXELS_PER_UNIT,
                   xAmount, yAmount, xPos, yPos, true );

    // Must re-get size now
    GetClientSize(&width,&height);

    if ( !HasVirtualWidth() )
    {
        m_pState->SetVirtualWidth(width);
        x = width;
    }

    m_width = width;
    m_height = height;

    m_canvas->SetSize( x, y );

    m_pState->CheckColumnWidths();

    if ( m_selected )
        CorrectEditorWidgetSizeX();

    m_iFlags &= ~wxPG_FL_RECALCULATING_VIRTUAL_SIZE;
}

// -----------------------------------------------------------------------

void wxPropertyGrid::OnResize( wxSizeEvent& event )
{
    if ( !(m_iFlags & wxPG_FL_INITIALIZED) )
        return;

    int width, height;
    GetClientSize(&width,&height);

#if __INTENSE_DEBUGGING__
    wxLogDebug(wxT("wxPropertyGrid::OnResize ( %i, %i )"),width,height);
#endif

    m_width = width;
    m_height = height;

    m_visPropArray.SetCount((height/m_lineHeight)+10);

#if wxPG_DOUBLE_BUFFER
    if ( !(GetExtraStyle() & wxPG_EX_NATIVE_DOUBLE_BUFFERING) )
    {
        int dblh = (m_lineHeight*2);
        if ( !m_doubleBuffer )
        {
            // Create double buffer bitmap to draw on, if none
            int w = (width>250)?width:250;
            int h = height + dblh;
            h = (h>400)?h:400;
            m_doubleBuffer = new wxBitmap( w, h );
        }
        else
        {
            int w = m_doubleBuffer->GetWidth();
            int h = m_doubleBuffer->GetHeight();

            // Double buffer must be large enough
            if ( w < width || h < (height+dblh) )
            {
                if ( w < width ) w = width;
                if ( h < (height+dblh) ) h = height + dblh;
                delete m_doubleBuffer;
                m_doubleBuffer = new wxBitmap( w, h );
            }
        }
    }

#endif

    m_pState->OnClientWidthChange( width, event.GetSize().x - m_ncWidth );
    m_ncWidth = event.GetSize().x;

    if ( !m_frozen )
    {
        if ( m_pState->m_itemsAdded )
            PrepareAfterItemsAdded();
        else
            // Without this, virtual size (atleast under wxGTK) will be skewed
            RecalculateVirtualSize();

        m_canvas->Refresh();
    }
}

// -----------------------------------------------------------------------

void wxPropertyGrid::SetVirtualWidth( int width )
{
    if ( width == -1 )
    {
        // Disable virtual width
        width = GetClientSize().x;
        ClearInternalFlag(wxPG_FL_HAS_VIRTUAL_WIDTH);
    }
    else
    {
        // Enable virtual width
        SetInternalFlag(wxPG_FL_HAS_VIRTUAL_WIDTH);
    }
    m_pState->SetVirtualWidth( width );
}

// -----------------------------------------------------------------------

void wxPropertyGridState::SetVirtualWidth( int width )
{
    wxASSERT( width >= 0 );
    wxPropertyGrid* pg = GetGrid();
    int gw = pg->GetClientSize().x;
    if ( width < gw )
        width = gw;

    m_width = width;
}

// -----------------------------------------------------------------------

void wxPropertyGridState::OnClientWidthChange( int newWidth, int widthChange )
{
    wxPropertyGrid* pg = GetGrid();

    if ( pg->HasVirtualWidth() )
    {
        if ( m_width < newWidth )
            SetVirtualWidth( newWidth );
    }
    else
    {
        SetVirtualWidth( newWidth );

        if ( GetColumnCount() == 2 &&
             pg->HasFlag( wxPG_SPLITTER_AUTO_CENTER ) &&
             widthChange )
        {
            int newFullWidth = pg->GetSize().x;
            float centerX = float(newFullWidth) * 0.5;

            float splitterX = m_fSplitterX + (float(widthChange) * 0.5);

            float deviation = fabs(centerX - splitterX);
            //wxLogDebug(wxT("deviation: %.1f"),deviation);

            // If deviated too far from the center, reset it
            if ( deviation > 30.0 )
                splitterX = centerX;

            DoSetSplitterPosition( (int)splitterX );

            m_fSplitterX = splitterX; // needed to retain accuracy
        }
        else if ( !(GetGrid()->GetInternalFlags() & wxPG_FL_SPLITTER_PRE_SET) )
        {
            long timeSinceCreation = (::wxGetLocalTimeMillis() - GetGrid()->m_timeCreated).ToLong();

            if ( m_properties->GetCount() || timeSinceCreation > 750 )
            {
                SetSplitterLeft( false );
            }
            else
            {
                DoSetSplitterPosition( newWidth / 2 );
                GetGrid()->ClearInternalFlag(wxPG_FL_SPLITTER_PRE_SET);
            }
        }
    }

    CheckColumnWidths();
}

// -----------------------------------------------------------------------
// wxPropertyGrid mouse event handling
// -----------------------------------------------------------------------

// selFlags uses same values DoSelectProperty's flags
// Returns true if event was vetoed.
bool wxPropertyGrid::SendEvent( int eventType, wxPGProperty* p, wxVariant* pValue, unsigned int WXUNUSED(selFlags) )
{
    // Send property grid event of specific type and with specific property
    wxPropertyGridEvent evt( eventType, m_eventObject->GetId() );
    evt.SetPropertyGrid(this);
    evt.SetEventObject(m_eventObject);
    evt.SetProperty(p);
    if ( pValue )
    {
        evt.SetCanVeto(true);
        evt.SetValue(*pValue);
    }
    wxEvtHandler* evtHandler = m_eventObject->GetEventHandler();

    evtHandler->ProcessEvent(evt);

    return evt.WasVetoed();
}

// -----------------------------------------------------------------------

// Return false if should be skipped
bool wxPropertyGrid::HandleMouseClick( int x, unsigned int y, wxMouseEvent &event )
{
    bool res = true;

#if __MOUSE_DEBUGGING__
    wxLogDebug( wxT("  \\--> HandleMouseClick") );
#endif

    // Need to set focus?
    if ( !(m_iFlags & wxPG_FL_FOCUSED) )
    {
        m_canvas->SetFocus();
    }

    wxPropertyGridState* state = m_pState;
    int splitterHit;
    int splitterHitOffset;
    int columnHit = state->HitTestH( x, &splitterHit, &splitterHitOffset );

    wxPGProperty* p = DoGetItemAtY(y);

    if ( p )
    {
        int depth = (int)p->GetDepth() - 1;

        int marginEnds = m_marginWidth + ( depth * m_subgroup_extramargin );

        if ( x >= marginEnds )
        {
            // Outside margin.

            if ( p->IsCategory() )
            {
                // This is category.
                wxPropertyCategory* pwc = (wxPropertyCategory*)p;

                int textX = m_marginWidth + ((unsigned int)((pwc->m_depth-1)*m_subgroup_extramargin));

                // Expand, collapse, activate etc. if click on text or left of splitter.
                if ( x >= textX
                     &&
                     ( x < (textX+pwc->GetTextExtent(this, m_captionFont)+(wxPG_CAPRECTXMARGIN*2)) ||
                       columnHit == 0
                     )
                    )
                {
                    if ( !DoSelectProperty( p ) )
                        return res;

                    // On double-click, expand/collapse.
                    if ( event.ButtonDClick() && !(m_windowStyle & wxPG_HIDE_MARGIN) )
                    {
                        if ( pwc->IsExpanded() ) DoCollapse( p, true );
                        else DoExpand( p, true );
                    }
                }
            }
            else if ( splitterHit == -1 )
            {
            // Click on value.
                unsigned int selFlag = 0;
                if ( columnHit == 1 )
                {
                    m_iFlags |= wxPG_FL_ACTIVATION_BY_CLICK;
                    selFlag = wxPG_SEL_FOCUS;
                }
                if ( !DoSelectProperty( p, selFlag ) )
                    return res;

                m_iFlags &= ~(wxPG_FL_ACTIVATION_BY_CLICK);

                if ( p->GetChildCount() && !p->IsCategory() )
                    // On double-click, expand/collapse.
                    if ( event.ButtonDClick() && !(m_windowStyle & wxPG_HIDE_MARGIN) )
                    {
                        wxPGProperty* pwc = (wxPGProperty*)p;
                        if ( pwc->IsExpanded() ) DoCollapse( p, true );
                        else DoExpand( p, true );
                    }

                res = false;
            }
            else
            {
            // click on splitter
                if ( !(m_windowStyle & wxPG_STATIC_SPLITTER) )
                {
                    if ( event.GetEventType() == wxEVT_LEFT_DCLICK )
                    {
                        // Double-clicking the splitter causes auto-centering
                        CenterSplitter( true );
                        // TODO: Would this be more natural?
                        //   .NET grid doesn't do it but maybe we should.
                        //CustomSetCursor ( wxCURSOR_ARROW );
                    }
                    else if ( m_dragStatus == 0 )
                    {
                    //
                    // Begin draggin the splitter
                    //
                    #if __MOUSE_DEBUGGING__
                        wxLogDebug( wxT("       dragging begins at splitter + %i"),
                            (int)(x - splitterX) );
                    #endif

                        if ( m_wndPrimary )
                        {
                            // Changes must be committed here or the
                            // value won't be drawn correctly
                            if ( !CommitChangesFromEditor() )
                                return res;

                            m_wndPrimary->Show ( false );
                        }

                        BEGIN_MOUSE_CAPTURE

                        m_dragStatus = 1;
                        m_draggedSplitter = splitterHit;
                        m_dragOffset = splitterHitOffset;

                        wxPG_CLIENT_DC_INIT()

                    #if wxPG_REFRESH_CONTROLS_AFTER_REPAINT
                        // Fixes button disappearance bug
                        if ( m_wndSecondary )
                            m_wndSecondary->Show ( false );
                    #endif

                        m_startingSplitterX = x - splitterHitOffset;
                    #if wxPG_HEAVY_GFX
                    #else
                        Update(); // clear graphics mess
                        DrawSplitterDragColumn( dc, splitterX );
                        m_splitterprevdrawnx = m_splitterX;
                    #endif

                    }
                }
            }
        }
        else
        {
        // Click on margin.
            if ( p->GetChildCount() )
            {
                int nx = x + m_marginWidth - marginEnds; // Normalize x.

                if ( (nx >= m_gutterWidth && nx < (m_gutterWidth+m_iconWidth)) )
                {
                    int y2 = y % m_lineHeight;
                    if ( (y2 >= m_buttonSpacingY && y2 < (m_buttonSpacingY+m_iconHeight)) )
                    {
                        // On click on expander button, expand/collapse
                        if ( ((wxPGProperty*)p)->IsExpanded() )
                            DoCollapse( p, true );
                        else
                            DoExpand( p, true );
                    }
                }
            }
        }
    }
    return res;
}

// -----------------------------------------------------------------------

bool wxPropertyGrid::HandleMouseRightClick( int WXUNUSED(x), unsigned int WXUNUSED(y),
                                            wxMouseEvent& WXUNUSED(event) )
{
    if ( m_propHover )
    {
        // Select property here as well
        wxPGProperty* p = m_propHover;
        if ( p != m_selected )
            DoSelectProperty( p );

        // Send right click event.
        SendEvent( wxEVT_PG_RIGHT_CLICK, p );

        return true;
    }
    return false;
}

// -----------------------------------------------------------------------

bool wxPropertyGrid::HandleMouseDoubleClick( int WXUNUSED(x), unsigned int WXUNUSED(y),
                                             wxMouseEvent& WXUNUSED(event) )
{
    if ( m_propHover )
    {
        // Select property here as well
        wxPGProperty* p = m_propHover;

        if ( p != m_selected )
            DoSelectProperty( p );

        // Send double-click event.
        SendEvent( wxEVT_PG_DOUBLE_CLICK, m_propHover );

        return true;
    }
    return false;
}

// -----------------------------------------------------------------------

#if wxPG_SUPPORT_TOOLTIPS

void wxPropertyGrid::SetToolTip( const wxString& tipString )
{
    if ( tipString.length() )
    {
        m_canvas->SetToolTip(tipString);
    }
    else
    {
    #if wxPG_ALLOW_EMPTY_TOOLTIPS
        m_canvas->SetToolTip( m_emptyString );
    #else
        m_canvas->SetToolTip( NULL );
    #endif
    }
}

#endif // #if wxPG_SUPPORT_TOOLTIPS

// -----------------------------------------------------------------------

// Return false if should be skipped
bool wxPropertyGrid::HandleMouseMove( int x, unsigned int y, wxMouseEvent &event )
{
    // Safety check (needed because mouse capturing may
    // otherwise freeze the control)
    if ( m_dragStatus > 0 && !event.Dragging() )
    {
        //wxLogDebug(wxT("MOUSE CAPTURE SAFETY RELEASE TRIGGERED"));
        HandleMouseUp(x,y,event);
    }

    wxPropertyGridState* state = m_pState;
    int splitterHit;
    int splitterHitOffset;
    int columnHit = state->HitTestH( x, &splitterHit, &splitterHitOffset );
    int splitterX = x - splitterHitOffset;

    if ( m_dragStatus > 0 )
    {
        if ( x > (m_marginWidth + wxPG_DRAG_MARGIN) &&
             x < (m_pState->m_width - wxPG_DRAG_MARGIN) )
        {

        #if wxPG_HEAVY_GFX

            int newSplitterX = x - m_dragOffset;
            int splitterX = x - splitterHitOffset;

            // Splitter redraw required?
            if ( newSplitterX != splitterX )
            {
                // Move everything
                state->DoSetSplitterPosition( newSplitterX, m_draggedSplitter, false );
                state->m_fSplitterX = (float) newSplitterX;

                if ( m_selected )
                    CorrectEditorWidgetSizeX();

                Update();
                RedrawAllVisible();
            }

        #else

            if ( x != splitterX )
            {
                wxPG_CLIENT_DC_INIT_R(false)

                if ( m_splitterprevdrawnx != -1 )
                    DrawSplitterDragColumn( dc, m_splitterprevdrawnx );

                splitterX = x;
                m_fSplitterX = (float) x;

                DrawSplitterDragColumn( dc, x );

                m_splitterprevdrawnx = x;
            }

        #endif

            m_dragStatus = 2;
        }

        return false;
    }
    else
    {

        int ih = m_lineHeight;
        int sy = y;

    #if wxPG_SUPPORT_TOOLTIPS
        wxPGProperty* prevHover = m_propHover;
        unsigned char prevSide = m_mouseSide;
    #endif
        int curPropHoverY = y - (y % ih);

        // On which item it hovers
        if ( ( !m_propHover )
             ||
             ( m_propHover && ( sy < m_propHoverY || sy >= (m_propHoverY+ih) ) )
           )
        {
            // Mouse moves on another property

            m_propHover = DoGetItemAtY(y);
            m_propHoverY = curPropHoverY;

            // Send hover event
            SendEvent( wxEVT_PG_HIGHLIGHTED, m_propHover );
        }

    #if wxPG_SUPPORT_TOOLTIPS
        // Store which side we are on
        m_mouseSide = 0;
        if ( columnHit == 1 )
            m_mouseSide = 2;
        else if ( columnHit == 0 )
            m_mouseSide = 1;

        //
        // If tooltips are enabled, show label or value as a tip
        // in case it doesn't otherwise show in full length.
        //
        if ( m_windowStyle & wxPG_TOOLTIPS )
        {
            wxToolTip* tooltip = m_canvas->GetToolTip();

            if ( m_propHover != prevHover || prevSide != m_mouseSide )
            {
                if ( m_propHover && !m_propHover->IsCategory() )
                {

                    if ( GetExtraStyle() & wxPG_EX_HELP_AS_TOOLTIPS )
                    {
                        // Show help string as a tooltip
                        wxString tipString = m_propHover->GetHelpString();

                        SetToolTip(tipString);
                    }
                    else
                    {
                        // Show cropped value string as a tooltip
                        wxString tipString;
                        int space = 0;

                        if ( m_mouseSide == 1 )
                        {
                            tipString = m_propHover->m_label;
                            space = splitterX-m_marginWidth-3;
                        }
                        else if ( m_mouseSide == 2 )
                        {
                            tipString = m_propHover->GetDisplayedString();

                            space = m_width - splitterX;
                            if ( m_propHover->m_flags & wxPG_PROP_CUSTOMIMAGE )
                                space -= wxPG_CUSTOM_IMAGE_WIDTH + wxCC_CUSTOM_IMAGE_MARGIN1 + wxCC_CUSTOM_IMAGE_MARGIN2;
                        }

                        if ( space )
                        {
                            int tw, th;
    	                    GetTextExtent( tipString, &tw, &th, 0, 0, &m_font );
                            if ( tw > space )
                            {
                                SetToolTip( tipString );
                            }
                        }
                        else
                        {
                            if ( tooltip )
                            {
                            #if wxPG_ALLOW_EMPTY_TOOLTIPS
                                m_canvas->SetToolTip( m_emptyString );
                            #else
                                m_canvas->SetToolTip( NULL );
                            #endif
                            }
                        }

                    }
                }
                else
                {
                    if ( tooltip )
                    {
                    #if wxPG_ALLOW_EMPTY_TOOLTIPS
                        m_canvas->SetToolTip( m_emptyString );
                    #else
                        m_canvas->SetToolTip( NULL );
                    #endif
                    }
                }
            }
        }
    #endif

        if ( splitterHit == -1 ||
             !m_propHover ||
             HasFlag(wxPG_STATIC_SPLITTER) )
        {
            // hovering on something else
            if ( m_curcursor != wxCURSOR_ARROW )
                CustomSetCursor( wxCURSOR_ARROW );
        }
        else
        {
            // Do not allow splitter cursor on caption items.
            // (also not if we were dragging and its started
            // outside the splitter region)

            if ( m_propHover &&
                 !m_propHover->IsCategory() &&
                 !event.Dragging() )
            {

                // hovering on splitter

                // NB: Condition disabled since MouseLeave event (from the editor control) cannot be
                //     reliably detected.
                //if ( m_curcursor != wxCURSOR_SIZEWE )
                CustomSetCursor( wxCURSOR_SIZEWE, true );

                return false;
            }
            else
            {
                // hovering on something else
                if ( m_curcursor != wxCURSOR_ARROW )
                    CustomSetCursor( wxCURSOR_ARROW );
            }
        }
    }
    return true;
}

// -----------------------------------------------------------------------

// Also handles Leaving event
bool wxPropertyGrid::HandleMouseUp( int x, unsigned int WXUNUSED(y),
                                    wxMouseEvent &WXUNUSED(event) )
{
    wxPropertyGridState* state = m_pState;
    bool res = false;

#if __MOUSE_DEBUGGING__
    wxLogDebug( wxT("  \\--> HandleMouseUp") );
#endif

    int splitterHit;
    int splitterHitOffset;
    state->HitTestH( x, &splitterHit, &splitterHitOffset );

    // No event type check - basicly calling this method should
    // just stop dragging.
    // Left up after dragged?
    if ( m_dragStatus >= 1 )
    {
    //
    // End Splitter Dragging
    //
    #if __MOUSE_DEBUGGING__
        wxLogDebug( wxT("       dragging ends") );
    #endif

        // DO NOT ENABLE FOLLOWING LINE!
        // (it is only here as a reminder to not to do it)
        //splitterX = x;

    #if wxPG_HEAVY_GFX
        //Refresh();
    #else
        DoSetSplitterPosition( -1 ); // -1 tells not to make change

        // Hack to clear-up editor graphics mess (on wxMSW, atleast)
        if ( m_selected )
            DrawItem ( m_selected );

    #endif
        // Disable splitter auto-centering
        m_iFlags |= wxPG_FL_DONT_CENTER_SPLITTER;

        // This is necessary to return cursor
        END_MOUSE_CAPTURE

        // Set back the default cursor, if necessary
        if ( splitterHit == -1 ||
             !m_propHover )
        {
            CustomSetCursor( wxCURSOR_ARROW );
        }

        m_dragStatus = 0;

        #if wxPG_HEAVY_GFX
        // Control background needs to be cleared
        if ( !(m_iFlags & wxPG_FL_PRIMARY_FILLS_ENTIRE) && m_selected )
            DrawItem( m_selected );
        #endif

        if ( m_wndPrimary )
        {
            m_wndPrimary->Show ( true );
        }

    #if wxPG_REFRESH_CONTROLS_AFTER_REPAINT
        // Fixes button disappearance bug
        if ( m_wndSecondary )
            m_wndSecondary->Show ( true );
    #endif

        // This clears the focus.
        m_editorFocused = 0;

    }
    return res;
}

// -----------------------------------------------------------------------

bool wxPropertyGrid::OnMouseCommon( wxMouseEvent& event, int* px, int* py )
{
    int splitterX = GetSplitterPosition();

    //int ux, uy;
    //CalcUnscrolledPosition( event.m_x, event.m_y, &ux, &uy );
    int ux = event.m_x;
    int uy = event.m_y;

    wxWindow* wnd = m_wndPrimary;

    // Hide popup on clicks
    if ( event.GetEventType() != wxEVT_MOTION )
        if ( wnd && wnd->IsKindOf(CLASSINFO(wxPGOwnerDrawnComboBox)) )
        {
            ((wxPGOwnerDrawnComboBox*)m_wndPrimary)->HidePopup();
        }

    //if (printmsg) wxLogDebug( wxT("On") wxT(#func) wxT("( %i, %i )"),(int)ux,(int)uy );
    wxRect r;
    if ( wnd )
        r = wnd->GetRect();
    if ( wnd == (wxWindow*) NULL || m_dragStatus ||
         (
           ux <= (splitterX + wxPG_SPLITTERX_DETECTMARGIN2) ||
           ux >= (r.x+r.width) ||
           event.m_y < r.y ||
           event.m_y >= (r.y+r.height)
         )
       )
    {
        *px = ux;
        *py = uy;
        return true;
    }
    else
    {
        if ( m_curcursor != wxCURSOR_ARROW ) CustomSetCursor ( wxCURSOR_ARROW );
    }
    return false;
}

// -----------------------------------------------------------------------

void wxPropertyGrid::OnMouseClick( wxMouseEvent &event )
{
    int x, y;
    if ( OnMouseCommon( event, &x, &y ) )
    {
        HandleMouseClick(x,y,event);
    }
    event.Skip();
}

// -----------------------------------------------------------------------

void wxPropertyGrid::OnMouseRightClick( wxMouseEvent &event )
{
    int x, y;
    CalcUnscrolledPosition( event.m_x, event.m_y, &x, &y );
    HandleMouseRightClick(x,y,event);
    event.Skip();
}

// -----------------------------------------------------------------------

void wxPropertyGrid::OnMouseDoubleClick( wxMouseEvent &event )
{
    // Always run standard mouse-down handler as well
    OnMouseClick(event);

    int x, y;
    CalcUnscrolledPosition( event.m_x, event.m_y, &x, &y );
    HandleMouseDoubleClick(x,y,event);
    event.Skip();
}

// -----------------------------------------------------------------------

void wxPropertyGrid::OnMouseMove( wxMouseEvent &event )
{
    int x, y;
    if ( OnMouseCommon( event, &x, &y ) )
    {
        HandleMouseMove(x,y,event);
    }
    event.Skip();
}

// -----------------------------------------------------------------------

void wxPropertyGrid::OnMouseMoveBottom( wxMouseEvent& WXUNUSED(event) )
{
    // Called when mouse moves in the empty space below the properties.
    CustomSetCursor( wxCURSOR_ARROW );
}

// -----------------------------------------------------------------------

void wxPropertyGrid::OnMouseUp( wxMouseEvent &event )
{
    int x, y;
    if ( OnMouseCommon( event, &x, &y ) )
    {
        HandleMouseUp(x,y,event);
    }
    event.Skip();
}

// -----------------------------------------------------------------------

void wxPropertyGrid::OnMouseEntry( wxMouseEvent &event )
{
    // This may get called from child control as well, so event's
    // mouse position cannot be relied on.
    //int x = event.m_x;
    //int y = event.m_y;

    if ( event.Entering() )
    {
        if ( !(m_iFlags & wxPG_FL_MOUSE_INSIDE) )
        {
        #if __MOUSE_DEBUGGING__
            wxLogDebug(wxT("Mouse Enters Window"));
        #endif
            //SetCursor ( *wxSTANDARD_CURSOR );
            // TODO: Fix this (detect parent and only do
            //   cursor trick if it is a manager).
            wxASSERT( GetParent() );
            GetParent()->SetCursor(wxNullCursor);

            m_iFlags |= wxPG_FL_MOUSE_INSIDE;
            //if ( m_wndPrimary ) m_wndPrimary->Show ( true );
        }
        else
            GetParent()->SetCursor(wxNullCursor);
    }
    else if ( event.Leaving() )
    {
        // Without this, wxSpinCtrl editor will sometimes have wrong cursor
        m_canvas->SetCursor( wxNullCursor );

        // Get real cursor position
        wxPoint pt = ScreenToClient(::wxGetMousePosition());

        if ( ( pt.x <= 0 || pt.y <= 0 || pt.x >= m_width || pt.y >= m_height ) )
        {
            //if ( CommitChangesFromEditor() )
            {

                if ( (m_iFlags & wxPG_FL_MOUSE_INSIDE) )
                {
                #if __MOUSE_DEBUGGING__
                    wxLogDebug(wxT("Mouse Leaves Window"));
                #endif
                    m_iFlags &= ~(wxPG_FL_MOUSE_INSIDE);
                    //if ( m_wndPrimary ) m_wndPrimary->Show ( false );
                }

                if ( m_dragStatus )
                    wxPropertyGrid::HandleMouseUp ( -1, 10000, event );

            }
        }
        else
        {
        /*#if wxPG_NO_CHILD_EVT_MOTION
            // cursor must be reset because EVT_MOTION handler is not there to do it
            if ( m_curcursor != wxCURSOR_ARROW ) CustomSetCursor ( wxCURSOR_ARROW );
        #endif*/
        }
    }

    event.Skip();
}

// -----------------------------------------------------------------------

// Common code used by various OnMouseXXXChild methods.
bool wxPropertyGrid::OnMouseChildCommon( wxMouseEvent &event, int* px, int *py )
{
    wxWindow* topCtrlWnd = (wxWindow*)event.GetEventObject();
    wxASSERT( topCtrlWnd );
    int x, y;
    event.GetPosition(&x,&y);

    AdjustPosForClipperWindow( topCtrlWnd, &x, &y );

    int splitterX = GetSplitterPosition();

    wxRect r = topCtrlWnd->GetRect();
    if ( !m_dragStatus &&
         x > (splitterX-r.x+wxPG_SPLITTERX_DETECTMARGIN2) &&
         y >= 0 && y < r.height \
       )
    {
        if ( m_curcursor != wxCURSOR_ARROW ) CustomSetCursor ( wxCURSOR_ARROW );
        event.Skip();
    }
    else
    {
        CalcUnscrolledPosition( event.m_x + r.x, event.m_y + r.y, \
            px, py );
        return true;
    }
    return false;
}

void wxPropertyGrid::OnMouseClickChild( wxMouseEvent &event )
{
    int x,y;
    if ( OnMouseChildCommon(event,&x,&y) )
    {
        bool res = HandleMouseClick(x,y,event);
        if ( !res ) event.Skip();
    }
}

void wxPropertyGrid::OnMouseRightClickChild( wxMouseEvent &event )
{
    int x,y;
    wxASSERT( m_wndPrimary );
    // These coords may not be exact (about +-2),
    // but that should not matter (right click is about item, not position).
    wxPoint pt = m_wndPrimary->GetPosition();
    CalcUnscrolledPosition( event.m_x + pt.x, event.m_y + pt.y, &x, &y );
    wxASSERT( m_selected );
    m_propHover = m_selected;
    bool res = HandleMouseRightClick(x,y,event);
    if ( !res ) event.Skip();
}

void wxPropertyGrid::OnMouseMoveChild( wxMouseEvent &event )
{
    int x,y;
    if ( OnMouseChildCommon(event,&x,&y) )
    {
        bool res = HandleMouseMove(x,y,event);
        if ( !res ) event.Skip();
    }
}

void wxPropertyGrid::OnMouseUpChild( wxMouseEvent &event )
{
    int x,y;
    if ( OnMouseChildCommon(event,&x,&y) )
    {
        bool res = HandleMouseUp(x,y,event);
        if ( !res ) event.Skip();
    }
}

// -----------------------------------------------------------------------
// wxPropertyGrid keyboard event handling
// -----------------------------------------------------------------------

void wxPropertyGrid::SendNavigationKeyEvent( int dir )
{
    wxNavigationKeyEvent evt;
    evt.SetFlags(wxNavigationKeyEvent::FromTab|
                 (dir?wxNavigationKeyEvent::IsForward:
                      wxNavigationKeyEvent::IsBackward));
    evt.SetEventObject(this);
    m_canvas->GetEventHandler()->AddPendingEvent(evt);
}

void wxPropertyGrid::HandleKeyEvent(wxKeyEvent &event)
{

    //
    // Handles key event when editor control is not focused.
    //

#if __INTENSE_DEBUGGING__
    wxLogDebug( wxT("wxPropertyGrid::HandleKeyEvent(%i)"),(int)event.GetKeyCode() );
#endif

    wxASSERT( !m_frozen );
    if ( m_frozen )
        return;

    // Travelsal between items, collapsing/expanding, etc.
    int keycode = event.GetKeyCode();

    if ( keycode == WXK_TAB )
    {
        SendNavigationKeyEvent( event.ShiftDown()?0:1 );
        return;
    }

    // Ignore Alt and Control when they are down alone
    if ( keycode == WXK_ALT ||
         keycode == WXK_CONTROL )
    {
        event.Skip();
        return;
    }

    if ( m_selected )
    {

        // Show dialog?
        if ( ButtonTriggerKeyTest(event) )
            return;

        wxPGProperty* p = m_selected;

        int selectDir = -2;

        if ( p->GetChildCount() &&
             !(p->m_flags & wxPG_PROP_DISABLED)
           )
        {
            if ( keycode == WXK_LEFT )
            {
                if ( (m_windowStyle & wxPG_HIDE_MARGIN) || Collapse ( p ) )
                    keycode = 0;
            }
            else if ( keycode == WXK_RIGHT )
            {
                if ( (m_windowStyle & wxPG_HIDE_MARGIN) || Expand ( p ) )
                    keycode = 0;
            }
        }

        if ( keycode )
        {
            if ( keycode == WXK_UP || keycode == WXK_LEFT )
            {
                selectDir = -1;
            }
            else if ( keycode == WXK_DOWN || keycode == WXK_RIGHT )
            {
                selectDir = 1;
            }
            else
            {
                event.Skip();
            }

        }

        if ( selectDir >= -1 )
        {
            p = wxPropertyGridIterator::OneStep( m_pState, wxPG_ITERATE_VISIBLE, p, selectDir );
            if ( p )
                DoSelectProperty(p);
        }

    }
    else
    {
        // If nothing was selected, select the first item now
        // (or navigate out of tab).
        if ( keycode != WXK_ESCAPE )
        {
            wxPGProperty* p = wxPropertyContainerMethods::GetFirst();
            if ( p ) DoSelectProperty(p);
        }
    }
}

// -----------------------------------------------------------------------

// Potentially handles a keyboard event for editor controls.
// Returns false if event should *not* be skipped (on true it can
// be optionally skipped).
// Basicly, false means that SelectProperty was called (or was about
// to be called, if canDestroy was false).
bool wxPropertyGrid::HandleChildKey( wxKeyEvent& event, bool canDestroy )
{
    int keycode = event.GetKeyCode();
    bool res = true;

#if __INTENSE_DEBUGGING__
    wxLogDebug( wxT("wxPropertyGrid::HandleChildKey(%i)"),(int)event.GetKeyCode() );
#endif

    // Unfocus?
    if ( keycode == WXK_ESCAPE )
    {
        // Esc cancels any changes
        EditorsValueWasNotModified();

        wxPGProperty* p = m_selected;

        res = false;

        if ( canDestroy )
        {
            DoSelectProperty( (wxPGProperty*)NULL, wxPG_SEL_NOVALIDATE );
            DoSelectProperty( p );
        }

    }

    return res;
}

// -----------------------------------------------------------------------

void wxPropertyGrid::OnKey( wxKeyEvent &event )
{

    //
    // Events to editor controls should get relayed here.
    //
    wxWindow* focused = wxWindow::FindFocus();

    wxWindow* primaryCtrl = GetEditorControl();

    if ( primaryCtrl &&
         (focused==primaryCtrl
          || m_editorFocused) )
    {
        // Child key must be processed here, since it can
        // destroy the control which is referred by its own
        // event handling.
        HandleChildKey( event, true );
    }
    else
        HandleKeyEvent( event );
}

// -----------------------------------------------------------------------

void wxPropertyGrid::OnKeyUp(wxKeyEvent &event)
{
    m_keyComboConsumed = 0;

    event.Skip();
}

// -----------------------------------------------------------------------

void wxPropertyGrid::OnNavigationKey( wxNavigationKeyEvent& event )
{
    // Ignore events that occur very close to focus set
    if ( m_iFlags & wxPG_FL_IGNORE_NEXT_NAVKEY )
    {
        m_iFlags &= ~(wxPG_FL_IGNORE_NEXT_NAVKEY);
        event.Skip();
        return;
    }

    wxPGProperty* next = (wxPGProperty*) NULL;

    int dir = event.GetDirection()?1:-1;

    if ( m_selected )
    {
        if ( dir == 1 && (m_wndPrimary || m_wndSecondary) )
        {
            wxWindow* focused = wxWindow::FindFocus();

            wxWindow* wndToCheck = GetEditorControl();

            // ODComboBox focus goes to its text ctrl, so we need to use it instead
            if ( wndToCheck && wndToCheck->IsKindOf(CLASSINFO(wxPGOwnerDrawnComboBox)) )
            {
                wxTextCtrl* comboTextCtrl = ((wxPGOwnerDrawnComboBox*)wndToCheck)->GetTextCtrl();
                if ( comboTextCtrl )
                    wndToCheck = comboTextCtrl;
            }

            /*
            // Because of problems navigating from wxButton, do not go to it.
            if ( !wndToCheck )
            {
                // No primary, use secondary
                wndToCheck = m_wndSecondary;
            }
            // If it has editor button, focus to it after the primary editor.
            // NB: Doesn't work since wxButton on wxMSW doesn't seem to propagate
            //     key events (yes, I'm using wxWANTS_CHARS with it, and yes I
            //     have somewhat debugged in window.cpp itself).
            else if ( focused == wndToCheck &&
                      m_wndSecondary &&
                      !(GetExtraStyle() & wxPG_EX_NO_TAB_TO_BUTTON) )
            {
                wndToCheck = m_wndSecondary;
                wxLogDebug(wxT("Exp1"));
            }
            */

            if ( focused != wndToCheck &&
                 wndToCheck )
            {
                wndToCheck->SetFocus();

                // Select all text in wxTextCtrl etc.
                if ( m_wndPrimary && wndToCheck == m_wndPrimary )
                    m_selected->GetEditorClass()->OnFocus(m_selected,wndToCheck);

                m_editorFocused = 1;
                next = m_selected;
            }
        }

        if ( !next )
        {
            next = wxPropertyGridIterator::OneStep(m_pState, wxPG_ITERATE_VISIBLE, m_selected, dir);

            if ( next )
            {
                // This allows preventing NavigateOut to occur
                DoSelectProperty( next, wxPG_SEL_FOCUS );
            }
        }
    }

    if ( !next )
        event.Skip();
}

// -----------------------------------------------------------------------

bool wxPropertyGrid::ButtonTriggerKeyTest( wxKeyEvent &event )
{
    int keycode = event.GetKeyCode();

    // Does the keycode trigger button?
    if ( keycode == m_pushButKeyCode &&
         m_wndSecondary &&
         (!m_pushButKeyCodeNeedsAlt || event.AltDown()) &&
         (!m_pushButKeyCodeNeedsCtrl || event.ControlDown()) )
    {
        m_keyComboConsumed = 1;

        wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED,m_wndSecondary->GetId());
        GetEventHandler()->AddPendingEvent(evt);
        return true;
    }

    return false;
}

// -----------------------------------------------------------------------

void wxPropertyGrid::OnChildKeyDown( wxKeyEvent &event )
{
    int keycode = event.GetKeyCode();

    // Ignore Alt and Control when they are down alone
    if ( keycode == WXK_ALT ||
         keycode == WXK_CONTROL )
    {
        event.Skip();
        return;
    }

    if ( ButtonTriggerKeyTest(event) )
        return;

    // Since event handling may destroy the control which
    // triggered this event, we need to send it separately
    // to the wxPropertyGrid itself. Also, to allow pushed
    // event handler to grab ENTER, ESC and such, this
    // has been changed to add all keys as events.
    if ( HandleChildKey(event,false) == true )
        event.Skip();

    GetEventHandler()->AddPendingEvent(event);
}

void wxPropertyGrid::OnChildKeyUp( wxKeyEvent &event )
{
    m_keyComboConsumed = 0;

    GetEventHandler()->AddPendingEvent(event);

    event.Skip();
}

// -----------------------------------------------------------------------
// wxPropertyGrid miscellaneous event handling
// -----------------------------------------------------------------------

void wxPropertyGrid::OnIdle( wxIdleEvent& WXUNUSED(event) )
{
    //
    // Check if the focus is in this control or one of its children
    wxWindow* newFocused = wxWindow::FindFocus();

    if ( newFocused != m_curFocused )
        HandleFocusChange( newFocused );
}

// Called by focus event handlers. newFocused is the window that becomes focused.
void wxPropertyGrid::HandleFocusChange( wxWindow* newFocused )
{
    unsigned int oldFlags = m_iFlags;

    //wxLogDebug(wxT("HandleFocusChange: %s"),newFocused?newFocused->GetClassInfo()->GetClassName():wxT("NULL"));

    m_iFlags &= ~(wxPG_FL_FOCUSED);

    wxWindow* parent = newFocused;

    // This must be one of nextFocus' parents.
    while ( parent )
    {
        // Use m_eventObject, which is either wxPropertyGrid or
        // wxPropertyGridManager, as appropriate.
        if ( parent == m_eventObject )
        {
            m_iFlags |= wxPG_FL_FOCUSED;
            break;
        }
        parent = parent->GetParent();
    }

    m_curFocused = newFocused;

    if ( (m_iFlags & wxPG_FL_FOCUSED) !=
         (oldFlags & wxPG_FL_FOCUSED) )
    {
        // On each focus kill, mark the next nav key event
        // to be ignored (can't do on set focus since the
        // event would occur before it).
        if ( !(m_iFlags & wxPG_FL_FOCUSED) )
        {
            m_iFlags |= wxPG_FL_IGNORE_NEXT_NAVKEY;

            // Need to store changed value
            CommitChangesFromEditor();
        }
        else
        {
            /*
            //
            // Preliminary code for tab-order respecting
            // tab-traversal (but should be moved to
            // OnNav handler)
            //
            wxWindow* prevFocus = event.GetWindow();
            wxWindow* useThis = this;
            if ( m_iFlags & wxPG_FL_IN_MANAGER )
                useThis = GetParent();

            if ( prevFocus &&
                 prevFocus->GetParent() == useThis->GetParent() )
            {
                wxList& children = useThis->GetParent()->GetChildren();

                wxNode* node = children.Find(prevFocus);

                if ( node->GetNext() &&
                     useThis == node->GetNext()->GetData() )
                    DoSelectProperty(GetFirst());
                else if ( node->GetPrevious () &&
                          useThis == node->GetPrevious()->GetData() )
                    DoSelectProperty(GetLastProperty());

            }
            */

            m_iFlags &= ~(wxPG_FL_IGNORE_NEXT_NAVKEY);
        }

        // Redraw selected
        if ( m_selected && (m_iFlags & wxPG_FL_INITIALIZED) )
            DrawItem( m_selected );
    }
}

void wxPropertyGrid::OnFocusEvent( wxFocusEvent& event )
{
    if ( event.GetEventType() == wxEVT_SET_FOCUS )
        HandleFocusChange((wxWindow*)event.GetEventObject());
    // Line changed to "else" when applying patch #1675902
    //else if ( event.GetWindow() )
    else
        HandleFocusChange(event.GetWindow());

    event.Skip();
}

// -----------------------------------------------------------------------

void wxPropertyGrid::OnChildFocusEvent( wxChildFocusEvent& event )
{
    HandleFocusChange((wxWindow*)event.GetEventObject());
    event.Skip();
}

// -----------------------------------------------------------------------

void wxPropertyGrid::OnScrollEvent( wxScrollWinEvent &event )
{
    m_iFlags |= wxPG_FL_SCROLLED;

    event.Skip();
}

// -----------------------------------------------------------------------

void wxPropertyGrid::OnCaptureChange( wxMouseCaptureChangedEvent& WXUNUSED(event) )
{
    if ( m_iFlags & wxPG_FL_MOUSE_CAPTURED )
    {
    #if __MOUSE_DEBUGGING__
        wxLogDebug( wxT("wxPropertyGrid: mouse capture lost") );
    #endif
        m_iFlags &= ~(wxPG_FL_MOUSE_CAPTURED);
    }
}

// -----------------------------------------------------------------------
// Property text-based storage
// -----------------------------------------------------------------------

void wxPGProperty::SetAttributes( const wxPGAttributeStorage& attributes )
{
    wxPGAttributeStorage::const_iterator it = attributes.StartIteration();
    wxVariant variant;

    while ( attributes.GetNext(it, variant) )
        SetAttribute( variant.GetName(), variant );
}

// -----------------------------------------------------------------------

// Returns name of property without 'Property' at the end, and 'wx'
// in the beginning (if any).
wxString wxPropertyContainerMethods::GetPropertyShortClassName( wxPGPropArg id )
{
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(wxEmptyString)

    if ( !p->IsCategory() )
    {
        const wxChar* src = p->GetClassName();
        wxString s;
        if ( src[0] == wxT('w') && src[1] == wxT('x') )
            s = &src[2];
        else
            s = src;
        wxASSERT( (((int)s.length())-8) > 0 );
        s.Truncate(s.length()-8);
        //s.LowerCase();
        return s;
    }
    return wxT("Category");
}

wxPGProperty* wxPropertyContainerMethods::GetPropertyByNameA( wxPGPropNameStr name ) const
{
    wxPGProperty* p = GetPropertyByName(name);
    wxASSERT_MSG(p,wxString::Format(wxT("no property with name '%s'"),name.c_str()));
    return p;
}

// ----------------------------------------------------------------------------

void wxPropertyContainerMethods::PropertiesToNames( wxArrayString* names,
                                                    const wxArrayPGProperty& properties ) const
{
    unsigned int i;
    for ( i=0; i<properties.size(); i++ )
        names->Add( properties[i]->GetName() );
}

// ----------------------------------------------------------------------------

void wxPropertyContainerMethods::NamesToProperties( wxArrayPGProperty* properties,
                                                    const wxArrayString& names ) const
{
    unsigned int i;
    for ( i=0; i<names.size(); i++ )
    {
        wxPGProperty* p = GetPropertyByName(names[i]);
        if ( p )
            properties->push_back(p);
    }
}

// ----------------------------------------------------------------------------
// VariantDatas
// ----------------------------------------------------------------------------

#if !wxCHECK_VERSION(2,9,0)
IMPLEMENT_ABSTRACT_CLASS(wxPGVariantData, wxVariantData)
#endif

WX_PG_IMPLEMENT_VARIANT_DATA(wxPGVariantDataPoint, wxPoint)
WX_PG_IMPLEMENT_VARIANT_DATA(wxPGVariantDataSize, wxSize)
WX_PG_IMPLEMENT_VARIANT_DATA(wxPGVariantDataArrayInt, wxArrayInt)
WX_PG_IMPLEMENT_VARIANT_DATA(wxPGVariantDataLongLong, wxLongLong)
WX_PG_IMPLEMENT_VARIANT_DATA(wxPGVariantDataULongLong, wxULongLong)

WX_PG_IMPLEMENT_WXOBJECT_VARIANT_DATA(wxPGVariantDataFont, wxFont)
#if !wxCHECK_VERSION(2,8,0)
    WX_PG_IMPLEMENT_WXOBJECT_VARIANT_DATA(wxPGVariantDataColour, wxColour)
#endif

#ifdef __WXPYTHON__
    WX_PG_IMPLEMENT_PTR_VARIANT_DATA(wxPGVariantDataPyObject, PyObject, Py_None)
#endif


wxObject* wxPG_VariantToWxObject( const wxVariant& variant, wxClassInfo* classInfo )
{
    if ( !variant.IsValueKindOf(classInfo) )
        return (wxObject*) NULL;

    wxVariantData* vdata = variant.GetData();

    wxPGVariantData* pgvdata = wxDynamicCastVariantData(vdata, wxPGVariantData);
    if ( pgvdata )
         return (wxObject*) pgvdata->GetValuePtr();

    if ( wxPGIsVariantClassInfo(wxPGVariantDataGetClassInfo(vdata), wxobject) )
#if wxCHECK_VERSION(2,8,0)
        return variant.GetWxObjectPtr();
#else
        return ((wxVariant*)(&variant))->GetWxObjectPtr();
#endif

    return (wxObject*) NULL;
}

// -----------------------------------------------------------------------
// wxVariant helpers
// -----------------------------------------------------------------------

long wxPGVariantToInt( const wxVariant& variant )
{
    if ( wxPGIsVariantType(variant, long) )
        return variant.GetLong();

#if wxCHECK_VERSION(2,9,0)
    if ( typeid(*variant.GetData()) == typeid(wxPGVariantDataLongLong) )
#else
    if ( wxPGVariantDataGetClassInfo(variant.GetData()) == &wxPGVariantDataLongLong::ms_classInfo )
#endif
    {
        wxLongLong ll = ((const wxPGVariantDataLongLong&)variant).GetValue();
        if ( ll >= LONG_MAX )
            return LONG_MAX;
        else if ( ll <= LONG_MIN )
            return LONG_MIN;
        return ll.ToLong();
    }

    long l = 0;

    if ( wxPGIsVariantType(variant, string) )
        variant.GetString().ToLong(&l, 0);

    return l;
}

// -----------------------------------------------------------------------
// Editor class specific.

// noDefCheck = true prevents infinite recursion.
wxPGEditor* wxPropertyGrid::RegisterEditorClass( wxPGEditor* editorclass,
                                                 const wxString& name,
                                                 bool noDefCheck )
{
    wxASSERT( editorclass );

    WX_PG_GLOBALS_LOCKER()

    if ( !noDefCheck && wxPGGlobalVars->m_mapEditorClasses.empty() )
        RegisterDefaultEditors();

    wxPGGlobalVars->m_mapEditorClasses[name] = (void*)editorclass;

    return editorclass;
}

// Registers all default editor classes
void wxPropertyGrid::RegisterDefaultEditors()
{
    wxPGRegisterDefaultEditorClass( TextCtrl );
    wxPGRegisterDefaultEditorClass( Choice );
    wxPGRegisterDefaultEditorClass( ComboBox );
    wxPGRegisterDefaultEditorClass( TextCtrlAndButton );
#if wxPG_INCLUDE_CHECKBOX
    wxPGRegisterDefaultEditorClass( CheckBox );
#endif
    wxPGRegisterDefaultEditorClass( ChoiceAndButton );

    // Register SpinCtrl etc. editors before use
    RegisterAdditionalEditors();
}

wxPGEditor* wxPropertyContainerMethods::GetEditorByName( const wxString& editor_name )
{
    wxPGEditor* editor = (wxPGEditor*) wxPGGlobalVars->m_mapEditorClasses[editor_name];
    wxASSERT_MSG( editor,
                  wxT("unregistered editor name") );
    return editor;
}

// -----------------------------------------------------------------------
// wxPGAttributeStorage
// -----------------------------------------------------------------------

wxPGAttributeStorage::wxPGAttributeStorage()
{
}

wxPGAttributeStorage::~wxPGAttributeStorage()
{
    wxPGHashMapS2P::iterator it;

    for ( it = m_map.begin(); it != m_map.end(); it++ )
    {
        wxVariantData* data = (wxVariantData*) it->second;
        data->DecRef();
    }
}

void wxPGAttributeStorage::Set( const wxString& name, const wxVariant& value )
{
    wxVariantData* data = value.GetData();

    // Free old, if any
    wxPGHashMapS2P::iterator it = m_map.find(name);
    if ( it != m_map.end() )
        ((wxVariantData*)it->second)->DecRef();

    if ( data )
        data->IncRef();

    m_map[name] = data;
}

// -----------------------------------------------------------------------
// wxPGStringTokenizer
//   Needed to handle C-style string lists (e.g. "str1" "str2")
// -----------------------------------------------------------------------

wxPGStringTokenizer::wxPGStringTokenizer( const wxString& str, wxChar delimeter )
    : m_str(&str), m_curPos(str.begin()), m_delimeter(delimeter)
{
}

wxPGStringTokenizer::~wxPGStringTokenizer()
{
}

bool wxPGStringTokenizer::HasMoreTokens()
{
    const wxString& str = *m_str;

    //wxASSERT_MSG( m_curPos != str.end(), wxT("Do not call wxPGStringTokenizer methods after HasMoreTokens has returned false."));

    wxString::const_iterator i = m_curPos;

    wxUniChar delim = m_delimeter;
    wxUniChar a;
    wxUniChar prev_a = wxT('\0');

    bool inToken = false;

    while ( i != str.end() )
    {
        a = wxPGGetIterChar(str, i);

        if ( !inToken )
        {
            if ( a == delim )
            {
                inToken = true;
                m_readyToken.clear();
            }
        }
        else
        {
            if ( prev_a != wxT('\\') )
            {
                if ( a != delim )
                {
                    if ( a != wxT('\\') )
                        m_readyToken << a;
                }
                else
                {
                    //wxLogDebug(m_readyToken);
                    i++;
                    m_curPos = i;
                    return true;
                }
                prev_a = a;
            }
            else
            {
                m_readyToken << a;
                prev_a = wxT('\0');
            }
        }
        i++;
    }

    m_curPos = str.end();

    if ( inToken )
        return true;

    return false;
}

wxString wxPGStringTokenizer::GetNextToken()
{
    //wxASSERT_MSG( m_curPos != m_str->end(), wxT("Do not call wxPGStringTokenizer methods after HasMoreTokens has returned false."));
    return m_readyToken;
}

// -----------------------------------------------------------------------
// wxPGChoiceEntry
// -----------------------------------------------------------------------

wxPGChoiceEntry::wxPGChoiceEntry()
    : wxPGCell(), m_value(wxPG_INVALID_VALUE)
{
}

wxPGChoiceEntry::wxPGChoiceEntry( const wxPGChoiceEntry& entry )
    : wxPGCell( entry.GetText(), entry.GetBitmap(),
        entry.GetFgCol(), entry.GetBgCol() ), m_value(entry.GetValue())
{
}

// -----------------------------------------------------------------------
// wxPGChoicesData
// -----------------------------------------------------------------------

wxPGChoicesData::wxPGChoicesData()
{
    m_refCount = 1;
}

wxPGChoicesData::~wxPGChoicesData()
{
    unsigned int i;

    for ( i=0; i<m_items.size(); i++ )
    {
        delete Item(i);
    }
}

void wxPGChoicesData::CopyDataFrom( wxPGChoicesData* data )
{
    wxASSERT( m_items.size() == 0 );

    unsigned int i;

    for ( i=0; i<data->GetCount(); i++ )
        m_items.push_back( new wxPGChoiceEntry(*data->Item(i)) );
}

// -----------------------------------------------------------------------
// wxPGChoices
// -----------------------------------------------------------------------

wxPGChoiceEntry& wxPGChoices::Add( const wxString& label, int value )
{
    EnsureData();

    wxPGChoiceEntry* p = new wxPGChoiceEntry(label, value);
    m_data->Insert( -1, p );
    return *p;
}

// -----------------------------------------------------------------------

wxPGChoiceEntry& wxPGChoices::Insert( const wxString& label, int index, int value )
{
    EnsureData();

    wxPGChoiceEntry* p = new wxPGChoiceEntry(label, value);
    m_data->Insert( index, p );
    return *p;
}

// -----------------------------------------------------------------------

wxPGChoiceEntry& wxPGChoices::AddAsSorted( const wxString& label, int value )
{
    EnsureData();

    size_t index = 0;

    while ( index < GetCount() )
    {
        int cmpRes = GetLabel(index).Cmp(label);
        if ( cmpRes > 0 )
            break;
        index++;
    }

    wxPGChoiceEntry* p = new wxPGChoiceEntry(label, value);
    m_data->Insert( index, p );
    return *p;
}

// -----------------------------------------------------------------------

void wxPGChoices::Add( const wxChar** labels, const ValArrItem* values )
{
    EnsureData();

    unsigned int itemcount = 0;
    const wxChar** p = &labels[0];
    while ( *p ) { p++; itemcount++; }

    unsigned int i;
    for ( i = 0; i < itemcount; i++ )
    {
        int value = wxPG_INVALID_VALUE;
        if ( values )
            value = values[i];
        m_data->Insert( -1, new wxPGChoiceEntry(labels[i], value) );
    }
}

// -----------------------------------------------------------------------

void wxPGChoices::Add( const wxArrayString& arr, const ValArrItem* values )
{
    EnsureData();

    unsigned int i;
    unsigned int itemcount = arr.GetCount();

    for ( i = 0; i < itemcount; i++ )
    {
        int value = wxPG_INVALID_VALUE;
        if ( values )
            value = values[i];
        m_data->Insert( -1, new wxPGChoiceEntry(arr[i], value) );
    }
}

// -----------------------------------------------------------------------

void wxPGChoices::Add( const wxArrayString& arr, const wxArrayInt& arrint )
{
    EnsureData();

    unsigned int i;
    unsigned int itemcount = arr.GetCount();

    for ( i = 0; i < itemcount; i++ )
    {
        int value = wxPG_INVALID_VALUE;
        if ( &arrint && arrint.size() )
            value = arrint[i];
        m_data->Insert( -1, new wxPGChoiceEntry(arr[i], value) );
    }
}

// -----------------------------------------------------------------------

void wxPGChoices::RemoveAt(size_t nIndex, size_t count)
{
    wxASSERT( m_data->m_refCount != 0xFFFFFFF );
    unsigned int i;
    for ( i=nIndex; i<(nIndex+count); i++)
        delete m_data->Item(i);
    // erase is bugged, atleast the way I use it :)
    //wxArrayPtrVoid::iterator it = m_data->m_items.begin() + nIndex;
    //m_data->m_items.erase( it, it+(count-1) );
    m_data->m_items.RemoveAt(nIndex, count);
}

// -----------------------------------------------------------------------

int wxPGChoices::Index( const wxString& str ) const
{
    if ( IsOk() )
    {
        unsigned int i;
        for ( i=0; i< m_data->GetCount(); i++ )
        {
            if ( m_data->Item(i)->GetText() == str )
                return i;
        }
    }
    return -1;
}

// -----------------------------------------------------------------------

int wxPGChoices::Index( int val ) const
{
    if ( IsOk() )
    {
        unsigned int i;
        for ( i=0; i< m_data->GetCount(); i++ )
        {
            if ( m_data->Item(i)->GetValue() == val )
                return i;
        }
    }
    return -1;
}

// -----------------------------------------------------------------------

wxArrayString wxPGChoices::GetLabels() const
{
    wxArrayString arr;
    unsigned int i;

    if ( this && IsOk() )
        for ( i=0; i<GetCount(); i++ )
            arr.push_back(GetLabel(i));

    return arr;
}

// -----------------------------------------------------------------------

bool wxPGChoices::HasValues() const
{
    if ( GetCount() && GetValue(0) != wxPG_INVALID_VALUE )
        return true;
    return false;
}

// -----------------------------------------------------------------------

void wxPGChoices::AssignData( wxPGChoicesData* data )
{
    Free();

    if ( data != wxPGChoicesEmptyData )
    {
        m_data = data;
        data->m_refCount++;
    }
}

// -----------------------------------------------------------------------

void wxPGChoices::Init()
{
    m_data = wxPGChoicesEmptyData;
}

// -----------------------------------------------------------------------

void wxPGChoices::Free()
{
    if ( m_data != wxPGChoicesEmptyData )
    {
        m_data->DecRef();
        m_data = wxPGChoicesEmptyData;
    }
}

// -----------------------------------------------------------------------
// wxPropertyGridEvent
// -----------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxPropertyGridEvent, wxCommandEvent)


DEFINE_EVENT_TYPE( wxEVT_PG_SELECTED )
DEFINE_EVENT_TYPE( wxEVT_PG_CHANGING )
DEFINE_EVENT_TYPE( wxEVT_PG_CHANGED )
DEFINE_EVENT_TYPE( wxEVT_PG_HIGHLIGHTED )
DEFINE_EVENT_TYPE( wxEVT_PG_RIGHT_CLICK )
DEFINE_EVENT_TYPE( wxEVT_PG_PAGE_CHANGED )
DEFINE_EVENT_TYPE( wxEVT_PG_ITEM_EXPANDED )
DEFINE_EVENT_TYPE( wxEVT_PG_ITEM_COLLAPSED )
DEFINE_EVENT_TYPE( wxEVT_PG_DOUBLE_CLICK )


// -----------------------------------------------------------------------

void wxPropertyGridEvent::Init()
{
    m_value = NULL;
    m_canVeto = false;
    m_wasVetoed = false;
}

// -----------------------------------------------------------------------

wxPropertyGridEvent::wxPropertyGridEvent(wxEventType commandType, int id)
    : wxCommandEvent(commandType,id)
{
    m_property = NULL;
    Init();
}

// -----------------------------------------------------------------------

wxPropertyGridEvent::wxPropertyGridEvent(const wxPropertyGridEvent& event)
    : wxCommandEvent(event)
{
    m_eventType = event.GetEventType();
    m_eventObject = event.m_eventObject;
    m_pg = event.m_pg;
    m_property = event.m_property;
    m_value = event.m_value;
    m_canVeto = event.m_canVeto;
    m_wasVetoed = event.m_wasVetoed;
}

// -----------------------------------------------------------------------

wxPropertyGridEvent::~wxPropertyGridEvent()
{
}

// -----------------------------------------------------------------------

wxEvent* wxPropertyGridEvent::Clone() const
{
    return new wxPropertyGridEvent( *this );
}

// -----------------------------------------------------------------------
// wxPropertyContainerMethods
// - common methods for wxPropertyGrid and wxPropertyGridManager -
// -----------------------------------------------------------------------

void wxPropertyContainerMethods::DoSetPropertyAttribute( wxPGPropArg id, const wxString& name,
                                                         wxVariant& value, long argFlags )
{
    wxPG_PROP_ARG_CALL_PROLOG()

    p->SetAttribute( name, value );

    if ( argFlags & wxPG_RECURSE )
    {
        unsigned int i;
        for ( i = 0; i < p->GetChildCount(); i++ )
            DoSetPropertyAttribute(p->Item(i), name, value, argFlags);
    }
}

void wxPropertyGrid::SetPropertyAttributeAll( const wxString& attrName, wxVariant value )
{
    DoSetPropertyAttribute(GetRoot(), attrName, value, wxPG_RECURSE);
}

// -----------------------------------------------------------------------

void wxPropertyContainerMethods::GetPropertiesWithFlag( wxArrayPGProperty* targetArr,
                                                        wxPGProperty::FlagType flags,
                                                        bool inverse,
                                                        int iterFlags ) const
{
    wxASSERT( targetArr );
    wxPGVIterator it = GetVIterator( iterFlags );

    for ( ;
          !it.AtEnd();
          it.Next() )
    {
        const wxPGProperty* property = it.GetProperty();

        if ( !inverse )
        {
            if ( (property->GetFlags() & flags) == flags )
                targetArr->push_back((wxPGProperty*)property);
        }
        else
        {
            if ( (property->GetFlags() & flags) != flags )
                targetArr->push_back((wxPGProperty*)property);
        }
    }
}

// -----------------------------------------------------------------------

void wxPropertyContainerMethods::SetPropertiesFlag( wxArrayPGProperty& srcArr,
                                                    wxPGProperty::FlagType flags,
                                                    bool inverse )
{
    unsigned int i;

    for ( i=0; i<srcArr.size(); i++ )
    {
        wxPGProperty* property = srcArr[i];

        if ( !inverse )
            property->SetFlag(flags);
        else
            property->ClearFlag(flags);
    }

    // If collapsed flag or hidden was manipulated, we need to update virtual
    // size.
    wxPropertyGrid* pg = GetState()->GetGrid();
    if ( flags & (wxPG_PROP_COLLAPSED|wxPG_PROP_HIDDEN) )
        pg->RecalculateVirtualSize();
}

// -----------------------------------------------------------------------

void wxPropertyContainerMethods::SetBoolChoices( const wxString& trueChoice,
                                                 const wxString& falseChoice )
{
    WX_PG_GLOBALS_LOCKER()
    wxPGGlobalVars->m_boolChoices[0] = falseChoice;
    wxPGGlobalVars->m_boolChoices[1] = trueChoice;
}

// -----------------------------------------------------------------------

wxPGChoices gs_emptyChoices;

wxPGChoices& wxPropertyContainerMethods::GetPropertyChoices( wxPGPropArg id )
{
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(gs_emptyChoices)

    wxPGChoiceInfo ci;
    ci.m_choices = (wxPGChoices*) NULL;

    p->GetChoiceInfo(&ci);

    if ( !ci.m_choices )
        return gs_emptyChoices;

    return *ci.m_choices;
}

// -----------------------------------------------------------------------

wxPGProperty* wxPropertyContainerMethods::DoGetPropertyByName( wxPGPropNameStr name ) const
{
    return m_pState->BaseGetPropertyByName(name);
}

// -----------------------------------------------------------------------

wxPGProperty* wxPropertyContainerMethods::GetPropertyByName( wxPGPropNameStr name,
                                                             wxPGPropNameStr subname ) const
{
    wxPGProperty* p = DoGetPropertyByName(name);
    if ( !p || !p->GetChildCount() )
        return wxNullProperty;

    return p->GetPropertyByName(subname);
}

// -----------------------------------------------------------------------

// Since GetPropertyByName is used *a lot*, this makes sense
// since non-virtual method can be called with less code.
wxPGProperty* wxPropertyContainerMethods::GetPropertyByName( wxPGPropNameStr name ) const
{
    wxPGProperty* p = DoGetPropertyByName(name);
    if ( p )
        return p;

    // Check if its "Property.SubProperty" format
    int pos = name.Find(wxT('.'));
    if ( pos <= 0 )
        return NULL;

    return GetPropertyByName(name.substr(0,pos),
                             name.substr(pos+1,name.length()-pos-1));
}

// -----------------------------------------------------------------------

bool wxPropertyContainerMethods::HideProperty( wxPGPropArg id, bool hide )
{
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(false)

    wxPropertyGrid* pg = m_pState->GetGrid();

    if ( pg == p->GetGrid() )
        return pg->DoHideProperty(p, hide);
    else
        m_pState->HideProperty(p, hide);

    return true;
}

// -----------------------------------------------------------------------

bool wxPropertyContainerMethods::Collapse( wxPGPropArg id )
{
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(false)
    wxPropertyGrid* pg = p->GetGridIfDisplayed();
    if ( pg )
        return pg->DoCollapse(p);

    return p->GetParentState()->DoCollapse(p);
}

// -----------------------------------------------------------------------

bool wxPropertyContainerMethods::Expand( wxPGPropArg id )
{
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(false)
    wxPropertyGrid* pg = p->GetGridIfDisplayed();
    if ( pg )
        return pg->DoExpand(p);

    return p->GetParentState()->DoExpand(p);
}

// -----------------------------------------------------------------------

void wxPropertyContainerMethods::SetPropertyLabel( wxPGPropArg id, const wxString& newproplabel )
{
    wxPG_PROP_ARG_CALL_PROLOG()

    p->SetLabel( newproplabel );

    wxPropertyGridState* state = p->GetParentState();
    wxPropertyGrid* pg = state->GetGrid();

    if ( pg->HasFlag(wxPG_AUTO_SORT) )
        pg->Sort(p->GetParent());

    if ( pg->GetState() == state )
    {
        if ( pg->HasFlag(wxPG_AUTO_SORT) )
            pg->Refresh();
        else
            pg->DrawItem( p );
    }
}

// -----------------------------------------------------------------------

bool wxPropertyContainerMethods::SetPropertyMaxLength( wxPGPropArg id, int maxLen )
{
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(false)

    wxPropertyGrid* pg = m_pState->GetGrid();

    p->m_maxLen = (short) maxLen;

    // Adjust control if selected currently
    if ( pg == p->GetGrid() && p == m_pState->GetSelection() )
    {
        wxWindow* wnd = pg->GetEditorControl();
        wxTextCtrl* tc = wxDynamicCast(wnd,wxTextCtrl);
        if ( tc )
            tc->SetMaxLength( maxLen );
        else
        // Not a text ctrl
            return false;
    }

    return true;
}

// -----------------------------------------------------------------------
// GetPropertyValueAsXXX methods

#define IMPLEMENT_GET_VALUE(T,TRET,BIGNAME,DEFRETVAL) \
TRET wxPropertyContainerMethods::GetPropertyValueAs##BIGNAME( wxPGPropArg id ) const \
{ \
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(DEFRETVAL) \
    wxVariant value = p->GetValue(); \
    if ( wxStrcmp(value.GetType(), wxPGTypeName_##T) != 0 ) \
    { \
        wxPGGetFailed(p,wxPGTypeName_##T); \
        return (TRET)DEFRETVAL; \
    } \
    return (TRET)value.Get##BIGNAME(); \
}

// String is different than others.
wxString wxPropertyContainerMethods::GetPropertyValueAsString( wxPGPropArg id ) const
{
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(wxEmptyString)
    return p->GetValueAsString(wxPG_FULL_VALUE);
}

bool wxPropertyContainerMethods::GetPropertyValueAsBool( wxPGPropArg id ) const
{
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(false)
    wxVariant value = p->GetValue();
    if ( wxStrcmp(value.GetType(), wxPGTypeName_bool) == 0 )
    {
        return value.GetBool();
    }
    if ( wxStrcmp(value.GetType(), wxPGTypeName_long) == 0 )
    {
        return value.GetLong()?true:false;
    }
    wxPGGetFailed(p,wxPGTypeName_bool);
    return false;
}

IMPLEMENT_GET_VALUE(long,long,Long,0)
IMPLEMENT_GET_VALUE(double,double,Double,0.0)
IMPLEMENT_GET_VALUE(void,void*,VoidPtr,NULL)

#ifdef __WXPYTHON__
PyObject* wxPropertyContainerMethods::GetPropertyValueAsPyObject( wxPGPropArg id ) const
{
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(NULL)
    wxVariant value = p->GetValue();

    wxPGVariantDataPyObject* pgvdata = wxDynamicCastVariantData(value.GetData(), wxPGVariantDataPyObject);
    if ( value.IsNull() || !pgvdata )
    {
        wxPGGetFailed(p,wxT("PyObject*"));
        return NULL;
    }
    return PyObjectFromVariant(value);
}
#endif

// wxObject is different than others.
wxObject* wxPropertyContainerMethods::GetPropertyValueAsWxObjectPtr( wxPGPropArg id ) const
{
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL((wxObject*)NULL)

    wxVariant value = p->GetValue();
    wxVariantData* vdata = value.GetData();

    if ( !vdata->GetValueClassInfo() )
        return (wxObject*) NULL;

	wxPGVariantData* pgvdata = wxDynamicCastVariantData(vdata, wxPGVariantData);
    if ( pgvdata )
         return (wxObject*) pgvdata->GetValuePtr();

    if ( wxPGIsVariantClassInfo(wxPGVariantDataGetClassInfo(vdata), wxobject) )
        return (wxObject*) value.GetWxObjectPtr();

    return (wxObject*) NULL;
}

// -----------------------------------------------------------------------

bool wxPropertyContainerMethods::IsPropertyExpanded( wxPGPropArg id ) const
{
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(false)
    return p->IsExpanded();
}

// -----------------------------------------------------------------------

wxPGProperty* wxPropertyContainerMethods::CreatePropertyByType(const wxString &/*valuetype*/,
                                                               const wxString &/*label*/,
                                                               const wxString &/*name*/)
{
/*    wxPGHashMapS2P::iterator it;

    it = wxPGGlobalVars->m_dictValueType.find(valuetype);

    if ( it != wxPGGlobalVars->m_dictValueType.end() )
    {
        wxPGValueType* vt = (wxPGValueType*) it->second;
        wxPGProperty* p = vt->GenerateProperty(label,name);
    #ifdef __WXDEBUG__
        if ( !p )
        {
            wxLogDebug(wxT("WARNING: CreatePropertyByValueType generated NULL property for ValueType \"%s\""),valuetype.c_str());
            return (wxPGProperty*) NULL;
        }
    #endif
        return p;
    }

    wxLogDebug(wxT("WARNING: No value type registered with name \"%s\""),valuetype.c_str());*/
    return (wxPGProperty*) NULL;
}

// -----------------------------------------------------------------------

wxPGProperty* wxPropertyContainerMethods::CreatePropertyByClass(const wxString &/*classname*/,
                                                                const wxString &/*label*/,
                                                                const wxString &/*name*/)
{
/*    wxPGHashMapS2P* cis =
        (wxPGHashMapS2P*) &wxPGGlobalVars->m_dictPropertyClassInfo;

    const wxString* pClassname = &classname;
    wxString s;

    // Translate to long name, if necessary
    if ( (pClassname->GetChar(0) != wxT('w') || pClassname->GetChar(1) != wxT('x')) &&
          pClassname->Find(wxT("Property")) < 0 )
    {
        if ( classname != wxT("Category") )
            s.Printf(wxT("wx%sProperty"),pClassname->c_str());
        else
            s = wxT("wxPropertyCategory");
        pClassname = &s;
    }

    wxPGHashMapS2P::iterator it;
    it = cis->find(*pClassname);

    if ( it != cis->end() )
    {
        wxPGPropertyClassInfo* pci = (wxPGPropertyClassInfo*) it->second;
        wxPGProperty* p = pci->m_constructor(label,name);
        return p;
    }
    wxLogError(wxT("No such property class: %s"),pClassname->c_str());*/
    return (wxPGProperty*) NULL;
}

// -----------------------------------------------------------------------
// wxPropertyGridIterator
// -----------------------------------------------------------------------

void wxPropertyGridIteratorBase::Init( wxPropertyGridState* state, int flags, wxPGProperty* property, int dir  )
{
    wxASSERT( dir == 1 || dir == -1 );

    m_state = state;
    m_baseParent = state->GetRoot();
    if ( !property && m_baseParent->GetChildCount() )
        property = m_baseParent->Item(0);

    m_property = property;

    wxPG_ITERATOR_CREATE_MASKS(flags, m_itemExMask, m_parentExMask)

    // Need to skip first?
    if ( property && (property->GetFlags() & m_itemExMask) )
    {
        if ( dir == 1 )
            Next();
        else
            Prev();
    }
}

void wxPropertyGridIteratorBase::Init( wxPropertyGridState* state, int flags, int startPos, int dir  )
{
    wxPGProperty* property;

    if ( startPos == wxTOP )
    {
        property = NULL;
        if ( dir == 0 )
            dir = 1;
    }
    else if ( startPos == wxBOTTOM )
    {
        property = state->GetLastItem(flags);
        if ( dir == 0 )
            dir = -1;
    }
    else
    {
        wxASSERT_MSG( false, wxT("Only supported stating positions are wxTOP and wxBOTTOM") );
        property = NULL;
    }

    Init( state, flags, property, dir );
}

void wxPropertyGridIteratorBase::Assign( const wxPropertyGridIteratorBase& it )
{
    m_property = it.m_property;
    m_state = it.m_state;
    m_baseParent = it.m_baseParent;
    m_itemExMask = it.m_itemExMask;
    m_parentExMask = it.m_parentExMask;
}

void wxPropertyGridIteratorBase::Prev()
{
    wxPGProperty* property = m_property;
    wxASSERT( property );

    wxPGProperty* parent = property->GetParent();
    wxASSERT( parent );
    unsigned int index = property->GetIndexInParent();

    if ( index > 0 )
    {
        // Previous sibling
        index--;

        property = parent->Item(index);

        // Go to last children?
        if ( property->GetChildCount() &&
             wxPG_ITERATOR_PARENTEXMASK_TEST(property, m_parentExMask) )
        {
            // First child
            property = property->Last();
        }
    }
    else
    {
        // Up to a parent
        if ( parent == m_baseParent )
        {
            m_property = NULL;
            return;
        }
        else
        {
            property = parent;
        }
    }

    m_property = property;

    // If property does not match our criteria, skip it
    if ( property->GetFlags() & m_itemExMask )
        Prev();
}

void wxPropertyGridIteratorBase::Next( bool iterateChildren )
{
    wxPGProperty* property = m_property;
    wxASSERT( property );

    if ( property->GetChildCount() &&
         wxPG_ITERATOR_PARENTEXMASK_TEST(property, m_parentExMask) &&
         iterateChildren )
    {
        // First child
        property = property->Item(0);
    }
    else
    {
        wxPGProperty* parent = property->GetParent();
        wxASSERT( parent );
        unsigned int index = property->GetIndexInParent() + 1;

        if ( index < parent->GetChildCount() )
        {
            // Next sibling
            property = parent->Item(index);
        }
        else
        {
            // Next sibling of parent
            if ( parent == m_baseParent )
            {
                m_property = NULL;
            }
            else
            {
                m_property = parent;
                Next(false);
            }
            return;
        }
    }

    m_property = property;

    // If property does not match our criteria, skip it
    if ( property->GetFlags() & m_itemExMask )
        Next();
}

// -----------------------------------------------------------------------
// wxPGVIterator_State
// -----------------------------------------------------------------------

// Default returned by wxPropertyContainerMethods::GetVIterator().
class wxPGVIteratorBase_State : public wxPGVIteratorBase
{
public:
    wxPGVIteratorBase_State( wxPropertyGridState* state, int flags )
    {
        m_it.Init( state, flags );
    }
    virtual ~wxPGVIteratorBase_State() { }
    virtual void Next() { m_it.Next(); }
};

wxPGVIterator wxPropertyContainerMethods::GetVIterator( int flags ) const
{
    return wxPGVIterator( new wxPGVIteratorBase_State( m_pState, flags ) );
}

// -----------------------------------------------------------------------
// wxPropertyGridState
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// wxPropertyGridState item iteration methods
// -----------------------------------------------------------------------

#if wxPG_COMPATIBILITY_1_2_0

// Skips categories and sub-properties (unless in wxParentProperty).
wxPGProperty* wxPropertyGridState::GetNextProperty( wxPGProperty* p )
{
    wxPropertyGridIterator it( (wxPropertyGridState*)this, wxPG_ITERATE_DEFAULT, p );

    it.Next();
    while ( !it.AtEnd() )
        it.Next();

    return it.GetProperty();
}

// -----------------------------------------------------------------------

wxPGProperty* wxPropertyGridState::GetNextSibling( wxPGProperty* p )
{
    wxPGProperty* parent = p->m_parent;
    size_t next_ind = p->m_arrIndex + 1;
    if ( next_ind >= parent->GetCount() ) return (wxPGProperty*)NULL;
    return parent->Item(next_ind);
}

// -----------------------------------------------------------------------

wxPGProperty* wxPropertyGridState::GetPrevSibling( wxPGProperty* p )
{
    size_t ind = p->m_arrIndex;
    if ( ind < 1 ) return (wxPGProperty*)NULL;
    return p->m_parent->Item(ind-1);
}

// -----------------------------------------------------------------------

// Skips categories and sub-properties (unless in wxParentProperty).
wxPGProperty* wxPropertyGridState::GetPrevProperty( wxPGProperty* p )
{
    wxPropertyGridIterator it( (wxPropertyGridState*)this, wxPG_ITERATE_DEFAULT, p );

    it.Prev();
    while ( !it.AtEnd() )
        it.Prev();

    return it.GetProperty();
}

// -----------------------------------------------------------------------

wxPGProperty* wxPropertyGridState::GetNextCategory( wxPGProperty* p ) const
{
    wxPGProperty* current = (wxPGProperty*)p;

    wxCHECK_MSG( !IsInNonCatMode() || current->IsCategory(), (wxPGProperty*)NULL,
        wxT("GetNextCategory should not be called with non-category argument in non-categoric mode.") );

    wxPropertyGridConstIterator it( this, wxPG_ITERATE_CATEGORIES, p );
    if ( *it != p )
        return (wxPGProperty*) *it;
    it++;
    return (wxPGProperty*) *it;
}

#endif // #if wxPG_COMPATIBILITY_1_2_0

// -----------------------------------------------------------------------

wxPGProperty* wxPropertyGridState::GetLastItem( int flags )
{
    if ( !m_properties->GetCount() )
        return (wxPGProperty*) NULL;

    wxPG_ITERATOR_CREATE_MASKS(flags, int itemExMask, int parentExMask)

    // First, get last child of last parent
    wxPGProperty* pwc = (wxPGProperty*)m_properties->Last();
    while ( pwc->GetChildCount() &&
            wxPG_ITERATOR_PARENTEXMASK_TEST(pwc, parentExMask) )
        pwc = (wxPGProperty*) pwc->Last();

    // Then, if it doesn't fit our criteria, back up until we find something that does
    if ( pwc->GetFlags() & itemExMask )
    {
        wxPropertyGridIterator it( this, flags, pwc );
        for ( ; !it.AtEnd(); it.Prev() );
        pwc = (wxPGProperty*) it.GetProperty();
    }

    return pwc;
}

wxPropertyCategory* wxPropertyGridState::GetPropertyCategory( const wxPGProperty* p ) const
{
    const wxPGProperty* parent = (const wxPGProperty*)p;
    const wxPGProperty* grandparent = (const wxPGProperty*)parent->GetParent();
    do
    {
        parent = grandparent;
        grandparent = (wxPGProperty*)parent->GetParent();
        if ( parent->IsCategory() && grandparent )
            return (wxPropertyCategory*)parent;
    } while ( grandparent );

    return (wxPropertyCategory*) NULL;
}

// -----------------------------------------------------------------------
// wxPropertyGridState GetPropertyXXX methods
// -----------------------------------------------------------------------

wxPGProperty* wxPropertyGridState::GetPropertyByLabel( const wxString& label,
                                                       wxPGProperty* parent ) const
{

    size_t i;

    if ( !parent ) parent = (wxPGProperty*) &m_regularArray;

    for ( i=0; i<parent->GetCount(); i++ )
    {
        wxPGProperty* p = parent->Item(i);
        if ( p->m_label == label )
            return p;
        // Check children recursively.
        if ( p->GetChildCount() )
        {
            p = GetPropertyByLabel(label,(wxPGProperty*)p);
            if ( p )
                return p;
        }
    }

    return NULL;
}

// -----------------------------------------------------------------------

wxPGProperty* wxPropertyGridState::BaseGetPropertyByName( wxPGPropNameStr name ) const
{
    wxPGHashMapS2P::const_iterator it;
    it = m_dictName.find(name);
    if ( it != m_dictName.end() )
        return (wxPGProperty*) it->second;
    return (wxPGProperty*) NULL;
}

// -----------------------------------------------------------------------
// wxPropertyGridState global operations
// -----------------------------------------------------------------------

bool wxPropertyGridState::EnableCategories( bool enable )
{
    //
    // NB: We can't use wxPropertyGridIterator in this
    //     function, since it depends on m_arrIndexes,
    //     which, among other things, is being fixed here.
    //
    ITEM_ITERATION_VARIABLES

    if ( enable )
    {
        //
        // Enable categories
        //

        if ( !IsInNonCatMode() )
            return false;

        m_properties = &m_regularArray;

        // fix parents, indexes, and depths
        ITEM_ITERATION_INIT_FROM_THE_TOP

        ITEM_ITERATION_LOOP_BEGIN

            p->m_arrIndex = i;

            p->m_parent = parent;

            // If parent was category, and this is not,
            // then the depth stays the same.
            if ( parent->IsCategory() &&
                 !p->IsCategory() )
                p->m_depth = parent->m_depth;
            else
                p->m_depth = parent->m_depth + 1;

        ITEM_ITERATION_LOOP_END

    }
    else
    {
        //
        // Disable categories
        //

        if ( IsInNonCatMode() )
            return false;

        // Create array, if necessary.
        if ( !m_abcArray )
            InitNonCatMode();

        m_properties = m_abcArray;

        // fix parents, indexes, and depths
        ITEM_ITERATION_INIT_FROM_THE_TOP

        ITEM_ITERATION_LOOP_BEGIN

            p->m_arrIndex = i;

            p->m_parent = parent;

            p->m_depth = parent->m_depth + 1;

        ITEM_ITERATION_LOOP_END
    }

    if ( m_pPropGrid->GetState() == this )
        m_pPropGrid->RecalculateVirtualSize();

    return true;
}

// -----------------------------------------------------------------------

static int wxPG_SortFunc(void **p1, void **p2)
{
    wxPGProperty *pp1 = *((wxPGProperty**)p1);
    wxPGProperty *pp2 = *((wxPGProperty**)p2);
    return pp1->GetLabel().compare( pp2->GetLabel() );
}

void wxPropertyGridState::Sort( wxPGProperty* p )
{
    if ( !p )
        p = (wxPGProperty*)m_properties;

    if ( !p->GetChildCount() )
        return;

    wxPGProperty* pwc = (wxPGProperty*)p;

    // Can only sort items with children
    if ( pwc->m_children.GetCount() < 1 )
        return;

    pwc->m_children.Sort( wxPG_SortFunc );

    // Fix indexes
    pwc->FixIndexesOfChildren();

}

// -----------------------------------------------------------------------

void wxPropertyGridState::Sort()
{
    Sort( m_properties );

    // Sort categories as well
    if ( !IsInNonCatMode() )
    {
        size_t i;
        for ( i=0;i<m_properties->GetCount();i++)
        {
            wxPGProperty* p = m_properties->Item(i);
            if ( p->IsCategory() )
                Sort( p );
        }
    }
}

// -----------------------------------------------------------------------
// wxPropertyGridState splitter, column and hittest functions
// -----------------------------------------------------------------------

wxPGProperty* wxPropertyGridState::DoGetItemAtY( int y ) const
{
    // Outside?
    if ( y >= (int)m_bottomY || y < 0 )
        return (wxPGProperty*) NULL;

    unsigned int a = 0;
    return m_properties->GetItemAtY(y, GetGrid()->m_lineHeight, &a);
}

// -----------------------------------------------------------------------

wxPropertyGridHitTestResult wxPropertyGridState::HitTest( const wxPoint&pt ) const
{
    wxPropertyGridHitTestResult result;
    result.column = HitTestH( pt.x, &result.splitter, &result.splitterHitOffset );
    result.property = DoGetItemAtY( pt.y );
    return result;
}

// -----------------------------------------------------------------------

// Used by SetSplitterLeft
int wxPropertyGridState::GetLeftSplitterPos(wxClientDC& dc,
                                            wxPGProperty* pwc,
                                            bool subProps)
{
    wxPropertyGrid* pg = m_pPropGrid;
    size_t i;
    int maxW = 0;
    int w, h;

    for ( i=0; i<pwc->GetCount(); i++ )
    {
        wxPGProperty* p = pwc->Item(i);
        if ( !p->IsCategory() )
        {
            dc.GetTextExtent( p->GetLabel(), &w, &h );

            w += pg->m_marginWidth + ( ((int)p->m_depth-1) * pg->m_subgroup_extramargin ) + (wxPG_XBEFORETEXT*2);

            if ( w > maxW )
                maxW = w;
        }

        if ( p->GetChildCount() &&
             ( subProps || p->IsCategory() ) )
        {
            w = GetLeftSplitterPos( dc, (wxPGProperty*) p, subProps );

            if ( w > maxW )
                maxW = w;
        }
    }

    return maxW;
}

int wxPropertyGridState::GetSplitterPosition( int splitterColumn ) const
{
    int n = GetGrid()->m_marginWidth;
    int i;
    for ( i=0; i<=splitterColumn; i++ )
        n += m_colWidths[i];
    return n;
}

int wxPropertyGridState::GetColumnMinWidth( int WXUNUSED(column) ) const
{
    return wxPG_DRAG_MARGIN;
}

void wxPropertyGridState::PropagateColSizeDec( int column, int decrease, int dir )
{
    int origWidth = m_colWidths[column];
    m_colWidths[column] -= decrease;
    int min = GetColumnMinWidth(column);
    int more = 0;
    if ( m_colWidths[column] < min )
    {
        more = decrease - (origWidth - min);
        m_colWidths[column] = min;
    }

    column += dir;
    if ( more && column < (int)m_colWidths.size() && column >= 0 )
        PropagateColSizeDec( column, more, dir );
}

void wxPropertyGridState::DoSetSplitterPosition( int newXPos, int splitterColumn, bool WXUNUSED(allPages) )
{
    wxPropertyGrid* pg = GetGrid();

    int adjust = newXPos - GetSplitterPosition(splitterColumn);

    if ( !pg->HasVirtualWidth() )
    {
        // No virtual width
        int otherColumn;
        if ( adjust > 0 )
        {
            otherColumn = splitterColumn + 1;
            if ( otherColumn == (int)m_colWidths.size() )
                otherColumn = 0;
            m_colWidths[splitterColumn] += adjust;
            PropagateColSizeDec( otherColumn, adjust, 1 );
        }
        else
        {
            otherColumn = splitterColumn + 1;
            if ( otherColumn < 0 )
                otherColumn = splitterColumn - 1;
            m_colWidths[otherColumn] -= adjust;
            PropagateColSizeDec( splitterColumn, -adjust, -1 );
        }
    }
    else
        m_colWidths[splitterColumn] += adjust;

    if ( splitterColumn == 0 )
        m_fSplitterX = (double) m_colWidths[splitterColumn];

    // Don't allow initial splitter auto-positioning after this.
    if ( pg->GetState() == this )
        pg->SetInternalFlag(wxPG_FL_SPLITTER_PRE_SET);

    CheckColumnWidths();
}

// Moves splitter so that all labels are visible, but just.
void wxPropertyGridState::SetSplitterLeft( bool subProps )
{
    wxPropertyGrid* pg = GetGrid();
    wxClientDC dc(pg);
    dc.SetFont(pg->m_font);

    int maxW = GetLeftSplitterPos(dc, m_properties, subProps );

    if ( maxW > 0 )
        DoSetSplitterPosition( maxW );

    pg->SetInternalFlag(wxPG_FL_DONT_CENTER_SPLITTER);
}

void wxPropertyGridState::CheckColumnWidths()
{
    if ( m_width == 0 )
        return;

    wxPropertyGrid* pg = GetGrid();

#ifdef __WXDEBUG__
    const bool debug = false;
#endif

    long timeSinceCreation = (::wxGetLocalTimeMillis() - pg->m_timeCreated).ToLong();

    unsigned int i;
    unsigned int lastColumn = m_colWidths.size() - 1;
    int width = m_width;
    int clientWidth = pg->GetClientSize().x;
    int widestColWidth = -1;
    int widestCol = -1;
    bool minimizedCols = false;

#ifdef __WXDEBUG__
    if ( debug )
        wxLogDebug(wxT("ColumnWidthCheck (virtualWidth: %i, clientWidth: %i)"), width, clientWidth);
#endif

    // Check min sizes
    if ( timeSinceCreation >= 750 )
    {
        for ( i=0; i<m_colWidths.size(); i++ )
        {
            int min = GetColumnMinWidth(i);
            if ( m_colWidths[i] <= min )
            {
                m_colWidths[i] = min;
                minimizedCols = true;
            }
            else if ( m_colWidths[i] > widestColWidth )
            {
                // Only make record of widest column if there was one
                // with width higher than minimum width. Not doing this
                // could result in infinite recursion.
                widestColWidth = m_colWidths[i];
                widestCol = i;
            }
        }
    }

    int colsWidth = pg->m_marginWidth;
    for ( i=0; i<m_colWidths.size(); i++ )
        colsWidth += m_colWidths[i];

#ifdef __WXDEBUG__
    if ( debug )
        wxLogDebug(wxT("  HasVirtualWidth: %i  colsWidth: %i"),(int)pg->HasVirtualWidth(),colsWidth);
#endif

    // Then mode-based requirement
    if ( !pg->HasVirtualWidth() )
    {
        int widthHigher = width - colsWidth;

        // Adapt colsWidth to width
        if ( colsWidth < width )
        {
            m_colWidths[lastColumn] = m_colWidths[lastColumn] + widthHigher;
        }
        else if ( colsWidth > width )
        {
            if ( widestCol != -1 )
            {
            #ifdef __WXDEBUG__
                if ( debug )
                    wxLogDebug(wxT("  Reduce widest column %i (by %i)"), widestCol, -widthHigher);
            #endif
                // Reduce widest column, and recheck
                m_colWidths[widestCol] = m_colWidths[widestCol] + widthHigher;
                CheckColumnWidths();
            }
        }
    }
    else
    {
        // Only check colsWidth against clientWidth
        if ( colsWidth < clientWidth )
        {
            m_colWidths[lastColumn] = m_colWidths[lastColumn] + (clientWidth-colsWidth);
        }

        m_width = colsWidth;

        // If width changed, recalculate virtual size
        if ( pg->GetState() == this )
            pg->RecalculateVirtualSize();
    }

#ifdef __WXDEBUG__
    if ( debug )
        for ( i=0; i<m_colWidths.size(); i++ )
            wxLogDebug(wxT("col%i: %i"),i,m_colWidths[i]);
#endif
}

void wxPropertyGridState::SetColumnCount( int colCount )
{
    wxASSERT( colCount >= 2 );
    m_colWidths.SetCount( colCount, wxPG_DRAG_MARGIN );
    if ( m_colWidths.size() > (unsigned int)colCount )
        m_colWidths.RemoveAt( m_colWidths.size(), m_colWidths.size() - colCount );

    if ( m_pPropGrid->GetState() == this )
        m_pPropGrid->RecalculateVirtualSize();
    else
        CheckColumnWidths();
}

// Returns column index, -1 for margin
int wxPropertyGridState::HitTestH( int x, int* pSplitterHit, int* pSplitterHitOffset ) const
{
    int cx = GetGrid()->m_marginWidth;
    int col = -1;
    int prevSplitter = -1;

    while ( x > cx )
    {
        col++;
        if ( col >= (int)m_colWidths.size() )
        {
            *pSplitterHit = -1;
            return col;
        }
        prevSplitter = cx;
        cx += m_colWidths[col];
    }

    // Near prev. splitter
    if ( col >= 1 )
    {
        int diff = x - prevSplitter;
        if ( abs(diff) < wxPG_SPLITTERX_DETECTMARGIN1 )
        {
            *pSplitterHit = col - 1;
            *pSplitterHitOffset = diff;
            return col;
        }
    }

    // Near next splitter
    int nextSplitter = cx;
    if ( col < (int)(m_colWidths.size()-1) )
    {
        int diff = x - nextSplitter;
        if ( abs(diff) < wxPG_SPLITTERX_DETECTMARGIN1 )
        {
            *pSplitterHit = col;
            *pSplitterHitOffset = diff;
            return col;
        }
    }

    *pSplitterHit = -1;
    return col;
}

// -----------------------------------------------------------------------
// wxPropertyGridState property value setting and getting
// -----------------------------------------------------------------------

bool wxPropertyGridState::DoSetPropertyValueString( wxPGProperty* p, const wxString& value )
{
    if ( p )
    {
        int flags = wxPG_REPORT_ERROR|wxPG_FULL_VALUE;
        CLEAR_PROPERTY_UNSPECIFIED_FLAG(p);

        wxVariant variant = p->GetValueRef();
        bool res;

        if ( p->GetMaxLength() <= 0 )
            res = p->ActualStringToValue( variant, value, flags );
        else
            res = p->ActualStringToValue( variant, value.Mid(0,p->GetMaxLength()), flags );

        if ( res )
        {
            p->SetValue(variant);
            if ( m_selected==p && this==m_pPropGrid->GetState() )
                p->UpdateControl(m_pPropGrid->GetEditorControl());
        }

        return true;
    }
    return false;
}

// -----------------------------------------------------------------------

bool wxPropertyGridState::DoSetPropertyValue( wxPGProperty* p, wxVariant& value )
{
    if ( p )
    {
        CLEAR_PROPERTY_UNSPECIFIED_FLAG(p);
        p->SetValue(value);
        if ( m_selected==p && this==m_pPropGrid->GetState() )
            p->UpdateControl(m_pPropGrid->GetEditorControl());

        return true;
    }
    return false;
}

// -----------------------------------------------------------------------

bool wxPropertyGridState::DoSetPropertyValueWxObjectPtr( wxPGProperty* p, wxObject* value )
{
    if ( p )
    {
        // TODO: Add proper testing
        //if ( wxStrcmp( p->GetValue().GetType(),
        //               value->GetClassInfo()->GetClassName()
        //              ) == 0
        //   )
        {
            // wnd_primary has to be given so the control can be updated as well.
            wxVariant v(value);
            DoSetPropertyValue(p, v);
            return true;
        }
        //wxPGTypeOperationFailed ( p, wxT("wxObject"), wxT("Set") );
    }
    return false;
}

// -----------------------------------------------------------------------

void wxPropertyGridState::DoSetPropertyUnspecified( wxPGProperty* p )
{
    wxCHECK_RET( p, wxT("invalid property id") );

    if ( !p->IsValueUnspecified() )
    {
        // Value should be set first - editor class methods may need it
        p->m_value.MakeNull();

        wxASSERT( m_pPropGrid );

        if ( m_pPropGrid->GetState() == this )
        {
            if ( m_pPropGrid->m_selected == p && m_pPropGrid->m_wndPrimary )
            {
                p->GetEditorClass()->SetValueToUnspecified(m_pPropGrid->GetEditorControl());
            }
        }

        unsigned int i;
        for ( i = 0; i < p->GetChildCount(); i++ )
            DoSetPropertyUnspecified( p->Item(i) );
    }
}

// -----------------------------------------------------------------------
// wxPropertyGridState property operations
// -----------------------------------------------------------------------

void wxPropertyGridState::ClearModifiedStatus( wxPGProperty* p )
{
    if ( p->m_flags & wxPG_PROP_MODIFIED )
    {
        p->m_flags &= ~(wxPG_PROP_MODIFIED);

        if ( m_pPropGrid->GetState() == this )
        {
            // Clear active editor bold
            if ( p == m_selected && m_pPropGrid->m_wndPrimary )
                m_pPropGrid->m_wndPrimary->SetFont( m_pPropGrid->GetFont() );

            m_pPropGrid->DrawItem( p );
        }
    }

    size_t i;
    for ( i = 0; i < p->GetChildCount(); i++ )
        ClearModifiedStatus( p->Item(i) );
}

// -----------------------------------------------------------------------

bool wxPropertyGridState::DoCollapse( wxPGProperty* p, int* pHeightAdd )
{
    wxCHECK_MSG( p, false, wxT("invalid property id") );

    wxPGProperty* pwc = (wxPGProperty*)p;
    if ( !pwc->GetChildCount() ) return false;

    if ( !pwc->IsExpanded() ) return false;

    int heightAdd = -pwc->GetChildrenHeight(m_pPropGrid->GetRowHeight());

    if ( pHeightAdd )
        *pHeightAdd = heightAdd;

    m_bottomY += heightAdd;

    pwc->SetExpanded(false);

    return true;
}

// -----------------------------------------------------------------------

bool wxPropertyGridState::DoExpand( wxPGProperty* p, int* pHeightAdd )
{
    wxCHECK_MSG( p, false, wxT("invalid property id") );

    wxPGProperty* pwc = (wxPGProperty*)p;
    if ( !pwc->GetChildCount() ) return false;

    if ( pwc->IsExpanded() ) return false;

    pwc->SetExpanded(true);

    int heightAdd = pwc->GetChildrenHeight(m_pPropGrid->GetRowHeight());

    if ( pHeightAdd )
        *pHeightAdd = heightAdd;

    m_bottomY += heightAdd;

    return true;
}

// -----------------------------------------------------------------------

bool wxPropertyGridState::DoSelectProperty( wxPGProperty* p, unsigned int flags )
{
    if ( this == m_pPropGrid->GetState() )
        return m_pPropGrid->DoSelectProperty( p, flags );

    m_selected = p;
    return true;
}

// -----------------------------------------------------------------------

bool wxPropertyGridState::HideProperty( wxPGProperty* p, bool hide )
{
    if ( !hide ) p->ClearFlag( wxPG_PROP_HIDDEN );
    else p->SetFlag( wxPG_PROP_HIDDEN );

    unsigned int i;
    for ( i = 0; i < p->GetChildCount(); i++ )
        HideProperty(p->Item(i), hide);

    return true;
}

// -----------------------------------------------------------------------

void wxPropertyGridState::SetPropertyAndChildrenFlags( wxPGProperty* p, long flags )
{
    p->m_flags |= flags;

    unsigned int i;
    for ( i = 0; i < p->GetChildCount(); i++ )
        ClearPropertyAndChildrenFlags( p->Item(i), flags );
}

// -----------------------------------------------------------------------

void wxPropertyGridState::ClearPropertyAndChildrenFlags( wxPGProperty* p, long flags )
{
    p->m_flags &= ~(flags);

    unsigned int i;
    for ( i = 0; i < p->GetChildCount(); i++ )
        ClearPropertyAndChildrenFlags( p->Item(i), flags );
}

// -----------------------------------------------------------------------

bool wxPropertyGridState::DoEnableProperty( wxPGProperty* p, bool enable )
{
    if ( p )
    {
        if ( enable )
        {
            if ( !(p->m_flags & wxPG_PROP_DISABLED) )
                return false;

            // Enabling

            p->m_flags &= ~(wxPG_PROP_DISABLED);
        }
        else
        {
            if ( p->m_flags & wxPG_PROP_DISABLED )
                return false;

            // Disabling

            p->m_flags |= wxPG_PROP_DISABLED;

        }

        // Apply same to sub-properties as well
        unsigned int i;
        for ( i = 0; i < p->GetChildCount(); i++ )
            DoEnableProperty( p->Item(i), enable );

        return true;
    }
    return false;
}

// -----------------------------------------------------------------------
// wxPropertyGridState wxVariant related routines
// -----------------------------------------------------------------------

// Returns list of wxVariant objects (non-categories and non-sub-properties only).
// Never includes sub-properties (unless they are parented by wxParentProperty).
wxVariant wxPropertyGridState::DoGetPropertyValues( const wxString& listname,
                                                    wxPGProperty* baseparent,
                                                    long flags ) const
{
    wxPGProperty* pwc = (wxPGProperty*) baseparent;

    // Root is the default base-parent.
    if ( !pwc )
        pwc = m_properties;

    wxVariantList tempList;
    wxVariant v( tempList, listname );

    if ( pwc->GetChildCount() )
    {
        if ( flags & wxPG_KEEP_STRUCTURE )
        {
            wxASSERT( !pwc->IsFlagSet(wxPG_PROP_AGGREGATE) );

            size_t i;
            for ( i=0; i<pwc->GetCount(); i++ )
            {
                wxPGProperty* p = pwc->Item(i);
                if ( !p->GetChildCount() || p->HasFlag(wxPG_PROP_AGGREGATE) )
                {
                    wxVariant variant = p->GetValue();
                    variant.SetName( p->GetBaseName() );
                    v.Append( variant );
                }
                else
                {
                    v.Append( DoGetPropertyValues(p->m_name,p,flags|wxPG_KEEP_STRUCTURE) );
                }
                if ( (flags & wxPG_INC_ATTRIBUTES) && p->m_attributes.GetCount() )
                    v.Append( p->GetAttributesAsList() );
            }
        }
        else
        {
            wxPropertyGridConstIterator it( this, wxPG_ITERATE_DEFAULT, pwc->Item(0) );
            it.SetBaseParent( pwc );

            for ( ; !it.AtEnd(); it.Next() )
            {
                const wxPGProperty* p = it.GetProperty();

                // Use a trick to ignore wxParentProperty itself, but not its sub-properties.
                if ( !p->GetChildCount() || p->HasFlag(wxPG_PROP_AGGREGATE) )
                {
                    wxVariant variant = p->GetValue();
                    variant.SetName( p->GetName() );
                    v.Append( variant );
                    if ( (flags & wxPG_INC_ATTRIBUTES) && p->m_attributes.GetCount() )
                        v.Append( p->GetAttributesAsList() );
                }
            }
        }
    }

    return v;
}

// -----------------------------------------------------------------------

void wxPropertyGridState::DoSetPropertyValues( const wxVariantList& list, wxPGProperty* defaultCategory )
{
    unsigned char origFrozen = 1;

    if ( m_pPropGrid->GetState() == this )
    {
        origFrozen = m_pPropGrid->m_frozen;
        if ( !origFrozen ) m_pPropGrid->Freeze();
    }

    wxPropertyCategory* use_category = (wxPropertyCategory*)defaultCategory;

    if ( !use_category )
        use_category = (wxPropertyCategory*)m_properties;

    // Let's iterate over the list of variants.
    wxVariantList::const_iterator node;
    int numSpecialEntries = 0;

    //
    // Second pass for special entries
    for ( node = list.begin(); node != list.end(); node++ )
    {
        wxVariant *current = (wxVariant*)*node;

        // Make sure it is wxVariant.
        wxASSERT( current );
        wxASSERT( wxStrcmp(current->GetClassInfo()->GetClassName(),wxT("wxVariant")) == 0 );

        const wxString& name = current->GetName();
        if ( name.length() > 0 )
        {
            //
            // '@' signified a special entry
            if ( name[0] == wxT('@') )
            {
                numSpecialEntries++;
            }
            else
            {
                wxPGId foundProp = BaseGetPropertyByName(name);
                if ( wxPGIdIsOk(foundProp) )
                {
                    wxPGProperty* p = wxPGIdToPtr(foundProp);

                    // If it was a list, we still have to go through it.
                    if ( wxStrcmp(current->GetType(), wxT("list")) == 0 )
                    {
                        DoSetPropertyValues( current->GetList(),
                                p->IsCategory()?p:((wxPGProperty*)NULL)
                            );
                    }
                    else
                    {
                #ifdef __WXDEBUG__
                        if ( wxStrcmp(current->GetType(), p->GetValue().GetType()) != 0)
                        {
                            wxLogDebug(wxT("wxPropertyGridState::DoSetPropertyValues Warning: Setting value of property \"%s\" from variant"),
                                p->GetName().c_str());
                            //wxLogDebug(wxT("    but variant's type name (%s) doesn't match either base type name (%s) nor custom type name (%s)."),
                            //    current->GetType().c_str(),vtype->GetTypeName(),
                            //    vtype->GetCustomTypeName());
                        }
                #endif

                        p->SetValue(*current);
                    }
                }
                else
                {
                    // Is it list?
                    if ( current->GetType() != wxT("list") )
                    {
                        // Not.
                        //AppendIn(use_category,current->GetName(),wxPG_LABEL,(wxVariant&)*current);
                        // ERROR: No such property.
                    }
                    else
                    {
                        // Yes, it is; create a sub category and append contents there.
                        wxPGProperty* newCat = DoInsert(use_category,-1,new wxPropertyCategory(current->GetName(),wxPG_LABEL));
                        DoSetPropertyValues( current->GetList(), newCat );
                    }
                }
            }
        }
    }

    if ( numSpecialEntries )
    {
        for ( node = list.begin(); node != list.end(); node++ )
        {
            wxVariant *current = (wxVariant*)*node;

            const wxString& name = current->GetName();
            if ( name.length() > 0 )
            {
                //
                // '@' signified a special entry
                if ( name[0] == wxT('@') )
                {
                    numSpecialEntries--;

                    size_t pos2 = name.rfind(wxT('@'));
                    if ( pos2 > 0 && pos2 < (name.size()-1) )
                    {
                        wxString propName = name.substr(1, pos2-1);
                        wxString entryType = name.substr(pos2+1, wxString::npos);

                        if ( entryType == wxT("attr") )
                        {
                            //
                            // List of attributes
                            wxPGProperty* foundProp = BaseGetPropertyByName(propName);
                            if ( foundProp )
                            {
                                wxASSERT( wxPGIsVariantType(*current, list) );

                                wxVariantList& list2 = current->GetList();
                                wxVariantList::const_iterator node2;

                                for ( node2 = list2.begin(); node2 != list2.end(); node2++ )
                                {
                                    wxVariant *attr = (wxVariant*)*node2;
                                    foundProp->SetAttribute( attr->GetName(), *attr );
                                }
                            }
                            else
                            {
                                // ERROR: No such property: 'propName'
                            }
                        }
                    }
                    else
                    {
                        // ERROR: Special entry requires name of format @<propname>@<entrytype>
                    }
                }
            }

            if ( !numSpecialEntries )
                break;
        }
    }

    if ( !origFrozen )
    {
        m_pPropGrid->Thaw();

        if ( this == m_pPropGrid->GetState() )
        {
            m_selected->UpdateControl(m_pPropGrid->GetEditorControl());
        }
    }

}

// -----------------------------------------------------------------------
// wxPropertyGridState property adding and removal
// -----------------------------------------------------------------------

// Call for after sub-properties added with AddChild
void wxPGProperty::PrepareSubProperties()
{
    wxPropertyGridState* state = GetParentState();

    wxASSERT(state);

    if ( !GetCount() )
        return;

    wxByte depth = m_depth + 1;
    wxByte depthBgCol = m_depthBgCol;

    wxByte inheritFlags = m_flags & wxPG_INHERITED_PROPFLAGS;

    wxByte bgColIndex = m_bgColIndex;
    wxByte fgColIndex = m_fgColIndex;

    //
    // Set some values to the children
    //
    size_t i = 0;
    wxPGProperty* nparent = this;

    while ( i < nparent->GetCount() )
    {
        wxPGProperty* np = nparent->Item(i);

        np->m_parentState = state;
        np->m_flags |= inheritFlags; // Hideable also if parent.
        np->m_depth = depth;
        np->m_depthBgCol = depthBgCol;
        np->m_bgColIndex = bgColIndex;
        np->m_fgColIndex = fgColIndex;

        // Also handle children of children
        if ( np->GetCount() > 0 )
        {
            nparent = np;
            i = 0;

            // Init
            nparent->SetExpanded(false);
            depth++;
        }
        else
        {
            // Next sibling
            i++;
        }

        // After reaching last sibling, go back to processing
        // siblings of the parent
        while ( i >= nparent->GetCount() )
        {
            // Exit the loop when top parent hit
            if ( nparent == this )
                break;

            depth--;

            i = nparent->GetArrIndex() + 1;
            nparent = nparent->GetParent();
        }
    }
}

// -----------------------------------------------------------------------

// Call after fixed sub-properties added/removed after creation.
// if oldSelInd >= 0 and < new max items, then selection is
// moved to it. Note: oldSelInd -2 indicates that this property
// should be selected.
void wxPGProperty::SubPropsChanged( int oldSelInd )
{
    wxPropertyGridState* state = GetParentState();
    wxPropertyGrid* grid = state->GetGrid();

    PrepareSubProperties();

    wxPGProperty* sel = (wxPGProperty*) NULL;
    if ( oldSelInd >= (int)m_children.GetCount() )
        oldSelInd = (int)m_children.GetCount() - 1;

    if ( oldSelInd >= 0 )
        sel = (wxPGProperty*) m_children[oldSelInd];
    else if ( oldSelInd == -2 )
        sel = this;

    if ( sel )
        state->DoSelectProperty(sel);

    if ( state == grid->GetState() )
    {
        grid->GetPanel()->Refresh();
    }
}

// -----------------------------------------------------------------------

int wxPropertyGridState::PrepareToAddItem( wxPGProperty* property,
                                           wxPGProperty* scheduledParent )
{
    wxPropertyGrid* propGrid = m_pPropGrid;

#if wxPG_COMPATIBILITY_1_2_0
    // Make sure deprecated functions are not implemented
    wxASSERT_MSG( property->GetImageSize().x == -1234,
                  wxT("Custom properties implement OnMeasureImage instead of GetImageSize") );
    int oldCommonVal = property->m_commonValue;
    property->m_commonValue = -1;
    property->SetValueFromString( wxEmptyString, 0xFFFF );
    wxASSERT_MSG( property->m_commonValue == -1234,
                  wxT("Implement StringToValue() instead of SetValueFromString()") );
    property->m_commonValue = -1;
    property->SetValueFromInt( 0, 0xFFFF );
    wxASSERT_MSG( property->m_commonValue == -1234,
                  wxT("Implement StringToValue() instead of SetValueFromString()") );
    property->m_commonValue = oldCommonVal;
#endif

    // This will allow better behaviour.
    if ( scheduledParent == m_properties )
        scheduledParent = (wxPGProperty*) NULL;

    property->m_parentState = this;

    if ( property->IsCategory() )
    {

        // Parent of a category must be either root or another category
        // (otherwise Bad Things might happen).
        wxASSERT_MSG( scheduledParent == NULL ||
                      scheduledParent == m_properties ||
                      scheduledParent->IsCategory(),
                 wxT("Parent of a category must be either root or another category."));

        /*
        wxASSERT_MSG( m_properties == &m_regularArray,
                wxT("Do not add categories in non-categoric mode!"));
        */

        // If we already have category with same name, delete given property
        // and use it instead as most recent caption item.
        wxPGId found_id = BaseGetPropertyByName( property->GetBaseName() );
        if ( wxPGIdIsOk(found_id) )
        {
            wxPropertyCategory* pwc = (wxPropertyCategory*)wxPGIdToPtr(found_id);
            if ( pwc->IsCategory() ) // Must be a category.
            {
                delete property;
                m_currentCategory = pwc;
                return 2; // Tells the caller what we did.
            }
        }
    }

#ifdef __WXDEBUG__
    // Warn for identical names in debug mode.
    if ( BaseGetPropertyByName(property->GetName()) &&
         (!scheduledParent || scheduledParent->IsCategory()) )
    {
        wxLogError(wxT("wxPropertyGrid: Warning - item with name \"%s\" already exists."),
            property->GetName().c_str());
        wxPGGlobalVars->m_warnings++;
    }
#endif

    // Make sure nothing is selected.
    if ( propGrid && propGrid->m_selected )
    {
        bool selRes = propGrid->ClearSelection();
        wxPG_CHECK_MSG_DBG( selRes,
                            -1,
                            wxT("failed to deselect a property (editor probably had invalid value)") );
    }

    if ( scheduledParent )
    {
        // Use parent's colours.
        property->m_bgColIndex = scheduledParent->m_bgColIndex;
        property->m_fgColIndex = scheduledParent->m_fgColIndex;

        // Fix no parent does not yet have parenting flag yet, set one now
        if ( !scheduledParent->HasFlag(wxPG_PROP_PARENTAL_FLAGS) )
            scheduledParent->SetParentalType(wxPG_PROP_MISC_PARENT);
            //scheduledParent->SetFlag(wxPG_PROP_MISC_PARENT);
    }

    // If in hideable adding mode, or if assigned parent is hideable, then
    // make this one hideable.
    if (
         ( scheduledParent && (scheduledParent->m_flags & wxPG_PROP_HIDDEN) ) ||
         ( propGrid && (propGrid->m_iFlags & wxPG_FL_ADDING_HIDEABLES) )
       )
        property->SetFlag( wxPG_PROP_HIDDEN );

    // Set custom image flag.
    int custImgHeight = property->OnMeasureImage().y;
    if ( custImgHeight < 0 /*|| custImgHeight > 1*/ )
    {
        property->m_flags |= wxPG_PROP_CUSTOMIMAGE;
    }

    if ( propGrid && (propGrid->GetWindowStyleFlag() & wxPG_LIMITED_EDITING) )
        property->m_flags |= wxPG_PROP_NOEDITOR;

    if ( !property->IsCategory() )
    {
        // This is not a category.

        //wxASSERT_MSG( property->GetEditorClass(), wxT("Editor class not initialized!") );

        // Depth.
        //
        unsigned char depth = 1;
        if ( scheduledParent )
        {
            depth = scheduledParent->m_depth;
            if ( !scheduledParent->IsCategory() )
                depth++;
        }
        property->m_depth = depth;
        unsigned char greyDepth = depth;

        if ( scheduledParent )
        {
            wxPropertyCategory* pc;

            if ( scheduledParent->IsCategory() || scheduledParent->IsRoot() )
                pc = (wxPropertyCategory*)scheduledParent;
            else
                // This conditional compile is necessary to
                // bypass some compiler bug.
                pc = GetPropertyCategory(scheduledParent);

            if ( pc )
                greyDepth = pc->GetDepth();
            else
                greyDepth = scheduledParent->m_depthBgCol;
        }

        property->m_depthBgCol = greyDepth;

        // Prepare children pre-added children
        if ( property->GetCount() )
        {
            property->SetParentalType(wxPG_PROP_AGGREGATE);

            property->SetExpanded(false); // Properties with children are not expanded by default.
            if ( propGrid && propGrid->GetWindowStyleFlag() & wxPG_HIDE_MARGIN )
                property->SetExpanded(true); // ...unless it cannot be expanded.

            property->PrepareSubProperties();

            return -1;
        }

        return 0;
    }
    else
    {
        // This is a category.

        // depth
        unsigned char depth = 1;
        if ( scheduledParent )
        {
            depth = scheduledParent->m_depth + 1;
        }
        property->m_depth = depth;
        property->m_depthBgCol = depth;

        m_currentCategory = (wxPropertyCategory*)property;

        wxPropertyCategory* pc = (wxPropertyCategory*)property;

        // Calculate text extent for caption item.
        if ( propGrid )
            pc->CalculateTextExtent(propGrid, propGrid->GetCaptionFont());

        return 1;
    }
}

// -----------------------------------------------------------------------

void wxPropertyContainerMethods::BeginAddChildren( wxPGPropArg id )
{
    wxPG_PROP_ARG_CALL_PROLOG()
    wxCHECK_RET( p->HasFlag(wxPG_PROP_AGGREGATE), wxT("only call on properties with fixed children") );
    p->ClearFlag(wxPG_PROP_AGGREGATE);
    p->SetFlag(wxPG_PROP_MISC_PARENT);
}

// -----------------------------------------------------------------------

void wxPropertyContainerMethods::EndAddChildren( wxPGPropArg id )
{
    wxPG_PROP_ARG_CALL_PROLOG()
    wxCHECK_RET( p->HasFlag(wxPG_PROP_MISC_PARENT), wxT("only call on properties for which BeginAddChildren was called prior") );
    p->ClearFlag(wxPG_PROP_MISC_PARENT);
    p->SetFlag(wxPG_PROP_AGGREGATE);
}

// -----------------------------------------------------------------------

wxPGProperty* wxPropertyGridState::DoAppend( wxPGProperty* property )
{
    wxPropertyCategory* cur_cat = m_currentCategory;
    if ( property->IsCategory() )
        cur_cat = (wxPropertyCategory*) NULL;

    return DoInsert( cur_cat, -1, property );
}

// -----------------------------------------------------------------------

wxPGProperty* wxPropertyGridState::DoInsert( wxPGProperty* parent, int index, wxPGProperty* property )
{
    if ( !parent )
        parent = m_properties;

    wxCHECK_MSG( !parent->HasFlag(wxPG_PROP_AGGREGATE),
                 wxNullProperty,
                 wxT("when adding properties to fixed parents, use BeginAddChildren and EndAddChildren.") );

    int parenting = PrepareToAddItem( property, (wxPropertyCategory*)parent );

    // This type of invalid parenting value indicates we should exit now, returning
    // id of most recent category.
    if ( parenting > 1 )
        return m_currentCategory;

    // Note that item must be added into current mode later.

    // If parent is wxParentProperty, just stick it in...
    // If parent is root (m_properties), then...
    //   In categoric mode: Add as last item in m_abcArray (if not category).
    //                      Add to given index in m_regularArray.
    //   In non-cat mode:   Add as last item in m_regularArray.
    //                      Add to given index in m_abcArray.
    // If parent is category, then...
    //   1) Add to given category in given index.
    //   2) Add as last item in m_abcArray.

    wxPGProperty* pwc = (wxPGProperty*) property;

    if ( !parent->IsCategory() && !parent->IsRoot() )
    {
        // Parent is wxParentingProperty: Just stick it in...
        parent->AddChild2( property, index );
    }
    else
    {
        // Parent is Category or Root.

        if ( m_properties == &m_regularArray )
        {
            // Categorized mode

            // Only add non-categories to m_abcArray.
            if ( m_abcArray && parenting <= 0 )
                m_abcArray->AddChild2( property, -1, false );

            // Add to current mode.
            parent->AddChild2( property, index );

        }
        else
        {
            // Non-categorized mode.

            if ( parent != m_properties )
                // Parent is category.
                parent->AddChild2( property, index, false );
            else
                // Parent is root.
                m_regularArray.AddChild2( property, -1, false );

            // Add to current mode (no categories).
            if ( parenting <= 0 )
                m_abcArray->AddChild2( property, index );
        }
    }

    // category stuff
    if ( property->IsCategory() )
    {
        // This is a category caption item.

        // Last caption is not the bottom one (this info required by append)
        m_lastCaptionBottomnest = 0;
    }

    // Only add name to hashmap if parent is root or category
    if ( (parent->IsCategory() || parent->IsRoot()) && property->m_name.length() )
        m_dictName[property->m_name] = (void*) property;

    // Increment m_bottomY
    if ( m_pPropGrid && parent->IsVisible() && parent->IsExpanded() )
    {
        int lh;

        lh = m_pPropGrid->m_lineHeight;

        m_bottomY += lh;
        if ( parenting != 0 && pwc->IsExpanded() )
            m_bottomY += pwc->GetChildrenHeight(lh);
    }

    m_itemsAdded = 1;

    return property;
}

// -----------------------------------------------------------------------

void wxPropertyGridState::DoDelete( wxPGProperty* item )
{
    wxCHECK_RET( item->GetParent(),
        wxT("this property was already deleted") );

    wxCHECK_RET( item != &m_regularArray && item != m_abcArray,
        wxT("wxPropertyGrid: Do not attempt to remove the root item.") );

    size_t i;
    unsigned int indinparent = item->GetIndexInParent();

    wxPGProperty* pwc = (wxPGProperty*)item;

    wxCHECK_RET( !item->GetParent()->HasFlag(wxPG_PROP_AGGREGATE),
        wxT("wxPropertyGrid: Do not attempt to remove sub-properties.") );

    if ( item->IsCategory() )
    {
        // deleting a category

        // erase category entries from the hash table
        for ( i=0; i<pwc->GetCount(); i++ )
        {
            wxPGProperty* sp = pwc->Item( i );
            if ( sp->GetBaseName().Len() ) m_dictName.erase( wxPGNameConv(sp->GetBaseName()) );
        }

        if ( pwc == m_currentCategory )
            m_currentCategory = (wxPropertyCategory*) NULL;

        if ( m_abcArray )
        {
            // Remove children from non-categorized array.
            for ( i=0; i<pwc->GetCount(); i++ )
            {
                wxPGProperty * p = pwc->Item( i );
                wxASSERT( p != NULL );
                if ( !p->IsCategory() )
                    m_abcArray->m_children.Remove( p );
            }

            if ( IsInNonCatMode() )
                m_abcArray->FixIndexesOfChildren();
        }
    }

    if ( !IsInNonCatMode() )
    {
        // categorized mode - non-categorized array

        // Remove from non-cat array, but only if parent is in it
        if ( !item->IsCategory() && item->GetParent()->IsCategory() )
        {
            if ( m_abcArray )
            {
                m_abcArray->m_children.Remove( item );
            }
        }

        // categorized mode - categorized array
        item->m_parent->m_children.RemoveAt(indinparent);
        item->m_parent->FixIndexesOfChildren(/*indinparent*/);
    }
    else
    {
        // non-categorized mode - categorized array

        // We need to find location of item.
        wxPGProperty* cat_parent = &m_regularArray;
        int cat_index = m_regularArray.GetCount();
        size_t i;
        for ( i = 0; i < m_regularArray.GetCount(); i++ )
        {
            wxPGProperty* p = m_regularArray.Item(i);
            if ( p == item ) { cat_index = i; break; }
            if ( p->IsCategory() )
            {
                int subind = ((wxPGProperty*)p)->Index(item);
                if ( subind != wxNOT_FOUND )
                {
                    cat_parent = ((wxPGProperty*)p);
                    cat_index = subind;
                    break;
                }
            }
        }
        cat_parent->m_children.RemoveAt(cat_index);

        // non-categorized mode - non-categorized array
        if ( !item->IsCategory() )
        {
            wxASSERT( item->m_parent == m_abcArray );
            item->m_parent->m_children.RemoveAt(indinparent);
            item->m_parent->FixIndexesOfChildren(indinparent);
        }
    }

    if ( item->GetBaseName().Len() ) m_dictName.erase( wxPGNameConv(item->GetBaseName()) );

#ifdef __WXPYTHON__
    // For some reason, Py_DECREF always crashes, even though we make
    // matching Py_INCREF call in propgrid_cbacks.cpp. Maybe refcount is decremented
    // somewhere automatically? Unlikely though...
    //if ( item->m_scriptObject )
    //    Py_DECREF( item->m_scriptObject );
#endif

    // We can actually delete it now
    delete item;

    m_itemsAdded = 1; // Not a logical assignment (but required nonetheless).
}

// -----------------------------------------------------------------------
// wxPropertyGridState init etc.
// -----------------------------------------------------------------------

void wxPropertyGridState::InitNonCatMode()
{
    if ( !m_abcArray )
    {
        m_abcArray = new wxPGRootProperty();
        m_abcArray->SetParentState(this);
        m_abcArray->SetFlag(wxPG_PROP_CHILDREN_ARE_COPIES);
    }

    // Must be called when state::m_properties still points to regularArray.
    wxPGProperty* oldProperties = m_properties;

    // Must use temp value in state::m_properties for item iteration loop
    // to run as expected.
    m_properties = &m_regularArray;

    if ( m_properties->GetChildCount() )
    {
        // Copy items.
        wxPropertyGridIterator it( this, wxPG_ITERATE_DEFAULT|wxPG_ITERATE_CATEGORIES );

        for ( ; !it.AtEnd(); it.Next() )
        {
            wxPGProperty* p = it.GetProperty();
            wxPGProperty* parent = p->GetParent();
            if ( p->HasFlag(wxPG_PROP_MISC_PARENT) &&
                ( parent == m_properties || (parent->IsCategory() || parent->IsRoot()) ) )
            {
                m_abcArray->AddChild2( p );
                p->m_parent = &m_regularArray;
            }
        }
    }

    m_properties = oldProperties;
}

// -----------------------------------------------------------------------

void wxPropertyGridState::Clear()
{
    m_regularArray.Empty();
    if ( m_abcArray )
        m_abcArray->Empty();

    m_dictName.clear();

    m_currentCategory = (wxPropertyCategory*) NULL;
    m_lastCaptionBottomnest = 1;
    m_itemsAdded = 0;

    m_selected = (wxPGProperty*) NULL;
}

// -----------------------------------------------------------------------

wxPropertyGridState::wxPropertyGridState()
{
    m_pPropGrid = (wxPropertyGrid*) NULL;
    m_regularArray.SetParentState(this);
    m_properties = &m_regularArray;
    m_abcArray = (wxPGRootProperty*) NULL;
    m_currentCategory = (wxPropertyCategory*) NULL;
    m_selected = (wxPGProperty*) NULL;
    m_width = 0;
    m_bottomY = 0;
    m_lastCaptionBottomnest = 1;
    m_itemsAdded = 0;
    m_anyModified = 0;
    m_colWidths.push_back( wxPG_DEFAULT_SPLITTERX );
    m_colWidths.push_back( wxPG_DEFAULT_SPLITTERX );
    m_fSplitterX = wxPG_DEFAULT_SPLITTERX;
}

// -----------------------------------------------------------------------

wxPropertyGridState::~wxPropertyGridState()
{
    delete m_abcArray;
}

// -----------------------------------------------------------------------
// wxPropertyGridPopulator
// -----------------------------------------------------------------------

wxPropertyGridPopulator::wxPropertyGridPopulator()
{
    m_state = NULL;
    m_pg = NULL;
    wxPGGlobalVars->m_offline++;
}

// -----------------------------------------------------------------------

void wxPropertyGridPopulator::SetState( wxPropertyGridState* state )
{
    m_state = state;
    m_propHierarchy.clear();
}

// -----------------------------------------------------------------------

void wxPropertyGridPopulator::SetGrid( wxPropertyGrid* pg )
{
    m_pg = pg;
    pg->Freeze();
}

// -----------------------------------------------------------------------

wxPropertyGridPopulator::~wxPropertyGridPopulator()
{
    //
    // Free unused sets of choices
    wxPGHashMapS2P::iterator it;

    for( it = m_dictIdChoices.begin(); it != m_dictIdChoices.end(); ++it )
    {
        wxPGChoicesData* data = (wxPGChoicesData*) it->second;
        data->DecRef();
    }

    if ( m_pg )
    {
        m_pg->Thaw();
        m_pg->GetPanel()->Refresh();
    }
    wxPGGlobalVars->m_offline--;
}

// -----------------------------------------------------------------------

wxPGProperty* wxPropertyGridPopulator::Add( const wxString& propClass,
                                            const wxString& propLabel,
                                            const wxString& propName,
                                            const wxString* propValue,
                                            wxPGChoices* pChoices )
{
    wxClassInfo* classInfo = wxClassInfo::FindClass(propClass);
    wxPGProperty* parent = GetCurParent();

    if ( parent->HasFlag(wxPG_PROP_AGGREGATE) )
    {
        ProcessError(wxString::Format(wxT("new children cannot be added to '%s'"),parent->GetName().c_str()));
        return NULL;
    }

    if ( !classInfo || !classInfo->IsKindOf(CLASSINFO(wxPGProperty)) )
    {
        ProcessError(wxString::Format(wxT("'%s' is not valid property class"),propClass.c_str()));
        return NULL;
    }

    wxPGProperty* property = (wxPGProperty*) classInfo->CreateObject();

    property->SetLabel(propLabel);
    property->DoSetName(propName);

    if ( pChoices && pChoices->IsOk() )
        property->SetChoices(*pChoices);

    m_state->DoInsert(parent, -1, property);

    if ( propValue )
        property->SetValueFromString( *propValue, wxPG_FULL_VALUE );

    return property;
}

// -----------------------------------------------------------------------

void wxPropertyGridPopulator::AddChildren( wxPGProperty* property )
{
    m_propHierarchy.push_back(property);
    DoScanForChildren();
    m_propHierarchy.pop_back();
}

// -----------------------------------------------------------------------

wxPGChoices wxPropertyGridPopulator::ParseChoices( const wxString& choicesString,
                                                   const wxString& idString )
{
    wxPGChoices choices;

    // Using id?
    if ( choicesString[0] == wxT('@') )
    {
        wxString ids = choicesString.substr(1);
        wxPGHashMapS2P::iterator it = m_dictIdChoices.find(ids);
        if ( it == m_dictIdChoices.end() )
            ProcessError(wxString::Format(wxT("No choices defined for id '%s'"),ids.c_str()));
        else
            choices.AssignData((wxPGChoicesData*)it->second);
    }
    else
    {
        bool found = false;
        if ( idString.length() )
        {
            wxPGHashMapS2P::iterator it = m_dictIdChoices.find(idString);
            if ( it != m_dictIdChoices.end() )
            {
                choices.AssignData((wxPGChoicesData*)it->second);
                found = true;
            }
        }

        if ( !found )
        {
            // Parse choices string
            wxString::const_iterator it = choicesString.begin();
            wxString label;
            wxString value;
            int state = 0;
            bool labelValid = false;

            for ( ; it != choicesString.end(); it++ )
            {
                wxChar c = *it;

                if ( state != 1 )
                {
                    if ( c == wxT('"') )
                    {
                        if ( labelValid )
                        {
                            long l;
                            if ( !value.ToLong(&l, 0) ) l = wxPG_INVALID_VALUE;
                            choices.Add(label, l);
                        }
                        labelValid = false;
                        //wxLogDebug(wxT("%s, %s"),label.c_str(),value.c_str());
                        value.clear();
                        label.clear();
                        state = 1;
                    }
                    else if ( c == wxT('=') )
                    {
                        if ( labelValid )
                        {
                            state = 2;
                        }
                    }
                    else if ( state == 2 && (wxIsalnum(c) || c == wxT('x')) )
                    {
                        value << c;
                    }
                }
                else
                {
                    if ( c == wxT('"') )
                    {
                        state = 0;
                        labelValid = true;
                    }
                    else
                        label << c;
                }
            }

            if ( labelValid )
            {
                long l;
                if ( !value.ToLong(&l, 0) ) l = wxPG_INVALID_VALUE;
                choices.Add(label, l);
            }

            if ( !choices.IsOk() )
            {
                choices.EnsureData();
            }

            // Assign to id
            if ( idString.length() )
                m_dictIdChoices[idString] = choices.GetData();
        }
    }

    return choices;
}

// -----------------------------------------------------------------------

bool wxPropertyGridPopulator::ToLongPCT( const wxString& s, long* pval, long max )
{
    if ( s.Last() == wxT('%') )
    {
        wxString s2 = s.substr(0,s.length()-1);
        long val;
        if ( s2.ToLong(&val, 10) )
        {
            *pval = (val*max)/100;
            return true;
        }
        return false;
    }

    return s.ToLong(pval, 10);
}

// -----------------------------------------------------------------------

bool wxPropertyGridPopulator::AddAttribute( const wxString& name,
                                            const wxString& type,
                                            const wxString& value )
{
    int l = m_propHierarchy.size();
    if ( !l )
        return false;

    wxPGProperty* p = m_propHierarchy[l-1];
    wxString valuel = value.Lower();
    wxVariant variant;

    if ( type.length() == 0 )
    {
        long v;

        // Auto-detect type
        if ( valuel == wxT("true") || valuel == wxT("yes") || valuel == wxT("1") )
            variant = true;
        else if ( valuel == wxT("false") || valuel == wxT("no") || valuel == wxT("0") )
            variant = false;
        else if ( value.ToLong(&v, 0) )
            variant = v;
        else
            variant = value;
    }
    else
    {
        if ( type == wxT("string") )
        {
            variant = value;
        }
        else if ( type == wxT("int") )
        {
            long v = 0;
            value.ToLong(&v, 0);
            variant = v;
        }
        else if ( type == wxT("bool") )
        {
            if ( valuel == wxT("true") || valuel == wxT("yes") || valuel == wxT("1") )
                variant = true;
            else
                variant = false;
        }
        else
        {
            ProcessError(wxString::Format(wxT("Invalid attribute type '%s'"),type.c_str()));
            return false;
        }
    }

    p->SetAttribute( name, variant );

    return true;
}

// -----------------------------------------------------------------------

void wxPropertyGridPopulator::ProcessError( const wxString& msg )
{
    wxLogError(_("Error in resource: %s"),msg.c_str());
}

// -----------------------------------------------------------------------
// Implementation of various deprecated methods that were inline,
// made non-inline to eliminate problems with wxDEPRECATED.
// -----------------------------------------------------------------------

#if wxPG_COMPATIBILITY_1_2_0

wxPGProperty* wxPropertyContainerMethods::AppendCategory( const wxString& label, const wxString& name )
{
    return Append( new wxPropertyCategory(label,name) );
}
             
void wxPropertyContainerMethods::Delete( wxPGPropArg id )
{
    DeleteProperty(id);
}

bool wxPropertyContainerMethods::Disable( wxPGPropArg id )
{
    return EnableProperty(id,false);
}

bool wxPropertyContainerMethods::IsPropertyValueType( wxPGPropArg id, const wxChar* typestr ) const
{
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(false)
    return (wxStrcmp(p->GetValue().GetType(),typestr) == 0);
}

bool wxPropertyContainerMethods::IsModified( wxPGPropArg id ) const
{
    return IsPropertyModified(id);
}

void wxPropertyGrid::ClearTargetPage()
{
    Clear();
}

bool wxPropertyGrid::IsPropertySelected( wxPGPropArg id ) const
{
    wxPG_PROP_ARG_CALL_PROLOG_RETVAL(false)
    return ( m_selected == p ) ? true : false;
}

void wxPropertyGrid::SetPropertyColour( wxPGPropArg id, const wxColour& col )
{
    SetPropertyBackgroundColour( id, col );
}

#endif  // wxPG_COMPATIBILITY_1_2_0

// -----------------------------------------------------------------------

