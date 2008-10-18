/*!
    Implementation for default interface stuff

    \file   definterface.cpp
    \author Robin McNeill
    \date   Created: 11/06/04

    Copyright (c) Robin McNeill
    Licensed under the terms of the wxWindows license
*/

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/ifm/definterface.h"
#include "wx/ifm/resize.h"
#include "wx/ifm/dragndrop.h"
#include "wx/panel.h"

#include "xpm/btn_hide.xpm"
#include "xpm/btn_min.xpm"
#include "xpm/btn_max.xpm"
#include "xpm/btn_hide_h.xpm"
#include "xpm/btn_min_h.xpm"
#include "xpm/btn_max_h.xpm"
#include "xpm/btn_hide_hp.xpm"
#include "xpm/btn_min_hp.xpm"
#include "xpm/btn_max_hp.xpm"

#include "wx/settings.h"

#include "wx/listimpl.cpp"


/////////////////////////////////////
//maks: Globalize colour storage
wxColour wxBorder_Pen(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW));
wxColour wxBorder_PenDark(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW));
wxColour wxBackground_Pen(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
wxColour wxResizer_Pen(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
wxColour wxResizerBorder_Pen(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));

wxColour wxTab_border_p(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW));
wxColour wxTab_active_bg_b(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
wxColour wxTab_active_bg_p(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
wxColour wxTab_inactive_bg_b(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
wxColour wxTab_inactive_bg_p(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
wxColour wxTab_bg_brush(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT));
wxColour wxTab_bg_pen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT));
/////////////////////////////////////

WX_DEFINE_LIST(wxIFMComponentList);

DEFINE_IFM_DATA_KEY(IFM_CONTAINER_DATA_KEY)
DEFINE_IFM_DATA_KEY(IFM_PANEL_DATA_KEY)
DEFINE_IFM_DATA_KEY(IFM_TABBED_PANEL_DATA_KEY)

DEFINE_IFM_COMPONENT_TYPE(IFM_COMPONENT_CONTAINER) //1
DEFINE_IFM_COMPONENT_TYPE(IFM_COMPONENT_PANEL_TAB) //2
DEFINE_IFM_COMPONENT_TYPE(IFM_COMPONENT_PANEL)     //3

DEFINE_EVENT_TYPE(wxEVT_IFM_ADDTOPCONTAINER);
DEFINE_EVENT_TYPE(wxEVT_IFM_REMOVETOPCONTAINER);
DEFINE_EVENT_TYPE(wxEVT_IFM_SELECTTAB);
DEFINE_EVENT_TYPE(wxEVT_IFM_COMPONENTBUTTONCLICK);

IMPLEMENT_DYNAMIC_CLASS(wxIFMDefaultInterfacePlugin, wxIFMInterfacePluginBase);
IMPLEMENT_DYNAMIC_CLASS(wxIFMDefaultContainerPlugin, wxIFMExtensionPluginBase);
IMPLEMENT_DYNAMIC_CLASS(wxIFMDefaultPanelPlugin, wxIFMExtensionPluginBase);
IMPLEMENT_DYNAMIC_CLASS(wxIFMDefaultChildData, wxIFMChildDataBase);
IMPLEMENT_DYNAMIC_CLASS(wxIFMContainerData, wxIFMExtensionDataBase);
IMPLEMENT_DYNAMIC_CLASS(wxIFMPanelTabData, wxIFMExtensionDataBase);
IMPLEMENT_DYNAMIC_CLASS(wxIFMPanelData, wxIFMExtensionDataBase);

// bitmaps for component buttons
wxBitmap m_bmpClose, m_bmpMin, m_bmpMax,
    m_bmpCloseH, m_bmpMinH, m_bmpMaxH,
    m_bmpCloseP, m_bmpMinP, m_bmpMaxP;

wxIFMTab *wxIFMTab::lastCreatedTab = NULL; //maks

#if IFM_CANFLOAT

/*
wxIFMFloatingFrame implementation
*/
BEGIN_EVENT_TABLE(wxIFMFloatingFrame, wxIFMFloatingWindowBase)
    EVT_CLOSE   (wxIFMFloatingFrame::OnClose)
END_EVENT_TABLE()

wxIFMFloatingFrame::wxIFMFloatingFrame(wxIFMInterfacePluginBase *ip, wxWindow *parent, wxWindowID id,
        const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
        : wxIFMFloatingWindowBase(ip)
{
    m_window = new wxFrame(parent, id, title, pos, size, style, name);
    ConnectEvents();
}

void wxIFMFloatingFrame::OnClose(wxCloseEvent &event)
{
    // don't know if this works, but at least try to close properly
    // if we can't veto the event
    if( event.CanVeto() )
    {
        // only hide the window
        event.Veto();
        m_window->Hide();

        // hide our root component too
        m_component->Show(false, false);
    }
    else
    {
        DisconnectEvents();
    }
}

#endif

/*
wxIFMDefaultInterfacePlugin implementation
*/
BEGIN_EVENT_TABLE(wxIFMDefaultInterfacePlugin, wxIFMInterfacePluginBase)
    EVT_IFM_DELETECOMPONENT (wxIFMDefaultInterfacePlugin::OnDeleteComponent)
    EVT_IFM_ADDCHILD    (wxIFMDefaultInterfacePlugin::OnAddChild)
    EVT_IFM_UPDATE      (wxIFMDefaultInterfacePlugin::OnUpdate)
    EVT_IFM_PAINTINTERFACE (wxIFMDefaultInterfacePlugin::OnPaint)
    EVT_IFM_GETCONTENTRECT(wxIFMDefaultInterfacePlugin::OnGetContentRect)

    EVT_IFM_ADDTOPCONTAINER (wxIFMDefaultInterfacePlugin::OnAddTopContainer)
    EVT_IFM_REMOVETOPCONTAINER (wxIFMDefaultInterfacePlugin::OnRemoveTopContainer)

#if IFM_CANFLOAT
    EVT_IFM_CREATEFLOATINGWINDOW    (wxIFMDefaultInterfacePlugin::OnCreateFloatingWindow)
    EVT_IFM_DESTROYFLOATINGWINDOW   (wxIFMDefaultInterfacePlugin::OnDestroyFloatingWindow)
    //EVT_IFM_FLOAT                   (wxIFMDefaultInterfacePlugin::OnFloat)
#endif
END_EVENT_TABLE()

wxIFMDefaultInterfacePlugin::wxIFMDefaultInterfacePlugin()
    : wxIFMInterfacePluginBase()
{ }

wxIFMDefaultInterfacePlugin::~wxIFMDefaultInterfacePlugin()
{
#if IFM_CANFLOAT
    // delete floating window objects
    for( int i = 0, count = m_floatingWindows.GetCount(); i < count; ++i )
    //for( wxIFMFloatingWindowArray::const_iterator i = m_floatingWindows.begin(), end = m_floatingWindows.end(); i != end; ++i )
        delete m_floatingWindows[i];
        //delete (*i);
#endif
}

void wxIFMDefaultInterfacePlugin::Clear() //maks
{
	int i, count;

	// delete containers
    /*for(i = 0, count = m_containers.GetCount(); i < count; ++i)
	{
		delete m_containers[i]; //delete when destroy components in wxIFMInterfacePluginBase
	}*/

	m_containers.clear();

	wxIFMInterfacePluginBase::Clear();
}

const wxIFMComponentList &wxIFMDefaultInterfacePlugin::GetTopContainerList() const
{
    return m_containers;
}

bool wxIFMDefaultInterfacePlugin::ProcessPluginEvent(wxIFMPluginEvent &event)
{
	if(!wxInterfaceManager::GetManager()->getProcessEvents()) return false; //maks

    bool ret = wxIFMInterfacePluginBase::ProcessPluginEvent(event);

#if IFM_CANFLOAT
    if( event.GetEventType() == wxEVT_IFM_CREATEFLOATINGWINDOW )
    {
        wxIFMCreateFloatingWindowEvent *evt = wxDynamicCast(&event, wxIFMCreateFloatingWindowEvent);
        if( evt )
            m_floatingWindows.push_back(evt->GetWindow());
    }
#endif

    return ret;
}

void wxIFMDefaultInterfacePlugin::OnAddTopContainer(wxIFMAddTopContainerEvent &event)
{
    wxIFMComponent *component = event.GetComponent();

    wxASSERT_MSG(component, wxT("NULL component?"));
    if( !component )
        return;

    if( event.GetIndexMode() )
    {
        int index = event.GetIndex();

        if( index == -1 )
        {
            m_containers.push_back(component);
        }
        else
        {
            // get an interator to the index position
            if( index == 0 )
                m_containers.push_front(component);
            else
            {
                wxIFMComponentList::iterator i; int n;
                for( i = m_containers.begin(), n = 0; n < index; ++i, n++ );
                m_containers.insert(i, component);
            }
        }
    }
    else
    {
        // find the index of the index container
        wxIFMComponent *index = event.GetIndexContainer();
        wxIFMComponentList::iterator i, end;
        for( i = m_containers.begin(), end = m_containers.end(); i != end; ++i)
        {
            if( *i == index )
                break;
        }

        // dock before or after this index
        if( !event.GetIndex() ) // GetIndex() == 0 means dock after
            i++;
        // dont advance to the end of the list
        if( i == end )
            m_containers.push_back(component);
        else
            m_containers.insert(i, component);
    }

#if IFM_CANFLOAT
    // if a container is moved from a floating window into a top level position, it needs
    // to recieve float notify events, or child windows fall off the face of the earth
    wxIFMFloatNotifyEvent evt(component, false);
    ProcessPluginEvent(evt);
#endif
}

void wxIFMDefaultInterfacePlugin::OnRemoveTopContainer(wxIFMRemoveTopContainerEvent &event)
{
    wxIFMComponent *container = event.GetComponent();

    wxASSERT_MSG(container, wxT("NULL component?"));
    if( !container )
        return;

    for( wxIFMComponentList::iterator i = m_containers.begin(), end = m_containers.end(); i != end; ++i )
    {
        if( *i == container )
        {
            m_containers.erase(i);

            // give the container no orientation value
            wxIFMContainerData *data = IFM_GET_EXTENSION_DATA(container, wxIFMContainerData);
            wxASSERT_MSG(data, wxT("Container with no container data?"));
            if( data )
                data->m_orientation = IFM_ORIENTATION_NONE;

            return;
        }
    }

    wxFAIL_MSG(wxT("Trying to remove a container not in top level list!"));
}

bool wxIFMDefaultInterfacePlugin::Initialize(wxInterfaceManager *m_manager)
{
    // base initialization
    wxIFMInterfacePluginBase::Initialize(m_manager);

    // push required plugins
    wxIFMExtensionPluginBase *plugin;

    plugin = new wxIFMDefaultContainerPlugin();
    if( !PushExtensionPlugin(plugin) )
        return false;

    plugin = new wxIFMDefaultPanelPlugin();
    if( !PushExtensionPlugin(plugin) )
        return false;

    plugin = new wxIFMDefaultResizePlugin();
    if( !PushExtensionPlugin(plugin) )
        return false;

    plugin = new wxIFMDefaultDockingPlugin();
    if( !PushExtensionPlugin(plugin) )
        return false;

    return true;
}

wxIFMComponent *wxIFMDefaultInterfacePlugin::GetContainer(int orientation)
{
    // look for the last container with the specified orientation
    // FIXME: This actually finds the first container?
    //for( wxIFMComponentList::compatibility_iterator node = m_containers.GetFirst(); node; node = node->GetNext() )
    for( wxIFMComponentList::const_iterator i = m_containers.begin(), end = m_containers.end(); i != end; ++i )
    {
        //wxIFMComponent *container = node->GetData();
        wxIFMComponent *container = (*i);
        wxIFMContainerData *data = IFM_GET_EXTENSION_DATA(container, wxIFMContainerData);
        if( data->m_orientation == orientation )
            return container;
    }

    // no container found
    return NULL;
}

void wxIFMDefaultInterfacePlugin::OnDeleteComponent(wxIFMDeleteComponentEvent &event)
{
    if( event.GetComponentType() == IFM_COMPONENT_CONTAINER )
    {
        wxIFMComponent *container = event.GetComponent();

        // remove the container from the top level container list
        for( wxIFMComponentList::iterator i = m_containers.begin(), end = m_containers.end(); i != end; ++i )
        {
            if( *i == container )
            {
                m_containers.erase(i);
                break;
            }
        }
    }

    event.Skip();
}

void wxIFMDefaultInterfacePlugin::OnAddChild(wxIFMAddChildEvent &event)
{
    static int s_nextOrientation = IFM_ORIENTATION_TOP; // for default orientation selection
    static wxIFMComponent *last_panel;
    static bool last_panel_floating;
    bool delete_data = false;

    wxIFMChildDataBase *base = event.GetChildData();

    // verify that the data is of the right type
    wxIFMDefaultChildData *data = wxDynamicCast(base, wxIFMDefaultChildData);

    // construct a defaultchilddata object from the regular object if we must
    if( !data )
    {
        delete_data = true;
        data = new wxIFMDefaultChildData(*base);
    }

    //! \todo Handle tabify. We don't make panels for children being tabbed
    bool tabify = data->m_tabify;

    // create a tab for the child
    wxIFMNewComponentEvent newevt(IFM_COMPONENT_PANEL_TAB);
    ProcessPluginEvent(newevt);

    wxIFMComponent *tab = newevt.GetComponent();

    tab->m_canHide = data->m_hideable;
    tab->m_fixed = data->m_fixed;
	tab->bCanDrag = data->bCanDrag; //maks

    if( !tab->m_fixed )
    {
        tab->m_minSize = data->m_minSize;
        tab->m_maxSize = data->m_maxSize;
        tab->m_desiredSize = data->m_desiredSize;
    }
    else
        tab->m_minSize = (tab->m_maxSize = (tab->m_desiredSize = data->m_desiredSize));

#if IFM_CANFLOAT
    // set floating data for the tab and the tabbed panel
    wxIFMFloatingData *floating_data = IFM_GET_EXTENSION_DATA(tab, wxIFMFloatingData);
    floating_data->m_rect.SetSize(tab->m_desiredSize);
    floating_data->m_rect.SetPosition(data->m_pos);
#endif

    tab->m_name = data->m_name;

    wxIFMPanelTabData *tab_data = IFM_GET_EXTENSION_DATA(tab, wxIFMPanelTabData);
    if(data->m_bitmap.Ok())
        tab_data->m_bitmap = data->m_bitmap.GetSubBitmap(wxRect(0, 0, data->m_bitmap.GetWidth(), data->m_bitmap.GetHeight()));

    // set the child of the tab
    wxIFMSetChildEvent setevt(tab, data->m_type, data->m_child);
    ProcessPluginEvent(setevt);

    // store the child window / component pair
    AddChildWindow(data->m_child, tab);

    int orientation = data->m_orientation;

    // are we floating?
#if IFM_CANFLOAT
    // if we are tabifying, dock as a tab of the previous panel

    // only make a new floating window if:
    // we are floating AND
    // we are not making a tab of the previous window OR
    // we are making a tab of the previous window and there was no previous window OR
    // the last panel that we would become a tab of is not floating
    if( orientation == IFM_ORIENTATION_FLOAT && (!tabify || (tabify && !last_panel) || !last_panel_floating) )
    {
        // send the tab a float event

        wxIFMFloatEvent floatevt(tab, floating_data->m_rect, false); //maks
        ProcessPluginEvent(floatevt);
        last_panel_floating = true;
    }

    // only dock as a tab of the previously created panel if:
    // we are making a tab AND
    // the last panel exists AND
    // the orientation of the new child is relatively similar to the orientation of the previous panel
    else if( tabify && last_panel && (orientation == IFM_ORIENTATION_FLOAT || !last_panel_floating) )
#else

    // dock as a tab of the previous panel if:
    // we are making a tab AND
    // the last panel exists
    if( tabify && last_panel )
#endif
    {
        // dock the child as a tab of the last panel
        wxIFMDockEvent evt(tab, last_panel, IFM_DEFAULT_INDEX);
        ProcessPluginEvent(evt);
    }
    else
    {
        last_panel_floating = false;

        // see if we need to chose an orientation
        if( orientation == IFM_ORIENTATION_DEFAULT )
        {
            orientation = s_nextOrientation;

            // calculate next orientation
            switch(s_nextOrientation)
            {
                case IFM_ORIENTATION_TOP:
                    s_nextOrientation = IFM_ORIENTATION_LEFT;
                    break;
                case IFM_ORIENTATION_LEFT:
                    s_nextOrientation = IFM_ORIENTATION_BOTTOM;
                    break;
                case IFM_ORIENTATION_BOTTOM:
                    s_nextOrientation = IFM_ORIENTATION_RIGHT;
                    break;
                case IFM_ORIENTATION_RIGHT:
                    s_nextOrientation = IFM_ORIENTATION_TOP;
                    break;
            }
        }

        // retrieve destinition container
        wxIFMComponent *container = GetContainer(orientation);

        // make a container if need be
        if( !container || data->m_newRow )
        {
            wxIFMNewComponentEvent newevt(IFM_COMPONENT_CONTAINER);
            ProcessPluginEvent(newevt);

            container = newevt.GetComponent();

            wxASSERT_MSG(container, wxT("NULL component returned by new component event?"));
            if( !container )
                return;

            wxIFMContainerData *data = IFM_GET_EXTENSION_DATA(container, wxIFMContainerData);
            wxASSERT_MSG(data, wxT("Container without container data?"));
            if( !data )
                return;

            data->m_orientation = orientation;
            switch(orientation)
            {
                case IFM_ORIENTATION_TOP:
                case IFM_ORIENTATION_BOTTOM:
                    container->m_alignment = IFM_ALIGN_HORIZONTAL;
                    break;
                case IFM_ORIENTATION_LEFT:
                case IFM_ORIENTATION_RIGHT:
                    container->m_alignment = IFM_ALIGN_VERTICAL;
                    break;
            }

            // add container as a top level one using the event
            wxIFMAddTopContainerEvent addevt(container, -1);
            ProcessPluginEvent(addevt);
        }

        // dock this tabbed panel into the specified container
        wxIFMDockEvent dockevt(tab, container, data->m_index);
        ProcessPluginEvent(dockevt);
    }

    // the tabs parent will be a panel automagically, so we store that
    // as the previously used panel for tabify
    last_panel = tab->m_parent;

    // show or hide the new component
    tab->Show(!data->m_hidden, false);

    // complete successfully
    event.SetSuccess();

    if( delete_data )
        delete data;
}

void wxIFMDefaultInterfacePlugin::OnUpdate(wxIFMUpdateEvent &event)
{
    wxSize size = event.GetSize();
    wxPoint pos = event.GetPosition();
    wxRect m_contentRect = event.GetRect();

    wxIFMComponent *container;
    wxIFMContainerData *data;
    wxWindow *content = event.GetContent();
    wxRect rect;

    int min_w = 0, min_h = 0;

    // update containers beginning from the start of the list
    // also calculate the minimum allowable size for the interface
    for( wxIFMComponentList::iterator i = m_containers.begin(), end = m_containers.end(); i != end; ++i )
    {
        container = *i;

        if( !container->IsVisible() )
            continue;

        data = IFM_GET_EXTENSION_DATA(container, wxIFMContainerData);

        // calculate size
        wxSize size = container->GetDesiredSize();
        wxSize min_size = container->GetMinSize();

        wxIFMComponentList::iterator temp = i;
        ++temp;
        if( temp == end && !content )
        {
            // make this container as big as the remaining area to deal with having no content window
            rect = m_contentRect;
        }
        else
        {
            switch(data->m_orientation)
            {
                case IFM_ORIENTATION_LEFT:
                    rect.x = m_contentRect.x;
                    rect.y = m_contentRect.y;
                    rect.width = size.GetWidth();
                    rect.height = m_contentRect.height;

                    m_contentRect.width -= rect.width;
                    m_contentRect.x += rect.width;

                    //if( min_h < min_size.y )
                        min_h += min_size.y;
                        min_w += min_size.x;

                    break;

                case IFM_ORIENTATION_RIGHT:
                    rect.width = size.GetWidth();
                    rect.x = m_contentRect.x + m_contentRect.width - rect.width;
                    rect.y = m_contentRect.y;
                    rect.height = m_contentRect.height;

                    m_contentRect.width -= rect.width;

                    //if( min_h < min_size.y )
                        min_h += min_size.y;
                        min_w += min_size.x;

                    break;

                case IFM_ORIENTATION_TOP:
                    rect.x = m_contentRect.x;
                    rect.y = m_contentRect.y;
                    rect.width = m_contentRect.width;
                    rect.height = size.GetHeight();

                    m_contentRect.y += rect.height;
                    m_contentRect.height -= rect.height;

                    //if( min_w < min_size.x )
                        min_h += min_size.y;
                        min_w += min_size.x;

                    break;

                case IFM_ORIENTATION_BOTTOM:
                    rect.width = m_contentRect.width;
                    rect.height = size.GetHeight();
                    rect.x = m_contentRect.x;
                    rect.y = m_contentRect.y + m_contentRect.height - rect.height;

                    m_contentRect.height -= rect.height;

                    //if( min_w < min_size.x )
                        min_h += min_size.y;
                        min_w += min_size.x;

                    break;

                default:
                    wxFAIL_MSG(wxT("Invalid orientation value"));
            }
        }

        // this code is commented out because its logic is flawed
        // I need to not update a container so long as none of its children
        // require an update
        //! \todo Add a flag to default data that specifies if a change has been made since the last update
        /*
        // only update the container if its new rect is different from its old rect
        wxIFMRectEvent rectevt(wxEVT_IFM_GETRECT, IFM_COMPONENT_CONTAINER, container);
        ProcessPluginEvent(rectevt);

        if( rect != rectevt.GetRect() )
        */
        {
            // update the container
            wxIFMUpdateComponentEvent updevt(container, rect);
            ProcessPluginEvent(updevt);
        }
    }

    // position content window
    if( content )
        content->SetSize(m_contentRect);

    // set size hints
    //GetManager()->GetParent()->ClientToScreen(&min_w, &min_h);

	//maks: Wrong min_w and min_h. Set hardcoded limits in frame creation
	/*if(GetManager()->GetParent() && GetManager()->GetParent()->GetParent()) //maks
	{
		//Panel used
		GetManager()->GetParent()->GetParent()->SetSizeHints(min_w, min_h);
	}
	else*/
	/*{
		//Frame used
		//GetManager()->GetParent()->SetSizeHints(min_w, min_h);		
	}*/

    // update floating windows if needed
#if IFM_CANFLOAT
    if( event.GetFoating() )
    {
        wxIFMFloatingWindowBase *base;

        for( size_t i = 0; i < m_floatingWindows.GetCount(); i++ )
        //for( wxIFMFloatingWindowArray::const_iterator i = m_floatingWindows.begin(), end = m_floatingWindows.end(); i != end; ++i )
        {
            base = m_floatingWindows[i];
            // base = *i;

            base->Update(true);
        }
    }
#endif
}

//#include <crtdbg.h>

void wxIFMDefaultInterfacePlugin::OnPaint(wxIFMPaintInterfaceEvent &event)
{
#if 0
    LARGE_INTEGER cfreq, cstart, cend;

    ::QueryPerformanceFrequency(&cfreq);
    ::QueryPerformanceCounter(&cstart);
#endif

    wxDC &dc = event.GetDC();

    wxIFMComponent *component;
    wxRegionContain result;

    // don't paint a component if we don't have to
    const wxRegion &region = event.GetUpdateRegion();
    for( wxIFMComponentList::const_iterator i = m_containers.begin(), end = m_containers.end(); i != end; ++i )
    {
        component = *i;

        if( !component->IsVisible() )
            continue;
        else
        {
            result = region.Contains(component->m_rect);
            if( result == wxInRegion || result == wxPartRegion )
            {
                // make a copy of the region and find the parts of the component
                // that need updating
                //wxRegion new_region = region;
                //new_region.Intersect(component->m_rect);
                component->Paint(dc, region);
            }
        }
    }

#if 0
    ::QueryPerformanceCounter(&cend);
    _CrtDbgReport(_CRT_WARN, 0, 0, 0, "Drawing time: %2.12f\n", double(cend.QuadPart - cstart.QuadPart) / cfreq.QuadPart);
#endif
}

wxIFMComponent *wxIFMDefaultInterfacePlugin::GetComponentByPos(const wxPoint &pos, wxIFMComponent *component, bool floating)
{
#if IFM_CANFLOAT
    if( floating )
    {
        wxIFMComponent *component = GetFloatingComponentByPos(GetManager()->GetParent()->ClientToScreen(pos));
        if( component )
            return component;
    }
#endif

    if( component == NULL )
    {
        wxIFMComponent *ret;
        for( wxIFMComponentList::const_iterator i = m_containers.begin(), end = m_containers.end(); i != end; ++i )
        {
            ret = GetComponentByPos(pos, *i);
            if( ret )
                return ret;
        }
        return NULL;
    }

    //wxIFMHitTestEvent evt(component, IFM_COORDS_ABSOLUTE, pos);
    //ProcessPluginEvent(evt);
    if( !component->m_hidden && component->m_rect.Inside(pos) )

    //if( evt.GetPassed() )
    {
        // check children
        wxIFMComponent *ret;
        const wxIFMComponentArray &children = component->m_children;
        for( size_t i = 0; i < children.GetCount(); i++ )
        //for( wxIFMComponentArray::const_iterator i = children.begin(), end = children.end(); i != end; ++i )
        {
            ret = GetComponentByPos(pos, children[i]);
            if( ret )
                return ret;
        }
        return component;
    }

    return NULL;
}

void wxIFMDefaultInterfacePlugin::OnGetContentRect(wxIFMRectEvent &event)
{
    wxRect m_contentRect = GetManager()->GetInterfaceRect();
    wxIFMComponent *container;
    wxIFMContainerData *data;

    // update containers beginning from the start of the list
    for( wxIFMComponentList::const_iterator i = m_containers.begin(), end = m_containers.end(); i != end; ++i )
    {
        container = *i;
        data = IFM_GET_EXTENSION_DATA(container, wxIFMContainerData);

        wxIFMRectEvent rectevt(wxEVT_IFM_GETRECT, container);
        ProcessPluginEvent(rectevt);
        const wxRect &rect = rectevt.GetRect();

        switch(data->m_orientation)
        {
            // skip docked containers
            case IFM_ORIENTATION_DOCKED:
                continue;

            case IFM_ORIENTATION_LEFT:
                m_contentRect.width -= rect.width;
                m_contentRect.x += rect.width;
                break;

            case IFM_ORIENTATION_RIGHT:
                m_contentRect.width -= rect.width;
                break;

            case IFM_ORIENTATION_TOP:
                m_contentRect.y += rect.height;
                m_contentRect.height -= rect.height;
                break;

            case IFM_ORIENTATION_BOTTOM:
                m_contentRect.height -= rect.height;
                break;

            default:
                wxFAIL_MSG(wxT("Invalid orientation value"));
        }
    }

    // return content rect
    event.SetRect(m_contentRect);
}

#if IFM_CANFLOAT
void wxIFMDefaultInterfacePlugin::OnCreateFloatingWindow(wxIFMCreateFloatingWindowEvent &event)
{
    // component being floated
    wxIFMComponent *component = event.GetComponent();

    wxASSERT_MSG(component, wxT("Creating a floating window for a NULL component?"));
    if( !component )
        return;

    // create a frame to float with
    long style = IFM_FLOATING_FRAME_STYLE;

    wxIFMFloatingWindowBase *window = new wxIFMFloatingFrame(this, event.GetParent(), wxID_ANY, wxT(""),
        wxPoint(0,0), wxSize(0,0), style);
    event.SetWindow(window);

    // create the root component as well
    if( event.GetComponent()->GetType() == IFM_COMPONENT_CONTAINER )
        window->m_component = event.GetComponent();
    else
    {
        wxIFMNewComponentEvent evt(IFM_COMPONENT_FLOATING_ROOT);
        ProcessPluginEvent(evt);
        window->m_component = evt.GetComponent();

        wxASSERT_MSG(window->m_component, wxT("NULL floating root returned?"));
        if( !window->m_component )
            return;
    }

    // tell the root component its floating
    wxIFMFloatNotifyEvent fltevt(window->GetComponent(), true, window);
    ProcessPluginEvent(fltevt);

    // horizontal child alignment by default
    if( window->GetComponent()->m_alignment == IFM_ALIGN_NONE )
        window->GetComponent()->m_alignment = IFM_ALIGN_HORIZONTAL;

    wxIFMContainerData *contdata = IFM_GET_EXTENSION_DATA(window->m_component, wxIFMContainerData);
    if( contdata )
        contdata->m_orientation = IFM_ORIENTATION_FLOAT;

    // dock the component into the root component if needed
    if( component->GetType() != IFM_COMPONENT_CONTAINER )
    {
        wxIFMDockEvent dockevt(component, window->GetComponent(), 0);
        ProcessPluginEvent(dockevt);
    }
}

void wxIFMDefaultInterfacePlugin::OnDestroyFloatingWindow(wxIFMDestroyFloatingWindowEvent &event)
{
    wxIFMFloatingWindowBase *base = event.GetWindow();

    wxASSERT_MSG(base, wxT("Destroying a NULL floating window?"));
    if( !base )
        return;

    // remove the floating window from the floating window array
    //for( int i = 0, count = m_floatingWindows.GetCount(); i < count; ++i )
    for( wxIFMFloatingWindowArray::iterator i = m_floatingWindows.begin(), end = m_floatingWindows.end(); i != end; ++i )
    {
        //if( m_floatingWindows[i] == base )
        if( *i == base )
        {
            //m_floatingWindows.RemoveAt(i);
            m_floatingWindows.erase(i);
            break;
        }
    }

    base->m_destroyRoot = event.ShouldDestroyRoot();

    delete base;
}

#endif

/*
wxIFMDefaultContainerPlugin implementation
*/
BEGIN_EVENT_TABLE(wxIFMDefaultContainerPlugin, wxIFMExtensionPluginBase)
    EVT_IFM_NEWCOMPONENT    (wxIFMDefaultContainerPlugin::OnCreateComponent)
    EVT_IFM_DOCK            (wxIFMDefaultContainerPlugin::OnDock) // docks panel rows into the container
    EVT_IFM_UNDOCK          (wxIFMDefaultContainerPlugin::OnUndock)
    EVT_IFM_UPDATECOMPONENT (wxIFMDefaultContainerPlugin::OnUpdate)
    EVT_IFM_VISIBILITYCHANGED (wxIFMDefaultContainerPlugin::OnVisibilityChange)
    EVT_IFM_GETDESIREDSIZE  (wxIFMDefaultContainerPlugin::OnGetDesiredSize)
    EVT_IFM_SETDESIREDSIZE  (wxIFMDefaultContainerPlugin::OnSetDesiredSize)
    EVT_IFM_LEFTDOWN        (wxIFMDefaultContainerPlugin::OnLeftDown)
    EVT_IFM_PAINTDECOR      (wxIFMDefaultContainerPlugin::OnPaintDecor)

    EVT_IFM_COMPONENTBUTTONCLICK (wxIFMDefaultContainerPlugin::OnComponentButtonClick)

#if IFM_CANFLOAT
    EVT_IFM_FLOATING_NOTIFY    (wxIFMDefaultContainerPlugin::OnFloatNotify)
#endif
END_EVENT_TABLE()

wxIFMDefaultContainerPlugin::wxIFMDefaultContainerPlugin()
    : wxIFMExtensionPluginBase()
{ }

bool wxIFMDefaultContainerPlugin::HasVisibleChildren(wxIFMComponent *component)
{
    wxIFMComponentArray &children = component->m_children;
    wxIFMComponent *child;

    for( int i = 0, size = children.GetCount(); i < size; ++i )
    //for( wxIFMComponentArray::const_iterator i = children.begin(), end = children.end(); i != end; ++i )
    {
        child = children[i];
        //child = *i;

        // child containers alone don't count, but their children do
        if( child->GetType() == IFM_COMPONENT_CONTAINER )
        {
            if( HasVisibleChildren(child) )
                return true;
        }
        else
        {
            if( child->IsVisible() )
                return true;
        }
    }

    return false;
}

int wxIFMDefaultContainerPlugin::GetVisibleChildrenCount(wxIFMComponent *component)
{
    wxIFMComponentArray &children = component->m_children;
    wxIFMComponent *child;
    int child_count = 0;

    for( int i = 0, size = children.GetCount(); i < size; ++i )
    //for( wxIFMComponentArray::const_iterator i = children.begin(), end = children.end(); i != end; ++i )
    {
        child = children[i];
        //child = *i;

        // child containers alone don't count, but their children do
        if( child->GetType() == IFM_COMPONENT_CONTAINER )
        {
            child_count += GetVisibleChildrenCount(child);
        }
        else
        {
            if( child->IsVisible() )
                child_count++;
        }
    }

    return child_count;
}

bool wxIFMDefaultContainerPlugin::HasNonCloseableChildren(wxIFMComponent *component)
{
    wxIFMComponentArray &children = component->m_children;
    wxIFMComponent *child;

    for( size_t i = 0; i < children.GetCount(); i++ )
    //for( wxIFMComponentArray::const_iterator i = children.begin(), end = children.end(); i != end; ++i )
    {
        child = children[i];
        //child = *i;
        if( !child->m_canHide )
            return true;
        else
            if( HasNonCloseableChildren(child) )
                return true;
    }

    return false;
}

void wxIFMDefaultContainerPlugin::OnCreateComponent(wxIFMNewComponentEvent &event)
{
    // only create containers or root floating components
    if( event.GetComponentType() == IFM_COMPONENT_CONTAINER ||
        event.GetComponentType() == IFM_COMPONENT_FLOATING_ROOT )
    {
        wxIFMComponent *container = new wxIFMComponent(GetIP(), IFM_COMPONENT_CONTAINER);

        // container specific storage
        wxIFMContainerData *data = new wxIFMContainerData(GetIP(), container);

        // add the drag gripper to the left side of all containers, and a close button
        data->m_tray_rect.width = IFM_CONTAINER_GRIPPER_WIDTH;
        container->m_margins.Set(0);
        container->m_margins.left = data->m_tray_rect.width;
        container->m_borders.Set(0);

#if 0 // no button yet
        wxIFMComponentButton *btn;
        btn = data->m_buttonManager.AddButton(IFM_COMPONENT_ID_CLOSE);
        btn->m_bmp = &m_bmpClose;
        btn->m_bmpH = &m_bmpCloseH;
        btn->m_bmpP = &m_bmpCloseP;
#endif

        container->AddExtensionData(data);

        // return the new component
        event.SetComponent(container);
    }
    else
        event.Skip();
}

// paint the drag gripper on the side of all containers
void wxIFMDefaultContainerPlugin::OnPaintDecor(wxIFMPaintEvent &event)
{
#if IFM_CONTAINER_GRIPPER_WIDTH
    if( event.GetComponentType() == IFM_COMPONENT_CONTAINER )
    {
        wxIFMComponent *component = event.GetComponent();

        wxASSERT_MSG(component, wxT("NULL component?"));
        if( !component )
            return;

        wxIFMContainerData *data = IFM_GET_EXTENSION_DATA(component, wxIFMContainerData);

        wxASSERT_MSG(data, wxT("Container with no container data?"));
        if( !data )
            return;

        wxDC &dc = event.GetDC();

        //! \todo Make this not hard coded color and style
        wxBrush brush(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW));

        dc.SetBrush(brush);
        dc.SetPen(*wxBLACK_PEN);

        dc.DrawRectangle(data->m_tray_rect);
    }
    else
#endif
        event.Skip();
}

void wxIFMDefaultContainerPlugin::OnSetDesiredSize(wxIFMRectEvent &event)
{
    if( event.GetComponentType() != IFM_COMPONENT_CONTAINER )
    {
        event.Skip();
        return;
    }

    GetNextHandler()->ProcessEvent(event);

    // set the desired size of our children
    wxIFMComponent *component = event.GetComponent();

    wxASSERT_MSG(component, wxT("NULL component?"));
    if( !component )
        return;

    const wxIFMComponentArray &children = component->m_children;
    wxRect rect, combined_rect;
    int numchildren = children.GetCount(), i;
    wxRect *child_rects = new wxRect[numchildren];

    // the desired size is specified in absolute cords, only use client coords for children
    wxIFMConvertRectEvent cvtevt(component, IFM_COORDS_ABSOLUTE, IFM_COORDS_CLIENT, event.GetRect());
    GetIP()->ProcessPluginEvent(cvtevt);
    rect = cvtevt.GetRect();

    // sum the total desired widths
    for( i = 0; i < numchildren; ++i )
    {
        wxIFMRectEvent evt(wxEVT_IFM_GETDESIREDSIZE, children[i]);
        GetIP()->ProcessPluginEvent(evt);

        child_rects[i] = evt.GetRect();
        combined_rect.height += child_rects[i].height;
        combined_rect.width += child_rects[i].width;
    }

    if( component->m_alignment == IFM_ALIGN_HORIZONTAL )
        combined_rect.height = rect.height;
    else if( component->m_alignment == IFM_ALIGN_VERTICAL )
        combined_rect.width = rect.width;

    for( i = 0; i < numchildren; ++i )
    {
        // scale desired size
        if( component->m_alignment == IFM_ALIGN_HORIZONTAL )
        {
            child_rects[i].height = combined_rect.height;
            child_rects[i].width = (int)(rect.width * ((double)child_rects[i].width / combined_rect.width));
        }
        else if( component->m_alignment == IFM_ALIGN_VERTICAL )
        {
            child_rects[i].width = combined_rect.width;
            child_rects[i].height = (int)(rect.height * ((double)child_rects[i].height / combined_rect.height));
        }

        wxIFMRectEvent evt(wxEVT_IFM_SETDESIREDSIZE, children[i], child_rects[i]);
        GetIP()->ProcessPluginEvent(evt);
    }

    delete[] child_rects;
}

void wxIFMDefaultContainerPlugin::OnGetDesiredSize(wxIFMRectEvent &event)
{
    //! \todo I can cache this result and only recalculate it if any part of the component was updated

    // only process containers
    if( event.GetComponentType() != IFM_COMPONENT_CONTAINER )
    {
        event.Skip();
        return;
    }

    // return the combined calculated size of all child components
    wxSize size;
    int width = 0, height = 0;
    wxIFMComponent *container = event.GetComponent();

    wxASSERT_MSG(container, wxT("NULL component?"));
    if( !container )
        return;

    // return 0 if hidden
    if( container->m_hidden  || !HasVisibleChildren(container) )
    {
        event.SetSize(wxSize(0,0));
        return;
    }

    for( int i = 0, count = container->m_children.GetCount(); i < count; ++i )
    //for( wxIFMComponentArray::const_iterator i = container->m_children.begin(), end = container->m_children.end(); i != end; ++i )
    {
        size = container->m_children[i]->GetDesiredSize();
        //size = (*i)->GetDesiredSize();

        if( container->m_alignment == IFM_ALIGN_HORIZONTAL )
        {
            if( size.GetHeight() > height )
                height = size.GetHeight();
            width += size.GetWidth();
        }
        else if( container->m_alignment == IFM_ALIGN_VERTICAL )
        {
            if( size.GetWidth() > width )
                width = size.GetWidth();
            height += size.GetHeight();
        }
    }

    size.Set(width, height);

    // never size less than our minimum
    wxSize min_size = container->GetMinSize();
    // convert the min size back into client coords
    min_size = container->GetConvertedRect(wxRect(wxPoint(), min_size), IFM_COORDS_ABSOLUTE, IFM_COORDS_CLIENT).GetSize();

    if( min_size.y > size.GetHeight() && min_size.y != IFM_NO_MINIMUM )
        size.SetHeight(min_size.y);
    if( min_size.x > size.GetWidth() && min_size.x != IFM_NO_MINIMUM  )
        size.SetWidth(min_size.x);

    // convert from client to absolute coords to allow containers to have border and margin widths
    event.SetSize(container->GetConvertedRect(wxRect(wxPoint(), size), IFM_COORDS_CLIENT, IFM_COORDS_ABSOLUTE).GetSize());
}

/*
NOTE

  The event table entry for this function is commented out, because I no longer return (0,0,0,0)
  if it has no visible children
*/
void wxIFMDefaultContainerPlugin::OnGetRect(wxIFMRectEvent &event)
{
    if( event.GetComponentType() != IFM_COMPONENT_CONTAINER )
    {
        // return (0,0,0,0) if there are no visible children
        if( !HasVisibleChildren(event.GetComponent()) )
            event.SetRect(wxRect(0,0,0,0));
        else
            event.Skip();
    }
    else
        event.Skip();
}

void wxIFMDefaultContainerPlugin::OnDock(wxIFMDockEvent &event)
{
    // handle docking of components into containers
    if( event.GetDestinationType() == IFM_COMPONENT_CONTAINER )
    {
        GetNextHandler()->ProcessEvent(event);

        wxIFMComponent *component = event.GetComponent();

        // if we are docking a container into a container, give the child container
        // the IFM_ORIENTATION_DOCKED orientation value
        // FIXME: should I do this?
        if( component->GetType() == IFM_COMPONENT_CONTAINER )
        {
            wxIFMContainerData *contdata = IFM_GET_EXTENSION_DATA(component, wxIFMContainerData);

            wxASSERT_MSG(contdata, wxT("Container with no container data?"));
            if( !contdata )
                return;

            contdata->m_orientation = IFM_ORIENTATION_DOCKED;
        }
    }
    else
        event.Skip();
}

void wxIFMDefaultContainerPlugin::OnUndock(wxIFMUndockEvent &event)
{
    if( event.GetParentType() == IFM_COMPONENT_CONTAINER && event.GetComponentType() == IFM_COMPONENT_PANEL )
    {
        /*
        If the container is a docked container and will only have one child docked into it
        after this event is processed:

        -undock the container
        -undock the child from the container
        -dock the child into the containers old parent,
        -and destroy the container
        */
        wxIFMComponent *container = event.GetParent();
        wxIFMComponent *parent = container->m_parent;

        if( container->m_docked && container->m_children.GetCount() == 2 )
        {
            // save the container's index for later
            int index = 0;
            {
                wxIFMComponentArray &components = parent->m_children;
                //for( int count = components.GetCount(); index < count; index++ )
                for( wxIFMComponentArray::const_iterator i = components.begin(), end = components.end(); i != end; ++i, index++ )
                    if( *i /*components[index]*/ == container )
                        break;
            }

            GetNextHandler()->ProcessEvent(event);
            wxIFMComponent *child = container->m_children[0];

            // undock the child, but don't destroy the container
            wxIFMUndockEvent undockevt1(child, false);
            GetIP()->ProcessPluginEvent(undockevt1);

            // undock the container
            wxIFMUndockEvent undockevt2(container, false);
            GetIP()->ProcessPluginEvent(undockevt2);

            // dock the child where the container was
            wxIFMDockEvent dockevt(child, parent, index);
            GetIP()->ProcessPluginEvent(dockevt);

            // delete the container
            wxIFMDeleteComponentEvent delevt(container);
            GetIP()->ProcessPluginEvent(delevt);

            // reset the events parent because this function deletes it
            event.SetParent(parent);
            return;
        }
    }

    event.Skip();
}

#if IFM_CANFLOAT
void wxIFMDefaultContainerPlugin::UpdateFloatingCaption(wxIFMComponent *container)
{
    // if we are the root container of floating window and we have more than one child
    // display a native caption. If we have only one child, remove the native caption
    wxIFMFloatingData *floating_data = IFM_GET_EXTENSION_DATA(container, wxIFMFloatingData);
    if( !container->m_docked && floating_data->m_floating )
    {
        bool lower = false;

        wxWindow *wnd = floating_data->m_window->GetWindow();
        int style = wnd->GetWindowStyle();

        // hack around some wxWidgets "features"
        if( style & wxCAPTION )
            lower = true;

        if( GetVisibleChildrenCount(container) >= 2 )
        {
            if( style & wxCAPTION )
                return;
            style |= (wxCAPTION | wxFRAME_TOOL_WINDOW);
        }
        else
        {
            if( !(style & wxCAPTION) )
                return;
            style &= ~(wxCAPTION | wxFRAME_TOOL_WINDOW);
        }

        wnd->SetWindowStyle(style);

        // its not worth it, wxWidgets can fix their own bugs
        //if( lower )
            //wnd->Lower();
    }
}
#endif

void wxIFMDefaultContainerPlugin::OnVisibilityChange(wxIFMComponentVisibilityChangedEvent &event)
{
    if( event.GetComponentType() == IFM_COMPONENT_CONTAINER && !event.GetShow() )
    {
        wxIFMComponent *component = event.GetComponent();
        wxASSERT_MSG(component, wxT("NULL component?"));
        if( !component )
            return;

        // make sure our component buttons are hidden
        wxIFMContainerData *data = IFM_GET_EXTENSION_DATA(component, wxIFMContainerData);
        wxASSERT_MSG(data, wxT("Container with no container data?"));
        if( !data )
            return;

        //data->m_buttonManager.Show(false);
    }

    event.Skip();
}

void wxIFMDefaultContainerPlugin::OnShowComponent(wxIFMShowComponentEvent &event)
{
    /*
    wxIFMComponent *component = event.GetComponent();

    wxASSERT_MSG(component, wxT("NULL component?"));
    if( !component )
        return;

    wxIFMComponent *parent = component->m_parent;

    // hide the container if it has no more visible children
    if( parent && parent->GetType() == IFM_COMPONENT_CONTAINER )
    {

    }
    */

    event.Skip();
}

void wxIFMDefaultContainerPlugin::OnUpdate(wxIFMUpdateComponentEvent &event)
{
    // only process containers
    if( event.GetComponentType() != IFM_COMPONENT_CONTAINER )
    {
        event.Skip();
        return;
    }

    wxIFMComponent *container = event.GetComponent();

    wxASSERT_MSG(container, wxT("NULL component?"));
    if( !container )
        return;

    // size and position ourself first by passing the event down the chain
    // and to the default plugin
    GetNextHandler()->ProcessEvent(event);

    wxIFMContainerData *data = IFM_GET_EXTENSION_DATA(container, wxIFMContainerData);

    wxASSERT_MSG(data, wxT("Container with no container data?"));
    if( !data )
        return;

#if IFM_CONTAINER_GRIPPER_WIDTH
    // update the size and position of the tray_rect
    wxRect bg_rect = container->GetBackgroundRect();
    data->m_tray_rect.height = bg_rect.height;
    data->m_tray_rect.x = bg_rect.x;
    data->m_tray_rect.y = bg_rect.y;
#endif

#if IFM_CANFLOAT
    // if we are hidden, make sure our floating frame is hidden (if applicable) and do nothing
    if( container->m_hidden )
    {
        wxIFMFloatingData *floating_data = IFM_GET_EXTENSION_DATA(container, wxIFMFloatingData);
        if( floating_data->m_floating && !container->m_docked )
        {
            if( floating_data->m_window->GetWindow()->IsShown() )
                floating_data->m_window->GetWindow()->Hide();
        }
        return;
    }
#endif

    // current will be -1 if no components are visible
    int current = wxIFMComponent::GetNextVisibleComponent(container->m_children, 0);

    if( !HasVisibleChildren(container) )
        current = -1;

#if IFM_CANFLOAT
    wxIFMFloatingData *floating_data = IFM_GET_EXTENSION_DATA(container, wxIFMFloatingData);
    // if there are no visible children and we are a top level floating container, hide the floating window
    if( current == -1 && floating_data->m_floating && !container->m_docked )
    {
        floating_data->m_window->GetWindow()->Hide();
        return;
    }

    // if there are visible children and we are a top level floating container, make sure our floating window is shown
    if( current >= 0 && floating_data->m_floating && !container->m_docked )
    {
        if( !floating_data->m_window->GetWindow()->IsShown() )
            floating_data->m_window->GetWindow()->Show();
    }
#endif

    // don't use this for now, its not done yet
#if 0
    {
        wxIFMContainerData *data = IFM_GET_EXTENSION_DATA(container, wxIFMContainerData);

        wxASSERT_MSG(data, wxT("Container with no container data?"));
        if( !data )
            return;

        if( current == -1 )
        {
            // no visible children, make sure our component buttons are hidden too
            if( data )
                data->m_buttonManager.Show(false);

            return;
        }
        else if( HasNonCloseableChildren(container) )
        {
            if( data )
            {
                data->m_buttonManager.Show(true);
                data->m_buttonManager.Show(false, IFM_COMPONENT_ID_CLOSE);
            }
        }
        else
            if( data )
                data->m_buttonManager.Show(true);
    }
#endif

    // calculate rects of our children using the calc rects event
    wxIFMCalcRectsEvent calcevt(container);

    wxIFMComponentArray &children = container->m_children;
    //wxIFMComponentArray::iterator i, end;
    //for( i = children.begin(), end = children.end(); i != end; ++i )
    for( size_t i1 = 0; i1 < children.GetCount(); i1++ )
        calcevt.AddComponent(children[i1]);

    GetIP()->ProcessPluginEvent(calcevt);

    // position children
    const wxRectArray &rects = calcevt.GetComponentRects();
    //wxRectArray::const_iterator ri;
    for( size_t i = 0, ri = 0; i < children.GetCount(); i++, ri++ )
    //for( i = children.begin(), end = children.end(), ri = rects.begin(); i != end; ++i, ri++ )
    {
        if( children[i]->m_hidden )
            continue;
        else
        {
            wxIFMUpdateComponentEvent updevt(children[i], rects[ri]);
            GetIP()->ProcessPluginEvent(updevt);
        }
    }

    // position our buttons
#if 0
    wxIFMContainerData *data = IFM_GET_EXTENSION_DATA(container, wxIFMContainerData);
    wxASSERT_MSG(data, wxT("Container with no container data?"));
    if( data )
    {
        wxSize size = data->m_buttonManager.GetRect().GetSize();
        wxRect rect = container->GetBackgroundRect();
        data->m_buttonManager.SetPosition(wxPoint(rect.x + 1, rect.y));
        data->m_buttonManager.Layout();
    }
#endif

#if IFM_CANFLOAT
    // make sure our caption is hidden or shown as needed
    UpdateFloatingCaption(container);
#endif
}

#if IFM_CANFLOAT
void wxIFMDefaultContainerPlugin::OnFloatNotify(wxIFMFloatNotifyEvent &event)
{
    if( event.GetComponentType() == IFM_COMPONENT_CONTAINER )
    {
        GetNextHandler()->ProcessEvent(event);

        wxIFMComponent *component = event.GetComponent();

        wxASSERT_MSG(component, wxT("NULL component?"));
        if( !component )
            return;

        wxIFMContainerData *data = IFM_GET_EXTENSION_DATA(component, wxIFMContainerData);
        wxASSERT_MSG(data, wxT("Container with no container data?"));
        if( !data )
            return;

        // if we are floating a top level floating container
        // remove ourselves from the top level container list first
        if( event.GetFloating() )
        {
            if( data && (data->m_orientation == IFM_ORIENTATION_LEFT || data->m_orientation == IFM_ORIENTATION_RIGHT || data->m_orientation == IFM_ORIENTATION_TOP || data->m_orientation == IFM_ORIENTATION_BOTTOM) )
            {
                wxIFMRemoveTopContainerEvent evt(component);
                GetIP()->ProcessPluginEvent(evt);
            }
        }

        //data->m_buttonManager.SetParent(component->GetParentWindow());
    }
    else
        event.Skip();
}
#endif

void wxIFMDefaultContainerPlugin::OnComponentButtonClick(wxIFMComponentButtonEvent &event)
{
    wxIFMComponent *component = event.GetComponent();
    if( component->GetType() == IFM_COMPONENT_CONTAINER )
    {
        wxIFMComponentButton *button = event.GetButton();
        if( button->GetID() == IFM_COMPONENT_ID_CLOSE )
        {
            // hide ourself
            component->Show(false, true);
            return;
        }
    }

    event.Skip();
}

void wxIFMDefaultContainerPlugin::OnLeftDown(wxIFMMouseEvent &event)
{
    if( event.GetComponentType() == IFM_COMPONENT_CONTAINER )
    {
        wxPoint pos = event.GetMouseEvent().GetPosition();

        wxIFMComponent *component = event.GetComponent();

        wxASSERT_MSG(component, wxT("NULL component?"));
        if( !component )
            return;

        wxIFMContainerData *data = IFM_GET_EXTENSION_DATA(component, wxIFMContainerData);

        wxASSERT_MSG(data, wxT("Container with no container data?"));
        if( !data )
            return;

        if( data->m_tray_rect.Inside(pos) )
        {
            wxIFMInitDragEvent evt(component, component, pos);
            GetIP()->ProcessPluginEvent(evt);
        }
    }
    else
        event.Skip();
}

/*
wxIFMDefaultPanelPlugin implementation
*/
BEGIN_EVENT_TABLE(wxIFMDefaultPanelPlugin, wxIFMExtensionPluginBase)
    EVT_IFM_NEWCOMPONENT    (wxIFMDefaultPanelPlugin::OnCreateComponent)
    EVT_IFM_DELETECOMPONENT (wxIFMDefaultPanelPlugin::OnDestroyComponent)
    EVT_IFM_UPDATECOMPONENT (wxIFMDefaultPanelPlugin::OnUpdate)
    EVT_IFM_VISIBILITYCHANGED (wxIFMDefaultPanelPlugin::OnVisibilityChanged)
    EVT_IFM_SHOWCOMPONENT   (wxIFMDefaultPanelPlugin::OnShowComponent)
    EVT_IFM_PAINTDECOR      (wxIFMDefaultPanelPlugin::OnPaintDecor)
    EVT_IFM_PAINTBG         (wxIFMDefaultPanelPlugin::OnPaintBg)
    EVT_IFM_DOCK            (wxIFMDefaultPanelPlugin::OnDock)
    EVT_IFM_UNDOCK          (wxIFMDefaultPanelPlugin::OnUndock)
    EVT_IFM_COMPONENTBUTTONCLICK (wxIFMDefaultPanelPlugin::OnComponentButtonClick)
    EVT_IFM_LEFTDOWN        (wxIFMDefaultPanelPlugin::OnLeftDown)
    EVT_IFM_SELECTTAB       (wxIFMDefaultPanelPlugin::OnSelectTab)
    EVT_IFM_GETDESIREDSIZE  (wxIFMDefaultPanelPlugin::OnGetDesiredSize)
    EVT_IFM_SETDESIREDSIZE  (wxIFMDefaultPanelPlugin::OnSetDesiredSize)
    EVT_IFM_GETMINSIZE      (wxIFMDefaultPanelPlugin::OnGetMinSize)
    EVT_IFM_GETMAXSIZE      (wxIFMDefaultPanelPlugin::OnGetMaxSize)
    EVT_IFM_QUERYCHILD      (wxIFMDefaultPanelPlugin::OnQueryChild)
    EVT_IFM_CONVERTRECT     (wxIFMDefaultPanelPlugin::OnConvertRect)
    EVT_IFM_UPDATECONFIG    (wxIFMDefaultPanelPlugin::OnUpdateConfig)

#if IFM_CANFLOAT
    EVT_IFM_FLOATING_NOTIFY (wxIFMDefaultPanelPlugin::OnFloatNotify)
#endif
END_EVENT_TABLE()

wxIFMCaptionConfig wxIFMDefaultPanelPlugin::m_caption_config;
wxIFMTabConfig wxIFMDefaultPanelPlugin::m_tab_config;
bool wxIFMDefaultPanelPlugin::init_config = true;

wxIFMCaptionConfig &wxIFMDefaultPanelPlugin::GetCaptionConfig()
{
    return m_caption_config;
}

wxIFMTabConfig &wxIFMDefaultPanelPlugin::GetTabConfig()
{
    return m_tab_config;
}

wxIFMDefaultPanelPlugin::wxIFMDefaultPanelPlugin()
: wxIFMExtensionPluginBase()
{
    if( init_config )
    {
        init_config = false;
        m_tab_config.init();
        m_caption_config.init();
    }
}

bool wxIFMDefaultPanelPlugin::Initialize(wxIFMInterfacePluginBase *plugin)
{
    wxIFMExtensionPluginBase::Initialize(plugin);

    // load component button bitmaps
    m_bmpClose = wxBitmap(btn_hide_xpm);
    m_bmpMin = wxBitmap(btn_min_xpm);
    m_bmpMax = wxBitmap(btn_max_xpm);
    m_bmpCloseH = wxBitmap(btn_hide_h_xpm);
    m_bmpMinH = wxBitmap(btn_min_h_xpm);
    m_bmpMaxH = wxBitmap(btn_max_h_xpm);
    m_bmpCloseP = wxBitmap(btn_hide_hp_xpm);
    m_bmpMinP = wxBitmap(btn_min_hp_xpm);
    m_bmpMaxP = wxBitmap(btn_max_hp_xpm);

    return true;
}

void wxIFMDefaultPanelPlugin::OnUpdateConfig(wxIFMUpdateConfigEvent &WXUNUSED(event))
{
    // iterate over all components looking for panels and update them
    const wxIFMComponentArray &components = GetIP()->GetComponents();
    for( unsigned int i = 0; i < components.size(); ++i )
    {
        wxIFMComponent *component = components[i];
        if( component->GetType() == IFM_COMPONENT_PANEL )
        {
            // recalc tab sizes
            UpdateTabs(component);

            // update top margin to reflect new caption size
            component->m_margins.top = m_caption_config.m_size + IFM_CAPTION_PAD*2;
        }
    }
}

void wxIFMDefaultPanelPlugin::OnCreateComponent(wxIFMNewComponentEvent &event)
{
    // create tabs
    if( event.GetComponentType() == IFM_COMPONENT_PANEL_TAB )
    {
        // create a new component
        wxIFMComponent *tab = new wxIFMComponent(GetIP(), IFM_COMPONENT_PANEL_TAB);

        // add panel specific data storage
        wxIFMPanelTabData *tabdata = new wxIFMPanelTabData();

        tab->AddExtensionData(tabdata);

        tab->m_borders.Set(0);
        tab->m_margins.Set(0);

        // return the new component
        event.SetComponent(tab);
    }
    // create panels too
    else if( event.GetComponentType() == IFM_COMPONENT_PANEL )
    {
        wxIFMComponent *panel = new wxIFMComponent(GetIP(), IFM_COMPONENT_PANEL);
        wxIFMPanelData *data = new wxIFMPanelData(GetIP(), panel);
        panel->AddExtensionData(data);

        panel->m_margins.Set(1);
        panel->m_margins.top = m_caption_config.m_size + IFM_CAPTION_PAD*2;
        panel->m_borders.Set(1);

        // add close, maximize, minimize buttons
        wxIFMComponentButton *btn;

        /*
            We dont use these buttons yet, comment them out until they are implemented

        btn = data->m_buttonManager.AddButton(IFM_COMPONENT_ID_MINIMIZE);
        btn->m_bmp = &m_bmpMin;
        btn->m_bmpH = &m_bmpMinH;
        btn->m_bmpP = &m_bmpMinP;

        btn = data->m_buttonManager.AddButton(IFM_COMPONENT_ID_MAXIMIZE);
        btn->m_bmp = &m_bmpMax;
        btn->m_bmpH = &m_bmpMaxH;
        btn->m_bmpP = &m_bmpMaxP;
        */

        data->m_button_manager.SetPosition(wxPoint(1, (m_caption_config.m_size - IFM_COMPONENT_BUTTON_Y) / 2));
        
		btn = data->m_button_manager.AddButton(IFM_COMPONENT_ID_CLOSE);
		btn->m_bmp = &m_bmpClose;
		btn->m_bmpH = &m_bmpCloseH;
		btn->m_bmpP = &m_bmpCloseP;
		
		if( m_caption_config.m_style == IFM_CAPTION_GRADIENT )
			btn->SetBackgroundColour(m_caption_config.m_gradient_color);
		else
			btn->SetBackgroundColour(m_caption_config.m_caption_color);
		
		
        event.SetComponent(panel);
    }
    else
        event.Skip();
}

void wxIFMDefaultPanelPlugin::OnDestroyComponent(wxIFMDeleteComponentEvent &event)
{
    if( event.GetComponentType() == IFM_COMPONENT_PANEL )
    {
        wxIFMComponent *panel = event.GetComponent();

        wxASSERT_MSG(panel, wxT("NULL component?"));
        if( !panel )
            return;

        wxIFMPanelData *data = IFM_GET_EXTENSION_DATA(panel, wxIFMPanelData);

        wxASSERT_MSG(data, wxT("Panel with no panel data?"));
        if( !data )
            return;

        data->m_button_manager.Hide();
    }
    event.Skip();
}

void wxIFMDefaultPanelPlugin::OnConvertRect(wxIFMConvertRectEvent &event)
{
    if( event.GetComponentType() == IFM_COMPONENT_PANEL )
    {
        int source = event.GetSourceCoords(), dest = event.GetDestinationCoords();

        wxIFMComponent *panel = event.GetComponent();
        wxIFMPanelData *data = IFM_GET_EXTENSION_DATA(panel, wxIFMPanelData);

        // if we have more than one tab, account for the tab row in the margins
        if( ShouldDrawTabs(data->m_tabs) )
        {
            if( source == IFM_COORDS_CLIENT && dest == IFM_COORDS_BACKGROUND )
            {
                wxRect rect = event.GetRect();

				if(!m_tab_config.get_top()) //maks
				{
					rect.height += m_tab_config.m_row_height;
				}

                event.SetRect(rect);
            }
            else if( source == IFM_COORDS_BACKGROUND && dest == IFM_COORDS_CLIENT )
            {
                wxRect rect = event.GetRect();

                if(!m_tab_config.get_top()) //maks				
				{
					rect.height -= m_tab_config.m_row_height;
				}

                event.SetRect(rect);
            }
        }
    }

    event.Skip();
}

void wxIFMDefaultPanelPlugin::OnQueryChild(wxIFMQueryChildEvent &event)
{
    wxIFMComponent *tab = event.GetComponent();
    if(tab && tab->GetType() == IFM_COMPONENT_PANEL_TAB ) //maks: avoid crash when use IsChildVisible with a invalid window
    {
        event.SetVisible(false);
        wxIFMComponent *parent = tab->m_parent;

        if( !parent->IsVisible() )
            return;

        wxIFMPanelData *data = IFM_GET_EXTENSION_DATA(parent, wxIFMPanelData);
        for( unsigned int i = 0; i < data->m_tabs.size(); i++ )
        {
            wxIFMTab *tab = data->m_tabs[i];
            if( tab->m_tab->m_child == event.GetChild() )
            {
                event.SetVisible(tab->m_visible);
                break;
            }
        }
    }
    else
        event.Skip();
}

void wxIFMDefaultPanelPlugin::OnDock(wxIFMDockEvent &event)
{
    // if we are docking a panel with one tab, dock the tab, instead of the panel
    if( event.GetDestinationType() == IFM_COMPONENT_PANEL && event.GetComponentType() == IFM_COMPONENT_PANEL )
    {
        wxIFMComponent *panel = event.GetComponent();

        wxASSERT_MSG(panel, wxT("NULL component?"));
        if( !panel )
            return;

        wxIFMPanelData *data = IFM_GET_EXTENSION_DATA(panel, wxIFMPanelData);

        wxASSERT_MSG(data, wxT("Panel with no panel data?"));
        if( !data )
            return;

#if IFM_ALLOW_NESTED_PANELS
        // if there is only one tab, dock that tab as a tab of this panel instead of nesting
        if( data->m_tabs.size() == 1 )
        {
            wxIFMComponent *tab = data->m_tabs[0]->m_tab;

            // dock the tab of that panel into the destination panel
            wxIFMUndockEvent undock(tab, false);
            GetIP()->ProcessPluginEvent(undock);

            wxIFMDockEvent dock(tab, event.GetDestination(), event.GetIndex());
            GetIP()->ProcessPluginEvent(dock);

            return;
        }
#else
        // dock all the tabs of this panel as tabs of the destination panel
        for( ; data->m_tabs.size(); )
        {
            wxIFMComponent *tab = data->m_tabs[0]->m_tab;

            // dock the tab of that panel into the destination panel
            wxIFMUndockEvent undock(tab, false);
            GetIP()->ProcessPluginEvent(undock);

            wxIFMDockEvent dock(tab, event.GetDestination(), event.GetIndex());
            GetIP()->ProcessPluginEvent(dock);
        }

        return;
#endif
    }

    // dock stuff into panels
    if( event.GetDestinationType() == IFM_COMPONENT_PANEL )
    {
        wxIFMComponent *panel = event.GetDestination();
        wxIFMComponent *panel_tab = event.GetComponent();

        wxASSERT_MSG(panel && panel_tab, wxT("NULL tab or panel?"));
        if( !panel || !panel_tab )
            return;

        // default processing
        GetNextHandler()->ProcessEvent(event);

        wxIFMPanelData *data = IFM_GET_EXTENSION_DATA(panel, wxIFMPanelData);

        wxASSERT_MSG(data, wxT("Panel with no panel data?"));
        if( !data )
            return;

        // if this is the first tab of this panel, make the panels desired size large enough
        // to accomodate the panel
        if( panel->m_children.size() == 1 )
        {
            panel->SetDesiredSize(panel->GetConvertedRect( // convert from client to absolute
                wxRect(wxPoint(), panel_tab->GetDesiredSize()), // make a rect
                IFM_COORDS_CLIENT, IFM_COORDS_ABSOLUTE).GetSize()); // get its size
        }

        int index = event.GetIndex(), count = panel_tab->m_children.size() - 1;

        // create and initialize the tab
        wxIFMTab *tab = new wxIFMTab;
        tab->m_tab = panel_tab;

        // calculate the width of the tabs name string
        int h;
		wxString tabName(tab->m_tab->m_name); //maks

		if(m_tab_config.m_rounding_factor < 0)
		{
			//Lightwave tabs
			tabName = wxT(" ");
			tabName += tab->m_tab->m_name;
			//tabName += wxT(" ");
		}
		

        GetManager()->GetParent()->GetTextExtent(tabName, &tab->m_name_width, &h, 0, 0, &m_tab_config.m_font); //maks
        if( tab->m_name_width == 0 )
            tab->m_name_width = 20;

        if( index == IFM_DEFAULT_INDEX || index > count )
        {
            // add it as the last tab
            data->m_tabs.push_back(tab);
        }
        else
        {
            // add it where it wants to be
            data->m_tabs.insert(data->m_tabs.begin() + index, tab);
        }

        panel_tab->m_parent = panel;

        // generate a name for this panel so that docking panels as tabs doesn't yield a blank tab
        wxString name;
        for( unsigned int i = 0; i < panel->m_children.size(); ++i )
            name += panel->m_children[i]->m_name + " / ";
        panel->m_name = name.SubString(0, name.Length() - 3);

        // update the tab names for our parent
        UpdateTabs(panel->m_parent);

        wxIFMSelectTabEvent evt(panel, tab, false);
        GetIP()->ProcessPluginEvent(evt);
    }
    // also, if a tab is docked directly into a container
    // dock a panel there instead, and then dock the tab into the panel
    else if( event.GetDestinationType() == IFM_COMPONENT_CONTAINER && event.GetComponentType() == IFM_COMPONENT_PANEL_TAB )
    {
        wxIFMComponent *tab = event.GetComponent();

        // create the tab
        wxIFMNewComponentEvent newevt(IFM_COMPONENT_PANEL);
        GetIP()->ProcessPluginEvent(newevt);

        wxIFMComponent *panel = newevt.GetComponent();

        // dock the panel where the tab was going
        wxIFMDockEvent dockevt1(panel, event.GetDestination(), event.GetIndex());
        GetIP()->ProcessPluginEvent(dockevt1);

        // dock the tab into the panel
        wxIFMDockEvent dockevt2(tab, panel, 0);
        GetIP()->ProcessPluginEvent(dockevt2);
    }
    else
        event.Skip();
}

void wxIFMDefaultPanelPlugin::OnUndock(wxIFMUndockEvent &event)
{
    // undock stuff from panels
    if( event.GetParentType() == IFM_COMPONENT_PANEL )
    {
        // find the tab which represented this component and remove it

        wxIFMComponent *panel = event.GetParent();
        wxIFMComponent *panel_tab = event.GetComponent();

        wxASSERT_MSG(panel, wxT("NULL component?"));
        if( !panel )
            return;

        wxIFMPanelData *data = IFM_GET_EXTENSION_DATA(panel, wxIFMPanelData);

        wxASSERT_MSG(data, wxT("Tabbed panel with no tabbed panel data?"));
        if( !data )
            return;

        wxIFMTab *tab = NULL;
        unsigned int i;
        for( i = 0; i < data->m_tabs.size(); i++ )
        //for( wxIFMTabArray::iterator i = data->m_tabs.begin(), end = data->m_tabs.end(); i != end; ++i )
        {
            tab = data->m_tabs[i];
            if( tab->m_tab == panel_tab )
            {
                data->m_tabs.RemoveAt(i);
                break;
            }
            tab = NULL;
        }

        // select a new tab
        if( data->m_tabs.size() > 0 )
        {
            if( tab == data->m_active_tab )
            {
                wxIFMSelectTabEvent evt(panel, data->m_tabs[0], true);
                GetIP()->ProcessPluginEvent(evt);
            }
        }
        else
            data->m_active_tab = NULL;

        delete tab;

        // default processing
        GetNextHandler()->ProcessEvent(event);

        // generate a name for this panel so that docking panels as tabs doesn't yield a blank tab
        wxString name;
        for( i = 0; i < panel->m_children.size(); ++i )
        {
            name += panel->m_children[i]->m_name + " / ";
        }

        panel->m_name = name.SubString(0, name.Length() - 3);

        // make sure the tabs of our parent (if applicable) use the new name string
        UpdateTabs(panel->m_parent);
    }
    else
        event.Skip();
}

void wxIFMDefaultPanelPlugin::UpdateTabs(wxIFMComponent *panel)
{
    if( panel && panel->GetType() == IFM_COMPONENT_PANEL )
    {
        wxIFMPanelData *data = IFM_GET_EXTENSION_DATA(panel, wxIFMPanelData);

        wxASSERT_MSG(data, wxT("Panel with no panel data?"));
        if( !data )
            return;

        for( unsigned int i = 0; i < data->m_tabs.size(); ++i )
        {
            wxIFMTab *tab = data->m_tabs[i];

            // calculate the width of the tabs name string
			wxString tabName(tab->m_tab->m_name); //maks
			
			if(m_tab_config.m_rounding_factor < 0)
			{
				//Lightwave tabs
				tabName = wxT(" ");
				tabName += tab->m_tab->m_name;
				//tabName += wxT(" ");
			}

            GetManager()->GetParent()->GetTextExtent(tabName, &tab->m_name_width, 0, 0, 0, &m_tab_config.m_font);
            if( tab->m_name_width == 0 )
                tab->m_name_width = 20;
        }

        // make sure the parents tabs (if any) are also updated
        UpdateTabs(panel->m_parent);
    }
}

void wxIFMDefaultPanelPlugin::OnUpdate(wxIFMUpdateComponentEvent &event)
{
    // process tabs
    if( event.GetComponentType() == IFM_COMPONENT_PANEL_TAB )
    {
        wxIFMComponent *tab = event.GetComponent();

        wxASSERT_MSG(tab, wxT("NULL component?"));
        if( !tab )
            return;

        wxASSERT_MSG(tab->m_child, wxT("Panel with null child!"));
        if( !tab->m_child )
            return;

        if( !tab->IsVisible() )
        {
            // make sure our child is hidden
            if( tab->m_child->IsShown() )
                tab->m_child->Show(false);
            return;
        }
        else
        {
            // make sure our child is visible since the tab is visible
            if( !tab->m_child->IsShown() )
                tab->m_child->Show();
        }

        // size and position ourself by passing the event down the chain
        // and to the default component handler
        // WARNING: If plugins are added in the wrong order GetNextHandler may return null!
        GetNextHandler()->ProcessEvent(event);

        // request client size and position
        wxIFMRectEvent rectevt2(wxEVT_IFM_GETCLIENTRECT, tab);
        GetIP()->ProcessPluginEvent(rectevt2);

        // size our child window
        tab->m_child->SetSize(rectevt2.GetRect());
    }
    // and panels
    else if( event.GetComponentType() == IFM_COMPONENT_PANEL )
    {
        wxIFMComponent *panel = event.GetComponent();

        wxASSERT_MSG(panel, wxT("NULL component?"));
        if( !panel )
            return;

        wxIFMPanelData *data = IFM_GET_EXTENSION_DATA(panel, wxIFMPanelData);

        wxASSERT_MSG(data, wxT("Tabbed panel with no tabbed panel data?"));
        if( !data )
            return;

        if( !data->m_active_tab )
            return;

        // forward the update event to the active panel but size ourself first
        GetNextHandler()->ProcessEvent(event);

        // set the caption rect
        wxRect rect = panel->GetBackgroundRect();
        data->m_caption.x = rect.x + IFM_CAPTION_PAD;
        data->m_caption.y = rect.y + IFM_CAPTION_PAD;
        data->m_caption.height = m_caption_config.m_size;
        data->m_caption.width = rect.width - IFM_CAPTION_PAD*2;

        // update the active panel tab
        wxRect client_rect = panel->GetClientRect();
        wxIFMUpdateComponentEvent evt(data->m_active_tab->m_tab, client_rect);
        GetIP()->ProcessPluginEvent(evt);

        wxRect tab_rect;
        tab_rect.x = rect.x + m_tab_config.m_indent;
		if(m_tab_config.get_top()) //maks
		{
			tab_rect.y = rect.y + IFM_CAPTION_PAD*2;
		}
		else
		{
			tab_rect.y = rect.y + rect.height - m_tab_config.m_row_height;
		}
        
        tab_rect.height = m_tab_config.m_tab_height;

        // update the tab positions
        for( size_t i = 0; i < data->m_tabs.GetCount(); i++ )
        //for( wxIFMTabArray::iterator i = data->m_tabs.begin(), end = data->m_tabs.end(); i != end; ++i )
        {
            wxIFMTab *tab = data->m_tabs[i];

            if( !tab->m_visible )
                continue;

            wxIFMPanelTabData *tab_data = IFM_GET_EXTENSION_DATA(tab->m_tab, wxIFMPanelTabData);

            tab_rect.width = tab->m_name_width + m_tab_config.m_tab_padding + m_tab_config.m_tab_padding;

            if(tab_data && tab_data->m_bitmap.Ok())
                tab_rect.width += tab_data->m_bitmap.GetWidth() + m_tab_config.m_tab_padding;

            tab->m_rect = tab_rect;
            tab_rect.x += tab_rect.width + m_tab_config.m_tab_spacing;
        }

        // position compontent buttons
        wxSize size = data->m_button_manager.GetRect().GetSize();
        data->m_button_manager.SetPosition(wxPoint(data->m_caption.x + data->m_caption.width - size.x, data->m_caption.y));
        data->m_button_manager.Layout();
    }
    else
        event.Skip();
}

void wxIFMDefaultPanelPlugin::OnShowComponent(wxIFMShowComponentEvent &event)
{
    GetNextHandler()->ProcessEvent(event);

    bool show = event.GetShow();

    wxIFMComponent *component = event.GetComponent();

    wxASSERT_MSG(component, wxT("NULL component?"));
    if( !component )
        return;

    wxIFMComponent *panel = component->m_parent;

    if( !panel )
        return;

    if( panel->GetType() == IFM_COMPONENT_PANEL )
    {
        // show or hide the tab for this child
        wxIFMPanelData *data = IFM_GET_EXTENSION_DATA(panel, wxIFMPanelData);

		if(show) //maks: adjust close button visibility
		{
			wxIFMComponentButton *button = data->m_button_manager.GetButton(IFM_COMPONENT_ID_CLOSE);
			
			if(button)
			{
				if(component->m_canHide) 
				{
					button->Show(true);	
				}
				else
				{
					button->Show(false);
				}
			}
		}

        // don't do anything if we hide this component ourselves
        if( data->m_dont_hide_tab )
        {
            data->m_dont_hide_tab = false;
            return;
        }

        // find the tab
        wxIFMTab *tab = NULL;
        for( size_t i = 0; i < data->m_tabs.GetCount(); i++ )
        //for( wxIFMTabArray::iterator i = data->m_tabs.begin(), end = data->m_tabs.end(); i != end; ++i )
        {
            tab = data->m_tabs[i];
            if( tab->m_tab == component )
                break;
        }

        tab->m_visible = show;

        if( !show )
        {
            bool vis = false;

            // if there are no visible tabs left, hide the panel too
            for( size_t i = 0; i < data->m_tabs.GetCount(); i++ )
            //for( wxIFMTabArray::iterator i = data->m_tabs.begin(), end = data->m_tabs.end(); i != end; ++i )
            {
                wxIFMTab *tab = data->m_tabs[i];
                if( tab->m_visible )
                {
                    vis = true;
                    break;
                }
            }

            if( !vis )
                panel->Hide();

            // if we hide the active tab, select another
            else if( tab->m_active )
            {
                // find the next visible tab to select
                wxIFMTab *new_tab;
                for( size_t i = 0; i < data->m_tabs.GetCount(); i++ )
                //for( wxIFMTabArray::iterator i = data->m_tabs.begin(), end = data->m_tabs.end(); i != end; ++i )
                {
                    new_tab = data->m_tabs[i];
                    if( new_tab->m_visible && new_tab != tab )
                    {
                        wxIFMSelectTabEvent evt(panel, new_tab, false);
                        GetIP()->ProcessPluginEvent(evt);
                        break;
                    }
                }
            }
        }
        // select the tab if its being shown
        else
        {
            wxIFMSelectTabEvent evt(panel, tab, false);
            GetIP()->ProcessPluginEvent(evt);
        }
    }
}

void wxIFMDefaultPanelPlugin::OnVisibilityChanged(wxIFMComponentVisibilityChangedEvent &event)
{
    wxIFMComponent *component = event.GetComponent();

    wxASSERT_MSG(component, wxT("NULL component?"));
    if( !component )
        return;

    bool shown = event.GetShow();

    if( event.GetComponentType() == IFM_COMPONENT_PANEL_TAB )
    {
        // we would normally show the child here as well, but we
        // will wait for the next update component event and let it
        // take care of that for us

        // we will however hide the child immediately if needed
        if( !shown )
            component->m_child->Show(false);
    }
    else if( event.GetComponentType() == IFM_COMPONENT_PANEL )
    {
        wxIFMPanelData *data = IFM_GET_EXTENSION_DATA(component, wxIFMPanelData);

        wxASSERT_MSG(data, wxT("Tabbed panel with no tabbed panel data?"));
        if( !data )
            return;

        data->m_button_manager.Show(shown);
    }

    // if the component's parent is a panel, then the component is a tab of that panel.
    // show or hide that tab if the component was hidden

    // skip to let the default implementation send the notification to our children, in case we have any
    event.Skip();
}

#if IFM_CANFLOAT

void wxIFMDefaultPanelPlugin::OnFloatNotify(wxIFMFloatNotifyEvent &event)
{
    wxIFMComponent *component = event.GetComponent();

    wxASSERT_MSG(component, wxT("NULL component?"));
    if( !component )
        return;

    // process for panel tabs
    if( event.GetComponentType() == IFM_COMPONENT_PANEL_TAB )
    {
        // let the default plugin go first
        GetNextHandler()->ProcessEvent(event);

        if( component->m_child )
            component->m_child->Reparent(component->GetParentWindow());
    }
    // and panels
    else if( event.GetComponentType() == IFM_COMPONENT_PANEL )
    {
        // let the default plugin go first
        GetNextHandler()->ProcessEvent(event);

        wxIFMPanelData *data = IFM_GET_EXTENSION_DATA(component, wxIFMPanelData);

        wxASSERT_MSG(data, wxT("Panel with no panel data?"));
        if( !data )
            return;

        data->m_button_manager.SetParent(component->GetParentWindow());
    }
    else
        event.Skip();
}

#endif // IFM_CANFLOAT

void wxIFMDefaultPanelPlugin::OnPaintDecor(wxIFMPaintEvent &event)
{
    if( event.GetComponentType() == IFM_COMPONENT_PANEL )
    {
        wxRegion region = event.GetRegion();
        wxIFMComponent *component = event.GetComponent();

        wxASSERT_MSG(component, wxT("NULL component?"));
        if( !component )
            return;

        if( component->m_hidden )
            return;

        wxIFMPanelData *data = IFM_GET_EXTENSION_DATA(component, wxIFMPanelData);

        wxASSERT_MSG(data, wxT("Panel with no panel data?"));
        if( !data )
            return;

		bool bDrawTabs = ShouldDrawTabs(data->m_tabs); //maks

        wxDC &dc = event.GetDC();
        wxRegionContain result = region.Contains(data->m_caption);
        if( result == wxPartRegion || result == wxInRegion )
        {
            // draw caption
            DrawCaption(dc, data->m_caption, (data->m_active_tab) ? data->m_active_tab->m_tab->m_name : wxT(""),
                IFM_ORIENTATION_TOP, !bDrawTabs); //maks
        }

        if( bDrawTabs )
            DrawTabs(dc, component, region, data->m_tabs);
    }
    else
        event.Skip();
}

bool wxIFMDefaultPanelPlugin::ShouldDrawTabs(const wxIFMTabArray &tabs)
{
    int count = 0;
	if(m_tab_config.get_top()) //maks
	{
		return true;
	}

    // show tabs if there is more than one visible tab
    for( unsigned int i = 0; i < tabs.size(); ++i )
    {
        if( tabs[i]->m_visible )
            count++;

        if( count > 1 )
            return true;
    }

    return false;
}

//#include <crtdbg.h>

void wxIFMDefaultPanelPlugin::OnPaintBg(wxIFMPaintEvent &event)
{
    /*
    _CrtDbgReport(_CRT_WARN, 0, 0, 0, "Painting bg\n");
    _CrtDbgReport(_CRT_WARN, 0, 0, 0, "Painting bg\n");

    _CrtDbgReport(_CRT_WARN, 0, 0, 0, "Painting bg\n");

    _CrtDbgReport(_CRT_WARN, 0, 0, 0, "Painting bg\n");
    _CrtDbgReport(_CRT_WARN, 0, 0, 0, "Painting bg\n");
    */

    if( event.GetComponentType() == IFM_COMPONENT_PANEL || event.GetComponentType() == IFM_COMPONENT_PANEL_TAB )
    {
        wxIFMComponent *component = event.GetComponent();

        wxASSERT_MSG(component, wxT("NULL component?"));
        if( !component )
            return;

        if( component->m_hidden )
            return;

        wxRect rect = component->GetBackgroundRect();
        wxDC &dc = event.GetDC();

        // draw the background with the same color as the active tab background
        dc.SetBrush(m_tab_config.m_bg_brush/*maks m_tab_active_bg_b*/);
        dc.SetPen(wxPen(m_tab_config.m_tab_active_bg_p /*maks b.GetColour()*/));

        dc.DrawRectangle(rect);
    }
    else
        event.Skip();
}

void wxIFMDefaultPanelPlugin::DrawOneLightWaveTab(wxDC &dc, wxIFMTab *tab) //maks
{
	wxIFMPanelTabData *tab_data;
	wxRect rect;
	rect = tab->m_rect;

	//rect.SetHeight(rect.GetHeight() - 4);
	
	
	wxPoint points[4];
	
	if(m_tab_config.get_top()) 
	{
		points[0].x = rect.x;
		points[0].y = rect.y + rect.height;
		
		points[1].x = rect.x;
		points[1].y = rect.y;

		points[2].x = rect.x + rect.width - rect.height/2;
		points[2].y = rect.y;
		
		points[3].x = rect.x + rect.width + rect.height/2 + 1;
		points[3].y = rect.y+ rect.height;		
	}
	else
	{
		points[0].x = rect.x;
		points[0].y = rect.y + rect.height;
		
		points[1].x = rect.x;
		points[1].y = rect.y;
		
		points[2].x = rect.x + rect.width + rect.height/2 + 1;
		points[2].y = rect.y;
		
		points[3].x = rect.x + rect.width - rect.height/2;
		points[3].y = rect.y + rect.height;
	}
	
	
	// draw the tab background
	// if the tab is not selected draw a black line above
	if( tab->m_active )
	{
		dc.SetBrush(m_tab_config.m_tab_active_bg_b);
		dc.SetPen(m_tab_config.m_tab_border_p);
		
		dc.DrawPolygon(4, points);		
	}
	else
	{
		dc.SetBrush(m_tab_config.m_tab_inactive_bg_b);
		dc.SetPen(m_tab_config.m_tab_border_p);

		dc.DrawPolygon(4, points);	
	}
	
	tab_data = IFM_GET_EXTENSION_DATA(tab->m_tab, wxIFMPanelTabData);
	
	// draw bitmap
	if(tab_data && tab_data->m_bitmap.Ok())
	{
		wxBitmap &bmp = tab_data->m_bitmap;
		dc.DrawBitmap(bmp, rect.x + m_tab_config.m_tab_padding, rect.y + (rect.height - bmp.GetHeight())/2, true);
		rect.x += m_tab_config.m_tab_padding + bmp.GetWidth();
	}
	
	// draw the label
	dc.SetFont(m_tab_config.m_font);
	dc.SetTextForeground(m_tab_config.m_font_color);
	
	dc.DrawText(wxString(" ") + tab->m_tab->m_name, rect.x + m_tab_config.m_tab_padding, rect.y + (rect.height - m_tab_config.m_font_height) / 2);
}

void wxIFMDefaultPanelPlugin::DrawTabs(wxDC &dc, wxIFMComponent *component, const wxRegion &region, const wxIFMTabArray &tabs)
{
    wxIFMTab *tab, *activeTab = NULL;

    wxRect bg = component->GetClientRect();
    wxRect tabs_area;
    tabs_area.x = bg.x;
	
	if(m_tab_config.get_top()) //maks
	{
		tabs_area.y = bg.y - m_tab_config.m_row_height;
	}
	else
	{
		tabs_area.y = bg.y + bg.height + 1;// - 1;//tab->m_tab->m_parent->m_margins.bottom;
	}

    tabs_area.width = bg.width;
    tabs_area.height = m_tab_config.m_row_height;

    // paint tabs only if we need to
    wxRegionContain result = region.Contains(tabs_area);
    if( !(result == wxInRegion || result == wxPartRegion) )
        return;
	
	if(!m_tab_config.get_top()) //maks
	{
		// background for tabs area
		dc.SetPen(m_tab_config.m_bg_pen);
		dc.SetBrush(m_tab_config.m_bg_brush);
		dc.DrawRectangle(tabs_area);
		
		// top line
		dc.SetPen(m_tab_config.m_line_pen);
		dc.DrawLine(tabs_area.x-1, tabs_area.y, tabs_area.x + tabs_area.width+1, tabs_area.y);
	}

    wxIFMPanelTabData *tab_data;
    wxRect rect;

	
	if(m_tab_config.m_rounding_factor < 0) //maks: Use LightWave tabs
	{
		//for( wxIFMTabArray::const_iterator i = tabs.begin(), end = tabs.end(); i != end; ++i )
		for( size_t i = 0; i < tabs.GetCount(); i++ )
		{
			//tab = *i;
			tab = tabs[i];
			
			if( !tab->m_visible )
				continue;
			
			// don't paint the tab if its not in the update region
			result = region.Contains(tab->m_rect);
			if( !(result == wxInRegion || result == wxPartRegion) )
				continue;
			
			if(!tab->m_active)
			{
				DrawOneLightWaveTab(dc, tab);
			}
			else
			{
				activeTab = tab;
			}
				        
		}

		//At last, draw the active tab
		if(activeTab)
		{
			DrawOneLightWaveTab(dc, activeTab);
		}
	}
	else
	{
		//for( wxIFMTabArray::const_iterator i = tabs.begin(), end = tabs.end(); i != end; ++i )
		for( size_t i = 0; i < tabs.GetCount(); i++ )
		{
			//tab = *i;
			tab = tabs[i];
			
			if( !tab->m_visible )
				continue;
			
			// don't paint the tab if its not in the update region
			result = region.Contains(tab->m_rect);
			if( !(result == wxInRegion || result == wxPartRegion) )
				continue;
			
			rect = tab->m_rect;	
			
			
			
			// draw the tab background
			// if the tab is not selected draw a black line above
			if( tab->m_active )
			{
				dc.SetBrush(m_tab_config.m_tab_active_bg_b);
				dc.SetPen(m_tab_config.m_tab_border_p);
				dc.DrawRoundedRectangle(rect.x, rect.y, rect.width, rect.height, m_tab_config.m_rounding_factor);
				
				dc.SetPen(m_tab_config.m_tab_active_bg_b.GetColour());//maks (m_tab_config.m_tab_active_bg_p);
				dc.DrawRectangle(rect.x, rect.y, rect.width, m_tab_config.m_rounding_factor);
				
				dc.SetPen(m_tab_config.m_tab_border_p);
				dc.DrawLine(rect.x, rect.y, rect.x, rect.y + m_tab_config.m_rounding_factor);
				dc.DrawLine(rect.x + rect.width - 1, rect.y, rect.x + rect.width - 1, rect.y + m_tab_config.m_rounding_factor);
			}
			else
			{
				dc.SetBrush(m_tab_config.m_tab_inactive_bg_b);
				dc.SetPen(m_tab_config.m_tab_border_p);
				dc.DrawRoundedRectangle(rect.x, rect.y, rect.width, rect.height, m_tab_config.m_rounding_factor);
				
				dc.SetPen(m_tab_config.m_tab_inactive_bg_p);
				dc.DrawRectangle(rect.x, rect.y, rect.width, m_tab_config.m_rounding_factor);
				
				dc.SetPen(m_tab_config.m_tab_border_p);
				dc.DrawLine(rect.x, rect.y, rect.x, rect.y + m_tab_config.m_rounding_factor);
				dc.DrawLine(rect.GetRight(), rect.y, rect.GetRight(), rect.y + m_tab_config.m_rounding_factor);
				
				dc.SetPen(m_tab_config.m_line_pen);
				dc.DrawLine(rect.x, rect.y, rect.x+rect.width, rect.y);
			}
			
			tab_data = IFM_GET_EXTENSION_DATA(tab->m_tab, wxIFMPanelTabData);
			
			// draw bitmap
			if(tab_data && tab_data->m_bitmap.Ok())
			{
				wxBitmap &bmp = tab_data->m_bitmap;
				dc.DrawBitmap(bmp, rect.x + m_tab_config.m_tab_padding, rect.y + (rect.height - bmp.GetHeight())/2, true);
				rect.x += m_tab_config.m_tab_padding + bmp.GetWidth();
			}
			
			// draw the label
			dc.SetFont(m_tab_config.m_font);
			dc.SetTextForeground(m_tab_config.m_font_color);
			
			dc.DrawText(tab->m_tab->m_name, rect.x + m_tab_config.m_tab_padding, rect.y + (rect.height - m_tab_config.m_font_height) / 2);			
			
		}
	}    
}

wxBitmap CreateGradientBitmap(int width, int height, wxColour color1, wxColour color2); //maks

class wxGradientData //maks
{
public:
	wxGradientData() {}
	wxGradientData(wxColour _color1, wxColour _color2, bool _bHorizontal)
	{
		color1 = _color1;
		color2 = _color2;
		bHorizontal = _bHorizontal;
	}

	wxGradientData &operator=(const wxGradientData &other)
    {
		color1 = other.color1;
		color2 = other.color2;
		bHorizontal = other.bHorizontal;

        return *this;
    }

	wxColour color1;
	wxColour color2;
	bool bHorizontal;
};

// hashing function
class GradientHash
{
public:
	GradientHash() { }
	
	unsigned long operator()( const wxGradientData& k ) const
	{ 
		/* compute the hash */ 
		return k.color1.GetPixel() ^ k.color2.GetPixel();
	}
	
	GradientHash& operator=(const GradientHash&) 
	{ 
		return *this; 
	}
};

// comparison operator
class GradientKeyEqual
{
public:
	GradientKeyEqual() { }
	bool operator()( const wxGradientData& a, const wxGradientData& b ) const
	{ 
		if( a.color1 == b.color1 && 
			a.color2 == b.color2 && 
			a.bHorizontal == b.bHorizontal )
		{
			return true;
		}

		return false;
	}
	
	GradientKeyEqual& operator=(const GradientKeyEqual&) 
	{ 
		return *this; 
	}
};


WX_DECLARE_HASH_MAP(wxGradientData, wxBitmap, GradientHash, GradientKeyEqual, wxGradientMap); //maks
wxGradientMap gradientCache; //maks


void DrawHorizontalGradientLine(wxDC &dc, int x, int y, int width, wxColour color1, wxColour color2)
{
	// calculate gradient coefficients
	double	rstep = double((color2.Red() -   color1.Red())) / double(width), rf = 0,
		gstep = double((color2.Green() - color1.Green())) / double(width), gf = 0,
		bstep = double((color2.Blue() -  color1.Blue())) / double(width), bf = 0;
	
	wxColour currCol;
	wxPen pen;

	for(int i = x; i < x + width; i++)
	{
		currCol.Set(
			(unsigned char)(color1.Red() + rf),
			(unsigned char)(color1.Green() + gf),
			(unsigned char)(color1.Blue() + bf)
			);
		
		if(pen.GetColour() != currCol) 
		{
			pen.SetColour(currCol);
			dc.SetPen(pen);
		}				
		
		dc.DrawPoint(i, y);
		
		rf += rstep; gf += gstep; bf += bstep;
	}
}

void DrawVerticalGradientLine(wxDC &dc, int x, int y, int height, wxColour color1, wxColour color2)
{
	// calculate gradient coefficients
	double	rstep = double((color2.Red() -   color1.Red())) / double(height), rf = 0,
		gstep = double((color2.Green() - color1.Green())) / double(height), gf = 0,
		bstep = double((color2.Blue() -  color1.Blue())) / double(height), bf = 0;
	
	wxColour currCol;
	wxPen pen;

	for(int i = y; i < y + height; i++)
	{
		currCol.Set(
			(unsigned char)(color1.Red() + rf),
			(unsigned char)(color1.Green() + gf),
			(unsigned char)(color1.Blue() + bf)
			);
		
		if(pen.GetColour() != currCol) 
		{
			pen.SetColour(currCol);
			dc.SetPen(pen);
		}
				
		dc.DrawPoint(x, i);
		
		rf += rstep; gf += gstep; bf += bstep;
	}
}

void DrawVerticalGradient(wxDC &dc, const wxRect &rect, wxColour color1, wxColour color2)
{
	// gradient fill from colour 1 to colour 2 with top to bottom

	if(rect.height < 1 || rect.width < 1)
		return;

	int size = rect.height;

	dc.SetPen(*wxTRANSPARENT_PEN);

	if (color1 == color2)
	{
		dc.SetBrush(wxBrush(color1, wxSOLID));
		dc.DrawRectangle(rect);
		return;
	}


	// calculate gradient coefficients
	double rstep = double((color2.Red() -   color1.Red())) / double(size), rf = 0,
	gstep = double((color2.Green() - color1.Green())) / double(size), gf = 0,
	bstep = double((color2.Blue() -  color1.Blue())) / double(size), bf = 0;

	wxColour currCol;
	wxBrush brush;
	brush.SetStyle(wxSOLID); //maks

	for(int y = rect.y; y < rect.y + size; y++)
	{
		currCol.Set(
		(unsigned char)(color1.Red() + rf),
		(unsigned char)(color1.Green() + gf),
		(unsigned char)(color1.Blue() + bf)
		);

		if(brush.GetColour() != currCol) //maks
		{
			brush.SetColour(currCol);
			dc.SetBrush( brush );
		}

		dc.DrawRectangle( rect.x, rect.y + (y - rect.y), rect.width, size );
		//currCol.Set(currCol.Red() + rstep, currCol.Green() + gstep, currCol.Blue() + bstep);
		rf += rstep; gf += gstep; bf += bstep;
	}
}

void DrawHorizontalGradient(wxDC &dc, const wxRect &rect, wxColour color1, wxColour color2)
{
	// gradient fill from colour 1 to colour 2 with left to right

	if(rect.height < 1 || rect.width < 1)
		return;

	int size = rect.width;

	dc.SetPen(*wxTRANSPARENT_PEN);

	if (color1 == color2)
	{
		dc.SetBrush(wxBrush(color1, wxSOLID));
		dc.DrawRectangle(rect);
		return;
	}

	// calculate gradient coefficients
	double rstep = double((color2.Red() -   color1.Red())) / double(size), rf = 0,
	gstep = double((color2.Green() - color1.Green())) / double(size), gf = 0,
	bstep = double((color2.Blue() -  color1.Blue())) / double(size), bf = 0;

	wxColour currCol;
	wxBrush brush;
	brush.SetStyle(wxSOLID); //maks

	for(int x = rect.x; x < rect.x + size; x++)
	{
		currCol.Set(
		(unsigned char)(color1.Red() + rf),
		(unsigned char)(color1.Green() + gf),
		(unsigned char)(color1.Blue() + bf)
		);
	    

		if(brush.GetColour() != currCol) //maks
		{
			brush.SetColour(currCol);
			dc.SetBrush( brush );
		}

	    dc.DrawRectangle( rect.x + (x - rect.x), rect.y, 1, rect.height );
	    rf += rstep; gf += gstep; bf += bstep;
    }
}

/*wxBitmap CreateGradientBitmap(int width, int height, wxColour color1, wxColour color2) //maks
{
    wxMemoryDC memDC;
    wxBitmap bitmap(width, height);

    if(width > height)
	{
		//Horizontal
		if(height > 1)
		{
			DrawHorizontalGradient1(memDC, wxRect(0, 0, width, height), color1, color2);
		}
		else
		{
			DrawHorizontalGradientLine1(memDC, 0, 0, width, color1, color2);
		}
	}
	else
	{
		//Vertical
		if(width > 1)
		{
			DrawVerticalGradient1(memDC, wxRect(0, 0, width, height), color1, color2);
		}
		else
		{
			DrawVerticalGradientLine1(memDC, 0, 0, height, color1, color2);
		}
	}

    return bitmap;
}

void DrawHorizontalGradient(wxDC &dc, const wxRect &rect, wxColour color1, wxColour color2) //maks
{
	wxGradientData data(color1, color2, true);

	if(gradientCache.find(data) == gradientCache.end())
	{
		gradientCache[data] = CreateGradientBitmap(rect.width, rect.height, color1, color2);
	}

	dc.Blit((rect.GetPosition(), rect.
}*/

void wxIFMDefaultPanelPlugin::DrawCaption(wxDC &dc, const wxRect &rect, const wxString &caption, int orientation, bool bDrawText) 
{
    switch(m_caption_config.m_style & IFM_CAPTION_MASK)
    {
    case IFM_CAPTION_GRADIENT:
        switch (orientation)
        {
        default:
        case IFM_ORIENTATION_DEFAULT:
        case IFM_ORIENTATION_NONE:
        case IFM_ORIENTATION_FLOAT:
        case IFM_ORIENTATION_DOCKED:
            wxASSERT(false);
            break;
        case IFM_ORIENTATION_TOP:
        case IFM_ORIENTATION_BOTTOM:
            DrawHorizontalGradient(dc, rect, m_caption_config.m_caption_color, m_caption_config.m_gradient_color);
            break;
        case IFM_ORIENTATION_LEFT:
            DrawVerticalGradient(dc, rect, m_caption_config.m_gradient_color, m_caption_config.m_caption_color);
            break;
        case IFM_ORIENTATION_RIGHT:
            DrawVerticalGradient(dc, rect, m_caption_config.m_caption_color, m_caption_config.m_gradient_color);
            break;
        }
        break;

    case IFM_CAPTION_SOLID:
    default:
        dc.SetPen(m_caption_config.m_caption_color);
        dc.SetBrush(m_caption_config.m_caption_color);

        dc.DrawRectangle(rect);
    }

    // draw caption text
    dc.SetFont(m_caption_config.m_font);
    dc.SetTextForeground(m_caption_config.m_font_color);

    int x, y;

    switch(m_caption_config.m_style & IFM_CAPTION_TITLE_MASK)
    {
    case IFM_CAPTION_LEFT:
    default:
        x = rect.x + m_caption_config.m_padding;
        y = rect.y + (m_caption_config.m_size - m_caption_config.m_font_height) / 2;
        break;
    }

	if(bDrawText) //maks
	{
		dc.DrawText(caption, x, y);
	}
}

void wxIFMDefaultPanelPlugin::OnComponentButtonClick(wxIFMComponentButtonEvent &event)
{
    wxIFMComponent *component = event.GetComponent();

    wxASSERT_MSG(component, wxT("NULL component?"));
    if( !component )
        return;

    if( component->GetType() == IFM_COMPONENT_PANEL )
    {
        wxIFMComponentButton *button = event.GetButton();
        if( button->GetID() == IFM_COMPONENT_ID_CLOSE )
        {
            wxIFMPanelData *data = IFM_GET_EXTENSION_DATA(component, wxIFMPanelData);

            // hide the active tab
            data->m_active_tab->m_tab->Show(false, false);

            // select a new one by looking for any visible tab starting from the back
            // that isnt currently active
            for( int i = data->m_tabs.size() - 1; i >= 0; --i )
            {
                if( data->m_tabs[i]->m_visible && !data->m_tabs[i]->m_active )
                {
                    wxIFMSelectTabEvent event(component, data->m_tabs[i], false);
                    GetIP()->ProcessPluginEvent(event);
                    break;
                }
            }

            // update the interface
#if IFM_CANFLOAT
            wxIFMFloatingData *floating_data = IFM_GET_EXTENSION_DATA(component, wxIFMFloatingData);
            if( floating_data->m_floating )
                floating_data->m_window->Update();
            else
#endif
                GetIP()->GetManager()->Update(IFM_DEFAULT_RECT, true);

            return;
        }
        else if( button->GetID() == IFM_COMPONENT_ID_MINIMIZE )
        {

            //! \todo implement minimizing of components
            return;
        }
        else if( button->GetID() == IFM_COMPONENT_ID_MAXIMIZE )
        {

            //! \todo implement maximizing of components
            return;
        }
    }

    event.Skip();
}

void wxIFMDefaultPanelPlugin::OnLeftDown(wxIFMMouseEvent &event)
{
    wxMouseEvent &evt = event.GetMouseEvent();
    wxPoint pos = evt.GetPosition();
    if( event.GetComponentType() == IFM_COMPONENT_PANEL )
    {
        wxIFMComponent *component = event.GetComponent();

        wxASSERT_MSG(component, wxT("NULL component?"));
        if( !component )
            return;

        wxIFMPanelData *data = IFM_GET_EXTENSION_DATA(component, wxIFMPanelData);

        wxASSERT_MSG(data, wxT("Panel with no panel data?"));
        if( !data )
            return;

		bool bTabSelected = false; //maks

        // see if the user clicked on a tab and select it
        for( size_t i = 0; i < data->m_tabs.GetCount(); i++ )
        //for( wxIFMTabArray::const_iterator i = data->m_tabs.begin(), end = data->m_tabs.end(); i != end; ++i )
        {
            wxIFMTab *tab = data->m_tabs[i];
            if( tab->m_visible && tab->m_rect.Inside(event.GetMouseEvent().GetPosition()) )
            {
                wxIFMSelectTabEvent evt(component, tab, true);
                GetIP()->ProcessPluginEvent(evt);
				bTabSelected = true; //maks
                break;
            }
        }

        // now check for drag initialization hot spots which are the caption and any tab
        if(!bTabSelected && data->m_caption.Inside(pos) ) //maks
        {
            wxIFMInitDragEvent evt(component, component, pos);
            GetIP()->ProcessPluginEvent(evt);
        }
        else
        {
            // check for individual tab hits
            for( size_t i = 0; i < data->m_tabs.GetCount(); i++ )
            //for(wxIFMTabArray::iterator i = data->m_tabs.begin(), end = data->m_tabs.end(); i != end; ++i )
            {
                wxIFMTab *tab = data->m_tabs[i];
                if( tab->m_visible && tab->m_rect.Inside(pos) )
                {
                    // if we only have one tab drag the panel instead of the tab
                    wxIFMComponent *drag;
                    if( data->m_tabs.size() == 1 )
                        drag = component;
                    else
                        drag = tab->m_tab;

                    wxIFMInitDragEvent evt(component, drag, pos);
                    GetIP()->ProcessPluginEvent(evt);
                    break;
                }
            }
        }
    }
    else
        event.Skip();
}

void wxIFMDefaultPanelPlugin::OnSelectTab(wxIFMSelectTabEvent &event)
{
    if( event.GetComponentType() == IFM_COMPONENT_PANEL )
    {
        wxIFMComponent *panel = event.GetComponent();

        wxASSERT_MSG(panel, wxT("NULL component?"));
        if( !panel )
            return;

        wxIFMPanelData *data = IFM_GET_EXTENSION_DATA(panel, wxIFMPanelData);

        wxASSERT_MSG(data, wxT("Tabbed panel with no tabbed panel data?"));
        if( !data )
            return;

        wxIFMTab *tab = event.GetTab();

        // don't active the current tab over again
        if( tab == data->m_active_tab )
            return;

        // record the new active tab
        data->m_active_tab = tab;

        // update the new tab
        if( event.GetUpdate() )
        {
            wxIFMUpdateComponentEvent evt(tab->m_tab, panel->GetClientRect());
            GetIP()->ProcessPluginEvent(evt);
        }

        // show and activate the new tab while hiding the others
        for( size_t i = 0; i < data->m_tabs.GetCount(); i++ )
        //for( wxIFMTabArray::iterator i = data->m_tabs.begin(), end = data->m_tabs.end(); i != end; ++i )
        {
            data->m_dont_hide_tab = true;
            data->m_tabs[i]->m_tab->Show(data->m_tabs[i] == tab, false);
            data->m_tabs[i]->m_active = data->m_tabs[i] == tab;
        }

        if(event.GetUpdate()) {
            wxIFMUpdateComponentEvent evt(panel, panel->GetRect());
            GetIP()->ProcessPluginEvent(evt);
        }
    }
    else
        event.Skip();
}

void wxIFMDefaultPanelPlugin::OnGetDesiredSize(wxIFMRectEvent &event)
{
    /*
    if( event.GetComponentType() == IFM_COMPONENT_PANEL )
    {
        wxIFMComponent *panel = event.GetComponent();

        wxASSERT_MSG(panel, wxT("NULL component?"));
        if( !panel )
            return;

        wxIFMPanelData *data = IFM_GET_EXTENSION_DATA(panel, wxIFMPanelData);

        wxASSERT_MSG(data, wxT("Tabbed panel with no tabbed panel data?"));
        if( !data )
            return;

        // use the active tabs desired size
        event.SetRect(panel->GetConvertedRect(wxRect(wxPoint(), data->m_active_tab->GetDesiredSize()), IFM_COORDS_CLIENT, IFM_COORDS_ABSOLUTE));
    }
    else
    */
        event.Skip();
}

void wxIFMDefaultPanelPlugin::OnSetDesiredSize(wxIFMRectEvent &event)
{
    if( event.GetComponentType() == IFM_COMPONENT_PANEL )
    {
        wxIFMComponent *panel = event.GetComponent();

        wxASSERT_MSG(panel, wxT("NULL component?"));
        if( !panel )
            return;

        wxIFMPanelData *data = IFM_GET_EXTENSION_DATA(panel, wxIFMPanelData);

        wxASSERT_MSG(data, wxT("Tabbed panel with no tabbed panel data?"));
        if( !data )
            return;

        // set the size for all tabs
        wxSize size = panel->GetConvertedRect(event.GetRect(), IFM_COORDS_ABSOLUTE, IFM_COORDS_CLIENT).GetSize();

        for( size_t i = 0; i < data->m_tabs.GetCount(); i++ )
        //for( wxIFMTabArray::iterator i = data->m_tabs.begin(), end = data->m_tabs.end(); i != end; ++i )
            data->m_tabs[i]->m_tab->SetDesiredSize(size);

        // skip to set our own desired size too
    }

    event.Skip();
}

void wxIFMDefaultPanelPlugin::OnGetMinSize(wxIFMRectEvent &event)
{
    if( event.GetComponentType() == IFM_COMPONENT_PANEL )
    {
        wxIFMComponent *panel = event.GetComponent();

        wxASSERT_MSG(panel, wxT("NULL component?"));
        if( !panel )
            return;

        wxIFMPanelData *data = IFM_GET_EXTENSION_DATA(panel, wxIFMPanelData);

        wxASSERT_MSG(data, wxT("Tabbed panel with no tabbed panel data?"));
        if( !data )
            return;

        // find the largest min size of our tabs and use that
        wxSize final_min = panel->m_minSize;

        for( size_t i = 0; i < panel->m_children.GetCount(); i++ )
        //for( wxIFMComponentArray::iterator i = panel->m_children.begin(), end = panel->m_children.end(); i != end; ++i )
        {
            wxSize min = panel->m_children[i]->GetMinSize();
            if( min.x > final_min.x )
                final_min.x = min.x;
            if( min.y > final_min.y )
                final_min.y = min.y;
        }

        wxSize converted_min = panel->GetConvertedRect(wxRect(wxPoint(),final_min), IFM_COORDS_CLIENT, IFM_COORDS_ABSOLUTE).GetSize();
        if( final_min.x != IFM_NO_MINIMUM )
            final_min.x = converted_min.x;
        if( final_min.y != IFM_NO_MINIMUM )
            final_min.y = converted_min.y;

        event.SetSize(final_min);
    }
    else
        event.Skip();
}

void wxIFMDefaultPanelPlugin::OnGetMaxSize(wxIFMRectEvent &event) //maks
{
    if( event.GetComponentType() == IFM_COMPONENT_PANEL )
    {
        wxIFMComponent *panel = event.GetComponent();

        wxASSERT_MSG(panel, wxT("NULL component?"));
        if( !panel )
            return;

        wxIFMPanelData *data = IFM_GET_EXTENSION_DATA(panel, wxIFMPanelData);

        wxASSERT_MSG(data, wxT("Tabbed panel with no tabbed panel data?"));
        if( !data )
            return;

        // find the min max size of our tabs and use that
        wxSize final_max = panel->m_maxSize;
		wxIFMComponent *child;

		if(final_max.x == IFM_NO_MAXIMUM) final_max.x = 1000000;
		if(final_max.y == IFM_NO_MAXIMUM) final_max.y = 1000000;

        for( size_t i = 0; i < panel->m_children.GetCount(); i++ )        
        {
			child = panel->m_children[i];
            wxSize max;
			
			if(child->m_children.size())
			{
				max = child->GetMaxSize();
			}
			else
			{
				max = child->m_maxSize;
			}

            if( max.x > 0 && max.x < final_max.x )
                final_max.x = max.x;

            if( max.y > 0 && max.y < final_max.y )
                final_max.y = max.y;
        }

        wxSize converted_max = panel->GetConvertedRect(wxRect(wxPoint(), final_max), IFM_COORDS_CLIENT, IFM_COORDS_ABSOLUTE).GetSize();

        if( final_max.x != IFM_NO_MAXIMUM )
            final_max.x = converted_max.x;

        if( final_max.y != IFM_NO_MAXIMUM )
            final_max.y = converted_max.y;

        event.SetSize(final_max);
    }
    else
        event.Skip();
}

/*
wxIFMComponentButtonManager implementation
*/
wxIFMComponentButtonManager::wxIFMComponentButtonManager(wxIFMInterfacePluginBase *ip, wxIFMComponent *component)
    : m_height(0),
    m_ip(ip),
    m_component(component),
    m_hidden(false)
{ }

wxIFMComponentButtonManager::~wxIFMComponentButtonManager() //maks
{
	int i, count;

	// delete buttons
    for(i = 0, count = m_buttons.GetCount(); i < count; ++i)
	{
		//maks
		wxIFMComponentButton *btn = m_buttons[i];
		if(globalPointerMap.find(btn) != globalPointerMap.end())
		{
			delete btn;
		}
	}

	m_buttons.clear();
}

wxIFMInterfacePluginBase *wxIFMComponentButtonManager::GetIP()
{
    return m_ip;
}

wxIFMComponent *wxIFMComponentButtonManager::GetComponent()
{
    return m_component;
}

const wxPoint &wxIFMComponentButtonManager::GetPosition() const
{
    return m_pos;
}

void wxIFMComponentButtonManager::Hide()
{
    Show(false);
}

wxIFMComponentButton *wxIFMComponentButtonManager::AddButton(int id)
{
    wxIFMComponentButton *btn = new wxIFMComponentButton(this, id, m_component->GetParentWindow());

    // size the button
    btn->SetSize(IFM_COMPONENT_BUTTON_X, IFM_COMPONENT_BUTTON_Y);

    // store the button
    m_buttons.push_back(btn);

    return btn;
}

void wxIFMComponentButtonManager::RemoveButton(int id)
{
    for( int i = 0, count = m_buttons.GetCount(); i < count; ++i )
    //for( wxIFMComponentButtonArray::iterator i = m_buttons.begin(), end = m_buttons.end(); i != end; ++i )
    {
        if( m_buttons[i]->GetID() == id )
        //if( (*i)->GetID() == id )
        {
            wxIFMComponentButton *btn = m_buttons[i];
            m_buttons.RemoveAt(i);
            //delete *i;
            //m_buttons.erase(i);
            delete btn;
            return;
        }
    }

    wxFAIL_MSG(wxT("Removing a button not managed by this button manager"));
}

wxIFMComponentButton *wxIFMComponentButtonManager::GetButton(int id)
{
    for( int i = 0, count = m_buttons.GetCount(); i < count; ++i )
    //for( wxIFMComponentButtonArray::const_iterator i = m_buttons.begin(), end = m_buttons.end(); i != end; ++i )
    {
        if( m_buttons[i]->GetID() == id )
        //if( (*i)->GetID() == id )
            return m_buttons[i];
            //return (*i);
    }

    return NULL;
}

const wxRect wxIFMComponentButtonManager::GetRect() const
{
    wxRect rect;
    rect.x = m_pos.x;
    rect.y = m_pos.y;
    rect.height = m_height;

    for( int i = 0, count = m_buttons.GetCount(); i < count; ++i )
    //for( wxIFMComponentButtonArray::const_iterator i = m_buttons.begin(), end = m_buttons.end(); i != end; ++i )
    {
        rect.width += m_buttons[i]->GetRect().width;
        //rect.width += (*i)->GetRect().width;
        rect.width += IFM_COMPONENT_BUTTON_PAD;
    }

    return rect;
}

void wxIFMComponentButtonManager::SetPosition(const wxPoint& pos)
{
    m_pos = pos;
}

void wxIFMComponentButtonManager::SetHeight(int height)
{
    m_height = height;
    for( int i = 0, count = m_buttons.GetCount(); i < count; ++i )
    //for( wxIFMComponentButtonArray::iterator i = m_buttons.begin(), end = m_buttons.end(); i != end; ++i )
        m_buttons[i]->SetSize(IFM_COMPONENT_BUTTON_X, IFM_COMPONENT_BUTTON_Y);
        //(*i)->SetSize(IFM_COMPONENT_BUTTON_X, IFM_COMPONENT_BUTTON_X);
}

void wxIFMComponentButtonManager::SetParent(wxWindow *parent)
{
    for( int i = 0, count = m_buttons.GetCount(); i < count; ++i )
    //for( wxIFMComponentButtonArray::iterator i = m_buttons.begin(), end = m_buttons.end(); i != end; ++i )
        m_buttons[i]->Reparent(parent);
        //(*i)->Reparent(parent);
}

void wxIFMComponentButtonManager::Layout()
{
    if( m_hidden )
        return;

    wxPoint pos = m_pos;
    pos.y += 2;

    for( int i = 0, count = m_buttons.GetCount(); i < count; ++i )
    //for( wxIFMComponentButtonArray::iterator i = m_buttons.begin(), end = m_buttons.end(); i != end; ++i )
    {
        m_buttons[i]->SetPosition(pos);
        //(*i)->SetPosition(pos);
        pos.x += (IFM_COMPONENT_BUTTON_X + IFM_COMPONENT_BUTTON_PAD);
    }
}

void wxIFMComponentButtonManager::Show(bool show)
{
    m_hidden = !show;

    for( int i = 0, count = m_buttons.GetCount(); i < count; ++i )
    //for( wxIFMComponentButtonArray::iterator i = m_buttons.begin(), end = m_buttons.end(); i != end; ++i )
        m_buttons[i]->Show(show);
        //(*i)->Show(show);
}

void wxIFMComponentButtonManager::Show(bool show, int id)
{
    wxIFMComponentButton *btn = GetButton(id);

    wxASSERT_MSG(btn, wxT("invalid button id"));
    if( btn )
        btn->Show(show);
}

BEGIN_EVENT_TABLE(wxIFMComponentButton, wxWindow)
    EVT_ERASE_BACKGROUND (wxIFMComponentButton::OnEraseBg)
    EVT_PAINT           (wxIFMComponentButton::OnPaint)
    EVT_ENTER_WINDOW    (wxIFMComponentButton::OnEnterWindow)
    EVT_LEAVE_WINDOW    (wxIFMComponentButton::OnLeaveWindow)
    EVT_LEFT_DOWN       (wxIFMComponentButton::OnLeftDown)
    EVT_LEFT_UP         (wxIFMComponentButton::OnLeftUp)
    EVT_MOTION          (wxIFMComponentButton::OnMouseMove)
END_EVENT_TABLE()


wxIFMComponentButton::wxIFMComponentButton(wxIFMComponentButtonManager *manager, int id, wxWindow *parent)
    : wxWindow(parent, wxID_ANY, wxPoint(0,0), wxSize(0,0), wxNO_BORDER),
    m_id(id),
    m_manager(manager),
    m_hover(false),
    m_pressed(false),
    m_leftDown(false),
    m_bmp(NULL),
    m_bmpH(NULL),
    m_bmpP(NULL)
{ 
	globalPointerMap[this] = 1;	//maks
}

wxIFMComponentButton::~wxIFMComponentButton() 
{
	globalPointerMap.erase(this);	//maks	
}

wxIFMInterfacePluginBase *wxIFMComponentButton::GetIP()
{
    return m_manager->GetIP();
}

int wxIFMComponentButton::GetID() const
{
    return m_id;
}

bool wxIFMComponentButton::GetHover() const
{
    return m_hover;
}

bool wxIFMComponentButton::GetPressed() const
{
    return m_pressed;
}

void wxIFMComponentButton::SetHover(bool h)
{
    if( h != m_hover )
    {
        m_hover = h;
        Refresh();
    }
}

void wxIFMComponentButton::SetPressed(bool p)
{
    if( p != m_pressed )
    {
        m_pressed = p;
        Refresh();
    }
}

void wxIFMComponentButton::OnEnterWindow(wxMouseEvent &WXUNUSED(event))
{
    SetHover(true);
}

void wxIFMComponentButton::OnLeaveWindow(wxMouseEvent &WXUNUSED(event))
{
    SetHover(false);
}

void wxIFMComponentButton::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    wxPaintDC dc(this);
    int w, h;

    GetClientSize(&w, &h);
    dc.SetBrush(GetBackgroundColour());
    dc.SetPen(GetBackgroundColour());
    dc.DrawRectangle(0, 0, w, h);

    if( m_hover )
    {
        if( m_pressed )
            dc.DrawBitmap(*m_bmpP, 0, 0, true);
        else
            dc.DrawBitmap(*m_bmpH, 0, 0, true);
    }
    else
        dc.DrawBitmap(*m_bmp, 0, 0, true);
}

