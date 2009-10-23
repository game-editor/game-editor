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


// EditBox.h: interface for the EditBox class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_EDITBOX_H__65916826_5DF6_4BD2_803F_830025A675AF__INCLUDED_)
#define AFX_EDITBOX_H__65916826_5DF6_4BD2_803F_830025A675AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "engine/kyra.h"
#include "Actor.h"
#include "ListPop.h"


#define LINE_CHANGED 101

class EditBox;
class ScrollBar;

class NumericSlide : public Actor
{
public:
	NumericSlide(Actor *parent, EditBox *_edit, double _min, double _max, double _step);
	virtual ~NumericSlide() {};

	bool OnMouseButtonDown(int x, int y, Uint8 button);
	void OnMouseButtonUp(int x, int y, Uint8 button);
	void OnMouseMove(int x, int y);

	void SetLimits(double _min, double _max, double _step);
	
	void setActorTip(const gedString& _tip) {actorTip = _tip;};
	const gedString *OnActorTip() {return &actorTip;}

private:
	gedString			actorTip;
	int xIni;
	double iniValue;
	bool bSlide;

	double minValue, maxValue, step;

	EditBox *edit;
};

class VariableSelector : public Actor
{
public:
	VariableSelector(Actor *parent, EditBox *_edit, bool _bPointer);
	virtual ~VariableSelector() {delete icon;};


	void SetList(ListData &list);
	
	void setActorTip(const gedString& _tip) {actorTip = _tip;};
	const gedString *OnActorTip() {return &actorTip;}

	bool OnList(ListPop *list, int index, gedString &text, int listId);
	bool OnMouseButtonDown(int x, int y, Uint8 button);
	bool OnListButtonDown(ListPop *list, int listId);

	EditBox *getEdit() {return edit;}
	ListPop *getList() {return listPop;}

private:

	gedString			actorTip;
	EditBox *edit;
	ListPop *listPop;
	bool bPointer;

	Actor *icon;
};

class EditBox  
{
public:
	void OnLineChanged();
	void OnEditSelection();
	void Disable() {text->Disable();}
	void Enable() {text->AddListener(text);}
	void ShowScrollBar();
	void UpdateScrollInfo();

	bool SetFocus();
	int Left();
	int Width();
	int Top();
	int Right();
	int Down();
	int Height();
	int X();
	int Y();

	EditBox(int x = 0, int y = 0, int width = 100, int height = 0, bool multiLine = false, Actor *parent = NULL, gedString fontName = "maksfont.bmp");
	virtual ~EditBox();
	
	void OnEnter(int line);
	void OnChar(SDLKey key, int line);
	gedString GetText(int line = 0, bool bTrim = true);
	gedString GetTextUntilCursor();
	void SetText(gedString str, int line = 0);
	void SetText(int n, int line = 0);
	void SetText(double n, int line = 0);
	void InsertText(const gedString& str);
	int NumLines() {return text->NumLines();}
	int ActualLine() {return text->ActualLine();}
	void SetNumeric(double _min, double _max, double _step);
	void SetVariableList(ListData &list, bool _bPointer);
	void SetSyntaxColorize(bool value) {text->SetSyntaxColorize(value);}

	void ScrollTo(int lineIndex);
	int ScrollUp();
	int ScrollDown();
	int ScrollPageUp();
	int ScrollPageDown();

	void StartAction();
	void EndAction();

	Actor *getParentActor() {return parentActor;}
	
	class EditTextWidget : public KrTextWidget						
	{
	public:
		EditTextWidget(	int width, int height, 
			bool drawBorder,
			bool drawBackground,
			bool editable,
			const KrScheme& scheme,
			EditBox *parent,
			bool multiLine
			)
			: KrTextWidget(	width,  height, 
			drawBorder,
			drawBackground,
			editable,
			scheme,
			multiLine)
		{
			this->parent = parent;
		}
		
		bool HandleWidgetEvent(	KrWidget* source, 
			U32 event, 
			U32 data, 
			const SDL_Event* sdlEvent,
			const char* command, 
			const char* arg );
		
		EditBox *parent;
	};
	
	private:
		EditTextWidget *text;
		KrFontResource *fontResource;
		Actor *parentActor;
		NumericSlide *numericSlide;
		ScrollBar *scrollBar;	
		VariableSelector *variableSelector;
		
};

#endif // !defined(AFX_EDITBOX_H__65916826_5DF6_4BD2_803F_830025A675AF__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME