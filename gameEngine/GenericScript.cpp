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


// GenericScript.cpp: implementation of the GenericScript class.
//
//////////////////////////////////////////////////////////////////////

#include "GenericScript.h"
#include "GameControl.h"

#include "eic.h"
#include "modules.h"
#include "preproc.h"


extern "C"
{
#include "global.h"
#include "symbol.h"

int EiC_lutClearFileEntries(char *FileName);
}

#undef malloc
#undef free

#include "dlmalloc.h"

MapCode GenericScript::mapCode;
ListCode GenericScript::listLoadedCodes;
ListString GenericScript::listLoadedPrototypes;
gedString GenericScript::error;

static stGenericCode *currentParserCode = NULL;
extern "C" void StoreFunctionPrototype(char *prototype)
{
	//Called by EiC parser
	if(currentParserCode)
	{
		currentParserCode->mapFunctionPrototypes.Add(prototype, 1);
	}
}

GenericScript::GenericScript()
{

}

GenericScript::~GenericScript()
{

}

void GenericScript::ParserAllFunctionPrototypes()
{
	//Parser prototypes one at once
	//If is someone wrong, don't affect the others

	MapCodeIterator it(mapCode);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		MapStringIterator itPrototype(it.Value()->mapFunctionPrototypes);	
		for(itPrototype.Begin(); !itPrototype.Done(); itPrototype.Next())
		{
			Script::ClearError();
			EiC_parseStringSingle((*itPrototype.Key()).getCharBuf());		
		}
	}

	Script::ClearError();
}

bool GenericScript::Add(const gedString &code, const gedString &name, /*bool bVerifySymbols,*/ bool bIgnoreErrors)
{
	bool bAddedWithErrors = false;
	stGenericCode tmp;

	if(mapCode[name])
	{
		//Edit
		RemoveCode(name); 
		ParserAllFunctionPrototypes();
		
		//Parser code
		Script::ClearError();
		gedString aux(code), aux2(name);

		currentParserCode = &tmp;
		EiC_parseStringFile(aux.c_str(), aux2.c_str()); //solve parser_bug.ged
		currentParserCode = NULL;

		error = Script::GetError();
		
		//New code Ok?
		if(error.length() > 0 /*&& error.find("Error") != gedString::npos*/) //Show warnings (Issue 94)
		{
			RemoveCode(name, true);

			if(!bIgnoreErrors)
			{
				RestoreCode(name); 
				return false;
			}

			if(error.find("Error") != gedString::npos)
			{
				bAddedWithErrors = true;
			}
		}
		AddCode(code, name, bAddedWithErrors);
	}
	else
	{
		//Add
		ParserAllFunctionPrototypes();
		
		//Parser code
		Script::ClearError();
		gedString aux(code), aux2(name);

		currentParserCode = &tmp;
		EiC_parseStringFile(aux.c_str(), aux2.c_str()); //solve parser_bug.ged
		currentParserCode = NULL;

		error = Script::GetError();
		
		if(error.length() > 0 /*&& error.find("Error") != gedString::npos*/) //Show warnings (Issue 94)
		{
			if(!bIgnoreErrors)
			{
				return false;
			}

			if(error.find("Error") != gedString::npos)
			{
				bAddedWithErrors = true;
			}
		}		
		
		AddCode(code, name, bAddedWithErrors);
	}
	
	//Add prototypes
	stGenericCode *newCode = mapCode[name];
	if(newCode)
	{
		newCode->mapFunctionPrototypes = tmp.mapFunctionPrototypes;
	}

	return true;
}

gedString GenericScript::Get(const gedString &name)
{
	stGenericCode *code = mapCode[name];
	if(!code) return "";
	
	return code->code;
}

bool GenericScript::GetCodeError(const gedString &name)
{
	stGenericCode *code = mapCode[name];
	if(!code) return false;
	
	return code->bAddedWithErrors;
}



void GenericScript::Remove(const gedString &name)
{
	RemoveCode(name);
	mapCode.Remove(name);
}



