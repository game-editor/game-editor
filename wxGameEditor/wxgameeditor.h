/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com

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
More info at http://game-editor.com/License
Be a Game Editor developer: http://game-editor.com/Sharing_Software_Revenues_in_Open_Source

***************************************************************************/


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
