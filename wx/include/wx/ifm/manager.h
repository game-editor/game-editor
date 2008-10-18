/*!
    Defines core components of the IFM system

    \file   manager.h
    \author Robin McNeill
    \date   Created: 10/15/04

    Copyright (c) Robin McNeill
    Licensed under the terms of the wxWindows license
*/
#ifndef _IFM_MANAGER_H_
#define _IFM_MANAGER_H_

#include "wx/ifm/ifmdefs.h"

#include "wx/window.h"
#include "wx/statusbr.h"

/*!
    \return New unique data key for component data
*/
wxIFMComponentDataKeyType GetNewDataKey();

/*!
    \return New unique identifier for a component type
*/
WXDLLIMPEXP_IFM int GetNewComponentType();

/*!
    \return New unique identifier for a child type
*/
WXDLLIMPEXP_IFM int GetNewChildType();

DECLARE_IFM_DATA_KEY(IFM_FLOATING_DATA_KEY)

#if IFM_CANFLOAT

/*!
    Base class for floating windows.
*/
class WXDLLIMPEXP_IFM wxIFMFloatingWindowBase : public wxEvtHandler
{
private:
    wxIFMInterfacePluginBase *m_ip;

protected:
    wxWindow *m_window;
    wxIFMInterfacePluginBase *GetIP();
    wxInterfaceManager *GetManager();

public:
    wxIFMComponent *m_component;
    bool m_destroyRoot;

protected:
    wxIFMFloatingWindowBase(wxIFMInterfacePluginBase *ip);

    void ConnectEvents();
    void DisconnectEvents();

    /*!
        Return the component at the given position or NULL if none found within the children
        of the given component and the component itself.

        \param pos Position in <b>managed window client coordinates</b>. The managed window
            is the parent of the wxInterfaceManager that owns this plugin.
        \param component If NULL, the components array will be searched. If non null, only
            the given component and its children will be searched.
        \param floating If true, floating windows will be searched first. This parameter
            does nothing if the current platform does not support floating.
    */
    /*!
        This function returns the component at the given position
        \param pos Position in client coordinates
    */
    wxIFMComponent *GetComponentByPos(const wxPoint &pos, wxIFMComponent *component = NULL);

public:
    wxIFMFloatingWindowBase(wxIFMInterfacePluginBase *ip,
        wxWindow *parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0,
        const wxString& name = wxPanelNameStr);

    virtual ~wxIFMFloatingWindowBase();

private:
    void OnSize(wxSizeEvent &event);
    void OnMoving(wxMoveEvent &event);
    void OnPaint(wxPaintEvent &event);
    void OnKeyDown(wxKeyEvent &event);
    void OnKeyUp(wxKeyEvent &event);
    void OnSetCursor(wxSetCursorEvent &event);
    void OnMouseEvent(wxMouseEvent &event);
    void OnShow(wxShowEvent &event);
    void OnEraseBg(wxEraseEvent &event);

public:
    wxWindow *GetWindow() const;
    wxIFMComponent *GetComponent() const;

    /*!
        Sends an update component event to the root component using its current size

        \param force Force an update even if the window is hidden
    */
    void Update(bool force = false);

    /*!
        Queues an update for the root component
    */
    void AddPendingUpdate();

    DECLARE_EVENT_TABLE()
};

#endif

/*!
    Class containing wxIFM version information. Plugins should use this
    to make sure that the manager they are loaded into supports everything
    they use.
*/
class WXDLLIMPEXP_IFM wxIFMVersionInfo
{
public:

    wxIFMVersionInfo()
        //:
    { }
};

/*!
    Main interace management object. Mediates between application
    window and the IFM system.
*/
class WXDLLIMPEXP_IFM wxInterfaceManager : public wxEvtHandler
{
private:

    bool m_initialized;
	static bool bProcessEvents; //maks
    int m_flags;

    wxWindow *m_parent, *m_content;
	static wxInterfaceManager *m_manager; //maks

    wxIFMInterfacePluginArray m_interfacePlugins;
    int m_activeInterface;

