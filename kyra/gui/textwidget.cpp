/*--License:
	Kyra Sprite Engine
	Copyright Lee Thomason (Grinning Lizard Software) 2001-2002
	www.grinninglizard.com/kyra
	www.sourceforge.net/projects/kyra

	Kyra is provided under 2 licenses:

	- The GPL, with no additional restrictions.
	- The LGPL, provided you display the Kyra splash screen, described below.


--- GPL License --
	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

	The full text of the license can be found in license.txt


--- LGPL License --
  **Provided you kindly display the Kyra splash screen (details below), 
	you	may use the LGPL license:**

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	The full text of the license can be found in lgpl.txt


--- Kyra Splash Screen.

	It would be appreciate if you display the Kyra splash screen when using
	either license, however it is only required for the LGPL. All the
	resources for the splash are compiled into the library, and it can be
	accessed through the following API:

		KrEngine::StartSplash
		KrEngine::UpdateSplash
		KrEngine::EndSplash

	Full documentation is provided with the KrEngine class. The splash screen
	should be displayed for 2 seconds.

	Thank you.
*/



#include "textwidget.h"
#include "eventmanager.h"
#include "../engine/boxresource.h"
#include "../engine/box.h"
#include "../engine/textbox.h"
#include "../engine/fontresource.h"




extern bool InModalActor(void *actor); //maks
extern bool InGameMode(); //maks
extern bool InStandAloneMode(); //maks
extern bool TutorialIndentationCompatible(); //maks

#ifndef STAND_ALONE_GAME
#include "../../gameEngine/WaitCursor.h"

typedef GlSList<Token> ListTokens;  //maks
typedef GlSListIterator<Token> ListTokensIterator; //maks
extern ListTokens GetTokens(gedString str); //maks
#endif


KrTextWidget::KrTextWidget(	int _width, int _height, 
							bool _drawBorder,
							bool _drawBackground,
							bool _editable,
							const KrScheme& _scheme,
							bool _multiLine) : KrWidget( _scheme ) //maks
{
	width = _width;
	height = _height;
	drawBorder = _drawBorder;
	drawBackground = _drawBackground;
	editable = _editable;
	//KrScheme scheme = _scheme;

	plateRes = 0;
	plate = 0;
	textBox = 0;
	cursorPos = 0;
	cursor = 0;
	bevel = 0;
	bHasFocus = false; //maks
	bEnable = true; //maks

	holder = NULL; //maks

	lineAnt = currentWindowLine = line = 0; //maks
	multiLine = _multiLine; //maks
	lineSelIni = posSelIni = lineSelFin = posSelFin = -1; //maks
	textOffSet = 0;

	SetAccelerator( KMOD_CTRL, SDLK_c ); //maks
	SetAccelerator( KMOD_CTRL, SDLK_v ); //maks
	SetAccelerator( KMOD_CTRL, SDLK_x ); //maks
	SetAccelerator( KMOD_CTRL, SDLK_z ); //maks
	SetAccelerator( KMOD_CTRL, SDLK_y ); //maks
	SetAccelerator( KMOD_CTRL, SDLK_r ); //maks

	lastMouseClick = 0; //maks
	backgroundColor = scheme.primary; //maks

	cursorRes = NULL; //maks
	align = KrTextBox::LEFT; //maks
}

KrTextWidget::KrTextWidget(	KrFontResource* _resource,
						int _width, int _height,
						int _lineSpacing,
						KrTextBox::Alignment _align,
						void *parent,
						bool _multiLine,
						bool _bEditable, bool _hasBorder, bool _hasBackGround, bool _getFocus,
						KrRGBA _borderColor, KrRGBA _cursorColor, KrRGBA _backGroundColor,
						KrScheme _scheme)  : KrWidget( _scheme )//maks
{
	width = _width;
	height = _height;
	
	editable = _bEditable;
	drawBorder = _hasBorder;
	drawBackground = _hasBackGround;
	bHasFocus = _getFocus;


	align = _align;
	

	plateRes = 0;
	plate = 0;
	textBox = 0;
	cursorPos = 0;
	cursor = 0;
	cursorRes = 0;
	bevel = 0;
	holder = NULL; //maks
	
	bEnable = true; //maks

	lineAnt = currentWindowLine = line = 0; //maks
	multiLine = _multiLine; //maks
	lineSelIni = posSelIni = lineSelFin = posSelFin = -1; //maks
	textOffSet = 0;

	lastMouseClick = 0; //maks

	//No transparency here
	if(!_cursorColor.all) _cursorColor.c.alpha = 255;
	if(!_borderColor.all) _borderColor.c.alpha = 255;
	if(!_backGroundColor.all) _backGroundColor.c.alpha = 255;

	scheme.font = _resource;
	scheme.cursor.all = _cursorColor.all;
	scheme.primary.all = _borderColor.all;
	backgroundColor.all = _backGroundColor.all;

	this->parentActor = parent;

	if(!parentActor)
	{
		SetAccelerator( KMOD_CTRL, SDLK_c ); //maks
		SetAccelerator( KMOD_CTRL, SDLK_v ); //maks
		SetAccelerator( KMOD_CTRL, SDLK_x ); //maks
		SetAccelerator( KMOD_CTRL, SDLK_z ); //maks
		SetAccelerator( KMOD_CTRL, SDLK_y ); //maks
		SetAccelerator( KMOD_CTRL, SDLK_r ); //maks
	}
}

