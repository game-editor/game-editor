// Actor.cpp: implementation of the Actor class.
//
//////////////////////////////////////////////////////////////////////

#include "Actor.h"
#include "gui.h"
#include "GameControl.h"
#include "Path.h"
#include "Action.h"
#include "ActorEdit.h"
#include "Tile.h"
#include <math.h>
#include "Sequence.h"
#include "Script.h"
#include "RegionLoad.h"
#include "dlmalloc.h"
#include "PathFinder/GeoPathfinder.h"

#include "../RakNet/Source/BitStream.h"
#include "../RakNet/Source/StringTable.h"
#include "../RakNet/Source/GetTime.h"
#include "../RakNet/Source/NetworkIDManager.h"

extern NetworkIDManager networkIDManager;


typedef GlDynArray<gedString> ListString; 

#define TEXT_NUMBER_INIT			0.0000000001

#ifndef STAND_ALONE_GAME
void UpdateActorAnim(Actor *_actor);
#endif


#ifndef APPLICATION_THREAD_TIMERS
SDL_mutex *timerMutEx = SDL_CreateMutex();
#endif

gedString VIEW_NAME("view");

extern double rand(double t);
extern double distance(double x1, double y1, double x2, double y2);


static MapTimer globalMapTimer;
MapActorAnimation mapActorAnimation;
static stTimer *lastCreatedTimer = NULL;

#ifndef STAND_ALONE_GAME
#include "../wxGameEditor/GameGraph.h"

void UpdateActorSize();
#endif

stTimer::stTimer()
{
	globalMapTimer.Add(this, 1);

	actor = NULL;
	timerID = 0;
	interval = minRandomInterval = 0;
	count = REPEATE_FOREVER;
	type = PERIODIC_TIMER;
}

stTimer::~stTimer()
{
	globalMapTimer.Remove(this);
}

static Uint32 ActorTimerCallback(Uint32 interval, void *param)
{
#ifdef APPLICATION_THREAD_TIMERS

	
	stTimer *timer = (stTimer *)param;
	if(!timer || globalMapTimer[timer] == NULL)
	{
		return 0;
	}

#ifdef DEBUG
	
		/*static U32 lastCheck = 0;
		U32 check = SDL_GetTicks();
		U32 realInterval = check - lastCheck;
		if(realInterval > timer->interval) 
			GLOUTPUT("*** Timer Check interval: %ld ms (interval: %ld)\n", realInterval, interval);

		lastCheck = check;*/
	
#endif

	int realFps = GameControl::Get()->getRealFrameRate();	
	if(realFps <= 0) realFps = 1;

	int fpsInterval = (int)(1000.0/realFps);


	//Adjust time interval
	if(timer->type == RANDOM_TIMER)
	{
		interval = (int)rand(timer->interval - timer->minRandomInterval) + timer->minRandomInterval;		
	}
	else
		interval = timer->interval;
	
	if(timer->actor->isWorkingActor())
	{
		//Execute timers only in working actors
		
		//SDL_PeepEvents( &event, 1, SDL_ADDEVENT, 0 );
		
		//Execute OnTimer before count test
		//Solve the createTimerTest.ged bug (only shoot two times)
		bool bRemove = false;
		SDL_TimerID oldTimerID = timer->timerID;
		if(timer && !timer->actor->OnTimer(timer->timerID))
		{
			bRemove = true;
		}
		
		if(
			globalMapTimer[timer] //Timer can be destroyed in OnTimer, so, make sure if still valid
			&& timer->timerID == oldTimerID //If the timer is destroyed in the OnTimer event and a new timer is created, the new create timer will have the same address of the old timer.
											//To make sure is the same timer, test the timerID too. (Solve the bug in 'GE Rally.ged' - Doesn't hide the semaphore)
		  ) 
		{
			//Adjust timer count
			if(timer && timer->count != REPEATE_FOREVER)
			{
				//Count timer
				if(timer->count <= 1)
				{
					if(timer->actor->getAction()) timer->actor->getAction()->CleanUpTimers(timer->actor, timer->timerID);
					SDL_RemoveTimer(timer->timerID); //Solve bug when create other timer in OnTimer
					timer->actor->RemoveTimer(timer);

					timer = NULL;

					interval = 0; //Destroy timer
				}
				else
					timer->count--;
			}

			if(bRemove && timer)
			{
				//Remove timer
				if(timer->actor->getAction()) timer->actor->getAction()->CleanUpTimers(timer->actor, timer->timerID);
				SDL_RemoveTimer(timer->timerID); //Solve bug when create other timer in OnTimer
				timer->actor->RemoveTimer(timer);

				return 0;
			}
		}
	}


	//Avoid timer event leak
	
	if(interval)
	{

		//Try solve the Timer Precision.ged
		/*if(timer->type != RANDOM_TIMER) 
		{
			if(realInterval > timer->interval)
			{
				int dt = realInterval - timer->interval;
				if(dt < interval)
				{
					interval -= dt;
				}
				else
				{
					interval = 0;
				}
			}
		}*/

		if(interval < fpsInterval)
		{
			interval = fpsInterval;
			
#ifdef DEBUG
			//GLOUTPUT("*** Low Timer: %ld ms\n", interval);
#endif
		}
	}



	return interval;

#else
	MuteEx mutex(timerMutEx);

#ifdef MULTI_THREAD_ACTOR_TIMER_EVENT
	//Multithread
	stTimer *timer = (stTimer *)param;
	
	if(globalMapTimer[timer] == NULL)
	{
		return 0;
	}

	Uint32 newInterval = timer->actor->OnTimer(timer->timerID);
	
	if(newInterval == 0)
	{
		//Remove timer
		timer->actor->RemoveTimer(timer);		
	}

	return newInterval;
#else
	//Avoid multithread
	stTimer *timer = (stTimer *)param;
	if(globalMapTimer[timer] == NULL)
	{
		return 0;
	}

	int realFps = GameControl::Get()->getRealFrameRate();	
	if(realFps <= 0) realFps = 1;

	int fpsInterval = (int)(1000.0/realFps);

	SDL_Event event;
	event.type = SDL_ACTOR_TIMER__EVENT;
	event.user.data1 = param;
	event.user.data2 = timer->actor;

	//Adjust time interval
	if(timer->type == RANDOM_TIMER)
	{
		interval = (int)rand(timer->interval - timer->minRandomInterval) + timer->minRandomInterval;		
	}
	else
		interval = timer->interval;
	
	if(timer->actor->isWorkingActor())
	{
		//Execute timers only in working actors

		//Solve the createTimerTest.ged bug (only shoot two times)
		SDL_PeepEvents( &event, 1, SDL_ADDEVENT, 0 );
		
		//Adjust timer count
		if(timer->count != REPEATE_FOREVER)
		{
			//Count timer
			if(timer->count <= 1)
			{
				if(timer->actor->getAction()) timer->actor->getAction()->CleanUpTimers(timer->actor, timer->timerID);
				timer->actor->RemoveTimer(timer);				
				interval = 0; //Destroy timer
			}
			else
				timer->count--;
		}		
		
	}

	//Avoid timer event leak
	if(interval && interval < fpsInterval)
	{
		interval = fpsInterval;
	}

	return interval;	
#endif	

#endif //APPLICATION_THREAD_TIMERS
}

MapActorSequence Actor::sequence;
bool Actor::bAnyPositionChanged = false;
int Actor::loadingActors = 0;

//#define _TESTSTRUCTALIGN_ 

#if defined(_TESTSTRUCTALIGN_) && defined(_DEBUG)
void testStructSize()
{
	static bool bInit = false;
	if(bInit) return;

	bInit = true;
	//////////////////////////////////////////////////
	//Size of saved structs test

	stEvent oi1;
	stAction oi2;

	oi1.eventType = 0;

	int s1 = sizeof(oi1);
	int s2 = sizeof(oi1.eventType);
	int s3 = sizeof(oi1.data);
	int s4 = sizeof(oi1.data.keyEvent);
	int s5 = sizeof(oi1.data.message);
	int s6 = sizeof(oi1.data.mouse);
	int s7 = sizeof(oi1.data.name);
	int s8 = sizeof(oi1.data.message.fromActor);

	int s[100], i=0;
	memset(s, 0, 100*sizeof(int));
	s[i++] = sizeof(oi2);

	 s[i++] = sizeof(oi2.actionType);
	 s[i++] = sizeof(oi2.actorName);
	 s[i++] = sizeof(oi2.bConditional);
	 s[i++] = sizeof(oi2.bAbsolutePosition);
	 s[i++] = sizeof(oi2.bLastActionInQueueOnly);
	 s[i++] = sizeof(oi2.eventData);
	 s[i++] = sizeof(oi2.executionType);
	 s[i++] = sizeof(oi2.frames);
	 s[i++] = sizeof(oi2.name);
	 s[i++] = sizeof(oi2.next);
	 s[i++] = sizeof(oi2.nFrames);
	 s[i++] = sizeof(oi2.timeStamp);
	 s[i++] = sizeof(oi2.x);
	 s[i++] = sizeof(oi2.y);

	 s[i++] = sizeof(oi2.data); //16

	 s[i++] = sizeof(oi2.data.bEnable);
	 s[i++] = sizeof(oi2.data.name);
	 s[i++] = sizeof(oi2.data.zdepth);



	 s[i++] = sizeof(oi2.data.animation); //20
	 s[i++] = sizeof(oi2.data.animation.name);
	 s[i++] = sizeof(oi2.data.animation.state);

	 s[i++] = sizeof(oi2.data.animationFrame); //23
	 s[i++] = sizeof(oi2.data.animationFrame.dataType);
	 s[i++] = sizeof(oi2.data.animationFrame.data);
	 s[i++] = sizeof(oi2.data.animationFrame.data.envelope);
	 s[i++] = sizeof(oi2.data.animationFrame.data.fixed);
	 
	 s[i++] = sizeof(oi2.data.color); //28
	 s[i++] = sizeof(oi2.data.color.data);
	 s[i++] = sizeof(oi2.data.color.dataType);
	 s[i++] = sizeof(oi2.data.color.data.envelope);
	 s[i++] = sizeof(oi2.data.color.data.fixed);

	 s[i++] = sizeof(oi2.data.conditionalaction); //33
	 s[i++] = sizeof(oi2.data.conditionalaction.condition);
	 s[i++] = sizeof(oi2.data.conditionalaction.expression1);
	 s[i++] = sizeof(oi2.data.conditionalaction.expression2);

	 s[i++] = sizeof(oi2.data.createActor); //37
	 s[i++] = sizeof(oi2.data.createActor.animationName);
	 s[i++] = sizeof(oi2.data.createActor.name);
	 s[i++] = sizeof(oi2.data.createActor.parentName);
	 s[i++] = sizeof(oi2.data.createActor.pathName);

	 s[i++] = sizeof(oi2.data.cursor); //42
	 s[i++] = sizeof(oi2.data.cursor.hotSpotX);
	 s[i++] = sizeof(oi2.data.cursor.hotSpotY);
	 s[i++] = sizeof(oi2.data.cursor.imgName);
	 s[i++] = sizeof(oi2.data.cursor.nFramesH);
	 s[i++] = sizeof(oi2.data.cursor.nFramesV);

	 s[i++] = sizeof(oi2.data.eventcontrol);

	 s[i++] = sizeof(oi2.data.message);
	 s[i++] = sizeof(oi2.data.message.message);

	 s[i++] = sizeof(oi2.data.moveto); //51
	 s[i++] = sizeof(oi2.data.moveto.x);
	 s[i++] = sizeof(oi2.data.moveto.y);
	 s[i++] = sizeof(oi2.data.moveto.velocity);
	 s[i++] = sizeof(oi2.data.moveto.relative);
	 s[i++] = sizeof(oi2.data.moveto.obstacle);
	 s[i++] = sizeof(oi2.data.moveto.moveType);

	 
	 s[i++] = sizeof(oi2.data.path); //58
	 s[i++] = sizeof(oi2.data.path.axis);
	 s[i++] = sizeof(oi2.data.path.name);

	 s[i++] = sizeof(oi2.data.physicalbounce); //61
	 s[i++] = sizeof(oi2.data.physicalbounce.moveType);
	 s[i++] = sizeof(oi2.data.physicalbounce.massType);
	 s[i++] = sizeof(oi2.data.physicalbounce.massEventActor);
	 s[i++] = sizeof(oi2.data.physicalbounce.massCollideActor);

	 s[i++] = sizeof(oi2.data.pathPosition);
	 s[i++] = sizeof(oi2.data.pathPosition.data);
	 s[i++] = sizeof(oi2.data.pathPosition.dataType);
	 s[i++] = sizeof(oi2.data.pathPosition.data.direction);
	 s[i++] = sizeof(oi2.data.pathPosition.data.envelope);
	 s[i++] = sizeof(oi2.data.pathPosition.data.envelope.env);
	 s[i++] = sizeof(oi2.data.pathPosition.data.fixed);

	 s[i++] = sizeof(oi2.data.position.dataType);
	 s[i++] = sizeof(oi2.data.position.data);
	 s[i++] = sizeof(oi2.data.position.data.envelope);
	 s[i++] = sizeof(oi2.data.position.data.envelope.env);
	 s[i++] = sizeof(oi2.data.position.data.fixed);

	 s[i++] = sizeof(oi2.data.settext);
	 s[i++] = sizeof(oi2.data.settext.fontPath);
	 s[i++] = sizeof(oi2.data.settext.iniASCIIChar);
	 s[i++] = sizeof(oi2.data.settext.nChars);
	 s[i++] = sizeof(oi2.data.settext.text);

	 s[i++] = sizeof(oi2.data.sound);
	 s[i++] = sizeof(oi2.data.sound.bMusic);
	 s[i++] = sizeof(oi2.data.sound.loop);
	 s[i++] = sizeof(oi2.data.sound.path);
	 s[i++] = sizeof(oi2.data.sound.volume);
	 s[i++] = sizeof(oi2.data.sound.priority);
	 s[i++] = sizeof(oi2.data.sound.pan);

	 s[i++] = sizeof(oi2.data.timer);
	 s[i++] = sizeof(oi2.data.timer.miliseconds);
	 s[i++] = sizeof(oi2.data.timer.name);

	 s[i++] = sizeof(oi2.data.transparency);
	 s[i++] = sizeof(oi2.data.transparency.dataType);
	 s[i++] = sizeof(oi2.data.transparency.data);
	 s[i++] = sizeof(oi2.data.transparency.data.envelope);
	 s[i++] = sizeof(oi2.data.transparency.data.fixed);
	 

	//////////////////////////////////////////////////

	 FILE *arq = fopen("structSize.txt", "w");
	 if(arq)
	 {
		 fprintf(arq, "************************ Struct size\n\n");

		 fprintf(arq, "***** stEvent\n\n");
		 fprintf(arq, "s1: %ld\n", s1);
		 fprintf(arq, "s2: %ld\n", s2);
		 fprintf(arq, "s3: %ld\n", s3);
		 fprintf(arq, "s4: %ld\n", s4);
		 fprintf(arq, "s5: %ld\n", s5);
		 fprintf(arq, "s6: %ld\n", s6);
		 fprintf(arq, "s7: %ld\n", s7);
		 fprintf(arq, "s8: %ld\n\n", s8);

		 fprintf(arq, "***** stAction\n\n");

		 for(int n = 0; n < i; n++)
		{
			fprintf(arq, "s[%ld]: %ld\n", n, s[n]);
		}

		 fprintf(arq, "\n***** rg\n\n");
		 fprintf(arq, "srg: %ld\n", sizeof(KrRGBA));


		 fprintf(arq, "\n***** segment\n\n");
		 KrRleSegment oiSeg;
		 fprintf(arq, "srs: %ld\n", sizeof(KrRleSegment));
		 fprintf(arq, "srs.rgba: %ld\n", sizeof(oiSeg.rgba));
		 fprintf(arq, "srs.info: %ld\n", sizeof(oiSeg.info));

		 fprintf(arq, "\n***** line\n\n");
		 KrRleLine oiLine;
		 fprintf(arq, "srl: %ld\n", sizeof(KrRleLine));
		 //fprintf(arq, "srl.flags: %ld\n", sizeof(oiLine.flags));
		 //fprintf(arq, "srl.nSegments: %ld\n", sizeof(oiLine.nSegments));
		 fprintf(arq, "srl.flags_nSegments: %ld\n", sizeof(oiLine.flags_nSegments));
		 fprintf(arq, "srl.segment: %ld\n", sizeof(oiLine.segment));

		 fprintf(arq, "\n***** rle\n\n");
		 KrRle oiRle;
		 fprintf(arq, "srle: %ld\n", sizeof(KrRle));
		 fprintf(arq, "srle.flags: %ld\n", sizeof(oiRle.flags));
		 fprintf(arq, "srle.line: %ld\n", sizeof(oiRle.line));
		 fprintf(arq, "srle.deltaHotToOrigin: %ld\n", sizeof(oiRle.deltaHotToOrigin));
		 fprintf(arq, "srle.size: %ld\n", sizeof(oiRle.size));
		 fprintf(arq, "srle.delta: %ld\n", sizeof(oiRle.delta));
		 fprintf(arq, "srle.collisionMap: %ld\n", sizeof(oiRle.collisionMap));
		 fprintf(arq, "srle.bitMapArea: %ld\n", sizeof(oiRle.bitMapArea));
		 fprintf(arq, "srle.bigData: %ld\n", sizeof(oiRle.bigData));
		 fprintf(arq, "srle.bigDataUsingGC: %ld\n", sizeof(oiRle.bigDataUsingGC));
		 fprintf(arq, "srle.frameIndex: %ld\n", sizeof(oiRle.frameIndex));
		 fprintf(arq, "srle.texture: %ld\n", sizeof(oiRle.texture));		 

		 fclose(arq);
	 }
}


void printStructMemberAddr(FILE *arq, void *baseAddr, char *memberName, void *memberAddr)
{
	fprintf(arq, "%50s: %ld\n", memberName, (int)((char *)memberAddr - (char *)baseAddr));
}

void testStructAlign()
{
	static bool bInit = false;
	if(bInit) return;

	bInit = true;



	stAction action;

	FILE *arq = fopen("structAlign.txt", "w");
	fprintf(arq, "************************ Struct align\n\n");


	printStructMemberAddr(arq, &action, "action.actionType", &action.actionType);
	printStructMemberAddr(arq, &action, "action.actorName", &action.actorName);
	printStructMemberAddr(arq, &action, "action.bAbsolutePosition", &action.bAbsolutePosition);
	printStructMemberAddr(arq, &action, "action.bConditional", &action.bConditional);
	fprintf(arq, "\n");
	
	printStructMemberAddr(arq, &action, "action.eventData", &action.eventData);	
	printStructMemberAddr(arq, &action, "action.eventData.eventType", &action.eventData.eventType);
	printStructMemberAddr(arq, &action, "action.eventData.data", &action.eventData.data);
	printStructMemberAddr(arq, &action, "action.eventData.data.mouse", &action.eventData.data.mouse);
	printStructMemberAddr(arq, &action, "action.eventData.data.mouse.x", &action.eventData.data.mouse.x);
	printStructMemberAddr(arq, &action, "action.eventData.data.mouse.y", &action.eventData.data.mouse.y);
	printStructMemberAddr(arq, &action, "action.eventData.data.mouse.bDrag", &action.eventData.data.mouse.bDrag);
	printStructMemberAddr(arq, &action, "action.eventData.data.name", &action.eventData.data.name);
	printStructMemberAddr(arq, &action, "action.eventData.data.keyEvent", &action.eventData.data.keyEvent);
	printStructMemberAddr(arq, &action, "action.eventData.data.keyEvent.key", &action.eventData.data.keyEvent.key);
	printStructMemberAddr(arq, &action, "action.eventData.data.keyEvent.bDisableRepeat", &action.eventData.data.keyEvent.bDisableRepeat);
	printStructMemberAddr(arq, &action, "action.eventData.data.keyEvent.keySequence", action.eventData.data.keyEvent.keySequence);
	printStructMemberAddr(arq, &action, "action.eventData.data.message", &action.eventData.data.message);
	printStructMemberAddr(arq, &action, "action.eventData.data.message.fromActor", &action.eventData.data.message.fromActor);
	printStructMemberAddr(arq, &action, "action.eventData.data.message.messageId", &action.eventData.data.message.messageId);
	printStructMemberAddr(arq, &action, "action.eventData.data.message.message", &action.eventData.data.message.message);
	fprintf(arq, "\n");

	printStructMemberAddr(arq, &action, "action.data", &action.data);
	printStructMemberAddr(arq, &action, "action.data.name", &action.data.name);
	printStructMemberAddr(arq, &action, "action.data.bEnable", &action.data.bEnable);
	fprintf(arq, "\n");

	printStructMemberAddr(arq, &action, "action.data.eventcontrol", &action.data.eventcontrol);
	printStructMemberAddr(arq, &action, "action.data.eventcontrol.bEnable", &action.data.eventcontrol.bEnable);
	printStructMemberAddr(arq, &action, "action.data.eventcontrol.events", &action.data.eventcontrol.events);
	fprintf(arq, "\n");

	printStructMemberAddr(arq, &action, "action.data.pathPosition", &action.data.pathPosition);
	printStructMemberAddr(arq, &action, "action.data.pathPosition.dataType", &action.data.pathPosition.dataType);
	printStructMemberAddr(arq, &action, "action.data.pathPosition.data", &action.data.pathPosition.data);
	printStructMemberAddr(arq, &action, "action.data.pathPosition.data.direction", &action.data.pathPosition.data.direction);
	printStructMemberAddr(arq, &action, "action.data.pathPosition.data.fixed", &action.data.pathPosition.data.fixed);
	printStructMemberAddr(arq, &action, "action.data.pathPosition.data.fixed.frame", &action.data.pathPosition.data.fixed.frame);
	printStructMemberAddr(arq, &action, "action.data.pathPosition.data.envelope", &action.data.pathPosition.data.envelope);
	printStructMemberAddr(arq, &action, "action.data.pathPosition.data.envelope.env", &action.data.pathPosition.data.envelope.env);
	fprintf(arq, "\n");

	printStructMemberAddr(arq, &action, "action.data.path", &action.data.path);
	printStructMemberAddr(arq, &action, "action.data.path.name", &action.data.path.name);
	printStructMemberAddr(arq, &action, "action.data.path.axis", &action.data.path.axis);
	fprintf(arq, "\n");

	printStructMemberAddr(arq, &action, "action.data.animationFrame", &action.data.animationFrame);
	printStructMemberAddr(arq, &action, "action.data.animationFrame.dataType", &action.data.animationFrame.dataType);
	printStructMemberAddr(arq, &action, "action.data.animationFrame.data", &action.data.animationFrame.data);
	printStructMemberAddr(arq, &action, "action.data.animationFrame.data.fixed", &action.data.animationFrame.data.fixed);
	printStructMemberAddr(arq, &action, "action.data.animationFrame.data.fixed.frame", &action.data.animationFrame.data.fixed.frame);
	printStructMemberAddr(arq, &action, "action.data.animationFrame.data.envelope", &action.data.animationFrame.data.envelope);
	printStructMemberAddr(arq, &action, "action.data.animationFrame.data.envelope.env", &action.data.animationFrame.data.envelope.env);
	fprintf(arq, "\n");

	printStructMemberAddr(arq, &action, "action.data.color", &action.data.color);
	printStructMemberAddr(arq, &action, "action.data.color.dataType", &action.data.color.dataType);
	printStructMemberAddr(arq, &action, "action.data.color.data", &action.data.color.data);
	printStructMemberAddr(arq, &action, "action.data.color.data.fixed", &action.data.color.data.fixed);
	printStructMemberAddr(arq, &action, "action.data.color.data.fixed.red", &action.data.color.data.fixed.red);
	printStructMemberAddr(arq, &action, "action.data.color.data.fixed.green", &action.data.color.data.fixed.green);
	printStructMemberAddr(arq, &action, "action.data.color.data.fixed.blue", &action.data.color.data.fixed.blue);
	printStructMemberAddr(arq, &action, "action.data.color.data.envelope", &action.data.color.data.envelope);
	printStructMemberAddr(arq, &action, "action.data.color.data.envelope.env", &action.data.color.data.envelope.env);
	fprintf(arq, "\n");

	printStructMemberAddr(arq, &action, "action.data.transparency", &action.data.transparency);
	printStructMemberAddr(arq, &action, "action.data.transparency.dataType", &action.data.transparency.dataType);
	printStructMemberAddr(arq, &action, "action.data.transparency.data", &action.data.transparency.data);
	printStructMemberAddr(arq, &action, "action.data.transparency.data.fixed", &action.data.transparency.data.fixed);
	printStructMemberAddr(arq, &action, "action.data.transparency.data.envelope", &action.data.transparency.data.envelope);
	printStructMemberAddr(arq, &action, "action.data.transparency.data.envelope.env", &action.data.transparency.data.envelope.env);
	fprintf(arq, "\n");

	printStructMemberAddr(arq, &action, "action.data.cursor", &action.data.cursor);
	printStructMemberAddr(arq, &action, "action.data.cursor.imgName", &action.data.cursor.imgName);
	printStructMemberAddr(arq, &action, "action.data.cursor.nFramesH", &action.data.cursor.nFramesH);
	printStructMemberAddr(arq, &action, "action.data.cursor.nFramesV", &action.data.cursor.nFramesV);
	printStructMemberAddr(arq, &action, "action.data.cursor.hotSpotX", &action.data.cursor.hotSpotX);
	printStructMemberAddr(arq, &action, "action.data.cursor.hotSpotY", &action.data.cursor.hotSpotY);
	fprintf(arq, "\n");

	printStructMemberAddr(arq, &action, "action.data.timer", &action.data.timer);
	printStructMemberAddr(arq, &action, "action.data.timer.name", &action.data.timer.name);
	printStructMemberAddr(arq, &action, "action.data.timer.miliseconds", &action.data.timer.miliseconds);
	fprintf(arq, "\n");

	printStructMemberAddr(arq, &action, "action.data.settext", &action.data.settext);
	printStructMemberAddr(arq, &action, "action.data.settext.text", &action.data.settext.text);
	printStructMemberAddr(arq, &action, "action.data.settext.fontPath", &action.data.settext.fontPath);
	printStructMemberAddr(arq, &action, "action.data.settext.iniASCIIChar", &action.data.settext.iniASCIIChar);
	printStructMemberAddr(arq, &action, "action.data.settext.nChars", &action.data.settext.nChars);
	fprintf(arq, "\n");

	printStructMemberAddr(arq, &action, "action.data.sound", &action.data.sound);
	printStructMemberAddr(arq, &action, "action.data.sound.bMusic", &action.data.sound.bMusic);
	printStructMemberAddr(arq, &action, "action.data.sound.path", &action.data.sound.path);
	printStructMemberAddr(arq, &action, "action.data.sound.volume", &action.data.sound.volume);
	printStructMemberAddr(arq, &action, "action.data.sound.loop", &action.data.sound.loop);
	printStructMemberAddr(arq, &action, "action.data.sound.priority", &action.data.sound.priority);
	printStructMemberAddr(arq, &action, "action.data.sound.pan", &action.data.sound.pan);
	fprintf(arq, "\n");

	printStructMemberAddr(arq, &action, "action.data.conditionalaction", &action.data.conditionalaction);
	printStructMemberAddr(arq, &action, "action.data.conditionalaction.expression1", &action.data.conditionalaction.expression1);
	printStructMemberAddr(arq, &action, "action.data.conditionalaction.expression2", &action.data.conditionalaction.expression2);
	printStructMemberAddr(arq, &action, "action.data.conditionalaction.condition", &action.data.conditionalaction.condition);
	fprintf(arq, "\n");

	printStructMemberAddr(arq, &action, "action.data.animation", &action.data.animation);
	printStructMemberAddr(arq, &action, "action.data.animation.name", &action.data.animation.name);
	printStructMemberAddr(arq, &action, "action.data.animation.state", &action.data.animation.state);
	fprintf(arq, "\n");

	printStructMemberAddr(arq, &action, "action.data.createActor", &action.data.createActor);
	printStructMemberAddr(arq, &action, "action.data.createActor.name", &action.data.createActor.name);
	printStructMemberAddr(arq, &action, "action.data.createActor.animationName", &action.data.createActor.animationName);
	printStructMemberAddr(arq, &action, "action.data.createActor.parentName", &action.data.createActor.parentName);
	printStructMemberAddr(arq, &action, "action.data.createActor.pathName", &action.data.createActor.pathName);
	fprintf(arq, "\n");

	printStructMemberAddr(arq, &action, "action.data.position", &action.data.position);
	printStructMemberAddr(arq, &action, "action.data.position.dataType", &action.data.position.dataType);
	printStructMemberAddr(arq, &action, "action.data.position.data", &action.data.position.data);
	printStructMemberAddr(arq, &action, "action.data.position.data.fixed", &action.data.position.data.fixed);
	printStructMemberAddr(arq, &action, "action.data.position.data.fixed.x", &action.data.position.data.fixed.x);
	printStructMemberAddr(arq, &action, "action.data.position.data.fixed.y", &action.data.position.data.fixed.y);
	printStructMemberAddr(arq, &action, "action.data.position.data.envelope", &action.data.position.data.envelope);
	printStructMemberAddr(arq, &action, "action.data.position.data.envelope.env", &action.data.position.data.envelope.env);
	fprintf(arq, "\n");

	printStructMemberAddr(arq, &action, "action.data.message", &action.data.message);
	printStructMemberAddr(arq, &action, "action.data.message.message", &action.data.message.message);
	fprintf(arq, "\n");

	printStructMemberAddr(arq, &action, "action.data.moveto", &action.data.moveto);
	printStructMemberAddr(arq, &action, "action.data.moveto.x", &action.data.moveto.x);
	printStructMemberAddr(arq, &action, "action.data.moveto.y", &action.data.moveto.y);
	printStructMemberAddr(arq, &action, "action.data.moveto.velocity", &action.data.moveto.velocity);
	printStructMemberAddr(arq, &action, "action.data.moveto.relative", &action.data.moveto.relative);
	printStructMemberAddr(arq, &action, "action.data.moveto.obstacle", &action.data.moveto.obstacle);
	printStructMemberAddr(arq, &action, "action.data.moveto.moveType", &action.data.moveto.moveType);	
	fprintf(arq, "\n");

	printStructMemberAddr(arq, &action, "action.data.physicalbounce", &action.data.physicalbounce);
	printStructMemberAddr(arq, &action, "action.data.physicalbounce.moveType", &action.data.physicalbounce.moveType);
	printStructMemberAddr(arq, &action, "action.data.physicalbounce.massType", &action.data.physicalbounce.massType);
	printStructMemberAddr(arq, &action, "action.data.physicalbounce.massEventActor", &action.data.physicalbounce.massEventActor);
	printStructMemberAddr(arq, &action, "action.data.physicalbounce.massCollideActor", &action.data.physicalbounce.massCollideActor);
	printStructMemberAddr(arq, &action, "action.data.physicalbounce.collideVelocityMultiplier", &action.data.physicalbounce.collideVelocityMultiplier);
	printStructMemberAddr(arq, &action, "action.data.physicalbounce.eventVelocityMultiplier", &action.data.physicalbounce.eventVelocityMultiplier);
	fprintf(arq, "\n");
	
	printStructMemberAddr(arq, &action, "action.data.zdepth", &action.data.zdepth);
	fprintf(arq, "\n");

	printStructMemberAddr(arq, &action, "action.next", &action.next);
	printStructMemberAddr(arq, &action, "action.x", &action.x);
	printStructMemberAddr(arq, &action, "action.y", &action.y);
	printStructMemberAddr(arq, &action, "action.executionType", &action.executionType);
	printStructMemberAddr(arq, &action, "action.bLastActionInQueueOnly", &action.bLastActionInQueueOnly);
	printStructMemberAddr(arq, &action, "action.name", &action.name);
	printStructMemberAddr(arq, &action, "action.nFrames", &action.nFrames);
	printStructMemberAddr(arq, &action, "action.frames", &action.frames);
	printStructMemberAddr(arq, &action, "action.timeStamp", &action.timeStamp);
	fprintf(arq, "\n");

	KrRleSegment srs;
	printStructMemberAddr(arq, &srs, "srs.rgba", &srs.rgba);
	printStructMemberAddr(arq, &srs, "srs.info", &srs.info);
	fprintf(arq, "\n");

	KrRleLine srl;
	//printStructMemberAddr(arq, &srl, "srl.flags", &srl.flags);
	//printStructMemberAddr(arq, &srl, "srl.nSegments", &srl.nSegments);
	printStructMemberAddr(arq, &srl, "srl.flags_nSegments", &srl.flags_nSegments);
	printStructMemberAddr(arq, &srl, "srl.segment", &srl.segment);
	fprintf(arq, "\n");

	KrRle srle;
	printStructMemberAddr(arq, &srle, "srle.flags", &srle.flags);
	printStructMemberAddr(arq, &srle, "srle.line", &srle.line);
	printStructMemberAddr(arq, &srle, "srle.deltaHotToOrigin", &srle.deltaHotToOrigin);
	printStructMemberAddr(arq, &srle, "srle.size", &srle.size);
	printStructMemberAddr(arq, &srle, "srle.delta", &srle.delta);
	printStructMemberAddr(arq, &srle, "srle.collisionMap", &srle.collisionMap);
	printStructMemberAddr(arq, &srle, "srle.bitMapArea", &srle.bitMapArea);
	printStructMemberAddr(arq, &srle, "srle.bigData", &srle.bigData);
	printStructMemberAddr(arq, &srle, "srle.bigDataUsingGC", &srle.bigDataUsingGC);
	printStructMemberAddr(arq, &srle, "srle.frameIndex", &srle.frameIndex);
	printStructMemberAddr(arq, &srle, "srle.texture", &srle.texture);
	fprintf(arq, "\n");

	fclose(arq);	
}



