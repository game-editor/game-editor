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


// FunctionCompletion.cpp: implementation of the FunctionCompletion class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "FunctionPanel.h"
#include "GameControl.h"
#include "ExpressionEditor.h"
#include "ActorProperty.h"

#include "eic.h"

extern "C"
{
#include "global.h"
#include "symbol.h"
}

#undef malloc
#undef free

#include "dlmalloc.h"



#define RETURN_TIP "Select a variable to store the returned value"

#define RETURN_TEXT "Store result"
#define NO_RETURN_TEXT "No store"





#define EDIT_CONTROL "edit"
#define LIST_CONTROL "list"
#define COLOR_CONTROL "color"

FunctionCompletion *FunctionCompletion::functionCompletion = NULL;

FunctionCompletion::FunctionCompletion()
{

}

FunctionCompletion::~FunctionCompletion()
{
	functionCompletion = NULL;
}

FunctionCompletion* FunctionCompletion::Get()
{
	if(!functionCompletion) functionCompletion = new FunctionCompletion();

	return functionCompletion;
}

void FunctionCompletion::GetArguments(TiXmlElement* root)
{
	TiXmlElement* child = NULL;
	
	for(child = root->FirstChildElement();	child; child = child->NextSiblingElement())
	{
		if(child->Value() == "argument")
		{
			const gedString *title, *c_type, *tip, *errorMsg;
			
			title = child->Attribute("title");
			c_type = child->Attribute("c_type");
			tip = child->Attribute("tip");
			errorMsg = child->Attribute("errorMsg");
			
			if(title && c_type)
			{
				//Get input
				TiXmlElement* input = child->FirstChildElement();
				if(input)
				{
					const gedString *type, *size;
					type = input->Attribute("type");
					size = input->Attribute("size");

					if(type)
					{
						stFuncionArgument argument;

						argument.c_type = *c_type;
						argument.title = *title;
						argument.inputType = *type;

						if(tip) argument.tip = *tip;
						if(errorMsg) argument.errorMsg = *errorMsg;
						if(size) argument.size = atol(size->getCharBuf());


			
						if(*type == EDIT_CONTROL)
						{
							if(input->Attribute("min")) argument.min = atof(input->Attribute("min")->getCharBuf());
							if(input->Attribute("max")) argument.max = atof(input->Attribute("max")->getCharBuf());							
							if(input->Attribute("step")) argument.step = atof(input->Attribute("step")->getCharBuf());	
							if(input->Attribute("variable_list")) argument.variable_list = true;
						}
						else if(*type == COLOR_CONTROL)
						{		
							if(input->Attribute("variable_list")) argument.variable_list = true;
						}
						else if(*type == LIST_CONTROL)
						{							
							const gedString *populateCmd = input->Attribute("populate");
							if(populateCmd) argument.populateCmd = *populateCmd;

							if(input->Attribute("variable_list")) argument.variable_list = true;
							
							//Get options
							TiXmlElement* option = NULL;
							
							for(option = input->FirstChildElement();	option; option = option->NextSiblingElement())
							{
								if(option->Value() == "option")
								{
									const gedString *name = option->Attribute("name"), 
													*value = option->Attribute("value"),
													*selected = option->Attribute("selected");
									
									if(name)
									{
										stFunctionInputOption opt;
										opt.name = *name;

										if(value) opt.value = *value;
										else opt.value = *name;

										if(selected) argument.selectedOption = *name;

										argument.listOptions.PushBack(opt);
									}
								}
							}
						}
						else
						{
							//Invalid type, don't consider this argument
							continue;
						}

						
						//Argument ok
						listArguments.PushBack(argument);
					}
				}				
			}
		}
	}
}

/*void FunctionCompletion::GetReturn(TiXmlElement* root)
{
	TiXmlElement* child = NULL;
	
	for(child = root->FirstChildElement();	child; child = child->NextSiblingElement())
	{
		if(child->Value() == "return")
		{
			const gedString *c_type, *storein;
			
			c_type = child->Attribute("c_type");
			storein = child->Attribute("storein");
			
			if(c_type && storein)
			{
				//Valid return
				returnType = *c_type;
				returnStorein = *storein;

				return;	
			}
		}
	}
}*/

