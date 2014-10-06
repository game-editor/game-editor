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

#include "SDL.h"
#include "textbox.h"
#include "sprite.h"
#include "engine.h"
#include "canvas.h" //maks
#include "../gui/textwidget.h" //maks

#include "../../gameEngine/dlmalloc.h" //maks

extern "C" int GetGameMode(); //maks

#ifndef STAND_ALONE_GAME

#define NO_LONG_LONG
#define NO_DIRENT
#define NO_FCNTL
#define NO_STAT
#define NO_UNISTD
#define NO_TERMIOS
#define NO_PIPE
#define NO_POSIX
#define NO_HISTORY



extern "C"
{
void getTokenColor(const char *str, int lastToken, int *currentToken, int *r, int *g, int *b);
#include "scrap.h"
}

ArrayString KrTextBox::clipBoard;

typedef GlSList<Token> ListTokens;  //maks
typedef GlSListIterator<Token> ListTokensIterator; //maks
extern ListTokens GetTokens(gedString str); //maks
#endif



#ifndef STAND_ALONE_GAME 
bool KrTextBox::bSystemClipBoardInited = false;
bool KrTextBox::bHasSystemClipBoard = false;
#endif

KrTextBox::KrTextBox(	KrFontResource* _resource,
						int _width, int _height,
						int _lineSpacing,
						Alignment _align,
						void *parent) //maks
{
#ifndef STAND_ALONE_GAME 
	//if(!bSystemClipBoardInited) //Always update the scrap info (solve the bug when try use function insertion after change the window size on Linux)
	{
		if(init_scrap() >= 0)
		{
#ifndef linux
			bHasSystemClipBoard = true;
#endif
		}

		bSystemClipBoardInited = true;
	}
#endif

	resource = _resource;
	width = _width;
	height = _height;
	numLines = 0;
	//line = 0; //maks
	align = _align;
	lineSpacing = _lineSpacing;
	this->parentActor = parent; //maks
	isText = true; //maks
	verticalScrollAnt = verticalScroll = textOffSetAnt = textOffSet = 0; //maks

	GLASSERT( _resource );

	// Create the array of TextLines.
	sizeOfLine = lineSpacing + resource->FontHeight();
	numLines = height / sizeOfLine; 

	windowLines = numLines;
	if(numLines > 1) numLines++; //don't scroll down on last line ok

	if ( numLines > 0 )
	{
		line.Resize(numLines); //maks //line = new TextLine[ numLines ];
		line.SetCount(numLines); //maks
		/*for ( int i=0; i<numLines; i++ ) //maks
		{
			//line[i].dy = i * sizeOfLine;
			line[i].str.PushBack( 0 );		// use a null terminator
			line[i].parent = 0;
			line[i].width = 0;
		}*/
	}

	

#ifndef STAND_ALONE_GAME 
	lineIni = posIni = lineFin = posFin = -1; //maks
	bSyntaxColorize = false;
	parentWidget = NULL;
#endif
}


KrTextBox::~KrTextBox()
{
	// Do not delete the child letters. They will be cleaned by the
	// tree.
	//delete [] line; //maks
}

void KrTextBox::SetAlignment(Alignment _align) //maks
{
	if(_align != align)
	{
		align = _align;

		for( int i=0; i<numLines; ++i ) 
		{
			//Remove old letters
			line[i].letter.Clear();
			if ( line[i].parent )
			{
				Engine()->Tree()->DeleteNode( line[i].parent );
				line[i].parent = 0;
			}

			//Create new
			CreateLetters( i );
		}
	}
}

int KrTextBox::ScrollDown() //maks
{
	//Return the first visible line

	if(verticalScroll + windowLines >= numLines) return verticalScroll;
	
	verticalScroll++;

	//Delete anterior lines
	int i;
	for(i=verticalScrollAnt; i < verticalScrollAnt + windowLines && i < numLines; i++ ) 
	{
		line[i].letter.Clear();
		if ( line[i].parent )
		{
			Engine()->Tree()->DeleteNode( line[i].parent );
			line[i].parent = 0;
		}
	}
	

	for(i=verticalScroll; i < verticalScroll + windowLines && i < numLines; i++ ) 
	{
		CreateLetters(i);
	}

	verticalScrollAnt = verticalScroll;

	return verticalScroll;
}



int KrTextBox::ScrollUp() //maks
{
	//Return the first visible line
	if(verticalScroll <= 0) return verticalScroll;
	
	verticalScroll--;

	//Delete anterior lines
	int i;
	for(i=verticalScrollAnt; i < verticalScrollAnt + windowLines && i < numLines; i++ ) 
	{
		line[i].letter.Clear();
		if ( line[i].parent )
		{
			Engine()->Tree()->DeleteNode( line[i].parent );
			line[i].parent = 0;
		}
	}

	for(i=verticalScroll; i < verticalScroll + windowLines && i < numLines; i++ ) 
	{
		CreateLetters(i);
	}

	verticalScrollAnt = verticalScroll;

	return verticalScroll;
}

int KrTextBox::ScrollPageDown() //maks
{
	if(verticalScroll + windowLines >= numLines) return 0;
	
	verticalScroll += windowLines;
	if(verticalScroll + windowLines > numLines) verticalScroll = numLines - windowLines;

	//Delete anterior lines
	int i;
	for(i=verticalScrollAnt; i < verticalScrollAnt + windowLines && i < numLines; i++ ) 
	{
		line[i].letter.Clear();
		if ( line[i].parent )
		{
			Engine()->Tree()->DeleteNode( line[i].parent );
			line[i].parent = 0;
		}
	}
	

	for(i=verticalScroll; i < verticalScroll + windowLines && i < numLines; i++ ) 
	{
		CreateLetters(i);
	}

	int dl = verticalScroll - verticalScrollAnt;
	verticalScrollAnt = verticalScroll;

	return dl;
}

int KrTextBox::ScrollPageUp() //maks
{
	if(verticalScroll <= 0) return 0;
	
	verticalScroll -= windowLines;
	if(verticalScroll < 0) verticalScroll = 0;

	//Delete anterior lines
	int i;
	for(i=verticalScrollAnt; i < verticalScrollAnt + windowLines && i < numLines; i++ ) 
	{
		line[i].letter.Clear();
		if ( line[i].parent )
		{
			Engine()->Tree()->DeleteNode( line[i].parent );
			line[i].parent = 0;
		}
	}

	for(i=verticalScroll; i < verticalScroll + windowLines && i < numLines; i++ ) 
	{
		CreateLetters(i);
	}

	int dl = abs(verticalScroll - verticalScrollAnt);
	verticalScrollAnt = verticalScroll;

	return dl;
}

void KrTextBox::GetText( gedString* buffer, int lineNum ) //maks
{
	/*const U16* p = line[lineNum].str.Memory();
	*buffer = "";
	//int i = 0;

	if ( lineNum >=0 && lineNum < numLines )
	{
		while ( *p )
		{	
			*buffer += *p;
			++p;
		}
	}*/

	*buffer = line[lineNum].str.Memory();
}

void KrTextBox::SetText( const gedString& text, int lineNum ) //maks
{
	if(text.IsUnicode())
	{
		SetText16(text.getUnicodeBuf(), lineNum);
		return;
	}

#ifndef STAND_ALONE_GAME //maks
	if(parentWidget)
	{
		parentWidget->PushUndo(lineNum);
	}
#endif

	if ( lineNum >=0 /*&& lineNum < numLines*/ ) //maks
	{
		if(lineNum >= numLines) //maks
		{
			//Create lines
			int newCount = lineNum + 1;

			line.Resize(newCount); //maks //line = new TextLine[ numLines ];
			line.SetCount(newCount); //maks

			/*for(int i = numLines; i < newCount; i++)
			{
				//line[i].dy = i*sizeOfLine;
				line[i].str.PushBack( 0 );		// use a null terminator
				line[i].parent = 0;
				line[i].width = 0;
			}*/

			numLines = newCount;
		}

		if ( !line[lineNum].str.Empty() && textOffSet == textOffSetAnt && verticalScroll == verticalScrollAnt)
		{			
			// Check if the line hasn't changed:
			if (text.getCharBuf())
			{
				const U16 *p;
				CHAR_TYPE *q;
				for( p = line[lineNum].str.Memory(), q = text.getCharBuf();
					 *p && *q;
					 ++p, ++q )
				{
					if ( *p != *q )
						break;
				}
				// They are the same! Do nothing.
				if ( *p == 0 && *q == 0 )
					return;
			}
		}

		// Remove the existing letters, then add the new ones.
		//maks: Don't remove to avoid mallocs. Just hide the letters		
		/*if(!GetGameMode() && Engine())
		{
			line[lineNum].letter.Clear();
			if ( line[lineNum].parent )
			{
				Engine()->Tree()->DeleteNode( line[lineNum].parent );
				line[lineNum].parent = 0;
			}
		}*/

		//maks: avoid bug when edit lines
		if(Engine())
		{
			for(int i = 0; i < line[lineNum].letter.Count(); i++)
			{
				if(line[lineNum].letter[i]) line[lineNum].letter[i]->SetVisible(DISABLE/*, 0*/);
			}
		}

		int textLength = text.size();
		

		if(text.getCharBuf())
		{
			if(clipLenght.Count() <= lineNum) //maks
			{
				clipLenght.SetCount(lineNum+1);
			}

			int offSet = 0; 
			//maks: offSet = textOffSet was used to try solve the text input bug
			//when the ttf text goes beyond text box.
			//But this can crash when type a text in Script Editor beyond text box.
			//So, don't use now

			int w = resource->FontWidth( text.getCharBuf() + offSet);
			if ( w > width) //maks: don't limit length
			{
				// The line doesn't fit in the bounding area. Truncate.
				// It could be any length...so start from the front and
				// see how much we can fit.
				int n = 0;
				for( n=0; n < textLength - offSet; n++ )
				{
					if ( resource->FontWidthN( text.getCharBuf() + offSet, n ) > width)
					{
						break;
					}
				}

				if(resource->getFontType() == KrFontResource::FIXED) //maks
				{
					if ( n > 0 ) 
						--n;
				}

				//textLength = n; //maks: don't limit length
				clipLenght[lineNum] = n; //maks: Cliped text lenght
			}
			else clipLenght[lineNum] = textLength - offSet; //maks

			// We have the text length. Copy it to string.
			line[lineNum].str.SetCount( textLength );			
			for ( int i=0; i<textLength; i++ )
			{
				line[lineNum].str[i] = text[i];

			}
		}
		else
		{
			line[lineNum].str.SetCount( 0 );
		}

		// Add a null terminator, since we pass out 16bit strings.
		line[lineNum].str.PushBack( 0 );

		if ( Engine() )
		{
			CreateLetters( lineNum );
		}
// 		Invalidate();
	}
}

