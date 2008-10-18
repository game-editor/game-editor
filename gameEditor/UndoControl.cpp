// UndoControl.cpp: implementation of the UndoControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(STAND_ALONE_GAME)

#include <stdio.h>
#include "GameControl.h"
#include "UndoControl.h"
#include "jdiff.h"
#include "ActorProperty.h"
#include "Config.h"

extern gedString sGameEditorCaption;

UndoControl *UndoControl::pUndoControl = NULL;

RedoPart::RedoPart(U32 _top, U32 _previousRedoTop, SDL_RWops *_part, char _type)
{
	top = _top;
	previousRedoTop = _previousRedoTop;
	part = _part;
	type = _type;
}

RedoPart::~RedoPart()
{
	//Don't erase the part here because the list copy
}

UndoControl::UndoControl()
{
	lastState = srcUndoFile = NULL;
}

UndoControl::~UndoControl()
{
	Clear();
}

void UndoControl::Destroy()
{
	delete pUndoControl;
	pUndoControl = NULL;
}

UndoControl *UndoControl::Get()
{
	if(!pUndoControl)
	{
		pUndoControl = new UndoControl();
	}

	return pUndoControl;
}

void UndoControl::CloseSDLFile(SDL_RWops **src)
{
	if(src && *src)
	{
		if((*src)->type == SDL_RW_TYPE_MEM && (*src)->hidden.mem.base)
		{
			free((*src)->hidden.mem.base);
		}

		SDL_RWclose(*src);

		*src = NULL;
	}
}

void UndoControl::Clear()
{
	CloseSDLFile(&srcUndoFile);
	CloseSDLFile(&lastState);
	EraseRedo();

	gedString s(undoFileName);
	s.lower();

	if(s.substr(s.length() - 4) == ".tmp")
	{
		//Game Editor not in crash
		//Can safety remove the temp undo
		remove(undoFileName.c_str());
	}

	undoFileName.clear();
	ClearEmergencyData();
}


SDL_RWops *UndoControl::CreateGEDFile()
{
	GameControl::Get()->setFastFile(true);

	SDL_RWops *src = SDL_RWFromMem(NULL, 0);
	if(!GameControl::Get()->Save(gedString("gedTmp_Undo.ged"), src))
	{
		GLASSERT(0);
		CloseSDLFile(&src);
	}	

	GameControl::Get()->setFastFile(false);

	if(src) SDL_RWseek( src, 0, SEEK_SET );
	return src;
}



SDL_RWops *UndoControl::Pop(bool bGetLastState)
{
	//Assumme the integrity is ok
	SDL_RWops *newState = NULL;
	CheckUndoFile();

	if(!srcUndoFile) return NULL;

	//Get partend
	SDL_RWseek( srcUndoFile, -4, SEEK_CUR );

	Uint32 partEnd = SDL_ReadLE32(srcUndoFile);
	if(memcmp(&partEnd, PART_END, 4) != 0) 
	{
		//GLASSERT(0);
		return NULL;
	}

	
	//Get part offset
	SDL_RWseek( srcUndoFile, -12, SEEK_CUR );
	U32 partOffset = SDL_ReadLE32(srcUndoFile);

	//Go to part info
	SDL_RWseek( srcUndoFile, partOffset, SEEK_SET );

	Uint32 currentPos = SDL_RWtell( srcUndoFile );
	
	if(bGetLastState)
	{
		//Get part version
		char version;
		SDL_RWread(srcUndoFile, &version, sizeof(version), 1);

		//Get part type
		char type;	
		SDL_RWread(srcUndoFile, &type, sizeof(type), 1);
		
			
		//Get part size
		U32 size = SDL_ReadLE32(srcUndoFile);
		
		//Read part
		void *mem = malloc(size);
		if(!mem) return NULL;
		
		SDL_RWread(srcUndoFile, mem, size, 1);

		//Read full crc
		U32 crcFull = SDL_ReadLE32(srcUndoFile);
		
		if(type == PART_TYPE_FULL_FILE)
		{
			//Get last state from file
			newState = SDL_RWFromMem(mem, size);
		}
		else
		{
			//New state = laststate + diff
			SDL_RWops *diff = SDL_RWFromMem(mem, size);

			GetPatch(lastState, diff, &newState);
			CloseSDLFile(&diff);
		}

		if(newState)
		{
			//Check full crc
			U32 crcCalc = Crc32(newState->hidden.mem.base, newState->hidden.mem.stop - newState->hidden.mem.base);
			GLASSERT(crcCalc == crcFull);

			if(crcCalc != crcFull)
			{
				CloseSDLFile(&newState);
			}
		}
	}

	//Adjust top
	SDL_RWseek( srcUndoFile, currentPos, SEEK_SET );
	return newState;
}

