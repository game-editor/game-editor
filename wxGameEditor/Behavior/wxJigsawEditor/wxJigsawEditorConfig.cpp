#include "wxJigsawEditorConfig.h"

XS_IMPLEMENT_CLONABLE_CLASS(wxJigsawEditorConfig, xsSerializable)

wxJigsawEditorConfig::wxJigsawEditorConfig()
{
	m_Categories.DeleteContents(true);
	InitSerialization();
}

wxJigsawEditorConfig::wxJigsawEditorConfig(const wxJigsawEditorConfig & value)
:xsSerializable(value)
{
	m_Categories.DeleteContents(true);
	WX_APPEND_ARRAY(m_ShapesLookupDirectories, value.m_ShapesLookupDirectories);
	for(wxJigsawShapeCategoryList::Node * node = value.m_Categories.GetFirst();
		node; node = node->GetNext())
	{
		wxJigsawShapeCategory * category = node->GetData();
		m_Categories.Append((wxJigsawShapeCategory *)category->Clone());
	}
	InitSerialization();
}

void wxJigsawEditorConfig::InitSerialization()
{
	XS_SERIALIZE_LISTJIGSAWSHAPECATEGORY(m_Categories, wxT("ShapeCategories"));
	XS_SERIALIZE_ARRAYSTRING(m_ShapesLookupDirectories, wxT("ShapesLookupDirectories"));
}
