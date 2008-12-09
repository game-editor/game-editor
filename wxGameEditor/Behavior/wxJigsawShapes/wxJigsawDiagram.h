#ifndef _WX_JIGSAW_DIAGRAM_H
#define _WX_JIGSAW_DIAGRAM_H

#include <wx/wx.h>
#include "wxJigsawShapeGroup.h"

/// \brief Jigsaw Diagram
class wxJigsawDiagram : public xsSerializable
{
	DECLARE_DYNAMIC_CLASS(wxJigsawDiagram)
	/// \brief List of groups of shapes
	wxJigsawShapeGroupList m_Groups;
public:
	/// \brief Constructor
	wxJigsawDiagram();
	/// \brief Copy constructor
	wxJigsawDiagram(const wxJigsawDiagram & diagram);

	/// \brief Returns the list of groups
	wxJigsawShapeGroupList & GetGroups();
	/// \brief Returns the group which contains specified shape
	/// \param shape Jigsaw shape object
	wxJigsawShapeGroup * GetShapeGroup(wxJigsawShape * shape);
	/// \brief Returns the size of diagram
	wxSize GetSize();
	/// \brief Updates the layout (moves all shapes to appropriate places)
	void UpdateLayout(double scale);
	void UpdateParents();

	/// \brief Clones current object
	virtual xsSerializable * Clone();
	/// \brief Initializes serialization (binds class members to xsSerializable properties
	void InitSerialization();
};

#endif
