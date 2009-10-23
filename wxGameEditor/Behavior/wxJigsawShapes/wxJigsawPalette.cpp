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


#include "wxJigsawPalette.h"
#include "wxJigsawShapePropertyIO.h"
#include <wx/listimpl.cpp>

IMPLEMENT_DYNAMIC_CLASS(wxJigsawPalette, xsSerializable)
WX_DEFINE_LIST(wxJigsawPaletteList);

wxJigsawPalette::wxJigsawPalette()
{
	m_Shapes.DeleteContents(true);
	InitSerialization();
}

wxJigsawPalette::wxJigsawPalette(const wxJigsawPalette & palette)
: xsSerializable(palette)
{
	m_Shapes.DeleteContents(true);
	for(wxJigsawShapeList::Node * node = palette.m_Shapes.GetFirst(); node; node = node->GetNext())
	{
		wxJigsawShape * srcShape = node->GetData();
		if(!srcShape) continue;
		m_Shapes.Append(new wxJigsawShape(*srcShape));
	}
	InitSerialization();
}

const wxString & wxJigsawPalette::GetPaletteName()
{
	return m_PaletteName;
}

void wxJigsawPalette::SetPaletteName(const wxString & value)
{
	m_PaletteName = value;
}

wxJigsawShapeList & wxJigsawPalette::GetShapes()
{
	return m_Shapes;
}

const wxColourData & wxJigsawPalette::GetColours()
{
	return m_Colours;
}

void wxJigsawPalette::SetColours(const wxColourData & value)
{
	m_Colours = value;
}

xsSerializable * wxJigsawPalette::Clone()
{
	return (xsSerializable *) new wxJigsawPalette(*this);
}

void wxJigsawPalette::InitSerialization()
{
	XS_SERIALIZE_LISTJIGSAWSHAPE(m_Shapes, wxT("shapes"));
}
