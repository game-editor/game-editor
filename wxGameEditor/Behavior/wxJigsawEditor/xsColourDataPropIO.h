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


#ifndef _XS_COLOUR_DATA_PROP_IO_H
#define _XS_COLOUR_DATA_PROP_IO_H

#include <wx/wx.h>
#include <wx/cmndata.h>
#include <wx/wxxmlserializer/XmlSerializer.h>
#include <wx/wxxmlserializer/PropertyIO.h>

// declaration of a class 'xsColourDataPropIO' encapsulating custom property I/O handler
// for 'wxColouData' data type
XS_DECLARE_IO_HANDLER(wxColourData, xsColourDataPropIO);

// definition macros which create new serialized wxColourData property (without/with defined default value). data type's assertion is not compulsory, but useful...
#define XS_SERIALIZE_COLOURDATA(x, name) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxColourData)), wxT("Object is not wxColourData"));XS_SERIALIZE_PROPERTY(x, wxT("colourdata"), name);
#define XS_SERIALIZE_COLOURDATA_EX(x, name, def) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxColourData)), wxT("Object is not wxColourData"));XS_SERIALIZE_PROPERTY_EX(x, wxT("colourdata"), name, xsColourDataPropIO::ToString(def));

#endif
