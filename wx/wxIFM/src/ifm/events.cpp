/*!
    Event type implementations for the IFM system

    \file   events.cpp
    \author Robin McNeill
    \date   Created: 10/16/04

    Copyright (c) Robin McNeill
    Licensed under the terms of the wxWindows license
*/

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/ifm/events.h"

IMPLEMENT_DYNAMIC_CLASS(wxIFMPluginEvent, wxEvent)

// mouse events
DEFINE_EVENT_TYPE(wxEVT_IFM_LEFTDOWN)
DEFINE_EVENT_TYPE(wxEVT_IFM_LEFTUP)
DEFINE_EVENT_TYPE(wxEVT_IFM_LEFTDCLICK)
DEFINE_EVENT_TYPE(wxEVT_IFM_RIGHTDOWN)
DEFINE_EVENT_TYPE(wxEVT_IFM_RIGHTUP)
DEFINE_EVENT_TYPE(wxEVT_IFM_RIGHTDCLICK)
DEFINE_EVENT_TYPE(wxEVT_IFM_MIDDLEDOWN)
DEFINE_EVENT_TYPE(wxEVT_IFM_MIDDLEUP)
DEFINE_EVENT_TYPE(wxEVT_IFM_MIDDLEDCLICK)
DEFINE_EVENT_TYPE(wxEVT_IFM_MOTION)
DEFINE_EVENT_TYPE(wxEVT_IFM_MOUSEWHEEL)

DEFINE_EVENT_TYPE(wxEVT_IFM_SETCURSOR)

DEFINE_EVENT_TYPE(wxEVT_IFM_SETFOCUS) // parent recieves focus
DEFINE_EVENT_TYPE(wxEVT_IFM_KILLFOCUS) // parent loses focus
DEFINE_EVENT_TYPE(wxEVT_IFM_ACTIVATE) // parent window [de]activates

DEFINE_EVENT_TYPE(wxEVT_IFM_KEYDOWN)
DEFINE_EVENT_TYPE(wxEVT_IFM_KEYUP)

DEFINE_EVENT_TYPE(wxEVT_IFM_NEWCOMPONENT) // used to create all components
DEFINE_EVENT_TYPE(wxEVT_IFM_DELETECOMPONENT) // used to delete components (not used during cleanup phase)
DEFINE_EVENT_TYPE(wxEVT_IFM_SHOWCOMPONENT) // used to show or hide components
DEFINE_EVENT_TYPE(wxEVT_IFM_VISIBILITYCHANGED) // used to notify a component that is has been shown or hidden
DEFINE_EVENT_TYPE(wxEVT_IFM_ADDCHILD) // add a child window to the interface
DEFINE_EVENT_TYPE(wxEVT_IFM_SETCHILD) // used to add children to panels
DEFINE_EVENT_TYPE(wxEVT_IFM_REMOVECHILD) // removes a child from its panel (and the interface)

DEFINE_EVENT_TYPE(wxEVT_IFM_PAINTINTERFACE) // indicates to plugins that some or all of the interface needs to be repainted
DEFINE_EVENT_TYPE(wxEVT_IFM_PAINTBG) // paints a components background
DEFINE_EVENT_TYPE(wxEVT_IFM_PAINTBORDER) // paints a components border
DEFINE_EVENT_TYPE(wxEVT_IFM_PAINTDECOR) // paints a components decorations
DEFINE_EVENT_TYPE(wxEVT_IFM_BEGINPAINT)
DEFINE_EVENT_TYPE(wxEVT_IFM_ENDPAINT)

DEFINE_EVENT_TYPE(wxEVT_IFM_SHOW) // called to show or hide panels
DEFINE_EVENT_TYPE(wxEVT_IFM_SHOWCHILD) // used to show children managed by the interface
DEFINE_EVENT_TYPE(wxEVT_IFM_HIDECHILD) // used to hide children managed by the interface
DEFINE_EVENT_TYPE(wxEVT_IFM_SETCHILDSIZE) // sets the min / max / desired size of a child
DEFINE_EVENT_TYPE(wxEVT_IFM_QUERYCHILD) 

DEFINE_EVENT_TYPE(wxEVT_IFM_UPDATE) // recalc layout
DEFINE_EVENT_TYPE(wxEVT_IFM_UPDATECOMPONENT) // updates invidual components
DEFINE_EVENT_TYPE(wxEVT_IFM_CALCRECTS)

DEFINE_EVENT_TYPE(wxEVT_IFM_UPDATECONFIG) // plugin configuration data was updated


DEFINE_EVENT_TYPE(wxEVT_IFM_SETDESIREDSIZE) // sets a component desired size
DEFINE_EVENT_TYPE(wxEVT_IFM_GETDESIREDSIZE) // determines the size a component would like to be
DEFINE_EVENT_TYPE(wxEVT_IFM_GETMINSIZE)
DEFINE_EVENT_TYPE(wxEVT_IFM_GETMAXSIZE)
DEFINE_EVENT_TYPE(wxEVT_IFM_SETMINSIZE)
DEFINE_EVENT_TYPE(wxEVT_IFM_SETMAXSIZE)

