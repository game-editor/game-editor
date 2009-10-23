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


#include "wxJigsawDiagram.h"
#include "wxJigsawShapeGroupPropertyIO.h"

IMPLEMENT_DYNAMIC_CLASS(wxJigsawDiagram, xsSerializable)

wxJigsawDiagram::wxJigsawDiagram()
{
	m_Groups.DeleteContents(true);
	InitSerialization();
}

wxJigsawDiagram::wxJigsawDiagram(const wxJigsawDiagram & diagram)
: xsSerializable(diagram)
{
	m_Groups.DeleteContents(true);
	for(wxJigsawShapeGroupList::Node * node = diagram.m_Groups.GetFirst();
		node; node = node->GetNext())
	{
		wxJigsawShapeGroup * group = node->GetData();
		if(!group) continue;
		m_Groups.Append(new wxJigsawShapeGroup(*group));
	}
	InitSerialization();
}

void wxJigsawDiagram::InitSerialization()
{
	XS_SERIALIZE_LISTJIGSAWSHAPEGROUP(m_Groups, wxT("groups"));
}

void wxJigsawDiagram::UpdateLayout(wxDC & dc, double scale)
{
	for(wxJigsawShapeGroupList::Node * node = m_Groups.GetFirst(); node; node = node->GetNext())
	{
		wxJigsawShapeGroup * group = node->GetData();
		if(!group) continue;
		group->Layout(dc, scale);
	}
}

wxJigsawShapeGroup * wxJigsawDiagram::GetShapeGroup(wxJigsawShape * shape)
{
	for(wxJigsawShapeGroupList::Node * node = m_Groups.GetFirst(); node; node = node->GetNext())
	{
		wxJigsawShapeGroup * group = node->GetData();
		if(!group) continue;
		if(group->GetShapes().IndexOf(shape) >= 0)
		{
			return group;
		}
	}
	return NULL;
}

wxSize wxJigsawDiagram::GetSize()
{
	wxSize res(0,0);
	for(wxJigsawShapeGroupList::Node * groupNode = m_Groups.GetFirst();
		groupNode; groupNode = groupNode->GetNext())
	{
		wxJigsawShapeGroup * group = groupNode->GetData();
		if(!group) break;
		for(wxJigsawShapeList::Node * node = group->GetShapes().GetFirst(); node; node = node->GetNext())
		{
			wxJigsawShape * shape = node->GetData();
			if(!shape) continue;
			wxSize shapeSize = shape->GetSize();
			res.SetWidth(wxMax(res.GetWidth(), shape->GetPosition().x + shapeSize.GetWidth()));
			res.SetHeight(wxMax(res.GetHeight(), shape->GetPosition().y + shapeSize.GetHeight()));
		}
	}
	return res;
}

xsSerializable * wxJigsawDiagram::Clone()
{
	return new wxJigsawDiagram(*this);
}

wxJigsawShapeGroupList & wxJigsawDiagram::GetGroups()
{
	return m_Groups;
}

void wxJigsawDiagram::UpdateParents()
{
	for(wxJigsawShapeGroupList::Node * node = m_Groups.GetFirst(); node; node = node->GetNext())
	{
		wxJigsawShapeGroup * group = node->GetData();
		if(!group) continue;
		group->UpdateParents();
	}
}