void KrTextBox::SetText16( const U16* text, int lineNum )
{
#ifndef STAND_ALONE_GAME //maks
	if(parentWidget)
	{
		parentWidget->PushUndo(lineNum);
	}
#endif

	if ( lineNum >=0 /*&& lineNum < numLines*/ ) //maks
	{
		if(lineNum >= numLines) //maks
		{
			//Create lines
			int newCount = lineNum + 1;

			line.Resize(newCount); //maks //line = new TextLine[ numLines ];
			line.SetCount(newCount); //maks

			/*for(int i = numLines; i < newCount; i++)
			{
				//line[i].dy = i*sizeOfLine;
				line[i].str.PushBack( 0 );		// use a null terminator
				line[i].parent = 0;
				line[i].width = 0;
			}*/

			numLines = newCount;
		}

		if ( !line[lineNum].str.Empty() && textOffSet == textOffSetAnt && verticalScroll == verticalScrollAnt)
		{			
			// Check if the line hasn't changed:
			if ( text && *text )
			{
				const U16 *p, *q;
				for( p = line[lineNum].str.Memory(), q = text;
					 *p && *q;
					 ++p, ++q )
				{
					if ( *p != *q )
						break;
				}
				// They are the same! Do nothing.
				if ( *p == 0 && *q == 0 )
					return;
			}
		}

		// Remove the existing letters, then add the new ones.
		//maks: Don't remove to avoid mallocs. Just hide the letters		
		/*if(!GetGameMode() && Engine())
		{
			line[lineNum].letter.Clear();
			if ( line[lineNum].parent )
			{
				Engine()->Tree()->DeleteNode( line[lineNum].parent );
				line[lineNum].parent = 0;
			}
		}*/

		//maks: avoid bug when edit lines
		if(Engine())
		{
			for(int i = 0; i < line[lineNum].letter.Count(); i++)
			{
				if(line[lineNum].letter[i]) line[lineNum].letter[i]->SetVisible(DISABLE/*, 0*/);
			}
		}

		int textLength = 0;
		for ( const U16* pt = text; pt && *pt; ++pt )
		{
			++textLength;
		}
		if ( text && *text )
		{
			if(clipLenght.Count() <= lineNum) //maks
			{
				clipLenght.SetCount(lineNum+1);
			}

			int offSet = 0; 
			//maks: offSet = textOffSet was used to try solve the text input bug
			//when the ttf text goes beyond text box.
			//But this can crash when type a text in Script Editor beyond text box.
			//So, don't use now

			int w = resource->FontWidth( text + offSet);
			if ( w > width) //maks: don't limit length
			{
				// The line doesn't fit in the bounding area. Truncate.
				// It could be any length...so start from the front and
				// see how much we can fit.
				int n = 0;
				for( n=0; n < textLength - offSet; n++ )
				{
					if ( resource->FontWidthN( text + offSet, n ) > width)
					{
						break;
					}
				}

				if(resource->getFontType() == KrFontResource::FIXED) //maks
				{
					if ( n > 0 ) 
						--n;
				}

				//textLength = n; //maks: don't limit length
				clipLenght[lineNum] = n; //maks: Cliped text lenght
			}
			else clipLenght[lineNum] = textLength - offSet; //maks

			// We have the text length. Copy it to string.
			line[lineNum].str.SetCount( textLength );			
			for ( int i=0; i<textLength; i++ )
			{
				line[lineNum].str[i] = text[i];

			}
		}
		else
		{
			line[lineNum].str.SetCount( 0 );
		}
		// Add a null terminator, since we pass out 16bit strings.
		line[lineNum].str.PushBack( 0 );
		
		if ( Engine() )
		{
			CreateLetters( lineNum );
		}
// 		Invalidate();
	}
}


void KrTextBox::AddedtoTree()
{
	KrImNode::AddedtoTree();			// Calculate the transform, invalidate.
	for( int i=0; i<numLines; ++i ) //maks
	{
		CreateLetters( i );
	}
}