bool FunctionCompletion::GetCallText(gedString _functionName)
{
	EditorDirectory editDir;

	TiXmlDocument doc;
	if(!doc.LoadFile("functions.xml")) return false;
	
	//Check tree
	TiXmlElement* root = NULL;
	TiXmlElement* child = NULL;
	
	root = doc.FirstChildElement("FunctionsDescription");
	if(!root) return false;
	
	for(child = root->FirstChildElement();	child; child = child->NextSiblingElement())
	{
		if(child->Value() == "function")
		{
			const gedString *name, *title, *tip, *pReturn;
			
			name = child->Attribute("name");
			title = child->Attribute("title");
			tip = child->Attribute("tip");
			pReturn = child->Attribute("return_type");
			
			if(name && *name == _functionName)
			{
				//Valid function
				functionName = _functionName;
				if(pReturn) return_type = *pReturn;
				
				GetArguments(child);				
				//GetReturn(child);

				if(IsEditingScript())
				{
					if(listArguments.Count() || pReturn)
					{
						//Generate panel
						
						gedString panelTitle, panelTip;
						if(title) panelTitle = *title;
						else panelTitle = *name;
						
						if(tip) panelTip = *tip;
						
						new FunctionPanel(panelTitle, panelTip, this);
					}
					else
					{
						//No arguments, insert text now
						
						gedString text(*name);
						text += "();";
						InsertIntoScript(text.getCharBuf());
						
						delete this;
					}
					
					return true;
				}
			}
		}
	}
	

	delete this;
	return false;
}






enum
{
	BT_ADD,
	BT_CANCEL	
};


FunctionPanel *FunctionPanel::functionPanel = NULL;

FunctionPanel::FunctionPanel(gedString _panelTitle, gedString _panelTip, FunctionCompletion *_functionCompletion)
: Panel(_panelTitle, 0, 0, 50, 50)
{
	SetModal();
	if(!_panelTip.empty()) SetToolTip(_panelTip);

	functionCompletion = _functionCompletion;
	functionPanel = this;
	listReturn = NULL;
	listActor = NULL;
	lastListControl = NULL;
	lastVariableSelector = NULL;
	listActorVariables = NULL;
	bHasActorVariable = false;
	bVerfyErrors = false;



	Text *text, *titleText;
	Button *button;	
	int y, width = 0, height = 0;

	GetCurrentCode();


	//Adjust text to make a align right
	int nControls = functionCompletion->getListArguments()->Count(), maxText = 0, i;
	gedString textSpace;

	for(i = 0; i < nControls; i++)
	{
		int len = (*functionCompletion->getListArguments())[i].title.length();
		maxText = max(maxText, len);
	}

	if(!functionCompletion->getReturnType().empty())
	{
		int len = strlen(RETURN_TEXT);
		maxText = max(maxText, len);
	}

	for(i = 0; i < maxText; i++)
	{
		textSpace += ' ';
	}

	titleText = text = AddText(_panelTitle, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	int lastY = y;

	for(i = 0; i < nControls; i++)
	{
		stFuncionArgument *pArgument = &(*functionCompletion->getListArguments())[i];
		gedString title(pArgument->title);

		pArgument->controlId = i;


		int nLeftSpaces = maxText - title.length();

		if(nLeftSpaces > 0)
		{
			gedString space(textSpace.c_str(), nLeftSpaces);
			title = space + title;
		}

		title += ": ";

		text = AddText(title, 10, lastY + 3);

		if(pArgument->inputType == EDIT_CONTROL)
		{			
			pArgument->editBox = AddEditBox(text->Right(), text->Top(), pArgument->size);
			if(pArgument->max > pArgument->min)
			{
				pArgument->editBox->SetNumeric(pArgument->min, pArgument->max, pArgument->step);
			}

			if(pArgument->variable_list)
			{
				ListData listVariable = GetVariableList(pArgument->c_type);
				pArgument->editBox->SetVariableList(listVariable, pArgument->c_type.find('*') != gedString::npos);
			}

			//if(!pArgument->tip.empty()) pArgument->editBox->SetToolTip(pArgument->tip);

			lastY = pArgument->editBox->Down();
			int newWidth = pArgument->editBox->Right();
			width = max(width, newWidth);
		}
		else if(pArgument->inputType == COLOR_CONTROL)
		{			
			pArgument->color = new ColorSample(this, text->Right(), text->Top(), 33, 0, true);
			
			pArgument->color->setColor(0, 0, 0);
			pArgument->color->getRedEdit()->SetText("");
			pArgument->color->getGreenEdit()->SetText("");
			pArgument->color->getBlueEdit()->SetText("");

			if(pArgument->variable_list)
			{
				ListData listVariable = GetVariableList(pArgument->c_type);
				pArgument->color->getRedEdit()->SetVariableList(listVariable, pArgument->c_type.find('*') != gedString::npos);
				pArgument->color->getGreenEdit()->SetVariableList(listVariable, pArgument->c_type.find('*') != gedString::npos);
				pArgument->color->getBlueEdit()->SetVariableList(listVariable, pArgument->c_type.find('*') != gedString::npos);
			}

			if(!pArgument->tip.empty()) pArgument->color->SetToolTip(pArgument->tip);

			lastY = pArgument->color->Down() + 5;

			int newWidth = pArgument->color->Right();
			width = max(width, newWidth);
		}
		else if(pArgument->inputType == LIST_CONTROL)
		{
			pArgument->listPop = AddListPop(text->Right(), text->Top() - 2, pArgument->size, 0, pArgument->controlId);

			if(!pArgument->tip.empty()) pArgument->listPop->SetToolTip(pArgument->tip);

			lastY = pArgument->listPop->Down() + 2;
			int newWidth = pArgument->listPop->Right();
			width = max(width, newWidth);

			PopulateListByCommand(pArgument->listPop, pArgument->populateCmd);

			if(pArgument->variable_list)
			{
				ListData list = GetVariableList(pArgument->c_type);
				for(int i1 = 0; i1 < list.Count(); i1++)
				{
					gedString str(list[i1].text);					
					pArgument->listPop->AddText(str, false, -1, list[i1].data);
				}
			}

			//Add options
			int nOptions = pArgument->listOptions.Count();

			for(int io = 0; io < nOptions; io++)
			{
				gedString option(pArgument->listOptions[io].name);

				if(option != "separator")
				{
					pArgument->listPop->AddText(option);
				}
				else
				{
					pArgument->listPop->AddSeparator();
				}
			}

			//Set default
			pArgument->listPop->SetItem(pArgument->selectedOption);
		}
	}	

	//Return
	int yReturn = 0;
	if(!functionCompletion->getReturnType().empty())
	{
		yReturn = lastY + 5;
		lastY = yReturn + 8;

		gedString title(RETURN_TEXT);
		int nLeftSpaces = maxText - title.length();

		if(nLeftSpaces > 0)
		{
			gedString space(textSpace.c_str(), nLeftSpaces);
			title = space + title;
		}

		title += ": ";

		text = AddText(title, 10, lastY + 3);

		listReturn = AddListPop(text->Right(), text->Top() - 2, 128);		
		listReturn->SetToolTip(RETURN_TIP);
		
		lastY = listReturn->Down();
		int newWidth = listReturn->Right();
		width = max(width, newWidth);

		listReturn->AddText(NO_RETURN_TEXT);
		
		ListData list = GetVariableList(functionCompletion->getReturnType());
		for(int i1 = 0; i1 < list.Count(); i1++)
		{
			gedString str(list[i1].text);

			//Don't add read only vars
			if(!Script::IsVariableReadOnly(str))
			{
				listReturn->AddText(str, false, -1, list[i1].data);
			}
		}

		if(listReturn->GetText(1) != ACTOR_VARIABLES)
		{
			listReturn->AddSeparator(1);
		}

		listReturn->SetItem(NO_RETURN_TEXT);
	}
	
	//Adjust dimensions
	width += 10 + SHADOW_THICKNESS;
	height = lastY + 40 + SHADOW_THICKNESS;
	
	//Resize panel
	getImage()->SetDimensions(
		(GameControl::Get()->Width() - width)/2, 
		(GameControl::Get()->Height() - height)/2, 
		width, height);

	Draw();

	titleText->getImage()->SetPos((width - SHADOW_THICKNESS - titleText->Width())/2, titleText->getImage()->Y());
	DrawHLine(titleText->Down() + 2);
	if(yReturn) DrawHLine(yReturn);

	
	//End dialog
	y = DrawHLine(lastY + 5);
	button = AddButton("Add", (width - SHADOW_THICKNESS - 129)/2, y, 0, 0, BT_ADD); //SetConfirmButton(button);
	button = AddButton("Cancel", button->Right() + 5, button->Top(), 0, 0, BT_CANCEL); //SetCancelButton(button);			
}

FunctionPanel::~FunctionPanel()
{
	int nControls = functionCompletion->getListArguments()->Count();

	for(int i = 0; i < nControls; i++)
	{
		stFuncionArgument *pArgument = &(*functionCompletion->getListArguments())[i];		

		if(pArgument->color)
		{
			delete pArgument->color;
		}
	}

	functionPanel = NULL;
	delete functionCompletion;	
}


void FunctionPanel::PopulateListByCommand(ListPop *listPop, gedString populateCmd)
{
	//Execute populate command
	if(!listPop || populateCmd.empty()) return;
	populateCmd.lower();

	if(populateCmd.find("populateactors") != gedString::npos)
	{
		bool bGetClones = populateCmd.find("true") != gedString::npos;
		GameControl::Get()->PopulateActors(listPop, bGetClones);
	}
	else if(populateCmd.find("populateanimations") != gedString::npos)
	{
		Actor *eventActor = ActorProperty::GetCurrentEventActor();

		if(eventActor)
		{
			GameControl::Get()->PopulateAnimations(eventActor, listPop);
		}
	}
	else if(populateCmd.find("populatesavegroups") != gedString::npos)
	{
		GameControl::Get()->PopulateGlobalUserVarsGroups(listPop);
		listPop->Sort();
	}
}

void FunctionPanel::VisibleVariableCallBack(char *name, int bPointer)
{
	if( strcmp(name, "conditionalResult") != 0 &&
		strcmp(name, "gedMaxGameMem") )
	{
		functionPanel->variableList.PushBack(stListData(name, (void *)(bPointer?1:0)));
	}

	if(Script::IsActorVariable(name))
	{
		functionPanel->bHasActorVariable = true;
	}
}

void FunctionPanel::GetCurrentCode()
{
	scriptCodeStart = GetEditingScriptTextUntilCursor();
	

	//Test if current code is rith to get the panel errors
	//Use the invalid symbol ged_Error to test
	

	Script::ClearError();
	gedString code(scriptCodeStart + "{ged_Error;}"), error;
	
	EiC_parseStringSingle((char *)code.c_str());
	
	error = Script::GetError();
	Script::ClearError();

	if(error.find("ged_Error") != gedString::npos)
	{
		bVerfyErrors = true;
	}
}

ListData FunctionPanel::GetVariableList(gedString c_type)
{
	int bPointer = 0, bIncludeConst = 0, type = -1;
	GlDynArray<int> listTypes;

	variableList.Clear();
	bHasActorVariable = false;

	if(c_type.find("const") != gedString::npos) bIncludeConst = 1;
	if(c_type.find("*") != gedString::npos) bPointer = 1;

	if(bPointer)
	{
		if(c_type.find("double") != gedString::npos) listTypes.PushBack(t_double);
		else if(c_type.find("float") != gedString::npos) listTypes.PushBack(t_float);
		else if(c_type.find("long") != gedString::npos) listTypes.PushBack(t_long);
		else if(c_type.find("int") != gedString::npos) listTypes.PushBack(t_int);
		else if(c_type.find("short") != gedString::npos) listTypes.PushBack(t_short);
		else if(c_type.find("char") != gedString::npos) listTypes.PushBack(t_char);
	
	}
	else
	{
		if( c_type.find("double") != gedString::npos || 
			c_type.find("float") != gedString::npos ||
			c_type.find("int") != gedString::npos || 
			c_type.find("short") != gedString::npos ||
			c_type.find("long") != gedString::npos )
		{
			listTypes.PushBack(t_double);
			listTypes.PushBack(t_float);
			listTypes.PushBack(t_long);
			listTypes.PushBack(t_ulong);
			listTypes.PushBack(t_int);
			listTypes.PushBack(t_uint);
			listTypes.PushBack(t_short);
			listTypes.PushBack(t_ushort);
		}
	}

	

	if(listTypes.Count())
	{
		for(int i = 0; i < listTypes.Count(); i++)
		{
			GetVisibleVars((scriptCodeStart + "ged_ProbeValidVars();").getCharBuf(), listTypes[i], bPointer, bIncludeConst, VisibleVariableCallBack, NULL);
			if(bPointer)
			{
				//Get the normal variables too
				//Must be write as reference: &variable
				GetVisibleVars((scriptCodeStart + "ged_ProbeValidVars();").getCharBuf(), listTypes[i], 0, bIncludeConst, VisibleVariableCallBack, NULL);
			}
		}
	}
	else
	{
		//Is not internal C type
		//Create a test var with the type
		Script::ClearError();	
		
		EiC_parseStringSingle((char *)(c_type + " _ged_Test_Type;").c_str());
		
		if(Script::GetError().empty())
		{
			//Type is ok
			//Find variables of this type

			symentry_t *testType = (symentry_t *)isSymbolInEic("_ged_Test_Type");
			if(testType && testType->type)
			{
				GetVisibleVars((scriptCodeStart + "ged_ProbeValidVars();").getCharBuf(), 0, bPointer, bIncludeConst, VisibleVariableCallBack, testType->type);
			}
		}

		Script::ClearError();
		Script::RemoveSymbol("_ged_Test_Type");
	}

	variableList.Sort();

	if(bHasActorVariable)
	{
		variableList.PushFront(stListData(LIST_SEPARATOR, 0));
		variableList.PushFront(stListData(ACTOR_VARIABLES, 0));
	}

	return variableList;
}

bool FunctionPanel::NeedReference(ListPop *list, gedString text)
{
	if(!list) return false;

	ListPop *compareList = lastListControl;

	if(text.empty())
	{
		text = list->GetText(list->GetSelectedIndex());

		//Remove actor
		if(text.find('.') != gedString::npos)
		{
			text = text.substr(text.find('.') + 1);
		}

		compareList = list;
	}

	
	bool bPointerVar = list->GetItemData(text) != NULL, bPointerField = false;
	
	int nControls = functionCompletion->getListArguments()->Count();
	stFuncionArgument *pArgument;
	
	for(int i = 0; i < nControls; i++)
	{
		pArgument = &(*functionCompletion->getListArguments())[i];
		bool bFound = false;
		
		if(pArgument->editBox && lastVariableSelector && pArgument->editBox == lastVariableSelector->getEdit())
		{		
			bFound = true;
		}
		else if(pArgument->listPop && pArgument->listPop == compareList)
		{
			bFound = true;
		}
		
		if(bFound)
		{
			bPointerField = pArgument->c_type.find('*') != gedString::npos;
		}
	}
	
	return bPointerField && !bPointerVar;
}

bool FunctionPanel::OnList(ListPop *list, int index, gedString &text, int listId)
{
	//if(list == listReturn)
	{
		if(text == ACTOR_VARIABLES)
		{
			PostMessage(this, LIST_ACTOR_VARIABLES, LIST_ACTOR_VARIABLES);	
			list->SetItem(NO_RETURN_TEXT);
		}
	}

	if(list == listActorVariables && !currentSelectedActor.empty())
	{
		if(currentSelectedActor == S_PARENT_ACTOR) currentSelectedActor = "parent";
		else if(currentSelectedActor == S_CREATOR_ACTOR) currentSelectedActor = "creator";
		else if(currentSelectedActor == S_EVENT_ACTOR) currentSelectedActor.clear();

		gedString actorVar(currentSelectedActor);
		if(!actorVar.empty()) actorVar += '.';
		actorVar += text;

		/*
		bool bPointerVar = list->GetItemData(index) != NULL, bPointerField = false;

		int nControls = functionCompletion->getListArguments()->Count();

		for(int i = 0; i < nControls; i++)
		{
			stFuncionArgument *pArgument = &(*functionCompletion->getListArguments())[i];
			bool bFound = false;
			
			if(pArgument->editBox && lastVariableSelector && pArgument->editBox == lastVariableSelector->getEdit())
			{		
				bFound = true;
			}
			else if(pArgument->listPop && pArgument->listPop == lastListControl)
			{
				bFound = true;
			}

			if(bFound)
			{
				bPointerField = pArgument->c_type.find('*') != gedString::npos;
			}
		}*/
		
		//Use list (lastListControl don't works when select an actor variable in vectoradd)
		if(NeedReference(list, text))
		{
			if(actorVar[0] != '&') actorVar = gedString("&") + actorVar;
		}

		if(lastListControl)
		{
			lastListControl->SetListText(actorVar);
		}
		else if(lastVariableSelector)
		{
			lastVariableSelector->getEdit()->InsertText(actorVar);
		}
	}

	if(list == listActor)
	{
		//Actor selected, create the variable list for last control
		ListPop *lastList = NULL;

		if(lastListControl) lastList = lastListControl;
		else if(lastVariableSelector) lastList = lastVariableSelector->getList();
		
		if(lastList)
		{
			currentSelectedActor = text;
			
			if(!listActorVariables)
			{
				listActorVariables = AddListPop(0, 0, 1, 1);			
			}
			
			listActorVariables->RemoveAll();
			
			//Populate
			for(int i = 0; i < lastList->Count(); i++)
			{
				gedString var(lastList->GetText(i));
				if(Script::IsActorVariable(var) && (lastList != listReturn || !Script::IsVariableReadOnly(var)))
				{
					listActorVariables->AddText(var, false, -1, lastList->GetItemData(i));
				}
			}
			
			//Call list
			listActorVariables->OnMouseButtonDown(GameControl::Get()->getMouseX(), GameControl::Get()->getMouseY(), 255);
		}
	}

	return true;
}

bool FunctionPanel::OnListButtonDown(ListPop *list, int listId)
{
	lastListControl = list;
	lastVariableSelector = NULL;

	currentSelectedActor.clear();

	return true;
}

void FunctionPanel::OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message)
{
	if(messageId == LIST_ACTOR_VARIABLES)
	{
		if(!listActor)
		{
			listActor = AddListPop(0, 0, 1, 1);

			GameControl::Get()->PopulateActors(listActor, true);

			listActor->AddText(S_EVENT_ACTOR);
			listActor->AddText(S_PARENT_ACTOR);
			listActor->AddText(S_CREATOR_ACTOR);
		}	
		
		listActor->OnMouseButtonDown(GameControl::Get()->getMouseX(), GameControl::Get()->getMouseY(), 255);
	}
	else if(messageId == CLICK_ON_VARIABLE_SELECTOR)
	{
		lastListControl = NULL;
		lastVariableSelector = (VariableSelector *)message;

		currentSelectedActor.clear();
	}
}

