/*!
    Defines plugin related classes

    \file   plugin.h
    \author Robin McNeill
    \date   Created: 02/24/05

    Copyright (c) Robin McNeill
    Licensed under the terms of the wxWindows license
*/

#ifndef _IFM_PLUGIN_H_
#define _IFM_PLUGIN_H_

#include "wx/ifm/manager.h"
#include "wx/ifm/events.h"

/*!
    Interface plugin base class
*/
class WXDLLIMPEXP_IFM wxIFMInterfacePluginBase : public wxEvtHandler
{
private:
    wxInterfaceManager *m_manager;
    wxIFMExtensionPluginBase *m_topExtensionPlugin;

protected:
    wxIFMComponentArray m_components;
    wxIFMChildWindowMap m_windows;

#if IFM_CANFLOAT
    wxIFMFloatingWindowArray m_floatingWindows; //!< stores all windows used for floating components
#endif

public:
    wxIFMInterfacePluginBase();
    virtual ~wxIFMInterfacePluginBase();

    wxInterfaceManager *GetManager();

    virtual bool Initialize(wxInterfaceManager *manager);
    virtual void Shutdown();

	virtual void Clear(); //maks

	void ConnectEvents(); //maks
	void DisconnectEvents(); //maks

    /*!
        \brief Adds an extension plugin.

        wxIFMExtensionPluginBase::Initialize will be called prior to adding the plugin.
        If that function returns false, the plugin will not be added and false will be returned.

        \param plugin The plugin to add
    */
    bool PushExtensionPlugin(wxIFMExtensionPluginBase *plugin);

    /*!
        \brief Removes and deletes the most recently added extension plugin
    */
    void PopExtensionPlugin();

    /*!
        \brief Removes and deletes all extension plugins
    */
    void PopAllExtensionPlugins();

    /*!
        \brief Enables or Disables the interface plugin as well as all extension plugins
    */
    virtual void Enable(bool val = true);
    void Disable() { Enable(false); }

    /*!
        \return Const reference to the components array
    */
    /*const*/ wxIFMComponentArray &GetComponents() /*const*/; //maks

    /*!
        \return Const reference to the floating windows array
    */
#if IFM_CANFLOAT
    const wxIFMFloatingWindowArray &GetFloatingWindows() const;
#endif

    /*!
        \brief Processes a plugin event.
    */
    virtual bool ProcessPluginEvent(wxIFMPluginEvent &event);

    /*!
        \brief Adds a plugin event to the event que to be processed later
    */
    void AddPendingPluginEvent(wxIFMPluginEvent &event);

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
    virtual wxIFMComponent *GetComponentByPos(const wxPoint &pos, wxIFMComponent *component = NULL,
        bool floating = false);

    /*!
        \brief Finds the component containing the given child window.
    */
    wxIFMComponent *FindChildWindow(wxWindow *window);

    /*!
        \brief Adds a child window to the interface plugin, associated with a given component.

        If a window was already associated with a component, and the component changed, calling
        this function again will update the its associated component.

        \param window Child window to add
        \param component Component owning this child window
    */
    void AddChildWindow(wxWindow *window, wxIFMComponent *component);

    /*!
        Return the floating component the mouse is within or NULL if the mouse is not
        over a floating component

        \param pos Position of the mouse in <b>screen coordinates</b>
    */
#if IFM_CANFLOAT
    virtual wxIFMComponent *GetFloatingComponentByPos(const wxPoint &pos);
#endif

	//maks: public
	void OnMouseEvent(wxMouseEvent &event);
    void OnKeyDown(wxKeyEvent &event);
    void OnKeyUp(wxKeyEvent &event);
    void OnPaint(wxPaintEvent &event);
    void OnSetCursor(wxSetCursorEvent &event);
    void OnSize(wxSizeEvent &event);
    void OnFocus(wxFocusEvent &event);
    void OnActivate(wxActivateEvent &event);

private:
    /*!
        Returns the top most extension plugin, or this if there are no extension plugins
    */
    wxEvtHandler *GetTopPlugin();

    

    DECLARE_DYNAMIC_CLASS_NO_COPY(wxIFMInterfacePluginBase)
};

/*!
    Extension plugin base class
*/
class WXDLLIMPEXP_IFM wxIFMExtensionPluginBase : public wxEvtHandler
{
private:
    wxIFMInterfacePluginBase *m_ip;

protected:
    wxInterfaceManager *GetManager();
    wxIFMInterfacePluginBase *GetIP();

public:
    wxIFMExtensionPluginBase();
    virtual ~wxIFMExtensionPluginBase();

    /*!
        Tells the extension plugin what interface plugin it extends.
        If an extension plugin only works for inteface plugin Q, a
        warning message should be generated by this function, and
        false should be returned. Otherwise, return true.

        If this function returns false, the extension plugin will be
        removed from the interface plugin that it was added to.
    */
    virtual bool Initialize(wxIFMInterfacePluginBase *plugin);
    virtual void Shutdown();

    virtual void Enable(bool val = true);
    void Disable();

    DECLARE_DYNAMIC_CLASS_NO_COPY(wxIFMExtensionPluginBase)
};

#endif
