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


// GridSettings.cpp: implementation of the GridSettings class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "GridSettings.h"
#include "GameControl.h"
#include "Config.h"



enum
{
	LS_SHOW,
	LS_SNAP,
	LS_SNAP_STATE,
	LS_TOOLTIP,
	LS_PERSISTENT_UNDO,
	BT_CLOSE,
	BT_ADD
};

#define WIDTH_GRID	300
#define HEIGHT_GRID	284

static int getHeight()
{
#ifdef WIN32
	return HEIGHT_GRID + 40;
#endif

	return HEIGHT_GRID;
}

Preferences::Preferences()
	: Panel("Preferences", (GameControl::Get()->Width() - WIDTH_GRID)/2, 
				          (GameControl::Get()->Height() - getHeight())/2,
						  WIDTH_GRID, getHeight())
{
	SetModal();

	Text *text;
	Button *button;	
	int y;

	imageEditorPath = NULL;

	//Title	
	text = AddText("Preferences", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 4);

	//Body
	text = AddText(" Show grid: ", 10, y + 3);
	listShow = AddListPop(text->Right(), text->Top() - 2, 32, 0, LS_SHOW); listShow->SetToolTip(TIP_GRID_SHOW);

	text = AddText("Grid snap: ", listShow->Right() + 17, listShow->Top() + 2);
	listSnap = AddListPop(text->Right(), text->Top() - 2, 32, 0, LS_SNAP); listSnap->SetToolTip(TIP_GRID_SNAP);

	text = AddText("Main color: ", 10, text->Down() + 10);
	mainColor = new ColorSample(this, text->Right(), text->Top(), 32); mainColor->SetToolTip(TIP_GRID_MAINCOLOR);

	text = AddText("Resolution: ", mainColor->Right() + 10, text->Top());
	resolutionColor = new ColorSample(this, text->Right(), text->Top(), 32); resolutionColor->SetToolTip(TIP_GRID_RESOLUTIONCOLOR);

	text = AddText(" Grid size: ", 10, text->Down() + 10);
	editX = AddEditBox(text->Right(), text->Top(), 30); editX->SetNumeric(2, 200, 1);
	text = AddText("x", editX->Right()+3, editX->Top());
	editY = AddEditBox(text->Right() + 4, text->Top(), 30); editY->SetNumeric(2, 200, 1);

	y = DrawHLine(text->Down() + 10);

	text = AddText("Enable tool tips: ", 10, y + 2);
	listToolTip = AddListPop(text->Right(), text->Top() - 2, 32, 0, LS_TOOLTIP); listToolTip->SetToolTip(TIP_PREFERENCES_TOOLTIP);

	y = DrawHLine(text->Down() + 10);

	text = AddText("    Persistent undo: ", 10, y + 2);
	listPersistentUndo = AddListPop(text->Right(), text->Top() - 2, 32, 0, LS_PERSISTENT_UNDO); listPersistentUndo->SetToolTip(TIP_PREFERENCES_PERSISTENT_UNDO);

	text = AddText("Maximum undo levels:", 10, text->Down() + 10);
	editMaxUndoLevels = AddEditBox(text->Right() + 6, text->Top(), 30); editMaxUndoLevels->SetNumeric(3, 10000, 1);

	y = DrawHLine(text->Down() + 10);

	text = AddText("Editor Size X:", 10, y + 10);
	editEditorSizeX = AddEditBox(text->Right() + 6, text->Top(), 30); editEditorSizeX->SetNumeric(640, 10000, 1);

	text = AddText("Editor Size Y:", 10, text->Down() + 10);
	editEditorSizeY = AddEditBox(text->Right() + 6, text->Top(), 30); editEditorSizeY->SetNumeric(480, 10000, 1);


#ifdef WIN32
	y = DrawHLine(editEditorSizeY->Down() + 10);

	text = AddText("Image Editor: ", 10, y + 2);
	imageEditorPath = AddListDirectory(text->Right(), text->Top() - 2, 200); imageEditorPath->SetToolTip(TIP_PREFERENCES_IMAGE_EDITOR);	
	imageEditorPath->AddFilter("exe");
#endif
	
	
	//Close
	y = DrawHLine(text->Down() + 10);
	button = AddButton("Ok", (WIDTH_GRID - 135)/2, y, 0, 0, BT_ADD); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right()+8, y, 0, 0, BT_CLOSE); SetCancelButton(button);


	listShow->AddText("Yes");
	listShow->AddText("No");

	listSnap->AddText("Yes");
	listSnap->AddText("No");

	listToolTip->AddText("Yes");
	listToolTip->AddText("No");

	listPersistentUndo->AddText("Yes");
	listPersistentUndo->AddText("No");

	if(GameControl::Get()->GetAxis()->getGridShow())
	{
		listShow->SetItem("Yes");
	}
	else
	{
		listShow->SetItem("No");
	}

	if(GameControl::Get()->GetAxis()->getGridSnap())
	{
		listSnap->SetItem("Yes");
	}
	else
	{
		listSnap->SetItem("No");
	}

	if(Config::Get()->getEnableToolTips())
	{
		listToolTip->SetItem("Yes");
	}
	else
	{
		listToolTip->SetItem("No");
	}

	if(Config::Get()->getPersistentUndo())
	{
		listPersistentUndo->SetItem("Yes");
	}
	else
	{
		listPersistentUndo->SetItem("No");
	}

	if(imageEditorPath)
	{
		if(!Config::Get()->getImageEditor().empty())
		{
			chdir(Config::Get()->getImageEditor().GetFilePath().c_str());
			imageEditorPath->AddText(Config::Get()->getImageEditor().GetFileName());
			imageEditorPath->SetItem(Config::Get()->getImageEditor().GetFileName());
		}
	}

	editX->SetText(GameControl::Get()->GetAxis()->getGridX());
	editY->SetText(GameControl::Get()->GetAxis()->getGridY());

	editMaxUndoLevels->SetText(Config::Get()->getMaxUndoLevels());

	editEditorSizeX->SetText(Config::Get()->getEditorSizeX());
	editEditorSizeY->SetText(Config::Get()->getEditorSizeY());

	mainColor->setColor(GameControl::Get()->GetAxis()->getMainColor());
	resolutionColor->setColor(GameControl::Get()->GetAxis()->getResolutionColor());
}

