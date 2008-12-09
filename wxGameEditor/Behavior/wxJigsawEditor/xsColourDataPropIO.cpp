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