void GenericScript::Load(SDL_RWops *src, bool bMergeGames)
{
	bool bParserAll = false;
	if(!bMergeGames)
	{
		ClearGenericScripts();
		listLoadedCodes.Clear();
		listLoadedPrototypes.Clear();
	}

	Uint16 nCodes = GameControl::Read16(src);

	if(GameControl::Get()->GetGameFileVersion() >= 34)
	{
		//Load functions prototypes
		Uint16 nPrototypes = GameControl::Read16(src);

		for(int i = 0; i < nPrototypes; i++)
		{
			//Read code
			gedString prototype;
			ReadString(src, &prototype);
			
			
			if(GameControl::Get()->getStandAloneMode())
			{
				//Don't parser now to avoid link errors
				listLoadedPrototypes.PushBack(prototype);
			}
			else
			{	
				//Parser function prototype
				Script::ClearError();
				EiC_parseStringSingle(prototype.getCharBuf());				
			}		
		}		

		Script::ClearError();
	}
	

	for(int i = 0; i < nCodes; i++)
	{
		//Read name
		gedString name;
		ReadString(src, &name);

		//Read code
		gedString code;
		ReadString(src, &code);

			
#ifndef STAND_ALONE_GAME
		if(GameControl::Get()->GetGameFileVersion() < 37)
		{
			code.replaceAll("diretional_velocity", "directional_velocity");
		}

		if(GameControl::Get()->GetGameFileVersion() < 49)
		{
			//Move to with relative option
			Script::UpdateMoveTo(code);
		}
#endif

		if(GameControl::Get()->getStandAloneMode())
		{
			//Don't parser now, to faster load
			listLoadedCodes.PushBack(stNameCode(name, code));
		}
		else
		{	
			GenericScript script;
			script.Add(code, name, true);
			if(script.GetCodeError(name))
			{
				bParserAll = true;
			}
		}		
	}

#ifndef STAND_ALONE_GAME
	if(bParserAll)
	{		
		//Avoid error when edit a script of a fast saved file
		gedString scriptError;
		ListCode parserOrder;
		ParserGenericScripts(scriptError, parserOrder);
	}
#endif
}

void GenericScript::ParserLoadedCode()
{
	if(listLoadedCodes.size())
	{
		ClearGenericScripts();

		//Parser functions prototypes
		for(int i = 0; i < listLoadedPrototypes.Count(); i++)
		{
			Script::ClearError();
			EiC_parseStringSingle(listLoadedPrototypes[i].getCharBuf());
		}
		
		listLoadedPrototypes.Clear();
		Script::ClearError();

		//Parser all codes
		ListCodeIterator itList(listLoadedCodes);
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			GenericScript script;
			script.Add(itList.Current().code, itList.Current().name, true);
		}

		listLoadedCodes.Clear();
	}
}




void GenericScript::RemoveCode(const gedString &name, bool bRemovePrototypes)
{
	if(name.empty() || !name.getCharBuf()) return;

	//solve parser_bug.ged
	if(EiC_IsIncluded(name.getCharBuf()))
	{
		int r = EiC_lutClearFileEntries(name.getCharBuf());

		EiC_ClearFileMacros(name.getCharBuf());
		if(r)
		{
			EiC_rmIncludeFileName(name.getCharBuf());
		}
	}

	stGenericCode *code = mapCode[name];
	if(bRemovePrototypes && code)
	{
		//Clear functions prototypes for the code
		code->mapFunctionPrototypes.Clear();
	}
}

void GenericScript::RestoreCode(const gedString &name)
{
	stGenericCode *code = mapCode[name];
	if(!code) return;

	gedString aux(name);
	currentParserCode = code;
	EiC_parseStringFile(code->code.c_str(), aux.c_str()); //solve parser_bug.ged
	currentParserCode = NULL;
}

void GenericScript::AddCode(const gedString &code, const gedString &name, bool bAddedWithErrors)
{
	mapCode.Remove(name);
	mapCode.Add(name, stGenericCode(code, bAddedWithErrors));
}

void GenericScript::ClearGenericScripts()
{
	MapCodeIterator it(mapCode);
	for(it.Begin(); !it.Done(); it.Next())
	{
		RemoveCode(it.Key()->c_str());
	}

	mapCode.Clear();	
}






