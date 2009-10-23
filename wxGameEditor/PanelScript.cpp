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


#include "PanelScript.h"
#include "PanelActors.h"
#include "GenericScript.h"



#define GLOBAL_CODE (void *)1

IMPLEMENT_CLASS(PanelScript, wxPanel)

BEGIN_EVENT_TABLE(PanelScript, wxPanel)
	EVT_SIZE(PanelScript::OnSize)
	EVT_SHOW(PanelScript::OnShow) 
	EVT_COMBOBOX( ID_ACTOR_CODE_SELECTOR, PanelScript::OnSelect )
	EVT_MENU(ID_REMOVE_SCRIPT, PanelScript::OnRemove )
	EVT_MENU(ID_APPLY_CHANGES, PanelScript::OnApplyChanges )
	EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, PanelScript::OnPageChanged)
	EVT_STE_STATE_CHANGED(wxID_ANY, PanelScript::OnSTEState)
END_EVENT_TABLE()

PanelScript *PanelScript::panelScript = NULL;

PanelScript::PanelScript(wxWindow *parent) : wxPanel(parent)
{
	
	panelScript = this;

	toolBar = new wxToolBar(this, 23,
                                       wxDefaultPosition,
									   wxDefaultSize, 
                                       wxTB_FLAT|wxTB_HORIZONTAL|wxTB_NODIVIDER|wxNO_BORDER//maks ((GetExtraStyle()&wxPG_EX_NO_FLAT_TOOLBAR)?0:wxTB_FLAT)
                                        /*| wxTB_HORIZONTAL | wxNO_BORDER*/ );
	CreateToolBar();	
	CreateNotebook();	
}

PanelScript::~PanelScript(void)
{
	panelScript = NULL;

	delete script;
	delete actorCodeSelector;
}

void PanelScript::CreateToolBar()
{
	wxBitmap bitmapAdd, bitmapDel, bitmapApply;

	bitmapAdd.LoadFile(wxT("event_add_b.gif"), wxBITMAP_TYPE_GIF);
	bitmapDel.LoadFile(wxT("event_del_b.gif"), wxBITMAP_TYPE_GIF);
	bitmapApply.LoadFile(wxT("save_changes.gif"), wxBITMAP_TYPE_GIF);

	
	toolBar->AddTool(ID_ADD_SCRIPT, bitmapAdd, wxT("Add Script"));
	toolBar->AddTool(ID_REMOVE_SCRIPT, bitmapDel, wxT("Remove Script"));
	toolBar->AddTool(ID_APPLY_CHANGES, bitmapApply, wxT("Apply Changes"));

	actorCodeSelector = new wxPGOwnerDrawnComboBox();
	wxString choices[] = {NO_SELECTION};
	actorCodeSelector->Create(toolBar, ID_ACTOR_CODE_SELECTOR, NO_SELECTION, wxDefaultPosition, wxDefaultSize, 1, choices, (wxComboPaintCallback) &wxPropertyGrid::OnComboItemPaint, wxODCB_TREE);
	actorCodeSelector->GetTextCtrl()->SetEditable(false);
	toolBar->AddControl(actorCodeSelector);	

	codeFunctionSelector = new wxListCtrl(toolBar, ID_CODE_FUNCTION_SELECTOR, wxDefaultPosition);
	toolBar->AddControl(codeFunctionSelector);	

	toolBar->SetBackgroundColour(wxBackground_Pen);

	toolBar->EnableTool(ID_APPLY_CHANGES, false);

	toolBar->Realize();
}

void PanelScript::CreateNotebook()
{
	wxSTEditorOptions steOptions(STE_DEFAULT_OPTIONS);
	steOptions.SetNotebookOption(STN_ALPHABETICAL_TABS, false); //Loose the images if sort
	steOptions.SetNotebookOption(STN_ALLOW_NO_PAGES, true);
	
	script = new wxSTEditorNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_BOTTOM | wxNO_BORDER );
	
	script->CreateOptions(steOptions);

	wxSTEditorPrefs::GetGlobalEditorPrefs().SetPrefBool(STE_PREF_INDENT_GUIDES, false, false);
	wxSTEditorPrefs::GetGlobalEditorPrefs().SetPrefInt(STE_PREF_EDGE_MODE, wxSTC_EDGE_NONE, false);
	wxSTEditorPrefs::GetGlobalEditorPrefs().SetPrefBool(STE_PREF_LOAD_INIT_LANG, false, false);
	wxSTEditorPrefs::GetGlobalEditorPrefs().SetPrefBool(STE_PREF_VIEW_LINEMARGIN, false, false);
	wxSTEditorPrefs::GetGlobalEditorPrefs().SetPrefBool(STE_PREF_USE_TABS, true, false);

	

	wxSTEditorPrefs::GetGlobalEditorPrefs().UpdateAllEditors();
	wxSTEditorStyles::GetGlobalEditorStyles().UpdateAllEditors();
	wxSTEditorLangs::GetGlobalEditorLangs().UpdateAllEditors();	
}

