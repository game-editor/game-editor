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


// MuteEx.h: interface for the MuteEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUTEEX_H__846E260D_7FFE_4E2E_B0E3_E11499C67D8F__INCLUDED_)
#define AFX_MUTEEX_H__846E260D_7FFE_4E2E_B0E3_E11499C67D8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct SDL_mutex;
class MuteEx  
{
public:
	MuteEx(SDL_mutex *mutexTimer);
	virtual ~MuteEx();

private:
	SDL_mutex *mutex;
};

#endif // !defined(AFX_MUTEEX_H__846E260D_7FFE_4E2E_B0E3_E11499C67D8F__INCLUDED_)
