// EditBox.cpp: implementation of the EditBox class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#pragma warning(disable:4786)

#include "EditBox.h"
#include "GameControl.h"
#include "../gameEditor/MainPanel.h"
#include "Slide.h"


NumericSlide::NumericSlide(Actor *parent, EditBox *_edit, double _min, double _max, double _step)
: Actor("NumericSlide", parent, CANVAS, 11, _edit->Height()) 
{
	int height = _edit->Height(), width = Width();
	edit = _edit;

	minValue = _min;
	maxValue = _max;
	step = _step;

	KrRGBA *pixels = getCanvasResource()->Pixels();
		
	
	//Fill background
	int i, j;
	
	for(j = 0; j < height; j++)
	{
		KrRGBA color(SlowCanvas::MixColors(Panel::getColorUp(), Panel::getColorDown(), j/(double)height));
		for(i = 0; i < width; i++)
		{
			pixels[ j*Width() + i ] = color;
		}
	}

	//Glass effect
	KrRGBA white(255, 255, 255);
	int hm = height/2;
	for(j = 0; j < hm; j++)
	{
		for(i = 0; i < width; i++)
		{
			pixels[ j*width + i ] = SlowCanvas::MixColors(pixels[ j*width + i ], white, .8*j/(double)hm);
		}
	}

	KrRGBA colorBorder(104, 104, 104);	

		//Horizontal lines
		for(i = 0; i < width; i++)
		{
			pixels[  i ] = colorBorder;	
			pixels[ (height-1)*Width() + i ] = colorBorder;
		}
		
		//Vertical lines
		for(j = 0; j < height; j++)
		{
			pixels[ j*Width() ] = colorBorder;		
			pixels[ j*Width() + (width-1) ] = colorBorder;
		}

	//Corner pixels
	pixels[ 1 + 1*Width() ] = colorBorder;
	pixels[ (width - 2) + 1*Width() ] = colorBorder;
	pixels[ (width - 2) + (height - 2)*Width() ] = colorBorder;
	pixels[ 1 + (height - 2)*Width() ] = colorBorder;


	//Arrows
	pixels[ 6*Width() + 4 ] = Panel::getColorDown();
	pixels[ 7*Width() + 4 ] = Panel::getColorDown();
	pixels[ 8*Width() + 4 ] = Panel::getColorDown();
	pixels[ 9*Width() + 4 ] = Panel::getColorDown();
	pixels[ 10*Width() + 4 ] = Panel::getColorDown();

	pixels[ 7*Width() + 3 ] = Panel::getColorDown();
	pixels[ 8*Width() + 3 ] = Panel::getColorDown();
	pixels[ 9*Width() + 3 ] = Panel::getColorDown();

	pixels[ 8*Width() + 2 ] = Panel::getColorDown();



	pixels[ 6*Width() + 6 ] = Panel::getColorDown();
	pixels[ 7*Width() + 6 ] = Panel::getColorDown();
	pixels[ 8*Width() + 6 ] = Panel::getColorDown();
	pixels[ 9*Width() + 6 ] = Panel::getColorDown();
	pixels[ 10*Width() + 6 ] = Panel::getColorDown();

	pixels[ 7*Width() + 7 ] = Panel::getColorDown();
	pixels[ 8*Width() + 7 ] = Panel::getColorDown();
	pixels[ 9*Width() + 7 ] = Panel::getColorDown();

	pixels[ 8*Width() + 8 ] = Panel::getColorDown();

	//Make trasparent corners
	KrRGBA transp(0, 0, 0, 0);

	pixels[ 0 + 0*Width() ] = transp;
	pixels[ 1 + 0*Width() ] = transp;
	pixels[ 0 + 1*Width() ] = transp;

	pixels[ (width - 1) + 0*Width() ] = transp;
	pixels[ (width - 2) + 0*Width() ] = transp;
	pixels[ (width - 1) + 1*Width() ] = transp;

	pixels[ (width - 1) + (height - 1)*Width() ] = transp;
	pixels[ (width - 2) + (height - 1)*Width() ] = transp;
	pixels[ (width - 1) + (height - 2)*Width() ] = transp;

	pixels[ 0 + (height - 1)*Width() ] = transp;
	pixels[ 1 + (height - 1)*Width() ] = transp;
	pixels[ 0 + (height - 2)*Width() ] = transp;


	bSlide = false;
	actorTip = "Click and drag to change the value";
}

