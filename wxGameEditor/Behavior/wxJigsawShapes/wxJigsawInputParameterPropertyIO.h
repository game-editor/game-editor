#ifndef _WX_JIGSAW_INPUT_PARAMETER_PROPERTY_IO
#define _WX_JIGSAW_INPUT_PARAMETER_PROPERTY_IO

#include <wx/wx.h>
#include <wx/wxxmlserializer/XmlSerializer.h>

class wxJigsawInputParameter;
class wxJigsawInputParameters;

XS_DECLARE_EXPORTED_IO_HANDLER(wxJigsawInputParameter *, xsJigsawInputParameterPropIO, WXDLLIMPEXP_XS);
XS_DECLARE_EXPORTED_IO_HANDLER(wxJigsawInputParameters, xsListJigsawInputParameterPropIO, WXDLLIMPEXP_XS);

#define XS_SERIALIZE_LISTJIGSAWINPUTPARAMETER(x, name) \
	XS_SERIALIZE_PROPERTY(x, wxT("listjigsawinputparameter"), name);

#endif
