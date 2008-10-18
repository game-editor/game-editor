/*--License:
	Kyra Sprite Engine
	Copyright Lee Thomason (Grinning Lizard Software) 2001-2002
	www.grinninglizard.com/kyra
	www.sourceforge.net/projects/kyra

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

	The full text of the license can be found in license.txt
*/


#ifndef KYRA_BOX_IMAGE_INCLUDED
#define KYRA_BOX_IMAGE_INCLUDED

class SharedStateData;
class TiXmlElement;

/*
class BoxImage : public KrUserImage
{
  public:
	enum 
	{
		TILE,
		SPRITE,
		HOTCROSS,
		DRAGGING,
		H_ALIGNMENT,
		V_ALIGNMENT
	};

	BoxImage(	int style, int width, int height, 
				TiXmlElement* owner,
				SharedStateData* );

	virtual void Draw(	KrPaintInfo* paintInfo, const KrRect* clip );
	virtual int  Width()	{ return width; }
	virtual int  Height()	{ return height; }

  private:
	int style;
	int width;
	int height;
	SharedStateData* shared;
	TiXmlElement* owner;
};
*/

#endif