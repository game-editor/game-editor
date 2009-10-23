/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

If GPL v3 does not work for you, we offer a "non-GPL" license 
More info at http://game-editor.com/License
Be a Game Editor developer: http://game-editor.com/Sharing_Software_Revenues_in_Open_Source

***************************************************************************/


#ifndef _WX_JIGSAW_INPUT_PARAMETER_H
#define _WX_JIGSAW_INPUT_PARAMETER_H

#include <wx/wx.h>
#include <wx/list.h>
#include <wx/wxxmlserializer/XmlSerializer.h>
#include <smart_enum.h>
#include "../CodeEmitter.h"

/// Source class for shape's style (value provider)
class wxJigsawShapeStyle_def
{
public:
	/// Values of shape's style
	enum type
	{
		/// No style (default)
		wxJS_TYPE_NONE,
		/// Numeric
		wxJS_TYPE_NUMERIC,
		/// Boolean
		wxJS_TYPE_BOOLEAN,
		/// String
		wxJS_TYPE_STRING,
		/// Default style (alias to wxJS_TYPE_NONE)
		wxJS_TYPE_DEFAULT = wxJS_TYPE_NONE
	};
};

/// Definition of shape's style (using C++ smart enum)
typedef smart_enum<wxJigsawShapeStyle_def> wxJigsawShapeStyle;

class wxJigsawShape;
class wxJigsawHotSpotArray;

/// \brief Input parameter
class wxJigsawInputParameter : public xsSerializable
{
	DECLARE_DYNAMIC_CLASS(wxJigsawInputParameter)
	/// \brief Label
	wxString m_Label;
	/// \brief 
	CodeEmitter m_Emit;
	/// \brief Style
	int m_Style;
	/// \brief Shape associated to parameter
	wxJigsawShape * m_Shape;
	/// \brief Flag which indicates that label size should be recalculated during next call of 
	/// wxJigsawInputParameter::Draw method
	bool m_NeedCalcLabelSize;
	/// \brief Label size
	wxSize m_LabelSize;
public:
	/// \brief Hit test result
	enum wxJigsawInputParameterHitTest
	{
		/// None
		wxJSP_HITTEST_NONE,
		/// Label
		wxJSP_HITETST_LABEL,
		/// Slot (for empty parameter)
		wxJSP_HITTEST_SLOT,
		/// Shape (if m_Shape is not NULL)
		wxJSP_HITTEST_SHAPE,
	};

	/// Empty space between parameters and parts of each parameter
	static const wxCoord ParameterSpacing;
	/// \brief Constructor
	wxJigsawInputParameter();
	/// \brief Constructor
	/// \param label input parameter label
	/// \param style input parameter style
	/// \param shape associated with input parameter
	wxJigsawInputParameter(const wxString & label, wxJigsawShapeStyle style, wxJigsawShape * shape);
	/// \brief Copy constructor
	wxJigsawInputParameter(const wxJigsawInputParameter & param);
	~wxJigsawInputParameter();
	
	/// \brief Returns the label of input parameter
	const wxString & GetLabel();
	/// \brief Sets the label of input parameter
	/// \param value new value
	void SetLabel(const wxString & value);

	/// \brief Returns the style of inout parameter
	wxJigsawShapeStyle GetStyle();
	/// \brief Sets the style of input parameter
	/// \param value new value
	void SetStyle(wxJigsawShapeStyle value);

	/// \brief Returns the size of input parameter
	/// \param dc device context which should be used for calculating the size of label
	wxSize GetSize(wxDC & dc, double scale);
	/// \brief Returns the size of input parameter
	/// \param dc device context which should be used for calculating the size of label
	/// \param w buffer which receives the width of input parameter
	/// \param h buffer which receives the height of input parameter
	void GetSize(wxDC & dc, wxCoord * w, wxCoord * h, double scale);
	/// \brief Returns the size of input parameter
	/// \param w buffer which receives the width of input parameter
	/// \param h buffer which receives the height of input parameter
	void GetSize(wxCoord * w, wxCoord * h);
	/// \brief Returns the size of input parameter
	wxSize GetSize();

	/// \brief Returns the shape associated with input parameter
	wxJigsawShape * GetShape();
	void SetShape(wxJigsawShape * value);

	/// \brief Returns the default size of empty input parameter (size of slot)
	static wxSize GetDefaultParameterSize();

	/// \brief Draws the input parameter
	/// \param dc device context
	/// \param pos position of input parameter (passed by its parent)
	/// \param offset offset on the device context (used for scrolling)
	void Draw(wxDC & dc, const wxPoint & pos, const wxSize & offset, double scale, wxColour color);
	/// \brief Returns the code of area under a given point
	/// \param pos point (in logical coordinates)
	/// \param paramRect rectangle of input parameter (passed by its parent)
	wxJigsawInputParameterHitTest HitTest(const wxPoint & pos, const wxRect & paramRect, bool bDebug = false);

	/// \brief Clones current object
	virtual xsSerializable * Clone();
	/// \brief Initializes serialization (binds class members to xsSerializable properties
	void InitSerialization();

	void ReCreateHotSpots(wxDC & dc, wxJigsawHotSpotArray & hotSpots, double scale);
	int GetSlotOffset(double scale);

	void RequestSizeRecalculation();
	void RecalculateLabelSizes(wxDC & dc);
};

/// List of input parameters
WX_DECLARE_LIST(wxJigsawInputParameter, wxJigsawInputParameters);

#endif
