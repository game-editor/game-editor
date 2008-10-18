#ifndef STAND_ALONE_GAME //maks

#include "KrImageListBox.h"
#include "../gui/eventmanager.h"
#include "../engine/boxresource.h"
#include "../engine/box.h"
#include "../engine/fontresource.h"
//#include "../engine/textbox.h"
#include "../gui/textwidget.h"

KrImageListBox::KrImageListBox(int _width, int _height, int _imageheight, 
						const KrScheme& s): KrWidget( s )
{
	imageHeight = _imageheight;
	width = _width;
	height = _height;
	firstItem = 0;
	selectedItem = 0;
	viewSelection = 0;
	currHiLight = 0;

	if(s.font)
	   fontHeight = s.font->FontHeight() + 2;
	else
	   fontHeight = 0;

	//We should not assume we got a font from the scheme
	numVisibleItems = ( height  ) / imageHeight + 1;	
	height = numVisibleItems * imageHeight;
	outerBevel = new KrBevelElement( width, height, s );
}

KrImageListBox::~KrImageListBox()
{
	for(unsigned i = 0; i < objectImages.Count(); ++i)
		delete objectImages[i].image;

	delete outerBevel;
}

int KrImageListBox::AddImage( KrImage* _image, gedString text )
{	
	ImageItem item;
	item.image = _image;
	item.clone = 0;
	item.textWidget = 0;
	item.imageText = text;
	objectImages.PushBack( item );
	
	DrawImage();
	return objectImages.Count() - 1;
}

void KrImageListBox::AddedtoTree()
{
	objectViewImages.SetCount( numVisibleItems );
		
	for( unsigned i=0; i<objectViewImages.Count(); ++i )
	{	
		objectViewImages[i]  = new KrBox(width, imageHeight, scheme.primary, KrBoxResource::FILL);
		objectViewImages[i]->SetPos( 1, i * imageHeight + 1 );
		Engine()->Tree()->AddNode( this, objectViewImages[i] );	
	}
	DrawImage();
	outerBevel->AddToTree( Engine(), this );
	outerBevel->DrawIn();
}

void KrImageListBox::MoveSelection( int delta )
{
	//There might be more items than we can display so check against
	//the item array but calc against the view array
	if (    delta != 0 
	     && GlInRange( selectedItem+delta, 0, (int) objectImages.Count()-1 ) )
		 
	{	
	
		selectedItem += delta;
		
		if ( selectedItem < firstItem )
		{
			firstItem = selectedItem;
		}
		//Out of the view range, so scroll down delta
		//keep selectedItem relative to the ImagesList
		else if ( selectedItem >= firstItem + (int) objectViewImages.Count() )
		{
			firstItem += delta;

		}
		//ViewSelection should always be relative to the view range
		viewSelection  += delta;;

		//Clamp it
		if(viewSelection <0)
			viewSelection = 0;
		else
		if(viewSelection>=(int)objectViewImages.Count())
		   viewSelection = (int)objectViewImages.Count()-1;

		PublishEvent( SELECTION, selectedItem, 0, 0, 0 );

		DrawImage();
	}

}

