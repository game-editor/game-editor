/*!
    Implementation file for core IFM components

    \file   manager.cpp
    \author Robin McNeill
    \date   Created: 10/15/04

    Copyright (c) Robin McNeill
    Licensed under the terms of the wxWindows license
*/

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/ifm/manager.h"
#include "wx/ifm/plugin.h"
#include "wx/ifm/definterface.h"
#include "wx/ifm/defplugin.h"
#include "wx/ifm/resize.h" //maks

#include "wx/statusbr.h"
#include "wx/dc.h"




DEFINE_IFM_DATA_KEY(IFM_FLOATING_DATA_KEY)

DEFINE_IFM_CHILD_TYPE(IFM_CHILD_GENERIC)
DEFINE_IFM_CHILD_TYPE(IFM_CHILD_TOOLBAR)

IMPLEMENT_DYNAMIC_CLASS(wxIFMChildDataBase, wxObject);

#include <wx/arrimpl.cpp>
WX_DEFINE_EXPORTED_OBJARRAY(wxRectArray);
WX_DEFINE_EXPORTED_OBJARRAY(wxSizeArray);

WX_DEFINE_OBJARRAY(wxIFMLayoutTabDataArray); //maks
WX_DEFINE_OBJARRAY(wxIFMLayoutPanelDataArray); //maks
WX_DEFINE_OBJARRAY(wxIFMLayoutContainerDataArray); //maks

wxPointerMap globalPointerMap; //maks

#define CURRENT_LAYOUT_VERSION 11 //maks

wxIFMComponentDataKeyType GetNewDataKey()
{
    static int next;

    return next++;
}

int GetNewComponentType()
{
    static int next = 1;

    return next++;
}

int GetNewChildType()
{
    static int next = 1;

    return next++;
}

/*
wxInterfaceManager implementation
*/

wxInterfaceManager *wxInterfaceManager::m_manager = NULL; //maks
bool wxInterfaceManager::bProcessEvents = true; //maks

wxInterfaceManager::wxInterfaceManager(wxWindow *parent, wxWindow *content)
    : m_initialized(false),
	m_flags(IFM_DEFAULT_FLAGS),
    m_parent(parent),
    m_content(content),
    m_capturedComponent(NULL),
    m_capturedType(IFM_COMPONENT_UNDEFINED),
#if IFM_CANFLOAT
    m_floatingCapture(NULL),
#endif
    m_useUpdateRect(false),
    m_statusbar(NULL),
    m_statusbarPane(IFM_DISABLE_STATUS_MESSAGES),
    m_statusMessageDisplayed(false)
{
    wxASSERT_MSG(m_parent, wxT("An interface cannot have a NULL parent"));
	m_manager = this; //maks
}

wxInterfaceManager::~wxInterfaceManager()
{
	m_manager = NULL; //maks
}

wxWindow *wxInterfaceManager::GetParent() const
{
    return m_parent;
}

const wxRect &wxInterfaceManager::GetInterfaceRect() const
{
    return m_updateRect;
}

void wxInterfaceManager::SetInterfaceRect(const wxRect &rect)
{
    m_useUpdateRect = true;
    m_updateRect = rect;
}

void wxInterfaceManager::ResetInterfaceRect()
{
    m_useUpdateRect = false;
}

wxWindow *wxInterfaceManager::GetContentWindow() const
{
    return m_content;
}

void wxInterfaceManager::SetContentWindow(wxWindow *content)
{
    m_content = content;
}

void wxInterfaceManager::SetFlags(int flags)
{
    m_flags = flags;
}

int wxInterfaceManager::GetFlags() const
{
    return m_flags;
}

void wxInterfaceManager::HideChild(wxWindow *child, bool update)
{
    ShowChild(child, false, update);
}

wxIFMInterfacePluginBase *wxInterfaceManager::GetActiveIP() const
{
    return m_interfacePlugins[m_activeInterface];
}

void wxInterfaceManager::setProcessEvents(bool value) //maks
{
	bProcessEvents = value;
	if(bProcessEvents)
	{
		wxInterfaceManager::GetManager()->GetActiveIP()->ConnectEvents();
	}
	else
	{
		wxInterfaceManager::GetManager()->GetActiveIP()->DisconnectEvents();
	}
}

bool wxInterfaceManager::IsInputCaptured() const
{
    return m_capturedComponent != NULL;
}

wxIFMComponent *wxInterfaceManager::GetCapturedComponent() const
{
    return m_capturedComponent;
}

bool wxInterfaceManager::Initialize(bool defaultPlugins, int flags)
{
    m_flags = flags;

    // event spying
    m_parent->PushEventHandler(this);

    if( defaultPlugins )
    {
        // load default interface plugin
        wxIFMInterfacePluginBase *plugin = new wxIFMDefaultInterfacePlugin();
        if( AddInterfacePlugin(plugin) == -1 )
        {
            delete plugin;
            return (m_initialized = false);
        }
    }

    return (m_initialized = true);
}

void wxInterfaceManager::Shutdown()
{
    wxASSERT_MSG(m_initialized, wxT("Interface not initialized"));

    RemoveAllInterfacePlugins();
    m_parent->RemoveEventHandler(this);
}

bool wxInterfaceManager::AddChild(wxIFMChildDataBase *data, bool update)
{
    wxASSERT_MSG(m_initialized, wxT("Interface not initialized"));
    wxASSERT_MSG(data, wxT("Child data cannot be NULL."));

    wxASSERT_MSG(data->m_child, wxT("Adding a NULL child?"));
    wxASSERT_MSG(data->m_type != IFM_CHILDTYPE_UNDEFINED, wxT("Must specify a child type"));

    // create event
    wxIFMAddChildEvent event(data);
    
    // process event
    GetActiveIP()->ProcessPluginEvent(event);
    if( !event.GetSuccess() )
        return false;

    // update interface?
    if( update )
        Update();

	mapWindowsName[data->m_name] = wxIFMWindowsData(data->m_child, data->m_minSize, data->m_maxSize);
    return true;
}

const wxIFMWindowsData *wxInterfaceManager::GetWindow(wxString& name) //maks
{
	if(mapWindowsName.find(name) != mapWindowsName.end())
	{
		return &mapWindowsName[name];
	}

	return NULL;
}

void wxInterfaceManager::ShowChild(wxWindow *child, bool show, bool update)
{
    // send showchild event
    wxIFMShowChildEvent event(child, show, update);
    GetActiveIP()->ProcessPluginEvent(event);
}

