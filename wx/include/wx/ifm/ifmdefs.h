/*!
    Defintions for the IFM system

    \file   ifmdefs.h
    \author Robin McNeill
    \date   Created: 10/15/04

    Copyright (c) Robin McNeill
    Licensed under the terms of the wxWindows license
*/

#ifndef _IFM_DEFS_H_
#define _IFM_DEFS_H_

// ===================================================
//                COMPILE TIME SETTINGS
// ===================================================

//! Controls whether or not Panels with tabs are allowed to become tabs within other panels
#define IFM_ALLOW_NESTED_PANELS 0

// make cvs integration easier by using standard wxwidgets export methods
#ifdef WXMAKINGDLL_IFM
#    define WXDLLIMPEXP_IFM WXEXPORT
#elif defined(WXUSINGDLL)
#    define WXDLLIMPEXP_IFM WXIMPORT
#else /* not making nor using DLL */
#    define WXDLLIMPEXP_IFM
#endif

#include <wx/hashmap.h>
#include <wx/dynarray.h>

//#include <vector>
//#include <map>

#include <wx/app.h>

#include "wx/wfstream.h" //maks
#include "wx/datstrm.h" //maks

// DLL options compatibility check:
// this code is used when integrate with wxWidgets CVS
//WX_CHECK_BUILD_OPTIONS("wxIFM")

#define IFM_GET_EXTENSION_DATA(c, d)    (wxDynamicCast((c)->GetExtensionData(d::DataKey()), d))

WX_DECLARE_HASH_MAP(void *, int, wxPointerHash, wxPointerEqual, wxPointerMap); //maks
extern wxPointerMap globalPointerMap; //maks

/*
Some forward declares
*/
class wxIFMInterfacePluginBase;
class wxIFMExtensionPluginBase;
class wxIFMPluginEvent;
class wxInterfaceManager;
class wxIFMExtensionDataBase;
class wxIFMFloatingData;
class wxIFMComponent;
class wxIFMChildDataBase;
class wxIFMFloatingWindowBase;

//typedef std::vector<wxIFMInterfacePluginBase *> wxIFMInterfacePluginArray;
//typedef std::vector<wxIFMComponent *> wxIFMComponentArray;
//typedef std::vector<wxIFMFloatingWindowBase *> wxIFMFloatingWindowArray;
WX_DEFINE_ARRAY(wxIFMInterfacePluginBase*, wxIFMInterfacePluginArray);
WX_DEFINE_ARRAY(wxIFMComponent*, wxIFMComponentArray);
WX_DEFINE_ARRAY(wxIFMFloatingWindowBase*, wxIFMFloatingWindowArray);

/*
Data key stuff
*/
typedef int wxIFMComponentDataKeyType; //!< Data type used to retrieve component data
//typedef std::map<const wxIFMComponentDataKeyType, wxIFMExtensionDataBase *> wxIFMComponentDataMap;
WX_DECLARE_HASH_MAP(wxIFMComponentDataKeyType, wxIFMExtensionDataBase*, wxIntegerHash, wxIntegerEqual, wxIFMComponentDataMap);
#define DECLARE_IFM_DATA_KEY(name)  extern const wxIFMComponentDataKeyType name;
#define DEFINE_IFM_DATA_KEY(name)   const wxIFMComponentDataKeyType name = GetNewDataKey();

/*
Component type stuff
*/
#define DECLARE_IFM_COMPONENT_TYPE(name)    extern const int name;
#define DEFINE_IFM_COMPONENT_TYPE(name)     const int name = GetNewComponentType();
#define IFM_COMPONENT_UNDEFINED 0 // uninitialized component (error!)
#define IFM_COMPONENT_FLOATING_ROOT -1 //!< Special value used to create root components for floating windows