void PanelScript::RemoveEditor(gedString &name)
{
	int page = script->FindEditorPageByFileName(name.c_str());

	if(page != wxNOT_FOUND)
	{
		script->DeletePage(page);
		if(actorCodeSelector->GetTextCtrl()->GetValue() == name.c_str())
		{
			int sel = script->GetSelection();
			if(sel < 0)
			{
				actorCodeSelector->GetTextCtrl()->SetValue(NO_SELECTION);
			}
			else
			{
				actorCodeSelector->GetTextCtrl()->SetValue(script->GetEditor(sel)->GetFileName());
			}
		}
	}	

	mapAction.Remove(name);
}


void PanelScript::OnShow( wxShowEvent &event )
{
	if(event.GetShow())
	{
		Update();
	}
}

void PanelScript::OnSize(wxSizeEvent & event)
{
	wxSize szToolBar = toolBar->GetSize();
	wxSize size = event.GetSize();
	int szActorCodeSelector = (size.GetWidth() - actorCodeSelector->GetPosition().x) /2;

	toolBar->SetSize(size.GetWidth(), -1);

	script->Move(0, szToolBar.GetHeight());
	script->SetSize(size.GetWidth(), size.GetHeight() - szToolBar.GetHeight());


	actorCodeSelector->SetSize(szActorCodeSelector, -1);
	int xCodeFunctionSelector = actorCodeSelector->GetPosition().x + 2 + szActorCodeSelector;

	codeFunctionSelector->Move(xCodeFunctionSelector, -1);
	codeFunctionSelector->SetSize(size.GetWidth() - xCodeFunctionSelector, -1);
}

void PanelScript::OnSelect ( wxCommandEvent& event )
{
	gedString title, code, imgName;

	stAction *currentAction = (stAction *)event.GetClientData();
	imgName = event.GetString();

	if(currentAction == GLOBAL_CODE)
	{
		gedString globalCodeName(event.GetString());
		imgName = title = "Global Code" ;
		title += " -> ";
		title += globalCodeName;
		code = GenericScript::Get(globalCodeName);
	}
	else if(currentAction)
	{
		Script *pScript;

		if((pScript = GameControl::Get()->GetExpression(currentAction->data.name)) != NULL)
		{
			gedString name;
			code = pScript->getCode();			

			GlobalMapActionsIterator it(Action::getGlobalMapActions());

			for(it.Begin(); !it.Done(); it.Next())
			{
				if(!name.empty()) break;

				stAction *action = (*it.Value())->getActions();
				while(action)
				{
					if(action == currentAction)
					{
						imgName = name = *it.Key();
						break;
					}

					action = action->next;
				}		
			}

			title += name;
			title += " -> ";				
			title += Action::GetDescription(currentAction, false);
		}		
	}
	else
	{
		title.clear();
	}

	if(!code.empty())
	{
		wxSTEditorSplitter *editor = NULL;

		int page = script->FindEditorPageByFileName(title.c_str());

		if(page != wxNOT_FOUND)
		{
			if(script->GetSelection() != page)
			{
				script->SetSelection(page);	
				actorCodeSelector->GetTextCtrl()->SetValue(title.c_str());
			}
		}
		else
		{
			editor = script->InsertEditorSplitter(0, 0, title.c_str(), true);
			page = script->FindEditorPageByFileName(title.c_str());			
			editor->GetEditor()->SetLanguage(STE_LANG_CPP);	
			editor->GetEditor()->SetScrollWidth(80);
			editor->GetEditor()->SetText(code.c_str());
			editor->GetEditor()->SetSavePoint();
			editor->GetEditor()->EmptyUndoBuffer();
			if(PanelActors::Get()) script->SetPageImage(page, PanelActors::Get()->GetImageIndex(imgName.c_str()));

			actorCodeSelector->GetTextCtrl()->SetValue(title.c_str());
			if(script->GetOptions().HasNotebookOption(STN_ALPHABETICAL_TABS)) script->SortTabs();

			mapAction.Add(title, currentAction);
		}	
	}
}