void KrImageListBox::DrawImage()
{
	
	//TODO: Only check those items previously in the range instead of all
	unsigned i;
	for(i=0; i<objectImages.Count(); ++i )
	{
		if(objectImages[i].clone && objectImages[i].clone->Parent())
		//	objectImages[i].clone->SetVisible(false);
		   Engine()->Tree()->DeleteNode(objectImages[i].clone);
		if(objectImages[i].textWidget && objectImages[i].textWidget->Parent())
		//	objectImages[i].textWidget->SetVisible(false);
			 Engine()->Tree()->DeleteNode(objectImages[i].textWidget);

		objectImages[i].clone = 0;
		objectImages[i].textWidget = 0;
	}
	
	for(  i=0; i<objectViewImages.Count(); ++i )
	{
		KrColorTransform normal;	

		int index = i + firstItem;				

		if ( GlInRange( index, 0, (int) objectImages.Count() -1) )
		{	
			objectImages[index].clone = (KrImage*)objectImages[index].image->Clone();
			CalcImagePosition(objectImages[index].clone);			
			Engine()->Tree()->AddNode( objectViewImages[i], objectImages[index].clone );

			if ( scheme.font )
			{
			     objectImages[index].textWidget = new KrTextWidget( width - 2, scheme.font->FontHeight(),
										          false, true, false, scheme );
			//	 objectImages[index].textWidget = new KrTextBox(  scheme.font, width - 2, scheme.font->FontHeight(),
			//		 0);

				 CalcTextPosition(objectImages[index]);
				 Engine()->Tree()->AddNode( objectViewImages[i], objectImages[index].textWidget );
			     objectImages[index].textWidget->SetText( objectImages[index].imageText ); //maks
			}

		}

		if ( i == (unsigned) viewSelection)
			objectViewImages[i]->SetColor( scheme.CalcHiPrimary() );
		else
			objectViewImages[i]->SetColor( normal );
	}
}

void KrImageListBox::CalcTextPosition(ImageItem& item)
{
	//Perhaps one day we can center the text?
	item.textWidget->SetPos(0, imageHeight - scheme.font->FontHeight());
}

void KrImageListBox::CalcImagePosition(KrImage* image)
{
	KrRect bounds;
	image->QueryBoundingBox( &bounds );

	GlFixed wScale = GlFixed( width ) / GlFixed( bounds.Width() );
	GlFixed hScale = GlFixed( imageHeight ) / GlFixed( bounds.Height() + fontHeight);
	GlFixed scale = GlMin( wScale, hScale );
	scale = GlMin( scale, GlFixed(1) );
	GLASSERT( scale > 0 );

	int imageX = width - ( scale * bounds.Width() ).ToInt();
		imageX = imageX / 2 - ( scale * bounds.xmin ).ToInt() + 1;

	int imageY = imageHeight - ( scale * bounds.Height() + fontHeight ).ToInt();
		imageY = imageY / 2 - ( scale * bounds.ymin ).ToInt() + 1;

	image->SetScale( scale, scale );
	image->SetPos( imageX, imageY );
}

void KrImageListBox::MouseClick( int down, int x, int y )
{
	if (down == KrWidget::LEFT_DOWN &&  //maks
		GlInRange( y / imageHeight, 0, (int) objectImages.Count()-1 ) ) 
	{			
		viewSelection = y / imageHeight;
		selectedItem = firstItem + viewSelection;
		PublishEvent( SELECTION, selectedItem, 0, 0, 0 );
		DrawImage();
	}
}

void KrImageListBox::MouseIn( bool down, bool in  )
{	
//	if(!in)
	{
		KrColorTransform normal;
		if(currHiLight == viewSelection)
			objectViewImages[currHiLight]->SetColor( scheme.CalcHiPrimary() );
		else
		objectViewImages[currHiLight]->SetColor( normal );
	}
}

void KrImageListBox::MouseMove( bool down, int x, int y  )
{
//	KrColorTransform normal;

//	if(viewSelection != currHiLight)
//	   objectViewImages[currHiLight]->SetColor( normal );
//	else
//	   objectViewImages[currHiLight]->SetColor( scheme.CalcHiPrimary() );

	currHiLight = y / imageHeight;
	if(currHiLight < (int) objectImages.Count())
	   objectViewImages[currHiLight]->SetColor( scheme.CalcHiSec() );
	
}

KrImage* KrImageListBox::QuerySelectedItem()
{
	if(objectImages.Count() == 0)
		return 0;
	else
	    return objectImages[selectedItem].image; 
}

gedString& KrImageListBox::QuerySelectedItemText()
{
	if(objectImages.Count() != 0)
	   return objectImages[selectedItem].imageText; 


	return emptyString;		
}

#endif //#ifndef STAND_ALONE_GAME //maks