/*
Child types stuff
*/
class wxWindow;
//typedef std::map<const wxWindow*, wxIFMComponent*> wxIFMChildWindowMap;
WX_DECLARE_HASH_MAP(unsigned int, wxIFMComponent*, wxIntegerHash, wxIntegerEqual, wxIFMChildWindowMap);
#define DECLARE_IFM_CHILD_TYPE(name)    extern WXDLLIMPEXP_IFM const int name;
#define DEFINE_IFM_CHILD_TYPE(name)     const int name = GetNewChildType();
#define IFM_CHILDTYPE_UNDEFINED 0 // don't know (error!)
DECLARE_IFM_CHILD_TYPE(IFM_CHILD_GENERIC) // defined in manager.cpp
DECLARE_IFM_CHILD_TYPE(IFM_CHILD_TOOLBAR) // defined in manager.cpp

#define IFM_COORDS_ABSOLUTE     1 //!< The actual size of the component including everything
#define IFM_COORDS_BACKGROUND   2 /*!< \brief Area in which the component can draw its background.
                                       Excludes border widths*/
#define IFM_COORDS_CLIENT       3 /*!< \brief Area in which a component is allowed to place children.
                                       Excludes margin and border widths*/
#define IFM_COORDS_FLOATINGWINDOW 4 //!< \brief Size of the floating window that would contain this component.

#define IFM_ALIGN_NONE          0 //!< erronous allignment value
#define IFM_ALIGN_HORIZONTAL    1 //!< children of a component are layed out horizontally
#define IFM_ALIGN_VERTICAL      2 //!< children of a component are layed out vertically

/*
    Various flags
*/
#define IFM_NO_RESIZE_UPDATE    0x00000001 //!< don't update the interface in response to an EVT_SIZE event
#define IFM_DEFAULT_FLAGS       0

#define IFM_DISABLE_STATUS_MESSAGES -1 //!< used to disable the display of status messages in the frames status bar
#define IFM_USE_CURRENT_VALUE   -1 //!< used to specify that existing values should be used during SetRect events
#define IFM_NO_MAXIMUM          -2 //!< no maximum size for a given direction
#define IFM_NO_MINIMUM          -3 //!< no minimum size for a given direction
#define IFM_NO_MAXIMUM_SIZE     wxSize(IFM_NO_MAXIMUM,IFM_NO_MAXIMUM) //!< no maximum size
#define IFM_NO_MINIMUM_SIZE     wxSize(IFM_NO_MINIMUM,IFM_NO_MINIMUM) //!< no minimum size
#define IFM_USE_CHILD_MINSIZE   wxSize(-2,-2) //!< query the child for its min size with wxWindow::GetBestSize
#define IFM_USE_CURRENT_SIZE    wxSize(IFM_USE_CURRENT_VALUE,IFM_USE_CURRENT_VALUE)
#define IFM_DEFAULT_INDEX       -1
#define IFM_DEFAULT_POS         wxPoint(-1, -1)
#define IFM_DEFAULT_SIZE        wxSize(100, 100)
#define IFM_DEFAULT_RECT        wxRect(IFM_DEFAULT_POS, wxSize(-1,-1))
#define IFM_INTERFACE_ACTIVE    -1
#define IFM_INTERFACE_ALL       -2
#define IFM_DEFAULT_COMPONENT_NAME  wxT("")

// floating
#if defined(__WXMSW__) || defined(__WXGTK__) || defined(__WXMAC__)
#   define IFM_CANFLOAT    1 // floating supported
#else
#   define IFM_CANFLOAT    0 // no reparent yet
#endif

class wxIFMDimensions;

/*!
    Data structure used to stream the widths of borders and margins for each side of a component
*/
class wxIFMDimensions
{
public:
    int top, bottom, left, right;

    wxIFMDimensions()
    {
        top =
        bottom =
        left =
        right = 0;
    }

    /*!
        Sets the dimension value for all sides.
        \param i Dimension value to set.
    */
    void Set(int i)
    {
        top =
        left =
        right =
        bottom = i;
    }

    wxIFMDimensions &operator =(const wxIFMDimensions &rect)
    {
        top = rect.top;
        bottom = rect.bottom;
        left = rect.left;
        right = rect.right;
        return *this;
    }
};

typedef wxIFMDimensions wxIFMBorders;
typedef wxIFMDimensions wxIFMMargins;

// Rect and Size object arrays needed for certain events
WX_DECLARE_EXPORTED_OBJARRAY(wxRect, wxRectArray);
WX_DECLARE_EXPORTED_OBJARRAY(wxSize, wxSizeArray);


