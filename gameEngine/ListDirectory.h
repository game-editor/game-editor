// ListDirectory.h: interface for the ListDirectory class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_LISTDIRECTORY_H__F7B6D562_1523_4A86_86A9_7D9C847DE250__INCLUDED_)
#define AFX_LISTDIRECTORY_H__F7B6D562_1523_4A86_86A9_7D9C847DE250__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ListPop.h"
#include "system.h"


#ifdef WIN32
#define USE_SYSTEM_FILE_DIALOG
#endif


class ListDirectory : public ListPop  
{
public:
	static gedString GetMultipleFileName(const gedString& file);
	void ReadDrives();
	gedString GetFullPath();
	void Populate(bool bForward = true);
	ListDirectory(int x = 0, int y = 0,
			   int width = 0, int height = 0,
			   int listpopId = -1,
			   Actor *parent = NULL,
			   gedString buttonText = "");
	virtual ~ListDirectory();

	
	bool OnMouseButtonDown(int x, int y, Uint8 button);
	bool OnList(ListPop *list, int index, gedString &text, int listId);

	void SetFilter(const gedString &filter);
	void AddFilter(const gedString &filter);
	bool Filter(gedString file);
	void setShowDir(bool value) {bShowDirs = value;}
	void setShowExtension(bool value) {bShowExtension = value;}

	void setCanUseSystemFileDialog(bool value) {bCanUseSystemFileDialog = value;}
	void SetSave() {bSave = true;}
	void SetSaveName(gedString name) {saveName = name;}

private:
	long handle;


	gedString filter, saveName;
	static char drives[27];
	static bool bReadDrives;
	ListString multiFilter;

	bool bShowDirs, bShowExtension, bSave, bCanUseSystemFileDialog;
};


#endif // !defined(AFX_LISTDIRECTORY_H__F7B6D562_1523_4A86_86A9_7D9C847DE250__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME