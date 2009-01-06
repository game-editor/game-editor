#ifndef _WX_JIGSAW_SHAPE_GROUP_H
#define _WX_JIGSAW_SHAPE_GROUP_H

#include <wx/wx.h>
#include <wx/gdicmn.h>
#include <wx/list.h>
#include "wxJigsawShape.h"

/// \brief Group of shapes
class wxJigsawShapeGroup : public xsSerializable
{
	DECLARE_DYNAMIC_CLASS(wxJigsawShapeGroup)
	/// \brief Position
	wxRealPoint m_Position;
	/// \brief Shapes
	wxJigsawShapeList m_Shapes;
	/// \brief Updates the layout (moves shapes to their appropriate places)
	void Layout(wxDC & dc, double scale);
public:
	/// \brief Constructor
	wxJigsawShapeGroup();
	/// \brief Copy constructor
	wxJigsawShapeGroup(const wxJigsawShapeGroup & group);
	/// \brief returns the list of shapes
	wxJigsawShapeList & GetShapes();
	
	/// \brief Sets the position
	/// \brief value new value
	void SetPosition(wxDC & dc, const wxRealPoint & value, double scale);
	/// \brief Sets the position
	/// \brief value new value
	void SetPosition(wxDC & dc, const wxPoint & value, double scale);

	/// \brief Returns the position
	const wxRealPoint & GetPosition();

	/// \brief Preforms dragging
	/// \param destPoint current position of mouse pointer (in logical coordinates)
	/// \param offset offset of mouse pointer from the top left corner of the first shape
	bool Drag(wxDC & dc, const wxPoint & destPoint, const wxSize & offset, double scale);

	/// \brief Returns the size of group
	wxSize GetSize();
	/// \brief Returns the rectangle of group
	wxRect GetRect();

	/// \brief Draws the group
	/// \param dc device context
	/// \param offset on device context (used for scrolling)
	void Draw(wxDC & dc, const wxSize & offset, double scale);

	/// \brief Returns the shape under specified point (in logical coordinates)
	/// \param pos point
	wxJigsawShape * GetShapeFromPoint(wxDC & dc, const wxPoint & pos,
		wxJigsawShape::wxJigsawShapeHitTestInfo & info, double scale);

	/// \brief Clones current object
	virtual xsSerializable * Clone();

	/// \brief Initializes serialization (binds class members to xsSerializable properties
	void InitSerialization();

	/// \brief Creates the group from list of shapes. Extracts the shapes starting 
	/// from specified position
	/// \param shapes list of shapes
	/// \param startIndex index of first shape fro new group
	static wxJigsawShapeGroup * CreateFromShapeList(wxDC & dc, wxJigsawShapeList & shapes, 
		int startIndex, double scale);

	void Detach(wxJigsawShape * shape);

	void ReCreateHotSpots(wxDC & dc, wxJigsawHotSpotArray & hotSpots, double scale);

	void RequestSizeRecalculation();

	void UpdateParents();

	bool IsSingleShapeGroup();
	bool IsInputParameterShapeGroup();
	wxJigsawShape * GetFirstShape();
};

/// \brief List of groups
WX_DECLARE_LIST(wxJigsawShapeGroup, wxJigsawShapeGroupList);

#endif