KrTextWidget::~KrTextWidget()
{
	// Children are deleted first, else this explodes.
	delete plateRes;
	delete cursorRes;
	delete bevel;
}


void KrTextWidget::AddedtoTree()
{
	KrWidget::AddedtoTree();
	
	holder = new KrImNode();
	Engine()->Tree()->AddNode( this, holder );

	// Add a background plate
	if ( drawBackground || 
		(editable && backgroundColor.c.alpha == 1) //Draw a invisible background if editable text has no background
		)
	{
		plateRes = new KrBoxResource( "TextWidget Plate",
									  width, height,
									  &backgroundColor, 1,
									  KrBoxResource::FILL );

		plate = new KrBox( plateRes );
		Engine()->Tree()->AddNode( holder, plate );

		plate->setParentActor(parentActor);
	}

	// Add a text box
	if(parentActor) //maks
	{
		textBox = new KrTextBox( scheme.font, 
							 ( drawBorder ) ? width -2 : width,
							 multiLine?height:scheme.font->FontHeight(), 
							 0,
							 editable?KrTextBox::LEFT:align,
							 parentActor);		
	}
	else
	{
		textBox = new KrTextBox( scheme.font, 
			( drawBorder ) ? width -2 : width,
			multiLine?height:scheme.font->FontHeight(), //maks
			0 );
	}

	textBox->SetPos(	( drawBorder ) ? 1 : 0,
						multiLine?(( drawBorder ) ? 1 : 0):(( height - scheme.font->FontHeight() ) / 2)  ); //maks
	
	Engine()->Tree()->AddNode( holder, textBox );

	

	// Borders
	if ( drawBorder )
	{
		bevel = new KrBevelElement( width, height, scheme, parentActor );
		bevel->AddToTree( Engine(), holder );
		bevel->DrawIn();		
	}

	if(editable && (!parentActor || InGameMode() || InStandAloneMode())) //maks
	{
		KrEventManager::Instance()->AddListener( this );
		CreateCursor();

		if(bHasFocus) KrEventManager::Instance()->GrabKeyFocus( this );
	}

#ifndef STAND_ALONE_GAME 
	if(editable) textBox->setParentWidget(this);
#endif
}

void KrTextWidget::LineDown() //maks
{
	line++;
	currentWindowLine++;
	
	if(line >= textBox->NumLines()) line = textBox->NumLines() - 1;
	if(currentWindowLine >= textBox->WindowLines()) 
	{
		currentWindowLine = textBox->WindowLines() - 1;
		
		if(line <= textBox->NumLines() - 1) textBox->ScrollDown();
	}
}

void KrTextWidget::LineUp() //maks
{
	line--;
	currentWindowLine--;
	
	if(line < 0) line = 0;
	if(currentWindowLine < 0) 
	{
		currentWindowLine = 0;
		textBox->ScrollUp();
	}
}

int KrTextWidget::ScrollDown() //maks
{
	line = textBox->ScrollDown();
	currentWindowLine = 0;

	PositionCursor();

	return line;
}

int KrTextWidget::ScrollUp() //maks
{
	line = textBox->ScrollUp();
	currentWindowLine = 0;

	PositionCursor();

	return line;
}

int KrTextWidget::ScrollPageUp() //maks
{
	line -= textBox->ScrollPageUp();
	if(line < 0) line = 0;

	PositionCursor();

	return line;
}

int KrTextWidget::ScrollPageDown() //maks
{
	line += textBox->ScrollPageDown();
	if(line >= textBox->NumLines()) line = textBox->NumLines() - 1;

	PositionCursor();

	return line;
}


