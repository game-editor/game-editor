/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com

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
More info at http://game-editor.com/License
Be a Game Editor developer: http://game-editor.com/Sharing_Software_Revenues_in_Open_Source

***************************************************************************/


// Action.h: interface for the Action class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACTION_H__9381DB4D_3672_4C48_A5B2_3A3405E175A9__INCLUDED_)
#define AFX_ACTION_H__9381DB4D_3672_4C48_A5B2_3A3405E175A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Actor.h"
#include "gedString.h"
#include "gui.h"
#include "Envelope.h"


#define NO_SELECTION	"(none)"
#define RANDOM_PATH	"random path"

#define NO_PATH		NO_SELECTION//"no path"
#define NO_PARENT	NO_SELECTION//"no parent"




#define MAX_KEY_SEQUENCE 12 //12*2 = 24 bytes, don't use more sizeof(keyEvent) <= 32

extern gedString ANY_ACTOR;
extern gedString ALL_ACTORS;
extern gedString ANY_ANIMATION;
extern gedString ANY_PATH;


#define S_COLLIDE_ACTOR			"Collide Actor"
#define S_COLLIDE				"collide"
#define COLLIDE_ACTOR			((Actor *)4)

#define S_EVENT_ACTOR			"Event Actor"
#define EVENT_ACTOR				((Actor *)1)

#define S_PARENT_ACTOR			"Parent Actor"
#define S_PARENT				"parent"
#define PARENT_ACTOR			((Actor *)2)

#define S_CREATOR_ACTOR			"Creator Actor"
#define S_CREATOR				"creator"
#define CREATOR_ACTOR			((Actor *)3)

#define S_NONE					"Game Center"
#define S_MOUSE					"Mouse Position"
#define S_SCREEN				"screen"


#define EV_ANIMATION			"Draw Actor"
#define EV_ANIMATION_FINISH		"Animation Finish"
#define EV_PATH_FINISH			"Path Finish"
#define EV_CREATE				"Create Actor"
#define EV_KEY_DOWN				"Key Down"
#define EV_KEY_UP				"Key Up"
#define EV_COLLISION			"Collision"
#define EV_COLLISION_FINISH		"Collision Finish"
#define EV_TIMER				"Timer"
#define EV_MESSAGE				"Message"
#define EV_MOUSE_BUTTON_DOWN	"Mouse Button Down"
#define EV_MOUSE_BUTTON_UP		"Mouse Button Up"
#define EV_MOUSE_ENTER			"Mouse Enter"
#define EV_MOUSE_LEAVE			"Mouse Leave"
#define EV_MOVE_FINISH			"Move Finish"
#define EV_DESTROY_ACTOR		"Destroy Actor"
#define EV_OUT_OF_VISION		"Out of Vision"
#define EV_ACTIVATION_EVENT		"Activation Event"



#define CHANGE_ANIMATION	"Change Animation"
#define CHANGE_ANIMATION_DIRECTION	"Change Animation Direction"
#define CHANGE_PATH			"Change Path"
#define FOLLOW_MOUSE		"Follow Mouse"
#define CHANGE_COLOR		"Change Color"
#define CHANGE_TRANSPARENCY	"Change Transparency"
#define CHANGE_PARENT		"Change Parent"
#define PATH_POSITION		"Path Position"
#define ANIMATION_FRAME		"Animation Frame"
#define CURSOR_POINTER		"Change Cursor"
#define CREATE_TIMER		"Create Timer"
#define DESTROY_ACTOR		"Destroy Actor"
#define DESTROY_TIMER		"Destroy Timer"
#define CREATE_ACTOR		"Create Actor"
#define ACTIVATION_EVENT	"Activation Event"
#define POSITION			"Position"
#define SEND_MESSAGE		"Send Message"
#define COLLISION_STATE		"Collision State"
#define VISIBILITY_STATE	"Visibility State"
#define EXPRESSION_EDITOR	"Script Editor"
#define CONDITIONAL_ACTION	"Conditional Action"
#define SET_TEXT			"Set Text"
#define CHANGE_ZDEPTH		"Change Z Depth"
#define PLAY_SOUND			"Play Sound"
#define PLAY_MUSIC			"Play Music"
#define EVENT_ENABLE		"Event Enable"
#define EVENT_DISABLE		"Event Disable"
#define EVENT_CONTROL		"Event Control"
#define TO_ANTERIOR_POSITION "To Anterior Position"
#define MOVE_TO				"Move To"
#define PHYSICAL_BOUNCE		"Physical Response"


#define EXEC_CLONE_NAME			1
#define EXEC_ANY_ACTOR			2
#define EXEC_ALL_ACTORS			4
#define EXEC_ANIMATION			8
#define EXEC_ANY_ANIMATION		16
#define EXEC_ACTOR_NAME			32
#define EXEC_KEY				64
#define EXEC_ANY_KEY			128
#define EXEC_PATH				256
#define EXEC_ANY_PATH			512
#define EXEC_AT_LEAST_ONE_KEY_IS_PRESSED				1024
#define EXEC_ALL_KEYS_PRESSED			2048
#define EXEC_KEYS_PRESSED_IN_ORDER			4096
/*#define EXEC_				8192
#define EXEC_		16384
#define EXEC_		32768
#define EXEC_	65536
#define EXEC_	131072
#define EXEC_			262144*/
#define EXECUTE_ALL				0xFFFFFFFF

enum
{
	EVENT_ANIMATION = 0,
	EVENT_ANIMATION_FINISH,
	EVENT_PATH_FINISH,
	EVENT_MOUSEENTER,	
	EVENT_MOUSELEAVE,
	EVENT_MOUSEMOVE,
	EVENT_CREATE,
	EVENT_COLLISION,
	EVENT_TIMER,
	EVENT_KEYDOWN,
	EVENT_KEYUP,
	EVENT_MOUSEBUTTONDOWN,
	EVENT_MOUSEBUTTONUP,
	EVENT_MESSAGE,

