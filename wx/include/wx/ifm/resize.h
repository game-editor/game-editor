/*!
    Header file for the resize plugin

    \file   resize.h
    \author Robin McNeill
    \date   Created: 01/02/05

    Copyright (c) Robin McNeill
    Licensed under the terms of the wxWindows license
*/

#ifndef _IFM_RESIZE_H_
#define _IFM_RESIZE_H_

#include "wx/ifm/plugin.h"
#include "wx/ifm/events.h"
#include "wx/ifm/definterface.h"

#include "wx/dc.h"
#include "wx/dcscreen.h"

#define IFM_RESIZE_SASH_BORDER  0
#define IFM_RESIZE_SASH_SIZE    (4 + IFM_RESIZE_SASH_BORDER * 2) //maks: change the resizer thick

class WXDLLIMPEXP_IFM wxIFMResizeEvent;
class WXDLLIMPEXP_IFM wxIFMUpdateResizeSashEvent;

DECLARE_IFM_DATA_KEY(IFM_RESIZE_DATA_KEY)

DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM,wxEVT_IFM_BEGINRESIZE, 0)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM,wxEVT_IFM_ENDRESIZE, 0)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM,wxEVT_IFM_RESIZE, 0)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM,wxEVT_IFM_RESIZESASHDBLCLICK, 0)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM,wxEVT_IFM_GETRESIZESASHRECT, 0)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM,wxEVT_IFM_UPDATERESIZESASHES, 0)

/*!
    Plugin responsible for handling resize sashes
*/
class WXDLLIMPEXP_IFM wxIFMDefaultResizePlugin : public wxIFMExtensionPluginBase
{
private:
    wxIFMDefaultInterfacePlugin *m_ip;

    bool m_resizing, m_invalidated;
    wxPoint m_oldPos;
    wxRect m_resizeRect;
	wxRect m_InitialResizeRect; //maks
    int m_orientation;
    int m_offset;

	void LimitResize(wxIFMComponent *component, wxPoint &pos); //maks

public:

    wxIFMDefaultResizePlugin();

    bool Initialize(wxIFMInterfacePluginBase *plugin);

    void OnCreateComponent(wxIFMNewComponentEvent &event);
    void OnDock(wxIFMDockEvent &event);
    void OnUndock(wxIFMUndockEvent &event);
    void OnConvertRect(wxIFMConvertRectEvent &event);
    void OnPaintDecor(wxIFMPaintEvent &event);
    void OnShowComponent(wxIFMShowComponentEvent &event);
    void OnGetRect(wxIFMRectEvent &event);
    void OnSetRect(wxIFMRectEvent &event);
    void OnSetCursor(wxIFMSetCursorEvent &event);
    void OnLeftDown(wxIFMMouseEvent &event);
    void OnLeftUp(wxIFMMouseEvent &event);
    void OnMouseMove(wxIFMMouseEvent &event);
    void OnKeyDown(wxIFMKeyEvent &event);
    void OnGetDesiredSize(wxIFMRectEvent &event);
#if IFM_CANFLOAT
    void OnFloat(wxIFMFloatEvent &event);
#endif

    void PaintResizeHint(wxScreenDC &dc, const wxRect &rect);

    void OnResizeBegin(wxIFMResizeEvent &event);
    void OnResizeEnd(wxIFMResizeEvent &event);
    void OnResizing(wxIFMResizeEvent &event);

    void OnAddTopContainer(wxIFMAddTopContainerEvent &event);

    void OnUpdate(wxIFMUpdateEvent &event);
    void OnUpdateResizeSashes(wxIFMUpdateResizeSashEvent &event);
    void UpdateContainerResizeSashes(wxIFMComponent *container);
    void UpdateResizeSashes(wxIFMComponent *component);
    void ResizeComponent(wxIFMComponent *component, wxSize size);
    bool HasResizeableChildren(wxIFMComponent *component);

    DECLARE_EVENT_TABLE()

    DECLARE_DYNAMIC_CLASS_NO_COPY(wxIFMDefaultResizePlugin)
};

/*!
    Storage class for resize plugin data
*/
class WXDLLIMPEXP_IFM wxIFMResizeData : public wxIFMExtensionDataBase
{
public:

    wxIFMResizeData();

    int m_side; /*! what side to display a resize sash on
                    can be IFM_ORIENTATION_(LEFT|RIGHT|TOP|BOTTOM) */
    bool m_display; //!< whether or not to display a resize sash
    wxRect m_rect; //!< resize sash rect

    wxIFMComponentDataKeyType GetDataKey() const;
    static wxIFMComponentDataKeyType DataKey();

    DECLARE_DYNAMIC_CLASS(wxIFMResizeData)
};

/*!
    Event for resizing events.
*/
class WXDLLIMPEXP_IFM wxIFMResizeEvent : public wxIFMPluginEvent
{
private:
    const wxPoint &m_pos;
    bool m_realtime, m_canceled;

public:

    wxIFMResizeEvent(wxEventType type, wxIFMComponent *component,
        const wxPoint &pos, bool realtime, bool canceled = false)
        : wxIFMPluginEvent(type, component),
        m_pos(pos),
        m_realtime(realtime),
        m_canceled(canceled)
    { }

    wxIFMResizeEvent(const wxIFMResizeEvent &event)
        : wxIFMPluginEvent(event),
        m_pos(event.m_pos),
        m_realtime(event.m_realtime),
        m_canceled(event.m_canceled)
    { }

    virtual wxEvent *Clone() const { return new wxIFMResizeEvent(*this); }

    /*!
        \return Position of the mouse in screen coordinates that generated the event
    */
    const wxPoint &GetPosition() const { return m_pos; }

    /*!
        \return If the resize was canceled by the user (or something else).
            Only makes sense for wxEVT_IFM_ENDRESIZE.
    */
    bool WasCanceled() const { return m_canceled; }

    /*!
        \return True if updates should be made in real time. Only makes sense
            for wxEVT_IFM_RESIZE
    */
    bool RealtimeUpdates() const { return m_realtime; }
};

/*!
    Used to calculate resize sash visibility and positions for a component and its children
*/
class WXDLLIMPEXP_IFM wxIFMUpdateResizeSashEvent : public wxIFMPluginEvent
{
public:
    wxIFMUpdateResizeSashEvent(wxIFMComponent *component)
        : wxIFMPluginEvent(wxEVT_IFM_UPDATERESIZESASHES, component)
    { }

    wxIFMUpdateResizeSashEvent(const wxIFMUpdateResizeSashEvent &event)
        : wxIFMPluginEvent(event)
    { }

    virtual wxEvent *Clone() const { return new wxIFMUpdateResizeSashEvent(*this); }
};

typedef void (wxEvtHandler::*wxIFMResizeEventFunction)(wxIFMResizeEvent&);
typedef void (wxEvtHandler::*wxIFMUpdateResizeSashEventFunction)(wxIFMUpdateResizeSashEvent&);

#define wxIFMResizeEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMResizeEventFunction, &func)
#define wxIFMUpdateResizeSashEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMUpdateResizeSashEventFunction, &func)

#define EVT_IFM_RESIZE_BEGIN(func)  wx__DECLARE_EVT0( wxEVT_IFM_BEGINRESIZE, wxIFMResizeEventFunctionHandler(func))
#define EVT_IFM_RESIZE_END(func)    wx__DECLARE_EVT0( wxEVT_IFM_ENDRESIZE, wxIFMResizeEventFunctionHandler(func))
#define EVT_IFM_RESIZE_RESIZING(func) wx__DECLARE_EVT0( wxEVT_IFM_RESIZE, wxIFMResizeEventFunctionHandler(func))
#define EVT_IFM_RESIZE_UPDATE_SASHES(func) wx__DECLARE_EVT0( wxEVT_IFM_UPDATERESIZESASHES, wxIFMUpdateResizeSashEventFunctionHandler(func))
#define EVT_IFM_GETRESIZESASHRECT(func) wx__DECLARE_EVT0( wxEVT_IFM_GETRESIZESASHRECT, wxIFMRectEventFunctionHandler(func))

#endif
