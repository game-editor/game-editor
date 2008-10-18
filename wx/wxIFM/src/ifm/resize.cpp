/*!
    Implementation file for the resize plugin

    \file   resize.cpp
    \author Robin McNeill
    \date   Created: 01/02/05

    Copyright (c) Robin McNeill
    Licensed under the terms of the wxWindows license
*/

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/msgdlg.h"
#include "wx/settings.h"

#include "wx/ifm/resize.h"

DEFINE_IFM_DATA_KEY(IFM_RESIZE_DATA_KEY)

DEFINE_EVENT_TYPE(wxEVT_IFM_BEGINRESIZE)
DEFINE_EVENT_TYPE(wxEVT_IFM_ENDRESIZE)
DEFINE_EVENT_TYPE(wxEVT_IFM_RESIZE)
DEFINE_EVENT_TYPE(wxEVT_IFM_RESIZESASHDBLCLICK)
DEFINE_EVENT_TYPE(wxEVT_IFM_GETRESIZESASHRECT)
DEFINE_EVENT_TYPE(wxEVT_IFM_UPDATERESIZESASHES)

IMPLEMENT_DYNAMIC_CLASS(wxIFMResizeData, wxIFMChildDataBase);
IMPLEMENT_DYNAMIC_CLASS(wxIFMDefaultResizePlugin, wxIFMExtensionPluginBase);

BEGIN_EVENT_TABLE(wxIFMDefaultResizePlugin, wxEvtHandler)
    EVT_IFM_NEWCOMPONENT    (wxIFMDefaultResizePlugin::OnCreateComponent)
    EVT_IFM_DOCK            (wxIFMDefaultResizePlugin::OnDock)
    EVT_IFM_UNDOCK          (wxIFMDefaultResizePlugin::OnUndock)
    EVT_IFM_CONVERTRECT     (wxIFMDefaultResizePlugin::OnConvertRect)
    EVT_IFM_SHOWCOMPONENT   (wxIFMDefaultResizePlugin::OnShowComponent)
    EVT_IFM_PAINTDECOR      (wxIFMDefaultResizePlugin::OnPaintDecor)
    EVT_IFM_GETRESIZESASHRECT(wxIFMDefaultResizePlugin::OnGetRect)
    EVT_IFM_SETRECT         (wxIFMDefaultResizePlugin::OnSetRect)
    EVT_IFM_SETCURSOR       (wxIFMDefaultResizePlugin::OnSetCursor)
    EVT_IFM_UPDATE          (wxIFMDefaultResizePlugin::OnUpdate)

#if IFM_CANFLOAT
    EVT_IFM_FLOAT           (wxIFMDefaultResizePlugin::OnFloat)
#endif

    EVT_IFM_LEFTDOWN        (wxIFMDefaultResizePlugin::OnLeftDown)
    EVT_IFM_LEFTUP          (wxIFMDefaultResizePlugin::OnLeftUp)
    EVT_IFM_MOTION          (wxIFMDefaultResizePlugin::OnMouseMove)
    EVT_IFM_KEYDOWN         (wxIFMDefaultResizePlugin::OnKeyDown)

    EVT_IFM_ADDTOPCONTAINER (wxIFMDefaultResizePlugin::OnAddTopContainer)
    EVT_IFM_RESIZE_BEGIN    (wxIFMDefaultResizePlugin::OnResizeBegin)
    EVT_IFM_RESIZE_END      (wxIFMDefaultResizePlugin::OnResizeEnd)
    EVT_IFM_RESIZE_RESIZING (wxIFMDefaultResizePlugin::OnResizing)
    EVT_IFM_RESIZE_UPDATE_SASHES (wxIFMDefaultResizePlugin::OnUpdateResizeSashes)
END_EVENT_TABLE()

wxIFMDefaultResizePlugin::wxIFMDefaultResizePlugin()
    : wxIFMExtensionPluginBase(),
    m_ip(NULL),
    m_resizing(false),
    m_invalidated(false),
    m_oldPos(wxPoint(-1,-1))
{ }

bool wxIFMDefaultResizePlugin::Initialize(wxIFMInterfacePluginBase *plugin)
{
    wxIFMExtensionPluginBase::Initialize(plugin);

    m_ip = wxDynamicCast(plugin, wxIFMDefaultInterfacePlugin);

    if( !m_ip )
    {
        wxMessageBox(wxT("wxIFMDefaultResizePlugin only extends wxIFMDefaultInterfacePlugin"), wxT("Error"),
            wxICON_ERROR | wxOK);
        return false;
    }

    return true;
}

