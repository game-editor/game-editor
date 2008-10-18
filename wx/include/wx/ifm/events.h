/*!
    Event definitions for the IFM system

    \file   events.h
    \author Robin McNeill
    \date   Created: 10/16/04

    Copyright (c) Robin McNeill
    Licensed under the terms of the wxWindows license
*/

#ifndef _IFM_EVENTS_H_
#define _IFM_EVENTS_H_

#include "wx/event.h"
#include "wx/ifm/manager.h"

#if WXWIN_COMPATIBILITY_EVENT_TYPES
#error "WXWIN_COMPATIBILITY_EVENT_TYPES defined, but new event system is required"
#endif

BEGIN_DECLARE_EVENT_TYPES()

// mouse events
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_LEFTDOWN, 0)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_LEFTUP, 0)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_LEFTDCLICK, 0)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_RIGHTDOWN, 0)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_RIGHTUP, 0)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_RIGHTDCLICK, 0)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_MIDDLEDOWN, 0)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_MIDDLEUP, 0)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_MIDDLEDCLICK, 0)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_MOTION, 0)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_MOUSEWHEEL, 0)

DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_SETCURSOR, 0)

DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_SETFOCUS, 0) // parent recieves focus
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_KILLFOCUS, 0) // parent loses focus
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_ACTIVATE, 0) // parent window [de]activates

DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_KEYDOWN, 0)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_KEYUP, 0)

DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_NEWCOMPONENT, 0) // used to create all components
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_DELETECOMPONENT, 0) // used to delete components (not used during cleanup phase)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_SHOWCOMPONENT, 0) // used to show or hide components
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_VISIBILITYCHANGED, 0) // used to notify a component that is has been shown or hidden
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_ADDCHILD, 0) // add a child window to the interface
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_SETCHILD, 0) // used to add children to panels
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_REMOVECHILD, 0) // removes a child from its panel (and the interface)

DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_PAINTINTERFACE, 0) // indicates to plugins that some or all of the interface needs to be repainted
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_PAINTBG, 0) // paints a components background
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_PAINTBORDER, 0) // paints a components border
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_PAINTDECOR, 0) // paints a components decorations
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_BEGINPAINT, 0)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_ENDPAINT, 0)

DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_SHOW, 0) // called to show or hide panels
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_SHOWCHILD, 0) // used to show children managed by the interface
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_HIDECHILD, 0) // used to hide children managed by the interface
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_SETCHILDSIZE, 0) // sets the min / max / desired size of a child
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_QUERYCHILD, 0)

DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_UPDATE, 0) // recalc layout
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_UPDATECOMPONENT, 0) // updates invidual components
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_CALCRECTS, 0)

DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_UPDATECONFIG, 0) // plugin configuration data was updated

// use wxIFMRectEvent for these size events
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_SETDESIREDSIZE, 0) // sets a component desired size
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_GETDESIREDSIZE, 0) // determines the size a component would like to be
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_GETMINSIZE, 0)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_GETMAXSIZE, 0)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_SETMINSIZE, 0)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_SETMAXSIZE, 0)

/*!
Rect events.

    These events are used to set and retrieve both size and position
    values for absolute, background, and client ares.

    If you only wish to set either the position or the size of a component,
    you may specify IFM_DEFAULT_VALUE for values which you are not changing. The existing
    values will be used.

    wxEVT_IFM_CONVERTRECT is used to convert between different rect values.
*/
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_SETRECT, 0) // sets absolute size and pos
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_SETABSOLUTERECT, 0) // same as SETRECT
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_SETBACKGROUNDRECT, 0) // sets background size and pos
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_SETCLIENTRECT, 0) // sets client size and pos
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_GETRECT, 0) // returns absolute size and pos
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_GETABSOLUTERECT, 0) // same as GETRECT
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_GETBACKGROUNDRECT, 0) // returns background size and pos
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_GETCLIENTRECT, 0) // returns client size and pos
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_CONVERTRECT, 0) // used to convert from [client|background]size to [client|background]size
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_HITTEST, 0) // used for position hit testing
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_GETCONTENTRECT, 0) // used to retrieve the rect available for the applications content window

DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_DOCK, 0) // dock a component
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_UNDOCK, 0) // undock a component

/*
Floating events
*/
#if IFM_CANFLOAT
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_FLOAT, 0) // float a component
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_CREATEFLOATINGWINDOW, 0) // used to create a frame for a floating window
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_DESTROYFLOATINGWINDOW, 0) // used to destroy a frame for a floating window
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_FLOATING_MOVING, 0) // a floating window is being moved
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_FLOATING_SIZING, 0) // a floating window is being sized
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_FLOATING_MOVE, 0) // a floating window has been moved
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_FLOATING_SIZE, 0) // a floating window has been sized
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_FLOATING_SHOW, 0) // used to show a floating window
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_FLOATING_HIDE, 0) // used to hide a floating window
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM, wxEVT_IFM_FLOATING_NOTIFY, 0) // notifies a component that it has been floated
#endif

END_DECLARE_EVENT_TYPES()

/*!
    \brief  IFM event base class.
*/
class WXDLLIMPEXP_IFM wxIFMPluginEvent : public wxEvent
{
protected:

    int m_componentType; // what type of component is this message for?
    wxIFMComponent *m_component; // the component associated with the event

public:

    wxIFMPluginEvent(wxEventType id, wxIFMComponent *component = NULL);

    wxIFMPluginEvent(const wxIFMPluginEvent &event)
        : wxEvent(event),
        m_componentType(event.m_componentType),
        m_component(event.m_component)
    { }

    virtual wxEvent *Clone() const { return new wxIFMPluginEvent(*this); }

    /*!
        Specifies what type of component this message applies to
    */
    int GetComponentType() const { return m_componentType; }

    /*!
        \return Component associated with this event
    */
    wxIFMComponent *GetComponent() const { return m_component; }

    wxIFMPluginEvent() { }
    DECLARE_DYNAMIC_CLASS(wxIFMPluginEvent)
};

// these classes are unused right now, keeping them incase I use them later
#if 0
    /*!
        \brief Initializes the interface.

        \see wxIFMShutdownEvent
    */
    class WXDLLIMPEXP_IFM wxIFMInitializeEvent : public wxIFMPluginEvent
    {
    public:

        bool m_initialized;

        wxIFMInitializeEvent()
            : wxIFMPluginEvent(wxEVT_IFM_INITIALIZE),
            m_initialized(false)
        { }

        wxIFMInitializeEvent(const wxIFMInitializeEvent &event)
            : wxIFMPluginEvent(event),
            m_initialized(event.m_initialized)
        { }

        virtual wxEvent *Clone() const { return new wxIFMInitializeEvent(*this); }

        /*!
            Specifies whether or not initialization was successful.
            \note Default value is false. Failing to call this function
                will result in initialization failure for the interface plugin
        */
        void SetInitialized(bool initialized = true) { m_initialized = initialized; }

        /*!
            \return True if initialization was successful
        */
        bool GetInitialized() const { return m_initialized; }
    };

    /*!
        \brief Event notifying plugins that the interface is shutting down.

        \see wxIFMInitializeEvent
    */
    class WXDLLIMPEXP_IFM wxIFMShutdownEvent : public wxIFMPluginEvent
    {
    public:

        wxIFMShutdownEvent()
            : wxIFMPluginEvent(wxEVT_IFM_SHUTDOWN)
        { }

        wxIFMShutdownEvent(const wxIFMShutdownEvent &event)
            : wxIFMPluginEvent(event)
        { }

        virtual wxEvent *Clone() const { return new wxIFMShutdownEvent(*this); }
    };
#endif

/*!
    \brief Event class WXDLLIMPEXP_IFM for creating interface components.

    The type of component being created can be determined with GetComponentType().
    Only components of this type should be returned by this event using SetComponent().
    \see wxIFMDeleteComponentEvent
*/
class WXDLLIMPEXP_IFM wxIFMNewComponentEvent : public wxIFMPluginEvent
{
public:

    wxIFMNewComponentEvent(int componentType)
        : wxIFMPluginEvent(wxEVT_IFM_NEWCOMPONENT)
    { m_componentType = componentType; }

    wxIFMNewComponentEvent(const wxIFMNewComponentEvent &event)
        : wxIFMPluginEvent(event)
    { }

    virtual wxEvent *Clone() const { return new wxIFMNewComponentEvent(*this); }

    /*!
        Sets the component to be added to the interface by this event
    */
    void SetComponent(wxIFMComponent *component) { m_component = component; }
};

/*!
    \brief Event class WXDLLIMPEXP_IFM for deleting components.

    This event should be used if a plugin determines that a certain component is no longer required
    and should be deleted. An example of this is when you drag the only panel of a container and dock
    it into another container. The first container now has no children, generates the delete component
    event to delete the itself, and in the case of the default interface plugin, removes the container
    from the top level container list if it was a top level container.

    This event will not be used to cleanup components while shutting down.

    If the component being deleted has children, the childre will also be sent a %wxIFMDeleteComponentEvent,
    and will be deleted with their parent.

    \attention You must not delete the component in response to this event. This event is provided to notify plugins
    that a component is about to be deleted so that they can perform necessary cleanup operations.

    \see wxIFMNewComponentEvent
*/
class WXDLLIMPEXP_IFM wxIFMDeleteComponentEvent : public wxIFMPluginEvent
{
public:

