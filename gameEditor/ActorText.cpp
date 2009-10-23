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


// ActorText.cpp: implementation of the ActorText class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "ActorText.h"
#include "GameControl.h"
#include "LoadSaveScript.h"
#include "Tutorial.h"
#include "Config.h"

#include "../gameEngine/dlmalloc.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings

#define TIP_TEXT			"\
Type your text here. If you type nothing and input is disable, the actual text will be removed.\n\
If input is enable and you don't want display an text, type blanks to define your input dimensions.\n\
Texts and inputs can be read in the text or textNumber actor variable"

#define TIP_TEXT_FONT			"\
Select the font text previously created with the \"New Font\" Button.\n\
If the font was created but was not used by any actor, it will be removed."

#define TIP_TEXT_NEWFONT			"\
Specify a new image font or a True Type font"

#define TIP_TEXT_FILE			"\
Load a text file or save the Actor text to a file"

#define TIP_TEXT_BITMAP			"\
Create a font from a image file.\n\
The image file must contain all image characters\n\
in one line in ASCII sequence:\n\
\n\
\" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_'abcdefghijklmnopqrstuvwxyz{|}~\"\n\
\n\
Set initial image character and number of characters\n\
On the sample above blank is the initial character\n\
and the number of characters is 95\n\
\n\
Loads tga, jpeg, bmp, png, xpm, xcf, pcx, jpg and tif image files"

#define TIP_TEXT_BITMAP_FILE			"Select the image file"

#define TIP_TEXT_TTF			"\
Select a True Type font file.\n\
\n\
These fonts can be found in the \"Fonts\"\n\
directory on your system or on the Internet."

#define TIP_TEXT_TTF_FILE			"Select the True Type font file"

#define TIP_TEXT_TTF_COLOR			"Click to choose the font color"

#define TIP_TEXT_TTF_STYLE			"\
Select the font style:\n\
Normal, Bold, Italic or Underline\n\
\n\
No all fonts have all styles"

#define TIP_TEXT_TTF_SOFT			"\
Use this options to make fonts with soft (antialiased) edges"

#define TIP_INPUT_ENABLE	"\
Select \"Yes\" to make this text\n\
editable by the user for text input."

#define TIP_INPUT_CONFIG	"\
Click here to configure the text input"

#define TIP_TEXT_CONFIG_INPUT "\
Use this panel to configure text input"

#define TIP_TEXT_CONFIG_INPUT_BORDER "\
Select \"Yes\" to draw a border around the text"

#define TIP_TEXT_CONFIG_INPUT_BORDER_COLOR "\
Click to choose the border color"

#define TIP_TEXT_CONFIG_INPUT_BACKGROUND "\
Select \"Yes\" to draw a background behind the text"

#define TIP_TEXT_CONFIG_INPUT_BACKGROUND_COLOR "\
Click to choose the background color"

#define TIP_TEXT_CONFIG_INPUT_CURSOR_COLOR "\
Click to choose the cursor color"

#define TIP_TEXT_CONFIG_INPUT_FOCUS "\
Select \"Yes\" to set the keyboard focus to the text input box"

//////////////////////////////////////////////////////////////////////////////

enum
{
	BT_OK,
	BT_CANCEL,
	LS_NEW_FONT,
	LS_ACTOR,
	LS_STYLE,
	LS_SOFT,
	LS_FONT,
	LS_FILE,
	LS_INPUT_ENABLE,
	BT_INPUT_CONFIG,
	LS_BORDER,
	LS_BACKGROUND,
	LS_INPUT_FOCUS
};

#define WIDTH	600
#define HEIGHT	330

#define BITMAP_WIDTH	380
#define BITMAP_HEIGHT	176
#define SAMPLE_SIDE		50
#define SAMPLE_X		128
#define SAMPLE_Y		93

#define TTF_WIDTH		360
#define TTF_HEIGHT		175

#define INPUT_WIDTH		360
#define INPUT_HEIGHT	180

char fontBitmapPath[PATH_LIMIT];
char fontTTFPath[PATH_LIMIT];
char winTTFPath[PATH_LIMIT];
char oldUserPath[PATH_LIMIT];


class EditBox;
static bool bInitBitmapPath = false;
static bool bInitTTFPath = false;

void CheckSystemFontPath()
{
	if(!bInitTTFPath)
	{
		fontTTFPath[0] = 0;
		bInitTTFPath = true;
		
#ifdef WIN32
		oldUserPath[0] = 0;
		if(GetWindowsDirectory(winTTFPath, PATH_LIMIT))
		{
			if(winTTFPath[strlen(winTTFPath) - 1] != '\\')
			{
				strcat(winTTFPath, "\\");
			}
			
			strcat(winTTFPath, "Fonts");
			
			strcpy(fontTTFPath, winTTFPath);
			
			ged_getcwd(oldUserPath, PATH_LIMIT);
			chdir(fontTTFPath);
			
			ged_getcwd(winTTFPath, PATH_LIMIT); //Make the correct slashs for restore
		}			
#endif
	}
	else chdir(fontTTFPath);
}

/* Creates a Surface which can be used by SFont or BFont. In addition to the
Surface which is returned by TTF, we use a fixed color for transparency. Because
TTF uses "255 - text color" as the colorkey, you can use each color for the text. 
(Pretty smart actually!). However, because we are saving the file, we don't know the
colorkey anymore, so we have to set it. We use black (0, 0, 0) as the transparent color,
because both SFont and BFont require it. This means that you cannot use this color as a text color.
*/
SDL_Surface* CreateFontSurface (const char* str_ttf, int i_style, int i_size, Uint8 ui_r, Uint8 ui_g, Uint8 ui_b, bool bAntiAlias, 
								const char*   str_charset = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_'abcdefghijklmnopqrstuvwxyz{|}~") 
{
	WaitCursor wait;

	/* SFont and DFont compatible character set */
	TTF_Font*     ttf_font = NULL ;


	/* Targets for the rendered characters */
	int n = strlen(str_charset);
	SDL_Surface**  p_surf_ch = (SDL_Surface **)malloc((strlen(str_charset) + 1) * sizeof(SDL_Surface *)) ;

	/* All characters gets blitted onto this Surface */
	SDL_Surface*  p_surf_str = NULL ;
	SDL_Rect      sdl_rc_dst ;
	/* Render color */
	SDL_Color sdl_fg = {ui_r, ui_g, ui_b} ;
	SDL_Color sdl_bg = {0, 0, 0} ;
	SDL_Color sdl_mark = {255, 0, 255} ;

	if( sdl_fg.r == sdl_bg.r &&
		sdl_fg.g == sdl_bg.g &&
		sdl_fg.b == sdl_bg.b) 
	{
		sdl_bg.r += 10;
		sdl_bg.g += 10;
		sdl_bg.b += 10;
	}

	if( sdl_fg.r == sdl_mark.r &&
		sdl_fg.g == sdl_mark.g &&
		sdl_fg.b == sdl_mark.b) 
	{
		sdl_mark.r -= 10;
		sdl_mark.b -= 10;
	}
	
	char ch_tmp [2] ;
	int i, j ;
	int i_total_w, i_total_h ;
	
	/* Render the characters to a Surface with SDL_ttf */
	if (TTF_Init() < 0) 
	{
		fprintf(stderr, "Error: couldn't initialize TTF: %s\n", SDL_GetError());

		free(p_surf_ch);
		return NULL ;
	}

	ttf_font = TTF_OpenFont (str_ttf, i_size) ;
	if (ttf_font == NULL) 
	{
		fprintf(stderr, "Error: couldn't load %d pt font from %s: %s\n", i_size, str_ttf, SDL_GetError()) ;

		free(p_surf_ch);
		return NULL ;
	}

	TTF_SetFontStyle (ttf_font, i_style) ;

	//char *n = TTF_FontFaceFamilyName(ttf_font);
	//char *n1 = TTF_FontFaceStyleName(ttf_font);

	/* Creates a 8 bits Software Surface per character */
	for (i = 0; i < strlen (str_charset); ++i) 
	{
		ch_tmp [0] = str_charset [i] ;
		ch_tmp [1] = '\0' ;

		U16 ch = str_charset [i];

		if(bAntiAlias)
			p_surf_ch [i] = TTF_RenderText_Blended(ttf_font, ch_tmp, sdl_fg);
		else			
			p_surf_ch [i] = TTF_RenderText_Solid (ttf_font, ch_tmp, sdl_fg) ;

		
		if (p_surf_ch [i] == NULL) 
		{
			for (j = 0; j < i; ++j) 
			{
				SDL_FreeSurface (p_surf_ch [j]) ;
			}
			TTF_CloseFont (ttf_font) ;
			TTF_Quit () ;
			fprintf(stderr, "Error: couldn't render the charset: %s\n", SDL_GetError());

			free(p_surf_ch);
			return NULL ;
		}
	}

	/* We don't use TTF anymore */
	TTF_CloseFont (ttf_font);
	TTF_Quit ();

	/* Calculate how large the Surface must be */
	i_total_w = 1 ;
	i_total_h = p_surf_ch [0]->h ;

	for (i = 0; i < strlen (str_charset); ++i) 
	{
		i_total_w += p_surf_ch [i]->w + 1 ;
	}
	/* Create the large Surface */
	if(bAntiAlias)
	{
		p_surf_str = SDL_CreateRGBSurface (SDL_SWSURFACE, i_total_w, i_total_h, 32,
				0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000) ;
	}
	else
	{
		p_surf_str = SDL_CreateRGBSurface (SDL_SWSURFACE, i_total_w, i_total_h, 8, 0, 0, 0, 0) ;
		
		/* Copy the palette. (It doesn't matter which character-surface we use, so we just
		take the first one. */
		p_surf_str->format->palette->ncolors = p_surf_ch [0]->format->palette->ncolors ;
		memcpy (p_surf_str->format->palette->colors, p_surf_ch [0]->format->palette->colors, p_surf_str->format->palette->ncolors * sizeof (SDL_Color)) ;
		/* Only the first two indexes of the palette are set by TTF :
		Index 0: Transparent color (255 - Textcolor)
		Index 1: Text color
		
		First overwrite the palette values for index 1 with our fixed color black */
		p_surf_str->format->palette->colors [0].r = sdl_bg.r ;
		p_surf_str->format->palette->colors [0].g = sdl_bg.g ;
		p_surf_str->format->palette->colors [0].b = sdl_bg.b ;
		
		/* Secondly, set a third palette index for the pink color (255, 0, 255) */
		p_surf_str->format->palette->colors [2].r = sdl_mark.r ;
		p_surf_str->format->palette->colors [2].g = sdl_mark.g ;
		p_surf_str->format->palette->colors [2].b = sdl_mark.b ;		
	}
	
	/* Blit characters on the large Surface*/
	sdl_rc_dst.x = sdl_rc_dst.y = 0 ;
	for (i = 0; i < strlen (str_charset); ++i) 
	{
		/* Set pink pixel in left corner before character */
		if(bAntiAlias)
			((Uint32*) p_surf_str->pixels) [sdl_rc_dst.x] = SDL_MapRGBA(p_surf_str->format, sdl_mark.r, sdl_mark.g, sdl_mark.b, 255);
		else
			((Uint8*) p_surf_str->pixels) [sdl_rc_dst.x] = 2;
		
		
		
		++sdl_rc_dst.x ;

		if(bAntiAlias)
		{
			//SDL_BlitSurface only works if amask = 0
			for(int x = 0; x < p_surf_ch [i]->w; x++)
			{
				for(int y = 0; y < p_surf_ch [i]->h; y++)
				{
					U8* src = (U8*) p_surf_ch [i]->pixels 
						+ y * p_surf_ch [i]->pitch
						+ x * p_surf_ch [i]->format->BytesPerPixel;
					
					U8* dst = (U8*) p_surf_str->pixels 
						+ y * p_surf_str->pitch
						+ (x + sdl_rc_dst.x) * p_surf_str->format->BytesPerPixel;
					
					*((U32 *)dst) = *((U32 *)src);
				}
			}
		}
		else
			SDL_BlitSurface (p_surf_ch [i], NULL, p_surf_str, &sdl_rc_dst) ;	

		sdl_rc_dst.x += p_surf_ch [i]->w ;
	}
	
	/* Set last pink pixel */
	if(bAntiAlias)
		((Uint32*) p_surf_str->pixels) [sdl_rc_dst.x] = SDL_MapRGBA(p_surf_str->format, sdl_mark.r, sdl_mark.g, sdl_mark.b, 255);
	else
		((Uint8*) p_surf_str->pixels) [sdl_rc_dst.x] = 2;
	
	/* Cleanup characters */
	for (i = 0; i < strlen (str_charset); ++i) 
	{
		SDL_FreeSurface (p_surf_ch [i]) ;
	}

	free(p_surf_ch);

	/* We made it! */
	return p_surf_str ;  
}


//Create a single unicode char surface
SDL_Surface* CreateUnicodeGlyphSurface (const char* str_ttf, U16 glyphCode, int i_style, int i_size, Uint8 ui_r, Uint8 ui_g, Uint8 ui_b, bool bAntiAlias) 
{
	WaitCursor wait(false); //Pause on turorials only

	/* SFont and DFont compatible character set */
	TTF_Font*     ttf_font = NULL ;

	/* Targets for the rendered characters */
	SDL_Surface*  p_surf_ch = NULL;

	/* All characters gets blitted onto this Surface */
	
	/* Render color */
	SDL_Color sdl_fg = {ui_r, ui_g, ui_b} ;
	SDL_Color sdl_bg = {0, 0, 0} ;
	SDL_Color sdl_mark = {255, 0, 255} ;

	if( sdl_fg.r == sdl_bg.r &&
		sdl_fg.g == sdl_bg.g &&
		sdl_fg.b == sdl_bg.b) 
	{
		sdl_bg.b++;
	}

	if( sdl_fg.r == sdl_mark.r &&
		sdl_fg.g == sdl_mark.g &&
		sdl_fg.b == sdl_mark.b) 
	{
		sdl_mark.g++;
	}
	
	
	/* Render the characters to a Surface with SDL_ttf */
	if (TTF_Init() < 0) 
	{
		fprintf(stderr, "Error: couldn't initialize TTF: %s\n", SDL_GetError());

		free(p_surf_ch);
		return NULL ;
	}

	ttf_font = TTF_OpenFont (str_ttf, i_size) ;
	if (ttf_font == NULL)
	{
		//Try in editor directory
		EditorDirectory editor;
		ttf_font = TTF_OpenFont (str_ttf, i_size);
	}

	if (ttf_font == NULL) 
	{
		fprintf(stderr, "Error: couldn't load %d pt font from %s: %s\n", i_size, str_ttf, SDL_GetError()) ;

		free(p_surf_ch);
		return NULL ;
	}

	TTF_SetFontStyle (ttf_font, i_style) ;

	//char *n = TTF_FontFaceFamilyName(ttf_font);
	//char *n1 = TTF_FontFaceStyleName(ttf_font);

	/* Creates a 8 bits Software Surface per character */
	
	U16 ch_tmp[2];
	ch_tmp [0] = glyphCode;
	ch_tmp [1] = 0;
	
	
	if(bAntiAlias)
		p_surf_ch = TTF_RenderUNICODE_Blended(ttf_font, ch_tmp, sdl_fg);
	else			
		p_surf_ch = TTF_RenderUNICODE_Solid (ttf_font, ch_tmp, sdl_fg) ;
	

	/* We don't use TTF anymore */
	TTF_CloseFont (ttf_font);
	TTF_Quit ();

/*#ifdef _DEBUG
	if(p_surf_ch) SDL_SaveBMP(p_surf_ch, "glyph.bmp");
#endif*/

	return p_surf_ch;  
}


gedString AddTrueTypeFont(const char* str_ttf, int i_style, int i_size, Uint8 ui_r, Uint8 ui_g, Uint8 ui_b, bool bAntiAlias)
{
	WaitCursor wait;
	if(!str_ttf || strlen(str_ttf) < 4) return "";

	char fontName[512];
	sprintf(fontName, "%s%03d%03d%03d%03d%03d%01d", str_ttf, i_style, i_size, ui_r, ui_g, ui_b, (int)bAntiAlias);

	KrFontResource *fontResource = engine->Vault()->GetFontResource(fontName);
	if(fontResource) return fontName;

	//if(i_style & TTF_STYLE_ITALIC) //Maks: crash on TTF_RenderText_Blended with italic (in SDL_ttf 2.0.6)
	//	bAntiAlias = false;

	//Adjust file name
	char *newName = (char *)malloc(strlen(str_ttf) + 1);
	correctDirectoryName(str_ttf, newName);
	
	SDL_Surface *surface = CreateFontSurface (newName, i_style, i_size, ui_r, ui_g, ui_b, bAntiAlias);

	free(newName);
	if(!surface) return "";


	KrPaintInfo info( surface );
	

	if(surface->format->BitsPerPixel < 32)
	{
		surface = ToSupportedSurface(surface);
		info.Init(surface);

		//Set transparent color
		KrPainter painter( surface );
		KrRGBA transRgba;
		painter.BreakPixel( 0, 1, &transRgba );	
		info.SetTranparentColor(transRgba);
	}

/*#ifdef _DEBUG
	SDL_SaveBMP(surface, "font.bmp");
#endif*/

	fontResource = new KrFontResource(	fontName,
													&info,
													' ',
													0,
													KrFontResource::SFONT,
													0 );

	if(fontResource->FontOk())
	{
		engine->Vault()->AddResource(fontResource);		
	}
	else
	{
		delete fontResource;
		fontName[0] = 0;

	}

	SDL_FreeSurface( surface );


	return fontName;
}

static int getActorTextHeight(bool bAction)
{
	if(!Tutorial::IsCompatible(VERSION_MOUSE_ENTER_MOUSE_LEAVE) || bAction)
	{
		//Old height
		return 300;
	}

	return HEIGHT;
}

Actor *ActorText::textCache = NULL;
bool ActorText::bActorTextOpen = false;

ActorText::ActorText(Actor *actor, bool bOnCollision, bool bAction)
: BaseAction("Actor Text", (GameControl::Get()->Width() - WIDTH)/2, 
			 (GameControl::Get()->Height() - (bAction?(getActorTextHeight(bAction)+20):getActorTextHeight(bAction)))/2,
			 WIDTH, (bAction?(getActorTextHeight(bAction)+20):getActorTextHeight(bAction)))
{
	SetModal();
	SetToolTip(TIP_TEXT);

	bActorTextOpen = true;

	eventActor = actor;	
	listActor = NULL;
	bEdit = false;

	bInput = false;
	bDrawBorder = false;
	bDrawBackground = false;
	bGetFocus = false;
	borderColor.all = cursorColor.all = backGroundColor.all = 0;
	
	ic = nc = 0;
	
			
	Text *text;
	Button *button;	
	int y;
	gedString textFont;
	
	text = AddText("Set Text", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);
	
	if(bAction)
	{
		text = AddText("Actor: ", 10, y);
		listActor = AddListPop(text->Right(), text->Top(), 128, 0, LS_ACTOR);
		GameControl::Get()->PopulateActors(listActor);
		listActor->AddText(S_EVENT_ACTOR);

		if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
		{
			listActor->AddText(S_PARENT_ACTOR);
			listActor->AddText(S_CREATOR_ACTOR);
		}

		if(bOnCollision) listActor->AddText(S_COLLIDE_ACTOR);
		listActor->SetItem(S_EVENT_ACTOR);
		y = listActor->Down() + 2;
	}
	
	text = AddText(" Text: ", 10, y);
	actorText = AddEditBox(10, text->Down() + 2, WIDTH-20, 200, true);

	if(!Tutorial::IsCompatible(VERSION_MOUSE_ENTER_MOUSE_LEAVE) || bAction)
	{
		textFont = " Font: ";
	}
	else
	{
		textFont = "      Font: ";
	}

	text = AddText(textFont, 10, actorText->Down()+4);
	listFont = AddListPop(text->Right(), text->Top(), 350, 0, LS_FONT); listFont->SetToolTip(TIP_TEXT_FONT);


	listNewFont = AddListPop(listFont->Right() + 5, listFont->Top(), 80, 0, LS_NEW_FONT, "New Font"); listNewFont->SetToolTip(TIP_TEXT_NEWFONT);
	listFile = AddListPop(actorText->Right() - 64, listNewFont->Top(), 64, 0, LS_FILE, "File"); listFile->SetToolTip(TIP_TEXT_FILE);

	if(Tutorial::IsCompatible(VERSION_MOUSE_ENTER_MOUSE_LEAVE) && !bAction)
	{
		y = DrawHLine(listFile->Down() + 2);

		text = AddText("Text Input: ", 10, y);
		listEnableInput = AddListPop(text->Right(), text->Top(), 38, 0, LS_INPUT_ENABLE); listEnableInput->SetToolTip(TIP_INPUT_ENABLE);

		buttonInputConfig = AddButton("Config", listEnableInput->Right() + 15, listEnableInput->Top(), 0, 0, BT_INPUT_CONFIG); buttonInputConfig->SetToolTip(TIP_INPUT_CONFIG);
		buttonInputConfig->Disable();

		listEnableInput->AddText("Yes");
		listEnableInput->AddText("No");
		listEnableInput->SetItem("No");

		y = listEnableInput->Down() + 2;
	}
	else
	{
		listEnableInput = NULL;
		buttonInputConfig = NULL;
		y = listFile->Down() + 2;
	}
	
	//End dialog
	y = DrawHLine(y);
	button = AddButton((!bAction || Action::getEditAction())?"Ok":"Add", (WIDTH-135)/2, y, 0, 0, BT_OK); if(bAction) button->SetToolTip(TIP_ACTION_ADD);
	button = AddButton("Cancel", button->Right() + 5, button->Top(), 0, 0, BT_CANCEL); button->SetToolTip(TIP_ACTION_CANCEL);
	
	listNewFont->AddText("From image file");
	listNewFont->AddText("True Type");

	listFile->AddText("Save");
	listFile->AddText("Load");

	if(Tutorial::IsOff())	
	{
		CheckSystemFontPath();
	}

	PopulateFonts();
	
	UpdateEdition();	
}

void ActorText::AddFontToCache(const gedString& font, int _ic, int _nc)
{
	if(bActorTextOpen || !engine->Vault()->GetFontResource(font))
	{
		if(!textCache) 
		{
			textCache = new Actor("ged_textCache", NULL, CANVAS, 5, 5);
			textCache->getImage()->SetVisible(false);
		}
		
		textCache->AddText(" ", 0, 0, 0, 0, 0, 0, font, _ic, _nc);	
	}
}

void ActorText::AddFontToList(gedString path)
{
	//Adjust ttf font
	gedString fileName, dir, fontName, desc, s;

	SplitPath(path, fontName, dir);
	s = fontName;
	s.lower();

	if(s.substr(s.length() - (16 + 4), 4) == ".ttf")
	{
		int i_style, i_size, ui_r, ui_g, ui_b, i_antialias;
		fileName = desc = fontName.substr(0, fontName.length() - 16);

		i_style	= atoi(fontName.substr(fontName.length() - 16, 3).c_str());
		i_size	= atoi(fontName.substr(fontName.length() - 13, 3).c_str());
		ui_r	= atoi(fontName.substr(fontName.length() - 10, 3).c_str());
		ui_g	= atoi(fontName.substr(fontName.length() - 7, 3).c_str());
		ui_b	= atoi(fontName.substr(fontName.length() - 4, 3).c_str());
		i_antialias	= atoi(fontName.substr(fontName.length() - 1, 1).c_str());



		char buf[256];
		gedString soft(i_antialias?", soft":""), style;

		switch(i_style)
		{
		case TTF_STYLE_NORMAL:
			style = "Normal";
			break;

		case TTF_STYLE_BOLD:
			style = "Bold";
			break;

		case TTF_STYLE_ITALIC:
			style = "Italic";
			break;

		case TTF_STYLE_UNDERLINE:
			style = "Underline";
			break;
		}

		sprintf(buf, " (%s, %ld%s, RGB: %ld, %ld, %ld)", style.c_str(), i_size, soft.c_str(), ui_r, ui_g, ui_b);

		desc += buf;		
	}
	else
	{
		desc = fontName;
	}

	if(dir.empty() && !fileName.empty() && strlen(fontTTFPath) > 0)
	{	
		//Solve bug "Sometimes don't copy fonts to game data directory"
		bool bAddFontPath = false;

		if(!GameControl::Get()->getGamePath().empty())
		{
			//File exists in the game data directory?
			gedString gameDataPath(GameControl::Get()->getGamePath() + DIR_SEP + "data");
			EditorDirectory dir(gameDataPath);
			gedString file(gameDataPath + DIR_SEP + fileName);

			SDL_RWops* src = ged_SDL_RWFromFile(file.c_str(), "rb");
			if(src)
			{
				//Ok
				SDL_RWclose(src);
			}
			else
			{
				bAddFontPath = true;	
			}
		}
		else
		{
			bAddFontPath = true;
		}

		if(bAddFontPath)
		{
			path = gedString(fontTTFPath) + DIR_SEP + path;
		}
	}

	int i = listFont->AddText(desc);
	listFont->SetItemData(i, new gedString(path));

	listFont->SetItem(desc);
}

void ActorText::PopulateFonts()
{
	MapResNamePointerIterator it(engine->Vault()->GetResourceMap());
	gedString fileName;

	for( it.Begin(); !it.Done(); it.Next() )
	{
		if((*it.Value())->Type() == KYRATAG_FONT)
		{
			KrFontResource *font = ((KrFontResource *)*it.Value());
			fileName = font->ResourceName();

			if(fileName != "maksfont.bmp")
			{
				AddFontToList(fileName);
			}
		}
	}
}

void ActorText::UpdateEdition()
{
	gedString fontName, dir;

	stAction *editAction = Action::getEditAction();
	if(editAction)
	{	
		SplitPath(editAction->data.settext.fontPath, fontName, dir);

		if(listActor) listActor->SetItem(editAction->actorName);	
		actorText->SetText(editAction->data.settext.text, ALL_LINES);

		AddFontToCache(editAction->data.settext.fontPath, 
					   editAction->data.settext.iniASCIIChar, 
					   editAction->data.settext.nChars);

		AddFontToList(editAction->data.settext.fontPath);

		bEdit = true;
	}
	else if(eventActor->getTextActor())
	{
		SplitPath(eventActor->getTextActor()->getFontResource()->ResourceName(), fontName, dir);
		actorText->SetText(eventActor->GetText(), ALL_LINES);
		bEdit = true;

		bInput = eventActor->getTextActor()->IsEditable();
		bDrawBorder = eventActor->getTextActor()->HasBorder();
		bDrawBackground = eventActor->getTextActor()->HasBackground();
		bGetFocus = eventActor->getTextActor()->HasGetFocus();

		borderColor = eventActor->getTextActor()->getBorderColor();
		cursorColor = eventActor->getTextActor()->getCursorColor();
		backGroundColor = eventActor->getTextActor()->getBackGroundColor();

		if(listEnableInput && bInput)
		{
			listEnableInput->SetItem("Yes");
			if(buttonInputConfig) buttonInputConfig->Enable();
		}
	}

	if(fontName.length())
	{ 
		for(int i = 0; i < listFont->Count(); i++)
		{
			gedString *s = (gedString *)listFont->GetItemData(i);
			if(s->substr(s->length() - fontName.length(), fontName.length()) == fontName)
			{
				listFont->SetItem(i);
				break;
			}
		}
	}
}


ActorText::~ActorText()
{
	bActorTextOpen = false;

	for(int i = 0; i < listFont->Count(); i++)
	{
		gedString *s = (gedString *)listFont->GetItemData(i);
		delete s;
	}	
}

void ActorText::SetText(char *str)
{
	actorText->SetText("");
	actorText->SetText(str, ALL_LINES);
}


gedString ActorText::GetText()
{
	return actorText->GetText(ALL_LINES);
}

bool ActorText::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(BaseAction::OnList(list, index, text, listId)) return true;

	switch(listId)
	{
	case LS_NEW_FONT:
		if(index == 0)
		{
			ActorTextBitmap *p = new ActorTextBitmap();
			if(p->Wait() == OK_BUTTON)
			{
				pathArq = p->getPath();
				AddFontToList(pathArq);
				AddFontToCache(pathArq, p->getInicialChar(), p->getNumberOfChars());
			}

			delete p;
		}
		else
		{
			ActorTextTrueType *p = new ActorTextTrueType();
			if(p->Wait() == OK_BUTTON)
			{
				pathArq = p->getPath();	
				AddFontToList(pathArq);
				AddFontToCache(pathArq, 0, 0);
			}

			delete p;
		}
		break;
	case LS_FILE:
		{
			if(text == "Save")
			{
				new LoadSaveScript(SAVE_DIALOG, NULL, NULL, this);
			}
			else
			{
				new LoadSaveScript(LOAD_DIALOG, NULL, NULL, this);
			}
		}
		break;

	case LS_INPUT_ENABLE:
		{
			if(text == "Yes")
			{
				bInput = true;
				if(buttonInputConfig) buttonInputConfig->Enable();
			}
			else
			{
				bInput = false;
				if(buttonInputConfig) buttonInputConfig->Disable();
			}
		}
		break;
	}

	return true;
}

