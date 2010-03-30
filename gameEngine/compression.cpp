#include "GameControl.h"
#include "compression.h"
#include "Compact/ucl.h"
#include "SDL_endian.h"
#if __APPLE__
#include "SDL_rwops.h"
#endif

/*
ARB255.CPP
This is loosly based on Mark Nelson arithmetic coder.
This is a "two state bijective code". That uses a tree
arrangement of two stat bijective coders for the
compression. See ARB2.CPP for details.
Use it your own risk I make no claims as to its safety
on you machine.
David A. Scott August 2001
demo code for 2 state 62 bit high low bijective adaptive
code the mother of all bijective acompressors.
used to compress bytes using a tree.
TYPICAL USE "arb255 filein fileout"
COMPLIE first compile all minor routines then
gcc arb255.cpp *.o -o arb255.exe
*/





/* OUTPUT BITS PLUS FOLLOWING OPPOSITE BITS. */

void Compression::bit_plus_follow(int bit)
{
	
	if (bit == 1) {
		for (; zc > 0; zc--)
			out.w(0);
		for (out.w(1^sww); bits_to_follow > 0; bits_to_follow--)
			zc++;
		sww = 0;
	} else if (bit == 0) {
		if (bits_to_follow > 0) {
			for (; zc > 0; zc--)
				out.w(0);
			for (out.w(0^sww); bits_to_follow > 0; bits_to_follow--)
				out.w(1);
			sww = 0;
		} else
			zc++;
	} 
}


/* ENCODE A SYMBOL. */

void Compression::encode_symbol(int symbol, bij_2c ff)
{
	code_value      range;	/* Size of the current code region          */
	int             s1;
	range = (code_value) (high - low) + 1;	/* so first bit unchanged */
	range = range / ff.Ftot;
	range = range * ff.Fone;
	range = high - range;
	
	if (symbol == 0)
		high = range - 1;
	else if (symbol == 1)
		low = range;
	else {
		/* do what ever symbol has output test each for 1 then 0 then C */
		if (range >= Half) {
			symbol = 1;
			low = range;
		} else {
			symbol = 0;
			high = range - 1;
		}
	}
	ZATE = ((high < Half) || (low >= Half)) ? 1 : 0;
	s1 = 0;
	for (;;) {			/* Loop to output bits.     */
		if (high < Half) {
			bit_plus_follow(0);	/* Output 0 if in low half. */
		} else if (low >= Half) {	/* Output 1 if in high half. */
			bit_plus_follow(1);
			Zero_av = 0;
			low -= Half;
			high -= Half;		/* Subtract offset to top.  */
		} else if (low >= First_qtr	/* Output an opposite bit   */
			&& high < Third_qtr) {	/* later if in middle half. */
			bits_to_follow += 1;
			Zero_av = 0;
			low -= First_qtr;	/* Subtract offset to middle */
			high -= First_qtr;
		} else {
			break;			/* Otherwise exit loop.     */
		}
		low = 2 * low;
		high = 2 * high + 1;	/* Scale up code range.     */
		s1 = 1;
	}
	if (ZATE == 1) {
		if (low == 0 && Zero_av == 0)
			Zero_av = 1;		/* only first time */
		else if (low > 0)
			Zero_av = 0;		/* available in future */
		else
			Zero_av = 2;		/* not available */
	} else if (s1 == 1)
		Zero_av = 0;
}


/* INPUT A BIT. */

int Compression::input_bit()
{
	int             t, garbage_bits = 0;
	t = ind.r();
	if (t == -2) {		/* Read the next byte if no */
		garbage_bits += 1;	/* Return arbitrary bits */
		if (garbage_bits > (2 * Code_value_bits)) 
		{ /* after eof, but check */
			//error
			return -1;
		}
	}
	if (t < 0) {
		if (t == -1)
			t = 1;
		else
			t = 0;
		ZEND = 1;
	}
	return t;
}

/* ARITHMETIC DECODING ALGORITHM. */
/* START DECODING A STREAM OF SYMBOLS. */