bool KrTextWidget::KeyEvent( const SDL_Event &event )
{
	if(!bEnable || !editable) return true; //maks

	if(Parent() && Parent()->getParentActor()) //maks
	{
		if(!InModalActor(Parent()->getParentActor()))
		{
			return true;
		}
	}

	Uint16 keycode; //maks: unicode error in symbian	
	
#ifdef __SYMBIAN32__
	keycode = event.key.keysym.sym;
#else
	keycode = event.key.keysym.unicode;
#endif
	
	if(!Engine()) return false; //maks
	if(event.type == SDL_KEYDOWN )
	{
#ifndef STAND_ALONE_GAME //maks
		StartAction();
#endif

		int length = textBox->GetLineClipLength( line ); //maks
		int lineAnt = line, cursorPosAnt = cursorPos; //maks
		GlDynArray<U16> text = textBox->GetText16Array( line ); //maks

		// Make sure the cursor position is in range.
		int cursorPosTemp = GlClamp( cursorPos, 0, length );
		GLASSERT( cursorPosTemp == cursorPos );
		cursorPos = cursorPosTemp;

		// Line processing keys:
		/*if ( (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT) && lineSelIni < 0 ) //maks
		{
			//Mark begin select
			lineSelIni = line;
			posSelIni = cursorPos + textOffSet;

			//GLOUTPUT("Begin selection\n");
		}
		else*/ if ( event.key.keysym.sym == SDLK_BACKSPACE ) //maks
		{

#ifndef STAND_ALONE_GAME
			int initialLine = textBox->getInitialSelectionLine(),
				initialPos = textBox->getInitialSelectionPos() - textOffSet,
				linesDeleted = textBox->RemoveSelectedText();
#else
			int initialLine = line,
				initialPos = cursorPos + textOffSet,
				linesDeleted = -1;
#endif

			if(linesDeleted == -1)
			{
				if(textOffSet > 0 && cursorPos == 0) 
				{
					text.Remove( textOffSet + cursorPos-1 );
					textBox->SetText16( text.Memory(), line );
					
					textOffSet--;
					textBox->SetTextOffSet(textOffSet);
				}
				
				if ( cursorPos > 0 )
				{
					text.Remove( textOffSet + cursorPos-1 );
					textBox->SetText16( text.Memory(), line ); 
					--cursorPos;				
				}
				
				
				if(textBox->GetLineClipLength( line ) == 0)
				{
					textOffSet = 0;
					textBox->SetTextOffSet(textOffSet);
					
					int i;
					for(i = line+1; i < textBox->NumLines(); i++)
					{
						textBox->SetText16(textBox->GetText16(i), i-1);
					}
					textBox->SetText("", i-1); //maks
				}
			}
			else if(initialLine != line || initialPos != cursorPos)
			{
				//Adjust cursor position
				cursorPos = initialPos;
				
				line -= linesDeleted;
				currentWindowLine -= linesDeleted;
				
				if(line < 0) line = 0;
				if(currentWindowLine < 0) currentWindowLine = 0;				
			} 
		} 
		else if ( event.key.keysym.sym == SDLK_DELETE ) //maks
		{
#ifndef STAND_ALONE_GAME
			int initialLine = textBox->getInitialSelectionLine(),
				initialPos = textBox->getInitialSelectionPos() - textOffSet,
				linesDeleted = textBox->RemoveSelectedText();
#else
			int initialLine = line,
				initialPos = cursorPos + textOffSet,
				linesDeleted = -1;
#endif

			if(linesDeleted == -1)
			{
				//Delete without selection
				if ( cursorPos < length )
				{
					text.Remove( textOffSet + cursorPos );
					textBox->SetText16( text.Memory(), line );
				}
				
				
				if(textBox->GetLineClipLength( line ) == 0)
				{
					int i;
					for(i = line+1; i < textBox->NumLines(); i++)
					{
						textBox->SetText16(textBox->GetText16(i), i-1);
					}
					textBox->SetText("", i-1); //maks
				}
			}
			else if(initialLine != line || initialPos != cursorPos)
			{
				//Adjust cursor position
				cursorPos = initialPos;
				
				line -= linesDeleted;
				currentWindowLine -= linesDeleted;
				
				if(line < 0) line = 0;
				if(currentWindowLine < 0) currentWindowLine = 0;				
			} 
		} 
		else if ( event.key.keysym.sym == SDLK_LEFT ) //maks
		{
			if(textOffSet > 0 && cursorPos == 0) 
			{
				textOffSet--;
				textBox->SetTextOffSet(textOffSet);				
			}

			if ( cursorPos > 0 )
				--cursorPos;
		}
		else if ( event.key.keysym.sym == SDLK_RIGHT ) //maks
		{
			int clip = textBox->GetLineClipLength( line ); 
			if ( cursorPos <= clip-1) //maks
			{
				++cursorPos;
			}
			else if(textOffSet < textBox->GetLineLength( line ) - clip) 
			{
				textOffSet++;
				textBox->SetTextOffSet(textOffSet);
			}			
		}
		/////////////////////////////////////////////////////////////
		//maks
		else if (multiLine &&  event.key.keysym.sym == SDLK_UP ) //maks
		{
			LineUp();			
		}
		else if (multiLine &&  event.key.keysym.sym == SDLK_DOWN ) //maks
		{
			LineDown();			
		}
		else if (multiLine &&  event.key.keysym.sym == SDLK_PAGEUP )
		{
			ScrollPageUp();			
		}
		else if (multiLine &&  event.key.keysym.sym == SDLK_PAGEDOWN )
		{
			ScrollPageDown();			
		}
		/////////////////////////////////////////////////////////////
		else if ( event.key.keysym.sym == SDLK_HOME )
		{
			cursorPos = 0;
			textOffSet = 0; //maks
			textBox->SetTextOffSet(0); //maks
		}
		else if ( event.key.keysym.sym == SDLK_END )
		{
			textOffSet = textBox->GetLineLength( line ) - textBox->GetLineClipLength( line ); //maks
			textBox->SetTextOffSet(textOffSet); //maks
			cursorPos = length;
		} 
		else if ( event.key.keysym.sym == SDLK_RETURN )
		{
			PublishEvent( ACTIVATED, 0, &event, 0, 0 );

			if(multiLine) //maks
			{
#ifndef STAND_ALONE_GAME
			int initialLine = textBox->getInitialSelectionLine(),
				initialPos = textBox->getInitialSelectionPos() - textOffSet,
				linesDeleted = textBox->RemoveSelectedText();
#else
			int initialLine = line,
				initialPos = cursorPos + textOffSet,
				linesDeleted = -1;
#endif
				//Remove selection
								
				if(linesDeleted >= 0 && (initialLine != line  || initialPos != cursorPos))
				{
					//Adjust cursor position
					cursorPos = initialPos;
					
					line -= linesDeleted;
					currentWindowLine -= linesDeleted;
					
					if(line < 0) line = 0;
					if(currentWindowLine < 0) currentWindowLine = 0;				
				} 

				//Copy
				int i = textOffSet + cursorPos, firstNonEmptyChar = 0;
				const U16 *str = textBox->GetText16(line);
				int l = textBox->GetLineLength(line);
				bool bSearchFirst = true;
				
				GlDynArray<U16> copy, ori;
				
				for(int i1 = 0; i1 < l; i1++)
				{
					if(i1 < i)
					{
						ori.PushBack(str[i1]);
					}
					else
					{
						copy.PushBack(str[i1]);
					}

					//Identation
					if(str[i1] == ' ' && bSearchFirst) 
					{
						if(TutorialIndentationCompatible()) //maks: avoid bug in old tutorials
						{
							firstNonEmptyChar++;
						}
					}
					else
					{
						bSearchFirst = false;
					}
				}

				if(l > 0 && str[l - 1] == '{')
				{
					if(TutorialIndentationCompatible()) //maks: avoid bug in old tutorials
					{
						//Increase identation
						firstNonEmptyChar += 4;
					}
				}


				//Delete
				ori.PushBack(0);
				copy.PushBack(0);
				textBox->SetText16(ori.Memory(), line);

				
				//Move lines
				cursorPos = firstNonEmptyChar; //Identation

				textOffSet = 0; 
				textBox->SetTextOffSet(textOffSet);
				
				
				//don't scroll down on last line ok
				line++; 
				currentWindowLine++;				
				if(currentWindowLine >= textBox->WindowLines()) 
				{
					currentWindowLine = textBox->WindowLines() - 1;	
					if(line <= textBox->NumLines() - 1) textBox->ScrollDown();
					else if(line == textBox->NumLines())
					{
						//Solve the bug:
						//"When press the enter key in last script editor line don't create a new line"

						GlDynArray<U16> emptyLine;
						emptyLine.PushBack(0);

						textBox->SetText16(emptyLine.Memory(), textBox->NumLines());
						textBox->ScrollDown();
					}
				}

				
				if(line >= textBox->NumLines()) line = textBox->NumLines() - 1;
				else
				{
					//insert line
							
					//Copy last line
					const U16 *str = textBox->GetText16(textBox->NumLines()-1);
					int l = textBox->GetText16Array(textBox->NumLines()-1).Count();
					
					GlDynArray<U16> copyLast;
					
					for(int i1 = 0; i1 < l; i1++)
					{
						copyLast.PushBack(str[i1]);
					}

					textBox->SetText16(copyLast.Memory(), textBox->NumLines());

					//Copy other lines
					int i;
					for(i = textBox->NumLines()-1; i > line; i--)
					{
						textBox->SetText16(textBox->GetText16(i-1), i);
					}

					//Identation
					for(i = 0; i < firstNonEmptyChar; i++)
					{
						copy.Insert(' ', 0);
					}

					textBox->SetText16(copy.Memory(), line);
				}
			}
		}
		else if ( keycode >= ' ' ) //maks
		{
#ifndef STAND_ALONE_GAME
			int initialLine = textBox->getInitialSelectionLine(),
				initialPos = textBox->getInitialSelectionPos() - textOffSet,
				linesDeleted = textBox->RemoveSelectedText();
#else
			int initialLine = line,
				initialPos = cursorPos + textOffSet,
				linesDeleted = -1;
#endif
			//Remove selection
			
			if(linesDeleted >= 0 && (initialLine != line || initialPos != cursorPos))
			{
				//Adjust cursor position
				cursorPos = initialPos;
				
				line -= linesDeleted;
				currentWindowLine -= linesDeleted;
				
				if(line < 0) line = 0;
				if(currentWindowLine < 0) currentWindowLine = 0;				
			}

			text = textBox->GetText16Array( line );

			//Insert char
			text.Insert( keycode, textOffSet + cursorPos ); //maks

			//Decrease indentation if the line has } as first character and have a open { before
			if(keycode == '}') //maks
			{
				//} is the first character on line?
				bool bFirst = false;
				int i;

				//Trim right
				for(i = text.Count() - 1; i >= 0; i--)
				{
					if(text[i] != ' ' && text[i] != 0)
					{
						text.Insert(0, i+1);
						text.SetCount(i + 2);
						break;
					}
				}
				
				for(i = 0; i < text.Count() - 1; i++)
				{
					if(text[i] != ' ' && text[i] != 0)
					{
						if(text[i] == '}') bFirst = true;
						break;
					}
				}

				if(bFirst)
				{
					//Copy the line after }
					GlDynArray<U16> copy;

					for(int ic = i; ic < text.Count(); ic++)
					{
						copy.PushBack(text[ic]);
					}

					//Find the last { position
					bool bFind = false;
					int closeFound = 0;

					for(i = line - 1; i >= 0; i--)
					{
						GlDynArray<U16> textLine = textBox->GetText16Array( i ); //maks
						

						for(int ic = 0; ic < textLine.Count(); ic++)
						{
							if(textLine[ic] == '{')
							{
								if(closeFound <= 0)
								{
									//Open found, insert blanks
									for(int ib = 0; ib < ic; ib++)
									{
										copy.Insert(' ', 0);
									}
									
									text = copy;
									cursorPos = text.Count() - 2 - textOffSet;
									if(cursorPos < 0) cursorPos = 0;
									bFind = true;
									break;
								}
								else
								{
									closeFound--;
								}
							}
							else if(textLine[ic] == '}')
							{
								closeFound++;
							}
						}

						if(bFind) break;
					}
				}
			}

			//const U16* glyph = textBox->GetText16(line);
			textBox->SetText16( text.Memory(), line ); //maks
			//glyph = textBox->GetText16(line);
			++cursorPos;
			
			if( cursorPos > textBox->GetLineClipLength( line ) ) //maks
			{
				textOffSet++;
				textBox->SetTextOffSet(textOffSet);
			}
			
			
			
			PublishEvent( 100, event.key.keysym.sym, &event, 0, 0 ); //maks
		}
		else
		{
			//			PublishEvent( KEY, 0, &event, 0, 0 );
			return false;	// we didn't handle.
		}

#ifndef STAND_ALONE_GAME //maks
			EndAction();
#endif

		PositionCursor();	

		//Key board selection
		if(event.key.keysym.sym == SDLK_LEFT ||
			 event.key.keysym.sym == SDLK_RIGHT ||
			 event.key.keysym.sym == SDLK_UP ||
			 event.key.keysym.sym == SDLK_DOWN ||
			 
			 event.key.keysym.sym == SDLK_PAGEUP ||
			 event.key.keysym.sym == SDLK_PAGEDOWN ||
			 event.key.keysym.sym == SDLK_HOME ||
			 event.key.keysym.sym == SDLK_END)
		{
			SDLMod keyMod = SDL_GetModState();
			//if(event.key.keysym.mod == KMOD_LSHIFT || event.key.keysym.mod == KMOD_RSHIFT) //Don't work with DirectX?
			if((keyMod & KMOD_LSHIFT) || (keyMod & KMOD_RSHIFT))
			{
#ifdef DEBUG
				GLOUTPUT("Shift selection\n");  
#endif

				if(lineSelIni == -1)				
				{
					lineSelIni = lineAnt;
					posSelIni = cursorPosAnt + textOffSet;
				}

				lineSelFin = line;
				posSelFin = cursorPos + textOffSet;

#ifndef STAND_ALONE_GAME
				textBox->SetSelectionRange(lineSelIni, posSelIni, lineSelFin, posSelFin);
#endif
			}
			else
			{
				lineSelIni = -1;

#ifndef STAND_ALONE_GAME
				textBox->ClearSelection();
#endif
			}
		}
		
		
		/*if(lineSelIni >= 0) //maks
		{
			lineSelFin = line;
			posSelFin = cursorPos + textOffSet;
			textBox->SetSelectionRange(lineSelIni, posSelIni, lineSelFin, posSelFin);			
		}*/

		return true;			// Almost all keys are handled.
	}
	/*else if(event.type == SDL_KEYUP )
	{
		if(event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT)
		{
			//No shift key: mark end select
			lineSelIni = -1;

#ifndef STAND_ALONE_GAME
			textBox->ClearSelection();
#endif

			//GLOUTPUT("End selection\n");
			
			return true;
		}
	}*/
	
	

	return false;
}


