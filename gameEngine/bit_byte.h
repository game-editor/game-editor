#include "SDL.h"

struct bit_byte {
public:
		int getNBytes() {return bytesWritten;}
	   int put(int b);
	   int get();

	   bit_byte() 
	   {
		   xx();
		   bytesWritten = 0;
	   }

	   int             ir(FILE * fr) 
	   {	/* open file for bit read FOF assumed */
		   type = 1;

		   
		   inuse = 0x01;
		   f = fr;
		   bn = get();

		   
		   return 0;
	   }

	   int             ir(SDL_RWops *src) 
	   {
		   type = 2;

		   
		   inuse = 0x01;
		   f = src;

		   
		   return 0;
	   }

	   int             ir(void *buf, int len) 
	   {
		   type = 3;
		   bufLen = len;

		   
		   inuse = 0x01;
		   f = buf;
		   bn = get();
		   
		   
		   return 0;
	   }

	   int             iw(SDL_RWops *src) 
	   {
		   type = 2;

		   
		   inuse = 0x02;
		   f = src;
		   
	   }

	   int             iw(void *buf, int len) 
	   {
		   type = 3;
		   bufLen = len;

		   
		   inuse = 0x02;
		   f = buf;
		   
		   return 0;
	   }

	   int             iw(FILE * fw) 
	   {	/* open file for bit write FOF
		   * assumed */
		   type = 1;

		   
		   inuse = 0x02;
		   f = fw;
		   
	   }

	   int             irr(FILE * frr) 
	   {	/* open and read first bit */
		   ir(frr);
		   return r();
	   }

	   int             r();		/* get next bit */
	   
	   int             iww(FILE * fww, int b) 
	   {	/* open and read first bit */
		   iw(fww);
		   return w(b);
	   }
	   int             w(int);	/* get next bit */
	   int             status() 
	   {
		   return inuse;
	   }
	   /* on read 0 if normal -1 if last bit -2 there after */
	   /* on write -1 if current is last -2 if call after last */
private:
	
	int             xx() 
	{
		inuse = 0x69;
		M = 0x80;
		l = 0;
		f = NULL;
		bn = 0;
		bo = 0;
		zerf = 0;
		onef = 0;
		zc = 0;
		type = 0;
		pos = 0;
		
		return 0;
	}
	int             inuse;
	int             zerf;
	int             onef;
	int             bn;
	int             bo;
	int             l;
	int             M;
	long            zc;
	void           *f;
	int				type;
	int				bufLen;
	int				pos;
	int				bytesWritten;
};
