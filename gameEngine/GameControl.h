/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

If GPL v3 does not work for you, we offer a "non-GPL" license 
<http://game-editor.com/License>, thats will help us to pay our developers 
<http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>.

***************************************************************************/


// GameControl.h: interface for the GameControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMECONTROL_H__9515E689_DC7A_4CE3_87DD_93F087A7ED24__INCLUDED_)
#define AFX_GAMECONTROL_H__9515E689_DC7A_4CE3_87DD_93F087A7ED24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)


#include "SDL.h"
#include "SDL_thread.h"
#include "engine/kyra.h"
#include "gui.h"
#include "Action.h"
#include "Axis.h"
#include "EditorDirectory.h"
#include "SDL_mixer.h"
#include "Tile.h"
#include "system.h"
#include "Script.h"
#include "MuteEx.h"
#include "gedString.h"
#include "dlmalloc.h"
#include "Actor.h"

#include "../RakNet/Source/RakPeerInterface.h"
//#include "../RakNet/Source/NatPunchthrough.h"

#ifdef USE_AVL_TREE
#include "ttree.h"
#endif

#ifndef STAND_ALONE_GAME
//////////////////////////////////////////////////////////////////////////////
//Generic tool tip strings
#define TIP_ACTIVATIONEVENT_CHOOSE			"\
Choose the actors that will receive the Activation Event.\n\
\n\
It will be drawn a line from the actor that is generating\n\
the event to the actor that will receive the event.\n\
The line is a green to red gradient, where green indicates\n\
the generation of the event and red indicates the reception.\n\
In that way, it can be visualized Activation Events's flow easily.\n\
\n\
Press Esc key to end.\n\
\n\
Stopping the cursor on top of an activation line, opens a tool tip detailing Activation Event.\n\
The activation event can be removed by right click in activation line, and select \"Remove\"\n\
Cloned actors don't share Activation Events, so that they can be accessed individually.\n\
\n\
Actors can generate Activation Events from Activation Events. More complex procedures can be made.\n\
To define the action that will happen when an Activation Event happens,\n\
it is enough to choose the event Activation Event in Actor Control"

#define TIP_ADDACTION			"\
Click here to define the Action in response to this Event.\n\
You can add one or more Actions."

#define TIP_ACTION_CLOSE			"\
Click here when you are finished adding Actions to this Event."

#define TIP_ACTION_EDIT			"\
Click here to edit the action for this event"

#define TIP_ACTION_CANCEL			"\
Click here to cancel without saving."

#define TIP_ACTION_ADD			"\
Click here to \"Add\" the action\n\
\n\
     Immediate action: When the Event occurs, the Action will be executed immediately.\n\
Wait for frame action: The Action will be put into a queue, it will be executed when\n\
                       certain frames of a specified animation are displayed."

#define TIP_ACTION_ACTOR			"\
\n\
The \"Event Actor\" is the Actor that currently receives the Event.\n\
If an Actor's name is selected, all of the clones will receive the Action."


#define TIP_SCRIPTEDITOR_ACTORS			"\
Select here an actor and your variable in the variables/functions list"

#define TIP_SCRIPTEDITOR_VARIABLES_FUNCTIONS			"\
Select here a variable or function. When an function is selected\n\
its corresponding panel will pop-up to auto complete script"

#define TIP_SCRIPTEDITOR_VARIABLES			"\
Add, edit or remove your variables"
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
//Actor Property tip strings
#define TIP_ACTORCONTROL				"\
Make your actor come to life with this control"

#define TIP_ACTORCONTROL_NAME			"\
Show the selected actor name\n\
The dot + number following the name indicates de clone number\n\
\n\
By changing the current actor will center the editor on new actor\n\
Left or right click in actor updates Actor Control panel"

#define TIP_ACTORCONTROL_TRANSPARENCY	"\
Change the actor's transparency\n\
\n\
Each actor can have its transparency adjusted separately\n\
Initially, transparency for all actors is zero.\n\
\n\
The actor transparency can be changed in the\n\
game with \"Change Transparency\" action\n\
\n\
Child actors will inherit parent transparency"

#define TIP_ACTORCONTROL_COLOR	"\
Change the actor's color\n\
Child actors will inherit parent color"


#define TIP_ACTORCONTROL_ZDEPTH	"\
Change the actor's depth to put an actor in front or behind others.\n\
\n\
Increasing Z Depth makes the actor look as if it's closer to you.\n\
The actor depth can be changed in the\n\
game with \"Change Z Depth\" action"

#define TIP_ACTORCONTROL_GRID_ZOOM	"\
Increase or decrease grid zoom"

#define TIP_ACTORCONTROL_CREATE	"\
Specify whether actor will be created when game starts\n\
To set an actor not to be created in the beginning of the game,\n\
change this option to \"No\" (the actor must have no parent)\n\
\n\
The actors can be created later with the \"Create Actor\" action"

#define TIP_ACTORCONTROL_NETACTOR	"\
Specify whether actor will be synchronized over a network.\n\
\n\
If a normal actor create a network actor in an user input,\n\
execute the action that causes the creation in the remote machines.\n\
\n\
An network actor created by a remote action will be controlled by the remote machine.\n\
\n\
In a local network the game will try to find other machines running the same game.\n\
In a Internet game, you can use the Game Editor server to find other machines running the same game.\n\
The Game Editor server can be configurated in the Game Properties panel."

#define TIP_ACTORCONTROL_NETOPTIMIZE	"\
If the option 'Optimize for precision' was selected, each position or frame change will be send to the remote machines.\n\
Use this option only if you need perfect position and frame synchronization  between machines.\n\
\n\
If the option 'Optimize for speed' was selected, only the action commands are sent to the remote machines."

#define TIP_ACTORCONTROL_X	"\
Current horizontal position, relative to actor parent"

#define TIP_ACTORCONTROL_Y	"\
Current vertical position, relative to actor parent"

#define TIP_ACTORCONTROL_WIDTH	"\
Current width"

#define TIP_ACTORCONTROL_HEIGHT	"\
Current height"


#define TIP_ACTORCONTROL_CLONE	"\
Create  a clone of current actor\n\
\n\
A clone has all the animations, events and actions of the original actor.\n\
Actually, there's no difference between the original and cloned actors.\n\
Adding new animations, events or actions to any of them will be affect all the others."

#define TIP_ACTORCONTROL_REMOVE	"Remove the current actor from game"

#define TIP_ACTORCONTROL_MOVE	"\
Use this option to see the actor animation\n\
and the actor move along the path in editor mode"

#define TIP_ACTORCONTROL_INFINITE	"\
Set the infinite options\n\
\n\
An infinite actor repeats itself along the horizontal axis\n\
(x axis) and vertical axis (y axis) of the screen\n\
\n\
Repeat actor disable collision detection on this actor"

#define TIP_ACTORCONTROL_TILE	"\
  Single tile:  Create single tiles\n\
Tile Matching:  Create tile matching"

#define TIP_ACTORCONTROL_TEXT	"\
Use this option to make the current actor have a text"

#define TIP_ACTORCONTROL_ANIMATION_LIST	"\
Change the current actor animation\n\
\n\
Animation is the static image or group of images\n\
in sequence that represents an actor on screen.\n\
By the concept, one can notice that not all\n\
animations are actually \"animated\"\n\
\n\
The actor animation can be changed in the\n\
game with \"Change Animation\" action"

#define TIP_ACTORCONTROL_ANIMATION_ADD	"Add a new animation to the current actor"

#define TIP_ACTORCONTROL_ANIMATION_SEQUENCE	"\
Create a new animation sequence\n\
based on an existing animation"

#define TIP_ACTORCONTROL_ANIMATION_EDIT	"Edit the current animation or sequence"

#define TIP_ACTORCONTROL_ANIMATION_REMOVE	"Remove the current animation or sequence from this actor"

#define TIP_ACTORCONTROL_PARENT		"\
Set actor's parent\n\
\n\
The parent can be any actor in game, included view actor, or no parent.\n\
The child actor will be moved with parent\n\
The (x, y) coordinates of a child actor are relative to the parent\n\
If the parent changes your transparency or color the childs will inherit the changes"

#define TIP_ACTORCONTROL_PATH		"\
Set actor's path\n\
\n\
You can choose:\n\
   - a created path\n\
   - no path\n\
   - random path (selected randomly in the created paths)"

#define TIP_ACTORCONTROL_LOCK		"\
Lock the current actor position"

#define TIP_ACTORCONTROL_EVENTS_ADD		"\
Add a new event to the current actor\n\
\n\
Events are things that happen during game. For example,\n\
when the user presses a key, or clicks with the mouse,\n\
or when one actor collides with another actor.\n\
\n\
All clones share the same events and actions\n\
\n\
Action is what should happen in response to an event.\n\
For example, when an actor collides with a rocket\n\
an explosion should be created.\n\
\n\
Any event can fire one or more actions."

#define TIP_ACTORCONTROL_EVENTS_EDIT		"Edit an actor's actions"

#define TIP_ACTORCONTROL_EVENTS_REMOVE		"Remove an actor's actions"



#define TIP_CLONEARRAY			"\
Use clone array tool to do a multiple clone of your selected actor\n\
\
  Horizontal count: number of actors in a line\n\
    Vertical count: number of actors in a column"

#define TIP_CLONEARRAY_DISTANCE			"\
Automatic: uses the actor dimensions to set distance\n\
   Manual: set the distance manually\n\
\n\
If you choose Manual, the current width and height will be show"




#define TIP_CLONEPATH			"\
Use clone along path tool to do a multiple clone of your selected actor\n\
along the selected path"


#define TIP_OUT_OF_VISION_OPTIMIZATION "\
Set this option to No to optimize your game.\n\
\n\
Actors out of vision with this option set to No\n\
don't will receive this events:\n\
\n\
   Animation Finish\n\
   Collision\n\
   Collision Finish\n\
   Draw Actor\n\
   Key Down\n\
   Key Up\n\
   Mouse Button Down\n\
   Mouse Button Up\n\
   Timer"

#define TIP_INHERIT_EVENTS_FROM "\
Event Inheritance:\n\
\n\
You can use the option 'Inherit events from' to make the current actor inherit events from selected actor.\n\
The current actor will have all your events plus the events from select actor.\n\
\n\
\n\
Example:\n\
\n\
You have  some different 'enemies' actors that move to left and right directions and can jump.\n\
You can create a 'baseEnemie' actor and add the events to move:\n\
\n\
   Right key: to move to right\n\
   Left key: to move to left\n\
   Up key: to jump\n\
\n\
After select the 'enemies' actors and select 'Inherit events from: baseEnemie'.\n\
If you want make some enemy  jump in a different form, just add a new Key Down event with the Up key.\n\
When the user press the Up key, the executed event will be the event defined in the enemy actor."

//////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////
//Add Animation tip strings
#define TIP_ANIMATION			"\
Add a new animation to selected actor\n\
\n\
If the image file has a color depth lower than 32 bits,\n\
Game Editor understands that the color of the first pixel\n\
(upper left corner) is the color that should become totally\n\
transparent in the image. If your image has 32 bits color depth,\n\
Game Editor will consider the alpha channel (transparency channel)\n\
of the image.\n\
If your image is a gif, Game Editor will consider the gif transparent color.\n\
If all pixels in the image have the same color (solid image) the\n\
image will be consider solid\n\
\n\
Set your animation frame rate (frames by second)\n\
If the animation has only one frame, that parameter will be ignored"

#define TIP_ANIMATION_FILE			"\
Select the animation file\n\
\n\
If the image file has a color depth lower than 32 bits,\n\
Game Editor understands that the color of the first pixel\n\
(upper left corner) is the color that should become totally\n\
transparent in the image. If your image has 32 bits color depth,\n\
Game Editor will consider the alpha channel (transparency channel)\n\
of the image.\n\
If your image is a gif, Game Editor will consider the gif transparent color.\n\
If all pixels in the image have the same color (solid image) the\n\
image will be consider solid"

#define TIP_ANIMATION_HORIZONTAL_FRAMES			"\
If the animation is contained in a single file, specify the number of horizontal frames"

#define TIP_ANIMATION_VERTICAL_FRAMES			"\
If the animation is contained in a single file, specify the number of vertical frames"

#define TIP_ANIMATION_FPS			"\
Set your animation frame rate (frames by second)"


#define TIP_ANIMATION_TYPE			"\
If your animation is contained in one file,\n\
select \"Single File\" and specify how many frames\n\
of your animation that file represents.\n\
You may want to set up your animation in a single file\n\
with all the frames side by side. Placing your frames\n\
side by side tells you how many horizontal and\n\
vertical frames are in your animation.\n\
\n\
But you may have your animation in a sequence of files.\n\
In that case, the files should be named in the following way:\n\
\n\
file001.ext, file002.ext, file003.ext,...\n\
\n\
Where \"file\" is the name of your animation and \".ext\" is\n\
the file extension.\n\
\n\
Selecting any of these files and choosing the option \"Multiple Files\"\n\
makes Game Editor automatically load the whole animation for you."

#define TIP_ANIMATION_WEB			"Click here to get more animations on the Internet"

#define TIP_EDIT_FRAME			"\
Click here edit the selected frame.\n\
Before edit, the game must be saved.\n\
You must select an image editor in the Preferences panel.\n\
The image editor need to handle transparent PNG image files."

#define TIP_ANIMATION_PLAY "Click to play the animation"
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_DLG_ACTIVATIONEVENTS			"\
Use this panel to execute actions when an Activation Event was received\n\
\n\
An action can be defined depending on the actor that is sending Activation Event.\n\
Thus, different actions can happen for different actor's Activation Events.\n\
\n\
Activation generated Events don't appear in Actor Control, because they are not shared by the clones."

#define TIP_DLG_ACTIVATIONEVENTS_FROM			"\
Choose the sender actor\n\
\n\
Choose \"Any Actors\" and the action will happen when the actor receives an Activation Event from any linked actor.\n\
Choose \"All Actors\" and the action will happen only when all linked actors send an Activation Event to the actor.\n\
\n\
This option allow the use of special actors that handle logical operations:\n\
   Actors OR are obtained through \"Activation Event (Any Actor)\"\n\
   Actors AND are obtained through \"Activation Event (All Actors)\""
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_DLG_ANIMATIONFINISH			"\
Use the Animation Finish Event to execute actions when the specified animation finishes"

#define TIP_DLG_ANIMATIONFINISH_ANIMATION			"\
Select the animation\n\
\n\
If \"Any Animation\" is selected, the actions will be execute\n\
when any animation for this actor is finished"

#define TIP_DLG_PATH_FINISH			"\
Use the Path Finish Event to execute actions when the current path finishes"


#define TIP_DLG_PATHFINISH_PATH			"\
Select the path\n\
\n\
If \"Any Path\" is selected, the actions will be executed\n\
when any path on the actor finishes."
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_DLG_COLLISION			"\
Use the Collision Event to execute actions when a collision\n\
happens with a specified side of a selected actor"

#define TIP_DLG_COLLISION_SIDE			"\
Choose the collision side on the selected actor.\n\
\n\
In many platform games, when a player collides on top side of a monster, the monster is destroyed.\n\
When a player collides on the left or right side of a monster, the player is destroyed."