void wxIFMComponentButton::OnEraseBg(wxEraseEvent &WXUNUSED(event))
{

}

void wxIFMComponentButton::OnLeftDown(wxMouseEvent &WXUNUSED(event))
{
    m_leftDown = true;
    CaptureMouse();

    SetPressed(true);
}

void wxIFMComponentButton::OnLeftUp(wxMouseEvent &event)
{
    if( m_leftDown )
    {
        m_leftDown = false;
        if(HasCapture()) /*maks*/ReleaseMouse();

        SetPressed(false);

        wxRect rect = GetRect();
        rect.y = rect.x = 0;
        if( rect.Inside(event.GetPosition()) )
        {
            // send button press event
            wxIFMComponentButtonEvent evt(wxEVT_IFM_COMPONENTBUTTONCLICK, m_manager->GetComponent(), this);
            GetIP()->ProcessPluginEvent(evt);
        }
    }
}

void wxIFMComponentButton::OnMouseMove(wxMouseEvent &event)
{
    if( m_leftDown )
    {
        wxRect rect = GetRect();
        rect.y = rect.x = 0;
        SetPressed(rect.Inside(event.GetPosition()));
    }
}

/*
    wxIFMDefaultChildData implementation
*/
wxIFMDefaultChildData::wxIFMDefaultChildData()
    : wxIFMChildDataBase(),
    m_orientation(IFM_ORIENTATION_DEFAULT),
    m_index(IFM_DEFAULT_INDEX),
    m_newRow(false),
    m_tabify(false),
    m_bitmap(wxNullBitmap),
    m_pos(IFM_DEFAULT_POS)
{ }