void PanelScript::OnPageChanged(wxNotebookEvent &event)
{
	int sel = event.GetSelection(), oldSel = event.GetOldSelection();
	if(sel != event.GetOldSelection() && sel >= 0 && oldSel >= 0)
	{
		wxString title(script->GetEditor(sel)->GetFileName());
		actorCodeSelector->GetTextCtrl()->SetValue(title);

		toolBar->EnableTool(ID_APPLY_CHANGES, script->GetEditor(sel)->IsModified());
	}
}

void PanelScript::OnSTEState(wxSTEditorEvent &event)
{
	toolBar->EnableTool(ID_APPLY_CHANGES, event.GetEditor()->IsModified());
	event.Skip();
}

void PanelScript::VerifyChanges()
{
	MatstActionIterator it(mapAction);
	ListString remove;

	for(it.Begin(); !it.Done(); it.Next())
	{
		stAction *currentAction = *it.Value();
		int page = script->FindEditorPageByFileName(it.Key()->c_str());
		wxString currentCode(script->GetEditor(page)->GetValue());

		if(currentAction == GLOBAL_CODE)
		{
			gedString codeName(it.Key()->substr(it.Key()->rfind(' ') + 1));
			gedString code(GenericScript::Get(codeName));

			if(currentCode != code.c_str())
			{
				remove.PushBack(*it.Key());
			}
		}
		else if(currentAction)
		{
			//Still valid?
			GlobalMapActionsIterator itActions(Action::getGlobalMapActions());
			bool bValid = false;

			for(itActions.Begin(); !itActions.Done(); itActions.Next())
			{
				if(bValid) break;

				stAction *action = (*itActions.Value())->getActions();
				while(action)
				{
					if(action->actionType == SET_EXPRESSION && action == currentAction)
					{
						bValid = true;
						break;
					}

					action = action->next;
				}		
			}

			if(bValid)
			{
				Script *pScript;

				if((pScript = GameControl::Get()->GetExpression(currentAction->data.name)) != NULL)
				{
					gedString code(pScript->getCode());			

					if(currentCode != code.c_str())
					{
						bValid = false;
					}
				}		
			}

			if(!bValid) remove.PushBack(*it.Key());
		}	
	}

	for(int i = 0; i < remove.Count(); i++)
	{
		RemoveEditor(remove[i]);
	}
}

void PanelScript::OnRemove( wxCommandEvent& event )
{
	gedString currentName(actorCodeSelector->GetTextCtrl()->GetValue());
	stAction *currentAction = NULL, **action = mapAction[currentName];

	if(action) currentAction = *action;

	if(currentAction >= GLOBAL_CODE)
	{
		int answer = wxMessageBox(wxT("Remove this script?"), wxT("Confirm"), wxYES_NO | wxCENTRE);

		if(answer == wxYES)
		{
			if(currentAction == GLOBAL_CODE)
			{				
				gedString codeName(currentName.substr(currentName.rfind(' ') + 1));
				if(!GenericScript::RemoveAndVerify(codeName))
				{
					int answer = wxMessageBox((GenericScript::GetError() + "\n\nThere are script code errors that need to be corrected before your program will execute.\nProceed anyway?").c_str(), wxT("Error"), wxYES_NO | wxCENTRE);
					if(answer == wxYES)
					{			
						//Remove with errors and check again in game mode
						GenericScript::RemoveAndVerify(codeName, true);
						RemoveEditor(currentName);
						VerifyChanges();
					}					
				}				
			}
			else
			{
				//Still valid?
				GlobalMapActionsIterator it(Action::getGlobalMapActions());

				for(it.Begin(); !it.Done(); it.Next())
				{

					Action *pAction = *it.Value();
					stAction *action = pAction->getActions();				

					while(action)
					{
						if(action == currentAction)
						{
							if(currentAction->actionType == SET_CONDITIONAL_ACTION) pAction->DeleteConditionalAction(currentAction);
							else 
							{
								pAction->RemoveAction(currentAction);
								delete currentAction;
								RemoveEditor(currentName);
								currentName.clear();
							}

							return;
						}

						action = action->next;
					}		
				}
			}

		}
	}
}