void KrTextBox::CreateLetters( int index )
{
	if(index < verticalScroll || index >= verticalScroll + windowLines) return; //maks

	GLASSERT( Engine() );
	KrAction* action = resource->GetAction( sNONE );
	GLASSERT( action );

	int oldCount = line[index].letter.Count(), 
		newCount = line[index].str.Count() - 1,
		i; //maks

	if(newCount > oldCount) line[index].letter.SetCount( newCount ); //maks
	line[index].width = resource->FontWidth( line[index].str.Memory() ); //maks

	//Reset new letters
	for(i = oldCount; i < line[index].letter.Count(); i++)
	{
		line[index].letter[i] = 0;
	}


	// Very important to set the offset as we go, else
	// get an extra set of DRs.
	int offset = 0;
	if ( align != LEFT )
	{
		if ( align == CENTER )
		{
			offset = ( width - line[index].width ) / 2;
		}
		else if ( align == RIGHT )
		{
			offset = width - line[index].width;
		}
	}

	if ( line[index].str.Count() > 1 )		// the null-term gives 1 character of length
	{
		// Check for the parent node:
		if ( !line[index].parent )
		{
			line[index].parent = new KrImNode();
			line[index].parent->SetZDepth( CHILD_DEPTH + index );
			/*#ifdef DEBUG
				char buf[256];
				static int id = 0;
				sprintf( buf, "parent index=%d id=%d", index, ++id );
				line[index].parent->SetNodeName( buf );
				//GLOUTPUT( "About to add parent '%s'\n", line[index].parent->NodeName().c_str() );
			#endif*/
			
			Engine()->Tree()->AddNode( this, line[index].parent );
			line[index].parent->SetPos( offset, index*sizeOfLine /*line[index].dy*/ - verticalScroll*sizeOfLine); //maks
//			GLOUTPUT( "Line Added. name='%s' xf=%f,%f cxf=%f,%f\n",
//					  line[index].parent->NodeName().c_str(),
//					  line[index].parent->XTransform( 0 ).x.ToDouble(),
//					  line[index].parent->XTransform( 0 ).y.ToDouble(),
//					  line[index].parent->CompositeXForm( 0 ).x.ToDouble(),
//					  line[index].parent->CompositeXForm( 0 ).y.ToDouble() );
//			GLOUTPUT( "  ...Textbox.         xf=%f,%f cxf=%f,%f\n",
//					  NodeName().c_str(),
//					  XTransform( 0 ).x.ToDouble(),
//					  XTransform( 0 ).y.ToDouble(),
//					  CompositeXForm( 0 ).x.ToDouble(),
//					  CompositeXForm( 0 ).y.ToDouble() );
		}

		int  count = line[index].str.Count() - 1;	// The string is null terminated. Don't need the last character.
		U16* glyph = line[index].str.Memory();
		int  x = 0;

		int clip = GetLineClipLength(index); //maks
		if(count - textOffSet > clip) //maks
			count = clip + textOffSet;

		int i;
		for(i=textOffSet; i < count; ++i ) //maks
		{
			if ( resource->GlyphInFont( glyph[i] ) )
			{
				// It's rather important to not add an empty sprite:
				// happens when a "space" is used.
				if ( action->Frame( resource->GlyphToFrame( glyph[i] ) ).Width() > 0 ) //maks
				{
					KrSprite *sprite = line[index].letter[i];

					if(!sprite)
					{
						sprite = new KrSprite( resource, parentActor ); 
						line[index].letter[i] = sprite;
						Engine()->Tree()->AddNode( line[index].parent, sprite );
					}

					sprite->SetHitTest(parentActor != NULL); //maks
					sprite->SetFrame( resource->GlyphToFrame( glyph[i] ) );
										
					/*#ifdef DEBUG
						char buf[256];
						static int id=0;
						sprintf( buf, "letter glyph='%c' id=%d", glyph[i], ++id );
						sprite->SetNodeName( buf );
						//GLOUTPUT( "adding letter '%s'\n", line[index].letter[i]->NodeName().c_str() );
					#endif*/

					sprite->SetVisible(ENABLE); //maks
					sprite->SetPos( x, 0 ); //maks


					sprite->SetQuality( KrQualityFast ); // Optimization; this is the only setting.

					sprite->SetZDepth( i );
					//line[index].letter[i]->SetNodeId((int)line[index].letter[i]); //maks: safe text select?
				}
				else if(i < line[index].letter.Count()) //maks
				{
					if(line[index].letter[i]) line[index].letter[i]->SetVisible(DISABLE); //maks
				}

				x += resource->FontWidth1( glyph[i] );
			}
			else
			{
				if(line[index].letter[i]) line[index].letter[i]->SetVisible(DISABLE); //maks
				// Need to advance x if it is a space.
				x += resource->FontWidth1( glyph[i] );
			}
		}

		//maks: Reset others letters. Don't remove to reduce mallocs
		int maxLetter = GlMin(textOffSet, (int)line[index].letter.Count());
		for(i = 0; i < maxLetter; i++)
		{
			if(line[index].letter[i]) line[index].letter[i]->SetVisible(DISABLE); //maks
		}

		for(i = count; i < line[index].letter.Count(); i++)
		{
			if(line[index].letter[i]) line[index].letter[i]->SetVisible(DISABLE); //maks
		}

#ifndef STAND_ALONE_GAME //maks
		SyntaxColorize(index);
#endif
	}
}



