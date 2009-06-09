#ifndef _WX_JIGSAW_SHAPE_H
#define _WX_JIGSAW_SHAPE_H

#include <wx/wx.h>
#include <wx/list.h>
#include "wxJigsawInputParameter.h"
#include "wxJigsawHotSpot.h"
#include "../CodeEmitter.h"

/// \brief Converts integer value to wxJigsawShapeStyle
wxJigsawShapeStyle IntToJigsawShapeStyle(int value);
/// \brief Converts wxJigsawShapeStyle to string
wxString JigsawShapeStyleToStr(wxJigsawShapeStyle value);

class wxJigsawShape;

/// Declaration of list of shapes
WX_DECLARE_LIST(wxJigsawShape, wxJigsawShapeList);

/// Jigsaw Shape
class wxJigsawShape : public xsSerializable
{
public:
	/// \brief Hit test result
	enum wxJigsawShapeHitTest
	{
		/// None
		wxJS_HITTEST_NONE,
		/// Moving area
		wxJS_HITTEST_MOVINGAREA,
		/// Editing area (for slots)
		wxJS_HITTEST_EDITING_AREA,
		/// Bump
		wxJS_HITTEST_BUMP,
		/// Notch
		wxJS_HITETST_NOTCH,
		/// C-Shape inner bump
		wxJS_HITTEST_C_SHAPE_BUMP,
		/// C-Shape inner notch
		wxJS_HITTEST_C_SHAPE_NOTCH,
		/// Slot
		wxJS_HITTEST_SLOT,
		/// Bump docking area
		wxJS_HITTEST_BUMP_DOCKING_AREA,
		/// Notch docking area
		wxJS_HITTEST_NOTCH_DOCKING_AREA,
		/// Inner notch docking area
		wxJS_HITTEST_INNER_NOTCH_DOCKING_AREA,
		/// Inner bump docking area
		wxJS_HITTEST_INNER_BUMP_DOCKING_AREA,
		wxJS_HITTEST_CHILD_INSERTION_AREA
	};

	/// \brief Contains the information gathered by wxJigsawShape::HitTest method
	class wxJigsawShapeHitTestInfo
	{
		/// \brief Offset of the specified point on shape
		wxSize m_Offset;
		/// \brief Index of input parameter under specified point
		int m_InputParameterIndex;
		int m_ChildIndex;
		wxJigsawShape::wxJigsawShapeHitTest m_Result;
		wxJigsawShape * m_Shape;
		wxJigsawShapeStyle m_InputParameterStyle;
		wxPoint m_MousePosition;
	public:
		/// \brief Constructor
		wxJigsawShapeHitTestInfo() : 
			m_Offset(0,0), 
			m_InputParameterIndex(wxID_ANY),
			m_InputParameterStyle(wxJigsawShapeStyle::wxJS_TYPE_NONE),
			m_ChildIndex(wxID_ANY),
			m_Result(wxJS_HITTEST_NONE),
			m_Shape(NULL),
			m_MousePosition(-1,-1){}

		void Clear()
		{
			m_Offset = wxSize(0,0);
			m_InputParameterIndex = wxID_ANY;
			m_InputParameterStyle = wxJigsawShapeStyle::wxJS_TYPE_NONE;
			m_ChildIndex = wxID_ANY;
			m_Result = wxJS_HITTEST_NONE;
			m_Shape = NULL;
			m_MousePosition = wxPoint(-1,-1);
		}

		/// \brief Sets the offset of point on a shape
		/// \param value new value
		void SetOffset(const wxSize & value) {m_Offset = value;}
		/// \brief Returns the offset of point on a shape
		const wxSize & GetOffset() {return m_Offset;}

		/// \brief Sets the index of input parameter under specified point on a shape
		/// \param value new value
		void SetInputParameterIndex(int value) {m_InputParameterIndex = value;}
		/// \brief Returns the index of input parameter under specified point on a shape
		int GetInputParameterIndex() {return m_InputParameterIndex;}

		void SetInputParameterStyle(wxJigsawShapeStyle value) {m_InputParameterStyle = value;}
		
