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