void KrTextWidget::PositionCursor() //maks
{
	if ( cursor && editable && Engine() )
	{
		// Get the position of the bottom line, and 
		// put the cursor there.
		int length = textBox->GetLineClipLength( line ); //maks
		int y      = textBox->GetLineY( currentWindowLine ); //maks
		int offset = textOffSet;

		// Make sure the cursor position is in range.
		cursorPos = GlClamp( cursorPos, 
							 0, length );

		if(cursorPos > textBox->GetLineLength(line) + offset)
		{
			//Prevention only, don't should goes here
			offset = 0;
		}

		int x = textBox->FontResource()->FontWidthN( textBox->GetText16( line ) + offset, cursorPos ); //maks

		//GLOUTPUT( "Cursor (%d,%d)\n", x, y );
		cursor->SetVisible(bHasFocus);
		cursor->SetPos( x, y );

		if(line != lineAnt) //maks
		{
			//Line changed
			PublishEvent( 101, 0, 0, 0, 0 ); 
		}

		lineAnt = line;
	}
}


void KrTextWidget::KeyFocus( bool receivingFocus )
{
	if(!bEnable || !editable) return; //maks

	if(cursor) cursor->SetVisible( receivingFocus );
	if ( receivingFocus )
		PublishEvent( SELECTION, 0, 0, 0, 0 );

	bHasFocus = receivingFocus;
}


