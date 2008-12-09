#ifndef _WX_JIGSAW_SHAPE_PROPERTY_IO_H
#define _WX_JIGSAW_SHAPE_PROPERTY_IO_H

#include <wx/wx.h>
#include <wx/wxxmlserializer/XmlSerializer.h>

class wxJigsawShape;
class wxJigsawShapeList;

/// \brief wxXmlSerializer IO Handler for wxJigsawShape
XS_DECLARE_EXPORTED_IO_HANDLER(wxJigsawShape *, xsJigsawShapePropIO, WXDLLIMPEXP_XS);
/// \brief wxXmlSerializer IO Handler for wxJigsawShapeList
XS_DECLARE_EXPORTED_IO_HANDLER(wxJigsawShapeList, xsListJigsawShapePropIO, WXDLLIMPEXP_XS);

#define XS_SERIALIZE_JIGSAWSHAPE(x, name) XS_SERIALIZE_PROPERTY(x, wxT("jigsawshape"), name);
#define XS_SERIALIZE_LISTJIGSAWSHAPE(x, name) XS_SERIALIZE_PROPERTY(x, wxT("listjigsawshape"), name);

#endif