    wxIFMComponent *m_capturedComponent; //!< indentifies which component has captured input, if any
    int m_capturedType; //!< specifies the type of the component that has captured input
#if IFM_CANFLOAT
    wxIFMFloatingWindowBase *m_floatingCapture;
#endif

    wxRect m_updateRect;
    bool m_useUpdateRect;

    wxStatusBar *m_statusbar;
    int m_statusbarPane;
    bool m_statusMessageDisplayed;
    wxString m_oldStatusMessage;

	MapWindowsName mapWindowsName; //maks

public:

    /*!
        \param parent Parent window of the interface
        \param content Content window to be managed by the interface
    */
    wxInterfaceManager(wxWindow *parent, wxWindow *content = NULL);
    ~wxInterfaceManager();

    /*!
        \return Parent window this interface manages
    */
    wxWindow *GetParent() const;

	static wxInterfaceManager *GetManager() {return m_manager;} //maks

	const wxIFMWindowsData *GetWindow(wxString& name); //maks

	static bool  getProcessEvents() 
	{
		return bProcessEvents;
	}

	static void  setProcessEvents(bool value); //maks

    /*!
        \return The rect currently available to the interface manager

        Plugins should use this function to figure out how much space an
        interface has to work with instead of assuming they have the whole
        client area.
    */
    const wxRect &GetInterfaceRect() const;

    /*!
        Specifies a rectangle for the interface to occupy. The interface
        will restrict itself to this rect for all subsequent updates. To
        allow the interface to use the client size while updating, call
        ResetInterfaceRect.

        \note If you call this function, and later call Update() and specify
        a different rect, the rect specified will be used as the new interface
        rect for all subsequent updates.

        \param rect The rect to use
    */
    void SetInterfaceRect(const wxRect &rect);

    /*!
        Calling this function will prevent the interface from being restricted to the
        rect previously set by SetInterfaceRect.
    */
    void ResetInterfaceRect();

    /*!
        Sets the content window that the interface will manage.

        \param content Content window
    */
    void SetContentWindow(wxWindow *content);

    /*!
        \return Content window managed by the interface
    */
    wxWindow *GetContentWindow() const;

    /*!
        Performs necessary actions to ready the interface to recieve and manage children.

        \param defaultPlugins If false, the default (essential functionality!) plugins will
            not be loaded.
        \param flags Specifies any flags.

        Currently the only flag is IFM_NO_RESIZE_UPDATE. If set, the interface
        will not be automatically updated in response to an EVT_SIZE event sent to its
        parent frame. It is the responsability of the parent frame to call
        wxInterfaceManager::Update in response to EVT_SIZE to update the interface.

        \warning If you are not going to be using the default plugins, you must add your
            desired interface plugin(s) before calling this function.

        \return true if initialization was succesful, false if not.
    */
    bool Initialize(bool defaultPlugins = true, int flags = IFM_DEFAULT_FLAGS);

    /*!
        Performs necessary cleanup to ready the interface for deletion.

        \note This function will remove (and delete) all plugins from the interface.
    */
    void Shutdown();

    /*!
        \brief Sets flags
    */
    void SetFlags(int flags);

    /*!
        \return Flags being used
    */
    int GetFlags() const;

    /*!
        Adds a child window to be managed by the interface. This child window will be
        added to all interface plugins.

        \param data Pointer to child data.
        \param update If true, the interface will be updated immediately. Otherwise,
            the interface will not be updated (and the child not sized or moved) until
            the next SizeEvent is generated by the parent frame, or until
            wxInterfaceManager::Update is called.
    */
    bool AddChild(wxIFMChildDataBase *data, bool update = false);

    /*!
        Shows or hides a child window that is managed by this interface

        \param show True to show the child, false to hide it
        \param child The child window to show or hide
        \param update If true, the interface will be updated immediately. Otherwise, changes
            to the interface will not be reflected until a call to Update(), or
            the frame size changes.
    */
    void ShowChild(wxWindow *child, bool show, bool update = true);