void Compression::start_decoding()
{
	int             i;
	VALUE = 0;
	ZEND = 0;
	ZATE = 1;
	for (i = 1; i <= Code_value_bits; i++) {	/* code value.              */
		VALUE = 2 * VALUE + input_bit();	/* Move in next input bit.  */
	}
}

/* DECODE THE NEXT SYMBOL. */
int Compression::decode_symbol(bij_2c ff)
{
	code_value      range;	/* Size of current code region              */
	int             symbol;	/* Symbol decoded                           */
	int             s1;
	range = (code_value) (high - low) + 1;	/* YEAH NO +1 */
	range = range / ff.Ftot;
	range = range * ff.Fone;
	range = high - range;
	if (VALUE >= range) {
		symbol = 1;
		low = range;
	} else {
		symbol = 0;
		high = range - 1;
	}
	ZATE = ((high < Half) || (low >= Half)) ? 1 : 0;
	s1 = 0;
	for (;;) {			/* Loop to get rid of bits. */
		if (high < Half) {
			/* nothing *//* Expand low half.         */
		} else if (low >= Half) {	/* Expand high half.        */
			Zero_av = 0;
			VALUE -= Half;
			low -= Half;		/* Subtract offset to top.  */
			high -= Half;
		} else if (low >= First_qtr	/* Expand middle half.      */
			&& high < Third_qtr) {
			Zero_av = 0;
			VALUE -= First_qtr;
			low -= First_qtr;	/* Subtract offset to middle */
			high -= First_qtr;
		} else
			break;			/* Otherwise exit loop.     */
		low = 2 * low;
		high = 2 * high + 1;	/* Scale up code range.     */
		s1 = 1;
		VALUE = 2 * VALUE + input_bit();	/* Move in next input bit.  */
	}
	if (ZATE == 1) {
		if (low == 0 && Zero_av == 0)
			Zero_av = 1;		/* only first time */
		else if (low > 0)
			Zero_av = 0;		/* available in future */
		else
			Zero_av = 2;		/* not available */
	} else if (s1 == 1)
		Zero_av = 0;
	return symbol;
}


bool Compression::bUclOk = false;

Compression::Compression()
{
	cc = 0;
	symbol = 0;
	Zero_av = 0;
	sw = 0;
	sww = 0;
	ZEND = 0;				 
	ZATE = 0;
	VALUE = 0;
	low = 0;
	high = 0;
	bits_to_follow = 0;	
	
	zc = 0;
	
	memset(ff, 0, 255*sizeof(bij_2c));
	
	if(!bUclOk)
		bUclOk = ucl_init() == UCL_E_OK;
}

Compression::~Compression()
{
	
}



int Compression::DoCompression(void *src, unsigned int lenSrc, void *dst, unsigned int lenDst)
{
#ifndef STAND_ALONE_GAME
	//Compress buf and put in src	
	/*in.ir(src, lenSrc);
	out.iw(dst, lenDst);
	
	  comp();
	  
	return out.getNBytes();*/
	
	
	//Fast lzo compression
	//HEAP_ALLOC(wrkmem,LZO1X_1_MEM_COMPRESS);
	//lzo1x_1_compress((unsigned char *)src, lenSrc, (unsigned char *)dst, &lenDst, wrkmem);
	
	//Ucl
	if(!bUclOk) return lenSrc;
	
	
	if(ucl_nrv2b_99_compress((const unsigned char *)src, lenSrc, (unsigned char *)dst, (unsigned int *)&lenDst, NULL, 10, NULL, NULL) != UCL_E_OK)
	{
		return lenSrc;
	}
	
	if(lenDst >= lenSrc)
	{
		//No compression
		return lenSrc;
	}
	
	return lenDst;
#else
	return lenSrc;
#endif
}