void wxIFMDefaultResizePlugin::OnCreateComponent(wxIFMNewComponentEvent &event)
{
    // let the component be created first
    GetNextHandler()->ProcessEvent(event);

    // add resize data to components
    wxIFMResizeData *data = new wxIFMResizeData;
    wxIFMComponent *component = event.GetComponent();

    wxASSERT_MSG(component, wxT("NULL component?"));
    if( !component )
        return;

    component->AddExtensionData(data);
}

void wxIFMDefaultResizePlugin::OnDock(wxIFMDockEvent &event)
{
    // let the component be docked first
    GetNextHandler()->ProcessEvent(event);

    wxIFMComponent *dest = event.GetDestination();
    wxASSERT_MSG(dest, wxT("NULL destination?"));
    if( !dest )
        return;

    wxIFMUpdateResizeSashEvent evt(dest);
    GetIP()->ProcessPluginEvent(evt);
}

void wxIFMDefaultResizePlugin::OnUndock(wxIFMUndockEvent &event)
{
    if( event.GetParentType() != IFM_COMPONENT_CONTAINER )
    {
        event.Skip();
        return;
    }

    wxIFMComponent *component = event.GetComponent();
    wxASSERT_MSG(component, wxT("NULL component?"));
    if( !component )
        return;

    // I need to determine before hand how many children will remain after the undock command is processed
    // If no children are left the container _may_ be destroyed and the parent pointer will point to a freed
    // object
    bool update = false;
    if( event.GetParent()->m_children.GetCount() > 1 )
        update = true;

    GetNextHandler()->ProcessEvent(event);

    if( update )
    {
        wxIFMUpdateResizeSashEvent evt(event.GetParent());
        GetIP()->ProcessPluginEvent(evt);
    }
}

void wxIFMDefaultResizePlugin::OnAddTopContainer(wxIFMAddTopContainerEvent &event)
{
    GetNextHandler()->ProcessEvent(event);

    wxIFMComponent *component = event.GetComponent();
    wxASSERT_MSG(component, wxT("NULL component?"));
    if( !component )
        return;

    wxIFMUpdateResizeSashEvent evt(component);
    GetIP()->ProcessPluginEvent(evt);
}

void wxIFMDefaultResizePlugin::OnShowComponent(wxIFMShowComponentEvent &event)
{
    // let the child be shown or hidden first
    GetNextHandler()->ProcessEvent(event);

    // only update the resize sashes if the event is supposed to update the interface
    if( event.GetUpdate() )
    {
        wxIFMComponent *component = event.GetComponent();
        wxASSERT_MSG(component, wxT("NULL component?"));
        if( !component )
            return;

        wxIFMComponent *parent = component->m_parent;

        if( parent ) // just to be sure
        {
            wxIFMUpdateResizeSashEvent evt(parent);
            GetIP()->ProcessPluginEvent(evt);
        }
    }
    else
        m_invalidated = true;
}

#if IFM_CANFLOAT
void wxIFMDefaultResizePlugin::OnFloat(wxIFMFloatEvent &event)
{
    GetNextHandler()->ProcessEvent(event);

    wxIFMComponent *component = event.GetComponent();

    wxASSERT_MSG(component, wxT("NULL component?"));
    if( !component )
        return;

    wxIFMResizeData *data = IFM_GET_EXTENSION_DATA(component, wxIFMResizeData);

    wxASSERT_MSG(data, wxT("no resize data?"));
    if( !data )
        return;

    // don't display resize sashes on the root component of a floating window
    if( !component->m_docked )
    {
        data->m_display = false;
    }

    wxIFMUpdateResizeSashEvent evt(component);
    GetIP()->ProcessPluginEvent(evt);
}
#endif

void wxIFMDefaultResizePlugin::OnUpdate(wxIFMUpdateEvent &event)
{
    // If components have been shown or hidden but not updated since
    // the last interface update, recalculate all resize sash states
    if( m_invalidated )
    {
        m_invalidated = false;

        const wxIFMComponentList &containers = m_ip->GetTopContainerList();
        for( unsigned int i = 0; i < containers.size(); ++i )
            UpdateContainerResizeSashes(containers[i]);
    }

    event.Skip();
}

void wxIFMDefaultResizePlugin::OnUpdateResizeSashes(wxIFMUpdateResizeSashEvent &event)
{
    wxIFMComponent *component = event.GetComponent();

    wxASSERT_MSG(component, wxT("NULL component?"));
    if( !component )
        return;

    // update the rootmost parent
    for( ;component->m_parent; component = component->m_parent );

    if( component->GetType() == IFM_COMPONENT_CONTAINER )
        UpdateContainerResizeSashes(component);
    else
        UpdateResizeSashes(component);
}