	SET_ANIMATION,
	SET_ANIMATION_DIRECTION,
	SET_PATH,
	SET_COLLISION_STATE,
	SET_COLOR,
	SET_TRANSPARENCY,
	SET_PARENT,
	SET_PATH_POSITION,
	SET_ANIMATION_FRAME,
	SET_CURSOR,
	SET_CREATE_TIMER,
	SET_DESTROY_TIMER,
	SET_DESTROY_ACTOR,
	SET_CREATE_ACTOR,
	SET_PLAY_SOUND,
	SET_VISIBILITY_STATE,
	SET_POSITION,
	SET_MESSAGE,
	SET_FOLLOW_MOUSE,
	SET_EXPRESSION,
	SET_SETTEXT,
	SET_CONDITIONAL_ACTION,
	SET_ZDEPTH,
	SET_EVENT_CONTROL,
	SET_TO_ANTERIOR_POSITION,

	EVENT_DESTROY_ACTOR,
	EVENT_OUT_OF_VISION,

	SET_MOVE_TO,
	SET_ACTIVATION_EVENT,
	EVENT_ACTIVATION_EVENT,
	SET_PHYSICAL_BOUNCE,
	EVENT_COLLISION_FINISH,
	EVENT_MOVEFINISH,

	SET_PLAY_MUSIC,
	SET_EVENT_ENABLE,
	SET_EVENT_DISABLE
};

enum
{
	SET_DIRECTION = 0,
	FIXED_VALUE,
	ENVELOPE
};



enum
{
	BOTH_AXIS = 0,
	X_AXIS,
	Y_AXIS,
	NONE_AXIS,
	FORWARD,
	BACKWARD,
	STOPPED,
	CUSTOM,
	NO_CHANGE
};

enum
{
	EQUAL = 0,				
	GREATER_THAN,		
	SMALLER_THAN,		
	GREATER_OR_EQUAL_THAN,
	SMALLER_OR_EQUAL_THAN
};

enum
{
	IMMEDIATE_EXECUTION = 0,
	WAIT_FOR_FRAME_EXECUTION,
	NO_ACTION_MODE
};

enum
{
	HIGH_PRIORITY_MUSIC = 0,
	MEDIUM_PRIORITY_MUSIC,
	LOW_PRIORITY_MUSIC
};

enum
{
	MOVE_EVENT_ACTOR_ONLY = 0,
	MOVE_COLLIDE_ACTOR_ONLY,
	MOVE_BOTH_ACTORS,
	USE_CALCULATED_MASS,
	USE_SPECIFIED_MASS
};

enum
{
	STRAIGHT_LINE = 0,
	FIND_PATH
};

enum
{
	ANY_SIDE = 0,
	TOP_SIDE,
	BOTTOM_SIDE,
	LEFT_SIDE,
	RIGHT_SIDE,
	TOP_OR_BOTTOM_SIDE,
	LEFT_OR_RIGHT_SIDE
};

//Event extra flag (Must be 1, 2, 4, 8, 16, ...)
enum
{
	EV_FLAG_OPENED_GUI = 1,
	AC_FLAG_OPENED_GUI = 2
};


class stTimerEvent
{
public:
	char name[NAME_LIMIT];
	int miliseconds;
};

class GP2X_Double
{
public:
	GP2X_Double(double value)
	{
		//Convert to GP2X double format;
		char *buf = (char *)&value;
		p1 = (float)*(double *)(buf);
		p2 = (float)*(double *)(buf + 4);
	}

	double getValue() {return p2;}

private:
	float p1;
	float p2;
};

class stEvent //Dont use STRUCT_PACK
{
public:
	stEvent()
	{
		eventType = -1;
		data.mouse.x = data.mouse.y = 0;
	};

	stEvent& operator=(const stEvent& event)
	{
		if( &event != this )
		{
			//To avoid align problems, do not use memcpy
			eventType = event.eventType;
			
			switch(eventType)
			{
				//Set name
			case EVENT_ANIMATION_FINISH:
			case EVENT_PATH_FINISH:
			case EVENT_COLLISION_FINISH:
			case EVENT_TIMER:
			case EVENT_ACTIVATION_EVENT:
				strcpy(data.name, event.data.name);
				break;
				
			case EVENT_COLLISION:
				strcpy(data.name, event.data.name);
				data.name[NAME_LIMIT-1] = event.data.name[NAME_LIMIT-1]; //Collision repeat work around
				data.name[NAME_LIMIT-2] = event.data.name[NAME_LIMIT-2]; //Collision side work around
				break;
				
				//Set key
			case EVENT_KEYDOWN:
			case EVENT_KEYUP:
				{
					data.keyEvent.key = event.data.keyEvent.key;
					data.keyEvent.bDisableRepeat = event.data.keyEvent.bDisableRepeat;
					memcpy(data.keyEvent.keySequence, event.data.keyEvent.keySequence, MAX_KEY_SEQUENCE*sizeof(unsigned short));
				}
				break;
				
				//Set mouse
			case EVENT_MOUSEENTER:
			case EVENT_MOUSELEAVE:
			case EVENT_MOUSEMOVE:
			case EVENT_MOUSEBUTTONDOWN:
			case EVENT_MOUSEBUTTONUP:
				{
					data.mouse.x = event.data.mouse.x;
					data.mouse.y = event.data.mouse.y;
					data.mouse.button = event.data.mouse.button;
					data.mouse.bDrag = event.data.mouse.bDrag;
				}
				break;
				
				//Set message
			case EVENT_MESSAGE:
				{
					data.message.fromActor = event.data.message.fromActor;
					data.message.messageId = event.data.message.messageId;
					data.message.message = event.data.message.message;
				}
				break;
				
				//Simple events only set eventType
			}
		}
		
		return *this;
	}

