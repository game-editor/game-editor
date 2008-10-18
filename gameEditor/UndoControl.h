// UndoControl.h: interface for the UndoControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(STAND_ALONE_GAME)

#if !defined(AFX_UNDOCONTROL_H__FB70CB37_337D_4754_9911_27634CC0DEE0__INCLUDED_)
#define AFX_UNDOCONTROL_H__FB70CB37_337D_4754_9911_27634CC0DEE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define GE_UNDO_FILE_TYPE		"GEDUNDO1"
#define PART_TYPE_FULL_FILE		1
#define PART_TYPE_DIFF			2
#define PART_END				"ENDP"

#define PART_VERSION			1

class RedoPart
{
public:

	RedoPart(U32 _top, U32 _previousRedoTop, SDL_RWops *_part, char _type);
	RedoPart()
	{
		previousRedoTop = top = 0;
		part = NULL;
		type = 0;
	}

	void operator=(const RedoPart &copy)
	{
		top = copy.top;
		previousRedoTop = copy.previousRedoTop;
		part = copy.part;
		type = copy.type;
	}

	virtual ~RedoPart();

	U32 top, previousRedoTop;
	SDL_RWops *part;
	char type;
};

class PartInfo
{
public:

	PartInfo(U32 _offSet, U32 _size)
	{
		offSet = _offSet;
		size = _size;
	}

	PartInfo()
	{
		offSet = size = 0;
	}

	void operator=(const PartInfo &copy)
	{
		offSet = copy.offSet;
		size = copy.size;
	}

	virtual ~PartInfo() {}

	U32 offSet, size;
};

typedef GlSList<RedoPart> RedoStack;  //push = PushFront, pop = PopFront, top = Front*/
typedef GlSListIterator<RedoPart> RedoStackIterator;

typedef GlSList<PartInfo> PartList;  //push = PushFront, pop = PopFront, top = Front*/
typedef GlSListIterator<PartInfo> PartListIterator;

class UndoControl  
{
public:
	virtual ~UndoControl();

	static void Destroy();
	static UndoControl *Get();

	void Clear();
	bool Undo();
	bool Redo();
	void PushCurrentState(SDL_RWops *src = NULL);
	void Save();

	bool LoadEmergencyData();
	bool HaveEmergencyData();

	static void CloseSDLFile(SDL_RWops **src);

	int ExtractAllFiles(gedString path);

private:
	UndoControl();
	SDL_RWops *CreateGEDFile();
	void CheckUndoFile();

	void SetEmergencyData();
	void ClearEmergencyData();
	
	
	bool CheckFileIntegrity(SDL_RWops *src, bool bGetLastState);

	void Push(SDL_RWops *src, bool bFullFile, Uint32 crcFull = 0);
	SDL_RWops *Pop(bool bGetLastState);
	bool PopLoad(bool bShowErrorMessage = false);
	bool GetDiff(SDL_RWops *oldFile, SDL_RWops *newFile, SDL_RWops **diffFile);
	bool GetPatch(SDL_RWops *oldFile, SDL_RWops *diffFile, SDL_RWops **newFile);
	void Truncate(U32 truncateSize);

	char CanUndo();
	void PushRedo(U32 top, U32 previousRedoTop, SDL_RWops *newState);
	void EraseRedo();
	void RemoveHeadPart(int topType);
	bool IsTop();
	U32 GetTopFullCRC();
	U32 GetPreviousPartOffSet();
	bool KeepOnlyLastParts(gedString fileName, int nUndo);

	bool Load(bool bShowErrorMessage = false);

	static UndoControl *pUndoControl;

	SDL_RWops *srcUndoFile;
	SDL_RWops *lastState;
	gedString undoFileName;

	U32 topStack;
	RedoStack redo;
};


#endif // !defined(AFX_UNDOCONTROL_H__FB70CB37_337D_4754_9911_27634CC0DEE0__INCLUDED_)

#endif
