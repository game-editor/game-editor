///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 26 2007)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUI__
#define __GUI__

#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class _MainFrame
///////////////////////////////////////////////////////////////////////////////
class _MainFrame : public wxFrame 
{
	private:
	
	protected:
		wxTextCtrl* m_pTextCtrl;
		wxStdDialogButtonSizer* m_sdbSizer;
		wxButton* m_sdbSizerOK;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnOk( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		_MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("SettingsSample Application"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
	
};

#endif //__GUI__