void NumericSlide::SetLimits(double _min, double _max, double _step)
{
	minValue = _min;
	maxValue = _max;
	step = _step;
}

bool NumericSlide::OnMouseButtonDown(int x, int y, Uint8 button)
{
	DisableCursor();

	xIni = x;
	bSlide = true;

	iniValue = atof(edit->GetText().c_str());

	return false;
}

void NumericSlide::OnMouseButtonUp(int x, int y, Uint8 button)
{
	EnableCursor();
	bSlide = false;
}

void NumericSlide::OnMouseMove(int x, int y)
{
	if(bSlide)
	{
		double newValue = (x - xIni)*step + iniValue;

		if(newValue < minValue)
		{
			iniValue = newValue = minValue;
			xIni = x;
		}
		else if(newValue > maxValue)
		{
			iniValue = newValue = maxValue;
			xIni = x;
		}

		edit->SetText(newValue);

		//Any key
		SDLKey key = SDLK_UNKNOWN;
		edit->OnChar(key, 0);
	}
}


VariableSelector::VariableSelector(Actor *parent, EditBox *_edit, bool _bPointer)
	: Actor("VariableSelector", parent, CANVAS, 22, _edit->Height())
{
	int height = _edit->Height();
	edit = _edit;
	bPointer = _bPointer;

	KrRGBA *pixels = getCanvasResource()->Pixels();

		
	//Fill background
	int i;
	for(i = 0; i < Width(); i++)
	{
		for(int j = 0; j < height; j++)
		{
			pixels[ j*Width() + i ] = Panel::getColorBack();
		}
	}

	//Horizontal lines
	for(i = 0; i < Width(); i++)
	{
		pixels[ i ] = Panel::getColorUp();			
		pixels[ (height-1)*Width() + i ] = Panel::getColorDown();
	}

	//Vertical lines
	for(int j = 0; j < height; j++)
	{
		pixels[ j*Width() ] = Panel::getColorUp();		
		pixels[ j*Width() + (Width()-1) ] = Panel::getColorDown();
	}


	//Arrows
	/*pixels[ 6*Width() + 4 ] = Panel::getColorDown();
	pixels[ 7*Width() + 4 ] = Panel::getColorDown();
	pixels[ 8*Width() + 4 ] = Panel::getColorDown();
	pixels[ 9*Width() + 4 ] = Panel::getColorDown();
	pixels[ 10*Width() + 4 ] = Panel::getColorDown();

	pixels[ 7*Width() + 3 ] = Panel::getColorDown();
	pixels[ 8*Width() + 3 ] = Panel::getColorDown();
	pixels[ 9*Width() + 3 ] = Panel::getColorDown();

	pixels[ 8*Width() + 2 ] = Panel::getColorDown();



	pixels[ 6*Width() + 6 ] = Panel::getColorDown();
	pixels[ 7*Width() + 6 ] = Panel::getColorDown();
	pixels[ 8*Width() + 6 ] = Panel::getColorDown();
	pixels[ 9*Width() + 6 ] = Panel::getColorDown();
	pixels[ 10*Width() + 6 ] = Panel::getColorDown();

	pixels[ 7*Width() + 7 ] = Panel::getColorDown();
	pixels[ 8*Width() + 7 ] = Panel::getColorDown();
	pixels[ 9*Width() + 7 ] = Panel::getColorDown();

	pixels[ 8*Width() + 8 ] = Panel::getColorDown();*/

	{
		EditorDirectory dir;

		icon = new Actor("ged_VariableSelector_icon", this);
		icon->AddAnimation("icon", "b_xy.png", 1, 1);
		icon->SetPos(9 + (Width() - icon->Width()) / 2, 10 + (Height() - icon->Height()) / 2);
		icon->getSprite()->SetHitTest(false);	
	}


	actorTip = "Click to select a available variable";

	listPop = AddListPop(Width()-1, height-1, 1, 1);
}

void VariableSelector::SetList(ListData &list)
{
	listPop->RemoveAll();

	for(int i = 0; i < list.Count(); i++)
	{
		listPop->AddText(list[i].text, false, -1, list[i].data);
	}
}

bool VariableSelector::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(text != ACTOR_VARIABLES)
	{
		if(bPointer && !list->GetItemData(index))
		{
			text = gedString("&") + text;
		}

		edit->InsertText(text);
	}
	else
	{
		PostMessage(edit->getParentActor(), LIST_ACTOR_VARIABLES, LIST_ACTOR_VARIABLES);
	}

	return true;
}

