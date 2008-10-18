#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "wxGedControls.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif



#include "wxGedControls.h"
#include <wx/ifm/ifm.h>




wxColour colorCaption;
wxColour colorCaptionText;
wxColour colorBgContent;
wxColour subCategoryColor;

IMPLEMENT_DYNAMIC_CLASS(wxGedButtonXmlHandler, wxXmlResourceHandler)

wxGedButtonXmlHandler::wxGedButtonXmlHandler()
: wxXmlResourceHandler()
{
    XRC_ADD_STYLE(wxBU_LEFT);
    XRC_ADD_STYLE(wxBU_RIGHT);
    XRC_ADD_STYLE(wxBU_TOP);
    XRC_ADD_STYLE(wxBU_BOTTOM);
    XRC_ADD_STYLE(wxBU_EXACTFIT);
    AddWindowStyles();
}

wxObject *wxGedButtonXmlHandler::DoCreateResource()
{
   XRC_MAKE_INSTANCE(button, wxGedButton)

   button->Create(m_parentAsWindow,
                    GetID(),
                    GetText(wxT("label")),
                    GetPosition(), GetSize(),
                    GetStyle(),
                    wxDefaultValidator,
                    GetName());

    if (GetBool(wxT("default"), 0))
        button->SetDefault();
    SetupWindow(button);

    return button;
}

bool wxGedButtonXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxGedButton"));
}



IMPLEMENT_DYNAMIC_CLASS(wxGedStaticTextXmlHandler, wxXmlResourceHandler)

wxGedStaticTextXmlHandler::wxGedStaticTextXmlHandler()
: wxXmlResourceHandler()
{
    XRC_ADD_STYLE(wxST_NO_AUTORESIZE);
    XRC_ADD_STYLE(wxALIGN_LEFT);
    XRC_ADD_STYLE(wxALIGN_RIGHT);
    XRC_ADD_STYLE(wxALIGN_CENTRE);
    AddWindowStyles();
}

wxObject *wxGedStaticTextXmlHandler::DoCreateResource()
{
    XRC_MAKE_INSTANCE(text, wxGedStaticText)

    text->Create(m_parentAsWindow,
                    GetID(),
                    GetText(wxT("label")),
                    GetPosition(), GetSize(),
                    GetStyle(),
                    GetName());

    SetupWindow(text);

    return text;
}

bool wxGedStaticTextXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxGedStaticText"));
}


IMPLEMENT_DYNAMIC_CLASS(wxGedButton, wxButton)

BEGIN_EVENT_TABLE(wxGedButton, wxButton)
  EVT_ERASE_BACKGROUND(wxGedButton::OnEraseBackground)
  EVT_PAINT(wxGedButton::OnPaint)
  EVT_ENTER_WINDOW(wxGedButton::OnEnterWindow)
  EVT_LEAVE_WINDOW(wxGedButton::OnLeaveWindow)
  EVT_LEFT_DOWN       (wxGedButton::OnLeftDown)
  EVT_LEFT_UP         (wxGedButton::OnLeftUp)
END_EVENT_TABLE()


wxGedButton::wxGedButton()
{
	bHover = false;
	bLeftDown = false;
}

wxGedButton::wxGedButton(wxWindow *parent,
						 wxWindowID id,
						 const wxString& label,
						 const wxPoint& pos,
						 const wxSize& size,
						 long style,
						 const wxValidator& validator,
						 const wxString& name)
{
	bHover = false;
	bLeftDown = false;
	Create(parent, id, label, pos, size, style, validator, name);
}

bool wxGedButton::Create(wxWindow *parent,
                wxWindowID id,
                const wxString& label,
                const wxPoint& pos,
                const wxSize& size,
                long style,
                const wxValidator& validator,
                const wxString& name)
{
	bool res = wxButton::Create(parent, id, label, pos, size, style, validator, name);



	return res;
}

void wxGedButton::OnEraseBackground(wxEraseEvent &event) 
{
	/*wxDC& dc = *event.GetDC();
	    
    PrepareDC( dc );

    dc.Clear();

    wxRect rect = GetClientRect();

	dc.SetPen(wxBorder_Pen);
    dc.SetBrush(wxBackground_Pen);
    dc.DrawRectangle(rect);*/

	event.Skip();
}

void wxGedButton::OnEnterWindow(wxMouseEvent &event)
{	
	bHover = true;
	event.Skip();
}

void wxGedButton::OnLeaveWindow(wxMouseEvent &event)
{
	bHover = false;
	event.Skip();
}

void wxGedButton::OnLeftDown(wxMouseEvent &event)
{
	bLeftDown = true;
	event.Skip();
}

void wxGedButton::OnLeftUp(wxMouseEvent &event)
{
	bLeftDown = false;
	event.Skip();
}

void wxGedButton::OnPaint(wxPaintEvent &event)
{
    wxPaintDC dc(this);		
	wxRect rect = GetClientRect();
	wxColour textColor(colorCaptionText), backColor;

	dc.SetPen(wxBackground_Pen);
    dc.SetBrush(wxBackground_Pen);
    dc.DrawRectangle(rect);
	
	
	if(bLeftDown && bHover) 
	{
		backColor = wxTab_active_bg_b;
		textColor = *wxBLACK;
	}
	else if(bHover) backColor = wxBorder_Pen;
	else backColor = wxTab_bg_pen;

	dc.SetPen(*wxBLACK);
	dc.SetBrush(backColor);
	dc.DrawRoundedRectangle(rect, 5);

	

	/*dc.SetPen(*wxTRANSPARENT_PEN);
    for (int y = rect.y; y < rect.height+rect.y; y++)
    {
       unsigned char intens = (unsigned char)(230 + 80 * (rect.y-y) / rect.height);
       dc.SetBrush( wxBrush( wxColour(intens,intens,intens), wxSOLID ) );
       dc.DrawRectangle( rect.x, y, rect.width, 1 );
    }

	dc.SetPen(*wxBLACK);
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.DrawRoundedRectangle(rect, 5);*/

	
	//Draw text
	wxString label(GetLabel());
	int indexAccel = label.Find('&');

	if(indexAccel >= 0)
	{
		label.Remove(indexAccel, 1);
	}

	dc.SetFont(wxIFMDefaultPanelPlugin::GetTabConfig().get_font());
	dc.SetTextForeground(textColor);
	dc.DrawLabel(label, rect, wxALIGN_CENTER, indexAccel);
}