void KrTextBox::SetTextOffSet(int offSet) //maks
{
	textOffSet = offSet;

	for( int i=0; i<numLines; ++i ) 
	{
		const U16 *str = GetText16(i);
		int l = line[i].str.Count();

		GlDynArray<U16> copy;
		
		for(int i1 = 0; i1 < l; i1++)
		{
			copy.PushBack(str[i1]);
		}

		SetText16(copy.Memory(), i);
	}

	textOffSetAnt = textOffSet;
}




void KrTextBox::FlushInvalid( /*int win,*/ bool cache )
{
	// Although text is nothing but a bunch of sprites -- which could
	// manage their own bounds -- they shoot the DR processing. 
	// "pre load" a DR, which will consume the individual letters.
	//
	// Note this can be conservative -- if it misses, the letters
	// can handle themselves.
	KrImNode::FlushInvalid( /*win,*/ cache );

	if ( IsVisible(/*win*/) )
	{
		for( int i=0; i<numLines; ++i )
		{
			KrRect bounds;
			bool first = true;
			KrImNode* parent = line[i].parent;

			if ( parent )
			{
				for( 	GlInsideNode< KrImNode* >* childNode = parent->Child()->next;		// start after the sentinel
						childNode != parent->Child();     								// look for the sentinel
						childNode = childNode->next )
				{
					if ( first )
					{
						bounds = childNode->data->Bounds( /*win*/ );
						first = false;
					}
					else
					{
						bounds.DoUnion( childNode->data->Bounds( /*win*/ ) );
					}
				}
				if ( !first )
					Engine()->DirtyRectangle(/*win*/)->AddRectangle( bounds );
			}
		}		
	}
}


KrImNode* KrTextBox::Clone()
{
	KrTextBox* box = new KrTextBox( resource, width, height, lineSpacing, align );
	gedString buf;

	for( int i=0; i<numLines; ++i )
		box->SetText16( GetText16( i ), i );
	return box;
}

/*KrRect& KrTextBox::CalculatedBounds(int window) //maks
{
	KrRect	fisrt = line[0].letter[0]->Bounds(window),
			last = line[numLines-1].letter[line[numLines-1].letter.Count() - 1]->Bounds(window);
	
	calculatedBounds.xmin = fisrt.xmin;
	calculatedBounds.ymin = fisrt.ymin;

	calculatedBounds.xmax = last.xmax;
	calculatedBounds.ymax = last.ymax;

	return calculatedBounds;
}*/

#ifndef STAND_ALONE_GAME //maks

KrImage *KrTextBox::CreateSelecionMark(KrImage *parent) //maks
{
	static KrCanvasResource *markResource = NULL;
	if(markResource == NULL)
	{
		int width = resource->FontWidth("A"),
			height = resource->FontHeight();

		markResource = new KrCanvasResource("_gedSelectionMark", 
			width,
			height,
			true );
		
		//Create a tansparent actor
		KrRGBA *pixels = markResource->Pixels(), color;
		
		color.Set(0, 0, 0, 60);
		
		//Fill background
		int i;
		for(i = 0; i < width*height; i++)
		{
			pixels[i] = color;
		}
		
		//Add
		Engine()->Vault()->AddResource(markResource);
	}	
	

	KrCanvas *mark = new KrCanvas( markResource );
	mark->SetNodeId((int)mark);
	Engine()->Tree()->AddNode( parent, mark );
	
	KrColorTransform color(parent->CTransform());

	if(bSyntaxColorize) color.Set(0, color.b.c.red, 0, color.b.c.green, 0, color.b.c.blue, 255); // would be normal highlight 
	else color.Set(255, 0, 255, 0, 0, 0, 255); // non-script highlighted color 

	parent->SetColor(color);

	return mark;
}

void KrTextBox::ClearSelection() //maks
{
	while(!listSelectedLetters.Empty())
	{
		KrImage *letter = listSelectedLetters.Front();
		listSelectedLetters.PopFront();

		if(Engine()->Tree()->FindNodeById((int)letter))
		{
			if(bSyntaxColorize)
			{
				KrColorTransform color(letter->Parent()->CTransform());
				color.Set(7*color.b.c.red, color.b.c.red, 7*color.b.c.green, color.b.c.green, 7*color.b.c.blue, color.b.c.blue, 255);
				letter->Parent()->SetColor(color);
			}
			else
			{
				KrColorTransform none;
				letter->Parent()->SetColor(none);
			}

			Engine()->Tree()->DeleteNode(letter);
		}	
	}

	lineIni = posIni = lineFin = posFin = -1;
}

