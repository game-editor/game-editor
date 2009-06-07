#include "wxJigsawInputParameter.h"
#include "wxJigsawShape.h"
#include "wxJigsawInputParameterPropertyIO.h"
#include "wxJigsawShapePropertyIO.h"
#include <wx/listimpl.cpp>

IMPLEMENT_DYNAMIC_CLASS(wxJigsawInputParameter, xsSerializable)

WX_DEFINE_LIST(wxJigsawInputParameters);

const wxCoord wxJigsawInputParameter::ParameterSpacing = 3;

wxJigsawInputParameter::wxJigsawInputParameter()
: m_Label(wxEmptyString), m_Style(wxJigsawShapeStyle::wxJS_TYPE_DEFAULT), 
m_Shape(NULL), m_NeedCalcLabelSize(true), m_LabelSize(0,0)
{
	InitSerialization();
}

wxJigsawInputParameter::wxJigsawInputParameter(const wxString & label,
	wxJigsawShapeStyle style, wxJigsawShape * shape)
: m_Label(label), m_Style(style), m_Shape(shape), m_NeedCalcLabelSize(true),
m_LabelSize(0,0)
{
	InitSerialization();
}

wxJigsawInputParameter::wxJigsawInputParameter(const wxJigsawInputParameter & param)
: xsSerializable(param),
m_Label(param.m_Label), m_Style(param.m_Style), 
m_Shape(param.m_Shape), m_NeedCalcLabelSize(param.m_NeedCalcLabelSize), 
m_LabelSize(param.m_LabelSize),
m_Emit(param.m_Emit)
{
	InitSerialization();
}

wxJigsawInputParameter::~wxJigsawInputParameter()
{
	wxDELETE(m_Shape);
}

xsSerializable * wxJigsawInputParameter::Clone()
{
	return new wxJigsawInputParameter(*this);
}

void wxJigsawInputParameter::InitSerialization()
{
	XS_SERIALIZE(m_Label, wxT("label"));
	XS_SERIALIZE(m_Style, wxT("style"));
	XS_SERIALIZE_CODEEMITTER(m_Emit, wxT("emit"));
	XS_SERIALIZE_JIGSAWSHAPE(m_Shape, wxT("shape"));
}

const wxString & wxJigsawInputParameter::GetLabel()
{
	return m_Label;
}

void wxJigsawInputParameter::SetLabel(const wxString & value)
{
	m_Label = value;
	m_NeedCalcLabelSize = true;
}

wxJigsawShapeStyle wxJigsawInputParameter::GetStyle()
{
	return IntToJigsawShapeStyle(m_Style);
}

void wxJigsawInputParameter::SetStyle(wxJigsawShapeStyle value)
{
	m_Style = value;
}

wxSize wxJigsawInputParameter::GetSize(wxDC & dc, double scale)
{
	wxSize res;
	GetSize(dc, &res.x, &res.y, scale);
	return res;
}

wxSize wxJigsawInputParameter::GetSize()
{
	wxSize res(0,0);
	GetSize(&res.x, &res.y);
	return res;
}

void wxJigsawInputParameter::GetSize(wxCoord * w, wxCoord * h)
{
	wxSize shapeSize;
	if(m_Shape)
	{
		shapeSize = m_Shape->GetSize();
	}
	else
	{
		shapeSize = wxJigsawInputParameter::GetDefaultParameterSize();
	}
	if(w)
	{
		*w = m_LabelSize.GetWidth() + wxJigsawInputParameter::ParameterSpacing + shapeSize.GetWidth();
	}
	if(h)
	{
		*h = wxMax(m_LabelSize.GetHeight(), shapeSize.GetHeight());
	}
}

void wxJigsawInputParameter::RequestSizeRecalculation()
{
	m_NeedCalcLabelSize = true;
	if(m_Shape)
	{
		m_Shape->RequestSizeRecalculation();
	}
}

void wxJigsawInputParameter::GetSize(wxDC & dc, wxCoord * w, wxCoord * h, double scale)
{
	wxCoord shapeWidth(0), shapeHeight(0);
	if(m_NeedCalcLabelSize)
	{
		dc.GetTextExtent(m_Label, &m_LabelSize.x, &m_LabelSize.y, 0, 0, (wxFont *)&dc.GetFont());
		m_NeedCalcLabelSize = false;
	}
	if(m_Shape)
	{
		m_Shape->GetSize(dc, &shapeWidth, &shapeHeight, scale);
	}
	else
	{
		wxSize defaultSize = wxJigsawInputParameter::GetDefaultParameterSize();
		shapeWidth = defaultSize.GetWidth()*scale;
		shapeHeight = defaultSize.GetHeight()*scale;
	}
	if(w)
	{
		*w = m_LabelSize.GetWidth() + wxJigsawInputParameter::ParameterSpacing*scale + shapeWidth;
	}
	if(h)
	{
		*h = wxMax(m_LabelSize.GetHeight(), shapeHeight);
	}
}

