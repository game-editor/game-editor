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


#ifndef _DND_JIGSAW_SHAPE_H
#define _DND_JIGSAW_SHAPE_H

#include <wx/wx.h>
#include <wx/dnd.h>

class wxJigsawShape;
class wxJigsawEditorCanvas;

struct DND_JigsawShapeInfo
{
	wxJigsawShape * m_shape;
	wxSize m_offset;
};

// ----------------------------------------------------------------------------
// A wxDataObject specialisation for the application-specific data
// ----------------------------------------------------------------------------

class DnDJigsawShapeDataObject : public wxDataObject
{
	static wxString FormatID;
public:
    // ctor doesn't copy the pointer, so it shouldn't go away while this object
    // is alive
    DnDJigsawShapeDataObject(DND_JigsawShapeInfo * shapeinfo = NULL);
    virtual ~DnDJigsawShapeDataObject();
	DND_JigsawShapeInfo * GetShapeInfo();
	DND_JigsawShapeInfo * ReadShapeInfo() {return m_shapeInfo;}
	wxSize GetShapeOffset();

    // implement base class pure virtuals
    // ----------------------------------

    virtual wxDataFormat GetPreferredFormat(Direction WXUNUSED(dir)) const;
    virtual size_t GetFormatCount(Direction dir) const;
    virtual void GetAllFormats(wxDataFormat *formats, Direction dir) const;
    virtual size_t GetDataSize(const wxDataFormat& format) const;
    virtual bool GetDataHere(const wxDataFormat& format, void *pBuf) const;
    virtual bool SetData(const wxDataFormat& format, size_t WXUNUSED(len), const void *buf);

private:
    wxDataFormat		m_formatShape;  // our custom format
    DND_JigsawShapeInfo	* m_shapeInfo;  // our data
};

#endif
