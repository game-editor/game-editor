/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com

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
More info at http://game-editor.com/License
Be a Game Editor developer: http://game-editor.com/Sharing_Software_Revenues_in_Open_Source

***************************************************************************/


#include "wxJigsawInputParameterPropertyIO.h"

#include "wxJigsawShapePropertyIO.h"
#include "wxJigsawInputParameter.h"

/////////////////////////////////////////////////////////////////////////////////////
// xsJigsawInputParameterPropIO class ///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsJigsawInputParameterPropIO, xsPropertyIO);

void xsJigsawInputParameterPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    wxXmlNode *objectNode = source->GetChildren();

    if( objectNode && (objectNode->GetName() == wxT("object")) )
    {
        *(wxJigsawInputParameter**)(property->m_pSourceVariable) = 
			(wxJigsawInputParameter*)
			wxCreateDynamicObject(objectNode->GetPropVal(wxT("type"), wxT("")));

        wxJigsawInputParameter * object = *(wxJigsawInputParameter**)(property->m_pSourceVariable);
        if(object)
        {
            object->DeserializeObject(objectNode);
        }
    }
}

void xsJigsawInputParameterPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    wxJigsawInputParameter * object = *(wxJigsawInputParameter**)(property->m_pSourceVariable);

    if( object && object->IsKindOf(CLASSINFO(wxJigsawInputParameter)))
    {
        wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
        newNode->AddChild(object->SerializeObject(NULL));

        target->AddChild(newNode);
        AppendPropertyType(property, newNode);
    }
}

wxString xsJigsawInputParameterPropIO::GetValueStr(xsProperty *property)
{
    return ToString(*(wxJigsawInputParameter**)(property->m_pSourceVariable));
}

wxString xsJigsawInputParameterPropIO::ToString(wxJigsawInputParameter * value)
{
	return ""; //return wxString::Format(wxT("wxJigsawInputParameter at address 0x%x"), value);
}

wxJigsawInputParameter * xsJigsawInputParameterPropIO::FromString(const wxString&)
{
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsListJigsawShapePropIO class ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsListJigsawInputParameterPropIO, xsPropertyIO);

void xsListJigsawInputParameterPropIO::Read(xsProperty *property, wxXmlNode *source)
{	
	wxJigsawInputParameters *list = (wxJigsawInputParameters*)property->m_pSourceVariable;
	
	// clear previous list content
	bool fDelState = list->GetDeleteContents();

	list->DeleteContents(true);
    list->Clear();
	list->DeleteContents(fDelState);

    wxXmlNode *listNode = source->GetChildren();
    while(listNode)
    {
        if(listNode->GetName() == wxT("object") && 
			listNode->GetPropVal(wxT("type"), wxEmptyString) == wxT("wxJigsawInputParameter"))
        {
			wxJigsawInputParameter * param = new wxJigsawInputParameter;
			param->DeserializeObject(listNode);
			list->Append(param);
        }

        listNode = listNode->GetNext();
    }
}

void xsListJigsawInputParameterPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    wxJigsawInputParameters *list = (wxJigsawInputParameters*)property->m_pSourceVariable;

    if(list->GetCount() > 0)
    {
        wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
		wxJigsawInputParameters::compatibility_iterator listNode = list->GetFirst();
        while(listNode)
        {
			wxJigsawInputParameter * param = listNode->GetData();
            wxXmlNode * paramXmlNode = AddPropertyNode(newNode, wxT("object"), 
				xsJigsawInputParameterPropIO::ToString(param));
			paramXmlNode->AddProperty(
				new wxXmlProperty(wxT("type"), param->GetClassInfo()->GetClassName()));
			param->SerializeObject(paramXmlNode);
            listNode = listNode->GetNext();
        }

        target->AddChild(newNode);
        AppendPropertyType(property, newNode);
    }
}

wxString xsListJigsawInputParameterPropIO::GetValueStr(xsProperty *property)
{
	return ToString(*((wxJigsawInputParameters*)property->m_pSourceVariable));
}

wxString xsListJigsawInputParameterPropIO::ToString(wxJigsawInputParameters value)
{
	wxString out = wxT("[ ");

	wxJigsawInputParameters::compatibility_iterator node = value.GetFirst();
	while( node )
	{
		out << xsJigsawInputParameterPropIO::ToString(node->GetData());
		if( node != value.GetLast() ) out << wxT(" | ");

		node = node->GetNext();
	}

	out << wxT(" ]");

	return out;
}

wxJigsawInputParameters xsListJigsawInputParameterPropIO::FromString(const wxString&)
{
	return wxJigsawInputParameters();
}