bool VariableSelector::OnListButtonDown(ListPop *list, int listId)
{
	PostMessage(edit->getParentActor(), CLICK_ON_VARIABLE_SELECTOR, (U32)this);
	return true;
}

bool VariableSelector::OnMouseButtonDown(int x, int y, Uint8 button)
{
	//Show list
	listPop->OnMouseButtonDown(x, y, button);

	return false;
}





EditBox::EditBox(int x, int y, int width, int height, bool multiLine, Actor *parent, gedString fontName)
{
	parentActor = parent;
	text = NULL;
	fontResource = GameControl::Get()->GetFont(fontName.c_str());
	if(fontResource)
	{
		KrScheme scheme(fontResource);
		
		if(height == 0)
			height = fontResource->FontHeight() + 4;
		
		text = new EditTextWidget(width, height, true, true, true, scheme, this, multiLine);
				
		

		text->SetNodeId((int)text);
		engine->Tree()->AddNode( parent->getImage(), text );
		text->SetPos( x, y );

		fontResource->refCount++;

		Enable(); //To message process		
	}

	numericSlide = NULL;
	scrollBar = NULL;
	variableSelector = NULL;
}

EditBox::~EditBox()
{
	if(engine->Vault()->ExistsResource(fontResource))
	{
		fontResource->refCount--;
		if(fontResource->refCount == 0)
			engine->Vault()->RemoveSpriteResource(fontResource);
	}

	if(engine->Tree()->FindNodeById((int)text))
	{
		engine->Tree()->DeleteNode(text);
	}

	delete numericSlide;
	delete scrollBar;
	delete variableSelector;
}

bool EditBox::EditTextWidget::HandleWidgetEvent(	KrWidget* source, 
							   U32 event, 
							   U32 data, 
							   const SDL_Event* sdlEvent,
							   const char* command, 
							   const char* arg )
{	

	if(event == 1 /*ACTIVATE*/)
	{
		parent->OnEnter(ActualLine());
		return true;
	}
	else if(event == 100 /*TYPE*/)
	{
		parent->OnChar((SDLKey)data, ActualLine());
		return true;
	}
	else if(event == LINE_CHANGED)
	{
		parent->UpdateScrollInfo();
		parent->OnLineChanged();		
		return true;
	}
	else if(event == 102 /*Right Click*/)
	{
		//Call the generic context menu on MainPanel actor
		int x = data, y = (int)sdlEvent;

		MainPanel::Get()->EditBoxContextMenu(this, x, y);

		return true;
	}
	else if(event == SELECTION)
	{
		parent->OnEditSelection();
	}

	return false;
}

void EditBox::OnLineChanged()
{
	if(scrollBar) scrollBar->SetScrollPos(ActualLine());
	parentActor->PostMessage(parentActor, LINE_CHANGED, ActualLine());
}

void EditBox::UpdateScrollInfo()
{
	if(scrollBar) scrollBar->SetScrollInfo(text->NumLines(), text->WindowLines());	
}

void EditBox::OnEnter(int line)
{
	parentActor->OnEditEnter(this, line);
}

void EditBox::OnEditSelection()
{
	parentActor->OnEditSelection(this);
}

void EditBox::OnChar(SDLKey key, int line)
{
	GameControl::Get()->setModified();
	parentActor->OnEditChar(this, key, line);
}

gedString EditBox::GetText(int line, bool bTrim)
{
	gedString buf;

	if(line == ALL_LINES)
	{
		WaitCursor wait;

		for(int i = 0; i < text->NumLines(); i++)
		{
			gedString line;
			text->GetText(&line, i);
			if(bTrim) line.trimRight();

			/*if(line.length() > 0)
			{
				if(i > 0 && buf[buf.length()-1] != '\n')  buf += "\n";
				buf += line;
			}*/

			buf += line;
			buf += "\n";
		}

		//Remove last empty lines
		while(buf.length() > 1 && buf[buf.length() - 1] == '\n')
		{
			buf = buf.substr(0, buf.length() - 1);
		}

		if(buf == "\n") 
			buf.clear();
	}
	else
	{
		if(line >= 0 && line < text->NumLines())
		{
			text->GetText(&buf, line);
		}
	}

	return buf;
}

