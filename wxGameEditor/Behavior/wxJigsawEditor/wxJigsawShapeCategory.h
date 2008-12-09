#ifndef _WX_JIGSAW_SHAPE_GATEGORY_H
#define _WX_JIGSAW_SHAPE_GATEGORY_H

#include <wx/wx.h>
#include <wx/cmndata.h>
#include <wx/list.h>
#include <wx/wxxmlserializer/XmlSerializer.h>
#include <wx/wxxmlserializer/PropertyIO.h>

class wxJigsawShapeCategory : public xsSerializable
{
	wxString m_CategoryName;
	wxColourData m_ColourData;
	wxArrayString m_ShapeFileNames;
	void InitSerialization();
public:
	wxJigsawShapeCategory();
	wxJigsawShapeCategory(const wxJigsawShapeCategory & value);
	XS_DECLARE_CLONABLE_CLASS(wxJigsawShapeCategory);

	const wxString & GetCategoryName() {return m_CategoryName;}
	void SetCategoryName(const wxString & value) {m_CategoryName = value;}

	wxColourData & GetColourData() {return m_ColourData;}
	wxArrayString & GetShapeFileNames() {return m_ShapeFileNames;}
};

XS_DECLARE_IO_HANDLER(wxJigsawShapeCategory, xsJigsawShapeCategoryPropIO);
#define XS_SERIALIZE_JIGSAWSHAPECATEGORY(x, name) XS_SERIALIZE_PROPERTY(x, wxT("jigsawshapecategory"), name);

WX_DECLARE_LIST(wxJigsawShapeCategory, wxJigsawShapeCategoryList);

XS_DECLARE_IO_HANDLER(wxJigsawShapeCategoryList, xsListJigsawShapeCategoryPropIO)
#define XS_SERIALIZE_LISTJIGSAWSHAPECATEGORY(x, name) XS_SERIALIZE_PROPERTY(x, wxT("listjigsawshapecategory"), name);

#endif
