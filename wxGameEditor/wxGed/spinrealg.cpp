/////////////////////////////////////////////////////////////////////////////
// Name:        spinbutt.cpp
// Purpose:     wxSpinReal
// Author:      Robert Roebling, John Labenski
// Modified by:
// RCS-ID:      $Id: spinrealg.cpp,v 1.1.1.1 2003/12/28 21:05:21 wyo Exp $
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "spinreal.h"
#endif

#include "spinreal.h"

#if wxUSE_SPINCTRL

#include "wx/utils.h"

#include <math.h>

#include <gdk/gdk.h>
#include <gtk/gtk.h>

//-----------------------------------------------------------------------------
// idle system
//-----------------------------------------------------------------------------

extern void wxapp_install_idle_handler();
extern bool g_isIdle;

//-----------------------------------------------------------------------------
// data
//-----------------------------------------------------------------------------

extern bool   g_blockEventsOnDrag;

//-----------------------------------------------------------------------------
// "value_changed"
//-----------------------------------------------------------------------------

static void gtk_spinreal_callback( GtkWidget *WXUNUSED(widget), wxSpinReal *win )
{
    if (g_isIdle) wxapp_install_idle_handler();

    if (!win->m_hasVMT) return;
    if (g_blockEventsOnDrag) return;

    wxCommandEvent event( wxEVT_COMMAND_SPINCTRL_UPDATED, win->GetId());
    event.SetEventObject( win );
    event.SetInt( (int)win->GetValue() ); // just for fun, unused
    wxString wxstr;
    wxstr.Printf("%f", win->GetValue()); 
    event.SetString( wxstr );            // actually the value
    win->GetEventHandler()->ProcessEvent( event );
}

//-----------------------------------------------------------------------------
//  "changed"
//-----------------------------------------------------------------------------

static void
gtk_spinreal_text_changed_callback( GtkWidget *WXUNUSED(widget), wxSpinReal *win )
{

// This can be handled by gtk_spin_button_update, in OnChar for ENTER
// with this code the spinctrl is updated every keypress

//    if (!win->m_hasVMT) return;

//    if (g_isIdle)
//        wxapp_install_idle_handler();

//    wxCommandEvent event( wxEVT_COMMAND_TEXT_UPDATED, win->GetId() );
//    event.SetEventObject( win );
//    event.SetInt( (int)win->GetValue() ); // unused
//    wxString wxstr;
//    wxstr.Printf("%f", win->GetValue()); // actual value
//    event.SetString( wxstr );
//    win->GetEventHandler()->ProcessEvent( event );
}

//-----------------------------------------------------------------------------
// wxSpinReal
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxSpinReal,wxControl)

BEGIN_EVENT_TABLE(wxSpinReal, wxControl)
    EVT_CHAR(wxSpinReal::OnChar)
    EVT_KEY_DOWN(wxSpinReal::OnKey)
    EVT_KEY_UP(wxSpinReal::OnKey)
END_EVENT_TABLE()

bool wxSpinReal::Create(wxWindow *parent, wxWindowID id,
                        const wxString& value,
                        const wxPoint& pos,  const wxSize& size,
                        long style,
                        float min, float max,
                        float initial,
                        float increment, int digits,
                        const wxString& name)
{
    m_needParent = TRUE;
    m_acceptsFocus = TRUE;

    if (!PreCreation( parent, pos, size ) ||
        !CreateBase( parent, id, pos, size, style, wxDefaultValidator, name ))
    {
        wxFAIL_MSG( wxT("wxSpinReal creation failed") );
        return FALSE;
    }

    increment = fabs(increment);
    m_increment = increment;
    if (digits == -1) digits = DoGetDigits(increment);

    m_adjust = (GtkAdjustment*) gtk_adjustment_new( initial, min, max,
                                                    increment, 10.0*increment, 0.0);

    m_widget = gtk_spin_button_new( m_adjust, increment, digits );

    gtk_spin_button_set_wrap( GTK_SPIN_BUTTON(m_widget),
                              (int)(m_windowStyle & wxSP_WRAP) );

    gtk_spin_button_set_snap_to_ticks( GTK_SPIN_BUTTON(m_widget), FALSE );
    // update_policy options  GTK_UPDATE_ALWAYS, GTK_UPDATE_IF_VALID
    gtk_spin_button_set_update_policy( GTK_SPIN_BUTTON(m_widget), GTK_UPDATE_IF_VALID );

    GtkEnableEvents();

    m_parent->DoAddChild( this );

    PostCreation();

    SetFont( parent->GetFont() );

    wxSize size_best( DoGetBestSize() );
    wxSize new_size( size );
    if (new_size.x == -1)
        new_size.x = size_best.x;
    if (new_size.y == -1)
        new_size.y = size_best.y;
    if (new_size.y > size_best.y)
        new_size.y = size_best.y;
    if ((new_size.x != size.x) || (new_size.y != size.y))
        SetSize( new_size.x, new_size.y );

    SetBackgroundColour( parent->GetBackgroundColour() );

    if ( !value.IsEmpty() ) SetValue( value );  //else keep initial value

    Show( TRUE );

    return TRUE;
}

