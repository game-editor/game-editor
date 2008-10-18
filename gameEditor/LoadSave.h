// LoadSave.h: interface for the LoadSave class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_LOADSAVE_H__F69676C5_0661_4778_A547_7AB8155C4608__INCLUDED_)
#define AFX_LOADSAVE_H__F69676C5_0661_4778_A547_7AB8155C4608__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"


enum
{
	LOAD_DIALOG,
	MERGE_DIALOG,
	SAVE_DIALOG,
	LOADSAVE_OK,
	LOADSAVE_CLOSE
};

class LoadSave : public Panel  
{
public:
	void Init(int command, gedString title, gedString confirmButtonText);
	void OnButton(Button *button, int buttonId);
	LoadSave(bool bExtendHeight = false);
	virtual ~LoadSave();
	
	bool OnList(ListPop *list, int index, gedString &text, int listId);

	void SetFilter(const gedString &filter);
	void AddFilter(const gedString &filter);
	void SetText(gedString str);
	gedString GetText();
	gedString GetDirectory();
	bool IsOk() {return bOk;}

private:
	EditBox *name;		
	bool bOk;

protected:
	int command;
	ListDirectory *dir;
};

#endif // !defined(AFX_LOADSAVE_H__F69676C5_0661_4778_A547_7AB8155C4608__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME