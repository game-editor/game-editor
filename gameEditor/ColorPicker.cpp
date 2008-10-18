// ColorPicker.cpp: implementation of the ColorPicker class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "ColorPicker.h"
#include "GameControl.h"
#include "SlowCanvas.h"
#include <math.h>

#define WIDTH	416
#define HEIGHT	322

enum
{
	FIX_RED = 0,
	FIX_GREEN,
	FIX_BLUE,

	FIX_HUE,
	FIX_SATURATION,
	FIX_BRIGHTNESS,

	SL_COLOR,
	LS_FIX_COLOR
};

#define min3(a, b, c) min(min(a, b), c)
#define max3(a, b, c) max(max(a, b), c)

extern double round(double x);


void RGB_to_HSV(double var_R, double var_G, double var_B, double &H, double &S, double &V) 
{
	//http://www.easyrgb.com/math.php?MATH=M20#text20

	var_R = round(var_R);
	var_G = round(var_G);
	var_B = round(var_B);

	double var_Min, var_Max, del_Max, del_R, del_G, del_B;
	
	var_R /= 255.0;                     //RGB values = From 0 to 255
	var_G /= 255.0;                     
	var_B /= 255.0;                     
	
	var_Min = min3( var_R, var_G, var_B );    //Min. value of RGB
	var_Max = max3( var_R, var_G, var_B );    //Max. value of RGB
	del_Max = var_Max - var_Min;             //Delta RGB value 
	
	V = var_Max;
	
	if ( del_Max == 0 )                     //This is a gray, no chroma...
	{
		H = 0;                                //HSV results = From 0 to 1
		S = 0;
	}
	else                                    //Chromatic data...
	{
		S = del_Max / var_Max;
		
		del_R = ( ( ( var_Max - var_R ) / 6.0 ) + ( del_Max / 2.0 ) ) / del_Max;
		del_G = ( ( ( var_Max - var_G ) / 6.0 ) + ( del_Max / 2.0 ) ) / del_Max;
		del_B = ( ( ( var_Max - var_B ) / 6.0 ) + ( del_Max / 2.0 ) ) / del_Max;
		
		if      ( var_R == var_Max ) H = del_B - del_G;
		else if ( var_G == var_Max ) H = ( 1 / 3.0 ) + del_R - del_B;
		else if ( var_B == var_Max ) H = ( 2 / 3.0 ) + del_G - del_R;
		
		if ( H < 0 ) H += 1;
		if ( H > 1 ) H -= 1;
	}

	//H = fabs(H);

	H *= 255;
	S *= 255;
	V *= 255;

	H = round(H);
	S = round(S);
	V = round(V);
}

KrRGBA HSV_to_RGB(double H, double S, double V, bool bRound = true) 
{ 
	//HSV values = From 0 to 1
	//RGB results = From 0 to 255
	//http://www.easyrgb.com/math.php?MATH=M21#text21

	if(bRound)
	{
		H = round(H);
		S = round(S);
		V = round(V);
	}
	
	H /= 255.0;
	S /= 255.0;
	V /= 255.0;  

	KrRGBA RGB;  
	
	
	if ( S == 0 )                       
	{
		RGB.Set(V * 255, V * 255, V * 255, 255);
	}
	else
	{
		double var_h, var_i, var_1, var_2, var_3, var_r, var_g, var_b;

		if(H == 1.0) H = 0.0;

		var_h = H * 6;
		var_i = int( var_h );             //Or ... var_i = floor( var_h )
		var_1 = V * ( 1 - S );
		var_2 = V * ( 1 - S * ( var_h - var_i ) );
		var_3 = V * ( 1 - S * ( 1 - ( var_h - var_i ) ) );
		
		if      ( var_i == 0 ) { var_r = V     ; var_g = var_3 ; var_b = var_1; }
		else if ( var_i == 1 ) { var_r = var_2 ; var_g = V     ; var_b = var_1; }
		else if ( var_i == 2 ) { var_r = var_1 ; var_g = V     ; var_b = var_3; }
		else if ( var_i == 3 ) { var_r = var_1 ; var_g = var_2 ; var_b = V;     }
		else if ( var_i == 4 ) { var_r = var_3 ; var_g = var_1 ; var_b = V;     }
		else                   { var_r = V     ; var_g = var_1 ; var_b = var_2; }


		if(bRound)
			RGB.Set(round(var_r * 255), round(var_g * 255), round(var_b * 255), 255);
		else
			RGB.Set(var_r * 255, var_g * 255, var_b * 255, 255);
	}
	
	return RGB; 
} 