#ifndef STAND_ALONE_GAME

void GenericScript::Populate(ListPop *list)
{
	list->RemoveAll();

	MapCodeIterator it(mapCode);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		list->AddText(*it.Key());
	}

	list->Sort();
}

void GenericScript::Populate(ListString &list)
{
	//To the wx interface
	list.Clear();

	MapCodeIterator it(mapCode);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		gedString title("Global Code");
		title += ",";				
		title += *it.Key();

		list.PushBack(title);
	}

	list.Sort();
}

void GenericScript::SaveSoundTo(SDL_RWops *exeFile, MapAudioFileIndex &audioIndex)
{
	//Put all global script sounds into executable

	MapCodeIterator it(mapCode);
	ListSounds list;
	
	for(it.Begin(); !it.Done(); it.Next())
	{		
		if(it.Value()->code.find("PlaySound") != gedString::npos)
		{
			ExtractSoundPath(it.Value()->code, "PlaySound", list);
		}

		if(it.Value()->code.find("PlayMusic") != gedString::npos)
		{
			ExtractSoundPath(it.Value()->code, "PlayMusic", list);
		}
	}
	
	SaveSoundList(list, exeFile, audioIndex);
}	

void GenericScript::Save(SDL_RWops *src)
{
	ListCode parserOrder;
	MapCodeIterator it(mapCode);

	if(!GameControl::Get()->getFastFile())
	{
		//Fix the paths
		for(it.Begin(); !it.Done(); it.Next())
		{
			Script::ChangePaths(it.Value()->code);
		}		
		
		//Fix parser order
		gedString scriptError;
		ParserGenericScripts(scriptError, parserOrder);		
	}
	else
	{
		for(it.Begin(); !it.Done(); it.Next())
		{
			parserOrder.PushBack(stNameCode(*it.Key(), it.Value()->code, it.Value()->bAddedWithErrors));
		}
	}

	

	/////////////////////////////////////////////
	//Save
	SDL_WriteLE16(src, parserOrder.size());

	//Prototypes
	Uint16 nPrototypes = 0;
	for(it.Begin(); !it.Done(); it.Next())
	{
		MapStringIterator itPrototype(it.Value()->mapFunctionPrototypes);	
		for(itPrototype.Begin(); !itPrototype.Done(); itPrototype.Next())
		{
			nPrototypes++;		
		}
	}

	SDL_WriteLE16(src, nPrototypes);

	for(it.Begin(); !it.Done(); it.Next())
	{
		MapStringIterator itPrototype(it.Value()->mapFunctionPrototypes);	
		for(itPrototype.Begin(); !itPrototype.Done(); itPrototype.Next())
		{
			WriteString(src, *itPrototype.Key());
		}
	}

	//Codes
	ListCodeIterator itList(parserOrder);	
	for(itList.Begin(); !itList.Done(); itList.Next())
	{
		//Save name
		WriteString(src, itList.Current().name);

		//Save code
		WriteString(src, itList.Current().code);
	}
}

bool GenericScript::ParserGenericScripts(gedString &scriptError, ListCode &parserOrder)
{
	bool bGlobalCodeError = false;
	scriptError.clear();

	
	//Remove all codes to avoid crash in game mode
	ListCode currentOrder;
	MapCodeIterator it(mapCode);

	for(it.Begin(); !it.Done(); it.Next())
	{
		currentOrder.PushBack(stNameCode(*it.Key(), it.Value()->code, it.Value()->bAddedWithErrors));
		RemoveCode(*it.Key(), false);
	}

	

	//Put codes in correct parser order

	//if nMax <= 0 can save, but the user must fix the code
	int nMax = mapCode.size()*mapCode.size();

	do
	{
		ListCodeIterator itList(currentOrder);
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			Script::ClearError();

			//Don't use ParserAllFunctionPrototypes(); RestoreCode(itList.Current().name);
			//Solve the bug when load checkers_Crash.ged
			GenericScript script;
			script.Add(itList.Current().code, itList.Current().name, true);

			if(nMax <= 0 || Script::GetError().find("Error") == gedString::npos /*|| itList.Current().bAddedWithErrors*/)
			{
				//Parser Ok
				if(Script::GetError().find("Error") != gedString::npos || nMax <= 0)
				{
					if(scriptError.empty())
					{
						scriptError = "Global code:\n";
						bGlobalCodeError = true;
					}
					scriptError += itList.Current().name + "\n";	
				}

				parserOrder.PushBack(stNameCode(itList.Current().name, itList.Current().code, itList.Current().bAddedWithErrors));
				currentOrder.FindAndDelete(stNameCode(itList.Current().name, itList.Current().code, itList.Current().bAddedWithErrors));
				
				break;
			}
			else
				RemoveCode(itList.Current().name, false);

			nMax--;
		}

	} while(parserOrder.size() < mapCode.size());

	return bGlobalCodeError;
}