	bool operator==(const stEvent& other) const
	{ 
		if( &other != this )
		{
			return memcmp(this, &other, sizeof(stEvent)) == 0; 
		}

		return true;
	}

	Sint32 eventType;

	union 
	{
		struct 
		{
			Sint32 x, y;
			Uint8 button;
			Uint8 bDrag;
		} mouse;

		char name[NAME_LIMIT];

		struct 
		{
			SDLKey key;
			Uint8 bDisableRepeat; //For compatibility with version < 5

			//Don't use SDLKey (4 bytes)
			unsigned short keySequence[MAX_KEY_SEQUENCE]; //Maximum MAX_KEY_SEQUENCE keys
		} keyEvent;

		struct 
		{
			Actor *fromActor;
			Uint32 messageId;
			Uint32 message;
		} message;
	} data;
};


class stAction
{
public:
	stAction()
	{
		pDescription = NULL; 
		next = NULL;
		Clear();		
	};

	~stAction()
	{
		delete pDescription;		
	};

#ifdef GP2X
	void ToGP2X()
	{
		switch(actionType)
		{
			case SET_PHYSICAL_BOUNCE:
				data.physicalbounce.massEventActor = GP2X_Double(data.physicalbounce.massEventActor).getValue();
				data.physicalbounce.massCollideActor = GP2X_Double(data.physicalbounce.massCollideActor).getValue();
				data.physicalbounce.eventVelocityMultiplier = GP2X_Double(data.physicalbounce.eventVelocityMultiplier).getValue();
				data.physicalbounce.collideVelocityMultiplier = GP2X_Double(data.physicalbounce.collideVelocityMultiplier).getValue();
				break;

			case SET_TRANSPARENCY:
				data.transparency.data.fixed = GP2X_Double(data.transparency.data.fixed).getValue();				
				break;

			case SET_MOVE_TO:
				data.moveto.x = GP2X_Double(data.moveto.x).getValue();
				data.moveto.y = GP2X_Double(data.moveto.y).getValue();
				data.moveto.velocity = GP2X_Double(data.moveto.velocity).getValue();
				break;
		}
	}
#endif

	stAction& operator=(const stAction& other)
	{
		if( &other != this )
		{
			gedString *tmp = pDescription; 
			memcpy(this, &other, sizeof(stAction));
			pDescription = tmp; 

#ifndef STAND_ALONE_GAME
			if(other.pDescription) setDescription(*other.pDescription); 
			else setDescription("");
#endif
		}
		
		return *this;
	}

	bool operator==(const stAction& other) const  
	{ 
		if( &other != this )
		{
			//Dont compare string description pointer
			return memcmp(this, &other, sizeof(stAction) - sizeof(gedString *)) == 0 && getDescription() == other.getDescription(); 
		}

		return true;
	}

	bool operator!=(const stAction& other) const  
	{ 
		if( &other != this )
		{			
			return !(*this == other); 
		}

		return false;
	}

	void Clear()
	{
		if(pDescription) 
		{
			delete pDescription;
			pDescription = NULL;
		}
		
		memset(this, 0, sizeof(stAction));
		executionType = NO_ACTION_MODE;
	}

	void Load(SDL_RWops *src, Uint16 actionSize);

#ifndef STAND_ALONE_GAME
	void Save(SDL_RWops *src);
#endif

	gedString getDescription() const
	{
		if(pDescription) 
		{
			return *pDescription;
		}

		return ""; 
	}

	void setDescription(gedString value)
	{
#ifndef STAND_ALONE_GAME
		if(!value.empty()) 
		{
			if(!pDescription)
			{
				pDescription = new gedString(value);
			}
			else
			{
				*pDescription = value;
			}
		}
		else if(pDescription)
		{
			pDescription->clear();
		}
#endif
	}

	Uint16 actionType;
	char actorName[NAME_LIMIT];
	Uint8 bAbsolutePosition;
	Uint8 bConditional;
	stEvent eventData;

	union /*STRUCT_PACK*/
	{
		char name[NAME_LIMIT];
		Uint8 bEnable;

		struct /*STRUCT_PACK*/
		{
			Uint8 bEnable;
			Uint32 events;
		} eventcontrol;

		struct /*STRUCT_PACK*/
		{
			Uint16 dataType;
			union /*STRUCT_PACK*/
			{
				Uint16 direction;
				struct /*STRUCT_PACK*/
				{
					Uint16 frame;	
				} fixed;

				struct /*STRUCT_PACK*/
				{
					Envelope *env;
				} envelope;
			} data;

		} pathPosition;

		struct /*STRUCT_PACK*/
		{
			char name[NAME_LIMIT];
			Uint16 axis;
		} path;

		struct /*STRUCT_PACK*/
		{
			Uint16 dataType;
			union /*STRUCT_PACK*/
			{
				struct /*STRUCT_PACK*/
				{
					Uint16 frame;	
				} fixed;

				struct /*STRUCT_PACK*/
				{
					Envelope *env;
				} envelope;
			} data;

		} animationFrame;

		struct /*STRUCT_PACK*/
		{
			Uint16 dataType;
			union /*STRUCT_PACK*/
			{
				struct /*STRUCT_PACK*/
				{
					Uint16 red, green, blue;	
				} fixed;

				struct /*STRUCT_PACK*/
				{
					Envelope *env;
				} envelope;
			} data;

		} color;

		struct /*STRUCT_PACK*/
		{
			Uint16 dataType;
			#ifdef linux
			char dummy[4]; //To correct align of transparency.data
			#endif
			union /*STRUCT_PACK*/
			{
				double fixed;

				struct /*STRUCT_PACK*/
				{
					Envelope *env;
				} envelope;
			} data;
		} transparency;
		
		class /*STRUCT_PACK*/
		{
		public:
			char imgName[PATH_LIMIT];
			Uint16 nFramesH;
			Uint16 nFramesV;
			Uint16 hotSpotX;
			Uint16 hotSpotY;			
		} cursor;

