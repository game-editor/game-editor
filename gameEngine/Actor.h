// Actor.h: interface for the Actor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACTOR_H__D0F60C29_A8DF_4646_B471_72EC53CB64D9__INCLUDED_)
#define AFX_ACTOR_H__D0F60C29_A8DF_4646_B471_72EC53CB64D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SDL.h"
#include "SDL_image.h"
#include <stdlib.h>
#include "engine/kyra.h"
#include "maksKyra.h"
#include "dlmalloc.h"

#include "../RakNet/Source/RakPeerInterface.h"

#ifndef STAND_ALONE_GAME
#	define ACTOR_USES_VIRTUAL_FUNCTIONS //Use only in the editor. Tested with "collision benchmark.ged"
#endif


#define DELETE_ME 123456									//Security way to actor delete
#define REMOVE_ME_FROM_HANDLED_COLLISION_MAP	(DELETE_ME+1)	//Secure collision map remove
#define ADD_ME_TO_WORKING_MAP					(DELETE_ME+2)	//Secure working map add
#define REMOVE_ME_FROM_WORKING_MAP				(DELETE_ME+3)	//Secure working map remove
#define LISTPOPSELECT_HIDED						(DELETE_ME+4)
#define WAITFORFRAME_CLOSED						(DELETE_ME+5)
#define LIST_ACTOR_VARIABLES					(DELETE_ME+6)
#define CLICK_ON_VARIABLE_SELECTOR				(DELETE_ME+7)
#define ADD_TO_PANEL_ACTORS						(DELETE_ME+8)
#define REMOVE_SELECTED_ACTOR					(DELETE_ME+9)
#define EXECUTE_PATH_FINDER						(DELETE_ME+10)
#define ADD_ME_TO_HANDLED_COLLISION_MAP			(DELETE_ME+11)	//Secure collision map add

#define NAME_LIMIT		32
#define PATH_LIMIT		256

#define ACTOR_NAME		(NAME_LIMIT+1)
#define CLONE_NAME		(NAME_LIMIT+10)

#define ACTOR_VARIABLES "Actor variables"

extern KrEngine *engine;

#define DISABLE_ANIMATION		0
#define ENABLE_ANIMATION	   -1

#define INVALID_ANGLE		   -1.12345



#define PARENT_NOT_FOUND	1
#define CYCLICAL_PARENT		2
#define PARENT_OK			3

#define RANDOM_TIMER		1
#define PERIODIC_TIMER		2 
#define REPEATE_FOREVER		-10

extern gedString VIEW_NAME;

#define CORNER_SIDE 15

class Actor;
class Sequence;
class GeoPathFinder;

#define FLAG_ACTORINVIEW				1
#define FLAG_CALCULATEDXVELOCITY		(1 << 1)
#define FLAG_CALCULATEDYVELOCITY		(1 << 2)
#define FLAG_CALLONCREATE				(1 << 3)
#define FLAG_CREATEATSTARTUP			(1 << 4)
#define FLAG_DELETECONFIRMATION			(1 << 5)
#define FLAG_EDIT						(1 << 6)
#define FLAG_EDITANIM					(1 << 7)
#define FLAG_ENABLECOLLISIONDETECTION	(1 << 8)	//Enable collision detection for this actor
#define FLAG_FORWARDANIMATION			(1 << 9)
#define FLAG_HANDLECOLLISION			(1 << 10)	//To control handled collision map with work actors
#define FLAG_HASSHADOWS					(1 << 11)
#define FLAG_LOCKACTOR					(1 << 12)
#define FLAG_MOVETO						(1 << 13)
#define FLAG_ONCREATECALLED				(1 << 14)	//Make sure don't call OnCreate twice
#define FLAG_PANEL						(1 << 15)
#define FLAG_PENDINGSETTEXT				(1 << 16)	//Optimize SetText (bsfgc010a.ged more fast)
#define FLAG_RANDOMPATH					(1 << 17)
#define FLAG_REGION						(1 << 18)
#define FLAG_RUNNING					(1 << 19)	//This actor was created in game mode	
#define FLAG_SETACTUALVALUES			(1 << 20)	//Don't set twice (Solve the issue 92)
#define FLAG_SETANTERIORPOSITION		(1 << 21)
#define FLAG_VIEW						(1 << 22)
#define FLAG_WORKING					(1 << 23)	//This is into view
#define FLAG_NETACTOR					(1 << 24)
#define FLAG_CALLONDESTROY				(1 << 25)
#define FLAG_NET_OPTIMIZED_PRECISION	(1 << 26)	//Don't execute in remote machine: paths, timers, Move To, frame change. Always send: animpos, x, y
#define FLAG_NET_UNLINKED				(1 << 27)