bool wxInterfaceManager::IsChildVisible(wxWindow *child) 
{
    wxIFMQueryChildEvent event(child);
    if(GetActiveIP()->ProcessPluginEvent(event))
        return event.IsVisible();

    return false;
}

void wxInterfaceManager::SetChildSize(wxWindow *child, const wxSize &desired, const wxSize &min,
                                      const wxSize &max, bool update)
{
    // send setchildsize event
    wxIFMSetChildSizeEvent event(child, desired, min, max, update);
    GetActiveIP()->ProcessPluginEvent(event);
}

int wxInterfaceManager::AddInterfacePlugin(wxIFMInterfacePluginBase *plugin, bool select)
{
    // add the default component extension as the very _last_ plugin in the 
    // interface event handler chain so that it always exists and always goes last
    {
        wxIFMDefaultPlugin *_plugin = new wxIFMDefaultPlugin(plugin);
        plugin->SetNextHandler(_plugin);
        _plugin->SetPreviousHandler(plugin);
    }

    if( !plugin->Initialize(this) )
        return -1;

    m_interfacePlugins.push_back(plugin);

    int index = m_interfacePlugins.size() - 1;

    if( select )
        SetActiveInterface(m_interfacePlugins.size() - 1);

    return index;
}

void wxInterfaceManager::SetActiveInterface(int plugin)
{
    for( int i = 0, count = m_interfacePlugins.GetCount(); i < count; ++i )
    //int n = 0;
    //for( wxIFMInterfacePluginArray::const_iterator i = m_interfacePlugins.begin(), end = m_interfacePlugins.end(); i != end; ++i, n++ )
    {
        if( i == plugin )
        //if( n == plugin )
            m_interfacePlugins[i]->Enable();
            //(*i)->Enable();
        else
            m_interfacePlugins[i]->Disable();
            //(*i)->Disable();
    }

    m_activeInterface = plugin;
}

void wxInterfaceManager::RemoveInterfacePlugin(int interface_index)
{
    m_interfacePlugins[interface_index]->Shutdown();

    // remove and delete default component plugin
    wxIFMInterfacePluginBase *plugin = m_interfacePlugins[interface_index];
    wxIFMDefaultPlugin *_plugin = wxDynamicCast(plugin->GetNextHandler(), wxIFMDefaultPlugin);

    delete _plugin;
    delete plugin;

    m_interfacePlugins.erase(m_interfacePlugins.begin() + interface_index);
}

void wxInterfaceManager::RemoveAllInterfacePlugins()
{
    while( m_interfacePlugins.size() > 0 )
        RemoveInterfacePlugin(0);
}

bool wxInterfaceManager::AddExtensionPlugin(wxIFMExtensionPluginBase *plugin/*, int interface_index*/)
{
    return GetActiveIP()->PushExtensionPlugin(plugin);
}

void wxInterfaceManager::RemoveExtensionPlugin(/*int interface_index*/)
{
    GetActiveIP()->PopExtensionPlugin();
}

void wxInterfaceManager::RemoveAllExtensionPlugins(/*int interface_index*/)
{
    GetActiveIP()->PopAllExtensionPlugins();
}

void wxInterfaceManager::CaptureInput(wxIFMComponent *component)
{
    wxASSERT_MSG(component, wxT("Invalid component attempting to capture input"));
    wxASSERT_MSG(!m_capturedComponent, wxT("A component already has captured input!"));

#if IFM_CANFLOAT
    wxIFMFloatingData *data = IFM_GET_EXTENSION_DATA(component, wxIFMFloatingData);
    if( data->m_floating )
    {
        m_floatingCapture = data->m_window;
        m_floatingCapture->GetWindow()->CaptureMouse();
    }
    else
#endif
        m_parent->CaptureMouse();

    m_capturedComponent = component;
    m_capturedType = component->GetType();
}

void wxInterfaceManager::ReleaseInput()
{
    wxASSERT_MSG(m_capturedComponent, wxT("Releasing input capture but nothing has captured it!"));
    if( !m_capturedComponent )
        return;

#if IFM_CANFLOAT
    if( m_floatingCapture )
    {
        if(m_floatingCapture->GetWindow()->HasCapture()) m_floatingCapture->GetWindow()->ReleaseMouse(); //maks
        m_floatingCapture = NULL;
    }
    else
#endif
        if(m_parent->HasCapture()) m_parent->ReleaseMouse(); //maks

    m_capturedComponent = NULL;
    m_capturedType = IFM_COMPONENT_UNDEFINED;
}

wxWindow *wxInterfaceManager::GetCapturedWindow() const
{
    if( !m_capturedComponent )
        return NULL;

#if IFM_CANFLOAT
    if( m_floatingCapture )
        return m_floatingCapture->GetWindow();
    else
#endif
        return m_parent;
}

void wxInterfaceManager::Update(wxRect rect, bool floating)
{
	if(!bProcessEvents) return; //maks

    wxRect *_rect;

    // if the application gave us a rect to use, use it
    if( m_useUpdateRect )
        _rect = &m_updateRect;
    else
    {
        if( rect == IFM_DEFAULT_RECT )
            m_updateRect = m_parent->GetClientRect();
        else
        {
            m_updateRect = rect;
            m_useUpdateRect = true;
        }
        _rect = &m_updateRect;
    }

    // generate update interface event
    wxIFMUpdateEvent updevt(m_content, *_rect, floating);
    GetActiveIP()->ProcessPluginEvent(updevt);
}

void wxInterfaceManager::AddPendingUpdate(wxRect rect, bool floating)
{
    wxRect *_rect;

    // if the application gave us a rect to use, use it
    if( m_useUpdateRect )
        _rect = &m_updateRect;
    else
    {
        if( rect == IFM_DEFAULT_RECT )
            m_updateRect = m_parent->GetClientRect();
        else
        {
            m_updateRect = rect;
            m_useUpdateRect = true;
        }
        _rect = &m_updateRect;
    }

    // generate update interface event
    wxIFMUpdateEvent updevt(m_content, *_rect, floating);
    GetActiveIP()->AddPendingEvent(updevt);
}