// docked ctor
wxIFMDefaultChildData::wxIFMDefaultChildData(wxWindow *child, int type, const wxString &name,
        wxSize size, bool hidden, int orientation, int index, bool newRow, bool tabify, const wxBitmap &bmp)
    : wxIFMChildDataBase(child, type, name, size, hidden),
    m_orientation(orientation),
    m_index(index),
    m_newRow(newRow),
    m_tabify(tabify),
    m_bitmap(bmp),
    m_pos(IFM_DEFAULT_POS)
{ }

// floating ctor
wxIFMDefaultChildData::wxIFMDefaultChildData(wxWindow *child, int type, wxPoint pos, wxSize size,
        bool hidden, const wxString &name)
    : wxIFMChildDataBase(child, type, name, size, hidden),
    m_orientation(IFM_ORIENTATION_FLOAT),
    m_index(0),
    m_newRow(false),
    m_tabify(false),
    m_bitmap(wxNullBitmap),
    m_pos(pos)
{ }

wxIFMDefaultChildData::wxIFMDefaultChildData(const wxIFMDefaultChildData &data)
    : wxIFMChildDataBase(data),
    m_orientation(data.m_orientation),
    m_index(data.m_index),
    m_newRow(data.m_newRow),
    m_tabify(data.m_tabify),
    m_bitmap(data.m_bitmap),
    m_pos(data.m_pos)
{ }

