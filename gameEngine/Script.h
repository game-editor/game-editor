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


// Script.h: interface for the Script class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCRIPT_H__653A4BE2_7343_4E6A_9C2F_6ECBA0ECE501__INCLUDED_)
#define AFX_SCRIPT_H__653A4BE2_7343_4E6A_9C2F_6ECBA0ECE501__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "Action.h"

#include "gedString.h"
#include "dlmalloc.h"


extern "C"
{
void EiC_parseString(char * fmt,...);
}

#define VAR_DELIMIT '.'

#define S_VAR_X				"x"			
#define S_VAR_Y				"y"	
#define S_VAR_R				"r"
#define S_VAR_G				"g"
#define S_VAR_B				"b"
#define S_VAR_TRANSP		"transp"
#define S_VAR_PATHPOS		"pathpos"
#define S_VAR_ANIMPOS		"animpos"
#define S_VAR_ANIMINDEX		"animindex"
#define S_VAR_NFRAMES		"nframes"
#define S_VAR_ANGLE			"angle"
#define S_VAR_DIRETIONAL_VELOCITY			"directional_velocity"
#define S_VAR_XVELOCITY		"xvelocity"
#define S_VAR_YVELOCITY		"yvelocity"
#define S_VAR_XSCREEN		"xscreen"
#define S_VAR_YSCREEN		"yscreen"
#define S_VAR_XPREVIOUS		"xprevious"
#define S_VAR_YPREVIOUS		"yprevious"
#define S_VAR_TEXT			"text"
#define S_VAR_WIDTH			"width"
#define S_VAR_HEIGHT		"height"



//Global vars
#define S_VAR_FRAMES		"frame"
#define S_REAL_FPS			"real_fps"
#define S_VAR_XMOUSE		"xmouse"
#define S_VAR_YMOUSE		"ymouse"
#define S_VAR_MUSIC_VOLUME	"musicvol"

#define NEED_DIMENTIONS					1
#define NEED_ANGLE						(1 << 1)
#define NEED_DIRECTIONAL_VELOCITY		(1 << 2)
#define NEED_MOUSE_POSITION				(1 << 3)
#define NEED_RAND						(1 << 4)
#define NEED_ALL						0xFFFFFFFF


class Actor;
struct stActorVars;

typedef GlSList<Actor *> ListActors;  
typedef GlSListIterator<Actor *> ListActorsIterator;

#ifndef STAND_ALONE_GAME
typedef GlSList<Token> ListTokens;  
typedef GlSListIterator<Token> ListTokensIterator;

ListTokens GetTokens(gedString str);
#endif


class stTime
{
public:
	int sec;	//Seconds after minute (0 - 59)
	int min;	//Minutes after hour (0 - 59)
	int hour;	//Hours since midnight (0 - 23)
	
	int mday;	//Day of month (1 - 31)
	int mon;	//Month (1 - 12; January = 1)
	int year;	//Year (current year)
	
	int wday;	//Day of week (0 - 6; Sunday = 0)
	int yday;	//Day of year (0 - 365)
	
	unsigned long sec_utc;//Number of seconds elapsed since midnight (00:00:00), January 1, 1970 (coordinated universal time)
};

typedef GlMap< gedString, char, GlStringHash >		MapUsedActor;
typedef GlMapIterator< gedString, char, GlStringHash >		MapUsedActorIterator;

class ActorVarsContainer
{
public:
	ActorVarsContainer() {mem = NULL;}
	ActorVarsContainer(stActorVars *actorVars)
	{
		//Due to variable size of stActorVars
		mem = malloc(Actor::getStructActorSize());
		memcpy(mem, actorVars, Actor::getStructActorSize());
	}

	virtual ~ActorVarsContainer()
	{
		if(mem)
			free(mem);
	}

	void operator=(const ActorVarsContainer &copy)
	{
		if( &copy != this )
		{
			if(mem)
				free(mem);
			
			mem = malloc(Actor::getStructActorSize());
			memcpy(mem, copy.mem, Actor::getStructActorSize());
		}
	}

	stActorVars *get() {return (stActorVars *)mem;}

	void *mem;
};

/*typedef GlSList<int> StackInt;  //push = PushFront, pop = PopFront, top = Front
typedef GlSList<double> StackDouble;  //push = PushFront, pop = PopFront, top = Front
typedef GlSList<ActorVarsContainer> StackActorVars;  //push = PushFront, pop = PopFront, top = Front
typedef GlSList<stActorVars *> StackActorVarsPtr;  //push = PushFront, pop = PopFront, top = Front*/
typedef GlSList<ListActors> StackListActors;  //push = PushFront, pop = PopFront, top = Front



