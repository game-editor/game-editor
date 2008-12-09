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
	void CreateDefaultDiagram();
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

	wxJigsawShape * GetShapeFromPoint(const wxPoint & pos,
		wxJigsawShape::wxJigsawShapeHitTestInfo & info,
		wxJigsawShapeGroup * ignoreGroup);

	bool AppendChildren(wxJigsawShape * dest, 
		wxJigsawShapeGroup * group, size_t beforeIndex);
	bool InsertGroup(wxJigsawShapeGroup * target, wxJigsawShapeGroup * source, int insertIndex);

	bool ProcessDrop(const wxPoint & pos, wxJigsawShapeGroup * group, const wxSize & hotSpotOffset);

	void UpdateLayout(double scale);

	/// Creates a new group of shapes by given shape
	/// If shape does not have any parent then new group will contain only this shape.
	/// If shape has a parent then group will contain this shape and all other children of
	/// shape's parent which come after given shape
	wxJigsawShapeGroup * CreateGroupByShape(wxJigsawShape * shape);

	/// Returns the size of diagram
	wxSize GetDiagramSize();

	void ReCreateHotSpots(wxJigsawHotSpotArray & hotSpots, 
		wxJigsawShapeGroup * groupToSkip, double scale);
	void RequestSizeRecalculation();
};

#endif