bool GenericScript::ParserAll()
{
	//Re parser codes
	gedString scriptError;
	ListCode parserOrder;
	bool bGlobalCodeError = ParserGenericScripts(scriptError, parserOrder);	

	//Re parser all scripts	to avoid crash in game mode
	//Errors in scripts can't causes errors on global codes
	
	GlobalMapActionsIterator itAction(Action::getGlobalMapActions());
	for(itAction.Begin(); !itAction.Done(); itAction.Next())
	{
		gedString err((*itAction.Value())->VerifyScripts(true));

		if(!err.empty() && bGlobalCodeError)
		{
			scriptError += "\nScripts:\n";
			bGlobalCodeError = false;
		}

		scriptError += err;
	}
	
	if(scriptError.length() > 0)
	{
		error = "There is errors in some scripts. Fix the errors and try again:\n\n";
		error += scriptError;
		
		return false;
	}
	
	return true;
}

bool GenericScript::RemoveAndVerify(const gedString &name, bool bIgnoreErrors)
{
	if(!mapCode[name]) return true;

	error.clear();

	RemoveCode(name); 
	ParserAllFunctionPrototypes();	

	//Re parser all scripts	
	gedString scriptError;
	error.clear();

	GlobalMapActionsIterator itAction(Action::getGlobalMapActions());
	for(itAction.Begin(); !itAction.Done(); itAction.Next())
	{
		scriptError += (*itAction.Value())->VerifyScripts();
	}

	if(!bIgnoreErrors && scriptError.length() > 0)
	{
		error = "Remove this code can cause error on:\n\n";
		error += scriptError;

		RestoreCode(name);
		
		//Restore scripts
		GlobalMapActionsIterator itAction(Action::getGlobalMapActions());
		for(itAction.Begin(); !itAction.Done(); itAction.Next())
		{
			(*itAction.Value())->VerifyScripts();
		}
		return false;
	}

	//Re parser all other codes
	MapCodeIterator it(mapCode);	
	scriptError.clear();

	for(it.Begin(); !it.Done(); it.Next())
	{
		gedString codeName = *it.Key();
		if(codeName != name)
		{
			RemoveCode(codeName);

			Script::ClearError();
			RestoreCode(codeName);

			if(Script::GetError().length() > 0 && Script::GetError().find("Error") != gedString::npos)
			{			
				scriptError += codeName + "\n";		
			}
		}
	}

	if(!bIgnoreErrors && scriptError.length() > 0)
	{
		error = "Remove this code can cause error on:\n\n";
		error += scriptError;

		RestoreCode(name);
		
		//Restore other codes
		MapCodeIterator it(mapCode);	
		for(it.Begin(); !it.Done(); it.Next())
		{
			gedString codeName = *it.Key();
			if(codeName != name)
			{
				RemoveCode(codeName);
				RestoreCode(codeName);	
			}
		}
		return false;
	}

	mapCode.Remove(name);
	return true;	
}

int GenericScript::RemoveLocalVar(const gedString &variable)
{
	int type = -1;
	char *p;

	gedString aux(variable);
	if((p = Script::getLocalVars().FindString(aux.c_str())) != NULL)
	{
		type = *p;
		Script::RemoveLocalVar(variable);
	}

	return type;
}