void KrTextWidget::SetText( const gedString& t, int line ) //maks
{
	if ( textBox )
	{
		textBox->SetText( t, line ); //maks
		if(editable) cursorPos = textBox->GetLineClipLength( line ); //maks
		PositionCursor();
	}
}


void KrTextWidget::GetText( gedString* buffer, int line ) //maks
{
	if ( textBox && line >= 0 && line < textBox->NumLines())
		*buffer = textBox->GetText16( line );
}




void KrTextWidget::MouseIn( bool down, bool in )
{
	if(!bEnable || !editable) return;

	if ( in )
	{
		if(Parent() && Parent()->getParentActor()) //maks
		{
			if(!InModalActor(Parent()->getParentActor()))
			{
				return;
			}
		}
		
		holder->SetColor( scheme.CalcHiSec() );
	}
	else
	{
		KrColorTransform none;
		holder->SetColor( none );
	}
}

void KrTextWidget::MouseMove( bool down, int x, int y ) //maks
{
	if(!bEnable || !editable) return;

	if(down && lineSelIni >= 0)
	{
		if(y >= height - 1)
		{
			ScrollDown();
			y = height - 1;
		}
		else if(y <= 0)
		{
			ScrollUp();
			y = 0;
		}

		int antWindowLine = currentWindowLine; //maks
		currentWindowLine = textBox->WindowLines()*(float)y/height; //maks
		if(currentWindowLine < 0) currentWindowLine = 0; //maks
		else if(currentWindowLine >= textBox->WindowLines()) currentWindowLine = textBox->WindowLines() - 1; //maks
		
		line += currentWindowLine - antWindowLine; //maks
		if(line < 0) currentWindowLine = 0; //maks
		else if(line >= textBox->NumLines()) line = textBox->NumLines() - 1; //maks

		cursorPos = textBox->GetLineClipLength(line); //maks
		for( int i=0; i<textBox->GetLineClipLength(line); ++i ) //maks
		{
			int lineX = textBox->FontResource()->FontWidthN( textBox->GetText16( line ), i ); //maks
			if ( lineX >= x )
			{
				cursorPos = i;
				break;		
			}
		}
		PositionCursor();

		lineSelFin = line;
		posSelFin = cursorPos + textOffSet;

#ifndef STAND_ALONE_GAME
		textBox->SetSelectionRange(lineSelIni, posSelIni, lineSelFin, posSelFin);
#endif
	}
}