#endif

//mapWorkingActors used for speedup collision test and for SetAnteriorPositions
#ifdef USE_AVL_TREE
int cmpString(const gedString& s1, const gedString& s2)
{
	return s1.compare(s2);
}

MapActor mapActors(cmpString), mapWorkingActors(cmpString);
#else
MapActor mapActors, mapWorkingActors;
#endif

MapActorMaxClone mapActorsMaxClone;

int structActorSize = sizeof(stActorVars);
Actor *Actor::change = NULL;
bool Actor::bShowRegions = true;
int Actor::baseZDepth = 0;
bool Actor::bInContructionCallBack = false;

#ifndef ACTOR_USES_VIRTUAL_FUNCTIONS

GlDynArray<void *> Actor::freeList;

void *Actor::operator new(size_t size)
{
	//Must allocate always the same size
	void *m = NULL;

#ifdef DEBUG
	static int oldsize = 0;

	if(oldsize && size != oldsize)
	{
		DebugBreak();
	}

	oldsize = size;
#endif

	if(freeList.Count())
	{
		m = (ActorMessage *)freeList.Back();
		freeList.PopBack();
		memset(m, 0, size);		
	}
	else
	{
		//No itens available in the free list, so allocate new
		m = (Actor *)dlmalloc(size);
	}

	return m;
}

void Actor::operator delete(void * mem)
{
	//Don't delete, just mark as available to avoid crash in the magic test
	if(mem) 
	{		
		//Put in the free list
		freeList.PushBack(mem);		
	}
}
#endif

Actor::Actor(const gedString& spriteName, Actor *_parent, ActorType _type, int canvasWidth, int canvasHeight, bool _bPanel, Action *copyAction, bool bIncrementCloneIndex, int newCloneIndex)
{
#if defined(_TESTSTRUCTALIGN_) && defined(_DEBUG)
	testStructSize();
	testStructAlign();
#endif

#ifndef ACTOR_USES_VIRTUAL_FUNCTIONS
	magic = this;
#endif

	parent = NULL;

	if(!_bPanel && !_parent)
	{
		_parent = GameControl::Get()->GetAxis();
	}
	
	owner = UNASSIGNED_SYSTEM_ADDRESS;
	ownerShipRequestTime = (RakNetTime)-1; //0 oldest time stamp, FFFF... newest time stamp
		
		
	type = _type;

	flags.Set(FLAG_PANEL, _bPanel);
	flags.Clear(FLAG_NETACTOR);
	flags.Clear(FLAG_VIEW);
	flags.Clear(FLAG_REGION);
	flags.Clear(FLAG_RANDOMPATH);
	flags.Clear(FLAG_LOCKACTOR);
	flags.Clear(FLAG_PENDINGSETTEXT);
	flags.Clear(FLAG_DELETECONFIRMATION);
	flags.Clear(FLAG_NET_OPTIMIZED_PRECISION);
	flags.Clear(FLAG_NET_UNLINKED);

	sprite = NULL;
	canvas = NULL;
	slowCanvas = NULL;
	spriteResource = NULL;
	spriteResource = NULL;
	pathIniX = pathIniY = 0;
	
	textActor = NULL;
	tile = NULL;
	drawInfo = NULL;
	
	nZeros = 0;
	
	
	

	//Script vars
	scriptVars = (struct stActorVars *)malloc(structActorSize);
	scriptVarsAnt = (struct stActorVars *)malloc(structActorSize);

	memset(scriptVars, 0, structActorSize);
	strcpy(scriptVars->name, spriteName.getCharBuf());
	scriptVars->animindex = -1;
	scriptVars->angle = INVALID_ANGLE;
	scriptVars->x = scriptVars->y = 2147483647L; //To avoid the (0, 0) position bug (if a  view has a (0, 0) nothing was happen)

	scriptVars->r = scriptVars->g = scriptVars->b = 255.0;
	
	//Solve the bug "when set textNumber to zero don't changes default text" (Issue 78, Test057.ged)
	scriptVars->textNumber = TEXT_NUMBER_INIT; 

	parent = _parent; //Set parent here to solve the bug "Activation Region are always created at same place"
	

	//Initialize resource
	if(type == SPRITE)
	{
		if((spriteResource = engine->Vault()->GetSpriteResource(spriteName)) == NULL)
		{
			spriteResource = new KrSpriteResource(spriteName);
			engine->Vault()->AddResource(spriteResource);
		}	
		
		spriteResource->refCount++;			
	}
	else if(type == CANVAS)
	{
		if((canvasResource = engine->Vault()->GetCanvasResource(spriteName)) == NULL)
		{
			canvasResource = new KrCanvasResource(spriteName, 
				canvasWidth,
				canvasHeight,
				true );

			//Create a tansparent actor
			KrRGBA *pixels = canvasResource->Pixels(), color;
			
			color.Set(0, 0, 0, 1);
			
			//Fill background
			int i;
			for(i = 0; i < canvasWidth*canvasHeight; i++)
			{
				pixels[i] = color;
			}
			
			//Add
			engine->Vault()->AddResource(canvasResource);
		}	
		
		canvas = new KrCanvas( canvasResource, this );
		canvas->SetNodeId((int)canvas);
		engine->Tree()->AddNode( _parent?_parent->getImage():0, canvas );

		scriptVars->width = widthClick = canvasResource->Width();
		scriptVars->height = heightClick = canvasResource->Height();

		//Border color
		red = 0;
		green = 255;
		blue = 255;

		if(!_bPanel)
		{
			canvas->SetZDepth(MAX_ACTOR_DEPTH/2 + baseZDepth);
			if(!GameControl::Get()->getGameMode() && _parent == GameControl::Get()->GetAxis()) 
				baseZDepth++;
		}

		CenterOnScreen();
		
		canvasResource->refCount++;
	}
	else
	{
		//Slow canvas
		slowCanvas = new SlowCanvas(this);
		slowCanvas->SetNodeId((int)slowCanvas);
		engine->Tree()->AddNode( _parent?_parent->getImage():0, slowCanvas );

		if(type == VIEW_ACTOR)
		{
			flags.Set(FLAG_VIEW);
			AdjustView(GameControl::Get()->getGameWidth(), GameControl::Get()->getGameHeight(), true);			
			getImage()->SetHitTest(true);
			((SlowCanvas *)getImage())->setEntireBoxHit(false);
			GameControl::Get()->SetViewActor(this);

			//Put the view on the top of others actors
			slowCanvas->SetZDepth(MAX_ACTOR_DEPTH);

			red = 255;
			green = 255;
			blue = 255;						
		}
		else if(type == REGION_ACTOR || type == REGION_ACTOR_FILLED)
		{
			getImage()->SetDimensions(0, 0, 200, 200);
			
			//Put the region on the top of others actors, but don't use the REGION_DEPTH
			//to allow change zdepth in the editor
			getImage()->SetZDepth(MAX_ACTOR_DEPTH);
			
			SetTransparency(0);	
			
			getImage()->SetHitTest(true);
			getImage()->setSlowCanvasCollision(true);
			
			((SlowCanvas *)getImage())->setThickness(3);
			((SlowCanvas *)getImage())->setMinDimentions(CORNER_SIDE, CORNER_SIDE);
			
			widthClick = ((SlowCanvas *)getImage())->Width();
			heightClick = ((SlowCanvas *)getImage())->Height();
			
			if(type == REGION_ACTOR)
			{				
				((SlowCanvas *)getImage())->setEntireBoxHit(false);				
				
				//Default color
				red = 0;
				green = 255;
				blue = 0;
			}
			else
			{
				((SlowCanvas *)getImage())->setEntireBoxHit(true);				
				
				//Default color
				red = 0;
				green = 0;
				blue = 255;
			}
			
			//Set default cursor
			cursor.imgName	= "makscursorMao.bmp";
			cursor.nFramesH	= 1;
			cursor.nFramesV	= 1;
			cursor.hotSpotX = 7;
			cursor.hotSpotY = 0;
			cursor.bAnimate = false;

			CenterOnScreen();
		}
	}
	
	
	//bNodeAdd = false;
	framesToAnim = ENABLE_ANIMATION;

	flags.Clear(FLAG_EDIT);
	flags.Set(FLAG_ENABLECOLLISIONDETECTION, !flags.IsSet(FLAG_VIEW)); //Disable collision on view actor
	flags.Set(FLAG_FORWARDANIMATION);
	flags.Set(FLAG_CREATEATSTARTUP);
	flags.Clear(FLAG_ONCREATECALLED);
	flags.Clear(FLAG_CALLONCREATE);
	
		
	pathX = pathY = pathFinder = NULL;
	pathDirectionX = pathDirectionY = FORWARD;
	
	
	action = NULL;
	if(copyAction)
		setAction(copyAction);
	
		

	cursor.nFramesH = 0;	

	followMouseAxis = NONE_AXIS;	

	flags.Clear(FLAG_HASSHADOWS);
	flags.Clear(FLAG_RUNNING);
	flags.Clear(FLAG_WORKING);
	flags.Clear(FLAG_HANDLECOLLISION);	
	flags.Clear(FLAG_EDITANIM);
	flags.Clear(FLAG_SETANTERIORPOSITION);
	flags.Clear(FLAG_MOVETO);
	flags.Clear(FLAG_ACTORINVIEW);
	flags.Clear(FLAG_SETACTUALVALUES);

	anteriorXPos = anteriorYPos = anteriorFramePathX = anteriorFramePathY = 0;
	frameAnt = 0;
	antAngle = 0.0;
	

	

	eventControl = EVENTALL;
	lastTimeStamp = 0;
	lastQueuedAction = NULL;
	totalConnectedActors = 0;
	
	if(flags.IsSet(FLAG_VIEW) && GameControl::Get()->getGameMode())
	{
		//Disable mouse button events in view
		setEventControl(EVENTMOUSEBUTTONDOWN, false);
		setEventControl(EVENTMOUSEBUTTONUP, false);
		setEventControl(EVENTMOUSEENTER, false);
		setEventControl(EVENTMOUSELEAVE, false);
		setEventControl(EVENTMOUSEMOVE, false);
	}
	
	//Set clone index
	int nextCloneIndex = 0;
	
	if(newCloneIndex == -1)
	{
		if(bIncrementCloneIndex || (!GameControl::Get()->getGameMode() && !loadingActors)) //Not increase edit actors in game mode
		{
			int *maxIndex = mapActorsMaxClone.FindString(scriptVars->name);
			if(maxIndex) 
			{
				(*maxIndex)++;
				nextCloneIndex = *maxIndex;
			}
			else
				mapActorsMaxClone.Add(scriptVars->name, nextCloneIndex);
		}
	}
	else
	{
		//Solve the Move to Region 2.ged actor move to a wrong position
		nextCloneIndex = newCloneIndex;
		
		int *maxIndex = mapActorsMaxClone.FindString(scriptVars->name);
		if(maxIndex) 
		{
			if(*maxIndex < nextCloneIndex)
				*maxIndex = nextCloneIndex;
		}
		else
		{
			mapActorsMaxClone.Add(scriptVars->name, nextCloneIndex);				
		}
	}

	
	scriptVars->cloneindex = nextCloneIndex;
	sprintf(scriptVars->clonename, "%s.%ld", scriptVars->name, scriptVars->cloneindex);
	GameControl::Get()->AddActor(this); //After clonename setup

	if(GameControl::Get()->getGameMode()) SetActualValues(NEED_ALL);

	//Previous script vars (memcopy is slow)
	scriptVarsAnt->x = scriptVars->x;	
	scriptVarsAnt->y = scriptVars->y;
	scriptVarsAnt->xscreen = scriptVars->xscreen;
	scriptVarsAnt->yscreen = scriptVars->yscreen;
	scriptVarsAnt->animpos = scriptVars->animpos;
	scriptVarsAnt->xvelocity = scriptVars->xvelocity;
	scriptVarsAnt->yvelocity = scriptVars->yvelocity;
	scriptVarsAnt->diretional_velocity = scriptVars->diretional_velocity;
	scriptVarsAnt->angle = scriptVars->angle;
	scriptVarsAnt->r = scriptVars->r;
	scriptVarsAnt->g = scriptVars->g;
	scriptVarsAnt->b = scriptVars->b;
	scriptVarsAnt->transp = scriptVars->transp;
	scriptVarsAnt->textNumber = scriptVars->textNumber;
	strcpy(scriptVarsAnt->text, scriptVars->text);

	
	sequence.FindOrCreate(scriptVars->name)->nRef++;	
	mySequence = sequence[scriptVars->name];

	flags.Set(FLAG_CALLONDESTROY);

	
	//Don't check action here
	//Causes the bug on 1945 reg.ged in stand alone mode: show level_01 due don't execute your OnCreate action
	//UpdateRegionLoad() don't will works here. Moved to setRunning
	if(/*UpdateRegionLoad()*/ /*&& action*/ /*&&*/ !RegionLoad::getCreatingActors())
	{
		flags.Set(FLAG_CALLONCREATE);
	}	

#ifdef DEBUG
	if(GameControl::Get()->getGameMode()) GLOUTPUT( "--- New Actor: %s (%X)\n",  scriptVars->clonename, this);
#endif
}

void Actor::InitEdit()
{
	//Avoid use of ActorEdit in stand alone games

	//if(!GameControl::Get()->getStandAloneMode()) //Bug in create actor
	{
		AddAnimation(ACTOR_GED_ICON, "maksiconActor.gif", 1, 1);
	}
	
	AddAnimation(ACTOR_GED_TEXT, "maksiconText.bmp", 1, 1);

	
	if(type != VIEW_ACTOR)
	{
		SetPathIni(0, 0);	
		SetPos(0, 0);
	}
	
	flags.Set(FLAG_EDIT);	

	//For script
	GameControl::Get()->getScript()->AddActor(this);
}

Actor::~Actor()
{
	if(action && GameControl::Get()->getGameMode() && flags.IsSet(FLAG_CALLONDESTROY)) action->OnDestroyActor(this);
	GameControl::Get()->RemoveActor(this);
	
#ifndef ACTOR_USES_VIRTUAL_FUNCTIONS
	magic = NULL; 
#endif

	delete drawInfo;
	delete pathFinder;

	
	if(flags.IsSet(FLAG_NET_UNLINKED)) 
	{
		GameControl::Get()->RemoveActorFromUnlinkedList(this);
	}

	if(flags.IsSet(FLAG_RUNNING)) 
	{
		GameControl::Get()->getScript()->InvalidateActor(scriptVars->name);
		GameControl::Get()->RemoveActorFromHandledCollisionMap(this);		
	}

	if(flags.IsSet(FLAG_WORKING))
	{
		GameControl::Get()->RemoveWorkingActor(this);		
	}

	if(flags.IsSet(FLAG_RUNNING) || flags.IsSet(FLAG_EDIT))
	{
		ListActor *list = mapActors.FindString(scriptVars->name);
		if((list && list->size() == 0) || !list)
		{		
			GameControl::Get()->getScript()->RemoveActor(this);
		}
		
	}

	if(tile)
	{
		if(tile == GameControl::Get()->GetTile()) 
		{
			GameControl::Get()->SetTileMode(NULL);
		}

		delete tile;
	}

	

	RemoveTimers();


	if(action) action->DetachAction();

#ifndef STAND_ALONE_GAME
	//Remove buttons
	while(!listButton.Empty())
	{
		Button *button = (Button *)listButton.Front();
		listButton.PopFront();

		delete button;
	}


	//Remove edit boxes
	while(!listEditBox.Empty())
	{
		EditBox *edit = (EditBox *)listEditBox.Front();
		listEditBox.PopFront();

		delete edit;
	}


	//Remove texts
	while(!listText.Empty())
	{
		Text *text = (Text *)listText.Front();
		listText.PopFront();

		delete text;
	}

	//Remove slides
	while(!listSlide.Empty())
	{
		Slide *slide = (Slide *)listSlide.Front();
		listSlide.PopFront();

		delete slide;
	}

	//Remove list boxes
	while(!listListBox.Empty())
	{
		ListBox *list = (ListBox *)listListBox.Front();
		listListBox.PopFront();

		delete list;
	}

	//Remove list pops
	while(!listListPop.Empty())
	{
		ListPop *list = (ListPop *)listListPop.Front();
		listListPop.PopFront();

		delete list;
	}

	//Remove list pops
	while(!listListDirectory.Empty())
	{
		ListDirectory *list = (ListDirectory *)listListDirectory.Front();
		listListDirectory.PopFront();

		delete list;
	}

#endif //#ifndef STAND_ALONE_GAME

	//Remove sequences
	{mySequence->nRef--;
	if(mySequence->nRef == 0)
	{
		mySequence->listSequence.Clear();
		if(!mySequence->mapSequence.empty())
		{
			MapSequenceIterator it(mySequence->mapSequence);
			it.Begin();
			while(!it.Done())
			{
				Sequence *seq = *it.Value();
				mySequence->mapSequence.Remove(*it.Key());
			
				delete seq;	
				
				it.Begin();
			}
		}
	}}

	if(GameControl::Get()->getGameMode())
	{
		//Notify childs
		if(engine->Tree()->FindNodeById((int)getImage())) //FindNodeById: solve the crash in 1945.ged when destroy the ship
		{			
			GlInsideNodeIt<KrImNode*> it = getImage()->ChildTreeIterator();
			it.Begin(); 
			while( !it.Done() )
			{
				KrImNode *node = it.CurrentData();
				if(!node->getIsText())
				{
					Actor *childActor = (Actor *)node->getParentActor();
					
					if(childActor && childActor != this && childActor->getRunning())
					{
						childActor->RemoveParent();

						//Solve the destroyPlayerBug.ged
						it.Begin(); 
						continue;
					}
				}
				
				it.Next();
			}
		}
		
	}

	if(type == SPRITE)
	{
		if(spriteResource && engine->Vault()->ExistsResource(spriteResource))
		{
			spriteResource->refCount--;
			
			//Don't test fast file here to avoid oversized exported files
			if(spriteResource->refCount == 0 /*&& !GameControl::Get()->getFastFile()*/)
				engine->Vault()->RemoveSpriteResource(spriteResource);
		}
		
		if(engine->Tree()->FindNodeById((int)sprite))
		{
			engine->Tree()->DeleteNode(sprite);
		}
	}
	else if(type == CANVAS)
	{
		if(engine->Tree()->FindNodeById((int)canvas))
		{
			engine->Tree()->DeleteNode(canvas); //Detele before resource delete
		}

		if(engine->Vault()->ExistsResource(canvasResource))
		{
			canvasResource->refCount--;
			
			//Don't test fast file here to avoid oversized exported files
			if(canvasResource->refCount == 0 /*&& !GameControl::Get()->getFastFile()*/)
				engine->Vault()->RemoveCanvasResource(canvasResource);	
		}
	} 
	else
	{
		if(engine->Tree()->FindNodeById((int)slowCanvas))
		{
			engine->Tree()->DeleteNode(slowCanvas);
		}
	}

	//Set max clone index
	//if(!GameControl::Get()->getGameMode()) 
	{
		int *maxIndex = mapActorsMaxClone.FindString(scriptVars->name);
		if(maxIndex) 
		{
			ListActor *listActor = mapActors.FindString(scriptVars->name);
			if(listActor)
			{
				bool bFound = false;
				*maxIndex = 0;

				for(int il = 0; il < listActor->Count(); il++)
				{
					Actor *actor = (*listActor)[il];
					if( GameControl::Get()->getGameMode() && actor->getRunning() || 
						!GameControl::Get()->getGameMode() && actor->EditMode())
					{
						if(actor->getCloneIndex() > *maxIndex)
						{
							*maxIndex = actor->getCloneIndex();
						}

						bFound = true;
					}
				}	

				if(!bFound)
				{
					//No more actors, remove the maxIndex
					//Solve the 1945_4.ged bug when myPlane is destroyed twice
					mapActorsMaxClone.Remove(scriptVars->name);
				}
			}
			else
				mapActorsMaxClone.Remove(scriptVars->name);
		}
	}

	if(this == GameControl::Get()->GetViewActor()) GameControl::Get()->SetViewActor(NULL);

	//Free script vars
	free(scriptVars);
	free(scriptVarsAnt);
}