void KrTextBox::SetSelectionRange(int _lineIni, int _posIni, int _lineFin, int _posFin) //maks
{
	if(parentActor) return; //Disable for Actor Text
	ClearSelection();

	int l1 = _lineIni, l2 = _lineFin, p1 = _posIni, p2 = _posFin;

	if(_lineIni < _lineFin)
	{
		l1 = _lineIni;
		p1 = _posIni;

		l2 = _lineFin;
		p2 = _posFin;
	}
	else if(_lineIni > _lineFin)
	{
		l1 = _lineFin;
		p1 = _posFin;

		l2 = _lineIni;
		p2 = _posIni;
	}
	else if(p2 < p1)
	{
		p1 = _posFin;
		p2 = _posIni;
	}

	if(l1 < 0) l1 = 0;
	else if(l1 >= numLines) l1 = numLines - 1;

	if(l2 < 0) l2 = 0;
	else if(l2 >= numLines) l2 = numLines - 1;
	
	lineIni = l1;
	lineFin = l2;
	posIni = p1;
	posFin = p2;

	

#ifdef DEBUG
	GLOUTPUT("Selection: %ld, %ld - %ld, %ld\n", l1, p1, l2, p2);
#endif


	for(int i = l1; i <= l2; ++i) 
	{
		int lineLen = line[i].letter.Count();
		int pi = (i == l1)?p1:0;
		int pf = (i == l2)?p2:lineLen;

		if(l1 == l2 && pi > pf)
		{
			int aux = pi;
			pi = pf;
			pf = aux;
		}

		if( pf > lineLen) pf = lineLen;

		for(int pos = pi; pos < pf; pos++)
		{
			KrSprite *letter = line[i].letter[pos];

			if(letter)
			{
				listSelectedLetters.PushBack(CreateSelecionMark(letter));
			}
		}
	}
}

void KrTextBox::SetClipBoard(const gedString& str) //maks
{
	clipBoard.Clear();
	clipBoard.SetCount(1);
	clipBoard[0] = str;

	CopyToSystemClipBoard();
}

void KrTextBox::Copy() //maks
{
	//Copy current selection to buffer
	clipBoard.Clear();

	if(lineIni < 0 || lineFin < 0 || (lineIni == lineFin && posIni == posFin)) return;

	clipBoard.SetCount(lineFin - lineIni + 1);

	int nLines = 0;

	for(int i = lineIni; i <= lineFin; ++i) 
	{
		int lineLen = line[i].str.Count() - 1;
		int pi = (i == lineIni)?posIni:0;
		int pf = (i == lineFin)?posFin:lineLen;
		gedString str;

		if( pf > lineLen) pf = lineLen;

		for(int pos = pi; pos < pf; pos++)
		{
			str += line[i].str[pos];
		}

		clipBoard[nLines] = str;
		nLines++;
	}

	CopyToSystemClipBoard();
}

bool KrTextBox::ClipBoardCopyOk()
{
	int scraplen;
	char *buf = NULL;
	get_scrap(T('T','E','X','T'), &scraplen, &buf);
	
	if(scraplen && buf)
	{
		int nLines = 0, ini = 0, i, len = strlen(buf);
		
		for(i = 0; i < len; i++)
		{
			if(buf[i] == '\n' || buf[i] == '\r')
			{
				buf[i] = '\n';
				if(i + 1 < len)
				{
					if(buf[i+1] == '\n' || buf[i+1] == '\r')
					{
						i++;
					}
				}
				
				nLines++;
			}
		}
		
		nLines = 0;
		
		for(i = 0; i < len; i++)
		{
			if(buf[i] == '\n')
			{
				buf[i] = 0;
				if(i + 1 < len)
				{
					if(buf[i+1] == '\n')
					{
						i++;
					}
				}
				
				if(strlen(&buf[ini]))
				{
					if(nLines >= clipBoard.Count() || clipBoard[nLines] != &buf[ini])
					{
						free(buf);
						return false;
					}
				}
				
				nLines++; //Increment on empty lines too
				ini = i+1;
				
			}
		}
		
		if(strlen(&buf[ini]))
		{
			if(nLines >= clipBoard.Count() || clipBoard[nLines] != &buf[ini])
			{
				free(buf);
				return false;
			}
		}
			

		free(buf);
		return true;
	}
	
	if(buf)
	{
		free(buf);
	}

	return false;
}

void KrTextBox::CopyToSystemClipBoard()
{
	if(bHasSystemClipBoard)
	{
		int i;
		gedString tmp;
		for(i = 0; i < clipBoard.Count(); i++)
		{
			tmp += clipBoard[i];

			if(i+1 < clipBoard.Count()) tmp += "\r";
		}

		put_scrap(T('T','E','X','T'), tmp.length(), tmp.c_str());

		if(ClipBoardCopyOk())
		{
			clipBoard.Clear();
		}
#ifndef WIN32
		else
		{
			//Solve linux bug
			bHasSystemClipBoard = false;
		}
#endif
	}
}

void KrTextBox::GetTextFromSystemClipBoard()
{
	if(bHasSystemClipBoard)
	{
		int scraplen;
		char *buf = NULL;
		get_scrap(T('T','E','X','T'), &scraplen, &buf);

		if(scraplen && buf)
		{
			clipBoard.Clear();

			
			int nLines = 0, ini = 0, i, len = strlen(buf);

			for(i = 0; i < len; i++)
			{
				if(buf[i] == '\n' || buf[i] == '\r')
				{
					buf[i] = '\n';
					if(i + 1 < len)
					{
						if(buf[i+1] == '\n' || buf[i+1] == '\r')
						{
							i++;
						}
					}

					nLines++;
				}
			}

			clipBoard.SetCount(nLines + 1);
			nLines = 0;
			
			for(i = 0; i < len; i++)
			{
				if(buf[i] == '\n')
				{
					buf[i] = 0;
					if(i + 1 < len)
					{
						if(buf[i+1] == '\n')
						{
							i++;
						}
					}

					if(strlen(&buf[ini]))
					{
						if(nLines + 1 > clipBoard.Count())
						{
							clipBoard.SetCount(nLines + 1);
						}

						clipBoard[nLines] = &buf[ini];				
					}

					nLines++; //Increment on empty lines too
					ini = i+1;
					
				}
			}
			
			if(strlen(&buf[ini])) clipBoard[nLines] = &buf[ini];	
		}

		if(buf)
		{
			free(buf);
		}
	}
}

void KrTextBox::Paste(int &cursorLine, int &cursorWindowLine, int &cursorPos) //maks
{
	//Remove current selection
	int initialLine = lineIni,
		initialPos = posIni,
		linesDeleted = RemoveSelectedText(false);

	if(linesDeleted >= 0 && (initialLine != cursorLine  || initialPos != cursorPos))
	{
		//Adjust cursor position
		cursorPos = initialPos;		
		cursorLine -= linesDeleted;
		cursorWindowLine -= linesDeleted;
		
		if(cursorLine < 0) cursorLine = 0;						
		if(cursorWindowLine < 0) cursorWindowLine = 0;				
	}
	else
	{
		initialLine = cursorLine;
		initialPos = cursorPos;
	}

	GetTextFromSystemClipBoard();
	if(clipBoard.Count() == 0) return; //Nothing to paste


	//Paste

	//If destiny is single and source is multiple line, copy only the first line
	int nLinesToPaste = (windowLines > 1)?clipBoard.Count():1, pos,
		finalCursorPos = (clipBoard.Count() == 1)?cursorPos + clipBoard[0].length():clipBoard[clipBoard.Count() - 1].length();


	//Store first and last clipboard lines
	gedString first(clipBoard[0]), last(clipBoard[clipBoard.Count() - 1]);


	//Adjust first line: current initial line before + first line on clipboard
	gedString str;

	for(pos = 0; pos < cursorPos; pos++)
	{
		if(pos < line[initialLine].str.Count())
		{
			str += line[initialLine].str[pos];
		}
	}

	clipBoard[0] = str + clipBoard[0];


	

	//Adjust last line: last clipboard line + current initial line after cursorPos
	str.clear();

	for(pos = cursorPos; pos < line[initialLine].str.Count(); pos++)
	{
		str += line[initialLine].str[pos];
	}

	clipBoard[clipBoard.Count() - 1] += str;

	
	//Delete anterior letters
	int i;
	for(i=verticalScrollAnt; i < verticalScrollAnt + windowLines && i < numLines; i++ ) 
	{
		line[i].letter.Clear();
		if ( line[i].parent )
		{
			Engine()->Tree()->DeleteNode( line[i].parent );
			line[i].parent = 0;
		}
	}

	//Create new lines
	int nNewLines = clipBoard.Count() - 1;
	
	line.Resize(numLines + nNewLines);
	line.SetCount(numLines + nNewLines);
	
	int oldNum = numLines;
	numLines = line.Count();

	//Move lines
	for(i = oldNum-1; i > initialLine; i--)
	{
		SetText16(GetText16(i), i + nNewLines);
	}

	//Copy new lines
	for(i = 0; i < clipBoard.Count(); i++)
	{
		SetText(clipBoard[i], i + initialLine);
	}

	//Restore first and last lipboard lines (for other pastes)
	clipBoard[0] = first;
	clipBoard[clipBoard.Count() - 1] = last;

	//Adjust position
	cursorLine += nNewLines;
	cursorWindowLine += nNewLines;

	if(cursorLine >= numLines) cursorLine = numLines - 1;

	if(cursorLine > verticalScroll + windowLines) 
	{
		verticalScroll += cursorLine - (verticalScroll + windowLines) + 1;
		if(verticalScroll + windowLines > numLines) verticalScroll = numLines - windowLines;
	}

	if(cursorWindowLine >= WindowLines()) 
	{
		cursorWindowLine = WindowLines() - 1;
	}

	//Delete anterior letters (again...)
	for(i=verticalScrollAnt; i < verticalScrollAnt + windowLines && i < numLines; i++ ) 
	{
		line[i].letter.Clear();
		if ( line[i].parent )
		{
			Engine()->Tree()->DeleteNode( line[i].parent );
			line[i].parent = 0;
		}
	}

	//Update letters
	for(i=verticalScroll; i < verticalScroll + windowLines && i < numLines; i++ ) 
	{
		CreateLetters(i);
	}


	verticalScrollAnt = verticalScroll;

	//Adjust cursor
	cursorPos = finalCursorPos;
	
}

int KrTextBox::Cut() //maks
{
	//Copy current selection to buffer
	Copy();	

	//Remove current selection
	return RemoveSelectedText(); //Return number of lines removed	
}

int KrTextBox::RemoveSelectedText(bool bCreateLetters)
{
	//Remove text
	if(lineIni < 0 || lineFin < 0 || (lineIni == lineFin && posIni == posFin)) return - 1;

	gedString intersectionText;

	
	{
		GlDynArray<U16> text = GetText16Array(lineIni);
		
		for(int i = 0; i < posIni; i++)
		{
			if(text[i] != 0) 
			{
				intersectionText += text[i];
			}
		}
		
	}
	
	{
		GlDynArray<U16> text = GetText16Array(lineFin);
		
		for(int i = posFin; i < text.Count(); i++)
		{
			if(text[i] != 0) 
			{
				intersectionText += text[i];
			}
		}
	}

	SetText(intersectionText, lineIni );


	//Copy lines (don't remove)
	if(lineFin != lineIni)
	{
		//maks: ini always equal to lineIni+1 
		//Solve the bug Shift-selecting text in the script can inadvertently remove an additional line if cursor is positioned at the start of that line
		int is, id, ini = /*(posIni == 0)?lineIni:*/lineIni+1;
		for(is = lineFin+1, id = ini; is < numLines; is++, id++)
		{
			SetText16(GetText16(is), id);
		}
		
		//Erase last lines
		while(id < numLines)
		{
			SetText("", id++);
		}
	}

	
	int linesDeleted = lineFin - lineIni;

	//Adjust position
	if(lineIni < verticalScroll) verticalScroll = lineIni;

	ClearSelection();	

	//Delete anterior letters
	int i;
	for(i=verticalScrollAnt; i < verticalScrollAnt + windowLines && i < numLines; i++ ) 
	{
		line[i].letter.Clear();
		if ( line[i].parent )
		{
			Engine()->Tree()->DeleteNode( line[i].parent );
			line[i].parent = 0;
		}
	}

	if(bCreateLetters)
	{
		for(i=verticalScroll; i < verticalScroll + windowLines && i < numLines; i++ ) 
		{
			CreateLetters(i);
		}
	}

	verticalScrollAnt = verticalScroll;

	return linesDeleted;
}



int KrTextBox::ScrollTo(int lineIndex) //maks
{
	verticalScroll = lineIndex;
	if(verticalScroll < 0) verticalScroll = 0;
	else if(verticalScroll + windowLines > numLines) verticalScroll = numLines - windowLines;

	//Delete anterior lines
	int i;
	for(i=verticalScrollAnt; i < verticalScrollAnt + windowLines && i < numLines; i++ ) 
	{
		line[i].letter.Clear();
		if ( line[i].parent )
		{
			Engine()->Tree()->DeleteNode( line[i].parent );
			line[i].parent = 0;
		}
	}

	for(i=verticalScroll; i < verticalScroll + windowLines && i < numLines; i++ ) 
	{
		CreateLetters(i);
	}

	verticalScrollAnt = verticalScroll;

	return verticalScroll;
}

void KrTextBox::SyntaxColorize(int lineIndex) //maks
{
	if(!bSyntaxColorize) return;

	KrColorTransform color;
	int r, g, b;
	
	int lineLen = line[lineIndex].letter.Count();

	gedString str;
	GlDynArray<U16> text = GetText16Array( lineIndex ); 

	for(int i = 0; i < text.Count(); i++)
	{
		str += text[i];
	}

	ListTokens tokens = GetTokens(str);
	
	ListTokensIterator it(tokens);
	int lastToken = 0, currentToken;

	for(it.Begin(); !it.Done(); it.Next())
	{
		currentToken = it.Current().tokenId;
		getTokenColor(it.Current().text.c_str(), lastToken, &currentToken, &r, &g, &b);


		color.Set(7*r/8, r/8, 7*g/8, g/8, 7*b/8, b/8, 255);

		for(int pos = it.Current().initialPosition; pos < it.Current().finalPosition; pos++)
		{
			if(pos >= 0 && pos < lineLen)
			{
				KrSprite *letter = line[lineIndex].letter[pos];			
				
				if(letter)
				{
					letter->SetColor(color);
				}
			}
		}
		
		lastToken = currentToken;
	}

}



#endif
