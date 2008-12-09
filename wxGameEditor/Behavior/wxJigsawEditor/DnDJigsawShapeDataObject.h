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