void Actor::RemoveTimers()
{
	//Remove timers
	if(!mapTimer.empty())
	{
		MapTimerIterator it(mapTimer);
		it.Begin();
		while(!it.Done())
		{
			stTimer *timer = *it.Key();
			mapTimer.Remove(timer);
			
			SDL_RemoveTimer(timer->timerID);
			if(action) action->CleanUpTimers(this, timer->timerID);
			delete timer;		

			it.Begin();
		}
	}
}

void Actor::setCreator(Actor *creatorActor) 
{
	//Save the name, not the pointer
	//The system can reuse a pointer and causes a error in the game
	//So, the pointer can point to a different actor, after destroy the creator
	if(creatorActor)
	{
		creatorCloneName = creatorActor->getCloneName();
	}
}

void Actor::setCreator(const gedString& creatorActorCloneName) 
{
	if(creatorActorCloneName.length())
	{
		creatorCloneName = creatorActorCloneName;
	}
}

Actor *Actor::getCreator() 
{
	Actor *creator = NULL;

	if(!creatorCloneName.empty())
	{
		//Test getRunning to solve the bug in creator_bug.ged (don't destroy first zombie)
		creator = GameControl::Get()->GetActor(creatorCloneName, true, false, false);
		if(!creator || !creator->getRunning())
		{
			//Cretor destroyed, don't try again
			creatorCloneName.clear();
			creator = NULL;
		}
	}

	return creator;
}

void Actor::FindMaxCloneIndex()
{
	int *maxIndex = mapActorsMaxClone.FindString(scriptVars->name);
	if(maxIndex) 
	{
		if(scriptVars->cloneindex > *maxIndex)
			*maxIndex = scriptVars->cloneindex;
	}
	else
		mapActorsMaxClone.Add(scriptVars->name, scriptVars->cloneindex);
}

void Actor::ResetMaxCloneIndex(char *actorName)
{
	mapActorsMaxClone.Remove(actorName);
}

bool Actor::ExistsAction(const gedString& actionName)
{
	if(type != SPRITE)
		return false;

	return mySequence->mapSequence[actionName] != NULL;
}

Text *Actor::AddText(const gedString& text, int x, int y, int align, int _red, int _green, int _blue, const gedString& fontName, int iniASCIIChar, int nChars,
					 bool _bEditable, bool _hasBorder, bool _hasBackGround, bool _getFocus,
					 KrRGBA _borderColor, KrRGBA _cursorColor, KrRGBA _backGroundColor)
{	
	Text *textBox = new Text(text, x, y, align, this, _red, _green, _blue, fontName, iniASCIIChar, nChars,
						     _bEditable, _hasBorder, _hasBackGround, _getFocus, 
							 _borderColor, _cursorColor, _backGroundColor);

	
	if(textBox->getFontResource())
	{
		listText.PushBack(textBox);
	}
	else
	{
		delete textBox;
		textBox = NULL;
	}

	
	return textBox;
}

static void SetImageError(const char *imgName)
{
	char *error = SDL_GetError();

	if(*error != 0)
	{
		char msg1[1024];
		strcpy(msg1, error);

		SDL_ClearError();

		SDL_SetError("Image load error: '%s' on '%s' file", msg1, imgName);
	}
}

bool Actor::AddAnimation(const gedString& actionName, const gedString& imgName, int nFramesH, int nFramesV, int hotSpotX, int hotSpotY, int fps, bool bTransparency, bool bReplaceFile, bool bForceGedIcon)
{
	/*
	actionName  : name of the inserted action
	imgName		: image file
	nFramesH	: number of horizontal frames
	nFramesV	: number of vertical frames
	*/
	
	if(type != SPRITE || !spriteResource)
		return false;
	
	if(bReplaceFile)
	{
		spriteResource->RemoveAction(actionName);
	}
	
	bool bAddAction = false;
	KrAction *krAction = spriteResource->GetAction( actionName );
	if (!krAction)
	{
		spriteResource->AddAction(new KrAction(actionName, imgName, nFramesH,  nFramesV, hotSpotX, hotSpotY, fps));
		bAddAction = true;
	}
	else if(!krAction->getimgName().length()) 
	{
		//Action already loaded, but not have your attributes initialized
		krAction->SetAttr(actionName, imgName, nFramesH,  nFramesV, hotSpotX, hotSpotY, fps);
	}
	
#ifndef STAND_ALONE_GAME //maks
	gedString aux(bForceGedIcon?"maksiconActor.gif":imgName);
	if(bAddAction)
	{
		/*KrRGBA rgba;
		rgba.c.red = 0;
		rgba.c.green = 0;
		rgba.c.blue = 0;*/

		//32 bits images uses RGBA alpha channel
		
		if(imgName.find("*") != gedString::npos)
		{
			//Multiple files
			gedString file, path;
			SplitPath(imgName, file, path);
			path = path + DIR_SEP;

			//Find *
			int numberPos = file.find('*');
			
			KrAction* actionRes = spriteResource->GetAction( actionName );

			long hnd = ged_findfirst(aux.c_str(), A_ANY);
			if(hnd != GED_NOTFOUND)
			{	
				//Read file names and sort
				ListString listFiles;
				do
				{
					char *fileName = ged_filename(hnd), *name = fileName + numberPos;

					//Add only the files in same sequence
					if(isdigit(*name))
					{
						listFiles.PushBack(fileName);					
					}

				} while((hnd = ged_findnext(hnd)) != GED_NOTFOUND);
				
				ged_findclose(hnd);
				listFiles.Sort();

				if(listFiles.Count())
				{					
					//Add frames
					for(int i0 = 0; i0 < listFiles.Count(); i0++)
					{
						gedString fullPath = path + listFiles[i0];
						SDL_Surface *surface = LoadSurface(fullPath.c_str()/*, rgba*/);
						if(!surface)
						{
							SetImageError(fullPath.c_str());
							
							spriteResource->RemoveAction(actionName);
							return false;
						}					
						
						int dx = surface->w;
						int dy = surface->h;
						
						actionRes->setFrameWidth(dx);
						actionRes->setFrameHeight(dy);
						
						int hx = (hotSpotX != -1)?hotSpotX:(dx/2);
						int hy = (hotSpotY != -1)?hotSpotY:(dy/2);
						
						int index = actionRes->NumFrames();
						actionRes->AddFrame();
						
						KrPaintInfo info( surface );
						
						//Set transparent color
						if(bTransparency)
						{
							KrPainter painter( surface );
							KrRGBA transRgba;
							
							if(!IsSolidImage(surface, transRgba))
							{
								painter.BreakPixel( 0, 0, &transRgba );	
								info.SetTranparentColor(transRgba);
							}						
						}
						
						KrRle* rle = actionRes->GetFrame( index );			
						
						rle->Create( &info, 0, 0, dx, dy, hx, hy, 0, 0);					
						
						SDL_FreeSurface( surface ); 
					}
				}
				else
				{
					if(imgName.getCharBuf()) SDL_SetError("File not found: %s", imgName.getCharBuf());
				}
			}	
			else
			{
				if(imgName.getCharBuf()) SDL_SetError("File not found: %s", imgName.getCharBuf());
			}
		}
		else if(IMG_isAnimatedGIF(aux.c_str()) && ((nFramesH == 1 && nFramesV == 1) || (nFramesH == -1 && nFramesV == -1)))
		{
			//Load animated gif
			int imageNumber = 1;
			double gifFps;
			SDL_RWops* src;
			SDL_Surface *surface, *surfaceAnt = NULL, *lastUndisposedFrame = NULL;
			
			
			if((src = ged_SDL_RWFromFile(aux.c_str(), "rb" )) != NULL)
			{
				KrAction* actionRes = spriteResource->GetAction( actionName );
				int left, top, disposal = -1, disposalAnt = -1;
				SDL_Color transpColor, transpColorAnt;
				transpColor.unused = 0;
				transpColorAnt = transpColor;
				bool bTo32 = true; //Always true (solve the transparency bug in maksiconActor.gif)
				bool bHasTransparentFrame = false;

				while(1)
				{				
					surface = IMG_LoadAnimatedGIF_RW(src, imageNumber, gifFps, left, top, disposal, transpColor);
					if(!surface) break;	

					int xi = 0, yi = 0;

					if( bTo32 ||
						(transpColorAnt.unused &&
						(transpColorAnt.r != transpColor.r ||
						transpColorAnt.g != transpColor.g ||
						transpColorAnt.b != transpColor.b)) )
					{
						if(transpColor.unused || bHasTransparentFrame)
						{
							//To 32 bits (to deal with different transparent colors between frames)
							
							//Auxiliar surfaces state
							bool	bSurfaceAntEqualSurface = false,
								bConvertSurfaceAnt = false,
								bLastUndisposedFrameEqualSurface = false,
								bLastUndisposedFrameEqualSurfaceAnt = false,
								bConvertLastUndisposedFrame = false;								
							
							if(surfaceAnt == surface) bSurfaceAntEqualSurface = true;
							else if(surfaceAnt && surfaceAnt->format->BitsPerPixel != 32) bConvertSurfaceAnt = true;	
							
							if(lastUndisposedFrame == surface) bLastUndisposedFrameEqualSurface = true;
							else if(lastUndisposedFrame == surfaceAnt) bLastUndisposedFrameEqualSurfaceAnt = true;
							else if(lastUndisposedFrame && lastUndisposedFrame->format->BitsPerPixel != 32) bConvertLastUndisposedFrame = true;	
							
							SDL_Surface *surface32 = ToSurface32(surface);
							if(surface32)
							{
								if(bSurfaceAntEqualSurface) surfaceAnt = surface32;
								else if(bConvertSurfaceAnt)
								{
									surfaceAnt = ToSurface32(surfaceAnt);
								}
								
								if(bLastUndisposedFrameEqualSurface) lastUndisposedFrame = surface32;
								else if(bLastUndisposedFrameEqualSurfaceAnt) lastUndisposedFrame = surfaceAnt;
								else if(bConvertLastUndisposedFrame)
								{
									lastUndisposedFrame = ToSurface32(lastUndisposedFrame);
								}
								
								surface = surface32;
								bTo32 = true;
							}

							bHasTransparentFrame = true;
						}
					}

					transpColorAnt = transpColor;
					
					/*Works with:
						- gif disposal bug.gif (disposal = 0)
						- Gif Error.gif (disposal = 1)

					  See:
						http://ioc.unesco.org/oceanteacher/resourcekit/M3/Formats/Integrated/GIF/gif.89.format.htm
						http://www.webreference.com/content/studio/disposal.html

					iv) Disposal Method - Indicates the way in which the graphic is to
						be treated AFTER being displayed.

						Values :    0 -   No disposal specified. The decoder is
										  not required to take any action.
									1 -   Do not dispose. The graphic is to be left
										  in place.
									2 -   Restore to background color. The area used by the
										  graphic must be restored to the background color.
									3 -   Restore to previous. The decoder is required to
										  restore the area overwritten by the graphic with
										  what was there prior to rendering the graphic.
								  4-7 -    To be defined.
						*/

					if(disposalAnt == 0 || disposalAnt == 1 || disposalAnt == -1)
					{
						if(lastUndisposedFrame && lastUndisposedFrame != surface && lastUndisposedFrame != surfaceAnt)
						{
							SDL_FreeSurface( lastUndisposedFrame ); 
						}

						lastUndisposedFrame = surface;
					}

					
					if((disposalAnt == 0 || disposalAnt == 1) && surfaceAnt)
					{
						//Place surface over last frame
						SDL_Surface *dstsurface = surfaceAnt;

						SDL_Rect srcrect, dstrect;
						
						srcrect.x = 0;
						srcrect.y = 0;
						srcrect.w = surface->w;
						srcrect.h = surface->h;


						dstrect.x = left;
						dstrect.y = top;
						dstrect.w = srcrect.w;
						dstrect.h = srcrect.h;

						SDL_BlitSurface(surface, &srcrect, dstsurface, &dstrect);

						if(surface != lastUndisposedFrame) SDL_FreeSurface( surface );
						surface = dstsurface;
					}
					else if(disposalAnt == 2 && surfaceAnt)
					{
						//Only change the offset (solve the bug in ddouble2.gif)

						xi = -left;
						yi = -top;
					}
					else if(disposalAnt == 3 && lastUndisposedFrame)
					{						
						//Place surface over lastUndisposedFrame
						SDL_Surface *dstsurface = SDL_ConvertSurface(lastUndisposedFrame, lastUndisposedFrame->format, lastUndisposedFrame->flags);

						SDL_Rect srcrect, dstrect;
						
						srcrect.x = 0;
						srcrect.y = 0;
						srcrect.w = surface->w;
						srcrect.h = surface->h;


						dstrect.x = left;
						dstrect.y = top;
						dstrect.w = dstsurface->w;
						dstrect.h = dstsurface->h;

						SDL_BlitSurface(surface, &srcrect, dstsurface, &dstrect);

						if(surface != lastUndisposedFrame) SDL_FreeSurface( surface );
						surface = dstsurface;
					}

					disposalAnt = disposal;


					if(surfaceAnt && surfaceAnt != surface && surfaceAnt != lastUndisposedFrame)
					{
						SDL_FreeSurface( surfaceAnt ); 
					}

					surfaceAnt = surface;


					int dx = surface->w;
					int dy = surface->h;
					
					
					actionRes->setFrameWidth(dx);
					actionRes->setFrameHeight(dy);
					
					int hx = (hotSpotX != -1)?hotSpotX:(dx/2) + xi;
					int hy = (hotSpotY != -1)?hotSpotY:(dy/2) + yi;

									
					int index = actionRes->NumFrames();
					actionRes->AddFrame();
					
					

					SDL_Surface *surfaceWork = ToSupportedSurface(surface, false);
					if(surfaceWork)
					{
						//Create rle
						KrPaintInfo info( surfaceWork );
						
						//Set transparent color
						if(bTransparency)
						{
							if(transpColor.unused || bHasTransparentFrame)
							{
								if(surfaceWork->format->BitsPerPixel != 32)
								{
									//Use the transparency information in gif files
									KrRGBA transRgba;
									
									transRgba.c.red = transpColor.r;
									transRgba.c.green = transpColor.g;
									transRgba.c.blue = transpColor.b;
									
									info.SetTranparentColor(transRgba);	
								}
							}
							else
							{
								//Use the first pixel information
								KrPainter painter( surfaceWork );
								KrRGBA transRgba;
								
								painter.BreakPixel( 0, 0, &transRgba );									
								info.SetTranparentColor(transRgba);
							}

						}
						
						
						KrRle* rle = actionRes->GetFrame( index );					
						rle->Create( &info, 0, 0, dx, dy, hx, hy, 0, 0);
						
						SDL_FreeSurface( surfaceWork ); 
					}

			

					SDL_RWseek(src, 0, SEEK_SET);
					imageNumber++;
				}

				if(lastUndisposedFrame && lastUndisposedFrame != surfaceAnt)
				{
					SDL_FreeSurface( lastUndisposedFrame ); 
				}

				if(surfaceAnt)
				{
					SDL_FreeSurface( surfaceAnt ); 
				}

				
				
				SDL_RWclose(src);

				if(imgName.getCharBuf()) SetImageError(imgName.getCharBuf());

				if(nFramesH == -1 || nFramesV == -1)
				{
					nFramesH = nFramesV = 1;
					actionRes->setnFramesH(nFramesH);
					actionRes->setnFramesV(nFramesV);
				}

				//Set fps
				if(!fps)
				{
					if(gifFps >= 1.0) fps = (int)gifFps;
					else if(gifFps > 0.0) fps = (int)(-gifFps*10000);
					
					actionRes->SetAttr(actionName, imgName, nFramesH,  nFramesV, hotSpotX, hotSpotY, fps);
				}				
			}

			if(imageNumber == 1)
			{
				spriteResource->RemoveAction(actionName);
				return false;
			}
		}
		else
		{
			//Single file			
			SDL_Surface *surface = LoadSurface( aux.c_str()/*, rgba*/ );
			if(!surface)
			{
				if(imgName.getCharBuf()) SetImageError(imgName.getCharBuf());

				spriteResource->RemoveAction(actionName);
				//delete sprite;
				//sprite = NULL;
				return false;
			}


			KrRGBA transColor;
			bool bSolidImage = (IsSolidImage(surface, transColor) && imgName != "maksiconText.bmp");

			if(!bSolidImage)
			{
				if((nFramesH == -1 || nFramesV == -1) && bTransparency)
				{
					//Calculate nFrames
					KrPaintInfo info( surface );
					
					//Set transparent color
					KrPainter painter1( surface );
					KrRGBA transRgba;
					painter1.BreakPixel( 0, 0, &transRgba );	
					info.SetTranparentColor(transRgba);				
					
					KrPainter painter( &info );
					
					painter.CalcNumberOfTiles(nFramesH, nFramesV);
					
					if(nFramesH <= 0 || nFramesV <= 0)
					{
						if(imgName.getCharBuf()) SetImageError(imgName.getCharBuf());
						spriteResource->RemoveAction(actionName);
						return false;
					}
				}
			}
			else
			{
				nFramesH = nFramesV = 1;
			}

			int dx = surface->w / nFramesH;
			int dy = surface->h / nFramesV;

			if(dx <= 0 || dy <= 0)
			{
				spriteResource->RemoveAction(actionName);
				return false;
			}
			
			KrAction* actionRes = spriteResource->GetAction( actionName );
			
			
			actionRes->setnFramesH(nFramesH);
			actionRes->setnFramesV(nFramesV);
			actionRes->setFrameWidth(dx);
			actionRes->setFrameHeight(dy);
			
			int hx = (hotSpotX != -1)?hotSpotX:(dx/2);
			int hy = (hotSpotY != -1)?hotSpotY:(dy/2);
			
			for(int j = 0; j < nFramesV; j++)
			{
				for(int i = 0; i < nFramesH; i++)
				{
					int index = actionRes->NumFrames();
					actionRes->AddFrame();
					
					KrPaintInfo info( surface );

					//Set transparent color
					if(bTransparency)
					{
						KrPainter painter( surface );
						KrRGBA transRgba;

						if(!bSolidImage)
						{
							painter.BreakPixel( 0, 0, &transRgba );	
						}
						else
						{
							transRgba = transColor;
						}

						info.SetTranparentColor(transRgba);
					}

					KrRle* rle = actionRes->GetFrame( index );			
					
					rle->Create( &info, i*dx, j*dy, dx, dy, i*dx + hx, j*dy + hy, 0, 0);
				}
			}
			
			SDL_FreeSurface( surface ); 
		}				
		
		//Replace frame, if any
		KrAction *actionRes = spriteResource->GetAction( actionName );
		if(actionRes)
		{
			int n = actionRes->NumFrames();
			for(int i = 0; i < n; i++)
			{
				ReplaceFrame(actionName, i);

				//////////////////////////////////////
				//save test
				/*EditorDirectory dir("E:\\GAMEDEVELOPMENT\\output\\t");
				KrRle* rle = actionRes->GetFrame(i);
				if(rle)
				{
					gedString name(GetUpdateImageName(actionRes->getimgName()));

					char buf[64];
					sprintf(buf, "%04ld", i);
					name += buf;

					rle->SaveImage(name);
				}*/
				//////////////////////////////////////
			}
		}
	}

	

	
#endif
	
	//Add to sequence
	if(!mySequence->mapSequence[actionName]) 
	{
		Sequence *seq = new Sequence(ANIMATION_SEQUENCE);
		seq->setAnimationName(actionName);
		seq->setSequenceName(actionName);			
		
		mySequence->mapSequence.Add(actionName, seq);
		mySequence->listSequence.PushBack(seq);
	}

	
	actualSequence = actionName;

	//Create sprite
	if(!sprite)
	{
		sprite = new KrSprite( spriteResource, this );
		sprite->SetNodeId((int)sprite);
		
		
		if(!GameControl::Get()->getGameMode() && parent == GameControl::Get()->GetAxis()) 
			baseZDepth++;

		engine->Tree()->AddNode( parent?parent->getImage():0, sprite );		
		sprite->SetZDepth(MAX_ACTOR_DEPTH/2 + baseZDepth);
		SetPos(0, 0);
	}

	return true;
}

void Actor::AddSequence(const gedString& sequenceName, const gedString& animationName, const gedString& frameSequence, int fps)
{
	if(!getSprite()) return;

	Sequence *seq = NULL;
	if(!mySequence->mapSequence[sequenceName]) 
	{
		seq = new Sequence(DEFINED_SEQUENCE);
		seq->setAnimationName(animationName);
		seq->setSequenceName(sequenceName);
		seq->SetFrameSequence(frameSequence, fps);
		
		mySequence->mapSequence.Add(sequenceName, seq);
		mySequence->listSequence.PushBack(seq);

		//scriptVars->animindex = GetAnimationIndex(sequenceName);
	}
	else
	{
		seq = *mySequence->mapSequence[sequenceName];
	}
	
	actualSequence = sequenceName;
	getSprite()->SetAction(animationName);
	
	SetFrame(0);
	if(seq->getType() == ANIMATION_SEQUENCE) scriptVars->nframes = getSprite()->NumFrames();
	else scriptVars->nframes = seq->getNFrames();	

	seq->setFrameRate(fps);

	scriptVars->animindex = GetAnimationIndex(sequenceName);	
}


bool Actor::OnMouseButtonDown(int x, int y, Uint8 button)
{
	if(GameControl::Get()->getGameMode())
	{
		if(action && (eventControl & EVENTMOUSEBUTTONDOWN)) return action->OnMouseButtonDown(this, x, y, button);
	}
	else if(type == REGION_ACTOR || type == REGION_ACTOR_FILLED || type == CANVAS)
	{
		KrVector2T< GlFixed > object;
		getImage()->ScreenToObject( x, y, &object );
		
		
		xClick = x;
		yClick = y;
		
		xObjClick = getImage()->X();
		yObjClick = getImage()->Y();
		
		widthClick = Width();
		heightClick = Height();
		
		x = object.x.ToInt();
		y = object.y.ToInt();
		
		int corner_side = (int)(CORNER_SIDE/GameControl::Get()->GetAxis()->getScale());
		
		if(y <= corner_side && x <= corner_side)
		{		
			corner = TOP_LEFT;
			
			change = this;
			return false;
		}
		else if(y <= corner_side && x >= widthClick - corner_side)
		{		
			corner = TOP_RIGHT;
			
			change = this;
			return false;
		}
		else if(y >=  heightClick - corner_side && x <= corner_side)
		{		
			corner = BOTTOM_LEFT;
			
			change = this;
			return false;
		}
		else if(y >=  heightClick - corner_side && x >= widthClick - corner_side)
		{		
			corner = BOTTOM_RIGHT;
			
			change = this;
			return false;
		}
		
		return true;
	}

	//bool test = button == SDL_BUTTON_RIGHT;
	return false; //return false if this object destroyed here
}

void Actor::OnMouseButtonUp(int x, int y, Uint8 button)
{
	change = NULL;
	if(action && (eventControl & EVENTMOUSEBUTTONUP)) action->OnMouseButtonUp(this, x, y, button);

	if(GameControl::Get()->getDragActor() == this && CanSendRemoteAction())
	{
		//Send reliable position in the drag finish
		GameControl::Get()->SendRemotePosition(this, getX(), getY(), true);
	}
}

