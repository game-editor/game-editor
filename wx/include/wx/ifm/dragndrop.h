/*!
    Header file for the drag and drop docking plugin

    \file   dragndrop.h
    \author Robin McNeill
    \date   Created: 01/15/05

    Copyright (c) Robin McNeill
    Licensed under the terms of the wxWindows license
*/

#ifndef _IFM_DND_H_
#define _IFM_DND_H_

#include "wx/ifm/definterface.h"

#include "wx/frame.h"
#include "wx/dcscreen.h"
//#include <vector>

class wxIFMDefaultDockingPlugin;
class wxIFMInitDragEvent;
class wxIFMDragEvent;
class wxIFMDockTargetButton;
class wxIFMShowDropTargetsEvent;

DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM,wxEVT_IFM_INITDRAG, 0);
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM,wxEVT_IFM_BEGINDRAG, 0);
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM,wxEVT_IFM_ENDDRAG, 0);
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM,wxEVT_IFM_DRAGGING, 0);

DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM,wxEVT_IFM_SHOWDROPTARGETS, 0);

#define IFM_DOCK_TARGET_BUTTON_WIDTH    20
#define IFM_DOCK_TARGET_BUTTON_SIZE     wxSize(IFM_DOCK_TARGET_BUTTON_WIDTH, IFM_DOCK_TARGET_BUTTON_WIDTH)

#define IFM_DRAG_DISTANCE 4 // how many pixels the mouse must move before dragging starts (only if SystemMetrics doesn't know)

//typedef std::vector<wxIFMDockTargetButton *> DockButtonArray;
WX_DEFINE_ARRAY(wxIFMDockTargetButton*, DockButtonArray);

enum dock_button_icons
{
    IFM_DOCK_ICON_LEFT,
    IFM_DOCK_ICON_RIGHT,
    IFM_DOCK_ICON_TOP,
    IFM_DOCK_ICON_BOTTOM,
    IFM_DOCK_ICON_TAB
};

enum dock_button_ids
{
    IFM_DOCK_ID_LEFT,
    IFM_DOCK_ID_RIGHT,
    IFM_DOCK_ID_BOTTOM,
    IFM_DOCK_ID_TOP,
    IFM_DOCK_ID_TAB,

    IFM_DOCK_ID_FRAME_LEFT,
    IFM_DOCK_ID_FRAME_RIGHT,
    IFM_DOCK_ID_FRAME_BOTTOM,
    IFM_DOCK_ID_FRAME_TOP
};

/*!
    Plugin to support drag and drop docking MSVC++8 style
*/
class wxIFMDefaultDockingPlugin : public wxIFMExtensionPluginBase
{
private:

    wxIFMDefaultInterfacePlugin *m_ip;

    bool m_realtime, m_dragging, m_captured;
    wxPoint m_offset, m_oldPos, m_clickPos;
    int m_dragx, m_dragy;

    wxRect m_hintRect;

    DockButtonArray m_dockButtonArray;
    wxIFMDockTargetButton *m_oldBtn;

	static bool bUseTargetButtons;//maks

#if IFM_CANFLOAT
    wxIFMFloatingWindowBase *m_window;
#endif

public:

    wxIFMDefaultDockingPlugin();

    bool Initialize(wxIFMInterfacePluginBase *plugin);

	static void setUseTargetButtons(bool value) {bUseTargetButtons = value;}; //maks
	static bool getUseTargetButtons() {return bUseTargetButtons;}; //maks

private:

#if IFM_CANFLOAT
    void OnLeftDClick(wxIFMMouseEvent &event);
#endif
    void OnLeftUp(wxIFMMouseEvent &event);
    void OnMouseMove(wxIFMMouseEvent &event);
    void OnKeyDown(wxIFMKeyEvent &event);

    void OnDragInit(wxIFMInitDragEvent &event);
    void OnDragBegin(wxIFMDragEvent &event);
    void OnDragEnd(wxIFMDragEvent &event);
    void OnDrag(wxIFMDragEvent &event);
    void OnDock(wxIFMDockEvent &event);
    void OnShowDropTargets(wxIFMShowDropTargetsEvent &event);

    void DrawHintRect(wxDC &dc, const wxRect &rect);

    void CreateTargetButtons();
    void DestroyTargetButtons();

    void ShowFrameDropButtons(bool show = true);
    void ShowComponentDropButtons(bool show  = true);

    /*!
        \param pos Position in screen coordinates
        \return The dock target button the position is over (if any)
    */
    wxIFMDockTargetButton *GetDockTargetButtonByPos(const wxPoint &pos);

