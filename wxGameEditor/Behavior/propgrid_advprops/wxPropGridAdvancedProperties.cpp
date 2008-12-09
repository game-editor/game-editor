#include "wxPropGridAdvancedProperties.h"

// -----------------------------------------------------------------------
// wxSizeProperty
// -----------------------------------------------------------------------

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxSizeProperty,wxPGProperty,
                               wxSize,const wxSize&,TextCtrl)

wxSizeProperty::wxSizeProperty( const wxString& label, const wxString& name,
    const wxSize& value) : wxPGProperty(label,name)
{
    SetValueI(value);
    AddChild( new wxIntProperty(wxT("Width"),wxPG_LABEL,value.x) );
    AddChild( new wxIntProperty(wxT("Height"),wxPG_LABEL,value.y) );
}

wxSizeProperty::~wxSizeProperty() { }

void wxSizeProperty::RefreshChildren()
{
    if ( !GetCount() ) return;
    const wxSize& size = wxSizeFromVariant(m_value);
    Item(0)->SetValue( (long)size.x );
    Item(1)->SetValue( (long)size.y );
}

void wxSizeProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
    wxSize& size = wxSizeFromVariant(thisValue);
    int val = wxPGVariantToInt(childValue);
    switch ( childIndex )
    {
        case 0: size.x = val; break;
        case 1: size.y = val; break;
    }
}
