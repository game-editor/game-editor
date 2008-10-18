/*--License:
	Kyra Sprite Engine
	Copyright Lee Thomason (Grinning Lizard Software) 2001-2002
	www.grinninglizard.com/kyra
	www.sourceforge.net/projects/kyra

	Kyra is provided under 2 licenses:

	- The GPL, with no additional restrictions.
	- The LGPL, provided you display the Kyra splash screen, described below.


--- GPL License --
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


--- LGPL License --
  **Provided you kindly display the Kyra splash screen (details below), 
	you	may use the LGPL license:**

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	The full text of the license can be found in lgpl.txt


--- Kyra Splash Screen.

	It would be appreciate if you display the Kyra splash screen when using
	either license, however it is only required for the LGPL. All the
	resources for the splash are compiled into the library, and it can be
	accessed through the following API:

		KrEngine::StartSplash
		KrEngine::UpdateSplash
		KrEngine::EndSplash

	Full documentation is provided with the KrEngine class. The splash screen
	should be displayed for 2 seconds.

	Thank you.
*/


#ifndef KYRA_EdWidget_INCLUDED
#define KYRA_EdWidget_INCLUDED


#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#include "../../gameEngine/gedString.h"

#include "../util/gllist.h"
#include "../engine/parser.h"
#include "../engine/color.h"


class KrImNode;
class KrImageTree;
class KrBox;
class KrBoxResource;
class EdWidgetFrame;
class EdWidgetAction;
class EdWidgetSprite;
class EdWidgetTile;
class KrFontResource;
class KrTextBox;
class KrConsole;
union KrRGBA;


class EdWidget
{
  public:
	enum 
	{
		WIDGET,
		SPRITE,
		ACTION,
		FRAME,
		TILE
	};

	enum
	{
		SELECT = 0x01,
        DRAGGING_NCOLORS = 4,
        HOTCROSS_NCOLORS = 2,
        TILE_SELECTED_NCOLORS = 4,
        TILE_NOT_SELECTED_NCOLORS = 2,
        SPRITE_SELECTED_NCOLORS = 4,
        SPRITE_NOT_SELECTED_NCOLORS = 2,
	};

	EdWidget( KrImageTree* _tree,
			  KrImNode* _krimNode,
			  EdWidget* _parent );

	virtual ~EdWidget();

	virtual void Select( bool select )	{}
	virtual void Update()				{}

	GlSList< EdWidget* > children;

	virtual bool	IsThisYours( KrBox* box) { return false; }
	EdWidget*		FindWidget( KrBox* box, int flags );
	EdWidget*		Parent()	{ return parent; }
	virtual int		Type()		{ return WIDGET; }

	virtual EdWidgetSprite*	ToSprite()	{ return 0; }
	virtual EdWidgetAction*	ToAction()	{ return 0; }
	virtual EdWidgetFrame*	ToFrame()	{ return 0; }
	virtual EdWidgetTile*	ToTile()	{ return 0; }

	void Save(	const gedString& xmlFilename,
				KrConsole* console,
				const gedString& surfaceName,
				int nTrans,
				const KrRGBA* trans );

	virtual void BuildXML( TiXmlNode* parent );

  protected:
	KrImNode* krimNode;
	KrImageTree* tree;
	EdWidget* parent;

    // Should really be static, having one for every class is silly:
	static bool colorInit;
    static KrRGBA DRAGGING_COLOR[ DRAGGING_NCOLORS ];
    static KrRGBA HOTCROSS_COLOR[HOTCROSS_NCOLORS];
    static KrRGBA TILE_SELECTED_COLOR[TILE_SELECTED_NCOLORS];
    static KrRGBA TILE_NOT_SELECTED_COLOR[TILE_NOT_SELECTED_NCOLORS];
    static KrRGBA SPRITE_NOT_SELECTED_COLOR[SPRITE_NOT_SELECTED_NCOLORS];
    static KrRGBA SPRITE_SELECTED_COLOR[SPRITE_SELECTED_NCOLORS];
};


class EdWidgetTile : public EdWidget
{
  public:
	EdWidgetTile(	KrImageTree* tree, KrImNode* krimNode, EdWidget* _parent,
					const gedString& name,
					int x, int y,
					int size );
	~EdWidgetTile();

	virtual void Select( bool select );

	virtual bool IsThisYours( KrBox* _box)	{	//GLOUTPUT( "Check Tile\n" );
												return ( box == _box ); }

	virtual int		Type()				{ return TILE; }
	virtual EdWidgetTile*	ToTile()	{ return this; }
	void Set( int x, int y, int size );
	const gedString& Name()			{ return name; }
	int Size()							{ return size; }

	virtual void BuildXML( TiXmlNode* parent );

  private:
	gedString name;

	KrBoxResource*	normalRes;
	KrBox*			box;

	int x;
	int y;
	int size;
};
	

class EdWidgetSprite : public EdWidget
{
  public:
	EdWidgetSprite(	KrImageTree* tree,
					KrImNode* krimNode,			// Where to add/remove image.
					EdWidget* _parent,
					const gedString& _name );

	~EdWidgetSprite()							{}
  
	virtual void	Select( bool select )		{}
	virtual bool	IsThisYours( KrBox* box)	{ //GLOUTPUT( "Check Sprite\n" ); 
												  return false; }
	virtual int		Type()						{ return SPRITE; }
	virtual EdWidgetSprite*	ToSprite()			{ return this; }
	const gedString& Name()					{ return name; }

	virtual void BuildXML( TiXmlNode* parent );

  private:
	gedString name;
};


class EdWidgetAction : public EdWidget
{
  public:
	EdWidgetAction(	KrImageTree* tree,
					KrImNode* krimNode,
					EdWidget* _parent,
					const gedString& name );
	~EdWidgetAction()					{}
  
	virtual void Select( bool select )	{}
	virtual bool	IsThisYours( KrBox* box) { //GLOUTPUT( "Check Action\n" ); 
												return false; }
	virtual EdWidgetAction*	ToAction()	{ return this; }
	virtual int		Type()		{ return ACTION; }
	const gedString& Name()	{ return name; }

	virtual void BuildXML( TiXmlNode* parent );

  private:
	gedString name;
};


class EdWidgetFrame : public EdWidget
{
  public:
	EdWidgetFrame(	KrImageTree* tree,
					KrImNode* krimNode,
					EdWidget* _parent,
					KrFontResource* fontRes,
					const KrDom::Frame& frameData  );
	~EdWidgetFrame();

	virtual void Select( bool select );
	virtual bool IsThisYours( KrBox* _box)	{ 
												//GLOUTPUT( "Check Frame\n" );
												return ( box == _box ); 
											}
 	virtual int		Type()		{ return FRAME; }
	virtual EdWidgetFrame*	ToFrame()	{ return this; }

	const KrDom::Frame& GetFrameData()	{ return frameData; }
	void SetFrameData( const KrDom::Frame& data );
	virtual void Update()				{ SetFrameNumber(); }

	virtual void BuildXML( TiXmlNode* parent );

  private:
	enum {
		CROSSSIZE = 5
	};

	void SetFrameNumber();

	KrDom::Frame frameData;
	KrBoxResource*	normalRes;
	KrBoxResource*	crossVRes;
	KrBoxResource*	crossHRes;
	
	KrBox*	box;
	KrBox*	crossV;
	KrBox*	crossH;
	KrTextBox*	text;
};

#endif