void FunctionPanel::OnEditChar(EditBox *edit, SDLKey key, int line)
{
	
}

void FunctionPanel::BuidFunctionCall()
{
	//Buid text if ok
	//In list arguments, get the value option, not the name
	
	if(!IsEditingScript()) return;

	
	gedString text;

	if(!scriptCodeStart.empty())
	{
		char last = scriptCodeStart.at(scriptCodeStart.length() - 1);
		if(last != ' ' && last != '\t' && last != '\r' && last != '\n')
		{
			text += " ";
		}
	}

	bool bReturnString = false;
	if(listReturn && listReturn->GetText() != NO_RETURN_TEXT)
	{
		if(functionCompletion->getReturnType().find("char") != gedString::npos && functionCompletion->getReturnType().find("*") != gedString::npos)
		{
			bReturnString = true;
			text += gedString("strcpy(") + listReturn->GetText() + ", ";
		}
		else
		{		
			text += listReturn->GetText() + " = ";
		}
	}
	
	text += functionCompletion->getFunctionName() + "(";


	int nControls = functionCompletion->getListArguments()->Count(), i;

	for(i = 0; i < nControls; i++)
	{
		stFuncionArgument *pArgument = &(*functionCompletion->getListArguments())[i];
		gedString value[3];
		int nValues = 1;

		bool bString = false;
		
		if( pArgument->bNeedQuotationMarks || 
			(pArgument->listPop && !pArgument->variable_list && pArgument->c_type.find("char") != gedString::npos && pArgument->c_type.find("*") != gedString::npos)
			)
		{
			//If is list and argument type is string, put quotation marks
			bString = true;
		}

		if(pArgument->inputType == EDIT_CONTROL)
		{			
			value[0] = pArgument->editBox->GetText();
			nValues = 1;
		}
		else if(pArgument->inputType == COLOR_CONTROL)
		{			
			value[0] = pArgument->color->getRedEdit()->GetText();
			value[1] = pArgument->color->getGreenEdit()->GetText();
			value[2] = pArgument->color->getBlueEdit()->GetText();

			nValues = 3;
		}
		else if(pArgument->inputType == LIST_CONTROL)
		{
			gedString option(pArgument->listPop->GetText());
			
			int nOptions = pArgument->listOptions.Count();

			for(int io = 0; io < nOptions; io++)
			{
				if(pArgument->listOptions[io].name == option)
				{
					if(!pArgument->listOptions[io].value.empty())
					{
						value[0] = pArgument->listOptions[io].value;
					}
					
					break;
				}
			}

			if(value[0].empty()) value[0] = option;	
			nValues = 1;

			if(!bString && NeedReference(pArgument->listPop))
			{
				if(value[0][0] != '&') value[0] = gedString("&") + value[0];
			}
		}
		
		

		for(int i0 = 0; i0 < nValues; i0++)
		{
			if(pArgument->bNeedCast)
			{
				//Put a cast to avoid "Cast loses const qualifier" error
				text += "(";
				text += pArgument->c_type;
				text += ")";
			}			
			
			if(bString) text += "\"";		
			
			text += value[i0];
			
			if(bString) text += "\"";

			if(i0 < nValues - 1) text += ", ";
		}

		if(i < nControls - 1) text += ", ";
	}	
		
	if(bReturnString)
	{
		text += ")";
	}

	text += "); ";

	InsertIntoScript(text.getCharBuf());	
}

