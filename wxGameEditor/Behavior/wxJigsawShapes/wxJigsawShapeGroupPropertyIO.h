#ifndef _WX_JIGSAW_SHAPE_GROUP_PROPERTY_IO_H
#define _WX_JIGSAW_SHAPE_GROUP_PROPERTY_IO_H

#include <wx/wx.h>
#include <wx/wxxmlserializer/XmlSerializer.h>

class wxJigsawShapeGroup;
class wxJigsawShapeGroupList;

XS_DECLARE_EXPORTED_IO_HANDLER(wxJigsawShapeGroup *, xsJigsawShapeGroupPropIO, WXDLLIMPEXP_XS);
XS_DECLARE_EXPORTED_IO_HANDLER(wxJigsawShapeGroupList, xsListJigsawShapeGroupPropIO, WXDLLIMPEXP_XS);

#define XS_SERIALIZE_JIGSAWSHAPEGROUP(x, name) XS_SERIALIZE_PROPERTY(x, wxT("jigsawshapegroup"), name);
#define XS_SERIALIZE_LISTJIGSAWSHAPEGROUP(x, name) XS_SERIALIZE_PROPERTY(x, wxT("listjigsawshapegroup"), name);

#endif
