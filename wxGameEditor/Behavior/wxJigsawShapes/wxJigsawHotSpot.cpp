#include "wxJigsawHotSpot.h"
#include <wx/arrimpl.cpp>

WX_DEFINE_OBJARRAY(wxJigsawHotSpotArray)

wxJigsawHotSpot::wxJigsawHotSpot(wxJigsawShape * shape, 
	const wxRect & rect, wxJigsawHotSpotType type, int index)
	: m_Shape(shape), m_Rect(rect), m_HotSpotType(type), m_Index(index)
{
}

wxJigsawShape * wxJigsawHotSpot::GetShape()
{
	return m_Shape;
}

const wxRect & wxJigsawHotSpot::GetRect()
{
	return m_Rect;
}

wxJigsawHotSpotType wxJigsawHotSpot::GetHotSpotType()
{
	return m_HotSpotType;
}

int wxJigsawHotSpot::GetIndex()
{
	return m_Index;
}
