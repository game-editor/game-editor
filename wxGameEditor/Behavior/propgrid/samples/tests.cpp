/////////////////////////////////////////////////////////////////////////////
// Name:        tests.cpp
// Purpose:     wxPropertyGrid tests
// Author:      Jaakko Salli
// Modified by:
// Created:     May-16-2007
// RCS-ID:      $Id:
// Copyright:   (c) Jaakko Salli
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/propdev.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>

#include "propgridsample.h"
#include "sampleprops.h"


// -----------------------------------------------------------------------

void FormMain::OnDumpList( wxCommandEvent& WXUNUSED(event) )
{
    wxVariant values = m_pPropGridMan->GetPropertyValues(wxT("list"), wxNullProperty, wxPG_INC_ATTRIBUTES);
    wxString text = wxT("This only tests that wxVariant related routines do not crash.");
    wxString t;

    wxDialog* dlg = new wxDialog(this,-1,wxT("wxVariant Test"),
        wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);

    wxPGIndex i;
    for ( i = 0; i < (wxPGIndex)values.GetCount(); i++ )
    {
        wxVariant& v = values[i];

        wxString strValue = v.GetString();

#if wxCHECK_VERSION(2,8,0)
        if ( v.GetName().EndsWith(wxT("@attr")) )
#else
        if ( v.GetName().Right(5) == wxT("@attr") )
#endif
        {
            text += wxString::Format(wxT("Attributes:\n"));

            wxPGIndex n;
            for ( n = 0; n < (wxPGIndex)v.GetCount(); n++ )
            {
                wxVariant& a = v[n];

                t.Printf(wxT("  atribute %i: name=\"%s\"  (type=\"%s\"  value=\"%s\")\n"),(int)n,
                    a.GetName().c_str(),a.GetType().c_str(),a.GetString().c_str());
                text += t;
            }
        }
        else
        {
            t.Printf(wxT("%i: name=\"%s\"  type=\"%s\"  value=\"%s\"\n"),(int)i,
                v.GetName().c_str(),v.GetType().c_str(),strValue.c_str());
            text += t;
        }
    }

    // multi-line text editor dialog
    const int spacing = 8;
    wxBoxSizer* topsizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer* rowsizer = new wxBoxSizer( wxHORIZONTAL );
    wxTextCtrl* ed = new wxTextCtrl(dlg,11,text,
        wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_READONLY);
    rowsizer->Add( ed, 1, wxEXPAND|wxALL, spacing );
    topsizer->Add( rowsizer, 1, wxEXPAND, 0 );
    rowsizer = new wxBoxSizer( wxHORIZONTAL );
    const int butSzFlags =
        wxALIGN_CENTRE_HORIZONTAL|wxALIGN_CENTRE_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT;
    rowsizer->Add( new wxButton(dlg,wxID_OK,wxT("Ok")),
        0, butSzFlags, spacing );
    topsizer->Add( rowsizer, 0, wxALIGN_RIGHT|wxALIGN_CENTRE_VERTICAL, 0 );

    dlg->SetSizer( topsizer );
    topsizer->SetSizeHints( dlg );

    dlg->SetSize(400,300);
    dlg->Centre();
    dlg->ShowModal();
}

// -----------------------------------------------------------------------

#if TESTING_WXPROPERTYGRIDADV

class TestRunner
{
public:

    TestRunner( const wxString& name, wxPropertyGridManager* man, wxTextCtrl* ed, wxArrayString* errorMessages )
    {
        m_name = name;
        m_man = man;
        m_ed = ed;
        m_errorMessages = errorMessages;
#ifdef __WXDEBUG__
        m_preWarnings = wxPGGlobalVars->m_warnings;
#endif

        if ( name != wxT("none") )
            Msg(name+wxT("\n"));
    }

    ~TestRunner()
    {
#ifdef __WXDEBUG__
        int warningsOccurred = wxPGGlobalVars->m_warnings - m_preWarnings;
        if ( warningsOccurred )
        {
            wxString s = wxString::Format(wxT("%i warnings occurred during test '%s'"), warningsOccurred, m_name.c_str());
            m_errorMessages->push_back(s);
            Msg(s);
        }
#endif
    }

    void Msg( const wxString& text )
    {
        m_ed->AppendText(text);
        m_ed->AppendText(wxT("\n"));
        wxLogDebug(text);
    }

protected:
    wxPropertyGridManager* m_man;
    wxTextCtrl* m_ed;
    wxArrayString* m_errorMessages;
    wxString m_name;
#ifdef __WXDEBUG__
    int m_preWarnings;
#endif
};

