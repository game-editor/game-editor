/////////////////////////////////////////////////////////////////////////////
// Name:        wxgameeditor.h
// Purpose:     
// Author:      
// Modified by: 
// Created:     01/08/06 14:24:22
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXGAMEEDITOR_H_
#define _WXGAMEEDITOR_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "wxgameeditor.cpp"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "MainFrame.h"
////@end includes

#include "SDL.h"


#define ID_SDL_PANEL		12123
#define ID_GAME_GRAPH_PANEL 12124


/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

/*!
 * wxGameEditorApp class declaration
 */

class wxGameEditorApp: public wxApp
{    
    DECLARE_CLASS( wxGameEditorApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    wxGameEditorApp();

    /// Initialises the application
    virtual bool OnInit();

	virtual int OnRun();

    /// Called on exit
    virtual int OnExit();

	wxString GetApplicationDir() {return applicationDir;}

////@begin wxGameEditorApp event handler declarations

////@end wxGameEditorApp event handler declarations

////@begin wxGameEditorApp member function declarations

////@end wxGameEditorApp member function declarations

////@begin wxGameEditorApp member variables
////@end wxGameEditorApp member variables

	/*void OnEndSession(wxCloseEvent& event);
	void OnQueryEndSession(wxCloseEvent& event);*/

private:
	wxString applicationDir;
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(wxGameEditorApp)
////@end declare app



/*******************************************************************************
// SDLPanel Class
*******************************************************************************/

class SDLPanel : public wxPanel 
{
    DECLARE_CLASS(SDLPanel)
    DECLARE_EVENT_TABLE()

public:
    /**
     * Creates a new SDLPanel.
     *
     * @param parent The wxWindow parent.
     */
    SDLPanel(wxWindow *parent);
    
    /**
     * Destructs this SDLPanel.
     */
    virtual ~SDLPanel();

	static SDLPanel *Get() {return sdlPanel;}
    
private:

    void onPaint(wxPaintEvent &event);    
    void onEraseBackground(wxEraseEvent &event);
	void onIdle(wxIdleEvent &event);
	void OnTimer(wxTimerEvent& event);


	wxTimer m_timer;
	static SDLPanel *sdlPanel;

};

#endif
    // _WXGAMEEDITOR_H_
