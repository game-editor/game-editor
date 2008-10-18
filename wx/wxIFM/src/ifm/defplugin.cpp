/*!
    Implementation of the default plugin

    \file   defplugin.cpp
    \author Robin McNeill
    \date   Created: 02/24/05

    Copyright (c) Robin McNeill
    Licensed under the terms of the wxWindows license
*/

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/ifm/defplugin.h"
#include "wx/ifm/definterface.h" //maks

#include "wx/dcbuffer.h"
#include "wx/dcclient.h"
#include "wx/settings.h"

void DrawHorizontalGradient(wxDC &dc, const wxRect &rect, wxColour color1, wxColour color2); //maks
void DrawVerticalGradient(wxDC &dc, const wxRect &rect, wxColour color1, wxColour color2); //maks
void DrawHorizontalGradientLine(wxDC &dc, int x, int y, int width, wxColour color1, wxColour color2); //maks
void DrawVerticalGradientLine(wxDC &dc, int x, int y, int height, wxColour color1, wxColour color2); //maks

IMPLEMENT_DYNAMIC_CLASS(wxIFMDefaultPlugin, wxEvtHandler);

BEGIN_EVENT_TABLE(wxIFMDefaultPlugin, wxEvtHandler)
    EVT_IFM_SETRECT         (wxIFMDefaultPlugin::OnSetRect)
    EVT_IFM_SETCLIENTRECT   (wxIFMDefaultPlugin::OnSetRect)
    EVT_IFM_SETBACKGROUNDRECT (wxIFMDefaultPlugin::OnSetRect)
    EVT_IFM_GETRECT         (wxIFMDefaultPlugin::OnGetRect)
    EVT_IFM_GETCLIENTRECT   (wxIFMDefaultPlugin::OnGetRect)
    EVT_IFM_GETBACKGROUNDRECT (wxIFMDefaultPlugin::OnGetRect)
    EVT_IFM_CONVERTRECT     (wxIFMDefaultPlugin::OnConvertRect)
    EVT_IFM_HITTEST         (wxIFMDefaultPlugin::OnHitTest)
    EVT_IFM_UPDATECOMPONENT (wxIFMDefaultPlugin::OnUpdate)
    EVT_IFM_CALCRECTS       (wxIFMDefaultPlugin::OnCalcRects)

    EVT_IFM_SETDESIREDSIZE  (wxIFMDefaultPlugin::OnSetDesiredSize)
    EVT_IFM_GETDESIREDSIZE  (wxIFMDefaultPlugin::OnGetDesiredSize)
    EVT_IFM_SETMINSIZE      (wxIFMDefaultPlugin::OnSetMinSize)
    EVT_IFM_SETMAXSIZE      (wxIFMDefaultPlugin::OnSetMaxSize)
    EVT_IFM_GETMINSIZE      (wxIFMDefaultPlugin::OnGetMinSize)
    EVT_IFM_GETMAXSIZE      (wxIFMDefaultPlugin::OnGetMaxSize)

    EVT_IFM_BEGINPAINT      (wxIFMDefaultPlugin::OnBeginPaint)
    EVT_IFM_ENDPAINT        (wxIFMDefaultPlugin::OnEndPaint)
    EVT_IFM_PAINTBG         (wxIFMDefaultPlugin::OnPaintBg)
    EVT_IFM_PAINTBORDER     (wxIFMDefaultPlugin::OnPaintBorder)

    EVT_IFM_DOCK            (wxIFMDefaultPlugin::OnDock)
    EVT_IFM_UNDOCK          (wxIFMDefaultPlugin::OnUndock)

    EVT_IFM_SETCHILD        (wxIFMDefaultPlugin::OnSetChild)
    EVT_IFM_SETCHILDSIZE    (wxIFMDefaultPlugin::OnSetChildSize)

    EVT_IFM_SHOWCHILD       (wxIFMDefaultPlugin::OnShowChild)
    EVT_IFM_HIDECHILD       (wxIFMDefaultPlugin::OnShowChild)
    EVT_IFM_SHOWCOMPONENT   (wxIFMDefaultPlugin::OnShowComponent)
    EVT_IFM_VISIBILITYCHANGED (wxIFMDefaultPlugin::OnVisibilityChanged)

#if IFM_CANFLOAT
    EVT_IFM_FLOAT           (wxIFMDefaultPlugin::OnFloat)
    EVT_IFM_FLOATING_SIZE   (wxIFMDefaultPlugin::OnFloatingSize)
    EVT_IFM_FLOATING_MOVE   (wxIFMDefaultPlugin::OnFloatingMove)
    EVT_IFM_FLOATING_NOTIFY (wxIFMDefaultPlugin::OnFloatNotify)
#endif
END_EVENT_TABLE()

wxIFMDefaultPlugin::wxIFMDefaultPlugin(wxIFMInterfacePluginBase *ip)
    : wxEvtHandler(),
    m_ip(ip)
{ }