		struct /*STRUCT_PACK*/
		{
			char name[NAME_LIMIT];
			U32 miliseconds; //Solve the 65000 ms timer limitation
		} timer;

		struct /*STRUCT_PACK*/
		{
			char text[PATH_LIMIT];
			char fontPath[PATH_LIMIT];
			Uint16 iniASCIIChar, nChars;
		} settext;

		struct /*STRUCT_PACK*/
		{
			Uint8 bMusic;
			char path[PATH_LIMIT];			
			Uint16 volume;
			Uint16 loop;
			Uint8 priority;
			Sint16 pan;
		} sound;

		struct /*STRUCT_PACK*/
		{
			char expression1[NAME_LIMIT];
			char expression2[NAME_LIMIT];
			Uint16 condition;
		} conditionalaction;

		struct /*STRUCT_PACK*/
		{
			char name[NAME_LIMIT];
			Uint16 state;
		} animation;


		struct /*STRUCT_PACK*/
		{
			char name[NAME_LIMIT];
			char animationName[NAME_LIMIT];
			char parentName[NAME_LIMIT];
			char pathName[NAME_LIMIT];
		} createActor;

		struct /*STRUCT_PACK*/
		{
			Uint16 dataType;
			union /*STRUCT_PACK*/
			{
				struct /*STRUCT_PACK*/
				{
					int x, y;
				} fixed;

				struct /*STRUCT_PACK*/
				{
					Envelope *env;
				} envelope;
			} data;
		} position;

		struct /*STRUCT_PACK*/
		{
			//char toActorName[NAME_LIMIT];
			char message[NAME_LIMIT];
		} message;

		struct /*STRUCT_PACK*/
		{
			double x, y, velocity;
			char relative[NAME_LIMIT];
			char obstacle[NAME_LIMIT];
			int moveType; //STRAIGHT_LINE, FIND_PATH
			#ifdef linux
			char dummy[4]; //To correct size of moveto
			#endif
		} moveto;

		struct /*STRUCT_PACK*/
		{
			int moveType;
			int massType;
			double massEventActor;
			double massCollideActor;
			double collideVelocityMultiplier;
			double eventVelocityMultiplier;
		} physicalbounce;

		Uint32 zdepth;

		#ifdef linux
	    char dummy[520]; //To correct align stAction.next
	    #endif
	} data;


	stAction *next;

	//More...
	Sint32 x, y;

	//Immediate attributes
	Uint8 executionType;
	Uint8 bLastActionInQueueOnly; //Enable or disable multiple queued actions
#if __iPhone__
	char dummy1[4]; //AKR
#endif

	//Wait for frame
	char name[NAME_LIMIT]; //animation name, "Activation Event" from actor
	Uint8 nFrames;
	Uint16 frames[10]; //Maximum number of frame for no immediate actions

	//Event time stamp
	Uint32 timeStamp;

	//Extra event flags 
	Uint32 flags; 

	//Pointer to action description
	gedString *pDescription;

	//Add new attributes here
} STRUCT_PACK;



class stActorsInCollision
{
public:
	stActorsInCollision() {actor1 = actor2 = NULL;}
	stActorsInCollision(Actor *_actor1, Actor *_actor2) 
					{
						actor1 = _actor1;
						actor2 = _actor2;
					}

	void operator=( const stActorsInCollision& copy ) 
	{ 
		if( &copy != this )
		{
			actor1 = copy.actor1;
			actor2 = copy.actor2;
		}
	}

	bool operator==( const stActorsInCollision& copy ) 
	{ 
		if(actor1 == copy.actor1 && actor2 == copy.actor2)
			return true;

		return false;
	}

	Actor *actor1;
	Actor *actor2;
};

class stActorsInCollision2
{
public:
	stActorsInCollision2() {actor1 = actor2 = NULL;}
	stActorsInCollision2(Actor *_actor1, Actor *_actor2) 
	{
		actor1 = _actor1;
		actor2 = _actor2;
	}
	
	void operator=( const stActorsInCollision2& copy ) 
	{ 
		if( &copy != this )
		{
			actor1 = copy.actor1;
			actor2 = copy.actor2;
		}
	}

	bool operator==( const stActorsInCollision2& copy ) 
	{ 
		if( (actor1 == copy.actor1 && actor2 == copy.actor2) ||
			(actor1 == copy.actor2 && actor2 == copy.actor1) )
			return true;

		return false;
	}

	Actor *actor1;
	Actor *actor2;
};

class stActorsInCollisionHash : public GlHash
{
  public:
	stActorsInCollisionHash( const stActorsInCollision& col )
	{	
		val = (U32)col.actor1 + (U32)col.actor2;
	}

	stActorsInCollisionHash( const stActorsInCollision2& col )
	{	
		val = (U32)col.actor1 + (U32)col.actor2;
	}
};

class stTimerDefinition
{
public:
	stTimerDefinition() {interval = minRandomInterval = 0; count = 0; type = 0;}
	stTimerDefinition(Uint32 _interval,
					Uint32 _minRandomInterval,
					int _count,
					char _type) 
					{
						interval = _interval; 
						minRandomInterval = _minRandomInterval; 
						count = _count;
						type = _type;
					}

	void operator=( const stTimerDefinition& copy ) 
	{ 
		if( &copy != this )
		{
			interval = copy.interval; 
			minRandomInterval = copy.minRandomInterval; 
			count = copy.count;
			type = copy.type;
		}
	}

	Uint32 interval;
	Uint32 minRandomInterval;
	int count;
	char type;
};


typedef GlMap< gedString, stTimerDefinition, GlStringHash >		MapTimerNameDefinition;
typedef GlMapIterator< gedString, stTimerDefinition, GlStringHash >		MapTimerNameDefinitionIterator;


