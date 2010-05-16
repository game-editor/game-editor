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


// Config.h: interface for the Config class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_CONFIG_H__E3E3A957_67AC_4152_8C11_9949BF7D2EE5__INCLUDED_)
#define AFX_CONFIG_H__E3E3A957_67AC_4152_8C11_9949BF7D2EE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "gedString.h"

class RecentFileList
{
// Constructors
public:

	RecentFileList(int _maxListSize = 10);

	int GetSize() const { return array.Count(); }
	gedString GetDisplayName(int nIndex);
	gedString GetFullPath(int nIndex);


	void Add(gedString filePath);
	void Remove(gedString filePath);

	void ReadList();    // reads from registry or ini file
	void WriteList();   // writes to registry or ini file


	virtual ~RecentFileList();

protected:
	int maxListSize;                // contents of the MRU list
	ListString array;
};


class Config  
{
public:
	Config();
	virtual ~Config();

	static Config *Get();
	static void Destroy();

	void Save();

	bool getEnableToolTips() {return bEnableToolTips;}
	bool getShowTutorialReminder() {return bShowTutorialReminder;}	
	bool getShowActorPropertyReminder() {return bShowActorPropertyReminder;}
	bool getShowIPhoneImageSizeReminder() {return bShowIPhoneImageSizeReminder;}
	gedString getEmergencySave() {return emergencySave;}
	gedString getRecentFileList() {return recentFileList;}

	gedString getPathImageFont() {return PathImageFont;}
	gedString getPathAnimations() {return PathAnimations;}
	gedString getPathGame() {return PathGame;}
	gedString getPathExport() {return PathExport;}
	gedString getPathSounds() {return PathSounds;}
	gedString getImageEditor() {return ImageEditor;}

	int getMaxUndoLevels() {return maxUndoLevels;}
	bool getPersistentUndo() {return bPersistentUndo;}

	void setEnableToolTips(bool value) {bEnableToolTips = value;}
	void setShowTutorialReminder(bool value) {bShowTutorialReminder = value;}	
	void setShowActorPropertyReminder(bool value) {bShowActorPropertyReminder = value;}
	void setShowIPhoneImageSizeReminder(bool value) {bShowIPhoneImageSizeReminder = value;}
	void setEmergencySave(gedString value) {emergencySave = value;}
	void setRecentFileList(gedString value) {recentFileList = value;}

	void setPathImageFont(gedString value) {PathImageFont = value;}
	void setPathAnimations(gedString value) {PathAnimations = value;}
	void setPathGame(gedString value) {PathGame = value;}
	void setPathExport(gedString value) {PathExport = value;}
	void setPathSounds(gedString value) {PathSounds = value;}
	void setImageEditor(gedString value) {ImageEditor = value;}

	void setMaxUndoLevels(int value) {maxUndoLevels = value;}
	void setPersistentUndo(bool value) {bPersistentUndo = value;}
	

private:
	void UpdateUninstallData();

	static Config *config;

	bool bEnableToolTips;
	bool bShowTutorialReminder;
	bool bShowActorPropertyReminder;
	bool bShowIPhoneImageSizeReminder;
	gedString emergencySave;
	gedString recentFileList;

	gedString PathImageFont;
	gedString PathAnimations;
	gedString PathGame;
	gedString PathExport;
	gedString PathSounds;
	gedString ImageEditor;

	gedString UninstallData;

	int maxUndoLevels;
	bool bPersistentUndo;
};

#endif // !defined(AFX_CONFIG_H__E3E3A957_67AC_4152_8C11_9949BF7D2EE5__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME
