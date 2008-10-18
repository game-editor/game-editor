

// KrImageListBox.h: interface for the KrImageListBox class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME //maks

#ifndef KYRA_IMAGESCROLLBOX_INCLUDED
#define KYRA_IMAGESCROLLBOX_INCLUDED


#include "../gui/widget.h"

class KrTextWidget;


/**	A widget to display a listbox of images.

	** Note: This is an externally submitted widget. It has a different
	   author and does not necessarily share the same license as
	   main Kyra code.
	** Also note: Send in your widgets! We'de like them to have a home
	   here where everyone can use them.

	Author:			Tim Meyer
	Contact info:	tmeyer3@bellsouth.net
	Contributing:	--
	Questions about this widget?	Arianne forums. (http://www.arianne.info)

*/
class KrImageListBox : public KrWidget  
{
public:
	KrImageListBox(int width, int height, int imageheight, const KrScheme& scheme);
	virtual ~KrImageListBox();

	KrImage* QuerySelectedItem();
	gedString& QuerySelectedItemText();

	void AddedtoTree();
	int AddImage( KrImage* _image, gedString text );
	int NumItems()									{ return objectImages.Count(); }
	/// Return the index of the selected item.
	int QuerySelectedItemNum()								{ return selectedItem; }
		
	void MoveSelection( int delta );
	virtual int  IsMouseListener()						{ return LEFT_MOUSE; }
	virtual void MouseClick( int down, int x, int y );		
	virtual void MouseMove( bool down, int x, int y );
	virtual void MouseIn( bool down, bool in  );
	
private:
	
	void DrawImage();
	void CalcImagePosition(KrImage* image);
	
	int width, height;
	int imageHeight;
	int fontHeight;
	
	KrBevelElement  *outerBevel;
	
	typedef struct{
		KrImage* image;
		KrImage* clone;
		KrTextWidget* textWidget;
		gedString imageText;
	}ImageItem;

	void CalcTextPosition(ImageItem& item);
	GlDynArray< ImageItem >   objectImages;	
	GlDynArray< KrBox* >     objectViewImages;
	
	int firstItem;
	int selectedItem;
	int viewSelection;
	int currHiLight;

	unsigned numVisibleItems;
	gedString emptyString;
};

#endif 

#endif //#ifndef STAND_ALONE_GAME //maks