void KrTextWidget::MouseClick( int down, int x, int y ) //maks
{
	if(!bEnable || !editable) return;

	//Double click?
	bool bDoubleClick = false;

	if(down == KrWidget::LEFT_DOWN)
	{
		U32 timeClick = SDL_GetTicks();
		
		if(timeClick - lastMouseClick < 500)
		{
			bDoubleClick = true;
			lastMouseClick = 0;
		}
		else
		{		
			lastMouseClick = SDL_GetTicks();
		}
	}


	if(Parent() && Parent()->getParentActor()) //maks
	{
		if(!InModalActor(Parent()->getParentActor()))
		{
			return;
		}
	}

	bool selectionSent = false;
	if ( down == KrWidget::LEFT_DOWN && editable )
	{
		int antWindowLine = currentWindowLine; //maks

		//currentWindowLine = textBox->WindowLines()*(float)y/height; //maks: dont catch correct begin and end of lines
		currentWindowLine = y/textBox->GetHeightOfLine(); //maks

		if(currentWindowLine < 0) currentWindowLine = 0; //maks
		else if(currentWindowLine >= textBox->WindowLines()) currentWindowLine = textBox->WindowLines() - 1; //maks
		
		line += currentWindowLine - antWindowLine; //maks
		if(line < 0)
		{
			currentWindowLine = 0; //maks
			line = 0;
		}
		else if(line >= textBox->NumLines()) line = textBox->NumLines() - 1; //maks

		cursorPos = textBox->GetLineClipLength(line); //maks
		for( int i=0; i<textBox->GetLineClipLength(line); ++i ) //maks
		{
			//May be have variable font width
			const U16 *text = textBox->GetText16( line );

			int lineX = textBox->FontResource()->FontWidthN( text, i ),
				lastWidth = textBox->FontResource()->FontWidthN( &text[i], 1 ); 

			if(x >= lineX - lastWidth && x < lineX)
			{
				if(x > lineX - lastWidth/2)	cursorPos = i;
				else cursorPos = i - 1;

				break;		
			}
		}
		PositionCursor();

		KrEventManager::Instance()->GrabKeyFocus( this );
		selectionSent = true;
	}

	if (    down == KrWidget::LEFT_DOWN
		 && !selectionSent )		// don't double send selection if we sent one for grabbing focus
	{
		PublishEvent( SELECTION, 0, 0, 0, 0 );
	}

	if(down == KrWidget::RIGHT_DOWN)
	{
		//Right click
		KrVector2T< GlFixed > screen;
		ObjectToScreen(x, y, &screen/*, 0*/);

		PublishEvent( 102, screen.x.ToIntRound(), (SDL_Event *)screen.y.ToIntRound(), 0, 0 ); 
	}
	else
	{
		
		if(bDoubleClick)
		{
#ifndef STAND_ALONE_GAME
			SelectToken();
#endif
		}
		else
		{
			if(down == KrWidget::LEFT_DOWN)
			{
				lineSelIni = line;
				posSelIni = cursorPos + textOffSet;
			}
			else if(down == KrWidget::LEFT_UP && lineSelIni >= 0)
			{
				lineSelFin = line;
				posSelFin = cursorPos + textOffSet;

#ifndef STAND_ALONE_GAME
				textBox->SetSelectionRange(lineSelIni, posSelIni, lineSelFin, posSelFin);
#endif

				lineSelIni = -1;
			}
		}	
	}
}





