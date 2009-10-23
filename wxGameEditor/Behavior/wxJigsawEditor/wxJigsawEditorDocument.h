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


#ifndef _WX_JIGSAW_EDITOR_DOCUMENT_H
#define _WX_JIGSAW_EDITOR_DOCUMENT_H

#include <wx/wx.h>
#include <wx/docview.h>
#include <wxJigsawShape.h>
#include <wxJigsawDiagram.h>

class wxJigsawEditorDocument : public wxDocument
{
	DECLARE_DYNAMIC_CLASS(wxJigsawEditorDocument)
	wxJigsawDiagram * m_Diagram;
	wxXmlSerializer m_XmlIO;
	wxSize m_DiagramSize;
public:
	wxJigsawEditorDocument();
	~wxJigsawEditorDocument();

	virtual bool OnSaveDocument(const wxString& filename);
    virtual bool OnOpenDocument(const wxString& filename);

	virtual wxInputStream & LoadObject(wxInputStream & stream);
	virtual wxOutputStream & SaveObject(wxOutputStream & stream);

	virtual void OnChangedViewList();

	wxJigsawDiagram & GetDiagram();

	wxJigsawShapeGroupList & GetGroups();

	wxJigsawShapeGroup * GetShapeGroup(wxJigsawShape * shape);

	wxJigsawShape * GetShapeFromPoint(wxDC & dc, const wxPoint & pos,
		wxJigsawShape::wxJigsawShapeHitTestInfo & info,
		wxJigsawShapeGroup * ignoreGroup, double scale);

	bool AppendChildren(wxJigsawShape * dest, 
		wxJigsawShapeGroup * group, size_t beforeIndex);
	bool InsertGroup(wxJigsawShapeGroup * target, wxJigsawShapeGroup * source, int insertIndex);

	bool ProcessDrop(wxDC & dc, const wxPoint & pos, wxJigsawShapeGroup * group, 
		const wxSize & hotSpotOffset, double scale);

	void UpdateLayout(wxDC & dc, double scale);

	/// Creates a new group of shapes by given shape
	/// If shape does not have any parent then new group will contain only this shape.
	/// If shape has a parent then group will contain this shape and all other children of
	/// shape's parent which come after given shape
	wxJigsawShapeGroup * CreateGroupByShape(wxDC & dc, wxJigsawShape * shape);

	/// Returns the size of diagram
	wxSize GetDiagramSize();

	void ReCreateHotSpots(wxDC & dc, wxJigsawHotSpotArray & hotSpots, 
		wxJigsawShapeGroup * groupToSkip, double scale);
	void RequestSizeRecalculation();
};

#endif
