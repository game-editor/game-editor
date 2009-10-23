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


#ifndef _WX_JIGSAW_DIAGRAM_H
#define _WX_JIGSAW_DIAGRAM_H

#include <wx/wx.h>
#include "wxJigsawShapeGroup.h"

/// \brief Jigsaw Diagram
class wxJigsawDiagram : public xsSerializable
{
	DECLARE_DYNAMIC_CLASS(wxJigsawDiagram)
	/// \brief List of groups of shapes
	wxJigsawShapeGroupList m_Groups;
public:
	/// \brief Constructor
	wxJigsawDiagram();
	/// \brief Copy constructor
	wxJigsawDiagram(const wxJigsawDiagram & diagram);

	/// \brief Returns the list of groups
	wxJigsawShapeGroupList & GetGroups();
	/// \brief Returns the group which contains specified shape
	/// \param shape Jigsaw shape object
	wxJigsawShapeGroup * GetShapeGroup(wxJigsawShape * shape);
	/// \brief Returns the size of diagram
	wxSize GetSize();
	/// \brief Updates the layout (moves all shapes to appropriate places)
	void UpdateLayout(wxDC & dc, double scale);
	void UpdateParents();

	/// \brief Clones current object
	virtual xsSerializable * Clone();
	/// \brief Initializes serialization (binds class members to xsSerializable properties
	void InitSerialization();
};

#endif
