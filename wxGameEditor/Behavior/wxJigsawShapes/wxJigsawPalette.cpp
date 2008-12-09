#include "wxJigsawPalette.h"
#include "wxJigsawShapePropertyIO.h"
#include <wx/listimpl.cpp>

IMPLEMENT_DYNAMIC_CLASS(wxJigsawPalette, xsSerializable)
WX_DEFINE_LIST(wxJigsawPaletteList);

wxJigsawPalette::wxJigsawPalette()
{
	m_Shapes.DeleteContents(true);
	InitSerialization();
}

wxJigsawPalette::wxJigsawPalette(const wxJigsawPalette & palette)
: xsSerializable(palette)
{
	m_Shapes.DeleteContents(true);
	for(wxJigsawShapeList::Node * node = palette.m_Shapes.GetFirst(); node; node = node->GetNext())
	{
		wxJigsawShape * srcShape = node->GetData();
		if(!srcShape) continue;
		m_Shapes.Append(new wxJigsawShape(*srcShape));
	}
	InitSerialization();
}

const wxString & wxJigsawPalette::GetPaletteName()
{
	return m_PaletteName;
}

void wxJigsawPalette::SetPaletteName(const wxString & value)
{
	m_PaletteName = value;
}

wxJigsawShapeList & wxJigsawPalette::GetShapes()
{
	return m_Shapes;
}

const wxColourData & wxJigsawPalette::GetColours()
{
	return m_Colours;
}

void wxJigsawPalette::SetColours(const wxColourData & value)
{
	m_Colours = value;
}

xsSerializable * wxJigsawPalette::Clone()
{
	return (xsSerializable *) new wxJigsawPalette(*this);
}

void wxJigsawPalette::InitSerialization()
{
	XS_SERIALIZE_LISTJIGSAWSHAPE(m_Shapes, wxT("shapes"));
}