wxIFMDefaultChildData::wxIFMDefaultChildData(const wxIFMChildDataBase &data)
    : wxIFMChildDataBase(data),
    m_orientation(IFM_ORIENTATION_DEFAULT),
    m_index(IFM_DEFAULT_INDEX),
    m_newRow(false),
    m_tabify(false),
    m_bitmap(wxNullBitmap),
    m_pos(IFM_DEFAULT_POS)
{ }

/*
    wxIFMContainerData implementation
*/
wxIFMContainerData::wxIFMContainerData()
    : wxIFMExtensionDataBase()/*,
    m_buttonManager(0,0)*/
{ }

wxIFMContainerData::wxIFMContainerData(wxIFMInterfacePluginBase *ip, wxIFMComponent *c)
    : wxIFMExtensionDataBase(),
    //m_buttonManager(ip, c),
    m_orientation(IFM_ORIENTATION_NONE)
{ }

wxIFMComponentDataKeyType wxIFMContainerData::GetDataKey() const
{
    return IFM_CONTAINER_DATA_KEY;
}

wxIFMComponentDataKeyType wxIFMContainerData::DataKey()
{
    return IFM_CONTAINER_DATA_KEY;
}

/*
    wxIFMPanelTabData implementation
*/
wxIFMPanelTabData::wxIFMPanelTabData()
    : wxIFMExtensionDataBase(),
    m_bitmap(wxNullBitmap)
{ }