		wxJigsawShapeStyle GetInputParameterStyle() {return m_InputParameterStyle;}

		wxJigsawShape::wxJigsawShapeHitTest GetResult() {return m_Result;}
		void SetResult(wxJigsawShape::wxJigsawShapeHitTest value) {m_Result = value;}

		wxJigsawShape * GetShape() {return m_Shape;}
		void SetShape(wxJigsawShape * value) {m_Shape = value;}

		void SetMousePos(wxPoint &pos) {m_MousePosition = pos;}
		wxPoint GetMousePos() {return m_MousePosition;}

		int GetChildIndex() {return m_ChildIndex;}
		void SetChildIndex(int value) {m_ChildIndex = value;}
	};

	/// \brief Constructor
	/// \param name Name of a shape
	/// \param bitmapFileName shape's icon
	/// \param colour shape's colour
	wxJigsawShape(const wxString & name = wxEmptyString,
		const wxString & bitmapFileName = wxEmptyString,
		const wxColour & colour = *wxBLACK);

	/// \brief Copy constructor
	/// \param shape source object
	wxJigsawShape(const wxJigsawShape & shape);

	bool IsAtomic() {return m_Style != wxJigsawShapeStyle::wxJS_TYPE_NONE && m_InputParameters.GetCount() == 0 && !m_HasBump && !m_HasCShape && !m_HasNotch;}

	/// \brief Destructor
	virtual ~wxJigsawShape();
	
	/// \brief Draws a shape
	/// \param dc device context
	/// \param offset on device context (used for scrolling)
	void Draw(wxDC & dc, const wxSize & offset, double scale);

	/// \brief Returns the parent shape
	wxJigsawShape * GetParent();
	/// \brief Sets the parent shape
	/// \param value new value
	void SetParent(wxJigsawShape * value);

	/// \brief Returns top level parent of a shape
	wxJigsawShape * GetTopParent();

	/// \brief Returns level of the shape
	int GetLevel();

	/// \brief Returns level of the shape with the same color
	int GetLevelColor();

	/// \brief Returns shape's name
	const wxString & GetName() const;
	/// \brief Sets shape's name
	/// \param value new value
	void SetName(const wxString & value);

	const wxString & GetDescription() const;
	void SetDescription(const wxString & value);

	const wxString & GetValue() const;
	void SetValue(const wxString & value);
	void SetValue(const bool value);
	void SetValue(const double value);

	/// \brief Returns shape's icon
	const wxBitmap & GetBitmap() const;

	/// \brief Returns the file name of shape's icon
	const wxString & GetBitmapFileName() const;
	/// \brief Sets the file name of shape's icon
	/// \param value new value
	void SetBitmapFileName(const wxString & value);

	/// \brief Returns true if shape has a notch, otherwise returns false
	bool GetHasNotch() const;
	/// \brief Specifies if shape has a notch
	/// \param value new value
	void SetHasNotch(bool value);

	/// \brief Returns true if shape has a bump, otherwise returns false
	bool GetHasBump() const;
	/// \brief Specifies if shape has a bump
	/// \param value new value
	void SetHasBump(bool value);

	/// \brief Returns true if shape has a C-shape, otherwise returns false
	bool GetHasCShape() const;
	/// \brief Specifies if shape has a C-shape
	/// \param value new value
	void SetHasCShape(bool value);

	/// \brief Returns the style of a shape
	wxJigsawShapeStyle GetStyle() const;
	/// \brief Sets the style of a shape
	/// \param value new value
	void SetStyle(wxJigsawShapeStyle value);

	/// \brief Returns the colour of a shape
	const wxColour & GetColour();
	/// \brief Sets the colour of a shape
	/// \param value new value
	void SetColour(const wxColour & value);

	/// \brief Returns the minimal size of a shape
	const wxSize & GetMinSize();
	/// \brief Returns the minimal size of a shape
	/// \param w buffer which receives the width of a shape
	/// \param h buffer which receives the height of a shape
	void GetMinSize(wxCoord * w, wxCoord * h);
	/// \brief Sets the minimal size of a shape
	/// \param w new width
	/// \param h new height
	void SetMinSize(wxCoord w, wxCoord h);
	/// \brief Sets the minimal size of a shape
	/// \param value new value
	void SetMinSize(const wxSize & value);

