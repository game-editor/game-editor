// FastSprite.cpp: implementation of the FastSprite class.
//
//////////////////////////////////////////////////////////////////////

#define _KYRA_

#ifdef _KYRA_
#include "SDL.h"
#include "engine.h"
#include "../../gameEngine/dlmalloc.h"
extern KrEngine *engine;
#else
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#endif

#include "FastSprite.h"
//#include "../../gameEngine/Compact/ucl.h"





/*
int Compile_Sprite16_ASM (const char *spritename, SDL_Surface *surface, int width, int height, int screenWidth) 
{
	//Compiled 16bit sprite without clipping
	
	// OK - pass this function a string for a filename, a pointer to a single
	// frame of a sprite, and the sprite's width and height.  The function will
	// then produce a file (of the sprite's name) containing ASM code which will
	// compile to the fastest possible way to blit a bitmap PERIOD (without
	// specialized hardware)!    ...in Watcom/DOS4GW.
	
	int x, y, offset, offset1;
	FILE *outfile;
	unsigned char *pixel1, *pixel2;
	char *filename;
	
	filename = strDup(spritename);
	strcat(filename, ".c");
	
	if((outfile = fopen(filename,"wt")) == NULL)
	{
		printf("Could not open output file %s\n", filename);
		return(1);
	};
	
	fprintf(outfile,"\n\nvoid %s(void *screen, int x, int y)\n{\n",spritename); //eax, ebx, edi
	fprintf(outfile,"__asm {\n");

	//if screenWidth = 320 (101000000) => addr = screen + (y << 8) + (y << 6) + x
	fprintf(outfile,"        push    eax\n");
	fprintf(outfile,"        push    ebx\n");
	fprintf(outfile,"        push    ecx\n");
	fprintf(outfile,"        push    edi\n");

	fprintf(outfile,"        mov edi, screen\n");
	fprintf(outfile,"        mov eax, x\n");
	fprintf(outfile,"        mov ebx, y\n");
	

	for(int n = 0; (1 << n) < screenWidth; n++)
	{
		if(screenWidth & (1 << n))
		{
			fprintf(outfile,"        mov ecx, ebx\n");
			fprintf(outfile,"        shl ecx, 0x%0x\n", n);
			fprintf(outfile,"        add edi, ecx\n");
		}
	}

	fprintf(outfile,"        add     edi,eax\n");
	fprintf(outfile,"        mov     ebx,10000\n");
	fprintf(outfile,"        clz\n");
	
	//int overHead = 0;
	for (y = 0; y < height; y++) 
	{
		offset = y * screenWidth * 2;
		
		x = 0;
		while (x < width) 
		{
			U16* bits = (U16*)( (U8*) surface->pixels 
					 + y * surface->pitch
					 + x * surface->format->BytesPerPixel );
			
			if (*bits != 0) 
			{
				// Pixel is non-black; save it and try to get more:  
				pixel1 = (unsigned char *)bits;
				offset1 = offset;
				
				//bitmap_offset++;
				offset += 2;
				x++;

				bits = (U16*)( (U8*) surface->pixels 
					 + y * surface->pitch
					 + x * surface->format->BytesPerPixel );
				
				if (*bits != 0 && (x < width)) 
				{
					// 2nd pixel is non-black; save it and try to get more:  
					pixel2 = (unsigned char *)bits;
					
					offset += 2;
					x++;


					
					// BEST CASE!  Write a double-word:					
					fprintf(outfile,"        mov     dword ptr [edi+0x%x],0x%02x%02x%02x%02x\n",
						offset1, pixel2[1], pixel2[0], pixel1[1], pixel1[0]);



					fprintf(outfile,"		sub ebx, 2\n");
					fprintf(outfile,"		jz exit\n");
				}
				else 
				{
					// Write a word for first two pixels:
					fprintf(outfile,"        mov     word ptr [edi+0x%x],0x%02x%02x\n", 
						offset1, pixel1[1], pixel1[0]);

					fprintf(outfile,"		dec ebx\n");
					fprintf(outfile,"		jz exit\n");
				}

			}
			else 
			{
				// Encountered black pixel; skip it:
				offset += 2;
				x++;
			}
			
		}  // end while

		fprintf(outfile,"\n");
		
	}  // end for
	

	fprintf(outfile,"exit:\n");

	fprintf(outfile,"        pop     edi\n");
	fprintf(outfile,"        pop     ecx\n");
	fprintf(outfile,"        pop     ebx\n");
	fprintf(outfile,"        pop     eax\n");

	fprintf(outfile,"\n}\n}\n");
	
	fclose(outfile);
	
	return(0);
	
}

*/