void wxIFMDefaultResizePlugin::UpdateContainerResizeSashes(wxIFMComponent *container)
{
    // give the container a resize sash
    wxIFMContainerData *contdata = IFM_GET_EXTENSION_DATA(container, wxIFMContainerData);
    wxIFMResizeData *contresizedata = IFM_GET_EXTENSION_DATA(container, wxIFMResizeData);

    // always give the sash a side
    switch(contdata->m_orientation)
    {
        case IFM_ORIENTATION_LEFT:
            contresizedata->m_side = IFM_ORIENTATION_RIGHT;
            break;
        case IFM_ORIENTATION_TOP:
            contresizedata->m_side = IFM_ORIENTATION_BOTTOM;
            break;
        case IFM_ORIENTATION_RIGHT:
            contresizedata->m_side = IFM_ORIENTATION_LEFT;
            break;
        case IFM_ORIENTATION_BOTTOM:
            contresizedata->m_side = IFM_ORIENTATION_TOP;
            break;
        //case IFM_ORIENTATION_DOCKED:
        default:
            // decide based on scale direction
            if( container->m_alignment == IFM_ALIGN_HORIZONTAL )
                contresizedata->m_side = IFM_ORIENTATION_BOTTOM;
            else if( container->m_alignment == IFM_ALIGN_VERTICAL )
                contresizedata->m_side = IFM_ORIENTATION_RIGHT;
    }

    // update children too
    UpdateResizeSashes(container);
}

void wxIFMDefaultResizePlugin::UpdateResizeSashes(wxIFMComponent *component)
{
    wxIFMResizeData *resizedata;

    //! \todo Allow for top / left resize sashes on children here
    // this would mean that the first visible child doesn't have a sash
    // as compared to the last visible child having no sash with right / bottom
    int next = 0, current = wxIFMComponent::GetNextVisibleComponent(component->m_children, 0);

    // if there are no visible children, do not display a resize sash on the container
    // never display a resize sash on a docked container
    resizedata = IFM_GET_EXTENSION_DATA(component, wxIFMResizeData);

#if IFM_CANFLOAT
    wxIFMFloatingData *floating_data = IFM_GET_EXTENSION_DATA(component, wxIFMFloatingData);
#endif

    if( !HasResizeableChildren(component)
#if IFM_CANFLOAT
        || (floating_data->m_floating && !component->m_docked)
#endif
      )
    {
        resizedata->m_display = false;
    }
    else
	{
		/////////////////////////////////////////////////////
		//maks: Remove unusable sahs
		if(component->GetType() == IFM_COMPONENT_CONTAINER)
		{
			if(/*component->m_children.size() == 1 &&*/ component->m_rect.GetWidth() > 0)
			{
				//Remove the sash above stage with no content window
				if(resizedata->m_rect.y == 0 && resizedata->m_side == IFM_ORIENTATION_TOP)
				{
					resizedata->m_display = false;
					return;
				}
				
				//Try found siblings sash
				/*const wxIFMComponentList &m_components = m_ip->GetTopContainerList();
				for(int i = 0; i < m_components.size(); ++i )
				{
					wxIFMComponent *other = m_components[i];
					if(other != component)
					{
						wxIFMResizeData *resizeOther = IFM_GET_EXTENSION_DATA(other, wxIFMResizeData);
						if(resizeOther)
						{
							if(resizeOther->m_side == IFM_ORIENTATION_BOTTOM && resizedata->m_side == IFM_ORIENTATION_TOP)
							{
								if(resizedata->m_rect.GetTop() == resizeOther->m_rect.GetBottom() + 1)
								{
									if(resizedata->m_rect.GetWidth() < resizeOther->m_rect.GetWidth())
									{
										resizedata->m_display = false;
										return;
									}
								}
							}
							else if(resizeOther->m_side == IFM_ORIENTATION_RIGHT && resizedata->m_side == IFM_ORIENTATION_LEFT)
							{
								if(resizedata->m_rect.GetLeft() == resizeOther->m_rect.GetRight() + 1)
								{
									if(resizedata->m_rect.GetHeight() < resizeOther->m_rect.GetHeight())
									{
										resizedata->m_display = false;
										return;
									}
								}
							}
						}
					}
				}*/
			}
		}
		/////////////////////////////////////////////////////

        resizedata->m_display = true;
	}

    // containers with no visible children don't get resize sashes
    if( component->GetType() == IFM_COMPONENT_CONTAINER )
    {
        if( component->GetNextVisibleComponent(component->m_children, 0) == -1 )
        {
            resizedata->m_display = false;
            return;
        }
    }

    // determine resize sash side
    int side = 0;
    if( component->m_alignment == IFM_ALIGN_VERTICAL )
        side = IFM_ORIENTATION_BOTTOM;
    else if( component->m_alignment == IFM_ALIGN_HORIZONTAL )
        side = IFM_ORIENTATION_RIGHT;

    // iterate over visible children
    while( current >= 0 )
    {
        wxIFMComponent *child = component->m_children[current];
        next = wxIFMComponent::GetNextVisibleComponent(component->m_children, current + 1);

        resizedata = IFM_GET_EXTENSION_DATA(child, wxIFMResizeData);
        resizedata->m_side = side;

        // update resize sashes for the child
        UpdateResizeSashes(child);

        // the last function set the visibility for the resize sash based on whether it has
        // resizeable children. Now we need to validate its choice taking into consideration
        // this childs position within its parent.
        if( next == -1 || !wxIFMDefaultContainerPlugin::HasVisibleChildren(component->m_children[next]))
            resizedata->m_display = false;

        current = next;
    }
}

