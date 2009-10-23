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


// Hermite.h : header file
//
////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATH_H__10025586_451C_462A_AF90_0250027A26C1__INCLUDED_)
#define AFX_PATH_H__10025586_451C_462A_AF90_0250027A26C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000




#include "Envelope.h"

/////////////////////////////////////////////////////////////////////////////
// Path 

class Path : public Envelope
{
public:
	
	void SetKey(int key, int x, int y);	
	void Load(SDL_RWops *src);
	void Save(SDL_RWops *src);


	
	void ConstantVelocity();
	
	void SetOri(int x, int y);
	Path(const gedString& pathName = "", int nKeys = 0, int pathLenght = 0);
	~Path();

	void GetXY(int &x, int &y, int frame);
	void GetX(int &x, int frame);
	void GetY(int &y, int frame);
	double GetAngle(int frame);
	void AddKey(int x, int y, bool bLinear, double tens = 0.0);
		
	
	int getXOri() {return xOri;};
	int getYOri() {return yOri;};

private:

	//Uniform key
	Uint16 maxKeys;
	Uint16 insertionFrame;
	Uint32 pathLenght;
	Uint32 xOri;
	Uint32 yOri;
};

#endif