ArmSpriteCompiler16::ArmSpriteCompiler16()
{
	code = NULL;
	currentAddress = nAlloc = n = 0;
}

ArmSpriteCompiler16::~ArmSpriteCompiler16()
{
	Clear();
}

void ArmSpriteCompiler16::Clear()
{
	if(code) free(code);
	code = NULL;
	currentAddress = nAlloc = n = 0;
}

void ArmSpriteCompiler16::Compile(unsigned short *src, int w, int h, int scrW, unsigned short trasparentColor)
{
	int xrun = 0, yrun = 0, x, y;
	int mod = scrW - w;
	unsigned short pix;
	unsigned long pcol = 0xffffffff;
	
	Clear();
	
	screenWidth = scrW;
	
	for (y = 0; y < h; y++) 
	{
		for (x = 0; x < w; x++) 
		{
			pix = *src++;
			
			if (pix == trasparentColor) 
			{
				xrun++;
			} 
			else 
			{
				xrun += (mod * yrun);
				
				if (xrun > 0) 
				{
					ADD_R0_R0(2*xrun);
					
					xrun = 0;
					yrun = 0;
				}
				
				
				if (pix != pcol) 
				{
					// mov rd,#pix with shift
					MOV(pix);
					pcol = pix;
				}
				
				// strh rd,[rn],#2
				STRH_R3_R0();
			}
			
		}
		
		yrun++;
	}
	
	RTS();
}

void ArmSpriteCompiler16::Draw(unsigned short *screen, int x, int y)
{
	F_ARM f = (F_ARM)code;
	f(screen + x + screenWidth * y);
}


void ArmSpriteCompiler16::MOV(unsigned short val)
{	
	//Move the data to r3		
	
	if(val < 256)
	{
		emit32(0xe3a03000 | val);
	}
	else
	{
		int val8, rot, _or;
		decomposit(val, val8, rot, _or);
		
		emit32(0xe3a03c00 | val8);	//mov       r3, val8, 24
		emit32(0xe3833002 | _or);	//orr       r3, r3, or	
	}
}

void ArmSpriteCompiler16::STRH_R3_R0()
{
	//Store the data and update the dst pointer (2 bytes)
	emit32(0xe0c030b2);			//strh      r3, [r0], #2 
	
	currentAddress += 2;
}


void ArmSpriteCompiler16::ADD_R0_R0(unsigned short val)
{		
	//Uptate the dst pointer
	if(!val)
	{
		return;
	}
	else if(val < 256)
	{
		emit32(0xe2800000 | val);	 //add       r0, r0, val
	}
	else
	{
		int val8, rot, _or;
		decomposit(val, val8, rot, _or);
		
		emit32(0xe2800c00 | val8);	//add       r0, r0, val, 24
		emit32(0xe3800000 | _or);	//orr       r0, r0, or	
	}
	
	currentAddress += val;
}

void ArmSpriteCompiler16::RTS()
{
	//Return to the caller
	emit32(0xe1a0f00e);				//mov pc,lr
}

unsigned long ArmSpriteCompiler16::ror(unsigned long val, char n) 
{
	for(int i = 0; i < n; i++)
	{
		int lowbit;
		
		if(val & 1) // Check the low bit
			lowbit = 1;
		else
			lowbit = 0;
		
		val >>= 1; // Right shift by one position
		
		// Rotate the low bit onto the top:
		val |= (lowbit << 31);
	}
	
	return val;
} 

void ArmSpriteCompiler16::decomposit(int val, int &val8, int &rot, int &_or)
{
	//val = [0, 65535]
	//mov       r3, val8, rot
	//orr       r3, r3, or //r3 = val
	
	//opcode for rot = 24:
	//e3a03c00 | val8 //mov       r3, val8, 24
	//e3833002 | or   //orr       r3, r3, or
	
	
	val8 = val >> 8 & 0x000000FF;
	_or = val & 0x000000FF;
	rot = 24;
	
	/*int val2 = ror(val8, rot) | _or;
	
	  if(val2 != val)
	  {
	  int erro;
	  erro = 1;
}*/
}