void wxInterfaceManager::UpdateConfiguration()
{
	if(!bProcessEvents) return; //maks

    // tell plugins their configuration data has changed
    wxIFMUpdateConfigEvent event;
    GetActiveIP()->ProcessPluginEvent(event);

    // update the interface including floating windows
    Update(IFM_DEFAULT_RECT, true);
	
	//maks: Update sash (need to unusable sash remotion)
	const wxIFMComponentList &m_containers = wxDynamicCast(GetActiveIP(), wxIFMDefaultInterfacePlugin)->GetTopContainerList();
	for(int i = 0; i < m_containers.size(); ++i )
	{
		wxIFMUpdateResizeSashEvent evt(m_containers[i]);
		GetActiveIP()->ProcessPluginEvent(evt);
	}
}

void wxInterfaceManager::SetStatusMessagePane(wxStatusBar *sb, int pane)
{
    wxASSERT_MSG(sb, wxT("NULL status bar?"));
    m_statusbar = sb;
    m_statusbarPane = pane;
}

void wxInterfaceManager::DisplayStatusMessage(const wxString &message)
{
    if( m_statusbarPane != IFM_DISABLE_STATUS_MESSAGES )
    {
        if( !m_statusMessageDisplayed )
        {
            m_statusMessageDisplayed = true;
            m_oldStatusMessage = m_statusbar->GetStatusText(m_statusbarPane);
        }

        m_statusbar->SetStatusText(message, m_statusbarPane);
    }
}

void wxInterfaceManager::ResetStatusMessage()
{
    if( m_statusbarPane != IFM_DISABLE_STATUS_MESSAGES )
    {
        if( m_statusMessageDisplayed )
        {
            m_statusMessageDisplayed = false;
            m_statusbar->SetStatusText(m_oldStatusMessage, m_statusbarPane);
            m_oldStatusMessage = wxT("");
        }
    }
}

#if IFM_CANFLOAT

/*
wxIFMFloatingWindow implementation
*/
BEGIN_EVENT_TABLE(wxIFMFloatingWindowBase, wxEvtHandler)
    EVT_MOUSE_EVENTS (wxIFMFloatingWindowBase::OnMouseEvent)
    EVT_PAINT       (wxIFMFloatingWindowBase::OnPaint)
    EVT_MOVE        (wxIFMFloatingWindowBase::OnMoving)
    EVT_MOVING      (wxIFMFloatingWindowBase::OnMoving)
    EVT_SIZE        (wxIFMFloatingWindowBase::OnSize)
    EVT_SIZING      (wxIFMFloatingWindowBase::OnSize)
    EVT_KEY_DOWN    (wxIFMFloatingWindowBase::OnKeyDown)
    EVT_KEY_UP      (wxIFMFloatingWindowBase::OnKeyUp)
    EVT_SET_CURSOR  (wxIFMFloatingWindowBase::OnSetCursor)
    EVT_SHOW        (wxIFMFloatingWindowBase::OnShow)
    EVT_ERASE_BACKGROUND (wxIFMFloatingWindowBase::OnEraseBg)
END_EVENT_TABLE()

wxIFMFloatingWindowBase::wxIFMFloatingWindowBase(wxIFMInterfacePluginBase *ip, wxWindow *parent,
        wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
    : m_ip(ip),
    m_component(NULL),
    m_destroyRoot(true)
{
    m_window = new wxWindow(parent, id, pos, size, style, name);
    ConnectEvents();
}

wxIFMFloatingWindowBase::wxIFMFloatingWindowBase(wxIFMInterfacePluginBase *ip)
    : m_ip(ip)
{ }

wxIFMFloatingWindowBase::~wxIFMFloatingWindowBase()
{
    DisconnectEvents();
    m_window->Destroy();

    // delete our root container
    if( m_destroyRoot && globalPointerMap.find(m_component) != globalPointerMap.end()) //maks
    {
        wxIFMDeleteComponentEvent evt(m_component);
        GetIP()->ProcessPluginEvent(evt);
    }
}

wxIFMInterfacePluginBase *wxIFMFloatingWindowBase::GetIP()
{
    return m_ip;
}

wxInterfaceManager *wxIFMFloatingWindowBase::GetManager()
{
    return m_ip->GetManager();
}

wxWindow *wxIFMFloatingWindowBase::GetWindow() const
{
    return m_window;
}

wxIFMComponent *wxIFMFloatingWindowBase::GetComponent() const
{
    return m_component;
}

void wxIFMFloatingWindowBase::ConnectEvents()
{
    m_window->PushEventHandler(this);
}

void wxIFMFloatingWindowBase::DisconnectEvents()
{
    m_window->RemoveEventHandler(this);
}

void wxIFMFloatingWindowBase::OnSize(wxSizeEvent &event)
{
    wxIFMFloatingSizeEvent sizeevt(
        (event.GetEventType() == wxEVT_SIZE ? wxEVT_IFM_FLOATING_SIZE : wxEVT_IFM_FLOATING_SIZING),
        this, event);
    if( !GetIP()->ProcessPluginEvent(sizeevt) )
        event.Skip();
}

void wxIFMFloatingWindowBase::OnMoving(wxMoveEvent &event)
{
    wxIFMFloatingMoveEvent moveevt(
        (event.GetEventType() == wxEVT_MOVE ? wxEVT_IFM_FLOATING_MOVE : wxEVT_IFM_FLOATING_MOVING),
        this, event);
    if( !GetIP()->ProcessPluginEvent(moveevt) )
        event.Skip();
}

void wxIFMFloatingWindowBase::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    // send BEGINPAINT message to get a DC with which to paint
    wxIFMBeginPaintEvent beginpaint(m_window);
    GetIP()->ProcessPluginEvent(beginpaint);

    wxDC *dc = beginpaint.GetDC();
    wxASSERT_MSG(dc, wxT("Invalid DC returned by EVT_IFM_BEGINPAINT"));

    m_component->Paint(*dc, m_window->GetUpdateRegion());

    // send ENDPAINT message to clean up the DC used to paint
    wxIFMEndPaintEvent endpaint(dc);
    GetIP()->ProcessPluginEvent(endpaint);
}

void wxIFMFloatingWindowBase::OnKeyDown(wxKeyEvent &event)
{
    wxIFMKeyEvent evt(wxEVT_IFM_KEYDOWN, GetManager()->GetCapturedComponent(), event);
    if( !GetIP()->ProcessPluginEvent(evt) )
        event.Skip();
}