	/// \brief Returns the size of a shape
	wxSize GetSize();
	/// \brief Returns the size of shape
	/// \param dc device context which should be used for retrieving sizes of text labels
	wxSize GetSize(wxDC & dc, double scale);
	/// Returns the size of shape
	/// \param dc device context which should be used for retrieving sizes of text labels
	/// \param w buffer where width of shape should be stored
	/// \param h buffer where height of shape should be stored
	void GetSize(wxDC & dc, wxCoord * w, wxCoord * h, double scale);

	/// \brief Returns the position of a shape
	const wxPoint & GetPosition();
	/// \brief Returns the position of a shape
	/// \param x buffer where x coordinate of shape's position should be stored
	/// \param y buffer where y coordinate of shape's position should be stored
	void GetPosition(int * x, int * y);
	/// \brief Sets the position of a shape
	/// \param value new value
	void SetPosition(const wxPoint & value);
	/// \brief Sets the position of a shape
	/// \param x new x coordinate of a shape
	/// \param y new y coordinate of a shape
	void SetPosition(int x, int y);

	/// \brief Returns the offset (by x coordinate) of input parameters from the left side of a shape
	int GetParametersOffset(double scale);
	int GetParametersOffset();
	/// \brief Returns the size of shape's header
	//wxSize GetHeaderSize();
	/// Returns the size of shape's header
	/// \param dc device context which should be used for retrieving sizes of text labels
	wxSize GetHeaderSize(wxDC & dc, double scale);
	/// Returns the size of shape's header
	/// \param dc device context which should be used for retrieving sizes of text labels
	/// \param w pointer to buffer where width of shape's header should be stored
	/// \param h pointer to buffer where height of shape's header should be stored
	void GetHeaderSize(wxDC & dc, wxCoord * w, wxCoord * h, double scale);

	/// \brief Returns the size of all input parameters
	wxSize GetInputParametersSize();
	/// \brief Returns the size of input parameters block
	/// \param dc device context which should be used for retrieving sizes of text labels
	wxSize GetInputParametersSize(wxDC & dc, double scale);

	/// \brief returns the size of shape's children
	/// \param dc device context for calculating label sizes
	wxSize GetChildrenSize(wxDC & dc, double scale);
	/// \brief returns the size of shape's children
	/// \param dc device context for calculating label sizes
	/// \param w pointer to buffer where width of area which contains shape's children should be stored
	/// \param h pointer to buffer where height of area which contains shape's children should be stored
	void GetChildrenSize(wxDC & dc, int * w, int * h, double scale);
	
	/// \brief Returns the array of input parameters
	wxJigsawInputParameters & GetInputParameters();

	/// \brief Returns the list of children
	wxJigsawShapeList & GetChildren();

	/// \brief Clones current object
	virtual xsSerializable * Clone();

	/// \brief Indicates which part of shape contains the specified point
	/// \param pos point
	/// \param info receives a clue about the area which contains a point
	wxJigsawShapeHitTest HitTest(wxDC & dc, wxPoint pos, 
		wxJigsawShapeHitTestInfo & info, double scale, bool bDebug = false);

	/// \brief Preforms dragging of a shape
	/// \param dragAction dragging action
	/// \param currentPos current position of mouse pointer (in logical coordinates)
	/// \param offset offset of mouse pointer from the top left corner of a shape
	/// \param clue contains the result of dragging (not used for now)
	bool PerformDragging(wxJigsawShapeHitTest dragAction, const wxPoint & currentPos,
		const wxSize & offset, int & clue);

	/// \brief Returns the default size of connector (bump/notch)
	static const wxSize & GetConnectorSize();
	/// \brief Sets the default size of connector (bump/notch)
	/// \param value new value
	static void SetConnectorSize(const wxSize & value);

	/// \brief Updates the layout (moves children to their appropriate places)
	void Layout(wxDC & dc, double scale, bool reCreateHotSpots = false);

	wxJigsawHotSpotArray & GetHotSpots();