wxIFMComponentDataKeyType wxIFMPanelTabData::GetDataKey() const
{
    return IFM_PANEL_DATA_KEY;
}


wxIFMComponentDataKeyType wxIFMPanelTabData::DataKey()
{
    return IFM_PANEL_DATA_KEY;
}

/*
    wxIFMPanelData implementation
*/

wxIFMPanelData::wxIFMPanelData(wxIFMInterfacePluginBase *ip, wxIFMComponent *c)
    : m_button_manager(ip, c),
    m_active_tab_index(-1),
    m_active_tab(NULL),
    m_dont_hide_tab(false)
{ 

}

wxIFMPanelData::~wxIFMPanelData()
{
    // clean up tabs
    for( size_t i = 0; i < m_tabs.GetCount(); i++ )
    //for( wxIFMTabArray::iterator i = m_tabs.begin(), end = m_tabs.end(); i != end; ++i )
        delete m_tabs[i];
}

wxIFMComponentDataKeyType wxIFMPanelData::GetDataKey() const
{
    return IFM_TABBED_PANEL_DATA_KEY;
}

wxIFMComponentDataKeyType wxIFMPanelData::DataKey()
{
    return IFM_TABBED_PANEL_DATA_KEY;
}


wxIFMTabConfig::wxIFMTabConfig()
: m_font_color(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT)),
m_indent(2),
m_tab_spacing(-1),
m_rounding_factor(3),
bTop(false) //maks
{

}

