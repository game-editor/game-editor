/***************************************************************
 * Name:      XmlSerializer.cpp
 * Purpose:   Implements XML serializer and related classes
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-08-28
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifdef _DEBUG_MSVC
#define new DEBUG_NEW
#endif

#include "wx/wxxmlserializer/XmlSerializer.h"

#include <wx/listimpl.cpp>
#include <wx/wfstream.h>
#include <wx/arrimpl.cpp>

WX_DEFINE_EXPORTED_LIST(PropertyList);
WX_DEFINE_EXPORTED_LIST(SerializableList);

// static members
PropertyIOMap wxXmlSerializer::m_mapPropertyIOHandlers;
int wxXmlSerializer::m_nRefCounter = 0;
wxString wxXmlSerializer::m_sLibraryVersion = wxT("1.1.3 beta");

/////////////////////////////////////////////////////////////////////////////////////
// xsProperty class /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsProperty, wxObject);

/////////////////////////////////////////////////////////////////////////////////////
// xsSerializable class /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_IMPLEMENT_CLONABLE_CLASS(xsSerializable, wxObject);

// constructor and destructor ///////////////////////////////////////////////////////

xsSerializable::xsSerializable()
{
	m_pParentManager = NULL;
    m_pParentItem = NULL;
    m_fSerialize = true;
	m_fClone = true;
    m_nId = -1;
}

xsSerializable::xsSerializable(const xsSerializable& obj)
{
	m_pParentManager = NULL;
    m_pParentItem = obj.m_pParentItem;
    m_fSerialize = obj.m_fSerialize;
	m_fClone = obj.m_fClone;
    m_nId = obj.m_nId;
}

xsSerializable::~xsSerializable()
{
    m_lstProperties.DeleteContents(true);
    m_lstProperties.Clear();

    m_lstChildItems.DeleteContents(true);
    m_lstChildItems.Clear();
}

// public functions /////////////////////////////////////////////////////////////////

void xsSerializable::AddChild(xsSerializable* child)
{
    wxASSERT(child);

    if( child )
    {
        child->m_pParentItem = this;
		child->m_pParentManager = m_pParentManager;
        m_lstChildItems.Append(child);
    }
}

void xsSerializable::Reparent(xsSerializable* parent)
{
    if(m_pParentItem)
    {
        m_pParentItem->m_lstChildItems.DeleteObject(this);
    }

    if(parent)
    {
        parent->AddChild(this);
    }
    else
        m_pParentItem = NULL;
}

xsSerializable* xsSerializable::GetFirstChild()
{
	SerializableList::compatibility_iterator node = m_lstChildItems.GetFirst();
    if(node)return node->GetData();
    else
        return NULL;
}

xsSerializable* xsSerializable::GetLastChild()
{
    SerializableList::compatibility_iterator node = m_lstChildItems.GetLast();
    if(node)return node->GetData();
    else
        return NULL;
}

xsSerializable* xsSerializable::GetSibbling()
{
    wxASSERT( m_pParentItem );

    if( m_pParentItem )
    {
		SerializableList::compatibility_iterator sibblingNode, node = m_pParentItem->GetChildren().GetFirst();
        while(node)
        {
            if( node->GetData() == this )
            {
                sibblingNode = node->GetNext();
                if( sibblingNode )return sibblingNode->GetData();
            }
            node = node->GetNext();
        }
    }

    return NULL;
}

xsSerializable* xsSerializable::GetChild(long id)
{
    SerializableList::compatibility_iterator node = m_lstChildItems.GetFirst();
    while(node)
    {
		if( node->GetData()->GetId() == id) return node->GetData();
        node = node->GetNext();
    }

	return NULL;
}

void xsSerializable::AddProperty(xsProperty* property)
{
    if(property)
    {
        if(!GetProperty(property->m_sFieldName))
        {
            m_lstProperties.Append(property);
        }
    }
}

xsProperty* xsSerializable::GetProperty(const wxString& field)
{
	PropertyList::compatibility_iterator node = m_lstProperties.GetFirst();
    while(node)
    {
        if(node->GetData()->m_sFieldName == field)return node->GetData();
        node = node->GetNext();
    }
    return NULL;
}

void xsSerializable::EnablePropertySerialization(const wxString& field, bool enab)
{
    xsProperty* property = GetProperty(field);
    if(property)
    {
        property->m_fSerialize = enab;
    }
}

bool xsSerializable::IsPropertySerialized(const wxString& field)
{
    xsProperty* property = GetProperty(field);
    if(property)
    {
        return property->m_fSerialize;
    }
    return false;
}

wxXmlNode* xsSerializable::SerializeObject(wxXmlNode* node)
{
	if(!node || (node->GetName() != wxT("object")))
	{
		node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("object"));
		node->AddProperty(new wxXmlProperty(wxT("type"), this->GetClassInfo()->GetClassName()));
	}

	if(node)
	{
		node =  this->Serialize(node);
	}

	return node;
}

void xsSerializable::DeserializeObject(wxXmlNode* node)
{
	if(node && (node->GetName()==wxT("object")))
	{
		this->Deserialize(node);
	}
}

// protected functions //////////////////////////////////////////////////////////////

wxXmlNode* xsSerializable::Serialize(wxXmlNode* node)
{
    xsProperty* property;
    xsPropertyIO* ioHandler;

    PropertyList::compatibility_iterator propNode = m_lstProperties.GetFirst();
    while(propNode)
    {
        property = propNode->GetData();

        if(property->m_fSerialize)
        {
			ioHandler = wxXmlSerializer::m_mapPropertyIOHandlers[property->m_sDataType];
            if(ioHandler)
            {
                ioHandler->Write(property, node);
            }
        }

        propNode = propNode->GetNext();
    }

    return node;
}

void xsSerializable::Deserialize(wxXmlNode* node)
{
    wxASSERT(node);
    if(!node)return;

    xsProperty* property;
    xsPropertyIO* ioHandler;
    wxString propName;

    wxXmlNode *xmlNode = node->GetChildren();
	while(xmlNode)
	{
	    if(xmlNode->GetName() == wxT("property"))
	    {
	        xmlNode->GetPropVal(wxT("name"), &propName);
	        property = GetProperty(propName);

	        if(property)
	        {
                ioHandler =  wxXmlSerializer::m_mapPropertyIOHandlers[property->m_sDataType];
                if(ioHandler)
                {
                    ioHandler->Read(property, xmlNode);
                }
	        }
	    }

	    xmlNode = xmlNode->GetNext();
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// wxXmlSerializer class ////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_IMPLEMENT_CLONABLE_CLASS(wxXmlSerializer, wxObject);

// constructor and destructor ///////////////////////////////////////////////////////

wxXmlSerializer::wxXmlSerializer()
{
    m_sOwner = wxT("");
    m_sRootName = wxT("root");
    m_sVersion = wxT("");

	m_fClone = true;

	m_pRoot = NULL;
    SetRootItem(new xsSerializable());

	if(m_nRefCounter == 0)
	{
		InitializeAllIOHandlers();
	}
	m_nRefCounter++;
}

wxXmlSerializer::wxXmlSerializer(wxXmlSerializer &obj)
{
	m_sOwner = obj.m_sOwner;
    m_sRootName = obj.m_sRootName;
	m_sVersion = obj.m_sVersion;

	m_fClone = obj.m_fClone;

	m_pRoot = NULL;
	SetRootItem((xsSerializable*)obj.m_pRoot->Clone());

	CopyItems(&obj);

	m_nRefCounter++;
}

wxXmlSerializer::wxXmlSerializer(const wxString& owner, const wxString& root, const wxString& version)
{
    m_sOwner = owner;
    m_sRootName = root;
    m_sVersion = version;

	m_fClone = true;

	m_pRoot = NULL;
    SetRootItem(new xsSerializable());

	if(m_nRefCounter == 0)
	{
		InitializeAllIOHandlers();
	}
	m_nRefCounter++;
}

wxXmlSerializer::~wxXmlSerializer()
{
    if( m_pRoot ) delete m_pRoot;

	m_nRefCounter--;
	if(m_nRefCounter == 0)
	{
		ClearIOHandlers();
	}
}

// public functions //////////////////////////////////////////////////////////////////

void wxXmlSerializer::InitializeAllIOHandlers()
{
	ClearIOHandlers();

    XS_REGISTER_IO_HANDLER(wxT("string"), xsStringPropIO);
	XS_REGISTER_IO_HANDLER(wxT("char"), xsCharPropIO);
	XS_REGISTER_IO_HANDLER(wxT("int"), xsIntPropIO);
    XS_REGISTER_IO_HANDLER(wxT("long"), xsLongPropIO);
	XS_REGISTER_IO_HANDLER(wxT("float"), xsFloatPropIO);
    XS_REGISTER_IO_HANDLER(wxT("double"), xsDoublePropIO);
    XS_REGISTER_IO_HANDLER(wxT("bool"), xsBoolPropIO);
    XS_REGISTER_IO_HANDLER(wxT("point"), xsPointPropIO);
    XS_REGISTER_IO_HANDLER(wxT("size"), xsSizePropIO);
    XS_REGISTER_IO_HANDLER(wxT("realpoint"), xsRealPointPropIO);
    XS_REGISTER_IO_HANDLER(wxT("colour"), xsColourPropIO);
    XS_REGISTER_IO_HANDLER(wxT("brush"), xsBrushPropIO);
    XS_REGISTER_IO_HANDLER(wxT("pen"), xsPenPropIO);
    XS_REGISTER_IO_HANDLER(wxT("font"), xsFontPropIO);
    XS_REGISTER_IO_HANDLER(wxT("arraystring"), xsArrayStringPropIO);
    XS_REGISTER_IO_HANDLER(wxT("arrayrealpoint"), xsArrayRealPointPropIO);
    XS_REGISTER_IO_HANDLER(wxT("listrealpoint"), xsListRealPointPropIO);
    XS_REGISTER_IO_HANDLER(wxT("serializablestatic"), xsStaticObjPropIO);
    XS_REGISTER_IO_HANDLER(wxT("serializabledynamic"), xsDynObjPropIO);
    XS_REGISTER_IO_HANDLER(wxT("serializabledynamicnocreate"), xsDynNCObjPropIO);
}

void wxXmlSerializer::ClearIOHandlers()
{
    PropertyIOMap::iterator it = m_mapPropertyIOHandlers.begin();
    while(it != m_mapPropertyIOHandlers.end())
    {
		if(it->second)delete it->second;
        it++;
    }
    m_mapPropertyIOHandlers.clear();
}

xsSerializable* wxXmlSerializer::GetItem(long id)
{
    if( m_pRoot )
    {
        return _GetItem(id, m_pRoot);
    }

    return NULL;
}

bool wxXmlSerializer::Contains(xsSerializable *object)
{
    if( m_pRoot )
    {
        return _Contains(object, m_pRoot);
    }

    return false;
}

int wxXmlSerializer::GetItems(wxClassInfo* type, SerializableList& list)
{
    m_nCounter = 0;

    if( m_pRoot )
    {
        _GetItems(type, m_pRoot, list);
    }

    return m_nCounter;
}

void wxXmlSerializer::CopyItems(wxXmlSerializer *src)
{
	// create new root (all old serializer's content will be lost)
	SetRootItem((xsSerializable*)m_pRoot->Clone());

	_CopyItems(m_pRoot, src->GetRootItem());
}

void wxXmlSerializer::AddItem(long parentId, xsSerializable* item)
{
    AddItem(GetItem(parentId), item);
}

void wxXmlSerializer::AddItem(xsSerializable* parent, xsSerializable* item)
{
    wxASSERT(m_pRoot);
    wxASSERT(item);

    if( item )
    {
        if( parent )parent->AddChild(item);
		else
			m_pRoot->AddChild(item);

        item->SetId(GetNewId());
    }
}

void wxXmlSerializer::RemoveItem(long id)
{
    RemoveItem(GetItem(id));
}

void wxXmlSerializer::RemoveItem(xsSerializable* item)
{
    wxASSERT(item);

    if(item)
    {
        if( item->GetParent() )
        {
            item->GetParent()->GetChildren().DeleteObject(item);
        }
        delete item;
    }
}

void wxXmlSerializer::RemoveAll()
{
	SetRootItem(new xsSerializable());
}

void wxXmlSerializer::SetRootItem(xsSerializable* root)
{
    wxASSERT(root);
    wxASSERT(root->IsKindOf(CLASSINFO(xsSerializable)));

	if( m_pRoot )delete m_pRoot;

    if(root && root->IsKindOf(CLASSINFO(xsSerializable)))
    {
        m_pRoot = root;
    }
	else
		m_pRoot = new xsSerializable();

	m_pRoot->m_pParentManager = this;
}

void wxXmlSerializer::SerializeToXml(const wxString& file, bool withroot)
{
	wxFileOutputStream outstream(file);

	if(outstream.IsOk())
	{
		this->SerializeToXml(outstream, withroot);
	}
	else
		wxMessageBox(wxT("Unable to initialize output file stream."), m_sOwner, wxICON_ERROR);
}

void wxXmlSerializer::SerializeToXml(wxOutputStream& outstream, bool withroot)
{
	// create root node
	wxXmlNode *root = new wxXmlNode(wxXML_ELEMENT_NODE, m_sRootName);

	if(root)
	{
	    // add version
	    root->AddProperty(wxT("owner"), m_sOwner);
	    root->AddProperty(wxT("version"), m_sVersion);

	    // serialize root item properties
	    if(withroot)
	    {
	        wxXmlNode *root_props = new wxXmlNode(wxXML_ELEMENT_NODE, m_sRootName + wxT("_properties"));
	        root_props->AddChild(m_pRoot->SerializeObject(NULL));
	        root->AddChild(root_props);
	    }

		// serialize shapes recursively
		this->SerializeObjects(m_pRoot, root, false);

		// create XML document
		try
		{
			wxXmlDocument xmlDoc;
			xmlDoc.SetRoot(root);

			xmlDoc.Save(outstream, 2);
		}
		catch (...)
		{
			wxMessageBox(wxT("Unable to save XML document."), m_sOwner, wxICON_ERROR);
		}
	}
}

void wxXmlSerializer::DeserializeFromXml(const wxString& file)
{
	wxFileInputStream instream(file);
	if(instream.IsOk())
	{
		this->DeserializeFromXml(instream);
	}
	else
		wxMessageBox(wxT("Unable to initialize input stream."), m_sOwner, wxICON_ERROR);
}

void wxXmlSerializer::DeserializeFromXml(wxInputStream& instream)
{
	// load an XML file
	try
	{
		wxXmlDocument xmlDoc;
		xmlDoc.Load(instream);

		wxXmlNode* root = xmlDoc.GetRoot();
		if(root && (root->GetName() == m_sRootName))
		{
		    // read project node's properties here...
		    wxString version, owner;
		    root->GetPropVal(wxT("owner"), &owner);
		    root->GetPropVal(wxT("version"), &version);

		    if( (owner == m_sOwner) && (version == m_sVersion) )
		    {
                // read shape objects from XML recursively
                this->DeserializeObjects(NULL, root);
		    }
            else
                wxMessageBox(wxT("No matching owner or file version."), m_sOwner, wxICON_WARNING);
		}
		else
			wxMessageBox(wxT("Unknown file format."), m_sOwner, wxICON_WARNING);
	}
	catch (...)
	{
		wxMessageBox(wxT("Unable to load XML file."), m_sOwner, wxICON_ERROR);
	}
}

void wxXmlSerializer::SerializeObjects(xsSerializable* parent, wxXmlNode* node, bool withparent)
{
    wxASSERT(parent);
    if(!parent)return;

	wxXmlNode* projectNode = NULL;
	xsSerializable* pChild;

	// serialize parent shape
	if(withparent)
	{
	    if(parent->IsSerialized())
	    {
            projectNode = parent->SerializeObject(NULL);
            if(projectNode)
            {
                SerializeObjects(parent, projectNode, false);
                node->AddChild(projectNode);
            }
	    }
	}
	else
	{
		// serialize parent's children
		SerializableList::compatibility_iterator snode = parent->GetChildren().GetFirst();
		while(snode)
		{
			pChild = snode->GetData();

            if(pChild->IsSerialized())
            {
                projectNode = pChild->SerializeObject(NULL);
                if(projectNode)
                {
                    SerializeObjects(pChild, projectNode, false);
                    node->AddChild(projectNode);
                }
            }

			snode = snode->GetNext();
		}
	}
}

void wxXmlSerializer::DeserializeObjects(xsSerializable* parent, wxXmlNode* node)
{
    wxASSERT(m_pRoot);
    if(!m_pRoot)return;

	xsSerializable *pItem;

	wxXmlNode* projectNode = node->GetChildren();
	while(projectNode)
	{
		if(projectNode->GetName() == wxT("object"))
		{
		    pItem = (xsSerializable*)wxCreateDynamicObject(projectNode->GetPropVal(wxT("type"), wxT("")));
			if(pItem)
			{
			    pItem->SetId(GetNewId());

			    if(parent)
			    {
			        parent->AddChild(pItem);
			    }
			    else
                    m_pRoot->AddChild(pItem);

				pItem->DeserializeObject(projectNode);

				// deserialize child objects
				DeserializeObjects(pItem, projectNode);
			}
		}
		else if(projectNode->GetName() == m_sRootName + wxT("_properties"))
		{
		    m_pRoot->DeserializeObject(projectNode->GetChildren());
		}

		projectNode = projectNode->GetNext();
	}
}

bool wxXmlSerializer::IsIdUsed(long id)
{
	return (GetIDCount(id) > 0);
}

int wxXmlSerializer::GetIDCount(long id)
{
	int nCount = 0;

    SerializableList items;
    GetItems(CLASSINFO(xsSerializable), items);

	SerializableList::compatibility_iterator node = items.GetFirst();
	while(node)
	{
		if(node->GetData()->GetId() == id)nCount++;
		node = node->GetNext();
	}

	return nCount;
}

long wxXmlSerializer::GetNewId()
{
	long nId = 1;

	while(IsIdUsed(nId))nId++;

	return nId;
}

// private virtual functions  ///////////////////////////////////////////////////////

xsSerializable* wxXmlSerializer::_GetItem(long id, xsSerializable* parent)
{
    wxASSERT(parent);

    if( !parent )return NULL;

    if( parent->GetId() == id )return parent;

    xsSerializable *pItem = NULL;
    SerializableList::compatibility_iterator node = parent->GetChildren().GetFirst();
    while(node)
    {
        pItem = _GetItem(id, node->GetData());
        if( pItem )break;
        node = node->GetNext();
    }
    return pItem;
}

void wxXmlSerializer::_GetItems(wxClassInfo* type, xsSerializable* parent, SerializableList& list)
{
    wxASSERT(parent);
    wxASSERT(type);

    if(!parent && !type)return;

    if( parent->IsKindOf(type) )
    {
        list.Append(parent);
        m_nCounter++;
    }

    SerializableList::compatibility_iterator node = parent->GetChildren().GetFirst();
    while(node)
    {
        _GetItems(type, node->GetData(), list);
        node = node->GetNext();
    }
}

bool wxXmlSerializer::_Contains(xsSerializable* object, xsSerializable* parent)
{
    wxASSERT(parent);

    if( !parent )return false;

    if( parent == object )return true;

    bool fFound = false;
    SerializableList::compatibility_iterator node = parent->GetChildren().GetFirst();
    while(node)
    {
        fFound = _Contains(object, node->GetData());
        if( fFound )break;
        node = node->GetNext();
    }
    return fFound;
}

void wxXmlSerializer::_CopyItems(xsSerializable *dest, xsSerializable *parent)
{
	if( !parent || !dest ) return;

	xsSerializable *pSrc, *pCopy, *pChild;

	SerializableList::compatibility_iterator node = parent->GetFirstChildNode();
	while(node)
	{
		pSrc = node->GetData();
		pChild = dest->GetChild(pSrc->GetId());
		// some children can be added to the data manager by user so you must check it
		if( !pChild )
		{
			pCopy = (xsSerializable*)pSrc->Clone();
			if( pCopy )
			{
				dest->AddChild(pCopy);
				_CopyItems(pCopy, pSrc);
			}
		}
		else
			// set correct parent manager in user-added shapes
			pChild->SetParentManager(this);

		node = node->GetNext();
	}
}
