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


#include "wxJigsawHotSpot.h"
#include <wx/arrimpl.cpp>

WX_DEFINE_OBJARRAY(wxJigsawHotSpotArray)

wxJigsawHotSpot::wxJigsawHotSpot(wxJigsawShape * shape, 
	const wxRect & rect, wxJigsawHotSpotType type, int index)
	: m_Shape(shape), m_Rect(rect), m_HotSpotType(type), m_Index(index)
{
}

wxJigsawShape * wxJigsawHotSpot::GetShape()
{
	return m_Shape;
}

const wxRect & wxJigsawHotSpot::GetRect()
{
	return m_Rect;
}

wxJigsawHotSpotType wxJigsawHotSpot::GetHotSpotType()
{
	return m_HotSpotType;
}

int wxJigsawHotSpot::GetIndex()
{
	return m_Index;
}
