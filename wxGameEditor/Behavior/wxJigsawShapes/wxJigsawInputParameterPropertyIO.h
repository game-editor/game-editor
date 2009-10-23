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


#ifndef _WX_JIGSAW_INPUT_PARAMETER_PROPERTY_IO
#define _WX_JIGSAW_INPUT_PARAMETER_PROPERTY_IO

#include <wx/wx.h>
#include <wx/wxxmlserializer/XmlSerializer.h>

class wxJigsawInputParameter;
class wxJigsawInputParameters;

XS_DECLARE_EXPORTED_IO_HANDLER(wxJigsawInputParameter *, xsJigsawInputParameterPropIO, WXDLLIMPEXP_XS);
XS_DECLARE_EXPORTED_IO_HANDLER(wxJigsawInputParameters, xsListJigsawInputParameterPropIO, WXDLLIMPEXP_XS);

#define XS_SERIALIZE_LISTJIGSAWINPUTPARAMETER(x, name) \
	XS_SERIALIZE_PROPERTY(x, wxT("listjigsawinputparameter"), name);

#endif