void ActorText::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_OK:
		{
			//Use blank text to remove the text actor

			gedString text(actorText->GetText(ALL_LINES, false));

			if(!bEdit && text.onlyBlank() && !bInput)
			{
				//new PanelInfo("Can't add an empty text");
				delete this;
				return;
			}

			if(listFont->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select a valid font");
				return;
			}
			
			if(bInput && text.onlyBlank() && !bDrawBorder && !bDrawBackground)
			{
				new PanelInfo("You must set an input with a border, a background or some text");
				return;
			}

			pathArq = *(gedString *)listFont->GetItemData(listFont->GetSelectedIndex());
			

			if(listActor)
			{
				if(listActor->GetSelectedIndex() == -1)
				{
					new PanelInfo("Please, select an actor");
					return;
				}

				if(text.length() >= PATH_LIMIT)
				{
					new PanelInfo("Max text lenght: 256");
					return;
				}
			}

			//Get ic and nc
			KrFontResource *fontResource = engine->Vault()->GetFontResource(pathArq);
			ic = fontResource->InitialChar();
			nc = fontResource->NumberOfChars();

			//Charset verification
			gedString invalid;//("\'");

			if(fontResource->getFontType() == KrFontResource::FIXED)
			{
				for(int i = 0; i < text.length(); i++)
				{
					if(text[i] != '\n' && text[i] != '\r' &&
						(text[i] < ic || text[i] >= (ic + nc)))
					{
						if(invalid.find(text[i]) == gedString::npos)
						{
							invalid += text[i];
						}
					}				
				}
			}
			else
			{
				//True Type Font
				for(int i = 0; i < text.length(); i++)
				{
					if(text[i] != '\n' && text[i] != '\r' && !fontResource->GlyphInFont(text[i], false))
					{
						if(invalid.find(text[i]) == gedString::npos)
						{
							invalid += text[i];
						}
					}				
				}
			}
			
			if(invalid.length() > 1)
			{
				//invalid += '\'';
				
				gedString s;
				for(int c = ic; c < (ic + nc); c++)
				{
					s += c;
				}
				
				new PanelInfo(gedString("Valid charset for this font:\n\n") + s +
					gedString("\n\nInvalid chars in your text:\n\n") + invalid);
				return;
			}

			
			
			if(listActor)
			{
				SelectActionType(eventActor, Action::Call(eventActor)->SetSetText(listActor->GetText(), 
					text, pathArq, ic, nc));
			}
			else
			{
				if(text.length() > 0)
				{
					
					Text *t = eventActor->AddText(text, 0, 0, ALIGN_AUTOMATIC, 
						255, 255, 255, 
						pathArq,
						ic, nc,
						
						bInput, bDrawBorder, bDrawBackground, bGetFocus,
						borderColor, cursorColor, backGroundColor);
					
					eventActor->setTextActor(NULL); //Solve the 00 bug when set a big text and set a number after
					eventActor->setTextActor(t);
				}
				else if(!bInput)
				{
					eventActor->setTextActor(NULL); //Remove
				}
				
				delete this;
			}			
			
			
		}
		break;

	case BT_INPUT_CONFIG:
		{
			new ActorTextInput(this);
		}
		break;
		
	case BT_CANCEL:
		{
			delete this;
		}
		break;
	}
}

ActorTextBitmap::ActorTextBitmap()
: Panel("ActorTextBitmap", (GameControl::Get()->Width() - BITMAP_WIDTH)/2, 
			 (GameControl::Get()->Height() - BITMAP_HEIGHT)/2,
			 BITMAP_WIDTH, BITMAP_HEIGHT)
{
	SetModal();
	SetToolTip(TIP_TEXT_BITMAP);
			
	Text *text;
	Button *button;	
	int y;
	
	text = AddText("New Image Font", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);
	
	text = AddText(" File: ", 10, y);
	path = AddListDirectory(text->Right(), text->Top()); path->SetToolTip(TIP_TEXT_BITMAP_FILE);
	
	text = AddText("    Initial font char: ", 10, path->Down() + 5);
	iniChar = AddEditBox(text->Right(), text->Top(), 30);
	//iniChar->SetText(" ");
	
	text = AddText(" Number of font chars: ", 10, iniChar->Down() + 2);
	nChars = AddEditBox(text->Right(), text->Top(), 30); nChars->SetNumeric(1, 1024, 1);
	//nChars->SetText("94");
	
	text = AddText("Note: The image file must\ncontain all image characters\nin one line (ASCII sequence)", path->Right() + 18, path->Top());
	text = AddText("Adjust 'Number of font chars'\nuntil being shown a single\ncharacter on the sample", path->Right() + 18, nChars->Top());


	//Sample
	animationSample = new Actor("AnimationSample", this);
	animationSample->AddAnimation(ACTOR_GED_ICON, "maksiconActor.gif", 1, 1);
	animationSample->getImage()->SetVisible(false);
	
	//Fill background
	KrRGBA black;
	black.c.red		= 0;
	black.c.green	= 0;
	black.c.blue	= 0;
	black.c.alpha	= 255;

	
	KrRGBA *pixels = getCanvasResource()->Pixels();
	for(int i = SAMPLE_X; i < SAMPLE_X + SAMPLE_SIDE; i++)
	{
		for(int j = SAMPLE_Y; j < SAMPLE_Y + SAMPLE_SIDE; j++)
		{
			pixels[ j*Width() + i ] = black;
		}
	}

	charSample = AddEditBox(178, nChars->Down() + SAMPLE_SIDE - 13, 1); charSample->SetNumeric(0, 100, 1);
	
	text = AddText("Character sample: ", 20, nChars->Down() + 2);
	
	//End dialog
	y = DrawHLine(nChars->Down() + 6 + SAMPLE_SIDE);
	button = AddButton("Ok", 120, y, 0, 0, OK_BUTTON);
	button = AddButton("Cancel", button->Right() + 5, button->Top(), 0, 0, CANCEL_BUTTON);
	
	path->AddFilter("tga");
	path->AddFilter("bmp");
	path->AddFilter("pnm");
	path->AddFilter("xpm");
	path->AddFilter("xcf");
	path->AddFilter("pcx");
	path->AddFilter("gif");
	path->AddFilter("jpg");
	path->AddFilter("jpeg");
	path->AddFilter("jpe");
	path->AddFilter("jfif");
	path->AddFilter("tif");
	path->AddFilter("tiff");
	path->AddFilter("iff");
	path->AddFilter("lbm");
	path->AddFilter("png");	
	
	if(!Tutorial::IsOff())
	{
		chdir(Tutorial::Get()->getTutorialPath());
	}
	else
	{
		if(!bInitBitmapPath)
		{
			fontBitmapPath[0] = 0;
			bInitBitmapPath = true;

			if(!Config::Get()->getPathImageFont().empty())
			{
				chdir(Config::Get()->getPathImageFont().c_str());
			}
		}
		else chdir(fontBitmapPath);
	}

	
}