    /*!
        Hides a child window that is managed by this interface

        \param child The child window to show or hide
        \param update If true, the interface will be updated immediately. Otherwise, changes
            to the interface will not be reflected until a call to Update(), or
            the frame size changes.
    */
    void HideChild(wxWindow *child, bool update = true);

    /*!
        \return True if this child is not hidden within the interface.
    */
    bool IsChildVisible(wxWindow *child);

    /*!
        \brief Change the minimum, maximum, and desired size of the given child window.

        \note If the width or height value of any of the given sizes is IFM_USE_CURRENT_VALUE, the current
            min, max, or desired width or height will be used. Passing IFM_USE_CURRENT_SIZE for any of the sizes
            will not alter that given size.

        \param child Window whos sizes to change
        \param desired New desired size of the window. Note that the window is not gaurenteed to
            actually be this size after calling this function.
        \param min New minimum size
        \param max New maximum size
        \param update If true, the interface will update immeditely. Otherwise, changes in window sizes
            will not be reflected until the next interface update.
    */
    void SetChildSize(wxWindow *child, const wxSize &desired, const wxSize &min = IFM_USE_CURRENT_SIZE,
        const wxSize &max = IFM_USE_CURRENT_SIZE, bool update = true);

	wxIFMInterfacePluginBase *GetActiveIP() const; //maks

// private for now because they don't actually work and the api for these functions will change
private:
    /*!
        Adds an interface plugin to the interface manager

        \param plugin The interface plugin to add
        \param select If true, the plugin will also be selected as the active interface
            plugin
        \return The index of the interface plugin. Use this value to select this interface plugin
            with wxInterfaceManager::SetActiveInterface. A return value of -1 indicates there
            was an error during plugin initialization, and that the plugin should be deleted.
    */
    int AddInterfacePlugin(wxIFMInterfacePluginBase *plugin, bool select = true);

    /*!
        Sets the active interface plugin

        \param plugin Index of the plugin to activate
    */
    void SetActiveInterface(int plugin);

    /*!
        Removes and deletes the specified interface plugin.

        \param Interface plugin to remove

        \note If the active interface plugin is removed, no new interface plugin
            will be selected until a call to SetActiveInterface is made.
    */
    void RemoveInterfacePlugin(int interface_index = IFM_INTERFACE_ACTIVE);

    /*!
        Removes and deletes all interface plugins.
    */
    void RemoveAllInterfacePlugins();


    

    /*!
        Adds an extension plugin to a given interface plugin

        \param plugin The extension plugin to add
        \param interface_index Interface plugin to add this extension to

        \note This function will initialize the plugin before adding it.
    */
    bool AddExtensionPlugin(wxIFMExtensionPluginBase *plugin/*, int interface_index = IFM_INTERFACE_ACTIVE*/);

    /*!
        Removes and deletes the most recently added extension plugin for the given interface plugin

        \param interface_index plugin to remove an extension from

        \note If an interface plugin adds extension plugins to itself during initialization,
            those plugins will be removed by this function.
    */
    void RemoveExtensionPlugin(/*int interface_index = IFM_INTERFACE_ACTIVE*/);

    /*!
        Removes and deletes all extension plugins for the given interface plugin

        \param interface_index plugin to remove extensions from

        \note If an interface plugin adds extension plugins to itself during initialization,
            those plugins will be removed by this function.
    */
    void RemoveAllExtensionPlugins(/*int interface_index = IFM_INTERFACE_ACTIVE*/);

	

public:
	
    /*!
        Called by plugins to allow for individual components to "capture" mouse and keyboard messages.

        \param component The component capturing input
    */
    void CaptureInput(wxIFMComponent *component);

    /*!
        Releases input if it is captured by a certain component
    */
    void ReleaseInput();

    /*!
        \return True if a component has captured input, false otherwise
    */
    bool IsInputCaptured() const;

    /*!
        \return The component which has captured input, or NULL if no componet has
    */
    wxIFMComponent *GetCapturedComponent() const;

    /*!
        \return The wxWindow object that has currently captured mouse input or NULL if input is not
            captured. Use this window for client to screen and back conversions.
    */
    wxWindow *GetCapturedWindow() const;