void UndoControl::PushCurrentState(SDL_RWops *src)
{
#if defined(GAME_EDITOR_HOME_EDITION)
	return;
#endif

	if(GameControl::Get()->getGameMode()) 
	{
		//No undo in game mode
		return;
	}

	if(GameControl::Get()->GetModal() && !src) 
	{
		//No undo when modal panels are open
		return;
	}

	WaitCursor wait;

	//Push only if differ from last state
	if(!src) src = CreateGEDFile();
	if(!src) return;

	SDL_RWops *diff = NULL;

	CheckUndoFile();
	if(!srcUndoFile) return;

	Uint32 top = SDL_RWtell( srcUndoFile );
	
	if(!lastState || top <= 8)
	{
		Push(src, true);
		lastState = src;
	}
	else
	{
		//Get differences from last state
		if(IsTop())
		{
			//Remove the last state at stack top
			Pop(false);	
		}
		else //Remove this else to see the save duplication
		{
			//The user want to cancel the action and put a new after undo
			//The current last state (newest action) is not more valid
			//So, get the last state from stack
			SDL_RWops *newState = Pop(true);
			if(newState)
			{
				CloseSDLFile(&lastState);
				lastState = newState;
			}
		}
		
		bool bRes = GetDiff(src, lastState, &diff);		
		
		GLASSERT(bRes);
		if(bRes)
		{
			//Again here due the second diff in an action cancelation
			if(diff) 
			{
				Push(diff, false, Crc32(lastState->hidden.mem.base, lastState->hidden.mem.stop - lastState->hidden.mem.base));

				//Push the current state
				Push(src, true);
				
				CloseSDLFile(&lastState);
				lastState = src;
			}
			else
			{
				//no difference found

				//Restore top due to Pops
				SDL_RWseek( srcUndoFile, top, SEEK_SET ); 

				//Close saved file
				CloseSDLFile(&src);
			}
		}
		else
		{
			//Error: push the entire file if has some differences

			//Restore top due to Pops
			SDL_RWseek( srcUndoFile, top, SEEK_SET ); 
			
			int sizeLast = lastState->hidden.mem.stop - lastState->hidden.mem.base,
				size = src->hidden.mem.stop - src->hidden.mem.base;
			
			if(size != sizeLast || memcmp(lastState->hidden.mem.base, src->hidden.mem.base, size) != 0)
			{
				Push(src, true);
				
				CloseSDLFile(&lastState);
				lastState = src;
			}	
			else
			{
				//no difference found
				CloseSDLFile(&src);
			}
		}
	}

	/*if(lastState)
	{
		U32 crc = Crc32(lastState->hidden.mem.base, lastState->hidden.mem.stop - lastState->hidden.mem.base);
		char s[64];
		sprintf(s, "lastState_%X.ged", crc);
		FILE *arq = fopen(s, "wb");
		fwrite(lastState->hidden.mem.base, lastState->hidden.mem.stop - lastState->hidden.mem.base, 1, arq);
		fclose(arq);
	}*/

	if(!undoFileName.empty())
	{
		CheckUndoFile();
		if(top != SDL_RWtell( srcUndoFile ))
		{
			//New data, set recovery
			SetEmergencyData();
		}
	}

	//Close here to keep the file integrity
	if(srcUndoFile && srcUndoFile->type != SDL_RW_TYPE_MEM)
	{		
		CloseSDLFile(&srcUndoFile);
	}
}

bool UndoControl::CheckFileIntegrity(SDL_RWops *src, bool bGetLastState)
{
	//Get file size
	SDL_RWseek( src, 0, SEEK_END );
	U32 size = SDL_RWtell( src );
	
	if(size < 8 + 1 + 4 + 4 + 4 + 4) return false;

	//Get file type
	char header[9];
	SDL_RWseek( src, 0, SEEK_SET );
	SDL_RWread(src, header, 8, 1);

	if(memcmp(header, GE_UNDO_FILE_TYPE, 8) != 0) return false;

	//Get partend
	SDL_RWseek( src, -4, SEEK_END );

	Uint32 partEnd = SDL_ReadLE32(src);
	if(memcmp(&partEnd, PART_END, 4) != 0) return false;

	//Get part CRC32
	SDL_RWseek( src, -8, SEEK_END );
	Uint32 crc32 = SDL_ReadLE32(src);

	//Get part offset
	SDL_RWseek( src, -12, SEEK_END );
	U32 partOffset = SDL_ReadLE32(src);

	//Go to part info
	SDL_RWseek( src, partOffset, SEEK_SET );

	//Get part version
	char version;
	SDL_RWread(src, &version, sizeof(version), 1);

	//Get part type
	char type;	
	SDL_RWread(src, &type, sizeof(type), 1);

	//Don't check only full part type if use Pop(false), Push(false), Push(true)
	//if(type != PART_TYPE_FULL_FILE) return false;

	if(type != PART_TYPE_FULL_FILE && type != PART_TYPE_DIFF) 
	{
		//Wrong file
		//Return to avoid big memory allocations due to wrong size
		//Solve crash when execute "Build the scenario with Tiles.gedt" tutorial
		//GLASSERT(0);
		return false;
	}

	//Get part size
	size = SDL_ReadLE32(src);

	//Read part
	void *mem = malloc(size);
	if(!mem) return false;

	SDL_RWread(src, mem, size, 1);

	//Calculate crc
	U32 crcCalc = Crc32(mem, size);	

	if(crcCalc != crc32)
	{
		free(mem);
		return false;
	}

	//File is ok
	if(bGetLastState)
	{
		//Get last state from file
		CloseSDLFile(&lastState);
		lastState = SDL_RWFromMem(mem, size);
	}
	else
	{
		free(mem);
	}

	SDL_RWseek( src, 0, SEEK_END );
	return true;
}