wxJigsawShape * wxJigsawInputParameter::GetShape()
{
	return m_Shape;
}

void wxJigsawInputParameter::SetShape(wxJigsawShape * value)
{
	m_Shape = value;
}

wxSize wxJigsawInputParameter::GetDefaultParameterSize()
{
	return wxSize(30, 10);
}

void wxJigsawInputParameter::Draw(wxDC & dc, const wxPoint & pos, const wxSize & offset, double scale, wxColour color)
{
	wxPoint realPosition(
		pos.x + offset.GetWidth(), 
		pos.y /*+ offset.GetHeight()*/);
	/*wxLogTrace(wxTraceMask(), _("ParameterPos = %i,%i; Offset=%i, %i"),
		realPosition.x, realPosition.y, offset.x, offset.y);*/
	wxSize labelSize;
	wxSize size = GetSize(dc, scale);
	dc.GetTextExtent(m_Label, &labelSize.x, &labelSize.y, 0, 0, (wxFont *)&dc.GetFont());

	//Draw the text with bevel
	wxRect rectText(realPosition, size);
	rectText.x++;
	rectText.y++;
	dc.SetTextForeground(*wxBLACK);
	dc.DrawLabel(m_Label, rectText, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);

	rectText.x--;
	rectText.y--;
	dc.SetTextForeground(*wxWHITE);
	dc.DrawLabel(m_Label, rectText, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);

	
	wxPoint shapePos;
	wxSize shapeSize;
	if(m_Shape)
	{
		shapeSize = m_Shape->GetSize(dc, scale);
		shapePos = wxPoint(
			pos.x + labelSize.x + wxJigsawInputParameter::ParameterSpacing*scale, 
			pos.y + (size.GetHeight()-shapeSize.GetHeight())/2);
		m_Shape->SetPosition(shapePos.x, 
			shapePos.y + (size.GetHeight()-shapeSize.GetHeight())*scale/2 - offset.GetHeight());
		m_Shape->Draw(dc, offset, scale);
	}
	else
	{
		dc.SetPen(*wxBLACK_PEN);

		wxBrush paramBrush(wxColour(color.Red()*.9,color.Green()*.9,color.Blue()*.9)); 
		dc.SetBrush(paramBrush);
		//dc.SetBrush(*wxWHITE_BRUSH);

		shapeSize = wxJigsawInputParameter::GetDefaultParameterSize();
		shapeSize.x *= scale;
		shapeSize.y *= scale;
		shapePos = wxPoint(
			realPosition.x + labelSize.x + wxJigsawInputParameter::ParameterSpacing*scale, 
			realPosition.y + (size.GetHeight()-shapeSize.GetHeight())/2);
		wxJigsawShape::DrawShapeHeader(dc, shapePos, 
			shapeSize, GetStyle());
	}
	//dc.SetBrush(*wxTRANSPARENT_BRUSH);
	//dc.SetPen(*wxBLACK_PEN);
	//dc.DrawRectangle(realPosition, size);
}

int wxJigsawInputParameter::GetSlotOffset(double scale)
{
	return m_LabelSize.x + wxJigsawInputParameter::ParameterSpacing*scale;
}

wxJigsawInputParameter::wxJigsawInputParameterHitTest wxJigsawInputParameter::HitTest(
	const wxPoint & pos, const wxRect & paramRect, bool bDebug)
{
	do
	{
		if(!paramRect.Contains(pos)) break;
		if(pos.x - paramRect.GetLeft() < m_LabelSize.GetWidth())
		{
			return wxJSP_HITETST_LABEL;
		}
		if(m_Shape)
		{
			return wxJSP_HITTEST_SHAPE;
		}
		return wxJSP_HITTEST_SLOT;
	}
	while(false);
	return wxJSP_HITTEST_NONE;
}

void wxJigsawInputParameter::ReCreateHotSpots(wxDC & dc, wxJigsawHotSpotArray & hotSpots, double scale)
{
	if(m_Shape)
	{
		m_Shape->ReCreateHotSpots(dc, hotSpots, scale);
	}
}

void wxJigsawInputParameter::RecalculateLabelSizes(wxDC & dc)
{
	dc.GetTextExtent(m_Label, &m_LabelSize.x, &m_LabelSize.y, 0, 0, (wxFont *)&dc.GetFont());
	if(m_Shape)
	{
		m_Shape->RecalculateLabelSizes(dc);
	}
}