    /*!
        Immediately updates the interface, repositioning child windows.
        Floating windows are not included in this update unless specified.

        \note If you specify a rect to use for sizing with this function,
        SetInterfaceRect() will be called, and the rect specified will be used for
        all subsequent updates unless ResetInterfaceRect() is called.

        \param rect Default value implies the entire client area should be used for the update.
            Otherwise, the specified area will be used.
        \param floating If true, floating windows will also be updated
    */
    void Update(wxRect rect = IFM_DEFAULT_RECT, bool floating = false);

    /*!
        This function is identical to the wxInterfaceManager::Update function, except that
        the interface is not updated immediately. An event is posted to the event queue and the
        interface will be updated when this event is processed.

        \param rect Default value implies the entire client area should be used for the update.
            Otherwise, the specified area will be used.
        \param floating If true, floating windows will also be updated
    */
    void AddPendingUpdate(wxRect rect = IFM_DEFAULT_RECT, bool floating = false);

    /*!
        Used to signify plugins that some of their configuration data has changed.
        Plugins which expose configuration data that is user modifyable will do any
        necessary operations to respect the new configuration.

        The interface will be updated including all floating windows by calling this function
    */
    void UpdateConfiguration();

    /*!
        This function is used to specify which pane of the given status bar
        will be used by wxIFM to display messages to the user.

        \param sb The status bar used to display messages.
        \param pane The pane to use to display messages. If IFM_DISABLE_STATUS_MESSAGES is
            specified, status messages will not be displayed.

        \note By default, the displaying of status bar messages is disabled.
        \note If you call this function while a status message is being displayed
            with DisplayStatusMessage, the new pane will not recieve the status message
            until another call to DisplayStatusMessage is made. Calls to ResetStatusMessage
            will not change the message of the original pane, but of the new one instead.
            If you change the pane used for display at an arbitrary time during your
            applications life span, be sure to clean up the message on the original pane
            yourself.
    */
    void SetStatusMessagePane(wxStatusBar *sb, int pane);

    /*!
        Used by plugins to display a status message to the user in the parent
        frames status bar using the pane set earlier by wxInterfaceManager::SetStatusPane

        \param message Message to be displayed.

        \note This function will do nothing if displaying status bar messages is currently disabled
            by called SetStatusMessagePane with the parameter of IFM_DISABLE_STATUS_MESSAGES, or if
            the window that this object is managing is not a frame (or a frame derivative).
    */
    void DisplayStatusMessage(const wxString &message);

    /*!
        Used by plugins to reset the status display message to the message that was
        present before they called wxInterfaceManager::DisplayStatusMessage. The first
        time DisplayStatusMessage is called (or the first time it is called after calling
        ResetStatusMessage) the current status bar pane message will be saved. Future
        calls to DisplayStatusMessage will not change the saved string until ResetStatusMessage
        is called.

        For example, if the status bar pane contained the text "Ready" and
        %DisplayStatusMessage was called two times with two different messages,
        ResetStatusMessage will change the status bar pane's text back to "Ready".
        A second call to ResetStatusMessage will do nothing.

        \note This function will do nothing if displaying status bar messages is currently disabled
            by called SetStatusMessagePane with the parameter of IFM_DISABLE_STATUS_MESSAGES.
    */
    void ResetStatusMessage();
};

/*!
    This class provides data storage for arguments required by the
    wxInterfaceManager::AddChild function.

    If an interface plugin requires specific data for the AddChild event,
    derive a new class from this one and add your data there. Then cast the
    return value of AddChildEvent::GetChildData to your class using wxStaticCast
    to provide for type safety.
*/

class WXDLLIMPEXP_IFM wxIFMChildDataBase : public wxObject
{
public:

    int m_type; //!< Type of child window (generic, toolbar, ...)
    wxSize m_desiredSize; //!< Initial size of the child
    wxSize m_minSize; //!< Minimal tolerable size
    wxSize m_maxSize; //!< Maximal tolerable size
    wxWindow *m_child; //!< Child window to add
    bool m_hidden; //!< Child visibility state
    bool m_hideable; //!< If the child can be hidden
    bool m_fixed; /*!< \brief This child window cannot be resized.
                    If this is true, the values of minSize and maxSize will be ignored,
                    minSize and maxSize will be set to m_desiredSize, and the component will
                    always be the size specified in m_desiredSize. */