void wxIFMDefaultResizePlugin::OnConvertRect(wxIFMConvertRectEvent &event)
{
    wxIFMComponent *component = event.GetComponent();
    wxIFMResizeData *data = IFM_GET_EXTENSION_DATA(component, wxIFMResizeData);

    // don't do anything if sash is hidden or if the component is hidden
    if( !data->m_display || component->m_hidden)
    {
        event.Skip();
        return;
    }

    wxRect rect = event.GetRect();

    // account for the resize sash size when converting to and from absolute and background coords
    if( event.GetSourceCoords() == IFM_COORDS_ABSOLUTE && event.GetDestinationCoords() == IFM_COORDS_BACKGROUND )
    {
        switch(data->m_side)
        {
            case IFM_ORIENTATION_LEFT:
                rect.x += IFM_RESIZE_SASH_SIZE;
                rect.width -= IFM_RESIZE_SASH_SIZE;
                break;

            case IFM_ORIENTATION_RIGHT:
                rect.width -= IFM_RESIZE_SASH_SIZE;
                break;

            case IFM_ORIENTATION_TOP:
                rect.y += IFM_RESIZE_SASH_SIZE;
                rect.height -= IFM_RESIZE_SASH_SIZE;
                break;

            case IFM_ORIENTATION_BOTTOM:
                rect.height -= IFM_RESIZE_SASH_SIZE;
                break;
        }
    }
    else if (event.GetSourceCoords() == IFM_COORDS_BACKGROUND && event.GetDestinationCoords() == IFM_COORDS_ABSOLUTE )
    {
        switch(data->m_side)
        {
            case IFM_ORIENTATION_LEFT:
                rect.x -= IFM_RESIZE_SASH_SIZE;
                rect.width += IFM_RESIZE_SASH_SIZE;
                break;

            case IFM_ORIENTATION_RIGHT:
                rect.width += IFM_RESIZE_SASH_SIZE;
                break;

            case IFM_ORIENTATION_TOP:
                rect.y -= IFM_RESIZE_SASH_SIZE;
                rect.height += IFM_RESIZE_SASH_SIZE;
                break;

            case IFM_ORIENTATION_BOTTOM:
                rect.height += IFM_RESIZE_SASH_SIZE;
                break;
        }
    }

    event.SetRect(rect);

    // default processing
    event.Skip();
}

void wxIFMDefaultResizePlugin::OnPaintDecor(wxIFMPaintEvent &event)
{
    wxIFMComponent *component = event.GetComponent();
    wxIFMResizeData *data = IFM_GET_EXTENSION_DATA(component, wxIFMResizeData);
	

    // paint a resize sash if it is visible
    if( data->m_display && component->IsVisible() )
    {
        // get resize sash rect
        wxIFMRectEvent rectevt(wxEVT_IFM_GETRESIZESASHRECT, component);
        GetIP()->ProcessPluginEvent(rectevt);
        wxRect sash_rect = rectevt.GetRect();

        //! \todo Globalize colour storage
        wxBrush brush_bg(wxResizer_Pen); //maks(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
        wxPen pen_bg(wxResizer_Pen); //maks(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
        wxPen border_pen(wxResizerBorder_Pen); //maks(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW));

        wxDC &dc = event.GetDC();

        // paint background
        dc.SetBrush(brush_bg);
        dc.SetPen(pen_bg);
        dc.DrawRectangle(sash_rect); //maks: draw external sizers

        // paint borders
        dc.SetPen(border_pen);

        // borders on the top and bottom sides only paint top and bottom borders
        // left and right sides paint left and right borders
#if IFM_RESIZE_SASH_BORDER
        switch(data->m_side)
        {
        case IFM_ORIENTATION_LEFT:
        case IFM_ORIENTATION_RIGHT: 
            dc.DrawLine(sash_rect.x + sash_rect.width - 1, sash_rect.y, sash_rect.x + sash_rect.width - 1, sash_rect.y + sash_rect.height);
            dc.DrawLine(sash_rect.x, sash_rect.y, sash_rect.x, sash_rect.y + sash_rect.height);

			//dc.DrawLine(sash_rect.x + sash_rect.width/2, sash_rect.y, sash_rect.x + sash_rect.width/2, sash_rect.y + sash_rect.height);
            break;
        case IFM_ORIENTATION_TOP:
        case IFM_ORIENTATION_BOTTOM:
            dc.DrawLine(sash_rect.x, sash_rect.y, sash_rect.x + sash_rect.width, sash_rect.y);
            dc.DrawLine(sash_rect.x, sash_rect.y + sash_rect.height - 1, sash_rect.x + sash_rect.width, sash_rect.y + sash_rect.height - 1);

			//dc.DrawLine(sash_rect.x + 10, sash_rect.y + sash_rect.height/2, sash_rect.x + sash_rect.width, sash_rect.y + sash_rect.height/2);
			break;
        }
#endif

        dc.EndDrawing();
    }

    event.Skip();
}

