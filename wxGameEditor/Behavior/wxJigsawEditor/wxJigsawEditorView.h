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