void ActorTextBitmap::ResetPath()
{
	bInitBitmapPath = false;
}

ActorTextBitmap::~ActorTextBitmap()
{
	delete animationSample;
}

gedString ActorTextBitmap::getPath()
{
	return path->GetFullPath();
}

bool ActorTextBitmap::OnList(ListPop *list, int index, gedString &text, int listId)
{
	UpdateSample(true);	
	
	return true;
}

void ActorTextBitmap::OnEditChar(EditBox *edit, SDLKey key, int line)
{
	if(edit == nChars)
	{
		UpdateSample(false);
	}
	else if(edit == charSample)
	{
		if(animationSample)
		{
			int f = atol(charSample->GetText().c_str());
			if(f >= animationSample->getScriptVars()->nframes)
			{
				f = animationSample->getScriptVars()->nframes - 1;
			}

			animationSample->SetFrame(f);
		}
	}
}

void ActorTextBitmap::UpdateSample(bool bChangeFile)
{
	WaitCursor wait;	
	
	int nv = 1, nh = atol(nChars->GetText().c_str());
	gedString fullPath(path->GetFullPath());
	
	if(bChangeFile)
	{
		nh = nv = -1;
	}

	if(!animationSample) return;
	
	animationSample->RemoveAnimation("SAMPLE");
	if(animationSample->AddAnimation("SAMPLE", fullPath, nh, nv, -1, -1, 30))
	{
		animationSample->getImage()->SetVisible(true);
		animationSample->SetAnimation("SAMPLE");
		animationSample->SetActorScale(1.0);
		animationSample->getImage()->CalcTransform();

		int maxWidth = 0, maxHeight = 0;

		for(int i = 0; i < animationSample->getScriptVars()->nframes; i++)
		{
			animationSample->SetFrame(i);
			
			maxWidth = max(maxWidth, animationSample->Width());
			maxHeight = max(maxHeight, animationSample->Height());
		}

				
		double scale = SAMPLE_SIDE/(double)max(maxWidth, maxHeight);
		//if(scale < 1.0)
		{
			animationSample->SetActorScale(scale);
		}

		animationSample->SetPos(SAMPLE_X + SAMPLE_SIDE/2, SAMPLE_Y + SAMPLE_SIDE/2);

		if(nh == -1)
		{
			nChars->SetText(animationSample->getSprite()->GetAction()->getnFramesH());		
		}

		charSample->SetNumeric(0, animationSample->getScriptVars()->nframes, 1);
	}
	else
	{
		animationSample->getImage()->SetVisible(false);
	}

	getImage()->Invalidate();
}

