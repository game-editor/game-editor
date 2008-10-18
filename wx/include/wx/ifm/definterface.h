/*!
    Prototypes the default interface plugins for IFM

    \file   definterface.h
    \author Robin McNeill
    \date   Created: 11/06/04

    Copyright (c) Robin McNeill
    Licensed under the terms of the wxWindows license
*/

#ifndef _DEFINTERFACE_H_
#define _DEFINTERFACE_H_

#include "wx/ifm/plugin.h"
#include "wx/ifm/events.h"

#include "wx/frame.h"
#include "wx/bitmap.h"
#include "wx/pen.h"
#include "wx/brush.h"

//#include <list>
//#include <vector>

#define IFM_FLOATING_FRAME_STYLE wxFRAME_NO_TASKBAR | wxFRAME_FLOAT_ON_PARENT | wxRESIZE_BORDER /*| wxSYSTEM_MENU | wxCLOSE_BOX*/ //maks

#define IFM_CAPTION_PAD 1 //!< gap between the caption and the components client area

#define IFM_COMPONENT_BUTTON_X    10
#define IFM_COMPONENT_BUTTON_Y    11
#define IFM_COMPONENT_BUTTON_PAD  2

//! \todo provide an auto generating facility for these id's
#define IFM_COMPONENT_ID_CLOSE      0
#define IFM_COMPONENT_ID_MAXIMIZE   1
#define IFM_COMPONENT_ID_MINIMIZE   2

#define IFM_CONTAINER_GRIPPER_WIDTH 0

class wxIFMDefaultInterfacePlugin;
class wxIFMFloatingFrame;
class wxIFMDefaultContainerPlugin;
class wxIFMDefaultPanelPlugin;
class wxIFMContainerData;
class wxIFMPanelTabData;
class wxIFMAddTopContainerEvent;
class wxIFMRemoveTopContainerEvent;
class wxIFMSelectTabEvent;
class wxIFMComponentButton;
class wxIFMComponentButtonManager;
class wxIFMComponentButtonEvent;
class wxIFMTab;

//typedef std::list<wxIFMComponent *> wxIFMComponentList;
WX_DECLARE_LIST(wxIFMComponent, wxIFMComponentList);

// typedef std::vector<wxIFMComponentButton *> wxIFMComponentButtonArray;
WX_DEFINE_ARRAY(wxIFMComponentButton *, wxIFMComponentButtonArray);
WX_DEFINE_ARRAY(wxIFMTab *, wxIFMTabArray);

// component data keys
DECLARE_IFM_DATA_KEY(IFM_CONTAINER_DATA_KEY)
DECLARE_IFM_DATA_KEY(IFM_PANEL_DATA_KEY)
DECLARE_IFM_DATA_KEY(IFM_TABBED_PANEL_DATA_KEY)

#define IFM_ORIENTATION_DEFAULT -1 //!< select orientation automatically
#define IFM_ORIENTATION_NONE    0 //!< unitialized (an erronous) orientation
#define IFM_ORIENTATION_TOP     1 //!< top container
#define IFM_ORIENTATION_BOTTOM  2 //!< bottom container
#define IFM_ORIENTATION_LEFT    3 //!< left container
#define IFM_ORIENTATION_RIGHT   4 //!< right container
#define IFM_ORIENTATION_FLOAT   5 //!< floating
#define IFM_ORIENTATION_DOCKED  6 //!< this container is docked inside another container

DECLARE_IFM_COMPONENT_TYPE(IFM_COMPONENT_CONTAINER)
DECLARE_IFM_COMPONENT_TYPE(IFM_COMPONENT_PANEL_TAB)
DECLARE_IFM_COMPONENT_TYPE(IFM_COMPONENT_PANEL) // a component that manages a group of panel tabs

DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM,wxEVT_IFM_ADDTOPCONTAINER, 0);
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM,wxEVT_IFM_REMOVETOPCONTAINER, 0);
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM,wxEVT_IFM_SELECTTAB, 0);
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_IFM,wxEVT_IFM_COMPONENTBUTTONCLICK, 0);

#if IFM_CANFLOAT

