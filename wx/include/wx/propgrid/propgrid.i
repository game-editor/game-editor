/////////////////////////////////////////////////////////////////////////////
// Name:        propgrid.i
// Purpose:     Wrappers for the wxPropertyGrid.
//
// Author:      Jaakko Salli
//
// Created:     17-Feb-2005
// RCS-ID:      $Id:
// Copyright:   (c) 2005 by Jaakko Salli
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

%define DOCSTRING
"The `PropertyGrid` provides a specialized two-column grid for editing
properties such as strings, numbers, colours, and string lists."
%enddef

%module(package="wx", docstring=DOCSTRING) propgrid

//#pragma SWIG nowarn=511

%{

#include "wx/wxPython/wxPython.h"
#include "wx/wxPython/pyclasses.h"
#include <wx/propgrid/propgrid.h>

%}

// wxPropertyGrid specific special code
%{
static wxString& wxString_wxPG_LABEL = *((wxString*)NULL);
static wxArrayInt& wxArrayInt_wxPG_EMPTY = *((wxArrayInt*)NULL);
static wxArrayString& wxArrayString_wxPG_EMPTY = *((wxArrayString*)NULL);
%}

//---------------------------------------------------------------------------

%import windows.i
%pythoncode { wx = _core }
%pythoncode { __docfilter__ = wx.__DocFilter(globals()) }

%{
const wxChar* wxPyPropertyGridNameStr = wxPropertyGridNameStr;
const wxChar* PropertyGridNameStr = wxPropertyGridNameStr;
%}

%pythoncode {
PropertyGridNameStr = "wxPropertyGrid"
}


%include _propgrid_rename.i

MustHaveApp(wxPropertyGrid);

//---------------------------------------------------------------------------
// Get all our defs from the REAL header file.

%include propgrid.h

//---------------------------------------------------------------------------
// Python functions to act like the event macros

%pythoncode {
EVT_PG_SELECTED = wx.PyEventBinder( wxEVT_PG_CHANGED, 1 )
EVT_PG_HIGHLIGHTED = wx.PyEventBinder( wxEVT_PG_HIGHLIGHTED, 1 )
EVT_PG_RIGHT_CLICK = wx.PyEventBinder( wxEVT_PG_RIGHT_CLICK, 1 )
EVT_PG_PAGE_CHANGED = wx.PyEventBinder( wxEVT_PG_PAGE_CHANGED, 1 )
}

//---------------------------------------------------------------------------

%init %{
%}


//---------------------------------------------------------------------------

