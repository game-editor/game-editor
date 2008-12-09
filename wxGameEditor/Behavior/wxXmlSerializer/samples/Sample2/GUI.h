///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 26 2007)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUI__
#define __GUI__

#include <wx/string.h>
#include <wx/button.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/treectrl.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class _MainFrame
///////////////////////////////////////////////////////////////////////////////
class _MainFrame : public wxFrame 
{
	private:
	
	protected:
		enum
		{
			IDC_TREE_ADD = 1000,
			IDC_TREE_REMOVE,
		};
		
		wxButton* btnAddNode;
		wxButton* btnRemoveNode;
		wxButton* btnAbout;
		wxTreeCtrl* treeCtrl;
		wxStdDialogButtonSizer* m_sdbSizer;
		wxButton* m_sdbSizerOK;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnAddClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnUpdateAdd( wxUpdateUIEvent& event ){ event.Skip(); }
		virtual void OnRemoveClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnUpdateRemove( wxUpdateUIEvent& event ){ event.Skip(); }
		virtual void OnAboutClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnOk( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		_MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("TreeSample Application"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
	
};

#endif //__GUI__
