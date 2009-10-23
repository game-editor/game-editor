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


#ifndef _WX_JIGSAW_EDITOR_VIEW_H
#define _WX_JIGSAW_EDITOR_VIEW_H

#include <wx/wx.h>
#include <wx/docview.h>
#include <wxJigsawShape.h>

//class wxJigsawShape;
class wxJigsawShapeGroup;

class wxJigsawEditorCanvas;

class wxJigsawEditorView : public wxView
{
	DECLARE_DYNAMIC_CLASS(wxJigsawEditorView)
	wxJigsawEditorCanvas * GetCanvas();
	double m_Scale;
	wxSize m_ViewOffset;
	wxJigsawShapeGroup * m_SelectedObject;
public:
	wxJigsawEditorView();

	double GetScale() const;
	void SetScale(double value);

	void SetViewOffset(const wxSize & value);
	const wxSize & GetViewOffset();

	wxSize GetDiagramSize();

	wxJigsawShape * GetShapeFromPoint(wxDC & dc, const wxPoint & pos, wxJigsawShape::wxJigsawShapeHitTestInfo & info,
		wxJigsawShapeGroup * ignoreGroup);

	wxJigsawShapeGroup * GetSelectedObject();
	void SetSelectedObject(wxJigsawShapeGroup * value);

	wxPoint GetRealGroupPosition(wxJigsawShapeGroup * group);

	void BringToFront(wxJigsawShapeGroup * group);

	virtual void OnDraw(wxDC* dc);
	virtual void OnUpdate(wxView *sender, wxObject *hint = (wxObject *) NULL);
    virtual bool OnClose(bool deleteWindow = true);
};

#endif
