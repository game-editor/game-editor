#ifndef _WX_JIGSAW_EDITOR_CONFIG_H
#define _WX_JIGSAW_EDITOR_CONFIG_H

#include <wx/wx.h>
#include <wx/wxxmlserializer/XmlSerializer.h>
#include <wx/wxxmlserializer/PropertyIO.h>
#include "wxJigsawShapeCategory.h"

class wxJigsawEditorConfig : public xsSerializable
{
	wxJigsawShapeCategoryList m_Categories;
	wxArrayString m_ShapesLookupDirectories;
	XS_DECLARE_CLONABLE_CLASS(wxJigsawEditorConfig);
	void InitSerialization();
public:
	wxJigsawEditorConfig();
	wxJigsawEditorConfig(const wxJigsawEditorConfig & value);

	wxArrayString & GetShapesLookupDirectories() {return m_ShapesLookupDirectories;}
	wxJigsawShapeCategoryList & GetCategories() {return m_Categories;}
};

#endif