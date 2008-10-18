// compression.h: interface for the Compression class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPRESSION_H__827ACC74_D1B7_42F1_9F39_0D97BB49FF52__INCLUDED_)
#define AFX_COMPRESSION_H__827ACC74_D1B7_42F1_9F39_0D97BB49FF52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct bij_2c {
   unsigned long   Fone;
   unsigned long   Ftot;
};

#include "bit_byte.h"
#include "bit_bytx.h"
#include "minilzo.h"

/* ADAPTIVE SOURCE MODEL */


/* DECLARATIONS USED FOR ARITHMETIC ENCODING AND DECODING */

/* SIZE OF ARITHMETIC CODE VALUES. */

#define Code_value_bits 62	/* Number of bits in a code value   */
typedef Uint64 code_value;	/* Type of an arithmetic code value */

#if defined(WIN32) && !defined(LLVM)
#	define Top_value code_value(0X3FFFFFFFFFFFFFFFui64)	/* Largest code value */
#else
#	define Top_value code_value(0X3FFFFFFFFFFFFFFFull)	/* Largest code value */
#endif


/* HALF AND QUARTER POINTS IN THE CODE VALUE RANGE. */

#define First_qtr code_value(Top_value/4+1)	/* Point after first quarter    */
#define Half	  code_value(2*First_qtr)	/* Point after first half  */
#define Third_qtr code_value(3*First_qtr)	/* Point after third quarter  */

class Compression  
{
public:
	U8 *Decompression(const char *from, int &lenght);
	int DoCompression(const char *from, const char *to);
	void DoCompression(const char *from, SDL_RWops *dst);
	

	int DoCompression(void *src, unsigned int lenSrc, void *dst, unsigned int lenDst);
	bool Decompression(void *src, unsigned int lenSrc, void *dst, unsigned int lenDst, bool bCheckVersion = true);
	bool Decompression(SDL_RWops* src, unsigned int lenghtComp, void *out, unsigned int lenght);

	Compression();
	virtual ~Compression();

private:
	void bit_plus_follow(int bit);
	void encode_symbol(int symbol, bij_2c ff);
	int input_bit();
	void start_decoding();
	int decode_symbol(bij_2c ff);
	

	int cc; /* curent cell */

	/* THE BIT BUFFER. */


bit_bytx        out;
bit_byte        in;

bit_bytx        ind;
bit_byte        outd;

int				zc;
int             symbol;
int             Zero_av;
int             sw;
int             sww;

int             ZEND;		/* next 2 flags to tell when last one bit in
				 * converted to FOF */
int             ZATE;		/* has just been processed. ***NOT always
				 * last one bin file** */

code_value      VALUE;



/* CURRENT STATE OF THE ENCODING. */

code_value      low, high;	/* Ends of the current code region          */
code_value      bits_to_follow;	/* Number of opposite bits to output after  */
                                /* the next bit. */

bij_2c          ff[255]; /* 256 terminal nodes */

	
protected:
	void comp();
	void decomp();

	static bool bUclOk;
};

#endif // !defined(AFX_COMPRESSION_H__827ACC74_D1B7_42F1_9F39_0D97BB49FF52__INCLUDED_)