typedef GlMap< stActorsInCollision, char, stActorsInCollisionHash >		MapActorsInCollision;
typedef GlMapIterator< stActorsInCollision, char, stActorsInCollisionHash >		MapActorsInCollisionIterator;

typedef GlMap< stActorsInCollision2, char, stActorsInCollisionHash >		MapActorsInCollision2;
typedef GlMapIterator< stActorsInCollision2, char, stActorsInCollisionHash >		MapActorsInCollisionIterator2;


typedef GlSList<stAction *> ListActions;  
typedef GlSListIterator<stAction *> ListActionsIterator;


typedef GlMap< SDLKey, ListActions, GlNumberHash<SDLKey> >		MapKeys;
typedef GlMapIterator< SDLKey, ListActions, GlNumberHash<SDLKey> >		MapKeysIterator;

typedef GlMap< Uint8, ListActions, GlNumberHash<Uint8> >		MapMouseButtons;
typedef GlMapIterator< Uint8, ListActions, GlNumberHash<Uint8> >		MapMouseButtonsIterator;

typedef GlMap< gedString, ListActions, GlStringHash >		MapStringAction;
typedef GlMapIterator< gedString, ListActions, GlStringHash >		MapStringActionIterator;

typedef GlDynArray<Actor *> ListActor;  



/*class stCollisionAction
{
public:
	stCollisionAction() {listActors = NULL;}
	ListActions listActions;

	//Optimization for Action::CheckCollisions
	ListActor *listActors;
};


typedef GlMap< gedString, stCollisionAction, GlStringHash >		MapCollision;
typedef GlMapIterator< gedString, stCollisionAction, GlStringHash >		MapCollisionIterator;*/

typedef GlMap< SDL_TimerID, stTimerEvent, GlPointerHash<SDL_TimerID> >		MapTimerIdName;
typedef GlMapIterator< SDL_TimerID, stTimerEvent, GlPointerHash<SDL_TimerID> >		MapTimerIdNameIterator;

typedef GlMap< gedString, SDL_TimerID, GlStringHash >		MapTimerNameId;
typedef GlMapIterator< gedString, SDL_TimerID, GlStringHash >		MapTimerNameIdIterator;

typedef GlMap< gedString, Action *, GlStringHash >		GlobalMapActions;
typedef GlMapIterator< gedString, Action *, GlStringHash >		GlobalMapActionsIterator;


/*typedef GlSList<Actor *> StackActors;  //push = PushFront, pop = PopFront, top = Front
typedef GlSList<Action *> StackActions; //push = PushFront, pop = PopFront, top = Front*/




class Script;




class Action  
{
public:

	void setOutOfVisionOptimization(bool value) {bOutOfVisionOptimization = value;}
	bool getOutOfVisionOptimization() {return bOutOfVisionOptimization;}

	bool setInheritedEvents(gedString actorName);
	static bool CanSetInheritedEvents(const gedString& _actionName, gedString parentName);

	static void setNetActorCreated(bool value) {bNetActorCreated = value;}

	Action *getInheritedEvents();
	gedString getInheritedEventsActorName() {return InheritedEvents;}
	void SetInheritedKeyEvents();

	stAction *getActions() {return listActions;};
	
#ifndef STAND_ALONE_GAME
	static bool GiveInheritedEvents(gedString parentActorName);
	static void RemoveInheritedEvents(gedString parentActorName);

	static void CleanUpUnusedTimers(MapTimerNameDefinition &timerDefinition);
	static gedString MapScriptFunctionToAction(const gedString& scriptFunction);
	static void DoAction(Actor *actor, const gedString& _actionName, bool bOnCollision = false);
	static void DoEvent(Actor *actor, const gedString& eventName, bool _bActivationEvent);
	static void Populate(ListPop *list);
	void PopulateConditionalActions(ListPop *listActions, stAction *condAction);
	void PopulateEventActions(ListPop *listActions, const gedString& eventName);
	static void PopulateEvents(ListPop *list, Actor *actor = NULL);
	static void PopulateEvents(ListString &list, Actor *actor = NULL);
	static void PopulateScriptActions(ListPop *list);
	static void PopulateScriptActions(ListData &list);
	static void SaveActions(SDL_RWops *src, Uint32 *indexAction);
	void Save(SDL_RWops *src);
	void SaveSound(SDL_RWops *exeFile, MapAudioFileIndex &audioIndex);
	static void SaveSoundTo(SDL_RWops *exeFile, MapAudioFileIndex &audioIndex);	
	static void ShowActionDialog(stAction *action, Actor *eventActor);
	static void ShowEventDialog(stAction *action, Actor *eventActor);
	

	//Set atual event
	static void EventAnimation();
	static void EventAnimationFinish(const gedString& animationName);
	static void EventPathFinish(const gedString& pathName);
	static void EventMouseEnter(int x, int y);	
	static void EventMouseLeave(int x, int y);
	static void EventMouseMove(int x, int y, Uint8 button);
	static void EventMoveFinish();
	static void EventCreate();
	static void EventDestroyActor();
	static void EventOutOfVision();

	static void EventCollision(const gedString& collideActorName, bool bRepeatCollision, int side);
	static void EventCollisionFinish(const gedString& collideActorName);
	static void EventActivationEvent(const gedString& fromActorCloneName);
	static void EventTimer(const gedString& timerName);
	static void EventKeyDown(SDLKey key, bool bDisableRepeat, unsigned short *keySequence);
	static void EventKeyUp(SDLKey key);
	static void EventMouseButtonDown(int x, int y, Uint8 button, bool bDrag);
	static void EventMouseButtonUp(int x, int y, Uint8 button);
	static void EventMessage(Actor *fromActor, Uint32 messageId, Uint32 message);

