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



#ifndef KYRA_WIDGET_INCLUDED
#define KYRA_WIDGET_INCLUDED

#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#include "../../gameEngine/gedString.h"
#include "SDL.h"
#include "../util/gllist.h"
#include "../engine/engine.h"
#include "../engine/color.h"


/** An abstract class, the HandleWidgetEvent when a widget
	listener receives an event. The events widgets publish
	are documented on a per-widget basis.

	The class is abstract, so it can be used as a mix in and
	there are no multiple inheritance issues.

	The HandleWidgetEvent is a strange method with a lot of
	paremeters. An obvious solution would be to send a WidgetEvent
	class that is abstract and polymorphic...but Kyra doesn't use/require
	dynamic cast, so implementing that is less useful than it
	seems.
*/

class IKrWidgetListener
{
  public:
	enum
	{
		NONE = 0, //maks
		ACTIVATED,
		DEACTIVATED,
		SELECTION,
	};

	virtual bool HandleWidgetEvent(	KrWidget* source, 
									U32 event, 
									U32 data, 
									const SDL_Event* sdlEvent,
									const char* command, 
									const char* arg ) = 0;
};


/**	A scheme defines a color system for widgets. Each widget
	can have it's own scheme, or an entire widget set can
	share the same one. (Or at least a copy of the same one.)

	A scheme consists of a font (some widgets allow this to
	be null), a primary color, and a secondary color that
	should visually match the primary color, and a cursor
	color. All other colors	used are calculated from the 
	primary and secondary.
*/
struct KrScheme
{
	KrScheme( KrFontResource* font );

	KrRGBA CalcBrightLine() const;
	KrRGBA CalcShadowLine() const;

	KrColorTransform CalcHiPrimary() const;
	KrColorTransform CalcHiSec() const;
	KrColorTransform CalcDark() const;
	KrColorTransform CalcDarkSec() const;

	KrRGBA				primary;
	KrRGBA				cursor;
	KrColorTransform	secondary;
	KrFontResource*		font;
	
	enum
	{
		BRIGHT = 60,
		DARK   = 60
	};
};


struct KrBevelElement
{
  public:
	KrBevelElement( int w, int h, const KrScheme&, void *_parentActor = NULL);
	~KrBevelElement();

	void AddToTree( KrEngine*, KrImNode* parent );
	void DrawIn();
	void DrawOut();

	int width;
	int height;
	KrBoxResource *vertDR, *vertLR, *horDR, *horLR;
	KrBox *vertD, *vertL, *horD, *horL;

	void *parentActor; //maks
};


/**	This is the base of any Kyra widget. A widget is much like any
	other Kyra image object. You new it, add it to the KrImageTree,
	and it is drawn as part of the Draw() pass.

	Widgets don't have resources, but do use schemes which are similar.
	
	Widgets get their events from an event manager. If you use widgets,
	you must therefore send events to the KrEventManager class. Widgets 
	generally need SDL_EnableUNICODE( true ) to function correctly.

	Widgets broadcast their events to "listeners." To register a class
	as a listener, call AddListener(). The class will then receive 
	notification of events. The event each widget broadcast is documented
	on a per-widget basis.

	A widget subclass can implement any of the following properties:

	- MouseListeners receieve and can respond to mouse events.
	- KeyListeners get keyboard focus and key messages. They grab
	  key focus with the "tab" key or if clicked on.
	- Selectable widgets have state. They can be on/off, up/down, or whatever.
	  A KrToggleButton is a selectable widget.
	- Groupable widgets are in a group, and are aware of other widgets 
	  in that group. Radio Buttons are groupable.
	- Some widgets can respond to accelerator keys. Use SetAccelerator
	  to specify a hotkey to pass to the widget.

	Widgets can be nested and you can query for parents. However, Kyra regards
	the widget model as flat and doesn't pay attention to the nesting, with
	one important exception: a key event that isn't handled by a widget
	will be passed through its parent chain. For example, a KrConsole
	uses  a KrTextWidget. If the KrTextWidget doesn't handle a 
	particular key, it will get passed up to the KrConsole.

	WARNING: You may want to use Widgets in window 0, if you are using multiple Kyra
	windows. They are not fully tested in higher window numbers. You may see placement
	or bounding errors in higher window numbers.

	Notes for implementing your own widgets:

	- All Widgets are of the type ToWidget(). To get a particular flavor,
	  the ToExtended() method can be used.
	- Widgets should initialize themselves in the AddedtoTree() method. When
	  the engine calls your AddedtoTree() method, you can add chilren because
	  you are already in the Tree(). Be sure to call the parent
	  KrWidget::AddedtoTree() as well!
	- If you have been added to the Tree(), your Engine() pointer will be non-null.
*/
class KrWidget : public KrImNode, 
				 public IKrWidgetListener
{
  public:
	virtual ~KrWidget();
	virtual KrWidget* ToWidget()	{ return this; }

	/** Add a pointer to a class that listens to this widget. It can be
		added at any time (even a callback.) It should not be deleted
		without calling RemoveListener.
	*/
	void AddListener( IKrWidgetListener* );
	/// Remove a listener. Can be called at any time, even during a callback.
	void RemoveListener( IKrWidgetListener* );

	enum {
		LEFT_MOUSE   = 1,
		RIGHT_MOUSE  = 2,	// not currently supported
		MIDDLE_MOUSE = 4,	// not currently supported

		LEFT_UP   = 0,
		LEFT_DOWN = 1,
		RIGHT_UP   = 2,		// not currently supported
		RIGHT_DOWN = 3,		// not currently supported
		MIDDLE_UP  = 4,		// not currently supported
		MIDDLE_DOWN = 5		// not currently supported
	};

	/**	IsMouseListener returns whether this is a mouse listener or not, and which buttons
		are listened to. A return value of 0 is no listening. Else
		it can return an OR mask of the buttons ( LEFT_MOUSE, RIGHT_MOUSE,
		MIDDLE_MOUSE ) it wants to listen for mouse clicks. 

		The simple case is to only listen to the left mouse, in which case
		return LEFT_MOUSE (1). The click messages can then be treated like
		a boolean for the left mouse (1 is down, 0 is up.)

		Currently, with version 2.0, only the LEFT_MOUSE is supported.

	
		MouseIn is called when a mouse moves in to the widget. The 'down' parameter
		reflects the state of the left mouse button. The 'in' reflects whether
		it is moving to the widget (true) or away from the widget (false).


		MouseMove reports when the mouse moves over this widget.


		MouseClick is called when the mouse is clicked on this widget.
		The 'click' param will have a single value (not OR mask) of
		LEFT_UP, LEFT_DOWN, RIGHT_UP, etc. with x and y coordinates
		of the action. 
		
		In the simple case that you are only listening to the left mouse,
		the parameter will be essentially a boolean: 1 for the left mouse
		down, 0 for the left mouse up.
	*/
	virtual int  IsMouseListener()						{ return 0; }
	virtual void MouseIn( bool down, bool in )			{}				///< @sa IsMouseListener
	virtual void MouseMove( bool down, int x, int y )	{}				///< @sa IsMouseListener
	virtual void MouseClick( int click, int x, int y )	{}				///< @sa IsMouseListener

	virtual bool IsKeyListener()						{ return false; }
	virtual void KeyFocus( bool focus )					{}
	virtual bool KeyEvent( const SDL_Event& key )		{ return false; }

	virtual bool IsSelectable()							{ return false; }
	virtual void Selected( bool selected )				{}

	void SetGroup( int _groupId )						{ groupId = _groupId; }
	int  Group()										{ return groupId; }

	virtual void Accelerate( bool down, int keymod, int keysym )				{} //maks
	void SetAccelerator( int keymod, int keysym );

	/// Handle SDL events, return true if handled, false if not ours
	virtual bool HandleWidgetEvent(	KrWidget* source, 
									U32 event, U32 data, const SDL_Event* sdlEvent,
									const char* command, const char* arg )	
														{ return false; }

	/// Find the parent of the widget that is also a widget.
	KrWidget* ParentWidget();

  protected:
	// Send an event to all the listeners.
	void PublishEvent( U32 event, U32 data, const SDL_Event*, const char* command, const char* arg );
	// Send an event to all listeners, set data=1 for the specified listener.
	void PublishTaggedEvent( U32 event, const SDL_Event*, const char* com, const char* arg, IKrWidgetListener* special );


	KrWidget( const KrScheme& scheme );

	GlDynArray< IKrWidgetListener* >	eventList;
	int groupId;
	KrScheme scheme;
};

#endif

