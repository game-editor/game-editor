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



#ifndef KYRA_KrEventManager_INCLUDED
#define KYRA_KrEventManager_INCLUDED

#include "SDL.h"
#include "../util/gldynarray.h"

class KrEngine;
class KrWidget;

/**	Manages widgets -- if you use widgets, you need one of these! All SDL keyboard
	and mouse events should be sent to the
	widget system via the KrEventManager::HandleEvent method.

	The EventManager will set SDL_EnableUNICODE to true when it is created.
*/
class KrEventManager
{
  public:
	~KrEventManager();
	static KrEventManager* Instance()		{	if ( !instance ) instance = new KrEventManager();
												return instance;
											}

	/// Dispenses events to the widgets.
	void HandleEvent( const SDL_Event& event, KrEngine* engine );

	/// Alternate call to set a widget accelerator. Usually easier to call the method on the widget itself.
	void SetAccelerator( int keymod, int keysym, KrWidget* );
	
	void AddListener( KrWidget* widget );
	void RemoveListener( KrWidget* widget );
	bool GrabKeyFocus( KrWidget* w ); //maks
	void SelectMe( KrWidget* w );
	int GetKeyFocus() {return keyFocus;} //maks
	void ChangeKeyFocus( int newFocus ); //maks
	
  private:
	struct Accel
	{
		int keymod;
		int keysym;
		KrWidget* target;
	};

	KrEventManager();
	static KrEventManager* instance;

	

	int keyFocus;
	KrWidget* mouseFocus;
	bool      mouseDown;
	bool newListnerFocus; //maks

	GlDynArray< KrWidget* > keyListeners;
	GlDynArray< KrWidget* > mouseListeners;
	GlDynArray< KrWidget* > selectListeners;
	GlDynArray< Accel >		accelListeners;
};

#endif


