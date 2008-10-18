/*!
    Implementation of plugin components

    \file   plugin.cpp
    \author Robin McNeill
    \date   Created: 02/24/05

    Copyright (c) Robin McNeill
    Licensed under the terms of the wxWindows license
*/

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/ifm/plugin.h"

IMPLEMENT_DYNAMIC_CLASS(wxIFMInterfacePluginBase, wxEvtHandler);
IMPLEMENT_DYNAMIC_CLASS(wxIFMExtensionPluginBase, wxEvtHandler);

/*
wxIFMInterfacePluginBase implementation
*/
wxIFMInterfacePluginBase::wxIFMInterfacePluginBase()
    : wxEvtHandler(),
    m_manager(NULL),
    m_topExtensionPlugin(NULL)
{ }

wxInterfaceManager *wxIFMInterfacePluginBase::GetManager()
{
    return m_manager;
}

/*const*/ wxIFMComponentArray &wxIFMInterfacePluginBase::GetComponents() /*const*/ //maks
{
    return m_components;
}

#if IFM_CANFLOAT
const wxIFMFloatingWindowArray &wxIFMInterfacePluginBase::GetFloatingWindows() const
{
    return m_floatingWindows;
}
#endif

bool wxIFMInterfacePluginBase::Initialize(wxInterfaceManager *manager)
{
    m_manager = manager;
	ConnectEvents(); //maks
    
    return true;
}

void wxIFMInterfacePluginBase::ConnectEvents() //maks
{
    GetManager()->Connect(wxID_ANY, wxEVT_PAINT, wxPaintEventHandler(wxIFMInterfacePluginBase::OnPaint), NULL, this);
    GetManager()->Connect(wxID_ANY, wxEVT_SET_CURSOR, wxSetCursorEventHandler(wxIFMInterfacePluginBase::OnSetCursor), NULL, this);
    GetManager()->Connect(wxID_ANY, wxEVT_MOTION, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Connect(wxID_ANY, wxEVT_LEFT_DOWN, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Connect(wxID_ANY, wxEVT_LEFT_UP, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Connect(wxID_ANY, wxEVT_MIDDLE_DOWN, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Connect(wxID_ANY, wxEVT_MIDDLE_UP, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Connect(wxID_ANY, wxEVT_RIGHT_DOWN, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Connect(wxID_ANY, wxEVT_RIGHT_UP, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Connect(wxID_ANY, wxEVT_LEFT_DCLICK, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Connect(wxID_ANY, wxEVT_MIDDLE_DCLICK, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Connect(wxID_ANY, wxEVT_RIGHT_DCLICK, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Connect(wxID_ANY, wxEVT_MOUSEWHEEL, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Connect(wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler(wxIFMInterfacePluginBase::OnKeyDown), NULL, this);
    GetManager()->Connect(wxID_ANY, wxEVT_KEY_UP, wxKeyEventHandler(wxIFMInterfacePluginBase::OnKeyUp), NULL, this);
    GetManager()->Connect(wxID_ANY, wxEVT_SIZE, wxSizeEventHandler(wxIFMInterfacePluginBase::OnSize), NULL, this);
    GetManager()->Connect(wxID_ANY, wxEVT_SET_FOCUS, wxFocusEventHandler(wxIFMInterfacePluginBase::OnFocus), NULL, this);
    GetManager()->Connect(wxID_ANY, wxEVT_KILL_FOCUS, wxFocusEventHandler(wxIFMInterfacePluginBase::OnFocus), NULL, this);
    GetManager()->Connect(wxID_ANY, wxEVT_ACTIVATE, wxActivateEventHandler(wxIFMInterfacePluginBase::OnActivate), NULL, this);
}

void wxIFMInterfacePluginBase::DisconnectEvents() //maks
{
    GetManager()->Disconnect(wxID_ANY, wxEVT_PAINT, wxPaintEventHandler(wxIFMInterfacePluginBase::OnPaint), NULL, this);
    GetManager()->Disconnect(wxID_ANY, wxEVT_SET_CURSOR, wxSetCursorEventHandler(wxIFMInterfacePluginBase::OnSetCursor), NULL, this);
    GetManager()->Disconnect(wxID_ANY, wxEVT_MOTION, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Disconnect(wxID_ANY, wxEVT_LEFT_DOWN, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Disconnect(wxID_ANY, wxEVT_LEFT_UP, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Disconnect(wxID_ANY, wxEVT_MIDDLE_DOWN, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Disconnect(wxID_ANY, wxEVT_MIDDLE_UP, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Disconnect(wxID_ANY, wxEVT_RIGHT_DOWN, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Disconnect(wxID_ANY, wxEVT_RIGHT_UP, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Disconnect(wxID_ANY, wxEVT_LEFT_DCLICK, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Disconnect(wxID_ANY, wxEVT_MIDDLE_DCLICK, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Disconnect(wxID_ANY, wxEVT_RIGHT_DCLICK, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Disconnect(wxID_ANY, wxEVT_MOUSEWHEEL, wxMouseEventHandler(wxIFMInterfacePluginBase::OnMouseEvent), NULL, this);
    GetManager()->Disconnect(wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler(wxIFMInterfacePluginBase::OnKeyDown), NULL, this);
    GetManager()->Disconnect(wxID_ANY, wxEVT_KEY_UP, wxKeyEventHandler(wxIFMInterfacePluginBase::OnKeyUp), NULL, this);
    GetManager()->Disconnect(wxID_ANY, wxEVT_SIZE, wxSizeEventHandler(wxIFMInterfacePluginBase::OnSize), NULL, this);
    GetManager()->Disconnect(wxID_ANY, wxEVT_SET_FOCUS, wxFocusEventHandler(wxIFMInterfacePluginBase::OnFocus), NULL, this);
    GetManager()->Disconnect(wxID_ANY, wxEVT_KILL_FOCUS, wxFocusEventHandler(wxIFMInterfacePluginBase::OnFocus), NULL, this);
    GetManager()->Disconnect(wxID_ANY, wxEVT_ACTIVATE, wxActivateEventHandler(wxIFMInterfacePluginBase::OnActivate), NULL, this);
}

void wxIFMInterfacePluginBase::Shutdown()
{
    // remove extensions
    PopAllExtensionPlugins();
}

bool wxIFMInterfacePluginBase::ProcessPluginEvent(wxIFMPluginEvent &event)
{
	if(!wxInterfaceManager::GetManager()->getProcessEvents()) return false; //maks

    // initialize the component member for child events
    if( event.GetEventType() == wxEVT_IFM_SHOWCHILD || event.GetEventType() == wxEVT_IFM_HIDECHILD ||
        event.GetEventType() == wxEVT_IFM_SETCHILDSIZE || event.GetEventType() == wxEVT_IFM_QUERYCHILD)
    {
        wxIFMChildEvent *evt = wxDynamicCast(&event, wxIFMChildEvent);
        if( evt )
        {
            evt->SetComponent(FindChildWindow(evt->GetChild()));
            return GetTopPlugin()->ProcessEvent(*evt);
        }
    }

    // process the event
    bool ret = GetTopPlugin()->ProcessEvent(event);

    if( event.GetEventType() == wxEVT_IFM_NEWCOMPONENT )
    {
        wxIFMNewComponentEvent *evt = wxDynamicCast(&event, wxIFMNewComponentEvent);
        if( evt )
            m_components.push_back(evt->GetComponent());
    }
    else if( event.GetEventType() == wxEVT_IFM_DELETECOMPONENT )
    {
        wxIFMDeleteComponentEvent *evt = wxDynamicCast(&event, wxIFMDeleteComponentEvent);
        if( evt )
        {
            wxIFMComponent *component = evt->GetComponent();
            // find and remove this component from the array
            //for( int i = 0, count = m_components.GetCount(); i < count; ++i )
            for( wxIFMComponentArray::iterator i = m_components.begin(), end = m_components.end(); i != end; ++i )
            {
                if( *i /*m_components[i]*/ == component )
                {
                    //m_components.RemoveAt(i);
                    m_components.erase(i);
                    break;
                }
            }

			//maks
			if(globalPointerMap.find(component) != globalPointerMap.end())
			{
				delete component;
			}
        }
    }

    return ret;
}

void wxIFMInterfacePluginBase::AddPendingPluginEvent(wxIFMPluginEvent &event)
{
    GetTopPlugin()->AddPendingEvent(event);
}

wxIFMInterfacePluginBase::~wxIFMInterfacePluginBase()
{
    // memory cleanup
    PopAllExtensionPlugins();

    // delete components
    for( int i = 0, count = m_components.GetCount(); i < count; ++i )
    //for( wxIFMComponentArray::const_iterator i = m_components.begin(), end = m_components.end(); i != end; ++i )
	{
		//maks
		wxIFMComponent *component = m_components[i];
		if(globalPointerMap.find(component) != globalPointerMap.end())
		{
			delete component;
		}
 
        //delete *i;
	}
}

void wxIFMInterfacePluginBase::Clear() //maks
{
	int i, count;

	// delete components
    for(i = 0, count = m_components.GetCount(); i < count; ++i)
	{
		wxIFMComponent *component = m_components[i];
		if(globalPointerMap.find(component) != globalPointerMap.end())
		{
			delete component;
		}
	}

	m_components.clear();

#if IFM_CANFLOAT
	// delete floating windows
    for(i = 0, count = m_floatingWindows.GetCount(); i < count; ++i)
	{
		delete m_floatingWindows[i];
	}

	m_floatingWindows.clear();
#endif

	m_windows.clear();
}

bool wxIFMInterfacePluginBase::PushExtensionPlugin(wxIFMExtensionPluginBase *plugin)
{
    // initialize the plugin
    if( !plugin->Initialize(this) )
        return false;

    // link event handlers
    if( m_topExtensionPlugin )
    {
        plugin->SetNextHandler(m_topExtensionPlugin);
        m_topExtensionPlugin->SetPreviousHandler(plugin);
    }
    else
    {
        // link the first plugin to ourself
        plugin->SetNextHandler(this);
        SetPreviousHandler(plugin);
    }

    m_topExtensionPlugin = plugin;
    return true;
}

void wxIFMInterfacePluginBase::PopExtensionPlugin()
{
    wxASSERT_MSG(m_topExtensionPlugin, wxT("Popping from an empty plugin list"));

    m_topExtensionPlugin->Shutdown();

    // re-link event handlers
    wxEvtHandler *plugin = m_topExtensionPlugin->GetNextHandler();
    
    if( plugin )
        plugin->SetPreviousHandler(NULL);
    else
        SetPreviousHandler(NULL); // unlink the last plugin from ourself
    
    delete m_topExtensionPlugin;

    m_topExtensionPlugin = wxDynamicCast(plugin, wxIFMExtensionPluginBase);
}

void wxIFMInterfacePluginBase::PopAllExtensionPlugins()
{
    while(m_topExtensionPlugin)
        PopExtensionPlugin();
}

wxEvtHandler *wxIFMInterfacePluginBase::GetTopPlugin()
{
    if( m_topExtensionPlugin )
        return m_topExtensionPlugin;
    else
        return this;
}

void wxIFMInterfacePluginBase::Enable(bool val)
{
    wxEvtHandler::SetEvtHandlerEnabled(val);

    // enable or disable all extension plugins
    wxIFMExtensionPluginBase *plugin = m_topExtensionPlugin;
    while(plugin)
    {
        plugin->Enable(val);
        plugin = wxDynamicCast(plugin->GetNextHandler(), wxIFMExtensionPluginBase);
    }

    // enable or disable the default plugin too
    GetNextHandler()->SetEvtHandlerEnabled(val);
}

wxIFMComponent *wxIFMInterfacePluginBase::GetComponentByPos(const wxPoint &pos, wxIFMComponent *component, bool floating)
{
#if IFM_CANFLOAT
    if( floating )
    {
        wxIFMComponent *component = GetFloatingComponentByPos(m_manager->GetParent()->ClientToScreen(pos));
        if( component )
            return component;
    }
#endif

    if( component == NULL )
    {
        wxIFMComponent *ret;
        for( int i = 0, count = m_components.GetCount(); i < count; ++i )
        //for( wxIFMComponentArray::const_iterator i = m_components.begin(), end = m_components.end(); i != end; ++i )
        {
            ret = GetComponentByPos(pos, m_components[i]);
            if( ret )
                return ret;
        }
        return NULL;
    }

    //wxIFMHitTestEvent evt(component, IFM_COORDS_ABSOLUTE, pos);
    //ProcessPluginEvent(evt);

    //if( evt.GetPassed() )
    if( !component->m_hidden && component->m_rect.Inside(pos) )
    {
        // check children
        wxIFMComponent *ret;
        const wxIFMComponentArray &components = component->m_children;
        for( int i = 0, count = components.GetCount(); i < count; ++i )
        //for( wxIFMComponentArray::const_iterator i = components.begin(), end = components.end(); i != end; ++i )
        {
            ret = GetComponentByPos(pos, components[i]);
            if( ret )
                return ret;
        }
        return component;
    }

    return NULL;
}

#if IFM_CANFLOAT
wxIFMComponent *wxIFMInterfacePluginBase::GetFloatingComponentByPos(const wxPoint &pos)
{
    wxIFMFloatingWindowBase *base;
    wxIFMComponent *ret;

    for( int i = 0, size = m_floatingWindows.GetCount(); i < size; ++i )
    //for( wxIFMFloatingWindowArray::const_iterator i = m_floatingWindows.begin(), end = m_floatingWindows.end(); i != end; ++i )
    {
        base = m_floatingWindows[i];
        //base = *i;

        ret = GetComponentByPos(base->GetWindow()->ScreenToClient(pos), base->GetComponent());
        if( ret )
            return ret;
    }

    return NULL;
}
#endif

void wxIFMInterfacePluginBase::AddChildWindow(wxWindow *window, wxIFMComponent *component)
{
    // store the child window / component pair
    m_windows[(int)window] = component;
}

wxIFMComponent *wxIFMInterfacePluginBase::FindChildWindow(wxWindow *window)
{
    return m_windows[(int)window];
}

void wxIFMInterfacePluginBase::OnMouseEvent(wxMouseEvent &event)
{
    wxEventType type = 0, _type = event.GetEventType();

    if( _type == wxEVT_LEFT_DOWN )
        type = wxEVT_IFM_LEFTDOWN;
    else if( _type == wxEVT_LEFT_UP )
        type = wxEVT_IFM_LEFTUP;
    else if( _type == wxEVT_LEFT_DCLICK )
        type = wxEVT_IFM_LEFTDCLICK;
    else if( _type == wxEVT_RIGHT_DOWN )
        type = wxEVT_IFM_RIGHTDOWN;
    else if( _type == wxEVT_RIGHT_UP )
        type = wxEVT_IFM_RIGHTUP;
    else if( _type == wxEVT_RIGHT_DCLICK )
        type = wxEVT_IFM_RIGHTDCLICK;
    else if( _type == wxEVT_MIDDLE_DOWN )
        type = wxEVT_IFM_MIDDLEDOWN;
    else if( _type == wxEVT_MIDDLE_UP )
        type = wxEVT_IFM_MIDDLEUP;
    else if( _type == wxEVT_MIDDLE_DCLICK )
        type = wxEVT_IFM_MIDDLEDCLICK;
    else if( _type == wxEVT_MOTION )
        type = wxEVT_IFM_MOTION;
    else if( _type == wxEVT_MOUSEWHEEL )
        type = wxEVT_IFM_MOUSEWHEEL;

    // figure out which component the mouse is over
    wxIFMComponent *component = NULL;

    if( m_manager->IsInputCaptured() )
        component = m_manager->GetCapturedComponent();
    else
        component = GetComponentByPos(event.GetPosition());

    // generate mouse event
    wxIFMMouseEvent evt(type, component, event);
    if( !ProcessPluginEvent(evt) )
        event.Skip();
}

void wxIFMInterfacePluginBase::OnSetCursor(wxSetCursorEvent &event)
{
    wxIFMSetCursorEvent evt(event, GetComponentByPos(wxPoint(event.GetX(), event.GetY())));
    if( !ProcessPluginEvent(evt) )
        event.Skip();
}

void wxIFMInterfacePluginBase::OnKeyDown(wxKeyEvent &event)
{
    wxIFMKeyEvent evt(wxEVT_IFM_KEYDOWN, m_manager->GetCapturedComponent(), event);
    if( !ProcessPluginEvent(evt) )
        event.Skip();
}

void wxIFMInterfacePluginBase::OnKeyUp(wxKeyEvent &event)
{
    wxIFMKeyEvent evt(wxEVT_IFM_KEYUP, m_manager->GetCapturedComponent(), event);
    if( !ProcessPluginEvent(evt) )
        event.Skip();
}

void wxIFMInterfacePluginBase::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    // send BEGINPAINT message to get a DC with which to paint
    wxIFMBeginPaintEvent beginpaint(m_manager->GetParent());
    ProcessPluginEvent(beginpaint);

    // make sure we have a valid dc
    wxDC *dc = beginpaint.GetDC();
    wxASSERT_MSG(dc, wxT("Invalid DC returned by EVT_IFM_BEGINPAINT"));

    // send paint interface event
    wxIFMPaintInterfaceEvent paintevt(*dc, GetManager()->GetParent()->GetUpdateRegion());
    ProcessPluginEvent(paintevt);

    // send ENDPAINT message to clean up the DC used to paint
    wxIFMEndPaintEvent endpaint(dc);
    ProcessPluginEvent(endpaint);
}

void wxIFMInterfacePluginBase::OnSize(wxSizeEvent &event)
{
    // update the interface
    if( m_manager->GetFlags() & IFM_NO_RESIZE_UPDATE )
        event.Skip();
    else
        m_manager->Update();
}

void wxIFMInterfacePluginBase::OnFocus(wxFocusEvent &event)
{
    // send focus event
    wxIFMFocusEvent evt((event.GetEventType() == wxEVT_SET_FOCUS) ? wxEVT_IFM_SETFOCUS : wxEVT_IFM_KILLFOCUS, event);
    if( !ProcessPluginEvent(evt) )
        event.Skip();
}

void wxIFMInterfacePluginBase::OnActivate(wxActivateEvent &event)
{
    wxIFMActivateEvent evt(event);
    if( !ProcessPluginEvent(evt) )
        event.Skip();
}

/*
wxIFMExtensionPluginBase implementation
*/
wxIFMExtensionPluginBase::wxIFMExtensionPluginBase()
    : wxEvtHandler(),
    m_ip(NULL)
{ }

wxIFMExtensionPluginBase::~wxIFMExtensionPluginBase()
{ }

wxInterfaceManager *wxIFMExtensionPluginBase::GetManager()
{
    return m_ip->GetManager();
}

wxIFMInterfacePluginBase *wxIFMExtensionPluginBase::GetIP()
{
    return m_ip;
}

bool wxIFMExtensionPluginBase::Initialize(wxIFMInterfacePluginBase *plugin)
{
    wxASSERT_MSG(!m_ip, wxT("Interface plugin has already been set!"));
    m_ip = plugin;
    return true;
}

void wxIFMExtensionPluginBase::Shutdown()
{ }

void wxIFMExtensionPluginBase::Enable(bool val)
{
    wxEvtHandler::SetEvtHandlerEnabled(val);
}

void wxIFMExtensionPluginBase::Disable()
{
    Enable(false);
}