    wxIFMDeleteComponentEvent(wxIFMComponent *component)
        : wxIFMPluginEvent(wxEVT_IFM_DELETECOMPONENT, component)
    { }

    wxIFMDeleteComponentEvent(const wxIFMNewComponentEvent &event)
        : wxIFMPluginEvent(event)
    { }

    virtual wxEvent *Clone() const { return new wxIFMDeleteComponentEvent(*this); }
};

/*!
    \brief Event class WXDLLIMPEXP_IFM for adding child windows to an interface.

    \see wxIFMShowChildEvent
*/
class WXDLLIMPEXP_IFM wxIFMAddChildEvent : public wxIFMPluginEvent
{
protected:
    wxIFMChildDataBase *m_data; //!< data about the child window
    bool m_success;

public:

    wxIFMAddChildEvent(wxIFMChildDataBase *data)
        : wxIFMPluginEvent(wxEVT_IFM_ADDCHILD),
        m_data(data),
        m_success(false)
    { }

    wxIFMAddChildEvent(const wxIFMAddChildEvent &event)
        : wxIFMPluginEvent(event),
        m_data(event.m_data),
        m_success(event.m_success)
    { }

    virtual wxEvent *Clone() const { return new wxIFMAddChildEvent(*this); }

    /*!
        \return Data about the child being added by this event
    */
    wxIFMChildDataBase *GetChildData() const { return m_data; }

    /*!
        Specifies if the child was successfully added
    */
    void SetSuccess(bool s = true) { m_success = s; }

    /*!
        \return If the child was added successfully
    */
    bool GetSuccess() const { return m_success; }
};

/*!
    \brief Event to dock a component into another component.

    Components can be docked in two ways: position or index mode.

    Position mode means that the destination component is provided
    with the requested (x,y) position of the component, and the
    destination decides where the component will end up.

    Index mode means that the destination component is told that
    the component should be in the first, second, or nth position.

    \see wxIFMUndockEvent
*/
class WXDLLIMPEXP_IFM wxIFMDockEvent : public wxIFMPluginEvent
{
protected:
    wxIFMComponent *m_destination; //!< destination component to dock into
    int m_destinationType; //!< type of destination component
    int m_index; //!< position of component within its destination in index mode

public:

    wxIFMDockEvent(wxIFMComponent *component, wxIFMComponent *destination, int index);

    wxIFMDockEvent(const wxIFMDockEvent &event)
        : wxIFMPluginEvent(event),
        m_destination(event.m_destination),
        m_destinationType(event.m_destinationType),
        m_index(event.m_index)
    { }

    virtual wxEvent *Clone() const { return new wxIFMDockEvent(*this); }

    /*!
        \return Destination component to dock into
    */
    wxIFMComponent *GetDestination() const  { return m_destination; }
    void SetDestination(wxIFMComponent *destination);

    /*!
        \return Type of destination component
    */
    int GetDestinationType() const { return m_destinationType; }

    /*!
        \return Position of the component within its destination
    */
    int GetIndex() const { return m_index; }
};

/*!
    \brief Event for undocking components.
*/
class WXDLLIMPEXP_IFM wxIFMUndockEvent : public wxIFMPluginEvent
{
protected:
    int m_parentType; //!< type of parent
    wxIFMComponent *m_parent; //!< parent component
    bool m_delete; //!< if true, the component will be deleted if it has no children left

public:

    wxIFMUndockEvent(wxIFMComponent *component, bool del = true);

    wxIFMUndockEvent(const wxIFMUndockEvent &event)
        : wxIFMPluginEvent(event),
        m_parentType(event.m_parentType),
        m_parent(event.m_parent),
        m_delete(event.m_delete)
    { }

    virtual wxEvent *Clone() const { return new wxIFMUndockEvent(*this); }

    /*!
        \return Parent the component is undocking from
    */
    wxIFMComponent *GetParent() const { return m_parent; }

    /*!
        \return Type of the parent component
    */
    int GetParentType() const { return m_parentType; }

    /*!
        \return True if the component should be deleted if it has no children after
            processing the undock event
    */
    bool GetDelete() const { return m_delete; }
    void SetDelete(bool d) { m_delete = d; }

    void SetParent(wxIFMComponent *parent);
};

/*!
    \brief Used to set the children of components.

    This event is sent during wxIFMAddChildEvent processing and is used to tell a component
    to manage (position and size) the given child window.

    \see wxIFMAddChildEvent, wxIFMShowChildEvent
*/
class WXDLLIMPEXP_IFM wxIFMSetChildEvent : public wxIFMPluginEvent
{
protected:
    wxWindow *m_child; //!< child we are adding
    int m_childType; //!< what type of child is it?

public:

    wxIFMSetChildEvent(wxIFMComponent *component, int childType, wxWindow *child)
        : wxIFMPluginEvent(wxEVT_IFM_SETCHILD, component),
        m_child(child),
        m_childType(childType)
    { }

    wxIFMSetChildEvent(const wxIFMSetChildEvent& event)
        : wxIFMPluginEvent(event),
        m_child(event.m_child),
        m_childType(event.m_childType)
    { }

    virtual wxEvent *Clone() const { return new wxIFMSetChildEvent(*this); }

    /*!
        \return Child being added to the panel
    */
    wxWindow *GetChild() const { return m_child; }

    /*!
        Specifies what type of child it is (Generic, Toolbar, ...)
    */
    int GetChildType() const { return m_childType; }
};

/*!
    \brief Events to set and get absolute, client, and background rects.

    If you would like the absolute rect, construct this event with the
    wxEVT_IFM_GETRECT or wxEVT_IFM_GETABSOLUTERECT event id's. For background
    rect, use wxEVT_IFM_GETBACKGROUNDRECT, and use wxEVT_IFM_GETCLIENTRECT for
    the client rect. The same applies for the SET variants of these events.

    This event is used to get and set all sizes and coordinate system rects for a given component.
    This means that some events will not use the position values of the rect, but instead only
    use the size values. The events that do this are get and set variants of desired size, minimum
    size, and maximum size.

    \see \ref coordsystems, wxIFMConvertRectEvent
*/
class WXDLLIMPEXP_IFM wxIFMRectEvent : public wxIFMPluginEvent
{
protected:
    wxRect m_rect;

public:

    // size / pos ctor
    wxIFMRectEvent(wxEventType id, wxIFMComponent *component, const wxPoint &pos, const wxSize &size)
        : wxIFMPluginEvent(id, component),
        m_rect(pos, size)
    { }

    // rect ctor
    wxIFMRectEvent(wxEventType id, wxIFMComponent *component, const wxRect &rect = wxRect(0,0,0,0))
        : wxIFMPluginEvent(id, component),
        m_rect(rect)
    { }


    wxIFMRectEvent(const wxIFMRectEvent &event)
        : wxIFMPluginEvent(event),
        m_rect(event.m_rect)
    { }

    virtual wxEvent *Clone() const { return new wxIFMRectEvent(*this); }

    wxSize GetSize() const { return m_rect.GetSize(); }
    wxPoint GetPosition() const { return m_rect.GetPosition(); }
    wxRect GetRect() const { return m_rect; }

    void SetSize(const wxSize &s) { m_rect.SetSize(s); }
    void SetPosition(const wxPoint &p) { m_rect.SetPosition(p); }
    void SetRect(const wxRect &r) { m_rect = r; }
};

/*!
    \brief Used to convert between coord systems (absolute, background, client).

    \see \ref coordsystems, wxIFMRectEvent
*/
class WXDLLIMPEXP_IFM wxIFMConvertRectEvent : public wxIFMRectEvent
{
protected:
    //@{
    int m_coordsTo, m_coordsFrom; //!< which coordinate systems are we converting to and from
    //@}

public:

    wxIFMConvertRectEvent(wxIFMComponent *component, int coordsFrom, int coordsTo,
        const wxRect &rect)
        : wxIFMRectEvent(wxEVT_IFM_CONVERTRECT, component, rect),
        m_coordsTo(coordsTo),
        m_coordsFrom(coordsFrom)
    { }

    wxIFMConvertRectEvent(wxIFMComponent *component, int coordsFrom, int coordsTo,
        const wxPoint &pos, const wxSize &size)
        : wxIFMRectEvent(wxEVT_IFM_CONVERTRECT, component, pos, size),
        m_coordsTo(coordsTo),
        m_coordsFrom(coordsFrom)
    { }

    wxIFMConvertRectEvent(const wxIFMConvertRectEvent &event)
        : wxIFMRectEvent(event),
        m_coordsTo(event.m_coordsTo),
        m_coordsFrom(event.m_coordsFrom)
    { }

    virtual wxEvent *Clone() const { return new wxIFMConvertRectEvent(*this); }

    /*!
        \return Coord system to convert from
    */
    int GetSourceCoords() const { return m_coordsFrom; }

    /*!
        \return Coord system to convert to
    */
    int GetDestinationCoords() const { return m_coordsTo; }
};

/*!
    \brief Used to test if a given position is within a given coordinate system for a component.

    \see \ref coordsystems, wxIFMRectEvent, wxIFMConvertRectEvent
*/

class WXDLLIMPEXP_IFM wxIFMHitTestEvent : public wxIFMPluginEvent
{
protected:
    int m_coords; //!< coords (rect) to hit test
    bool m_passed; //!< true if the hit test passed
    const wxPoint &m_pos; //!< position to test in \b screen coordinates

public:
    wxIFMHitTestEvent(wxIFMComponent *component, int coords, const wxPoint &pos)
        : wxIFMPluginEvent(wxEVT_IFM_HITTEST, component),
        m_coords(coords),
        m_passed(false),
        m_pos(pos)
    { }

    wxIFMHitTestEvent(const wxIFMHitTestEvent &event)
        : wxIFMPluginEvent(event),
        m_coords(event.m_coords),
        m_passed(event.m_passed),
        m_pos(event.m_pos)
    { }

    virtual wxEvent *Clone() const { return new wxIFMHitTestEvent(*this); }

    /*!
        \return Which coordinate system (rect) to hit test
    */
    int GetCoords() const { return m_coords; }

    /*!
        \return If hit testing passed or not
    */
    bool GetPassed() const { return m_passed; }
    void SetPassed(bool passed = true) { m_passed = passed; }

    /*!
        \return Position to hit test
    */
    const wxPoint &GetPos() const { return m_pos; }
};

/*!
    \brief Event for updating the interface.

    This event is sent when the interface needs to be updated. It is typically sent
    when the window a wxInterfaceManager object is managing changes size, or when
    wxInterfaceManager::Update() is called.

    \see wxInterfaceManager::Update(), wxIFMUpdateComponentEvent
*/
class WXDLLIMPEXP_IFM wxIFMUpdateEvent : public wxIFMPluginEvent
{
protected:
    wxRect m_rect; //!< dimensions of area managed by interface manager
    wxWindow *m_content; //!< content window of the interface
    bool m_floating; //!< update floating windows as well
    wxIFMComponent *m_component;

public:

    wxIFMUpdateEvent(wxWindow *content, wxRect rect, bool floating)
        : wxIFMPluginEvent(wxEVT_IFM_UPDATE),
        m_rect(rect),
        m_content(content),
        m_floating(floating),
        m_component(NULL)
    { }

    wxIFMUpdateEvent(const wxIFMUpdateEvent &event)
        : wxIFMPluginEvent(event),
        m_rect(event.m_rect),
        m_content(event.m_content),
        m_floating(event.m_floating),
        m_component(event.m_component)
    { }

    virtual wxEvent *Clone() const { return new wxIFMUpdateEvent(*this); }

    /*
        \return Area available for the interface in frame coordinates
    */
    wxRect GetRect() const { return m_rect; }

    /*!
        \return Size available to the interface
    */
    wxSize GetSize() const { return m_rect.GetSize(); }

    /*!
        \return Origin of the interface area in frame coordinates
    */
    wxPoint GetPosition() const { return m_rect.GetPosition(); }

    /*!
        \return Content window of the interface
    */
    wxWindow *GetContent() const { return m_content; }

    /*!
        \return True if floating windows should also be updated
    */
    bool GetFoating() const { return m_floating; }
};

/*!
    \brief Event for updating a component.

    This event is used to set the size and position of a component, and instruct
    the component to update any of its child components or child window.

    \see wxIFMUpdateEvent
*/
class WXDLLIMPEXP_IFM wxIFMUpdateComponentEvent : public wxIFMPluginEvent
{
protected:
    wxRect m_rect; //!< new absolute size and position for the component

public:

    wxIFMUpdateComponentEvent(wxIFMComponent *component, wxRect rect)
        : wxIFMPluginEvent(wxEVT_IFM_UPDATECOMPONENT, component),
        m_rect(rect)
    { }

    wxIFMUpdateComponentEvent(const wxIFMUpdateComponentEvent &event)
        : wxIFMPluginEvent(event),
        m_rect(event.m_rect)
    { }

    virtual wxEvent *Clone() const { return new wxIFMUpdateComponentEvent(*this); }

    /*!
        \return New absolute size of the component
    */
    wxSize GetSize() const { return m_rect.GetSize(); }

    /*!
        \return New absolute position of the component
    */
    wxPoint GetPosition() const { return m_rect.GetPosition(); }

    /*!
        \return New absolute size and position of the component
    */
    wxRect GetRect() const { return m_rect; }
};

/*!
    \brief Used to calculate the sizes and positions of children of a given component.

    Because this event contains the components the parent should size, components that
    are not actually children of this component can have sizes calculated as if they were.
    This is used by the dragndrop plugin to preview the result of a dock operation.

    The constructors rect argument optionally specifies a rectangle to use for the calculations.
    The default argument implies that the components current client rect will be used.

    When this event is sent, m_rects will be initialized with the desired sizes of the components
    contained in m_components. There will be as many elements in m_rects as there are in m_components.
    Only read access is given for m_components, however, partial write access is given to m_rect along
    side write access. While processing this event, you set the size you calculated for a component by
    call SetComponentSize() and passing the index of the component you are setting the size for.

    You add components that you would like to size by this event by calling AddComponent().
*/
class WXDLLIMPEXP_IFM wxIFMCalcRectsEvent : public wxIFMPluginEvent
{
private:
    wxRectArray m_rects;
    wxSizeArray m_min, m_max;
    wxIFMComponentArray m_components;
    wxRect m_rect;

public:
    wxIFMCalcRectsEvent(wxIFMComponent *component, const wxRect &rect = IFM_DEFAULT_RECT);

    /*!
        Sets the component rect for the component at the given index
    */
    void SetComponentRect(int index, const wxRect &rect);

    /*!
        \return The component rect array containing calculated sizes and positions
    */
    const wxRectArray &GetComponentRects();

    /*!
        \return Array containing the minimum sizes of the components being sized
    */
    const wxSizeArray &GetMinSizes();

    /*!
        \return Array containing the maximum sizes of the components being sized
    */
    const wxSizeArray &GetMaxSizes();

    /*!
        Adds a component to be sized by this event. The order in which components are added
        defines the order they will be positioned
    */
    void AddComponent(wxIFMComponent *component);

    /*!
        \return Components being sized
    */
    const wxIFMComponentArray &GetComponents();

    /*!
        \return Rect avaiable to size components in
    */
    const wxRect &GetRect();
};

/*!
    \brief Event to paint components.

    \see wxIFMBeginPaintEvent, wxIFMEndPaintEvent
*/
class WXDLLIMPEXP_IFM wxIFMPaintEvent : public wxIFMPluginEvent
{
protected:
    wxRegion m_region; //!< invalidated region
    wxDC &m_dc; //!< DC to use for painting

public:

    wxIFMPaintEvent(wxEventType id, wxIFMComponent *component, const wxRegion &region, wxDC &dc)
        : wxIFMPluginEvent(id, component),
        m_region(region),
        m_dc(dc)
    { }

    wxIFMPaintEvent(const wxIFMPaintEvent& event)
        : wxIFMPluginEvent(event),
        m_region(event.m_region),
        m_dc(event.m_dc)
    { }

    virtual wxEvent *Clone() const { return new wxIFMPaintEvent(*this); }

    /*!
        \return Invalidated region that should be painted
    */
    wxRegion GetRegion() const { return m_region; }

    /*!
        \return Reference to the DC to paint with
    */
    wxDC &GetDC() const { return m_dc; }
};

/*!
    \brief Notifies plugins that some or all of the interface needs to be repainted.

    \see wxIFMBeginPaintEvent, wxIFMEndPaintEvent, wxIFMPaintEvent
*/
class WXDLLIMPEXP_IFM wxIFMPaintInterfaceEvent : public wxIFMPluginEvent
{
protected:
    const wxRegion &m_region; //!< invalidated region
    wxDC &m_dc; //!< DC to use for painting

public:

    wxIFMPaintInterfaceEvent(wxDC &dc, const wxRegion &region)
        : wxIFMPluginEvent(wxEVT_IFM_PAINTINTERFACE),
        m_region(region),
        m_dc(dc)
    { }

    wxIFMPaintInterfaceEvent(const wxIFMPaintInterfaceEvent& event)
        : wxIFMPluginEvent(event),
        m_region(event.m_region),
        m_dc(event.m_dc)
    { }

    virtual wxEvent *Clone() const { return new wxIFMPaintInterfaceEvent(*this); }

    /*!
        \return Reference to the DC to paint with
    */
    wxDC &GetDC() const { return m_dc; }

    /*!
        \return Region to be updated
    */
    const wxRegion &GetUpdateRegion() const { return m_region; }
};

/*!
    \brief Event used to signify that painting is beginning. The purpose
    of this event is to create a DC on which to paint. It will be cleaned up
    with a wxIFMEndPaintEvent.

    \see wxIFMEndPaintEvent
*/
class WXDLLIMPEXP_IFM wxIFMBeginPaintEvent : public wxIFMPluginEvent
{
protected:
    wxDC *m_dc; //!< dc to use to paint
    bool m_onpaint; //!< true if this event is being sent as the result of a wxEVT_PAINT message
    wxWindow *m_window; //!< window on which to paint

public:

    wxIFMBeginPaintEvent(wxWindow *window, bool onpaint = true)
        : wxIFMPluginEvent(wxEVT_IFM_BEGINPAINT),
        m_dc(NULL),
        m_onpaint(onpaint),
        m_window(window)
    { }

    wxIFMBeginPaintEvent(const wxIFMBeginPaintEvent& event)
        : wxIFMPluginEvent(event),
        m_dc(event.m_dc),
        m_onpaint(event.m_onpaint),
        m_window(event.m_window)
    { }

    virtual wxEvent *Clone() const { return new wxIFMBeginPaintEvent(*this); }

    /*!
        Returns the DC on which to paint
    */
    wxDC *GetDC() const { return m_dc; }

    /*!
        Sets the DC to use to paint
    */
    void SetDC(wxDC *dc) { m_dc = dc; }

    /*!
        \return true if this event is being generated in response to a wxEVT_PAINT message.
    */
    bool GetOnPaint() const { return m_onpaint; }

    /*!
        Returns the window on which to paint
    */
    wxWindow *GetWindow() const { return m_window; }
};

/*!
    \brief Event used to clean up a DC that was created by wxIFMBeginPaintEvent.

    \see wxIFMBeginPaintEvent
*/
class WXDLLIMPEXP_IFM wxIFMEndPaintEvent : public wxIFMPluginEvent
{
protected:
    wxDC *m_dc; //!< dc to use to paint

public:

    wxIFMEndPaintEvent(wxDC *dc)
        : wxIFMPluginEvent(wxEVT_IFM_ENDPAINT),
        m_dc(dc)
    { }

    wxIFMEndPaintEvent(const wxIFMEndPaintEvent& event)
        : wxIFMPluginEvent(event),
        m_dc(event.m_dc)
    { }

    virtual wxEvent *Clone() const { return new wxIFMEndPaintEvent(*this); }

    /*!
        Returns the DC to delete
    */
    wxDC *GetDC() const { return m_dc; }
};

/*!
    \brief Event for keyboard input messages.

    For this event, wxIFMPluginEvent::GetComponent() returns the component that ha captured input,
    or NULL if no component has captured input.

    \see wxIFMMouseEvent, wxIFMSetCursorEvent
*/
class WXDLLIMPEXP_IFM wxIFMKeyEvent : public wxIFMPluginEvent
{
protected:
    wxKeyEvent &m_event; //!< Key event generated by wxWidgets

public:

    wxIFMKeyEvent(wxEventType id, wxIFMComponent *component, wxKeyEvent &event)
        : wxIFMPluginEvent(id, component),
        m_event(event)
    { }

    wxIFMKeyEvent(const wxIFMKeyEvent &event)
        : wxIFMPluginEvent(event),
        m_event(event.m_event)
    { }

    virtual wxEvent *Clone() const { return new wxIFMKeyEvent(*this); }

    /*!
        \return Reference to the wxKeyEvent generated by wxWidgets
    */
    wxKeyEvent &GetKeyEvent() const { return m_event; }
};

/*!
    \brief Event for mouse input messages.

    For this event, wxIFMPluginEvent::GetComponent() returns the component that the mouse
    is hovering over, or NULL if the mouse was not over any component. If a component has
    captured input, GetComponent() will always return that component.

    \see wxIFMKeyEvent, wxIFMSetCursorEvent
*/
class WXDLLIMPEXP_IFM wxIFMMouseEvent : public wxIFMPluginEvent
{
protected:
    wxMouseEvent &m_event; //!< Mouse event generated by wxWidgets

public:

    wxIFMMouseEvent(wxEventType id, wxIFMComponent *component, wxMouseEvent &event)
        : wxIFMPluginEvent(id, component),
        m_event(event)
    { }

    wxIFMMouseEvent(const wxIFMMouseEvent &event)
        : wxIFMPluginEvent(event),
        m_event(event.m_event)
    { }

    virtual wxEvent *Clone() const { return new wxIFMMouseEvent(*this); }

    /*!
        \return Reference to the wxMouseEvent generated by wxWidgets
    */
    wxMouseEvent &GetMouseEvent() const { return m_event; }
};

/*!
    \brief Event for EVT_SET_CURSOR.

    For this event, wxIFMPluginEvent::GetComponent() returns the component that the mouse
    is hovering over, or NULL if the mouse was not over any component. If a component has
    captured input, GetComponent() will always return that component.

    \see wxIFMMouseEvent, wxIFMKeyEvent
*/
class WXDLLIMPEXP_IFM wxIFMSetCursorEvent : public wxIFMPluginEvent
{
protected:
    wxSetCursorEvent &m_event; //!< SetCursor event generated by wxWidgets

public:

    wxIFMSetCursorEvent(wxSetCursorEvent &event, wxIFMComponent *component)
        : wxIFMPluginEvent(wxEVT_IFM_SETCURSOR, component),
        m_event(event)
    { }

    wxIFMSetCursorEvent(const wxIFMSetCursorEvent &event)
        : wxIFMPluginEvent(event),
        m_event(event.m_event)
    { }

    virtual wxEvent *Clone() const { return new wxIFMSetCursorEvent(*this); }

    /*!
        \return Reference to the wxSetCursorEvent generated by wxWidgets
    */
    wxSetCursorEvent &GetCursorEvent() const { return m_event; }
};

/*!
    \brief Generated when the interface's parent window recieves EVT_SETFOCUS or EVT_KILLFOCUS.
*/
class WXDLLIMPEXP_IFM wxIFMFocusEvent : public wxIFMPluginEvent
{
protected:
    wxFocusEvent &m_event; //!< reference to wxwidgets generated focus event

public:

    wxIFMFocusEvent(wxEventType id, wxFocusEvent &event)
        : wxIFMPluginEvent(id),
        m_event(event)
    { }

    wxIFMFocusEvent(const wxIFMFocusEvent &event)
        : wxIFMPluginEvent(event),
        m_event(event.m_event)
    { }

    virtual wxEvent *Clone() const { return new wxIFMFocusEvent(*this); }

    /*!
        \return Reference to the focus event generated by wxwidgets
    */
    wxFocusEvent &GetFocusEvent() const { return m_event; }
};

/*!
    \brief Used to notify plugins that the interface's parent window is [de]activating.
*/
class WXDLLIMPEXP_IFM wxIFMActivateEvent : public wxIFMPluginEvent
{
protected:
    wxActivateEvent &m_event; //!< reference to wxwidgets generated activate event

public:

    wxIFMActivateEvent(wxActivateEvent &event)
        : wxIFMPluginEvent(wxEVT_IFM_ACTIVATE),
        m_event(event)
    { }

    wxIFMActivateEvent(const wxIFMActivateEvent &event)
        : wxIFMPluginEvent(event),
        m_event(event.m_event)
    { }

    virtual wxEvent *Clone() const { return new wxIFMActivateEvent(*this); }

    /*!
        \return Reference to the activate event generated by wxwidgets
    */
    wxActivateEvent &GetActivateEvent() const { return m_event; }
};

/*!
    \brief Base class WXDLLIMPEXP_IFM used for some child window related events

    \note GetComponent will return the component that manages this child window.
        It will be set by the interface plugin sending the event before the event
        is actually sent into the plugin chain.
*/
class WXDLLIMPEXP_IFM wxIFMChildEvent : public wxIFMPluginEvent
{
private:
    friend class WXDLLIMPEXP_IFM wxIFMInterfacePluginBase;
    void SetComponent(wxIFMComponent *c) { m_component = c; }

protected:
    wxWindow *m_child; //!< child being hidden or shown

public:

    wxIFMChildEvent(wxEventType type, wxWindow *child)
        : wxIFMPluginEvent(type),
        m_child(child)
    { }

    wxIFMChildEvent(const wxIFMChildEvent &event)
        : wxIFMPluginEvent(event),
        m_child(event.m_child)
    { }

    virtual wxEvent *Clone() const { return new wxIFMChildEvent(*this); }

    /*!
        \return Pointer ot the child window whos sizes are being changed
    */
    wxWindow *GetChild() const { return m_child; }
};

/*!
    \brief Used to hide or show child windows managed by the interface.

    The default implementation for this event will find the component that
    contains this child, and send that component a wxIFMShowComponentEvent
    in order to hide it.

    You should use wxIFMShowComponentEvent to hide child windows instead of processing
    this event, unless the default wxIFMShowComponentEvent functionality is not adequet
    for your needs. Even if you do require more advanced functionality, there will
    be instances where your component is sent wxIFMShowComponentEvent to hide or show it,
    and you must properly hide or show your child window in this event.

    \see wxIFMAddChildEvent, wxIFMSetChildEvent, wxIFMShowComponentEvent
*/
class WXDLLIMPEXP_IFM wxIFMShowChildEvent : public wxIFMChildEvent
{
protected:
    bool m_show; //!< Show or hide the child
    bool m_update; //!< Update the interface?

public:

    wxIFMShowChildEvent(wxWindow *child, bool show, bool update)
        : wxIFMChildEvent((show ? wxEVT_IFM_SHOWCHILD : wxEVT_IFM_HIDECHILD), child),
        m_show(show),
        m_update(update)
    { }

    wxIFMShowChildEvent(const wxIFMShowChildEvent &event)
        : wxIFMChildEvent(event),
        m_show(event.m_show),
        m_update(event.m_update)
    { }

    virtual wxEvent *Clone() const { return new wxIFMShowChildEvent(*this); }

    /*!
        \return True if the child is being shown, false if beind hidden
    */
    bool GetShow() const { return m_show; }

    /*!
        \return True if the interface should be updated, false if not
    */
    bool GetUpdate() const { return m_update; }
    void SetUpdate(bool update) { m_update = update; }
};

/*!
    \brief Used to query information on a given child window. This event
    is used to determine if a child window is hidden or shown within the
    interface.
*/
class WXDLLIMPEXP_IFM wxIFMQueryChildEvent : public wxIFMChildEvent
{
protected:
    bool m_visible; //!< the child is visible

public:

    wxIFMQueryChildEvent(wxWindow *child)
        : wxIFMChildEvent(wxEVT_IFM_QUERYCHILD, child),
        m_visible(false)
    { }

    wxIFMQueryChildEvent(const wxIFMQueryChildEvent &event)
        : wxIFMChildEvent(event),
        m_visible(event.m_visible)
    { }

    virtual wxEvent *Clone() const { return new wxIFMQueryChildEvent(*this); }

    /*!
        \return True if the child is not hidden.
    */
    bool IsVisible() const { return m_visible; }
    void SetVisible(bool vis) { m_visible = vis; }
};

/*!
    \brief Used to change minimum, maximum, and desired sizes of a child window
*/
class WXDLLIMPEXP_IFM wxIFMSetChildSizeEvent : public wxIFMChildEvent
{
protected:
    wxSize m_min, m_max, m_desired;
    bool m_update;

public:
    wxIFMSetChildSizeEvent(wxWindow *child, const wxSize &desired, const wxSize &min, const wxSize &max, bool update)
        : wxIFMChildEvent(wxEVT_IFM_SETCHILDSIZE, child),
        m_min(min),
        m_max(max),
        m_desired(desired),
        m_update(update)
    { }

    wxIFMSetChildSizeEvent(const wxIFMSetChildSizeEvent &event)
        : wxIFMChildEvent(event),
        m_min(event.m_min),
        m_max(event.m_max),
        m_desired(event.m_desired),
        m_update(event.m_update)
    { }

    virtual wxEvent *Clone() const { return new wxIFMSetChildSizeEvent(*this); }

    /*!
        \return New minimum size of the window

        \note A width or height of IFM_USE_CURRENT_VALUE means the current width or height value should be used
    */
    wxSize GetMinSize() const { return m_min; }

    /*!
        \return New maximum size of the window

        \note A width or height of IFM_USE_CURRENT_VALUE means the current width or height value should be used
    */
    wxSize GetMaxSize() const { return m_max; }

    /*!
        \return New desired size of the window

        \note A width or height of IFM_USE_CURRENT_VALUE means the current width or height value should be used
    */
    wxSize GetDesiredSize() const { return m_desired; }

    /*!
        \return If the interface should be updated
    */
    bool GetUpdate() const { return m_update; }
    void SetUpdate(bool s) { m_update = s; }
};

/*!
    \brief This event is used to show or hide a component.

    This event is used to change the visibility state of a component. Note that this event
    is very different from wxIFMComponentVisibilityChangedEvent. wxIFMComponentVisibilityChangedEvent
    is only a notification that the visibility state has changed. It does not modify the internal state
    variable. When a components parent is hidden while that component is visible, the component recieves
    wxIFMComponentVisibilityChangedEvent notifying it that it is no longer visibile, while the parent recieves
    wxIFMShowComponentEvent to actually hide it, and change its m_hidden member. The m_hidden member
    for the component recieving wxIFMComponentVisibilityChangedEvent does not change.

    While processing this event, it is important to note that wxIFMComponent::m_hidden
    will reflect the old visibility state. After this event is done being processed, the
    wxIFMComponent::Show() function will change the m_hidden member. You should not send this
    event directly, but instead only call wxIFMComponent::Show().

    The default implementation for this event sends wxIFMComponentVisibilityChangedEvent to the component and
    its children. The default implementation will also ensure that this components parents are all visible
    if this component is being shown. If those parents are hidden, Show() will be called on them. Any children
    of this component who are shown, but were not visible because this component was hidden, will recieve
    wxIFMComponentVisibilityChangedEvent events.

    \see wxIFMComponentVisibilityChangedEvent
*/
class WXDLLIMPEXP_IFM wxIFMShowComponentEvent : public wxIFMPluginEvent
{
protected:
    bool m_show, m_update;

public:
    wxIFMShowComponentEvent(wxIFMComponent *component, bool show, bool update)
        : wxIFMPluginEvent(wxEVT_IFM_SHOWCOMPONENT, component),
        m_show(show),
        m_update(update)
    { }

    wxIFMShowComponentEvent(const wxIFMShowComponentEvent &event)
        : wxIFMPluginEvent(event),
        m_show(event.m_show),
        m_update(event.m_update)
    { }

    virtual wxEvent *Clone() const { return new wxIFMShowComponentEvent(*this); }

    /*!
        \return Wether or not the component is being hidden or shown
    */
    bool GetShow() const { return m_show; }

    /*!
        \return If the interface should be updated
    */
    bool GetUpdate() const { return m_update; }
    void SetUpdate(bool s) { m_update = s; }
};

/*!
    \brief Notifies a component that its visibility state is changed.

    This event will be sent when a component is visible, and its parent is either hidden,
    or shown. It is only a notification that the visibility state of this component has changed,
    it does not actually change the internal visibility state flag for the component.

    You should process this event if you need to hide or show other windows in response
    to this component being hidden or shown, not wxIFMShowComponentEvent.

    \note While processing this event. wxIFMComponent::m_visible will reflect the old visibility state.
    After this event has been processed, wxIFMComponent::m_visible will be updated to reflect the new state.
    This is performed by wxIFMComponent::VisibilityChanged(); you should not send this event directly, but instead
    use this function.

    The default implementation of this event sends a wxIFMComponentVisibilityChangedEvent notification
    to all of this component's children.

    To actually hide a component, use wxIFMShowComponentEvent

    \see wxIFMShowComponentEvent
*/
class WXDLLIMPEXP_IFM wxIFMComponentVisibilityChangedEvent : public wxIFMPluginEvent
{
protected:
    bool m_show;

public:
    wxIFMComponentVisibilityChangedEvent(wxIFMComponent *component, bool show)
        : wxIFMPluginEvent(wxEVT_IFM_VISIBILITYCHANGED, component),
        m_show(show)
    { }

    wxIFMComponentVisibilityChangedEvent(const wxIFMComponentVisibilityChangedEvent &event)
        : wxIFMPluginEvent(event),
        m_show(event.m_show)
    { }

    virtual wxEvent *Clone() const { return new wxIFMComponentVisibilityChangedEvent(*this); }

    /*!
        \return Wether or not the component is being hidden or shown
    */
    bool GetShow() const { return m_show; }
};

/*!
    This event is used to notify plugins that a configurable option has been changed.
    Plugins which expose configurable settings should perform any operations necessary
    to honor the new settings. Plugins should assume that all settings have been changed
    and perform the operations necessary to honor all settings as if they had been changed.
*/
class WXDLLIMPEXP_IFM wxIFMUpdateConfigEvent : public wxIFMPluginEvent
{
public:
    wxIFMUpdateConfigEvent()
        : wxIFMPluginEvent(wxEVT_IFM_UPDATECONFIG, 0)
    { }

    wxIFMUpdateConfigEvent(const wxIFMUpdateConfigEvent &event)
        : wxIFMPluginEvent(event)
    { }

    virtual wxEvent *Clone() const { return new wxIFMUpdateConfigEvent(*this); }
};

#if IFM_CANFLOAT

/*!
    \brief Used to float or unfloat components

    This event is sent during wxIFMFloatEvent processing, and should create
    a floating window for the desired component and dock the component
    into this floating window.

    \note The component returned by GetComponent() is the component that
    is being floated by the wxIFMFloatEvent

    \see wxIFMFloatEvent, wxIFMFloatNotifyEvent, wxIFMCreateFloatingWindowEvent
*/
class WXDLLIMPEXP_IFM wxIFMFloatEvent : public wxIFMPluginEvent
{
protected:
    wxRect m_rect; //!< Specifies the client rect of the floating window
	bool bConvertCoordinates; //maks

public:

    wxIFMFloatEvent(wxIFMComponent* component, const wxRect &rect = wxRect(0,0,0,0), bool _bConvertCoordinates = true) //maks
        : wxIFMPluginEvent(wxEVT_IFM_FLOAT, component),
        m_rect(rect),
		bConvertCoordinates(_bConvertCoordinates) //maks
    { }

    wxIFMFloatEvent(wxIFMComponent* component, const wxPoint &pos, const wxSize &size, bool _bConvertCoordinates = true)
        : wxIFMPluginEvent(wxEVT_IFM_FLOAT, component),
        m_rect(pos, size),
		bConvertCoordinates(_bConvertCoordinates) //maks
    { }