#ifdef ACTOR_USES_VIRTUAL_FUNCTIONS
#	define virtual1 virtual
#else
#	define virtual1
#endif

class stTimer
{
public:
	stTimer();
	virtual ~stTimer();

	Actor *actor;
	SDL_TimerID timerID;
	int interval, minRandomInterval;
	int count; //REPEATE_FOREVER or > 0 
	char type; //RANDOM_TIMER, PERIODIC_TIMER
};


class stAction;
class RegionLoad;

typedef GlMap< RegionLoad *, char, GlPointerHash<RegionLoad *> >		MapRegions;
typedef GlMapIterator< RegionLoad *, char, GlPointerHash<RegionLoad *> >		MapRegionsIterator;

typedef GlMap< gedString, KrAction, GlStringHash >		MapActorAnimation;
extern MapActorAnimation mapActorAnimation;

typedef GlSList<void *> ListObject;  
typedef GlSListIterator<void *> ListObjectIterator;

typedef GlSList<Sequence *> ListSequence;  
typedef GlSListIterator<Sequence *> ListSequenceIterator;

typedef GlSList<stAction *> QueueActorAction; //Not queue  
typedef GlSListIterator<stAction *> QueueActorActionIterator;

typedef GlMap< stTimer *, char, GlPointerHash<stTimer *> >		MapTimer;
typedef GlMapIterator< stTimer *, char, GlPointerHash<stTimer *> >		MapTimerIterator;

typedef GlMap< gedString, Sequence *, GlStringHash >		MapSequence;
typedef GlMapIterator< gedString, Sequence *, GlStringHash >		MapSequenceIterator;

typedef GlMap< Uint32, QueueActorAction, GlNumberHash<Uint32> >		SameTimeStamp;
typedef GlMapIterator< Uint32, QueueActorAction, GlNumberHash<Uint32> >		SameTimeStampIterator;

typedef GlMap< gedString, int, GlStringHash >		MapActorMaxClone;
typedef GlMapIterator< gedString, int, GlStringHash >		MapActorMaxCloneIterator;

typedef GlMap< gedString, U8, GlStringHash >		MapString;
typedef GlMapIterator< gedString, U8, GlStringHash >		MapStringIterator;

typedef GlMap< gedString, gedString, GlStringHash >		MapStringString;
typedef GlMapIterator< gedString, gedString, GlStringHash >		MapStringStringIterator;



class stSequence
{
public:
	stSequence() {nRef = 0;}
	void operator=( const stSequence& copy ) //maks	
	{ 
		if( &copy != this )
		{
			nRef = copy.nRef;
			mapSequence = copy.mapSequence;
			listSequence = copy.listSequence;
		}
	}

	MapSequence mapSequence;
	ListSequence listSequence;
	int nRef;
};

typedef GlMap< gedString, stSequence, GlStringHash >		MapActorSequence;
typedef GlMapIterator< gedString, stSequence, GlStringHash >		MapActorSequenceIterator;


enum ActorType
{
	SPRITE = 0,
	CANVAS,
	SLOW_CANVAS,
	VIEW_ACTOR,
	REGION_ACTOR,
	REGION_ACTOR_FILLED
};

#ifndef STAND_ALONE_GAME

class Button;
class EditBox;
class Slide;
class ListBox;
class ListPop;
class ListDirectory;

#endif //#ifndef STAND_ALONE_GAME