/*!
    Floating frame window for the default interface
*/
class WXDLLIMPEXP_IFM wxIFMFloatingFrame : public wxIFMFloatingWindowBase
{
public:

    wxIFMFloatingFrame(wxIFMInterfacePluginBase *ip,
        wxWindow *parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxFrameNameStr);

    ~wxIFMFloatingFrame() {}

    void OnClose(wxCloseEvent &event);

    DECLARE_EVENT_TABLE()
};

#endif

/*!
    Default interface functionality plugin
*/
class WXDLLIMPEXP_IFM wxIFMDefaultInterfacePlugin : public wxIFMInterfacePluginBase
{
public:

    wxIFMDefaultInterfacePlugin();
    ~wxIFMDefaultInterfacePlugin();

	virtual void Clear(); //maks

    /*!
        Initializes the interface plugin by pushing required extension plugins
    */
    bool Initialize(wxInterfaceManager *manager);

    /*!
        \return Const reference to the top level container list
    */
    const wxIFMComponentList &GetTopContainerList() const;

    bool ProcessPluginEvent(wxIFMPluginEvent &event);

    /*!
        \return Pointer to the LAST container object in the list with
            the specified orientation
    */
    wxIFMComponent *GetContainer(int orientation);

    wxIFMComponent *GetComponentByPos(const wxPoint &pos, wxIFMComponent *component = NULL, bool floating = false);

private:

    wxIFMComponentList m_containers;

    void OnDeleteComponent(wxIFMDeleteComponentEvent &event);
    void OnAddChild(wxIFMAddChildEvent &event);
    void OnUpdate(wxIFMUpdateEvent &event);
    void OnPaint(wxIFMPaintInterfaceEvent &event);
    void OnGetContentRect(wxIFMRectEvent &event);

    void OnAddTopContainer(wxIFMAddTopContainerEvent &event);
    void OnRemoveTopContainer(wxIFMRemoveTopContainerEvent &event);

#if IFM_CANFLOAT
    void OnCreateFloatingWindow(wxIFMCreateFloatingWindowEvent &event);
    void OnDestroyFloatingWindow(wxIFMDestroyFloatingWindowEvent &event);
    void OnFloat(wxIFMFloatEvent &event);
#endif

    DECLARE_EVENT_TABLE()

    DECLARE_DYNAMIC_CLASS_NO_COPY(wxIFMDefaultInterfacePlugin)
};

/*!
    Handles default container functionality
*/
class WXDLLIMPEXP_IFM wxIFMDefaultContainerPlugin : public wxIFMExtensionPluginBase
{
public:

    wxIFMDefaultContainerPlugin();

private:

    /*
    Event handlers
    */
    void OnCreateComponent(wxIFMNewComponentEvent &event);
    void OnDock(wxIFMDockEvent &event);
    void OnUndock(wxIFMUndockEvent &event);
    void OnUpdate(wxIFMUpdateComponentEvent &event);
    void OnVisibilityChange(wxIFMComponentVisibilityChangedEvent &event);
    void OnShowComponent(wxIFMShowComponentEvent &event);
    void OnLeftDown(wxIFMMouseEvent &event);

#if IFM_CANFLOAT
    void OnFloatNotify(wxIFMFloatNotifyEvent &event);
    void UpdateFloatingCaption(wxIFMComponent *container);
#endif

    void OnGetDesiredSize(wxIFMRectEvent &event);
    void OnSetDesiredSize(wxIFMRectEvent &event);
    void OnGetRect(wxIFMRectEvent &event);

    void OnPaintDecor(wxIFMPaintEvent &event);

    void OnComponentButtonClick(wxIFMComponentButtonEvent &event);

public:
    /*!
        This functions searchs for a visible child of this container among its children
        and its childrens children. It ignores children that are containers and considers them hidden
        unless they container a visible child themselves (that isn't a container too).

        \return True if this container has a visible child.

        \todo I fixed some resize sash calculation issues by making this function public.
            I should figure out what the two uses of the function really are and abstract
            it into an event.
    */
    static bool HasVisibleChildren(wxIFMComponent *component);
private:

    /*!
        \return True if any child of this component cannot be closed
    */
    bool HasNonCloseableChildren(wxIFMComponent *component);

    /*!
        Returns the number of visible children, ignoring containers.
    */
    int GetVisibleChildrenCount(wxIFMComponent *component);

    DECLARE_EVENT_TABLE()

    DECLARE_DYNAMIC_CLASS_NO_COPY(wxIFMDefaultContainerPlugin)
};

/*!
    Configuration data for tab appearance
*/
class wxIFMTabConfig
{
public:
    /*!
        \return Font used for rendering tab names
    */
    const wxFont &get_font() const;

    /*!
        Sets the font used for rendering tab names. Modifying the font
        will also modify the padding used for tabs, which is based on the size of
        the font used. This is so that the interface will scale properly on machines
        running larger than normal DPI settings (120 vs 96). The padding value used
        is defined to be get_font_height() / 4.
    */
    void set_font(const wxFont &font);

	void update_height(); //maks

    /*!
        \return The height of the font used for rendering tab names. Calculated
        by passing " " to GetTextExtents
    */
    int get_font_height() const;

    /*!
        \return The color used for rendering the tab names
    */
    const wxColour &get_font_colour() const;

    /*!
        Sets the color used for rendering the tab names
    */
    void set_font_colour(const wxColour &colour);

    /*!
        \return The gap between the first tab and the start of the tab area
    */
    int get_indent() const;

	int get_m_row_height() {return m_row_height;} //maks

    /*!
        Sets the gap between the first tab and the start of the tab area
    */
    void set_indent(int indent);

    /*!
        \return The rounding factor used to draw rounded tab corners as passed to
        DrawRoundedRectangle
    */
    int get_rounding_factor() const;

    /*!
        Sets the rounding factor used to draw rounded tab corners as passed to
        DrawRoundedRectangle
    */
    void set_rounding_factor(int factor);

    /*!
        \return The spacing between tabs in pixels
    */
    int get_tab_spacing() const;

    /*!
        Sets the spacing between tabs in pixels
    */
    void set_tab_spacing(int spacing);

	bool get_top() {return bTop;} //maks
	void set_top(bool bValue) {bTop = bValue;} //maks

private:
    wxIFMTabConfig();

    /*!
        Performs initialization that would otherwise be done statically but cannot
        be done statically due to limitations of wxWidgets
    */
    void init();

    wxFont m_font;
    wxColour m_font_color;
    int m_font_height;
    int m_row_height;
    int m_indent;
    int m_tab_height;
    int m_tab_padding;
    int m_tab_spacing;
    int m_rounding_factor;

    wxPen m_tab_border_p;

    wxBrush m_tab_active_bg_b;
    wxPen m_tab_active_bg_p;

    wxBrush m_tab_inactive_bg_b;
    wxPen m_tab_inactive_bg_p;

    wxBrush m_bg_brush;
    wxPen m_bg_pen;
    wxPen m_line_pen;

	bool bTop; //maks

    friend class wxIFMDefaultPanelPlugin;
};

enum IFM_CAPTION_STYLES
{
    //! Mask to access caption styles
    IFM_CAPTION_MASK        = 0xf,
    //! Mask to acces caption title styles
    IFM_CAPTION_TITLE_MASK  = 0xf0,

    //! Draw a solid caption
    IFM_CAPTION_SOLID       = 0x1,
    //! Draw a gradient caption
    IFM_CAPTION_GRADIENT    = 0x2,
    //! Left align the caption title
    IFM_CAPTION_LEFT        = 0x10,
    //! Center align the caption title
    IFM_CAPTION_CENTER      = 0x20,
    //! Right align the caption title
    IFM_CAPTION_RIGHT       = 0x30,
};

/*!
    Config data for caption appearance.

    \see IFM_CAPTION_STYLES
*/
class wxIFMCaptionConfig
{
public:
    /*!
        \return The style of the caption.
    */
    int get_caption_style() const;

    /*!
        Sets the caption style. The style of the caption changes its appearance
    */
    void set_caption_style(int style);

    /*!
        \return The font used to render caption text
    */
    const wxFont &get_font() const;

