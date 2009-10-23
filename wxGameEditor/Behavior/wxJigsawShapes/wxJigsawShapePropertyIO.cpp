/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

If GPL v3 does not work for you, we offer a "non-GPL" license 
<http://game-editor.com/License>, thats will help us to pay our developers 
<http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>.

***************************************************************************/


#include "wxJigsawShapePropertyIO.h"
#include "wxJigsawShape.h"

/////////////////////////////////////////////////////////////////////////////////////
// xsJigsawShapePropIO class ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsJigsawShapePropIO, xsPropertyIO);

void xsJigsawShapePropIO::Read(xsProperty *property, wxXmlNode *source)
{
    wxXmlNode *objectNode = source->GetChildren();

    if( objectNode && (objectNode->GetName() == wxT("object")) )
    {
        *(wxJigsawShape**)(property->m_pSourceVariable) = 
			(wxJigsawShape*)wxCreateDynamicObject(objectNode->GetPropVal(wxT("type"), wxT("")));

        wxJigsawShape * object = *(wxJigsawShape**)(property->m_pSourceVariable);
        if(object)
        {
            object->DeserializeObject(objectNode);
        }
    }
}

void xsJigsawShapePropIO::Write(xsProperty *property, wxXmlNode *target)
{
    wxJigsawShape * object = *(wxJigsawShape**)(property->m_pSourceVariable);

    if( object && object->IsKindOf(CLASSINFO(wxJigsawShape)))
    {
        wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
        newNode->AddChild(object->SerializeObject(NULL));

        target->AddChild(newNode);
        AppendPropertyType(property, newNode);
    }
}

wxString xsJigsawShapePropIO::GetValueStr(xsProperty *property)
{
    return ToString(*(wxJigsawShape**)(property->m_pSourceVariable));
}

wxString xsJigsawShapePropIO::ToString(wxJigsawShape * value)
{
	return ""; //return wxString::Format(wxT("wxJigsawShape at address 0x%x"), value);
}

wxJigsawShape * xsJigsawShapePropIO::FromString(const wxString&)
{
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsListJigsawShapePropIO class ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsListJigsawShapePropIO, xsPropertyIO);

void xsListJigsawShapePropIO::Read(xsProperty *property, wxXmlNode *source)
{	
	wxJigsawShapeList *list = (wxJigsawShapeList*)property->m_pSourceVariable;
	
	// clear previous list content
	bool fDelState = list->GetDeleteContents();

	list->DeleteContents(true);
    list->Clear();
	list->DeleteContents(fDelState);

    wxXmlNode *listNode = source->GetChildren();
    while(listNode)
    {
        if(listNode->GetName() == wxT("object") && 
			listNode->GetPropVal(wxT("type"), wxEmptyString) == wxT("wxJigsawShape"))
        {
			wxJigsawShape * shape = new wxJigsawShape;
			shape->DeserializeObject(listNode);
			list->Append(shape);
        }

        listNode = listNode->GetNext();
    }
}

void xsListJigsawShapePropIO::Write(xsProperty *property, wxXmlNode *target)
{
    wxJigsawShapeList *list = (wxJigsawShapeList*)property->m_pSourceVariable;

    if(list->GetCount() > 0)
    {
        wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
		wxJigsawShapeList::compatibility_iterator listNode = list->GetFirst();
        while(listNode)
        {
			wxJigsawShape * shape = listNode->GetData();
            wxXmlNode * shapeXmlNode = AddPropertyNode(newNode, wxT("object"), 
				xsJigsawShapePropIO::ToString(shape));
			shapeXmlNode->AddProperty(
				new wxXmlProperty(wxT("type"), shape->GetClassInfo()->GetClassName()));
			shape->SerializeObject(shapeXmlNode);
            listNode = listNode->GetNext();
        }

        target->AddChild(newNode);
        AppendPropertyType(property, newNode);
    }
}

wxString xsListJigsawShapePropIO::GetValueStr(xsProperty *property)
{
	return ToString(*((wxJigsawShapeList*)property->m_pSourceVariable));
}

wxString xsListJigsawShapePropIO::ToString(wxJigsawShapeList value)
{
	wxString out = wxT("[ ");

	wxJigsawShapeList::compatibility_iterator node = value.GetFirst();
	while( node )
	{
		out << xsJigsawShapePropIO::ToString(node->GetData());
		if( node != value.GetLast() ) out << wxT(" | ");

		node = node->GetNext();
	}

	out << wxT(" ]");

	return out;
}

wxJigsawShapeList xsListJigsawShapePropIO::FromString(const wxString&)
{
	return wxJigsawShapeList();
}