void UndoControl::Save()
{
	//Copy the current memory or temp file to undo file

	//Get the old file name
	gedString from(undoFileName);

	//Copy memory to temp if exists
	CheckUndoFile();
	if(!srcUndoFile) return;

	if(from.empty()) from = undoFileName;
	
	GLASSERT(srcUndoFile->type != SDL_RW_TYPE_MEM);
	if(srcUndoFile->type == SDL_RW_TYPE_MEM) 
	{
		//No file to save
		return;
	}

	if(!IsTop())
	{
		GLASSERT(0);
		PushCurrentState();
	}
		
	//Close to copy
	CloseSDLFile(&srcUndoFile);

	gedString s(undoFileName);
	s.lower();

	if(s.substr(s.length() - 4) == ".tmp")
	{
		//Copy
		//Double copy is need if is saving in a new location 
		gedString fileName(undoFileName.substr(0, undoFileName.length() - 4));

		if(Config::Get()->getPersistentUndo())
		{
			bool bRes = copy(from.c_str(), fileName.c_str());
			
			GLASSERT(bRes);
			if(bRes)
			{
				int nUndo = Config::Get()->getMaxUndoLevels();
				Config::Destroy();
				
				if(KeepOnlyLastParts(fileName, nUndo))
				{
					//Remove old temp file
					remove(from.c_str());
					remove(undoFileName.c_str());
					undoFileName.clear();
					
					ClearEmergencyData();
					
					
					///////////////////////////////////////////////
					//Syncronize with last saved game file
					/*CheckUndoFile();
					SDL_RWops *lastSavedFile = LoadToMemoryFile(GameControl::Get()->getGameName());
					
					  if(lastSavedFile)
					  {
					  //Remove full top
					  Pop(false);
					  
						PushCurrentState(lastSavedFile);
						}			
						
						  //Again
						  CloseSDLFile(&srcUndoFile);
						  
							gedString to(undoFileName.substr(0, undoFileName.length() - 4));
							copy(undoFileName.c_str(), to.c_str());
							remove(undoFileName.c_str());
					undoFileName.clear();*/
					///////////////////////////////////////////////
				}
			}		
		}
		else
		{
			//Don't save undo, but keep .tmp file
			if(undoFileName != from) remove(from.c_str());
			remove(fileName.c_str());
			undoFileName.clear();
			
			ClearEmergencyData();
		}
	}
	
	SDL_ClearError();
}

