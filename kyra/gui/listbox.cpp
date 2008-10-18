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

#include "listbox.h"
#include "../engine/fontresource.h"
#include "textwidget.h"

KrListBox::KrListBox(	KrFontResource* _font,
						int _width, int _height, int _scrollWidth, 
						const KrScheme& s ) : KrWidget( s )
{
	font = _font;
	width = _width;
	height = _height;
	firstItem = 0;
	selectedItem = 0;

	unsigned numVisibleItems = ( height - 2 ) / font->FontHeight();
	textWidgets.SetCount( numVisibleItems );
	
	height = numVisibleItems * font->FontHeight() + 2;
	outerBevel = new KrBevelElement( width, height, scheme );
}

KrListBox::~KrListBox()
{
	delete outerBevel;
}

void KrListBox::AddedtoTree()
{
	outerBevel->AddToTree( Engine(), this );
	outerBevel->DrawIn();

	for( unsigned i=0; i<textWidgets.Count(); ++i )
	{
		textWidgets[i] = new KrTextWidget( width /*- scrollWidth*/ - 2, font->FontHeight(),
										   false, true, false, scheme );
		textWidgets[i]->SetPos( 1, i * font->FontHeight() + 1 );
		Engine()->Tree()->AddNode( this, textWidgets[i] );
		textWidgets[i]->AddListener( this );
	}
	DrawText();
}


bool KrListBox::HandleWidgetEvent(	KrWidget* source, 
									U32 event, U32 data, const SDL_Event* sdlEvent,
									const char* command, const char* arg )
{
	if ( event == SELECTION )
	{
		unsigned i;
		KrColorTransform normal;
		KrColorTransform selected = scheme.CalcHiPrimary();

		for( i=0; i<textWidgets.Count(); ++i )
		{
			if ( textWidgets[i] == source  )
			{
				int indexToString = i + firstItem;
				if ( GlInRange( indexToString, 0, (int) textStrings.Count()-1 ) )
				{
					selectedItem = indexToString;
					//PublishEvent( SELECTION, indexToString, 0, 0, 0 ); //maks
					break;
				}
			}
		}

		for( i=0; i<textWidgets.Count(); ++i )
		{
			if ( i == (unsigned) ( selectedItem - firstItem ) )
				textWidgets[i]->SetColor( selected );
			else
				textWidgets[i]->SetColor( normal );
		}
		return true;
	}
	return false;
}


int KrListBox::AddTextChar( const gedString& text )
{
	textStrings.PushBack( text );
	DrawText();
	return textStrings.Count() - 1;
}


void KrListBox::GetTextChar( int id, gedString* text )
{
	*text = "";
	if ( id < (int) textStrings.Count() )
		*text = textStrings[ id ];
}


void KrListBox::MoveSelection( int delta )
{
	if (    delta != 0
	     && GlInRange( selectedItem+delta, 0, (int) textStrings.Count()-1 ) )
	{
		selectedItem += delta;
		if ( selectedItem < firstItem )
			firstItem = selectedItem;
		else if ( selectedItem >= firstItem + (int) textWidgets.Count() )
			firstItem = selectedItem - textWidgets.Count() + 1;

		PublishEvent( SELECTION, selectedItem, 0, 0, 0 );

		DrawText();
	}
}


void KrListBox::DrawText()
{
	for( int i=0; i<(int)textWidgets.Count(); ++i )
	{
		KrColorTransform normal;
		KrColorTransform selected = scheme.CalcHiPrimary();

		int index = i + firstItem;
		textWidgets[i]->SetColor( normal );

		if ( GlInRange( index, 0, (int) textStrings.Count()-1 ) )
		{
			textWidgets[i]->SetText( textStrings[ index ] ); //maks

			if ( selectedItem == index )
			{
				textWidgets[i]->SetColor( selected );
			}
		}
		else
		{
			textWidgets[i]->SetText(""); //maks
			selectedItem = 0; //maks
		}
	}
}

void KrListBox::RemoveText( const gedString& text ) //maks
{
	int i;
	if((i = textStrings.Find(text)) != UINT_MAX)
	{
		textStrings.Remove(i);
		DrawText();
	}
}

void KrListBox::RemoveAll() //maks
{
	textStrings.SetCount(0);
	DrawText();
}

#endif //#ifndef STAND_ALONE_GAME //maks