#define TIP_DLG_COLLISION_ACTOR			"Select the collision actor"

#define TIP_DLG_COLLISION_REPEAT			"\
Select \"Yes\" to send a Collision Event while the Actor is colliding.\n\
Select \"No\" to send a Collision Event only at the beginning of the collision."



#define TIP_DLG_COLLISION_FINISH			"\
Use the Collision Finish Event to execute actions when\n\
there are no more collisions with the selected actor."

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_DLG_KEYDOWN			"\
Use the \"Key Down Event\" to execute Actions when the specified key or sequence of keys are down.\n\
Right click on the panel to receive this Event for 'any' key.\n\
\n\
Up to 12 keys can be specified."

#define TIP_DLG_KEYDOWN_KEYS			"\
Set the key or sequence of keys"

#define TIP_DLG_KEYUP_KEY			"\
Set the key"

#define TIP_DLG_KEYDOWN_MODE	"\
Execution mode\n\
\n\
Choose: \"At least one key is pressed\" to execute the actions when at least one of the specified keys is pressed.\n\
Choose: \"All keys are pressed\" to execute the actions when all specified keys are pressed.\n\
Choose: \"Keys are pressed in order\" to execute the actions when the specified keys are pressed in order.\n\
         (You can use this mode to make cheat codes for your games)\n\
\n\
Note: If only one key is specified, this option isn't used."

#define TIP_DLG_KEYDOWN_CLEAR	"\
Click to erase all specified keys."

#define TIP_DLG_KEYDOWN_REPEAT			"\
If \"Repeat\" is set to \"Enable\", the Action(s) will be executed while the key is pressed."

#define TIP_DLG_KEYUP			"\
Use the Key Up Event to execute actions when the specified key is released\n\
Right click on the panel to receive this event for any key"
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_DLG_MOUSEBUTTON_DOWN			"\
Use this event to execute action(s) when the specified mouse button is clicked.\n\
Click on the panel with the left, middle or right mouse button in order to specify the button.\n\
On the PocketPC, a stylus tap on the screen will generate a Mouse Button Down Event."

#define TIP_DLG_MOUSEBUTTON_DRAG			"\
Select Enable if actor will be dragged after button down"

#define TIP_DLG_MOUSEBUTTON_BUTTON			"\
Select the mouse button"

#define TIP_DLG_MOUSEBUTTON_UP			"\
Use this event to execute actions when the specified mouse button is released.\n\
Click on the panel with the left, middle or right mouse button in order to specify the button."
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_DLG_TIMER			"\
Use the Timer Event to execute actions when the specified timer elapses\n\
Timer must have been previously created in some action"

#define TIP_DLG_TIMER_TIMER			"\
Select the timer"
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_CHANGEANIMATION			"\
Use this action to change the current animation"

#define TIP_CHANGEANIMATION_ACTOR			"\
Select the actor that will have its animation changed\n"

#define TIP_CHANGEANIMATION_ANIMATION			"\
Select the new animation"

#define TIP_CHANGEANIMATION_DIRECTION			"\
Select the animation direction:\n\
\n\
     Forward: play the animation from beginning to end\n\
    Backward: play the animation from end to beginning\n\
     Stopped: keep the animation stopped\n\
   No change: keep the current animation direction"


//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_GRID_SHOW		"Choose if the editor's grid must be show or hide"
#define TIP_GRID_SNAP		"\
If the snap mode is on, the actors will\n\
be atracted to grid lines"

#define TIP_GRID_MAINCOLOR			"Click to choose the main grid color"
#define TIP_GRID_RESOLUTIONCOLOR	"Click to choose the resolution grid color"

#define TIP_GRID_X			"Set horizontal grid space"
#define TIP_GRID_Y			"Set vertical grid space"

#define TIP_PREFERENCES_TOOLTIP		"Enable or disable tool tips"

#define TIP_PREFERENCES_PERSISTENT_UNDO		"\
If you select \"Yes\" Game Editor will search for the\n\
corresponding .undo file and recover your game history\n\
in next game load.\n\
\n\
The maximum number of desired undo levels.\n\
This setting will take place only if the \"Persistent undo\"\n\
box is set to \"Yes\".\n\
Reduce the \"Maximum undo levels\" setting to decrease\n\
the size of the undo file."

#define TIP_PREFERENCES_IMAGE_EDITOR "\n\
Select an image editor to edit your animation frames.\n\
The image editor need to handle transparent PNG image files.\n\
\n\
If you don't have an image editor, use a search engine and\
put the query \"Image Editors with PNG Support\" to find\
a good image editor."
//////////////////////////////////////////////////////////////////////////////


#define TIP_GAMESETTINGS_BACKGROUNDCOLOR		"Choose the game background color"




#define GAME_EDITOR_VERSION_ERROR	"\
This feature is only available in the Game Editor full version.\n\
Buy now Game Editor to remove this limitation."

#endif


#ifndef STAND_ALONE_GAME
void *GetSDLPanelWindow();
void SetMainWindowGameMode(bool bGameMode);
void ShowMainWindow(bool bShow);
#endif

void *GetMainWindow();


//Marketing functions
#if defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL) && !defined(STAND_ALONE_GAME)
#define BUY_NOW "Buy Now!"
void openBuySite();
#endif


#define SDL_TIMER_EVENT				( SDL_USEREVENT + 0 )
#define SDL_LOAD_GAME__EVENT		( SDL_USEREVENT + 1 )
#define SDL_ACTOR_TIMER__EVENT		( SDL_USEREVENT + 2 )
#define SDL_TUTORIAL_PAINT_START_EVENT	( SDL_USEREVENT + 3 )
#define SDL_TUTORIAL_PAINT_END_EVENT	( SDL_USEREVENT + 4 )
#define SDL_TUTORIAL_TEXT_EVENT			( SDL_USEREVENT + 5 )
#define SDL_POCKET_CONTINUE				( SDL_USEREVENT + 6 )
#define SDL_DROPFILES					( SDL_USEREVENT + 7 )
#define SDL_POCKET_HIBERNATE			( SDL_USEREVENT + 8 )
#define SDL_WAIT_FOR_CONNECT			( SDL_USEREVENT + 9 )

//Network message
#define ID_USER_EXECUTE_ACTION				(ID_USER_PACKET_ENUM + 1)
#define ID_USER_SET_POSITION				(ID_USER_PACKET_ENUM + 2)
#define ID_USER_MOUSE_POSITION				(ID_USER_PACKET_ENUM + 3)
#define ID_USER_LAST_KEY					(ID_USER_PACKET_ENUM + 4)
#define ID_USER_REQUEST_OWNERSHIP			(ID_USER_PACKET_ENUM + 5)
#define ID_USER_RELEASE_OWNERSHIP			(ID_USER_PACKET_ENUM + 6)
#define ID_USER_ADDRESS_CHANGE				(ID_USER_PACKET_ENUM + 7)
#define ID_USER_ENVIRONMENT_INFO_REQUEST	(ID_USER_PACKET_ENUM + 8)
#define ID_USER_ENVIRONMENT_INFO			(ID_USER_PACKET_ENUM + 9)
#define ID_USER_OLD_GAME_VERSION			(ID_USER_PACKET_ENUM + 10)
#define ID_USER_GAME_STATE					(ID_USER_PACKET_ENUM + 11)
#define ID_USER_HANDSHAKE_FINISHED			(ID_USER_PACKET_ENUM + 12)
#define ID_USER_REQUEST_GAME_STATE			(ID_USER_PACKET_ENUM + 13)
#define ID_USER_LOCAL_ACTORS				(ID_USER_PACKET_ENUM + 14)
#define ID_USER_SET_FRAME					(ID_USER_PACKET_ENUM + 15)