void wxIFMDefaultPlugin::OnSetRect(wxIFMRectEvent &event)
{
    wxIFMComponent *component = event.GetComponent();
    wxEventType type = event.GetEventType();

    if( type == wxEVT_IFM_SETRECT )
    {
        // store the rect
        wxRect rect = event.GetRect();
        
        // see if we need to use existing values
        if( rect.x == IFM_USE_CURRENT_VALUE || rect.y == IFM_USE_CURRENT_VALUE ||
            rect.width == IFM_USE_CURRENT_VALUE || rect.height == IFM_USE_CURRENT_VALUE )
        {
            // we need to use some existing values, find out what they are first
            // do this with an event to allow GETRECT to be processed in all circumstances
            wxIFMRectEvent rectevt(wxEVT_IFM_GETRECT, component);
            GetIP()->ProcessPluginEvent(rectevt);
            wxRect m_rect = rectevt.GetRect();

            if( rect.x == IFM_USE_CURRENT_VALUE )
                rect.x = m_rect.x;
            if( rect.y == IFM_USE_CURRENT_VALUE )
                rect.y = m_rect.y;
            if( rect.width == IFM_USE_CURRENT_VALUE )
                rect.width = m_rect.width;
            if( rect.height == IFM_USE_CURRENT_VALUE )
                rect.height = m_rect.height;
        }
        
        // make sure we don't size the component smaller than its minimum size
        wxIFMRectEvent minevt(wxEVT_IFM_GETMINSIZE, component);
        GetIP()->ProcessPluginEvent(minevt);
        const wxSize &min_size = minevt.GetSize();

        if( min_size.GetWidth() != IFM_NO_MINIMUM && min_size.GetWidth() > rect.width )
            rect.width = min_size.GetWidth();
        if( min_size.GetHeight() != IFM_NO_MINIMUM && min_size.GetHeight() > rect.height )
            rect.height = min_size.GetHeight();

        // dont size bigger than the maximum either
        wxIFMRectEvent maxevt(wxEVT_IFM_GETMAXSIZE, component);
        GetIP()->ProcessPluginEvent(maxevt);
        const wxSize &max_size = maxevt.GetSize();

        if( max_size.GetWidth() != IFM_NO_MAXIMUM && max_size.GetWidth() < rect.width )
            rect.width = max_size.GetWidth();
        if( max_size.GetHeight() != IFM_NO_MAXIMUM && max_size.GetHeight() < rect.height )
            rect.height = max_size.GetHeight();

        component->m_rect = rect;
    }
    else if( type == wxEVT_IFM_SETBACKGROUNDRECT )
    {
        // calculate new rect
        wxRect rect = event.GetRect();

        // see if we need to use existing values
        if( rect.x == IFM_USE_CURRENT_VALUE || rect.y == IFM_USE_CURRENT_VALUE ||
            rect.width == IFM_USE_CURRENT_VALUE || rect.height == IFM_USE_CURRENT_VALUE )
        {
            // we need to use some existing values, find out what they are first
            // do this with an event to allow GETBACKGROUNDRECT to be processed in all circumstances
            wxIFMRectEvent rectevt(wxEVT_IFM_GETBACKGROUNDRECT, component);
            GetIP()->ProcessPluginEvent(rectevt);
            wxRect m_rect = rectevt.GetRect();

            if( rect.x == IFM_USE_CURRENT_VALUE )
                rect.x = m_rect.x;

            if( rect.y == IFM_USE_CURRENT_VALUE )
                rect.y = m_rect.y;

            if( rect.width == IFM_USE_CURRENT_VALUE )
                rect.width = m_rect.width;

            if( rect.height == IFM_USE_CURRENT_VALUE )
                rect.height = m_rect.height;
        }

        // Calculate the new absolute size using CONVERTRECT event
        wxIFMConvertRectEvent evt(component, IFM_COORDS_BACKGROUND, IFM_COORDS_ABSOLUTE, rect);
        GetIP()->ProcessPluginEvent(evt);
        rect = evt.GetRect();

        // set the rect
        component->m_rect = rect;
    }
    else if( type == wxEVT_IFM_SETCLIENTRECT )
    {
        // calculate new rect
        wxRect rect = event.GetRect();

        // see if we need to use existing values
        if( rect.x == IFM_USE_CURRENT_VALUE || rect.y == IFM_USE_CURRENT_VALUE ||
            rect.width == IFM_USE_CURRENT_VALUE || rect.height == IFM_USE_CURRENT_VALUE )
        {
            // we need to use some existing values, find out what they are first
            // do this with an event to allow GETCLIENTRECT to be processed in all circumstances
            wxIFMRectEvent rectevt(wxEVT_IFM_GETCLIENTRECT, component);
            GetIP()->ProcessPluginEvent(rectevt);
            wxRect m_rect = rectevt.GetRect();

            if( rect.x == IFM_USE_CURRENT_VALUE )
                rect.x = m_rect.x;

            if( rect.y == IFM_USE_CURRENT_VALUE )
                rect.y = m_rect.y;

            if( rect.width == IFM_USE_CURRENT_VALUE )
                rect.width = m_rect.width;

            if( rect.height == IFM_USE_CURRENT_VALUE )
                rect.height = m_rect.height;
        }

        // Calculate the new absolute size using CONVERTRECT event
        wxIFMConvertRectEvent evt(component, IFM_COORDS_CLIENT, IFM_COORDS_ABSOLUTE, rect);
        GetIP()->ProcessPluginEvent(evt);
        rect = evt.GetRect();

        // set the new rect
        component->m_rect = rect;
    }
#ifdef __WXDEBUG__
    else
        wxFAIL_MSG(wxT("Unknown event type encountered"));
#endif
}

void wxIFMDefaultPlugin::OnGetRect(wxIFMRectEvent &event)
{
    wxIFMComponent *component = event.GetComponent();
    wxEventType type = event.GetEventType();

    // return wxRect(0,0,0,0) if we are hidden
    /*
    if( component->m_hidden )
    {
        event.SetRect(wxRect(0,0,0,0));
        return;
    }
    */

    if( type == wxEVT_IFM_GETRECT )
    {
        // return the rect
        event.SetRect(component->m_rect);
    }
    else if( type == wxEVT_IFM_GETBACKGROUNDRECT )
    {
        // get the absolute rect first
        wxIFMRectEvent rectevt(wxEVT_IFM_GETRECT, component);
        GetIP()->ProcessEvent(rectevt);

        // use CONVERTRECT to go from absolute to background coords
        wxIFMConvertRectEvent evt(component,
            IFM_COORDS_ABSOLUTE, IFM_COORDS_BACKGROUND, rectevt.GetRect());
        GetIP()->ProcessPluginEvent(evt);

        // return new rect
        event.SetRect(evt.GetRect());
    }
    else if( type == wxEVT_IFM_GETCLIENTRECT )
    {
        // get the absolute rect first
        wxIFMRectEvent rectevt(wxEVT_IFM_GETRECT, component);
        GetIP()->ProcessEvent(rectevt);

        // use CONVERTRECT to go from absolute to client coords
        wxIFMConvertRectEvent evt(component,
            IFM_COORDS_ABSOLUTE, IFM_COORDS_CLIENT, rectevt.GetRect());
        GetIP()->ProcessPluginEvent(evt);

        // return new rect
        event.SetRect(evt.GetRect());
    }
#ifdef __WXDEBUG__
    else
        wxFAIL_MSG(wxT("Unknown event type encountered"));
#endif
}

