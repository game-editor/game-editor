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


// ActorText.h: interface for the ActorText class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_ACTORTEXT_H__B29613B2_6AA4_4E17_9126_307C246DD2AD__INCLUDED_)
#define AFX_ACTORTEXT_H__B29613B2_6AA4_4E17_9126_307C246DD2AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseAction.h"
#include "../SDL/SDL_ttf/SDL_ttf.h"
#include "ColorPicker.h"

gedString AddTrueTypeFont(const char* str_ttf, int i_style, int i_size, Uint8 ui_r, Uint8 ui_g, Uint8 ui_b, bool bAntiAlias);

class ActorText : public BaseAction  
{
public:
	void AddFontToList(gedString path);
	void PopulateFonts();
	void SetText(char *str);
	gedString GetText();

	void UpdateEdition();
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	ActorText(Actor *actor, bool bOnCollision = false, bool bAction = false);
	virtual ~ActorText();

	static void AddFontToCache(const gedString& font, int _ic, int _nc);
	static void DestroyTextCache() {delete textCache; textCache = NULL;}
	
	bool IsEditable() {return bInput;}
	bool HasBorder() {return bDrawBorder;}
	bool HasBackground() {return bDrawBackground;}
	bool HasGetFocus() {return bGetFocus;}

	KrRGBA getBorderColor() {return borderColor;}
	KrRGBA getCursorColor() {return cursorColor;}
	KrRGBA getBackGroundColor() {return backGroundColor;}

	void setBorder(bool value) {bDrawBorder = value;}
	void setBackground(bool value) {bDrawBackground = value;}
	void setGetFocus(bool value) {bGetFocus = value;}

	void setBorderColor(KrRGBA color) {borderColor = color;}
	void setCursorColor(KrRGBA color) {cursorColor = color;}
	void setBackGroundColor(KrRGBA color) {backGroundColor = color;}

private:
	Actor *eventActor;
	
	ListPop *listActor, *listFile, *listFont, *listNewFont, *listEnableInput;
	Button *buttonInputConfig;
	EditBox *actorText;

	gedString pathArq;
	int ic, nc;

	bool bEdit;

	static Actor *textCache;
	static bool bActorTextOpen;

	bool bInput, bDrawBorder, bDrawBackground, bGetFocus;
	KrRGBA borderColor, cursorColor, backGroundColor;
};

class ActorTextBitmap : public Panel  
{
public:
	void UpdateSample(bool bChangeFile);

	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnEditChar(EditBox *edit, SDLKey key, int line);
	
	ActorTextBitmap();
	virtual ~ActorTextBitmap();

	static void ResetPath();

	int getInicialChar() {return ic;}
	int getNumberOfChars() {return nc;}
	gedString getPath();

	

private:
	ListDirectory *path;
	EditBox *iniChar, *nChars, *charSample;

	int ic, nc;

	Actor *animationSample;
};

class ActorTextTrueType : public Panel  
{
public:
	void OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message);
	void OnEditChar(EditBox *edit, SDLKey key, int line);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnButton(Button *button, int buttonId);
	

	void UpdateSample();
	
	ActorTextTrueType();
	virtual ~ActorTextTrueType();

	static void ResetPath();

	gedString getPath() {return pathArq;}

	

private:
	ListDirectory *path;
	ListPop *listStyle, *listSoft;
	EditBox *editSize;
	Text *textBox;

	ColorSample *colorSample;

	gedString pathArq;
	int style, size;
	bool bSoft;
};

class ActorTextInput : public Panel  
{
public:

	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnButton(Button *button, int buttonId);
	
	ActorTextInput(ActorText *_actorText);
	virtual ~ActorTextInput();

	

private:

	ListPop *listDrawBorder, *listDrawBackground, *listGetFocus;
	ColorSample *borderColorSample, *cursorColorSample, *backgroundColorSample;

	ActorText *actorText;
};

#endif // !defined(AFX_ACTORTEXT_H__B29613B2_6AA4_4E17_9126_307C246DD2AD__INCLUDED_)

#endif //STAND_ALONE_GAME
