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