void wxIFMDefaultPlugin::OnConvertRect(wxIFMConvertRectEvent &event)
{
    wxIFMComponent *component = event.GetComponent();

    int source = event.GetSourceCoords(), dest = event.GetDestinationCoords();

    // make sure we don't try to convert to the same type
    if( source == dest )
        return;

    if( source == IFM_COORDS_CLIENT && dest == IFM_COORDS_FLOATINGWINDOW )
    {
        // use events to go from client -> background -> absolute -> floatingwindow

        // to absolute
        wxIFMConvertRectEvent toabsevt(component,
            IFM_COORDS_CLIENT, IFM_COORDS_ABSOLUTE, event.GetRect());
        GetIP()->ProcessPluginEvent(toabsevt);

        // to floatingwindow
        wxIFMConvertRectEvent tofwevt(component,
            IFM_COORDS_ABSOLUTE, IFM_COORDS_FLOATINGWINDOW, toabsevt.GetRect());
        GetIP()->ProcessPluginEvent(tofwevt);

        // return new rect
        event.SetRect(tofwevt.GetRect());
    }
    if( source == IFM_COORDS_BACKGROUND && dest == IFM_COORDS_FLOATINGWINDOW )
    {
        // use events to go from background -> absolute -> floatingwindow

        // to absolute
        wxIFMConvertRectEvent toabsevt(component,
            IFM_COORDS_BACKGROUND, IFM_COORDS_ABSOLUTE, event.GetRect());
        GetIP()->ProcessPluginEvent(toabsevt);

        // to floatingwindow
        wxIFMConvertRectEvent tofwevt(component,
            IFM_COORDS_ABSOLUTE, IFM_COORDS_FLOATINGWINDOW, toabsevt.GetRect());
        GetIP()->ProcessPluginEvent(tofwevt);

        // return new rect
        event.SetRect(tofwevt.GetRect());
    }
    else if( source == IFM_COORDS_CLIENT && dest == IFM_COORDS_ABSOLUTE )
    {
        // use events to go from client -> background -> absolute
        
        // to background
        wxIFMConvertRectEvent tobgevt(component,
            IFM_COORDS_CLIENT, IFM_COORDS_BACKGROUND, event.GetRect());
        GetIP()->ProcessPluginEvent(tobgevt);
        
        // to absolute
        wxIFMConvertRectEvent toabsevt(component,
            IFM_COORDS_BACKGROUND, IFM_COORDS_ABSOLUTE, tobgevt.GetRect());
        GetIP()->ProcessPluginEvent(toabsevt);

        // return new rect
        event.SetRect(toabsevt.GetRect());
    }
    else if( source == IFM_COORDS_CLIENT && dest == IFM_COORDS_BACKGROUND )
    {
        // calculate new rect
        wxRect rect = event.GetRect();

        // account for padding
        rect.height += component->m_margins.top + component->m_margins.bottom;
        rect.width += component->m_margins.left + component->m_margins.right;
        rect.y -= component->m_margins.top;
        rect.x -= component->m_margins.left;

        // return new rect
        event.SetRect(rect);
    }
    else if( source == IFM_COORDS_BACKGROUND && dest == IFM_COORDS_ABSOLUTE )
    {
        // calculate new rect
        wxRect rect = event.GetRect();

        // account for borders
        rect.height += component->m_borders.top + component->m_borders.bottom;
        rect.width += component->m_borders.left + component->m_borders.right;
        rect.y -= component->m_borders.top;
        rect.x -= component->m_borders.left;

        // return new rect
        event.SetRect(rect);
    }
    else if( source == IFM_COORDS_ABSOLUTE && dest == IFM_COORDS_FLOATINGWINDOW )
    {
        // calculate new rect
        wxRect rect = event.GetRect();

        wxWindow *wnd = component->GetParentWindow();

        int caption_height = wxSystemSettings::GetMetric(wxSYS_CAPTION_Y, wnd);
        if( caption_height == -1 )
            caption_height = IFM_DEF_CAPTION_HEIGHT;

        int border_width = wxSystemSettings::GetMetric(wxSYS_FRAMESIZE_X, wnd);
        if( border_width == -1 )
            border_width = IFM_DEF_FRAME_X;

        rect.y -= caption_height;
#ifdef __WXMSW__
        rect.y -= wxSystemSettings::GetMetric(wxSYS_FRAMESIZE_Y , wnd);
#endif
        rect.x -= border_width;

        // assume borders are the same width
        rect.width += border_width;
        rect.height += border_width;

        // return new rect
        event.SetRect(rect);
    }

    else if( source == IFM_COORDS_FLOATINGWINDOW && dest == IFM_COORDS_CLIENT )
    {
        // use events to go from floatingwindow -> absolute -> background -> client
        
        // to absolute
        wxIFMConvertRectEvent toabsevt(component,
            IFM_COORDS_FLOATINGWINDOW, IFM_COORDS_ABSOLUTE, event.GetRect());
        GetIP()->ProcessPluginEvent(toabsevt);
        
        // to client
        wxIFMConvertRectEvent tocltevt(component,
            IFM_COORDS_ABSOLUTE, IFM_COORDS_CLIENT, toabsevt.GetRect());
        GetIP()->ProcessPluginEvent(tocltevt);

        // return new rect
        event.SetRect(tocltevt.GetRect());
    }
    else if( source == IFM_COORDS_FLOATINGWINDOW && dest == IFM_COORDS_BACKGROUND )
    {
        // use events to go from floatingwindow -> absolute -> background
        
        // to absolute
        wxIFMConvertRectEvent toabsevt(component,
            IFM_COORDS_FLOATINGWINDOW, IFM_COORDS_ABSOLUTE, event.GetRect());
        GetIP()->ProcessPluginEvent(toabsevt);

        // to background
        wxIFMConvertRectEvent tobgevt(component,
            IFM_COORDS_ABSOLUTE, IFM_COORDS_BACKGROUND, toabsevt.GetRect());
        GetIP()->ProcessPluginEvent(tobgevt);

        // return new rect
        event.SetRect(tobgevt.GetRect());
    }
    else if( source == IFM_COORDS_ABSOLUTE && dest == IFM_COORDS_CLIENT )
    {
        // use events to go from absolute -> background -> client
        
        // to background
        wxIFMConvertRectEvent tobgevt(component,
            IFM_COORDS_ABSOLUTE, IFM_COORDS_BACKGROUND, event.GetRect());
        GetIP()->ProcessPluginEvent(tobgevt);
        
        // to client
        wxIFMConvertRectEvent tocltevt(component,
            IFM_COORDS_BACKGROUND, IFM_COORDS_CLIENT, tobgevt.GetRect());
        GetIP()->ProcessPluginEvent(tocltevt);

        // return new rect
        event.SetRect(tocltevt.GetRect());
    }
    else if( source == IFM_COORDS_ABSOLUTE && dest == IFM_COORDS_BACKGROUND )
    {
        // calculate new rect
        wxRect rect = event.GetRect();

        // account for borders
        rect.height -= component->m_borders.top + component->m_borders.bottom;
        rect.width -= component->m_borders.left + component->m_borders.right;
        rect.y += component->m_borders.top;
        rect.x += component->m_borders.left;

        // return new rect
        event.SetRect(rect);
    }
    else if( source == IFM_COORDS_BACKGROUND && dest == IFM_COORDS_CLIENT )
    {
        // calculate new rect
        wxRect rect = event.GetRect();

        // account for padding
        rect.height -= component->m_margins.top + component->m_margins.bottom;
        rect.width -= component->m_margins.left + component->m_margins.right;
        rect.y += component->m_margins.top;
        rect.x += component->m_margins.left;

        // return new rect
        event.SetRect(rect);
    }
    else if( source == IFM_COORDS_FLOATINGWINDOW && dest == IFM_COORDS_ABSOLUTE )
    {
        // calculate new rect
        wxRect rect = event.GetRect();

        wxWindow *wnd = component->GetParentWindow();

        int caption_height = wxSystemSettings::GetMetric(wxSYS_CAPTION_Y, wnd);
        if( caption_height == -1 )
            caption_height = IFM_DEF_CAPTION_HEIGHT;

        int border_width = wxSystemSettings::GetMetric(wxSYS_FRAMESIZE_X , wnd);
        if( border_width == -1 )
            border_width = IFM_DEF_FRAME_X;

        rect.y += caption_height;
#ifdef __WXMSW__
        rect.y += wxSystemSettings::GetMetric(wxSYS_FRAMESIZE_Y , wnd);
#endif
        rect.x += border_width;

        // assume borders are the same width
        rect.width -= border_width;
        rect.height -= border_width;

        // return new rect
        event.SetRect(rect);
    }
}

void wxIFMDefaultPlugin::OnHitTest(wxIFMHitTestEvent &event)
{
    int coords = event.GetCoords();
    wxIFMComponent *component = event.GetComponent();

    if( component->m_hidden )
        return;

    wxRect rect;
    const wxPoint &pos = event.GetPos();

    switch(coords)
    {
        case IFM_COORDS_ABSOLUTE:
            rect = component->m_rect;
            break;
        case IFM_COORDS_BACKGROUND:
        {
            wxIFMRectEvent rectevt(wxEVT_IFM_GETBACKGROUNDRECT, component);
            GetIP()->ProcessPluginEvent(rectevt);
            rect = rectevt.GetRect();
            break;
        }
        case IFM_COORDS_CLIENT:
        {
            wxIFMRectEvent rectevt(wxEVT_IFM_GETCLIENTRECT, component);
            GetIP()->ProcessPluginEvent(rectevt);
            rect = rectevt.GetRect();
            break;
        }
    }

    if( rect.Inside(pos) )
        event.SetPassed();
}