void wxIFMFloatingWindowBase::OnKeyUp(wxKeyEvent &event)
{
    wxIFMKeyEvent evt(wxEVT_IFM_KEYUP, GetManager()->GetCapturedComponent(), event);
    if( !GetIP()->ProcessPluginEvent(evt) )
        event.Skip();
}

void wxIFMFloatingWindowBase::OnSetCursor(wxSetCursorEvent &event)
{
    wxIFMSetCursorEvent evt(event, GetComponentByPos(wxPoint(event.GetX(), event.GetY())));
    if( !GetIP()->ProcessPluginEvent(evt) )
        event.Skip();
}

void wxIFMFloatingWindowBase::OnMouseEvent(wxMouseEvent &event)
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

    if( GetManager()->IsInputCaptured() )
        component = GetManager()->GetCapturedComponent();
    else
        component = GetComponentByPos(event.GetPosition());

    // convert event position into screen coordinates first
    /*
    wxPoint screen = m_window->ClientToScreen(event.GetPosition());
    event.m_x = screen.x;
    event.m_y = screen.y;
    */

    // generate mouse event
    wxIFMMouseEvent evt(type, component, event);
    if( !GetIP()->ProcessPluginEvent(evt) )
        event.Skip();
}

void wxIFMFloatingWindowBase::OnShow(wxShowEvent &event)
{
    m_component->Show(event.GetShow(), true);
}

void wxIFMFloatingWindowBase::OnEraseBg(wxEraseEvent &WXUNUSED(event))
{

}

wxIFMComponent *wxIFMFloatingWindowBase::GetComponentByPos(const wxPoint &pos, wxIFMComponent *component)
{
    if( !m_window->IsShown() )
        return NULL;
    else
        return GetIP()->GetComponentByPos(pos, (component == NULL) ? m_component : component);
}

void wxIFMFloatingWindowBase::Update(bool force)
{
    if( m_window->IsShown() || force )
    {
        wxIFMUpdateComponentEvent updevt(m_component, m_component->m_rect);
        GetIP()->ProcessPluginEvent(updevt);
    }
}

void wxIFMFloatingWindowBase::AddPendingUpdate()
{
    if( m_window->IsShown() )
    {
        wxIFMUpdateComponentEvent updevt(m_component, m_component->m_rect);
        GetIP()->AddPendingPluginEvent(updevt);
    }
}

#endif

/*
wxIFMComponent implementation
*/
wxIFMComponent::wxIFMComponent(wxIFMInterfacePluginBase *ip, int type)
    : m_type(type),
    m_ip(ip),
    m_minSize(IFM_NO_MINIMUM_SIZE),
    m_maxSize(IFM_NO_MAXIMUM_SIZE),
    m_fixed(false),
    m_hidden(false),
    m_visible(true),
    m_canHide(true),
    m_docked(false),
    m_alignment(IFM_ALIGN_NONE),
    m_parent(NULL),
    m_childType(IFM_CHILDTYPE_UNDEFINED),
    m_child(NULL)
{
	globalPointerMap[this] = 1;	//maks

#if IFM_CANFLOAT
    // create floating data data
    wxIFMExtensionDataBase *data = new wxIFMFloatingData;
    m_data[data->GetDataKey()] = data;
#endif
}

wxIFMComponent::wxIFMComponent(const wxIFMComponent &)
{ 
	globalPointerMap[this] = 1;	//maks
}

wxIFMComponent::~wxIFMComponent()
{
	globalPointerMap.erase(this);	//maks	

    // clean up data
    for( wxIFMComponentDataMap::iterator i = m_data.begin(), end = m_data.end(); i != end; ++i )
        delete i->second;

	//maks
	/*int i1, count;

	// delete children
    for(i1 = 0, count = m_children.GetCount(); i1 < count; ++i1)
	{
		delete m_children[i1];
	}

	m_children.clear();*/
}

wxInterfaceManager *wxIFMComponent::GetManager()
{
    return m_ip->GetManager();
}

int wxIFMComponent::GetType() const
{
    return m_type;
}

void wxIFMComponent::AddExtensionData(wxIFMExtensionDataBase *data)
{
    m_data[data->GetDataKey()] = data;
}

wxIFMExtensionDataBase *wxIFMComponent::GetExtensionData(wxIFMComponentDataKeyType key)
{
    return m_data[key];
}

wxIFMExtensionDataBase *wxIFMComponent::RemoveExtensionData(wxIFMComponentDataKeyType key)
{
    wxIFMExtensionDataBase *ret = m_data[key];
    m_data.erase(key);
    return ret;
}

int wxIFMComponent::GetNextVisibleComponent(const wxIFMComponentArray &components, int start)
{
    for( int i = start, count = components.GetCount(); i < count; ++i )
    {
        if( components[i]->IsVisible() )
            return i;
    }

    // no visible component found
    return -1;
}

bool wxIFMComponent::IsChildOf(wxIFMComponent *parent, wxIFMComponent *child)
{
    wxIFMComponentArray &children = parent->m_children;
    wxIFMComponent *component;
    for( int i = 0, size = children.GetCount(); i < size; ++i )
    //for( wxIFMComponentArray::const_iterator i = children.begin(), end = children.end(); i != end; ++i )
    {
        component = children[i];
        //component = *i;

        if( component == child )
            return true;
        else
        {
            // look through children of the child too
            if( wxIFMComponent::IsChildOf(component, child) )
                return true;
        }
    }

    return false;
}

/*
wxIFMComponent *wxIFMComponent::FindChildWindow(wxWindow *child, const wxIFMComponentArray &components)
{
    wxIFMComponent *component;

    // look for the component containing the child window
    //for( int i = 0, count = components.GetCount(); i < count; ++i )
    for( wxIFMComponentArray::const_iterator i = components.begin(), end = components.end(); i != end; ++i )
    {
        //component = components[i];
        component = *i;

        if( component->m_child == child )
            return component;
    }

    // no component found
    return NULL;
}
*/

