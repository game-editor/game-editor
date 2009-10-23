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


#ifndef _WX_JIGSAW_HOTSPOT_H
#define _WX_JIGSAW_HOTSPOT_H

#include <wx/wx.h>
#include <wx/dynarray.h>
#include <smart_enum.h>

class wxJigsawShape;

class wxJigsawHotSpotType_def
{
public:
	/// Values of shape's style
	enum type
	{
		/// No style (default)
		wxJS_HOTSPOT_BUMP,
		/// Numeric
		wxJS_HOTSPOT_NOTCH,
		/// Boolean
		wxJS_HOTSPOT_INNER_NOTCH,
		/// Default style (alias to wxJS_TYPE_NONE)
		wxJS_HOTSPOT_INPUT_PARAMETER
	};
};

typedef smart_enum<wxJigsawHotSpotType_def> wxJigsawHotSpotType;

class wxJigsawHotSpot
{
	wxJigsawShape * m_Shape;
	wxRect m_Rect;
	wxJigsawHotSpotType m_HotSpotType;
	int m_Index;
public:
	wxJigsawHotSpot(wxJigsawShape * shape, const wxRect & rect, 
		wxJigsawHotSpotType type, int index = wxID_ANY);

	wxJigsawShape * GetShape();
	const wxRect & GetRect();
	wxJigsawHotSpotType GetHotSpotType();
	int GetIndex();
};

WX_DECLARE_OBJARRAY(wxJigsawHotSpot, wxJigsawHotSpotArray);

#endif