bool Compression::Decompression(void *src, unsigned int lenSrc, void *dst, unsigned int lenDst, bool bCheckVersion)
{
	if(bCheckVersion)
	{
		if(GameControl::Get()->GetGameFileVersion() < 19)
		{
			ind.ir(src, lenSrc);
			outd.iw(dst, lenDst);
			
			decomp();
		}
		else if(GameControl::Get()->GetGameFileVersion() < 20)
		{
			//Fast lzo decompression
			lzo1x_decompress((unsigned char *)src, lenSrc, (unsigned char *)dst, &lenDst, NULL);
		}
		else
		{
			//Ucl		
			if(!bUclOk) return false;	
			
			return ucl_nrv2b_decompress_8((const unsigned char *)src, lenSrc, (unsigned char *)dst, (unsigned int *)&lenDst, NULL) == UCL_E_OK;				
		}
	}
	else
	{
		//Ucl		
		if(!bUclOk) return false;		
		return ucl_nrv2b_decompress_8((const unsigned char *)src, lenSrc, (unsigned char *)dst, (unsigned int *)&lenDst, NULL) == UCL_E_OK;				
	}
	
	return true;
}






void Compression::comp()
{
	int             ch;
    
	for ( cc = 255; cc-->0; ) 
	{
		ff[cc].Fone = 1;
		ff[cc].Ftot = 2;
	}
	
	
	
	cc = 0;
	ch = in.r();
	if (ch < 0) {
		out.w(-1);
		return;
	}
	if (ch == 0) {
		sw = 1;
		sww = 1; 
	} else {
		sw = 0;
		sww = 0;
	}
	high = Top_value;
	low = Half - 1;
	ff[cc].Fone = 2;
	ff[cc].Ftot = 3;
	cc = 2;
	ch = in.r();
	Zero_av = 1;
	bits_to_follow = 0;		/* No bits to follow next.  */
	for (;;) 
	{			/* Loop through characters. */
		if (ch < 0)
			break;
		encode_symbol(ch^sw, ff[cc]);
		if ( (ch^sw) == 1)
			ff[cc].Fone++;
		ff[cc].Ftot++;
		if (ch == 0 ) {
			cc = 2 * cc + 1;
		} else {
			cc = 2 * cc + 2;
		}
		if (cc >= 255 ) cc = 0;
		/* cc = 0; */ 
		ch = in.r();
	}
	encode_symbol(-1, ff[cc]);
	if (low == 0 && Zero_av == 1) {
		bit_plus_follow(0);
	} else
		bit_plus_follow(1);
	out.w(-2);
}

void Compression::decomp()
{
	//int             symbol = 1;
	int             ch;
	
	
	for ( cc = 255; cc-->0; ) {
		ff[cc].Fone = 1;
		ff[cc].Ftot = 2;
	}
	
	
	cc = 0;
	start_decoding();
	if (ZATE == 1 && ZEND == 1 && VALUE == Half) {
		outd.w(-1);		/* only a eof marker in file */
		return;
	}
	if (VALUE < Half) {
		VALUE += Half;
		ch = 1;
		sw = 1;
	} else {
		sw = 0;
		ch = 1;
	}
	high = Top_value;
	low = Half - 1;
	ff[cc].Fone = 2;
	ff[cc].Ftot = 3;
	Zero_av = 1;
	cc = 2;
	for (;;) {			/* Loop through characters. */
		
		if (ZEND == 1 && ZATE == 1) {
			if ((VALUE == 0 && Zero_av == 1) || VALUE == Half)
				break;
		}
		
		if(outd.w(sw^ch) == -3)
			break;
		
		ch = decode_symbol(ff[cc]);
		if (ch == 1)
			ff[cc].Fone++;
		ff[cc].Ftot++;
		if ((sw^ch) == 0 ) {
			cc = 2 * cc + 1;
		} else {
			cc = 2 * cc + 2;
		}
		if (cc >= 255 ) cc = 0;
		/* cc = 0; */
	}
	outd.w(-1);
}

