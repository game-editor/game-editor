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