void FunctionPanel::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_ADD:	
		if(VerifyData())
		{
			BuidFunctionCall();
			delete this;
		}
		break;

	case BT_CANCEL:
		delete this;
		break;
	}

	
}

bool FunctionPanel::VerifyData()
{
	gedString error;
	int nControls = functionCompletion->getListArguments()->Count();

	for(int i = 0; i < nControls; i++)
	{
		stFuncionArgument *pArgument = &(*functionCompletion->getListArguments())[i];		

		if(pArgument->editBox)
		{		
			bool bArgumentError = false;

			if(pArgument->editBox->GetText().empty() && !pArgument->errorMsg.empty())
			{
				//Test empty
				error += gedString("\'") + pArgument->title + "\': " + pArgument->errorMsg + "\n";			

				bArgumentError = true;
			}
			else if(pArgument->editBox->GetText().isNumber() && pArgument->max > pArgument->min)
			{
				//Test valid range
				double value = atof(pArgument->editBox->GetText().c_str());
				
				if(value < pArgument->min || value > pArgument->max)
				{
					error += gedString("\'") + pArgument->title + "\' value should be between " + gedString(pArgument->min) + " and " + gedString(pArgument->max) + "\n";
					bArgumentError = true;
				}
			}

			if(!bArgumentError)
			{
				gedString errorArgument(VerifyArgument(pArgument));

				if(!errorArgument.empty())
				{
					error += gedString("\'") + pArgument->title + "\': " + errorArgument + "\n";
				}
			}
		}
		else if(pArgument->color)
		{
			EditBox *editColor[3] = {
				pArgument->color->getRedEdit(),
				pArgument->color->getGreenEdit(),
				pArgument->color->getBlueEdit()
			};

			char *text[3] = {"red", "green", "blue"};
			
			for(int i = 0; i < 3; i++)
			{
				bool bArgumentError = false;
				
				if(editColor[i]->GetText().empty())
				{
					//Test empty
					error += gedString("\'") + pArgument->title + ", " + text[i] + "\': Please, enter a value\n";			
					
					bArgumentError = true;
				}
				
				if(!bArgumentError)
				{
					gedString errorArgument(VerifyArgument(pArgument, editColor[i]));
					
					if(!errorArgument.empty())
					{
						error += gedString("\'") + pArgument->title + ", " + text[i] + "\': " + errorArgument + "\n";
					}
				}
			}
		}
		else if(pArgument->listPop && 
			((!pArgument->variable_list && pArgument->listPop->GetSelectedIndex() == -1) || pArgument->listPop->GetText().empty()) && 
			!pArgument->errorMsg.empty())
		{
			//Test select
			error += gedString("\'") + pArgument->title + "\': " + pArgument->errorMsg + "\n";			
		}
	}

	if(!error.empty())
	{
		//Remove last \n
		error = error.substr(0, error.length() - 1);

		//Show panel
		new PanelInfo(error, "Error", (error.find('\n') != gedString::npos)?ALIGN_LEFT:ALIGN_CENTER);
		return false;
	}

	return true;
}

