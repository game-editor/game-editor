#ifndef _WX_PROPGRID_ADVANCED_PROPERTIES_H
#define _WX_PROPGRID_ADVANCED_PROPERTIES_H

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/propdev.h>

#ifndef __WXPYTHON__
    // This decl is what you would use in a normal app
    #define wxPG_EXTRAS_DECL
#else
    #define wxPG_EXTRAS_DECL     WXDLLIMPEXP_PG
#endif

class wxPG_EXTRAS_DECL wxSizeProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxSizeProperty)
public:

    wxSizeProperty( const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL,
                    const wxSize& value = wxSize() );
    virtual ~wxSizeProperty();

    WX_PG_DECLARE_PARENTAL_METHODS()

protected:

    // I stands for internal
    void SetValueI( const wxSize& value )
    {
        m_value = wxSizeToVariant(value);
    }
};

#endif