gedString EditBox::GetTextUntilCursor()
{
	return text->GetTextUntilCursor();
}

void EditBox::SetText(gedString str, int line)
{
	WaitCursor wait; //Time to render new characters

	if(str.length() == 0)
	{
		for(int i = 0; i < text->NumLines(); i++)
		{
			text->SetText("", i);
		}
	}
	else
	{
		if(line == ALL_LINES)
		{
			WaitCursor wait;
			gedString tmp(str);

			U16 *buf = (U16 *)tmp.c_str16();
						
			int nLines = 0, ini = 0;
			
			for(int i = 0; i < str.size(); i++)
			{
				if(buf[i] == '\n')
				{
					buf[i] = 0;
					if(wcsLen(&buf[ini]))
					{
						text->SetText(&buf[ini], nLines);				
						//nLines++;
					}

					nLines++; //Increment on empty lines too
					ini = i+1;
					
				}
			}
			
			if(wcsLen(&buf[ini])) text->SetText(&buf[ini], nLines);		
			text->ScrollTo(0);
		}
		else
		{
			if(line >= 0 && line < text->NumLines())
			{
				text->SetText(str, line);
			}
		}
	}

	UpdateScrollInfo();
}

void EditBox::SetText(int n, int line)
{
	char buf[256];
	sprintf(buf, "%ld", n);
	SetText(buf, line);	
}

void EditBox::SetText(double n, int line)
{
	char buf[256];
	sprintf(buf, "%.12f", n);

	//No right zeros
	int len = strlen(buf);
	while(len && buf[len-1] == '0')
	{
		len--;
	}

	if(len > 0 && buf[len-1] == '.') len--;

	buf[len] = 0;

	SetText(buf, line);	
}

int EditBox::Width()
{
	if(!numericSlide && !scrollBar && !variableSelector)
		return text->Width();
	else
	{
		int width = text->Width();

		if(numericSlide) width += numericSlide->Width();
		if(scrollBar) width += scrollBar->Width();
		if(variableSelector) width += variableSelector->Width();

		return width;
	}
}

int EditBox::Height()
{
	return text->Height();
}

int EditBox::Top()
{
	return text->Y();
}

int EditBox::Down()
{
	return text->Y() + text->Height();
}

int EditBox::Left()
{
	return text->X();
}

int EditBox::Right()
{
	return text->X() + Width();
}

int EditBox::X()
{
	return text->X();
}

int EditBox::Y()
{
	return text->Y();
}

void EditBox::InsertText(const gedString& str)
{
	text->InsertText(str);
}

bool EditBox::SetFocus()
{
	return KrEventManager::Instance()->GrabKeyFocus( text );	
}

void EditBox::SetNumeric(double _min, double _max, double _step)
{
	if(!numericSlide)
	{
		int width = text->Width();
		if(variableSelector)  width += variableSelector->Width() + 1;

		numericSlide = new NumericSlide(parentActor, this, _min, _max, _step);
		numericSlide->SetPos(text->X() + width + 1, Top());
	}
	else
	{
		numericSlide->SetLimits(_min, _max, _step);
	}
}

void EditBox::SetVariableList(ListData &list, bool _bPointer)
{
	if(list.Count())
	{
		if(!variableSelector)
		{
			int width = text->Width();
			if(numericSlide)  width += numericSlide->Width() + 1;
			
			variableSelector = new VariableSelector(parentActor, this, _bPointer);
			variableSelector->SetPos(text->X() + width + 1, Top());
		}
		
		variableSelector->SetList(list);
	}
}

void EditBox::ShowScrollBar()
{
	if(!scrollBar)
	{
		scrollBar = new ScrollBar(parentActor, this);
		scrollBar->SetPos(text->X() + text->Width() + 1, Top());
	}
}

void EditBox::ScrollTo(int lineIndex)
{
	text->ScrollTo(lineIndex);
}

int EditBox::ScrollUp()
{
	return text->ScrollUp();
}

int EditBox::ScrollDown()
{
	return text->ScrollDown();
}

int EditBox::ScrollPageUp()
{
	return text->ScrollPageUp();
}

int EditBox::ScrollPageDown() 
{
	return text->ScrollPageDown();
}

void EditBox::StartAction()
{
	text->StartAction();
}

void EditBox::EndAction()
{
	text->EndAction();
}

#endif //#ifndef STAND_ALONE_GAME