void ActorTextBitmap::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case OK_BUTTON:
		{
			gedString pathArq = path->GetFullPath();
						
			if(iniChar->GetText().size() <= 0)
			{
				new PanelInfo("Please, enter valid initial font char");
				return;
			}
			
			nc = atol(nChars->GetText().c_str());
			if(nChars->GetText().size() <= 0 || nc < 1 || nc > 1024)
			{
				new PanelInfo("Please, enter valid number of font chars");
				return;
			}
			
			ic = iniChar->GetText().c_str()[0];
			
			SDL_RWops *src = ged_SDL_RWFromFile(pathArq.c_str(), "rb");
			if(src)
			{
				SDL_RWclose(src);
				
				SDL_Surface* surface = ged_IMG_Load(pathArq.c_str());
				if(!surface)
				{
					new PanelInfo("This format is not supported or file is corrupted\nPlease, select other file");
					return;
				}
				
				//Ok
				SDL_FreeSurface( surface );
			}
			else
			{
				//Erro	
				new PanelInfo("Please, select a file");
				return;
			}		

			
			
			//Store current path
			ged_getcwd(fontBitmapPath, PATH_LIMIT);
			Config::Get()->setPathImageFont(fontBitmapPath);
			
			result = buttonId;
		}
		break;
		
	case CANCEL_BUTTON:
		{
			result = buttonId;
		}
		break;
	}
}