void wxIFMTabConfig::init()
{
    set_font(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));

	//maks: global color settings
    m_tab_border_p = wxPen(wxTab_border_p);//(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW));
    m_tab_active_bg_b = wxBrush(wxTab_active_bg_b);//(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
    m_tab_active_bg_p = wxPen(wxTab_active_bg_p);//(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
    m_tab_inactive_bg_b = wxBrush(wxTab_inactive_bg_b);//(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
    m_tab_inactive_bg_p = wxPen(wxTab_inactive_bg_p);//(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
    m_bg_brush = wxBrush(wxTab_bg_brush);//(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT));
    m_bg_pen = wxPen(wxTab_bg_pen);//(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT));
    m_line_pen = m_tab_border_p;
}

const wxFont &wxIFMTabConfig::get_font() const
{
    return m_font;
}

void wxIFMTabConfig::update_height() //maks
{
	if(m_rounding_factor < 0)
	{
		//Better look for lightwave tabs
		m_tab_height = m_font_height + 2;
	}
	else
	{
		m_tab_height = m_font_height + m_tab_padding * 2;
	}

    m_row_height = m_tab_height + 2;
}

void wxIFMTabConfig::set_font(const wxFont &font)
{
    m_font = font;

    wxMemoryDC dc;
    dc.SetFont(m_font);
    dc.GetTextExtent(wxT(" "), 0, &m_font_height, 0, 0);

    m_tab_padding = m_font_height / 4; //maks: change the tab text space

	update_height(); //maks
}

