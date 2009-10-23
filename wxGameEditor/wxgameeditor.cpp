/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

If GPL v3 does not work for you, we offer a "non-GPL" license 
<http://game-editor.com/License>, thats will help us to pay our developers 
<http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>.

***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// Name:        wxgameeditor.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     01/08/06 14:24:22
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

//http://www.wxwidgets.org/wiki/index.php/MSVC_.NET_Setup_Guide
#if defined(__WXMSW__) && !defined(__WXWINCE__)
#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='X86' publicKeyToken='6595b64144ccf1df'\"")
#endif

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "wxgameeditor.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include "wx/cmdline.h"
#include "wxGed/wxGedControls.h"
#include "GameControl.h"


#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "wxgameeditor.h"
#include <wx/dcbuffer.h>
#include <wx/image.h>
#include "SDL_video.h"

////@begin XPM images
////@end XPM images



#define TIMER_ID 1000


void EngineStart();
void EngineLoad(const char *gamePath);
void EngineShutDown();
bool EngineTick();

/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( wxGameEditorApp )
////@end implement app

/*!
 * wxGameEditorApp type definition
 */

IMPLEMENT_CLASS( wxGameEditorApp, wxApp )

/*!
 * wxGameEditorApp event table definition
 */

BEGIN_EVENT_TABLE( wxGameEditorApp, wxApp )

////@begin wxGameEditorApp event table entries
////@end wxGameEditorApp event table entries

	/*EVT_END_SESSION(wxGameEditorApp::OnEndSession)
    EVT_QUERY_END_SESSION(wxGameEditorApp::OnQueryEndSession)*/

END_EVENT_TABLE()

/*!
 * Constructor for wxGameEditorApp
 */

wxGameEditorApp::wxGameEditorApp()
{
////@begin wxGameEditorApp member initialisation
////@end wxGameEditorApp member initialisation
}

/*!
 * Initialisation for wxGameEditorApp
 */

static wxString loadGameAtStartup;

