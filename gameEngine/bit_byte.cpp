#include <stdio.h>
#include <stdlib.h>
/*
 this package the fast io byte file to bit stream
 or bit to byte file use "0 0x80 and other" method
 done by David Scott 2001 August 
*/

#include "bit_byte.h"

int             bit_byte::r()
{
   /* return -1 on last bit which is last one in file -2 thereafter */
   /* return 0 on zero return 1 most of the time one */
   if (f == NULL)
      return -2;
   if ((l >>= 1) == 0) {
      l = 0x80;
      bo = bn;
      bn = get();
      if (bo == 0)
	 zerf = 1;
      else if (bo != M) {
	 zerf = 0;
	 onef = 0;
      } else if (zerf == 1)
	 onef = 1;
      if (bn == EOF && (onef + zerf) > 0) {
	 onef = 0;
	 zerf = 0;
	 bn = M;
      }
   }
   if ((bo & l) == 0)
      return 0;;
   bo ^= l;
   if (bn != EOF || bo != 0) 
      return 1;
   xx(); 
   return -1;
}

int             bit_byte::w(int x)
{
   /* return 0 if ok */
   /* return -1 if sending last byte */
   /* return -2 if closed longer; */
   /* 0 or 1 writes and -1 wirtes last one -2 means previous 1 was last */

   if (f == NULL)
      return -2;
   if (x == -1) {
      w(1);
      w(-2);
      return -1;
   }
   if (x == -2) {
      if (bo == M &&  (zerf + onef) == 0)  
         put(bo);
      if (bo == M || bo == 0 ) {
         xx();
         return -2;
      }
   }

   if ((l >>= 1) == 0) {
      l = 0x80;
   }
   if (x > 0)
      bo ^= l;

   if (l == 1 || x < 0) {
      if (bo == 0)
	 zerf = 1;
      else if (bo != M) {
	 zerf = 0;
	 onef = 0;
      } else if (zerf == 1)
	 onef = 1;
      if (x < 0) {
         if ( (onef + zerf) == 0 || bo != M) 
            put(bo);
         xx();
         return -2;
      }
      else {
         int res = put(bo);
         bo = 0;
		 if(res)
			 return -3;
     }
   }
   return 0;
}

int bit_byte::get()
{
	if(f)
	{
		if(type == 1)
			return getc((FILE *)f);
		else if(type == 2)
		{
			Uint8 byte;
			if(SDL_RWread((SDL_RWops *)f, &byte, 1, 1 ) != 1)
				return EOF;
			
			return byte;
		}
		else if(type == 3)
		{
			if(pos < bufLen) 
				return ((unsigned char *)f)[pos++];		
		}
	}

	return EOF;
}

int bit_byte::put(int b)
{
	if(f)
	{
		if(type == 1)
		{
			fputc(b, (FILE *)f);
			bytesWritten++;
		}
		else if(type == 2)
		{
			Uint8 byte = b;
			SDL_RWwrite( (SDL_RWops *)f, &byte, 1, 1 );
			bytesWritten++;
		}
		else if(type == 3)
		{
			if(pos < bufLen) 
			{
				((unsigned char *)f)[pos++] = b;
				bytesWritten++;
				
				if(pos == bufLen) 
					return 1;
			}
		}
	}

	return 0;
}