class Text;
class SlowCanvas;
class Path;
class Action;
class Tile;
class RegionLoad;

#define MAX_SCRIPTTEXT 256

//Local user vars
extern int structActorSize;

struct stActorVars
{
public:
	
	void operator=( const stActorVars &copy ) 
	{ 
		if( &copy != this ) memcpy(this, &copy, structActorSize);
	}
	
	double x;			
	double y;
	double xprevious;			
	double yprevious;	
	double xscreen;
	double yscreen;
	double r;
	double g;
	double b;
	double transp;
	double pathxpos;
	double pathypos;
	double animpos;
	int animindex;
	int nframes;
	double angle; //in degrees
	double xvelocity;
	double yvelocity;
	double diretional_velocity;
	int width;
	int height;
	double textNumber;
	char text[MAX_SCRIPTTEXT];	

	char name[ACTOR_NAME]; //actor name
	char clonename[CLONE_NAME]; //clone name (actorname.0, ...)
	long cloneindex; //clone index (0, 1, ...)
};

enum Corner
{
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT
};

class DrawInfo
{
public:
	DrawInfo() 
	{
		x = y = 0;
		pensize = 0;

		color.Set(0, 0, 0, 0);
	}

	KrRGBA color;
	int pensize;

	int x, y;
};

class Actor
{
public:
	//The magic variable must be the first location in the memory
	//this variable is used to check if an actor still valid.
	//In the actor creation, make the magic = actor address
	//When destroy, make magic = NULL.
	//With this technic, don't need to use the globalMapActor check (slow)
	//Works only if the class has no virtual functions (see virtual1)

#ifndef ACTOR_USES_VIRTUAL_FUNCTIONS
	Actor *magic;

	void *operator new(size_t size);
	void operator delete(void * mem);
#endif

	

	void DrawSetPen(int r, int g, int b, double transp, int pensize);
	void DrawErase(int r, int g, int b, double transp);
	void DrawMoveTo(int x, int y);
	void DrawLineTo(int x, int y);
	void DrawPutPixel(int x, int y);
	void DrawSaveCanvas();
	void DrawLoadCanvas();

	void FindMaxCloneIndex();
	static void ResetMaxCloneIndex(char *actorName);

	void RemoveTile();
	bool IsVisible();

	void InitEdit();

	
#ifndef STAND_ALONE_GAME

	void RemoveTileAnimation(const gedString& animationName);

	ListString GetDescendantsList();

	KrAction *GetAnimationInfo(const gedString& nameAnimation) {return spriteResource->GetAction(nameAnimation);}
	void DrawTile(const gedString& animationName, bool bSingle = false, bool bRandom = false);

	ListBox *AddListBox(int x = 0, int y = 0, int width = 100, int height = 50, int listId = -1, gedString fontName = "maksfont.bmp");
	Slide *AddSlide(int x, int y, int width = 100, int height = 14, int slideId = -1);
	
	EditBox *AddEditBox(int x = 0, int y = 0, int width = 100, int height = 0, bool multiLine = false, gedString fontName = "maksfont.bmp");
	Button *AddButton(gedString text, 
			   int x = 0, int y = 0,
			   int width = 0, int height = 0,
			   int buttonId = -1,
			   int red = 0, int green = 0, int blue = 0,
			   const gedString spriteName = "maksbutton.bmp",
			   gedString fontName = "maksfont.bmp");

	ListPop *AddListPop(int x = 0, int y = 0,
			   int width = 128, int height = 0,
			   int listpopId = -1,
			   gedString buttonText = "");

	ListDirectory *AddListDirectory(int x = 0, int y = 0,
			   int width = 128, int height = 0,
			   int listpopId = -1,
			   gedString buttonText = "");

	virtual1 void OnButton(Button *button, int buttonId);	//Called by system when button goes up	
	virtual1 bool OnList(ListPop *list, int index, gedString &text, int listId);
	virtual1 bool OnListButtonDown(ListPop *list, int listId); //Called by system after list button down
	virtual1 void OnSlide(Slide *slide, int slideId, double pos);
	virtual1 void OnEditEnter(EditBox *edit, int line);
	virtual1 void OnEditSelection(EditBox *edit);
	virtual1 void OnEditChar(EditBox *edit, SDLKey key, int line);

