#include "wxJigsawEditorDocument.h"
#include "wxJigsawEditorApp.h"
#include <wxJigsawShapePropertyIO.h>
#include <wxJigsawInputParameterPropertyIO.h>
#include <wxJigsawShapeGroupPropertyIO.h>

IMPLEMENT_DYNAMIC_CLASS(wxJigsawEditorDocument, wxDocument)

wxJigsawEditorDocument::wxJigsawEditorDocument()
: m_Diagram(new wxJigsawDiagram)
{
	XS_REGISTER_IO_HANDLER(wxT("jigsawshape"), xsJigsawShapePropIO);
	XS_REGISTER_IO_HANDLER(wxT("listjigsawshape"), xsListJigsawShapePropIO);
	XS_REGISTER_IO_HANDLER(wxT("jigsawinputparameter"), xsJigsawInputParameterPropIO);
	XS_REGISTER_IO_HANDLER(wxT("listjigsawinputparameter"), xsListJigsawInputParameterPropIO);
	XS_REGISTER_IO_HANDLER(wxT("jigsawshapegroup"), xsJigsawShapeGroupPropIO);
	XS_REGISTER_IO_HANDLER(wxT("listjigsawshapegroup"), xsListJigsawShapeGroupPropIO);

	m_XmlIO.SetSerializerOwner(wxT("wxJigsawShapeEngine"));
	m_XmlIO.SetSerializerRootName(wxT("wxJigsawDiagram"));
	m_XmlIO.SetSerializerVersion(wxT("1.0.0"));
	m_XmlIO.SetRootItem(m_Diagram);

	m_DiagramSize = wxSize(0,0);

	CreateDefaultDiagram();
}

wxJigsawEditorDocument::~wxJigsawEditorDocument()
{
	m_XmlIO.RemoveAll();
}

void wxJigsawEditorDocument::CreateDefaultDiagram()
{
	do
	{
		wxJigsawShapeList::Node * node(NULL);
		wxJigsawShapeGroup * group(NULL);
		wxJigsawShape * shape(NULL);
		wxJigsawShape * child(NULL);
		
		if(wxGetApp().GetShapeRegistry()->GetCount() < 3) break;

		// Group A
		group = new wxJigsawShapeGroup;
		group->SetPosition(wxRealPoint(100, 100), 1.0);
		
		// Shape A
		node = wxGetApp().GetShapeRegistry()->Item(2);
		shape = wxDynamicCast(node->GetData()->Clone(), wxJigsawShape);
		if(!shape) break;
		shape->SetPosition(100, 100);

		//// Child A.1
		//node = wxGetApp().GetShapeRegistry()->Item(0);
		//child = wxDynamicCast(node->GetData()->Clone(), wxJigsawShape);
		//if(!child) break;
		//// Add child A.1 to shape A
		//shape->GetChildren().Append(child);
		//child->SetParent(shape);
		
		// Add shape A to gtoup A
		group->GetShapes().Append(shape);

		//// Shape B
		//node = wxGetApp().GetShapeRegistry()->Item(2);
		//shape = wxDynamicCast(node->GetData()->Clone(), wxJigsawShape);
		//if(!shape) break;
		//shape->SetPosition(100, 100);
		//// Add shape B to group A
		//group->GetShapes().Append(shape);

		// Add group A to list
		GetGroups().Append(group);
		
		//// Group B
		//group = new wxJigsawShapeGroup;
		//group->SetPosition(wxRealPoint(300, 150));
		//// Shape C
		//node = wxGetApp().GetShapeRegistry()->Item(2);
		//shape = wxDynamicCast(node->GetData()->Clone(), wxJigsawShape);
		//if(!shape) break;
		//shape->SetPosition(100, 100);
		//// Add shape C to group B
		//group->GetShapes().Append(shape);

		//// Shape D
		//node = wxGetApp().GetShapeRegistry()->Item(2);
		//shape = wxDynamicCast(node->GetData()->Clone(), wxJigsawShape);
		//if(!shape) break;
		//shape->SetPosition(100, 100);
		//// Add shape D to group B
		//group->GetShapes().Append(shape);
		//// Add group B to list
		//GetGroups().Append(group);
		
		//// Group C
		//group = new wxJigsawShapeGroup;
		//group->SetPosition(wxRealPoint(200, 250));
		//// Shape E
		//node = wxGetApp().GetShapeRegistry()->Item(1);
		//shape = wxDynamicCast(node->GetData()->Clone(), wxJigsawShape);
		//if(!shape) break;
		//shape->SetPosition(100, 100);
		//// Add shape E to group C
		//group->GetShapes().Append(shape);
		//// Add group C to list
		//GetGroups().Append(group);
	}
	while(false);
}

