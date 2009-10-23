/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com, http://game-editor.com

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
Be a Game Editor developer: Be a Game Editor developer: <http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>  .

***************************************************************************/


// ChangeCursor.cpp: implementation of the ChangeCursor class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "ChangeCursor.h"
#include "GameControl.h"
#include "ExpressionEditor.h"
#include "Tutorial.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_CHANGECURSOR			"\
Use this action to set a mouse cursor for selected actor\n\
\n\
If the image file has a color depth lower than 32 bits,\n\
Game Editor understands that the color of the first pixel\n\
(upper left corner) is the color that should become totally\n\
transparent in the image. If your image has 32 bits color depth,\n\
Game Editor will consider the alpha channel (transparency channel)\n\
of the image.\n\
\n\
If you want to set up an animated cursor,\n\
just tell how many horizontal and vertical frames are in your animation.\n\
\n\
The horspot is the point on your cursor centered on the mouse"

#define TIP_CHANGECURSOR_ACTOR			"\
Select the actor that will have this mouse cursor\n"

#define TIP_CHANGECURSOR_FILE			"\
Select the image file\n\
Supports tga, jpeg, bmp, png, xpm, xcf, pcx, lbm, gif and tif formats"

//////////////////////////////////////////////////////////////////////////////

enum
{
	BT_ADD,
	BT_CLOSE,
	LS_ACTOR
};

#define WIDTH	210
#define HEIGHT	185


ChangeCursor::ChangeCursor(Actor *actor)
: BaseAction("Change Cursor", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_CHANGECURSOR);
	eventActor = actionActor = actor;

	Text *text;
	Button *button;
	int y;

	//Title
	text = AddText(CURSOR_POINTER, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("Actor:", 10, y);
	listActor = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ACTOR); listActor->SetToolTip(gedString(TIP_CHANGECURSOR_ACTOR) + TIP_ACTION_ACTOR);

	text = AddText("File: ", 10, listActor->Down() + 2);
	path = AddListDirectory(text->Right(), text->Top()); path->SetToolTip(TIP_CHANGECURSOR_FILE);

	text = AddText("Horizontal Frames: ", 10, path->Down() + 5);
	hframes = AddEditBox(text->Right(), text->Top(), 40); hframes->SetNumeric(1, 16000, 1);
	hframes->SetText("1");

	text = AddText("  Vertical Frames: ", 10, hframes->Down() + 2);
	vframes = AddEditBox(text->Right(), text->Top(), 40); vframes->SetNumeric(1, 16000, 1);
	vframes->SetText("1");

	text = AddText("        HotSpot X: ", 10, vframes->Down() + 5);
	hotx = AddEditBox(text->Right(), text->Top(), 40); hotx->SetNumeric(-512, 512, 1);
	hotx->SetText("0");

	text = AddText("        HotSpot Y: ", 10, hotx->Down() + 2);
	hoty = AddEditBox(text->Right(), text->Top(), 40); hoty->SetNumeric(-512, 512, 1);
	hoty->SetText("0");

	
	//Close
	y = DrawHLine(hoty->Down() + 2);
	button = AddButton(Action::getEditAction()?"Ok":"Add", 40, y, 0, 0, BT_ADD); button->SetToolTip(TIP_ACTION_ADD); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right()+8, y, 0, 0, BT_CLOSE); button->SetToolTip(TIP_ACTION_CANCEL); SetCancelButton(button);	

	GameControl::Get()->PopulateActors(listActor);
	listActor->AddText(S_EVENT_ACTOR);

	if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		listActor->AddText(S_PARENT_ACTOR);
		listActor->AddText(S_CREATOR_ACTOR);
	}

	listActor->SetItem(S_EVENT_ACTOR);

	path->AddFilter("tga");
	path->AddFilter("bmp");
	path->AddFilter("pnm");
	path->AddFilter("xpm");
	path->AddFilter("xcf");
	path->AddFilter("pcx");
	path->AddFilter("gif");
	path->AddFilter("jpg");
	path->AddFilter("jpeg");
	path->AddFilter("jpe");
	path->AddFilter("jfif");
	path->AddFilter("tif");
	path->AddFilter("tiff");
	path->AddFilter("iff");
	path->AddFilter("lbm");
	path->AddFilter("png");

	if(!Tutorial::IsOff())
	{
		chdir(Tutorial::Get()->getTutorialPath());
	}


	if(!ExpressionEditor::getExpressionEditor()) UpdateEdition();
}

void ChangeCursor::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	listActor->SetItem(editAction->actorName);
	hframes->SetText(editAction->data.cursor.nFramesH);
	vframes->SetText(editAction->data.cursor.nFramesV);
	hotx->SetText(editAction->data.cursor.hotSpotX);
	hoty->SetText(editAction->data.cursor.hotSpotY);
	
	//Update file and directory
	gedString file, dir;
	SplitPath(editAction->data.cursor.imgName, file, dir);
	path->AddText(file);
	path->SetItem(file);
	chdir(dir.c_str());
}

ChangeCursor::~ChangeCursor()
{

}

bool ChangeCursor::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(BaseAction::OnList(list, index, text, listId)) return true;

	switch(listId)
	{
	case LS_ACTOR:
		{
			actionActor = GameControl::Get()->GetActor(text);
			if(actionActor == EVENT_ACTOR)
			{
				actionActor = eventActor;
			}
		}
		break;
	}

	return true;
}

void ChangeCursor::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_ADD:
		{
			gedString pathArq = path->GetFullPath();
			
			if(listActor->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select an actor");
				return;
			}

			int nh, nv, hx, hy;
			SDL_RWops *src = ged_SDL_RWFromFile(pathArq.c_str(), "r");
			if(src)
			{
				//Ok
				SDL_RWclose(src);
				
				SDL_Surface* surface = ged_IMG_Load(pathArq.c_str());
				if(!surface)
				{
					new PanelInfo("This format is not supported or file is corrupted\nPlease, select other file");
					return;
				}
				
				//Ok
				SDL_FreeSurface( surface );
				
				nh = atol(hframes->GetText().c_str());
				nv = atol(vframes->GetText().c_str());
				hx = atol(hotx->GetText().c_str());
				hy = atol(hoty->GetText().c_str());
				
				if(nh < 1 || nh > 16000 || nv < 1 || nv > 16000)
				{
					new PanelInfo("Please, enter valid frame numbers");
					return;
				}

				if(hx < -512 || hx > 512 || hy < -512 || hy > 512)
				{
					new PanelInfo("Please, enter valid hot spot coordinates");
					return;
				}
			}
			else
			{
				//Erro	
				new PanelInfo("Please, select a file");
				return;
			}
			
			if(IsEditingScript())
			{
				//Auto complete script
				const char *pathOri = pathArq.c_str();
				gedString pathStr;
				for(int i = 0; i < strlen(pathOri); i++)
				{
					pathStr += pathOri[i];
					if(pathOri[i] == '\\') pathStr += "\\";
				}
				

				char buf[256];
				sprintf(buf, "ChangeCursor(\"%s\", \"%s\", %ld, %ld, %ld, %ld);",
					listActor->GetText().c_str(),
					pathStr.c_str(),
					nh, nv, hx, hy);
				InsertIntoScript(buf);
				delete this;
			}
			else
			{
				SelectActionType(eventActor, Action::Call(eventActor)->SetCursor(listActor->GetText(), pathArq, nh, nv, hx, hy));
			}			
		}
		break;

	case BT_CLOSE:
		{
			delete this;
		}
		break;
	}
}

#endif //#ifndef STAND_ALONE_GAME