IMPLEMENT_DYNAMIC_CLASS(wxGedStaticText, wxStaticText)

bool wxGedStaticText::Create(wxWindow *parent,
                wxWindowID id,
                const wxString& label,
                const wxPoint& pos,
                const wxSize& size,
                long style,
                const wxString& name)
{

	bool res = wxStaticText::Create(parent, id, label, pos, size, style, name);

	SetForegroundColour(colorCaptionText);

	return res;
}

IMPLEMENT_DYNAMIC_CLASS(wxGedSpin, wxSpinReal)

BEGIN_EVENT_TABLE(wxGedSpin, wxSpinReal)

END_EVENT_TABLE()

bool wxGedSpin::Create(wxWindow *parent,
                wxWindowID id,
                const wxString& value,
                const wxPoint& pos,
                const wxSize& size,
                long style,
                float min, float max,
                float initial,
                float increment, int digits,
                const wxString& name)
{
	bool res = wxSpinReal::Create(parent,
                id,
                value,
                pos,
                size,
                style,
                min,
				max,
                initial,
                increment,
				digits,
                name);

	drag = new wxGedSpinDrag(this);
	return res;
}

wxGedSpin::~wxGedSpin()
{
    
}

IMPLEMENT_CLASS(wxGedSpinDrag, wxWindow)

BEGIN_EVENT_TABLE(wxGedSpinDrag, wxWindow)
    EVT_PAINT(wxGedSpinDrag::OnPaint)
    EVT_ERASE_BACKGROUND(wxGedSpinDrag::OnEraseBackground)
	EVT_LEFT_DOWN       (wxGedSpinDrag::OnLeftDown)
	EVT_LEFT_UP         (wxGedSpinDrag::OnLeftUp)
	EVT_MOTION          (wxGedSpinDrag::OnMouseMove)
END_EVENT_TABLE()


wxGedSpinDrag::wxGedSpinDrag(wxGedSpin *parent, int _id) 
 : wxWindow(parent, _id, wxPoint(parent->GetButtonSize().x/2 - 2, 2), wxSize(4, parent->GetSize().GetHeight() - 7))
{
	SetCursor(wxCursor(wxCURSOR_SIZEWE));
	bButtonDown =false;

	//Get center of screen
	screenXCenter = wxSystemSettings::GetMetric(wxSYS_SCREEN_X)/2;


#ifdef WIN32 //Can enable other platforms when implement parent->GetTextCtrl()
	//Enable spin drag with first click in control
	wxPoint pt(parent->GetTextCtrl()->ScreenToClient(::wxGetMousePosition()));
	wxRect rect;
	rect.SetSize(parent->GetTextCtrl()->GetRect().GetSize());

	if(rect.Inside(pt))
	{		
		//Forward event
		wxPoint ptDrag(ScreenToClient(::wxGetMousePosition()));

		wxMouseEvent event(wxEVT_LEFT_DOWN);
		event.m_leftDown = true;
		event.SetEventObject(this);
		event.SetId(GetId());	
		event.m_x = ptDrag.x;
		event.m_y = ptDrag.y;
		AddPendingEvent(event);
	}
#endif
}

wxGedSpinDrag::~wxGedSpinDrag() 
{

}

void wxGedSpinDrag::OnEraseBackground(wxEraseEvent &event) 
{

}

void wxGedSpinDrag::OnPaint(wxPaintEvent &event) 
{
	//event.Skip();

	wxPaintDC dc(this);		
	wxRect rect = GetClientRect();

	dc.SetPen(*wxBLACK);
	dc.DrawLine(rect.GetWidth()/2, 0, rect.GetWidth()/2, rect.GetHeight());

	
    //dc.SetBrush(wxBackground_Pen);
    //dc.DrawRectangle(rect);
}

void wxGedSpinDrag::OnLeftDown(wxMouseEvent &event)
{
	mouseClickPos = event.GetPosition();
	bButtonDown = true;

	//Move cursor to center of screen to avoid stops when the control is near of borders
	xCenter = screenXCenter;
	ScreenToClient(&xCenter, NULL);
	WarpPointer(xCenter, -2000);

	CaptureMouse();

	//Hide the global cursor
	::wxSetCursor(wxCursor(wxCURSOR_BLANK));
}

void wxGedSpinDrag::OnLeftUp(wxMouseEvent &event)
{
	bButtonDown = false;
	WarpPointer(mouseClickPos.x, mouseClickPos.y);

	if(HasCapture()) ReleaseMouse();
}

void wxGedSpinDrag::OnMouseMove(wxMouseEvent &event)
{
	if(bButtonDown && event.GetPosition().x != xCenter)
	{
		bool bUp = event.GetPosition().x > xCenter;

		wxSpinEvent spinEvent(bUp?wxEVT_SCROLL_LINEUP:wxEVT_SCROLL_LINEDOWN, GetId());
		
		spinEvent.SetPosition(bUp?1:-1);
		spinEvent.SetEventObject(this);

		GetEventHandler()->ProcessEvent(spinEvent);		
		WarpPointer(xCenter, -2000);
	}	
}