stVarInfo GenericScript::RemoveGlobalVar(const gedString &variable)
{
	stVarInfo varInfo, *p;

	gedString aux(variable);
	if((p = GameControl::Get()->getVars().FindString(aux.c_str())) != NULL)
	{
		varInfo = *p;
		GameControl::Get()->getVars().Remove(variable);
		Script::RemoveSymbol(aux.c_str());
	}

	return varInfo;
}

void GenericScript::RestoreVar(const gedString &variable, int type)
{
	Script::CreateLocalUserVar(variable, type);
}

void GenericScript::RestoreVar(const stVarInfo& varInfo)
{
	GameControl::Get()->CreateUserVar(varInfo.name, varInfo.type, varInfo.arrayLenght, varInfo.saveGroup);
}

bool GenericScript::RemoveAndVerifyVariable(const gedString& variable, int scope, stVarInfo *newVar, int newScope, bool bIgnoreErrors)
{
	error.clear();

	int type; //for local variables
	stVarInfo varInfo; //for global variables
	
	if(scope == LOCAL_VAR) 
	{
		type = RemoveLocalVar(variable); 
	}
	else
	{
		varInfo = RemoveGlobalVar(variable); 
	}


	if(newVar)
	{
		//Replace variable with newVar
		if(newScope == LOCAL_VAR) 
		{
			RestoreVar(newVar->name, newVar->type); 
		}
		else
		{
			RestoreVar(*newVar); 
		}
	}

	
	

	//Re parser all scripts	
	gedString scriptError;

	GlobalMapActionsIterator itAction(Action::getGlobalMapActions());
	for(itAction.Begin(); !itAction.Done(); itAction.Next())
	{
		scriptError += (*itAction.Value())->VerifyScripts();
	}

	if(!bIgnoreErrors && scriptError.length() > 0)
	{
		if(newVar) error = "Modify ";
		else error = "Remove ";

		error += "this variable can cause error on:\n\n";
		error += scriptError;

		//Remove new var
		if(newVar)
		{
			if(newScope == LOCAL_VAR) 
			{
				RemoveLocalVar(newVar->name); 
			}
			else
			{
				RemoveGlobalVar(newVar->name); 
			}
		}

		//Restore old var
		if(scope == LOCAL_VAR) 
		{
			RestoreVar(variable, type); 
		}
		else
		{
			RestoreVar(varInfo); 
		}
		
		//Restore scripts
		GlobalMapActionsIterator itAction(Action::getGlobalMapActions());
		for(itAction.Begin(); !itAction.Done(); itAction.Next())
		{
			(*itAction.Value())->VerifyScripts();
		}
		return false;
	}

	//Re parser all other codes
	MapCodeIterator it(mapCode);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		gedString codeName = *it.Key();
		
		RemoveCode(codeName);
		Script::ClearError();
		RestoreCode(codeName);
		
		if(Script::GetError().length() > 0 && Script::GetError().find("Error") != gedString::npos)
		{			
			scriptError += codeName + "\n";		
		}
		
	}

	if(!bIgnoreErrors && scriptError.length() > 0)
	{
		if(newVar) error = "Modify ";
		else error = "Remove ";

		error += "this variable can cause error on:\n\n";
		error += scriptError;

		//Remove new var
		if(newVar)
		{
			if(newScope == LOCAL_VAR) 
			{
				RemoveLocalVar(newVar->name); 
			}
			else
			{
				RemoveGlobalVar(newVar->name); 
			}
		}

		//Restore old var
		if(scope == LOCAL_VAR) 
		{
			RestoreVar(variable, type); 
		}
		else
		{
			RestoreVar(varInfo); 
		}
		
		//Restore other codes
		MapCodeIterator it(mapCode);	
		for(it.Begin(); !it.Done(); it.Next())
		{
			gedString codeName = *it.Key();
			
			RemoveCode(codeName);
			RestoreCode(codeName);	
			
		}
		return false;
	}

	return true;	
}


#endif //#ifndef STAND_ALONE_GAME