void Actor::OnMouseMove(int x, int y)
{
	if(GameControl::Get()->getGameMode())
	{
		if(action && (eventControl & EVENTMOUSEMOVE)) action->OnMouseMove(this, x, y);	
	}
#ifndef STAND_ALONE_GAME
	else 
	{
		if(change == this && (type == REGION_ACTOR || type == REGION_ACTOR_FILLED || type == CANVAS))
		{
			engine->InvalidateRectangle(getImage()->Bounds());
			
			int dx = (int)((xClick - x)/GameControl::Get()->GetAxis()->getScale());
			int dy = (int)((yClick - y)/GameControl::Get()->GetAxis()->getScale());
			
			switch(corner)
			{
			case TOP_LEFT:
				getImage()->SetDimensions(xObjClick - dx, yObjClick - dy, widthClick + dx, heightClick + dy);
				break;
			case TOP_RIGHT:
				getImage()->SetDimensions(xObjClick, yObjClick - dy, widthClick - dx, heightClick + dy);
				break;
			case BOTTOM_LEFT:
				getImage()->SetDimensions(xObjClick - dx, yObjClick, widthClick + dx, heightClick - dy);
				break;
			case BOTTOM_RIGHT:
				getImage()->SetDimensions(xObjClick, yObjClick, widthClick - dx, heightClick - dy);
				if(type == CANVAS) getCanvas()->Invalidate();
				break;
			}
			
			engine->InvalidateRectangle(getImage()->Bounds());

			if(type == CANVAS)
			{
				scriptVars->width = Width();
				scriptVars->height = Height();
			}

			UpdateActorSize();
		}
		/*else
		{
			KrVector2T< GlFixed > object;
			getImage()->ScreenToObject( x, y, &object, 0 );		
			
			x = object.x.ToInt();
			y = object.y.ToInt();
			
			int corner_side = (int)(CORNER_SIDE/GameControl::Get()->GetAxis()->getScale());
			
			if(y <= corner_side && x <= corner_side)
				cursor.imgName	= "makscursorResize1.bmp";
			else if(y <= corner_side && x >= widthClick - corner_side)
				cursor.imgName	= "makscursorResize2.bmp";
			else if(y >=  heightClick - corner_side && x <= corner_side)
				cursor.imgName	= "makscursorResize2.bmp";
			else if(y >=  heightClick - corner_side && x >= widthClick - corner_side)
				cursor.imgName	= "makscursorResize1.bmp";
			else
				cursor.imgName	= "makscursorMao.bmp";
		}*/
	}
#endif
}

void Actor::OnAnimation()
{
	if(flags.IsSet(FLAG_PENDINGSETTEXT) && IsVisible())
	{
		SetText(scriptVars->text);		
	}

	if(action && (eventControl & EVENTANIMATION)) action->OnAnimation(this);
}

void Actor::OnMoveFinish()
{
	if(flags.IsSet(FLAG_MOVETO))
	{
		//Reset the move variables
		scriptVars->xvelocity = scriptVars->yvelocity = 0.0;		
		flags.Clear(FLAG_MOVETO);

		if(!pathFinder)
		{
			//Don't set final stop when use pathfinder
			SetPos(xstop, ystop);
		}

		delete pathFinder;
		pathX = pathY = pathFinder = NULL;
		
		//HaveMoveFinish only for otimization
		if(action && (eventControl & EVENTMOVEFINISH) && action->HaveMoveFinish(true)) action->OnMoveFinish(this);
	}
}

void Actor::OnAnimationFinish()
{
	if(action && (eventControl & EVENTANIMATIONFINISH)) action->OnAnimationFinish(this);
}

void Actor::OnPathFinish()
{
	if(flags.IsSet(FLAG_MOVETO))
	{
		//Is a path finder move		
		OnMoveFinish();
	}
	else
	{
		if(action && (eventControl & EVENTPATHFINISH)) action->OnPathFinish(this);
	}
}

void Actor::OnMouseEnter(int x, int y)
{
	if(action && (eventControl & EVENTMOUSEENTER) && flags.IsSet(FLAG_RUNNING)) action->OnMouseEnter(this, x, y);
}

void Actor::OnMouseLeave(int x, int y)
{
	if(action && (eventControl & EVENTMOUSELEAVE) && flags.IsSet(FLAG_RUNNING)) action->OnMouseLeave(this, x, y);
}

void Actor::OnKeyDown(SDLKey key, int repeatLoopCount)
{
	if(action && (eventControl & EVENTKEYDOWN) && flags.IsSet(FLAG_RUNNING)) action->OnKeyDown(this, key, repeatLoopCount);
}

void Actor::OnKeyUp(SDLKey key)
{
	if(action && (eventControl & EVENTKEYUP) && flags.IsSet(FLAG_RUNNING)) action->OnKeyUp(this, key);
}



const stCursor *Actor::OnGetCursor()
{
	if(type != REGION_ACTOR /*&& type != REGION_ACTOR_FILLED*/)
	{
		if(cursor.nFramesH)
			return &cursor;
		else
			return NULL;	
	}
	else
	{
		if(GameControl::Get()->getGameMode()) 
			return NULL;
		else
			return &cursor;
	}
}

Uint32 Actor::OnTimer(SDL_TimerID timerID)
{	
	if(action && flags.IsSet(FLAG_RUNNING) && (!flags.IsSet(FLAG_NET_OPTIMIZED_PRECISION) || CanExecuteActionLocally()))
	{
		if(eventControl & EVENTTIMER)
			return action->OnTimer(this, timerID);
		else
		{
			stTimerEvent *p = action->mapTimerIdName[timerID];
			if(p)
			{
				//Time is disable don't destroy
				return p->miliseconds;
			}
			
		}
	}

	return 0;	//return 0 if this object destroyed here
}

bool Actor::RemoveTimer(stTimer *timer)
{
	bool res = mapTimer.Remove(timer);
	
	if(timer && globalMapTimer[timer])
	{
		delete timer;
	}

	return res;
}

SDL_TimerID Actor::SetTimer(Uint32 interval, int count, int _type, Uint32 minRandomInterval)
{
	stTimer *timer = new stTimer;
	timer->actor = this;

	timer->interval = interval;
	timer->count = count;
	timer->type = _type;
	timer->minRandomInterval = minRandomInterval;

	if(timer->type == RANDOM_TIMER)
	{
		interval = (int)rand(timer->interval - timer->minRandomInterval) + timer->minRandomInterval;		
	}

	timer->timerID = SDL_AddTimer(interval, ActorTimerCallback, timer);
	mapTimer.Add(timer, 1);

	lastCreatedTimer = timer;

	//GLOUTPUT("SetTimer: %X, %X, %X\n", (long)this, (long)timer, (long)timer->timerID);
	return timer->timerID;
}

void Actor::OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message)
{
	if(action && (eventControl & EVENTMESSAGE)) action->OnMessage(this, fromActor, messageId, message);
}

void Actor::PostMessage(gedString actorName, Uint32 messageId, Uint32 message)
{
	GameControl::Get()->PostMessage(this, actorName, messageId, message);
}

void Actor::PostMessage(Actor *actorTo, Uint32 messageId, Uint32 message)
{
	if(messageId == message && message == DELETE_ME)
	{
		actorTo->setDeleteConfirmation();
	}

	GameControl::Get()->PostMessage(this, actorTo, messageId, message);
}

void Actor::OnCollision(Actor *collideActor)
{
	if(action && (eventControl & EVENTCOLLISION) && flags.IsSet(FLAG_RUNNING)) action->OnCollision(this, collideActor);
}

void Actor::OnCollisionFinish(Actor *collideActor)
{
	if(action && (eventControl & EVENTCOLLISIONFINISH)) action->OnCollisionFinish(this, collideActor);
}

void Actor::OnActivationEvent(Actor *fromActor)
{
	if(action && (eventControl & EVENTACTIVATIONEVENT)) action->OnActivationEvent(this, fromActor);
}


KrSprite *Actor::getSprite() 
{
	return sprite;
}

KrCanvas *Actor::getCanvas()
{
	return canvas;
}

KrImage *Actor::getImage() 
{
	if(type == SPRITE && sprite)
	{
		return (KrImage *)sprite;
	}
	else if(type == CANVAS && canvas)
	{
		return (KrImage *)canvas;
	}
	else if(slowCanvas)
	{
		return (KrImage *)slowCanvas;
	}
	else
	{
		static KrNullImage nullImage;
		return &nullImage;
	}
}

KrCanvasResource *Actor::getCanvasResource()
{
	return canvasResource;
}

void Actor::SetTransparency(double trans)
{
	KrImage *pImage = getImage();
	if(!pImage) return;
	
	
	
	if(trans > 1.0)
		trans = 1.0;
	
	if(trans < 0.0)
		trans = 0.0;
	
	KrColorTransform color(pImage->CTransform());
	color.SetAlpha(255 - 255.0*trans);
	pImage->SetColor(color);
	
	scriptVars->transp = trans;
}


double Actor::GetTransparency()
{
	KrImage *pImage = getImage();
	if(!pImage) return 0.0;

	return (255 - pImage->CTransform().Alpha())/255.0;
}

int Actor::Width()
{
	KrImage *pImage = getImage();
	

	if(textActor)
		return textActor->Width();
	else if((type == SLOW_CANVAS || type == REGION_ACTOR || type == REGION_ACTOR_FILLED) && pImage)
		return ((SlowCanvas *)pImage)->Width();
	else if(type == CANVAS && pImage)
		return ((KrCanvas *)pImage)->Width();
	else if(tile)
		return tile->Width();
	

	if(pImage)
	{
		KrRect rect;
		pImage->QueryBoundingBox(&rect);
		
#ifdef USE_WALK
		return rect.Width()*pImage->XScale().ToDouble();
#else
		return rect.Width();//*getImage()->XScale().ToDouble(); //Don't need with Walk1() optimization
#endif
	}

	return 0;
}



int Actor::Height()
{
	KrImage *pImage = getImage();


	if(textActor)
		return textActor->Height();
	else if((type == SLOW_CANVAS || type == REGION_ACTOR || type == REGION_ACTOR_FILLED) && pImage)
		return ((SlowCanvas *)pImage)->Height();
	else if(type == CANVAS && pImage)
		return ((KrCanvas *)pImage)->Height();
	else if(tile)
		return tile->Height();

	if(pImage)
	{
		KrRect rect;
		pImage->QueryBoundingBox(&rect);
		
#ifdef USE_WALK
		return rect.Height()*pImage->YScale().ToDouble();
#else
		return rect.Height();//*getImage()->YScale().ToDouble(); //Don't need with Walk1() optimization
#endif
	}

	return 0;
}

int Actor::Top()
{
	KrImage *pImage = getImage();

	return pImage?pImage->Y():0;
}

int Actor::Down()
{
	KrImage *pImage = getImage();
	if(pImage)
	{
		KrRect rect;
		pImage->QueryBoundingBox(&rect);
		
#ifdef USE_WALK
		return pImage->Y() + rect.Height()*pImage->YScale().ToDouble();
#else
		return pImage->Y() + rect.Height();//*getImage()->YScale().ToDouble(); //Don't need with Walk1() optimization
#endif
	}

	return 0;
}

int Actor::Left()
{
	KrImage *pImage = getImage();

	return pImage?pImage->X():0;
}

int Actor::Right()
{
	KrImage *pImage = getImage();
	if(pImage)
	{
		KrRect rect;
		pImage->QueryBoundingBox(&rect);
		
#ifdef USE_WALK
		return pImage->X() + rect.Width()*pImage->XScale().ToDouble(); 
#else
		return pImage->X() + rect.Width();//*getImage()->XScale().ToDouble(); //Don't need with Walk1() optimization
#endif
	}

	return 0;
}



void Actor::SetModal()
{
	GameControl::Get()->SetModal(this);
}



void Actor::RemoveAnimation(gedString actionName)
{
	KrSprite *pSprite = getSprite();
	if(type != SPRITE || !pSprite)
		return;



	Sequence **p = mySequence->mapSequence[actionName];
	if(p)
	{		
		//Solve the crash when select a image font and select other
		//And when remove a animation
		//Don't change with SetAnimation
		//Need this only with use Walk1() optimization
		pSprite->SetAction(ACTOR_GED_ICON); //SetAnimation(ACTOR_GED_ICON); 

		//Remove
		Sequence *seq = *p;
		if(spriteResource && seq->getType() == ANIMATION_SEQUENCE)
		{
			spriteResource->RemoveAction(actionName);
		}

		mySequence->mapSequence.Remove(actionName);
		mySequence->listSequence.FindAndDelete(seq);
		delete seq;			
	}	
}

int Actor::NumAnimations()
{
	if(type != SPRITE || !mySequence)
		return 0;

	return mySequence->mapSequence.size() - 2; //Exclude icon and transparent animation
}

gedString Actor::AnimationName(int i, Sequence **pSeq)
{
	int nAnimations = mySequence->mapSequence.size();

	if(type != SPRITE || i < 0 || i > nAnimations/*spriteResource->NumActions()*/)
	{
		if(pSeq) *pSeq = NULL;
		return "";
	}

	//return spriteResource->GetActionByIndex(i)->Name();

	int n = 0;
	ListSequenceIterator it(mySequence->listSequence);
	for(it.Begin(); !it.Done(); it.Next())
	{
		if(n == i)
		{
			Sequence *seq = it.Current();

			if(pSeq) *pSeq = seq;

			return seq->getSequenceName();
		}
		n++;	
	}

	if(pSeq) *pSeq = NULL;
	return "";	
}

Sequence *Actor::GetSequence(const gedString& sequenceName)
{
	Sequence **seq = mySequence->mapSequence[sequenceName];
	if(seq)
	{
		return *seq;
	}

	return NULL;
}

void Actor::UpdateAnimIndex()
{
	//Speed up the GetAnimationIndex function
	//Solve???: 
	//		Low frame rate on ppc's with activation regions
	//		Game: Apalia
	//		Reported by:  jimmynewguy, omniarts
	//		http://game-editor.com/forum/ucp.php?i=pm&mode=view&f=0&p=13087
	
	int i = 0;

	ListSequenceIterator it(mySequence->listSequence);
	for(it.Begin(); !it.Done(); it.Next())
	{
		Sequence *seq = it.Current();
		if(i >= 2)
		{
			seq->setAnimIndex(i - 2);
		}
		i++;
	}
}

int Actor::GetAnimationIndex(const gedString& name)
{
#ifdef STAND_ALONE_GAME
	//Speed up the GetAnimationIndex function
	//Solve???: 
	//		Low frame rate on ppc's with activation regions
	//		Game: Apalia
	//		Reported by:  jimmynewguy, omniarts
	//		http://game-editor.com/forum/ucp.php?i=pm&mode=view&f=0&p=13087

	Sequence **pSeq = mySequence->mapSequence.FindString(name.getCharBuf());
	if(pSeq && *pSeq && (*pSeq)->getAnimIndex() != -1)
	{
		return (*pSeq)->getAnimIndex();
	}
#endif


	if(getSprite())
	{
		int i = 0;
		
		ListSequenceIterator it(mySequence->listSequence);
		for(it.Begin(); !it.Done(); it.Next())
		{
			Sequence *seq = it.Current();
			if(name == seq->getSequenceName() && i >= 2)
			{

#ifdef STAND_ALONE_GAME
				seq->setAnimIndex(i - 2);
#endif

				return i - 2;
			}
			i++;
		}
	}

	return -1;
}

int Actor::SetParent(const gedString& parentName)
{
	//If the parentName don't exists, remove the parent
	//Solve the problem if set "no parent" or "(none)"

	/*if(parentName ==  NO_PARENT)
	{
		RemoveParent();
		return PARENT_OK;
	}*/

	//Will get edit actors by use with the interface
	//So, don't use in game mode
	Actor *parentActor = GameControl::Get()->GetActor(parentName);

	/*if(!parentActor)
		return PARENT_NOT_FOUND;*/

	return SetParent(parentActor);
}

int Actor::SetParent(Actor *parentActor)
{
	//If the parentName don't exists, remove the parent
	//Solve the problem if set "no parent" or "(none)"

	/*if(!parentActor)
		return PARENT_NOT_FOUND;*/

	if(!parentActor)
	{
		RemoveParent();
		return PARENT_NOT_FOUND;
	}

	if(parent == parentActor)
		return PARENT_OK;

	KrImage *pImage = getImage(), *pParentImage = NULL;
	if(!pImage)
		return PARENT_OK;

	//Screen coordinates
	//Don't use int and ToInt (solve the Bug "Shift when parenting an object")
	GlFixed x = pImage->CompositeXForm().x,
			y = pImage->CompositeXForm().y;

	pParentImage = parentActor->getImage();
	if(!pParentImage)
		return PARENT_OK;

	if(!engine->Tree()->SetParent(pParentImage, pImage))
		return CYCLICAL_PARENT;

	//Screen to parent coordinates
	KrVector2T< GlFixed > object;
	pParentImage->ScreenToObject(x, y, &object);
	SetPos(object.x.ToDouble(), object.y.ToDouble());
	SetPathIni(object.x.ToDouble(), object.y.ToDouble());

	//Make anterior positions relative to new parent
	//Or set new values relative to new parent
	anteriorXPos = getX();
	anteriorYPos = getY();

	//Update position (solve changeParent.ged)
	scriptVarsAnt->x = scriptVars->x;	
	scriptVarsAnt->y = scriptVars->y;	
	
	parent = parentActor;

	return PARENT_OK;
}



void Actor::RemoveParent()
{
	if(parent != GameControl::Get()->GetAxis())
	{
		KrImage *pImage = getImage(), *pAxisImage = GameControl::Get()->GetAxis()->getImage();


		if(GameControl::Get()->getGameMode() && pImage && pAxisImage)
		{
			//Adjust position relative to new parent
			//Don't use ToInt (solve the shift bug in pareng.ged)
			
			KrVector2T< GlFixed > object;									
			pAxisImage->ScreenToObject(pImage->CompositeXForm().x, pImage->CompositeXForm().y, &object);		
			
			
			SetPos(object.x.ToDouble(), object.y.ToDouble());
			
			//Make anterior positions relative to new parent
			//Or set new values relative to new parent
			anteriorXPos = getX();
			anteriorYPos = getY();
		}
		
		engine->Tree()->SetParent(pAxisImage, pImage);
		parent = GameControl::Get()->GetAxis();
	}
}

void OnSlowCanvasPaint(void *actor, KrPaintInfo *paintInfo, SDL_Surface* surface, KrRect &rect, int openGLZ)
{
	((Actor *)actor)->OnSlowCanvasPaint(paintInfo, paintInfo->surface, rect, openGLZ);				
}

void Actor::OnSlowCanvasPaint(KrPaintInfo *paintInfo, SDL_Surface* surface, KrRect &rect, int openGLZ)
{
#ifndef STAND_ALONE_GAME
	if(!GameControl::Get()->getGameMode() && (flags.IsSet(FLAG_VIEW) || type == REGION_ACTOR || type == REGION_ACTOR_FILLED || (type == CANVAS && flags.IsSet(FLAG_EDIT))))
	{
		KrPainter painter(paintInfo);

		KrRect robj = getImage()->Bounds(), r;
		
		//Top
		r = robj;
		r.ymax = r.ymin;		
		if(r.Intersect(rect))
		{
			r.DoIntersection(rect);			
			painter.DrawHLine( r.xmin, r.ymin, r.Width(), red, green, blue, openGLZ );			
		}

		//Down
		r = robj;
		r.ymin = r.ymax;		
		if(r.Intersect(rect))
		{
			r.DoIntersection(rect);			
			painter.DrawHLine( r.xmin, r.ymin, r.Width(), red, green, blue, openGLZ );			
		}

		//Left
		r = robj;
		r.xmax = r.xmin;		
		if(r.Intersect(rect))
		{
			r.DoIntersection(rect);			
			painter.DrawVLine( r.xmin, r.ymin, r.Height(), red, green, blue, openGLZ );			
		}

		//Right
		r = robj;
		r.xmin = r.xmax;		
		if(r.Intersect(rect))
		{
			r.DoIntersection(rect);			
			painter.DrawVLine( r.xmin, r.ymin, r.Height(), red, green, blue, openGLZ );			
		}
	} 
#endif //#ifndef STAND_ALONE_GAME
}


void Actor::SetPath(const gedString& pathName, int axis)
{
	KrImage *pImage = getImage();

	if(flags.IsSet(FLAG_NET_OPTIMIZED_PRECISION) && !CanExecuteActionLocally()) return;

	if(pImage)
	{
		switch(axis)
		{
		case BOTH_AXIS:
			{
				pathX = pathY = GameControl::Get()->GetPath(pathName);
			}
			break;
		case X_AXIS:
			{
				pathX = GameControl::Get()->GetPath(pathName);
				pathY = NULL;
			}
			break;
		case Y_AXIS:
			{
				pathY = GameControl::Get()->GetPath(pathName);
				pathX = NULL;
			}
			break;
		}	
		
		flags.Set(FLAG_RANDOMPATH, pathName == RANDOM_PATH);
		
		SetPathIni(pImage->X(), pImage->Y());

		delete pathFinder;
		pathFinder = NULL;
		flags.Clear(FLAG_MOVETO);
	}	
}


void Actor::SetPathIni(int x, int y)
{
	pathIniX = x;
	pathIniY = y;
	scriptVars->pathxpos = scriptVars->pathypos = 0;
}

void Actor::setFollowMouseAxis(int axis) 
{

	followMouseAxis = axis;
}


void Actor::SetAnimation(const gedString &name, bool bKeepActualFrame)
{
	SetAnimation(name.getCharBuf(), bKeepActualFrame);
}

void Actor::SetAnimation(const char *name, bool bKeepActualFrame)
{
	Sequence **p = mySequence->mapSequence[name];
	KrSprite *pSprite = getSprite();

	if(!pSprite)
	{
		//Add the default icon (solve the bug "Don't create actors with animation sequences")
		AddAnimation(ACTOR_GED_ICON, "maksiconActor.gif", 0, 0);
		pSprite = getSprite();
	}

	if(pSprite && p)
	{
		Sequence *seq = *p;

		int _frameAnt = scriptVars->animpos;
		KrRect antRect = Bounds(), rectActor;

		
/*#ifdef DEBUG
		if(GameControl::Get()->getGameMode()) GLOUTPUT( "SetAnimation: %s,  %s\n",  getCloneName(), name);
#endif*/

		actualSequence = name;
		pSprite->SetAction(seq->getAnimationName());		
		
		//Solve the bug when add a animation, remove and the area around icon is not redrawed
		pSprite->CalcTransform();
		rectActor = Bounds();		
		
		engine->InvalidateRectangle(antRect);
		if(rectActor != antRect) 
		{
			//Solve the bug when change small iamge to a big image and the area around the big image is not redrawed
			engine->InvalidateRectangle(rectActor);
		}


		/*if(!bKeepActualFrame || pSprite->Frame() < 0)
		{
			if(seq->getType() == ANIMATION_SEQUENCE) SetFrame(0);
			else SetFrame(seq->GetFrame(0));
		}
		else
		{
			if(seq->getType() == ANIMATION_SEQUENCE) SetFrame(_frameAnt);
			else SetFrame(seq->GetFrame(_frameAnt));
		}*/
		
		if(!bKeepActualFrame || pSprite->Frame() < 0)
		{			
			//solve the bug "Doesn't show the first frame after change the animation" (firstFrame.ged)
			int start = pSprite->NumFrames()-1;

			if(framesToAnim == DISABLE_ANIMATION || !GameControl::Get()->getGameMode()) start = 0; //avoid the bug in anim_test1.ged (doesn't show the first frame in a stopped animation and afer load)

			if(_frameAnt == 0)
			{
				if(seq->getType() == ANIMATION_SEQUENCE) SetFrame(getForwardAnimation()?start:0); 
				else SetFrame(getForwardAnimation()?seq->getNFrames()-1:0);
			}
			else
			{
				if(seq->getType() == ANIMATION_SEQUENCE) SetFrame(getForwardAnimation()?0:start); 
				else SetFrame(getForwardAnimation()?0:seq->getNFrames()-1);
			}
		}
		else
		{
			SetFrame(_frameAnt); 			
		}

		scriptVars->animindex = GetAnimationIndex(actualSequence);

		if(seq->getType() == ANIMATION_SEQUENCE) scriptVars->nframes = getSprite()->NumFrames();		
		else scriptVars->nframes = seq->getNFrames();


		scriptVars->width = Width();
		scriptVars->height = Height();

#ifndef STAND_ALONE_GAME
		if(!GameControl::Get()->getGameMode()) UpdateActorAnim(this);		
#endif
	}
}



Sequence *Actor::GetSequence()
{
	Sequence **seq = mySequence->mapSequence[actualSequence];
	if(getSprite() && seq)
	{
		return *seq;
	}

	return NULL;
}

void Actor::SetCursor(const gedString& imgName, int nFramesH, int nFramesV, int hotSpotX, int hotSpotY)
{


	cursor.imgName = imgName;
	cursor.nFramesH = nFramesH;
	cursor.nFramesV = nFramesV;
	cursor.hotSpotX = hotSpotX;
	cursor.hotSpotY = hotSpotY;
	cursor.bAnimate = true;

}

void Actor::setAction(Action *_action) 
{
	if(action != _action)
	{
		action = _action; 
		action->AttachAction();
	}
}


void Actor::SetAnimationState(int state)
{
	if(flags.IsSet(FLAG_NET_OPTIMIZED_PRECISION) && !CanExecuteActionLocally()) return;

	switch(state)
	{
	case FORWARD:
		{
			framesToAnim = ENABLE_ANIMATION;
			flags.Set(FLAG_FORWARDANIMATION);
		}
		break;
	case BACKWARD:
		{
			framesToAnim = ENABLE_ANIMATION;
			flags.Clear(FLAG_FORWARDANIMATION);
		}
		break;
	case STOPPED:
		{
			framesToAnim = DISABLE_ANIMATION;
		}
		break;
	case NO_CHANGE:
		//Do nothing
		break;
	}

}