	bool bCanDrag; //maks

    wxString m_name; //!< name of the child window

    wxIFMChildDataBase();

    wxIFMChildDataBase(wxWindow *child, int type, const wxString &name = IFM_DEFAULT_COMPONENT_NAME,
        wxSize size = IFM_DEFAULT_SIZE, bool hidden = false,
        wxSize minSize = IFM_NO_MINIMUM_SIZE, wxSize maxSize = IFM_NO_MAXIMUM_SIZE);

    wxIFMChildDataBase(const wxIFMChildDataBase &data);

    virtual ~wxIFMChildDataBase();

    DECLARE_DYNAMIC_CLASS(wxIFMChildDataBase)
};

/*!
    \brief Class representing a generic component in the interface.
*/
class WXDLLIMPEXP_IFM wxIFMComponent
{
private:

    wxIFMComponentDataMap m_data;
    int m_type; //!< type of component
    wxIFMInterfacePluginBase *m_ip;
    

public:

	wxInterfaceManager *GetManager(); //maks

    wxRect m_rect; //!< size and position of the component
    wxSize m_minSize; //!< minimum size of the component, stored in client coordinates
    wxSize m_maxSize; //!< maximum size of the component, stored in client coordinates
    wxSize m_desiredSize; //!< desired size of the component, stored in client coordinates

    /*!
        \brief Specifies wether the component is allowed to be resized.

        Settings this to true implies that m_minSize = m_maxSize = m_desiredSize,
        and that the component will never be allowed to have a different size value.

        Note that the min, max, and desired sizes do not need to be equal so long as
        this value is true. The min and max sizes will be ignored, and the desired size
        will be used for the components size.

        \warning This value is not respected entirely as the above description states that it
            should be.
        \todo Update code to reflect the above description
    */
    bool m_fixed;

    wxIFMBorders m_borders; //!< widths of the top, left, right, bottom borders
    wxIFMMargins m_margins; //!< widths of the top, left, right, bottom margins

    bool m_hidden; //!< if the component is hidden
    bool m_visible; //!< if the component is shown, and all of its parents are shown
    bool m_canHide; //!< true if the component is allowed to be hidden
    bool m_docked; //!< true if the component is docked into another component

	bool bCanDrag; //maks

    int m_alignment; //!< specifies how the children of the component are aligned
    wxIFMComponentArray m_children; //!< the children of this component
    wxIFMComponent *m_parent; //!< the parent of this component

    wxString m_name; //!< the name of this component

    int m_childType; //!< if the component has a child, this specifies the type of child
    wxWindow *m_child; //!< if the component has a child, this is a pointer to the child window

    /*!
        \param manager The interface that this component belongs to
        \param type The type of component. (Container, Panel, ...)
    */
    wxIFMComponent(wxIFMInterfacePluginBase *ip, int type);
private:
    wxIFMComponent(const wxIFMComponent &);
public:

    ~wxIFMComponent();

    /*!
        Called by plugins to add data storage to this component.

        \note Unless manually removed by the plugin that added it, the data will
            be deleted by the component destructor.
    */
    void AddExtensionData(wxIFMExtensionDataBase *data);

    /*!
        Retrieves the data stored by this component with the specified key.

        \return A pointer to the data with this key, or NULL no data with the given key exists.

        \note The default component data can be accessed by passing IFM_DEFAULT_DATA_KEY to this function.
        \note Trying to retrieve data that has not been set is an error.
    */
    wxIFMExtensionDataBase *GetExtensionData(wxIFMComponentDataKeyType key);

    /*!
        Removes data with the specified key from this component, but does not delete it.

        \return A pointer to the data removed, or NULL if no data with the given key exists.

        \note Attempting to remove data that has not been set is an error.
        \note Trying to remove default component data is illegal and also considered an error.
    */
    wxIFMExtensionDataBase *RemoveExtensionData(wxIFMComponentDataKeyType key);

