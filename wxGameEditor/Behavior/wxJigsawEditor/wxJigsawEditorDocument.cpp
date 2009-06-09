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
	XS_REGISTER_IO_HANDLER(wxT("emit"), xsCodeEmitter);

	m_XmlIO.SetSerializerOwner(wxT("wxJigsawShapeEngine"));
	m_XmlIO.SetSerializerRootName(wxT("wxJigsawDiagram"));
	m_XmlIO.SetSerializerVersion(wxT("1.0.0"));
	m_XmlIO.SetRootItem(m_Diagram);

	m_DiagramSize = wxSize(0,0);
}

wxJigsawEditorDocument::~wxJigsawEditorDocument()
{
}

wxJigsawDiagram & wxJigsawEditorDocument::GetDiagram()
{
	return *m_Diagram;
}

void wxJigsawEditorDocument::UpdateLayout(wxDC & dc, double scale)
{
	m_Diagram->UpdateLayout(dc, scale);
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
		//return true;
		break;
	}
	while(false);
    return true;
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

wxJigsawShapeGroup * wxJigsawEditorDocument::CreateGroupByShape(wxDC & dc, wxJigsawShape * shape)
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
				group->SetPosition(dc, wxRealPoint(shape->GetPosition().x, shape->GetPosition().y), 1.0);
				// Add shape to the group
				group->GetShapes().Append(shape);
				// Layout the group
				group->Layout(dc, 1.0);
			}
			else
			{
				// If shape belongs to group and it is not a first shape in group then we have 
				// to extract it and all shapes after it
				if(group->GetShapes().IndexOf(shape) > 0)
				{
					group = wxJigsawShapeGroup::CreateFromShapeList(dc, 
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
				group = wxJigsawShapeGroup::CreateFromShapeList(dc, 
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
						group->SetPosition(dc, shape->GetPosition(), 1.0);
						group->GetShapes().Append(shape);
						group->Layout(dc, 1.0);
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

wxJigsawShape * wxJigsawEditorDocument::GetShapeFromPoint(wxDC & dc, const wxPoint & pos,
		wxJigsawShape::wxJigsawShapeHitTestInfo & info,
		wxJigsawShapeGroup * ignoreGroup, double scale)
{
	do
	{
		for(wxJigsawShapeGroupList::Node * groupNode = GetGroups().GetFirst();
			groupNode; groupNode = groupNode->GetNext())
		{
			wxJigsawShapeGroup * group = groupNode->GetData();
			if(!group) break;
			if(group == ignoreGroup) continue;
			wxJigsawShape * shape = group->GetShapeFromPoint(dc, pos, info, scale);
			if(shape)
			{
				return info.GetShape();
			}
		}
	}
	while(false);

	info.Clear();
	return NULL;
}

bool wxJigsawEditorDocument::ProcessDrop(wxDC & dc, const wxPoint & pos, 
		wxJigsawShapeGroup * group, const wxSize & hotSpotOffset, double scale)
{
	do
	{
		wxJigsawEditorView * view = wxDynamicCast(GetFirstView(), wxJigsawEditorView);
		if(!view) break;
		if(!group) break;
		if(GetGroups().IndexOf(group) < 0) break;
		wxJigsawShape::wxJigsawShapeHitTestInfo info;
		wxJigsawShape * shape = GetShapeFromPoint(dc, pos, info, group, scale);
		if(!shape) break;
		bool isSingleShape = (group->GetShapes().GetCount() == 1);
		bool isParamShape = false;
		if(isSingleShape)
		{
			wxJigsawShapeList::Node * node = group->GetShapes().GetFirst();
			if(node)
			{
				wxJigsawShape * firstShape = node->GetData();
				if(firstShape)
				{
					isParamShape = (firstShape->GetStyle() != wxJigsawShapeStyle::wxJS_TYPE_DEFAULT);
				}
			}
		}
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
			if(isParamShape) break;
			AppendChildren(info.GetShape(), group, 0);
		}
		else if(info.GetResult() == wxJigsawShape::wxJS_HITTEST_C_SHAPE_NOTCH)
		{
			if(isParamShape) break;
			AppendChildren(info.GetShape(), group, info.GetShape()->GetChildren().GetCount());
		}
		else if(info.GetResult() == wxJigsawShape::wxJS_HITTEST_CHILD_INSERTION_AREA)
		{
			if(isParamShape) break;
			AppendChildren(info.GetShape(), group, info.GetChildIndex());
		}
		else if(info.GetResult() == wxJigsawShape::wxJS_HITTEST_BUMP_DOCKING_AREA)
		{
			if(isParamShape) break;
			wxJigsawShapeGroup * targetGroup = GetShapeGroup(info.GetShape());
			if(!targetGroup) break;
			if(!InsertGroup(targetGroup, group, 
				targetGroup->GetShapes().IndexOf(info.GetShape())+1))
			{
				break;
			}
			//group->GetShapes().Insert(group->GetShapes().IndexOf(info.GetShape()), info.GetShape())
			GetGroups().DeleteObject(group);
		}
		else if(info.GetResult() == wxJigsawShape::wxJS_HITTEST_NOTCH_DOCKING_AREA)
		{
			if(isParamShape) break;
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
		UpdateLayout(dc, view->GetScale());
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
		if(!target || !source || ((insertIndex < 0) || (insertIndex > target->GetShapes().GetCount()))) break;
		wxJigsawShape * prevShape(NULL);
		wxJigsawShape * nextShape(NULL);
		wxJigsawShape * sourceFirstShape(NULL);
		wxJigsawShape * sourceLastShape(NULL);
		wxJigsawShapeList::Node * node(NULL);

		// Check if group is insertable
		node = source->GetShapes().GetFirst();
		sourceFirstShape = node->GetData();
		if(insertIndex > 0)
		{
			node = target->GetShapes().Item(insertIndex-1);
			if(!node) break;
			prevShape = node->GetData();
		}

		node = source->GetShapes().GetLast();
		sourceLastShape = node->GetData();
		if(insertIndex < (target->GetShapes().GetCount()-1))
		{
			node = target->GetShapes().Item(insertIndex);
			nextShape = node->GetData();
		}

		// If "previous" shape exists but we can't insert the shape then exit
		if(prevShape && (!prevShape->GetHasBump()  || !sourceFirstShape->GetHasNotch())) break;
		// If "next" shape exists but we can't insert the shape then exit
		if(nextShape && (!nextShape->GetHasNotch() || !sourceLastShape->GetHasBump())) break;

		for(node = source->GetShapes().GetLast(); node; node = node->GetPrevious())
		{
			wxJigsawShape * shape = node->GetData();
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
		//UpdateLayout(view->GetScale());
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

void wxJigsawEditorDocument::ReCreateHotSpots(wxDC & dc, wxJigsawHotSpotArray & hotSpots, 
		wxJigsawShapeGroup * groupToSkip, double scale)
{
	hotSpots.Clear();
	for(wxJigsawShapeGroupList::Node * node = GetGroups().GetFirst(); node; node = node->GetNext())
	{
		wxJigsawShapeGroup * group = node->GetData();
		if(!group || (group == groupToSkip)) continue;
		group->ReCreateHotSpots(dc, hotSpots, scale);
	}
	wxLogTrace(wxTraceMask(), 
		_("wxJigsawEditorDocument::ReCreateHotSpots; Count =  %i"), 
		hotSpots.GetCount());
}