    /*!
        Sets the font used to render caption text. This also calculates the
        padding values and the final caption height, which is based on the height
        of the caption font
    */
    void set_font(const wxFont &font);

    /*!
        \return The color used to render caption text
    */
    const wxColour &get_font_colour() const;

    /*!
        Sets the color used to render caption text
    */
    void set_font_colour(const wxColour &colour);

    /*!
        Returns the background color used to draw the caption. If the
        caption has the IFM_CAPTION_GRADIENT style, this color will be
        used for the start of the gradient.

        \return The background color used for captions
    */
    const wxColour &get_caption_color() const;

    /*!
        Sets the background color used to draw the caption. If the
        caption has the IFM_CAPTION_GRADIENT style, this color will be
        used for the start of the gradient.
    */
    void set_caption_color(const wxColour &colour);

    /*!
        Returns the background color used as the end gradient color if
        the caption has the IFM_CAPTION_GRADIENT style. If it does not have
        this style, this color is ignored.

        \return The background color used for the end of the gradient.
    */
    const wxColour &get_caption_gradient_color() const;

    /*!
        Sets the background color used as the end gradient color.
    */
    void set_caption_gradient_color(const wxColour &colour);

	void set_3d_boder(bool value) {b3dBorder = value;}//maks
	bool get_3d_boder() {return b3dBorder;}//maks

private:
    wxIFMCaptionConfig();

    /*!
        Performs initialization that would otherwise be done statically but cannot
        be done statically due to limitations of wxWidgets
    */
    void init();

    wxFont m_font;
    wxColour m_font_color;
    wxColour m_caption_color, m_gradient_color;
    int m_font_height;
    int m_size;
    int m_padding;
    int m_style;

	bool b3dBorder; //maks

    friend class wxIFMDefaultPanelPlugin;
};

/*!
    Handles default panel functionality.

    Panels are components with a caption that arrange children using tabs.

    You can configure certain look and feel options relating to panels using
    GetCaptionConfig and GetTabConfig. See the wxIFMTabConfig and wxIFMCaptionConfig
    classes for a listing of all of the configurable options.

    \see wxIFMTabConfig, wxIFMCaptionConfig
*/
class WXDLLIMPEXP_IFM wxIFMDefaultPanelPlugin : public wxIFMExtensionPluginBase
{
public:

    wxIFMDefaultPanelPlugin();
    bool Initialize(wxIFMInterfacePluginBase *plugin);

    /*!
        Returns a reference to the configuration data controlling the appearance
        of captions.
    */
    static wxIFMCaptionConfig &GetCaptionConfig();

    /*!
        Returns a reference to the configuration data controlling the appearance
        of tabs.
    */
    static wxIFMTabConfig &GetTabConfig();

private:

    static wxIFMCaptionConfig m_caption_config;
    static wxIFMTabConfig m_tab_config;
    static bool init_config;

    void OnCreateComponent(wxIFMNewComponentEvent &event);
    void OnDestroyComponent(wxIFMDeleteComponentEvent &event);
    void OnQueryChild(wxIFMQueryChildEvent &event);
    void OnUpdate(wxIFMUpdateComponentEvent &event);
    void OnVisibilityChanged(wxIFMComponentVisibilityChangedEvent &event);
    void OnShowComponent(wxIFMShowComponentEvent &event);
    void OnPaintDecor(wxIFMPaintEvent &event);
    void OnPaintBg(wxIFMPaintEvent &event);
    void OnLeftDown(wxIFMMouseEvent &event);
    void OnLeftUp(wxIFMMouseEvent &event);
    void OnMouseMove(wxIFMMouseEvent &event);
    void OnDock(wxIFMDockEvent &event);
    void OnUndock(wxIFMUndockEvent &event);
    void OnComponentButtonClick(wxIFMComponentButtonEvent &event);
    void OnConvertRect(wxIFMConvertRectEvent &event);
    void OnUpdateConfig(wxIFMUpdateConfigEvent &event);

    void OnSelectTab(wxIFMSelectTabEvent &event);
    void OnGetDesiredSize(wxIFMRectEvent &event);
    void OnSetDesiredSize(wxIFMRectEvent &event);
    void OnGetMinSize(wxIFMRectEvent &event);
    void OnGetMaxSize(wxIFMRectEvent &event);