Actor * Actor::NewActor(bool bClone, bool bSetPos, int xPos, int yPos, bool bCacheActor, bool bSetCloneName)
{
	KrImage *pImage = getImage(), *pNewImage = NULL;
	if(!pImage || !parent) return NULL;

	if(!bCacheActor && GameControl::Get()->getStandAloneMode())
	{
		//if(engine->Tree()->FindNodeById((int)pImage)) //FindNodeById: solve the crash with cache actors
		{
			setRunning();
			pImage->SetVisible(true);
			flags.Clear(FLAG_EDIT);
			return this;
		}
	}

	Actor *newActor;
	int width = 0, height = 0;

	if(bCacheActor)
	{
		if(type == REGION_ACTOR || type == REGION_ACTOR_FILLED || type == CANVAS || isView())
		{
			//Don't cache regions
			return NULL;
		}
	}
	else
	{
		width = Width();
		height = Height();
	}

	gedString sParentName;//(parent->getCloneIndex()?parent->getCloneName():parent->getActorName());
	
	if(mapActors.FindString(parent->getActorName())->Count() > 1)
	{
		sParentName = parent->getCloneName(); //solve the issue 119
	}
	else
	{
		sParentName = parent->getActorName();
	}
	
	if(bClone)
	{
#ifdef ACTOR_USES_VIRTUAL_FUNCTIONS
		if(bCacheActor) 
		{			
			newActor = new ActorEdit(scriptVars->name, (sParentName == AXIS_NAME)?NULL:GameControl::Get()->GetActor(sParentName, true), false, type, width, height);
		}
		else 
		{
			newActor = new ActorEdit(scriptVars->name, parent, false, /*(type != REGION_ACTOR && type != REGION_ACTOR_FILLED)?type:SPRITE*/type, width, height); //Can clone regions
		}
#else
		if(bCacheActor) 
		{			
			newActor = new Actor/*Edit*/(scriptVars->name, (sParentName == AXIS_NAME)?NULL:GameControl::Get()->GetActor(sParentName, true), /*false,*/ type, width, height);
		}
		else 
		{
			newActor = new Actor/*Edit*/(scriptVars->name, parent, /*false,*/ /*(type != REGION_ACTOR && type != REGION_ACTOR_FILLED)?type:SPRITE*/type, width, height); //Can clone regions
		}

		newActor->InitEdit();
#endif

		Action *copyAction = getAction();
		if(copyAction) newActor->setAction(copyAction);
	}
	else
	{
		newActor = new Actor(scriptVars->name, GameControl::Get()->GetActor(sParentName, true), type, Width(), Height(), false, getAction(), false, bSetCloneName?scriptVars->cloneindex:-1);		
		//newActor->setRunning(); //Why here?
	}
	
	newActor->AddAnimation(ACTOR_GED_ICON, "maksiconActor.gif", 0, 0);

	if(textActor)
	{
		Text *text = newActor->AddText(textActor->GetText(), 0, 0, ALIGN_AUTOMATIC, 255, 255, 255, 
			textActor->getFontName(), 
			textActor->getiniASCIIChar(), 
			textActor->getnChars(),
				
			textActor->IsEditable(),
			textActor->HasBorder(),
			textActor->HasBackground(),
			textActor->HasGetFocus(),
			textActor->getBorderColor(),
			textActor->getCursorColor(),
			textActor->getBackGroundColor());

		newActor->nZeros = nZeros; //Solve the 00 bug when set a big text and set a number after
		newActor->setTextActor(text);		
	}

	if(NumAnimations())
	{		
		newActor->SetAnimation(GetAnimation(), true);
	}	

	
	int x = 0, y = 0;
	
	if(!bCacheActor)
	{
		x = pImage->X();
		y = pImage->Y();
		
		if(bClone)
		{
			if(!bSetPos)
			{
				x += 10;
				y += 10;
			}
			else
			{
				x = xPos;
				y = yPos;
			}
		}
	}


	newActor->SetPos(x, y);

	if(flags.IsSet(FLAG_RANDOMPATH))
	{
		newActor->SetPath(RANDOM_PATH, BOTH_AXIS);
	}
	else
	{
		newActor->SetPathIni(x, y);
		newActor->SetPathX(getPathX());			
		newActor->SetPathY(getPathY());	
	}

	pNewImage = newActor->getImage();
	if(!pNewImage) return NULL;

	if(newActor->getType() == REGION_ACTOR || newActor->getType() == REGION_ACTOR_FILLED)
		pNewImage->SetDimensions(newActor->getPathIniX(), newActor->getPathIniY(), width, height);


	if(!bClone || bCacheActor)
	{
		//Use the same code in both versions
		//Solve the bug "For some reason the code on the AI spawn sometimes causes the player character to turn red or yellow" in ALPHA_1_1_4a.ged

//#ifdef STAND_ALONE_GAME
		//Get from original loaded actor
		//Solve the bug "don't show myPlane smoke in 1945.ged in stand alone game"
		pNewImage->SetZDepth(getLoadedZDepth());
		newActor->SetTransparency(getLoadedTransparency());
		newActor->SetColor(getLoadedColor().c.red, getLoadedColor().c.green, getLoadedColor().c.blue);

		//Copy original loaded values
		newActor->setLoadedZDepth(getLoadedZDepth());
		newActor->setLoadedTransparency(getLoadedTransparency());
		newActor->setLoadedColor(getLoadedColor());
/*#else
		pNewImage->SetZDepth(getImage()->ZDepth());
		newActor->SetTransparency(GetTransparency());
		newActor->SetColor(getScriptVars()->r, getScriptVars()->g, getScriptVars()->b);
#endif*/
	}
	else
	{
		pNewImage->SetZDepth(pImage->ZDepth() + baseZDepth);
		newActor->SetTransparency(GetTransparency());
		newActor->SetColor(getScriptVars()->r, getScriptVars()->g, getScriptVars()->b);
	}

	newActor->flags.Set(FLAG_NETACTOR, flags.IsSet(FLAG_NETACTOR));
	newActor->flags.Set(FLAG_NET_OPTIMIZED_PRECISION, flags.IsSet(FLAG_NET_OPTIMIZED_PRECISION));

	if(tile)
	{
		newActor->tile = new Tile(newActor);	
		newActor->tile->Equal(tile);
	}

	if(getInfinite() != NO_INFINITE) newActor->SetInfinite(getInfinite());

	if(!bClone)
	{
		//Copy clone index
		newActor->scriptVars->cloneindex = scriptVars->cloneindex;
		sprintf(newActor->scriptVars->clonename, "%s.%ld", newActor->scriptVars->name, newActor->scriptVars->cloneindex);

		//Copy total connected actors
		newActor->totalConnectedActors = totalConnectedActors;

		if(GameControl::Get()->getGameMode() && flags.IsSet(FLAG_CREATEATSTARTUP))
		{
			//Update max clone index
			//Need to update the first clone name (Issue 85)
			int *maxIndex = mapActorsMaxClone.FindString(newActor->scriptVars->name);
			if(maxIndex) 
			{
				if(*maxIndex < newActor->scriptVars->cloneindex)
					*maxIndex = newActor->scriptVars->cloneindex;
			}
			else
			{
				mapActorsMaxClone.Add(newActor->scriptVars->name, newActor->scriptVars->cloneindex);				
			}
		}

		newActor->setRunning();
	}

	if(!bCacheActor && newActor->getCallOnCreate() && newActor->getAction() && newActor->getRunning()) //Avoid call on create in editor mode
		newActor->getAction()->OnCreate(newActor);

	//To clone array
	newActor->flags.Set(FLAG_LOCKACTOR, flags.IsSet(FLAG_LOCKACTOR));
	

#ifndef STAND_ALONE_GAME
	if(!GameControl::Get()->getGameMode())
	{
		newActor->description = description;
	}
#endif
	
	if(bCacheActor)
	{
		pNewImage->SetVisible(false);
	}

	return newActor;
}



Actor *Actor::Load(const char *name, bool bExplicitlyCreateActor)
{
	//Load actor from disk by clone name
	
	//Get actor location
	SDL_RWops *src = GameControl::Get()->GetGameFilePointer();	
	if(!src) return NULL;

	Uint32 index = GameControl::Get()->GetActorIndex(name, bExplicitlyCreateActor);	
	if(!index) return NULL;

	//Move file pointer
	Uint32 pos = SDL_RWtell( src ); //Load actual position
	SDL_RWseek( src, index, SEEK_SET );
	
	//Load
	Actor *newActor = NewActor(src, false, GameControl::Get()->GetGameFileVersion());
	
	//Restore file pointer
	SDL_RWseek( src, pos, SEEK_SET );

	
	if(GameControl::Get()->getStandAloneMode())
	{
		KrImage *pImage = newActor->getImage();

		if(!newActor->getCreateAtStartup() && pImage)
			pImage->SetVisible(false);		
		else if(
			!Action::getInCreateActorAction() //If in create actor action the setRunning will be called in the create function. //Don't allow load. Solve the msrp.ged bug (create only the tank shadow)
			)
		{
			newActor->setRunning();
		}
	}

	return newActor;
}

Actor *Actor::NewActor(SDL_RWops *src, bool bMergeGames, Uint32 version)
{ 
#ifndef STAND_ALONE_GAME
	WaitCursor wait;
#endif

#ifdef ACTOR_USES_VIRTUAL_FUNCTIONS
	ActorEdit *newActor = NULL;
#else
	Actor *newActor = NULL;
#endif
	
	gedString name;
	name.Read(src);

	gedString parentName;
	parentName.Read(src);

	
	bool bIsView = (name == VIEW_NAME);
	if(bMergeGames && bIsView) 
	{
#ifdef ACTOR_USES_VIRTUAL_FUNCTIONS
		newActor = (ActorEdit *)GameControl::Get()->GetActor(VIEW_NAME);
#else
		newActor = GameControl::Get()->GetActor(VIEW_NAME);
#endif		
	}

	
	if(loadingActors < 0) loadingActors = 0; //Must never happen
	if(version > 6) loadingActors++;

	int actorType = SPRITE, regionWidth = 0, regionHeight = 0;
	if(version > 13) 
	{
		//Read type
		actorType = GameControl::Read16(src);
		
		if(actorType == REGION_ACTOR || actorType == REGION_ACTOR_FILLED || actorType == CANVAS)
		{
			//Load region defination
			regionWidth = GameControl::Read32( src ), 
			regionHeight = GameControl::Read32( src );
		}
	}

#ifdef ACTOR_USES_VIRTUAL_FUNCTIONS
	if(!newActor) newActor = new ActorEdit(name, (parentName == AXIS_NAME)?NULL:GameControl::Get()->GetActor(parentName), bIsView, (ActorType)actorType, regionWidth, regionHeight);
#else
	if(!newActor) newActor = new Actor(name, (parentName == AXIS_NAME)?NULL:GameControl::Get()->GetActor(parentName), bIsView?VIEW_ACTOR:(ActorType)actorType, regionWidth, regionHeight);
	newActor->InitEdit();
#endif
	
	

	if(actorType == CANVAS && version > 21) 
	{
		//Read data
		newActor->getCanvas()->Load(src);
	}

	
	newActor->Load(src, version);
	newActor->SetPos(newActor->getPathIniX(), newActor->getPathIniY());


	if(!newActor->getCreateAtStartup())
	{
		//Don't Create at Startup actors will have the parents set
		//in a Create Actor action
		newActor->RemoveParent();
	}

	
	//Set actions
	Action::Call(newActor, false);	

	//Fix animation finish
	if(version < 4 && newActor->getAction())
	{
		newActor->getAction()->FixAnimationFinish(newActor);
	}

	if(actorType == REGION_ACTOR || actorType == REGION_ACTOR_FILLED)
	{
		//Set position
		KrImage *pImage = newActor->getImage();

		if(pImage)
		{
			pImage->SetDimensions(newActor->getPathIniX(), newActor->getPathIniY(), regionWidth, regionHeight);
			engine->InvalidateRectangle(pImage->Bounds());
		}
	}	

	if(version > 6) loadingActors--;

	return newActor;
}

void Actor::Load(SDL_RWops *src, Uint32 version)
{
	//Animations
	Uint16 nAnimations = GameControl::Read16(src);
	
	if(nAnimations)
	{
		if(version > 2)
		{
			KrAction tempAction(""), *pAction = NULL;
			for(int i = 1; i < nAnimations+1; i++)
			{
				Uint8 animationType;
				GameControl::Read(src, &animationType, sizeof(animationType), 1);
				
				pAction = NULL;
				if(animationType == ANIMATION_SEQUENCE)
				{					
					gedString animKey;
					if(GameControl::Get()->GetGameFileVersion() < 39)
					{
						tempAction.Load(src, version);
						if(tempAction.getFrameRate() == 0) tempAction.setFrameRate(GameControl::Get()->getFrameRate());
						
						pAction = &tempAction;
					}
					else 
					{					
						animKey.Read(src);
						pAction = mapActorAnimation.Find1(animKey);
					}

					GLASSERT(pAction != NULL);
					if(pAction)
					{
						bool bRes = AddAnimation(pAction->Name(), pAction->getimgName(), 
							pAction->getnFramesH(), pAction->getnFramesV(), 
							pAction->gethotSpotX(), pAction->gethotSpotY(), pAction->getFrameRate());

						if(!bRes && GameControl::Get()->getFastFile())
						{
							//Load image failed
							//Force reload with ged icon

							SDL_ClearError();

							AddAnimation(pAction->Name(), pAction->getimgName(), 
								pAction->getnFramesH(), pAction->getnFramesV(), 
								pAction->gethotSpotX(), pAction->gethotSpotY(), pAction->getFrameRate(),
								true, false, true);
						}
					}
				}
				else
				{
					Sequence *seq = new Sequence(DEFINED_SEQUENCE);
					seq->Load(src, version);
					
					mySequence->mapSequence.Add(seq->getSequenceName(), seq);
					mySequence->listSequence.PushBack(seq);
				}	
				
				if(GameControl::Get()->getFastFile())
				{
					SDL_ClearError();
				}
			}
		}
		else
		{
			KrAction tempAction("");
			for(int i = 1; i < nAnimations+1; i++)
			{
				tempAction.Load(src, version);
				AddAnimation(tempAction.Name(), tempAction.getimgName(), 
					tempAction.getnFramesH(), tempAction.getnFramesV(), 
					tempAction.gethotSpotX(), tempAction.gethotSpotY());
			}
		}
		
		//Current action
		gedString currentAction;
		currentAction.Read(src);
		if(currentAction.length()) SetAnimation(currentAction, true);
	}

#ifdef STAND_ALONE_GAME
	UpdateAnimIndex();
#endif

	GameControl::Read(src, &pathDirectionX, sizeof(pathDirectionX), 1);
	GameControl::Read(src, &pathDirectionY, sizeof(pathDirectionY), 1);
	pathIniX = GameControl::Read32(src);
	pathIniY = GameControl::Read32(src);

	
	{
		//Don't need set bEnableCollisionDetection anymore
		//Infinite actors can handle collision
		bool _bEnableCollisionDetection;
		GameControl::Read(src, &_bEnableCollisionDetection, sizeof(Uint8), 1);
	}

	Uint8 tmp;
	GameControl::Read(src, &tmp, sizeof(Uint8), 1);
	flags.Set(FLAG_FORWARDANIMATION, tmp != 0);

	GameControl::Read(src, &tmp, sizeof(Uint8), 1);
	flags.Set(FLAG_CREATEATSTARTUP, tmp != 0);

	if(version > 49)
	{
		GameControl::Read(src, &tmp, sizeof(Uint8), 1);
		flags.Set(FLAG_NETACTOR, tmp != 0);
	}

	if(version > 53)
	{
		GameControl::Read(src, &tmp, sizeof(Uint8), 1);
		flags.Set(FLAG_NET_OPTIMIZED_PRECISION, tmp != 0);
	}
	
	cursor.Load(src);	

	//Paths must already have been loaded before
	gedString pathName;

	pathName.Read(src);
	if(pathName.length()) pathX = GameControl::Get()->GetPath(pathName);

	pathName.Read(src);
	if(pathName.length()) pathY = GameControl::Get()->GetPath(pathName);

	
	//Text Actor
	Uint8 hasText;
	GameControl::Read(src, &hasText, sizeof(hasText), 1);

	
	if(hasText)
	{
		//Load text
		gedString textStr;
		textStr.Read(src);

		//Load font name
		gedString font;
		font.Read(src);	

		
		//Load ini char and number of chars
		Uint16 iniChar, nChars;

		iniChar = GameControl::Read16(src);
		nChars = GameControl::Read16(src);

		Uint8 inputText = 0, hasBorder = 0, hasBackGround = 0, getFocus = 0;
		KrRGBA borderColor = 0, cursorColor = 0, backGroundColor = 0;

		
		if(version > 30)
		{
			//Read input values
			GameControl::Read(src, &inputText, sizeof(inputText), 1);

			if(inputText)
			{
				GameControl::Read(src, &hasBorder, sizeof(hasBorder), 1);
				GameControl::Read(src, &hasBackGround, sizeof(hasBackGround), 1);
				GameControl::Read(src, &getFocus, sizeof(getFocus), 1);				
				
				borderColor.all = GameControl::Read32(src);
				cursorColor.all = GameControl::Read32(src);
				backGroundColor.all = GameControl::Read32(src);
			}
		}

		
		//Add text		
		Text *text = AddText(textStr, 0, 0, ALIGN_AUTOMATIC, 255, 255, 255, 
			font, iniChar, nChars,
			inputText, hasBorder, hasBackGround, getFocus, 
			borderColor, cursorColor, backGroundColor);

		
		if(!text && GameControl::Get()->getFastFile())
		{
			SDL_ClearError();

			//Text load failed
			//Add default font
			
			text = AddText(textStr, 0, 0, ALIGN_AUTOMATIC, 255, 255, 255, 
				"maksfont.bmp", 32, 94,
				inputText, hasBorder, hasBackGround, getFocus, 
				borderColor, cursorColor, backGroundColor);
		}
		
		nZeros = textStr.length();//Solve the 00 bug when set a big text and set a number after
		setTextActor(text);	
	}

	
	Uint32 zDepth = GameControl::Read32(src);
	if(version <= 8 && zDepth == 5000)
	{
		//Increment base zdepth for actors with unchanged zdepth (5000 = old mid zdepth)
		zDepth += baseZDepth;
		baseZDepth++;
	}

	
	loadedZDepth = zDepth; //Solve the bug "don't show myPlane smoke in 1945.ged in stand alone game"

	KrImage *pImage = getImage();
	if(pImage) pImage->SetZDepth(zDepth);

	
	
	//Load transparency only
	Uint8 alpha;
	GameControl::Read(src, &alpha, sizeof(alpha), 1);
	
	double t = (255 - alpha)/255.0;
	SetTransparency(t);
	loadedTransparency = t;//Solve the bug "don't show myPlane smoke in 1945.ged in stand alone game"
	
	if(version > 42)
	{
		//Load full color
		KrRGBA color;
		color.all = GameControl::Read32(src);
	
		//Set color
		SetColor(color.c.red, color.c.green, color.c.blue);
		loadedColor = color;
	}
	else
	{
		loadedColor.Set(255, 255, 255);
	}

	

	//Tiles
	Uint8 hasTile;
	GameControl::Read(src, &hasTile, sizeof(hasTile), 1);

	
	if(hasTile)
	{
		Tile *_tile = new Tile(this);	
		_tile->Load(src);
		setTile(_tile);	
	}

	if(version > 5)
	{
		//Infinite
		Uint8 infinite;
		GameControl::Read(src, &infinite, sizeof(infinite), 1);

		if(infinite != NO_INFINITE) SetInfinite(infinite);
	}

	if(version > 6)
	{
		//Clone index
		Uint32 cloneindex = GameControl::Read32(src);

		scriptVars->cloneindex = cloneindex;
		sprintf(scriptVars->clonename, "%s.%ld", scriptVars->name, scriptVars->cloneindex);
		
		if(version < 9)
		{
			//Fix repeated clone names
			Actor *actor;
			while((actor = GameControl::Get()->ExistsClone(scriptVars->name, scriptVars->cloneindex)) != NULL && actor != this)
			{			
				int *maxIndex = mapActorsMaxClone[scriptVars->name];
				(*maxIndex)++;
				scriptVars->cloneindex = *maxIndex;
				sprintf(scriptVars->clonename, "%s.%ld", scriptVars->name, scriptVars->cloneindex);
			}
		}

		//Update max clone index
		if( !GameControl::Get()->getGameMode() ||
			flags.IsSet(FLAG_CREATEATSTARTUP) //Only increase if was running
			) 
		{
			int *maxIndex = mapActorsMaxClone.FindString(scriptVars->name);
			if(maxIndex) 
			{
				if(*maxIndex < scriptVars->cloneindex)
					*maxIndex = scriptVars->cloneindex;
			}
			else
			{
				mapActorsMaxClone.Add(scriptVars->name, scriptVars->cloneindex);				
			}
		}

		if(version > 9)
		{
			totalConnectedActors = GameControl::Read32(src);
		}

		if(version > 22)
		{
			Uint8 lock;
			GameControl::Read(src, &lock, sizeof(lock), 1);
			flags.Set(FLAG_LOCKACTOR, lock != 0);			
		}

		
		//Previous script vars
		//*scriptVarsAnt = *scriptVars;
		
		scriptVarsAnt->x = scriptVars->x;	
		scriptVarsAnt->y = scriptVars->y;
		scriptVarsAnt->xscreen = scriptVars->xscreen;
		scriptVarsAnt->yscreen = scriptVars->yscreen;
		scriptVarsAnt->animpos = scriptVars->animpos;
		scriptVarsAnt->xvelocity = scriptVars->xvelocity;
		scriptVarsAnt->yvelocity = scriptVars->yvelocity;
		scriptVarsAnt->diretional_velocity = scriptVars->diretional_velocity;
		scriptVarsAnt->angle = scriptVars->angle;
		scriptVarsAnt->r = scriptVars->r;
		scriptVarsAnt->g = scriptVars->g;
		scriptVarsAnt->b = scriptVars->b;
		scriptVarsAnt->transp = scriptVars->transp;
		scriptVarsAnt->textNumber = scriptVars->textNumber;
		strcpy(scriptVarsAnt->text, scriptVars->text);
	}

#ifndef STAND_ALONE_GAME
	if(version > 43)
	{
		//Read description (don't read in exported game)
		description.Read(src);
	}
#endif

	//Final expansion indicator
	Uint8 expansion = 0;
	GameControl::Read(src, &expansion, sizeof(expansion), 1);
	
	//To update clone name
	if(scriptVars->cloneindex != 0)
	{
		gedString aux(scriptVars->name);
		aux += ".0";
		
		Actor *edit = GameControl::Get()->FindEditActor(aux);		
		if(edit == this)
		{
			//Incorrect add in Actor::Actor with cloneindex = 0
			GameControl::Get()->RemoveEditActor(aux);
		}
		
		GameControl::Get()->AddEditActor(this);
	}
	
}



void Actor::setTextActor(Text *text)
{


	if(textActor)
	{
		ListObjectIterator it(listText);
		for(it.Begin(); !it.Done(); it.Next())
		{
			if(it.Current() == textActor)
			{
				listText.FindAndDelete(textActor);
				break;
			}
		}		

		delete textActor;
		textActor = NULL;
	}
	else
	{
		//N zeros to fill numbers
		if(text  
			&& !nZeros //Solve the 00 bug when set a big text and set a number after
			)
			nZeros = text->GetText().length();
	}

	if(text)
	{
		flags.Clear(FLAG_ENABLECOLLISIONDETECTION);

		//Solve the bug: "Action Set Text don't erase icon actor at first time"
		engine->InvalidateRectangle(Bounds());
		SetAnimation(ACTOR_GED_TEXT);		
	}
	else
	{
		flags.Set(FLAG_ENABLECOLLISIONDETECTION);
		if(NumAnimations())
		{
			SetAnimation(AnimationName(2), true);
		}
		else
		{
			SetAnimation(AnimationName(0), true);
		}

		nZeros = 0;
	}

	textActor = text;

	if(textActor && scriptVars->text[0] == 0 && textActor->GetText().length() < MAX_SCRIPTTEXT) 
	{
		gedString aux(textActor->GetText());
		strcpy(scriptVars->text, aux.c_str());
	}

}

void Actor::SetText(const gedString& text)
{
	if(textActor)
	{
		//-2 Solve the clip problem when set initial text to "textActor"
		//and after set the text to "-00000001"
		//In variable true type fonts
		//if(text.length() > textActor->GetText().length() - 2) 
		

		//Solve the one char bug (set text to 0 and change in an action to 12: only show 1)
		//textOnCharBug.ged
		KrRect newText = textActor->GetDimensionsFont(text);
		KrRect currentText = textActor->GetDimensionsFont(textActor->GetText());

		//Test != not > (solve the align bug in textAlign.ged)
		if(newText.Width() != currentText.Width() || newText.Height() != currentText.Height())
		{
			Text *t = AddText(text, 0, 0, ALIGN_AUTOMATIC, 255, 255, 255, 
				textActor->getFontName(), 
				textActor->getiniASCIIChar(), 
				textActor->getnChars(),
				
				textActor->IsEditable(),
				textActor->HasBorder(),
				textActor->HasBackground(),
				false, //Don't change the focus here, only on text creation
				textActor->getBorderColor(),
				textActor->getCursorColor(),
				textActor->getBackGroundColor());

			setTextActor(t);
		}

		textActor->SetText(text);
		if(text.length() < MAX_SCRIPTTEXT) 
		{
			gedString aux(text);
			strcpy(scriptVars->text, aux.c_str());
		}
	}

	flags.Clear(FLAG_PENDINGSETTEXT);
}

gedString Actor::GetText()
{
	if(textActor)
	{
		return textActor->GetText();
	}

	return "";
}

void Actor::CopyActorTree()
{
	KrImage *pImage = getImage();
	if(!pImage) return;

	//Copy this actor
	if(flags.IsSet(FLAG_CREATEATSTARTUP))
	{
		NewActor(false);
	}
	else pImage->SetVisible(false);


	//Copy childs
	GlInsideNodeIt<KrImNode*> it = pImage->ChildTreeIterator();
	it.Begin(); 
	while( !it.Done() )
	{
		KrImNode *node = it.CurrentData();
		if(!node->getIsText())
		{
			Actor *childActor = (Actor *)node->getParentActor();
			
			if(childActor && childActor != this && childActor->EditMode())
			{
				childActor->CopyActorTree();
			}
		}

		it.Next();
	}
}



void Actor::AdjustView(int width, int height, bool bSetViewPos)
{
	KrImage *pImage = getImage();

	if(pImage && type == VIEW_ACTOR)
	{
		pImage->SetDimensions(width, height);
		if(bSetViewPos) 
		{
			/*pImage->*/SetPos(-width/2, -height/2); //To avoid the (0, 0) position bug (if a  view has a (0, 0) nothing was happen)
			SetPathIni(-width/2, -height/2);
		}
	}
}



void Actor::SetAnteriorPositions()
{
	anteriorXPos = getX();
	anteriorYPos = getY();
	anteriorFramePathX = scriptVars->pathxpos;
	anteriorFramePathY = scriptVars->pathypos;

	frameAnt = sprite?sprite->Frame():0;
	
	flags.Set(FLAG_SETANTERIORPOSITION);
}

void Actor::ToAnteriorPosition()
{


	SetPos(anteriorXPos, anteriorYPos);
	scriptVars->pathxpos = anteriorFramePathX;
	scriptVars->pathypos = anteriorFramePathY;

}

void Actor::SetSize(int width, int height)
{
	if(type == REGION_ACTOR || type == REGION_ACTOR_FILLED || type == CANVAS)
	{
		engine->InvalidateRectangle(getImage()->Bounds());

		getImage()->SetDimensions(width, height);
		if(type == CANVAS) getCanvas()->Invalidate();
		
		engine->InvalidateRectangle(getImage()->Bounds());
		
		if(type == CANVAS)
		{
			scriptVars->width = Width();
			scriptVars->height = Height();
		}
	}
}

void Actor::SetPos(double x, double y, bool bDrag)
{
	//Move the actor relative to the parent
	KrImage *pImage = getImage();

	if((scriptVars->x == x && scriptVars->y == y) || !pImage)
	{
		return;
	}

	bool bSendPositionForPrecision = flags.IsSet(FLAG_NET_OPTIMIZED_PRECISION) && CanExecuteActionLocally();

	if(bSendPositionForPrecision || (bDrag && CanSendRemoteAction()))
	{
		//Send a position change by user input
		//Send unreliable for performance, if not have the precision flag
		GameControl::Get()->SendRemotePosition(this, x, y, bSendPositionForPrecision);
	}
	
	engine->InvalidateRectangle(Bounds());

	//Round, don't just truncate
	//pImage->SetPos(GlFixedToIntRound(GlDoubleToFixed(x)), GlFixedToIntRound(GlDoubleToFixed(y)));

	//More smooth moves (round only in KrRle::CalculateBounds)
	pImage->SetPos(x, y);

#ifdef USE_WALK
	pImage->CalcTransform(); //Don't need with Walk1() optimization
#endif

	if(bDrag && !GameControl::Get()->getGameMode())
	{
		int movex, movey;
		KrImage *pSprite = getSprite();

		if(pSprite && GameControl::Get()->ImageSnap(pSprite, GameControl::Get()->GetAxis()->getImage(), &movex, &movey))
		{
			engine->InvalidateRectangle(Bounds());
			pImage->CalcTransform();

			x = pSprite->X();
			y = pSprite->Y();
		}
	}
	
	scriptVars->x = x;
	scriptVars->y = y;

	scriptVars->xscreen = pImage->CompositeXForm().x.ToIntRound();
	scriptVars->yscreen = pImage->CompositeXForm().y.ToIntRound();

	bAnyPositionChanged = true; //For tree validation in collision events
}



void Actor::SetColor(int r, int g, int b)
{
	KrImage *pImage = getImage();
	if(!pImage) return;

	if(r < 0) r = 0;
	else if(r > 255) r = 255;

	if(g < 0) g = 0;
	else if(g > 255) g = 255;

	if(b < 0) b = 0;
	else if(b > 255) b = 255;
	
	
	KrColorTransform color(pImage->CTransform());

	//Change m don't b (solve color.ged)
	color.Set(r, 0, g, 0, b, 0, color.Alpha());
	pImage->SetColor(color);

	scriptVars->r = r;
	scriptVars->g = g;
	scriptVars->b = b;
}

void Actor::SetFrame(int f)
{
	KrSprite *pSprite = getSprite();

	if(pSprite)
	{
		Sequence *seq = GetSequence();

		if(seq)
		{
			if(seq->getType() == ANIMATION_SEQUENCE) 
			{
				if(f < 0) f = 0;
				else if(f >= pSprite->NumFrames()) f = pSprite->NumFrames() - 1;
		
				pSprite->SetFrame(f);
			}
			else 
			{
				if(f < 0) f = 0;
				else if(f >= seq->getNFrames()) f = seq->getNFrames() - 1;
		
				pSprite->SetFrame(seq->GetFrame(f));
				seq->SetFrame(f); //Solve the bug in sequenceGlitch.ged
			}
		}
	}

	if(scriptVars->animpos != f && flags.IsSet(FLAG_NET_OPTIMIZED_PRECISION) && CanExecuteActionLocally())
	{
		//Send the frame change		
		scriptVars->animpos = f;
		GameControl::Get()->SendRemoteFrame(this, true);
	}
	else
	{
		scriptVars->animpos = f;
	}
}

void Actor::SetPosScreen(double x, double y)
{
	if(getParent())
	{
		KrImage *pImage = getParent()->getImage();
		
		if(pImage)
		{
			KrVector2T< GlFixed > object;
			
			pImage->ScreenToObject( GlFixed(x), GlFixed(y), &object );
			SetPos(object.x.ToDouble(), object.y.ToDouble());
		}
	}
	else
	{
		SetPos(x, y);
	}

	scriptVars->xscreen = x;
	scriptVars->yscreen = y;
}

void Actor::SetCloneIndex(int newCloneIndex)
{
	if(scriptVars->cloneindex == newCloneIndex) return;

	//Update clone index
	GameControl::Get()->RemoveActor(this);
	scriptVars->cloneindex = newCloneIndex;
	sprintf(scriptVars->clonename, "%s.%ld", scriptVars->name, scriptVars->cloneindex);
	GameControl::Get()->AddActor(this); //After clonename setup

	//Update max clone index
	//Need to update the first clone name (issue 57)
	int *maxIndex = mapActorsMaxClone.FindString(scriptVars->name);
	if(maxIndex) 
	{
		if(*maxIndex < scriptVars->cloneindex)
			*maxIndex = scriptVars->cloneindex;
	}
	else
	{
		mapActorsMaxClone.Add(scriptVars->name, scriptVars->cloneindex);				
	}
}

void Actor::setRunning(bool bFromReadState)
{
	//Call after actor initializations
	if(flags.IsSet(FLAG_RUNNING) || bInContructionCallBack) return;

	flags.Set(FLAG_RUNNING);
	flags.Clear(FLAG_SETACTUALVALUES);
	
	GameControl::Get()->getScript()->AddActor(this);
	GameControl::Get()->AddRunningActor(this);

#ifndef USE_RTREE
	flags.Set(FLAG_WORKING); //Initial running state
	GameControl::Get()->AddWorkingActor(this);
#endif


	//Update global collision map
	if( getAction() && 
		(getAction()->HaveCollisions(true) || getAction()->HaveCollisionFinish(true)) && 
		getEnableCollisionDetection() &&
		(eventControl & EVENTCOLLISION))
	{
		GameControl::Get()->AddActorToHandledCollisionMap(this);
		flags.Set(FLAG_HANDLECOLLISION);
	}

	//Update max clone index
	//Need to update the first clone name (issue 57)
	int *maxIndex = mapActorsMaxClone.FindString(scriptVars->name);
	if(maxIndex) 
	{
		if(*maxIndex < scriptVars->cloneindex)
			*maxIndex = scriptVars->cloneindex;
	}
	else
	{
		mapActorsMaxClone.Add(scriptVars->name, scriptVars->cloneindex);				
	}

	//Solve the bug 'actors are destroyed when out of vision' (see net.ged)
	UpdateRegionLoad();

	InitNetActor();

	if(!CanExecuteActionLocally())
	{
		//This actor will be controlled by the remote machine.
		//To avoid this actor have a clone name other than the clone name on the remote machine,
		//invalidate it now, and put in a unlinked list for the remote machine.
		//In the first remote machine command, if can't get the actor in the GetActor function,
		//try get from the unlinked list.
		
		if(!bFromReadState) GameControl::Get()->AddToUnlinkedList(owner, this);
		
		//Allow automatic frame change?
		if(flags.IsSet(FLAG_NET_OPTIMIZED_PRECISION))
		{
			framesToAnim = DISABLE_ANIMATION;
		}
	}

	//Call OnCreate
	//Solve the bug on 1945 reg.ged in stand alone mode: show level_01 due don't execute your OnCreate action
	//If not bEdit, will be called later
	//If remove bEdit test some bullets don't will folow myPlane
	if(flags.IsSet(FLAG_EDIT) && flags.IsSet(FLAG_CALLONCREATE) && getAction() && GameControl::Get()->getStandAloneMode())
	{
		getAction()->OnCreate(this);
	}
}

void Actor::SetVisible(int state)
{
	getImage()->SetVisible(state);
}

void Actor::setEnableCollisionDetection(bool bEnable) 
{


	flags.Set(FLAG_ENABLECOLLISIONDETECTION, bEnable);

	if(bEnable)
	{
		if(	getAction() &&
			(getAction()->HaveCollisions(true) || getAction()->HaveCollisionFinish(true)) && 
			(eventControl & EVENTCOLLISION))
		{
			GameControl::Get()->AddActorToHandledCollisionMap(this);
			flags.Set(FLAG_HANDLECOLLISION);
		}	
	}
	else
	{
		GameControl::Get()->RemoveActorFromHandledCollisionMap(this);
		flags.Clear(FLAG_HANDLECOLLISION);
	}
}

void Actor::setEventControl(Uint32 events, bool bEnable)
{


	if(bEnable) eventControl |= events;
	else eventControl &= ~events;

	if(events & EVENTCOLLISION)
	{
		if(bEnable)
		{
			if(	getAction() &&
				(getAction()->HaveCollisions(true) || getAction()->HaveCollisionFinish(true)) && 
				getEnableCollisionDetection())
			{
				GameControl::Get()->AddActorToHandledCollisionMap(this);
				flags.Set(FLAG_HANDLECOLLISION);
			}
		}
		else
		{
			GameControl::Get()->RemoveActorFromHandledCollisionMap(this);
			flags.Clear(FLAG_HANDLECOLLISION);
		}
	}
}

extern double getAngle(double dx, double dy);


void Actor::SetActualValues(U32 updateFlags)
{
	if(flags.IsSet(FLAG_SETACTUALVALUES)) return; //Solve the bug in 'runtime 0.1.0r.ged'

	flags.Set(FLAG_SETACTUALVALUES);

	if(scriptVars->xvelocity == 0 && flags.IsSet(FLAG_SETANTERIORPOSITION))
	{
		//Calculate x velocity
		scriptVars->xvelocity = scriptVars->x - anteriorXPos;
		flags.Set(FLAG_CALCULATEDXVELOCITY);
	}
	else 
	{
		flags.Clear(FLAG_CALCULATEDXVELOCITY);
	}

	if(scriptVars->yvelocity == 0 && flags.IsSet(FLAG_SETANTERIORPOSITION))
	{
		//Calculate y velocity
		scriptVars->yvelocity = scriptVars->y - anteriorYPos;
		flags.Set(FLAG_CALCULATEDYVELOCITY);
	}
	else 
	{
		flags.Clear(FLAG_CALCULATEDYVELOCITY);
	}

	

	//View was moved?
	KrImage *pImage = getImage();
	if(pImage)
	{
		scriptVars->xscreen = pImage->CompositeXForm().x.ToIntRound();
		scriptVars->yscreen = pImage->CompositeXForm().y.ToIntRound();
	}

	scriptVars->xprevious = anteriorXPos;
	scriptVars->yprevious = anteriorYPos;

	//Calculate move angle
	if(updateFlags & NEED_DIRECTIONAL_VELOCITY || updateFlags & NEED_ANGLE)
	{
		if(pathX && pathY)
		{
			scriptVars->angle = pathX->GetAngle(getFramePathX());
		}
		else if(scriptVars->yvelocity != 0.0 || scriptVars->xvelocity != 0.0)
		{
			scriptVars->angle = ::getAngle(scriptVars->xvelocity, scriptVars->yvelocity);			
		}
		else
		{
			scriptVars->angle = antAngle;
		}

		scriptVars->diretional_velocity = sqrt(scriptVars->xvelocity*scriptVars->xvelocity + scriptVars->yvelocity*scriptVars->yvelocity);
	}
	
	/*scriptVars->angle = (scriptVars->angle + antAngle) / 2.0;
	antAngle = scriptVars->angle;*/


	

	//scriptVars->width and scriptVars->height are updated on SetAnimation, SetScale and when animpos is changed
	if(updateFlags & NEED_DIMENTIONS)
	{
		//Slow test
		if((scriptVars->animpos != scriptVarsAnt->animpos || scriptVars->width <= 0 || scriptVars->height <= 0) && getImage())
		{
			scriptVars->width = Width();
			scriptVars->height = Height();
		}
	}

	/*if(getSprite())
	{
		scriptVars->animindex = GetAnimationIndex(GetAnimation()); //Slow... Try put only in SetAnimation (bug if call ChangeAnimation in sript)??????

		if(seq->getType() == ANIMATION_SEQUENCE) scriptVars->nframes = getSprite()->NumFrames();		
		else scriptVars->nframes = seq->getNFrames();
	}*/

	//*scriptVarsAnt = *scriptVars;
	scriptVarsAnt->x = scriptVars->x;	
	scriptVarsAnt->y = scriptVars->y;
	scriptVarsAnt->xscreen = scriptVars->xscreen;
	scriptVarsAnt->yscreen = scriptVars->yscreen;
	scriptVarsAnt->r = scriptVars->r;
	scriptVarsAnt->g = scriptVars->g;
	scriptVarsAnt->b = scriptVars->b;
	scriptVarsAnt->transp = scriptVars->transp;
	scriptVarsAnt->animpos = scriptVars->animpos;
	scriptVarsAnt->angle = scriptVars->angle;
	scriptVarsAnt->xvelocity = scriptVars->xvelocity;
	scriptVarsAnt->yvelocity = scriptVars->yvelocity;
	scriptVarsAnt->diretional_velocity = scriptVars->diretional_velocity;
	
	
	
	
	if(textActor)
	{
		if(!textActor->IsEditable())
		{
			scriptVarsAnt->textNumber = scriptVars->textNumber;
			strcpy(scriptVarsAnt->text, scriptVars->text);
		}
		else
		{
			gedString text(textActor->GetText().getCharBuf(), MAX_SCRIPTTEXT - 1);
			
			if(text.length())
			{
				strcpy(scriptVars->text, text.getCharBuf());
				strcpy(scriptVarsAnt->text, text.getCharBuf());
				
				scriptVars->textNumber = scriptVarsAnt->textNumber = atof(scriptVars->text);
			}
			else
			{
				scriptVars->text[0] = 0;
				scriptVarsAnt->text[0] = 0;
				scriptVars->textNumber = scriptVarsAnt->textNumber = 0.0;
			}
		}
	}
}

void Actor::UpdateScriptChanges(U32 updateFlags, bool bFromNetwork)
{
	flags.Clear(FLAG_SETACTUALVALUES);
	bool bExecutePrecision = !flags.IsSet(FLAG_NET_OPTIMIZED_PRECISION) || CanExecuteActionLocally();

	if(!bFromNetwork 
		&& !flags.IsSet(FLAG_MOVETO) //Solve the Move Finish Bug.ged
		)
	{
		if(flags.IsSet(FLAG_CALCULATEDXVELOCITY) && scriptVars->xvelocity == scriptVarsAnt->xvelocity)
		{
			//Reset velocity
			scriptVars->xvelocity = 0;
		}

		if(flags.IsSet(FLAG_CALCULATEDYVELOCITY) && scriptVars->yvelocity == scriptVarsAnt->yvelocity)
		{
			//Reset velocity
			scriptVars->yvelocity = 0;
		}


		if(updateFlags & NEED_DIRECTIONAL_VELOCITY || updateFlags & NEED_ANGLE)
		{
			if(scriptVars->diretional_velocity != scriptVarsAnt->diretional_velocity || scriptVars->angle != scriptVarsAnt->angle)
			{
				//Set new velocities (priority over velocity set)
				double teta = scriptVars->angle*Script::PI/180.0;

				scriptVars->xvelocity = scriptVars->diretional_velocity*cos(teta);
				scriptVars->yvelocity = -scriptVars->diretional_velocity*sin(teta);
			}
		}
	}

	if(bExecutePrecision || bFromNetwork)
	{
		if(scriptVars->x != scriptVarsAnt->x || scriptVars->y != scriptVarsAnt->y)
		{
			//More smooth moves (round only in KrRle::CalculateBounds)
			KrImage *pImage = getImage();

			if(pImage)
			{
				pImage->SetPos(scriptVars->x, scriptVars->y);

				//Round, don't just truncate
				//pImage->SetPos(GlFixedToIntRound(GlDoubleToFixed(scriptVars->x)), GlFixedToIntRound(GlDoubleToFixed(scriptVars->y)));

				scriptVars->xscreen = pImage->CompositeXForm().x.ToDouble();
				scriptVars->yscreen = pImage->CompositeXForm().y.ToDouble();

				bAnyPositionChanged = true;
			}
		}
		else if(!bFromNetwork)
		{
			//Only update screen coordinates if not is a network update
			if( scriptVars->xscreen != scriptVarsAnt->xscreen ||
				scriptVars->yscreen != scriptVarsAnt->yscreen) 
			{
				if(this != GameControl::Get()->GetViewActor())
				{
					SetPosScreen(scriptVars->xscreen, scriptVars->yscreen);		
				}
				else
				{
					//The view screen coordinate must be always (0, 0)
					//Is a conceptual error try change the view screen coordinate

					scriptVars->xscreen = scriptVarsAnt->xscreen = 0.0;
					scriptVars->yscreen = scriptVarsAnt->yscreen = 0.0;

					Script::myDisplay("Trying to change the view screen coordinate.\n   The view is the game screen, so the view\n   screen coordinate must be always (0, 0)");
				}
			}
		}
	}

	if(scriptVars->r != scriptVarsAnt->r ||
	   scriptVars->g != scriptVarsAnt->g ||
	   scriptVars->b != scriptVarsAnt->b)
	{
		SetColor(scriptVars->r, scriptVars->g, scriptVars->b);
	}

	if(scriptVars->transp != scriptVarsAnt->transp) SetTransparency(scriptVars->transp);

	if(	
		scriptVars->animpos != scriptVarsAnt->animpos 		
		|| bFromNetwork //solve the 'net animpos.ged' with speed flag		 
	  ) 
	{
		//solve the 'net animpos.ged' with precision flag
		//Ignore the changes in animpos into scripts if gets the value from a remote machine
		if(bFromNetwork || bExecutePrecision)
		{
			SetFrame(scriptVars->animpos);		
		}
		else
		{
			//Keep the frame sent remotely
			scriptVars->animpos = scriptVarsAnt->animpos;
		}
	}

	if(getTextActor())
	{
		gedString *newText = NULL;
		bool bNumber = false;

		if(scriptVars->textNumber != scriptVarsAnt->textNumber)
		{
			newText =  new gedString(scriptVars->textNumber, nZeros);			
			bNumber = true;			
		}
		else if(strcmp(scriptVars->text, scriptVarsAnt->text) != 0)
		{			
			newText = new gedString(scriptVars->text);
		}

		if(newText)
		{
			if(IsVisible())
			{
				SetText(*newText);				
			}
			else
			{
				//Update text latter, when the actor become visible
				flags.Set(FLAG_PENDINGSETTEXT);

				if(bNumber)
				{
					//Update text
					strcpy(scriptVars->text, newText->c_str());
				}
			}

			delete newText;
		}
	}
}

bool Actor::IsVisible()
{
	KrImage *pImage = getImage();
	if(!pImage->IsVisible() || !pImage->CompositeCForm().Alpha()) return false;

	return true;
}

void Actor::RestartFrameSequences()
{
	MapActorSequenceIterator it(sequence);
	for(it.Begin(); !it.Done(); it.Next())
	{
		ListSequenceIterator itList(it.Value()->listSequence);
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			itList.Current()->resetFrame();
		}
	}
}

void Actor::PushQueueAction(stAction *queueAction)
{
	//Don't remove now actions with same time stamp

	if(queueAction->timeStamp == lastTimeStamp && lastQueuedAction)
	{
		//Remove later
		QueueActorAction *q = sameTimeStamp[queueAction->timeStamp];
		if(!q)
		{
			//Add last
			sameTimeStamp.FindOrCreate(queueAction->timeStamp)->PushBack(lastQueuedAction);
		}

		sameTimeStamp[queueAction->timeStamp]->PushBack(queueAction);
	}
	else if(queueAction->bLastActionInQueueOnly)
	{
		queueActorAction.Clear();
	}

	queueActorAction.PushBack(queueAction);
	
	lastTimeStamp = queueAction->timeStamp;
	lastQueuedAction = queueAction;
}

void Actor::ProcessQueueActions()
{
	bool bRemove = false;
	QueueActorAction removeActions;
	QueueActorActionIterator itList(queueActorAction);
	
	for(itList.Begin(); !itList.Done(); itList.Next())
	{
		stAction *queueAction = itList.Current();

		if(queueAction->executionType == WAIT_FOR_FRAME_EXECUTION)
		{
			for(int i = 0; i < queueAction->nFrames; i++)
			{
				if(queueAction->frames[i] == scriptVars->animpos && 
				   (!(*queueAction->name) || strcmp(queueAction->name, actualSequence.c_str()) == 0))
				{
					if(action)
					{
						action->Execute(queueAction, this, NULL, false, true);

						//Remove same time stamp actions
						QueueActorAction *q = sameTimeStamp[queueAction->timeStamp];
						if(q)
						{
							QueueActorActionIterator itList1(*q);	
							for(itList1.Begin(); !itList1.Done(); itList1.Next())
							{
								removeActions.PushBack(itList1.Current());
							}

							sameTimeStamp.Remove(queueAction->timeStamp);
						}
					}
					
					removeActions.PushBack(queueAction);
					bRemove = true;
					break;
				}
			}
		}
	}
	
	if(bRemove)
	{
		QueueActorActionIterator itList1(removeActions);	
		for(itList1.Begin(); !itList1.Done(); itList1.Next())
		{
			stAction *queueAction = itList1.Current();
			queueActorAction.FindAndDelete(queueAction);
		}
	}
}

void Actor::SetActorInView(bool _bActorInView)
{
	flags.Set(FLAG_ACTORINVIEW, _bActorInView);
	

	//Update working state
	if(flags.IsSet(FLAG_RUNNING))
	{
		if(!flags.IsSet(FLAG_WORKING))
		{
			if(_bActorInView)
			{
				flags.Set(FLAG_WORKING);
				GameControl::Get()->AddWorkingActor(this);	
				
				if(flags.IsSet(FLAG_HANDLECOLLISION))
				{
					GameControl::Get()->AddActorToHandledCollisionMap(this);
				}
			}
		}
		else
		{
			if(getAction() && getOutOfVisionOptimization() && !_bActorInView)
			{
				flags.Clear(FLAG_WORKING);
				GameControl::Get()->RemoveWorkingActor(this);		

				if(flags.IsSet(FLAG_HANDLECOLLISION))
				{
					GameControl::Get()->RemoveActorFromHandledCollisionMap(this);
				}
			}
		}
	}
}

void Actor::CheckOutOfVision()
{
	//Must be update position here
	
	KrRect antRect = Bounds(), rectActor;
	KrImage *pImage = getImage(), *pImageView = GameControl::Get()->GetViewActor()->getImage();
	if(!pImage || !pImageView) return;

	if(pImageView->IsInvalid(true))
	{
		//Update parent and children
		//Don't check pImage (slow down in 1945.ged)

		//Solve Text Region.ged bug when use Move To:
		//Destroy child of child of view after change the region

		pImageView->CalcCompositeTransform();
		pImage->CalcTransform(); //Solve the bug in ALPHA_1_1_4.ged (click em LOGO_darkspire only works when open and close Preferences)
		engine->Tree()->Walk(pImageView, true, true);
	}
	else
	{
		pImage->CalcTransform();
	}

	rectActor = Bounds();
	
	if(rectActor != antRect) //Fast mode
		engine->InvalidateRectangle(antRect);


	//Get bounds: must be get here (can change along the game)
	KrRect	rectView = pImageView->Bounds();

	//Expand the rect to avoid flick when use working actors
	rectView.EdgeAdd(GameControl::Get()->getViewSafeMargin()); 
	
	if(rectActor.IsValid())
	{
		if(rectView.Intersect(rectActor))
			flags.Set(FLAG_ACTORINVIEW);
		else
		{
			//Destroys actors that don't belong more any region
			if(getRegionCount() <= 0)
			{
				PostMessage(this, DELETE_ME, DELETE_ME);
			}
			
			flags.Clear(FLAG_ACTORINVIEW);
		}

		if(getAction() && getAction()->HaveOutOfVision(true) && !flags.IsSet(FLAG_ACTORINVIEW))
		{
			getAction()->OnOutOfVision(this);			
		}
	}

	SetActorInView(flags.IsSet(FLAG_ACTORINVIEW));	
}

bool Actor::IsActorInView(bool bCheckParent)
{	
	if(bCheckParent)
	{
		//Return true if the actor or one of your ascendants are in the view
		//Solve the bug "GUI_debug_msg actor will occasionally move for no reason in exported game" in ALPHA_1_1_4a.ged
		Actor *node = this;
		do
		{
			if(node->flags.IsSet(FLAG_ACTORINVIEW)) 
				return true;
			
			node = node->parent;
			
		} while(node /*&& node != GameControl::Get()->GetAxis()*/); //Don't need check axis. Axis never has FLAG_ACTORINVIEW set
		
		return false;
	}
	else
	{
		return flags.IsSet(FLAG_ACTORINVIEW);
	}
}

void Actor::setOutOfVisionOptimization(bool value) 
{
	if(getAction())
	{
		getAction()->setOutOfVisionOptimization(value);
	}
}

bool Actor::getOutOfVisionOptimization() 
{
	if(getAction())
	{
		return getAction()->getOutOfVisionOptimization();
	}
	
	//Default in Actor control
	return false;
}

void Actor::SetInfinite(int infinite)
{
	KrSprite *pSprite = getSprite();

	if(pSprite)
	{
		//Disable collisions when infinite
		pSprite->SetInfinite(infinite);
	}
}

void Actor::GetObstacleBoundingBoxes(GeoPathFinder &gp, Actor *avoid)
{
	//Get box positions relative to axis

	//Get axis position, for coordinate tranlation (screen to axis)
	const KrMatrix2 &posAxis = GameControl::Get()->GetAxis()->getImage()->CompositeXForm();
	double  axisX = posAxis.x.ToDouble(),
			axisY = posAxis.y.ToDouble();

	if(!avoid->getTile())
	{
		//Single actor
		const KrMatrix2 &pos = avoid->getImage()->CompositeXForm();

		Box box;

		box.setOrigin(Vec2f(pos.x.ToDouble() - axisX, pos.y.ToDouble() - axisY));
		box.setDimensions(Vec2f(avoid->Width(), avoid->Height()));

		gp.AddBox(box);				
	}
	else
	{
		//Get the tiles bounding boxes
		MapTileIterator it(avoid->getTile()->getMapTile());	
		for(it.Begin(); !it.Done(); it.Next())
		{
			TileElement *element = *it.Value();	
			const KrMatrix2 &pos = element->getSprite()->CompositeXForm();

			KrRect rect;
			element->getSprite()->QueryBoundingBox(&rect);

			Box box;
	
			box.setOrigin(Vec2f(pos.x.ToDouble() - axisX, pos.y.ToDouble() - axisY));
			box.setDimensions(Vec2f(rect.Width(), rect.Height()));

			gp.AddBox(box);			
		}
	}
}

void Actor::GetObstacles(char *obstacleActor)
{
	//If is the same obstacle name and the sum of all obstacle positions is the same,
	//don't need to put the obstacles again
	Actor *avoid = NULL;

	//Get axis position, for coordinate tranlation (screen to axis)
	const KrMatrix2 &posAxis = GameControl::Get()->GetAxis()->getImage()->CompositeXForm();
	double  axisX = posAxis.x.ToDouble(),
			axisY = posAxis.y.ToDouble();

	//Check obstacle name
	if(strcmp(obstacleActor, S_EVENT_ACTOR) == 0)
	{
		avoid = Action::getActualEventActor();
	}
	else if(strcmp(obstacleActor, S_COLLIDE_ACTOR) == 0)
	{
		avoid = Action::getActualCollideActor();
	}
	else if(strcmp(obstacleActor, S_PARENT_ACTOR) == 0)
	{
		if(getParent() != GameControl::Get()->GetAxis()) avoid = getParent();
	}
	else if(strcmp(obstacleActor, S_CREATOR_ACTOR) == 0)
	{
		avoid = getCreator();
	}

	//Check if can reuse
	double newSum = 0.0;

	//Get the bounding boxes
	if(IS_VALID_ACTOR(avoid))
	{
		//Single actor	
		const KrMatrix2 &pos = avoid->getImage()->CompositeXForm();
		newSum = pos.x.ToDouble() - axisX + pos.y.ToDouble() - axisY;
	}
	else
	{
		//Multiple actors
		ListActor *listActor = mapActors.FindString(obstacleActor);
		if(listActor)
		{
			for(int il = 0; il < listActor->Count(); il++)
			{
				Actor *avoid = (*listActor)[il];
				if(avoid && avoid->getRunning())
				{
					const KrMatrix2 &pos = avoid->getImage()->CompositeXForm();
					newSum += pos.x.ToDouble() - axisX + pos.y.ToDouble() - axisY;
				}			
			}
		}
		else if(strchr(obstacleActor, '.'))
		{
			//Clone specified
			avoid = GameControl::Get()->GetActor(obstacleActor, true, false, false);
			if(avoid && avoid->getRunning())
			{
				const KrMatrix2 &pos = avoid->getImage()->CompositeXForm();
				newSum += pos.x.ToDouble() - axisX + pos.y.ToDouble() - axisY;
			}			
		}		
	}

	GeoPathFinder& gp = *GeoPathFinder::Get(obstacleActor);

	if(/*gp.obstacle == obstacleActor &&*/ gp.positionSum == newSum)
	{
		//Can reuse the old obstacles
		return;
	}

	{		
		MuteEx mutex(gp.GetMutex()); //Ensure calc thread is not executing		

		//Can't reuse, so get all retangles again
		avoid = NULL;

		//Check obstacle name
		if(strcmp(obstacleActor, S_EVENT_ACTOR) == 0)
		{
			avoid = Action::getActualEventActor();
		}
		else if(strcmp(obstacleActor, S_COLLIDE_ACTOR) == 0)
		{
			avoid = Action::getActualCollideActor();
		}
		else if(strcmp(obstacleActor, S_PARENT_ACTOR) == 0)
		{
			if(getParent() != GameControl::Get()->GetAxis()) avoid = getParent();
		}
		else if(strcmp(obstacleActor, S_CREATOR_ACTOR) == 0)
		{
			avoid = getCreator();
		}


	

		//gp.obstacle = obstacleActor;
		gp.positionSum = newSum;

		//Clear GeoPathFinder
		gp.Clear();


		//Get the bounding boxes
		if(IS_VALID_ACTOR(avoid))
		{
			//Single actor
			GetObstacleBoundingBoxes(gp, avoid);		
		}
		else
		{
			//Multiple actors
			ListActor *listActor = mapActors.FindString(obstacleActor);
			if(listActor)
			{
				for(int il = 0; il < listActor->Count(); il++)
				{
					Actor *avoid = (*listActor)[il];
					if(avoid && avoid->getRunning())
					{
						GetObstacleBoundingBoxes(gp, avoid);
					}			
				}
			}
			else if(strchr(obstacleActor, '.'))
			{
				//Clone specified
				avoid = GameControl::Get()->GetActor(obstacleActor, true, false, false);
				if(avoid && avoid->getRunning())
				{
					GetObstacleBoundingBoxes(gp, avoid);
				}			
			}		
		}
	}
}

void Actor::PathFinderRequisition(char *obstacleActor, double xEnd, double yEnd, double velocity)
{
	//xEnd, yEnd relative to axis	
	GetObstacles(obstacleActor);

	//Get axis position, for coordinate tranlation (screen to axis)
	const KrMatrix2 &posAxis = GameControl::Get()->GetAxis()->getImage()->CompositeXForm();
	double  axisX = posAxis.x.ToDouble(),
			axisY = posAxis.y.ToDouble();
	
	
	const KrMatrix2 &pos = getImage()->CompositeXForm();

	//Put in the calc thread
	GameControl::Get()->PushFindPathRequisition(
		this, 
		obstacleActor, 
		pos.x.ToDouble() - axisX, 
		pos.y.ToDouble() - axisY,
		xEnd - axisX, 
		yEnd - axisY, 
		Width(), 
		Height(),
		velocity );	
}

void Actor::MoveTo(double x, double y, double velocity, char *relativeActor, char *obstacleActor, int moveType)
{
	//(x, y) in relativeActor coordinates	
	//relativeActor: none, parent, collide, creator, mouse, any actor in game
	//Don't need to use moveType. If there is an obstacleActor must use PathFinder

	//Get the relative actor
	Actor *relative = NULL;
	bool bMouse = false;

	if(!relativeActor) return;
	if(flags.IsSet(FLAG_NET_OPTIMIZED_PRECISION) && !CanExecuteActionLocally()) return;


	
	if(strcmp(relativeActor, S_EVENT_ACTOR) == 0)
	{
		relative = Action::getActualEventActor();
	}
	else if(strcmp(relativeActor, S_COLLIDE_ACTOR) == 0)
	{
		relative = Action::getActualCollideActor();
	}
	else if(strcmp(relativeActor, S_PARENT_ACTOR) == 0)
	{
		relative = getParent();
	}
	else if(strcmp(relativeActor, S_CREATOR_ACTOR) == 0)
	{
		relative = getCreator();
	}
	else if(strcmp(relativeActor, S_NONE) == 0 || 
		relativeActor[0] == 0 //Old ged files
		)
	{
		relative = GameControl::Get()->GetAxis();
	}
	else if(strcmp(relativeActor, S_MOUSE) == 0)
	{
		bMouse = true;
	}
	else
	{
		relative = GameControl::Get()->GetActor(relativeActor, true, false, false);
	}

	//From relative to screen coordinates
	int sx, sy;
	
	if(!bMouse)
	{		
		if(IS_VALID_ACTOR(relative))
		{
			KrVector2T< GlFixed > screen;
			KrImage *pRelativeImage = relative->getImage();
			
			if(pRelativeImage) pRelativeImage->ObjectToScreen(x, y, &screen);
			
			sx = screen.x.ToInt();
			sy = screen.y.ToInt();
		}
		else
		{
			return;
		}
	}
	else
	{
		sx = GameControl::Get()->getMouseX() + x;
		sy = GameControl::Get()->getMouseY() + y;
	}

	KrImage *pParentImage = getParent()->getImage();
	if(!pParentImage) return;		 

	//Remove the path, if any
	//Don't remove here (Solve 'MoveTo Obstacle Draw Actor.ged' bug)
	/*SetPath("", BOTH_AXIS);

	delete pathFinder;
	pathFinder = NULL;*/

	velocity = fabs(velocity);
	if(velocity == 0.0) velocity = 1.0;

	//To actor coordinates
	KrVector2T< GlFixed > object;
	pParentImage->ScreenToObject(sx, sy, &object);		

	//(x, y) in actor coordinates
	x = object.x.ToDouble();
	y = object.y.ToDouble();

	//Avoid actor
	if(obstacleActor && strlen(obstacleActor) > 0 && strcmp(obstacleActor, NO_SELECTION) != 0)
	{
		//Requisite move with screen coordinates
		PathFinderRequisition(obstacleActor, sx, sy, velocity);		

		//Return here to avoid move if is not possible path
		return;
	}
	else
	{
		//Line motion		

		//Remove the path, if any
		SetPath("", BOTH_AXIS);

		delete pathFinder;
		pathFinder = NULL;
	
		//Calcule direction		
		double angleDeg = direction(scriptVars->x, scriptVars->y, x, y); 

		//Special cases: more precision
		if(angleDeg == 0.0)
		{
			scriptVars->xvelocity = velocity;
			scriptVars->yvelocity = 0.0;
		}
		else if(angleDeg == 90.0)
		{
			scriptVars->xvelocity = 0.0;
			scriptVars->yvelocity = -velocity;
		}
		else if(angleDeg == 180.0)
		{
			scriptVars->xvelocity = -velocity;
			scriptVars->yvelocity = 0.0;
		}
		else if(angleDeg == 270.0)
		{
			scriptVars->xvelocity = 0.0;
			scriptVars->yvelocity = velocity;
		}
		else
		{
			scriptVars->xvelocity = velocity*cos(angleDeg*Script::PI/180.0);
			scriptVars->yvelocity = -velocity*sin(angleDeg*Script::PI/180.0);
		}
	}

	xstop = x;
	ystop = y;
	flags.Set(FLAG_MOVETO);
}

void Actor::SetPathFinder(Path *_path)
{
	if(_path)
	{
		//Path ok
		KrImage *pImage = getImage();
		pathX = pathY = pathFinder =  _path;
		SetPathIni(pImage->X(), pImage->Y());

		flags.Set(FLAG_MOVETO);
	}
	else
	{
		//Solve 'MoveTo Obstacle Draw Actor.ged' bug
		//Remove the path, if any
		SetPath("", BOTH_AXIS);

		delete pathFinder;
		pathFinder = NULL;
	}
}

void Actor::CheckMoveTo()
{
	if(!flags.IsSet(FLAG_MOVETO)) return;

	if(!pathX && //Path finder moves are checked in the OnPathFinish function
		(((scriptVars->xvelocity > 0 && scriptVars->x >= xstop) || 
	   (scriptVars->yvelocity > 0 && scriptVars->y >= ystop) || 
	   (scriptVars->xvelocity < 0 && scriptVars->x <= xstop) || 
	   (scriptVars->yvelocity < 0 && scriptVars->y <= ystop))))
	{
		//Call Move Finish event
		OnMoveFinish();
	}
}

bool Actor::IsChild(Actor *_parent, Actor *child)
{
	while(child)
	{
		if(child == _parent)
			return true;

		child = child->getParent();
	}

	return false;
}

void Actor::SetActorScale(double scale)
{
	KrImage *pImage = getImage();

	if(pImage)
	{
		pImage->SetScale(scale, scale);
		pImage->CalcTransform();
		
		scriptVars->width = Width();
		scriptVars->height = Height();
	}
}


void Actor::CenterOnScreen()
{	
	Axis *axis = GameControl::Get()->GetAxis();
	KrImage *pAxisImage = axis->getImage();
	if(!pAxisImage) return;

	int x = axis->getScreenWidth()/2, 
		y = axis->getScreenHeight()/2;


	//Works even if actor has a parent
	if(type == REGION_ACTOR || type == REGION_ACTOR_FILLED || type == CANVAS)
	{
		x -= Width()/2;
		y -= Height()/2;
	}

	SetPosScreen(x, y);
	SetPathIni(scriptVars->x, scriptVars->y);


	//Don't works if actor has a parent
	/*KrVector2T< GlFixed > object;
	pAxisImage->ScreenToObject(x, y, &object, 0);

	if(type == REGION_ACTOR || type == REGION_ACTOR_FILLED || type == CANVAS)
	{
		object.x -= Width()/2;
		object.y -= Height()/2;
	}

	SetPos(object.x.ToInt(), object.y.ToInt());
	SetPathIni(object.x.ToInt(), object.y.ToInt());*/
}

const KrRect& Actor::Bounds()
{
	KrImage *pImage = getImage();
	if(!pImage)
	{
		static KrRect rect;
		rect.Set(0, 0, 0, 0);

		return rect;
	}

	if(textActor || tile)
		return pImage->CompositeBounds();//textActor->Bounds(window);
	
	return pImage->Bounds();
}

bool Actor::UpdateRegionLoad()
{
	//Assign default region to this actor

	if(flags.IsSet(FLAG_VIEW) || flags.IsSet(FLAG_REGION) || !flags.IsSet(FLAG_RUNNING) || !RegionLoad::getDefaultRegion() || !GameControl::Get()->getGameMode())
		return false;

	ownerRegions.Add(RegionLoad::getDefaultRegion(), 1);
	RegionLoad::AddActorToDefaultRegion(this);
	
	//Solve the alpha8_debug.ged bug? (http://game-editor.com/forum/viewtopic.php?p=7444#7444)
	/*if(parent && parent != GameControl::Get()->GetAxis())
	{
		parent->UpdateRegionLoad(); 
	}*/

	return true;
}

void Actor::SetRegionsFrom(Actor *actor)
{
	if(actor && actor->getRegionCount())
	{
		//Remove old regions
		MapRegionsIterator it(ownerRegions);
		it.Begin();
		
		while(!it.Done())
		{
			RegionLoad *pRegion = *it.Key();
			if(pRegion)
			{
				pRegion->RemoveActor(this);
			}

			it.Next();
		}
		
		ownerRegions.Clear();
		
		//Add new regions
		MapRegionsIterator it1(actor->GetOwnerRegions());
		it1.Begin();
		
		while(!it1.Done())
		{
			RegionLoad *pRegion = *it1.Key();
			if(pRegion && pRegion->IsRegionInView())
			{
				ownerRegions.Add(pRegion, 1);
				pRegion->AddActor(this);
			}

			it1.Next();
		}

		if(!ownerRegions.size())
		{
			//No visible regions.
			//Keep the default region
			ownerRegions.Add(RegionLoad::getDefaultRegion(), 1);
			RegionLoad::AddActorToDefaultRegion(this);
		}
	}
}


bool Actor::OnSlowCanvasHitTest(int x, int y)
{
	return false;
}

const gedString *Actor::OnActorTip()
{
	return NULL;	
}

bool Actor::CanExecuteAllActorsActivationEventAction(char *fromActor)
{
	//Return true only when receive Activation Events from all actors connected to him

	mapFromActivationEventActors.Add(fromActor, 1);
	if(mapFromActivationEventActors.size() == totalConnectedActors)
	{
		mapFromActivationEventActors.Clear(); //Reset
		return true;
	}

	return false;
}

int Actor::GetArea()
{
	if(type == REGION_ACTOR || type == REGION_ACTOR_FILLED || type == CANVAS)
		return Width()*Height();

	if(tile)
		return tile->GetArea();
	
	if(type == SPRITE && sprite)
		return sprite->GetAction()->GetFrame(sprite->Frame())->getArea();

	//view...
	return 0;
}

void Actor::IncreaseActorStruct(int newSize)
{
	structActorSize = newSize;

	scriptVars = (struct stActorVars *)realloc(scriptVars, structActorSize);
	scriptVarsAnt = (struct stActorVars *)realloc(scriptVarsAnt, structActorSize);
}




void Actor::NotifyMouseCoord(int x, int y)
{
	if(change)
		change->OnMouseMove(x, y);
}

void Actor::NotifyMouseButtonUp()
{
	if(change)
		change->OnMouseButtonUp(0, 0, 0);
}

void Actor::RegionActorToggleVisibility()
{
	bShowRegions = !bShowRegions;
	
	MapActorIterator it(mapActors);
	for(it.Begin(); !it.Done(); it.Next())
	{
		ListActor *listActor = it.Value();
		for(int il = 0; listActor && il < listActor->Count(); il++)
		{
			Actor *actor = (*listActor)[il];
			if(actor->EditMode() && (actor->type == REGION_ACTOR || actor->type == REGION_ACTOR_FILLED))
			{
				KrImage *pImage = actor->getImage();
				if(pImage) pImage->SetVisible(bShowRegions);
			}
		}
	}
}

void Actor::DrawSetPen(int r, int g, int b, double transp, int pensize)
{
	if(type != CANVAS) return;

	if(!drawInfo) drawInfo = new DrawInfo();

	if(r < 0) r = 0;
	if(g < 0) g = 0;
	if(b < 0) b = 0;
	if(transp < 0.0) transp = 0.0;
	if(pensize < 1) pensize = 1;

	if(r > 255) r = 255;
	if(g > 255) g = 255;
	if(b > 255) b = 255;
	if(transp > 1.0) transp = 1.0;
	
	drawInfo->color.Set(r, g, b, 255 - 255.0*transp);
	drawInfo->pensize = pensize;
}

void Actor::DrawMoveTo(int x, int y)
{
	if(type != CANVAS) return;

	if(!drawInfo) drawInfo = new DrawInfo();

	
	drawInfo->x = x;
	drawInfo->y = y;
}

void Actor::DrawLineTo(int x, int y)
{
	if(type != CANVAS) return;

	if(!drawInfo) drawInfo = new DrawInfo();


	KrRGBA *pixels = getCanvasResource()->Pixels();	
	
	
	KrPaintInfo paintInfo(pixels, Width(), Height());
	KrRect clip;
	clip.Set(0, 0, paintInfo.width-1, paintInfo.height-1);
	
	SlowCanvas::Line(&paintInfo, clip, x, y, drawInfo->x, drawInfo->y, drawInfo->color, drawInfo->color, drawInfo->pensize);
	
	/*KrRect r;
	r.Set(min(x, drawInfo->x), min(y, drawInfo->y), max(x, drawInfo->x), max(y, drawInfo->y));
	r.EdgeAdd(20);
	engine->InvalidateRectangle(r);*/
	

	drawInfo->x = x;
	drawInfo->y = y;

	canvasResource->Refresh();
}

void Actor::DrawErase(int r, int g, int b, double transp)
{
	if(type != CANVAS) return;

	if(!drawInfo) drawInfo = new DrawInfo();

	if(r < 0) r = 0;
	if(g < 0) g = 0;
	if(b < 0) b = 0;
	if(transp < 0.0) transp = 0.0;
	
	if(r > 255) r = 255;
	if(g > 255) g = 255;
	if(b > 255) b = 255;
	if(transp > 1.0) transp = 1.0;
	
	//Create a tansparent actor
	KrRGBA *pixels = canvasResource->Pixels(), color;
	
	color.Set(r, g, b, 255 - 255.0*transp);
	
	//Fill background
	int i, width = Width(), height = Height();
	for(i = 0; i < width*height; i++)
	{
		pixels[i] = color;
	}

	canvasResource->Refresh();
}

void Actor::DrawPutPixel(int x, int y)
{
	if(type != CANVAS) return;

	if(!drawInfo) drawInfo = new DrawInfo();

	int width = Width(), height = Height();
	KrRGBA *pixels = canvasResource->Pixels();
	
	int start = -(drawInfo->pensize >> 1), xi, yj;
	int end = start + drawInfo->pensize;
	for(int i = start; i < end; i++)
	{
		xi = x + i;

		for(int j = start; j < end; j++)
		{
			yj = y + j;

			if( xi >= 0 && xi < width &&
				yj >= 0 && yj < height )
			{
				pixels[ yj*width + xi ] = drawInfo->color;				
			}
		}
	}

	//if(drawInfo->pensize > 1)  	
	{		

		/* Don't works!

		if(x+1 < width) { x++; pixels[ y*width + x ] = drawInfo->color; } else return;
		if(y+1 < height) { y++; pixels[ y*width + x ] = drawInfo->color; } else return;
		if(x-1 >= 0) { x--; pixels[ y*width + x ] = drawInfo->color; } else return;
		
		if (drawInfo->pensize > 2) 
		{
			if(x+2 < width) { x += 2;	pixels[ y*width + x ] = drawInfo->color; } else return;
			if(y+1 < height) { y++; pixels[ y*width + x ] = drawInfo->color; } else return;
			if(y+1 < height) { y++; pixels[ y*width + x ] = drawInfo->color; } else return;
			if(x-1 >= 0) { x--; pixels[ y*width + x ] = drawInfo->color; } else return;
			if(x-1 >= 0) { x--; pixels[ y*width + x ] = drawInfo->color; } else return;
			
			if (drawInfo->pensize > 3) 
			{	// expecting 4 ... 9	
				
				if(x+3 < width) { x += 3;	pixels[ y*width + x ] = drawInfo->color; } else return;
				if(y+1 < height) { y++; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(y+1 < height) { y++; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(y+1 < height) { y++; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(x-1 >= 0) { x--; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(x-1 >= 0) { x--; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(x-1 >= 0) { x--; pixels[ y*width + x ] = drawInfo->color; } else return;
			}
			
			if (drawInfo->pensize > 7) 
			{	// who knows	
				
				if(x+4 < width) { x += 4;	pixels[ y*width + x ] = drawInfo->color; } else return;
				if(y+1 < height) { y++; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(y+1 < height) { y++; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(y+1 < height) { y++; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(y+1 < height) { y++; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(x-1 >= 0) { x--; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(x-1 >= 0) { x--; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(x-1 >= 0) { x--; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(x-1 >= 0) { x--; pixels[ y*width + x ] = drawInfo->color; } else return;
			}

			if (drawInfo->pensize > 12) 
			{	// who knows	
				
				if(x+5 < width) { x += 5;	pixels[ y*width + x ] = drawInfo->color; } else return;
				if(y+1 < height) { y++; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(y+1 < height) { y++; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(y+1 < height) { y++; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(y+1 < height) { y++; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(y+1 < height) { y++; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(x-1 >= 0) { x--; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(x-1 >= 0) { x--; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(x-1 >= 0) { x--; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(x-1 >= 0) { x--; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(x-1 >= 0) { x--; pixels[ y*width + x ] = drawInfo->color; } else return;
			}

			if (drawInfo->pensize > 15) 
			{	// who knows
				
				if(x+6 < width) { x += 6;	pixels[ y*width + x ] = drawInfo->color; } else return;
				if(y+1 < height) { y++; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(y+1 < height) { y++; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(y+1 < height) { y++; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(y+1 < height) { y++; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(y+1 < height) { y++; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(y+1 < height) { y++; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(x-1 >= 0) { x--; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(x-1 >= 0) { x--; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(x-1 >= 0) { x--; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(x-1 >= 0) { x--; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(x-1 >= 0) { x--; pixels[ y*width + x ] = drawInfo->color; } else return;
				if(x-1 >= 0) { x--; pixels[ y*width + x ] = drawInfo->color; } else return;
			}
			
		}*/
	}

	canvasResource->Refresh();
}

void Actor::DrawSaveCanvas()
{
	if(type != CANVAS) return;

	canvasResource->SaveBuffer(1);
}

void Actor::DrawLoadCanvas()
{
	if(type != CANVAS) return;

	canvasResource->RestoreBuffer(1);

	canvasResource->Refresh();
}

void Actor::RemoveTile()
{
	if(tile)
	{
		delete tile;
		tile = NULL;

		if(NumAnimations())
		{
			SetAnimation(AnimationName(2));
		}
		else
		{
			SetAnimation(AnimationName(0));
		}
	}
}


void Actor::InitNetActor()
{
	//Initialize this actor on the local machine and send to the others
	if(!flags.IsSet(FLAG_NETACTOR) || owner != UNASSIGNED_SYSTEM_ADDRESS || bInContructionCallBack || !GameControl::Get()->RequestNetworkStart()) return;

	owner = GameControl::Get()->GetOwnerMachine();
	ownerShipRequestTime = RakNet::GetTime();

#ifdef DEBUG
	GLOUTPUT("+++ Actor::InitNetActor: %s, %s\n", getCloneName(), owner.ToString(true));
#endif
}


bool Actor::CanExecuteActionLocally()
{	
	if(
		flags.IsSet(FLAG_NETACTOR)
#ifndef STAND_ALONE_GAME
		&& GameControl::Get()->getGameMode()
#endif
		)
	{
		if(owner != networkIDManager.GetExternalSystemAddress() && owner != UNASSIGNED_SYSTEM_ADDRESS)
		{
			//This machine is not controller for this actor
			return false;
		}
	}

	//Always return true, if not a net actor
	return true;
}

bool Actor::CanSendRemoteAction()
{
#ifndef STAND_ALONE_GAME
	if(!GameControl::Get()->getGameMode()) return true;
#endif

	//Can send remote actions only if have an owner and is the local machine
	return owner == networkIDManager.GetExternalSystemAddress() && owner != UNASSIGNED_SYSTEM_ADDRESS;
}

