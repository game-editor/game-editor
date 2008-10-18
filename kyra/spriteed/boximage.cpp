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


#include "boximage.h"
#include "../engine/color.h"
#include "../engine/painter.h"
#include "sharedstate.h"



// BoxImage::BoxImage( int _style, int _width, int _height, 
// 					TiXmlElement* _owner, SharedStateData* _shared )
// {
// 	style  = _style;
// 	width  = _width;
// 	height = _height;
// 	shared = _shared;
// 	owner  = _owner;
// }
// 
// 


/*void BoxImage::Draw( KrPaintInfo* paintInfo, const KrRect* clip )
{
// 	KrPainter painter( paintInfo );
// 
// 	switch ( style )
// 	{
// 		case TILE:
// 		{
// 			if ( shared->CurrentTile() == owner )
// 			{
// 				painter.DrawBox( bounds.xmin, 
// 								 bounds.ymin, 
// 								 bounds.Width(), 
// 								 bounds.Height(), 
// 								 TILE_SELECTED_COLOR,
// 								 TILE_SELECTED_NCOLORS );
// 			}
// 			else
// 			{
// 				painter.DrawBox( bounds.xmin, 
// 								 bounds.ymin, 
// 								 bounds.Width(), 
// 								 bounds.Height(), 
// 								 TILE_NOT_SELECTED_COLOR,
// 								 TILE_NOT_SELECTED_NCOLORS );
// 			}
// 		}
// 		break;
// 
// 		case SPRITE:
// 		{
// 			if ( shared->CurrentFrame() == owner )
// 			{
// 				painter.DrawBox( bounds.xmin, 
// 								 bounds.ymin, 
// 								 bounds.Width(), 
// 								 bounds.Height(), 
// 								 SPRITE_SELECTED_COLOR,
// 								 SPRITE_SELECTED_NCOLORS );
// 			}
// 			else
// 			{
// 				painter.DrawBox( bounds.xmin, 
// 								 bounds.ymin, 
// 								 bounds.Width(), 
// 								 bounds.Height(), 
// 								 SPRITE_NOT_SELECTED_COLOR,
// 								 SPRITE_NOT_SELECTED_NCOLORS );
// 			}
// // 			// Print up the frame #
// // 			TiXmlNode* parent = owner->Parent();
// // 			TiXmlNode* child;
// // 			int count = 0;
// // 
// // 			for( child = parent->FirstChild( "Frame" );
// // 				 child;
// // 				 child = child->NextSibling( "Frame" ), count++ )
// // 			{
// // 				if ( child == owner )
// // 				{
// // 					sprintf( buf, "%d", frame. )
// // 					CON_DrawText( buf, 
// 		}
// 		break;
// 
// 		case DRAGGING:
// 		{
// 			painter.DrawBox( bounds.xmin, 
// 							 bounds.ymin, 
// 							 bounds.Width(), 
// 							 bounds.Height(), 
// 							 DRAGGING_COLOR,
// 							 DRAGGING_NCOLORS );
// 		}
// 		break;
// 
// 		case HOTCROSS:
// 		{
// 			painter.DrawHLine( bounds.xmin,
// 							   bounds.ymin + bounds.Height() / 2,
// 							   bounds.Width(),
// 							   HOTCROSS_COLOR,
// 							   HOTCROSS_NCOLORS );
// 			painter.DrawVLine( bounds.xmin + bounds.Width() / 2,
// 							   bounds.ymin,
// 							   bounds.Height(),
// 							   HOTCROSS_COLOR,
// 							   HOTCROSS_NCOLORS );
// 		}
// 		break;
// 
// 		case H_ALIGNMENT:
// 		{
// 			painter.DrawHLine( bounds.xmin,
// 							   bounds.ymin,
// 							   bounds.Width(),
// 							   ALIGNMENT_COLOR,
// 							   ALIGNMENT_NCOLORS );
// 		}
// 		break;
// 
// 		case V_ALIGNMENT:
// 		{
// 			painter.DrawVLine( bounds.xmin,
// 							   bounds.ymin,
// 							   bounds.Height(),
// 							   ALIGNMENT_COLOR,
// 							   ALIGNMENT_NCOLORS );
// 		}
// 		break;
// 
// 		default:	
// 			GLASSERT( 0 );
// 	}
}


*/