void wxIFMDefaultPlugin::OnGetDesiredSize(wxIFMRectEvent &event)
{
    wxIFMComponent *component = event.GetComponent();

    // return the desired size
    wxSize size = component->m_desiredSize;

    // never size less than our minimum
    // FIXME: This case should never occur, so I assert when it does
    if( component->m_minSize.GetHeight() > size.GetHeight() )
    {
        //wxFAIL_MSG(wxT("Desired size was smaller than minimum!"));
        size.SetHeight(component->m_minSize.GetHeight());
    }
    if( component->m_minSize.GetWidth() > size.GetWidth() )
    {
        //wxFAIL_MSG(wxT("Desired size was smaller than minimum!"));
        size.SetWidth(component->m_minSize.GetWidth());
    }

    // desired sizes are stored in client coords, but absolute coords must be returned
    wxIFMConvertRectEvent evt(component, IFM_COORDS_CLIENT, IFM_COORDS_ABSOLUTE, wxPoint(0,0), size);
    GetIP()->ProcessPluginEvent(evt);

    event.SetSize(evt.GetSize());
}

void wxIFMDefaultPlugin::OnSetDesiredSize(wxIFMRectEvent &event)
{
    wxIFMComponent *component = event.GetComponent();

    wxASSERT_MSG(component, wxT("NULL component?"));
    if( !component )
        return;

    wxSize size = event.GetSize();

    // handle existing values
    if( size.GetWidth() == IFM_USE_CURRENT_VALUE )
        size.SetWidth(component->m_desiredSize.GetWidth());
    if( size.GetHeight() == IFM_USE_CURRENT_VALUE )
        size.SetHeight(component->m_desiredSize.GetHeight());

    // dont size below minimum or bigger than maximum sizes
    wxIFMRectEvent minevt(wxEVT_IFM_GETMINSIZE, component);
    GetIP()->ProcessPluginEvent(minevt);
    const wxSize &minsize = minevt.GetSize();

    if( size.x < minsize.x )
        size.x = minsize.x;
    if( size.y < minsize.y )
        size.y = minsize.y;

    wxIFMRectEvent maxevt(wxEVT_IFM_GETMAXSIZE, component);
    GetIP()->ProcessPluginEvent(maxevt);
    const wxSize &maxsize = maxevt.GetSize();

    if( size.x > maxsize.x && maxsize.x != IFM_NO_MAXIMUM )
        size.x = maxsize.x;
    if( size.y > maxsize.y && maxsize.y != IFM_NO_MAXIMUM )
        size.y = maxsize.y;

    // desired sizes are set in absolute coords but stored in client coords
    wxIFMConvertRectEvent cvtevt(component, IFM_COORDS_ABSOLUTE, IFM_COORDS_CLIENT, wxPoint(0,0), size);
    GetIP()->ProcessPluginEvent(cvtevt);
    size = cvtevt.GetSize();

    component->m_desiredSize = size;

    // set desired floating size if possible
#if IFM_CANFLOAT
    wxIFMFloatingData *data = IFM_GET_EXTENSION_DATA(component, wxIFMFloatingData);
    data->m_rect.SetSize(size);
#endif
}

void wxIFMDefaultPlugin::OnDock(wxIFMDockEvent &event)
{
    wxIFMComponent *component = event.GetComponent();
    wxASSERT_MSG(!component->m_docked, wxT("Component already docked!"));

    wxIFMComponent *destination = event.GetDestination();

    wxASSERT_MSG(component != destination, wxT("Docking a component into itself?"));
    if( component == destination )
        return;

    component->m_docked = true;
    component->m_parent = destination;

#if IFM_CANFLOAT
    wxIFMFloatingData *floating_data = IFM_GET_EXTENSION_DATA(destination, wxIFMFloatingData);

    // let the component know if it is floating now
    wxIFMFloatNotifyEvent notifyevt(component, floating_data->m_floating, floating_data->m_window);
    GetIP()->ProcessPluginEvent(notifyevt);
#endif

    int index = event.GetIndex(), count = destination->m_children.size() - 1;

    wxASSERT_MSG(component, wxT("Docking a null component?"));
    wxASSERT_MSG(destination, wxT("Null destination component?"));

    if( index == IFM_DEFAULT_INDEX || index > count )
    {
        // add it after existing components
        destination->m_children.push_back(component);
    }
    else //if( index >= 0 )
    {
        // add it where it wants to be
        destination->m_children.insert(destination->m_children.begin() + index, component);
    }
}

void wxIFMDefaultPlugin::OnUpdate(wxIFMUpdateComponentEvent &event)
{
    wxIFMComponent *component = event.GetComponent();

    wxASSERT_MSG(component, wxT("NULL component?"));
    if( !component )
        return;

    // invalidate the current (old)position
    component->GetParentWindow()->RefreshRect(component->m_rect);

    // size and position the component
    wxIFMRectEvent rectevt(wxEVT_IFM_SETRECT, component, event.GetRect());
    GetIP()->ProcessPluginEvent(rectevt);

    // invalidate the current (new) position
    component->GetParentWindow()->RefreshRect(component->m_rect);

    // if this component is the root component of a floating window, set the floating windows size hints here
#if IFM_CANFLOAT
    wxIFMFloatingData *data = IFM_GET_EXTENSION_DATA(component, wxIFMFloatingData);
    if( data && data->m_floating && !component->m_docked )
    {
        wxSize min = component->GetMinSize();
		wxSize max = component->GetMaxSize(); //maks

        // absolute to floating window coords
        wxIFMConvertRectEvent cvtevt(component, IFM_COORDS_ABSOLUTE, IFM_COORDS_FLOATINGWINDOW, wxPoint(0,0), min);
        GetIP()->ProcessEvent(cvtevt);

		wxIFMConvertRectEvent cvtevt1(component, IFM_COORDS_ABSOLUTE, IFM_COORDS_FLOATINGWINDOW, wxPoint(0,0), max); //maks
        GetIP()->ProcessEvent(cvtevt1); //maks

        const wxRect &rect = cvtevt.GetRect();
        wxSize min2(rect.width - rect.x, rect.height - rect.y);

		const wxRect &rect1 = cvtevt1.GetRect(); //maks
        wxSize max2(rect1.width - rect1.x, rect1.height - rect1.y); //maks

		if(min.x < 0) min2.x = -1; //maks
		if(min.y < 0) min2.y = -1; //maks
		if(max.x < 0) max2.x = -1; //maks
		if(max.y < 0) max2.y = -1; //maks

        data->m_window->GetWindow()->SetSizeHints(min2.x, min2.y, max2.x, max2.y);

        // if the window is currently too small for the minimum size, make it bigger
        wxSize size = data->m_window->GetWindow()->GetClientSize();
        if( size.x < min.x || size.y < min.y ||
			size.x > max.x || size.y > max.y ) //maks
        {
            if( min.x >= 0 && size.x < min.x )
                size.x = min.x;

            if( min.y >= 0 && size.y < min.y )
                size.y = min.y;

			if( max.x >= 0 && size.x > max.x ) 
                size.x = max.x;

            if( max.y >= 0 && size.y > max.y ) 
                size.y = max.y;

            data->m_window->GetWindow()->SetClientSize(size);
        }
    }
#endif
}