int Compression::DoCompression(const char *from, const char *to)
{
	int lenght = 0, lenghtComp; 
	
	SDL_RWops *src = ged_SDL_RWFromFile(from, "rb");
	if(!src) return lenght;
	
	SDL_RWops *dst = ged_SDL_RWFromFile(to, "wb");
	if(!dst) 
	{
		SDL_RWclose(src);
		return lenght;
	}
	
	SDL_RWseek( src, 0, SEEK_END );
	lenght = SDL_RWtell( src );
	SDL_RWseek( src, 0, SEEK_SET );
	
	U8 *in = new U8[lenght];
	U8 *out = new U8[lenght + lenght / 8 + 256];
	
	SDL_RWread(src, in, lenght, 1);
	lenghtComp = DoCompression(in, lenght, out, lenght + lenght / 8 + 256);
	
	if(lenghtComp < lenght)
	{
		SDL_WriteLE32( dst, lenght );
		SDL_WriteLE32( dst, lenghtComp );
		SDL_RWwrite(dst, out, lenghtComp, 1);
	}
	else
	{
		//No compression
		SDL_WriteLE32( dst, lenght );
		SDL_WriteLE32( dst, lenght );
		SDL_RWwrite(dst, in, lenght, 1);
		
		lenghtComp = lenght;
	}
	
	delete [] in;
	delete [] out;
	
	SDL_RWclose(src);
	SDL_RWclose(dst);
	
	return lenghtComp + 2*sizeof(U32);
}

void Compression::DoCompression(const char *from, SDL_RWops *dst)
{
	int lenght, lenghtComp; 
	
	SDL_RWops *src = ged_SDL_RWFromFile(from, "rb");
	if(!src) return;
	
	SDL_RWseek( src, 0, SEEK_END );
	lenght = SDL_RWtell( src );
	SDL_RWseek( src, 0, SEEK_SET );
	
	U8 *in = new U8[lenght];
	U8 *out = new U8[2*lenght/*lenght + lenght / 8 + 256*/]; //Solved crash when export Alittlecash_player.ged
	
	SDL_RWread(src, in, lenght, 1);
	lenghtComp = DoCompression(in, lenght, out, 2*lenght/*lenght + lenght / 8 + 256*/);
	
	if(lenghtComp < lenght)
	{
		SDL_WriteLE32( dst, lenght );
		SDL_WriteLE32( dst, lenghtComp );
		SDL_RWwrite(dst, out, lenghtComp, 1);
	}
	else
	{
		//No compression
		SDL_WriteLE32( dst, lenght );
		SDL_WriteLE32( dst, lenght );
		SDL_RWwrite(dst, in, lenght, 1);
	}
	
	
	
	delete [] in;
	delete [] out;
	
	SDL_RWclose(src);	
}

U8 *Compression::Decompression(const char *from, int &lenght)
{
	U8 *out = NULL;
	int lenghtComp; 
	
	SDL_RWops *src = ged_SDL_RWFromFile(from, "rb");
	if(!src) return NULL;
	
	lenght = SDL_ReadLE32( src );
	lenghtComp = SDL_ReadLE32( src );
	
	out = new U8[lenght];
	
	if(lenghtComp < lenght)
	{
		U8 *in = new U8[lenghtComp];
		
		SDL_RWread(src, in, lenghtComp, 1);
		Decompression(in, lenghtComp, out, lenght, false);
		
		delete [] in;
	}
	else
	{
		SDL_RWread(src, out, lenght, 1);
		lenght;
	}
	
	
	SDL_RWclose(src);
	return out;
}

bool Compression::Decompression(SDL_RWops* src, unsigned int lenghtComp, void *out, unsigned int lenght)
{
	if(lenghtComp < lenght)
	{
		U8 *in = new U8[lenghtComp];
		
		SDL_RWread(src, in, lenghtComp, 1);
		bool res = Decompression(in, lenghtComp, out, lenght, false);
		
#ifdef _DEBUG
		if(!res)
		{
			GLOUTPUT("*** Decompression error (%ld, %ld)\n", lenghtComp, lenght);
		}
#endif
		
		delete [] in;
		
		return res;
	}
	
	return false;
}