ColorPointer::ColorPointer(Actor *parent)
	: Actor("PointerColorPickerCanvas", parent, CANVAS, 11, 11)
{
	//Create pointer
	KrRGBA *pixels = getCanvasResource()->Pixels(), black, white, color;
	black.Set(0, 0, 0, 255);
	white.Set(255, 255, 255, 255);
	color.Set(0, 0, 0, 0);

	int i;
	for(i = 0; i < Width(); i++)
	{
		for(int j = 0; j < Height(); j++)
		{
			pixels[ j*Width() + i ] = color;			
		}
	}

	for(i = 0; i < Width(); i++)
	{
		if(i != Width()/2)
		{
			color = (i % 2)?black:white;
			pixels[ (Height()/2)*Width() + i ] = color;					
		}
	}

	for(int j = 0; j < Height(); j++)
	{
		if(j != Height() /2)
		{
			color = (j % 2)?black:white;
			pixels[ j*Width() + (Width()/2) ] = color;			
		}
	}
}

bool ColorPointer::OnMouseButtonDown(int x, int y, Uint8 button)
{
	return false;
}

ColorCanvas::ColorCanvas(Actor *parent)
	: Actor("DlgColorPickerCanvas", parent, CANVAS, 256, 256)
{
	canPick = false;

	pointer = new ColorPointer(this);
}

ColorCanvas::~ColorCanvas()
{
	delete pointer;
}

bool ColorCanvas::OnMouseButtonDown(int x, int y, Uint8 button)
{
	KrVector2T< GlFixed > object;
	getImage()->ScreenToObject( x, y, &object );

	KrRGBA *pixels = getCanvasResource()->Pixels(), color;
	color = pixels[ object.y.ToInt()*Width() + object.x.ToInt() ];

	((ColorPicker *)getParent())->SetColor(color.c.red, color.c.green, color.c.blue);
	SetPointer(object.x.ToInt(), object.y.ToInt());

	canPick = true;

	getCanvasResource()->Refresh();
	return false;
}

void ColorCanvas::OnMouseButtonUp(int x, int y, Uint8 button)
{
	canPick = false;
}

void ColorCanvas::OnMouseMove(int x, int y)
{
	if(canPick)
	{
		KrVector2T< GlFixed > object;
		getImage()->ScreenToObject( x, y, &object );

		if(object.x >= 0 && object.y >= 0 && object.x < Width() && object.y < Height())
		{
			KrRGBA *pixels = getCanvasResource()->Pixels(), color;
			color = pixels[ object.y.ToInt()*Width() + object.x.ToInt() ];
			
			((ColorPicker *)getParent())->SetColor(color.c.red, color.c.green, color.c.blue);
			SetPointer(object.x.ToInt(), object.y.ToInt());

			getCanvasResource()->Refresh();
		}
	}
}

ColorSampleOfPicker::ColorSampleOfPicker(Actor *parent)
	: Actor(getRandomName(), parent, CANVAS, 104, 32)
{
	KrRGBA *pixels = getCanvasResource()->Pixels(), color;
	color.Set(255, 255, 255, 255);

	for(int i = 0; i < Width(); i++)
	{
		for(int j = 0; j < Height(); j++)
		{
			pixels[ j*Width() + i ] = color;			
		}
	}
}

bool ColorSampleOfPicker::OnMouseButtonDown(int x, int y, Uint8 button)
{
	KrColorTransform color(getImage()->CTransform());

	((ColorPicker *)getParent())->SetColor(color.b.c.red, color.b.c.green, color.b.c.blue);
	((ColorPicker *)getParent())->UpdateColorControls();
	
	return false;
}

ColorSample::ColorSample(Actor *parent, int x, int y, int w, int h, bool _bEditControls)
: Actor(getRandomName(), parent, CANVAS, w, _bEditControls?56:h)
{
	rEdit = gEdit = bEdit = NULL;
	setColor(255, 255, 255);

	SetPos(x, y);

	

	if(_bEditControls)
	{
		Text *text;
		text = AddText("  Red: ", Width() + 5, 0);
		rEdit = AddEditBox(text->Right(), text->Top(), 48); rEdit->SetNumeric(0, 255, 1);
		
		text = AddText("Green: ", text->Left(), rEdit->Down() + 2);
		gEdit = AddEditBox(text->Right(), text->Top(), 48); gEdit->SetNumeric(0, 255, 1);
		
		text = AddText(" Blue: ", text->Left(), gEdit->Down() + 2);
		bEdit = AddEditBox(text->Right(), text->Top(), 48); bEdit->SetNumeric(0, 255, 1);
	}
}

ColorSample::~ColorSample()
{
}

int ColorSample::Width()
{
	if(rEdit)
	{
		return rEdit->X() + rEdit->Width();
	}
	else
	{
		return Actor::Width();
	}
}


int ColorSample::Right()
{
	return getImage()->X() + Width();
}

gedString ColorSample::getRedText() 
{
	return rEdit?rEdit->GetText():(long)r;
}

gedString ColorSample::getGreenText() 
{
	return gEdit?gEdit->GetText():(long)g;
}

gedString ColorSample::getBlueText() 
{
	return bEdit?bEdit->GetText():(long)b;
}

void ColorSample::setColor(KrRGBA color) 
{
	KrRGBA *pixels = getCanvasResource()->Pixels();
	int width = Actor::Width(), height = Actor::Height();

	r = color.c.red; 
	g = color.c.green; 
	b = color.c.blue; 
	color.c.alpha = 255;

	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			pixels[ j*width + i ] = color;			
		}
	}

	getCanvasResource()->Refresh();

	if(rEdit)
	{
		rEdit->SetText(r);
		gEdit->SetText(g);
		bEdit->SetText(b);
	}
}

void ColorSample::setColor(int _r, int _g, int _b) 
{
	KrRGBA color;

	color.Set(_r, _g, _b, 255);
	setColor(color);
}

bool ColorSample::OnMouseButtonDown(int x, int y, Uint8 button)
{
	ColorPicker *colorPicker = new ColorPicker(r, g, b);
	if(colorPicker->Wait() == OK_BUTTON)
	{
		setColor(colorPicker->getRed(), colorPicker->getGreen(), colorPicker->getBlue());

		PostMessage(parent, COLOR_CHANGED);
	}
	
	delete colorPicker;
	return false;
}

void ColorSample::OnEditChar(EditBox *edit, SDLKey key, int line)
{
	gedString text(edit->GetText());
	text.trimLeft();
	text.trimRight();

	if(text.isNumber())
	{
		int value = atol(text.c_str());
		
		if(value < 0) value = 0;
		else if(value > 255) value = 255;
		
		
		if(edit == rEdit) r = value;
		else if(edit == gEdit) g = value;
		else if(edit == bEdit) b = value;
		
		setColor(r, g, b);
	}
}

void ColorSample::OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message)
{
	if(messageId == CLICK_ON_VARIABLE_SELECTOR || messageId == LIST_ACTOR_VARIABLES)
	{
		PostMessage(getParent(), messageId, message);
	}
}