    /*!
        This function returns the floating component the mouse if under, but ignores any components
        within the given wxIFMFloatingWindowBase.

        \param pos Position in client coordinates
        \param excluse Window to ignore in the search
    */
#if IFM_CANFLOAT
    wxIFMComponent *GetFloatingComponentByPosExclusion(const wxPoint &pos, wxIFMFloatingWindowBase *exclude);
#endif

    DECLARE_EVENT_TABLE()

    DECLARE_DYNAMIC_CLASS_NO_COPY(wxIFMDefaultDockingPlugin)
};

/*!
    This event is used to tell the dragndrop plugin to start dragging the
    component identified by m_drag. Dragging does not immediately begin,
    but the dragndrop plugin will capture input and wait for enough motion
    to begin the drag. This used to happen within the dragndrop plugin in
    response to mouse down events. Now, the mouse down even will send the
    wxEVT_IFM_INITDRAG event.
*/
class wxIFMInitDragEvent : public wxIFMPluginEvent
{
private:
    wxIFMComponent *m_drag; //!< component that should be dragged
    wxPoint m_pos; //!< position where the mouse was clicked in client coordinates

public:
    wxIFMInitDragEvent(wxIFMComponent *component, wxIFMComponent *drag, const wxPoint &pos)
        : wxIFMPluginEvent(wxEVT_IFM_INITDRAG, component),
        m_drag(drag),
        m_pos(pos)
    { }

    wxIFMInitDragEvent(const wxIFMInitDragEvent &event)
        : wxIFMPluginEvent(event),
        m_drag(event.m_drag),
        m_pos(event.m_pos)
    { }

    virtual wxEvent *Clone() const { return new wxIFMInitDragEvent(*this); }

    /*!
        \return The component that will be dragged
    */
    wxIFMComponent *GetDraggedComponent() const { return m_drag; }

    /*!
        \return The position of the mouse in client coordinates
    */
    const wxPoint &GetPos() const { return m_pos; }
};

/*!
    Drag event class
*/
class wxIFMDragEvent : public wxIFMPluginEvent
{
private:

    wxPoint m_pos;
    bool m_canceled, m_realtime;
    bool m_altDown, m_ctrlDown, m_shiftDown;

public:

    wxIFMDragEvent(wxEventType type, wxIFMComponent *component,
        wxPoint pos, bool alt, bool shift, bool control,
        bool realtime = false, bool canceled = false)
        : wxIFMPluginEvent(type, component),
        m_pos(pos),
        m_canceled(canceled),
        m_realtime(realtime),
        m_altDown(alt),
        m_ctrlDown(control),
        m_shiftDown(shift)
    { }

    wxIFMDragEvent(const wxIFMDragEvent &event)
        : wxIFMPluginEvent(event),
        m_pos(event.m_pos),
        m_canceled(event.m_canceled),
        m_realtime(event.m_realtime),
        m_altDown(event.m_altDown),
        m_ctrlDown(event.m_ctrlDown),
        m_shiftDown(event.m_shiftDown)
    { }

    virtual wxEvent *Clone() const { return new wxIFMDragEvent(*this); }

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

    /*!
        @return True if control is pressed
    */
    bool ControlDown() const { return m_ctrlDown; }

    /*!
        @return True if shift is pressed
    */
    bool ShiftDown() const { return m_shiftDown; }

    /*!
        @return True if alt is pressed
    */
    bool AltDown() const { return m_altDown; }
};

/*!
    Derived dock event providing for "Dock (right,left,below,above,into)" events. Used by wxIFMDefaultDockingPlugin.

    \see wxIFMDockEvent
*/
class wxIFMDockEventEx : public wxIFMDockEvent
{
private:
    int m_where; // above, below, right, left, into

public:
    // default ctor for wx rtti to work
    wxIFMDockEventEx() : wxIFMDockEvent(0, 0, 0) { }

    wxIFMDockEventEx(wxIFMComponent *component, wxIFMComponent *destination,
        int index, int where)
        : wxIFMDockEvent(component, destination, index),
        m_where(where)
    { }

    wxIFMDockEventEx(const wxIFMDockEventEx &event)
        : wxIFMDockEvent(event),
        m_where(event.m_where)
    { }

    virtual wxEvent *Clone() const { return new wxIFMDockEventEx(*this); }

    int GetWhere() const { return m_where; }
    void SetIndex(int i) { m_index = i; }

    DECLARE_DYNAMIC_CLASS(wxIFMDockEventEx)
};

