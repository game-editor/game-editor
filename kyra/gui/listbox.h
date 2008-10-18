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

#ifndef STAND_ALONE_GAME //maks

#ifndef KYRA_SCROLLBOX_INCLUDED
#define KYRA_SCROLLBOX_INCLUDED

#include "widget.h"
class KrTextWidget;


/**	A very simple listbox. Items can be added, but not removed. It can
	be scrolled, but through the method MoveSelection. Normally, you set
	up button to accomplish this. (The guitest demonstrates this.)

	<b> Events Sent </b>
	- SELECTION when an item in the listbox is selected.
*/
class KrListBox : public KrWidget
{
  public:
	void RemoveText( const gedString& text ); //maks
	void RemoveAll(); //maks
	int Width()		{ return width; } //maks
	int Height()	{ return height; } //maks

	KrListBox( KrFontResource* font, int width, int height, int scrollWidth, const KrScheme& scheme );
	virtual ~KrListBox();

	/// Return the number of items in the list box.
	int NumItems()									{ return textStrings.Count(); }
	/// Return the index of the selected item.
	int SelectedItem()								{ return selectedItem; }
	/// Add an item. Returns the index of the item.
	int AddTextChar( const gedString& text );
	/// Query an item and write it to the "text" string.
	void GetTextChar( int index, gedString* text );
	/// Move the selection, positive or negative, scrolling if necessary.
	void MoveSelection( int delta );

	virtual bool HandleWidgetEvent(	KrWidget* source, 
									U32 event, U32 data, const SDL_Event* sdlEvent,
									const char* command, const char* arg );

	// Used to initialize the widget.
	virtual void AddedtoTree();

  private:
	void DrawText();

	int width, height;
	KrFontResource* font;
	KrBevelElement  *outerBevel;

	GlDynArray< KrTextWidget* > textWidgets;
	GlDynArray< gedString >   textStrings;

	int firstItem;
	int selectedItem;
};



#endif


#endif //#ifndef STAND_ALONE_GAME //maks