void PanelScript::OnApplyChanges( wxCommandEvent& event )
{
	gedString currentName(actorCodeSelector->GetTextCtrl()->GetValue());
	stAction *currentAction = NULL, **action = mapAction[currentName];

	if(action) currentAction = *action;

	int page = script->GetSelection();
	wxSTEditor *editor = script->GetEditor(page);

	gedString code(editor->GetValue());

	if(code.empty())
	{
		wxMessageBox(wxT("Can't add an empty script\nPlease, enter your script or close"), wxT("Error"), wxOK | wxCENTRE);
		return;
	}

	if(currentAction >= GLOBAL_CODE)
	{
		if(currentAction == GLOBAL_CODE)
		{				
			gedString codeName(currentName.substr(currentName.rfind(' ') + 1));

			GenericScript globalCode;
			if(!globalCode.Add(code, codeName) || globalCode.GetError().length())
			{
				if(globalCode.GetError().find("Error") == gedString::npos)
				{
					//Warnings
					int answer = wxMessageBox((globalCode.GetError() + "\n\nThere is some errors but are not fatal.\nProceed anyway?").c_str(), wxT("Confirm"), wxYES_NO | wxCENTRE);
					if(answer != wxYES)
					{
						return;
					}					
					
					//Add with errors and check again in game mode
					globalCode.Add(code, codeName, true);
				}
				else
				{
					int answer = wxMessageBox((globalCode.GetError() + "\n\nThere are script code errors that need to be corrected before your program will execute.\nProceed anyway?").c_str(), wxT("Error"), wxYES_NO | wxCENTRE);
					if(answer != wxYES)
					{
						return;
					}					
					
					//Add with errors and check again in game mode
					globalCode.Add(code, codeName, true);	
				}							
			}
		}
		else
		{
			//Still valid?
			GlobalMapActionsIterator it(Action::getGlobalMapActions());

			for(it.Begin(); !it.Done(); it.Next())
			{
				Action *pAction = *it.Value();
				stAction *action = pAction->getActions();				

				while(action)
				{
					if(action == currentAction)
					{
						break;
					}

					action = action->next;
				}


				if(action == currentAction)
				{
					Script *pScript = GameControl::Get()->GetExpression(action->data.name);

					if(pScript)
					{
						if(!pScript->Parser(code, false, false, true) || pScript->GetError().length())
						{
							//Error or only warnings?

							if(pScript->GetError().find("Error") == gedString::npos)
							{
								//Warnings
								int answer = wxMessageBox((pScript->GetError() + "\n\nThere is some errors but are not fatal.\nProceed anyway?").c_str(), wxT("Confirm"), wxYES_NO | wxCENTRE);
								if(answer != wxYES)
								{
									return;
								}									
							}
							else
							{
								//Errors
								int answer = wxMessageBox((pScript->GetError() + "\n\nThere are script code errors that need to be corrected before your program will execute.\nProceed anyway?").c_str(), wxT("Error"), wxYES_NO | wxCENTRE);
								if(answer != wxYES)
								{
									return;
								}

								//Add with errors and check again in game mode
								pScript->setAddedWithError(true);
							}				
						}
						else
						{
							pScript->setAddedWithError(false);
						}
					}
				}
			}		
		}
	}

	//Update the UI
	editor->SetSavePoint();
	toolBar->EnableTool(ID_APPLY_CHANGES, editor->IsModified());	
}

void UpdatePanelScript()
{
	PanelScript::Update();
}

void PanelScript::Update()
{
	if(!panelScript) return;

	//Populate global code
	ListString globalCode;

	GenericScript::Populate(globalCode);

	//Populate scripts
	ListData list;
	Action::PopulateScriptActions(list);

	int total = list.Count() + globalCode.Count();
	int n = (total > 0)?total:1;

	wxString *strings = new wxString[n]; 
	if(!strings) return;

	void **data = (void **)malloc(n*sizeof(void *));
	if(!data) 
	{
		delete [] strings;
		return;
	}

	int i=0;

	if(globalCode.Count())
	{
		for(; i < globalCode.Count(); i++)
		{			
			strings[i] = globalCode[i].c_str();
			data[i] = (void *)GLOBAL_CODE;
		}
	}

	if(list.Count())
	{
		int i1 = 0;
		for(; i < list.Count(); i++, i1++)
		{			
			strings[i] = list[i1].text.c_str();
			data[i] = list[i1].data;
		}
	}
	
	if(!i)
	{	
		strings[0] = NO_SELECTION;
		data[0] = 0;
	}

	//Set property	
	panelScript->actorCodeSelector->Clear();
	panelScript->actorCodeSelector->GetPopupInterface()->Populate(n, strings, data); 

	delete [] strings;
	free(data);

	if(PanelActors::Get())
	{
		panelScript->script->SetImageList(PanelActors::Get()->GetImageList());
	}

	panelScript->VerifyChanges();
}