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


#ifndef _WX_JIGSAW_SHAPE_GATEGORY_H
#define _WX_JIGSAW_SHAPE_GATEGORY_H

#include <wx/wx.h>
#include <wx/cmndata.h>
#include <wx/list.h>
#include <wx/wxxmlserializer/XmlSerializer.h>
#include <wx/wxxmlserializer/PropertyIO.h>

class wxJigsawShapeCategory : public xsSerializable
{
	wxString m_CategoryName;
	wxColourData m_ColourData;
	wxArrayString m_ShapeFileNames;
	bool m_Overwrite_Shape_Color;

	void InitSerialization();
public:
	wxJigsawShapeCategory();
	wxJigsawShapeCategory(const wxJigsawShapeCategory & value);
	XS_DECLARE_CLONABLE_CLASS(wxJigsawShapeCategory);

	const wxString & GetCategoryName() {return m_CategoryName;}
	void SetCategoryName(const wxString & value) {m_CategoryName = value;}

	const bool GetOverwriteShapeColor() {return m_Overwrite_Shape_Color;}
	void SetOverwriteShapeColor(const bool value) {m_Overwrite_Shape_Color = value;}

	wxColourData & GetColourData() {return m_ColourData;}
	wxArrayString & GetShapeFileNames() {return m_ShapeFileNames;}
};

XS_DECLARE_IO_HANDLER(wxJigsawShapeCategory, xsJigsawShapeCategoryPropIO);
#define XS_SERIALIZE_JIGSAWSHAPECATEGORY(x, name) XS_SERIALIZE_PROPERTY(x, wxT("jigsawshapecategory"), name);

WX_DECLARE_LIST(wxJigsawShapeCategory, wxJigsawShapeCategoryList);

XS_DECLARE_IO_HANDLER(wxJigsawShapeCategoryList, xsListJigsawShapeCategoryPropIO)
#define XS_SERIALIZE_LISTJIGSAWSHAPECATEGORY(x, name) XS_SERIALIZE_PROPERTY(x, wxT("listjigsawshapecategory"), name);

#endif
