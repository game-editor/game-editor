#include "SDL.h"

struct bit_bytx {
public:
		int getNBytes() {return bytesWritten;}
	   void put(int b);
	   int get();
	   
	   
	   bit_bytx() 
	   {
		   xx();
		   bytesWritten = 0;
	   }
	   
	   int        ir(FILE * fr) 
	   {	/* open file for bit read FOF assumed */
		   type = 1;

		   inuse = 0x01;
		   f = fr;
		   bn = get();	   
	   }

	   int        ir(SDL_RWops *src) 
	   {
		   type = 2;

		   inuse = 0x01;
		   f = src;
		   bn = get();	   

		   return 0;
	   }

	   int        ir(void *buf, int len) 
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
		   
		   return 0;
	   }

	   int             iw(void *buf, int len) 
	   {
		   type = 3;
		   bufLen = len;

		   inuse = 0x02;
		   f = buf;
		   
		   return 0;
	   }
	   
	   int             irr(FILE * frr) 
	   {	/* open and read first bit */
		   ir(frr);
		   return r();
	   }
	   
	   int             r();		/* get next bit */
	   int             iw(FILE * fw) 
	   {	/* open file for bit write FOF
		   * assumed */
		   inuse = 0x02;
		   f = fw;
		   
		   type = 1;
		   return 0;
	   }
	   
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
		l = 0x80;
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
