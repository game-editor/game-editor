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

#ifndef KYRA_BUTTONWIDGET_INCLUDED
#define KYRA_BUTTONWIDGET_INCLUDED

#include "../gui/widget.h"

/** A super class for more specialized buttons. It's only 
	external method is to set the text of the button.
*/
class KrButton : public KrWidget
{
  public:
	virtual ~KrButton();

	/// Set the text of the button. The font used is specified in the Scheme.
	void SetTextChar( const gedString& text );

	/** Put a sprite on the button, as an icon or "decal" image. The sprite
		is passed to the Button, which owns it and will delete it when the
		Button is deleted. The Frame and Action of the Sprite will be unchanged
		by the button.

		You can change the Frame or Action, but the button will always position
		and size the icon based on the Frame and Action at the time it was added.

		The icon can be changed at any time, and the button will clean up memory
		and create a new one.
	*/
	void SetIcon( KrSprite* giveSprite );

	// Used to initialize the widget.
	virtual void AddedtoTree();

  protected:
	KrButton(	int width, 
				int height,
				const KrScheme& scheme );

	enum
	{
		OVER = 0x01,
		DOWN = 0x02,

		TEXT_DEPTH = 2,
		ICON_DEPTH = 1
	};
	void SetMode( int mode );
	void PlaceText();
	void PlaceIcon();

	int width, height, mode;
		
  private:
	KrImNode	  *holder;
	KrBoxResource *plateRes;
	KrBox		  *plate;
	KrBevelElement bevel;
	KrSprite	  *icon;
	int				iconX, iconY;

	KrTextBox*		  textBox;
	gedString		  text;
};


/**	A push button that is only depressed while the mouse or accelerator
	key is down.

	This type of button is not groupable or selectable.

	<b> Events Sent </b>

	- ACTIVATED when the button is pushed down.
	- DEACTIVATED when the button is released.
*/
class KrPushButton : public KrButton
{
  public:
	KrPushButton(	int width, 
					int height,
					const KrScheme& scheme ) : KrButton( width, height, scheme )	{}
	~KrPushButton()	{}

	virtual int  IsMouseListener()						{ return LEFT_MOUSE; }
	virtual void MouseIn( bool down, bool in );
	virtual void MouseMove( bool down, int x, int y );
	virtual void MouseClick( int down, int x, int y );

	// Not a key listener.
	// Not groupable or selectable.

//	virtual bool CanAccelerate()						{ return true; }
	virtual void Accelerate( bool down, int keymod, int keysym ); //maks
};


/**	A toggle type button, that clicks down and stays down until clicked again.
	It also responds to hot keys.

	The toggle button is selectable and can be accelerated.

	<b> Events Sent </b>

	- ACTIVATED when the button switches to the down state.
	- DEACTIVATED when the button switches to the up state.
*/
class KrToggleButton : public KrButton
{
  public:
	KrToggleButton(	int width, 
					int height,
					const KrScheme& scheme ) : KrButton( width, height, scheme )	{}
	~KrToggleButton()	{}

	virtual int IsMouseListener()						{ return LEFT_MOUSE; }
	virtual void MouseIn( bool down, bool in );
	virtual void MouseMove( bool down, int x, int y );
	virtual void MouseClick( int down, int x, int y );

	// Not a key listener.

	virtual bool IsSelectable()							{ return true; }
	virtual void Selected( bool selected );
	virtual void Accelerate( bool down, int keymod, int keysym ); //maks
};


#endif

#endif //#ifndef STAND_ALONE_GAME //maks