#define ID_SERVER_REGISTER			1
#define ID_SERVER_PING_REQUEST		2

#define NET_CHANNEL_ACTIONS		0
#define NET_CHANNEL_POSITION	1
#define NET_CHANNEL_MOUSE		2
#define NET_CHANNEL_FRAME		4

#define NET_RETRY 64

#define MAX_GAME_ID		36
#define MAX_CLIENT_ID	92

//Introdutory feature versions (for tutorial retro compatibility)
#define	VERSION_IDENTATION					0x00100101
#define VERSION_CLONE_ARRAY					0x01020200
#define VERSION_GAME_SUSPEND				0x01020300
#define VERSION_OUT_OF_VISION_OPTIMIZATION	0x01020400
#define VERSION_MOUSE_ENTER_MOUSE_LEAVE		0x01020800
#define VERSION_GET_ANIMINDEX				0x01020800
#define VERSION_ESC_TO_EXIT					0x01020800
#define VERSION_MOVE_TO_RELATIVE			0x01020800
#define VERSION_PREFERENCES					0x01020900
#define VERSION_MOVE_TO_PATHFINDER			0x01030500
#define VERSION_NETWORK						0x01040200


#define GAME_NET_VERSION	1
#define GAME_FILE_VERSION	55
#define GAME_EDITOR_VERSION	0x01030900 //1 byte: Major version, 1 byte: Minor version, 1 byte: Patch level, 1 byte: build

extern gedString EDITOR_DAT;

extern gedString AXIS_NAME;
extern gedString REGION_NAME;
extern gedString ACTIVATION_EVENTS_CANVAS_NAME;
extern gedString ACTOR_TIP_NAME;
extern gedString ACTOR_GED_ICON;
extern gedString ACTOR_GED_TEXT;
extern gedString AXIS_GRID_CANVAS_NAME;
extern gedString AXIS_PATH_CANVAS_NAME;

#ifndef max
#define max(a, b) (a>=b?a:b)
#endif

#ifndef min
#define min(a, b) (a<b?a:b)
#endif


extern KrEngine *engine;



enum
{
	WINDOWS_EXECUTABLE = 0,
	POCKETPC_EXECUTABLE,
	LINUX_EXECUTABLE,
	GAME_DATA_ONLY,
	POCKETPC_DATA,
	GP2X_EXECUTABLE
};

enum
{
	INTEGER_VAR = 0,
	REAL_VAR,
	STRING_VAR
};

enum
{
	LOCAL_VAR = 0,
	GLOBAL_VAR
};

class stVarInfo
{
public:
	stVarInfo() {type = INTEGER_VAR; arrayLenght = 0; addr = 0; size = 0; bInternalAddr = false;}
		
	stVarInfo(const gedString& _name, Uint8 _type, int _arrayLenght, const gedString& _saveGroup, void *_addr, int _size) 
	{ 
		name = _name;
		type = _type;
		arrayLenght = _arrayLenght;
		saveGroup = _saveGroup;
		addr = _addr;
		size = _size;

		bInternalAddr = false;
	}

	virtual ~stVarInfo()
	{
		if(bInternalAddr && addr)
		{
			free(addr);
		}
	}

	void operator=( const stVarInfo& copy ) 
	{ 
		if( &copy != this )
		{
			name = copy.name;
			type = copy.type;
			arrayLenght = copy.arrayLenght;
			saveGroup = copy.saveGroup;
			size = copy.size;
			
			if(bInternalAddr || copy.bInternalAddr)
			{
				if(!addr) 
				{
					addr = malloc(size);
					bInternalAddr = true;
				}
				
				if(addr)
				{
					memcpy(addr, copy.addr, size);				
				}
			}
			else
			{
				addr = copy.addr;
			}
		}
	}

	bool operator==( const stVarInfo& copy ) 
	{ 
		//Not need to be in the save group
		return	name == copy.name && 
				type == copy.type &&
				arrayLenght == copy.arrayLenght &&
				size == copy.size;
	}

	void Load(SDL_RWops* src);
	void Save(SDL_RWops* src);

	void LoadValue(SDL_RWops* src);
	void SaveValue(SDL_RWops* src);
	

	gedString name; //Var name
	Uint8 type; //Integer, real, string
	int arrayLenght; // > 0 var is an array
	gedString saveGroup; //If != '' must be save in this group

	void *addr; //Internal script address to save and load actions (don't save)
	int size; //Internal script size
	bool bInternalAddr; //add is allocated here?
};

typedef GlMap< gedString, stVarInfo, GlStringHash >		MapVars;
typedef GlMapIterator< gedString, stVarInfo, GlStringHash >		MapVarsIterator;



class Actor;
class ActorMessage
{
public:
	ActorMessage(Actor *_fromActor, Actor *_toActor, Uint32 _messageId, Uint32 _message, const char *_nameActorTo)
	{
		fromActor = _fromActor;
		toActor = _toActor;
		messageId = _messageId;
		message = _message;

		if(!nameActorTo) nameActorTo = new gedString(_nameActorTo);
		else *nameActorTo = _nameActorTo;
	}

	Actor *getActorTo() {return toActor;};
	Actor *getActorFrom() {return fromActor;};
	int getMessageId() {return messageId;};
	int getMessage() {return message;};

	const gedString& getNameActorTo() {return *nameActorTo;}
	
	void *operator new(size_t size)
	{
		ActorMessage *m = NULL;

		if(freeList.Count())
		{
			m = (ActorMessage *)freeList.Back();
			freeList.PopBack();
			return m;
		}
		
		//No itens available in the free list, so allocate new
		m = (ActorMessage *)dlmalloc(size);
		m->nameActorTo = NULL;

		return m;
	}
	
	void operator delete(void * mem)
	{
		if(mem) 
		{		
			//Put in the free list
			freeList.PushBack(mem);		
		}
	}

	static void Remove()
	{
		//Delete itens in the free list
		int n = freeList.Count();
		for(int i = 0; i < n; i++)
		{
			ActorMessage *p = (ActorMessage *)freeList[i];
			if(p)
			{
				delete p->nameActorTo;
				delete p;	
			}
		}

		freeList.Clear();
	}
	
private:
	Actor *fromActor;
	Actor *toActor;
	Uint32 messageId;
	Uint32 message;

	//Keep the actor name
	//So, can remove from maps that need the name even if toActor not exists anymore 
	//See: REMOVE_ME_FROM_WORKING_MAP
	//Speedup collision test of "A Bloody Mess.ged"
	//In slow machines the Action::CheckCollisionsBetween will slow down the game
	gedString *nameActorTo; 

	//maks: to avoid allocations and sppedup
	static GlDynArray<void *> freeList;
};

class PathFinderMessage
{
public:
	PathFinderMessage() {}

	PathFinderMessage(Actor *_toActor, char *_obstacleActor, double _posX, double _posY, double _endX, double _endY, double _width, double _height, double _velocity)
	{
		toActor = _toActor;
		obstacleActor = _obstacleActor;
		posX = _posX;
		posY = _posY;
		endX = _endX;
		endY = _endY;
		width = _width;
		height = _height;
		velocity = _velocity;
	}

	void operator=( const PathFinderMessage& copy ) 
	{ 
		if( &copy != this )
		{
			toActor = copy.toActor;
			obstacleActor = copy.obstacleActor;
			posX = copy.posX;
			posY = copy.posY;
			endX = copy.endX;
			endY = copy.endY;
			width = copy.width;
			height = copy.height;
			velocity = copy.velocity;
		}
	}