void ArmSpriteCompiler16::emit32(unsigned long op)
{
	ResizePower2(n+1);
	
	code[n++] = op;
}

void ArmSpriteCompiler16::ResizePower2( int _size )
{
	int newSize = 1;
	
	while ( newSize < _size )
		newSize <<= 1;
	
	if((newSize > 0 && newSize > nAlloc) || !code) 
	{ 
		//Must set the execution flag in the windows
		nAlloc = newSize;
		code = (unsigned long *)realloc(code, nAlloc*sizeof(unsigned long));
	}
}



FastSprite::FastSprite()
{
	spriteData = NULL;
	spriteLine = NULL;
	//spriteOffSet = NULL;

	screenWidth = -1;
}

FastSprite::~FastSprite()
{
	if(spriteData) free(spriteData);
	if(spriteLine) free(spriteLine);
	//if(spriteOffSet) free(spriteOffSet);
}

#ifdef _KYRA_
SDL_Surface * FastSprite::Convert16(KrRle *rle)
{
	SDL_Surface *surface = SDL_CreateRGBSurface(
		SDL_SWSURFACE, rle->Width(), rle->Height(), 16,
        0x0000f800, 0x000007e0, 0x0000001f, 0);


	KrPaintInfo info(surface);
	KrMatrix2 xForm;
	KrColorTransform cForm;
	KrRect clipping;
	
	xForm.Set(rle->Hotspot().x, rle->Hotspot().y);
	cForm.SetIdentity();
	clipping.Set(0, 0, rle->Width() - 1, rle->Height() - 1);
	


#ifdef __iPhone__ //AKR
	Uint32 key;
	SDL_SetColorKey(surface,1,SDL_MapRGB(surface->format, rle->getColorKey().c.red, rle->getColorKey().c.green, rle->getColorKey().c.blue));
	SDL_GetColorKey(surface, &key);
	SDL_FillRect( surface, NULL, key );
#else
	surface->format->colorkey = SDL_MapRGB(surface->format, rle->getColorKey().c.red, rle->getColorKey().c.green, rle->getColorKey().c.blue);
	SDL_FillRect( surface, NULL, surface->format->colorkey);
#endif

	
	rle->Draw(&info, xForm, cForm, clipping, 0, false);

	return surface;
}

void FastSprite::Create(KrRle *rle, int _screenWidth)
{
	//Fast blit 16bit sprite with clipping
	
	int x, y, offset, bufLen, startOffset;
	int offsetIndex = 0, dataIndex = 0;	

	SDL_Surface *surface = Convert16(rle);

	unsigned short *segBuf = new unsigned short[surface->w];

	width = surface->w;
	height = surface->h;
	screenWidth = _screenWidth;
	
		
	if(spriteData) free(spriteData);
	if(spriteLine) free(spriteLine);
	//if(spriteOffSet) free(spriteOffSet);

	spriteLine = (long *)malloc((surface->h + 1)*sizeof(long));
	//spriteOffSet = (unsigned long *)malloc(2*surface->w*surface->h*sizeof(unsigned long)); //estimated
	spriteData = (unsigned short *)malloc(2*surface->w*surface->h*sizeof(unsigned short)); //estimated

	for(int i = 0; i < surface->h; i++) spriteLine[i] = -1;
	
	

	for (y = 0; y < surface->h; y++) 
	{
		offset = 0;
		
		x = 0;
		bufLen = 0;

		while (x < surface->w) 
		{
			U16* bits = (U16*)( (U8*) surface->pixels 
					 + y * surface->pitch
					 + x * surface->format->BytesPerPixel );
#ifdef __iPhone__
			uint32_t k;
			SDL_GetColorKey(surface, &k);

			if(*bits != k) 
#else

			if(*bits != surface->format->colorkey) 
#endif
			{
				if(!bufLen) 
				{
					startOffset = offset;

					if(spriteLine[y] == -1) 
						spriteLine[y] = dataIndex;
				}

				segBuf[bufLen] = *bits;
				bufLen++;

				offset++;
				x++;
			}
			else 
			{
				// Encountered black pixel; skip it:

				if(bufLen)
				{
					CopyToData(spriteData, segBuf, dataIndex, bufLen, startOffset);
					bufLen = 0;
				}

				offset++;
				x++;
			}
			
		}  // end while
		
		if(bufLen)
		{
			CopyToData(spriteData, segBuf, dataIndex, bufLen, startOffset);			
			bufLen = 0;
		}
		
		
	}  // end for

	spriteLine[y] = dataIndex;

	SDL_FreeSurface(surface);

	//Adjust buffers
	//spriteOffSet = (unsigned long *)realloc(spriteOffSet, offsetIndex*sizeof(unsigned long)); //estimated
	spriteData = (unsigned short *)realloc(spriteData, dataIndex*sizeof(unsigned short)); //estimated

	delete [] segBuf;
}