void wxIFMDefaultPlugin::OnBeginPaint(wxIFMBeginPaintEvent &event)
{
    wxDC *dc;

    // create a dc
    if( event.GetOnPaint() )
        dc = new wxBufferedPaintDC(event.GetWindow());
    else
        dc = new wxClientDC(event.GetWindow());

    event.SetDC(dc);
}

void wxIFMDefaultPlugin::OnEndPaint(wxIFMEndPaintEvent &event)
{
    // delete the dc
    delete event.GetDC();
}

void wxIFMDefaultPlugin::OnPaintBg(wxIFMPaintEvent &event)
{
    wxIFMComponent *component = event.GetComponent();

    // don't need to paint if we arent visible
    if( !component->IsVisible() )
        return;

    // get background rect first, as the rect provided in the wxIFMPaintEvent
    // is the rect that needs painting, not the background rect
    wxIFMRectEvent rectevt(wxEVT_IFM_GETBACKGROUNDRECT, component);
    GetIP()->ProcessPluginEvent(rectevt);
    wxRect bgrect = rectevt.GetRect();

    // paint normal bg
    //! \todo Globalize colour storage
    wxColour color(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
    wxBrush brush(color);
    wxPen pen(color);
    wxDC &dc = event.GetDC();

    dc.SetBrush(brush);
    dc.SetPen(pen);

    dc.DrawRectangle(bgrect);

    dc.SetPen(wxNullPen);
    dc.SetBrush(wxNullBrush);
}

void wxIFMDefaultPlugin::OnPaintBorder(wxIFMPaintEvent &event)
{
    wxIFMComponent *component = event.GetComponent();
	
    // don't need to paint if we arent visible
    if( !component->IsVisible() )
        return;

    // get background rect to draw borders around
    wxIFMRectEvent rectevt(wxEVT_IFM_GETBACKGROUNDRECT, component);
    GetIP()->ProcessPluginEvent(rectevt);

    // get border rect
    wxRect rect = rectevt.GetRect();
    
    rect.y--;
    rect.x--;
    rect.width++;
    rect.height++;

    //! \todo Globalize colour storage
    //wxPen pen_light(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
    //wxPen pen_dark(wxSystemSettings::GetColour(wxSYS_COLOUR_3DDKSHADOW));
    wxPen border_pen(wxBorder_Pen); //maks(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW));
    wxDC &dc = event.GetDC();

	bool b3DLook = true; //maks

	if(b3DLook)
	{
		//maks
		wxPen border_pen_dark(wxBorder_PenDark);
		wxColour light(230, 230, 230);//(wxTab_border_p.Red()*2, wxTab_border_p.Green()*2, wxTab_border_p.Blue()*2);

		// paint borders
		// top
		if( component->m_borders.top )
		{
			if(!wxIFMDefaultPanelPlugin::GetCaptionConfig().get_3d_boder())
			{
				border_pen.SetWidth(component->m_borders.top);			
				dc.SetPen(border_pen);
				dc.DrawLine(rect.x, rect.y, rect.x + rect.width, rect.y);
			}
			else
			{	
				//DrawHorizontalGradientLine(dc, rect.x, rect.y-1, rect.width, light, wxBackground_Pen);	
				//DrawHorizontalGradientLine(dc, rect.x+1, rect.y, rect.width, wxTab_border_p, wxBackground_Pen);	


				//Don't works with size = 1
				DrawHorizontalGradient(dc, wxRect(rect.x, rect.y-1, rect.width, 2), light, wxBackground_Pen);
				DrawHorizontalGradient(dc, wxRect(rect.x+1, rect.y, rect.width, 2), wxTab_border_p, wxBackground_Pen);

				//Erase last line
				dc.SetPen(wxBackground_Pen);
				dc.DrawLine(rect.x+1, rect.y+1, rect.x + rect.width, rect.y+1);
			}
		}
		
		// left
		if( component->m_borders.left )
		{
			if(!wxIFMDefaultPanelPlugin::GetCaptionConfig().get_3d_boder())
			{
				border_pen.SetWidth(component->m_borders.left);
				dc.SetPen(border_pen);
				dc.DrawLine(rect.x, rect.y + 1, rect.x, rect.y + rect.height);
			}
			else
			{
				DrawVerticalGradientLine(dc, rect.x-1, rect.y-1, rect.height, light, wxBackground_Pen);
				DrawVerticalGradientLine(dc, rect.x, rect.y, rect.height, wxTab_border_p, wxBackground_Pen);
			
				//Don't works with script at botton and actor at right
				//DrawVerticalGradient(dc, wxRect(rect.x-1, rect.y-1, 1, rect.height), light, wxBackground_Pen);
				//DrawVerticalGradient(dc, wxRect(rect.x, rect.y, 1, rect.height), wxTab_border_p, wxBackground_Pen);
			}
		}
		
		
		
		// right
		if( component->m_borders.right )
		{
			border_pen_dark.SetWidth(component->m_borders.right);

			if(!wxIFMDefaultPanelPlugin::GetCaptionConfig().get_3d_boder()) dc.SetPen(border_pen_dark);
			else dc.SetPen(wxBackground_Pen);

			dc.DrawLine(rect.x + rect.width, rect.y, rect.x + rect.width, rect.y + rect.height + 1);
		}
		
		// bottom
		if( component->m_borders.bottom )
		{
			border_pen_dark.SetWidth(component->m_borders.bottom);
			if(!wxIFMDefaultPanelPlugin::GetCaptionConfig().get_3d_boder()) dc.SetPen(border_pen_dark);
			else dc.SetPen(wxBackground_Pen);

			dc.DrawLine(rect.x, rect.y + rect.height, rect.x + rect.width + 1, rect.y + rect.height);
		}
		
	}
	else
	{
		// paint borders
		// top
		if( component->m_borders.top )
		{
			border_pen.SetWidth(component->m_borders.top);
			dc.SetPen(border_pen);
			dc.DrawLine(rect.x, rect.y, rect.x + rect.width, rect.y);
		}
		
		// left
		if( component->m_borders.left )
		{
			border_pen.SetWidth(component->m_borders.left);
			dc.SetPen(border_pen);
			dc.DrawLine(rect.x, rect.y, rect.x, rect.y + rect.height);
		}
		
		// right
		if( component->m_borders.right )
		{
			border_pen.SetWidth(component->m_borders.right);
			dc.SetPen(border_pen);
			dc.DrawLine(rect.x + rect.width, rect.y, rect.x + rect.width, rect.y + rect.height + 1);
		}
		
		// bottom
		if( component->m_borders.bottom )
		{
			border_pen.SetWidth(component->m_borders.bottom);
			dc.SetPen(border_pen);
			dc.DrawLine(rect.x, rect.y + rect.height, rect.x + rect.width + 1, rect.y + rect.height);
		}
	}
}

void wxIFMDefaultPlugin::OnUndock(wxIFMUndockEvent &event)
{
    wxIFMComponent *component = event.GetComponent();
    wxIFMComponent *parent = component->m_parent;
    wxASSERT_MSG(parent, wxT("Undocking a component with a null parent?"));

    // find the child within its parent's children list and remove it
    wxIFMComponentArray &children = parent->m_children;
    //for( int i = 0, count = parent->m_children.GetCount(); i < count; ++i )
    for( wxIFMComponentArray::iterator i = children.begin(), end = children.end(); i != end; ++i )
    {
        //if( children[i] == component )
        if( *i == component )
        {
            //children.RemoveAt(i);
            children.erase(i);
            break;
        }
    }

    // delete the parent if necessary
    if( event.GetDelete() && parent->m_children.GetCount() == 0 )
    {
#if IFM_CANFLOAT
        wxIFMFloatingData *floating_data = IFM_GET_EXTENSION_DATA(parent, wxIFMFloatingData);
#endif
        // undock the parent from its parent first
        if( parent->m_docked )
        {
            wxIFMUndockEvent evt(parent);
            GetIP()->ProcessPluginEvent(evt);
        }
#if IFM_CANFLOAT
        else if( floating_data->m_floating )
        {
            // if a component is not docked but is floating, that means its the root level component
            // of a floating window. It has no children left, so destroy the floating window.
            wxIFMDestroyFloatingWindowEvent evt(floating_data->m_window, true);
            GetIP()->AddPendingEvent(evt);
        }

        if( !floating_data->m_floating )
#endif
        {
            wxIFMDeleteComponentEvent delevt(parent);
            GetIP()->ProcessPluginEvent(delevt);
        }
    }

    component->m_docked = false;
    component->m_parent = NULL;
}

void wxIFMDefaultPlugin::OnDeleteComponent(wxIFMDeleteComponentEvent &event)
{
    wxIFMComponent *component = event.GetComponent();

    // undock ourselves if we are still docked
    if( component->m_docked )
    {
        wxIFMUndockEvent evt(component);
        GetIP()->ProcessPluginEvent(evt);
    }

    // delete any children first
    wxIFMComponentArray &children = component->m_children;
    for( size_t i = 0; i < children.GetCount(); i++ )
    //for( wxIFMComponentArray::iterator i = children.begin(), end = children.end(); i != end; ++i )
    {
        wxIFMDeleteComponentEvent evt(children[i]);
        GetIP()->ProcessPluginEvent(evt);
    }
}

void wxIFMDefaultPlugin::OnSetChild(wxIFMSetChildEvent &event)
{
    wxIFMComponent *component = event.GetComponent();

    // set the child
    component->m_child = event.GetChild();
    component->m_childType = event.GetChildType();

    // hide the child if the component is hidden
    if( component->m_hidden )
        component->m_child->Hide();
}

void wxIFMDefaultPlugin::OnSetChildSize(wxIFMSetChildSizeEvent &event)
{
    wxIFMComponent *component = event.GetComponent();

    wxCHECK_RET(component, wxT("Setting the size of a child not managed by this interface"));

    wxSize size;

    // min size
    size = event.GetMinSize();
    if( size.GetWidth() == IFM_USE_CURRENT_VALUE )
        size.SetWidth(component->m_minSize.GetWidth());

    if( size.GetHeight() == IFM_USE_CURRENT_VALUE )
        size.SetHeight(component->m_minSize.GetHeight());
    component->m_minSize = size;

    // max size
    size = event.GetMaxSize(); //maks
    if( size.GetWidth() == IFM_USE_CURRENT_VALUE )
        size.SetWidth(component->m_maxSize.GetWidth());

    if( size.GetHeight() == IFM_USE_CURRENT_VALUE )
        size.SetHeight(component->m_maxSize.GetHeight());
    component->m_maxSize = size;

    // desired size
    size = event.GetDesiredSize();
    if( size.GetWidth() == IFM_USE_CURRENT_VALUE )
        size.SetWidth(component->m_desiredSize.GetWidth());

    if( size.GetHeight() == IFM_USE_CURRENT_VALUE )
        size.SetHeight(component->m_desiredSize.GetHeight());
    component->m_desiredSize = size;

    if( event.GetUpdate() )
    {
#if IFM_CANFLOAT
        wxIFMFloatingData *data = IFM_GET_EXTENSION_DATA(component, wxIFMFloatingData);
        if( data->m_floating )
            data->m_window->Update();
        else
#endif
            GetManager()->Update();
    }
}

void wxIFMDefaultPlugin::OnSetMinSize(wxIFMRectEvent &WXUNUSED(event))
{
    //! \todo Why do I have this empty function?
}

void wxIFMDefaultPlugin::OnSetMaxSize(wxIFMRectEvent &WXUNUSED(event))
{
    //! \todo Why do I have this empty function?
}

void wxIFMDefaultPlugin::OnGetMinSize(wxIFMRectEvent &event)
{
    wxIFMComponent *component = event.GetComponent();

    wxASSERT_MSG(component, wxT("NULL component?"));
    if( !component )
        return;

    wxSize size;

    // query the child for our min size if applicable
    if( component->m_minSize == IFM_USE_CHILD_MINSIZE )
    {
        if( component->m_child )
        {
            size = component->m_child->GetBestSize();
        }
    }
    else
    {
        // return cummulative minimum size of this window and all children
        // in absolute coordinates
        wxSize min_size, child_size;
        wxIFMComponent *child;
        const wxIFMComponentArray &children = component->m_children;
        for( size_t i = 0; i < children.GetCount(); i++ )
        //for( wxIFMComponentArray::const_iterator i = children.begin(), end = children.end(); i != end; ++i )
        {
            child = children[i];
            if( !child->IsVisible() )
                continue;

            wxIFMRectEvent minevt(wxEVT_IFM_GETMINSIZE, child);
            GetIP()->ProcessPluginEvent(minevt);
            child_size = minevt.GetSize();

            if( component->m_alignment == IFM_ALIGN_HORIZONTAL )
            {
                min_size.x += child_size.x;
                if( child_size.y > min_size.y )
                    min_size.y = child_size.y;
            }
            else if( component->m_alignment == IFM_ALIGN_VERTICAL )
            {
                min_size.y += child_size.y;
                if( child_size.x > min_size.x )
                    min_size.x = child_size.x;
            }
        }

        // account for our min sizes too
        child_size = component->m_minSize;
        if( child_size.x >= min_size.x )
            min_size.x = child_size.x;
        if( child_size.y >= min_size.y )
            min_size.y = child_size.y;

        size = min_size;
    }

    // client to absolute
    wxSize converted_min = component->GetConvertedRect(wxRect(wxPoint(), size), IFM_COORDS_CLIENT, IFM_COORDS_ABSOLUTE).GetSize();
    if( size.x != IFM_NO_MINIMUM )
        size.x = converted_min.x;
    if( size.y != IFM_NO_MINIMUM )
        size.y = converted_min.y;
    
    event.SetSize(size);
}

void wxIFMDefaultPlugin::OnGetMaxSize(wxIFMRectEvent &event) //maks
{
    wxIFMComponent *component = event.GetComponent();

    wxASSERT_MSG(component, wxT("NULL component?"));
    if( !component )
        return;

    // if any child has a max size of IFM_NO_MAXIMUM_SIZE, there is no max size for the children
    // and we immediatley return with this components max size
    // if all children have a specified max size, return the cummulative total
    wxSize max_size, size, child_size;
    wxIFMComponent *child;
    const wxIFMComponentArray &children = component->m_children;
    for( wxIFMComponentArray::const_iterator i = children.begin(), end = children.end(); i != end; ++i ) //maks
    {
        child = *i;
        size = child->GetMaxSize();		
		
		if(max_size.x == 0) max_size.x = size.x;
		else
		{
			if(size.x < max_size.x) 
				max_size.x = size.x;
		}
		
		if(max_size.y == 0) max_size.y = size.y;
		else
		{
			if(size.y < max_size.y) 
				max_size.y = size.y;
		}


		size = max_size;
    }
	
	// account for our min sizes too
	child_size = component->m_maxSize;
	if( size.x == 0 || (child_size.x < size.x && child_size.x != IFM_NO_MAXIMUM) )
		size.x = child_size.x;

	if( size.y == 0 || (child_size.y < size.y && child_size.y != IFM_NO_MAXIMUM) )
		size.y = child_size.y;
	

	if(size.x == 0) size.x = IFM_NO_MAXIMUM;
	if(size.y == 0) size.y = IFM_NO_MAXIMUM;

	// client to absolute
    wxSize converted_max = component->GetConvertedRect(wxRect(wxPoint(), size), IFM_COORDS_CLIENT, IFM_COORDS_ABSOLUTE).GetSize();
    if( size.x != IFM_NO_MAXIMUM )
        size.x = converted_max.x;

    if( size.y != IFM_NO_MAXIMUM )
        size.y = converted_max.y;

    event.SetSize(size);
}

void wxIFMDefaultPlugin::OnCalcRects(wxIFMCalcRectsEvent &event)
{
    wxIFMComponent *component = event.GetComponent();
    wxASSERT_MSG(component, wxT("NULL component?"));
    if( !component )
        return;

    // retrieve and store all calculated sizes for visible components
    const wxIFMComponentArray &children = event.GetComponents();
    int numchildren = children.size(), current = wxIFMComponent::GetNextVisibleComponent(children, 0);
    wxRectArray children_rects = event.GetComponentRects();
    const wxSizeArray &children_minSize = event.GetMinSizes();
    const wxSizeArray &children_maxSize = event.GetMaxSizes();

    bool *children_done_sizing = new bool[numchildren];
    memset(children_done_sizing, false, sizeof(bool) * numchildren);

    bool again = false;

    const wxRect &clientrect = event.GetRect();

    int available = 0;
    if( component->m_alignment == IFM_ALIGN_VERTICAL )
        available = clientrect.height;
    else if( component->m_alignment == IFM_ALIGN_HORIZONTAL )
        available = clientrect.width;

    do
    {
        again = false;
        wxRect children_combined_rect;

        for( current = 0; current < numchildren; current++ )
        {
            if( children[current]->m_hidden )
                continue;

            // keep track of sizes
            if( !children_done_sizing[current] )
            {
                children_combined_rect.width += children_rects[current].width;
                children_combined_rect.height += children_rects[current].height;
            }
        }

        // expand or shrink children to fit the client area
        float scale_factor = 0;
        if( component->m_alignment == IFM_ALIGN_VERTICAL )
            scale_factor = available / (float)children_combined_rect.height;
        else if( component->m_alignment == IFM_ALIGN_HORIZONTAL )
            scale_factor = available / (float)children_combined_rect.width;

        // calculate component sizes
        for( int i = 0; i < numchildren; ++i )
        {
            if( children[i]->m_hidden )
                continue;

            int *scaled = 0;
            int min = 0, max = 0;
            int actual = 0;

            if( component->m_alignment == IFM_ALIGN_VERTICAL )
            {
                scaled = &children_rects[i].height;
                children_rects[i].width = clientrect.width;

                min = children_minSize[i].GetHeight();
                max = children_maxSize[i].GetHeight();
            }
            else if( component->m_alignment == IFM_ALIGN_HORIZONTAL )
            {
                scaled = &children_rects[i].width;
                children_rects[i].height = clientrect.height;

                min = children_minSize[i].GetWidth();
                max = children_maxSize[i].GetWidth();
            }

            if(scaled && !children_done_sizing[i] )
            {
                actual = (int)(*scaled * scale_factor);

                if( actual < min )
                {
                    children_done_sizing[i] = true;
                    *scaled = min;
                    available -= min;
                    again = true;
                }
                else if( actual > max && max != IFM_NO_MAXIMUM )
                {
                    children_done_sizing[i] = true;
                    *scaled = max;
                    available -= max;
                    again = true;
                }
                else
                    *scaled = actual;
            }
        }

    } while(again);


    wxPoint pos = clientrect.GetPosition();

    // position visible children
    for( int i = 0; i < numchildren; ++i )
    {
        if( children[i]->m_hidden )
                continue;

        children_rects[i].y = pos.y;
        children_rects[i].x = pos.x;

        //! \todo Find a better way to fix rounding errors
        if( i + 1 == numchildren )
        {
            int *scaled = 0, actual = 0, max = 0;
            if( component->m_alignment == IFM_ALIGN_VERTICAL )
            {
                actual = clientrect.height - pos.y + clientrect.y;
                max = children_maxSize[i].GetHeight();
                scaled = &children_rects[i].height;
                
            }
            else if( component->m_alignment == IFM_ALIGN_HORIZONTAL )
            {
                scaled = &children_rects[i].width;
                actual = clientrect.width - pos.x + clientrect.x;
                max = children_maxSize[i].GetWidth();
            }

            if( actual > max && max != IFM_NO_MAXIMUM )
                actual = max;
            if(scaled) *scaled = actual; //maks
        }
        else
        {
            if( component->m_alignment == IFM_ALIGN_VERTICAL )
                pos.y += children_rects[i].height;
            else if( component->m_alignment == IFM_ALIGN_HORIZONTAL )
                pos.x += children_rects[i].width;
        }

        // return the calculated rect with the event
        event.SetComponentRect(i, children_rects[i]);
    }

    delete[] children_done_sizing;
}

void wxIFMDefaultPlugin::OnShowChild(wxIFMShowChildEvent &event)
{
    wxIFMComponent *component = event.GetComponent();

    //wxASSERT_MSG(component, wxT("NULL component?")); //maks: to avoid messages with invalid windows from menu ids 
    if( !component )
        return;

    component->Show(event.GetShow(), event.GetUpdate());
}

void wxIFMDefaultPlugin::OnShowComponent(wxIFMShowComponentEvent &event)
{
    wxIFMComponent *component = event.GetComponent();

    wxASSERT_MSG(component, wxT("NULL component?"));
    if( !component )
        return;

    bool show = event.GetShow();

    // ensure all parents are visible
    // !\todo This is kind of inneficient, each parent will check its parents who check their parents...
    if( show )
    {
        for( wxIFMComponent *parent = component->m_parent; parent; parent = parent->m_parent )
        {
            if( !parent->IsShown() )
                parent->Show(true, event.GetUpdate());
        }
    }

    component->VisibilityChanged(event.GetShow());

    if( event.GetUpdate() )
        GetIP()->GetManager()->AddPendingUpdate(IFM_DEFAULT_RECT, true);
}

void wxIFMDefaultPlugin::OnVisibilityChanged(wxIFMComponentVisibilityChangedEvent &event)
{
    wxIFMComponent *component = event.GetComponent();

    wxASSERT_MSG(component, wxT("NULL component?"));
    if( !component )
        return;

    bool show = event.GetShow();

    // send visibility change notifications to our children who need them
    const wxIFMComponentArray &components = component->m_children;
    for( size_t i = 0; i < components.GetCount(); i++ )
    //for( wxIFMComponentArray::const_iterator i = components.begin(), end = components.end(); i != end; ++i )
    {
        wxIFMComponent *child = components[i];
        if( child->IsShown() && (child->IsVisible() != show) )
            child->VisibilityChanged(show);
    }
}

#if IFM_CANFLOAT

void wxIFMDefaultPlugin::OnFloat(wxIFMFloatEvent &event)
{
    wxIFMComponent *component = event.GetComponent();

    // undock if we are docked already
    if( component->m_docked )
    {
        wxIFMUndockEvent evt(component, true);
        GetIP()->ProcessPluginEvent(evt);
    }

    // create a window with which to float
    wxIFMCreateFloatingWindowEvent wndevt(GetManager()->GetParent(), component);
    GetIP()->ProcessPluginEvent(wndevt);

    wxIFMFloatingWindowBase *base = wndevt.GetWindow();
    wxASSERT_MSG(base, wxT("NULL window returned by CreateFloatingWindow event!"));

    // size and position the window
    wxPoint pos = event.GetPosition();
    wxSize size = event.GetSize();

    // if default rect is specified, use desired size and mouse position
    if( size == IFM_DEFAULT_RECT.GetSize() )
    {
        wxIFMRectEvent evt(wxEVT_IFM_GETDESIREDSIZE, component);
        GetIP()->ProcessPluginEvent(evt);
        size = evt.GetSize();
    }

    // convert from client to absolute coordinates up through the chain of parents until
    // we reach the root component
    for( wxIFMComponent *parent = component; parent && parent != base->m_component; parent = parent->m_parent )
    {
        wxIFMConvertRectEvent cvtevt(parent, IFM_COORDS_CLIENT, IFM_COORDS_ABSOLUTE, wxPoint(), size);
        GetIP()->ProcessPluginEvent(cvtevt);
        size = cvtevt.GetSize();
    }

    wxSize display_size = wxGetDisplaySize();

    // hide the root component to make sure it doesnt display the frame before we are ready
    //! \note Yes, I know I directly access m_hidden here.
    base->m_component->m_hidden = true;

    base->GetWindow()->SetClientSize(size);
    size = base->GetWindow()->GetSize();

    // use mouse position now if needed as at this point we have an accurate size
    if( pos == IFM_DEFAULT_RECT.GetPosition() )
    {
        pos = ::wxGetMousePosition();
        pos.x -= size.GetWidth() / 2;
        
        wxIFMConvertRectEvent cvtevt(base->m_component, IFM_COORDS_FLOATINGWINDOW, IFM_COORDS_ABSOLUTE, pos, size);
        GetIP()->ProcessPluginEvent(cvtevt);
        const wxPoint &pos2 = cvtevt.GetPosition();

        pos.x = pos2.x;
        pos.y -= (pos.y - pos2.y) / 2;
    }

    // convert from absolute coords of the root component into floating coords to position and size the
    // floating window
    wxIFMConvertRectEvent cvtevt3(base->m_component, IFM_COORDS_ABSOLUTE, IFM_COORDS_FLOATINGWINDOW, pos, size);
    GetIP()->ProcessPluginEvent(cvtevt3);

    //! \todo Find out why windows get nudged to the right when double clicked when already floating
	if(event.GetConvertCoordinates()) //maks
	{
		pos = cvtevt3.GetPosition();
		size = cvtevt3.GetSize();
	}
	else
	{
		pos.x -= wxSystemSettings::GetMetric(wxSYS_EDGE_X) + wxSystemSettings::GetMetric(wxSYS_BORDER_X);
		pos.y -= wxSystemSettings::GetMetric(wxSYS_EDGE_Y) + wxSystemSettings::GetMetric(wxSYS_BORDER_Y);
	}

    {
        // dont position the window off screen
        if( pos.x < 0 )
            pos.x = 0;
        if( pos.y < 0 )
            pos.y = 0;

        if( pos.x + size.GetWidth() > display_size.GetWidth() )
            pos.x = display_size.GetWidth() - size.GetWidth();
        if( pos.y + size.GetHeight() > display_size.GetHeight() )
            pos.y = display_size.GetHeight() - size.GetHeight();
    }

    base->GetWindow()->Move(pos);

    // unhide the root component
    base->m_component->m_hidden = false;

    // show the window
    base->GetWindow()->Show();

    // updating after we show the window seems to fix broken 2.5.3 release mode behavior
    base->Update();
}

void wxIFMDefaultPlugin::OnFloatingSize(wxIFMFloatingSizeEvent &event)
{
    wxIFMFloatingWindowBase *base = event.GetWindow();

    const wxRect &client_rect = base->GetWindow()->GetClientRect();

    // set out root components desired size
    wxIFMRectEvent rectevt(wxEVT_IFM_SETDESIREDSIZE, base->GetComponent(), client_rect);
    GetIP()->ProcessPluginEvent(rectevt);

    // update our root component
    wxIFMUpdateComponentEvent updevt(base->GetComponent(), client_rect);
    GetIP()->ProcessPluginEvent(updevt);
}

void wxIFMDefaultPlugin::OnFloatingMove(wxIFMFloatingMoveEvent &event)
{
    // update the floating position of all child windows
    UpdateFloatingPos(event.GetWindow()->GetComponent());
}

void wxIFMDefaultPlugin::UpdateFloatingPos(wxIFMComponent *component)
{
    wxPoint pos = component->m_rect.GetPosition();

    wxIFMFloatingData *data = IFM_GET_EXTENSION_DATA(component, wxIFMFloatingData);

    // client to screen
    data->m_rect.SetPosition(data->m_window->GetWindow()->ClientToScreen(pos));

    // child windows too
    const wxIFMComponentArray &children = component->m_children;
    for( size_t i = 0; i < children.GetCount(); i++ )
    //for( wxIFMComponentArray ::const_iterator i = children.begin(), end = children.end(); i != end; ++i )
        UpdateFloatingPos(children[i]);
}

void wxIFMDefaultPlugin::OnFloatNotify(wxIFMFloatNotifyEvent &event)
{
    wxIFMComponent *component = event.GetComponent();
    wxIFMFloatingData *data = IFM_GET_EXTENSION_DATA(component, wxIFMFloatingData);

    data->m_floating = event.GetFloating();
    data->m_window = event.GetWindow();

    // notify our children they are now floating
    wxIFMComponentArray &children = component->m_children;
    for( int i = 0, count = children.GetCount(); i < count; ++i )
    //for( wxIFMComponentArray::iterator i = children.begin(), end = children.end(); i != end; ++i )
    {
        wxIFMFloatNotifyEvent evt(children[i], event.GetFloating(), event.GetWindow());
        GetIP()->ProcessPluginEvent(evt);
    }
}

#endif // IFM_CANFLOAT
