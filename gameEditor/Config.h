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