Preferences::~Preferences()
{
	delete mainColor;
	delete resolutionColor;
}

bool Preferences::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(list == imageEditorPath)
	{
		if(imageEditorPath->GetText().find_no_case("mspaint.exe") != gedString::npos)
		{
			new PanelInfo("Some Microsoft Paint versions can load PNG files,\nbut can't save correct transparency information.\nSo, your image after edition will look solid,\nwithout transparent areas.\n\nIf you don't have other image editor, use a search engine and\nput the query \"Image Editors with PNG Support\" to find\na suitable image editor.",
				"Info", ALIGN_LEFT);
		}
	}

	return true;
}

void Preferences::OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message)
{

}

void Preferences::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_CLOSE:
		{
			delete this;
		}
		break;
	case BT_ADD:
		{	
			int x = atol(editX->GetText().c_str());
			int y = atol(editY->GetText().c_str());
			int maxUndoLevels = atol(editMaxUndoLevels->GetText().c_str());
			int editorSizeX = atol(editEditorSizeX->GetText().c_str());
			int editorSizeY = atol(editEditorSizeY->GetText().c_str());

			if(x < 2 || x > 200 || y < 2 || y > 200)
			{
				new PanelInfo("Grid size is invalid");
				return;
			}

			if(maxUndoLevels < 3 || maxUndoLevels > 10000)
			{
				new PanelInfo("Invalid maximum undo levels");
				return;
			}

			if(editorSizeX<640 || editorSizeY<480)
			{
			  new PanelInfo("Invalid editor dimensions (minimum 640x480)");
			}


			if(listShow->GetText() == "Yes") GameControl::Get()->GetAxis()->setGridShow(true);
			else GameControl::Get()->GetAxis()->setGridShow(false);

			if(listSnap->GetText() == "Yes") GameControl::Get()->GetAxis()->setGridSnap(true);
			else GameControl::Get()->GetAxis()->setGridSnap(false);

			if(listToolTip->GetText() == "Yes") Config::Get()->setEnableToolTips(true);
			else Config::Get()->setEnableToolTips(false);

			if(listPersistentUndo->GetText() == "Yes") Config::Get()->setPersistentUndo(true);
			else Config::Get()->setPersistentUndo(false);

			GameControl::Get()->GetAxis()->setGridX(x);
			GameControl::Get()->GetAxis()->setGridY(y);

			Config::Get()->setMaxUndoLevels(maxUndoLevels);

			Config::Get()->setEditorSizeX(editorSizeX);
			Config::Get()->setEditorSizeY(editorSizeY);

			GameControl::Get()->GetAxis()->setMainColor(mainColor->getColor());
			GameControl::Get()->GetAxis()->setResolutionColor(resolutionColor->getColor());

			GameControl::Get()->GetAxis()->getImage()->Invalidate();

			if(imageEditorPath)
			{
				Config::Get()->setImageEditor(imageEditorPath->GetFullPath());
			}

			Config::Destroy();

			delete this;
		}
		break;
	}
}








#endif //#ifndef STAND_ALONE_GAME