bool UndoControl::KeepOnlyLastParts(gedString fileName, int nUndo)
{
	//Keep only the last nParts in the undo file
	SDL_RWops *src = ged_SDL_RWFromFile(fileName.c_str(), "r+b");
	GLASSERT(src != NULL);
	if(!src) return false;

	int nParts = nUndo + 1;

	if(!CheckFileIntegrity(src, false))
	{
		GLASSERT(0);		
		CloseSDLFile(&src);
		return false;
	}
	

	//Get last nParts offsets
	PartList partList;

	for(int n = 0; n < nParts; n++)
	{
		//Get partend
		SDL_RWseek( src, -4, SEEK_CUR );
		
		Uint32 partEnd = SDL_ReadLE32(src);
		if(memcmp(&partEnd, PART_END, 4) == 0) 
		{
			U32 partFin = SDL_RWtell( src );
			
			//Get part offset
			SDL_RWseek( src, -12, SEEK_CUR );
			U32 partOffset = SDL_ReadLE32(src);
			U32 totalSize = partFin - partOffset;

			//Go to part info
			SDL_RWseek( src, partOffset, SEEK_SET );

			partList.PushFront(PartInfo(partOffset, totalSize));
		}
		else
		{
			break;
		}
	}

	if(!partList.Count())
	{
		GLASSERT(0);		
		CloseSDLFile(&src);
		return false;
	}

	SDL_RWops *newMemFile = SDL_RWFromMem(NULL, 0);

	if(!newMemFile)
	{
		GLASSERT(0);		
		CloseSDLFile(&src);
		return false;
	}

	SDL_ClearError();

	//Save header
	SDL_RWwrite(newMemFile, GE_UNDO_FILE_TYPE, 8, 1);

	PartListIterator itList(partList);
	for(itList.Begin(); !itList.Done(); itList.Next())
	{
		void *buf = malloc(itList.Current().size);
		if(!buf)
		{
			GLASSERT(0);		
			CloseSDLFile(&newMemFile);
			CloseSDLFile(&src);
			return false;
		}

		//Read part
		SDL_RWseek(src, itList.Current().offSet, SEEK_SET);
		SDL_RWread(src, buf, itList.Current().size, 1);

		//Get correct offset
		U32 offset = SDL_RWtell( newMemFile );

		//Write part
		SDL_RWwrite(newMemFile, buf, itList.Current().size, 1);

		//Write correct offset
		U32 pos = SDL_RWtell( newMemFile );

		SDL_RWseek( newMemFile, -12, SEEK_CUR );
		SDL_WriteLE32(newMemFile, offset);

		SDL_RWseek( newMemFile, pos, SEEK_SET );

		free(buf);
	}

	CloseSDLFile(&src);

	if(*SDL_GetError() == 0)
	{
		//Remove old file
		remove(fileName.c_str());

		//Copy new
		SDL_RWops *srcNew = ged_SDL_RWFromFile(fileName.c_str(), "wb");
		if(!srcNew)
		{
			GLASSERT(0);		
			CloseSDLFile(&newMemFile);
			return false;
		}

		SDL_RWwrite(srcNew, newMemFile->hidden.mem.base, newMemFile->hidden.mem.stop - newMemFile->hidden.mem.base, 1);
		CloseSDLFile(&srcNew);
	}

	CloseSDLFile(&newMemFile);

	return *SDL_GetError() == 0;
}


void UndoControl::CheckUndoFile()
{
	//Don't use "a+b" option: 
	//	When a file is opened with the "a" or "a+" access type, 
	//	all write operations occur at the end of the file. 
	//	The file pointer can be repositioned using fseek or rewind,
	//	but is always moved back to the end of the file before any
	//	write operation is carried out. Thus, existing data cannot be overwritten.
	
	if(!srcUndoFile)
	{
		if(!GameControl::Get()->getGameName().empty())
		{
			//Create the file on game directory	
			bool bNew = false, bFromUndoFile = false;

			gedString fileName(GameControl::Get()->getGamePath() + DIR_SEP + GameControl::Get()->getGameName() + ".undo");
			

			//Open temp for reading and write (the file must exists)
			//Temp file will exists if Game Editor exit on crash
			undoFileName = fileName + ".tmp";
			srcUndoFile = ged_SDL_RWFromFile(undoFileName.c_str(), "r+b");

			//There is no temp file
			//Try copy the undo
			if(!srcUndoFile) 
			{
				copy(fileName.c_str(), undoFileName.c_str());

				//Try open temp file again
				srcUndoFile = ged_SDL_RWFromFile(undoFileName.c_str(), "r+b");

				if(srcUndoFile)
				{
					bFromUndoFile = true;
				}
			}

			//Failed to open the undo file
			//Create a new temp file
			if(!srcUndoFile) 
			{
				srcUndoFile = ged_SDL_RWFromFile(undoFileName.c_str(), "w+b");
				bNew = true;
			}

			SDL_ClearError();

			if(srcUndoFile)
			{
				if(!CheckFileIntegrity(srcUndoFile, true))
				{
					//Not valid, truncate
					SDL_RWseek( srcUndoFile, 0, SEEK_SET );
					//GLASSERT(bNew); //Error only if not new
				}
				else if(bFromUndoFile)
				{
					//The load is ok, but if you try undo
					//will get an error.
					//So, make this corrections
				
					//Remove last state to avoid duplications
					CloseSDLFile(&lastState);
					lastState = Pop(true);

					//make corrections
					PushCurrentState(CreateGEDFile());
					CheckUndoFile();					
										
					CloseSDLFile(&lastState);
					lastState = Pop(true);
				}

				//SetEmergencyData();
			}
			else
			{
				undoFileName.clear();
			}
		}
		else
		{
			//User don't save the game yet, create a memory file
			srcUndoFile = SDL_RWFromMem(NULL, 0);			
		}
	}
	else if(srcUndoFile->type == SDL_RW_TYPE_MEM)
	{
		if(!GameControl::Get()->getGameName().empty())
		{
			//User already save the game, transfer memory undo to file
			
			undoFileName = GameControl::Get()->getGamePath() + DIR_SEP + GameControl::Get()->getGameName() + ".undo.tmp";

			//Opens an empty file for both reading and writing
			SDL_RWops *src = ged_SDL_RWFromFile(undoFileName.c_str(), "w+b");
			SDL_ClearError();

			if(src)
			{
				int size = srcUndoFile->hidden.mem.stop - srcUndoFile->hidden.mem.base;
				char *memBase = (char *)srcUndoFile->hidden.mem.base;

				//Truncate the file
				SDL_RWseek( src, 0, SEEK_SET );
				

				//Write memory undo
				SDL_RWwrite(src, memBase, size, 1);

				if(*SDL_GetError() == 0)
				{
					CloseSDLFile(&srcUndoFile);
					srcUndoFile = src;

					//SetEmergencyData();
				}
				else
				{
					undoFileName.clear();
					SDL_RWclose(src);
				}
			}
			else
			{
				undoFileName.clear();
			}

			SDL_ClearError();
		}
	}

	//Secury check
	if(!srcUndoFile)
	{
		//Create a memory file
		srcUndoFile = SDL_RWFromMem(NULL, 0);		
	}

	//New file?
	Uint32 currentPos = SDL_RWtell( srcUndoFile );
	if(currentPos == 0)
	{
		//Save header
		SDL_RWwrite(srcUndoFile, GE_UNDO_FILE_TYPE, 8, 1);
	}
}

