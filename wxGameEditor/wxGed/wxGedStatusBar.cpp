#include "wxGedStatusBar.h"
#include <wx/ifm/ifm.h>
#include "wx/wx.h"


BEGIN_EVENT_TABLE(wxGedStatusBar, wxStatusBar)
    EVT_PAINT(wxGedStatusBar::OnPaint)
	EVT_ERASE_BACKGROUND( wxGedStatusBar::OnEraseBackground )
END_EVENT_TABLE()

wxGedStatusBar::wxGedStatusBar(wxWindow *parent,
			   wxWindowID id,
			   long style,
			   const wxString& name)
{
	(void)Create(parent, id, style, name);
}

void wxGedStatusBar::OnEraseBackground( wxEraseEvent& event )
{
	wxDC& dc = *event.GetDC();
	    
    PrepareDC( dc );

    dc.Clear();

    wxRect rect = GetClientRect();

	dc.SetPen(wxBorder_Pen);
    dc.SetBrush(wxBackground_Pen);
    dc.DrawRectangle(rect);
}

void wxGedStatusBar::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxPaintDC dc(this);


   for (int i = 0; i < m_nFields; i ++)
        DrawField(dc, i, i == m_nFields-1);
}

void wxGedStatusBar::DrawField(wxDC& dc, int i, bool bLast)
{
    wxRect rect;
    GetFieldRect(i, rect);

	if(bLast)
	{
		//To avoid a space between last field and end of client area
		wxRect rectClient = GetClientRect();
		rect.width += rectClient.width - (rect.x + rect.width);
	}

    int style = wxSB_NORMAL;

    if (m_statusStyles)
        style = m_statusStyles[i];

    if (style != wxSB_FLAT)
    {
        // Draw border
        // For wxSB_NORMAL:
        // Have grey background, plus 3-d border -
        // One black rectangle.
        // Inside this, left and top sides - dark grey. Bottom and right -
        // white.
        // Reverse it for wxSB_RAISED

        

        // Right and bottom lines
		dc.SetPen(wxBorder_Pen);

		dc.DrawLine(rect.x + rect.width, rect.y,
				rect.x + rect.width, rect.y + rect.height);
	
        dc.DrawLine(rect.x + rect.width, rect.y + rect.height,
                rect.x, rect.y + rect.height);

        
        // Left and top lines
		wxColour topBorder(wxBorder_Pen.Red()/2, wxBorder_Pen.Green()/2, wxBorder_Pen.Blue()/2);
		dc.SetPen(topBorder);

        dc.DrawLine(rect.x, rect.y + rect.height,
               rect.x, rect.y);
        dc.DrawLine(rect.x, rect.y,
            rect.x + rect.width, rect.y);

    }

    DrawFieldText(dc, i);
}

void wxGedStatusBar::DrawFieldText(wxDC& dc, int i)
{
    int leftMargin = 6;

    wxRect rect;
    GetFieldRect(i, rect);

    wxString text(GetStatusText(i));

    long x, y;

    dc.GetTextExtent(text, &x, &y);

    int xpos = rect.x + leftMargin;
    int ypos = (int) (((rect.height - y) / 2 ) + rect.y + 0.5) ;

#if defined( __WXGTK__ ) || defined(__WXMAC__)
    xpos++;
    ypos++;
#endif

	wxIFMCaptionConfig &caption_config = wxIFMDefaultPanelPlugin::GetCaptionConfig();


	dc.SetFont(caption_config.get_font());
	dc.SetTextForeground(caption_config.get_font_colour());

    dc.SetClippingRegion(rect.x, rect.y, rect.width, rect.height);

    dc.DrawText(text, xpos, ypos);

    dc.DestroyClippingRegion();
}