void FastSprite::Draw(KrRle *rle, int x, int y, const KrRect &clip)
{
	//Fast blit 16bit sprite with clipping
	//Segment Format: len, offset, data 
	//Clip is a intersection between sprite bounds and the clip rectangle

	if(!spriteData) 
	{
		Create(rle, engine->FullScreenBounds().Width());
		if(!spriteData) return;
	}	

	int xMin = clip.xmin - x, yMin = clip.ymin - y;
	int sx = x + xMin;

	if(sx < 0) sx = 0;

	int clipWidth = clip.Width();
	unsigned short *start = (unsigned short *)((unsigned short *)(engine->Surface()->pixels) + sx);
	register unsigned short *sprite = spriteData + spriteLine[yMin], *spriteFin = spriteData + spriteLine[yMin + clip.Height()];
	int i0 = clip.ymin, dx, w;
	long *nextLineIndex = spriteLine + yMin + 1;
	unsigned short *nextLine = spriteData + *nextLineIndex;

	register unsigned short *lineOffset = (unsigned short *)(start + i0*screenWidth), *addr;
	register int len = *(sprite++), da;
	int lenghtDraw = 0;
	
	
	while(sprite < spriteFin)
	{		
		addr = lineOffset + *(sprite++);
		da = addr - lineOffset;
	

		lenghtDraw = len;
		dx = 0;

		//////////////////////////////////////////////
		//Left clip		
		if(xMin)
		{
			if(da >  xMin)
			{
				//Segment starts after the clip line
				addr -= xMin;				
			}
			else if(da <= xMin)
			{
				//Clip line intersect the segment or
				//Line ends before the clip line				
				dx = xMin - da;
				lenghtDraw -= dx;
				addr = lineOffset;
			}

			da = addr - lineOffset;
		}

		//Right clip
		w = da + lenghtDraw;
		if(w > clipWidth) 
		{			
			w = clipWidth - da;
		}
		
		//Draw
		if(lenghtDraw > 0 && w > 0) 
		{
			memcpy(addr, sprite+dx, ((lenghtDraw < w)?lenghtDraw:w) << 1);
		}

		sprite += len;	
		len = *(sprite++);

		if(sprite > nextLine) 
		{			
			nextLine = spriteData + *(++nextLineIndex);			
			lineOffset = (unsigned short *)(start + (++i0)*screenWidth);			
		}
	}
}
#endif

void FastSprite::CopyToData(unsigned short *spriteData, unsigned short *segBuf, int &dataIndex, int bufLen, int startOffset)
{
	////////////////////
	//
	/*{
		static int init = 0;
		if(!init)
		{
			ucl_init();
			init = 1;
		}

		unsigned int lenDst = 0;
		unsigned char *dst = new unsigned char[bufLen*2 + bufLen*2 / 8 + 256];
		bool bOk = ucl_nrv2b_99_compress((const unsigned char *)segBuf, bufLen*2, (unsigned char *)dst, (unsigned int *)&lenDst, NULL, 10, NULL, NULL) == UCL_E_OK;				
		delete dst;
	}*/
	////////////////////


	spriteData[dataIndex++] = bufLen;
	spriteData[dataIndex++] = startOffset;
	//spriteOffSet[offsetIndex++] = startOffset;
	
	for(int i = 0; i < bufLen; i++)
	{
		spriteData[dataIndex++] = segBuf[i];						
	}
}

