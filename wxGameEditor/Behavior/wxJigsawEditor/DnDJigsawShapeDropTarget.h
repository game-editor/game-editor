#ifndef _DND_JIGSAW_SHAPE_DROP_TARGET_H
#define _DND_JIGSAW_SHAPE_DROP_TARGET_H

#include <wx/wx.h>
#include <wx/dnd.h>

struct DND_JigsawShapeInfo;
class wxJigsawEditorCanvas;
class wxJigsawShapeListBox;

class DnDJigsawShapeDropTarget : public wxDropTarget
{
	void OnDropShape(const wxPoint & pos, DND_JigsawShapeInfo * shapeinfo);
public:
    DnDJigsawShapeDropTarget(wxJigsawEditorCanvas * pOwner);

    virtual wxDragResult OnData(wxCoord x, wxCoord y, wxDragResult def);

	virtual wxDragResult OnEnter(wxCoord x, wxCoord y, wxDragResult def);
	virtual wxDragResult OnDragOver(wxCoord x, wxCoord y, wxDragResult def);

private:
    wxJigsawEditorCanvas * m_pOwner;
};

class DnDJigsawShapeSource : public wxDropSource 
{

public:

	DnDJigsawShapeSource(wxDataObject& data, wxJigsawShapeListBox *_pSource, wxJigsawEditorCanvas *_pTarget)
	: wxDropSource(data)
	{
		pSource = _pSource;
		pTarget = _pTarget;
	}

	virtual ~DnDJigsawShapeSource()
	{
	}

	
	virtual bool GiveFeedback(wxDragResult effect);

private:
	wxJigsawEditorCanvas *pTarget;
	wxJigsawShapeListBox *pSource;
};

#endif