ActorTextTrueType::ActorTextTrueType()
: Panel("ActorTextTrueType", (GameControl::Get()->Width() - TTF_WIDTH)/2, 
			 (GameControl::Get()->Height() - TTF_HEIGHT)/2,
			 TTF_WIDTH, TTF_HEIGHT)
{
	SetModal();
	SetToolTip(TIP_TEXT_TTF);
			
	Text *text;
	Button *button;	
	
	int y;
	
	text = AddText("New True Type Font", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);
	
	text = AddText(" File: ", 10, y);
	path = AddListDirectory(text->Right(), text->Top(), 200); path->SetToolTip(TIP_TEXT_TTF_FILE);

	//Can't select files in Windows Font directory with Windows file dialog
	path->setCanUseSystemFileDialog(false);
	
		
	text = AddText(" Size: ", 10, path->Down() + 10);
	editSize = AddEditBox(text->Right(), text->Top(), 30); editSize->SetNumeric(8, 160, 1); //maks: loop when use 164 with arial font

	text = AddText("Color: ", 10, editSize->Down() + 2);
	colorSample = new ColorSample(this, text->Right(), text->Top(), 32); colorSample->SetToolTip(TIP_TEXT_TTF_COLOR);

	text = AddText("Style: ", 10, colorSample->Down() + 2);
	listStyle = AddListPop(text->Right(), text->Top(), 128, 0, LS_STYLE); listStyle->SetToolTip(TIP_TEXT_TTF_STYLE);

	text = AddText(" Soft: ", 10, listStyle->Down() + 2);
	listSoft = AddListPop(text->Right(), text->Top(), 32, 0, LS_SOFT); listSoft->SetToolTip(TIP_TEXT_TTF_SOFT);
		
		
	//End dialog
	y = DrawHLine(listSoft->Down() + 2);
	button = AddButton("Ok", 120, y, 0, 0, OK_BUTTON);
	button = AddButton("Cancel", button->Right() + 5, button->Top(), 0, 0, CANCEL_BUTTON);
	
	
	path->AddFilter("ttf");	

	size = 32;
	editSize->SetText(size);
	

	listStyle->AddText("Normal");
	listStyle->AddText("Bold");
	listStyle->AddText("Italic");
	listStyle->AddText("Underline");
	listStyle->SetText("Normal");
	style = TTF_STYLE_NORMAL;

	listSoft->AddText("Yes");
	listSoft->AddText("No");
	listSoft->SetText("No");
	bSoft = false;
	
	if(!Tutorial::IsOff())
	{
		chdir(Tutorial::Get()->getTutorialPath());
	}
	else
	{
		CheckSystemFontPath();
	}

	textBox = NULL;
}

void ActorTextTrueType::ResetPath()
{
	bInitTTFPath = false;
}

ActorTextTrueType::~ActorTextTrueType()
{
	delete colorSample;
	delete textBox;

#ifdef WIN32
	//Restore old path
	char path[PATH_LIMIT];
	ged_getcwd(path, PATH_LIMIT);
	if(strcmp(winTTFPath, path) == 0)
	{
		chdir(oldUserPath);
	}
#endif
}

void ActorTextTrueType::OnEditChar(EditBox *edit, SDLKey key, int line)
{
	size = atol(edit->GetText().c_str());
	if(size < 0) size = 0;

	UpdateSample();
}