bool wxGameEditorApp::OnInit()
{    

    // Remove the comment markers above and below this block
    // to make permanent changes to the code.


#if wxUSE_XPM
    wxImage::AddHandler( new wxXPMHandler );
#endif
#if wxUSE_LIBPNG
    wxImage::AddHandler( new wxPNGHandler );
#endif
#if wxUSE_LIBJPEG
    wxImage::AddHandler( new wxJPEGHandler );
#endif
#if wxUSE_GIF
    wxImage::AddHandler( new wxGIFHandler );
#endif

	wxXmlResource::Get()->AddHandler(new wxGedButtonXmlHandler);
	wxXmlResource::Get()->AddHandler(new wxGedStaticTextXmlHandler);


	//Define colors
	wxIFMCaptionConfig &caption_config = wxIFMDefaultPanelPlugin::GetCaptionConfig();
	

	//Light blue
	/*wxBorder_Pen		= wxColour(144, 152, 160);
	wxBorder_PenDark	= wxColour(144, 152, 160);
	wxBackground_Pen	= wxColour(144, 152, 160);

	wxResizer_Pen		= wxColour(143, 143, 143);
	wxResizerBorder_Pen	= wxColour(100, 100, 100);

	colorCaption		= wxColour(144, 152, 160);
	colorCaptionText	= wxColour(0, 0, 0);

	wxTab_border_p		= wxColour(100, 100, 100);
	wxTab_active_bg_b	= wxColour(210, 210, 210);
	wxTab_active_bg_p	= wxColour(144, 152, 160);

	wxTab_inactive_bg_b	= wxColour(140, 140, 140);
	wxTab_inactive_bg_p	= wxTab_inactive_bg_b;

	wxTab_bg_brush		= wxColour(140, 140, 140);
	wxTab_bg_pen		= wxColour(144, 152, 160);

	colorBgContent		= wxColour(144, 152, 160);/**/

	//Gray border
	/*wxBorder_Pen		= wxColour(100, 100, 100);
	wxBorder_PenDark	= wxColour(100, 100, 100);
	wxBackground_Pen	= wxColour(167, 167, 167);

	wxResizer_Pen		= wxColour(150, 150, 150);
	wxResizerBorder_Pen	= wxColour(100, 100, 100);

	colorCaption		= wxColour(167, 167, 167);
	colorCaptionText	= wxColour(0, 0, 0);

	wxTab_border_p		= wxColour(100, 100, 100);
	wxTab_active_bg_b	= wxColour(210, 210, 210);
	wxTab_active_bg_p	= wxColour(167, 167, 167);

	wxTab_inactive_bg_b	= wxColour(140, 140, 140);
	wxTab_inactive_bg_p	= wxTab_inactive_bg_b;

	wxTab_bg_brush		= wxColour(140, 140, 140);
	wxTab_bg_pen		= wxColour(167, 167, 167);

	colorBgContent		= wxColour(167, 167, 167);/**/

	

	//Flat
	/*wxBorder_Pen		= wxColour(167, 167, 167);
	wxBorder_PenDark	= wxColour(167, 167, 167);
	wxBackground_Pen	= wxColour(167, 167, 167);

	wxResizer_Pen		= wxColour(167, 167, 167);
	wxResizerBorder_Pen	= wxColour(100, 100, 100);

	colorCaption		= wxColour(167, 167, 167);
	colorCaptionText	= wxColour(0, 0, 0);

	wxTab_border_p		= wxColour(100, 100, 100);
	wxTab_active_bg_b	= wxColour(210, 210, 210);
	wxTab_active_bg_p	= wxColour(167, 167, 167);

	wxTab_inactive_bg_b	= wxColour(140, 140, 140);
	wxTab_inactive_bg_p	= wxTab_inactive_bg_b;

	wxTab_bg_brush		= wxColour(140, 140, 140);
	wxTab_bg_pen		= wxColour(167, 167, 167);

	colorBgContent		= wxColour(167, 167, 167);/**/



	//Black Orange
	wxBorder_Pen		= wxColour(104, 104, 104);
	wxBorder_PenDark	= wxColour(wxBorder_Pen.Red()/2, wxBorder_Pen.Green()/2, wxBorder_Pen.Blue()/2);
	wxBackground_Pen	= wxColour(72, 72, 72);

	wxResizer_Pen		= wxBackground_Pen;
	wxResizerBorder_Pen	= wxBorder_Pen;

	colorCaption		= wxColour(48, 48, 48);
	colorCaptionText	= wxColour(220, 220, 220);

	wxTab_border_p		= wxColour(72, 72, 72);
	wxTab_active_bg_b	= wxColour(244, 156, 28);
	wxTab_active_bg_p	= wxColour(96, 96, 96);
	wxTab_inactive_bg_b	= wxColour(174, 174, 174);
	wxTab_inactive_bg_p	= wxTab_inactive_bg_b;
	wxTab_bg_brush		= wxColour(96, 96, 96);
	wxTab_bg_pen		= wxColour(96, 96, 96);

	colorBgContent		= wxColour(176, 184, 192);//wxColour(144, 152, 160);
	subCategoryColor		= wxColour(255,255,183);
	caption_config.set_3d_boder(false);
	
	//Gradient
	caption_config.set_caption_style(IFM_CAPTION_GRADIENT);
	caption_config.set_caption_color(colorCaption);
	caption_config.set_caption_gradient_color(wxTab_bg_brush);/**/


	//Gray border thin
	/*wxColour			backGround(167, 167, 167);
	wxBorder_Pen		= wxColour(backGround);
	wxBorder_PenDark	= wxColour(backGround);
	wxBackground_Pen	= wxColour(backGround);

	wxResizer_Pen		= wxColour(backGround);
	wxResizerBorder_Pen	= wxColour(backGround);

	colorCaption		= wxColour(backGround);
	colorCaptionText	= wxColour(0, 0, 0);

	wxTab_border_p		= wxColour(100, 100, 100);
	wxTab_active_bg_b	= wxColour(210, 210, 210);
	wxTab_active_bg_p	= wxColour(backGround);

	wxTab_inactive_bg_b	= wxColour(140, 140, 140);
	wxTab_inactive_bg_p	= wxTab_inactive_bg_b;

	wxTab_bg_brush		= wxColour(140, 140, 140);
	wxTab_bg_pen		= wxColour(backGround);

	colorBgContent		= wxColour(backGround);
	
	caption_config.set_3d_boder(true);
	
	//Solid
	caption_config.set_caption_color(wxTab_bg_brush);

	/**/



	//Get application directory
	wxFileName arg(argv[0]);
	applicationDir = arg.GetPath(wxPATH_GET_SEPARATOR | wxPATH_GET_VOLUME );

	if(argc == 2 && argv[1])
	{
		loadGameAtStartup = argv[1];
	}


	//Load resources
    wxXmlResource::Get()->InitAllHandlers(); 
    wxXmlResource::Get()->Load("resources.xrc"); 
    
	//Create the frame
    wxMainFrame* mainWindow = new wxMainFrame( applicationDir, ID_FRAME );
    SetTopWindow(mainWindow);
    mainWindow->Show(true);


    return true;
}

