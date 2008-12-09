#ifndef _WX_JIGSAW_SHAPE_DESIGNER_VIEW_H
#define _WX_JIGSAW_SHAPE_DESIGNER_VIEW_H

#include <wx/wx.h>
#include <wx/docview.h>

class wxJigsawShapeDesignerCanvas;

class wxJigsawShapeDesignerView : public wxView
{
	DECLARE_DYNAMIC_CLASS(wxJigsawShapeDesignerView)
	double m_Scale;
	wxJigsawShapeDesignerCanvas * GetCanvas();
public:
	wxJigsawShapeDesignerView();

	virtual void OnDraw(wxDC* dc);
	virtual void OnUpdate(wxView *sender, wxObject *hint = (wxObject *) NULL);
    virtual bool OnClose(bool deleteWindow = true);
};

#endif