    /*!
        Draws a caption using the given style.

        \param dc wxDC on which to draw
        \param rect Rect to draw the caption in
        \param caption Caption string to use
        \param orientation What side of the component is the orientation on. One of
        IFM_ORIENTATION_LEFT, IFM_ORIENTATION_TOP, IFM_ORIENTATION_RIGHT, IFM_ORIENTATION_BOTTOM
    */
	void DrawCaption(wxDC &dc, const wxRect &rect, const wxString &caption, int orientation, bool bDrawText); //maks

    /*!
        Updates the sizes for the tabs of this component. If
        the component is not actually a panel, this function does nothing.
        If the parent of this panel is another panel, this funciton will
        also update the tab rects for the parent.
    */
    void UpdateTabs(wxIFMComponent *panel);

    /*!
        Draws the tabs for the component

        \param dc wxDC to use for drawing
        \param component Component to draw tabs for
        \param region Clipping region for the drawing
        \param tabs The tabs to draw
    */
    void DrawTabs(wxDC &dc, wxIFMComponent *component, const wxRegion &region, const wxIFMTabArray &tabs);

	void DrawOneLightWaveTab(wxDC &dc, wxIFMTab *tab); //maks

    /*!
        Returns true if the tab row should be displayed for the given tabs
    */
    bool ShouldDrawTabs(const wxIFMTabArray &tabs);

#if IFM_CANFLOAT
    void OnFloatNotify(wxIFMFloatNotifyEvent &event);
#endif

    DECLARE_EVENT_TABLE()

    DECLARE_DYNAMIC_CLASS_NO_COPY(wxIFMDefaultPanelPlugin)
};

/*!
    \brief Class for managing small buttons that appear for a component.

    \warning The api for this class will change in an incompatible way before the next release
    \todo Make this class be able to position buttons horizontally or vertically
*/
class WXDLLIMPEXP_IFM wxIFMComponentButtonManager
{
private:
    wxPoint m_pos;
    int m_height;
    wxIFMInterfacePluginBase *m_ip;
    wxIFMComponent *m_component;
    bool m_hidden;

public:

    wxIFMComponentButtonArray m_buttons;

    wxIFMComponentButtonManager(wxIFMInterfacePluginBase *ip, wxIFMComponent *component);
    ~wxIFMComponentButtonManager();

    wxIFMInterfacePluginBase *GetIP();
    wxIFMComponent *GetComponent();

    /*!
        \brief Adds a button to this button manager.

        \param id ID of the button to add
        \return Pointer to the button created
    */
    wxIFMComponentButton *AddButton(int id);

    /*!
        \brief Removes (and deletes) button from this manager.

        \param id ID of the button to remove
    */
    void RemoveButton(int id);

    /*!
        Returns the button with the requested id
    */
    wxIFMComponentButton *GetButton(int id);

    /*!
        \return The rect that this button manager occupies
    */
    const wxRect GetRect() const;

    /*!
        \return Position of this button manager
    */
    const wxPoint &GetPosition() const;

    /*!
        Sets the position of this button manager
    */
    void SetPosition(const wxPoint& pos);

    /*!
        Sets the height (and indirectly the size of all buttons) of this button manager

        \note The default height if IFM_CAPTION_HEIGHT
    */
    void SetHeight(int height);

    /*!
        Used to show or hide the component buttons
    */
    void Show(bool show = true);
    void Hide();

    /*!
        Shows or hides only the component with the given i
        \param id Button to show or hide
    */
    void Show(bool show, int id);

    /*!
        Used to change the parent window of the buttons. Called during FLOATNOTIFY processing
    */
    void SetParent(wxWindow *parent);

    /*!
        Used to position the component buttons
    */
    void Layout();
};

class WXDLLIMPEXP_IFM wxIFMComponentButton : public wxWindow
{
private:

    int m_id;

    wxIFMComponentButtonManager *m_manager;

    bool m_hover, m_pressed, m_leftDown;

