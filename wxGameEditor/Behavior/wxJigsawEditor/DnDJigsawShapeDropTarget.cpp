#include "DnDJigsawShapeDropTarget.h"
#include <wxJigsawShape.h>
#include "wxJigsawEditorCanvas.h"
#include "DnDJigsawShapeDataObject.h"

DnDJigsawShapeDropTarget::DnDJigsawShapeDropTarget(wxJigsawEditorCanvas * pOwner)
: m_pOwner(pOwner)
{
	SetDataObject(new DnDJigsawShapeDataObject);
}

void DnDJigsawShapeDropTarget::OnDropShape(const wxPoint & pos, DND_JigsawShapeInfo * shapeinfo)
{
	do
	{
		if(!shapeinfo) break;
		if(!shapeinfo->m_shape) break;
		m_pOwner->ProcessDrop(pos, shapeinfo->m_shape, shapeinfo->m_offset);
		/*wxMessageBox(wxString::Format(_("Shape %s was dropped on (%i,%i); Offset: (%i,%i)"),
			shapeinfo->m_shape->GetName(), pos.x, pos.y,
			shapeinfo->m_offset.GetWidth(), shapeinfo->m_offset.GetHeight()));*/
	}
	while(false);
	wxDELETE(shapeinfo);
}

wxDragResult DnDJigsawShapeDropTarget::OnData(wxCoord x, wxCoord y, wxDragResult def)
{
	do
	{
		if (!GetData() || !m_pOwner->GetView()) break;
		OnDropShape(wxPoint(x, y), ((DnDJigsawShapeDataObject *)GetDataObject())->GetShapeInfo());
		return def;
	}
	while(false);
	return wxDragNone;
}

wxDragResult DnDJigsawShapeDropTarget::OnEnter(wxCoord x, wxCoord y, wxDragResult def)
{
	if((m_pOwner->GetView() != NULL) && m_pOwner->GetDragImage())
	{
		m_pOwner->GetDragImage()->Show();
	}
	return OnDragOver(x, y, def);
}

wxDragResult DnDJigsawShapeDropTarget::OnDragOver(wxCoord x, wxCoord y, wxDragResult def)
{
	do
	{
		if(!m_pOwner->GetView()) break;
		if((m_pOwner->GetView() != NULL) && m_pOwner->GetDragImage())
		{
			m_pOwner->GetDragImage()->Move(wxPoint(x, y));
		}
		return wxDropTarget::OnDragOver(x, y, def);
	}
	while(false);
	return wxDragNone;
}