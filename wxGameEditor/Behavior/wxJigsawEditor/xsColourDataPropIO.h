#ifndef _XS_COLOUR_DATA_PROP_IO_H
#define _XS_COLOUR_DATA_PROP_IO_H

#include <wx/wx.h>
#include <wx/cmndata.h>
#include <wx/wxxmlserializer/XmlSerializer.h>
#include <wx/wxxmlserializer/PropertyIO.h>

// declaration of a class 'xsColourDataPropIO' encapsulating custom property I/O handler
// for 'wxColouData' data type
XS_DECLARE_IO_HANDLER(wxColourData, xsColourDataPropIO);

// definition macros which create new serialized wxColourData property (without/with defined default value). data type's assertion is not compulsory, but useful...
#define XS_SERIALIZE_COLOURDATA(x, name) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxColourData)), wxT("Object is not wxColourData"));XS_SERIALIZE_PROPERTY(x, wxT("colourdata"), name);
#define XS_SERIALIZE_COLOURDATA_EX(x, name, def) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxColourData)), wxT("Object is not wxColourData"));XS_SERIALIZE_PROPERTY_EX(x, wxT("colourdata"), name, xsColourDataPropIO::ToString(def));

#endif
