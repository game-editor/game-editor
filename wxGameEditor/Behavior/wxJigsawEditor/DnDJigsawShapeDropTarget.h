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


#ifndef _DND_JIGSAW_SHAPE_DROP_TARGET_H
#define _DND_JIGSAW_SHAPE_DROP_TARGET_H

#include <wx/wx.h>
#include <wx/dnd.h>

struct DND_JigsawShapeInfo;
class wxJigsawEditorCanvas;
class wxJigsawShapeListBox;

class DnDJigsawShapeDropTarget : public wxDropTarget
{
	void OnDropShape(const wxPoint & pos, DND_JigsawShapeInfo * shapeinfo);
public:
    DnDJigsawShapeDropTarget(wxJigsawEditorCanvas * pOwner);

    virtual wxDragResult OnData(wxCoord x, wxCoord y, wxDragResult def);

	virtual wxDragResult OnEnter(wxCoord x, wxCoord y, wxDragResult def);
	virtual wxDragResult OnDragOver(wxCoord x, wxCoord y, wxDragResult def);

private:
    wxJigsawEditorCanvas * m_pOwner;
};

class DnDJigsawShapeSource : public wxDropSource 
{

public:

	DnDJigsawShapeSource(wxDataObject& data, wxJigsawShapeListBox *_pSource, wxJigsawEditorCanvas *_pTarget)
	: wxDropSource(data)
	{
		pSource = _pSource;
		pTarget = _pTarget;
	}

	virtual ~DnDJigsawShapeSource()
	{
	}

	
	virtual bool GiveFeedback(wxDragResult effect);

private:
	wxJigsawEditorCanvas *pTarget;
	wxJigsawShapeListBox *pSource;
};

#endif
