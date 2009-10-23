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


#include "wxJigsawEditorConfig.h"

XS_IMPLEMENT_CLONABLE_CLASS(wxJigsawEditorConfig, xsSerializable)

wxJigsawEditorConfig::wxJigsawEditorConfig()
{
	m_Categories.DeleteContents(true);
	InitSerialization();
}

wxJigsawEditorConfig::wxJigsawEditorConfig(const wxJigsawEditorConfig & value)
:xsSerializable(value)
{
	m_Categories.DeleteContents(true);
	WX_APPEND_ARRAY(m_ShapesLookupDirectories, value.m_ShapesLookupDirectories);
	for(wxJigsawShapeCategoryList::Node * node = value.m_Categories.GetFirst();
		node; node = node->GetNext())
	{
		wxJigsawShapeCategory * category = node->GetData();
		m_Categories.Append((wxJigsawShapeCategory *)category->Clone());
	}
	InitSerialization();
}

void wxJigsawEditorConfig::InitSerialization()
{
	XS_SERIALIZE_LISTJIGSAWSHAPECATEGORY(m_Categories, wxT("ShapeCategories"));
	XS_SERIALIZE_ARRAYSTRING(m_ShapesLookupDirectories, wxT("ShapesLookupDirectories"));
}