void wxIFMComponent::Paint(wxDC &dc, const wxRegion &region)
{
    // get component rect first
    wxRect rect = m_rect;

    // set clipping region of DC
    dc.DestroyClippingRegion();
    dc.SetClippingRegion(region);

    // paint background first
    wxIFMPaintEvent bgevt(wxEVT_IFM_PAINTBG, this, region, dc);
    m_ip->ProcessPluginEvent(bgevt);

    // paint border second
    wxIFMPaintEvent bdevt(wxEVT_IFM_PAINTBORDER, this, region, dc);
    m_ip->ProcessPluginEvent(bdevt);

    // paint decorations last
    wxIFMPaintEvent dcevt(wxEVT_IFM_PAINTDECOR, this, region, dc);
    m_ip->ProcessPluginEvent(dcevt);

    // recursively paint children of this component
    for( size_t i = 0; i < m_children.GetCount(); i++ )
    //for( wxIFMComponentArray::const_iterator i = m_children.begin(), end = m_children.end(); i != end; ++i )
    {
        //wxIFMComponent *child = *i;
        wxIFMComponent *child = m_children[i];

        // only paint the child if needed
        if( child->IsVisible() )
        {
            wxRegionContain result = region.Contains(child->m_rect);
            if( result == wxPartRegion || result == wxInRegion )
            {
                //wxRegion new_region = region;
                //new_region.Intersect(child->m_rect);
                child->Paint(dc, region);
            }
        }
    }
}

wxWindow *wxIFMComponent::GetParentWindow()
{
#if IFM_CANFLOAT
    wxIFMFloatingData *data = IFM_GET_EXTENSION_DATA(this, wxIFMFloatingData);
    if( data->m_floating )
        return data->m_window->GetWindow();
    else
#endif
        return GetManager()->GetParent();
}

wxRect wxIFMComponent::GetRect()
{
    // FIXME: should I use GETRECT event here?
    return m_rect;
}

wxRect wxIFMComponent::GetBackgroundRect()
{
    wxIFMRectEvent evt(wxEVT_IFM_GETBACKGROUNDRECT, this);
    m_ip->ProcessPluginEvent(evt);
    return evt.GetRect();
}

wxRect wxIFMComponent::GetClientRect()
{
    wxIFMRectEvent evt(wxEVT_IFM_GETCLIENTRECT, this);
    m_ip->ProcessPluginEvent(evt);
    return evt.GetRect();
}

wxRect wxIFMComponent::GetConvertedRect(wxRect rect, int coords_from, int coords_to)
{
    wxIFMConvertRectEvent evt(this, coords_from, coords_to, rect);
    m_ip->ProcessPluginEvent(evt);
    return evt.GetRect();
}

wxSize wxIFMComponent::GetDesiredSize()
{
    wxIFMRectEvent evt(wxEVT_IFM_GETDESIREDSIZE, this);
    m_ip->ProcessPluginEvent(evt);
    return evt.GetSize();
}

void wxIFMComponent::SetDesiredSize(const wxSize &size)
{
    wxIFMRectEvent evt(wxEVT_IFM_SETDESIREDSIZE, this, wxPoint(), size);
    m_ip->ProcessPluginEvent(evt);
}

void wxIFMComponent::SetRect(const wxRect &rect) //maks
{
    wxIFMRectEvent evt(wxEVT_IFM_SETRECT, this, rect);
    m_ip->ProcessPluginEvent(evt);
}

wxSize wxIFMComponent::GetMinSize()
{
    wxIFMRectEvent evt(wxEVT_IFM_GETMINSIZE, this);
    m_ip->ProcessPluginEvent(evt);
    return evt.GetSize();
}

wxSize wxIFMComponent::GetMaxSize()
{
    wxIFMRectEvent evt(wxEVT_IFM_GETMAXSIZE, this);
    m_ip->ProcessPluginEvent(evt);
    return evt.GetSize();
}

void wxIFMComponent::Show(bool s, bool update)
{
    wxIFMShowComponentEvent evt(this, s, update);
    m_ip->ProcessPluginEvent(evt);
    m_hidden = !s;
}

bool wxIFMComponent::IsShown()
{
    return !m_hidden;
}

void wxIFMComponent::VisibilityChanged(bool vis)
{
    if( vis == m_visible )
        return; // we are alread visible

    wxIFMComponentVisibilityChangedEvent evt(this, vis);
    m_ip->ProcessPluginEvent(evt);
    m_visible = vis;
}

bool wxIFMComponent::IsVisible()
{
    return (!m_hidden) & m_visible;
}

/*
    wxIFMChildDataBase implementation
*/
wxIFMChildDataBase::wxIFMChildDataBase()
    : m_type(IFM_CHILDTYPE_UNDEFINED),
    m_minSize(IFM_NO_MINIMUM_SIZE),
    m_maxSize(IFM_NO_MAXIMUM_SIZE),
    m_child(NULL),
    m_hidden(false),
    m_hideable(true),
	bCanDrag(true), //maks
    m_fixed(false)
{ }

wxIFMChildDataBase::wxIFMChildDataBase(wxWindow *child, int type, const wxString &name,
    wxSize size, bool hidden, wxSize minSize, wxSize maxSize)
    : m_type(type),
    m_desiredSize(size),
    m_minSize(minSize),
    m_maxSize(maxSize),
    m_child(child),
    m_hidden(hidden),
    m_hideable(true),
	bCanDrag(true), //maks
    m_fixed(false),
    m_name(name)
{ }

wxIFMChildDataBase::wxIFMChildDataBase(const wxIFMChildDataBase &data)
    : m_type(data.m_type),
    m_desiredSize(data.m_desiredSize),
    m_minSize(data.m_minSize),
    m_maxSize(data.m_maxSize),
    m_child(data.m_child),
    m_hidden(data.m_hidden),
    m_hideable(data.m_hideable),
	bCanDrag(data.bCanDrag), //maks
    m_fixed(data.m_fixed),
    m_name(data.m_name)
{ }

wxIFMChildDataBase::~wxIFMChildDataBase()
{ }

/*
    wxIFMExtensionDataBase implementation
*/
wxIFMExtensionDataBase::~wxIFMExtensionDataBase()
{ }

wxIFMComponentDataKeyType wxIFMExtensionDataBase::DataKey()
{
    return IFM_COMPONENT_UNDEFINED;
}



//maks
wxIFMLayoutData::wxIFMLayoutData()
{
	bShowTitleBar = true;
	bMaximized = false;

	GetFrameInfo();
}

void wxIFMLayoutData::GetFrameInfo()
{
	//Get the wxFrame
	if( wxInterfaceManager::GetManager() && 
		wxInterfaceManager::GetManager()->GetParent() && 
		wxInterfaceManager::GetManager()->GetParent()->GetParent() )
	{
		wxFrame *mainFrame = (wxFrame *)wxInterfaceManager::GetManager()->GetParent()->GetParent();
		
		if(mainFrame)
		{
			bMaximized = mainFrame->IsMaximized();
			bShowTitleBar = mainFrame->GetWindowStyle() & (wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX);

			m_rect = wxIFMLayoutControl::getNonMaximizedFrameRect();
		}
	}
}