/////////////////////////////////////
//maks: color schema
extern wxColour wxBorder_Pen;
extern wxColour wxBorder_PenDark;
extern wxColour wxBackground_Pen;
extern wxColour wxResizer_Pen;
extern wxColour wxResizerBorder_Pen;

extern wxColour wxTab_border_p;
extern wxColour wxTab_active_bg_b;
extern wxColour wxTab_active_bg_p;
extern wxColour wxTab_inactive_bg_b;
extern wxColour wxTab_inactive_bg_p;
extern wxColour wxTab_bg_brush;
extern wxColour wxTab_bg_pen;
/////////////////////////////////////


class wxIFMLayoutTabData //maks
{
public:
    
    wxIFMLayoutTabData()
    {
        
    }
    
    wxIFMLayoutTabData &operator =(const wxIFMLayoutTabData &other)
    {
		m_desiredSize = other.m_desiredSize; 
		m_hidden = other.m_hidden;
		m_hideable = other.m_hideable;
		m_fixed = other.m_fixed;		
		m_pos = other.m_pos;
		bCanDrag = other.bCanDrag;
		m_name = other.m_name;		
		bActive = other.bActive;
		m_newRow = other.m_newRow;
		
		return *this;
    }

	void operator>>(wxDataOutputStream &stream);
	void operator<<(wxDataInputStream &stream);
	
	//Tab	
    wxSize m_desiredSize; //!< Initial size of the child
    bool m_hidden; //!< Child visibility state
    bool m_hideable; //!< If the child can be hidden
    bool m_fixed; /*!< \brief This child window cannot be resized.
                    If this is true, the values of minSize and maxSize will be ignored,
                    minSize and maxSize will be set to m_desiredSize, and the component will
                    always be the size specified in m_desiredSize. */

	wxPoint m_pos;
	bool bCanDrag;
	wxString m_name;
	
	bool bActive;
	bool m_newRow;
};

WX_DECLARE_OBJARRAY(wxIFMLayoutTabData, wxIFMLayoutTabDataArray); //maks

class wxIFMLayoutPanelData //maks
{
public:
    
    wxIFMLayoutPanelData()
    {
        
    }
    
    wxIFMLayoutPanelData &operator =(const wxIFMLayoutPanelData &other)
    {
        return *this;
    }

	void AddTab(wxIFMLayoutTabData& tab)
	{
		m_tabs.push_back(tab);
	}

	void operator>>(wxDataOutputStream &stream);
	void operator<<(wxDataInputStream &stream);

	
	//Panel	


	//Tabs
	wxIFMLayoutTabDataArray m_tabs;
};

WX_DECLARE_OBJARRAY(wxIFMLayoutPanelData, wxIFMLayoutPanelDataArray); //maks

class wxIFMLayoutContainerData //maks
{
public:
    
    wxIFMLayoutContainerData()
    {
        
    }

    
    wxIFMLayoutContainerData &operator=(const wxIFMLayoutContainerData &other)
    {
		m_orientation	= other.m_orientation;
        m_rect			= other.m_rect;

        return *this;
    }

	void AddPanel(wxIFMLayoutPanelData& panel)
	{
		m_panels.push_back(panel);
	}

	void operator>>(wxDataOutputStream &stream);
	void operator<<(wxDataInputStream &stream);
	
	//Container
	int m_orientation;	
	wxRect m_rect;

	//Panels
	wxIFMLayoutPanelDataArray m_panels;
};


WX_DECLARE_OBJARRAY(wxIFMLayoutContainerData, wxIFMLayoutContainerDataArray); //maks


class wxIFMLayoutData //maks
{
public:
    
    wxIFMLayoutData();
    
    wxIFMLayoutData &operator=(const wxIFMLayoutData &other)
    {
        m_containers = other.m_containers;
		bMaximized = other.bMaximized;
		bShowTitleBar = other.bShowTitleBar;
		m_rect = other.m_rect;

        return *this;
    }

	void GetFrameInfo();

	void operator>>(wxDataOutputStream &stream);
	void operator<<(wxDataInputStream &stream);

	

