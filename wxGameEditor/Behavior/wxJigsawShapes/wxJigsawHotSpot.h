#ifndef _WX_JIGSAW_HOTSPOT_H
#define _WX_JIGSAW_HOTSPOT_H

#include <wx/wx.h>
#include <wx/dynarray.h>
#include <smart_enum.h>

class wxJigsawShape;

class wxJigsawHotSpotType_def
{
public:
	/// Values of shape's style
	enum type
	{
		/// No style (default)
		wxJS_HOTSPOT_BUMP,
		/// Numeric
		wxJS_HOTSPOT_NOTCH,
		/// Boolean
		wxJS_HOTSPOT_INNER_NOTCH,
		/// Default style (alias to wxJS_TYPE_NONE)
		wxJS_HOTSPOT_INPUT_PARAMETER
	};
};

typedef smart_enum<wxJigsawHotSpotType_def> wxJigsawHotSpotType;

class wxJigsawHotSpot
{
	wxJigsawShape * m_Shape;
	wxRect m_Rect;
	wxJigsawHotSpotType m_HotSpotType;
	int m_Index;
public:
	wxJigsawHotSpot(wxJigsawShape * shape, const wxRect & rect, 
		wxJigsawHotSpotType type, int index = wxID_ANY);

	wxJigsawShape * GetShape();
	const wxRect & GetRect();
	wxJigsawHotSpotType GetHotSpotType();
	int GetIndex();
};

WX_DECLARE_OBJARRAY(wxJigsawHotSpot, wxJigsawHotSpotArray);

#endif