void wxSpinReal::GtkDisableEvents()
{
    gtk_signal_disconnect_by_func( GTK_OBJECT(m_adjust),
                        GTK_SIGNAL_FUNC(gtk_spinreal_callback),
                        (gpointer) this );

    gtk_signal_disconnect_by_func( GTK_OBJECT(m_widget),
                        GTK_SIGNAL_FUNC(gtk_spinreal_text_changed_callback),
                        (gpointer) this );
}

void wxSpinReal::GtkEnableEvents()
{
    gtk_signal_connect( GTK_OBJECT(m_adjust),
                        "value_changed",
                        GTK_SIGNAL_FUNC(gtk_spinreal_callback),
                        (gpointer) this );

    gtk_signal_connect( GTK_OBJECT(m_widget),
                        "changed",
                        GTK_SIGNAL_FUNC(gtk_spinreal_text_changed_callback),
                        (gpointer)this);
}

float wxSpinReal::GetMin() const
{
    wxCHECK_MSG( (m_widget != NULL), 0, wxT("invalid spin button") );

    return m_adjust->lower;
}

float wxSpinReal::GetMax() const
{
    wxCHECK_MSG( (m_widget != NULL), 0, wxT("invalid spin button") );

    return m_adjust->upper;
}

float wxSpinReal::GetIncrement() const
{
    wxCHECK_MSG( (m_widget != NULL), 0, wxT("invalid spin button") );

    return m_adjust->step_increment;
}

int wxSpinReal::GetDigits() const
{
    wxCHECK_MSG( (m_widget != NULL), 0, wxT("invalid spin button") );

    return GTK_SPIN_BUTTON(m_widget)->digits;
}

float wxSpinReal::GetValue() const
{
    wxCHECK_MSG( (m_widget != NULL), 0, wxT("invalid spin button") );

    gtk_spin_button_update( GTK_SPIN_BUTTON(m_widget) );

    return m_adjust->value;
}

bool wxSpinReal::GetSnap() const
{
    wxCHECK_MSG( (m_widget != NULL), 0, wxT("invalid spin button") );

    return bool(GTK_SPIN_BUTTON(m_widget)->snap_to_ticks);
}

void wxSpinReal::SetValue( const wxString& value )
{
    wxCHECK_RET( (m_widget != NULL), wxT("invalid spin button") );

    float n;
    if ( (wxSscanf(value, wxT("%f"), &n) == 1) )
    {
        // a number - set it
        SetValue(n);
    }
    else
    {
        // invalid number - set text as is (wxMSW compatible)
        GtkDisableEvents();
        gtk_entry_set_text( GTK_ENTRY(m_widget), value.mbc_str() );
        GtkEnableEvents();
    }
}

void wxSpinReal::SetValue( float value )
{
    wxCHECK_RET( (m_widget != NULL), wxT("invalid spin button") );

    m_adjust->value = value;

    GtkDisableEvents();
    gtk_signal_emit_by_name( GTK_OBJECT(m_adjust), "value_changed" );
    GtkEnableEvents();
}

