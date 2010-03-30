#ifndef __MAD_DECODE_H__
#define __MAD_DECODE_H__

#include "libmad/mad.h"
#include "buffer.h"
#include "mad_internal.h" //maks

#include "begin_code.h"

//struct MAD_decoder; //maks

DECLSPEC int           SDLCALL MAD_Decode(struct MAD_decoder *decoder, Uint8 *buffer, size_t bytes, int channels);
DECLSPEC void          SDLCALL MAD_Free(struct MAD_decoder *decoder);

/* Convenience function, create a MAD decoder from filename */
DECLSPEC struct MAD_decoder * SDLCALL MAD_CreateDecoder(const char *fname, size_t bufsize);
/* Create a MAD decoder from a SDL_RWops stream */
DECLSPEC struct MAD_decoder * SDLCALL MAD_CreateDecoder_RW(SDL_RWops *rwIn, size_t buffersize, int freerw);
void MAD_DecoderReset(MAD_decoder *decoder); //maks

#include "close_code.h"

#endif/*__MAD_DECODE_H__*/