	Actor *toActor;
	gedString obstacleActor;
	double posX;
	double posY;
	double endX;
	double endY;
	double width;
	double height;
	double velocity;
};

class Path;

typedef GlMap< void *, char, GlPointerHash<void *> >		MapPointer;
typedef GlMapIterator< void *, char, GlPointerHash<void *> >		MapPointerIterator;

typedef GlDynArray<Actor *> ArrayActor;  

class GlSystemAddressHash : public GlHash
{
  public:
	GlSystemAddressHash( const SystemAddress& addr )
	{	
		val = addr.binaryAddress + addr.port;	
	}
};


typedef GlSList<ActorMessage *> QueueMessage;  //push = PushBack, pop = PopFront
typedef GlSList<PathFinderMessage *> QueuePathFinder;  //push = PushBack, pop = PopFront
typedef GlSList<unsigned short> LastKeys;  //Add = PushFront, Remove = PopBack
typedef GlSListIterator<unsigned short> LastKeysIterator;
typedef GlSListIterator<PathFinderMessage *> QueuePathFinderIterator;


typedef GlMap< gedString, Path *, GlStringHash >		MapPath;
typedef GlMapIterator< gedString, Path *, GlStringHash >		MapPathIterator;

typedef GlMap< gedString, Script *, GlStringHash >		MapExpression;
typedef GlMapIterator< gedString, Script *, GlStringHash >		MapExpressionIterator;

#ifndef USE_AVL_TREE
typedef GlMap< gedString, ListActor, GlStringHash >		MapActor;
typedef GlMapIterator< gedString, ListActor, GlStringHash >		MapActorIterator;
#else
typedef CTTree< gedString, ListActor >		MapActor;
typedef CTTreeIterator< gedString, ListActor >		MapActorIterator;
#endif

typedef GlMap< gedString, Actor *, GlStringHash >		MapRunningActor;


typedef GlMap< Actor *, char, GlPointerHash<Actor *> >		GlobalMapActor;
typedef GlMapIterator< Actor *, char, GlPointerHash<Actor *> >		GlobalMapActorIterator;

typedef GlMap< Actor *, U32, GlPointerHash<Actor *> >		CacheMapActor;
typedef GlMapIterator< Actor *, U32, GlPointerHash<Actor *> >		CacheMapActorIterator;



typedef GlMap< gedString, Mix_Chunk *, GlStringHash >		MapSound;
typedef GlMapIterator< gedString, Mix_Chunk *, GlStringHash >		MapSoundIterator;

typedef GlMap< SDLKey, char, GlNumberHash<SDLKey> >		MapKeyDown;
typedef GlMapIterator< SDLKey, char, GlNumberHash<SDLKey> >		MapKeyDownIterator;

typedef GlMap< gedString, Uint32, GlStringHash >		MapGameFileIndex;
typedef GlMapIterator< gedString, Uint32, GlStringHash >		MapGameFileIndexIterator;

typedef GlMap< SDLKey, SDLKey, GlNumberHash<Uint16> >		MapRemapKeys;
typedef GlMapIterator< SDLKey, SDLKey, GlNumberHash<Uint16> >		MapRemapKeysIterator;


class stActorState
{
public:
	stActorState() {bDestroyed = false; index = 0;}
	stActorState(Uint32 idx) {bDestroyed = false; index = idx;}
	void operator=( const stActorState& copy ) 
	{ 
		if( &copy != this )
		{
			bDestroyed = copy.bDestroyed;
			index = copy.index;
		}
	}

	bool bDestroyed;
	Uint32 index;
};
typedef GlMap< gedString, stActorState, GlStringHash >		MapActorFileIndex;
typedef GlMapIterator< gedString, stActorState, GlStringHash >		MapActorFileIndexIterator;


class PathPoint;
class Tile;
class Actor;


class GameControl  
{
public:

#if defined(GAME_EDITOR_PROFESSIONAL) && defined(WIN32) && !defined(STAND_ALONE_GAME)
	void setExecuteUpToFrame(long value) {executeUpToFrame = value;}
	void setCheckFrame(long value) {checkFrame = value;}
	int getCheckFrame() {return checkFrame;}
#endif
	
	static bool CanDraw();

	void AddWorkingActor(Actor *actor);
	void RemoveWorkingActor(Actor *actor);
	void AddToUnlinkedList(const SystemAddress &owner, Actor *actor);
	Actor *GetActorFromUnlinkedList(const SystemAddress &owner, const char *actorName);
	Actor *GetNetActor(const SystemAddress &owner, const char *actorName, long cloneIndex);
	void RemoveActorFromUnlinkedList(Actor *actor);

	void SaveVars(char *file, char *group);
	void LoadVars(char *file, char *group);
	void ResetVars();
	
	bool getExportMode() {return bExportMode;}
	bool getAudioOpened() {return bAudioOpened;}
		
	SDLKey PocketPCKey(SDLKey from);
	SDLKey UserDefinedKeys(SDLKey from);
	SDLKey GetLastKey(const SystemAddress& owner);
	void MapUserKey(SDLKey from, SDLKey to);

	static Uint32 getLastGameFileVersion() {return lastGameFileVersion;}

	void SuspendGame(int bSuspendOn);
	void PauseGame(int bPauseOn);
	void StartGameRender();

	void setFlipPocketPCScreen(bool value) {bFlipPocketPCScreen = value;}
	bool getFlipPocketPCScreen() {return bFlipPocketPCScreen;}

	void PushFindPathRequisition(Actor *actor, char *obstacleActor, double posX, double posY, double endX, double endY, double width, double height, double velocity);

	void RemoveOldActorsFromCache(bool bForce = false);
	RakPeerInterface *GetRakPeer() {return rakPeer;}
	bool RequestNetworkStart();
	bool WaitingForConnectionComplete() {return bWaitingForConnectionComplete;}
	void ProcessNetwork(bool bFromFrameTick);	
	void SendRemoteAction(
			int sendOrder,
			U16 actionNumber, const gedString& actionName, 
			const gedString& eventActorName, long eventActorCloneIndex, const RakNetTime &eventActorOwnershipTimestamp,
			const gedString& collideActorName, long collideActorCloneIndex,
			bool bExecuteConditional, bool bForceExecute,
			unsigned int seed
			);

	void PushActionInGlobalQueue(		    
			U16 actionNumber, const gedString& actionName, 
			const gedString& eventActorName, long eventActorCloneIndex, const RakNetTime &eventActorOwnershipTimestamp,
			const gedString& collideActorName, long collideActorCloneIndex,
			bool bExecuteConditional		
			);

	void SendRemotePosition(Actor *actor, float x, float y, bool bReliableSend);
	void SendRemoteFrame(Actor *actor, bool bReliableSend);
	void GetMousePosition(int &mousex, int &mousey, const SystemAddress& owner);

	bool RequestActorOwnership(const char *cloneName);
	bool ReleaseActorOwnership(const char *cloneName);

	SystemAddress GetOwnerMachine();
	
#ifndef STAND_ALONE_GAME
	void SaveEmergencyRecovery();
	void RestoreEditorCanvas();
	void SaveEditorCanvas();