int wxGameEditorApp::OnRun() 
{
    // initialize SDL
    /*if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "unable to init SDL: " << SDL_GetError() << '\n';
        
        return -1;
    }
    
    // Setup video mode, but don't create a window
    SDL_SetVideoMode(0, 0, 0, SDL_SWSURFACE);
    
    // generate an initial idle event to start things
    wxIdleEvent event;
    event.SetEventObject(&frame->getPanel());
    frame->getPanel().AddPendingEvent(event);*/

    // start the main loop
    return wxApp::OnRun();
}

int wxGameEditorApp::OnExit() 
{
    
    
    // return the standard exit code
    return wxApp::OnExit();
}

/*void wxGameEditorApp::OnEndSession(wxCloseEvent& WXUNUSED(event))
{
    if (GetTopWindow())
        GetTopWindow()->Close(true);
}

// Default behaviour: close the application with prompts. The
// user can veto the close, and therefore the end session.
void wxGameEditorApp::OnQueryEndSession(wxCloseEvent& event)
{
    if (GetTopWindow())
    {
        if (!GetTopWindow()->Close(!event.CanVeto()))
            event.Veto(true);
    }
}*/




IMPLEMENT_CLASS(SDLPanel, wxPanel)

BEGIN_EVENT_TABLE(SDLPanel, wxPanel)
    EVT_PAINT(SDLPanel::onPaint)
    EVT_ERASE_BACKGROUND(SDLPanel::onEraseBackground)
	EVT_TIMER(TIMER_ID, SDLPanel::OnTimer)
	EVT_IDLE(SDLPanel::onIdle)	
END_EVENT_TABLE()

SDLPanel *SDLPanel::sdlPanel = NULL;

SDLPanel::SDLPanel(wxWindow *parent) 
: wxPanel(parent, ID_SDL_PANEL),
	m_timer(this, TIMER_ID)
{
	sdlPanel = this;

	char windowID[64];
	sprintf(windowID, "SDL_WINDOWID=%ld", (unsigned long)GetHandle());

	putenv(windowID);

	EngineStart();

	if(loadGameAtStartup.empty())
	{
		EngineLoad(NULL);
	}
	else
	{
		EngineLoad(loadGameAtStartup.wx_str());
	}

	m_timer.Start(10);

	// generate an initial idle event to start things
    wxIdleEvent event;
    event.SetEventObject(this);
    AddPendingEvent(event);
}

SDLPanel::~SDLPanel() 
{
	sdlPanel = NULL;
    EngineShutDown();
}

void SDLPanel::onPaint(wxPaintEvent &event) 
{
	event.Skip();
}


inline void SDLPanel::onEraseBackground(wxEraseEvent &event) 
{

}

void SDLPanel::OnTimer(wxTimerEvent& event)
{
	if(!GameControl::Get()->getGameMode())
	{		
		EngineTick();
	}
}

void SDLPanel::onIdle(wxIdleEvent &event) 
{
	//Don't works without this
	EngineTick();	
}



void *GetSDLPanelWindow()
{
	if(SDLPanel::Get())
	{
		return SDLPanel::Get()->GetHandle();
	}

	return NULL;
}

void *GetMainWindow()
{
	if(wxMainFrame::Get())
	{
		return wxMainFrame::Get()->GetHandle();
	}

	return NULL;
}

void SetMainWindowCaption(const char *str, const char *icon)
{
	if(wxMainFrame::Get())
	{
		wxMainFrame::Get()->SetTitle(str);		
	}
}

