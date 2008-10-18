/////////////////////////////////////////////////////////////////////////////
// Name:        spinreal.h
// Purpose:     wxSpinReal class
// Author:      Robert Roebling, John Labenski
// Modified by:
// RCS-ID:      $Id: spinrealg.h,v 1.1.1.1 2003/12/28 21:05:21 wyo Exp $
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __GTKSPINREALH__
#define __GTKSPINREALH__

#ifdef __GNUG__
#pragma interface
#endif

#include "wx/defs.h"

#if wxUSE_SPINCTRL

#include "wx/control.h"

//-----------------------------------------------------------------------------
// wxSpinReal
//-----------------------------------------------------------------------------

class wxSpinReal : public wxControl
{
public:
    wxSpinReal() {}

    wxSpinReal(wxWindow *parent,
               wxWindowID id = -1,
               const wxString& value = wxEmptyString,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxSP_ARROW_KEYS,
               float min = 0.0, float max = 100.0,
               float initial = 0.0,
               float increment = .1, int digits = -1,
               const wxString& name = _T("wxSpinReal"))
    {
        Create(parent, id, value, pos, size, style,
               min, max, initial, increment, digits, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id = -1,
                const wxString& value = wxEmptyString,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxSP_ARROW_KEYS,
                float min = 0.0, float max = 100.0,
                float initial = 0.0,
                float increment = .1, int digits = -1,
                const wxString& name = _T("wxSpinReal"));

    virtual void SetValue(const wxString& text);
    virtual void SetValue( float value );
    virtual void SetRange( float minVal, float maxVal );
    virtual void SetIncrement( float increment, int digits = -1 ); //digits -1 is autodetect
    virtual void SetSnap( bool snap_to_increment );

    virtual float GetValue() const;
    virtual float GetMin() const;
    virtual float GetMax() const;
    virtual float GetIncrement() const;
    virtual int GetDigits() const;
    virtual bool GetSnap() const;

    // implementation
    // --------------

    int DoGetDigits( float increment ); // find min # of digits from increment

    void OnChar( wxKeyEvent &event );
    void OnKey( wxKeyEvent &event );

    bool IsOwnGtkWindow( GdkWindow *window );
    void ApplyWidgetStyle();
    void GtkDisableEvents();
    void GtkEnableEvents();

    GtkAdjustment  *m_adjust;
    float m_increment;                // for shift/ctrl/alt modifiers

protected:
    virtual wxSize DoGetBestSize() const;

private:
    DECLARE_DYNAMIC_CLASS(wxSpinReal)
    DECLARE_EVENT_TABLE()
};

#endif
    // wxUSE_SPINCTRL

#endif
    // __GTKSPINREALH__