	//Event create
	stAction * SetAnimation(const gedString& actionActor, const gedString& name, int state);
	stAction * SetAnimationDirection(const gedString& actionActor, int state);
	stAction * SetTransparency(const gedString& actionActor, double trans);
	stAction * SetCollisionEnable(const gedString& actionActor, bool bEnable);
	stAction * SetVisibilityEnable(const gedString& actionActor, int state);
	stAction * SetParent(const gedString& actionActor, const gedString& name);
	stAction * SetMoveTo(const gedString& actionActor, double x, double y, double velocity, char *relativeActor, char *obstacleActor, int moveType);
	
	stAction * SetPath(const gedString& actionActor, const gedString& name, int axis);
	stAction * SetDirectionPath(const gedString& actionActor, int direction);
	stAction * SetFixedPathPosition(const gedString& actionActor, int frame);
	stAction * SetEnvelopePathPosition(const gedString& actionActor, Envelope *env);

	stAction * SetFixedAnimationFrame(const gedString& actionActor, int frame);
	stAction * SetEnvelopeAnimationFrame(const gedString& actionActor, Envelope *env);

	stAction * SetFixedColor(const gedString& actionActor, int red, int green, int blue);
	stAction * SetEnvelopeColor(const gedString& actionActor, Envelope *env);

	stAction * SetCursor(const gedString& actionActor, const gedString& imgName, int nFramesH, int nFramesV, int hotSpotX, int hotSpotY);
	stAction * SetCreateActor(const gedString& actorName, const gedString& animationName, Actor *parent, Path *path, int xini, int yini, bool bAbsolutePosition);
	stAction * SetDestroyActor(const gedString& actorName);
	stAction * SetCreateTimer(const gedString& actionActor, const gedString& timerName, int miliseconds);
	stAction * SetDestroyTimer(const gedString& actionActor, const gedString& timerName);
	stAction * SetFollowMouse(const gedString& actionActor, int axis);
	stAction * SetPhysicalBounce(int moveType, int massType, double massEventActor, double massCollideActor, double eventVelocityMultiplier, double collideVelocityMultiplier);
	stAction * SetActivationEvent(const gedString& actorFromCloneName, const gedString& actorToCloneName);

	stAction * SetExpression(Script *pScript);
	stAction *SetConditionalAction(Script *pScript);
	stAction * SetSetText(const gedString& actionActor, const gedString& text, const gedString& fontPath,  int iniASCIIChar, int nChars);
	stAction * SetSetZDepth(const gedString& actionActor, double depth);
	stAction * SetSound(bool bMusic, const gedString& path, int volume, int loop, int priority, Sint16 pan);
	stAction * SetEventControl(const gedString& actionActor, Uint32 events, bool bEnable);
	stAction * SetToAnteriorPosition(const gedString& actionActor);

	static stEvent& getActualEvent() {return actualEvent;}
	static void setActualEvent(stEvent &event) {actualEvent = event;}

	void UpdateEvent();
	gedString GetDescriptions(Uint32 events);
#endif //#ifndef STAND_ALONE_GAME

	static void SetExpressionPointers();

	gedString VerifyScripts(bool bVerifyScriptsAddedWithError = false);
	static void ClearActorsInCollision();
	static gedString GetDescription(stAction *action, bool bActionDescriptionBefore, bool bEventNameOnly = false, bool bActionDescriptionAfter = false);
	
	static bool inNewActivationEvent() {return bActivationEvent;}
	static GlobalMapActions &getGlobalMapActions();
	static GlobalMapActions &getActionsKeyDown();
	static GlobalMapActions &getActionsKeyUp();
	void RestoreExecStaticData();
	void StoreExecStaticData();
	void FixAnimationFinish(Actor *actor);
	void CleanUpTimers(Actor *actor, SDL_TimerID timerId);
	void DetachAction();
	void AttachAction();
	
	
	static gedString GetActionName(stAction *action);
	static void SetConditionalAction(bool bConditional) {bConditionalAction = bConditional;};
	//Script *GetExpression();
	void CheckCollisions(Actor *actor);
	
	static void RemoveAction(Action *actionRemove);
	bool RemoveAction(stAction *actionRemove);
	void DeleteConditionalAction(stAction *actionRemove);
	
	void Load(SDL_RWops *src);
	static Action *Load(const gedString& _actionName);
	
	static void LoadActions(SDL_RWops *src);
	
	
	void ToEvent(stAction *action, ListActions *pUpdateList = NULL);
	void Execute(U16 actionNumber, Actor *eventActor, Actor *collideActor, bool bExecuteConditional, bool bForceExecute, bool bFromRemoteMachine, unsigned int remoteSeed);
	void Execute(stAction *action,  Actor *eventActor, Actor *collideActor = NULL, bool bExecuteConditional = false, bool bForceExecute = false, bool bFromRemoteMachine = false, unsigned int remoteSeed = 0);
	static Action *Call(Actor *actor, bool bCreate = true);
	static Action *Call(const gedString& name);

	void OnCollision(Actor *eventActor, Actor *collideActor);
	void OnCollisionFinish(Actor *eventActor, Actor *collideActor);
	void OnActivationEvent(Actor *eventActor, Actor *fromActor);
	Uint32 OnTimer(Actor *eventActor, SDL_TimerID timerID);
	void OnKeyDown(Actor *eventActor, SDLKey key, int repeatLoopCount = 0);
	void OnKeyUp(Actor *eventActor, SDLKey key);
	bool OnMouseButtonDown(Actor *eventActor, int x, int y, Uint8 button);
	void OnMouseButtonUp(Actor *eventActor, int x, int y, Uint8 button);
	void OnMessage(Actor *eventActor, Actor *fromActor, Uint32 messageId, Uint32 message);	

		