/*
Rect events

    These events are used to set and retrieve both size and position
    values for absolute, background, and client ares.

    If you only wish to set either the position or the size of a component,
    you may specify IFM_DEFAULT_VALUE for values which you are not changing. The existing
    values will be used.

    wxEVT_IFM_CONVERTRECT is used to convert between different rect values. For example,
    you can use it to convert a client size into an absolute size without having to
    set the size of the component and then use GETCLIENTSIZE.
*/
DEFINE_EVENT_TYPE(wxEVT_IFM_SETRECT) // sets absolute size and pos
#define wxEVT_IFM_SETABSOLUTERECT wxEVT_IFM_SETRECT;
DEFINE_EVENT_TYPE(wxEVT_IFM_SETBACKGROUNDRECT) // sets background size and pos
DEFINE_EVENT_TYPE(wxEVT_IFM_SETCLIENTRECT) // sets client size and pos
DEFINE_EVENT_TYPE(wxEVT_IFM_GETRECT) // returns absolute size and pos
#define wxEVT_IFM_GETABSOLUTERECT wxEVT_IFM_GETRECT;
DEFINE_EVENT_TYPE(wxEVT_IFM_GETBACKGROUNDRECT) // returns background size and pos
DEFINE_EVENT_TYPE(wxEVT_IFM_GETCLIENTRECT) // returns client size and pos
DEFINE_EVENT_TYPE(wxEVT_IFM_CONVERTRECT) // used to convert from [client|background]size to [client|background]size
DEFINE_EVENT_TYPE(wxEVT_IFM_HITTEST) // used for position hit testing
DEFINE_EVENT_TYPE(wxEVT_IFM_GETCONTENTRECT) // used to retrieve the rect available for the applications content window

DEFINE_EVENT_TYPE(wxEVT_IFM_DOCK)
DEFINE_EVENT_TYPE(wxEVT_IFM_UNDOCK)

/*
Floating events
*/
#if IFM_CANFLOAT
DEFINE_EVENT_TYPE(wxEVT_IFM_FLOAT)
DEFINE_EVENT_TYPE(wxEVT_IFM_CREATEFLOATINGWINDOW) // used to create a frame for a floating window
DEFINE_EVENT_TYPE(wxEVT_IFM_DESTROYFLOATINGWINDOW) // used to destroy a frame for a floating window
DEFINE_EVENT_TYPE(wxEVT_IFM_FLOATING_MOVING) // a floating window is being moved
DEFINE_EVENT_TYPE(wxEVT_IFM_FLOATING_SIZING) // a floating window is being sized
DEFINE_EVENT_TYPE(wxEVT_IFM_FLOATING_MOVE) // a floating window has been moved
DEFINE_EVENT_TYPE(wxEVT_IFM_FLOATING_SIZE) // a floating window has been sized
DEFINE_EVENT_TYPE(wxEVT_IFM_FLOATING_SHOW) // used to show a floating window
DEFINE_EVENT_TYPE(wxEVT_IFM_FLOATING_HIDE) // used to hide a floating window
DEFINE_EVENT_TYPE(wxEVT_IFM_FLOATING_NOTIFY) // notifies a component that it has been floated
#endif

wxIFMPluginEvent::wxIFMPluginEvent(wxEventType id, wxIFMComponent *component)
    : wxEvent(wxID_ANY, id),
    m_componentType(IFM_COMPONENT_UNDEFINED),
    m_component(component)
{
    if( component )
        m_componentType = component->GetType();
}

wxIFMDockEvent::wxIFMDockEvent(wxIFMComponent *component, wxIFMComponent *destination, int index)
    : wxIFMPluginEvent(wxEVT_IFM_DOCK,component),
    m_destination(destination),
    m_destinationType(IFM_COMPONENT_UNDEFINED),
    m_index(index)
{
    wxASSERT_MSG(destination, wxT("NULL docking destination?"));
    m_destinationType = destination->GetType();
}

void wxIFMDockEvent::SetDestination(wxIFMComponent *destination)
{
    wxASSERT_MSG(destination, wxT("NULL docking destination?"));
    m_destinationType = destination->GetType();
    m_destination = destination;
}

wxIFMUndockEvent::wxIFMUndockEvent(wxIFMComponent *component, bool del)
    : wxIFMPluginEvent(wxEVT_IFM_UNDOCK, component),
    m_delete(del)
{
    wxIFMComponent *parent = component->m_parent;
    wxASSERT_MSG(parent, wxT("Undocking from a NULL parent?"));
    m_parent = parent;
    m_parentType = parent->GetType();
}

void wxIFMUndockEvent::SetParent(wxIFMComponent *parent)
{
    m_parent = parent;
    m_parentType = parent->GetType();
}

/*!
wxIFMCalcRectsEvent implementation
*/
wxIFMCalcRectsEvent::wxIFMCalcRectsEvent(wxIFMComponent *component, const wxRect &rect)
    : wxIFMPluginEvent(wxEVT_IFM_CALCRECTS, component),
    m_rect(rect)
{
    if( m_rect == IFM_DEFAULT_RECT )
        m_rect = component->GetClientRect();
}

void wxIFMCalcRectsEvent::SetComponentRect(int index, const wxRect &rect)
{
    m_rects[index] = rect;
}

void wxIFMCalcRectsEvent::AddComponent(wxIFMComponent *component)
{
    m_components.push_back(component);
    m_rects.push_back(wxRect(wxPoint(), component->GetDesiredSize()));
    m_min.push_back(component->GetMinSize());
    m_max.push_back(component->GetMaxSize());
}

const wxRectArray &wxIFMCalcRectsEvent::GetComponentRects()
{
    return m_rects;
}

const wxSizeArray &wxIFMCalcRectsEvent::GetMinSizes()
{
    return m_min;
}

const wxSizeArray &wxIFMCalcRectsEvent::GetMaxSizes()
{
    return m_max;
}

const wxIFMComponentArray &wxIFMCalcRectsEvent::GetComponents()
{
    return m_components;
}

const wxRect &wxIFMCalcRectsEvent::GetRect()
{
    return m_rect;
}