void UndoControl::SetEmergencyData()
{
	//Put the game name, game directory and temp undo file name on ini file
	Config::Get()->setEmergencySave(undoFileName);
	Config::Destroy();
}

void UndoControl::ClearEmergencyData()
{
	Config::Get()->setEmergencySave("");
	Config::Destroy();
}

bool UndoControl::HaveEmergencyData()
{
	bool bRes = false;

	if(Config::Get()->getEmergencySave().length())
	{
		char oldpath[PATH_LIMIT];
		ged_getcwd(oldpath, PATH_LIMIT);

		gedString file(Config::Get()->getEmergencySave());
		chdir(file.GetFilePath().c_str());	
		
		SDL_RWops *src = ged_SDL_RWFromFile(file.c_str(), "r+b");
		bool bError = true;

		chdir(oldpath);

		if(src)
		{			
			if(CheckFileIntegrity(src, false))
			{
				bRes = true;
			}

			CloseSDLFile(&src);
		}
		
		//Don't remove emergency data
		Config::Destroy();
	}

	return bRes;
}

bool UndoControl::LoadEmergencyData()
{
	if(Config::Get()->getEmergencySave().length())
	{
		undoFileName = Config::Get()->getEmergencySave();
		chdir(undoFileName.GetFilePath().c_str());
		

		CloseSDLFile(&srcUndoFile);
		srcUndoFile = ged_SDL_RWFromFile(undoFileName.c_str(), "r+b");
		bool bError = false;

		if(srcUndoFile)
		{			
			if(CheckFileIntegrity(srcUndoFile, false))
			{
				gedString gamePath(undoFileName.substr(0, undoFileName.rfind(DIR_SEP)));
				gedString gameName(undoFileName.substr(undoFileName.rfind(DIR_SEP) + 1));

				gameName = gameName.substr(0, gameName.length() - 9);

				GameControl::Get()->setGamePath(gamePath);
				GameControl::Get()->setGameName(gameName);

				if(PopLoad(true))
				{
					SetCaption((sGameEditorCaption + gameName).c_str(), NULL);
					
					//The load is ok, but if you try undo
					//will get an error.
					//So, make this corrections
					PushCurrentState(CreateGEDFile());
					CheckUndoFile();					
										
					CloseSDLFile(&lastState);
					lastState = Pop(true);
				}
				else
				{
					GLASSERT(0);
					bError = true;	

					GameControl::Get()->setGamePath("");
					GameControl::Get()->setGameName("");
				}
			}
			else
			{
				GLASSERT(0);
				bError = true;	
			}
		}

		if(bError)
		{			
			CloseSDLFile(&srcUndoFile);			
			undoFileName.clear();

			return false;
		}
		
		Config::Destroy();
	}

	return true;
}

int UndoControl::ExtractAllFiles(gedString path)
{
	undoFileName = path;
	chdir(undoFileName.GetFilePath().c_str());
	
	
	CloseSDLFile(&srcUndoFile);
	srcUndoFile = ged_SDL_RWFromFile(undoFileName.c_str(), "r+b");
	if(!srcUndoFile || !CheckFileIntegrity(srcUndoFile, false)) 
	{
		Destroy();
		return 0;
	}

	gedString fileName(undoFileName.GetFileName());
	fileName = fileName.substr(0, fileName.find('.'));
	int n = 0;

	while(CanUndo() >= 0)
	{
		SDL_RWops *newState = Pop(true);

		CloseSDLFile(&lastState);
		lastState = newState;

		if(newState)
		{
			char buf[256];
			sprintf(buf, "%s_%04ld.ged", fileName.c_str(), n);
			SDL_RWops *src = ged_SDL_RWFromFile(buf, "w+b");
			if(src)
			{
				int size = newState->hidden.mem.stop - newState->hidden.mem.base;
				char *memBase = (char *)newState->hidden.mem.base;
				
				
				SDL_RWwrite(src, memBase, size, 1);

				CloseSDLFile(&src);
				n++;
			}
		}
		else
		{
			break;
		}
	}

	return n;
}

void UndoControl::Push(SDL_RWops *src, bool bFullFile, Uint32 crcFull)
{
	if(src->type != SDL_RW_TYPE_MEM) return;

	EraseRedo();

	CheckUndoFile();
	if(!srcUndoFile) return;

	//Part size and crc
	Uint32 partSize = src->hidden.mem.stop - src->hidden.mem.base;
	Uint32 crcPart = Crc32(src->hidden.mem.base, partSize);
	if(bFullFile) crcFull = crcPart;

	//Get part offset
	Uint32 partOffset = SDL_RWtell( srcUndoFile );

	//Write part version
	char version = PART_VERSION;	
	SDL_RWwrite(srcUndoFile, &version, sizeof(version), 1);

	//Write part type
	char type = bFullFile?PART_TYPE_FULL_FILE:PART_TYPE_DIFF;	
	SDL_RWwrite(srcUndoFile, &type, sizeof(type), 1);

	//Write part size	
	SDL_WriteLE32(srcUndoFile, partSize);

	//Write part
	SDL_RWwrite(srcUndoFile, src->hidden.mem.base, partSize, 1);

	//Write full CRC to check the part reconstruction
	SDL_WriteLE32(srcUndoFile, crcFull);


	//Write part offset
	SDL_WriteLE32(srcUndoFile, partOffset);


	//Write part CRC32	
	SDL_WriteLE32(srcUndoFile, crcPart);


	//Write part end
	SDL_RWwrite(srcUndoFile, PART_END, 4, 1);


	//Truncate the file to avoid errors when the file shrink after undo operations
	Uint32 fileSize = SDL_RWtell( srcUndoFile );
	

	//Close here to keep the file integrity
	if(srcUndoFile->type != SDL_RW_TYPE_MEM)
	{		
		CloseSDLFile(&srcUndoFile);
	}	

	Truncate(fileSize);

	SDL_ClearError();
}

void UndoControl::Truncate(U32 truncateSize)
{
	if(srcUndoFile && srcUndoFile->type == SDL_RW_TYPE_MEM)
	{
		U32 size = srcUndoFile->hidden.mem.stop - srcUndoFile->hidden.mem.base;
		if(size > truncateSize)
		{
			mem_expand(srcUndoFile, truncateSize);
		}

		return;
	}

	if(undoFileName.empty()) return;

	//Get file size
	SDL_RWops *src = ged_SDL_RWFromFile(undoFileName.c_str(), "rb");
	if(src)
	{
		SDL_RWseek( src, 0, SEEK_END );
		U32 size = SDL_RWtell( src );
		
		CloseSDLFile(&src);		

		if(size > truncateSize)
		{
			//Truncate
#ifdef WIN32

			//Windows
			HANDLE arq = CreateFile(	undoFileName.c_str(), 
									GENERIC_READ | GENERIC_WRITE,   
									0,  
									NULL,      
									OPEN_EXISTING, 
									FILE_ATTRIBUTE_NORMAL,
									NULL);    
			
			if(arq != INVALID_HANDLE_VALUE) 
			{         
				DWORD pos = SetFilePointer(arq, truncateSize, NULL, FILE_BEGIN);
				if(pos != 0xFFFFFFFF)
				{
					SetEndOfFile(arq); 
				}

				CloseHandle(arq);				
			} 
#else

			//Linux
			truncate(undoFileName.c_str(), truncateSize);
#endif
		}
	}
}

bool UndoControl::GetDiff(SDL_RWops *oldFile, SDL_RWops *newFile, SDL_RWops **diffFile)
{
	//diff = new - old
	*diffFile = SDL_RWFromMem(NULL, 0);

	SDL_RWseek( oldFile, 0, SEEK_SET );
	SDL_RWseek( newFile, 0, SEEK_SET );

	JDiff diff;
	int res = diff.Diff(oldFile, newFile, *diffFile);

	if(res != 0 && res != 1)
	{
		//error
		CloseSDLFile(diffFile);
		return false;
	}
	else if(res == 1)
	{
		//no diffrerences found
		CloseSDLFile(diffFile);
	}
#ifdef DEBUG
	else if(res == 0)
	{
		//Verification
		SDL_RWops *oriState = NULL;
		GetPatch(oldFile, *diffFile, &oriState);

		GLASSERT(oriState != NULL);

		int sizeOri = oriState->hidden.mem.stop - oriState->hidden.mem.base,
				size = newFile->hidden.mem.stop - newFile->hidden.mem.base;

		GLASSERT(sizeOri == size);
		GLASSERT(memcmp(oriState->hidden.mem.base, newFile->hidden.mem.base, size) == 0);

		CloseSDLFile(&oriState);
	}
#endif

	return true;
}

bool UndoControl::GetPatch(SDL_RWops *oldFile, SDL_RWops *diffFile, SDL_RWops **newFile)
{
	//new = old + diff

	if(!oldFile) return false;
	*newFile = SDL_RWFromMem(NULL, 0);

	SDL_RWseek( oldFile, 0, SEEK_SET );
	SDL_RWseek( diffFile, 0, SEEK_SET );

	JDiff diff;
	int res = diff.Patch(oldFile, diffFile, *newFile);

	if(res != 0)
	{
		//error
		CloseSDLFile(newFile);
		return false;
	}

	return true;
}

char UndoControl::CanUndo()
{
	char type = -1, version;
	if(!srcUndoFile) return type;

	U32 pos = SDL_RWtell( srcUndoFile );

	//Get partend
	SDL_RWseek( srcUndoFile, -4, SEEK_CUR );

	Uint32 partEnd = SDL_ReadLE32(srcUndoFile);
	if(memcmp(&partEnd, PART_END, 4) == 0) 
	{
		//Get part offset
		SDL_RWseek( srcUndoFile, -12, SEEK_CUR );
		U32 partOffset = SDL_ReadLE32(srcUndoFile);
		
		//Go to part info
		SDL_RWseek( srcUndoFile, partOffset, SEEK_SET );

		//Get part version
		SDL_RWread(srcUndoFile, &version, sizeof(version), 1);
		
		//Get part type
		SDL_RWread(srcUndoFile, &type, sizeof(type), 1);
	}

	SDL_RWseek( srcUndoFile, pos, SEEK_SET );
	return type;
}

U32 UndoControl::GetTopFullCRC()
{
	if(!srcUndoFile) return 0;
	U32 pos = SDL_RWtell( srcUndoFile ), crc = 0;

	//Get partend
	SDL_RWseek( srcUndoFile, -4, SEEK_CUR );

	Uint32 partEnd = SDL_ReadLE32(srcUndoFile);
	if(memcmp(&partEnd, PART_END, 4) == 0) 
	{
		//Get full crc
		SDL_RWseek( srcUndoFile, -16, SEEK_CUR );
		crc = SDL_ReadLE32(srcUndoFile);
	}

	SDL_RWseek( srcUndoFile, pos, SEEK_SET );
	return crc;
}

U32 UndoControl::GetPreviousPartOffSet()
{
	if(!srcUndoFile) return 0;
	U32 pos = SDL_RWtell( srcUndoFile ), partOffset = 0;

	//Get partend
	SDL_RWseek( srcUndoFile, -4, SEEK_CUR );

	Uint32 partEnd = SDL_ReadLE32(srcUndoFile);
	if(memcmp(&partEnd, PART_END, 4) == 0) 
	{
		//Get part offset
		SDL_RWseek( srcUndoFile, -12, SEEK_CUR );
		partOffset = SDL_ReadLE32(srcUndoFile);
	}

	SDL_RWseek( srcUndoFile, pos, SEEK_SET );
	return partOffset;
}

void UndoControl::EraseRedo()
{
	RedoStackIterator itList(redo);
	for(itList.Begin(); !itList.Done(); itList.Next())
	{
		CloseSDLFile(&(itList.Current().part));
	}

	redo.Clear();
}

void UndoControl::PushRedo(U32 top, U32 previousRedoTop, SDL_RWops *newState)
{
	//Save the laststate and top position
	if(!newState) return;

	SDL_RWops *diff = NULL, *full = NULL, *lastRedo = NULL;

	if(redo.Count())
	{
		lastRedo = redo.Front().part;
	}

	if(lastRedo)
	{
		bool bRes = GetDiff(lastRedo, newState, &diff);
		
		GLASSERT(bRes && diff);
		if(!bRes || !diff)
		{
			//Error: push the entire file if has some differences
			
			int sizeLast = lastRedo->hidden.mem.stop - lastRedo->hidden.mem.base,
				size = newState->hidden.mem.stop - newState->hidden.mem.base;
			
			if(size != sizeLast || memcmp(lastRedo->hidden.mem.base, newState->hidden.mem.base, size) != 0)
			{
				void *mem = malloc(sizeLast);
				if(!mem) return;
				
				memcpy(mem, newState->hidden.mem.base, size);
				
				full = SDL_RWFromMem(mem, size);
			}
			
			CloseSDLFile(&diff); //If get error into GetDiff
		}
	}
	else
	{
		full = newState;
	}

	
	if(diff || full)
	{
		//Push the current position to redo stack
		RedoPart part(top, previousRedoTop, full?full:diff, full?PART_TYPE_FULL_FILE:PART_TYPE_DIFF);
		redo.PushFront(part);
	}
}