/*
class wxIFMDropTargetEventBase : public wxIFMPluginEvent
{
public:
    wxIFMDropTargetEventBase(wxInterfaceManager *manager, wxEventType id, wxIFMComponent *component = NULL,
        )
        : wxIFMPluginEvent(manager, id, component)
};
*/

/*!
    \brief Event for positioning drop target buttons. This event specifies
    the component that the mouse is hovering over, which is the component that
    the "cross" drop target buttons should be positioned over. The component
    actually being dragged is specified by GetDraggedComponent.
*/
class wxIFMShowDropTargetsEvent : public wxIFMPluginEvent
{
private:
    wxPoint m_pos; // position of mouse in client coordinates
    wxIFMComponent *m_draggedComponent; // component being dragged

public:
    wxIFMShowDropTargetsEvent( wxIFMComponent *component,
        const wxPoint &pos, wxIFMComponent *dragged_component)
        : wxIFMPluginEvent(wxEVT_IFM_SHOWDROPTARGETS, component),
        m_pos(pos),
        m_draggedComponent(dragged_component)
    { }

    wxIFMShowDropTargetsEvent(const wxIFMShowDropTargetsEvent &event)
        : wxIFMPluginEvent(event),
        m_pos(event.m_pos),
        m_draggedComponent(event.m_draggedComponent)
    { }

    virtual wxEvent *Clone() const { return new wxIFMShowDropTargetsEvent(*this); }

    /*!
        \return Position of the mouse in client coordinates
    */
    const wxPoint &GetPosition() const { return m_pos; }

    /*!
        \return The component which is being dragged
    */
    wxIFMComponent *GetDraggedComponent() const { return m_draggedComponent; }
};

class wxIFMPlaceMark : public wxFrame //maks
{
public:

    wxIFMPlaceMark(wxWindow *parent, const wxRect &rect, int _m_id);
   

private:

    void OnEraseBackground(wxEraseEvent &event);
    void OnPaint(wxPaintEvent &event);

	void SetRegion();


    DECLARE_EVENT_TABLE()
};

class wxIFMDockTargetButton : public wxFrame
{
private:

    bool m_hover;
	bool bCanShow; //maks
    int m_id, m_icon;
	wxIFMPlaceMark *placeMark; //maks 

    wxIFMComponent *m_component;

public:

    wxIFMDockTargetButton(wxWindow *parent, const wxPoint &pos, int id, int icon, wxIFMComponent *component = NULL);
	virtual ~wxIFMDockTargetButton(); //maks

    /*!
        Notify this button that the mouse is hovering over it. This will cause
        the button to redraw itself to reflect its new state
    */
    void SetHover(bool hover = true);

    int GetId() const { return m_id; }

    void SetComponent(wxIFMComponent *component) 
	{ 
		if(m_component != component)
		{
			Show(true);
		}

		m_component = component; 
	}

    wxIFMComponent *GetComponent() const { return m_component; }

	bool Show(bool show); //maks
	bool CanShow() {return bCanShow;} //maks

private:

    void OnEraseBackground(wxEraseEvent &event);
    void OnPaint(wxPaintEvent &event);

    DECLARE_EVENT_TABLE()
};



typedef void (wxEvtHandler::*wxIFMInitDragEventFunction)(wxIFMInitDragEvent&);
typedef void (wxEvtHandler::*wxIFMDragEventFunction)(wxIFMDragEvent&);
typedef void (wxEvtHandler::*wxIFMShowDropTargetsEventFunction)(wxIFMShowDropTargetsEvent&);

#define wxIFMInitDragEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMInitDragEventFunction, &func)
#define wxIFMDragEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMDragEventFunction, &func)
#define wxIFMShowDropTargetsEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMShowDropTargetsEventFunction, &func)

#define EVT_IFM_DRAG_INIT(func)     wx__DECLARE_EVT0(wxEVT_IFM_INITDRAG, wxIFMInitDragEventFunctionHandler(func))
#define EVT_IFM_DRAG_BEGIN(func)    wx__DECLARE_EVT0(wxEVT_IFM_BEGINDRAG, wxIFMDragEventFunctionHandler(func))
#define EVT_IFM_DRAG_END(func)      wx__DECLARE_EVT0(wxEVT_IFM_ENDDRAG, wxIFMDragEventFunctionHandler(func))
#define EVT_IFM_DRAG_DRAGGING(func) wx__DECLARE_EVT0(wxEVT_IFM_DRAGGING, wxIFMDragEventFunctionHandler(func))
#define EVT_IFM_SHOWDROPTARGETS(func) wx__DECLARE_EVT0(wxEVT_IFM_SHOWDROPTARGETS, wxIFMShowDropTargetsEventFunctionHandler(func))

#endif