	//Frame
	bool bMaximized;
	bool bShowTitleBar;
	wxRect m_rect;
	
	//Containers
	wxIFMLayoutContainerDataArray m_containers;	
};

class wxIFMWindowsData //maks
{
public:
	wxIFMWindowsData() {}

	wxIFMWindowsData(wxWindow *_window, wxSize _m_minSize, wxSize _m_maxSize)
	{
		window = _window;
		m_minSize = _m_minSize;
		m_maxSize = _m_maxSize;
	}

	wxIFMWindowsData &operator=(const wxIFMWindowsData &other)
    {
        window = other.window;
		m_minSize = other.m_minSize;
		m_maxSize = other.m_maxSize;

        return *this;
    }

	wxWindow *window;
	wxSize m_minSize;
	wxSize m_maxSize;
};

WX_DECLARE_STRING_HASH_MAP( wxIFMLayoutData, wxIFMLayoutMap ); //maks
WX_DECLARE_STRING_HASH_MAP( wxIFMWindowsData, MapWindowsName ); //maks


class wxIFMLayoutControl //maks
{
public:
    
    wxIFMLayoutControl();
	~wxIFMLayoutControl();   
	
	void AddLayout(wxIFMLayoutData &layout, wxString name);
	void AddLayout(wxString name);
	bool RemoveLayout(wxString name);
	wxIFMLayoutData *GetLayout(wxString name);

	void LoadCurrent();
	void LoadDefault();
	bool SaveLayout(wxIFMLayoutData &save);
	bool LoadLayout(wxIFMLayoutData &load);
	bool LoadLayout(wxString &name);
	void SaveDefault();
	void SaveCurrent();
	void Save(wxString name = wxT(""));
	void Load(wxString name = wxT(""));

	void Init(wxString _fileName);

	int size() {return layouts.size();}

	wxString getDefaultString() {return defaultString;}
	const wxStringList& GetLayoutList();

	static void setNonMaximizedFrameRect(wxRect rect) {m_NonMaximizedFrameRect = rect;}
	static wxRect getNonMaximizedFrameRect() {return m_NonMaximizedFrameRect;}
	
private:
	void SaveLayoutContainer(wxIFMComponent *container, wxIFMLayoutContainerDataArray &save, wxIFMComponent **lastContainer); //maks

	wxIFMLayoutMap layouts;
	wxStringList layoutList; //maks: avoid the crash wuen call layouts.begin()
	wxIFMLayoutData current;

	wxString fileName, defaultString;

	static wxRect m_NonMaximizedFrameRect;
};

#define LAYOUT_STATE_SAVE 1
#define LAYOUT_STATE_LOAD 2

class wxLayoutStateEvent : public wxCommandEvent //maks
{
public:

    /** Constructor. */
    wxLayoutStateEvent(int _type = 0, void *_stream = NULL);

    /** Copy constructor. */
    wxLayoutStateEvent(const wxLayoutStateEvent& event);

    /** Destructor. */
    ~wxLayoutStateEvent();

    /** Copyer. */
    virtual wxEvent *Clone() const
                  { return new wxLayoutStateEvent(*this); }


    int GetType() {return type;}
	wxDataOutputStream *GetOutputStream() {return type == LAYOUT_STATE_SAVE?(wxDataOutputStream *)stream:NULL;}
	wxDataInputStream *GetInputStream() {return type == LAYOUT_STATE_LOAD?(wxDataInputStream *)stream:NULL;}

private:

	int type;
	void *stream;

    DECLARE_DYNAMIC_CLASS(wxLayoutStateEvent)

};

typedef void (wxEvtHandler::*wxLayoutStateEventFunction)
                                        (wxLayoutStateEvent&);


BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LAYOUT_STATE, 801)
END_DECLARE_EVENT_TYPES()

#define EVT_LAYOUT_STATE(id, fn)              DECLARE_EVENT_TABLE_ENTRY( wxEVT_COMMAND_LAYOUT_STATE, id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxLayoutStateEventFunction, & fn ), (wxObject *) NULL ),




#endif // _IFM_DEFS_H_