	void IgnoreEditorResources();
	void MapPocketPCKey(SDLKey pocketKey, SDLKey pcKey);
	void PopulatePocketPCKeys(ListPop *list);
	gedString AdjustFileName(gedString filePath);
	void PopulatePaths(ListPop *list, bool bNoPaths = true);
	void PopulatePaths(ListString &list, bool bNoPaths = true);
	void PopulateAnimations(Actor *actor, ListPop *list, bool bAddSequences = true);
	void PopulateAnimations(Actor *actor, ListString &list, bool bAddSequences = true);
	void PopulateTimers(ListPop *list, bool bShowParam = true);
	void PopulateTimers(ListString &list, bool bShowParam = true);
	void PopulateActors(ListPop *list, bool bGetClones = true);
	void PopulateActors(ListString &list, bool bGetClones = true, bool bNewInterface = false);
	void PopulateGlobalUserVars(ListPop *list);
	void PopulateGlobalUserVarsGroups(ListPop *list);
	bool Save(const gedString& gameName, SDL_RWops *src = NULL);
	bool Save(bool bExport);
	bool ExportGame(const gedString& exportName, int exportType);

	void RestoreGame();
	void StoreScreenSize();
	
	void UpdateToolTip(gedString tip);
	static void EnableAddFrameRate() {bCanAddFrameRateEvent = true;}
	bool getSnapMode() {return bActorSnap;}

	KrRGBA getBackGroundColor() {return backgroundColor;}	
#endif //#ifndef STAND_ALONE_GAME

	void setBackGroundColor(KrRGBA _backgroundColor);

	static bool getGameEngineIsRunning() {return bGameEngineIsRunning;}

	void InitPocketPCKeys();

	bool getDestroingGame() {return bDestroingGame;}
	void ClearIcon();
	void LoadIcon();
	void PrepareRegions();
	Actor * GetEditView();
	Actor *ExistsClone(char *actorName, int cloneIndex);
	void AddMaxCloneIndex(const gedString &name, int index);
	int * GetMaxCloneIndex(const gedString& name);

#ifdef ACTOR_USES_VIRTUAL_FUNCTIONS
	inline bool isValidActor(Actor *actor) {return actor && globalMapActor[actor] != NULL;}
#endif

	void RemoveExpression(const gedString& expressionName);
	Uint32 GetGameFileVersion() {return indexedGameVersion;}
	Path *LoadPath(const gedString& name);
	Uint32 GetActionIndex(const gedString& actionName);
	Uint32 GetScriptIndex(const gedString& scriptName);
	Uint32 GetActorIndex(const char *actorName, bool bExplicitlyCreateActor);
	void SetDestroyedActor(Actor *actor);
	//void SetDestroyedActor(const gedString& actorName);
	SDL_RWops *GetGameFilePointer() {return indexedGame;};
	//static Uint32 RealFrameRateCallback(Uint32 interval, void *param);
	
#ifdef APPLICATION_THREAD_TIMERS
	static int TimerCallback( void *pParam );
#else
	static Uint32 TimerCallback(Uint32 interval, void *param);
#endif

	void StartKyraSplash() {startKyraSplash = SDL_GetTicks(); engine->StartSplash(startKyraSplash);}
	void SetMainActor(Actor *actor) {mainActor = actor;}
	bool GameTick(SDL_Event &event);
	void HandleMouseMotion();
	void ToggleSnap();
	bool ImageSnap(KrImage *image, KrImage *root, int *pMoveX = NULL, int *pMoveY = NULL);
	Actor *GetViewActor() {return viewActor;}
	void SetViewActor(Actor *view);
	bool getStandAloneMode() {return bStandAloneMode;}
	bool CheckStandAloneMode(gedString executableName);
	
	bool getSaveCache() {return bSaveCache;}
	void setSaveCache(bool value) {bSaveCache = value;}

	int getExpressionsCount() {return expressions.size();}
	
	static bool Write(SDL_RWops *context, const void *ptr, int size, int num);
	static bool Read(SDL_RWops *context, void *ptr, int size, int num);
	static Uint16 Read16(SDL_RWops *context);
	static Uint32 Read32(SDL_RWops *context);
	char * GetErrorMsg();
	bool SwitchResolution(SDL_Surface* screen, int width, int height, bool bFullScreen = false);
	void SetTimer(int fps = 0);
	
	void Delete();
	int PlaySound(bool bMusic, const gedString& path, int volume, int loop, int priority, Sint16 pan);
	bool CreateUserVar(gedString varName, Uint8 _type, int _arrayLenght, gedString _saveGroup);
	gedString AddExpression(Script *pScript);
	Script *GetExpression(const char *expressionName);
	void ClearGame();
	void NewGame(bool bResetMainPanel = false);
	const gedString& getGameName() {return gameName;};
	void setGameName(gedString value) {gameName = value;};
	void setGamePath(gedString value) {gamePath = value;};
	bool Load(SDL_RWops *src, bool bLoadCursor = true, bool bMergeGames = false, Uint32 offsetKyraDat = 0, bool bShowErrorMessage = true);
	bool LoadV8(SDL_RWops *src, bool bLoadCursor, bool bMergeGames, Uint32 version);
	bool Load(const gedString& gameName, bool bLoadCursor = true, bool bMergeGames = false, bool bShowErrorMessage = true);
	bool UpdateLoadedActors(SDL_RWops *src, bool bClose);
	
	Axis *GetAxis() {return axis;}
	void CreateAxis();
	bool getGameMode() {return bGameMode;};
	bool SetGameMode(bool bGameMode, bool bSwitchResolution = true);
	
	stTimerDefinition *GetTimer(gedString timerName);
	int GetNumTimers();
	bool AddTimerDefinition(const gedString& timerName, Uint32 interval, int count, int type, Uint32 minRandomInterval);
	
	int NumActors(bool bRunning = false);
	gedString ActorName(int i);
	Path *GetPath(const gedString& pathName);
	gedString PathName(int i);
	int NumPaths();
	bool ExistsPath(const gedString& pathName);
	void RemovePath(const gedString& pathName);
	void AddPath(const gedString& pathName, int pathLenght);
	PathPoint *GetPathRoot();
	
	void ChangePathPoint(PathPoint *point);
	
	void SetTileMode(Tile *tile);
	Tile *GetTile() {return currentTile;}
	bool PathMode();
	void SetPathMode(bool bPathMode);
	Actor *GetActor(const gedString &actorName, bool bRunning = false, bool bExplicitlyCreateActor = false, bool bAllowLoad = true);
	bool InModal(Actor *actor);
	void SetModal(Actor *actor);
	Actor *GetModal();
	int Width();
	int Height();
	KrFontResource *GetFont(const gedString &fontName, int iniASCIIChar = 32, int nChars = 94);
	
	
	Actor *getDragActor();
	
	void RemoveActor(Actor *actor, bool bNotifyActors = false);

	void AddActorToHandledCollisionMap(Actor *actor);
	void RemoveActorFromHandledCollisionMap(Actor *actor);
	void CheckCollisions();
	void AddActorsInCollisionInActualFrame(stActorsInCollision2 &actorsInCollision);
	bool IsActorsInCollisionInActualFrame(stActorsInCollision2 &actorsInCollision);

	void SetAnteriorPositions();

	void AddActor(Actor *actor);
	void AddRunningActor(Actor *actor);
	


	void ProcessMessage();
	void PostMessage(Actor *from, gedString &toName, Uint32 messageId, Uint32 message);
	void PostMessage(Actor *from, Actor *to, Uint32 messageId, Uint32 message);
	static GameControl * Get() {return gameControl;}
	GameControl();
	virtual ~GameControl();
	unsigned long getGameFrame() {return gameFrame;};
	void incGameFrame() {gameFrame++;};
	gedString getGamePath() {return gamePath;};
	gedString getOldGamePath() {return oldGamePath;};
	gedString getTmpGamePath() {return tmpGamePath;};

	//Home path come from argv[0]
	//So, editorPath must come from argv[0]
	//Solve the bug when load a .ged from explorer
	//Don't works on linux (pass . directory, not /usr/ ...)
	void setHomePath(const gedString path) 
	{
		homePath = path; 

#ifndef linux
		editorPath = path;
#endif
	}