int KrTextWidget::WindowLines() //maks
{
	return textBox->WindowLines();
} 

void KrTextWidget::Disable()
{
	bEnable = false;
	KrEventManager::Instance()->RemoveListener(this);
}

void KrTextWidget::setEditable(bool value) //maks
{
		  
	if(value && !editable)
	{
		editable = true; //Set editable before call listener

		textBox->SetAlignment(KrTextBox::LEFT);
		CreateCursor();
		KrEventManager::Instance()->AddListener( this );
	}
	else if(!value && editable)
	{
		editable = false; //Set editable before call listener
		textBox->SetAlignment(align);

		KrEventManager::Instance()->RemoveListener( this );
	}
	
	
} 

void KrTextWidget::CreateCursor() //maks
{
	if(!cursor)
	{
		// The cursor
		cursorRes = new KrBoxResource(	"KrTextWidget cursor",
			CURSOR_WIDTH, scheme.font->FontHeight(),
			&scheme.cursor, 1,
			KrBoxResource::FILL );
		
		cursor = new KrBox( cursorRes );
		cursor->SetVisible( false );
		//	cursor->SetColor( scheme.secondary );
		Engine()->Tree()->AddNode( textBox, cursor );
		cursor->SetZDepth(KrTextBox::CHILD_DEPTH + 512);
	}
}

int KrTextWidget::NumLines() //maks
{
	return textBox->NumLines();
}

#ifndef STAND_ALONE_GAME //maks

void KrTextWidget::SetSyntaxColorize(bool value) //maks
{
	if(parentActor) return; //Disable for Actor Text

	textBox->setSyntaxColorize(value);
} 

void KrTextWidget::SelectToken() //maks
{
	if(parentActor) return; //Disable for Actor Text

	gedString str;
	GlDynArray<U16> text = textBox->GetText16Array( line ); 

	for(int i = 0; i < text.Count(); i++)
	{
		str += text[i];
	}

	ListTokens tokens = GetTokens(str);
	int absoluteCursorPos = cursorPos + textOffSet;

	ListTokensIterator it(tokens);
	for(it.Begin(); !it.Done(); it.Next())
	{
		if(absoluteCursorPos >= it.Current().initialPosition &&
		   absoluteCursorPos <= it.Current().finalPosition)
		{
			//Slect the token

			lineSelIni = lineSelFin = line;
			posSelIni = it.Current().initialPosition;
			posSelFin = it.Current().finalPosition;

			textBox->SetSelectionRange(lineSelIni, posSelIni, lineSelFin, posSelFin);
			lineSelIni = -1;

			cursorPos = posSelFin - textOffSet;
			PositionCursor();
			break;
		}
	}
}

void KrTextWidget::InsertText(const gedString& t) //maks
{
	/*GlDynArray<U16> text = textBox->GetText16Array( line );

	for(int i = 0; i < t.length(); i++)
	{
		text.Insert( t[i], textOffSet + cursorPos++ );
	}

	textBox->SetText16( text.Memory(), line );
	PositionCursor();*/

	textBox->SetClipBoard(t);
	Paste();
}



void KrTextWidget::Copy() //maks
{
	textBox->Copy();
}

void KrTextWidget::Paste() //maks
{
	StartAction();

	int absoluteCursorPos = cursorPos + textOffSet, lineIni = line;
	textBox->Paste(line, currentWindowLine, absoluteCursorPos);
	
	cursorPos = absoluteCursorPos - textOffSet;								
	PositionCursor();

	EndAction();
}

void KrTextWidget::Cut() //maks
{
	StartAction();

	int initialLine = textBox->getInitialSelectionLine(),
					initialPos = textBox->getInitialSelectionPos() - textOffSet,
					linesDeleted = textBox->Cut();
				
	if(linesDeleted >= 0 && (initialLine != line  || initialPos != cursorPos))
	{
		//Adjust cursor position
		cursorPos = initialPos;
		
		line -= linesDeleted;
		currentWindowLine -= linesDeleted;
		
		if(line < 0) line = 0;
		if(currentWindowLine < 0) currentWindowLine = 0;
		
		PositionCursor();
	}
	
	EndAction();
}



void KrTextWidget::Accelerate( bool down, int keymod, int keysym ) //maks
{
	if(down)
	{
		switch(keysym)
		{
		case SDLK_c:
			Copy();
			break;

		case SDLK_v:
			Paste();
			break;

		case SDLK_x:
			Cut();
			break;

		case SDLK_z:
			DoUndo();
			break;

		case SDLK_r:
		case SDLK_y:
			DoRedo();
			break;
		}
	}
}