void wxIFMDefaultResizePlugin::OnGetRect(wxIFMRectEvent &event)
{
    if( event.GetEventType() != wxEVT_IFM_GETRESIZESASHRECT )
    {
        event.Skip();
        return;
    }

    wxIFMComponent *component = event.GetComponent();
    wxIFMResizeData *data = IFM_GET_EXTENSION_DATA(component, wxIFMResizeData);
    if( !data->m_display || component->m_hidden )
        return;

    event.SetRect(data->m_rect);
}

void wxIFMDefaultResizePlugin::OnSetRect(wxIFMRectEvent &event)
{
    if( event.GetEventType() != wxEVT_IFM_SETRECT )
    {
        event.Skip();
        return;
    }

    // default processing
    GetNextHandler()->ProcessEvent(event);

    // set the resize sash as well
    wxIFMComponent *component = event.GetComponent();
    wxIFMResizeData *data = IFM_GET_EXTENSION_DATA(component, wxIFMResizeData);
    wxRect component_rect = event.GetRect(), sash_rect;

    // calculate resize sash rect
    switch(data->m_side)
    {
        case IFM_ORIENTATION_LEFT:
            sash_rect.x = component_rect.x;
            sash_rect.y = component_rect.y;
            sash_rect.width = IFM_RESIZE_SASH_SIZE;
            sash_rect.height = component_rect.height;
            break;

        case IFM_ORIENTATION_TOP:
            sash_rect.x = component_rect.x;
            sash_rect.y = component_rect.y;
            sash_rect.width = component_rect.width;
            sash_rect.height = IFM_RESIZE_SASH_SIZE;
            break;

        case IFM_ORIENTATION_RIGHT:
            sash_rect.x = component_rect.x + component_rect.width - IFM_RESIZE_SASH_SIZE;
            sash_rect.y = component_rect.y;
            sash_rect.width = IFM_RESIZE_SASH_SIZE;
            sash_rect.height = component_rect.height;
            break;

        case IFM_ORIENTATION_BOTTOM:
            sash_rect.x = component_rect.x;
            sash_rect.y = component_rect.y + component_rect.height - IFM_RESIZE_SASH_SIZE;
            sash_rect.height = IFM_RESIZE_SASH_SIZE;
            sash_rect.width = component_rect.width;
            break;
    }

    data->m_rect = sash_rect;
}

void wxIFMDefaultResizePlugin::OnSetCursor(wxIFMSetCursorEvent &event)
{
    wxIFMComponent *component = event.GetComponent();

    if( component )
    {
        wxSetCursorEvent &cursor_event = event.GetCursorEvent();
        wxPoint pos = wxPoint(cursor_event.GetX(), cursor_event.GetY());

        wxIFMRectEvent evt(wxEVT_IFM_GETRESIZESASHRECT, component);
        GetIP()->ProcessPluginEvent(evt);

        if( evt.GetRect().Inside(pos) )
        {
            wxIFMResizeData *data = IFM_GET_EXTENSION_DATA(component, wxIFMResizeData);
            switch(data->m_side)
            {
                case IFM_ORIENTATION_TOP:
                case IFM_ORIENTATION_BOTTOM:
                    cursor_event.SetCursor(wxCursor(wxCURSOR_SIZENS));
                    break;
                case IFM_ORIENTATION_LEFT:
                case IFM_ORIENTATION_RIGHT:
                    cursor_event.SetCursor(wxCursor(wxCURSOR_SIZEWE));
                    break;
            }
            return;
        }
    }

    event.Skip();
}