    wxIFMFloatEvent(const wxIFMFloatEvent &event)
        : wxIFMPluginEvent(event),
        m_rect(event.m_rect),
		bConvertCoordinates(event.bConvertCoordinates) //maks
    { }

    virtual wxEvent *Clone() const { return new wxIFMFloatEvent(*this); }

    /*!
        This function returns a position and size value that describe the size and position
        that the floating window should occupy. This value should be interpretted as follows.\n
        \n
        The \b Size of the returned rect specifies the absolute size that the component being
        floating should occupy. You must modify the actual size of the floating window to account
        for the margins and borders of the root component in order to gaurentee that the component
        being floated is exactly as large as this value.
        \n
        The \Position of the returned rect specifies the position that the component being floated
        should be located in screen coordinates. You must modify the position of the floating window
        such that any borders or caption of the floating window, and any margins and borders of the
        root component are compensated so that the component being floated appears exactly at the
        specified position.
    */
    wxRect GetRect() const { return m_rect; }

    /*!
        \return Position of the frame in screen coordinates
    */
    wxPoint GetPosition() const { return m_rect.GetPosition(); }

    /*!
        \return Size of the frame's client area
    */
    wxSize GetSize() const { return m_rect.GetSize(); }

	bool GetConvertCoordinates() {return bConvertCoordinates;}
};

/*!
    \brief Used to create a window to float a component.

    This event is resonsible for creating a window to float the component in,
    createing the root component of that floating window, and docking the component
    into the root component of the window. It should not do anything else.

    \see wxIFMDestroyFloatingWindowEvent
*/
class WXDLLIMPEXP_IFM wxIFMCreateFloatingWindowEvent : public wxIFMPluginEvent
{
protected:
    wxIFMFloatingWindowBase *m_window;
    wxWindow *m_parent;

public:

    wxIFMCreateFloatingWindowEvent(wxWindow *parent, wxIFMComponent *component)
        : wxIFMPluginEvent(wxEVT_IFM_CREATEFLOATINGWINDOW, component),
        m_window(NULL),
        m_parent(parent)
    { }

    wxIFMCreateFloatingWindowEvent(const wxIFMCreateFloatingWindowEvent &event)
        : wxIFMPluginEvent(event),
        m_window(event.m_window),
        m_parent(event.m_parent)
    { }

    virtual wxEvent *Clone() const { return new wxIFMCreateFloatingWindowEvent(*this); }

    /*!
        Set the window to be used to float the component
    */
    void SetWindow(wxIFMFloatingWindowBase *window) { m_window = window; }

    /*!
        \return The window specified by this event
    */
    wxIFMFloatingWindowBase *GetWindow() const { return m_window; }

    /*!
        \return Parent window used to create the floating window
    */
    wxWindow *GetParent() const { return m_parent; }
};

/*!
    \brief Used to delete floating windows.

    \see wxIFMCreateFloatingWindowEvent
*/
class WXDLLIMPEXP_IFM wxIFMDestroyFloatingWindowEvent : public wxIFMPluginEvent
{
protected:
    wxIFMFloatingWindowBase *m_window;
    bool m_destroyRootComponent;

public:

    wxIFMDestroyFloatingWindowEvent(wxIFMFloatingWindowBase *window, bool destroy = true)
        : wxIFMPluginEvent(wxEVT_IFM_DESTROYFLOATINGWINDOW),
        m_window(window),
        m_destroyRootComponent(destroy)
    { }

    wxIFMDestroyFloatingWindowEvent(const wxIFMDestroyFloatingWindowEvent &event)
        : wxIFMPluginEvent(event),
        m_window(event.m_window),
        m_destroyRootComponent(event.m_destroyRootComponent)
    { }

    virtual wxEvent *Clone() const { return new wxIFMDestroyFloatingWindowEvent(*this); }

    /*!
        \return The window specified by this event
    */
    wxIFMFloatingWindowBase *GetWindow() const { return m_window; }

    /*!
        \return True if the root component should be destroyed
    */
    bool ShouldDestroyRoot() const { return m_destroyRootComponent; }
};

/*!
    \brief Event to signify that a floating window has been sized or is being sized.
*/
class WXDLLIMPEXP_IFM wxIFMFloatingSizeEvent : public wxIFMPluginEvent
{
protected:
    wxSizeEvent &m_event;
    wxIFMFloatingWindowBase *m_window;

public:

    wxIFMFloatingSizeEvent(wxEventType type, wxIFMFloatingWindowBase *window, wxSizeEvent &event)
        : wxIFMPluginEvent(type),
        m_event(event),
        m_window(window)
    { }

    wxIFMFloatingSizeEvent(const wxIFMFloatingSizeEvent& event)
        : wxIFMPluginEvent(event),
        m_event(event.m_event),
        m_window(event.m_window)
    { }

    virtual wxEvent *Clone() const { return new wxIFMFloatingSizeEvent(*this); }

    /*!
        \return Pointer to window generating event
    */
    wxIFMFloatingWindowBase *GetWindow() const { return m_window; }

    /*!
        \return Reference to the wxWidgets generated wxSizeEvent
    */
    wxSizeEvent &GetSizeEvent() const { return m_event; }
};

/*!
    \brief Event to signify that a floating window has been moved or is being moved.
*/
class WXDLLIMPEXP_IFM wxIFMFloatingMoveEvent : public wxIFMPluginEvent
{
protected:
    wxMoveEvent &m_event;
    wxIFMFloatingWindowBase *m_window;

public:

    wxIFMFloatingMoveEvent(wxEventType type, wxIFMFloatingWindowBase *window, wxMoveEvent &event)
        : wxIFMPluginEvent(type),
        m_event(event),
        m_window(window)
    { }

    wxIFMFloatingMoveEvent(const wxIFMFloatingMoveEvent& event)
        : wxIFMPluginEvent(event),
        m_event(event.m_event),
        m_window(event.m_window)
    { }

    virtual wxEvent *Clone() const { return new wxIFMFloatingMoveEvent(*this); }

    /*!
        \return Pointer to window generating event
    */
    wxIFMFloatingWindowBase *GetWindow() const { return m_window; }

    /*!
        \return Reference to the wxWidgets generated wxMoveEvent
    */
    wxMoveEvent &GetMoveEvent() const { return m_event; }
};

/*!
    \brief Used to notify a component that it has been docking into a floating window.

    Note that this is different from wxIFMFloatEvent as %wxIFMFloatEvent
    actually floats a component and creates its floating window, while
    %wxIFMFloatNotifyEvent simply "tells" a component that it has been floated.

    During %wxIFMFloatEvent processing, this event is sent to a compoment after
    the floating window has been created, and the component has been docked into
    the floating windows root container. This is the time to call wxWindow::Reparent
    on child windows managed by panels for example. After this event is sent, the floating
    window will send an wxIFMUpdateComponentEvent to the floating windows root container
    which will size any components contained in the floating window. Your component must
    for this event after handling this event.
*/
class WXDLLIMPEXP_IFM wxIFMFloatNotifyEvent : public wxIFMPluginEvent
{
protected:
    wxIFMFloatingWindowBase *m_window;
    bool m_floating;

public:

    /*!
        \param floating True if the component has been floated, false if unfloated
        \param window Pointer to the window the compone thas bene floated into, or NULL
            if floating is false.
    */
    wxIFMFloatNotifyEvent(wxIFMComponent *component, bool floating, wxIFMFloatingWindowBase *window = NULL)
        : wxIFMPluginEvent(wxEVT_IFM_FLOATING_NOTIFY, component),
        m_window(window),
        m_floating(floating)
    { }

    wxIFMFloatNotifyEvent(const wxIFMFloatNotifyEvent &event)
        : wxIFMPluginEvent(event),
        m_window(event.m_window),
        m_floating(event.m_floating)
    { }

    virtual wxEvent *Clone() const { return new wxIFMFloatNotifyEvent(*this); }

    /*!
        \return Pointer to the window the compone thas bene floated into, or NULL
            if floating is false.
    */
    wxIFMFloatingWindowBase *GetWindow() const { return m_window; }

    /*!
        \return True if the component has been floated, false if unfloated
    */
    bool GetFloating() const { return m_floating; }
};

#endif

typedef void (wxEvtHandler::*wxIFMNewComponentEventFunction)(wxIFMNewComponentEvent&);
typedef void (wxEvtHandler::*wxIFMDeleteComponentEventFunction)(wxIFMDeleteComponentEvent&);
typedef void (wxEvtHandler::*wxIFMAddChildEventFunction)(wxIFMAddChildEvent&);
typedef void (wxEvtHandler::*wxIFMSetChildEventFunction)(wxIFMSetChildEvent&);

/*
typedef void (wxEvtHandler::*wxIFMInitializeEventFunction)(wxIFMInitializeEvent&);
typedef void (wxEvtHandler::*wxIFMShutdownEventFunction)(wxIFMShutdownEvent&);
*/

typedef void(wxEvtHandler::*wxIFMUpdateConfigEventFunction)(wxIFMUpdateConfigEvent&);

typedef void (wxEvtHandler::*wxIFMDockEventFunction)(wxIFMDockEvent&);
typedef void (wxEvtHandler::*wxIFMUndockEventFunction)(wxIFMUndockEvent&);

