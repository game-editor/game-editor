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


#include "DnDJigsawShapeDropTarget.h"
#include <wxJigsawShape.h>
#include "wxJigsawEditorCanvas.h"
#include "wxJigsawShapeListBox.h"
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
	/*if((m_pOwner->GetView() != NULL) && m_pOwner->GetDragImage())
	{
		m_pOwner->GetDragImage()->Show();
	}*/

	return OnDragOver(x, y, def);
}

wxDragResult DnDJigsawShapeDropTarget::OnDragOver(wxCoord x, wxCoord y, wxDragResult def)
{
	do
	{
		if(!m_pOwner->GetView()) break;
		if((m_pOwner->GetView() != NULL) && m_pOwner->GetDragImage())
		{	
			//Update hot spots
			m_pOwner->MotionUpdate(m_pOwner->ScreenToClient(::wxGetMousePosition()));

			//m_pOwner->GetDragImage()->Move(wxPoint(x, y));
		}

		return wxDropTarget::OnDragOver(x, y, def);
	}
	while(false);

	return wxDragNone;
}

bool DnDJigsawShapeSource::GiveFeedback(wxDragResult effect)
{
	if(pTarget->GetDragImage())
	{
		//Must show the drag image in any place of the screen
		wxPoint point(::wxGetMousePosition());
		point -= pSource->ClientToScreen(wxPoint(0,0));

		pTarget->GetDragImage()->Move(point);
	}

	return false;
}