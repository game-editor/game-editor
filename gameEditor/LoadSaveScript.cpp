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


// LoadSaveScript.cpp: implementation of the LoadSaveScript class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "LoadSaveScript.h"
#include "GameControl.h"
#include "Tutorial.h"

char scriptPath[PATH_LIMIT];
static bool bInitPath = false;

LoadSaveScript::LoadSaveScript(int command, ExpressionEditor *scriptEditor, ScriptGlobals *globalsEditor, ActorText *actorText)
{
	this->scriptEditor = scriptEditor;
	this->globalsEditor = globalsEditor;
	this->actorText = actorText;

	//Title
	gedString title;
	if(command == LOAD_DIALOG) 
	{
		if(actorText)
			title = "Load Text";
		else
			title = "Load Script";
	}
	else
	{
		if(actorText)
			title = "Save Text";
		else
			title = "Save Script";
	}


	//Button
	gedString buttontext = "Save";
	if(command == LOAD_DIALOG) buttontext = "Load";


	//Init
	chdir(GameControl::Get()->getGamePath().c_str());

	Init(command, title, buttontext);

	AddFilter("c");
	AddFilter("cpp");
	AddFilter("txt");

	if(!Tutorial::IsOff())
	{
		chdir(Tutorial::Get()->getTutorialPath());
	}
	else
	{
		if(!bInitPath)
		{
			scriptPath[0] = 0;
			bInitPath = true;
		}
		else chdir(scriptPath);
	}
}

LoadSaveScript::~LoadSaveScript()
{

}

void LoadSaveScript::ResetPath()
{
	bInitPath = false;
}

void LoadSaveScript::OnButton(Button *button, int buttonId)
{
	LoadSave::OnButton(button, buttonId);
	if(!IsOk()) return;

	switch(buttonId)
	{
	case LOADSAVE_OK:
		{
			if(command == SAVE_DIALOG)
			{
				gedString file = GetText();				
				
				char *buf = (char *)file.c_str();
				int iExt = strlen(buf) - 2;
				if(!actorText && (iExt < 0 || strcmp(&buf[iExt], ".c") != 0))
				{
					file += ".c";
				}

				SDL_RWops *src = ged_SDL_RWFromFile(file.c_str(), "w");
				if(!src)
				{
					char error[256];
					sprintf(error, "File save error: %s", SDL_GetError());

					new PanelInfo(error);
					return;
				}

				if(globalsEditor)
				{
					int lenght = globalsEditor->GetText().length();
					SDL_RWwrite(src, globalsEditor->GetText().c_str(), lenght, 1);
				}
				else if(scriptEditor)
				{
					int lenght = scriptEditor->GetText().length();
					SDL_RWwrite(src, scriptEditor->GetText().c_str(), lenght, 1);
				}
				else if(actorText)
				{
					int lenght = actorText->GetText().length();
					SDL_RWwrite(src, actorText->GetText().c_str(), lenght, 1);
				}

				SDL_RWclose(src);
			}			
			else 
			{
				SDL_RWops *src = ged_SDL_RWFromFile(GetDirectory().c_str(), "rb");
				if(!src)
				{
					char error[256];
					sprintf(error, "File load error: %s", SDL_GetError());

					new PanelInfo(error);
					return;
				}

				SDL_RWseek(src, 0, SEEK_END);
				int length = SDL_RWtell(src);
				SDL_RWseek(src, 0, SEEK_SET);

				char *code = new char[length+1];

				SDL_RWread(src, code, length, 1);
				code[length] = 0;

				if(globalsEditor) 
					globalsEditor->SetText(code);
				else if(scriptEditor)
					scriptEditor->SetText(code);
				else if(actorText)
					actorText->SetText(code);

				delete [] code;
				SDL_RWclose(src);			
			}

			//Store current path
			ged_getcwd(scriptPath, PATH_LIMIT);
			delete this;
		}	
		break;
	case LOADSAVE_CLOSE:
		{
			delete this;
		}
		break;
	}
}


#endif //#ifndef STAND_ALONE_GAME