	gedString getHomePath() {return homePath;};

	void SetMouse(int x, int y) {mouseX = x; mouseY = y;};
	int getMouseX() {return mouseX;};
	int getMouseY() {return mouseY;};

	const LastKeys& getLastKeys() {return lastKeys;}
	const MapKeyDown& getMapKeyDown() {return mapKeyDown;}

	MapVars& getVars() {return globalUserVars;}
	bool existsVar(const gedString& name) {return globalUserVars[name] != NULL;}

	static const gedString& getEditorPath() {return editorPath;}

	Uint16 getGameWidth() {return resX;}
	Uint16 getGameHeight() {return resY;}
	bool getFullScreen() {return bFullScreen;}
	Uint32 getFrameTimeInterval() {return frameTimeInterval;};
	Uint16 getFrameRate() {return fps;}
	int getRealFrameRate() {return realFrameRate;}
	Uint16 getAudioSamplerRate() {return audioSamplerRate;}
	bool getStereo() {return bStereo;}
	Uint16 getMaximumSounds() {return maximumSounds;}
	bool getShowMouse() {return bShowMouse;}
	bool getSuspendGameIfLostFocus() {return bSuspendGameIfLostFocus;}
	bool IsGameSuspended() {return bSuspendGame;}
	bool getUseESCKeyToExit() {return bUseESCKeyToExit;}
	bool getUseMotionCorrection() {return bUseMotionCorrection;}
	void setUseMotionCorrection(bool value) {bUseMotionCorrection = value;}

	gedString getGameID() {return gameID;}
	void setGameID(gedString value) {gameID = value;}

	gedString getClientID() {return clientID;}
	void setClientID(gedString value) {clientID = value;}

	U32 getGameVersion() {return gameVersion;}

	bool getUseGameEditorServer() {return bUseGameEditorServer;}
	void setUseGameEditorServer(bool value) {bUseGameEditorServer = value;}

	int getConnectionLostTimeOut() {return connectionLostTimeOut;}
	void setConnectionLostTimeOut(int value) {connectionLostTimeOut = value;}

	bool getAutoStartNetwork() {return bAutoStartNetwork;}
	void setAutoStartNetwork(bool value) {bAutoStartNetwork = value;}

	int getGamePort() {return gameNetworkPort;}
	void setGamePort(int value) {gameNetworkPort = value;}

	

	bool getFastFile() {return bFastFile;}
	void setFastFile(bool value) {bFastFile = value;}

	U32 getViewSafeMargin() { return viewSafeMargin;} 


	void SetGameProperties(Uint16 resX, Uint16 resY, Uint16 fps, bool  bFullScreen,
		Uint16  audioSamplerRate, bool bStereo, Uint16  maximumSounds, bool bShowMouse,
		gedString iconPath, gedString gameTitle, bool bSetViewPos, KrRGBA _backgroundColor,
		bool _bSuspendGameIfLostFocus, U32 _viewSafeMargin, bool _bUseESCKeyToExit);

	const Script *getScript() {return &script;}

	void LoadGameEvent(char *gamePath);
	const gedString& getLoadGameEvent() {return loadGameEvent;}

	const gedString& getGameTitle() {return gameTitle;}
	const gedString& getIconPath() {return iconPath;}

	void ReadEditorResources();

	bool Modified() { return bModified;}  //<Odilon> for confirmations about 'New' and 'Exit'
	void setModified() {bModified = true;}

	void AddEditActor(Actor *actor);
	Actor *FindEditActor(gedString &name);
	void RemoveEditActor(gedString &name);

	int InitRakNet(int port);
	void DestroyRakNet();

	bool Connect( const char *host, unsigned short remotePort, bool bForceDirectConnection = false);
	
	//KrRect&	getRectView() {return rectView;}
	

private:
#ifndef STAND_ALONE_GAME
	void SaveIcon();
	bool InsertResourceIcon(U8 **peFile, int &peSize);
	bool SetPocketPC_VGA(U8 **peFile, int &peSize, bool bUseVGA);
	void PocketPCKeyDesc(ListPop *list, SDLKey key);
	void SaveFingerPrint(SDL_RWops *src);
	

	int screenEditWidth, screenEditHeight;
#endif

	gedString GenerateUniqueID();

#ifdef USE_RTREE
	static bool RTreeSearchCallback(int id, void* arg);
	ArrayActor visibleActors;
#endif

	RakPeerInterface *rakPeer;
	SystemAddress currentOwnerMachine;
	
	//NatPunchthrough natPunchthrough;
	GlMap<SystemAddress, KrVector2, GlSystemAddressHash> mapRemoteMousePos;
	GlMap<SystemAddress, SDLKey, GlSystemAddressHash> mapRemoteLastKey;

	GlMap<SystemAddress, U32, GlSystemAddressHash> mapRequestPing;
	GlMap<SystemAddress, U32, GlSystemAddressHash> mapConnectionLost;
	GlMap<SystemAddress, U32, GlSystemAddressHash> mapPendingConnection;
	GlMap<SystemAddress, U32, GlSystemAddressHash> mapConnections;
	GlMap<SystemAddress, ListActor, GlSystemAddressHash> mapUnlinkedActors;	
	GlMap<SystemAddress, QueueAction, GlSystemAddressHash> mapActionQueue;

	GlDynArray<RakNet::BitStream *> pendingActions;
	
	bool bWaitingForConnectionComplete;

	void ProcessPingRequests();
	void ProcessLostConnections();
	void ProcessPendingConnections();

	bool IsRemoteAddress(const SystemAddress& addr);
	inline bool CanAcceptPacket(Packet *p);
	
	void CreateStringTable();
	void PingThroughGameServer(const SystemAddress& addr);
	void PublishGameSession();
	void SendIDToServer();

	bool ExecuteAction(const SystemAddress& addr,
			U16 actionNumber, const gedString& actionName, 
			const gedString& eventActorName, long eventActorCloneIndex, const RakNetTime &eventActorOwnershipTimestamp,
			const gedString& collideActorName, long collideActorCloneIndex,
			bool bExecuteConditional, bool bForceExecute,
			bool bRemote,
			unsigned int seed);

	bool ExecuteRemoteAction(RakNet::BitStream &in, RakNetTime &timeStamp, SystemAddress &addr);
	bool ExecuteRemotePosition(RakNet::BitStream &in, RakNetTime &timeStamp, SystemAddress &addr);
	void SetExternalSystemAddress(const SystemAddress& addr);

	void SendRemoteMousePosition(bool bReliableSend);
	bool ExecuteRemoteMousePosition(RakNet::BitStream &in, RakNetTime &timeStamp, SystemAddress &addr);

	bool ExecuteRemoteFrame(RakNet::BitStream &in, RakNetTime &timeStamp, SystemAddress &addr);

	void SendRemoteLastKey();
	bool ExecuteRemoteLastKey(RakNet::BitStream &in, RakNetTime &timeStamp, SystemAddress &addr);

	void SendRemoteRequestActorOwnership(const char *actorName, long cloneIndex, const RakNetTime &timeStamp, SystemAddress addr = UNASSIGNED_SYSTEM_ADDRESS);
	bool ExecuteRemoteRequestActorOwnership(RakNet::BitStream &in, RakNetTime &timeStamp, SystemAddress &addr);

	void SendRemoteReleaseActorOwnership(const char *actorName, long cloneIndex);
	bool ExecuteRemoteReleaseActorOwnership(RakNet::BitStream &in, RakNetTime &timeStamp, SystemAddress &addr);
	
	bool ExecuteTimeStampedMessages(Packet *packet);

