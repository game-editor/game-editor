#include "wxJigsawShapeGroupPropertyIO.h"

#include "wxJigsawShapePropertyIO.h"
#include "wxJigsawShapeGroup.h"

/////////////////////////////////////////////////////////////////////////////////////
// xsJigsawInputParameterPropIO class ///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsJigsawShapeGroupPropIO, xsPropertyIO);

void xsJigsawShapeGroupPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    wxXmlNode *objectNode = source->GetChildren();

    if( objectNode && (objectNode->GetName() == wxT("object")) )
    {
        *(wxJigsawShapeGroup**)(property->m_pSourceVariable) = 
			(wxJigsawShapeGroup*)
			wxCreateDynamicObject(objectNode->GetPropVal(wxT("type"), wxT("")));

		wxJigsawShapeGroup* object = *(wxJigsawShapeGroup**)(property->m_pSourceVariable);
        if(object)
        {
            object->DeserializeObject(objectNode);
        }
    }
}

void xsJigsawShapeGroupPropIO::Write(xsProperty *property, wxXmlNode *target)
{
	wxJigsawShapeGroup * object = *(wxJigsawShapeGroup**)(property->m_pSourceVariable);

	if( object && object->IsKindOf(CLASSINFO(wxJigsawShapeGroup)))
    {
        wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
        newNode->AddChild(object->SerializeObject(NULL));

        target->AddChild(newNode);
        AppendPropertyType(property, newNode);
    }
}

wxString xsJigsawShapeGroupPropIO::GetValueStr(xsProperty *property)
{
	return ToString(*(wxJigsawShapeGroup**)(property->m_pSourceVariable));
}

wxString xsJigsawShapeGroupPropIO::ToString(wxJigsawShapeGroup * value)
{
	return wxString::Format(wxT("wxJigsawShapeGroup at address 0x%x"), value);
}

wxJigsawShapeGroup * xsJigsawShapeGroupPropIO::FromString(const wxString&)
{
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsListJigsawShapeGroupPropIO class ///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsListJigsawShapeGroupPropIO, xsPropertyIO);

void xsListJigsawShapeGroupPropIO::Read(xsProperty *property, wxXmlNode *source)
{	
	wxJigsawShapeGroupList *list = (wxJigsawShapeGroupList*)property->m_pSourceVariable;
	
	// clear previous list content
	bool fDelState = list->GetDeleteContents();

	list->DeleteContents(true);
    list->Clear();
	list->DeleteContents(fDelState);

    wxXmlNode *listNode = source->GetChildren();
    while(listNode)
    {
		if(listNode->GetName() == wxT("object") && 
			listNode->GetPropVal(wxT("type"), wxEmptyString) == wxT("wxJigsawShapeGroup"))
        {
			wxJigsawShapeGroup * group = new wxJigsawShapeGroup;
			group->DeserializeObject(listNode);
			list->Append(group);
        }

        listNode = listNode->GetNext();
    }
}

void xsListJigsawShapeGroupPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    wxJigsawShapeGroupList *list = (wxJigsawShapeGroupList*)property->m_pSourceVariable;

    if(list->GetCount() > 0)
    {
        wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
		wxJigsawShapeGroupList::compatibility_iterator listNode = list->GetFirst();
        while(listNode)
        {
			wxJigsawShapeGroup * group = listNode->GetData();
            wxXmlNode * groupXmlNode = AddPropertyNode(newNode, wxT("object"), 
				xsJigsawShapeGroupPropIO::ToString(group));
			groupXmlNode->AddProperty(
				new wxXmlProperty(wxT("type"), group->GetClassInfo()->GetClassName()));
			group->SerializeObject(groupXmlNode);
            listNode = listNode->GetNext();
        }

        target->AddChild(newNode);
        AppendPropertyType(property, newNode);
    }
}

wxString xsListJigsawShapeGroupPropIO::GetValueStr(xsProperty *property)
{
	return ToString(*((wxJigsawShapeGroupList*)property->m_pSourceVariable));
}

wxString xsListJigsawShapeGroupPropIO::ToString(wxJigsawShapeGroupList value)
{
	wxString out = wxT("[ ");

	wxJigsawShapeGroupList::compatibility_iterator node = value.GetFirst();
	while( node )
	{
		out << xsJigsawShapeGroupPropIO::ToString(node->GetData());
		if( node != value.GetLast() ) out << wxT(" | ");

		node = node->GetNext();
	}

	out << wxT(" ]");

	return out;
}

wxJigsawShapeGroupList xsListJigsawShapeGroupPropIO::FromString(const wxString&)
{
	return wxJigsawShapeGroupList();
}