void wxIFMDefaultResizePlugin::OnLeftDown(wxIFMMouseEvent &event)
{
    wxIFMComponent *component = event.GetComponent();

    if( !component )
    {
        event.Skip();
        return;
    }

    //! \todo Possibly provide resize sash hit test event?
    wxIFMRectEvent rectevt(wxEVT_IFM_GETRESIZESASHRECT, component);
    GetIP()->ProcessPluginEvent(rectevt);
    wxRect resize_rect = rectevt.GetRect();

    if( !resize_rect.Inside(event.GetMouseEvent().GetPosition()) )
    {
        event.Skip();
        return;
    }

    if( !m_resizing )
    {
        GetManager()->CaptureInput(component);
        GetManager()->GetCapturedWindow()->SetFocus();
        m_resizing = true;

        wxIFMResizeEvent evt(wxEVT_IFM_BEGINRESIZE, component,
            event.GetMouseEvent().GetPosition(), false);
        GetIP()->ProcessPluginEvent(evt);
    }
}

void wxIFMDefaultResizePlugin::OnLeftUp(wxIFMMouseEvent &event)
{
    if( m_resizing  )
    {
        m_resizing = false;
        GetManager()->ReleaseInput();

        wxIFMResizeEvent evt(wxEVT_IFM_ENDRESIZE, event.GetComponent(),
            event.GetMouseEvent().GetPosition(), false);
        GetIP()->ProcessPluginEvent(evt);
    }
    else
        event.Skip();
}

void wxIFMDefaultResizePlugin::OnMouseMove(wxIFMMouseEvent &event)
{
    if( m_resizing )
    {
        wxIFMResizeEvent evt(wxEVT_IFM_RESIZE, event.GetComponent(),
            event.GetMouseEvent().GetPosition(), false);
        GetIP()->ProcessPluginEvent(evt);
    }
    else
        event.Skip();
}

void wxIFMDefaultResizePlugin::OnKeyDown(wxIFMKeyEvent &event)
{
    if( m_resizing )
    {
        if( event.GetKeyEvent().GetKeyCode() == WXK_ESCAPE )
        {
            wxIFMResizeEvent evt(wxEVT_IFM_ENDRESIZE, event.GetComponent(),
                event.GetKeyEvent().GetPosition(), false, true);
            if( GetIP()->ProcessPluginEvent(evt) )
            {
                m_resizing = false;
                GetManager()->ReleaseInput();
            }
        }
    }
}

void wxIFMDefaultResizePlugin::PaintResizeHint(wxScreenDC &dc, const wxRect &rect)
{
    dc.SetBrush(*wxGREY_BRUSH);
    dc.SetPen(*wxGREY_PEN);

    dc.SetLogicalFunction(wxXOR);
	dc.DrawRectangle(rect); //maks: draw moving sizers

    dc.SetPen(wxNullPen);
    dc.SetBrush(wxNullBrush);
}

void wxIFMDefaultResizePlugin::OnResizeBegin(wxIFMResizeEvent &event)
{
    wxIFMComponent *component = event.GetComponent();
    wxPoint pos = event.GetPosition();

    wxIFMRectEvent rectevt(wxEVT_IFM_GETRESIZESASHRECT, component);
    GetIP()->ProcessPluginEvent(rectevt);
    wxRect resize_rect = rectevt.GetRect();

    wxIFMResizeData *data = IFM_GET_EXTENSION_DATA(component, wxIFMResizeData);
    switch(data->m_side)
    {
        case IFM_ORIENTATION_LEFT:
        case IFM_ORIENTATION_RIGHT:
            m_orientation = IFM_ALIGN_VERTICAL;
            pos.y = resize_rect.y;
            m_offset = pos.x - resize_rect.x;
            pos.x -= m_offset;
            break;
        case IFM_ORIENTATION_TOP:
        case IFM_ORIENTATION_BOTTOM:
            m_orientation = IFM_ALIGN_HORIZONTAL;
            pos.x = resize_rect.x;
            m_offset = pos.y - resize_rect.y;
            pos.y -= m_offset;
            break;
    }

    m_oldPos = GetManager()->GetCapturedWindow()->ClientToScreen(pos);
    m_InitialResizeRect = m_resizeRect = wxRect(m_oldPos, rectevt.GetSize()); //maks

    if( !event.RealtimeUpdates() )
    {
        wxScreenDC dc;
        PaintResizeHint(dc, m_resizeRect);
    }
}