	void SendRemoteAdressUpdate(const SystemAddress &oldAddr);
	bool ExecuteRemoteAdressUpdate(RakNet::BitStream &in, RakNetTime &timeStamp, SystemAddress &addr);

	bool HandleNewConnection(SystemAddress &addr);
	bool HandleEnvironmentInfoRequest(SystemAddress &addr);
	bool HandleEnvironmentInfo(Packet *packet);
	void SendGameState(SystemAddress &addr);
	bool ExecuteGameStateUpdate(Packet *packet);
	bool HandleHandShakeFinished(Packet *packet);

	
	
	void SendOnlyStateOfActorsControlledLlocaly(SystemAddress &addr);
	bool ExecuteStateUpdateOfRemoteControlledActors(Packet *packet);

	void WriteActorIndex(RakNet::BitStream& out);
	void WriteGlobalVars(RakNet::BitStream& out);
	void WriteLiveActors(RakNet::BitStream& out, KrImNode *pNode, bool bOnlyLocallyControlledActors);

	void ReadActorIndex(RakNet::BitStream& in);
	void ReadGlobalVars(RakNet::BitStream& in);
	void ReadLiveActors(RakNet::BitStream& in, bool bDeleteAll, const SystemAddress &addr);

	gedString LoadFingerPrint(SDL_RWops *src);

#ifdef DEBUG
	void RemoveFingerPrint(SDL_RWops *src);
#endif

	void ClearPathFinder();
	Path *CalculatePathFinder(GeoPathFinder &gp, PathFinderMessage &req);
	static int PathFinderThread( void *pParam );

	void SDL_Pause(int bPause);
	void PauseNetwork(int bPause);

	void ProcessActor(Actor *actor);
	void ProcessActions();
	RakNetTime GetNextGVT();
	void DoKeyDownRepeat(Actor *actor);
	void ProcessKeyDownRepeat();

	void MoveView();
	bool AddToActorCache(Actor *actor);
	


	void CheckSurfaceLost();
	

	static int LoadMusicThread( void *pParam );
	void UpdateActorTip(Actor *actor);
	void ReleaseActorTip(bool bDisableTips);

	void WriteEditorResources();
	void CreateEngines();
	void AddEditorResource(char *fileName);

	static unsigned long gameFrame;	//global game frame count
	static int realFrameRate;
	static GameControl *gameControl;
	

	Actor *viewActor;
	//KrRect	rectView;
	

#ifdef ACTOR_USES_VIRTUAL_FUNCTIONS
	GlobalMapActor globalMapActor; //For delete control
#endif

	GlobalMapActor handledCollisionMap; //For check collision optimization
	CacheMapActor cacheActor; //For actor cache control
	MapRunningActor mapRunningActor, mapEditActor; //To speed up GetActor

	QueueMessage		queueMessage;
	QueuePathFinder		queuePathFinder;
	ArrayActor			modal;
	MapPath				paths;
	MapTimerNameDefinition	mapTimerNameDefinition;
	bool bPathMode;
	bool bGameMode;
	bool bLoadGameUsedInGameMode;
	Axis *axis;

	gedString gameName, gamePath, oldGamePath, tmpGamePath, iconPath, homePath, gameTitle;
	static gedString editorPath;

	MapExpression expressions;
	int mouseX, mouseY;
	bool bMouseButtonDown;

	
	MapSound			mapSound;
	Mix_Music			*music;	

	//Game properties
	Uint16 resX, resY;
	bool  bFullScreen;
	Uint16	fps;
	Uint16  audioSamplerRate;
	bool bStereo;
	Uint16  maximumSounds;
	bool bShowMouse;
	bool bSuspendGameIfLostFocus, bUseESCKeyToExit;
	KrRGBA backgroundColor;
	U32 viewSafeMargin;

	bool bFlipPocketPCScreen;

	bool bSaveCache;

	static Uint32 frameTimeInterval, realFrameTimeInterval, lastTick;
	double motionCorrection;
	bool bUseMotionCorrection;

	bool bUseGameEditorServer;
	bool bAutoStartNetwork;
	bool bRestartNetworkAfterPause;
	SystemAddress gameEditorServer;
	int gameNetworkPort, currentListenPort;
	gedString gameID, clientID;
	U32 gameVersion;
	int connectionLostTimeOut;

#ifdef APPLICATION_THREAD_TIMERS
	SDL_Thread *timerThread;
#else
	SDL_TimerID timerId/*, timerRealFpsCalcId*/;
#endif
	
	Script script; //Global game script

	gedString loadGameEvent; //To LoadGame script function
	bool bStandAloneMode, bActorSnap;

	//Main loop control
	Actor *mainActor;
	Actor *currentActor, *actorButtonDown;
	Actor *actorDrag;
	Actor *actorModal;
	PathPoint *pathPoint;
	Tile *currentTile;
	int mouseActorDownX, mouseActorDownY, itemSelect;
	MapKeyDown mapKeyDown;
	LastKeys lastKeys;
	Uint32 startKyraSplash;

	static bool bCanAddFrameRateEvent;

	//For indexed game file
	SDL_RWops*			indexedGame; 
	Uint32				indexedGameVersion;
	MapGameFileIndex	actionIndex;
	MapGameFileIndex	pathIndex;
	MapGameFileIndex	scriptIndex;
	MapActorFileIndex	actorIndex;
	
	long tipTime; //Count to call actor tip
	
	//Set modified true when:
	//	- Hit any key on Edit Box
	//	- Click on Edit Actor
	//	- Click on Button

	//Set modified false when:
	//	- New Game
	//	- Save Game
	

	bool bModified;  //<Odilon> for confirmations about 'New' and 'Exit'
	SDL_RWIndex *sdlAudioIndex;

	bool bDestroingGame;

	MapRemapKeys pocketKeys, userKeys;
	SDLKey lastKey;

	MapActorsInCollision2 mapActorsInCollisionInActualFrame;

	static bool bGameEngineIsRunning;
	bool bCheckOutOfVision;
	bool bExportMode;
	bool bAudioOpened;

	bool bPauseGame;
	bool bSuspendGame;
	bool bCacheClean;

	bool bFastFile; //Use this flag to optimize undo operations

	bool bInitialCheck; //To call the initial message panels


	static Uint32 lastGameFileVersion;

#if !defined(GAME_EDITOR_PROFESSIONAL) && defined(STAND_ALONE_GAME)
	Text *gameEditorInformation;
#endif

#if defined(GAME_EDITOR_PROFESSIONAL) && defined(WIN32) && !defined(STAND_ALONE_GAME)
	long checkFrame, executeUpToFrame, totalFrames;
#endif

	MapVars				globalUserVars;		//Custom user vars

	//EiC memory reset
	size_t u_ENV_bot, u_lut_bot, u_pp_bot, u_xalloc_bot;
};

#if defined(WIN32) && !defined(_WIN32_WCE)
class NormalPriority  
{
public:
	NormalPriority(bool _bEnable = true);
	virtual ~NormalPriority();

private:
	bool bEnable;
};
#endif

extern MapActor mapActors, mapWorkingActors;

#ifdef ACTOR_USES_VIRTUAL_FUNCTIONS
#	define IS_VALID_ACTOR(actor) (GameControl::Get()->isValidActor(actor))
#	define IS_VALID_ACTOR1(actor) (actor && globalMapActor[actor])
#else
#	define IS_VALID_ACTOR(actor) (actor && *(U32*)actor == (U32)actor) //when use magic
#	define IS_VALID_ACTOR1(actor) IS_VALID_ACTOR(actor)
#endif



#endif // !defined(AFX_GAMECONTROL_H__9515E689_DC7A_4CE3_87DD_93F087A7ED24__INCLUDED_)
