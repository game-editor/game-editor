/***************************************************************
 * Name:      MainFrame.cpp
 * Purpose:   DataTreeSample tutorial (application main frame)
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-10-28
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "wx_pch.h"

#ifdef _DEBUG_MSVC
#define new DEBUG_NEW
#endif

#include "MainFrame.h"
#include "TreeSample.h"

MainFrame::MainFrame( wxWindow* parent )
: _MainFrame( parent )
{
    #ifdef __WXMSW__
	SetIcon(wxICON(amain));
	#endif

	// initialize root node
	CreateTreeRoot();

	// build the tree control content
	BuildTreeFromData();
}

void MainFrame::CreateTreeRoot()
{
	// initialize root node
	treeCtrl->AddRoot(wxT("Root"));
	// Every tree control's item holds user data container encapsulating an ID of the corespondent serializable
	// data object. Note that the root data object is created by the serializer and shouldn't be manipulated
	// direcly (but it is possible as well).
	treeCtrl->SetItemData(treeCtrl->GetRootItem(), new TreeData(wxGetApp().m_XmlIO.GetRootItem()->GetId()));
}

void MainFrame::BuildTreeFromData()
{
	// create a content of the tree control in accordance to the structure of the data objects stored in
	// the serializer
	_BuildChildren(treeCtrl->GetRootItem());
	treeCtrl->ExpandAll();
}

void MainFrame::_BuildChildren(wxTreeItemId parent)
{
	// get ID of a serializable object associated with relevant tree node
	long nId = ((TreeData*)treeCtrl->GetItemData(parent))->m_nDataId;
	// get pointer to serializable object
	xsSerializable *parentObject = wxGetApp().m_XmlIO.GetItem(nId);
	if(parentObject)
	{
	    wxTreeItemId treeId;
	    SerializableObject *pChild;
		// iterate through all the parent data object's items and create relevant tree control nodes
		SerializableList::compatibility_iterator node = parentObject->GetFirstChildNode();
		while(node)
		{
		    pChild = (SerializableObject*)node->GetData();
		    // create new tree control node
		    treeId = treeCtrl->AppendItem(parent, pChild->m_sTreeItemName, -1, -1, new TreeData(pChild->GetId()));
			// create next tree level items recursively
			_BuildChildren(treeId);

			node = node->GetNext();
		}
	}
}

void MainFrame::OnAddClick( wxCommandEvent& event )
{
	// This function creates new tree node as a child of currently selected tree node.
	// Hierarchical structure of data objects stored in the serializer copies the tree control's
	// nodes structure.

	// get selected tree node
	wxTreeItemId treeNode = treeCtrl->GetSelection();
	// create new serializable data object
	SerializableObject *data = new SerializableObject();
	// add new data object to the serializer at a position given by the tree structure
	// (selected tree node is a parent)
	long parentId = ((TreeData*)treeCtrl->GetItemData(treeNode))->m_nDataId;
	wxGetApp().m_XmlIO.AddItem(parentId, data);
	// create new tree node containing an ID of new data object and set it visible
	treeCtrl->EnsureVisible(treeCtrl->AppendItem(treeNode, data->m_sTreeItemName, -1, -1, new TreeData(data->GetId())));
}

void MainFrame::OnUpdateAdd( wxUpdateUIEvent& event )
{
	event.Enable(treeCtrl->GetSelection().IsOk());
}

void MainFrame::OnRemoveClick( wxCommandEvent& event )
{
	// This function removes currently selected tree node. Also relevant data object
	// in the serializer is removed.

	// get selected tree node
	wxTreeItemId treeNode = treeCtrl->GetSelection();
	if(treeNode == treeCtrl->GetRootItem())
	{
		// remove all data objects from the serializer
		wxGetApp().m_XmlIO.RemoveAll();
		// clear the tree control's content
		treeCtrl->DeleteAllItems();

		// re-create tree root
		CreateTreeRoot();
	}
	else
	{
	    // get ID of a serializable object associated with relevant tree node
        long nId = ((TreeData*)treeCtrl->GetItemData(treeNode))->m_nDataId;
		// remove the data object stored in the serializer
		wxGetApp().m_XmlIO.RemoveItem(nId);
		// remove tree control node
		treeCtrl->Delete(treeNode);
	}
}

void MainFrame::OnUpdateRemove( wxUpdateUIEvent& event )
{
	event.Enable(treeCtrl->GetSelection().IsOk());
}

void MainFrame::OnOk( wxCommandEvent& event )
{
	Destroy();
}

void MainFrame::OnAboutClick( wxCommandEvent& event )
{
	wxMessageBox(wxT("The sample demostrates how to create and use a persistent \
hierarchical data structure. The serializer object (instance \
of the wxXmlSerializer class) holds data objects (instances of \
xsSerializable class ) reflecting a current structure of the \
tree contols' items. The serializer's content is stored in a \
configuration file so the tree control's content can be reconstructed \
from this information at the appliaction's start up."), wxT("About TreeSample ..."));
}
