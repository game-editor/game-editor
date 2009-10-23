/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues

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
<http://game-editor.com/License>, thats will help us to pay our developers 
<http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>.

***************************************************************************/


// Text.cpp: implementation of the Text class.
//
//////////////////////////////////////////////////////////////////////

#include "Text.h"
#include "GameControl.h"

#include "dlmalloc.h"

extern bool InGameMode(); //maks
extern bool InStandAloneMode(); //maks


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Text::Text(const gedString& text, int x, int y, int align,
		   Actor *parent, 
		   int red, int green, int blue,
		   const gedString &fontName,
		   int iniASCIIChar, int nChars,
		   bool _bEditable, bool _hasBorder, bool _hasBackGround, bool _getFocus,
		   KrRGBA _borderColor, KrRGBA _cursorColor, KrRGBA _backGroundColor)
{	
	this->fontName = fontName;
	this->iniASCIIChar = iniASCIIChar;
	this->nChars = nChars;
	this->text = text;

	bEditable = _bEditable;
	bDrawBorder = _hasBorder;
	bDrawBackground = _hasBackGround;
	bGetFocus = _getFocus;
	borderColor = _borderColor;
	cursorColor = _cursorColor;
	backGroundColor = _backGroundColor;

	textBox = NULL;

	
	fontResource = GameControl::Get()->GetFont(fontName, iniASCIIChar, nChars);
	
	if(fontResource)
	{
		const int lineSpacing = 0;
		int nLines = 1;
		gedString maxTextWidth;	
		bool bOnlyBlank = true;

		

		if(bEditable)
		{
			for(int i = 0; i < text.size(); i++)
			{
				if(text[i] != '\n')
				{
					maxTextWidth += 'O';
				}
				else
				{
					maxTextWidth += '\n';
					nLines++;
				}

				if( text[i] != ' ' && 
					text[i] != '\t' &&
					text[i] != '\r' &&
					text[i] != '\n')
				{
					bOnlyBlank = false;					
				}
			}
		}
		else
		{
			maxTextWidth = text;
			
			for(int i = 0; i < text.size(); i++)
			{
				if(text[i] == '\n')
				{
					nLines++;
				}
			}
		}

		
		int textWidth;

		if(nLines == 1)
		{
			textWidth  = fontResource->FontWidth(maxTextWidth.c_str());
		}
		else
		{
			int height;
			GetDimensions(maxTextWidth, textWidth, height, fontName);
		}

				
		switch(align)
		{
		case ALIGN_CENTER:
			align = KrTextBox::CENTER;
			break;
		case ALIGN_RIGHT:
			align = KrTextBox::RIGHT;
			break;
		case ALIGN_AUTOMATIC: //Only left (solve the text jump in textJump.ged)
			//if(nLines == 1) align = KrTextBox::CENTER;
			/*else*/ align = KrTextBox::LEFT;
			break;
		default:
			align = KrTextBox::LEFT;
			break;
		}			
		

		if(bEditable && !bDrawBackground)
		{
			//Draw a invisible background if editable text has no background
			backGroundColor.all = 0;
			backGroundColor.c.alpha = 1;
		}

		
		textBox = new KrTextWidget( fontResource, 
							 textWidth,
							 nLines*(fontResource->FontHeight() + lineSpacing), 
							 lineSpacing,
							 (KrTextBox::Alignment)align,
							 parent,
							 nLines > 1,
							 bEditable, bDrawBorder, bDrawBackground, bGetFocus,
							 borderColor, cursorColor, backGroundColor);

				
		KrColorTransform color;
		color.Set(red, 0, green, 0, blue, 0, 255);
		textBox->SetColor(color);

		if(x == CENTER_TEXT)
			x = (parent->Width() - textWidth) / 2;

		if(y == CENTER_TEXT)
			y = (parent->Height() - fontResource->FontHeight()) / 2;

				
		textBox->SetNodeId((int)textBox);
		engine->Tree()->AddNode( parent?parent->getImage():NULL, textBox );
		textBox->SetPos(x, y);

		
		if(!bEditable || !bOnlyBlank || (!InGameMode() && !InStandAloneMode()))
		{
			SetText(text); //After add tree
		}

		fontResource->refCount++;

		
		//Call walk here to update the compositeBounds
		//So, the text will be load by the RegionLoad::DefineActors()
		//Solve the reload - text bug.ged
		//Only in game mode (solve the Issue 80)

		if(parent && GameControl::Get()->getGameMode())
		{
			engine->Tree()->CalculateCompositBounds(parent->getImage());
		}
	}
}