bool Actor::UpdateOwner(const SystemAddress& oldAddr, const SystemAddress& newAddr)
{	
	//Updates the actor owner address if the oldAddr is the controller
	//or if the oldAddr is unassigned (no anterior connection).
	//This function is called when the local address changes.
	//Don't change the addres if not have the ownership

	if(!flags.IsSet(FLAG_NETACTOR)) return false;

	if(owner == oldAddr || oldAddr == UNASSIGNED_SYSTEM_ADDRESS)
	{
		owner = newAddr;

		//Don't update the ownership timestamp
		//ownerShipRequestTime = RakNet::GetTime();

#ifdef DEBUG
	GLOUTPUT("+++ Actor::UpdateOwner: %s, %s to %s\n", getCloneName(), oldAddr.ToString(true), owner.ToString(true));
#endif

		return true;
	}

#ifdef DEBUG
	GLOUTPUT("+++ Actor::UpdateOwner: denied, %s, %s to %s\n", getCloneName(), oldAddr.ToString(true), owner.ToString(true));
#endif

	return false;
}

bool Actor::RequestActorOwnership(const SystemAddress& newOwner, const RakNetTime &newRequestTime)
{	
	//If the owner is unassigned or newRequestTime is older than current owner request,
	//can change the ownership

	if(!flags.IsSet(FLAG_NETACTOR)) 
	{
#ifndef STAND_ALONE_GAME
		gedString msg(gedString("\
In order to use the RequestActorOwnership function, the actor must have the 'Net Actor' attribute.\n\
Please, set the net actor attribute for the actor '") + gedString(getActorName()) + "' to use this function.");

		Script::myDisplay(msg.getCharBuf());
#endif
		return false;
	}

	if(owner == newOwner && newOwner != UNASSIGNED_SYSTEM_ADDRESS)
	{
		//Don't update timestamp
		return true;
	}
	else if(owner == UNASSIGNED_SYSTEM_ADDRESS || newRequestTime < ownerShipRequestTime)
	{	
#ifdef DEBUG
		GLOUTPUT("+++ Actor::RequestActorOwnership: %s, %s %s to %s\n", getCloneName(), (newRequestTime < ownerShipRequestTime)?"oldest resquest, ":"", owner.ToString(true), newOwner.ToString(true));
#endif

		owner = newOwner;
		ownerShipRequestTime = newRequestTime;

		if(flags.IsSet(FLAG_NET_OPTIMIZED_PRECISION))
		{
			if(!CanExecuteActionLocally())
			{
				//Disable automatic frame change
				framesToAnim = DISABLE_ANIMATION;

				//Remove paths
				delete pathFinder;
				pathFinder = NULL;
				pathX = pathY = NULL;
				flags.Clear(FLAG_MOVETO);
			}
			else
			{
				framesToAnim = ENABLE_ANIMATION;
			}
		}

		return true;
	}

#ifdef DEBUG
		GLOUTPUT("+++ Actor::RequestActorOwnership: denied %s, %s to %s\n", getCloneName(), owner.ToString(true), newOwner.ToString(true));
#endif
	return false;
}

bool Actor::ReleaseOwnership()
{	
	if(!flags.IsSet(FLAG_NETACTOR)) return false;

	if(owner == GameControl::Get()->GetOwnerMachine())
	{
		//Give the opportunity to any machine request the ownership

#ifdef DEBUG
		GLOUTPUT("+++ Actor::ReleaseOwnership: %s, %s\n", getCloneName(), owner.ToString(true));
#endif

		owner = UNASSIGNED_SYSTEM_ADDRESS;
		ownerShipRequestTime = (RakNetTime)-1;

		return true;
	}

#ifdef DEBUG
		if(owner != UNASSIGNED_SYSTEM_ADDRESS) GLOUTPUT("+++ Actor::ReleaseOwnership: denied %s, %s\n", getCloneName(), owner.ToString(true));
#endif

	return false;
}


void Actor::WriteState(RakNet::BitStream& out)
{
	//Don't need to write the name end clone index

	/*
	- Usar delta compression para atualizar estados 
		
			- jdiff
			- xdelta 
			- MSG_WriteDeltaPlayerstate, SV_WriteSnapshotToClient (quake3ce)
			- Artigo sobre delta compression no livro MMGD
	*/

	KrImage *pImage = getImage();

	//Flags
	out.Write(flags.ToU32());	

	//Parent
	if(parent && parent != GameControl::Get()->GetAxis())
	{
		gedString sParentName(NO_SELECTION);
		out.Write(true);

		if(mapActors.FindString(parent->getActorName())->Count() > 1)
		{
			sParentName = parent->getCloneName(); //solve the issue 119
		}
		else
		{
			sParentName = parent->getActorName();
		}

		RakNet::StringTable::Instance()->EncodeString(sParentName.c_str(), ACTOR_NAME, &out);
	}
	else
	{
		out.Write(false);
	}

	//Strings	
	RakNet::StringTable::Instance()->EncodeString(actualSequence.c_str(), ACTOR_NAME, &out);	
	RakNet::StringTable::Instance()->EncodeString(creatorCloneName.c_str(), ACTOR_NAME, &out);
	
	//Others
	out.Write(framesToAnim);	
	out.Write(eventControl);
	out.Write(followMouseAxis);
	out.Write(xstop);
	out.Write(ystop);
	out.Write(pImage->ZDepth());
	out.Write(pImage->getVisible());
	
	//Owner
	out.Write(sizeof(SystemAddress));
	out.Write((char *)&owner, sizeof(SystemAddress));

	out.Write(ownerShipRequestTime);

	
	if(!flags.IsSet(FLAG_NET_OPTIMIZED_PRECISION))
	{
		//Paths
		gedString sPathX(NO_SELECTION), sPathY(NO_SELECTION);

		out.Write(pathIniX);
		out.Write(pathIniY);
		out.Write(pathDirectionX);
		out.Write(pathDirectionY);

		if(pathX) sPathX = pathX->getName();
		if(pathY) sPathY = pathY->getName();

		RakNet::StringTable::Instance()->EncodeString(sPathX.c_str(), ACTOR_NAME, &out);
		RakNet::StringTable::Instance()->EncodeString(sPathY.c_str(), ACTOR_NAME, &out);

		//Path finder
		if(pathFinder)
		{
			SDL_RWops *src = SDL_RWFromMem(NULL, 0);
			if(src)
			{
				out.Write(true);

				//Save to src
				pathFinder->Save(src);

				//Write to stream
				int size = src->hidden.mem.stop - src->hidden.mem.base;
				char *memBase = (char *)src->hidden.mem.base;

				out.Write(size);
				out.Write(memBase, size);

				//Close memory file
				free(src->hidden.mem.base);
				SDL_RWclose(src);
			}
			else
			{
				out.Write(false);
			}
		}
		else
		{
			out.Write(false);
		}

		//Timer
		if(!mapTimer.empty())
		{
			MapTimerIterator it(mapTimer);

			for(it.Begin(); !it.Done(); it.Next())
			{
				stTimer *timer = *it.Key();
				if(timer)
				{
					stTimerEvent *p = action->mapTimerIdName[timer->timerID];
					if(p)
					{
						int time_remaining = timer->interval - (SDL_GetTicks() - timer->timerID->last_alarm);
						if(time_remaining < 0) time_remaining = 0;
						else if(time_remaining > timer->interval) time_remaining = timer->interval;

						out.Write(true);
						RakNet::StringTable::Instance()->EncodeString(p->name, ACTOR_NAME, &out);					
						out.Write(timer->interval);
						out.Write(timer->count);
						out.Write(time_remaining);
					}
				}
			}
		}

		//End of timers
		out.Write(false);	
	}

	//Script vars
	out.Write(structActorSize);
	out.Write((char *)scriptVars, structActorSize);
}

void Actor::ReadState(RakNet::BitStream& in)
{
	KrImage *pImage = getImage();
	int size, visible;
	U32 zdepth, fl;
	bool bValue;
	char name[ACTOR_NAME];

	RemoveTimers();
	SetPath(NO_SELECTION, BOTH_AXIS);

	//Flags
	in.Read(fl); flags.FromU32(fl); //Read flags before call SetActualValues
	SetActualValues(NEED_ALL);
	
	
	//Parent
	in.Read(bValue);
	if(bValue)
	{
		//Have parent
		RakNet::StringTable::Instance()->DecodeString(name, ACTOR_NAME, &in);
		Actor *parentActor = GameControl::Get()->GetActor(name, true, false, false);
		SetParent(parentActor);
	}
	else
	{
		RemoveParent();
	}
	

	//Animation
	RakNet::StringTable::Instance()->DecodeString(name, ACTOR_NAME, &in);
	SetAnimation(name);

	//Creator
	RakNet::StringTable::Instance()->DecodeString(name, ACTOR_NAME, &in);
	setCreator(name);
	
	//Others
	in.Read(framesToAnim);	
	in.Read(eventControl);
	in.Read(followMouseAxis);
	in.Read(xstop);
	in.Read(ystop);
	in.Read(zdepth); pImage->SetZDepth(zdepth);
	in.Read(visible); pImage->SetVisible(visible);
	
	//Owner
	in.Read(size);
	in.Read((char *)&owner, size);

	in.Read(ownerShipRequestTime);


	if(!flags.IsSet(FLAG_NET_OPTIMIZED_PRECISION))
	{
		//Paths
		in.Read(pathIniX);
		in.Read(pathIniY);
		in.Read(pathDirectionX);
		in.Read(pathDirectionY);

		RakNet::StringTable::Instance()->DecodeString(name, ACTOR_NAME, &in);
		if(strcmp(name, NO_SELECTION) != 0) pathX = GameControl::Get()->GetPath(name);

		RakNet::StringTable::Instance()->DecodeString(name, ACTOR_NAME, &in);
		if(strcmp(name, NO_SELECTION) != 0) pathY = GameControl::Get()->GetPath(name);

		//Path finder
		in.Read(bValue);
		if(bValue)	
		{
			in.Read(size);
			char *memBase = (char *)malloc(size);
			in.Read(memBase, size);

			SDL_RWops *src = SDL_RWFromMem(memBase, size);

			pathFinder = new Path();
			pathFinder->Load(src);

			pathX = pathY = pathFinder;
			flags.Set(FLAG_MOVETO);

			free(memBase);
			SDL_RWclose(src);	
		}

		//Timer
		in.Read(bValue);
		while(bValue)
		{
			int interval, count, time_remaining; 

			RakNet::StringTable::Instance()->DecodeString(name, ACTOR_NAME, &in);
			in.Read(interval);
			in.Read(count);
			in.Read(time_remaining);

			lastCreatedTimer = NULL;
			execCreateTimer(getCloneName(), name, interval);
			if(lastCreatedTimer)
			{
				lastCreatedTimer->count = count;			
				lastCreatedTimer->timerID->last_alarm = SDL_GetTicks() - (interval - time_remaining);			
			}

			//Next timer
			in.Read(bValue);
		}
	}

	//Script vars (read here after change the animation)
	in.Read(size);
	if(size != structActorSize) 
	{
		memset(scriptVars, 0, structActorSize);		
	}

	in.Read((char *)scriptVars, size);

	//After read
	flags.Clear(FLAG_CALLONCREATE);
	flags.Clear(FLAG_RUNNING);

	setRunning(true);

	//Other updates
	UpdateScriptChanges(NEED_ALL, true);
}

#ifndef STAND_ALONE_GAME

void Actor::OnButton(Button *button, int buttonId)
{
}

bool Actor::OnList(ListPop *list, int index, gedString &text, int listId)
{
	return true;
}

bool Actor::OnListButtonDown(ListPop *list, int listId)
{
	//Return false to cancel the list
	return true;
}

Button *Actor::AddButton(gedString text, 
			   int x, int y,
			   int width, int height,
			   int buttonId,
			   int red, int green, int blue,
			   const gedString spriteName,
			   gedString fontName)
{
	Button *button = new Button(text, 
			   x, y,
			   width, height,
			   buttonId,
			   this,
			   red, green, blue,
			   spriteName,
			   fontName);

	listButton.PushBack(button);

	return button;
}

ListPop *Actor::AddListPop(int x, int y,
			   int width, int height,
			   int listpopId, gedString buttonText)
{

	ListPop *list = new ListPop(x, y, width, height, listpopId, this, buttonText);
	listListPop.PushBack(list);

	return list;
}

ListDirectory *Actor::AddListDirectory(int x, int y,
			   int width, int height,
			   int listpopId, gedString buttonText)
{

	ListDirectory *list = new ListDirectory(x, y, width, height, listpopId, this, buttonText);
	listListDirectory.PushBack(list);

	return list;
}

EditBox *Actor::AddEditBox(int x, int y, int width, int height, bool multiLine, gedString fontName)
{
	EditBox *edit = new EditBox(x, y, width, height, multiLine, this, fontName);
	listEditBox.PushBack(edit);

	return edit;
}



Slide *Actor::AddSlide(int x, int y, int width, int height, int slideId)
{
	Slide *slide = new Slide(this, x, y, width, height, slideId);
	listSlide.PushBack(slide);

	return slide;
}

ListBox *Actor::AddListBox(int x, int y, int width, int height, int listId, gedString fontName)
{
	ListBox *list = new ListBox(x, y, width, height, listId, this, fontName);
	listListBox.PushBack(list);

	return list;
}

void Actor::OnEditEnter(EditBox *edit, int line)
{
}

void Actor::OnEditSelection(EditBox *edit)
{
}

void Actor::OnEditChar(EditBox *edit, SDLKey key, int line)
{
}

void Actor::OnSlide(Slide *slide, int slideId, double pos)
{

}

void Actor::Save(SDL_RWops *src)
{
	gedString name(scriptVars->name);
	name.Write(src);


	if(parent)
	{
		gedString sParentName;//(parent->getCloneIndex()?parent->getCloneName():parent->getActorName());

		if(mapActors.FindString(parent->getActorName())->Count() > 1)
		{
			sParentName = parent->getCloneName(); //solve the issue 119
		}
		else
		{
			sParentName = parent->getActorName();
		}

		sParentName.Write(src);
	}
	else
	{
		SDL_WriteLE16(src, 0);
	}	

	//Type
	SDL_WriteLE16(src, type);

	if(type == REGION_ACTOR || type == REGION_ACTOR_FILLED || type == CANVAS)
	{
		//Save region defination
		SDL_WriteLE32(src, Width());
		SDL_WriteLE32(src, Height());
	}

	if(type == CANVAS)
	{
		//Write data
		getCanvas()->Save(src);
	}
	
	//Animations
	Uint16 nAnimations = NumAnimations();
	SDL_WriteLE16(src, nAnimations);
	if(nAnimations)
	{
		//Error: save more than nAnimations when use sequences
		/*int i = 0;
		ListSequenceIterator it(mySequence->listSequence);
		for(it.Begin(); !it.Done(); it.Next())
		{
			Sequence *seq = it.Current();
			if(i >= 2)
			{
				Uint8 animationType = seq->getType();
				SDL_RWwrite(src, &animationType, sizeof(animationType), 1);

				if(animationType == ANIMATION_SEQUENCE)
				{
					KrAction *action = spriteResource->GetAction(seq->getAnimationName());
					action->setimgName(GameControl::Get()->AdjustFileName(action->getimgName()));
					action->Save(src);
				}
				else
				{
					seq->Save(src);
				}
			}

			i++;
		}*/

		//Works right
		Sequence *seq;
		for(int i = 2; i < nAnimations+2; i++)
		{
			gedString name = AnimationName(i, &seq);
			if(!seq) continue;

			Uint8 animationType = seq->getType();
			SDL_RWwrite(src, &animationType, sizeof(animationType), 1);
			
			if(animationType == ANIMATION_SEQUENCE)
			{
				gedString animKey(scriptVars->name);
				animKey += name;

				animKey.Write(src);

				/*if(!GameControl::Get()->getFastFile())
				{
					KrAction *action = spriteResource->GetAction(seq->getAnimationName());
					action->setimgName(GameControl::Get()->AdjustFileName(action->getimgName()));
					action->Save(src);
				}
				else if(!mapActorAnimation.Find1(animKey))
				{
					//Save only in first actor
					KrAction *action = spriteResource->GetAction(seq->getAnimationName());
					action->setimgName(GameControl::Get()->AdjustFileName(action->getimgName()));
					action->Save(src);

					mapActorAnimation.Add(animKey, *action);	
				}*/
			}
			else			
			{
				seq->Save(src);
			}
		}

		//Current action
		GetAnimation().Write(src);
	}	

	Uint8 tmp;
	SDL_RWwrite(src, &pathDirectionX, sizeof(pathDirectionX), 1);
	SDL_RWwrite(src, &pathDirectionY, sizeof(pathDirectionY), 1);
	SDL_WriteLE32(src, pathIniX);
	SDL_WriteLE32(src, pathIniY);

	tmp = flags.IsSet(FLAG_ENABLECOLLISIONDETECTION);
	SDL_RWwrite(src, &tmp, sizeof(Uint8), 1);

	tmp = flags.IsSet(FLAG_FORWARDANIMATION);
	SDL_RWwrite(src, &tmp, sizeof(Uint8), 1);

	tmp = flags.IsSet(FLAG_CREATEATSTARTUP)?1:0;
	SDL_RWwrite(src, &tmp, sizeof(Uint8), 1);

	tmp = flags.IsSet(FLAG_NETACTOR)?1:0;
	SDL_RWwrite(src, &tmp, sizeof(Uint8), 1);

	if(!flags.IsSet(FLAG_NETACTOR))
	{
		flags.Set(FLAG_NET_OPTIMIZED_PRECISION, false);
	}

	tmp = flags.IsSet(FLAG_NET_OPTIMIZED_PRECISION)?1:0;
	SDL_RWwrite(src, &tmp, sizeof(Uint8), 1);

	cursor.Save(src);	

	//Paths
	if(pathX)
	{
		pathX->getName().Write(src);
	}
	else
	{
		SDL_WriteLE16(src, 0);
	}

	if(pathY)
	{
		pathY->getName().Write(src);		
	}
	else
	{
		SDL_WriteLE16(src, 0);
	}

	//Text Actor
	if(textActor)
	{
		Uint8 hasText = 1;
		SDL_RWwrite(src, &hasText, sizeof(hasText), 1);

		//Save text
		textActor->GetText().Write(src);

		//Save font name
		GameControl::Get()->AdjustFileName(textActor->getFontName()).Write(src);
		
		//Save ini char and number of chars
		SDL_WriteLE16(src, textActor->getiniASCIIChar());
		SDL_WriteLE16(src, textActor->getnChars());

		//Save input data
		Uint8 inputText = textActor->IsEditable()?1:0;
		SDL_RWwrite(src, &inputText, sizeof(inputText), 1);

		if(inputText)
		{
			Uint8	hasBorder = textActor->HasBorder()?1:0,
					hasBackGround = textActor->HasBackground()?1:0,
					getFocus = textActor->HasGetFocus()?1:0;

			SDL_RWwrite(src, &hasBorder, sizeof(hasBorder), 1);
			SDL_RWwrite(src, &hasBackGround, sizeof(hasBackGround), 1);
			SDL_RWwrite(src, &getFocus, sizeof(getFocus), 1);

			SDL_WriteLE32(src, textActor->getBorderColor().all);
			SDL_WriteLE32(src, textActor->getCursorColor().all);
			SDL_WriteLE32(src, textActor->getBackGroundColor().all);
		}
	}
	else
	{
		Uint8 hasText = 0;
		SDL_RWwrite(src, &hasText, sizeof(hasText), 1);
	}

	//Set loaded vars here (solve the bug "For some reason the code on the AI spawn sometimes causes the player character to turn red or yellow" in ALPHA_1_1_4a.ged)
	//Save zdepth
	loadedZDepth = getImage()->ZDepth();
	SDL_WriteLE32(src, loadedZDepth);

	//Save transparency
	Uint8 alpha = getImage()->CTransform().Alpha();
	SDL_RWwrite(src, &alpha, sizeof(alpha), 1);
	loadedTransparency = (255 - alpha)/255.0;

	//Save full color
	loadedColor.all = getImage()->CTransform().m.all;
	SDL_WriteLE32(src, loadedColor.all); 

	if(tile && tile->NumTiles())
	{
		Uint8 hasTile = 1;
		SDL_RWwrite(src, &hasTile, sizeof(hasTile), 1);

		tile->Save(src);
	}
	else
	{
		Uint8 hasTile = 0;
		SDL_RWwrite(src, &hasTile, sizeof(hasTile), 1);
	}

	//Infinite
	Uint8 infinite = getInfinite();
	SDL_RWwrite(src, &infinite, sizeof(infinite), 1);

	//Clone index
	SDL_WriteLE32(src, getCloneIndex());

	//Total connected actors
	SDL_WriteLE32(src, totalConnectedActors);

	//Lock information for editing
	Uint8 lock = flags.IsSet(FLAG_LOCKACTOR);
	SDL_RWwrite(src, &lock, sizeof(lock), 1);

	if(!GameControl::Get()->getExportMode()) 
	{
		//Save description if not exporting
		description.Write(src);
	}

	//Final expansion indicator
	Uint8 expansion = 0;
	SDL_RWwrite(src, &expansion, sizeof(expansion), 1);
}

void Actor::SaveAnimationData(SDL_RWops *src)
{
	Uint16 nAnimations = NumAnimations();
	if(nAnimations)
	{
		Sequence *seq;
		for(int i = 2; i < nAnimations+2; i++)
		{
			gedString name = AnimationName(i, &seq);
			if(!seq) continue;
			
			if(seq->getType() == ANIMATION_SEQUENCE)
			{
				gedString animKey(scriptVars->name);
				animKey += name;
				
				animKey.Write(src);
				KrAction *action = spriteResource->GetAction(seq->getAnimationName());
				gedString imgName(action->getimgName());
				action->setimgName(GameControl::Get()->AdjustFileName(imgName));
				action->Save(src);

				//Copy the updates
				if(!GameControl::Get()->getFastFile())
				{
					gedString imgUpdateName(GetUpdateImageName(imgName));
					
					if(!imgUpdateName.empty())
					{
						GameControl::Get()->AdjustFileName(gedString("data") + DIR_SEP + imgUpdateName + "*.*");
					}
				}
				
			}
		}
	}	
}

gedString Actor::GetUpdateImageName(const gedString &animationName)
{
	//Get image name without extension or *
	gedString imgName(animationName);

	imgName = imgName.GetFileName();

	int i;
	if((i = imgName.find('*')) != gedString::npos)
	{
		imgName = imgName.substr(0, i);
	}
	else if((i = imgName.rfind('.')) != gedString::npos)
	{
		imgName = imgName.substr(0, i);
	}

	if(!imgName.empty())
	{
		imgName += "_mod_";
	}

	return imgName;
}

bool Actor::ReplaceFrame(const gedString &animationName, int frame, gedString imagePrefix)
{
	//Look for bmp or png images in the game data directory
	//with name:
	//image_name_mod_frame

	gedString gamePath(GameControl::Get()->getGamePath());

	if(gamePath.empty()) gamePath = GameControl::Get()->getTmpGamePath(); //Loading game

	if(!spriteResource || frame < 0 || gamePath.empty()) return false;
	if(animationName == ACTOR_GED_ICON || animationName == ACTOR_GED_TEXT) return false;

	KrAction *krAction = spriteResource->GetAction(animationName);
	if(!krAction || frame >= krAction->NumFrames()) return false;

	KrRle* rle = krAction->GetFrame( frame );
	if(!rle) return false;
	
	//Change to game data directory
	EditorDirectory changeDir(gamePath);
	chdir("data");

	gedString imgName(GetUpdateImageName(krAction->getimgName()));

	if(imgName.empty()) return false;

	imgName = imagePrefix + imgName;

	//Append frame number
	char buf[8];	
	sprintf(buf, "%04ld", frame);
	imgName += buf;

	//Load surface
	SDL_Surface *surface = NULL;
	surface = LoadSurface( (imgName + ".png").c_str(), false );
	if(!surface) surface = LoadSurface( (imgName + ".bmp").c_str(), false );

	SDL_ClearError();
	if(!surface) return false;
	
	KrRGBA transColor;
	bool bSolidImage = IsSolidImage(surface, transColor);	
			
	KrPaintInfo info( surface );
	
	//Set transparent color
	KrPainter painter( surface );
	KrRGBA transRgba;
	
	if(!bSolidImage)
	{
		painter.BreakPixel( 0, 0, &transRgba );	
	}
	else
	{
		transRgba = transColor;
	}
	
	info.SetTranparentColor(transRgba);	
				
	
	
	//Replace
	KrVector2 originalHotSpot = rle->OriginalHotSpot();
	

	rle->Clear();
	rle->Create( &info, 0, 0, surface->w, surface->h, originalHotSpot.x, originalHotSpot.y, 0, 0);		
	
	
	
	SDL_FreeSurface( surface );
	
	SDL_ClearError();
	return rle != NULL;
}


void Actor::DrawTile(const gedString& animationName, bool bSingle, bool bRandom)
{
	if(!tile)
	{
		tile = new Tile(this);		
	}

	tile->SetAnimation(animationName);
	tile->SetVisibleCursor(true);
	tile->SetSingleMode(bSingle);
	tile->SetRandom(bRandom);

	GameControl::Get()->SetTileMode(tile);

	SetAnimation(ACTOR_GED_TEXT);
}

void Actor::RemoveTileAnimation(const gedString& animationName)
{
	if(tile)
	{
		tile->RemoveAnimation(animationName);
		if(!tile->NumTiles())
		{
			if(tile == GameControl::Get()->GetTile()) 
			{
				GameControl::Get()->SetTileMode(NULL);
			}
			
			delete tile;
			tile = NULL;
		}
	}
}

ListString Actor::GetDescendantsList()
{
	//Get descendants list
	//Use format used by GameControl::PopulateActors
	ListString list;

	
	GlInsideNodeIt<KrImNode*> it = getImage()->ChildTreeIterator();
	it.Begin(); 
	while( !it.Done() )
	{
		KrImNode *node = it.CurrentData();
		if(!node->getIsText())
		{
			Actor *childActor = (Actor *)node->getParentActor();
			
			if(childActor && childActor != this && childActor->EditMode())
			{
				if(mapActors[childActor->getActorName()]->Count() > 1)
				{
					list.PushBack(gedString(childActor->getActorName()) + "," + childActor->getCloneName());
				}
				else
				{
					list.PushBack(childActor->getActorName());
				}

				list.Append(childActor->GetDescendantsList());
			}
		}

		it.Next();
	}

	return list;
}


#endif //#ifndef STAND_ALONE_GAME