void wxIFMDefaultResizePlugin::OnResizeEnd(wxIFMResizeEvent &event)
{
    if( !event.RealtimeUpdates() )
    {
        // unpaint resize hint
        wxScreenDC dc;
        PaintResizeHint(dc, m_resizeRect);
    }

    if( !event.WasCanceled() )
    {
        wxIFMComponent *component = event.GetComponent();

        wxIFMResizeData *resize_data = IFM_GET_EXTENSION_DATA(component, wxIFMResizeData);

        wxPoint pos = event.GetPosition();

        if( m_orientation == IFM_ALIGN_HORIZONTAL )
        {
            pos.y -= m_offset;
            pos.x = m_resizeRect.x;
        }
        else if( m_orientation == IFM_ALIGN_VERTICAL )
        {
            pos.x -= m_offset;
            pos.y = m_resizeRect.y;
        }

        // accessing m_rect directly here is OK because we know the component cannot be hidden
        wxRect rect = component->m_rect;
        int new_width = 0, new_height = 0;

        switch(resize_data->m_side)
        {
            case IFM_ORIENTATION_BOTTOM:
                new_width = rect.width;
                new_height = pos.y - rect.y + IFM_RESIZE_SASH_SIZE;
                break;
            case IFM_ORIENTATION_TOP:
                new_width = rect.width;
                new_height = rect.y + rect.height - pos.y;
                break;
            case IFM_ORIENTATION_LEFT:
                new_height = rect.height;
                new_width = rect.x + rect.width - pos.x;
                break;
            case IFM_ORIENTATION_RIGHT:
                new_height = rect.height;
                new_width = pos.x - rect.x + IFM_RESIZE_SASH_SIZE;
                break;
        }

        wxSize size(new_width, new_height);

        if( component->m_docked )
        {
            // set the desired size of the component and its siblings to make room
            ResizeComponent(component, size);
        }
        else
        {
            // just update the component
            wxIFMRectEvent setevt(wxEVT_IFM_SETDESIREDSIZE, component, wxPoint(), size);
            GetIP()->ProcessPluginEvent(setevt);
        }

        // only update the parent container to save updating the entire interface if we can get away with it
        wxIFMComponent *parent = component->m_parent;
        if( parent )
        {
            wxIFMUpdateComponentEvent evt(parent, parent->GetRect());
            GetIP()->ProcessPluginEvent(evt);
        }
        else
            GetManager()->Update();
    }
}

void wxIFMDefaultResizePlugin::ResizeComponent(wxIFMComponent *component, wxSize size)
{
    wxIFMComponent *parent = component->m_parent;

    if( parent->GetType() != IFM_COMPONENT_CONTAINER )
        return;

    // set our desired size first
    wxIFMRectEvent setevt(wxEVT_IFM_SETDESIREDSIZE, component, wxPoint(), size);
    GetIP()->ProcessPluginEvent(setevt);

    // set the desired size of our siblings
    wxIFMComponentArray &children = parent->m_children;
    wxRect parent_rect;
    int numchildren = children.size(), i;

    parent_rect = parent->GetClientRect();

    // find the index of the thing we resized in its parent
    int index = 0;
    for( i = 0; i < numchildren; ++i )
    {
        if( children[i] == component )
        {
            index = i;
            break;
        }
    }

    // calculate the available parent rect to use for component rect calculations
    for( i = 0; i <= index; ++i )
    {
        const wxRect &rect = (i == index) ? wxRect(wxPoint(), size) : children[i]->GetRect();

        if( parent->m_alignment == IFM_ALIGN_HORIZONTAL )
        {
            parent_rect.width -= rect.width;
            parent_rect.x += rect.width;
        }
        else
        {
            parent_rect.height -= rect.height;
            parent_rect.y += rect.height;
        }
    }

    // add the children that require sizing to the parent rect
    wxIFMCalcRectsEvent calcevt(parent, parent_rect);

    for( i = index + 1; i < numchildren; ++i )
        calcevt.AddComponent(children[i]);

    GetIP()->ProcessPluginEvent(calcevt);

    // set children desired sizes
    const wxRectArray &rects = calcevt.GetComponentRects();
    for( i = 0; i < numchildren; ++i )
    {
        wxRect rect;

        if( i < index )
            rect = children[i]->GetRect();
        else if( i == index )
            continue;
        else
            rect = rects[i - index - 1];

        wxIFMRectEvent evt(wxEVT_IFM_SETDESIREDSIZE, children[i], rect);
        GetIP()->ProcessPluginEvent(evt);
    }
}

void wxIFMDefaultResizePlugin::LimitResize(wxIFMComponent *component, wxPoint &pos) //maks
{
	// accessing m_rect directly here is OK because we know the component cannot be hidden
	wxRect rect = component->m_rect;
	wxSize max = component->GetMaxSize();
	wxSize min = component->GetMinSize();
	int new_width = 0, new_height = 0, m_side = IFM_ORIENTATION_NONE;

	rect.SetPosition(GetManager()->GetCapturedWindow()->ClientToScreen(rect.GetPosition()));

	if( m_orientation == IFM_ALIGN_HORIZONTAL )
	{
		if(rect.y + rect.height == m_InitialResizeRect.y + m_InitialResizeRect.height)
		{
			m_side = IFM_ORIENTATION_BOTTOM;
		}
		else if(rect.y == m_InitialResizeRect.y + m_InitialResizeRect.height)
		{
			m_side = IFM_ORIENTATION_TOP;
		}
	}
	else if( m_orientation == IFM_ALIGN_VERTICAL )
	{
		if(rect.x + rect.width == m_InitialResizeRect.x + m_InitialResizeRect.width)
		{
			m_side = IFM_ORIENTATION_RIGHT;
		}
		else if(rect.x == m_InitialResizeRect.x)
		{
			m_side = IFM_ORIENTATION_LEFT;
		}
	}

	switch(m_side)
	{
	case IFM_ORIENTATION_BOTTOM:
		new_height = pos.y - rect.y + IFM_RESIZE_SASH_SIZE;
		if(new_height > max.GetHeight())
		{
			pos.y = max.GetHeight() + rect.y - IFM_RESIZE_SASH_SIZE;
		}
		else if(new_height < min.GetHeight())
		{
			pos.y = min.GetHeight() + rect.y - IFM_RESIZE_SASH_SIZE;	
		}
		break;

	case IFM_ORIENTATION_TOP:
		new_height = rect.y + rect.height - pos.y;
		if(new_height > max.GetHeight())
		{
			pos.y = rect.y + rect.height - max.GetHeight();
		}
		else if(new_height < min.GetHeight())
		{
			pos.y = rect.y + rect.height - min.GetHeight() - IFM_RESIZE_SASH_SIZE;
		}
		break;

	case IFM_ORIENTATION_LEFT:
		new_width = rect.x + rect.width - pos.x;
		if(new_width > max.GetWidth())
		{
			pos.x = rect.x + rect.width - max.GetWidth();
		}
		else if(new_width < min.GetWidth())
		{
			pos.x = rect.x + rect.width - min.GetWidth();
		}
		break;

	case IFM_ORIENTATION_RIGHT:
		new_width = pos.x - rect.x + IFM_RESIZE_SASH_SIZE;
		if(new_width > max.GetWidth())
		{
			pos.x = max.GetWidth() + rect.x - IFM_RESIZE_SASH_SIZE;
		}
		else if(new_width < min.GetWidth())
		{
			pos.x = min.GetWidth() + rect.x - IFM_RESIZE_SASH_SIZE;
		}
		break;
	}
}

void wxIFMDefaultResizePlugin::OnResizing(wxIFMResizeEvent &event)
{
    wxPoint pos = GetManager()->GetCapturedWindow()->ClientToScreen(event.GetPosition());

    if( m_orientation == IFM_ALIGN_HORIZONTAL )
    {
        pos.y -= m_offset;
        pos.x = m_resizeRect.x;
    }
    else if( m_orientation == IFM_ALIGN_VERTICAL )
    {
        pos.x -= m_offset;
        pos.y = m_resizeRect.y;
    }

	///////////////////////////////////////////////////
	//maks: limit the size in real time
	wxIFMComponent *component = event.GetComponent();
	if(component->m_parent)
	{
		wxIFMComponent *child;
		
		const wxIFMComponentArray &children = component->m_parent->m_children;
		for( int i = 0, count = children.GetCount(); i < count; ++i )
		{
			child = children[i];
			if(child->IsVisible())
			{				
				LimitResize(child, pos);
			}
		}
	}
	else
	{	
		LimitResize(component, pos);
	}
	///////////////////////////////////////////////////
	
    if( !event.RealtimeUpdates() && pos != m_oldPos )
    {
        // draw resize sash
        wxScreenDC dc;

        m_resizeRect.SetPosition(m_oldPos);
        PaintResizeHint(dc, m_resizeRect);
        m_resizeRect.SetPosition(pos);
        PaintResizeHint(dc, m_resizeRect);
    }

    m_oldPos = pos;
}

bool wxIFMDefaultResizePlugin::HasResizeableChildren(wxIFMComponent *component)
{
    wxIFMComponent *child;

    const wxIFMComponentArray &children = component->m_children;
    for( int i = 0, count = children.GetCount(); i < count; ++i )
    //for( wxIFMComponentArray::const_iterator i = children.begin(), end = children.end(); i != end; ++i )
    {
        child = children[i];
        //child = *i;
        if( !child->IsVisible() )
            continue;

        if( child->GetType() != IFM_COMPONENT_CONTAINER )
        {
            if( !child->m_fixed )
                return true;
        }

        if( !child->m_fixed )
        {
            // check children
            if( HasResizeableChildren(child) )
                return true;
        }
    }

    return false;
}

/*
wxIFMResizeData implementation
*/
wxIFMResizeData::wxIFMResizeData()
    : wxIFMExtensionDataBase(),
    m_side(IFM_ORIENTATION_NONE),
    m_display(false)
{ }

wxIFMComponentDataKeyType wxIFMResizeData::GetDataKey() const
{
    return IFM_RESIZE_DATA_KEY;
}

wxIFMComponentDataKeyType wxIFMResizeData::DataKey()
{
    return IFM_RESIZE_DATA_KEY;
}
