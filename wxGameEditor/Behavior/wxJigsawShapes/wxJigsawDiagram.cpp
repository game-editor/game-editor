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
