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


#include "xsColourDataPropIO.h"

/////////////////////////////////////////////////////////////////////////////////////
// xsColourDataPropIO class /////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

// define class encapsulating custom data format handler
XS_DEFINE_IO_HANDLER(wxColourData, xsColourDataPropIO);

// two following static member functions of the data handler class MUST be defined manualy:

// wxString xsPropIO::ToString(T value) -> creates a string representation of the given value
wxString xsColourDataPropIO::ToString(wxColourData value)
{
	wxString out;

	out << xsColourPropIO::ToString(value.GetColour());
	for(int i = 0; i < 16; i++)
	{
		out << wxT("|") << xsColourPropIO::ToString(value.GetCustomColour(i));
	}

	return out;
}

// T xsPropIO::FromString(const wxString& value) -> converts data from given string representation to its relevant value
wxColourData xsColourDataPropIO::FromString(const wxString& value)
{
	wxColourData data;

	if(!value.IsEmpty())
	{
		int i = 0;
		wxStringTokenizer tokens(value, wxT("|"), wxTOKEN_STRTOK);

		data.SetColour(xsColourPropIO::FromString(tokens.GetNextToken()));
		while(tokens.HasMoreTokens())
		{
			data.SetCustomColour(i, xsColourPropIO::FromString(tokens.GetNextToken()));
			i++;
		}
	}

	return data;
}
