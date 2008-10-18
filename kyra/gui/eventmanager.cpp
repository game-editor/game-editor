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



#include "eventmanager.h"
#include "widget.h"

/*static*/ KrEventManager* KrEventManager::instance = 0;



#ifdef _WIN32_WCE
#include <windows.h>
#include <Aygshell.h>

extern "C" HWND SDL_Window; //maks
extern bool CanOpenSIP();
extern void InvalidateGameScreen(); //maks
extern "C" HWND sipHwnd;

void FocusState(bool bHasFocus) //maks
{
	if(bHasFocus && CanOpenSIP())
	{
		SHSipPreference(SDL_Window, SIP_UP);
		sipHwnd = FindWindow(_T("SipWndClass"), NULL);
	}

	if(!bHasFocus)
	{
		sipHwnd = NULL;
		SHSipPreference(SDL_Window, SIP_DOWN);
		InvalidateGameScreen();		
	}	
}
#else
void FocusState(bool bHasFocus) 
{
	//Nothing...	
}
#endif

KrEventManager::KrEventManager()
{
	keyFocus = -1;
	mouseFocus = 0;
	mouseDown  = false;
	newListnerFocus = false; //maks
	SDL_EnableUNICODE( true );	// Force this! Too many bugs that it's not on.
}

KrEventManager::~KrEventManager()
{
	instance = 0;	// Very important, so we don't screw up if we're re-created.
}


void KrEventManager::AddListener( KrWidget* widget )
{
	if ( widget->IsKeyListener() )
	{
		keyListeners.PushBack( widget );
		if ( keyFocus < 0 )
		{
			//ChangeKeyFocus( 0 ); //maks
		}
	}
	if ( widget->IsMouseListener() & KrWidget::LEFT_MOUSE )
	{
		mouseListeners.PushBack( widget );
	}
	if ( widget->IsSelectable() )
	{
		selectListeners.PushBack( widget );
	}
}


void KrEventManager::RemoveListener( KrWidget* widget )
{
	int i = keyListeners.Find( widget ); //maks
	if ( i >= 0 && i == keyFocus) //maks: Only remove the focus if the widget has the focus (solve focus bug in Math for Kids.ged)
	{
		keyFocus = -1;
		FocusState(false);
	}

	keyListeners.FindAndDelete( widget );
	mouseListeners.FindAndDelete( widget );
	selectListeners.FindAndDelete( widget );

	for( i=0; i<accelListeners.Count(); ++i )
	{
		if ( accelListeners[i].target == widget )
		{
			accelListeners.Remove( i );
			//break; //maks
		}
	}

	if ( mouseFocus == widget )
	{
		mouseFocus = 0;
	}
}


void KrEventManager::ChangeKeyFocus( int newFocus )
{
	if ( newFocus >= (int) keyListeners.Count() )
		newFocus -= (int) keyListeners.Count();

	if ( keyListeners.Count() > 0 )
	{
		if ( newFocus != keyFocus )
		{
			if ( GlInRange( keyFocus, (int) 0, int( keyListeners.Count() - 1 ) ) )
				keyListeners[ keyFocus ]->KeyFocus( false );
			if ( GlInRange( newFocus, (int) 0, int( keyListeners.Count() - 1 ) ) )
				keyListeners[ newFocus ]->KeyFocus( true );
			keyFocus = newFocus;
			newListnerFocus = true; //maks
		}
	}
	else
	{
		keyFocus = -1;
	}

	FocusState(keyFocus != -1);
}


bool KrEventManager::GrabKeyFocus( KrWidget* w ) //maks
{
	int i = keyListeners.Find( w );
	if ( i >= 0 )
	{
		ChangeKeyFocus( i );
		return true;
	}

	return false;
}


void KrEventManager::SetAccelerator( int keymod, int keysym, KrWidget* target )
{
	//maks: more than one accelerator per control

	/*int i;
	for ( i=0; i<accelListeners.Count(); ++i )
	{
		if ( accelListeners[i].target == target )
		{
			accelListeners[i].keymod = keymod;
			accelListeners[i].keysym = keysym;
			break;
		}
	}

	if ( i == accelListeners.Count() )
	{
		Accel a = { keymod, keysym, target };
		accelListeners.PushBack( a );
	}*/

	Accel a = { keymod, keysym, target };
	accelListeners.PushBack( a );
}


void KrEventManager::SelectMe( KrWidget* w )
{
	if ( w->IsSelectable() )
	{
		int group = w->Group();

		for( int i=0; i < selectListeners.Count(); ++i )
		{
			if ( selectListeners[i]->Group() == group )
			{
				if ( selectListeners[i] == w )
					selectListeners[i]->Selected( true );
				else
					selectListeners[i]->Selected( false );
			}
		}
	}
}


void KrEventManager::HandleEvent( const SDL_Event& event, KrEngine* engine )
{
	if ( event.type == SDL_KEYDOWN && keyFocus >= 0) //maks
	{
		//	- the tab key changes key focus.
		//	- accelerators are checked
		//	- keys passed through to the handler.

		#ifdef DEBUG
		GLOUTPUT( "KeyDown mod=%d sym=%d, unicode=%d, name=%s\n", event.key.keysym.mod, event.key.keysym.sym, event.key.keysym.unicode, SDL_GetKeyName(event.key.keysym.sym)); //maks
		#endif

		if (    event.key.keysym.sym == SDLK_TAB 
				 && keyListeners.Count() > 1 )
		{
			if ( event.key.keysym.mod & KMOD_SHIFT )
				ChangeKeyFocus( keyFocus + keyListeners.Count() - 1 );
			else	
				ChangeKeyFocus( keyFocus + 1 );
			return;
		}

		for( int i=0; i<accelListeners.Count(); ++i )
		{
			int sym = accelListeners[i].keysym;
			int mod = accelListeners[i].keymod;

			if (    event.key.keysym.sym == sym && 
				    event.key.keysym.mod & mod &&
					keyListeners.Count() &&
					accelListeners[i].target == keyListeners[ keyFocus ]) //maks: send accelerators for key focus owners
			{
				accelListeners[i].target->Accelerate( true, mod, sym ); //maks
				return;
			}
		}

		if ( keyListeners.Count() > 0 )
		{
			keyFocus = GlClamp( keyFocus, 0, int( keyListeners.Count()-1 ) );
			KrWidget* widget = keyListeners[ keyFocus ];

			// Go up the chain until handled.
			while( widget && !widget->KeyEvent( event ) )
			{
				widget = widget->ParentWidget();
			}
		}
	}
	else if ( event.type == SDL_KEYUP )
	{
		// - only accelerates key up
		for( int i=0; i<accelListeners.Count(); ++i )
		{
			if (    event.key.keysym.sym == accelListeners[i].keysym &&
				    event.key.keysym.mod & accelListeners[i].keymod &&
					keyFocus >= 0 &&
					keyListeners.Count() &&
					accelListeners[i].target == keyListeners[ keyFocus ]) //maks
			{
				accelListeners[i].target->Accelerate( false, event.key.keysym.mod, event.key.keysym.sym ); //maks
				return;
			}
		}

		//Send shift key up
		if ( keyListeners.Count() > 0 && (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT))
		{
			keyFocus = GlClamp( keyFocus, 0, int( keyListeners.Count()-1 ) );
			KrWidget* widget = keyListeners[ keyFocus ];

			// Go up the chain until handled.
			while( widget && !widget->KeyEvent( event ) )
			{
				widget = widget->ParentWidget();
			}
		}
	}
	else if ( event.type == SDL_MOUSEMOTION )
	{
		GlDynArray<KrImage*> hitArray;
		KrWidget* hit = 0;
		//int window = engine->GetWindowFromPoint( event.motion.x, event.motion.y );
	
		KrVector2T< GlFixed > object;

#ifndef _WIN32_WCE
		if(mouseDown && mouseFocus) //maks
		{
			//Don't change the focus if mouse button is down
			//Don't works on Pocket PC. In input.ged don't close the SIP keyboard (only send keyFocus = true)
			
			hit = mouseFocus;
			hit->ScreenToObject( event.motion.x, event.motion.y, &object/*, window*/ );
		}
		else
#endif
		{			
			engine->Tree()->HitTest( event.motion.x, event.motion.y,
				KrImageTree::ALWAYS_INSIDE_BOX, //| GET_ALL_HITS,
				&hitArray/*,
				&window*/ );
			
			
			for( int i=0; i<hitArray.Count(); ++i )
			{
				KrImNode* parent = hitArray[i]->Parent();
				while( parent )
				{
					if ( parent->ToWidget() )
					{
						hit = parent->ToWidget();
						hit->ScreenToObject( event.motion.x, event.motion.y, &object/*, window*/ );
						break;
					}
					parent = parent->Parent();
				}
			}
		}
	
		// 1) Something has the focus. Nothing had it before.
		// 2) Something has the focus, something else had it before.
		// 3) Something loses the focus.
		// 5) The thing with focus gets a move.
		if ( hit && !mouseFocus )
		{
			mouseFocus = hit;
			mouseFocus->MouseIn( mouseDown, true );
			mouseFocus->MouseMove( mouseDown, object.x.ToIntRound(), object.y.ToIntRound() );
		}
		else if ( hit && mouseFocus && mouseFocus != hit )
		{
			mouseFocus->MouseIn( mouseDown, false );
			mouseFocus = hit;
			mouseFocus->MouseIn( mouseDown, true );
			mouseFocus->MouseMove( mouseDown, object.x.ToIntRound(), object.y.ToIntRound() );
		}
		else if ( !hit && mouseFocus )
		{
			mouseFocus->MouseIn( mouseDown, false );
			mouseFocus = hit;
		}
		else if ( hit && hit == mouseFocus )
		{
			GLASSERT( hit == mouseFocus );
			mouseFocus->MouseMove( mouseDown, object.x.ToIntRound(), object.y.ToIntRound() );
		}	
		else if ( !hit && !mouseFocus )
		{
			// nothing to do
		}
		else
		{
			GLASSERT( 0 );
		}
		
	}
	else if ( event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP )
	{
		if ( event.button.button == SDL_BUTTON_LEFT )
		{
			bool down = event.button.state != 0;	// & SDL_BUTTON_LMASK;
			if ( down != mouseDown )
			{
				mouseDown = down;
				if ( mouseFocus )
				{
					int window = engine->GetWindowFromPoint( event.motion.x, event.motion.y );
					KrVector2T< GlFixed > object;
					mouseFocus->ScreenToObject( event.motion.x, event.motion.y, &object/*, window*/ );

					mouseFocus->MouseClick( mouseDown ? KrWidget::LEFT_DOWN : KrWidget::LEFT_UP, 
											object.x.ToIntRound(), 
											object.y.ToIntRound() );
				}
				else //maks
				{
					if(
						keyFocus >= 0 && keyFocus < keyListeners.Count() &&
						!newListnerFocus //maks: Only remove the focus if the widget don't has changed the focus in this cycle (solve focus bug in Math for Kids.ged)
					  )
					{
						keyListeners[ keyFocus ]->KeyFocus( false );
						keyFocus = -1; 
						FocusState(false);
					}
				}
			}
		}
		else if ( event.button.button == SDL_BUTTON_RIGHT ) //maks
		{
			bool down = event.button.state != 0;
			if ( down != mouseDown )
			{
				mouseDown = down;
				if ( mouseFocus )
				{
					int window = engine->GetWindowFromPoint( event.motion.x, event.motion.y );
					KrVector2T< GlFixed > object;
					mouseFocus->ScreenToObject( event.motion.x, event.motion.y, &object/*, window*/ );

					mouseFocus->MouseClick( mouseDown ? KrWidget::RIGHT_DOWN : KrWidget::RIGHT_UP, 
											object.x.ToIntRound(), 
											object.y.ToIntRound() );
				}				
			}
		}
	}

	newListnerFocus = false; //maks
}


