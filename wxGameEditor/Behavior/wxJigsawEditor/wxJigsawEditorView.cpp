#include "wxJigsawEditorView.h"
#include "wxJigsawEditorMainFrame.h"
#include "wxJigsawEditorApp.h"
#include "wxJigsawEditorCanvas.h"
#include "wxJigsawEditorDocument.h"
#include <wxJigsawShape.h>

IMPLEMENT_DYNAMIC_CLASS(wxJigsawEditorView, wxView)

wxJigsawEditorView::wxJigsawEditorView()
: m_Scale(1.0), m_ViewOffset(wxSize(0,0)), m_SelectedObject(NULL)
{
	SetFrame(wxTheApp->GetTopWindow());

	wxJigsawEditorCanvas * canvas = GetCanvas();
	if(canvas)
	{
		canvas->SetView(this);
	}

	Activate(true);
}

void wxJigsawEditorView::OnDraw(wxDC* dc)
{
	do
	{
		wxJigsawEditorDocument * document = 
			wxDynamicCast(GetDocument(), wxJigsawEditorDocument);
		if(!document) break;
		for(wxJigsawShapeGroupList::Node * node = document->GetGroups().GetLast(); 
			node; node = node->GetPrevious())
		{
			wxJigsawShapeGroup * group = node->GetData();
			/*if(group != m_SelectedObject)
			{*/
				group->Draw(*dc, m_ViewOffset, m_Scale);
			/*}*/
		}
		wxJigsawEditorCanvas * canvas = GetCanvas();
		if(canvas)
		{
			canvas->DrawHotSpot(dc, m_Scale);
		}
		/*if(m_SelectedObject)
		{
			m_SelectedObject->Draw(*dc, m_ViewOffset, m_Scale);
		}*/
	}
	while(false);
}

void wxJigsawEditorView::OnUpdate(wxView *sender, wxObject *hint)
{
	do
	{
		wxJigsawEditorCanvas * canvas = GetCanvas();
		if(!canvas) break;
		canvas->AdjustScrollBars();
        canvas->RefreshBuffer();
		wxJigsawEditorDocument * document = wxDynamicCast(GetDocument(), wxJigsawEditorDocument);
		if(!document) break;
		document->UpdateLayout(canvas->GetDoubleBufferDC(), m_Scale);
		canvas->RefreshBuffer();
	}
	while(false);
}

wxJigsawEditorCanvas * wxJigsawEditorView::GetCanvas()
{
	//wxJigsawEditorMainFrame * frame = wxDynamicCast(GetFrame(), wxJigsawEditorMainFrame);
	wxJigsawEditorMainFrame * frame = wxJigsawEditorMainFrame::Get();
	if(frame)
	{
		return frame->m_Canvas;
	}
	return NULL;
}

bool wxJigsawEditorView::OnClose(bool deleteWindow)
{
	if (!GetDocument()->Close())
	{
        return false;
	}
    
	wxJigsawEditorCanvas * canvas = GetCanvas();
		
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
		frame->SetTitle(SYMBOL_WXJIGSAWEDITORMAINFRAME_TITLE);
	}
    
    SetFrame(NULL);
    Activate(false);
    return true;
}

void wxJigsawEditorView::SetViewOffset(const wxSize & value)
{
	m_ViewOffset = value;
}

const wxSize & wxJigsawEditorView::GetViewOffset()
{
	return m_ViewOffset;
}

wxJigsawShape * wxJigsawEditorView::GetShapeFromPoint(wxDC & dc, const wxPoint & pos,
		wxJigsawShapeGroup * ignoreGroup)
{
	do
	{
		wxJigsawEditorDocument * document = 
			wxDynamicCast(GetDocument(), wxJigsawEditorDocument);
		if(!document) break;
		wxJigsawShape::wxJigsawShapeHitTestInfo info;
		return document->GetShapeFromPoint(dc, pos, info, ignoreGroup, m_Scale);
	}
	while(false);
	return NULL;
}

wxSize wxJigsawEditorView::GetDiagramSize()
{
	do
	{
		wxJigsawEditorDocument * document = 
			wxDynamicCast(GetDocument(), wxJigsawEditorDocument);
		if(!document) break;
		return document->GetDiagramSize();
	}
	while(false);
	return wxSize(0,0);
}

wxJigsawShapeGroup * wxJigsawEditorView::GetSelectedObject()
{
	return m_SelectedObject;
}

void wxJigsawEditorView::SetSelectedObject(wxJigsawShapeGroup * value)
{
	m_SelectedObject = value;
}

double wxJigsawEditorView::GetScale() const
{
	return m_Scale;
}

void wxJigsawEditorView::SetScale(double value)
{
	m_Scale = wxMin(5, wxMax(0.1, value));
	do
	{
		wxJigsawEditorDocument * document = 
			wxDynamicCast(GetDocument(), wxJigsawEditorDocument);
		if(!document) break;
		document->RequestSizeRecalculation();
		wxJigsawEditorCanvas * canvas = GetCanvas();
		if(!canvas) break;
		canvas->RefreshBuffer();
		document->UpdateLayout(canvas->GetDoubleBufferDC(), m_Scale);
	}

	while(false);
}

wxPoint wxJigsawEditorView::GetRealGroupPosition(wxJigsawShapeGroup * group)
{
	do
	{
		if(!group) break;
		return wxPoint(
			(group->GetPosition().x - m_ViewOffset.GetWidth())*m_Scale+m_ViewOffset.GetWidth(),
			(group->GetPosition().y - m_ViewOffset.GetHeight())*m_Scale+m_ViewOffset.GetHeight());
	}
	while(false);
	return wxPoint(0,0);
}

void wxJigsawEditorView::BringToFront(wxJigsawShapeGroup * group)
{
	do
	{
		wxJigsawEditorDocument * document = 
			wxDynamicCast(GetDocument(), wxJigsawEditorDocument);
		if(!document) break;
		int index = document->GetGroups().IndexOf(group);
		if(index < 0) break;
		bool oldDeleteContents = document->GetGroups().GetDeleteContents();
		document->GetGroups().DeleteContents(false);
		document->GetGroups().DeleteObject(group);
		document->GetGroups().Insert((size_t)0, group);
		document->GetGroups().DeleteContents(oldDeleteContents);
	}
	while(false);
}