gedString FunctionPanel::VerifyArgument(stFuncionArgument *pArgument, EditBox *edit)
{
	gedString errorArgument;

	pArgument->bNeedCast = false;
	pArgument->bNeedQuotationMarks = false;

	if(!edit) edit = pArgument->editBox;

	if(bVerfyErrors && edit)
	{
		Script::ClearError();

		//Begin error
		gedString code(scriptCodeStart + "{int _ged_1 = ged_Error;\n"), error;

		//Build test
		code += pArgument->c_type + " _ged_Test = " + edit->GetText() + ";\n";

		//End error
		code += "int _ged_2 = ged_Error1;\n";

		code += "}";
		
		EiC_parseStringSingle((char *)code.c_str());
		
		error = Script::GetError();
		Script::ClearError();
		
		//Remove all errors before ged_Error
		int i;
		if((i = error.find("ged_Error")) != gedString::npos)
		{
			error = error.substr(i + strlen("ged_Error") + 1);

			if((i = error.rfind("ged_Error1")) != gedString::npos)
			{
				//Remove all errors after ged_Error1
				error = error.substr(0, i);
				
				if((i = error.rfind(':')) != gedString::npos)
				{
					error = error.substr(0, i);
				}
			}

			if((i = error.find(':')) != gedString::npos)
			{
				error = error.substr(i + 1);

				if((i = error.find('\n')) != gedString::npos)
				{
					error = error.substr(0, i);
				}

				error.trimLeft();
				error.trimRight();

				if(error.find("const qualifier") == gedString::npos)
				{
					if(pArgument->c_type.find("char") != gedString::npos && pArgument->c_type.find("*") != gedString::npos)
					{
						//No error on string (put quotation marks)
						pArgument->bNeedQuotationMarks = true;
					}
					else
					{
						//Not string, show error
						errorArgument = error;
					}					
				}
				else
				{
					//A cast will solve the problem
					pArgument->bNeedCast = true;
				}
			}
		}

	}

	return errorArgument;
}


#endif