    /*!
        Returns the index of the next visible component, or -1 if there are
        no more components or they are all hidden.

        This is a helper function provided for plugins for easier OnUpdate
        handling of hidden components.

        \param components Components array to search through
        \param start Where to start searching
        \note The value specified by start is included in the search
        \return Index of the next visible component, or -1 if there are
            no more (visible) components.

        \warning This function may be removed in a future release
    */
    static int GetNextVisibleComponent(const wxIFMComponentArray &components, int start);

    /*!
        \return True if \a child is a child of \a parent

        \warning This function may be removed in a future release
    */
    static bool IsChildOf(wxIFMComponent *parent, wxIFMComponent *child);

    /*!
        Paints this component by sending PAINT_BACKGROUND, PAINTBORDER, and PAINT_DECOR events

        \param dc DC to paint on
        \param region Region that needs painting
    */
    void Paint(wxDC &dc, const wxRegion &region);

    /*!
        \return The type of the component
    */
    int GetType() const;

    /*!
        \return Parent window of this component
    */
    wxWindow *GetParentWindow();

    /*!
        \return Absolute rect and position
        \see \ref coordsystems
    */
    wxRect GetRect();

    /*!
        \return Position and size of the background rectangle
        \see \ref coordsystems
    */
    wxRect GetBackgroundRect();

    /*!
        \return Position and size of the client rectangle
        \see \ref coordsystems
    */
    wxRect GetClientRect();

    /*!
        Converts the given rect from the given coordinate system to the desired coordinate system
        using this component.
        \return Rect in the desired coordinate system
    */
    wxRect GetConvertedRect(wxRect rect, int coords_from, int coords_to);

    /*!
        \return The desired size of the component in absolute coordinates
    */
    wxSize GetDesiredSize();

    /*!
        Sets the desired size of this component. The size is specified in absolute coordinates
    */
    void SetDesiredSize(const wxSize &size);

	void SetRect(const wxRect &rect); //maks

    /*!
        \return The minimum tolerable size of the component in absolute coordinates
    */
    wxSize GetMinSize();

    /*!
        \return The maximimum tolerable size of the component in absolute coordinates
    */
    wxSize GetMaxSize();

    /*!
        Shows or hides this component
        \param update Update the interface immediately
    */
    void Show(bool s, bool update);
    void Hide() { Show(false, true); }

    /*!
        \return Whether this component is visible
    */
    bool IsShown();

    /*!
        Called when the visibility of this component has changed. This happens if the component
        is visible and its parent was hidden or shown.

        \param vis True if the component is now visible, false if it is not visible.
    */
    void VisibilityChanged(bool vis);

    /*!
        \return Whether or not the component is visible
    */
    bool IsVisible();
};

/*!
    Base class for plugin specific data storage within individual components.
*/
class WXDLLIMPEXP_IFM wxIFMExtensionDataBase : public wxObject
{
public:

    virtual ~wxIFMExtensionDataBase();

    /*!
        \return Key used to retrieve this data
    */
    virtual wxIFMComponentDataKeyType GetDataKey() const = 0;

    /*!
        Note that this function is the static version of GetDataKey.
        ALL component data types must implement their own copy of this function.
        \return Key used to retrieve this data
    */
    static wxIFMComponentDataKeyType DataKey();
};

#if IFM_CANFLOAT
/*!
    Storage for floating data
*/
class WXDLLIMPEXP_IFM wxIFMFloatingData : public wxIFMExtensionDataBase
{
public:

    bool m_floating; //!< floating or not
    wxIFMFloatingWindowBase *m_window; //!< window we are floating in
    wxRect m_rect;
    /*!<
        \brief Rect we occupy while floated.
        Contains a screen position and absolute size
    */

    wxIFMFloatingData();

    wxIFMComponentDataKeyType GetDataKey() const;
    static wxIFMComponentDataKeyType DataKey();
};
#endif

#endif // _IFM_MANAGER_H_
