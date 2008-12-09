#ifndef _WX_JIGSAW_SHAPE_DESIGNER_DOCUMENT_H
#define _WX_JIGSAW_SHAPE_DESIGNER_DOCUMENT_H

#include <wx/wx.h>
#include <wx/docview.h>
#include <wxJigsawShape.h>

class wxJigsawShapeDesignerDocument : public wxDocument
{
	DECLARE_DYNAMIC_CLASS(wxJigsawShapeDesignerDocument)
	wxJigsawShape * m_Shape;
	wxXmlSerializer m_XmlIO;
public:
	wxJigsawShapeDesignerDocument();
	~wxJigsawShapeDesignerDocument();
	wxInputStream & LoadObject(wxInputStream & stream);
	wxOutputStream & SaveObject(wxOutputStream & stream);

	virtual bool OnSaveDocument(const wxString& filename);
    virtual bool OnOpenDocument(const wxString& filename);

	virtual void OnChangedViewList();

	wxJigsawShape * GetShape();
};

#endif
