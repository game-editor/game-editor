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

#ifndef KYRA_TEXT_INCLUDED
#define KYRA_TEXT_INCLUDED

#include "image.h"
#include "fontresource.h"
#include "../util/gllist.h" //maks

class KrSprite;
class KrTextWidget; //maks

/**
	Defines an area of the screen to be used to display text.
	Created from a font resource.

	Note that this is a container class; it does not draw. It
	will create sprites -- as children -- for the letters of text.
	The sprites will be fully managed by the text box and should
	not be changed by the client.

	If hit testing, it may be necessary to check the Parent's parent
	to see if it is a text box.
*/

typedef GlSList<KrImage *> ListSprite;  //maks
typedef GlSListIterator<KrImage *> ListSpriteIterator; //maks
typedef GlDynArray<gedString> ArrayString; //maks

class KrTextBox : public KrImNode
{
  public:
	 //KrRect& CalculatedBounds(int window); //maks
	enum Alignment
	{
		LEFT,
		CENTER,
		RIGHT
	};
	/**	Create a box that contains text. 
		@param resource		A pointer to the font to use. 
		@param width		Width of the text box.
		@param height		Height of the text box.
		@param lineSpacing	Pixels of padding between each line.
		@param alignment	LEFT, RIGHT, or CENTER justification.
	*/	
	KrTextBox( KrFontResource* resource,
			   int width, int height,	   // set to 0 to be un-bound
			   int lineSpacing,
			   Alignment alignment = LEFT,
			   void *parent = NULL); //maks

	virtual ~KrTextBox();

#ifndef STAND_ALONE_GAME //maks	
	void SetSelectionRange(int _lineIni, int _posIni, int _lineFin, int _posFin); //maks
	void ClearSelection(); 
	void Copy();
	void Paste(int &cursorLine, int &cursorWindowLine, int &cursorPos);
	int Cut();
	int RemoveSelectedText(bool bCreateLetters = true);
	int getInitialSelectionLine() {return lineIni;}
	int getInitialSelectionPos() {return posIni;}
	void SyntaxColorize(int lineIndex);
	 
	int ScrollTo(int lineIndex);
	void setSyntaxColorize(bool value) {bSyntaxColorize = value;}
	void SetClipBoard(const gedString& str);

	void setParentWidget(KrTextWidget *parent) {parentWidget = parent;}
#endif

	int ScrollDown(); 
	int ScrollUp(); 
	int ScrollPageDown(); 
	int ScrollPageUp();
	

	/// Get the resource for this TextBox.
	KrFontResource*  FontResource()	{ return resource; }

	/// Size, untransformed, of this box:
	int Width()	const	{ return width; }
	/// Size, untransformed, of this box:
	int Height() const	{ return height; }

	/// Number of lines of text in this text box.
	int NumLines() const					{ return numLines; }
	/// Get the y offset to a given line.
	int GetLineY( int lineNum ) const		{ return lineNum*sizeOfLine;/*return line[lineNum].dy;*/ } //maks
	int GetHeightOfLine() const {return sizeOfLine;} //maks

	/// Set the text of a given linenumber, in 16 bit characters.
	void		SetText16(   const U16*  text, int lineNum );
	/// Get the text of a given linenumber, in 16 bit characters.
	const U16*  GetText16( int lineNum ) 					{ return line[lineNum].str.Memory(); } //maks
	/// A slightly faster way to get the 16 bit text.
	const GlDynArray<U16>& GetText16Array( int lineNum )   { return line[lineNum].str; } //maks

	/// Set the text of a given linenumber, in 8 bit characters.
	void SetText( const gedString& text, int lineNum ); //maks
	/// Get the text of a given linenumber, in 8 bit characters.
	void GetText( gedString* buffer, int lineNum ); //maks

	/// Return the length of the line, without counting the terminator.
	int GetLineLength( int lineNum = 0 )	
	{	
		if(lineNum >= 0 && lineNum < (int)line.Count())
		{
			GLASSERT( line[lineNum].str.Count() > 0 ); 
			return line[lineNum].str.Count() - 1; 
		}

		return 0;
	}

	int GetLineClipLength( int lineNum = 0 ) //maks
	{
		if(clipLenght.Count() <= lineNum) //maks
		{
			clipLenght.SetCount(lineNum+1);
			clipLenght[lineNum] = 0;
		}

		int clip = clipLenght[lineNum], lenght = GetLineLength(lineNum);
		return (clip < lenght)?clip:lenght;
	}

	void SetTextOffSet(int offSet); //maks
	
	int WindowLines() {return windowLines;}

	virtual KrImNode* Clone();

	// ---- Internal ----- //
	virtual KrTextBox* ToTextBox()	{ return this; }	
	virtual void	AddedtoTree();
	void			CreateLetters( int index );
	virtual void FlushInvalid( /*int win,*/ bool cache );

	void SetAlignment(Alignment _align);

	enum {
		CHILD_DEPTH = 65536	// Depth where the letters will start.
	};

  private:



	
	
	// The collision routines depend on letter->parent->textbox relationship.
	// Be careful changing the tree structure.
	class TextLine //maks
	{
	public:
		TextLine()
		{
			str.PushBack( 0 );		// use a null terminator
			parent = 0;
			width = 0;
		}

		//int						dy; //maks
		GlDynArray<U16>			str;
		KrImNode*				parent;		
		GlDynArray<KrSprite*> 	letter;		
		int						width;		// The width of this line. Used for positioning of LEFT, RIGHT, CENTER.
	};

	int				width, height;	// Untransformed.
	int				numLines;		// Number of text lines in the box.
	GlDynArray<TextLine> line; //maks //TextLine*		line;
	KrFontResource*	resource;
	Alignment		align;
	int				lineSpacing;

	GlDynArray<U16>	clipLenght; //maks
	int textOffSet, textOffSetAnt, verticalScroll, verticalScrollAnt; //maks
	int windowLines, sizeOfLine; //maks
	KrRect calculatedBounds; //maks	

	//Selection //maks
#ifndef STAND_ALONE_GAME 
	KrImage *CreateSelecionMark(KrImage *parent); 
	void CopyToSystemClipBoard();
	void GetTextFromSystemClipBoard();
	bool ClipBoardCopyOk();


	ListSprite listSelectedLetters;
	int lineIni, posIni, lineFin, posFin;
	bool bSyntaxColorize;

	static ArrayString clipBoard;

	static bool bSystemClipBoardInited;
	static bool bHasSystemClipBoard;

	KrTextWidget *parentWidget; //maks
#endif

};

#endif
