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


#ifndef _WX_JIGSAW_SHAPE_PROPERTY_IO_H
#define _WX_JIGSAW_SHAPE_PROPERTY_IO_H

#include <wx/wx.h>
#include <wx/wxxmlserializer/XmlSerializer.h>

class wxJigsawShape;
class wxJigsawShapeList;

/// \brief wxXmlSerializer IO Handler for wxJigsawShape
XS_DECLARE_EXPORTED_IO_HANDLER(wxJigsawShape *, xsJigsawShapePropIO, WXDLLIMPEXP_XS);
/// \brief wxXmlSerializer IO Handler for wxJigsawShapeList
XS_DECLARE_EXPORTED_IO_HANDLER(wxJigsawShapeList, xsListJigsawShapePropIO, WXDLLIMPEXP_XS);

#define XS_SERIALIZE_JIGSAWSHAPE(x, name) XS_SERIALIZE_PROPERTY(x, wxT("jigsawshape"), name);
#define XS_SERIALIZE_LISTJIGSAWSHAPE(x, name) XS_SERIALIZE_PROPERTY(x, wxT("listjigsawshape"), name);

#endif