void FastSprite::Create(unsigned short *src, int w, int h, int scrW, unsigned short trasparentColor)
{
	//Fast blit 16bit sprite with clipping
	
	int x, y, offset, bufLen, startOffset;
	int offsetIndex = 0, dataIndex = 0;	


	unsigned short *segBuf = new unsigned short[w];

	width = w;
	height = h;
	screenWidth = scrW;
	
		
	if(spriteData) free(spriteData);
	if(spriteLine) free(spriteLine);
	//if(spriteOffSet) free(spriteOffSet);

	spriteLine = (long *)malloc((h + 1)*sizeof(long));
	//spriteOffSet = (unsigned long *)malloc(2*surface->w*surface->h*sizeof(unsigned long)); //estimated
	spriteData = (unsigned short *)malloc(2*w*h*sizeof(unsigned short)); //estimated

	for(int i = 0; i < h; i++) spriteLine[i] = -1;
	
	

	for (y = 0; y < h; y++) 
	{
		offset = 0;
		
		x = 0;
		bufLen = 0;

		while (x < w) 
		{
			unsigned short* bits = (unsigned short*)( (unsigned char*) src + y*w*2 + x*2 );
			
			if(*bits != trasparentColor) 
			{
				if(!bufLen) 
				{
					startOffset = offset;

					if(spriteLine[y] == -1) 
						spriteLine[y] = dataIndex;
				}

				segBuf[bufLen] = *bits;
				bufLen++;

				offset++;
				x++;
			}
			else 
			{
				// Encountered black pixel; skip it:

				if(bufLen)
				{
					CopyToData(spriteData, segBuf, dataIndex, bufLen, startOffset);
					bufLen = 0;
				}

				offset++;
				x++;
			}
			
		}  // end while
		
		if(bufLen)
		{
			CopyToData(spriteData, segBuf, dataIndex, bufLen, startOffset);			
			bufLen = 0;
		}
		
		
	}  // end for

	spriteLine[y] = dataIndex;


	//Adjust buffers
	//spriteOffSet = (unsigned long *)realloc(spriteOffSet, offsetIndex*sizeof(unsigned long)); //estimated
	spriteData = (unsigned short *)realloc(spriteData, dataIndex*sizeof(unsigned short)); //estimated

	delete [] segBuf;
}

void FastSprite::Draw(unsigned short *screen, int x, int y, const KrRect &clip)
{
	//Fast blit 16bit sprite with clipping
	//Segment Format: len, offset, data 
	//Clip is a intersection between sprite bounds and the clip rectangle

	if(!spriteData) return;

	int xMin = clip.xmin - x, yMin = clip.ymin - y;
	int sx = x + xMin;

	if(sx < 0) sx = 0;

	int clipWidth = clip.Width(), height = clip.Height();
	unsigned short *addr, *start = (unsigned short *)(screen + sx);
	register unsigned short *sprite = spriteData + spriteLine[yMin], *spriteFin = spriteData + spriteLine[yMin + clip.Height()];
	int len = *(sprite++), i0 = clip.ymin, dx, w;
	long *nextLineIndex = spriteLine + yMin + 1;
	unsigned short *nextLine = spriteData + *nextLineIndex;
	//unsigned long *offset = spriteOffSet + i0;

	unsigned short *lineOffset = (unsigned short *)(start + i0*screenWidth);//, *leftClip = lineOffset + xMin;
	int lenghtDraw = 0, offSet = 0;
	
	
	while(sprite < spriteFin)
	{		
		offSet = *(sprite++);
		addr = lineOffset + offSet;
	

		lenghtDraw = len;
		dx = 0;

		//////////////////////////////////////////////
		//Left clip		
		if(xMin)
		{
			if(addr - lineOffset >  xMin)
			{
				//Segment starts after the clip line
				addr -= xMin;
			}
			else if(addr - lineOffset <= xMin)
			{
				//Clip line intersect the segment or
				//Line ends before the clip line				
				dx = xMin - (addr - lineOffset);
				lenghtDraw -= dx;
				addr = lineOffset;
			}
		}

		//Right clip
		w = addr - lineOffset + lenghtDraw;
		if(w > clipWidth) 
		{			
			w = clipWidth - (addr - lineOffset);
		}
		
		//Draw
		if(lenghtDraw > 0 && w > 0) 
		{
			memcpy(addr, sprite+dx, ((lenghtDraw < w)?lenghtDraw:w) << 1);			
		}

		sprite += len;	
		len = *(sprite++);

		if(sprite > nextLine) 
		{
			i0++;
			*nextLineIndex++;

			nextLine = spriteData + *nextLineIndex;
			
			lineOffset = (unsigned short *)(start + i0*screenWidth);
			//leftClip = lineOffset + xMin;
		}
	}
}
