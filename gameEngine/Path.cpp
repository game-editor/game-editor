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


#pragma warning(disable:4786)

#include <math.h>
#include <stdlib.h>
#include <stdlib.h>
#include "Path.h"
#include "GameControl.h"
#include "SDL_endian.h"

#include "dlmalloc.h"


Path::Path(const gedString& pathName, int nKeys, int pathLenght)
{
	name = pathName;

	this->pathLenght = pathLenght;
	maxKeys = nKeys;
	insertionFrame = 0;	
	xOri = yOri = 0;

	nChannels = 2;
}

Path::~Path()
{
		
}


void Path::AddKey(int x, int y, bool bLinear, double tens)
{
	if(m_KeyFrame < maxKeys)
	{
		AddKeyFrame(x, insertionFrame, 0, tens);
		AddKeyFrame(y, insertionFrame, 1, tens);
		SetLinear(m_KeyFrame-1, bLinear);
		
		insertionFrame += pathLenght / (maxKeys - 1);
	}
}

void Path::GetXY(int &x, int &y, int frame)
{
	ChanVec f;
	Value(f, frame);

	x = (int)f[0];
	y = (int)f[1];
}

void Path::GetX(int &x, int frame)
{
	ChanVec f;
	Value(f, frame);

	x = (int)f[0];
}

void Path::GetY(int &y, int frame)
{
	ChanVec f;
	Value(f, frame);

	y = (int)f[1];
}

extern double getAngle(double dx, double dy);

double Path::GetAngle(int frame)
{
	ChanVec f;
	double angle = 0.0;
	if(m_TotalFrame < 3) return 0.0; //Need 3 frames

	if(frame < 0) frame = 0;
	else if(frame >= m_TotalFrame) frame = m_TotalFrame - 1; 


	if(frame == 0)
	{
		double x1, y1, x2, y2, dx, dy;

		//First point
		Value(f, 0);
		
		x1 = f[0];
		y1 = f[1];

		//Second point
		Value(f, 1);
		
		x2 = f[0];
		y2 = f[1];

		dx = x2 - x1;
		dy = y2 - y1;

		angle = getAngle(dx, dy);
	}
	else if(frame == m_TotalFrame - 1)
	{
		double x1, y1, x2, y2, dx, dy;

		//First point
		Value(f, m_TotalFrame - 2);
		
		x1 = f[0];
		y1 = f[1];

		//Second point
		Value(f, m_TotalFrame - 1);
		
		x2 = f[0];
		y2 = f[1];

		dx = x2 - x1;
		dy = y2 - y1;

		angle = getAngle(dx, dy);
	}
	else
	{
		double x1, y1, x3, y3;

		//First point
		Value(f, frame - 1);
		
		x1 = f[0];
		y1 = f[1];

		//Third point
		Value(f, frame + 1);
		
		x3 = f[0];
		y3 = f[1];

		angle = getAngle(x3-x1, y3-y1);
	}
	

	return angle;
}

void Path::SetOri(int x, int y)
{
	xOri = x;
	yOri = y;
}

void Path::ConstantVelocity()
{
	//Make a constant velocity path based on path time lengh and path total distance

	double distance = 0;
	double *distanceKey = (double *)malloc(m_KeyFrame*sizeof(double));
	int x1, x2, y1, y2, keyFrame = 0;

	//Calculate distance
	int i;
	for(i = 1; i <= m_TotalFrame; i++)
	{
		GetXY(x1, y1, i-1);
		GetXY(x2, y2, i);

		distance += sqrt((double)((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)));

		if((keyFrame = KeyNumber(i)) != 0)
		{
			distanceKey[keyFrame] = distance;
		}
	}

	//Adjust frames
	if(keys)
	{
		for(i = 1; i < m_KeyFrame-1; i++)
		{
			keys[i].step = (int)(m_TotalFrame*distanceKey[i]/distance);
			if(keys[i].step <= keys[i-1].step)
			{
				keys[i].step = keys[i-1].step+1;
			}
		}
	}

	free(distanceKey);
}


void Path::Save(SDL_RWops *src)
{
	name.Write(src);

	SDL_WriteLE16(src, nChannels);
	SDL_WriteLE16(src, m_KeyFrame);
	SDL_WriteLE16(src, m_TotalFrame);
	SDL_RWwrite(src, &f_ini, sizeof(f_ini), 1);
	SDL_WriteLE16(src, maxKeys);
	SDL_WriteLE16(src, insertionFrame);
	SDL_WriteLE32(src, pathLenght);
	SDL_WriteLE32(src, xOri);
	SDL_WriteLE32(src, yOri);
	SDL_RWwrite(src, keys, m_KeyFrame*sizeof(KeyFrame), 1);

	//Final expansion indicator
	Uint8 expansion = 0;
	SDL_RWwrite(src, &expansion, sizeof(expansion), 1);
}


void Path::Load(SDL_RWops *src)
{
	name.Read(src);

	nChannels = GameControl::Read16(src);
	m_KeyFrame = GameControl::Read16(src);
	m_TotalFrame = GameControl::Read16(src);
	GameControl::Read(src, &f_ini, sizeof(f_ini), 1);
	maxKeys = GameControl::Read16(src);
	insertionFrame = GameControl::Read16(src);
	pathLenght = GameControl::Read32(src);
	xOri = GameControl::Read32(src);
	yOri = GameControl::Read32(src);
	
	if(keys) delete [] keys;
	keys = new KeyFrame[m_KeyFrame];
	GameControl::Read(src, keys, m_KeyFrame*sizeof(KeyFrame), 1);

#ifdef GP2X
	for(int i = 0; i < m_KeyFrame; i++)
	{
		keys[i].ToGP2X();
	}
#endif

	motion.keylist = keys;
	motion.keys = m_KeyFrame;
	motion.steps = m_TotalFrame;

	//Final expansion indicator
	Uint8 expansion = 0;
	GameControl::Read(src, &expansion, sizeof(expansion), 1);
}

void Path::SetKey(int key, int x, int y)
{
	if(keys && key >= 0 &&  key < m_KeyFrame)
	{
		keys[key].cv[0] = x;
		keys[key].cv[1] = y;
	}
}