bool ActorTextTrueType::OnList(ListPop *list, int index, gedString &text, int listId)
{
	switch(listId)
	{
	case LS_STYLE:
		if(text == "Normal") style = TTF_STYLE_NORMAL;
		else if(text == "Bold") style = TTF_STYLE_BOLD;
		else if(text == "Italic") 
		{
			style = TTF_STYLE_ITALIC;
			/*listSoft->SetText("No");
			bSoft = false;*/
		}
		else if(text == "Underline") style = TTF_STYLE_UNDERLINE;
		break;

	case LS_SOFT:
		if(text == "Yes" /*&& style != TTF_STYLE_ITALIC*/) bSoft = true;
		else
		{
			//listSoft->SetText("No");
			bSoft = false;
		}
		break;
	}

	UpdateSample();

	return true;
}

void ActorTextTrueType::OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message)
{
	if(messageId == COLOR_CHANGED)
	{
		UpdateSample();
	}
}

void ActorTextTrueType::UpdateSample()
{
	if(size < 8 || size > 164) return;

	pathArq = AddTrueTypeFont(path->GetFullPath().c_str(), style, size, 
					colorSample->getRed(),
					colorSample->getGreen(),
					colorSample->getBlue(),
					bSoft);

	if(!pathArq.empty())
	{
		delete textBox;
		textBox = new Text("Abc", listStyle->Right() + 48, CENTER_TEXT/*colorSample->Top()*/, 0, this, 255, 255, 255, pathArq, 0, 0);
	}
}

void ActorTextTrueType::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case OK_BUTTON:
		{
			gedString pathArq = path->GetFullPath();
			
			
			pathArq = AddTrueTypeFont(path->GetFullPath().c_str(), style, size, 
					colorSample->getRed(),
					colorSample->getGreen(),
					colorSample->getBlue(),
					bSoft);

			if(pathArq.empty())
			{
				new PanelInfo("This font format is not supported or file is corrupted\nPlease, select other file");
				return;
			}
			
			
			//Store current path
			ged_getcwd(fontTTFPath, PATH_LIMIT);

			
			result = buttonId;			
			
			
		}
		break;
		
	case CANCEL_BUTTON:
		{
			result = buttonId;
		}
		break;
	}
}



ActorTextInput::ActorTextInput(ActorText *_actorText)
: Panel("ActorTextInput", (GameControl::Get()->Width() - INPUT_WIDTH)/2, 
			 (GameControl::Get()->Height() - INPUT_HEIGHT)/2,
			 INPUT_WIDTH, INPUT_HEIGHT)
{
	actorText = _actorText;

	SetModal();
	SetToolTip(TIP_TEXT_CONFIG_INPUT);
			
	Text *text;
	Button *button;	
	
	int y;
	
	text = AddText("Text Input", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 3);

	text = AddText("Get input focus: ", 10, y);
	listGetFocus = AddListPop(text->Right(), text->Top(), 35, 0, LS_INPUT_FOCUS); listGetFocus->SetToolTip(TIP_TEXT_CONFIG_INPUT_FOCUS);

	y = DrawHLine(listGetFocus->Down() + 5);


	text = AddText("    Draw border: ", 10, y);
	listDrawBorder = AddListPop(text->Right(), text->Top(), 35, 0, LS_BORDER); listDrawBorder->SetToolTip(TIP_TEXT_CONFIG_INPUT_BORDER);

	text = AddText("Color: ", listDrawBorder->Right() + 10, listDrawBorder->Top() + 2);
	borderColorSample = new ColorSample(this, text->Right(), text->Top(), 32); borderColorSample->SetToolTip(TIP_TEXT_CONFIG_INPUT_BORDER_COLOR);

	
	text = AddText("Draw background: ", 10, listDrawBorder->Down() + 10);
	listDrawBackground = AddListPop(text->Right(), text->Top(), 35, 0, LS_BACKGROUND); listDrawBackground->SetToolTip(TIP_TEXT_CONFIG_INPUT_BACKGROUND);

	text = AddText("Color: ", listDrawBackground->Right() + 10, listDrawBackground->Top() + 2);
	backgroundColorSample = new ColorSample(this, text->Right(), text->Top(), 32); backgroundColorSample->SetToolTip(TIP_TEXT_CONFIG_INPUT_BACKGROUND_COLOR);


	text = AddText("   Cursor color: ", 10, listDrawBackground->Down() + 10);
	cursorColorSample = new ColorSample(this, text->Right(), text->Top(), 32); cursorColorSample->SetToolTip(TIP_TEXT_CONFIG_INPUT_CURSOR_COLOR);

	
		
		
	//End dialog
	y = DrawHLine(cursorColorSample->Down() + 8);
	button = AddButton("Ok", 120, y, 0, 0, OK_BUTTON);
	button = AddButton("Cancel", button->Right() + 5, button->Top(), 0, 0, CANCEL_BUTTON);
	

	listDrawBorder->AddText("Yes");
	listDrawBorder->AddText("No");

	listDrawBackground->AddText("Yes");
	listDrawBackground->AddText("No");

	listGetFocus->AddText("Yes");
	listGetFocus->AddText("No");

	//Update data

	if(actorText->HasBorder()) listDrawBorder->SetItem("Yes");	
	else listDrawBorder->SetItem("No");

	if(actorText->HasBackground()) listDrawBackground->SetItem("Yes");	
	else listDrawBackground->SetItem("No");

	if(actorText->HasGetFocus()) listGetFocus->SetItem("Yes");	
	else listGetFocus->SetItem("No");

	borderColorSample->setColor(actorText->getBorderColor());
	backgroundColorSample->setColor(actorText->getBackGroundColor());
	cursorColorSample->setColor(actorText->getCursorColor());
}


ActorTextInput::~ActorTextInput()
{
	delete borderColorSample;
	delete cursorColorSample;
	delete backgroundColorSample;
}


bool ActorTextInput::OnList(ListPop *list, int index, gedString &text, int listId)
{
	return true;
}

void ActorTextInput::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case OK_BUTTON:
		{
			if(listDrawBorder->GetText() == "Yes") actorText->setBorder(true);
			else actorText->setBorder(false);

			if(listDrawBackground->GetText() == "Yes") actorText->setBackground(true);
			else actorText->setBackground(false);

			if(listGetFocus->GetText() == "Yes") actorText->setGetFocus(true);
			else actorText->setGetFocus(false);

			actorText->setBorderColor(borderColorSample->getColor());
			actorText->setCursorColor(cursorColorSample->getColor());
			actorText->setBackGroundColor(backgroundColorSample->getColor());

			delete this;
		}
		break;
		
	case CANCEL_BUTTON:
		{
			delete this;
		}
		break;
	}
}

#endif //STAND_ALONE_GAME






