#ifndef STAND_ALONE_GAME

#include "MainPanel.h"
#include "Action.h"


Actor *Game_Main()
{
	return MainPanel::Create();		
}


#endif //#ifndef STAND_ALONE_GAME