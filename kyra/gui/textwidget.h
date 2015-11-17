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



#ifndef KYRA_TEXTWIDGET_INCLUDED
#define KYRA_TEXTWIDGET_INCLUDED

#include "../gui/widget.h"
#include "../engine/textbox.h" //maks
#include "UndoFrame.h"

/**	A "widget" version the the textbox. The regular textbox (KrTextBox) 
	can be used to display dynamic, but not editable text. The widget
	version can be editable, and integrated with the widget
	event system.

	<b> Events Sent </b>
	- ACTIVATED when the 'return' key is pressed.
	- SELECTION when it gets or loses focus.
*/
class KrTextWidget : public KrWidget
{
  public:
	  void setEditable(bool value); //maks

	void Disable(); //maks
	void SetText( const gedString&, int line = 0); //maks
	
	void GetText( gedString* buffer, int line = 0 ); //maks
	int NumLines(); //maks
	int WindowLines(); //maks
	int ActualLine() {return line;} //maks

	void LineDown(); //maks
	void LineUp(); //maks
	int ScrollDown(); //maks
	int ScrollUp(); //maks
	int ScrollPageUp(); //maks
	int ScrollPageDown(); //maks
	void Select(bool select);
	void ChangeEditable(bool select);	

#ifndef STAND_ALONE_GAME //maks
	virtual void Accelerate( bool down, int keymod, int keysym ); //maks
	void SetSyntaxColorize(bool value); //maks
	void SelectToken(); //maks
	void Copy(); //maks
	void Paste(); //maks
	void Cut(); //maks
	void InsertText(const gedString& t); //maks
	int ScrollTo(int lineIndex); //maks
	void PushUndo(int lineNum); //maks
	void StartAction();
	void EndAction();
	gedString GetTextUntilCursor(); //maks
#endif //#ifndef STAND_ALONE_GAME //maks

	/**	A rather feature rich constructor. 
		@param drawBorder		Draw a square outline arount the text box.
		@param drawBackground	Fill in the box behind the text. If false, transparent.
		@param editable			If the textbox should support a cursor and keyboard editing.
	*/
	

	KrTextWidget(	int width, int height, 
					bool drawBorder,
					bool drawBackground,
					bool editable,
					const KrScheme& scheme,
					bool multiLine = false //maks
				 );



	KrTextWidget( KrFontResource* resource,
			   int width, int height,	   
			   int lineSpacing,
			   KrTextBox::Alignment _align = KrTextBox::LEFT,
			   void *parent = NULL,
			   bool _multiLine = false,
			   bool _bEditable = false, bool _hasBorder = false, bool _hasBackGround = false, bool _getFocus = false,
			   KrRGBA _borderColor = 0, KrRGBA _cursorColor = 0, KrRGBA _backGroundColor = 0,
			   KrScheme _scheme = NULL); //maks

	~KrTextWidget();

	virtual int  IsMouseListener()						{ return LEFT_MOUSE; }
	virtual void MouseIn( bool down, bool in );
	virtual void MouseMove( bool down, int x, int y ); //maks
	virtual void MouseClick( int down, int x, int y );

	
	

	virtual bool IsKeyListener()						{ return editable; }
	virtual void KeyFocus( bool focus );
	virtual bool KeyEvent( const SDL_Event& key );

	/// Set the text of the box.
	
	/// Get the text of the box to the given string.
	
	int Width()		{ return width; }
	int Height()	{ return height; }

	// Used to initialize the widget.
	virtual void AddedtoTree();

  private:
	enum
	{
		CURSOR_WIDTH = 2
	};

	void PositionCursor();
	void CreateCursor(); //maks

	int width, height;
	bool drawBorder;
	bool drawBackground;
	bool editable;
	
	int cursorPos;
	int textOffSet; //maks

	U32 lastMouseClick; //maks: To double click catch

	KrImNode*		holder;
	KrBevelElement* bevel;
//	KrFontResource* fontRes;
	KrBoxResource *plateRes, *cursorRes;
	KrBox		  *plate, *cursor;
	KrTextBox	  *textBox;

	bool bHasFocus; //maks

	int lineAnt, line, currentWindowLine; //maks
	bool multiLine; //maks
	int lineSelIni, posSelIni, lineSelFin, posSelFin; //maks

	bool bEnable; //maks

	KrTextBox::Alignment		align; //maks

	KrRGBA backgroundColor; //maks

#ifndef STAND_ALONE_GAME //maks
	// undo/redo state

	
	void DoUndo();
	void DoRedo();
	void DoUndoRedo(undostack &inStack, undostack& outStack);
	void ClearUndoStack(undostack &inStack);
	void ClearUndoHistory();

   // current action frame. an action can be composed of one or more sub-actions.
   // for a single keystroke, the undo frame holds one action: the keystroke.
   // for complex actions, like a paste or a cut, the frame will contain all of the 
   // actions required to perform the meta-action.
   CUndoFrame  m_currentUndoFrame;

   // holds all of our actions - this might get big. feel free to clear it once in a while.
   undostack   m_undoStack;

   // when the user starts undo-ing stuff, the undo's are placed here. 
   // this is cleared when a new undo frame is created, ie. when a new edit action occurs.
   undostack   m_redoStack;
#endif
};


#endif