void ShowMainWindow(bool bShow)
{
	if(wxMainFrame::Get())
	{
		wxMainFrame::Get()->Show(bShow);
	}
}

void SetMainWindowGameMode(bool bGameMode)
{	
	if(wxMainFrame::Get()) 
	{
		if(GameControl::Get()->getFullScreen())
		{
			//If using a different window in game mode
			wxMainFrame::Get()->Show(!bGameMode);
			wxMainFrame::Get()->Enable(!bGameMode);

			if(!bGameMode)
			{
				wxInterfaceManager::GetManager()->setProcessEvents(true);

				//Restore layout
				wxMainFrame::Get()->GetLayoutControl()->LoadCurrent();
			}
			else
			{
				//Save current layout
				wxMainFrame::Get()->GetLayoutControl()->SaveCurrent();

				wxMainFrame::Get()->Move(10000, 10000);

				wxInterfaceManager::GetManager()->setProcessEvents(false);
			}
		}
		else
		{	
			//Resize main window in game mode
			static wxRect rect;
			static int oldStyle = 0;
			static wxMenuBar *menu = NULL;
			static wxStatusBar *status = NULL;
			static wxToolBar *tool = NULL;
			
			if(!bGameMode) 
			{
				//Enable event processing
				wxInterfaceManager::GetManager()->setProcessEvents(true);

				//Enable resize
				if(oldStyle) wxMainFrame::Get()->SetWindowStyle(oldStyle);
				oldStyle = 0;
				
				//Enable status and menu
				wxMainFrame::Get()->SetStatusBar(status);
				wxMainFrame::Get()->SetMenuBar(menu);
				if(tool) tool->Show(true);//wxMainFrame::Get()->SetToolBar(tool);
				
				//Move the SDL panel
				if(wxMainFrame::Get()->GetRootPanel()) wxMainFrame::Get()->GetRootPanel()->Move(0, 0);
				
				//Change frame size
				if(rect.GetWidth() > 1) wxMainFrame::Get()->SetSize(rect);			
				rect = wxRect(0, 0, 0, 0);
				
				//Restore layout
				wxMainFrame::Get()->GetLayoutControl()->LoadCurrent();

				//For unusable sash remotion
				wxInterfaceManager::GetManager()->UpdateConfiguration();
				
				//Set focus to stage, if active
				if(SDLPanel::Get() && SDLPanel::Get()->IsShown()) 
				{
					SDLPanel::Get()->SetFocus();
				}

				//Limit min frame size
				wxMainFrame::Get()->SetSizeHints(750, 550);
			}
			else 
			{
				//Save current layout
				wxMainFrame::Get()->GetLayoutControl()->SaveCurrent();				
				
				//Get frame rect
				rect = wxMainFrame::Get()->GetRect();
				
				//Disable resize
				oldStyle = wxMainFrame::Get()->GetWindowStyle();
				int style = oldStyle | (wxCAPTION | wxSYSTEM_MENU);
				style &= ~(wxRESIZE_BORDER | /*wxMINIMIZE_BOX |*/ wxMAXIMIZE_BOX);
				
				wxMainFrame::Get()->SetWindowStyle(style);				
				wxMainFrame::Get()->Maximize(false);
				
				//Disable status and menu
				status = wxMainFrame::Get()->GetStatusBar();
				menu = wxMainFrame::Get()->GetMenuBar();
				tool = wxMainFrame::Get()->GetToolBar();
				wxMainFrame::Get()->SetStatusBar(NULL);
				wxMainFrame::Get()->SetMenuBar(NULL);
				if(tool) tool->Show(false);//wxMainFrame::Get()->SetToolBar(NULL);

				//Remove min frame size
				wxMainFrame::Get()->SetSizeHints(0, 0);
				
				//Change frame size
				wxMainFrame::Get()->SetClientSize(GameControl::Get()->Width(), GameControl::Get()->Height());
				wxMainFrame::Get()->Center();
				
			
				//Move the SDL panel
				wxMainFrame::Get()->SetupSDLPanelGameModePosition();
				
				//Disable event processing (solve the repaint problem when stage is smaller than game resolution)
				wxInterfaceManager::GetManager()->setProcessEvents(false);
			}
		}
	}
}