int KrTextWidget::ScrollTo(int lineIndex) //maks
{	
	line = textBox->ScrollTo(lineIndex);

	currentWindowLine = 0;
	PositionCursor();

	return line;
}

void KrTextWidget::StartAction()
{
	m_currentUndoFrame.Clear();	
}

void KrTextWidget::EndAction()
{
	if (m_currentUndoFrame.m_actions.Empty())
	{
		// nothing here.
		return;
	}

	ClearUndoStack(m_redoStack);
	
	m_undoStack.PushFront(m_currentUndoFrame);
	m_currentUndoFrame.Clear();
}

void KrTextWidget::ClearUndoStack(undostack &inStack)
{
   while (!inStack.Empty())
   {
      inStack.PopFront();
   }
}

void KrTextWidget::ClearUndoHistory()
{
   m_currentUndoFrame.Clear();
   ClearUndoStack(m_redoStack);
   ClearUndoStack(m_undoStack);
}

void KrTextWidget::DoUndo()
{
   DoUndoRedo(m_undoStack, m_redoStack);
}



void KrTextWidget::DoRedo()
{
   DoUndoRedo(m_redoStack, m_undoStack);
}

void KrTextWidget::PushUndo(int lineNum)
{
	if(!editable) return;

	CUndoAction act;
	act.m_actionType = CUndoAction::eChangeLine;
	GetText(&act.m_csText, lineNum);
	act.m_iLineNumber = lineNum;
	act.m_cursorPos.cursorPos = cursorPos;
	act.m_cursorPos.line = line;
	act.m_cursorPos.currentWindowLine = currentWindowLine;
	//act.m_anchorPos = AnchorPos();
	m_currentUndoFrame.Add(act);
}

void KrTextWidget::DoUndoRedo(undostack &inStack, undostack& outStack)
{
	CUndoFrame inFrame;
	if (inStack.Empty())
	{
		return; // nothing to undo
	}

	WaitCursor wait;
	
	inFrame = inStack.Front();
	inStack.PopFront();
	
	
	// put the undo/redo actions here. undo actions become redo actions and vice versa
	CUndoFrame outputFrame;
	
	int scrollPos = -1;
	
	CUndoAction action;
	while (inFrame.GetNext(action))
	{
		// show the user what's going on
		ScrollTo(action.m_iLineNumber);
		
		textBox->setParentWidget(NULL);
		
		CUndoAction outputAction;
		switch (action.m_actionType)
		{
		case CUndoAction::eAddLine:
			outputAction.m_actionType  = CUndoAction::eRemoveLine;
			GetText(&outputAction.m_csText, action.m_iLineNumber);
			
			SetText("", action.m_iLineNumber);
			break;
			/* case CUndoAction::eRemoveLine:
			outputAction.m_actionType  = CUndoAction::eAddLine;
			outputAction.m_csText      = "";
			
			  InsertTextLine(action.m_csText, action.m_iLineNumber, true, true);
			break;*/
			
		case CUndoAction::eChangeLine:
			outputAction.m_actionType  = CUndoAction::eChangeLine;
			GetText(&outputAction.m_csText, action.m_iLineNumber);
			
			SetText(action.m_csText, action.m_iLineNumber);
			break;
		}
		
		textBox->setParentWidget(this);
		
		outputAction.m_cursorPos.cursorPos = action.m_cursorPos.cursorPos;
		outputAction.m_cursorPos.line = action.m_cursorPos.line;
		outputAction.m_cursorPos.currentWindowLine = action.m_cursorPos.currentWindowLine;
		//outputAction.m_anchorPos   = AnchorPos();
		outputAction.m_iLineNumber = action.m_iLineNumber;
		
		// reset the positioning
		cursorPos = action.m_cursorPos.cursorPos;
		if(&inStack == &m_redoStack) cursorPos++;

		currentWindowLine = action.m_cursorPos.currentWindowLine;
		scrollPos = action.m_cursorPos.line;
		
		//_SetAnchorPos(action.m_anchorPos);
		
		outputFrame.Add(outputAction);
	}
	
	outStack.PushFront(outputFrame);   

	if(scrollPos != -1)
	{
		textBox->ScrollTo(scrollPos - currentWindowLine);
		line = scrollPos;
		PositionCursor();
	}
}

gedString KrTextWidget::GetTextUntilCursor() //maks
{
	//Return all text until current cursor position
	gedString buffer;

	if (textBox)
	{
		int absoluteCursorPos = cursorPos + textOffSet,
			lastLineLenght = textBox->GetLineLength(line);

		if(absoluteCursorPos > lastLineLenght)
		{
			absoluteCursorPos = lastLineLenght;
		}

		for(int i = 0; i <= line; i++)
		{
			if( i != line)
			{
				buffer += textBox->GetText16( i );
			}
			else
			{
				//Get only until current cursor position
				for(int pos = 0; pos < absoluteCursorPos; pos++)
				{
					buffer += textBox->GetText16( i )[pos];
				}
			}

			if(i < line - 1) buffer += "\n";
		}		
	}

	return buffer;
}

#endif //#ifndef STAND_ALONE_GAME //maks