/*
	//Usage:
	
	ColorPicker *colorPicker = new ColorPicker(123, 50, 200);
	if(colorPicker->Wait() == OK_BUTTON)
	{
		colorPicker->getRed();
		colorPicker->getGreen();
		colorPicker->getBlue();
	}
*/

ColorPicker::ColorPicker(int _r, int _g, int _b)
	: Panel("DlgColorPicker", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();

	Text *text;
	Button *button;

	//Title
	text = AddText("Color Picker", CENTER_TEXT, 5);
	int y = DrawHLine(text->Down() + 2);

	//Body
	canvas = new ColorCanvas(this);
	oldColor = new ColorSampleOfPicker(this);
	newColor = new ColorSampleOfPicker(this);

	canvas->SetPos(10, y + 2);	
	
	

	colorSlide = AddSlide(canvas->Right() + 5, canvas->Top(), 20, 256, SL_COLOR);
	colorSlide->SetPos(1.0);


	newColor->SetPos(colorSlide->Right() + 5, colorSlide->Top());
	oldColor->SetPos(newColor->Left(), newColor->Down());

	text = AddText("       Red: ", oldColor->Left(), oldColor->Down() + 16);
	rEdit = AddEditBox(text->Right(), text->Top(), 32); rEdit->SetNumeric(0, 255, 1);

	text = AddText("     Green: ", text->Left(), rEdit->Down() + 2);
	gEdit = AddEditBox(text->Right(), text->Top(), 32); gEdit->SetNumeric(0, 255, 1);

	text = AddText("      Blue: ", text->Left(), gEdit->Down() + 2);
	bEdit = AddEditBox(text->Right(), text->Top(), 32); bEdit->SetNumeric(0, 255, 1);


	text = AddText("       Hue: ", text->Left(), bEdit->Down() + 16);
	hEdit = AddEditBox(text->Right(), text->Top(), 32); hEdit->SetNumeric(0, 359, 1);

	text = AddText("Saturation: ", text->Left(), hEdit->Down() + 2);
	sEdit = AddEditBox(text->Right(), text->Top(), 32); sEdit->SetNumeric(0, 100, 1);

	text = AddText("Brightness: ", text->Left(), sEdit->Down() + 2);
	vEdit = AddEditBox(text->Right(), text->Top(), 32); vEdit->SetNumeric(0, 100, 1);

	listFix = AddListPop(oldColor->Left(), vEdit->Down() + 16, 104, 0, LS_FIX_COLOR);

	//Close
	y = DrawHLine(canvas->Down() + 2);

	button = AddButton("Ok", (WIDTH-135)/2, y, 0, 0, OK_BUTTON); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right()+8, y, 0, 0, CANCEL_BUTTON); SetCancelButton(button);

	

	listFix->AddText("Red");
	listFix->AddText("Green");
	listFix->AddText("Blue");

	listFix->AddText("Hue");
	listFix->AddText("Saturation");
	listFix->AddText("Brightness");

	listFix->SetText("Red");	
	fixedColor = FIX_RED;
	r = _r;
	g = _g;
	b = _b;
	
	oldColor->SetColor(r, g, b);
	

	colorSlide->SetPos(r / 255.0);

	UpdateSample(r, g, b);	
	UpdateCanvas(r, fixedColor);
	UpdateSlideColor(r, g, b, fixedColor);
}

ColorPicker::~ColorPicker()
{
	delete canvas;
	delete oldColor;
	delete newColor;
}

void ColorPicker::OnSlide(Slide *slide, int slideId, double pos)
{
	switch(slideId)
	{
	case SL_COLOR:
		{
			int color = pos*255;

			if(fixedColor <= FIX_BLUE)
			{
				switch(fixedColor)
				{
				case FIX_RED:
					r = color;			
					break;
					
				case FIX_GREEN:
					g = color;
					break;
					
				case FIX_BLUE:
					b = color;
					break;
				}

				UpdateSample(r, g, b);
			}
			else
			{
				double H, S, V;
				KrRGBA c;

				RGB_to_HSV(r, g, b, H, S, V);

				switch(fixedColor)
				{					
				case FIX_HUE:					
					c = HSV_to_RGB(color, S, V);
					break;
					
				case FIX_SATURATION:
					c = HSV_to_RGB(H, color, V);					
					break;
					
				case FIX_BRIGHTNESS:
					c = HSV_to_RGB(H, S, color);
					break;
				}

				UpdateSample(c.c.red, c.c.green, c.c.blue);
			}			

			UpdateCanvas(color, fixedColor);
		}
		break;
	}

	
}

void ColorPicker::OnButton(Button *button, int buttonId)
{
	result = buttonId;	
}

void ColorPicker::OnEditChar(EditBox *edit, SDLKey key, int line)
{
	int value = atol(edit->GetText().c_str());
	bool bUpdateHSV = true;

	if( (edit == rEdit || edit == gEdit || edit == bEdit) &&
		(value < 0 || value > 255))
	{
		return;
	}

	if( edit == hEdit &&
		(value < 0 || value > 359))
	{
		return;
	}

	if( (edit == sEdit || edit == vEdit) &&
		(value < 0 || value > 100))
	{
		return;
	}

	if(edit == rEdit) r = value;
	else if(edit == gEdit) g = value;
	else if(edit == bEdit) b = value;
	else
	{
		double H, S, V;
		KrRGBA c;

		RGB_to_HSV(r, g, b, H, S, V);

		if(edit == hEdit) c = HSV_to_RGB(round(255*value/360.0), S, V);
		else if(edit == sEdit) c = HSV_to_RGB(H, round(255*value/100.0), V);
		else if(edit == vEdit) c = HSV_to_RGB(H, S, round(255*value/100.0));

		r = c.c.red;
		g = c.c.green;
		b = c.c.blue;

		bUpdateHSV = false;
	}

	UpdateColorControls();	
	UpdateSample(r, g, b, bUpdateHSV);
}

bool ColorPicker::OnList(ListPop *list, int index, gedString &text, int listId)
{
	fixedColor = index;
	UpdateColorControls();

	return true;
}

void ColorPicker::SetColor(int _r, int _g, int _b)
{
	r = _r;
	g = _g;
	b = _b;

	UpdateSlideColor(r, g, b, fixedColor);
	UpdateSample(r, g, b);
}

void ColorPicker::UpdateSlideColor(int r, int g, int b, int fixedColor)
{
	KrRGBA *pixels = colorSlide->getCanvasResource()->Pixels(), color1, color2;
		
	int width = colorSlide->Width();

	if(fixedColor <= FIX_BLUE)
	{
		switch(fixedColor)
		{
		case FIX_RED:
			color1.Set(255, g, b);
			color2.Set(0, g, b);			
			break;
			
		case FIX_GREEN:
			color1.Set(r, 255, b);
			color2.Set(r, 0, b);			
			break;
			
		case FIX_BLUE:
			color1.Set(r, g, 255);
			color2.Set(r, g, 0);			
			break;
		}

		for(int i = 2; i < width - 3; i++)
		{
			for(int j = 1; j < 255; j++)
			{
				pixels[ j*width + i ] = SlowCanvas::MixColors(color1, color2, (j - 1)/(254.0));			
			}
		}
	}
	else
	{
		double H, S, V;
		RGB_to_HSV(r, g, b, H, S, V);
				
		for(int j = 1; j < 255; j++)
		{			
			KrRGBA color;

			switch(fixedColor)
			{
			case FIX_HUE:					
				color = HSV_to_RGB(255 - j, 255, 255);
				break;
				
			case FIX_SATURATION:
				color = HSV_to_RGB(H, 255 - j, V);
				break;
				
			case FIX_BRIGHTNESS:
				color = HSV_to_RGB(H, S, 255 - j);
				break;
			}

			for(int i = 2; i < width - 3; i++)
			{
				pixels[ j*width + i ] = color;				
			}
		}
	}

	colorSlide->getImage()->Invalidate();
	colorSlide->getCanvasResource()->Refresh();
}

void ColorPicker::UpdateCanvas(int c, int fixedColor)
{
	//Draw
	KrRGBA *pixels = canvas->getCanvasResource()->Pixels(), color;
		
	int i, width = canvas->Width();

	if(fixedColor <= FIX_BLUE)
	{
		//RGB

		for(i = 0; i < 256; i++)
		{
			for(int j = 0; j < 256; j++)
			{
				switch(fixedColor)
				{
				case FIX_RED:
					color.Set(c, 255 - j, i, 255);
					break;
					
				case FIX_GREEN:
					color.Set(255 - j, c,  i, 255);
					break;
					
				case FIX_BLUE:
					color.Set(i, 255 - j, c, 255);
					break;
				}
				
				pixels[ j*width + i ] = color;			
			}
		}
	}
	else
	{
		//HSV
		double H, S, V;
		
		for(i = 0; i < 256; i++)
		{
			for(int j = 0; j < 256; j++)
			{
				switch(fixedColor)
				{
				case FIX_HUE:
					H = c;
					S = i;
					V = 255 - j;					
					break;
					
				case FIX_SATURATION:
					H = i;
					S = c;
					V = 255 - j;					
					break;
					
				case FIX_BRIGHTNESS:
					H = i;
					S = 255 - j;
					V = c;					
					break;
				}

				color = HSV_to_RGB(H, S, V, false);								
				pixels[ j*width + i ] = color;			
			}
		}
	}

	canvas->getImage()->Invalidate();
	canvas->getCanvasResource()->Refresh();
}

void ColorPicker::UpdateSample(int _r, int _g, int _b, bool bUpdateHSV)
{
	newColor->SetColor(_r, _g, _b);
	

	rSample = _r;
	gSample = _g;
	bSample = _b;

	rEdit->SetText(rSample);
	gEdit->SetText(gSample);
	bEdit->SetText(bSample);

	double H, S, V;
	RGB_to_HSV(rSample, gSample, bSample, H, S, V);

	if(bUpdateHSV)
	{
		hEdit->SetText(round(360*H/255.0));
		sEdit->SetText(round(100*S/255.0));
		vEdit->SetText(round(100*V/255.0));
	}
}

void ColorPicker::UpdateColorControls()
{
	UpdateSlideColor(r, g, b, fixedColor);

	if(fixedColor <= FIX_BLUE)
	{
		switch(fixedColor)
		{
		case FIX_RED:
			colorSlide->SetPos(r/255.0);
			canvas->SetPointer(b, 255 - g);
			break;
			
		case FIX_GREEN:
			colorSlide->SetPos(g/255.0);
			canvas->SetPointer(b, 255 - r);
			break;
			
		case FIX_BLUE:
			colorSlide->SetPos(b/255.0);
			canvas->SetPointer(r, 255 - g);
			break;
		}
	}
	else
	{
		double H, S, V;
		RGB_to_HSV(r, g, b, H, S, V);		
		
		switch(fixedColor)
		{
		case FIX_HUE:					
			colorSlide->SetPos(H/255.0);
			canvas->SetPointer(S, 255 - V);
			break;
			
		case FIX_SATURATION:
			colorSlide->SetPos(S/255.0);
			canvas->SetPointer(H, 255 - V);
			break;
			
		case FIX_BRIGHTNESS:
			colorSlide->SetPos(V/255.0);
			canvas->SetPointer(H, 255 - S);
			break;
		}
		
	}

	UpdateCanvas(colorSlide->GetPos()*255, fixedColor);
}

#endif //#ifndef STAND_ALONE_GAME








