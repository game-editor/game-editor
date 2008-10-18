 /*
    Demo application for wxIFM

    Copyright (c) Robin McNeill
    Licensed under the terms of the wxWindows license
*/

#include <wx/ifm/ifm.h>

#include <wx/app.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/statusbr.h>
#include <wx/panel.h>
#include <wx/toolbar.h>
//#include <wx/artprov.h>
#include <wx/sysopt.h>
#include <wx/hashmap.h>
#include <wx/settings.h>

//icon for our tabs
#include "classes.xpm"

#define WINDOW_MENU_START       0
#define WINDOW_MENU_END         1000
#define WINDOW_MENU_SHOW_ALL    1001
#define WINDOW_MENU_HIDE_ALL    1002

WX_DECLARE_HASH_MAP(int, wxWindow*, wxIntegerHash, wxIntegerEqual, wxWindowMap);

class MyApp : public wxApp
{
public:

    bool OnInit();
};

class MyFrame : public wxFrame
{
    int count;

public:
    wxInterfaceManager *m_ifm; // the interface management object
    wxWindowMap m_windows;
    wxPanel *m_root_panel;

    MyFrame(wxWindow* parent, wxWindowID id, const wxString& title,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE);

    void OnClose(wxCloseEvent &event);
    void OnMenu(wxCommandEvent &event);
    void OnShowAll(wxCommandEvent &event);
    void OnSize(wxSizeEvent &event);
    void OnEraseBackground(wxEraseEvent &event);
    void OnMenuOpen(wxMenuEvent &event);

    wxWindow* CreateTextWindow(bool visible = true);

    DECLARE_EVENT_TABLE()
};

DECLARE_APP(MyApp);
IMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    //wxSystemOptions::SetOption(wxT("msw.window.no-clip-children"), 1);
    wxFrame *frame = new MyFrame(NULL, wxID_ANY, wxT("wxIFM Test Application"), wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE);
    SetTopWindow(frame);
    frame->Show();
    return true;
}

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_CLOSE(MyFrame::OnClose)
    EVT_MENU_RANGE(WINDOW_MENU_START, WINDOW_MENU_END, MyFrame::OnMenu)
    EVT_MENU(WINDOW_MENU_SHOW_ALL, MyFrame::OnShowAll)
    EVT_MENU(WINDOW_MENU_HIDE_ALL, MyFrame::OnShowAll)
    //EVT_SIZE(MyFrame::OnSize)
    EVT_ERASE_BACKGROUND(MyFrame::OnEraseBackground)
    EVT_MENU_OPEN(MyFrame::OnMenuOpen)
END_EVENT_TABLE()

