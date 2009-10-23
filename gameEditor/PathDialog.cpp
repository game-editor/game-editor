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


// PathDialog.cpp: implementation of the PathDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "PathDialog.h"
#include "GameControl.h"
#include "gui.h"
#include "AddPathDialog.h"
#include "PathPoint.h"
#include "Path.h"
#include "ActorProperty.h"


//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_PATH			"\
Use this panel to add or edit the actor paths\n\
The paths are relatives to the current actor position"

#define TIP_PATH_EDIT		"\
Select a path to edit\n\
Right click on path points to\n\
   Set the node type (Linear or Curved)\n\
   Remove the node"

#define TIP_PATH_ADD		"Create a new path"
//////////////////////////////////////////////////////////////////////////////


#define WIDTH	310
#define HEIGHT	85

PathDialog *PathDialog::pathDialog = NULL;
Path *PathDialog::actualPath = NULL;

PathDialog::PathDialog()
	: Panel("PathDialog", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	Text *text;
	Button *button;	
	int y;

	SetToolTip(TIP_PATH);

	//Title
	text = AddText("Path Control", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("Path: ", 10, y);
	listPath = AddListPop(text->Right() + 2, text->Top()); listPath->SetToolTip(TIP_PATH_EDIT);
	button = AddButton("Add", listPath->Right()+2, listPath->Top(), 0, 0, BT_ADDPATH); button->SetToolTip(TIP_PATH_ADD);
	button = AddButton("Remove", button->Right(), button->Top(), 0, 0, BT_REMPATH);
	
	//Close
	y = DrawHLine(listPath->Down() + 2);
	button = AddButton("Close", (WIDTH - 60)/2, y, 0, 0, BT_CLOSE);

	SetPaths();
	pathDraw = NULL;
	actualPath = NULL;
}

PathDialog::~PathDialog()
{
	pathDialog = NULL;
	actualPath = NULL;
	GameControl::Get()->GetAxis()->getImage()->Invalidate();

	if(pathDraw)
		delete pathDraw->getRootPath();
}

void PathDialog::Call()
{
	if(!pathDialog)
	{
		pathDialog = new PathDialog();
	}
}

void PathDialog::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_CLOSE:
		{
			pathDialog = NULL;
			delete this;
		}
		break;
	case BT_ADDPATH:
		{
			AddPathDialog::Call();			
		}
		break;
	case BT_REMPATH:
		{
			gedString pathName = listPath->GetText();
			if(listPath->RemoveText(pathName))
			{
				listPath->SetItem(-1);				
				GameControl::Get()->RemovePath(pathName);
				
				if(pathDraw)
				{
					delete pathDraw->getRootPath();					
					pathDraw = NULL;
					actualPath = NULL;
					GameControl::Get()->GetAxis()->getImage()->Invalidate();
				}

				//Notify actor dialog
				ActorProperty::SetPaths();
			}
		}
		break;
	}
}

void PathDialog::SetPaths()
{
	GameControl::Get()->PopulatePaths(listPath, false);
}

bool PathDialog::OnList(ListPop *list, int index, gedString &text, int listId)
{
	Path *path = GameControl::Get()->GetPath(text);

	//Delete anterior path draw
	if(pathDraw)
	{
		delete pathDraw->getRootPath();
		pathDraw = NULL;
		actualPath = NULL;
	}

	//Draw path
	if(path)
	{
		int x = path->getXOri(), y = path->getYOri();
		pathDraw = new PathPoint(x, y, NULL, true, path, 0);

		for(int i = 1; i < path->GetTotalKeys(); i++)
		{
			path->GetXY(x, y, path->GetFrame(i));
			pathDraw = pathDraw->AddPoint(x, y, true, path, i);
		}

		pathDraw = pathDraw->getRootPath();
		actualPath = path;		
	}	

	GameControl::Get()->GetAxis()->getImage()->Invalidate();

	return true;
}

PathPoint *PathDialog::getPathPoint()
{
	return pathDraw?pathDraw->getRootPath():NULL;
}


#endif //#ifndef STAND_ALONE_GAME