void wxIFMLayoutData::operator>>(wxDataOutputStream &stream)
{
	stream << bMaximized;
	stream << bShowTitleBar;

	stream << m_rect.x;
	stream << m_rect.y;
	stream << m_rect.width;
	stream << m_rect.height;

	//Save count
	stream << m_containers.size();

	for(unsigned int i = 0; i < m_containers.size(); ++i )
    {
		m_containers[i] >> stream;		
	}
}

void wxIFMLayoutData::operator<<(wxDataInputStream &stream)
{
	int _bMaximized, _bShowTitleBar;

	stream >> _bMaximized;
	stream >> _bShowTitleBar;

	stream >> m_rect.x;
	stream >> m_rect.y;
	stream >> m_rect.width;
	stream >> m_rect.height;

	bMaximized = _bMaximized?true:false;
	bShowTitleBar = _bShowTitleBar?true:false;

	int count;
	stream >> count;

	for(int i = 0; i < count; ++i )
    {
		wxIFMLayoutContainerData newContainer;
		newContainer << stream;
		
		m_containers.push_back(newContainer);
	}
}

void wxIFMLayoutContainerData::operator>>(wxDataOutputStream &stream)
{
	stream << m_orientation;

	stream << m_rect.x;
	stream << m_rect.y;
	stream << m_rect.width;
	stream << m_rect.height;

	//Save count
	stream << m_panels.size();
	
	for(unsigned int i = 0; i < m_panels.size(); ++i )
    {
		m_panels[i] >> stream;		
	}
}

void wxIFMLayoutContainerData::operator<<(wxDataInputStream &stream)
{
	stream >> m_orientation;

	stream >> m_rect.x;
	stream >> m_rect.y;
	stream >> m_rect.width;
	stream >> m_rect.height;
	
	int count;
	stream >> count;

	for(int i = 0; i < count; ++i )
    {
		wxIFMLayoutPanelData newPanel;
		newPanel << stream;
		
		m_panels.push_back(newPanel);
	}
}

void wxIFMLayoutPanelData::operator>>(wxDataOutputStream &stream)
{
	//Save count
	stream << m_tabs.size();

	for(unsigned int i = 0; i < m_tabs.size(); ++i )
    {
		m_tabs[i] >> stream;		
	}
}

void wxIFMLayoutPanelData::operator<<(wxDataInputStream &stream)
{
	int count;
	stream >> count;

	for(int i = 0; i < count; ++i )
    {
		wxIFMLayoutTabData newTab;
		newTab << stream;
		
		m_tabs.push_back(newTab);
	}
}

void wxIFMLayoutTabData::operator>>(wxDataOutputStream &stream)
{
	stream << bActive;
	stream << bCanDrag;

	stream << m_desiredSize.x;
	stream << m_desiredSize.y;
	
	stream << m_fixed;
	stream << m_hidden;
	stream << m_hideable;

	stream << m_name;
	stream << m_newRow;

	stream << m_pos.x;
	stream << m_pos.y;
}

void wxIFMLayoutTabData::operator<<(wxDataInputStream &stream)
{
	int _bActive, _bCanDrag, _m_fixed, _m_hidden, _m_hideable, _m_newRow;

	stream >> _bActive;
	stream >> _bCanDrag;

	stream >> m_desiredSize.x;
	stream >> m_desiredSize.y;
	
	stream >> _m_fixed;
	stream >> _m_hidden;
	stream >> _m_hideable;

	stream >> m_name;
	stream >> _m_newRow;

	stream >> m_pos.x;
	stream >> m_pos.y;

	bActive = _bActive?true:false;
	bCanDrag = _bCanDrag?true:false;
	m_fixed = _m_fixed?true:false;
	m_hidden = _m_hidden?true:false;
	m_hideable = _m_hideable?true:false;
	m_newRow = _m_newRow?true:false;
}

wxRect wxIFMLayoutControl::m_NonMaximizedFrameRect;
wxIFMLayoutControl::wxIFMLayoutControl() 
{
	fileName = wxT("layout.dat");
	defaultString = wxT("Default");
}

wxIFMLayoutControl::~wxIFMLayoutControl()
{
	
}

void wxIFMLayoutControl::Init(wxString _fileName)
{
	//Call this function after program initialization

	Load(_fileName);
	SaveDefault();
	LoadCurrent();
}

void wxIFMLayoutControl::Save(wxString name)
{
	if(name.IsEmpty())
	{
		name = fileName;
	}
	else
	{
		fileName = name;
	}

	wxFileOutputStream output(name);

	if(output.Ok())
	{
		wxDataOutputStream stream(output);
		
		//Save version
		stream << CURRENT_LAYOUT_VERSION;
		
		//Save current
		current.GetFrameInfo();
		current >> stream;

		//Save count
		stream << layouts.size();
		
		//Save layouts

		
		/*wxIFMLayoutMap::iterator it;
		for( it = layouts.begin(); it != layouts.end(); ++it ) //Crash here (infinite loop)
		{
			stream << it->first;
			it->second >> stream;
		}*/

		for(unsigned int i = 0; i < layoutList.size(); i++)
		{
			wxString name = layoutList[i];

			stream << name;
			layouts[name] >> stream;
		}
		
		//Let the main frame do extra save
		wxLayoutStateEvent event(LAYOUT_STATE_SAVE, &stream);

		event.SetEventObject(wxInterfaceManager::GetManager()->GetParent());
		wxInterfaceManager::GetManager()->GetParent()->ProcessEvent(event);		
	}
}

void wxIFMLayoutControl::Load(wxString name)
{
	if(name.IsEmpty())
	{
		name = fileName;
	}
	else
	{
		fileName = name;
	}
	
	if(wxFile::Exists(name))
	{
		wxFileInputStream input(name);
		
		if(input.Ok())
		{
			wxDataInputStream stream(input);
			layouts.clear();
			layoutList.clear();
			
			int version;
			stream >> version;
			
			if(version == CURRENT_LAYOUT_VERSION)
			{		
				current << stream;
				
				int count;
				stream >> count;
				
				for(int i = 0; i < count; i++)
				{
					wxString name;
					wxIFMLayoutData newLayout;
					
					stream >> name;
					newLayout << stream;
					
					AddLayout(newLayout, name);
				}
				
				//Let the main frame do extra load
				wxLayoutStateEvent event(LAYOUT_STATE_LOAD, &stream);
				
				event.SetEventObject(wxInterfaceManager::GetManager()->GetParent());
				wxInterfaceManager::GetManager()->GetParent()->ProcessEvent(event);
			}
		}
	}
}

void wxIFMLayoutControl::SaveLayoutContainer(wxIFMComponent *container, wxIFMLayoutContainerDataArray &save, wxIFMComponent **lastContainer)
{
	//Containers
	if(container->GetType() == IFM_COMPONENT_CONTAINER)
	{
		wxIFMLayoutContainerData data;
		wxIFMContainerData *contdata = IFM_GET_EXTENSION_DATA(container, wxIFMContainerData);
		wxIFMFloatingData *floating_data = IFM_GET_EXTENSION_DATA(container, wxIFMFloatingData);
		
		const wxIFMComponentArray &children = container->m_children;
		
		//Container data
		data.m_orientation = contdata->m_orientation;
		
		if(floating_data && floating_data->m_floating)
		{
			data.m_rect = floating_data->m_rect;
		}
		else
		{
			data.m_rect = container->m_rect;
		}
		
		//Panels
		for(unsigned int i = 0; i < children.size(); ++i )
		{
			wxIFMComponent *panel = children[i];
			
			if( panel->GetType() == IFM_COMPONENT_PANEL )
			{				
				wxIFMLayoutPanelData layoutPanel;
				wxIFMPanelData *panelData = IFM_GET_EXTENSION_DATA(panel, wxIFMPanelData);
				
				//layoutPanel.m_rect = panel->m_rect;
				//layoutPanel.m_hidden = panel->m_hidden;
				
				
				//Tabs			
				if(panelData)
				{
					//Get the tabs
					int visibleTabs = 0;
					for( unsigned int i1 = 0; i1 < panelData->m_tabs.size(); ++i1 )
					{
						wxIFMLayoutTabData layoutTab;
						wxIFMTab *tab = panelData->m_tabs[i1];
						
						layoutTab.m_desiredSize = tab->m_tab->m_desiredSize;						

						layoutTab.m_hidden = !tab->m_visible;
						layoutTab.m_hideable = tab->m_tab->m_canHide;
						layoutTab.m_fixed = tab->m_tab->m_fixed;
						layoutTab.m_pos = data.m_rect.GetPosition();
						layoutTab.bCanDrag = tab->m_tab->bCanDrag;
						layoutTab.m_name = tab->m_tab->m_name;
						layoutTab.bActive = tab->m_active;

						if( (data.m_orientation == IFM_ORIENTATION_BOTTOM ||
							data.m_orientation == IFM_ORIENTATION_TOP) && 
							//lastContainer[data.m_orientation] != container &&  //maks: don't put this to solve "Layout loads wrong when dock Actors in left border and Script in bottom border"
							i == 0)
						{
							layoutTab.m_newRow = true;
						}
						else
						{
							layoutTab.m_newRow = false;
						}

						if(!layoutTab.m_hidden)
						{
							visibleTabs++;
						}
						
						layoutPanel.AddTab(layoutTab);
					}
				}				
				
				if(layoutPanel.m_tabs.size())
				{
					data.AddPanel(layoutPanel);
				}
			}
		}
		
		if(data.m_panels.size())
		{
			save.push_back(data);
		}

		lastContainer[data.m_orientation] = container;
	}	
}

bool wxIFMLayoutControl::SaveLayout(wxIFMLayoutData &save) //maks
{
	unsigned int i;
	wxIFMComponent *lastContainer[10];
	save.m_containers.Clear();

	memset(lastContainer, 0, 10*sizeof(wxIFMComponent *));
    
	//Docked windows
	const wxIFMComponentList &m_containers = wxDynamicCast(wxInterfaceManager::GetManager()->GetActiveIP(), wxIFMDefaultInterfacePlugin)->GetTopContainerList();
	
	for(i = 0; i < m_containers.size(); ++i )
    {
		wxIFMComponent *container = m_containers[i];
		
		SaveLayoutContainer(container, save.m_containers, lastContainer);
	}

	//Float windows

	const wxIFMFloatingWindowArray &m_floating = wxInterfaceManager::GetManager()->GetActiveIP()->GetFloatingWindows();
	
	for(i = 0; i < m_floating.size(); ++i )
    {
		wxIFMComponent *container = m_floating[i]->GetComponent();
		
		SaveLayoutContainer(container, save.m_containers, lastContainer);
	}

	return true;
}


