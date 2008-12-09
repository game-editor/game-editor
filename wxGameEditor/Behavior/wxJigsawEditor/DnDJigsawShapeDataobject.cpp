#include "DnDJigsawShapeDataObject.h"
#include <wxJigsawShape.h>
#include "wxJigsawEditorCanvas.h"

wxString DnDJigsawShapeDataObject::FormatID = wxT("DND_JigsawShapeInfo");

DnDJigsawShapeDataObject::DnDJigsawShapeDataObject(DND_JigsawShapeInfo * shapeinfo)
{
	if ( shapeinfo )
	{
		m_shapeInfo = shapeinfo;
	}
	else
	{
		// nothing to copy
		m_shapeInfo = NULL;
	}

	// this string should uniquely identify our format, but is otherwise arbitrary
	m_formatShape.SetId(DnDJigsawShapeDataObject::FormatID.GetData());
}

DnDJigsawShapeDataObject::~DnDJigsawShapeDataObject() 
{ 
	wxDELETE(m_shapeInfo); 
}

DND_JigsawShapeInfo * DnDJigsawShapeDataObject::GetShapeInfo()
{
	DND_JigsawShapeInfo *shapeinfo = m_shapeInfo;
	m_shapeInfo = NULL;
	return shapeinfo;
}

wxSize DnDJigsawShapeDataObject::GetShapeOffset()
{
	do
	{
		if(!m_shapeInfo) break;
		return m_shapeInfo->m_offset;
	}
	while(false);
	return wxSize(0,0);
}

wxDataFormat DnDJigsawShapeDataObject::GetPreferredFormat(Direction WXUNUSED(dir)) const
{
	return m_formatShape;
}

size_t DnDJigsawShapeDataObject::GetFormatCount(Direction dir) const
{
	return 1;
}

void DnDJigsawShapeDataObject::GetAllFormats(wxDataFormat *formats, Direction dir) const
{
	formats[0] = m_formatShape;
}

size_t DnDJigsawShapeDataObject::GetDataSize(const wxDataFormat& format) const
{
	bool isSupported = (format == m_formatShape);
	if ( isSupported )
	{
		return sizeof(DND_JigsawShapeInfo);
	}
	wxASSERT_MSG(isSupported, wxT( "unsupported format"));
	return 0;
}

bool DnDJigsawShapeDataObject::GetDataHere(const wxDataFormat& format, void *pBuf) const
{
	wxCHECK_MSG( format == m_formatShape, false,
		wxT( "unsupported format") );
	memcpy(pBuf, m_shapeInfo, sizeof(DND_JigsawShapeInfo));
	//m_shape->GetDataHere(pBuf);
	//memcpy(pBuf, m_shape, sizeof(wxJigsawShape));
	return true;
}

bool DnDJigsawShapeDataObject::SetData(const wxDataFormat& format,
								 size_t WXUNUSED(len), const void *buf)
{
	wxCHECK_MSG( format == m_formatShape, false,
		wxT( "unsupported format") );
	//wxDELETE(m_shape);
	//m_shape = new wxJigsawShape(*((wxJigsawShape*)buf));
	//m_shape = (wxJigsawShape*)buf;
	memcpy(m_shapeInfo, buf, sizeof(DND_JigsawShapeInfo));
	return true;
}