	void Save(SDL_RWops *src);
	void SaveAnimationData(SDL_RWops *src);

#endif //#ifndef STAND_ALONE_GAME

	Text *AddText(const gedString& text, int x = 0, int y = 0, int align = 0, int _red = 0, int _green = 0, int _blue = 0, 
			      const gedString& fontName = "maksfont.bmp", 
				  int iniASCIIChar = 32, int nChars = 94,
				  bool _bEditable = false, bool _hasBorder = false, bool _hasBackGround = false, bool _getFocus = false,
				  KrRGBA _borderColor = 0, KrRGBA _cursorColor = 0, KrRGBA _backGroundColor = 0);

	static void RegionActorToggleVisibility();
	static void NotifyMouseCoord(int x, int y); //Region
	static void NotifyMouseButtonUp(); //Region

	void IncreaseActorStruct(int newSize);
	int GetArea();
	void decrementConnectedActors() {if(totalConnectedActors > 0) totalConnectedActors--;}
	void incrementConnectedActors() {totalConnectedActors++;}
	bool CanExecuteAllActorsActivationEventAction(char *fromActor);
	virtual1 bool OnSlowCanvasHitTest(int x, int y);
	bool IsActorInView(bool bCheckParent = false);
	bool isWorkingActor() {return flags.IsSet(FLAG_WORKING);}
	bool getCallOnCreate() {return flags.IsSet(FLAG_CALLONCREATE);}
	void setCallOnDestroy(bool value) {flags.Set(FLAG_CALLONDESTROY, value);}
	bool RemoveRegionLoad(RegionLoad *_region) {return ownerRegions.Remove(_region);}
	const MapRegions& GetOwnerRegions() {return ownerRegions;}
	void SetRegionsFrom(Actor *actor);
	int getRegionCount() {return ownerRegions.size();}
	bool UpdateRegionLoad();
	const KrRect& Bounds();
	void CenterOnScreen();

	bool getOnCreateCalled() {return flags.IsSet(FLAG_ONCREATECALLED);}
	void setOnCreateCalled() {flags.Set(FLAG_ONCREATECALLED);}

	bool getNetActor() {return flags.IsSet(FLAG_NETACTOR);}
	void setNetActor(bool value) {flags.Set(FLAG_NETACTOR, value);}

	bool getNetOptimizedPrecision() {return flags.IsSet(FLAG_NET_OPTIMIZED_PRECISION);}
	void setNetOptimizedPrecision(bool value) {flags.Set(FLAG_NET_OPTIMIZED_PRECISION, value);}

	bool getUnlinked() {return flags.IsSet(FLAG_NET_UNLINKED);}
	void setUnlinked(bool value) {flags.Set(FLAG_NET_UNLINKED, value);}

	bool isRegion() {return flags.IsSet(FLAG_REGION);}
	static Actor *Load(const char *name, bool bExplicitlyCreateActor = false);
	void SetActorScale(double scale);
	static bool IsChild(Actor *_parent, Actor *child);
	void CheckMoveTo();
	void MoveTo(double x, double y, double velocity, char *relativeActor, char *obstacle, int moveType);
	int getInfinite() {if(!getSprite()) return NO_INFINITE; return getSprite()->getInfinite();}
	void SetInfinite(int infinite);
	void CheckOutOfVision();
	void SetActorInView(bool _bActorInView);
	double getX() {return scriptVars->x;}
	double getY() {return scriptVars->y;}
	static void ResetAnyPositionChanged() {bAnyPositionChanged = false;}
	static bool AnyPositionChanged() {return bAnyPositionChanged;}

	void ProcessQueueActions();
	void PushQueueAction(stAction *queueAction);
	Sequence *GetSequence(const gedString& sequenceName);
	static void RestartFrameSequences();
	Sequence *GetSequence();
	void AddSequence(const gedString& sequenceName, const gedString& animationName, const gedString& frameSequence, int fps);
	int GetAnimationIndex(const gedString& name);	
	void SetActualValues(U32 updateFlags);
	void SetPosScreen(double x, double y);
	void SetFrame(int f);

	void SetColor(int r, int g, int b);
	void SetPos(double x, double y, bool bDrag = false);
	void SetSize(int width, int height);
	void UpdateScriptChanges(U32 updateFlags, bool bFromNetwork = false);
	void ToAnteriorPosition();
	void SetAnteriorPositions();
	Tile *getTile() {return tile;}
	void setTile(Tile *_tile) {tile = _tile;};
	
	void AdjustView(int width, int height, bool bSetViewPos);
	void setEventControl(Uint32 events, bool bEnable);
	void CopyActorTree();
	gedString GetText();
	void SetText(const gedString& text);
	void setTextActor(Text *text);
	Text *getTextActor() {return textActor;}	
	static Actor *NewActor(SDL_RWops *src, bool bMergeGames, Uint32 version);
	void Load(SDL_RWops *src, Uint32 version);
	
	Actor * NewActor(bool bClone, bool bSetPos = false, int xPos = 0, int yPos = 0, bool bCacheActor = false, bool bSetCloneName = false);
	void SetAnimationState(int state);
	bool getPanel() {return flags.IsSet(FLAG_PANEL);};
	void SetCursor(const gedString& imgName, int nFramesH, int nFramesV, int hotSpotX, int hotSpotY);
	Action *getAction() {return action;};
	void setAction(Action *_action);
	
	const gedString& GetAnimation()
	{
		/*if(getSprite())
		{
		//gedString name = getSprite()->ActionName();
		//if(name != "NONE")
		//	return name;
			
			return actualSequence;
		}
		
		return "";*/

		return actualSequence;
	}


	void SetAnimation(const char* name, bool bKeepActualFrame = false);
	void SetAnimation(const gedString& name, bool bKeepActualFrame = false);
	void SetPathIni(int x, int y);
	void SetPath(const gedString& pathName, int axis);
	void SetPathX(Path *path) {pathX = path;};
	void SetPathY(Path *path) {pathY = path;};
	bool EditMode() {return flags.IsSet(FLAG_EDIT);};
	void RemoveParent();
	int SetParent(const gedString& parentName);
	int SetParent(Actor *parentActor);
	void setCreator(Actor *creatorActor);
	void setCreator(const gedString& creatorActorCloneName);
	bool ExistsAction(const gedString& actionName);	
	gedString AnimationName(int i, Sequence **pSeq = NULL);
	int NumAnimations();
	void SetModal();
	
	int Left();
	int Width();
	int Top();
	int Right();
	int Down();
	int Height();

	void SetVisible(int state);
	void SetTransparency(double trans);
	double GetTransparency();

	virtual1 void OnCollision(Actor *collideActor);	//Called by system when collisions happen
	virtual1 void OnCollisionFinish(Actor *collideActor);	//Called by system when collisions is finish
	virtual1 void OnActivationEvent(Actor *fromActor);
	virtual1 Uint32 OnTimer(SDL_TimerID timerID);	//return 0 to remove timer
	virtual1 void OnKeyDown(SDLKey key, int repeatLoopCount = 0);
	virtual1 void OnKeyUp(SDLKey key);
	virtual1 bool OnMouseButtonDown(int x, int y, Uint8 button);	//return true to enabledrag (released at next miouse button up)
	virtual1 void OnMouseButtonUp(int x, int y, Uint8 button);
	virtual1 void OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message);	

	virtual1 void OnAnimation();	//Called before frame change and after path calculation
	virtual1 void OnAnimationFinish(); //Called when animation reinit
	virtual1 void OnPathFinish();
	virtual1 void OnMouseEnter(int x, int y);	
	virtual1 void OnMouseLeave(int x, int y);
	virtual1 void OnMouseMove(int x, int y);

	virtual1 void OnMoveFinish();
	


	virtual1 void OnSlowCanvasPaint(KrPaintInfo *paintInfo, SDL_Surface* surface, KrRect &rect, int openGLZ);
	
	virtual1 const stCursor *OnGetCursor();
	virtual1 const gedString *OnActorTip();
	
	

	Actor(const gedString& spriteName, Actor *_parent = NULL, ActorType _type = SPRITE, int canvasWidth = 0, int canvasHeight = 0, bool _bPanel = false, Action *copyAction = NULL, bool bIncrementCloneIndex = false, int newCloneIndex = -1); //Actors can be sprite or canvas
	virtual1 ~Actor();

	

	void PostMessage(gedString actorName, Uint32 messageId, Uint32 message = 0); //Send massage to other actor
	void PostMessage(Actor *actorTo, Uint32 messageId, Uint32 message = 0);
	SDL_TimerID SetTimer(Uint32 interval, int count = REPEATE_FOREVER, int _type = PERIODIC_TIMER, Uint32 minRandomInterval = 10);	//interval in ms, return timer ID, count = RANDOM_TIMER, REPEATE_FOREVER_TIMER, > 0 = repeat count
	bool AddAnimation(const gedString& actionName, const gedString& imgName, int nFramesH, int nFramesV, int hotSpotX = -1, int hotSpotY = -1, int fps = 0, bool bTransparency = true, bool bReplaceFile = false, bool bForceGedIcon = false);
	void RemoveAnimation(gedString actionName);

	KrSprite *getSprite();
	KrCanvas *getCanvas();
	KrImage *getImage();
	KrCanvasResource *getCanvasResource();
	KrSpriteResource *getSpriteResource() {return spriteResource;}

	long getFramesToAnim() {return framesToAnim;};
	void setFramesToAnim(long _framesToAnim) {framesToAnim = _framesToAnim;};
	bool getEnableCollisionDetection() {return flags.IsSet(FLAG_ENABLECOLLISIONDETECTION);};
	void setEnableCollisionDetection(bool bEnable);
	char *getActorName() {return scriptVars->name;};
	char *getCloneName() {return scriptVars->clonename;};
	long getCloneIndex() {return scriptVars->cloneindex;};
	Actor *getParent() {return parent;};
	Actor *getCreator();
	Path *getPathX() {return pathX;};
	Path *getPathY() {return pathY;};
	int getPathIniX() {return pathIniX;};
	int getPathIniY() {return pathIniY;};
	int getFramePathX() {return (int)scriptVars->pathxpos;};
	int getFramePathY() {return (int)scriptVars->pathypos;};
	void setFramePathX(int framePath) {scriptVars->pathxpos = framePath;};
	void setFramePathY(int framePath) {scriptVars->pathypos = framePath;};
	int getPathDirectionX() {return pathDirectionX;};
	int getPathDirectionY() {return pathDirectionY;};
	bool getForwardAnimation() {return flags.IsSet(FLAG_FORWARDANIMATION);};
	void setForwardAnimation(bool bForward) {flags.Set(FLAG_FORWARDANIMATION, bForward);};	
	void setRunning(bool bFromReadState = false);
	bool getRunning() {return flags.IsSet(FLAG_RUNNING);};
	const MapTimer *getMapTimer() {return &mapTimer;};
	bool RemoveTimer(stTimer *timer);
	void RemoveTimers();
	void setFollowMouseAxis(int axis);
	int getFollowMouseAxis() {return followMouseAxis;};
	double getVelocityX() {return scriptVars->xvelocity;};
	double getVelocityY() {return scriptVars->yvelocity;};
	double getRealVelocityX() {return (scriptVars->xvelocity != 0)?scriptVars->xvelocity:scriptVars->x - anteriorXPos;}
	double getRealVelocityY() {return (scriptVars->yvelocity != 0)?scriptVars->yvelocity:scriptVars->y - anteriorYPos;}	
	void setVelocityX(double v) {scriptVars->xvelocity = v;};
	void setVelocityY(double v) {scriptVars->yvelocity = v;};
	double getAngle() {return scriptVars->angle;};
	double getDiretionalVelocity() {return scriptVars->diretional_velocity;}
	Uint32 getEventControl() {return eventControl;}
	bool isView() {return flags.IsSet(FLAG_VIEW);}
	bool editAnim() {return flags.IsSet(FLAG_EDITANIM);}
	void setEditAnim(bool bAnim) {flags.Set(FLAG_EDITANIM, bAnim);}
	bool getCreateAtStartup() {return flags.IsSet(FLAG_CREATEATSTARTUP);}
	void setCreateAtStartup(bool bCreate) {flags.Set(FLAG_CREATEATSTARTUP, bCreate);}

	struct stActorVars *getScriptVars() {return scriptVars;};
	static int getStructActorSize() {return structActorSize;}

	void setRegionColor(int r, int g, int b) {if(type == REGION_ACTOR || type == REGION_ACTOR_FILLED) {red = r; green = g; blue = b;}}
	ActorType getType() {return type;}
	bool getMoveTo() {return flags.IsSet(FLAG_MOVETO);}
	static int getBaseZDpth() {return baseZDepth;}
	static void setBaseZDpth(int base) {baseZDepth = base;}

	int getFrameAnt() {return frameAnt;}
	int getFrame() {return sprite?sprite->Frame():0;};
	bool IsLocked() {return flags.IsSet(FLAG_LOCKACTOR);}
	void setLocked(bool value) {flags.Set(FLAG_LOCKACTOR, value);}

	void setOutOfVisionOptimization(bool value);
	bool getOutOfVisionOptimization();

	U32 getLoadedZDepth() {return loadedZDepth;}
	double getLoadedTransparency() {return loadedTransparency;}
	KrRGBA getLoadedColor() {return loadedColor;}

	void setLoadedZDepth(U32 value) {loadedZDepth = value;}
	void setLoadedTransparency(double value) {loadedTransparency = value;}
	void setLoadedColor(KrRGBA value) {loadedColor = value;}

	bool getSetActualValues() {return flags.IsSet(FLAG_SETACTUALVALUES);}
	bool getDeleteConfirmation() {return flags.IsSet(FLAG_DELETECONFIRMATION);}
	void clearDeleteConfirmation() {flags.Clear(FLAG_DELETECONFIRMATION);}

	int getTextZeros() {return nZeros;}
	void setTextZeros(int n) {nZeros = n;}

	bool HaveQueueAction() {return queueActorAction.Count() > 0;}

	static bool getLoadingActors() {return loadingActors > 0;}
	static void resetLoadingActors() {loadingActors = 0;}

#ifndef STAND_ALONE_GAME
	bool getHasShadows() {return flags.IsSet(FLAG_HASSHADOWS);}
	bool ReplaceFrame(const gedString &animationName, int frame, gedString imagePrefix = "");

	gedString getDescription() {return description;}
	void setDescription(gedString value) {description = value;}
#endif

	void SetPathFinder(Path *_path);
	void SetCloneIndex(int newCloneIndex);

	//////////////////////////////////////////////////////////
	//Raknet
	void InitNetActor();
	bool CanExecuteActionLocally();
	bool CanSendRemoteAction();
	bool UpdateOwner(const SystemAddress& oldAddr, const SystemAddress& newAddr);
	const SystemAddress& GetOwner() {return owner;}
	const RakNetTime &GetOwnershipTimestamp() {return ownerShipRequestTime;}
	bool RequestActorOwnership(const SystemAddress& newOwner, const RakNetTime &newRequestTime);
	bool ReleaseOwnership();
	void WriteState(RakNet::BitStream& out);
	void ReadState(RakNet::BitStream& in);
	//////////////////////////////////////////////////////////

	static void setInContructionCallBack(bool value) {bInContructionCallBack = value;}
	
protected:
	void setDeleteConfirmation() {flags.Set(FLAG_DELETECONFIRMATION);}
	void GetObstacleBoundingBoxes(GeoPathFinder &gp, Actor *avoid);
	void GetObstacles(char *obstacleActor);
	void PathFinderRequisition(char *obstacleActor, double xEnd, double yEnd, double velocity);
	

	Uint8					pathDirectionX, pathDirectionY;
	Sint32					pathIniX;
	Sint32					pathIniY;
	
	
		
	
	

	stCursor			cursor;	
	

	Actor				*parent;
	Action				*action;
	Path				*pathX, *pathY;	//Actual actor path
	Path				*pathFinder; //Current 

	//Don't use pointer to save the cretor
	//Need clonename due to avoid pointer reuse issues
	//(don't need in parent due to Notify childs in destructor)
	gedString creatorCloneName;

	Tile *tile;

	

	//Don't save	
	long				framesToAnim;	//frames to anim until stop		
	int					followMouseAxis; //Define if this actor follow mouse
	
	
	double anteriorXPos, anteriorYPos, anteriorFramePathX, anteriorFramePathY;
	int frameAnt;


	MapTimer   mapTimer;