wxJigsawDiagram & wxJigsawEditorDocument::GetDiagram()
{
	return *m_Diagram;
}

void wxJigsawEditorDocument::UpdateLayout(double scale)
{
	m_Diagram->UpdateLayout(scale);
}

wxInputStream & wxJigsawEditorDocument::LoadObject(wxInputStream & stream)
{
	return stream;
}

wxOutputStream & wxJigsawEditorDocument::SaveObject(wxOutputStream & stream)
{
	return stream;
}

bool wxJigsawEditorDocument::OnSaveDocument(const wxString& filename)
{
	wxLogTrace(wxTraceMask(), wxT("wxJigsawEditorDocument::OnSaveDocument"));
	do
	{
		m_XmlIO.SerializeToXml(filename, xsWITH_ROOT);

		Modify(false);
#ifdef __WXMAC__
		wxFileName fn(filename) ;
		fn.MacSetDefaultTypeAndCreator() ;
#endif
	}
	while(false);
    return true;
}

bool wxJigsawEditorDocument::OnOpenDocument(const wxString& filename)
{
	wxLogTrace(wxTraceMask(), wxT("wxJigsawEditorDocument::OnOpenDocument"));
    do
	{
		m_XmlIO.DeserializeFromXml(filename);

		SetFilename(filename, true);
		Modify(false);
		m_Diagram->UpdateParents();
		//UpdateAllViews();
		return true;
	}
	while(false);
    return false;
}

wxJigsawShapeGroupList & wxJigsawEditorDocument::GetGroups()
{
	return m_Diagram->GetGroups();
}

void wxJigsawEditorDocument::OnChangedViewList()
{
	UpdateAllViews();
}

wxJigsawShapeGroup * wxJigsawEditorDocument::GetShapeGroup(wxJigsawShape * shape)
{
	return m_Diagram->GetShapeGroup(shape);
}

wxJigsawShapeGroup * wxJigsawEditorDocument::CreateGroupByShape(wxJigsawShape * shape)
{
	do
	{
		// If NULL was passed then exit
		if(!shape) break;
		// This will be a result
		wxJigsawShapeGroup * group(NULL);
		// Get parent of a shape
		wxJigsawShape * parent = shape->GetParent();
		// If shape does not have a parent
		if(!parent)
		{
			// Find group to which the shape belongs
			group = GetShapeGroup(shape);
			// If shape does not belong to any group then create a group for it
			if(!group)
			{
				// Create a group
				group = new wxJigsawShapeGroup();
				// Set the position of a group
				group->SetPosition(wxRealPoint(shape->GetPosition().x, shape->GetPosition().y), 1.0);
				// Add shape to the group
				group->GetShapes().Append(shape);
				// Layout the group
				group->Layout(1.0);
			}
			else
			{
				// If shape belongs to group and it is not a first shape in group then we have 
				// to extract it and all shapes after it
				if(group->GetShapes().IndexOf(shape) > 0)
				{
					group = wxJigsawShapeGroup::CreateFromShapeList(
						group->GetShapes(),
						group->GetShapes().IndexOf(shape), 1.0);
				}
			}
			// Add group to list
			GetGroups().Append(group);
			RequestSizeRecalculation();
			// Return the group
			return group;
		}
		else
		{
			int indexInChildren = parent->GetChildren().IndexOf(shape);
			// If shape belongs to children
			if(indexInChildren >= 0)
			{
				group = wxJigsawShapeGroup::CreateFromShapeList(
					parent->GetChildren(),
					parent->GetChildren().IndexOf(shape),
					1.0);
				for(wxJigsawShapeList::Node * node = group->GetShapes().GetFirst(); node; node = node->GetNext())
				{
					wxJigsawShape * groupShape = node->GetData();
					groupShape->SetParent(NULL);
				}
			}
			else // If shape belongs to input parameters
			{
				for(wxJigsawInputParameters::Node * node = parent->GetInputParameters().GetFirst();
					node; node = node->GetNext())
				{
					wxJigsawInputParameter * param = node->GetData();
					if(!param) continue;
					if(param->GetShape() == shape)
					{
						param->SetShape(NULL);
						shape->SetParent(NULL);
						group = new wxJigsawShapeGroup;
						group->SetPosition(shape->GetPosition(), 1.0);
						group->GetShapes().Append(shape);
						group->Layout(1.0);
					}
				}
			}
			// Add group to list
			GetGroups().Append(group);
			RequestSizeRecalculation();
			// Return a gruop
			return group;
		}
	}
	while(false);
	return NULL;
}

