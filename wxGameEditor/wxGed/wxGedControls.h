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


#ifndef _WX_GED_HANDLERS_H_
#define _WX_GED_HANDLERS_H_

#include "wx/defs.h"
#include "wx/xrc/xmlres.h"
#include "wx/wx.h"
#include "spinreal.h"


extern wxColour colorCaption;
extern wxColour colorCaptionText;
extern wxColour colorBgContent;
extern wxColour subCategoryColor;

// ----------------------------------------------------------------------------
// Pushbutton
// ----------------------------------------------------------------------------

class wxGedButton : public wxButton
{
	DECLARE_EVENT_TABLE()

public:
    wxGedButton();
    wxGedButton(wxWindow *parent,
             wxWindowID id,
             const wxString& label = wxEmptyString,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = 0,
             const wxValidator& validator = wxDefaultValidator,
             const wxString& name = wxButtonNameStr);

	bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& label = wxEmptyString,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxButtonNameStr);

	void OnEraseBackground(wxEraseEvent &event);
	void OnPaint(wxPaintEvent &event);

	void OnEnterWindow(wxMouseEvent &event);
	void OnLeaveWindow(wxMouseEvent &event);
	void OnLeftDown(wxMouseEvent &event);
	void OnLeftUp(wxMouseEvent &event);

private:

	bool bHover;
	bool bLeftDown;

    DECLARE_DYNAMIC_CLASS_NO_COPY(wxGedButton)
};

class wxGedStaticText : public wxStaticText
{
public:
    wxGedStaticText() { }

    /*wxGedStaticText(wxWindow *parent,
                 wxWindowID id,
                 const wxString& label,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0,
                 const wxString& name = wxStaticTextNameStr)
    {
        Create(parent, id, label, pos, size, style, name);
    }*/

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& label,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxStaticTextNameStr);

   

protected:

    DECLARE_DYNAMIC_CLASS_NO_COPY(wxGedStaticText)
};

class wxGedSpinDrag;
class wxGedSpin : public wxSpinReal
{
public:
    wxGedSpin() { drag = NULL; }

    wxGedSpin(wxWindow *parent,
               wxWindowID id = -1,
               const wxString& value = wxEmptyString,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxSP_ARROW_KEYS,
               float min = 0.0, float max = 100.0,
               float initial = 0.0,
               float increment = 0.1, int digits = -1,
               const wxString& name = _T("wxGedSpin"))
	{
        Create(parent, id, value, pos, size, style,
               min, max, initial, increment, digits, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id = -1,
                const wxString& value = wxEmptyString,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxSP_ARROW_KEYS,
                float min = 0.0, float max = 100.0,
                float initial = 0.0,
                float increment = 0.1, int digits = -1,
                const wxString& name = _T("wxSpinReal"));
   

    virtual ~wxGedSpin();

private:	

	wxGedSpinDrag *drag;

    DECLARE_DYNAMIC_CLASS(wxGedSpin)
    DECLARE_EVENT_TABLE()
};

class wxGedSpinDrag : public wxWindow
{
    DECLARE_CLASS(wxGedSpinDrag)
    DECLARE_EVENT_TABLE()

public:

    wxGedSpinDrag(wxGedSpin *parent, int _id = 0); 
    virtual ~wxGedSpinDrag();


    
private:
	void OnPaint(wxPaintEvent &event);    
    void OnEraseBackground(wxEraseEvent &event);

	void OnLeftDown(wxMouseEvent &event);
	void OnLeftUp(wxMouseEvent &event);
	void OnMouseMove(wxMouseEvent &event);

	wxPoint mouseClickPos;
	int screenXCenter, xCenter;
	bool bButtonDown;
};


class WXDLLIMPEXP_XRC wxGedButtonXmlHandler : public wxXmlResourceHandler
{
DECLARE_DYNAMIC_CLASS(wxGedButtonXmlHandler)
public:
    wxGedButtonXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

class WXDLLIMPEXP_XRC wxGedStaticTextXmlHandler : public wxXmlResourceHandler
{
DECLARE_DYNAMIC_CLASS(wxGedStaticTextXmlHandler)
public:
    wxGedStaticTextXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};


#endif
    // _WX_GED_HANDLERS_H_