	void OnAnimation(Actor *eventActor);
	void OnAnimationFinish(Actor *eventActor);
	void OnPathFinish(Actor *eventActor);
	void OnMouseEnter(Actor *eventActor, int x, int y);	
	void OnMouseLeave(Actor *eventActor, int x, int y);
	void OnMouseMove(Actor *eventActor, int x, int y);
	void OnCreate(Actor *eventActor);
	void OnDestroyActor(Actor *eventActor);
	void OnOutOfVision(Actor *eventActor);
	void OnMoveFinish(Actor *eventActor);


	
	Action(const gedString& name);
	Action();
	virtual ~Action();
	stAction *newAction(gedString actionActor, int actionType, ListActions *pUpdateList = NULL);

	


	const gedString& getActionName() const {return actionName;}
	stAction *getLasAction() {return actionAnt;}

	bool HaveKeyDown(bool bGetInheritedEvents);
	bool HaveKeyUp(bool bGetInheritedEvents);
	bool HaveCollisions(bool bGetInheritedEvents);
	bool HaveCollisionFinish(bool bGetInheritedEvents);
	bool HaveActivationEvent(bool bGetInheritedEvents);
	bool HaveTimer(bool bGetInheritedEvents);
	bool HaveAnimation(bool bGetInheritedEvents);
	bool HaveAnimationFinish(bool bGetInheritedEvents);
	bool HavePathFinish(bool bGetInheritedEvents);
	bool HaveCreate(bool bGetInheritedEvents);
	bool HaveDestroyActor(bool bGetInheritedEvents);
	bool HaveMouseEnter(bool bGetInheritedEvents);
	bool HaveMouseLeave(bool bGetInheritedEvents);
	bool HaveMoveFinish(bool bGetInheritedEvents);
	bool HaveOutOfVision(bool bGetInheritedEvents);
	bool HaveMouseButtonDown(bool bGetInheritedEvents);
	bool HaveMouseUpActions(bool bGetInheritedEvents);

#ifndef STAND_ALONE_GAME	
	Uint32 GetEventBits(bool bGetInheritedEvents);
#endif

	static stAction *getEditAction() {return editAction;}
	static void setEditAction(stAction *edit) 
	{
		editAction = edit;
	}

	static void clearEditAction() 
	{
		editAction = NULL;
	}

	static Actor *getActualEventActor() {return actualEventActor;}
	static Actor *getActualCollideActor() {return actualCollideActor;}
	//static Action *getActualAction() {return actualAction;};	

	static bool getInCreateActorEvent() {return bInCreateActorEvent;} //To avoid create actors loops
	static bool getInCreateActorAction() {return bInCreateActorAction;}
	static void setInCreateActorAction(bool value) {bInCreateActorAction = value;}

	
	MapTimerIdName	mapTimerIdName;
	MapTimerNameId	mapTimerNameId;

	//For no duplicated collision events
	static MapActorsInCollision mapActorsInCollision;

private:

	int nRef;

#ifndef STAND_ALONE_GAME
	void CleanUpUnusedTimers(MapString& usedTimers);

	void PopulateKeyActions(MapKeys& map, ListPop *listActions);
	void PopulateStringActions(MapStringAction& map, ListPop *listActions);
	void PopulateListActions(ListActions& list, ListPop *listActions);
	void PopulateMouseActions(MapMouseButtons& map, ListPop *listActions);

	void PopulateKeyActions(MapKeys& map, ListString &list);
	void PopulateStringActions(MapStringAction& map, ListString &list);
	void PopulateListActions(ListActions& list, ListString &listString);
	void PopulateMouseActions(MapMouseButtons& map, ListString &list);

	ListActions *FindAction(stAction *actionFind);

#endif //#ifndef STAND_ALONE_GAME

	void CheckCollisionsBetween(Actor *actor, Actor *actorTest);
	void CheckCollisionList(Actor *actor);

	void ExecuteTimerActions(Actor *eventActor, char *timerName, Uint32 timeStamp);
	void ExecuteActivationEventActions(Actor *eventActor, char *cloneName, Uint32 timeStamp, Uint32 execFlags);
	void ExecuteAnimationActions(Actor *eventActor, Uint32 timeStamp);
	void ExecuteAnimationFinishActions(Actor *eventActor, Uint32 timeStamp, Uint32 execFlags);
	void ExecuteCollisionActions(Actor *eventActor, Actor *collideActor, bool bActorsInCollision, double angle, Uint32 timeStamp, Uint32 execFlags);
	void ExecuteCollisionFinishActions(Actor *eventActor, Actor *collideActor, Uint32 timeStamp, Uint32 execFlags);
	void ExecuteCreateActions(Actor *eventActor, Uint32 timeStamp);
	void ExecuteDestroyActorActions(Actor *eventActor, Uint32 timeStamp);
	void ExecuteKeyUpActions(Actor *eventActor, SDLKey key, Uint32 timeStamp, Uint32 execFlags);
	bool ExecuteMouseButtonDownActions(Actor *eventActor, Uint8 button, Uint32 timeStamp);
	void ExecuteMouseButtonUpActions(Actor *eventActor, Uint8 button, Uint32 timeStamp);
	void ExecuteMouseEnterActions(Actor *eventActor, Uint32 timeStamp);
	void ExecuteMouseLeaveActions(Actor *eventActor, Uint32 timeStamp);
	void ExecuteMoveFinisActions(Actor *eventActor, Uint32 timeStamp);
	void ExecuteOutOfVisionActions(Actor *eventActor, Uint32 timeStamp);
	void ExecutePathFinishActions(Actor *eventActor, Path *path, Uint32 timeStamp, Uint32 execFlags);
	void ExecuteKeyDownActions(Actor *eventActor, SDLKey key, int repeatLoopCount, Uint32 timeStamp, Uint32 execFlags);

	bool HaveCollisionWithAnyActor();

	int GetActionIndex(stAction *action);
	stAction *GetActionByIndex(int n);

	gedString actionName;
	Uint16 nActions;
	stAction *listActions;
	
	bool bOutOfVisionOptimization; //Enable or disable working actors optimization

