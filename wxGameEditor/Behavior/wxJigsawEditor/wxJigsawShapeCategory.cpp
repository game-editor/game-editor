#include "wxJigsawShapeCategory.h"
#include "xsColourDataPropIO.h"
#include <wx/listimpl.cpp>

XS_IMPLEMENT_CLONABLE_CLASS(wxJigsawShapeCategory, xsSerializable);
WX_DEFINE_LIST(wxJigsawShapeCategoryList);

wxJigsawShapeCategory::wxJigsawShapeCategory()
{
	m_ColourData.SetCustomColour(0, wxColour(132,125,132));
	m_ColourData.SetCustomColour(1, wxColour(74,69,74));
	m_ColourData.SetCustomColour(2, wxColour(0,0,0));
	m_ColourData.SetCustomColour(3, wxColour(57,56,57));
	m_ColourData.SetCustomColour(4, wxColour(57,56,57));
	m_ColourData.SetCustomColour(5, wxColour(0,0,0));
	m_Overwrite_Shape_Color = false;
	InitSerialization();
}

wxJigsawShapeCategory::wxJigsawShapeCategory(const wxJigsawShapeCategory & value)
: xsSerializable(value), m_ColourData(value.m_ColourData), m_Overwrite_Shape_Color(value.m_Overwrite_Shape_Color),
m_ShapeFileNames(value.m_ShapeFileNames)
{
	InitSerialization();
}

void wxJigsawShapeCategory::InitSerialization()
{
	XS_SERIALIZE_STRING_EX(m_CategoryName, wxT("CategoryName"), wxEmptyString);
	XS_SERIALIZE_COLOURDATA(m_ColourData, wxT("Colours"));
	XS_SERIALIZE(m_Overwrite_Shape_Color, wxT("overwrite_shape_color"));
	XS_SERIALIZE_ARRAYSTRING(m_ShapeFileNames, wxT("ShapeFileNames"));
}

/////////////////////////////////////////////////////////////////////////////////////
// xsJigsawShapeCategoryPropIO class ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsJigsawShapeCategoryPropIO, xsPropertyIO);

void xsJigsawShapeCategoryPropIO::Read(xsProperty *property, wxXmlNode *source)
{
	wxXmlNode *objectNode = source->GetChildren();

	if( objectNode && (objectNode->GetName() == wxT("object")) )
	{
		*(wxJigsawShapeCategory**)(property->m_pSourceVariable) = 
			(wxJigsawShapeCategory*)wxCreateDynamicObject(objectNode->GetPropVal(wxT("type"), wxT("")));

		wxJigsawShapeCategory * object = *(wxJigsawShapeCategory**)(property->m_pSourceVariable);
		if(object)
		{
			object->DeserializeObject(objectNode);
		}
	}
}

void xsJigsawShapeCategoryPropIO::Write(xsProperty *property, wxXmlNode *target)
{
	wxJigsawShapeCategory * object = *(wxJigsawShapeCategory**)(property->m_pSourceVariable);

	if( object && object->IsKindOf(CLASSINFO(wxJigsawShapeCategory)))
	{
		wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
		newNode->AddChild(object->SerializeObject(NULL));

		target->AddChild(newNode);
		AppendPropertyType(property, newNode);
	}
}

wxString xsJigsawShapeCategoryPropIO::GetValueStr(xsProperty *property)
{
	return ToString(*(*(wxJigsawShapeCategory**)(property->m_pSourceVariable)));
}

wxString xsJigsawShapeCategoryPropIO::ToString(wxJigsawShapeCategory value)
{
	return ""; //return wxString::Format(wxT("wxJigsawShapeCategory at address 0x%x"), value);
}

wxJigsawShapeCategory xsJigsawShapeCategoryPropIO::FromString(const wxString&)
{
	return wxJigsawShapeCategory();
}

/////////////////////////////////////////////////////////////////////////////////////
// xsListJigsawShapeCategoryPropIO class ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsListJigsawShapeCategoryPropIO, xsPropertyIO);

void xsListJigsawShapeCategoryPropIO::Read(xsProperty *property, wxXmlNode *source)
{	
	wxJigsawShapeCategoryList *list = (wxJigsawShapeCategoryList*)property->m_pSourceVariable;

	// clear previous list content
	bool fDelState = list->GetDeleteContents();

	list->DeleteContents(true);
	list->Clear();
	list->DeleteContents(fDelState);

	wxXmlNode *listNode = source->GetChildren();
	while(listNode)
	{
		if(listNode->GetName() == wxT("object") && 
			listNode->GetPropVal(wxT("type"), wxEmptyString) == wxT("wxJigsawShapeCategory"))
		{
			wxJigsawShapeCategory * shape = new wxJigsawShapeCategory;
			shape->DeserializeObject(listNode);
			list->Append(shape);
		}

		listNode = listNode->GetNext();
	}
}

void xsListJigsawShapeCategoryPropIO::Write(xsProperty *property, wxXmlNode *target)
{
	wxJigsawShapeCategoryList *list = (wxJigsawShapeCategoryList*)property->m_pSourceVariable;

	if(list->GetCount() > 0)
	{
		wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
		wxJigsawShapeCategoryList::compatibility_iterator listNode = list->GetFirst();
		while(listNode)
		{
			wxJigsawShapeCategory * shapeCategory = listNode->GetData();
			wxXmlNode * shapeXmlNode = AddPropertyNode(newNode, wxT("object"), 
				xsJigsawShapeCategoryPropIO::ToString(*shapeCategory));
			shapeXmlNode->AddProperty(
				new wxXmlProperty(wxT("type"), shapeCategory->GetClassInfo()->GetClassName()));
			shapeCategory->SerializeObject(shapeXmlNode);
			listNode = listNode->GetNext();
		}

		target->AddChild(newNode);
		AppendPropertyType(property, newNode);
	}
}

wxString xsListJigsawShapeCategoryPropIO::GetValueStr(xsProperty *property)
{
	return ToString(*((wxJigsawShapeCategoryList*)property->m_pSourceVariable));
}

wxString xsListJigsawShapeCategoryPropIO::ToString(wxJigsawShapeCategoryList value)
{
	wxString out = wxT("[ ");

	wxJigsawShapeCategoryList::compatibility_iterator node = value.GetFirst();
	while( node )
	{
		out << xsJigsawShapeCategoryPropIO::ToString(*(node->GetData()));
		if( node != value.GetLast() ) out << wxT(" | ");

		node = node->GetNext();
	}

	out << wxT(" ]");

	return out;
}

wxJigsawShapeCategoryList xsListJigsawShapeCategoryPropIO::FromString(const wxString&)
{
	return wxJigsawShapeCategoryList();
}
