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


#ifndef _WX_JIGSAW_EDITOR_CONFIG_H
#define _WX_JIGSAW_EDITOR_CONFIG_H

#include <wx/wx.h>
#include <wx/wxxmlserializer/XmlSerializer.h>
#include <wx/wxxmlserializer/PropertyIO.h>
#include "wxJigsawShapeCategory.h"

class wxJigsawEditorConfig : public xsSerializable
{
	wxJigsawShapeCategoryList m_Categories;
	wxArrayString m_ShapesLookupDirectories;
	XS_DECLARE_CLONABLE_CLASS(wxJigsawEditorConfig);
	void InitSerialization();
public:
	wxJigsawEditorConfig();
	wxJigsawEditorConfig(const wxJigsawEditorConfig & value);

	wxArrayString & GetShapesLookupDirectories() {return m_ShapesLookupDirectories;}
	wxJigsawShapeCategoryList & GetCategories() {return m_Categories;}
};

#endif