typedef void (wxEvtHandler::*wxIFMShowComponentEventFunction)(wxIFMShowComponentEvent&);
typedef void (wxEvtHandler::*wxIFMComponentVisibilityChangedEventFunction)(wxIFMComponentVisibilityChangedEvent&);

typedef void (wxEvtHandler::*wxIFMShowChildEventFunction)(wxIFMShowChildEvent&);
typedef void (wxEvtHandler::*wxIFMSetChildSizeEventFunction)(wxIFMSetChildSizeEvent&);
typedef void (wxEvtHandler::*wxIFMQueryChildEventFunction)(wxIFMQueryChildEvent&);

typedef void (wxEvtHandler::*wxIFMRectEventFunction)(wxIFMRectEvent&);
typedef void (wxEvtHandler::*wxIFMConvertRectEventFunction)(wxIFMConvertRectEvent&);
typedef void (wxEvtHandler::*wxIFMHitTestEventFunction)(wxIFMHitTestEvent&);

typedef void (wxEvtHandler::*wxIFMUpdateEventFunction)(wxIFMUpdateEvent&);
typedef void (wxEvtHandler::*wxIFMUpdateComponentEventFunction)(wxIFMUpdateComponentEvent&);
typedef void (wxEvtHandler::*wxIFMCalcRectsEventFunction)(wxIFMCalcRectsEvent&);

typedef void (wxEvtHandler::*wxIFMPaintInterfaceEventFunction)(wxIFMPaintInterfaceEvent&);
typedef void (wxEvtHandler::*wxIFMPaintEventFunction)(wxIFMPaintEvent&);
typedef void (wxEvtHandler::*wxIFMBeginPaintEventFunction)(wxIFMBeginPaintEvent&);
typedef void (wxEvtHandler::*wxIFMEndPaintEventFunction)(wxIFMEndPaintEvent&);

typedef void (wxEvtHandler::*wxIFMKeyEventFunction)(wxIFMKeyEvent&);
typedef void (wxEvtHandler::*wxIFMMouseEventFunction)(wxIFMMouseEvent&);
typedef void (wxEvtHandler::*wxIFMSetCursorEventFunction)(wxIFMSetCursorEvent&);
typedef void (wxEvtHandler::*wxIFMFocusEventFunction)(wxIFMFocusEvent&);
typedef void (wxEvtHandler::*wxIFMActivateEventFunction)(wxIFMActivateEvent&);

#if IFM_CANFLOAT
typedef void (wxEvtHandler::*wxIFMFloatingSizeEventFunction)(wxIFMFloatingSizeEvent&);
typedef void (wxEvtHandler::*wxIFMFloatingMoveEventFunction)(wxIFMFloatingMoveEvent&);
typedef void (wxEvtHandler::*wxIFMFloatNotifyEventFunction)(wxIFMFloatNotifyEvent&);
typedef void (wxEvtHandler::*wxIFMFloatEventFunction)(wxIFMFloatEvent&);
typedef void (wxEvtHandler::*wxIFMCreateFloatingWindowEventFunction)(wxIFMCreateFloatingWindowEvent&);
typedef void (wxEvtHandler::*wxIFMDestroyFloatingWindowEventFunction)(wxIFMDestroyFloatingWindowEvent&);
#endif

#define wxIFMNewComponentEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMNewComponentEventFunction, &func)
#define wxIFMDeleteComponentEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMDeleteComponentEventFunction, &func)
#define wxIFMAddChildEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMAddChildEventFunction, &func)
#define wxIFMSetChildEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMSetChildEventFunction, &func)
/*
#define wxIFMInitializeEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMInitializeEventFunction, &func)
#define wxIFMShutdownEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMShutdownEventFunction, &func)
*/

#define wxIFMUpdateConfigEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMUpdateConfigEventFunction, &func)

#define wxIFMDockEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMDockEventFunction, &func)
#define wxIFMUndockEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMUndockEventFunction, &func)

#define wxIFMShowComponentEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMShowComponentEventFunction, &func)
#define wxIFMComponentVisibilityChangedEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMComponentVisibilityChangedEventFunction, &func)

#define wxIFMShowChildEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMShowChildEventFunction, &func)
#define wxIFMSetChildSizeEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMSetChildSizeEventFunction, &func)
#define wxIFMQueryChildEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMQueryChildEventFunction, &func)

#define wxIFMRectEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMRectEventFunction, &func)
#define wxIFMConvertRectEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMConvertRectEventFunction, &func)
#define wxIFMHitTestEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMHitTestEventFunction, &func)

#define wxIFMUpdateEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMUpdateEventFunction, &func)
#define wxIFMUpdateComponentEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMUpdateComponentEventFunction, &func)
#define wxIFMCalcRectsEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMCalcRectsEventFunction, &func)

#define wxIFMPaintInterfaceEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMPaintInterfaceEventFunction, &func)
#define wxIFMPaintEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMPaintEventFunction, &func)
#define wxIFMBeginPaintEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMBeginPaintEventFunction, &func)
#define wxIFMEndPaintEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMEndPaintEventFunction, &func)

#define wxIFMKeyEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMKeyEventFunction, &func)
#define wxIFMMouseEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMMouseEventFunction, &func)
#define wxIFMSetCursorEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMSetCursorEventFunction, &func)
#define wxIFMFocusEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMFocusEventFunction, &func)
#define wxIFMActivateEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMActivateEventFunction, &func)

#if IFM_CANFLOAT
#define wxIFMFloatingSizeEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMFloatingSizeEventFunction, &func)
#define wxIFMFloatingMoveEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMFloatingMoveEventFunction, &func)
#define wxIFMFloatNotifyEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMFloatNotifyEventFunction, &func)
#define wxIFMFloatEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMFloatEventFunction, &func)
#define wxIFMCreateFloatingWindowEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMCreateFloatingWindowEventFunction, &func)
#define wxIFMDestroyFloatingWindowEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMDestroyFloatingWindowEventFunction, &func)
#endif

#define EVT_IFM_NEWCOMPONENT(func)  wx__DECLARE_EVT0( wxEVT_IFM_NEWCOMPONENT, wxIFMNewComponentEventFunctionHandler(func))
#define EVT_IFM_DELETECOMPONENT(func) wx__DECLARE_EVT0( wxEVT_IFM_DELETECOMPONENT, wxIFMDeleteComponentEventFunctionHandler(func))
#define EVT_IFM_ADDCHILD(func)      wx__DECLARE_EVT0( wxEVT_IFM_ADDCHILD, wxIFMAddChildEventFunctionHandler(func))
#define EVT_IFM_SETCHILD(func)      wx__DECLARE_EVT0( wxEVT_IFM_SETCHILD, wxIFMSetChildEventFunctionHandler(func))

#define EVT_IFM_DOCK(func)          wx__DECLARE_EVT0( wxEVT_IFM_DOCK, wxIFMDockEventFunctionHandler(func))
#define EVT_IFM_UNDOCK(func)        wx__DECLARE_EVT0( wxEVT_IFM_UNDOCK, wxIFMUndockEventFunctionHandler(func))
#define EVT_IFM_FLOAT(func)         wx__DECLARE_EVT0( wxEVT_IFM_FLOAT, wxIFMFloatEventFunctionHandler(func))
#define EVT_IFM_UNFLOAT(func)       wx__DECLARE_EVT0( wxEVT_IFM_UNFLOAT, wxIFMFloatEventFunctionHandler(func))

#define EVT_IFM_SHOWCOMPONENT(func) wx__DECLARE_EVT0( wxEVT_IFM_SHOWCOMPONENT, wxIFMShowComponentEventFunctionHandler(func))
#define EVT_IFM_VISIBILITYCHANGED(func) wx__DECLARE_EVT0( wxEVT_IFM_VISIBILITYCHANGED, wxIFMComponentVisibilityChangedEventFunctionHandler(func))

#define EVT_IFM_SHOWCHILD(func)     wx__DECLARE_EVT0( wxEVT_IFM_SHOWCHILD, wxIFMShowChildEventFunctionHandler(func))
#define EVT_IFM_HIDECHILD(func)     wx__DECLARE_EVT0( wxEVT_IFM_HIDECHILD, wxIFMShowChildEventFunctionHandler(func))
#define EVT_IFM_SETCHILDSIZE(func)  wx__DECLARE_EVT0( wxEVT_IFM_SETCHILDSIZE, wxIFMSetChildSizeEventFunctionHandler(func))
#define EVT_IFM_QUERYCHILD(func)    wx__DECLARE_EVT0( wxEVT_IFM_QUERYCHILD, wxIFMQueryChildEventFunctionHandler(func))

