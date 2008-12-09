#ifndef _WX_INDEX_TREE_ITEM_DATA_H
#define _WX_INDEX_TREE_ITEM_DATA_H

#include <wx/wx.h>
#include <wx/treectrl.h>

/// \brief Data object for binding elements of array oe list by index
class wxIndexTreeItemData : public wxTreeItemData
{
public:
	/// \brief Constructor
	/// \param index index of an object in array or list
	wxIndexTreeItemData(size_t index);

	/// \brief Returns the index of associated object
	size_t GetIndex();
private:
	/// Index of associated object
	size_t m_Index;
};

#endif