    wxIFMInterfacePluginBase *GetIP();

public:
    wxBitmap *m_bmp, *m_bmpH, *m_bmpP;

    wxIFMComponentButton(wxIFMComponentButtonManager *manager, int id, wxWindow *parent);
	virtual ~wxIFMComponentButton(); //maks
    int GetID() const;

    /*!
        Sets the hover state of this button.
    */
    void SetHover(bool h);
    bool GetHover() const;

    /*!
        Sets the pressed state of this button.
    */
    void SetPressed(bool p);
    bool GetPressed() const;

    void OnPaint(wxPaintEvent &event);
    void OnEraseBg(wxEraseEvent &event);
    void OnEnterWindow(wxMouseEvent &event);
    void OnLeaveWindow(wxMouseEvent &event);
    void OnLeftDown(wxMouseEvent &event);
    void OnLeftUp(wxMouseEvent &event);
    void OnMouseMove(wxMouseEvent &event);

    DECLARE_EVENT_TABLE()
};

/*!
    Data storage for AddChild data for wxIFMDefaultInterfacePlugin
*/
class WXDLLIMPEXP_IFM wxIFMDefaultChildData : public wxIFMChildDataBase
{
public:

    /*!
        Possible values: IFM_ORIENTATION_TOP, IFM_ORIENTATION_LEFT, IFM_ORIENTATION_BOTTOM,
        IFM_ORIENTATION_RIGHT, IFM_ORIENTATION_FLOATING

        Specifies what side of the window to add the child to

        \note The first child added on the left side will be against the
        left edge of the frame. the next child added to the left (in a new row)
        will be against the left edge of the first child. Each subsequent child
        will be against the left edge of the previously added child. If you want
        panels appearing in a specific order, add them in the proper order to achieve
        your desired sorting.
    */
    int m_orientation;
    
    /*!
        Specifies the position of the child within the container specified by orientation.
    */
    int m_index;

    /*!
        If true a new container will be created for this child Index
        Only used if new row is false,
    */
    bool m_newRow;

    /*!
        If true, the child will be created as a tab. It will be docked as a sibling of the previously
        docked window, provided that the orientation of the previous window is relatively the same as
        the newly specified orientation. For example, creating a child with IFM_ORIENTATION_FLOAT
        and then creating a child with IFM_ORIENTATION_BOTTOM with m_tabify set to true will not
        cause the child to become a tab of the floating window because the orientations are not
        relatively the same.
    */
    bool m_tabify;


    /*!
        This is tab's bitmap
    */
    wxBitmap m_bitmap;

    /*!
        Position in screen coordinates of the floating window that will be
        created to float the child in.
    */
    wxPoint m_pos;

    wxIFMDefaultChildData();

    // docked ctor
    wxIFMDefaultChildData(wxWindow *child, int type, const wxString &name = IFM_DEFAULT_COMPONENT_NAME,
        wxSize size = IFM_DEFAULT_SIZE, bool hidden = false,
        int orientation = IFM_ORIENTATION_DEFAULT, int index = IFM_DEFAULT_INDEX,
        bool newRow = false, bool tabify = false, const wxBitmap &bmp = wxNullBitmap);

    // floating ctor
    wxIFMDefaultChildData(wxWindow *child, int type, wxPoint pos, wxSize size, bool hidden = false,
        const wxString &name = IFM_DEFAULT_COMPONENT_NAME);

    // copy ctors
    wxIFMDefaultChildData(const wxIFMDefaultChildData &data);
    wxIFMDefaultChildData(const wxIFMChildDataBase &data);

    DECLARE_DYNAMIC_CLASS(wxIFMDefaultChildData)
};

/*!
    Storage class for data specific to containers.
*/
class WXDLLIMPEXP_IFM wxIFMContainerData : public wxIFMExtensionDataBase
{
public:

    wxIFMContainerData();
    wxIFMContainerData(wxIFMInterfacePluginBase *ip, wxIFMComponent *c);

    //wxIFMComponentButtonManager m_buttonManager;

    int m_orientation; //!< where is this container? (top, bottom, left, right, docked)
    wxRect m_tray_rect; //!< area containing buttons and extras for the container. Also the drag enabled area

