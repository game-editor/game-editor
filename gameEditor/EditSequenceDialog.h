/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues

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
<http://game-editor.com/License>, thats will help us to pay our developers 
<http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>.

***************************************************************************/


// EditSequenceDialog.h: interface for the EditSequenceDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_EDITSEQUENCEDIALOG_H__2A2E2233_3578_4C69_BCFE_DD50F1E7ED56__INCLUDED_)
#define AFX_EDITSEQUENCEDIALOG_H__2A2E2233_3578_4C69_BCFE_DD50F1E7ED56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"
#include "Sequence.h"

class EditSequenceDialog : public Panel  
{
public:
	void OnButton(Button *button, int buttonId);
	EditSequenceDialog(Actor *actor, Sequence *seq);
	virtual ~EditSequenceDialog();

private:
	EditBox *frames, *editFps;
	Sequence *sequence;
	Actor *actor;
};

#endif // !defined(AFX_EDITSEQUENCEDIALOG_H__2A2E2233_3578_4C69_BCFE_DD50F1E7ED56__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME