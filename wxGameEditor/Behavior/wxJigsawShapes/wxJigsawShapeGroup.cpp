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


#include "wxJigsawShapeGroup.h"
#include "wxJigsawShapePropertyIO.h"
#include <wx/listimpl.cpp>

IMPLEMENT_DYNAMIC_CLASS(wxJigsawShapeGroup, wxObject)

WX_DEFINE_LIST(wxJigsawShapeGroupList)

wxJigsawShapeGroup::wxJigsawShapeGroup()
: m_Position(0,0)
{
	m_Shapes.DeleteContents(true);
	InitSerialization();
}

wxJigsawShapeGroup::wxJigsawShapeGroup(const wxJigsawShapeGroup & group)
: xsSerializable(group), m_Position(group.m_Position)
{
	for(wxJigsawShapeList::Node * node = group.m_Shapes.GetFirst();
		node; node = node->GetNext())
	{
		m_Shapes.Append(node->GetData());
	}
	InitSerialization();
}

xsSerializable * wxJigsawShapeGroup::Clone()
{
	return new wxJigsawShapeGroup(*this);
}

void wxJigsawShapeGroup::InitSerialization()
{
	XS_SERIALIZE(m_Position, wxT("position"));
	XS_SERIALIZE_LISTJIGSAWSHAPE(m_Shapes, wxT("shapes"));
}

wxJigsawShapeList & wxJigsawShapeGroup::GetShapes()
{
	return m_Shapes;
}

void wxJigsawShapeGroup::SetPosition(wxDC & dc, const wxRealPoint & value, double scale)
{
	m_Position = value;
	Layout(dc, scale);
}

void wxJigsawShapeGroup::SetPosition(wxDC & dc, const wxPoint & value, double scale)
{
	m_Position = wxRealPoint(value.x, value.y);
	Layout(dc, scale);
}

const wxRealPoint & wxJigsawShapeGroup::GetPosition()
{
	return m_Position;
}

void wxJigsawShapeGroup::Layout(wxDC & dc, double scale)
{
	wxRealPoint shapePosition = m_Position;
	shapePosition.x *= scale;
	shapePosition.y *= scale;
	wxSize shapeSize(0,0);
	for(wxJigsawShapeList::Node * node = m_Shapes.GetFirst();
		node; node = node->GetNext())
	{
		wxJigsawShape * shape = node->GetData();
		if(!shape) continue;
		wxPoint newPosition(shapePosition.x, shapePosition.y);
		wxLogTrace(wxTraceMask(), _("Shape position = (%i,%i)"),
			newPosition.x, newPosition.y);
		shape->SetPosition(newPosition);
		shape->Layout(dc, scale);
		shapeSize = shape->GetSize();
		shapePosition.y += shapeSize.GetHeight();
	}
}

bool wxJigsawShapeGroup::Drag(wxDC & dc, const wxPoint & destPoint, const wxSize & offset, double scale)
{
	wxLogTrace(wxTraceMask(), _("wxJigsawShapeGroup::Drag; OldPos=(%i,%i); Offset=(%i,%i); Cursor=(%i,%i); NewPos=(%i,%i)"),
		static_cast<int>(m_Position.x), static_cast<int>(m_Position.y), 
		offset.x, offset.y, 
		destPoint.x, destPoint.y,
		destPoint.x - offset.GetWidth(), destPoint.y - offset.GetHeight());
	m_Position = wxRealPoint(destPoint.x - offset.GetWidth(), destPoint.y - offset.GetHeight());
	Layout(dc, scale);
	return true;
}

wxSize wxJigsawShapeGroup::GetSize()
{
	wxSize res(0,0);
	for(wxJigsawShapeList::Node * node = m_Shapes.GetFirst(); node; node = node->GetNext())
	{
		wxJigsawShape * shape = node->GetData();
		if(!shape) continue;
		wxSize shapeSize = shape->GetSize();
		res.SetWidth(wxMax(shapeSize.GetWidth(), res.GetWidth()));
		res.SetHeight(res.GetHeight() + shapeSize.GetHeight());
	}
	return res;
}

wxRect wxJigsawShapeGroup::GetRect()
{
	return wxRect(wxPoint(m_Position.x, m_Position.y), GetSize());
}