wxSize wxJigsawEditorDocument::GetDiagramSize()
{
	wxSize diagramSize = m_Diagram->GetSize();
	m_DiagramSize = wxSize(wxMax(m_DiagramSize.GetWidth(), diagramSize.GetWidth()),
		wxMax(m_DiagramSize.GetHeight(), diagramSize.GetHeight()));
	return m_DiagramSize;
	/*return wxSize(2000,2000);*/
}

wxJigsawShape * wxJigsawEditorDocument::GetShapeFromPoint(const wxPoint & pos,
		wxJigsawShape::wxJigsawShapeHitTestInfo & info,
		wxJigsawShapeGroup * ignoreGroup)
{
	do
	{
		for(wxJigsawShapeGroupList::Node * groupNode = GetGroups().GetFirst();
			groupNode; groupNode = groupNode->GetNext())
		{
			wxJigsawShapeGroup * group = groupNode->GetData();
			if(!group) break;
			if(group == ignoreGroup) continue;
			wxJigsawShape * shape = group->GetShapeFromPoint(pos, info);
			if(shape)
			{
				return info.GetShape();
			}
		}
	}
	while(false);
	return NULL;
}

bool wxJigsawEditorDocument::ProcessDrop(const wxPoint & pos, 
		wxJigsawShapeGroup * group, const wxSize & hotSpotOffset)
{
	do
	{
		wxJigsawEditorView * view = wxDynamicCast(GetFirstView(), wxJigsawEditorView);
		if(!view) break;
		if(!group) break;
		if(GetGroups().IndexOf(group) < 0) break;
		wxJigsawShape::wxJigsawShapeHitTestInfo info;
		wxJigsawShape * shape = GetShapeFromPoint(pos, info, group);
		if(!shape) break;
		/// If user dropped the group on the slot
		if(info.GetResult() == wxJigsawShape::wxJS_HITTEST_SLOT)
		{
			if(!info.GetShape()) break;
			// Group should have conly one element to be added to the slot
			if(group->GetShapes().GetCount() != 1) break;
			wxJigsawInputParameters::Node * paramNode = 
				info.GetShape()->GetInputParameters().Item(info.GetInputParameterIndex());
			if(!paramNode) break;
			wxJigsawInputParameter * param = paramNode->GetData();
			if(!param) break;
			wxJigsawShapeList::Node * insertShapeNode = group->GetShapes().Item(0);
			if(!insertShapeNode) break;
			wxJigsawShape * insertShape = insertShapeNode->GetData();
			if(!insertShape) break;
			if((param->GetStyle() != insertShape->GetStyle()) || 
				(insertShape->GetStyle() == wxJigsawShapeStyle::wxJS_TYPE_NONE)) break;
			group->Detach(insertShape);
			insertShape->SetParent(info.GetShape());
			param->SetShape(insertShape);
			GetGroups().DeleteObject(group);
			//info.GetShape()->RequestSizeRecalculation();
			//UpdateLayout(view->GetScale());
			//wxDELETE(group);
		}
		else if(info.GetResult() == wxJigsawShape::wxJS_HITTEST_C_SHAPE_BUMP)
		{
			AppendChildren(info.GetShape(), group, 0);
		}
		else if(info.GetResult() == wxJigsawShape::wxJS_HITTEST_C_SHAPE_NOTCH)
		{
			AppendChildren(info.GetShape(), group, info.GetShape()->GetChildren().GetCount());
		}
		else if(info.GetResult() == wxJigsawShape::wxJS_HITTEST_CHILD_INSERTION_AREA)
		{
			AppendChildren(info.GetShape(), group, info.GetChildIndex());
		}
		else if(info.GetResult() == wxJigsawShape::wxJS_HITTEST_BUMP_DOCKING_AREA)
		{
			wxJigsawShapeGroup * targetGroup = GetShapeGroup(info.GetShape());
			if(!targetGroup) break;
			if(!InsertGroup(targetGroup, group, 
				targetGroup->GetShapes().IndexOf(info.GetShape())+1))
			{
				break;
			}
			//group->GetShapes().Insert(group->GetShapes().IndexOf(info.GetShape()), info.GetShape())
			GetGroups().DeleteObject(group);
			UpdateLayout(view->GetScale());
		}
		else if(info.GetResult() == wxJigsawShape::wxJS_HITTEST_NOTCH_DOCKING_AREA)
		{
			wxJigsawShapeGroup * targetGroup = GetShapeGroup(info.GetShape());
			if(!targetGroup) break;
			if(!InsertGroup(targetGroup, group, 
				targetGroup->GetShapes().IndexOf(info.GetShape())))
			{
				break;
			}
			//group->GetShapes().Insert(group->GetShapes().IndexOf(info.GetShape()), info.GetShape())
			GetGroups().DeleteObject(group);
		}
		UpdateAllViews();
		return true;
	}
	while(false);
	return false;
}

