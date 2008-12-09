#include "wxIndexTreeItemData.h"

wxIndexTreeItemData::wxIndexTreeItemData(size_t index) 
: m_Index(index) 
{
}

size_t wxIndexTreeItemData::GetIndex() 
{
	return m_Index;
}