    wxIFMComponentDataKeyType GetDataKey() const;
    static wxIFMComponentDataKeyType DataKey();

    DECLARE_DYNAMIC_CLASS(wxIFMContainerData)
};

/*!
    Storage class for data specific to panels, which are actually tabs inside of "tabbed panels".
*/
class WXDLLIMPEXP_IFM wxIFMPanelTabData : public wxIFMExtensionDataBase
{
public:

    wxBitmap m_bitmap;

    wxIFMPanelTabData();

    wxIFMComponentDataKeyType GetDataKey() const;
    static wxIFMComponentDataKeyType DataKey();

    DECLARE_DYNAMIC_CLASS(wxIFMPanelTabData)
};

/*!
    Data structure for tabs inside tabbedpanels
*/
class WXDLLIMPEXP_IFM wxIFMTab
{
public:
    wxIFMTab()
        : m_active(false),
        m_visible(true),
        m_tab(NULL),
        m_name_width(0)
    { 
		lastCreatedTab = this; //maks
	}

    bool m_active, m_visible;
    wxRect m_rect; //!< The area this tab occupies
    wxIFMComponent *m_tab; //!< The component this tab represents
    int m_name_width; //!< Width of the components name string

	static wxIFMTab *lastCreatedTab; //maks
};

/*!
    Storage class for data specific to "tabbed panels"
*/
class WXDLLIMPEXP_IFM wxIFMPanelData : public wxIFMExtensionDataBase
{
public:

    wxIFMPanelData()
        : m_button_manager(0,0)
    { }

    wxIFMPanelData(wxIFMInterfacePluginBase *ip, wxIFMComponent *c);
    ~wxIFMPanelData();

    wxIFMComponentButtonManager m_button_manager;

    int m_active_tab_index; //!< index of the currently selected tab
    wxIFMTab *m_active_tab; //!< the selected tab
    wxRect m_caption;

    /*!
        Before hiding a child because it is not selected, this member will be set to true.
        During wxIFMShowComponentEvent processing, the tab associated with the child will
        be hidden only if this member is false. If that is not the case, it won't hide the tab
        and it will set this member to false.
    */
    bool m_dont_hide_tab;

    wxIFMTabArray m_tabs;

    wxIFMComponentDataKeyType GetDataKey() const;
    static wxIFMComponentDataKeyType DataKey();

    DECLARE_DYNAMIC_CLASS(wxIFMPanelData)

};

/*!
    Event used to add a container to the top level container list at the given index, or before
    or after an existing top level container

    \see wxIFMRemoveTopContainerEvent
*/
class WXDLLIMPEXP_IFM wxIFMAddTopContainerEvent : public wxIFMPluginEvent
{
private:
    int m_index;
    wxIFMComponent *m_indexContainer; // container to dock relatively to

public:
    // index mode ctor
    wxIFMAddTopContainerEvent(wxIFMComponent *container, int index)
        : wxIFMPluginEvent(wxEVT_IFM_ADDTOPCONTAINER, container),
        m_index(index),
        m_indexContainer(NULL)
    { }

    // relative mode ctor
    // if before == false, after is implied
    wxIFMAddTopContainerEvent(wxIFMComponent *container, wxIFMComponent *index_container, bool before)
        : wxIFMPluginEvent(wxEVT_IFM_ADDTOPCONTAINER, container),
        m_index(before ? 1 : 0),
        m_indexContainer(index_container)
    { }

    virtual wxEvent *Clone() const { return new wxIFMAddTopContainerEvent(*this); }

    /*!
        \return True if index mode, false if relative mode
    */
    bool GetIndexMode() const { return m_indexContainer == NULL; }

    /*!
        \return If in index mode, the return value is the index desired for the container.
            A return of -1 means the container should be added to the end of the list.
            If in relative mode, the return value is 1 to dock before the relative container,
            and the return value is 0 to dock after the relative container.
    */
    int GetIndex() const { return m_index; }

    wxIFMComponent *GetIndexContainer() const { return m_indexContainer; }
};

