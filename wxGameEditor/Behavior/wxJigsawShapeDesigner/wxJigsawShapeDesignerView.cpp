#include "wxJigsawShapeDesignerView.h"
#include "wxJigsawShapeDesignerMainFrame.h"
#include "wxJigsawShapeDesignerCanvas.h"

IMPLEMENT_DYNAMIC_CLASS(wxJigsawShapeDesignerView, wxView)

wxJigsawShapeDesignerView::wxJigsawShapeDesignerView()
: m_Scale(1.0)
{
	SetFrame(wxTheApp->GetTopWindow());
	wxJigsawShapeDesignerCanvas * canvas = GetCanvas();
	if(canvas)
	{
		canvas->SetView(this);
		canvas->Refresh();
	}
	wxJigsawShapeDesignerMainFrame * mainFrame = wxDynamicCast(GetFrame(), wxJigsawShapeDesignerMainFrame);
	if(mainFrame)
	{
		mainFrame->FillDocumentTree();
	}
}

void wxJigsawShapeDesignerView::OnDraw(wxDC* dc)
{
	wxJigsawShapeDesignerDocument * document =
		wxDynamicCast(GetDocument(), wxJigsawShapeDesignerDocument);
	if(document)
	{
		wxSize size = dc->GetSize();
		wxSize shapeSize = document->GetShape()->GetSize(*dc, m_Scale);
		document->GetShape()->SetPosition(
			wxPoint((size.GetWidth()-shapeSize.GetWidth())/2,
			(size.GetHeight()-shapeSize.GetHeight())/2));
		document->GetShape()->Draw(*dc, wxSize(0,0), m_Scale);
	}
}

void wxJigsawShapeDesignerView::OnUpdate(wxView *, wxObject *)
{
	wxJigsawShapeDesignerCanvas * canvas = GetCanvas();
    if (canvas)
	{
		canvas->AdjustScrollBars();
        canvas->Refresh();
	}
	wxJigsawShapeDesignerMainFrame * mainFrame = wxDynamicCast(GetFrame(), wxJigsawShapeDesignerMainFrame);
	if(mainFrame)
	{
		mainFrame->FillDocumentTree();
	}
}

wxJigsawShapeDesignerCanvas * wxJigsawShapeDesignerView::GetCanvas()
{
	wxJigsawShapeDesignerMainFrame * frame = 
		wxDynamicCast(GetFrame(), wxJigsawShapeDesignerMainFrame);
	if(frame)
	{
		return frame->m_Canvas;
	}
	return NULL;
}

bool wxJigsawShapeDesignerView::OnClose(bool)
{
	if (!GetDocument()->Close())
	{
        return false;
	}
    
	wxJigsawShapeDesignerCanvas * canvas = GetCanvas();
		
    // Clear the canvas in  case we're in single-window mode,
    // and the canvas stays.
	if(canvas)
	{
		canvas->ClearBackground();
		canvas->SetView(NULL);
	}
    
    wxFrame * frame = wxDynamicCast(GetFrame(), wxFrame);
	if(frame)
	{
		frame->SetTitle(SYMBOL_WXJIGSAWSHAPEDESIGNERMAINFRAME_TITLE);
	}
    
    SetFrame(NULL);
    Activate(false);
    return true;
}