#ifndef STAND_ALONE_GAME
	ListObject listButton;
	ListObject listEditBox;	
	ListObject listSlide;
	ListObject listListBox;
	ListObject listListPop;
	ListObject listListDirectory;	
#endif

	ListObject listText;

	

	static MapActorSequence sequence;

	ActorType type;
	GlFlag<U32>			flags;

private:
	void UpdateAnimIndex();
	

	KrSpriteResource	*spriteResource;
	KrSprite			*sprite;

	KrCanvasResource	*canvasResource;
	KrCanvas			*canvas;

	SlowCanvas			*slowCanvas;

	

	
	Text				*textActor;

	//Event control
	Uint32	eventControl;

	//Script changes
	struct stActorVars *scriptVars, *scriptVarsAnt;


	gedString actualSequence;

	QueueActorAction queueActorAction;
	SameTimeStamp	sameTimeStamp;
	Uint32	lastTimeStamp;
	stAction *lastQueuedAction;
	double antAngle;
	
	


	

	static bool bAnyPositionChanged; //true if call SetPos in any actor
	static int loadingActors; //true if in Load funcion
	MapRegions ownerRegions; //Owner regions

	//MoveTo	
	double xstop, ystop;

	

	stSequence *mySequence;

	//Activation Event from all actors
	int totalConnectedActors;
	MapString mapFromActivationEventActors;

	

	static int baseZDepth;

	//Region Actor
protected:
	static bool bShowRegions;
	static Actor *change;
	int xClick, yClick, xObjClick, yObjClick, widthClick, heightClick;
	Corner corner;

	

	int red, green, blue;

	DrawInfo *drawInfo;
	int nZeros; //To text number fill

	//Solve the bug "don't show myPlane smoke in 1945.ged in stand alone game"
	U32 loadedZDepth;
	double loadedTransparency;
	KrRGBA loadedColor;

	//Raknet		
	SystemAddress owner;	
	RakNetTime ownerShipRequestTime;
	static bool bInContructionCallBack;
	
#ifndef ACTOR_USES_VIRTUAL_FUNCTIONS
	static GlDynArray<void *> freeList;
#endif

#ifndef STAND_ALONE_GAME

	gedString description;
	gedString GetUpdateImageName(const gedString &animationName);
	
#endif
};


#define EVENTMOUSEBUTTONDOWN	1
#define EVENTMOUSEBUTTONUP		2
#define EVENTMOUSEMOVE			4
#define EVENTANIMATION			8
#define EVENTANIMATIONFINISH	16
#define EVENTPATHFINISH			32
#define EVENTMOUSEENTER			64
#define EVENTMOUSELEAVE			128
#define EVENTKEYDOWN			256
#define EVENTKEYUP				512
#define EVENTTIMER				1024
#define EVENTMESSAGE			2048
#define EVENTCOLLISION			4096
#define EVENTCREATE				8192
#define EVENTDESTROYACTOR		16384
#define EVENTOUTOFVISION		32768
#define EVENTACTIVATIONEVENT	65536
#define EVENTCOLLISIONFINISH	131072
#define EVENTMOVEFINISH			262144
#define EVENTALL				0xFFFFFFFF


#endif // !defined(AFX_ACTOR_H__D0F60C29_A8DF_4646_B471_72EC53CB64D9__INCLUDED_)