bool wxJigsawEditorDocument::InsertGroup(wxJigsawShapeGroup * target, 
		wxJigsawShapeGroup * source, int insertIndex)
{
	do
	{
		if((insertIndex < 0 ) || !target || !source) break;
		wxJigsawShapeList::Node * node = source->GetShapes().GetFirst();
		wxJigsawShape * shape = node->GetData();
		if(!shape->GetHasNotch()) break;
		node = source->GetShapes().GetLast();
		shape = node->GetData();
		if(!shape->GetHasBump()) break;
		for(node = source->GetShapes().GetLast(); node; node = node->GetPrevious())
		{
			shape = node->GetData();
			if(!shape) continue;
			target->GetShapes().Insert(insertIndex, shape);
		}
		bool oldDeleteContents = source->GetShapes().GetDeleteContents();
		source->GetShapes().DeleteContents(false);
		source->GetShapes().Clear();
		source->GetShapes().DeleteContents(oldDeleteContents);
		return true;
	}
	while(false);
	return false;
}

bool wxJigsawEditorDocument::AppendChildren(wxJigsawShape * dest, 
		wxJigsawShapeGroup * group, size_t beforeIndex)
{
	do
	{
		if(!dest || !group) break;
		if(dest->GetChildren().GetCount() < beforeIndex) break;
		
		wxJigsawShape * shape(NULL);
		wxJigsawShapeList::Node * firstNode = group->GetShapes().GetFirst();
		wxJigsawShapeList::Node * lastNode = group->GetShapes().GetLast();		
		shape = firstNode->GetData();
		if(!shape || !shape->GetHasBump()) break;
		shape = lastNode->GetData();
		if(!shape || !shape->GetHasNotch()) break;

		size_t realIndex(beforeIndex);
		for(wxJigsawShapeList::Node * node = group->GetShapes().GetFirst(); 
			node; node = group->GetShapes().GetFirst(), realIndex++)
		{
			wxJigsawShape * insertShape = node->GetData();
			if(!insertShape) continue;
			group->Detach(insertShape);
			insertShape->SetParent(dest);
			dest->GetChildren().Insert(realIndex, insertShape);
		}
		GetGroups().DeleteObject(group);
		wxJigsawEditorView * view = wxDynamicCast(GetFirstView(), wxJigsawEditorView);
		RequestSizeRecalculation();
		UpdateLayout(view->GetScale());
		return true;
	}
	while(false);
	return false;
}

void wxJigsawEditorDocument::RequestSizeRecalculation()
{
	for(wxJigsawShapeGroupList::Node * node = GetGroups().GetFirst(); node; node = node->GetNext())
	{
		wxJigsawShapeGroup * group = node->GetData();
		if(!group) continue;
		group->RequestSizeRecalculation();
	}
}

void wxJigsawEditorDocument::ReCreateHotSpots(wxJigsawHotSpotArray & hotSpots, 
		wxJigsawShapeGroup * groupToSkip, double scale)
{
	hotSpots.Clear();
	for(wxJigsawShapeGroupList::Node * node = GetGroups().GetFirst(); node; node = node->GetNext())
	{
		wxJigsawShapeGroup * group = node->GetData();
		if(!group || (group == groupToSkip)) continue;
		group->ReCreateHotSpots(hotSpots, scale);
	}
	wxLogTrace(wxTraceMask(), 
		_("wxJigsawEditorDocument::ReCreateHotSpots; Count =  %i"), 
		hotSpots.GetCount());
}
