#include "wxJigsawShapeDesignerDocument.h"
#include <wxJigsawShapePropertyIO.h>
#include <wxJigsawInputParameterPropertyIO.h>

IMPLEMENT_DYNAMIC_CLASS(wxJigsawShapeDesignerDocument, wxDocument)

wxJigsawShapeDesignerDocument::wxJigsawShapeDesignerDocument()
: m_Shape(new wxJigsawShape(wxT("New Shape")))
{
	XS_REGISTER_IO_HANDLER(wxT("jigsawshape"), xsJigsawShapePropIO);
	XS_REGISTER_IO_HANDLER(wxT("listjigsawshape"), xsListJigsawShapePropIO);
	XS_REGISTER_IO_HANDLER(wxT("jigsawinputparameter"), xsJigsawInputParameterPropIO);
	XS_REGISTER_IO_HANDLER(wxT("listjigsawinputparameter"), xsListJigsawInputParameterPropIO);

	m_XmlIO.SetSerializerOwner(wxT("wxJigsawShapeEngine"));
	m_XmlIO.SetSerializerRootName(wxT("wxJigsawShape"));
	m_XmlIO.SetSerializerVersion(wxT("1.0.0"));
	m_XmlIO.SetRootItem(m_Shape);
}

wxJigsawShapeDesignerDocument::~wxJigsawShapeDesignerDocument()
{
	m_XmlIO.RemoveAll();
	//m_XmlIO.SetRootItem(NULL);
	//wxDELETE(m_Shape);
}

wxInputStream & wxJigsawShapeDesignerDocument::LoadObject(wxInputStream & stream)
{
	
	return stream;
}

wxOutputStream & wxJigsawShapeDesignerDocument::SaveObject(wxOutputStream & stream)
{
	return stream;
}

wxJigsawShape * wxJigsawShapeDesignerDocument::GetShape()
{
	return m_Shape;
}

bool wxJigsawShapeDesignerDocument::OnSaveDocument(const wxString& filename)
{
	wxLogTrace(wxTraceMask(), wxT("wxJigsawShapeDesignerDocument::OnSaveDocument"));
	do
	{
		m_XmlIO.SerializeToXml(filename, xsWITH_ROOT);

		Modify(false);
#ifdef __WXMAC__
		wxFileName fn(filename) ;
		fn.MacSetDefaultTypeAndCreator() ;
#endif
	}
	while(false);
    return true;
}

bool wxJigsawShapeDesignerDocument::OnOpenDocument(const wxString& filename)
{
	wxLogTrace(wxTraceMask(), wxT("wxJigsawShapeDesignerDocument::OnOpenDocument"));
    do
	{
		m_XmlIO.DeserializeFromXml(filename);

		SetFilename(filename, true);
		Modify(false);
		UpdateAllViews();
		//return true;
	}
	while(false);
    return true;
}

void wxJigsawShapeDesignerDocument::OnChangedViewList()
{
	UpdateAllViews();
}