/*!
    Event used to remove a container from the top level container list

    \see wxIFMAddTopContainerEvent
*/
class WXDLLIMPEXP_IFM wxIFMRemoveTopContainerEvent : public wxIFMPluginEvent
{
public:
    wxIFMRemoveTopContainerEvent(wxIFMComponent *container)
        : wxIFMPluginEvent(wxEVT_IFM_REMOVETOPCONTAINER, container)
    { }

    wxIFMRemoveTopContainerEvent(const wxIFMRemoveTopContainerEvent &event)
        : wxIFMPluginEvent(event)
    { }

    virtual wxEvent *Clone() const { return new wxIFMRemoveTopContainerEvent(*this); }
};

/*!
    Event used to select a tab of a panel
*/
class WXDLLIMPEXP_IFM wxIFMSelectTabEvent : public wxIFMPluginEvent
{
private:
    wxIFMTab *m_tab; //!< the tab being selected
    bool m_update; //!< update the interface

public:
    wxIFMSelectTabEvent(wxIFMComponent *panel, wxIFMTab *tab, bool update)
        : wxIFMPluginEvent(wxEVT_IFM_SELECTTAB, panel),
        m_tab(tab),
        m_update(update)
    { }

    wxIFMSelectTabEvent(const wxIFMSelectTabEvent &event)
        : wxIFMPluginEvent(event),
        m_tab(event.m_tab),
        m_update(event.m_update)
    { }

    virtual wxEvent *Clone() const { return new wxIFMSelectTabEvent(*this); }

    /*!
        \return The tab being selected
    */
    wxIFMTab *GetTab() { return m_tab; }

    /*!
        \return True if the interface should be immediately updated
    */
    bool GetUpdate() { return m_update; }
};

/*!
    Event used to notify plugins of component button events
*/
class WXDLLIMPEXP_IFM wxIFMComponentButtonEvent : public wxIFMPluginEvent
{
private:
    wxIFMComponentButton *m_button;

public:
    wxIFMComponentButtonEvent(wxEventType type, wxIFMComponent *component, wxIFMComponentButton *button)
        : wxIFMPluginEvent(type, component),
        m_button(button)
    { }

    /*!
        \return Component button triggering the event
    */
    wxIFMComponentButton *GetButton() const { return m_button; }
};

typedef void (wxEvtHandler::*wxIFMAddTopContainerEventFunction)(wxIFMAddTopContainerEvent&);
typedef void (wxEvtHandler::*wxIFMRemoveTopContainerEventFunction)(wxIFMRemoveTopContainerEvent&);
typedef void (wxEvtHandler::*wxIFMSelectTabEventFunction)(wxIFMSelectTabEvent&);
typedef void (wxEvtHandler::*wxIFMComponentButtonEventFunction)(wxIFMComponentButtonEvent&);

#define wxIFMAddTopContainerEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMAddTopContainerEventFunction, &func)
#define wxIFMRemoveTopContainerEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMRemoveTopContainerEventFunction, &func)
#define wxIFMSelectTabEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMSelectTabEventFunction, &func)
#define wxIFMComponentButtonEventFunctionHandler(func) \
    (wxObjectEventFunction)wxStaticCastEvent(wxIFMComponentButtonEventFunction, &func)

#define EVT_IFM_ADDTOPCONTAINER(func)       wx__DECLARE_EVT0(wxEVT_IFM_ADDTOPCONTAINER, wxIFMAddTopContainerEventFunctionHandler(func))
#define EVT_IFM_REMOVETOPCONTAINER(func)    wx__DECLARE_EVT0(wxEVT_IFM_REMOVETOPCONTAINER, wxIFMRemoveTopContainerEventFunctionHandler(func))
#define EVT_IFM_SELECTTAB(func)             wx__DECLARE_EVT0(wxEVT_IFM_SELECTTAB, wxIFMSelectTabEventFunctionHandler(func))
#define EVT_IFM_COMPONENTBUTTONCLICK(func)  wx__DECLARE_EVT0(wxEVT_IFM_COMPONENTBUTTONCLICK, wxIFMComponentButtonEventFunctionHandler(func))

#endif // _DEFINTERFACE_H_
