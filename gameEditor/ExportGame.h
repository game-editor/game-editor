// ExportGame.h: interface for the ExportGame class.
//
//////////////////////////////////////////////////////////////////////

#if /*defined(GAME_EDITOR_PROFESSIONAL) &&*/ defined(WIN32) && !defined(STAND_ALONE_GAME)

#if !defined(AFX_EXPORTGAME_H__2A2119ED_8FB7_4345_97CA_082E905FB445__INCLUDED_)
#define AFX_EXPORTGAME_H__2A2119ED_8FB7_4345_97CA_082E905FB445__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class ExportGame : public Panel  
{
public:
	void OnButton(Button *button, int buttonId);
	ExportGame();
	virtual ~ExportGame();
	
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnEditChar(EditBox *edit, SDLKey key, int line);

	static void ClearExportName() {exportName.clear();}


private:
	EditBox *name;
	ListPop *listSystem;
	ListDirectory *dir;	

	static gedString exportName;
};

#endif // !defined(AFX_EXPORTGAME_H__2A2119ED_8FB7_4345_97CA_082E905FB445__INCLUDED_)


#endif ///*!defined(GAME_EDITOR_PROFESSIONAL) &&*/ !defined(STAND_ALONE_GAME)
