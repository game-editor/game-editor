// TutorialTextPanel.cpp: implementation of the TutorialTextPanel class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "TutorialTextPanel.h"
#include "GameControl.h"
#include "Tutorial.h"
#include "TutorialPaint.h"


enum
{
	BT_CLOSE,
	BT_ADD
};

#define BUTTON_HEIGHT 17


MapPointer TutorialTextPanel::mapTextPanel;
long TutorialTextPanel::depth = CURSOR_DEPTH - 499;

static gedString getUniqueText()
{
	static int count = 0;
	char buf[50];
	sprintf(buf, "TutorialTextPanel%ld", SDL_GetTicks() + count++);

	return buf;
}

TutorialTextPanel::TutorialTextPanel(const gedString& _explanationText)
	: Actor(getUniqueText(), TutorialPaint::Call(), CANVAS, 
	Text::GetDimensions(_explanationText).Width() + 6 + (Tutorial::InRecMode()?50:0), 
	Text::GetDimensions(_explanationText).Height() + 4 + (Tutorial::InRecMode()?BUTTON_HEIGHT:0), true)
{
	explanationText = _explanationText;
	mapTextPanel.Add(this, 1);

	Button *button;

	getImage()->SetZDepth(++depth);
	SetTransparency(.16);

	int x = (GameControl::Get()->Width() - Width())/2, 
		y = (GameControl::Get()->Height() - Height())/2, 
		width = Width(), 
		height = Height();

	if(x + width > GameControl::Get()->Width())
		x -= x + width - GameControl::Get()->Width();

	if(y + height > GameControl::Get()->Width())
		y -= y + height - GameControl::Get()->Height();

	getImage()->SetPos(x, y);

	
	//Draw
	KrRGBA *pixels = getCanvasResource()->Pixels(), colorBack, colorBlack;

	colorBack.c.red		= 255;
	colorBack.c.green	= 255;
	colorBack.c.blue	= 225;
	colorBack.c.alpha	= 255;

	colorBlack.Set(0, 0, 0);

	//Fill background
	int i;
	for(i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			if(i == 0 || j == 0 || i == width-1 || j == height - 1 || (Tutorial::InRecMode() && j == height - BUTTON_HEIGHT))
				pixels[ j*width + i ] = colorBlack;
			else
				pixels[ j*width + i ] = colorBack;
		}
	}

	//Text
	AddText(explanationText, 3, 2);

	if(Tutorial::InRecMode())
	{
		button = AddButton("Remove", (width - 50)/2, height - BUTTON_HEIGHT + 2, 50, 12, BT_CLOSE, 0, 0, 0, "maksbutton_bright.bmp"); 
	}	
}

TutorialTextPanel::~TutorialTextPanel()
{
	mapTextPanel.Remove(this);
}

int TutorialTextPanel::Count()
{
	return mapTextPanel.size();
}

void TutorialTextPanel::Show()
{
	MapPointerIterator it(mapTextPanel);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		((TutorialTextPanel *)(*it.Key()))->getImage()->SetVisible(true);
	}
}

void TutorialTextPanel::Hide()
{
	MapPointerIterator it(mapTextPanel);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		((TutorialTextPanel *)(*it.Key()))->getImage()->SetVisible(false);
	}
}

void TutorialTextPanel::DestroyAll()
{
	MapPointerIterator it(mapTextPanel);	
	
	if(mapTextPanel.size())
	{
		it.Begin();	
		do
		{
			TutorialTextPanel *p = (TutorialTextPanel *)(*it.Key());

			if(Tutorial::InRecMode())
			{
				p->Save();
			}

			delete p;
			it.Begin();
			
		} while(!it.Done());
	}
}

bool TutorialTextPanel::OnMouseButtonDown(int x, int y, Uint8 button)
{
	if(Tutorial::InRecMode())
	{
		if(getImage()->ZDepth() < depth)
		{
			getImage()->SetZDepth(++depth);
		}
		
		return true;
	}
	
	return false;
}

const stCursor *TutorialTextPanel::OnGetCursor()
{
	/*cursor.imgName	= "makscursorMao.bmp";
	cursor.nFramesH	= 1;
	cursor.nFramesV	= 1;
	cursor.hotSpotX = 7;
	cursor.hotSpotY = 0;
	cursor.bAnimate = false;
	
	return &cursor;	*/

	return NULL;
}

void TutorialTextPanel::OnButton(Button *button, int buttonId)
{
	delete this;
}

void TutorialTextPanel::Save()
{
	Tutorial::Get()->StoreText(getImage()->X(), getImage()->Y(), explanationText);
}


#endif //STAND_ALONE_GAME









