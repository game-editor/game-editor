#ifndef _WXGEDSTATUSBAR_
#define _WXGEDSTATUSBAR_

#include "wx/statusbr.h"


class wxGedStatusBar : public wxStatusBar
{
	DECLARE_EVENT_TABLE()

public:

	wxGedStatusBar(wxWindow *parent,
                  wxWindowID id = wxID_ANY,
                  long style = wxST_SIZEGRIP,
                  const wxString& name = wxEmptyString);

	void OnPaint(wxPaintEvent& event);
	void OnEraseBackground( wxEraseEvent& event );

private:

	void DrawField(wxDC& dc, int i, bool bLast);
	void DrawFieldText(wxDC& dc, int i);

    //DECLARE_DYNAMIC_CLASS_NO_COPY(wxGedStatusBar)
};

#endif //_WXGEDSTATUSBAR_