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


#ifndef _WX_JIGSAW_PALETTE_H
#define _WX_JIGSAW_PALETTE_H

#include <wx/wx.h>
#include <wx/list.h>
#include <wx/cmndata.h>
#include "wxJigsawShape.h"
#include <wx/wxxmlserializer/XmlSerializer.h>

class wxJigsawPalette : public xsSerializable
{
	DECLARE_DYNAMIC_CLASS(wxJigsawPalette)
	wxString m_PaletteName;
	wxColourData m_Colours;
	wxJigsawShapeList m_Shapes;
public:
	wxJigsawPalette();
	wxJigsawPalette(const wxJigsawPalette & palette);
	wxJigsawShapeList & GetShapes();

	const wxString & GetPaletteName();
	void SetPaletteName(const wxString & value);

	const wxColourData & GetColours();
	void SetColours(const wxColourData & value);

	/// \brief Clones current object
	virtual xsSerializable * Clone();

	/// \brief Initializes serialization (binds class members to xsSerializable properties
	void InitSerialization();
};

WX_DECLARE_LIST(wxJigsawPalette, wxJigsawPaletteList);

#endif