Text::~Text()
{
	if(fontResource && engine->Vault()->ExistsResource(fontResource))
	{
		fontResource->refCount--;
		if(fontResource->refCount == 0)
			engine->Vault()->RemoveSpriteResource(fontResource);
	}

	if(engine->Tree()->FindNodeById((int)textBox))
	{
		engine->Tree()->DeleteNode(textBox);
	}
}



void Text::SetColor(int r, int g, int b)
{
	KrColorTransform color;
	color.Set(r, 0, g, 0, b, 0, 255);
	if(textBox) textBox->SetColor(color);
}



int Text::Width()
{
	if(textBox) return textBox->Width();
	else return 0;
}

int Text::Height()
{
	if(textBox) return textBox->Height();
	else return 0;
}

int Text::Top()
{
	if(textBox) return textBox->Y();
	else return 0;
}

int Text::Down()
{
	if(textBox) return textBox->Y() + textBox->Height();
	else return 0;
}

int Text::Left()
{
	if(textBox) return textBox->X();
	else return 0;
}

int Text::Right()
{
	if(textBox) return textBox->X() + textBox->Width();
	else return 0;
}

//#ifndef STAND_ALONE_GAME
void Text::GetDimensions(const gedString& text, int &width, int &height, const gedString& fontName)
{
	//Calculate dimensions
	KrFontResource *fontResource = GameControl::Get()->GetFont(fontName);
	if(fontResource)
	{
		if(text.find('\n') == gedString::npos)
		{			
			width = fontResource->FontWidth(text.getCharBuf());
			height = fontResource->FontHeight();
		}
		else
		{
			gedString aux(text);
			int w = 0;
			//KrScheme scheme(fontResource);	
			width = 0;
			
			const int lineSpacing = 0;
			int nLines = 1, ini = 0;
			char *buf = (char *)malloc(aux.size()+1);
			strcpy(buf, (char *)aux.c_str());
			
			for(int i = 0; i < aux.size(); i++)
			{
				if(buf[i] == '\n')
				{
					buf[i] = 0;
					w = fontResource->FontWidth((const char *)&buf[ini]);
					width = max(width, w);
					ini = i+1;
					nLines++;
				}
			}
			
			w = fontResource->FontWidth((const char *)&buf[ini]);
			width = max(width, w);
			height = nLines*(fontResource->FontHeight() + lineSpacing);
			free(buf);
		}
	}
	else
	{
		width = height = 0;
	}
}

KrRect Text::GetDimensions(const gedString& text, const gedString &fontName)
{
	KrRect rect;
	rect.Zero();

	GetDimensions(text, rect.xmax, rect.ymax, fontName);

	return rect;
}

KrRect Text::GetDimensionsFont(const gedString& text)
{
	KrRect rect;
	rect.Zero();

	GetDimensions(text, rect.xmax, rect.ymax, fontName);

	return rect;
}

void Text::SetText(gedString text)
{
	if(!textBox) return;
	
	if(text.length())
	{
		if(text.find('\n') != gedString::npos)
		{
			gedString tmp(text);
			U16 *buf = (U16 *)tmp.c_str16();
			
			int line = 0, ini = 0;
			for(int i = 0; i < text.size(); i++)
			{
				if(buf[i] == '\n')
				{
					buf[i] = 0;
					textBox->SetText(&buf[ini], line);	
					ini = i+1;
					line++;
				}
			}
			
			if(buf[ini])
				textBox->SetText(&buf[ini], line);
		}
		else
		{
			textBox->SetText(text, 0);
		}
	}
	else
	{
		for(int i = 0; i < textBox->NumLines(); i++)
		{
			textBox->SetText("", i);
		}
	}
}

gedString Text::GetText(int line)
{
	if(!textBox) return "";

	gedString buf;

	if(textBox->NumLines() == 1)
	{
		line = 0;
	}

	if(line == ALL_LINES)
	{
#ifndef STAND_ALONE_GAME
		WaitCursor wait;
#endif

		for(int i = 0; i < textBox->NumLines(); i++)
		{
			gedString line;
			textBox->GetText(&line, i);
			if(!bEditable) line.trimRight();

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
		if(line >= 0 && line < textBox->NumLines())
		{
			textBox->GetText(&buf, line);
		}
	}

	return buf;
}