class Script  
{
public:
	
	void InvalidateActor(const char *actorName) const;
	void setAddedWithError(bool value) {bAddedWithError = value;}
	bool getAddedWithError() {return bAddedWithError;}

	bool getUseRand() {return (updateFlags & NEED_RAND)?true:false;}
	

#ifndef STAND_ALONE_GAME
	static void ChangePaths(gedString& code);
	ListSounds GetSoundList();
	ListString GetTimerList();
	static void PopulateFunctions(ListPop *list);
	static void PopulateVars(ListPop *list);
	static void PopulateActorVars(ListPop *list);
	static void PopulateLocalUserVars(ListPop *list);
	static MapUsedActor& getLocalVars() {return mapLocalUserVar;}
	static void RemoveLocalVar(const gedString& variable);
	
#endif //#ifndef STAND_ALONE_GAME

	static void LoadLocalUserVars(SDL_RWops *src, bool bMergeGames);

#ifndef STAND_ALONE_GAME
	static void SaveLocalUserVars(SDL_RWops *src);
	void Save(SDL_RWops *src);
	static void ReplacePath(gedString& code, gedString function, int nStringArg);
	static void UpdateMoveTo(gedString& code);
#endif //#ifndef STAND_ALONE_GAME


	static void InitActorFunctions();
	static void DestroyLocalUserVars();
	static void InitAuxVars();
	static bool CreateLocalUserVar(const gedString& varName, int type);
	static bool IsActorVariable(const gedString& varName);
	static bool IsVariableReadOnly(const gedString &varName);
	bool getParserOk() {return bParserOk;}
	void CallDirectExecution();
	static void Load(const gedString& name);
	void RestoreStaticData();
	void StoreStaticData();
	static double PI;

	bool Parser(bool bOnDemandLoad, bool bReportErrors = false);
	
	void Load(SDL_RWops *src, int version);
	
	static void Reset();
	bool Execute(Actor *eventActor, Actor *collideActor
#ifndef STAND_ALONE_GAME
		, stAction *_currentAction = NULL
#endif
		);
	void SetFunctionName(gedString name);
	bool Parser(const gedString& str, bool bConditional = false, bool bOnDemandLoad = false, bool bEditScript = false);
	static const gedString& GetError();
	static void SetError(gedString _error) {error = _error;}
	static void ClearError() {error.clear();}
	void EditOff();
	void EditOn();
	static void RemoveActor(Actor *actor);
	static void AddActor(Actor *actor, const char *actorName = NULL);
	static void RemoveSymbol(const char *symbol, bool bTestMacros = false);
	void Init();
	Script();
	virtual ~Script();
	static void myDisplay(char *msg);

	const gedString& getCode() {return code;}
	const gedString& getFunctionName() {return functionName;}
	static bool getConditionalResult() {return conditionalResult != 0;}
	static void setConditionalResult(bool value) {conditionalResult = value?1:0;}
	static ListActors *getListCreateActors() {return &listCreateActors;}
	static stActorVars *getEventActorVars() {return eventActorVars;}
	static stActorVars *getCollideActorVars() {return collideActorVars;}

	static struct stActorVars *invalidScriptActor; //Don't return null, return &invalidScriptActor
	static Uint8 keyState[SDLK_LAST + 1];

private:	
	static void UpdateLocalUserVars();
	void AddActorVars(Actor *eventActor, Actor *collideActor);

	gedString functionName;
	gedString code;

	//Don't save

	///////////////////////////////////////////////////////
	//Static execute control
	static stActorVars *eventActorVars, *collideActorVars;
	static int conditionalResult;
	static int frame, mousex, mousey, real_fps;
	static double musicVol;
	static ListActors listCreateActors;
	

	//Stack control
	/*static StackInt				stackInt;
	static StackDouble			stackDouble;
	static StackActorVars		stackActorVars;
	static StackActorVarsPtr	stackActorVarsPtr;*/
	static StackListActors		stackListActors;
	///////////////////////////////////////////////////////

	gedString GenerateFunctionName();

	
	double musicVolAnt;
	static gedString error;

	ListString usedActor;	
	bool bParserOk;	
	bool bAddedWithError;
	U32 updateFlags; //Speed up code SetActualValues

	static struct stActorVars *fakeEditActor; //For parser only

	void *functionAddress;
	
	//Local vars
	static MapUsedActor mapLocalUserVar;	
};

//Public functions
double direction(double x1, double y1, double x2, double y2);

#endif // !defined(AFX_SCRIPT_H__653A4BE2_7343_4E6A_9C2F_6ECBA0ECE501__INCLUDED_)