#endif // TESTING_WXPROPERTYGRIDADV


#define RT_START_TEST(TESTNAME) \
    TestRunner tr(wxT(#TESTNAME), pgman, ed, &errorMessages);

#define RT_MSG(S) \
    tr.Msg(S);

#define RT_FAILURE() \
    { \
        wxString s1 = wxString::Format(wxT("Test failure in tests.cpp, line %i."),__LINE__-1); \
        errorMessages.push_back(s1); \
        wxLogDebug(s1); \
        failures++; \
    }

#define RT_FAILURE_MSG(MSG) \
    { \
        wxString s1 = wxString::Format(wxT("Test failure in tests.cpp, line %i."),__LINE__-1); \
        errorMessages.push_back(s1); \
        wxLogDebug(s1); \
        wxString s2 = wxString::Format(wxT("Message: %s"),MSG); \
        errorMessages.push_back(s2); \
        wxLogDebug(s2); \
        failures++; \
    }

void FormMain::RunTests ()
{
#if TESTING_WXPROPERTYGRIDADV
    wxString t;

    wxPropertyGridManager* pgman = m_pPropGridMan;
    wxPropertyGrid* pg;

    size_t i;

    pgman->ClearSelection();

    // Set to false inorder to disable most time-consuming tests
    bool fullTest = true;

    int failures = 0;
    wxArrayString errorMessages;

    wxDialog* dlg = new wxDialog (this,-1,wxT("wxPropertyGrid Regression Tests"),
        wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);

    // multi-line text editor dialog
    const int spacing = 8;
    wxBoxSizer* topsizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer* rowsizer = new wxBoxSizer( wxHORIZONTAL );
    wxTextCtrl* ed = new wxTextCtrl(dlg,11,wxEmptyString,
        wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_READONLY);
    rowsizer->Add ( ed, 1, wxEXPAND|wxALL, spacing );
    topsizer->Add ( rowsizer, 1, wxEXPAND, 0 );
    rowsizer = new wxBoxSizer( wxHORIZONTAL );
    const int butSzFlags =
        wxALIGN_CENTRE_HORIZONTAL|wxALIGN_CENTRE_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT;
    rowsizer->Add ( new wxButton(dlg,wxID_OK,wxT("Ok")),
        0, butSzFlags, spacing );
    topsizer->Add ( rowsizer, 0, wxALIGN_RIGHT|wxALIGN_CENTRE_VERTICAL, 0 );

    dlg->SetSizer ( topsizer );
    topsizer->SetSizeHints( dlg );

    dlg->SetSize (400,300);
    dlg->Move(wxSystemSettings::GetMetric(wxSYS_SCREEN_X)-dlg->GetSize().x,
              wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)-dlg->GetSize().y);
    dlg->Show();

    {
        //
        // Basic iterator tests
        RT_START_TEST(GetIterator)

        wxPGVIterator it;
        int count;

        count = 0;
        for ( it = pgman->GetVIterator(wxPG_ITERATE_PROPERTIES);
              !it.AtEnd();
              it.Next() )
        {
            wxPGProperty* p = it.GetProperty();
            if ( p->IsCategory() )
                RT_FAILURE_MSG(wxString::Format(wxT("'%s' is a category (non-private child property expected)"),p->GetLabel().c_str()).c_str())
            else if ( p->GetParent()->HasFlag(wxPG_PROP_AGGREGATE) )
                RT_FAILURE_MSG(wxString::Format(wxT("'%s' is a private child (non-private child property expected)"),p->GetLabel().c_str()).c_str())
            count++;
        }

        RT_MSG(wxString::Format(wxT("GetVIterator(wxPG_ITERATE_PROPERTIES) -> %i entries"), count));

        count = 0;
        for ( it = pgman->GetVIterator(wxPG_ITERATE_CATEGORIES);
              !it.AtEnd();
              it.Next() )
        {
            wxPGProperty* p = it.GetProperty();
            if ( !p->IsCategory() )
                RT_FAILURE_MSG(wxString::Format(wxT("'%s' is not a category (only category was expected)"),p->GetLabel().c_str()).c_str())
            count++;
        }

        RT_MSG(wxString::Format(wxT("GetVIterator(wxPG_ITERATE_CATEGORIES) -> %i entries"), count));

        count = 0;
        for ( it = pgman->GetVIterator(wxPG_ITERATE_PROPERTIES|wxPG_ITERATE_CATEGORIES);
              !it.AtEnd();
              it.Next() )
        {
            wxPGProperty* p = it.GetProperty();
            if ( p->GetParent()->HasFlag(wxPG_PROP_AGGREGATE) )
                RT_FAILURE_MSG(wxString::Format(wxT("'%s' is a private child (non-private child property or category expected)"),p->GetLabel().c_str()).c_str())
            count++;
        }

        RT_MSG(wxString::Format(wxT("GetVIterator(wxPG_ITERATE_PROPERTIES|wxPG_ITERATE_CATEGORIES) -> %i entries"), count));

        count = 0;
        for ( it = pgman->GetVIterator(wxPG_ITERATE_VISIBLE);
              !it.AtEnd();
              it.Next() )
        {
            wxPGProperty* p = it.GetProperty();
            if ( (p->GetParent() != p->GetParentState()->GetRoot() && !p->GetParent()->IsExpanded()) )
                RT_FAILURE_MSG(wxString::Format(wxT("'%s' had collapsed parent (only visible properties expected)"),p->GetLabel().c_str()).c_str())
            else if ( p->HasFlag(wxPG_PROP_HIDDEN) )
                RT_FAILURE_MSG(wxString::Format(wxT("'%s' was hidden (only visible properties expected)"),p->GetLabel().c_str()).c_str())
            count++;
        }

        RT_MSG(wxString::Format(wxT("GetVIterator(wxPG_ITERATE_VISIBLE) -> %i entries"), count));
    }

    if ( fullTest )
    {
        // Test that setting focus to properties does not crash things
        RT_START_TEST(SelectProperty)

        wxPropertyGridIterator it;
        size_t ind;

        for ( ind=0; ind<pgman->GetPageCount(); ind++ )
        {
            wxPropertyGridPage* page = pgman->GetPage(ind);
            pgman->SelectPage(page);

            for ( it = page->GetIterator(wxPG_ITERATE_VISIBLE);
                  !it.AtEnd();
                  it++ )
            {
                wxPGProperty* p = *it;
                RT_MSG(p->GetLabel());
                pgman->GetGrid()->SelectProperty(p, true);
                ::wxMilliSleep(150);
                Update();
            }
        }
    }

    {
        RT_START_TEST(GetPropertiesWithFlag)

        //
        // Get list of expanded proeperties
        wxArrayPGProperty array = pgman->GetExpandedProperties();

        // Make sure list only has items with children
        for ( i=0; i<array.size(); i++ )
        {
            wxPGProperty* p = array[i];
            if ( !p->IsKindOf(CLASSINFO(wxPGProperty)) )
                RT_FAILURE_MSG(wxString::Format(wxT("'%s' was returned by GetExpandedProperties(), but was not a parent"),p->GetName().c_str()).c_str());
        }

        wxArrayString names;
        pgman->PropertiesToNames( &names, array );

        //
        // ... and then collapse them
        wxArrayPGProperty array2;
        pgman->NamesToProperties( &array2, names );
        pgman->SetExpandedProperties( array2, false );

        // Make sure everything is collapsed
        wxPGVIterator it;

        for ( it = pgman->GetVIterator(wxPG_ITERATE_ALL);
              !it.AtEnd();
              it.Next() )
        {
            wxPGProperty* p = it.GetProperty();
            if ( p->IsExpanded() )
                RT_FAILURE_MSG(wxString::Format(wxT("'%s.%s' was expanded"),p->GetParent()->GetName().c_str(),p->GetName().c_str()).c_str());
        }

        pgman->Refresh();
    }

    {
        //
        // Delete everything in reverse order
        RT_START_TEST(DeleteProperty)

        wxPGVIterator it;
        wxArrayPGProperty array;

        for ( it = pgman->GetVIterator(wxPG_ITERATE_ALL&~(wxPG_IT_CHILDREN(wxPG_PROP_AGGREGATE)));
              !it.AtEnd();
              it.Next() )
            array.push_back(it.GetProperty());

        wxArrayPGProperty::reverse_iterator it2;

        for ( it2 = array.rbegin(); it2 != array.rend(); it2++ )
        {
            wxPGProperty* p = (wxPGProperty*)*it2;
            RT_MSG(wxString::Format(wxT("Deleting '%s' ('%s')"),p->GetLabel().c_str(),p->GetName().c_str()));
            pgman->DeleteProperty(p);
        }

        // Recreate grid
        CreateGrid( -1, -1 );
        pgman = m_pPropGridMan;
    }

    {
        //
        // Clear property value
        RT_START_TEST(ClearPropertyValue)

        wxPGVIterator it;

        for ( it = pgman->GetVIterator(wxPG_ITERATE_PROPERTIES);
              !it.AtEnd();
              it.Next() )
        {
            RT_MSG(wxString::Format(wxT("Clearing value of '%s'"),it.GetProperty()->GetLabel().c_str()));
            pgman->ClearPropertyValue(it.GetProperty());
        }

        // Recreate grid
        CreateGrid( -1, -1 );
        pgman = m_pPropGridMan;
    }

    {
        RT_START_TEST(GetPropertyValues)

        for ( i=0; i<3; i++ )
        {
            wxString text;

            wxPropertyGridPage* page = pgman->GetPage(i);

            wxVariant values = page->GetPropertyValues();

            wxPGIndex i;
            for ( i = 0; i < (wxPGIndex)values.GetCount(); i++ )
            {
                wxVariant& v = values[i];

                t.Printf(wxT("%i: name=\"%s\"  type=\"%s\"\n"),(int)i,
                    v.GetName().c_str(),v.GetType().c_str());

                text += t;
            }
            ed->AppendText(text);
        }
    }

    {
        RT_START_TEST(SetPropertyValue_and_GetPropertyValue)

        //pg = (wxPropertyGrid*) NULL;

        wxArrayString test_arrstr_1;
        test_arrstr_1.Add(wxT("Apple"));
        test_arrstr_1.Add(wxT("Orange"));
        test_arrstr_1.Add(wxT("Lemon"));

        wxArrayString test_arrstr_2;
        test_arrstr_2.Add(wxT("Potato"));
        test_arrstr_2.Add(wxT("Cabbage"));
        test_arrstr_2.Add(wxT("Cucumber"));

        wxArrayInt test_arrint_1;
        test_arrint_1.Add(1);
        test_arrint_1.Add(2);
        test_arrint_1.Add(3);

        wxArrayInt test_arrint_2;
        test_arrint_2.Add(0);
        test_arrint_2.Add(1);
        test_arrint_2.Add(4);

#if wxUSE_DATETIME
        wxDateTime dt1 = wxDateTime::Now();
        dt1.SetYear(dt1.GetYear()-1);

        wxDateTime dt2 = wxDateTime::Now();
        dt2.SetYear(dt2.GetYear()-10);
#endif

#define FLAG_TEST_SET1 (wxCAPTION|wxCLOSE_BOX|wxSYSTEM_MENU|wxRESIZE_BORDER)
#define FLAG_TEST_SET2 (wxSTAY_ON_TOP|wxCAPTION|wxICONIZE|wxSYSTEM_MENU)

        pgman->SetPropertyValue(wxT("StringProperty"),wxT("Text1"));
        pgman->SetPropertyValue(wxT("IntProperty"),1024);
        pgman->SetPropertyValue(wxT("FloatProperty"),1024.0000000001);
        pgman->SetPropertyValue(wxT("BoolProperty"),FALSE);
        pgman->SetPropertyValue(wxT("EnumProperty"),120);
        pgman->SetPropertyValue(wxT("Custom FlagsProperty"),FLAG_TEST_SET1);
        pgman->SetPropertyValue(wxT("ArrayStringProperty"),test_arrstr_1);
        wxColour emptyCol;
        pgman->SetPropertyValue(wxT("ColourProperty"),emptyCol);
        pgman->SetPropertyValue(wxT("ColourProperty"),(wxObject*)wxBLACK);
        pgman->SetPropertyValue(wxT("Size"),wxSize(150,150));
        pgman->SetPropertyValue(wxT("Position"),wxPoint(150,150));
        pgman->SetPropertyValue(wxT("MultiChoiceProperty"),test_arrint_1);
#if wxUSE_DATETIME
        pgman->SetPropertyValue(wxT("DateProperty"),dt1);
#endif

        pgman->SelectPage(2);
        pg = pgman->GetGrid();

        if ( pg->GetPropertyValueAsString(wxT("StringProperty")) != wxT("Text1") )
            RT_FAILURE();
        if ( pg->GetPropertyValueAsInt(wxT("IntProperty")) != 1024 )
            RT_FAILURE();
        if ( pg->GetPropertyValueAsDouble(wxT("FloatProperty")) != 1024.0000000001 )
            RT_FAILURE();
        if ( pg->GetPropertyValueAsBool(wxT("BoolProperty")) != FALSE )
            RT_FAILURE();
        if ( pg->GetPropertyValueAsLong(wxT("EnumProperty")) != 120 )
            RT_FAILURE();
        if ( pg->GetPropertyValueAsArrayString(wxT("ArrayStringProperty")) != test_arrstr_1 )
            RT_FAILURE();
        if ( pg->GetPropertyValueAsLong(wxT("Custom FlagsProperty")) != FLAG_TEST_SET1 )
            RT_FAILURE();
        wxColour col;
        col << pgman->GetPropertyValue(wxT("ColourProperty"));
        if ( col != *wxBLACK )
            RT_FAILURE();
        if ( pg->GetPropertyValueAsSize(wxT("Size")) != wxSize(150,150) )
            RT_FAILURE();
        if ( pg->GetPropertyValueAsPoint(wxT("Position")) != wxPoint(150,150) )
            RT_FAILURE();
        if ( !(pg->GetPropertyValueAsArrayInt(wxT("MultiChoiceProperty")) == test_arrint_1) )
            RT_FAILURE();
#if wxUSE_DATETIME
        if ( !(pg->GetPropertyValueAsDateTime(wxT("DateProperty")) == dt1) )
            RT_FAILURE();
#endif

        pgman->SetPropertyValue(wxT("IntProperty"),wxLL(10000000000));
        if ( pg->GetPropertyValueAsLongLong(wxT("IntProperty")) != wxLL(10000000000) )
            RT_FAILURE();

        pg->SetPropertyValue(wxT("StringProperty"),wxT("Text2"));
        pg->SetPropertyValue(wxT("IntProperty"),512);
        pg->SetPropertyValue(wxT("FloatProperty"),512.0);
        pg->SetPropertyValue(wxT("BoolProperty"),TRUE);
        pg->SetPropertyValue(wxT("EnumProperty"),80);
        pg->SetPropertyValue(wxT("ArrayStringProperty"),test_arrstr_2);
        pg->SetPropertyValue(wxT("Custom FlagsProperty"),FLAG_TEST_SET2);
        pg->SetPropertyValue(wxT("ColourProperty"),(wxObject*)wxWHITE);
        pg->SetPropertyValue(wxT("Size"),wxSize(300,300));
        pg->SetPropertyValue(wxT("Position"),wxPoint(300,300));
        pg->SetPropertyValue(wxT("MultiChoiceProperty"),test_arrint_2);
#if wxUSE_DATETIME
        pg->SetPropertyValue(wxT("DateProperty"),dt2);
#endif

        //pg = (wxPropertyGrid*) NULL;

        pgman->SelectPage(0);

        if ( pgman->GetPropertyValueAsString(wxT("StringProperty")) != wxT("Text2") )
            RT_FAILURE();
        if ( pgman->GetPropertyValueAsInt(wxT("IntProperty")) != 512 )
            RT_FAILURE();
        if ( pgman->GetPropertyValueAsDouble(wxT("FloatProperty")) != 512.0 )
            RT_FAILURE();
        if ( pgman->GetPropertyValueAsBool(wxT("BoolProperty")) != TRUE )
            RT_FAILURE();
        if ( pgman->GetPropertyValueAsLong(wxT("EnumProperty")) != 80 )
            RT_FAILURE();
        if ( pgman->GetPropertyValueAsArrayString(wxT("ArrayStringProperty")) != test_arrstr_2 )
            RT_FAILURE();
        if ( pgman->GetPropertyValueAsLong(wxT("Custom FlagsProperty")) != FLAG_TEST_SET2 )
            RT_FAILURE();
        col << pgman->GetPropertyValue(wxT("ColourProperty"));
        if ( col != *wxWHITE )
            RT_FAILURE();
        if ( pgman->GetPropertyValueAsSize(wxT("Size")) != wxSize(300,300) )
            RT_FAILURE();
        if ( pgman->GetPropertyValueAsPoint(wxT("Position")) != wxPoint(300,300) )
            RT_FAILURE();
        if ( !(pgman->GetPropertyValueAsArrayInt(wxT("MultiChoiceProperty")) == test_arrint_2) )
            RT_FAILURE();
#if wxUSE_DATETIME
        if ( !(pgman->GetPropertyValueAsDateTime(wxT("DateProperty")) == dt2) )
            RT_FAILURE();
#endif

        pgman->SetPropertyValue(wxT("IntProperty"),wxLL(-80000000000));
        if ( pgman->GetPropertyValueAsLongLong(wxT("IntProperty")) != wxLL(-80000000000) )
            RT_FAILURE();
    }

    {
        RT_START_TEST(SetPropertyUnspecified)

        // Null variant setter tests
        pgman->SetPropertyUnspecified(wxT("StringProperty"));
        pgman->SetPropertyUnspecified(wxT("IntProperty"));
        pgman->SetPropertyUnspecified(wxT("FloatProperty"));
        pgman->SetPropertyUnspecified(wxT("BoolProperty"));
        pgman->SetPropertyUnspecified(wxT("EnumProperty"));
        pgman->SetPropertyUnspecified(wxT("ArrayStringProperty"));
        pgman->SetPropertyUnspecified(wxT("Custom FlagsProperty"));
        pgman->SetPropertyUnspecified(wxT("ColourProperty"));
        pgman->SetPropertyUnspecified(wxT("Size"));
        pgman->SetPropertyUnspecified(wxT("Position"));
        pgman->SetPropertyUnspecified(wxT("MultiChoiceProperty"));
#if wxUSE_DATETIME
        pgman->SetPropertyUnspecified(wxT("DateProperty"));
#endif
    }

    {
        wxPropertyGridPage* page1;
        wxPropertyGridPage* page2;
        wxPropertyGridPage* page3;
        wxVariant pg1_values;
        wxVariant pg2_values;
        wxVariant pg3_values;

        {
            RT_START_TEST(GetPropertyValues)

            page1 = pgman->GetPage(0);
            pg1_values = page1->GetPropertyValues(wxT("Page1"),NULL,wxPG_KEEP_STRUCTURE);
            page2 = pgman->GetPage(1);
            pg2_values = page2->GetPropertyValues(wxT("Page2"),NULL,wxPG_KEEP_STRUCTURE);
            page3 = pgman->GetPage(2);
            pg3_values = page3->GetPropertyValues(wxT("Page3"),NULL,wxPG_KEEP_STRUCTURE);
        }

        {
            RT_START_TEST(SetPropertyValues)

            page1->SetPropertyValues(pg3_values);
            page2->SetPropertyValues(pg1_values);
            page3->SetPropertyValues(pg2_values);
        }
    }

    if ( !(pgman->GetWindowStyleFlag()&wxPG_HIDE_CATEGORIES) )
    {
        RT_START_TEST(Collapse_and_GetFirstCategory_and_GetNextCategory)

        for ( i=0; i<3; i++ )
        {
            wxPropertyGridPage* page = pgman->GetPage(i);

            wxPropertyGridIterator it;

            for ( it = page->GetIterator( wxPG_ITERATE_CATEGORIES );
                  !it.AtEnd();
                  it++ )
            {
                wxPGProperty* p = *it;

                if ( !page->IsPropertyCategory(p) )
                    RT_FAILURE();

                page->Collapse( p );

                t.Printf(wxT("Collapsing: %s\n"),page->GetPropertyLabel(p).c_str());
                ed->AppendText(t);
            }
        }
    }

    //if ( !(pgman->GetWindowStyleFlag()&wxPG_HIDE_CATEGORIES) )
    {
        RT_START_TEST(Expand_and_GetFirstCategory_and_GetNextCategory)

        for ( i=0; i<3; i++ )
        {
            wxPropertyGridPage* page = pgman->GetPage(i);

            wxPropertyGridIterator it;

            for ( it = page->GetIterator( wxPG_ITERATE_CATEGORIES );
                  !it.AtEnd();
                  it++ )
            {
                wxPGProperty* p = *it;

                if ( !page->IsPropertyCategory(p) )
                    RT_FAILURE();

                page->Expand( p );

                t.Printf(wxT("Expand: %s\n"),page->GetPropertyLabel(p).c_str());
                ed->AppendText(t);
            }
        }
    }

    //if ( !(pgman->GetWindowStyleFlag()&wxPG_HIDE_CATEGORIES) )
    {
        RT_START_TEST(RandomCollapse)

        // Select the most error prone page as visible.
        pgman->SelectPage(1);

        for ( i=0; i<3; i++ )
        {
            wxArrayPtrVoid arr;

            wxPropertyGridPage* page = pgman->GetPage(i);

            wxPropertyGridIterator it;

            for ( it = page->GetIterator( wxPG_ITERATE_CATEGORIES );
                  !it.AtEnd();
                  it++ )
            {
                arr.Add((void*)*it);
            }

            if ( arr.GetCount() )
            {
                size_t n;

                pgman->Collapse( (wxPGProperty*)arr.Item(0) );

                for ( n=arr.GetCount()-1; n>0; n-- )
                {
                    pgman->Collapse( (wxPGProperty*)arr.Item(n) );
                }
            }

        }
    }

    {
        RT_START_TEST(EnsureVisible)
        pgman->EnsureVisible(wxT("Cell Colour"));
    }

    {
        RT_START_TEST(SetPropertyBackgroundColour)
        wxCommandEvent evt;
        evt.SetInt(1); // IsChecked() will return TRUE.
        evt.SetId(ID_COLOURSCHEME4);
        OnCatColours(evt);
        OnColourScheme(evt);
    }

    {
        // Test ClearPropertyValue
        RT_START_TEST(ClearPropertyValue)

        for ( i=0; i<3; i++ )
        {
            wxPropertyGridPage* page = pgman->GetPage(i);

            // Iterate over all properties.
            wxPropertyGridIterator it;

            for ( it = page->GetIterator();
                  !it.AtEnd();
                  it++ )
            {
                wxLogDebug((*it)->GetLabel());
                pgman->ClearPropertyValue( *it );
            }
        }

    }

    {
        RT_START_TEST(ManagerClear)
        pgman->Clear();

        if ( pgman->GetPageCount() )
            RT_FAILURE();

        // Recreate the original grid
        CreateGrid( -1, -1 );
        pgman = m_pPropGridMan;
    }

    {
        RT_START_TEST(MultipleColumns)

        // Test with multiple columns
        // FIXME: Does not display changes.
        for ( i=3; i<12; i+=2 )
        {
            RT_MSG(wxString::Format(wxT("%i columns"),i));
            CreateGrid( -1, -1 );
            pgman = m_pPropGridMan;
            pgman->SetColumnCount(i);
            Refresh();
            Update();
            wxMilliSleep(500);
        }
    }

    if ( fullTest )
    {
        RT_START_TEST(WindowStyles)

        // Recreate grid with all possible (single) flags
        wxASSERT(wxPG_AUTO_SORT == 0x000000010);

        for ( i=4; i<16; i++ )
        {
            int flag = 1<<i;
            RT_MSG(wxString::Format(wxT("Style: 0x%X"),flag));
            CreateGrid( flag, -1 );
            pgman = m_pPropGridMan;
            Update();
            wxMilliSleep(500);
        }

        wxASSERT(wxPG_EX_INIT_NOCAT == 0x00001000);

        for ( i=12; i<24; i++ )
        {
            int flag = 1<<i;
            RT_MSG(wxString::Format(wxT("ExStyle: 0x%X"),flag));
            CreateGrid( -1, flag );
            pgman = m_pPropGridMan;
            Update();
            wxMilliSleep(500);
        }

        // Recreate the original grid
        CreateGrid( -1, -1 );
        pgman = m_pPropGridMan;
    }

    RT_START_TEST(none)

    if ( failures || errorMessages.size() )
    {
        wxString s;
#ifdef __WXDEBUG__
        if ( failures )
#endif
            s = wxString::Format(wxT("%i tests failed!!!"), failures);
#ifdef __WXDEBUG__
        else
            s = wxString::Format(wxT("All tests were succesfull, but there were %i warnings!"), wxPGGlobalVars->m_warnings);
#endif
        RT_MSG(s)
        for ( i=0; i<errorMessages.size(); i++ )
            RT_MSG(errorMessages[i])
        wxMessageBox(s, wxT("Some Tests Failed"));
    }
    else
    {
        RT_MSG(wxT("All tests succesfull"))
    }

    pgman->SelectPage(0);

    // Test may screw up the toolbar, so we need to refresh it.
    wxToolBar* toolBar = pgman->GetToolBar();
    if ( toolBar )
        toolBar->Refresh();

#endif // TESTING_WXPROPERTYGRIDADV
}

// -----------------------------------------------------------------------
