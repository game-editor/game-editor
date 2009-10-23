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


/***************************************************************
 * Name:      PropertyIO.cpp
 * Purpose:   Implements data types I/O and conversion functions
 * Author:    Michal Bližòák (michal.bliznak@tiscali.cz)
 * Created:   2007-10-28
 * Copyright: Michal Bližòák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifdef _DEBUG_MSVC
#define new DEBUG_NEW
#endif

#include "wx/wxxmlserializer/PropertyIO.h"
#include "wx/wxxmlserializer/XmlSerializer.h"

#include <wx/listimpl.cpp>
#include <wx/arrimpl.cpp>

WX_DEFINE_EXPORTED_OBJARRAY(RealPointArray);
WX_DEFINE_EXPORTED_LIST(RealPointList);

/////////////////////////////////////////////////////////////////////////////////////
// xsPropertyIO class ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsPropertyIO, wxObject);

wxXmlNode* xsPropertyIO::AddPropertyNode(wxXmlNode* parent, const wxString& name, const wxString& value)
{
	if(parent)
	{
		wxXmlNode* child = new wxXmlNode(wxXML_ELEMENT_NODE, name);
		child->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxT(""), value));
		parent->AddChild(child);
		return child;
	}
	return NULL;
}

void xsPropertyIO::AppendPropertyType(xsProperty *source, wxXmlNode *target)
{
    target->AddProperty(wxT("name"), source->m_sFieldName);
    target->AddProperty(wxT("type"), source->m_sDataType);
}

/////////////////////////////////////////////////////////////////////////////////////
// xsStringPropIO class /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(wxString, xsStringPropIO);

wxString xsStringPropIO::ToString(wxString value)
{
    return value;
}

wxString xsStringPropIO::FromString(const wxString& value)
{
	return value;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsCharPropIO class ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(wxChar, xsCharPropIO);

wxString xsCharPropIO::ToString(wxChar value)
{
    return wxString::Format(wxT("%c"), value);
}

wxChar xsCharPropIO::FromString(const wxString& value)
{
	return value.GetChar(0);
}


/////////////////////////////////////////////////////////////////////////////////////
// xsLongPropIO class ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(long, xsLongPropIO);

wxString xsLongPropIO::ToString(long value)
{
    return wxString::Format(wxT("%ld"), value);
}

long xsLongPropIO::FromString(const wxString& value)
{
	long num = 0;
	if(!value.IsEmpty())
	{
		value.ToLong(&num);
	}
	return num;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsIntPropIO class ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(int, xsIntPropIO);

wxString xsIntPropIO::ToString(int value)
{
    return wxString::Format(wxT("%d"), value);
}

int xsIntPropIO::FromString(const wxString& value)
{
	long num = 0;
	if(!value.IsEmpty())
	{
		value.ToLong(&num);
	}
	return (int)num;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsBoolPropIO class ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(bool, xsBoolPropIO);

wxString xsBoolPropIO::ToString(bool value)
{
    return wxString::Format(wxT("%d"), value);
}

bool xsBoolPropIO::FromString(const wxString& value)
{
	long num = 0;
	if(!value.IsEmpty())
	{
		value.ToLong(&num);
	}
	return (num == 1);
}

/////////////////////////////////////////////////////////////////////////////////////
// xsDoublePropIO class /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(double, xsDoublePropIO);

wxString xsDoublePropIO::ToString(double value)
{
    return wxString::Format(wxT("%lf"), value);
}

double xsDoublePropIO::FromString(const wxString& value)
{
	double num = 0;
	if(!value.IsEmpty())
	{
		value.ToDouble(&num);
	}
	return num;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsFloatPropIO class //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(float, xsFloatPropIO);

wxString xsFloatPropIO::ToString(float value)
{
    return wxString::Format(wxT("%f"), value);
}

float xsFloatPropIO::FromString(const wxString& value)
{
	double num = 0;
	if(!value.IsEmpty())
	{
		value.ToDouble(&num);
	}
	return (float)num;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsPointPropIO class //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(wxPoint, xsPointPropIO);

wxString xsPointPropIO::ToString(wxPoint value)
{
    return wxString::Format(wxT("%d,%d"), value.x, value.y);
}

wxPoint xsPointPropIO::FromString(const wxString& value)
{
	wxPoint pt;
	long x, y;

	if(!value.IsEmpty())
	{
		wxStringTokenizer tokens(value, wxT(","), wxTOKEN_STRTOK);

		tokens.GetNextToken().ToLong(&x);
		tokens.GetNextToken().ToLong(&y);
		pt.x = x;
		pt.y = y;
	}

	return pt;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsSizePropIO class ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(wxSize, xsSizePropIO);

wxString xsSizePropIO::ToString(wxSize value)
{
    return wxString::Format(wxT("%d,%d"), value.x, value.y);
}

wxSize xsSizePropIO::FromString(const wxString& value)
{
	wxPoint pt = xsPointPropIO::FromString(value);
	return wxSize(pt.x, pt.y);
}

/////////////////////////////////////////////////////////////////////////////////////
// xsRealPointPropIO class //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(wxRealPoint, xsRealPointPropIO);

wxString xsRealPointPropIO::ToString(wxRealPoint value)
{
    return wxString::Format(wxT("%lf,%lf"), value.x, value.y);
}

wxRealPoint xsRealPointPropIO::FromString(const wxString& value)
{
	wxRealPoint pt;

	if(!value.IsEmpty())
	{
		wxStringTokenizer tokens(value, wxT(","), wxTOKEN_STRTOK);

		tokens.GetNextToken().ToDouble(&pt.x);
		tokens.GetNextToken().ToDouble(&pt.y);
	}

	return pt;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsColourPropIO class /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(wxColour, xsColourPropIO);

wxString xsColourPropIO::ToString(wxColour value)
{
    return wxString::Format(wxT("%d,%d,%d"), value.Red(), value.Green(), value.Blue());
}

wxColour xsColourPropIO::FromString(const wxString& value)
{
	long nRed = 0;
	long nGreen = 0;
	long nBlue = 0;

	if(!value.IsEmpty())
	{
		wxStringTokenizer tokens(value, wxT(","), wxTOKEN_STRTOK);

		tokens.GetNextToken().ToLong(&nRed);
		tokens.GetNextToken().ToLong(&nGreen);
		tokens.GetNextToken().ToLong(&nBlue);
	}

	return wxColour(nRed, nGreen, nBlue);
}

/////////////////////////////////////////////////////////////////////////////////////
// xsPenPropIO class ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(wxPen, xsPenPropIO);

wxString xsPenPropIO::ToString(wxPen value)
{
    return wxString::Format(wxT("%s %d %d"), xsColourPropIO::ToString(value.GetColour()).c_str(), value.GetWidth(), value.GetStyle());
}

wxPen xsPenPropIO::FromString(const wxString& value)
{
	wxPen pen;

	wxStringTokenizer tokens(value, wxT(" "), wxTOKEN_STRTOK);
	pen.SetColour(xsColourPropIO::FromString(tokens.GetNextToken()));
	pen.SetWidth(xsLongPropIO::FromString(tokens.GetNextToken()));
	pen.SetStyle(xsLongPropIO::FromString(tokens.GetNextToken()));

	return pen;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsBrushPropIO class //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(wxBrush, xsBrushPropIO);

wxString xsBrushPropIO::ToString(wxBrush value)
{
    return wxString::Format(wxT("%s %d"), xsColourPropIO::ToString(value.GetColour()).c_str(), value.GetStyle());
}

wxBrush xsBrushPropIO::FromString(const wxString& value)
{
	wxBrush brush;

	wxStringTokenizer tokens(value, wxT(" "), wxTOKEN_STRTOK);
	brush.SetColour(xsColourPropIO::FromString(tokens.GetNextToken()));
	brush.SetStyle(xsLongPropIO::FromString(tokens.GetNextToken()));

	return brush;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsFontPropIO class ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(wxFont, xsFontPropIO);

wxString xsFontPropIO::ToString(wxFont value)
{
    return value.GetNativeFontInfoUserDesc();
}

wxFont xsFontPropIO::FromString(const wxString& value)
{
	wxFont font;

	font.SetNativeFontInfoUserDesc(value);

	return font;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsArrayStringPropIO class ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsArrayStringPropIO, xsPropertyIO);

void xsArrayStringPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    ((wxArrayString*)property->m_pSourceVariable)->Clear();

    wxXmlNode *listNode = source->GetChildren();
    while(listNode)
    {
        if(listNode->GetName() == wxT("item"))
        {
            ((wxArrayString*)property->m_pSourceVariable)->Add(listNode->GetNodeContent());
        }

        listNode = listNode->GetNext();
    }
}

void xsArrayStringPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    wxArrayString array(*((wxArrayString*)property->m_pSourceVariable));

    size_t cnt = array.GetCount();
    if(cnt > 0)
    {
        wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
        for(size_t i = 0; i < cnt; i++)
        {
            AddPropertyNode(newNode, wxT("item"), array[i]);
        }

        target->AddChild(newNode);
        AppendPropertyType(property, newNode);
    }
}

wxString xsArrayStringPropIO::GetValueStr(xsProperty *property)
{
	return ToString(*((wxArrayString*)property->m_pSourceVariable));
}

wxString xsArrayStringPropIO::ToString(wxArrayString value)
{
 	wxString out = wxT("[ ");

	for( size_t i = 0; i < value.GetCount(); i++)
	{
		out << value[i];
		if( i < value.GetCount()-1 ) out << wxT(" | ");
	}

	out << wxT(" ]");

	return out;
}

wxArrayString xsArrayStringPropIO::FromString(const wxString& value)
{
	return wxArrayString();
}

/////////////////////////////////////////////////////////////////////////////////////
// xsArrayRealPointPropIO class /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsArrayRealPointPropIO, xsPropertyIO);

void xsArrayRealPointPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    ((RealPointArray*)property->m_pSourceVariable)->Clear();

    wxXmlNode *listNode = source->GetChildren();
    while(listNode)
    {
        if(listNode->GetName() == wxT("item"))
        {
            ((RealPointArray*)property->m_pSourceVariable)->Add(xsRealPointPropIO::FromString(listNode->GetNodeContent()));
        }

        listNode = listNode->GetNext();
    }
}

void xsArrayRealPointPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    RealPointArray array(*((RealPointArray*)property->m_pSourceVariable));

    size_t cnt = array.GetCount();
    if(cnt > 0)
    {
        wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
        for(size_t i = 0; i < cnt; i++)
        {
			AddPropertyNode(newNode, wxT("item"), xsRealPointPropIO::ToString(array[i]));
        }

        target->AddChild(newNode);
        AppendPropertyType(property, newNode);
    }
}

wxString xsArrayRealPointPropIO::GetValueStr(xsProperty *property)
{
	return ToString(*((RealPointArray*)property->m_pSourceVariable));
}

wxString xsArrayRealPointPropIO::ToString(RealPointArray value)
{
	wxString out = wxT("[ ");

	for( size_t i = 0; i < value.GetCount(); i++)
	{
		out << xsRealPointPropIO::ToString(value[i]);
		if( i < value.GetCount()-1 ) out << wxT(" | ");
	}

	out << wxT(" ]");

	return out;
}

RealPointArray xsArrayRealPointPropIO::FromString(const wxString& value)
{
	return RealPointArray();
}

/////////////////////////////////////////////////////////////////////////////////////
// xsListRealPointPropIO class //////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsListRealPointPropIO, xsPropertyIO);

void xsListRealPointPropIO::Read(xsProperty *property, wxXmlNode *source)
{	
	RealPointList *list = (RealPointList*)property->m_pSourceVariable;
	
	// clear previous list content
	bool fDelState = list->GetDeleteContents();

	list->DeleteContents(true);
    list->Clear();
	list->DeleteContents(fDelState);

    wxXmlNode *listNode = source->GetChildren();
    while(listNode)
    {
        if(listNode->GetName() == wxT("item"))
        {
            list->Append(new wxRealPoint(xsRealPointPropIO::FromString(listNode->GetNodeContent())));
        }

        listNode = listNode->GetNext();
    }
}

void xsListRealPointPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    RealPointList *list = (RealPointList*)property->m_pSourceVariable;

    if(list->GetCount() > 0)
    {
        wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
		RealPointList::compatibility_iterator listNode = list->GetFirst();
        while(listNode)
        {
            AddPropertyNode(newNode, wxT("item"), xsRealPointPropIO::ToString(*(wxRealPoint*)listNode->GetData()));
            listNode = listNode->GetNext();
        }

        target->AddChild(newNode);
        AppendPropertyType(property, newNode);
    }
}

wxString xsListRealPointPropIO::GetValueStr(xsProperty *property)
{
	return ToString(*((RealPointList*)property->m_pSourceVariable));
}

wxString xsListRealPointPropIO::ToString(RealPointList value)
{
	wxString out = wxT("[ ");

	RealPointList::compatibility_iterator node = value.GetFirst();
	while( node )
	{
		out << xsRealPointPropIO::ToString(*(wxRealPoint*)node->GetData());
		if( node != value.GetLast() ) out << wxT(" | ");

		node = node->GetNext();
	}

	out << wxT(" ]");

	return out;
}

RealPointList xsListRealPointPropIO::FromString(const wxString& value)
{
	return RealPointList();
}

/////////////////////////////////////////////////////////////////////////////////////
// xsDynObjPropIO class /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsDynObjPropIO, xsPropertyIO);

void xsDynObjPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    wxXmlNode *objectNode = source->GetChildren();

    if( objectNode && (objectNode->GetName() == wxT("object")) )
    {
        *(xsSerializable**)(property->m_pSourceVariable) = (xsSerializable*)wxCreateDynamicObject(objectNode->GetPropVal(wxT("type"), wxT("")));

        xsSerializable* object = *(xsSerializable**)(property->m_pSourceVariable);
        if(object)
        {
            object->DeserializeObject(objectNode);
        }
    }
}

void xsDynObjPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    xsSerializable* object = *(xsSerializable**)(property->m_pSourceVariable);

    if( object && object->IsKindOf(CLASSINFO(xsSerializable)))
    {
        wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
        newNode->AddChild(object->SerializeObject(NULL));

        target->AddChild(newNode);
        AppendPropertyType(property, newNode);
    }
}

wxString xsDynObjPropIO::GetValueStr(xsProperty *property)
{
    return ToString(*(xsSerializable**)(property->m_pSourceVariable));
}

wxString xsDynObjPropIO::ToString(xsSerializable* value)
{
	return wxString::Format(wxT("Dynamic object at address 0x%x"), value);
}

xsSerializable* xsDynObjPropIO::FromString(const wxString& value)
{
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsDynNCObjPropIO class ///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsDynNCObjPropIO, xsPropertyIO);

void xsDynNCObjPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    wxXmlNode *objectNode = source->GetChildren();

    if( objectNode && (objectNode->GetName() == wxT("object")) )
    {
        xsSerializable* object = *(xsSerializable**)(property->m_pSourceVariable);
        if(object)
        {
            object->DeserializeObject(objectNode);
        }
    }
}

void xsDynNCObjPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    xsSerializable* object = *(xsSerializable**)(property->m_pSourceVariable);

    if( object && object->IsKindOf(CLASSINFO(xsSerializable)))
    {
        wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
        newNode->AddChild(object->SerializeObject(NULL));

        target->AddChild(newNode);
        AppendPropertyType(property, newNode);
    }
}

wxString xsDynNCObjPropIO::GetValueStr(xsProperty *property)
{
    return ToString(*(xsSerializable**)(property->m_pSourceVariable));
}

wxString xsDynNCObjPropIO::ToString(xsSerializable* value)
{
	return wxString::Format(wxT("Dynamic object at address 0x%x"), value);
}

xsSerializable* xsDynNCObjPropIO::FromString(const wxString& value)
{
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsStaticObjPropIO class //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsStaticObjPropIO, xsPropertyIO);

void xsStaticObjPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    wxXmlNode *objectNode = source->GetChildren();

    if( objectNode && (objectNode->GetName() == wxT("object")) )
    {
        (*((xsSerializable*)property->m_pSourceVariable)).DeserializeObject(objectNode);
    }
}

void xsStaticObjPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
    newNode->AddChild((*((xsSerializable*)property->m_pSourceVariable)).SerializeObject(NULL));

    target->AddChild(newNode);
    AppendPropertyType(property, newNode);
}

wxString xsStaticObjPropIO::GetValueStr(xsProperty *property)
{
	return ToString(*(xsSerializable*)property->m_pSourceVariable);
}

wxString xsStaticObjPropIO::ToString(xsSerializable value)
{
	return wxString::Format(wxT("Static object at address 0x%x"), &value);
}

xsSerializable xsStaticObjPropIO::FromString(const wxString& value)
{
	xsSerializable dummy;
	return dummy;
}