MyFrame::MyFrame(wxWindow* parent, wxWindowID id, const wxString& title,
        const wxPoint& pos, const wxSize& size,
        long style)
        : wxFrame(parent, id, title, pos, size, style),
        count(0),
        m_ifm(NULL)
{
    // make some menus
    wxMenuBar *mb = new wxMenuBar;
    wxMenu *menu1 = new wxMenu;
    menu1->Append(WINDOW_MENU_SHOW_ALL, wxT("Show All Windows"));
    menu1->Append(WINDOW_MENU_HIDE_ALL, wxT("Hide All Windows"));
    mb->Append(menu1, wxT("Window"));
    SetMenuBar(mb);

    CreateStatusBar();
    GetStatusBar()->SetStatusText(wxT("Hello"), 0);

    // toolbar
    wxToolBar *tb;
    tb =  CreateToolBar( wxTB_FLAT|wxTB_HORIZONTAL|wxTB_TEXT|wxTB_NODIVIDER, 2000);
    tb->SetToolBitmapSize(wxSize(32, 32));
    tb->AddTool(2500, wxT("   Tool1   "), wxBitmap(classes_xpm));
    tb->AddTool(2501, wxT("   Tool2   "), wxBitmap(classes_xpm));
    tb->AddCheckTool(2502, wxT("   Tool3   "), wxBitmap(classes_xpm));
    tb->Realize();
    SetToolBar(tb); 

    // panel as the only child
    m_root_panel = new wxPanel(this);

    // connect erase background for the root panel to get flicker free drawing back
    m_root_panel->Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(MyFrame::OnEraseBackground));

    // create the interface management object
    m_ifm = new wxInterfaceManager(m_root_panel);

    m_ifm->Initialize(true/*, IFM_NO_RESIZE_UPDATE*/);
    //m_ifm->SetInterfaceRect(wxRect(50,50,800,800));

    // specify the status bar pane to use for displaying messages
    m_ifm->SetStatusMessagePane(GetStatusBar(), 0);

    // Add children to be managed

    // First we need to create the add child data object
    // We are using the default interface, so we use its child data object
    wxIFMDefaultChildData data;

    // add some windows in a format most common to programming IDE's
    data.m_name = wxT("Build");
    data.m_type = IFM_CHILD_GENERIC;
    data.m_desiredSize.Set(200, 150);
    data.m_minSize.Set(80,60);
    data.m_orientation = IFM_ORIENTATION_BOTTOM; // bottom of the frame
    data.m_child = CreateTextWindow();
    data.m_hideable = false; // prevent this window from being closed by the user

    // note that this call does not change any of the values of the data passed to it,
    // so we don't need to set everything after each call, and can use the same data
    // many times
    m_ifm->AddChild(&data);
    data.m_child = CreateTextWindow();
    m_ifm->AddChild(&data);

    data.m_name = wxT("Console");
    data.m_desiredSize.Set(200, 150);
    data.m_tabify = true; // make this child a tab of the previously added child
    data.m_child = CreateTextWindow();

    m_ifm->AddChild(&data);
    data.m_child = CreateTextWindow();
    m_ifm->AddChild(&data);

    data.m_name = wxT("Classes");
    data.m_desiredSize.Set(150, 200);
    data.m_minSize.Set(150,80);
    data.m_orientation = IFM_ORIENTATION_LEFT; // left of the frame
    data.m_tabify = false; // not a tab this time
    data.m_child = CreateTextWindow();
    data.m_bitmap = wxBitmap(classes_xpm);
    data.m_hideable = true; // this window can be closed

    m_ifm->AddChild(&data);
    data.m_bitmap = wxNullBitmap;
    data.m_child = CreateTextWindow();
    m_ifm->AddChild(&data);

    // reuse the old size and orientation
    data.m_name = wxT("Files");
    data.m_minSize = IFM_NO_MINIMUM_SIZE;
    data.m_child = CreateTextWindow();
    data.m_tabify = true; // make this a tab

    m_ifm->AddChild(&data);
    data.m_child = CreateTextWindow();
    m_ifm->AddChild(&data);

    // we are done creating children, now lets make a textbox for the content window
	wxTextCtrl *content = new wxTextCtrl(m_root_panel, wxID_ANY, wxT("This is the content window"),
        wxPoint(), wxSize(), wxTE_MULTILINE | wxTE_WORDWRAP);
    content->LoadFile(wxT("demo_text.txt"));
    m_ifm->SetContentWindow( content );

    // demonstrate using the configuration api to change the font for captions and tabs
    // Note that wxSYS_DEFAULT_GUI_FONT is the default setting, so this won't actually
    // change the appearance of the demo, its only for demonstrating the api
    wxIFMCaptionConfig &caption_config = wxIFMDefaultPanelPlugin::GetCaptionConfig();
    caption_config.set_font(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));

    wxIFMTabConfig &tab_config = wxIFMDefaultPanelPlugin::GetTabConfig();
    tab_config.set_font(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
	tab_config.set_rounding_factor(5);

    m_ifm->UpdateConfiguration();
}

void MyFrame::OnClose(wxCloseEvent &event)
{
    // shut down the interface management object
    m_ifm->Shutdown();

    // it is now safe to delete it
    delete m_ifm;

    event.Skip();
}

void MyFrame::OnMenu(wxCommandEvent &event)
{
    // hide or show the appropriate window
    m_ifm->ShowChild(m_windows[event.GetId()], event.IsChecked(), true);
}

void MyFrame::OnMenuOpen(wxMenuEvent &event) {
    wxMenu *menu = GetMenuBar()->GetMenu(0);
    for(int i = 0; i < count; i++)
        menu->Check(i, m_ifm->IsChildVisible(m_windows[i]));
}

wxWindow *MyFrame::CreateTextWindow(bool visible)
{
    wxWindow *window = new wxTextCtrl(m_root_panel, -1, wxString::Format(wxT("%d"), count), wxDefaultPosition, wxSize(0,0), wxTE_MULTILINE);
    m_windows.insert(wxWindowMap::value_type(count, window));
    GetMenuBar()->GetMenu(0)->AppendCheckItem(count, wxString::Format(wxT("Show window %d"), count));
    window->SetSizeHints(0,0);
    return m_windows[count++];
}

void MyFrame::OnShowAll(wxCommandEvent &event)
{
    bool show;
    if( event.GetId() == WINDOW_MENU_SHOW_ALL )
        show = true;
    else
        show = false;

    // hide or show all components, but don't update the interface imediately
    for( wxWindowMap::iterator i = m_windows.begin(), end = m_windows.end(); i != end; i++ )
        m_ifm->ShowChild(i->second, show, false);

    // now update the interface, including floating windows
    m_ifm->Update(IFM_DEFAULT_RECT, true);
}

void MyFrame::OnSize(wxSizeEvent &event)
{
    if( m_ifm )
        m_ifm->Update(wxRect(50,100,500,800));
}

void MyFrame::OnEraseBackground(wxEraseEvent &event)
{

}
