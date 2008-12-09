/***************************************************************
 * Name:      PropertyIO.cpp
 * Purpose:   Declares data types I/O and conversion functions
 * Author:    Michal Bližòák (michal.bliznak@tiscali.cz)
 * Created:   2007-10-28
 * Copyright: Michal Bližòák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef XSPROPERTYIO_H
#define XSPROPERTYIO_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/hashmap.h>
#include <wx/xml/xml.h>
#include <wx/tokenzr.h>
#include <wx/list.h>

#include "wx/wxxmlserializer/Defs.h"

class WXDLLIMPEXP_XS xsProperty;
class WXDLLIMPEXP_XS xsSerializable;

WX_DECLARE_OBJARRAY_WITH_DECL(wxRealPoint, RealPointArray, class WXDLLIMPEXP_XS);
WX_DECLARE_LIST_WITH_DECL(wxRealPoint, RealPointList, class WXDLLIMPEXP_XS);

/*!
 * \brief Base class encapsulating a property I/O handler. The class is used by
 * the xsSerializable class and is responsiblefor reading and writing of an XML node
 * containing property information. Each supported property (data) type should have
 * its own I/O handler class. Moreover, all derived classes must provide public functions
 * 'static wxString classname::ToString(datatype value)' and 'static datatype classname::
 * FromString( const wxString& value )' responsible for conversion between datatype and
 * and its string representation (these functions are used internally by class virtual functions.
 */
class WXDLLIMPEXP_XS xsPropertyIO : public wxObject
{
public:
    DECLARE_DYNAMIC_CLASS(xsProperty);

    /*! \brief Constructor. */
    xsPropertyIO(){;}
    /*! \brief Destructor. */
    virtual ~xsPropertyIO(){;}

    /*!
     * \brief Read content of the property XML node and store it to given property object.
     * \param property Pointer to the target property object
     * \param source Pointer to the source XML node
     */
    virtual void Read(xsProperty *property, wxXmlNode *source){;}
    /*!
     * \brief Write content of given property object to target XML node.
     * \param property Pointer to the source property object
     * \param target Pointer to the target XML node
     */
    virtual void Write(xsProperty *property, wxXmlNode *target){;}
    /*!
     * \brief Get textual representation of current property value.
	 * \param property Pointer to the source property object
     */
	virtual wxString GetValueStr(xsProperty *property){return wxT("");}

    /*!
     * \brief Create new XML node of given name and value and assign it to the given
     * parent XML node.
     * \param parent Pointer to parent XML node
     * \param name Name of new XML node
     * \param value Content of new XML node
     */
     wxXmlNode* AddPropertyNode(wxXmlNode* parent, const wxString& name, const wxString& value);

protected:

    /*!
     * \brief Append info about the source property to given XML node.
     * \param source Pointer to the source property
     * \param target Pointer to modified XML node
     */
    void AppendPropertyType(xsProperty *source, wxXmlNode *target);
};

/*!
 * \brief Macro suitable for declaration of new property I/O handler
 * \param datatype Property's data type
 * \param name Handler class name
 */
#define XS_DECLARE_IO_HANDLER(datatype, name) \
class name : public xsPropertyIO \
{ \
public: \
	DECLARE_DYNAMIC_CLASS(name); \
	name(){;} \
	virtual ~name(){;} \
\
	virtual void Read(xsProperty *property, wxXmlNode *source); \
	virtual void Write(xsProperty *property, wxXmlNode *target); \
	virtual wxString GetValueStr(xsProperty *property); \
	static wxString ToString(datatype value); \
	static datatype FromString(const wxString& value); \
}; \

/*!
 * \brief Macro suitable for declaration of exported new property I/O handler
 * \param datatype Property's data type
 * \param name Handler class name
 * \param decoration Class decoration
 */
#define XS_DECLARE_EXPORTED_IO_HANDLER(datatype, name, decoration) \
class decoration name : public xsPropertyIO \
{ \
public: \
	DECLARE_DYNAMIC_CLASS(name); \
	name(){;} \
	virtual ~name(){;} \
\
	virtual void Read(xsProperty *property, wxXmlNode *source); \
	virtual void Write(xsProperty *property, wxXmlNode *target); \
	virtual wxString GetValueStr(xsProperty *property); \
	static wxString ToString(datatype value); \
	static datatype FromString(const wxString& value); \
}; \

/*!
 * \brief Macro suitable for implementation of new property I/O handler
 * \param datatype Property's data type
 * \param name Handler class name
 */
#define XS_DEFINE_IO_HANDLER(datatype, name) \
IMPLEMENT_DYNAMIC_CLASS(name, xsPropertyIO); \
\
void name::Read(xsProperty *property, wxXmlNode *source) \
{ \
    *((datatype*)property->m_pSourceVariable) = FromString(source->GetNodeContent()); \
} \
\
void name::Write(xsProperty *property, wxXmlNode *target) \
{ \
    wxString val = ToString(*((datatype*)property->m_pSourceVariable)); \
\
    if(val != property->m_sDefaultValueStr) \
    { \
        wxXmlNode *newNode = AddPropertyNode(target, wxT("property"), val); \
        AppendPropertyType(property, newNode); \
    } \
} \
\
wxString name::GetValueStr(xsProperty *property) \
{ \
	return ToString(*((datatype*)property->m_pSourceVariable)); \
} \