wxJigsawShapeGroup * wxJigsawShapeGroup::CreateFromShapeList(wxDC & dc, wxJigsawShapeList & shapes, 
	int startIndex, double scale)
{
	do
	{
		// Retrieve list item in specified position
		wxJigsawShapeList::Node * shapeNode = shapes.Item(startIndex);
		if(!shapeNode) break;
		// Retrieve the shape in specified position
		wxJigsawShape * shape = shapeNode->GetData();
		if(!shape) break;
		// Create new group
		wxJigsawShapeGroup * group = new wxJigsawShapeGroup();
		// Set the position of a group
		group->SetPosition(dc, wxRealPoint(shape->GetPosition().x, shape->GetPosition().y), scale);
		// Add a shape and all shapes after it to the group
		for(wxJigsawShapeList::Node * node = shapeNode; node; node = node->GetNext())
		{
			wxJigsawShape * child = node->GetData();
			group->GetShapes().Append(child);
		}
		// Layout the shapes inside a group
		group->Layout(dc, scale);
		// Delete the shapes from parent
		bool prevDeleteContents = shapes.GetDeleteContents();
		shapes.DeleteContents(false);
		for(wxJigsawShapeList::Node * node = group->GetShapes().GetFirst(); 
			node; node = node->GetNext())
		{
			wxJigsawShape * child = node->GetData();
			shapes.DeleteObject(child);
		}
		shapes.DeleteContents(prevDeleteContents);
		return group;
	}
	while(false);
	return NULL;
}

void wxJigsawShapeGroup::Draw(wxDC & dc, const wxSize & offset, double scale)
{
	for(wxJigsawShapeList::Node * node = m_Shapes.GetFirst(); node; node = node->GetNext())
	{
		wxJigsawShape * shape = node->GetData();
		shape->Draw(dc, offset, scale);
	}
}

wxJigsawShape * wxJigsawShapeGroup::GetShapeFromPoint(wxDC & dc, const wxPoint & pos,
		wxJigsawShape::wxJigsawShapeHitTestInfo & info, double scale)
{
	for(wxJigsawShapeList::Node * node = m_Shapes.GetLast(); node; node = node->GetPrevious())
	{
		wxJigsawShape * shape = node->GetData();
		if(!shape) continue;
		wxJigsawShape::wxJigsawShapeHitTestInfo tempInfo;
		wxJigsawShape::wxJigsawShapeHitTest hittest = shape->HitTest(dc, pos, tempInfo, scale);
		if(hittest != wxJigsawShape::wxJS_HITTEST_NONE)
		{
			info = tempInfo;
			return shape;
		}
		else
		{
			for(wxJigsawShapeList::Node * childNode = shape->GetChildren().GetFirst();
				childNode; childNode = childNode->GetNext())
			{
				wxJigsawShape * child = childNode->GetData();
				if(!child) continue;
				hittest = child->HitTest(dc, pos, tempInfo, scale);
				if(hittest != wxJigsawShape::wxJS_HITTEST_NONE)
				{
					info = tempInfo;
					return child;
				}
			}
		}
	}
	return NULL;
}

void wxJigsawShapeGroup::Detach(wxJigsawShape * shape)
{
	bool oldDeleteContents = m_Shapes.GetDeleteContents();
	m_Shapes.DeleteContents(false);
	m_Shapes.DeleteObject(shape);
	m_Shapes.DeleteContents(oldDeleteContents);
}

void wxJigsawShapeGroup::RequestSizeRecalculation()
{
	for(wxJigsawShapeList::Node * node = m_Shapes.GetFirst(); node; node = node->GetNext())
	{
		wxJigsawShape * shape = node->GetData();
		if(!shape) continue;
		shape->RequestSizeRecalculation();
	}
}

void wxJigsawShapeGroup::ReCreateHotSpots(wxDC & dc, wxJigsawHotSpotArray & hotSpots, double scale)
{
	for(wxJigsawShapeList::Node * node = m_Shapes.GetFirst(); node; node = node->GetNext())
	{
		wxJigsawShape * shape = node->GetData();
		if(!shape) continue;
		shape->ReCreateHotSpots(dc, hotSpots, scale);
	}
}

void wxJigsawShapeGroup::UpdateParents()
{
	for(wxJigsawShapeList::Node * node = m_Shapes.GetFirst(); node; node = node->GetNext())
	{
		wxJigsawShape * shape = node->GetData();
		if(!shape) continue;
		shape->UpdateParents(NULL);
	}
}

bool wxJigsawShapeGroup::IsSingleShapeGroup()
{
	return (m_Shapes.GetCount() == 1);
}

bool wxJigsawShapeGroup::IsInputParameterShapeGroup()
{
	do 
	{
		if(!IsSingleShapeGroup()) break;
		wxJigsawShape * shape = GetFirstShape();
		if(!shape) break;
		return shape->GetStyle() != (wxJigsawShapeStyle::wxJS_TYPE_NONE);
	} 
	while (false);
	return false;
}

wxJigsawShape * wxJigsawShapeGroup::GetFirstShape()
{
	do 
	{
		if(!IsSingleShapeGroup()) break;
		wxJigsawShapeList::Node * node = m_Shapes.GetFirst();
		if(!node) break;
		wxJigsawShape * shape = node->GetData();
		return shape;
	} 
	while (false);
	return NULL;
}