	void ReCreateHotSpots(wxDC & dc, wxJigsawHotSpotArray & hotSpots, double scale);

	void RequestSizeRecalculation();
	void RecalculateLabelSizes(wxDC & dc);

	void UpdateParents(wxJigsawShape * thisShapeParent);

private:
	DECLARE_DYNAMIC_CLASS(wxJigsawShape);
	/// \brief Parent shape
	wxJigsawShape * m_Parent;
	/// \brief Name of a shape object. It is also used as label when displaying a shape
	wxString m_Name;

	/// \brief Description of a shape object.
	wxString m_Description;

	/// \brief 
	CodeEmitter m_Emit;

	/// \brief 
	CodeEmitter m_EmitOpen;

	/// \brief 
	CodeEmitter m_EmitIntra;

	/// \brief 
	CodeEmitter m_EmitClose;

	/// \brief File name of shape icon
	wxString m_BitmapFileName;
	/// \brief Shape colour
	wxColour m_Colour;
	/// \brief Shape style. Shapes with different styles will have different form when displayed
	/// We can't use smart_enum directly because wxXmlSerializer does not support smart enum
	int m_Style;
	/// \brief Specifies if shape has a notch
	bool m_HasNotch;
	/// \brief Specifies if shape has a bump
	bool m_HasBump;
	/// \brief Specifies if shape has a C-shape
	bool m_HasCShape;
	/// \brief Minimal size of a shape
	wxSize m_MinSize;
	/// \brief Position of a shape on canvas
	wxPoint m_Position;
	/// \brief Array of input parameters
	wxJigsawInputParameters m_InputParameters;
	/// \brief List of children. Only those shapes which have C-shape can have children
	wxJigsawShapeList m_Children;
	/// \brief Shape icon
	wxBitmap m_Bitmap;
	/// \brief Flag which indicates that label size should be recalculated during next call of 
	/// wxJigsawShape::Draw method
	bool m_NeedCalcLabelSize;
	/// \brief List of hotspots
	wxJigsawHotSpotArray m_HotSpots;
	/// \brief Label size
	wxSize m_LabelSize;

	wxSize m_HeaderSize;

	wxFont labelFont;

	int m_ParametersOffset;

	wxSize m_Size;
	/// \brief Thickness of C-Shape
	static int CShapeThickness;
	/// \brief Default connector size
	static wxSize ConnectorSize;
	/// \brief Specifies how much free space should be on top, bottom, left and right side of each shape
	static wxSize ShapeLabelOffset;
	/// \brief Specifies how much free space should be between elements 
	/// of shape header (bitmap, label, input parameters
	static int HeaderElementSpacing;
	/// \brief Default size of empty C-shape
	static int ChildrenSpacing;
	static int HotSpotHeight;

	/// \brief Draws the whole shape's background
	/// \param dc device context
	/// \param pos position of top left corner
	/// \param headerSize size of shape's header
	/// \param size size of a shape
	wxColour DrawBackground(wxDC & dc, const wxPoint & pos, 
		const wxSize & headerSize, const wxSize & size, double scale);

	/// \brief Draws shape's header
	/// \param dc Device context to draw a header
	/// \param pos position of top left corner
	/// \param size size of a shape's header
	static void DrawShapeHeader(wxDC & dc, const wxPoint & pos, 
		const wxSize & size, const wxJigsawShapeStyle style);

	/// /// \brief Copies children of a shape to another shape
	/// \param dest destination
	/// \param src source
	static void CopyChildren(wxJigsawShape & dest, const wxJigsawShape & src);

	/// Copies parameters of a shape to another shape
	/// \param dest destination
	/// \param src source
	static void CopyInputParameters(wxJigsawShape & dest, const wxJigsawShape & src);
	
	/// \brief Initializes serialization (binds class members to xsSerializable properties
	void InitSerialization();

	/// \brief Reloads the bitmap
	void ReLoadBitmap();

	/// \brief Returns the top left corner of input parameters
	/// \param dc device context for calculating label sizes
	wxPoint GetinputParametersPosition(wxDC & dc, double scale);
	//wxPoint GetinputParametersPosition();

};

#endif