/*!
 * \brief Property class encapsulating I/O functions used by 'wxString' properties.
 */
XS_DECLARE_EXPORTED_IO_HANDLER(wxString, xsStringPropIO, WXDLLIMPEXP_XS);

/*!
 * \brief Property class encapsulating I/O functions used by 'wxChar' properties.
 */
XS_DECLARE_EXPORTED_IO_HANDLER(wxChar, xsCharPropIO, WXDLLIMPEXP_XS);

/*!
 * \brief Property class encapsulating I/O functions used by 'long' properties.
 */
XS_DECLARE_EXPORTED_IO_HANDLER(long, xsLongPropIO, WXDLLIMPEXP_XS);

/*!
 * \brief Property class encapsulating I/O functions used by 'int' properties.
 */
XS_DECLARE_EXPORTED_IO_HANDLER(int, xsIntPropIO, WXDLLIMPEXP_XS);

/*!
 * \brief Property class encapsulating I/O functions used by 'bool' properties.
 */
XS_DECLARE_EXPORTED_IO_HANDLER(bool, xsBoolPropIO, WXDLLIMPEXP_XS);

/*!
 * \brief Property class encapsulating I/O functions used by 'double' properties.
 */
XS_DECLARE_EXPORTED_IO_HANDLER(double, xsDoublePropIO, WXDLLIMPEXP_XS);

/*!
 * \brief Property class encapsulating I/O functions used by 'float' properties.
 */
XS_DECLARE_EXPORTED_IO_HANDLER(float, xsFloatPropIO, WXDLLIMPEXP_XS);

/*!
 * \brief Property class encapsulating I/O functions used by 'wxPoint' properties.
 */
XS_DECLARE_EXPORTED_IO_HANDLER(wxPoint, xsPointPropIO, WXDLLIMPEXP_XS);

/*!
 * \brief Property class encapsulating I/O functions used by 'wxSize' properties.
 */
XS_DECLARE_EXPORTED_IO_HANDLER(wxSize, xsSizePropIO, WXDLLIMPEXP_XS);

/*!
 * \brief Property class encapsulating I/O functions used by 'wxRealPoint' properties.
 */
XS_DECLARE_EXPORTED_IO_HANDLER(wxRealPoint, xsRealPointPropIO, WXDLLIMPEXP_XS);

/*!
 * \brief Property class encapsulating I/O functions used by 'wxColour' properties.
 */
XS_DECLARE_EXPORTED_IO_HANDLER(wxColour, xsColourPropIO, WXDLLIMPEXP_XS);

/*!
 * \brief Property class encapsulating I/O functions used by 'wxPen' properties.
 */
XS_DECLARE_EXPORTED_IO_HANDLER(wxPen, xsPenPropIO, WXDLLIMPEXP_XS);

/*!
 * \brief Property class encapsulating I/O functions used by 'wxBrush' properties.
 */
XS_DECLARE_EXPORTED_IO_HANDLER(wxBrush, xsBrushPropIO, WXDLLIMPEXP_XS);

/*!
 * \brief Property class encapsulating I/O functions used by 'wxFont' properties.
 */
XS_DECLARE_EXPORTED_IO_HANDLER(wxFont, xsFontPropIO, WXDLLIMPEXP_XS);

/*!
 * \brief Property class encapsulating I/O functions used by 'wxArrayString' properties.
 */
XS_DECLARE_EXPORTED_IO_HANDLER(wxArrayString, xsArrayStringPropIO, WXDLLIMPEXP_XS);

/*!
 * \brief Property class encapsulating I/O functions used by 'RealPointArray' (array of
 * wxRealPoint objects) properties.
 */
XS_DECLARE_EXPORTED_IO_HANDLER(RealPointArray, xsArrayRealPointPropIO, WXDLLIMPEXP_XS);

/*!
 * \brief Property class encapsulating I/O functions used by 'ListRealPoint' (list of
 * wxRealPoint objects) properties.
 */
XS_DECLARE_EXPORTED_IO_HANDLER(RealPointList, xsListRealPointPropIO, WXDLLIMPEXP_XS);

/*!
 * \brief Property class encapsulating I/O functions used by 'serializabledynamic' (xsSerializable
 * dynamic class objects which are created during the deserialization process) properties.
 */
XS_DECLARE_EXPORTED_IO_HANDLER(xsSerializable*, xsDynObjPropIO, WXDLLIMPEXP_XS);

/*!
 * \brief Property class encapsulating I/O functions used by 'serializabledynamicnocreate' (already
 * existing xsSerializable dynamic class objects) properties.
 */
XS_DECLARE_EXPORTED_IO_HANDLER(xsSerializable*, xsDynNCObjPropIO, WXDLLIMPEXP_XS);

/*!
 * \brief Property class encapsulating I/O functions used by 'serializablestatic' (static
 * xsSerializable class objects) properties.
 */
XS_DECLARE_EXPORTED_IO_HANDLER(xsSerializable, xsStaticObjPropIO, WXDLLIMPEXP_XS);

WX_DECLARE_HASH_MAP( wxString, xsPropertyIO*, wxStringHash, wxStringEqual, PropertyIOMap );

#endif // XSPROPERTYIO_H