// rect events
#define EVT_IFM_SETRECT(func)       wx__DECLARE_EVT0( wxEVT_IFM_SETRECT, wxIFMRectEventFunctionHandler(func))
#define EVT_IFM_SETBACKGROUNDRECT(func) wx__DECLARE_EVT0( wxEVT_IFM_SETBACKGROUNDRECT, wxIFMRectEventFunctionHandler(func))
#define EVT_IFM_SETCLIENTRECT(func) wx__DECLARE_EVT0( wxEVT_IFM_SETCLIENTRECT, wxIFMRectEventFunctionHandler(func))
#define EVT_IFM_GETRECT(func)       wx__DECLARE_EVT0( wxEVT_IFM_GETRECT, wxIFMRectEventFunctionHandler(func))
#define EVT_IFM_GETBACKGROUNDRECT(func) wx__DECLARE_EVT0( wxEVT_IFM_GETBACKGROUNDRECT, wxIFMRectEventFunctionHandler(func))
#define EVT_IFM_GETCLIENTRECT(func) wx__DECLARE_EVT0( wxEVT_IFM_GETCLIENTRECT, wxIFMRectEventFunctionHandler(func))
#define EVT_IFM_CONVERTRECT(func)   wx__DECLARE_EVT0( wxEVT_IFM_CONVERTRECT, wxIFMConvertRectEventFunctionHandler(func))
#define EVT_IFM_HITTEST(func)       wx__DECLARE_EVT0( wxEVT_IFM_HITTEST, wxIFMHitTestEventFunctionHandler(func))
#define EVT_IFM_GETCONTENTRECT(func) wx__DECLARE_EVT0( wxEVT_IFM_GETCONTENTRECT, wxIFMRectEventFunctionHandler(func))

#define EVT_IFM_UPDATE(func)        wx__DECLARE_EVT0( wxEVT_IFM_UPDATE, wxIFMUpdateEventFunctionHandler(func))
#define EVT_IFM_UPDATECOMPONENT(func) wx__DECLARE_EVT0( wxEVT_IFM_UPDATECOMPONENT, wxIFMUpdateComponentEventFunctionHandler(func))
#define EVT_IFM_CALCRECTS(func)     wx__DECLARE_EVT0( wxEVT_IFM_CALCRECTS, wxIFMCalcRectsEventFunctionHandler(func))

#define EVT_IFM_UPDATECONFIG(func)  wx__DECLARE_EVT0( wxEVT_IFM_UPDATECONFIG, wxIFMUpdateConfigEventFunctionHandler(func))

#define EVT_IFM_GETDESIREDSIZE(func) wx__DECLARE_EVT0( wxEVT_IFM_GETDESIREDSIZE, wxIFMRectEventFunctionHandler(func))
#define EVT_IFM_SETDESIREDSIZE(func) wx__DECLARE_EVT0( wxEVT_IFM_SETDESIREDSIZE, wxIFMRectEventFunctionHandler(func))
#define EVT_IFM_GETMINSIZE(func)    wx__DECLARE_EVT0( wxEVT_IFM_GETMINSIZE, wxIFMRectEventFunctionHandler(func))
#define EVT_IFM_SETMINSIZE(func)    wx__DECLARE_EVT0( wxEVT_IFM_SETMINSIZE, wxIFMRectEventFunctionHandler(func))
#define EVT_IFM_GETMAXSIZE(func)    wx__DECLARE_EVT0( wxEVT_IFM_GETMAXSIZE, wxIFMRectEventFunctionHandler(func))
#define EVT_IFM_SETMAXSIZE(func)    wx__DECLARE_EVT0( wxEVT_IFM_SETMAXSIZE, wxIFMRectEventFunctionHandler(func))

// paint events
/*
Paint events are sent in this order:
background, border, decorations
*/

#define EVT_IFM_BEGINPAINT(func)    wx__DECLARE_EVT0( wxEVT_IFM_BEGINPAINT, wxIFMBeginPaintEventFunctionHandler(func))
#define EVT_IFM_ENDPAINT(func)      wx__DECLARE_EVT0( wxEVT_IFM_ENDPAINT, wxIFMEndPaintEventFunctionHandler(func))
#define EVT_IFM_PAINTINTERFACE(func) wx__DECLARE_EVT0( wxEVT_IFM_PAINTINTERFACE, wxIFMPaintInterfaceEventFunctionHandler(func))
#define EVT_IFM_PAINTBG(func)       wx__DECLARE_EVT0( wxEVT_IFM_PAINTBG, wxIFMPaintEventFunctionHandler(func))
#define EVT_IFM_PAINTBORDER(func)   wx__DECLARE_EVT0( wxEVT_IFM_PAINTBORDER, wxIFMPaintEventFunctionHandler(func))
#define EVT_IFM_PAINTDECOR(func)    wx__DECLARE_EVT0( wxEVT_IFM_PAINTDECOR, wxIFMPaintEventFunctionHandler(func))

// input events
#define EVT_IFM_KEYDOWN(func)       wx__DECLARE_EVT0( wxEVT_IFM_KEYDOWN, wxIFMKeyEventFunctionHandler(func))
#define EVT_IFM_KEYUP(func)         wx__DECLARE_EVT0( wxEVT_IFM_KEYUP, wxIFMKeyEventFunctionHandler(func))

#define EVT_IFM_LEFTDOWN(func)      wx__DECLARE_EVT0( wxEVT_IFM_LEFTDOWN, wxIFMMouseEventFunctionHandler(func))
#define EVT_IFM_LEFTUP(func)        wx__DECLARE_EVT0( wxEVT_IFM_LEFTUP, wxIFMMouseEventFunctionHandler(func))
#define EVT_IFM_LEFTDCLICK(func)    wx__DECLARE_EVT0( wxEVT_IFM_LEFTDCLICK, wxIFMMouseEventFunctionHandler(func))
#define EVT_IFM_RIGHTDOWN(func)     wx__DECLARE_EVT0( wxEVT_IFM_RIGHTDOWN, wxIFMMouseEventFunctionHandler(func))
#define EVT_IFM_RIGHTUP(func)       wx__DECLARE_EVT0( wxEVT_IFM_RIGHTUP, wxIFMMouseEventFunctionHandler(func))
#define EVT_IFM_RIGHTDCLICK(func)   wx__DECLARE_EVT0( wxEVT_IFM_RIGHTDCLICK, wxIFMMouseEventFunctionHandler(func))
#define EVT_IFM_MIDDLEDOWN(func)    wx__DECLARE_EVT0( wxEVT_IFM_MIDDLEDOWN, wxIFMMouseEventFunctionHandler(func))
#define EVT_IFM_MIDDLEUP(func)      wx__DECLARE_EVT0( wxEVT_IFM_MIDDLEUP, wxIFMMouseEventFunctionHandler(func))
#define EVT_IFM_MIDDLEDCLICK(func)  wx__DECLARE_EVT0( wxEVT_IFM_MIDDLEDCLICK, wxIFMMouseEventFunctionHandler(func))
#define EVT_IFM_MOTION(func)        wx__DECLARE_EVT0( wxEVT_IFM_MOTION, wxIFMMouseEventFunctionHandler(func))
#define EVT_IFM_MOUSEWHEEL(func)    wx__DECLARE_EVT0( wxEVT_IFM_MOUSEWHEEL, wxIFMMouseEventFunctionHandler(func))

#define EVT_IFM_SETCURSOR(func)     wx__DECLARE_EVT0( wxEVT_IFM_SETCURSOR, wxIFMSetCursorEventFunctionHandler(func))
#define EVT_IFM_SETFOCUS(func)      wx__DECLARE_EVT0( wxEVT_IFM_SETFOCUS, wxIFMFocusEventFunctionHandler(func))
#define EVT_IFM_KILLFOCUS(func)     wx__DECLARE_EVT0( wxEVT_IFM_KILLFOCUS, wxIFMFocusEventFunctionHandler(func))
#define EVT_IFM_ACTIVATE(func)      wx__DECLARE_EVT0( wxEVT_IFM_ACTIVATE, wxIFMActivateEventFunctionHandler(func))

// floating events
#if IFM_CANFLOAT
#define EVT_IFM_CREATEFLOATINGWINDOW(func) wx__DECLARE_EVT0( wxEVT_IFM_CREATEFLOATINGWINDOW, wxIFMCreateFloatingWindowEventFunctionHandler(func ))
#define EVT_IFM_DESTROYFLOATINGWINDOW(func) wx__DECLARE_EVT0( wxEVT_IFM_DESTROYFLOATINGWINDOW, wxIFMDestroyFloatingWindowEventFunctionHandler(func ))
#define EVT_IFM_FLOATING_SIZE(func)     wx__DECLARE_EVT0( wxEVT_IFM_FLOATING_SIZE, wxIFMFloatingSizeEventFunctionHandler(func ))
#define EVT_IFM_FLOATING_SIZING(func)   wx__DECLARE_EVT0( wxEVT_IFM_FLOATING_SIZING, wxIFMFloatingSizeEventFunctionHandler(func ))
#define EVT_IFM_FLOATING_MOVE(func)     wx__DECLARE_EVT0( wxEVT_IFM_FLOATING_MOVE, wxIFMFloatingMoveEventFunctionHandler(func ))
#define EVT_IFM_FLOATING_MOVING(func)   wx__DECLARE_EVT0( wxEVT_IFM_FLOATING_MOVING, wxIFMFloatingMoveEventFunctionHandler(func ))
#define EVT_IFM_FLOATING_NOTIFY(func)   wx__DECLARE_EVT0( wxEVT_IFM_FLOATING_NOTIFY, wxIFMFloatNotifyEventFunctionHandler(func ))
#endif

#endif
