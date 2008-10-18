/////////////////////////////////////////////////////////////////////////////
// Name:        spinreal.h
// Purpose:     wxSpinCtrlBase class
// Author:      Vadim Zeitlin
// Modified by:
// Created:     22.07.99
// RCS-ID:      $Id: spinreal.h,v 1.1.1.1 2003/12/28 21:05:20 wyo Exp $
// Copyright:   (c) Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_SPINREAL_H_
#define _WX_SPINREAL_H_

#include "wx/spinbutt.h"        // should make wxSpinEvent visible to the app

// ----------------------------------------------------------------------------
// a spin ctrl is a text control with a spin button which is usually used to
// prompt the user for a numeric input
// ----------------------------------------------------------------------------

/* there is no generic base class for this control because it's imlpemented
   very differently under MSW and other platforms

class WXDLLEXPORT wxSpinCtrlBase : public wxControl
{
public:
    wxSpinCtrlBase() { Init(); }

    // accessors
    virtual int GetValue() const = 0;
    virtual int GetMin() const { return m_min; }
    virtual int GetMax() const { return m_max; }

    // operations
    virtual void SetValue(const wxString& value) = 0;
    virtual void SetValue(int val) = 0;
    virtual void SetRange(int minVal, int maxVal) = 0;

protected:
    // initialize m_min/max with the default values
    void Init() { m_min = 0; m_max = 100; }

    int   m_min;
    int   m_max;
};
*/

// ----------------------------------------------------------------------------
// include the platform-dependent class implementation
// ----------------------------------------------------------------------------

#if defined(__WXMSW__) && defined(__WIN32__)
    #include "spinrealm.h"
#elif defined(__WXGTK__)
    #include "spinrealg.h"
#else // Win16 || !Win
    #include "wx/generic/spinctlg.h"
#endif // platform

// Macro must be defined here, not event.h, since it must reference wxSpinEventFunction
//#define EVT_SPINCTRL(id, fn) DECLARE_EVENT_TABLE_ENTRY( wxEVT_COMMAND_SPINCTRL_UPDATED, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxSpinEventFunction) & fn, (wxObject *) NULL ),

#endif // _WX_SPINREAL_H_