int wxIFMTabConfig::get_font_height() const
{
    return m_font_height;
}

const wxColour &wxIFMTabConfig::get_font_colour() const
{
    return m_font_color;
}

void wxIFMTabConfig::set_font_colour(const wxColour &colour)
{
    m_font_color = colour;
}

int wxIFMTabConfig::get_indent() const
{
    return m_indent;
}

void wxIFMTabConfig::set_indent(int indent)
{
    m_indent = indent;
}

int wxIFMTabConfig::get_rounding_factor() const
{
    return m_rounding_factor;
}

void wxIFMTabConfig::set_rounding_factor(int factor)
{
    m_rounding_factor = factor;

	update_height(); //maks
}

int wxIFMTabConfig::get_tab_spacing() const
{
    return m_tab_spacing;
}

void wxIFMTabConfig::set_tab_spacing(int spacing)
{
    m_tab_spacing = spacing;
}

wxIFMCaptionConfig::wxIFMCaptionConfig()
: m_font_color(wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT)),
m_caption_color(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION)),
m_gradient_color(wxSystemSettings::GetColour(wxSYS_COLOUR_GRADIENTACTIVECAPTION)),
m_style(IFM_CAPTION_SOLID | IFM_CAPTION_LEFT),
b3dBorder(false) //maks
{
}

void wxIFMCaptionConfig::init()
{
    set_font(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
}

int wxIFMCaptionConfig::get_caption_style() const
{
    return m_style;
}

void wxIFMCaptionConfig::set_caption_style(int style)
{
    m_style = style;
}

const wxFont &wxIFMCaptionConfig::get_font() const
{
    return m_font;
}

void wxIFMCaptionConfig::set_font(const wxFont &font)
{
    m_font = font;

    wxMemoryDC dc;
    dc.SetFont(m_font);
    dc.GetTextExtent(wxT(" "), 0, &m_font_height, 0, 0);

    m_padding = m_font_height / 4;
    m_size = m_font_height + m_padding;
}

const wxColour &wxIFMCaptionConfig::get_font_colour() const
{
    return m_font_color;
}

void wxIFMCaptionConfig::set_font_colour(const wxColour &colour)
{
    m_font_color = colour;
}

const wxColour &wxIFMCaptionConfig::get_caption_color() const
{
    return m_caption_color;
}

void wxIFMCaptionConfig::set_caption_color(const wxColour &colour)
{
    m_caption_color = colour;
}

const wxColour &wxIFMCaptionConfig::get_caption_gradient_color() const
{
    return m_gradient_color;
}

void wxIFMCaptionConfig::set_caption_gradient_color(const wxColour &colour)
{
    m_gradient_color = colour;
}