	//Dont save
	stAction *actionAnt;
	static stEvent actualEvent;
	static bool bConditionalAction;
	static bool bActivationEvent;
	static bool bInCreateActorEvent;
	static bool bInCreateActorAction;
	static bool bNetActorCreated;
	static bool bUserInput;
	static stAction *editAction;

	MapKeys mapKeyDown, mapKeyUp;
	MapStringAction mapCollision;
	MapStringAction mapCollisionFinish;
	MapString mapCollisionOrCollisionFinish;
	MapStringAction mapActivationEvent;
	MapStringAction mapTimerNameAction;	
	MapStringAction	mapAnimationFinishActions;
	MapStringAction	mapPathFinishActions;
	ListActions		listAnimationActions;	
	ListActions		listCreateActions;
	ListActions		listDestroyActorActions;
	ListActions		listOutOfVisionActions;
	ListActions		listMouseEnterActions;
	ListActions		listMouseLeaveActions;
	ListActions		listMoveFinishActions;
	MapMouseButtons		mapMouseButtonDownActions;
	MapMouseButtons		mapMouseUpActions;
	
	//For speedup collisions
	bool bHasCollisionWithAnyActor;

	//Inherited Events
	gedString InheritedEvents;
	
	

	//For script execution
	static Actor *actualEventActor, *actualCollideActor;
	//static Action *actualAction;
	/*static StackActors stackActors;
	static StackActions stackActions;*/

	
};

class ActionInfo
{
public:

	ActionInfo() {}

	ActionInfo(const RakNetTime &_timeStampOfRequest, U16 _actionNumber, const gedString& _actionName, 
			const gedString& _eventActorName, long _eventActorCloneIndex, const RakNetTime &_eventActorOwnershipTimestamp,
			const gedString& _collideActorName, long _collideActorCloneIndex,
			bool _bExecuteConditional)
	{
		timeStampOfRequest = _timeStampOfRequest;
		actionNumber = _actionNumber;				
		eventActorCloneIndex = _eventActorCloneIndex;
		eventActorOwnershipTimestamp = _eventActorOwnershipTimestamp;		
		collideActorCloneIndex = _collideActorCloneIndex;
		bExecuteConditional = _bExecuteConditional;


		//To avoid allocations and sppedup
		if(!actionName) actionName = new gedString(_actionName);
		else *actionName = _actionName;

		if(!collideActorName) collideActorName = new gedString(_collideActorName);
		else *collideActorName = _collideActorName;

		if(!eventActorName) eventActorName = new gedString(_eventActorName);
		else *eventActorName = _eventActorName;
	}

	virtual ~ActionInfo() {}

	void *operator new(size_t size)
	{
		ActionInfo *m = NULL;

		if(freeList.Count())
		{
			m = (ActionInfo *)freeList.Back();
			freeList.PopBack();
			return m;
		}
		
		//No itens available in the free list, so allocate new
		m = (ActionInfo *)dlmalloc(size);
		m->actionName = NULL;
		m->collideActorName = NULL;
		m->eventActorName = NULL;

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
			ActionInfo *p = (ActionInfo *)freeList[i];
			delete p->actionName;
			delete p->collideActorName;
			delete p->eventActorName;
			delete p;	
		}

		freeList.Clear();
	}

	RakNetTime timeStampOfRequest;
	U16 actionNumber;	
	long eventActorCloneIndex;
	RakNetTime eventActorOwnershipTimestamp;	
	long collideActorCloneIndex;
	bool bExecuteConditional;

	gedString *actionName;
	gedString *collideActorName;	
	gedString *eventActorName;

private:
	//To avoid allocations and sppedup
	static GlDynArray<void *> freeList;
};

typedef GlSList<ActionInfo *> QueueAction;  //push = PushBack, pop = PopFront

int execChangeAnimation(char *actorName, char *animationName, int state);
int execChangeAnimationDirection(char *actorName, int state);
int execChangeTransparency(char *actorName, double transp);
int execCollisionState(char *actorName, int state);
int execVisibilityState(char *actorName, int state);
int execChangePath(char *actorName, char *pathName, int axis);
int execFollowMouse(char *actorName, int axis);
int execChangeParent(char *actorName, char *parentName);
int execChangeCursor(char *actorName, char *imgName, int nFramesH, int nFramesV, int hotSpotX, int hotSpotY);
Actor *execCreateActor(char *creatorName, char *animationName, char *parentName, char *pathName, int xpos, int ypos, int absolutePosition);
int execActivationEvent(char *cloneName);
int execDestroyActor(char *actorName);
int execCreateTimer(char *actorName, char *timerName, int milliseconds);
int execDestroyTimer(char *timerName);
int execChangeZDepth(char *actorName, double zdepth);
int execPlaySound(char *soundPath, double volume, int loop, Sint16 pan);
int execPlayMusic(char *soundPath, double volume, int loop, int priority);
int execToAnteriorPosition(char *actorName);
int execEventEnable(char *actorName, unsigned long event);
int execEventDisable(char *actorName, unsigned long event);
int execExitGame();
int execLoadGame(char *gamePath);
int execActorCount(char *actorName);
int execCollisionFree(char *actorName, int x, int y);
int execMoveTo(char *actorName, double x, double y, double velocity, char *relativeActor, char *obstacleActor, int moveType);
int execPhysicalBounce(int moveType, int massType, double massEventActor, double massCollideActor, double eventVelocityMultiplier, double collideVelocityMultiplier);

void exec_setpen(int r, int g, int b, double transp, int pensize);
void exec_moveto(int x, int y);
void exec_lineto(int x, int y);
void exec_screen_to_actor(int *x, int *y);
void exec_actor_to_screen(int *x, int *y);
void exec_erase(int r, int g, int b, double transp);
void exec_putpixel(int x, int y);
void exec_savecanvas();
void exec_restorecanvas();


#endif // !defined(AFX_ACTION_H__9381DB4D_3672_4C48_A5B2_3A3405E175A9__INCLUDED_)