bool wxIFMLayoutControl::LoadLayout(wxIFMLayoutData &load) //maks
{
	if(load.m_containers.size() == 0)
	{
		return false;
	}

	//Clear layout
	wxInterfaceManager::GetManager()->GetActiveIP()->Clear();

	//Load main frame definitions
	wxFrame *mainFrame = (wxFrame *)wxInterfaceManager::GetManager()->GetParent()->GetParent();
	
	if(!load.bMaximized)
	{
		mainFrame->Maximize(false);	
		mainFrame->SetSize(load.m_rect);
	}
	else
	{		
		//mainFrame->SetSize(load.m_rect); //maks: if use SetSize here, reset layout don't works
		mainFrame->Maximize(true);
		wxIFMLayoutControl::setNonMaximizedFrameRect(load.m_rect);
		
		if(!load.bShowTitleBar)
		{
			long style = mainFrame->GetWindowStyle();
			style &= ~(wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX);
			mainFrame->SetWindowStyle(style);
		}
	}

	//Load containers
	for( unsigned int i = 0; i < load.m_containers.size(); ++i )
    {		
		wxIFMLayoutContainerData &layoutData = load.m_containers[i];
		
		//Panels
		for( unsigned int i1 = 0; i1 < layoutData.m_panels.size(); ++i1 )
		{
			wxIFMLayoutPanelData &layoutPanelData = layoutData.m_panels[i1];
			wxIFMTab *activeTab = NULL;

			for( unsigned int i2 = 0; i2 < layoutPanelData.m_tabs.size(); ++i2 )
			{
				wxIFMLayoutTabData &layoutTabData = layoutPanelData.m_tabs[i2];				
				
			
				wxIFMDefaultChildData childData;

				const wxIFMWindowsData *windowsData = wxInterfaceManager::GetManager()->GetWindow(layoutTabData.m_name);

				if(!windowsData)
				{
					return false;
				}

				childData.m_child = windowsData->window;
				childData.m_minSize = windowsData->m_minSize;
				childData.m_maxSize = windowsData->m_maxSize;

				childData.m_index = IFM_DEFAULT_INDEX;//layoutPanelData.m_tabs.size() - i2;				
				childData.m_tabify = i2 > 0; 
				childData.m_newRow = layoutTabData.m_newRow;
				childData.m_orientation = layoutData.m_orientation;			
				//childData.m_bitmap = ;
				childData.m_type = IFM_CHILD_GENERIC; //maks: get this information when use tool bars
				childData.m_desiredSize = layoutTabData.m_desiredSize;								
				childData.m_hidden = layoutTabData.m_hidden;
				childData.m_hideable = layoutTabData.m_hideable;
				childData.m_fixed = layoutTabData.m_fixed;
				childData.m_pos = layoutTabData.m_pos;
				childData.bCanDrag = layoutTabData.bCanDrag;
				childData.m_name = layoutTabData.m_name;

				bool bAddRowHeight = false;
				if(childData.m_orientation == IFM_ORIENTATION_FLOAT && layoutPanelData.m_tabs.size() > 1 /*&& i2 == 0*/)
				{
					//Add tabs height
					childData.m_desiredSize.y += wxIFMDefaultPanelPlugin::GetTabConfig().get_m_row_height();
					bAddRowHeight = true;
				}
				
				if(wxInterfaceManager::GetManager()->AddChild(&childData) && layoutTabData.bActive && !layoutTabData.m_hidden)
				{
					if( wxIFMTab::lastCreatedTab && 
						wxIFMTab::lastCreatedTab->m_tab && 
						wxIFMTab::lastCreatedTab->m_tab->m_parent &&
						wxIFMTab::lastCreatedTab->m_tab->m_name == childData.m_name )
					{
						activeTab = wxIFMTab::lastCreatedTab;
					}
				}

				if(bAddRowHeight &&	wxIFMTab::lastCreatedTab && wxIFMTab::lastCreatedTab->m_tab)
				{
					//Set again here to avoid size changes when save, resore, save restore

					childData.m_desiredSize.y -= wxIFMDefaultPanelPlugin::GetTabConfig().get_m_row_height();
					wxIFMTab::lastCreatedTab->m_tab->SetDesiredSize(childData.m_desiredSize);
				}
			}

			if(activeTab)
			{
				wxIFMSelectTabEvent event(activeTab->m_tab->m_parent, activeTab, false);
				wxInterfaceManager::GetManager()->GetActiveIP()->ProcessPluginEvent(event);
			}
		}		
	}

	wxInterfaceManager::GetManager()->UpdateConfiguration();

	//For unusable sash remotion
	wxInterfaceManager::GetManager()->UpdateConfiguration();

	return true;
}

void wxIFMLayoutControl::AddLayout(wxIFMLayoutData &layout, wxString name)
{
	layouts[name] = layout;
	
	if(!layoutList.Member(name))
	{
		layoutList.Add(name);
	}
}

void wxIFMLayoutControl::AddLayout(wxString name)
{
	wxIFMLayoutData layout;
	SaveLayout(layout);
	AddLayout(layout, name);
}

bool wxIFMLayoutControl::RemoveLayout(wxString name)
{
	if(layouts.find(name) != layouts.end())
	{
		layouts.erase(name);
		layoutList.Delete(name);

		return true;
	}

	return false;
}

wxIFMLayoutData *wxIFMLayoutControl::GetLayout(wxString name)
{
	if(layouts.find(name) != layouts.end())
	{
		return &(layouts[name]);
	}

	return NULL;
}

const wxStringList& wxIFMLayoutControl::GetLayoutList()
{
	return layoutList;
}


void wxIFMLayoutControl::LoadCurrent()
{
	if(!LoadLayout(current))
	{
		wxIFMLayoutData *layoutDefault = GetLayout(defaultString);

		if(layoutDefault)
		{
			LoadLayout(*layoutDefault);
		}
	}
}

void wxIFMLayoutControl::LoadDefault()
{
	wxIFMLayoutData *layoutDefault = GetLayout(defaultString);
	
	if(layoutDefault)
	{
		LoadLayout(*layoutDefault);
		Save();
	}
}

bool wxIFMLayoutControl::LoadLayout(wxString &name)
{
	wxIFMLayoutData *newLayout = GetLayout(name);

	if(newLayout)
	{
		return LoadLayout(*newLayout);
	}

	return false;
}

void wxIFMLayoutControl::SaveDefault()
{
	//Use this function to get the default configuration after program inicialization
	wxIFMLayoutData layoutDefault;
	layoutDefault.bShowTitleBar = true;
	layoutDefault.bMaximized = true;	
	SaveLayout(layoutDefault);

	AddLayout(layoutDefault, defaultString);
}

void wxIFMLayoutControl::SaveCurrent()
{
	current.GetFrameInfo();
	SaveLayout(current);
}

DEFINE_EVENT_TYPE(wxEVT_COMMAND_LAYOUT_STATE) //maks
IMPLEMENT_DYNAMIC_CLASS(wxLayoutStateEvent, wxCommandEvent)

wxLayoutStateEvent::wxLayoutStateEvent(int _type, void *_stream) : wxCommandEvent(wxEVT_COMMAND_LAYOUT_STATE, 0) //maks
{
	type = _type;
	stream = _stream;
}

wxLayoutStateEvent::wxLayoutStateEvent(const wxLayoutStateEvent& event) : wxCommandEvent(event) //maks
{
	type = event.type;
	stream = event.stream;
}

wxLayoutStateEvent::~wxLayoutStateEvent() //maks
{

}

/*
    wxIFMFloatingData implementation
*/
#if IFM_CANFLOAT
wxIFMFloatingData::wxIFMFloatingData()
    : wxIFMExtensionDataBase(),
    m_floating(false),
    m_window(NULL),
    m_rect(IFM_DEFAULT_RECT)
{ }

wxIFMComponentDataKeyType wxIFMFloatingData::GetDataKey() const
{
    return IFM_FLOATING_DATA_KEY;
}

wxIFMComponentDataKeyType wxIFMFloatingData::DataKey()
{
    return IFM_FLOATING_DATA_KEY;
}

#endif