void wxSpinReal::SetRange(float minVal, float maxVal)
{
    wxCHECK_RET( (m_widget != NULL), wxT("invalid spin button") );

    m_adjust->lower = minVal;
    m_adjust->upper = maxVal;

    gtk_signal_emit_by_name( GTK_OBJECT(m_adjust), "changed" );

    // these two calls are required due to some bug in GTK
    Refresh();
    SetFocus();
}

void wxSpinReal::SetIncrement(float increment, int digits)
{
    wxCHECK_RET( (m_widget != NULL), wxT("invalid spin button") );

    increment = fabs(increment);
    if ( digits == -1 ) DoGetDigits( increment );

    if ( (digits<0) || (digits>5)) digits = 5;

    gtk_spin_button_set_digits(GTK_SPIN_BUTTON(m_widget), digits);
    m_adjust->step_increment = increment;
    m_adjust->page_increment = 10.0 * increment;
    m_increment = increment;
}

void wxSpinReal::SetSnap(bool snap_to_increment)
{
    wxCHECK_RET( (m_widget != NULL), wxT("invalid spin button") );

    gtk_spin_button_set_snap_to_ticks( GTK_SPIN_BUTTON(m_widget), snap_to_increment );
}

void wxSpinReal::OnChar( wxKeyEvent &event )
{
    wxCHECK_RET( m_widget != NULL, wxT("invalid spin ctrl") );

    if (event.KeyCode() == WXK_RETURN)
    {
        gtk_spin_button_update(GTK_SPIN_BUTTON(m_widget));

        // I don't understand this??? the above works nicely, Labenski
        // sends only one event, when you're done editing

        //wxWindow *top_frame = m_parent;
        //while (top_frame->GetParent() && !(top_frame->GetParent()->IsTopLevel()))
        //    top_frame = top_frame->GetParent();
        //GtkWindow *window = GTK_WINDOW(top_frame->m_widget);

        //if (window->default_widget)
        //{
        //    gtk_widget_activate (GTK_WIDGET(window->default_widget));
        //    return;
        //}
    }
    event.Skip();
}
void wxSpinReal::OnKey( wxKeyEvent &event )
{

    // Check for accelerator keys, shift, ctrl
    static float lastmodifier = 1.0;
    float modifier = 1.0;

    // in GTK XxxDown() & m_xxxDown reports state before keypress
    // in MSW report state after keypress
    if ( event.KeyCode() == WXK_SHIFT )
    {
        // if this is the first time shift is pressed
        if ( !event.ShiftDown() ) modifier = 10.0;
    }
    // if shift is being held down
    else if ( event.ShiftDown() ) modifier = 10.0;

    if ( event.KeyCode() == WXK_CONTROL )
    {
        if ( !event.ControlDown() ) modifier *= 100.0;
    }
    else if ( event.ControlDown() ) modifier *= 100.0;

    // note: CTRL PG-UP/DOWN in gtk goes to MAX/MIN
    //       ALT key reserved in MSW for menubar

    if ( lastmodifier != modifier )
    {
        //adjust it only when needed
        m_adjust->step_increment = m_increment * modifier;
        m_adjust->page_increment = 10.0 * m_increment * modifier;
        lastmodifier = modifier;
    }

    event.Skip();
}

bool wxSpinReal::IsOwnGtkWindow( GdkWindow *window )
{
    return GTK_SPIN_BUTTON(m_widget)->panel == window;
}

void wxSpinReal::ApplyWidgetStyle()
{
    SetWidgetStyle();
    gtk_widget_set_style( m_widget, m_widgetStyle );
}

wxSize wxSpinReal::DoGetBestSize() const
{
    wxSize ret( wxControl::DoGetBestSize() );
    return wxSize(95, ret.y);
}

int wxSpinReal::DoGetDigits( float increment )
{
    // calculate the number of digits to show from increment
    // MAX allowed digits in GTK is 5, MIN is 0
    wxString increment_str;
    increment_str.Printf("%f", increment);
    int digits = increment_str.Find('.');

    int i = 0;
    for ( i=increment_str.Len()-1; i>digits; i-- ){
        if ( increment_str.GetChar(i) != '0' ) break;
    }
    digits = i - digits;

    // sanity check
    if ( (digits < 0) ||  (digits > 5) ) digits = 5;

    return digits;
}

#endif
   // wxUSE_SPINCTRL