bool UndoControl::Load(bool bShowErrorMessage)
{
	bool bRes = false;

	//Don't set fast file here
	//The flag will be set in the load

	EditorDirectory dir(GameControl::Get()->getGamePath());

	if(GameControl::Get()->Load(lastState, true, false, 0, false))
	{
		if(GameControl::Get()->UpdateLoadedActors(lastState, false))
		{
			ActorProperty::Update();
			bRes = true;
		}
		else
		{
			bRes = false;
		}
	}

	GameControl::Get()->setFastFile(false);


	if(!bShowErrorMessage) SDL_ClearError();

	return bRes;
}

void UndoControl::RemoveHeadPart(int topType)
{
	if(!srcUndoFile) return;
	U32 currrentPos = SDL_RWtell( srcUndoFile );

	SDL_RWseek( srcUndoFile, 0, SEEK_END );
	U32 endPos = SDL_RWtell( srcUndoFile );

	SDL_RWseek( srcUndoFile, currrentPos, SEEK_SET );

	if(currrentPos == endPos)
	{
		if(topType == PART_TYPE_FULL_FILE) Pop(false);
	}
}

bool UndoControl::IsTop()
{
	U32 currrentPos = SDL_RWtell( srcUndoFile );

	SDL_RWseek( srcUndoFile, 0, SEEK_END );
	U32 endPos = SDL_RWtell( srcUndoFile );

	SDL_RWseek( srcUndoFile, currrentPos, SEEK_SET );

	if(currrentPos == endPos)
	{
		return true;
	}

	return false;
}

bool UndoControl::PopLoad(bool bShowErrorMessage)
{
	//Get new state	
	SDL_RWops *newState = Pop(true);
	if(!newState) return false;


	//Update state
	SDL_RWops *lastStateCopy = lastState;
	lastState = newState;
	if(Load(bShowErrorMessage))
	{
		CloseSDLFile(&lastStateCopy);
	}
	else
	{
		GLASSERT(0);
		lastState = lastStateCopy;
		CloseSDLFile(&newState);

		if(lastState)
		{
			//Reload good state
			Load();
		}

		return false;
	}

	return true;
}

bool UndoControl::Undo()
{
#if defined(GAME_EDITOR_HOME_EDITION)
	new PanelInfo(GAME_EDITOR_VERSION_ERROR);
	return false;
#endif

	if(GameControl::Get()->getGameMode()) 
	{
		//No undo in game mode
		return false;
	}

	if(GameControl::Get()->GetModal()) 
	{
		//No undo when modal panels are open
		//Must use specialized undo (script, paths, ...)
		return false;
	}

	WaitCursor wait;

	CheckUndoFile();
	if(!srcUndoFile) return false;

	//Return false if not more undo
	char topType = CanUndo();
	if(topType < 0) return false;
	
	
	//Get top to redo
	if(!redo.Count())
	{
		//Push current state to first redo
		PushCurrentState();
		CheckUndoFile();

		U32 previousRedoTop = GetPreviousPartOffSet();
		PushRedo(SDL_RWtell( srcUndoFile ), previousRedoTop, lastState);
		
		//Restore current state
		//lastState closed by redo stack
		lastState = Pop(true);
		
	}
	else
	{
		U32 previousRedoTop = SDL_RWtell( srcUndoFile );

		//Go to previous state
		RedoPart part;
		part = redo.Front();		
		SDL_RWseek( srcUndoFile, part.previousRedoTop, SEEK_SET );

		//Get state
		SDL_RWops *newState = Pop(true);
		PushRedo(part.previousRedoTop, SDL_RWtell( srcUndoFile ), newState);
	}
	

	return PopLoad();
}

bool UndoControl::Redo()
{
#if defined(GAME_EDITOR_HOME_EDITION)
	new PanelInfo(GAME_EDITOR_VERSION_ERROR);
	return false;
#endif

	if(GameControl::Get()->getGameMode()) 
	{
		//No undo in game mode
		return false;
	}

	if(GameControl::Get()->GetModal()) 
	{
		//No undo when modal panels are open
		//Must use specialized undo (script, paths, ...)
		return false;
	}

	//Return false if not more redo
	if(!redo.Count()) return false;

	WaitCursor wait;

	CheckUndoFile();
	if(!srcUndoFile) return false;

	//Get part
	RedoPart part;
	part = redo.Front();
	redo.PopFront();

	SDL_RWops *newState = NULL;
	
	if(part.type == PART_TYPE_FULL_FILE)
	{
		//Get last state from file
		newState = part.part;		
	}
	else if(redo.Count())
	{
		//New state = laststate + diff		
		GetPatch(redo.Front().part, part.part, &newState);
		
		//Release part
		CloseSDLFile(&part.part);		
	}

	GLASSERT(newState);

	if(!newState) return false;

	//Update state
	CloseSDLFile(&lastState);
	lastState = newState;

	//Redo
	SDL_RWseek( srcUndoFile, part.top, SEEK_SET );
	PopLoad();

	return true;
}


#endif