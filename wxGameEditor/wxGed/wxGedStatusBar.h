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


#ifndef _WXGEDSTATUSBAR_
#define _WXGEDSTATUSBAR_

#include "wx/statusbr.h"


class wxGedStatusBar : public wxStatusBar
{
	DECLARE_EVENT_TABLE()

public:

	wxGedStatusBar(wxWindow *parent,
                  wxWindowID id = wxID_ANY,
                  long style = wxST_SIZEGRIP,
                  const wxString& name = wxEmptyString);

	void OnPaint(wxPaintEvent& event);
	void OnEraseBackground( wxEraseEvent& event );

private:

	void DrawField(wxDC& dc, int i, bool bLast);
	void DrawFieldText(wxDC& dc, int i);

    //DECLARE_DYNAMIC_CLASS_NO_COPY(wxGedStatusBar)
};

#endif //_WXGEDSTATUSBAR_