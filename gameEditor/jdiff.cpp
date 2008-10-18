#ifndef STAND_ALONE_GAME

/*******************************************************************************
* Jojo's Diff : diff on binary files
*
* Copyright (C) 2002 Joris Heirbaut (joheirba@tijd.com)
*
* Author                Version Date       Modification
* --------------------- ------- -------    -----------------------
* Joris Heirbaut        v0.0    10-06-2002 hashed compare
* Joris Heirbaut                14-06-2002 full compare
* Joris Heirbaut                17-06-2002 global positions
* Joris Heirbaut        v0.1    18-06-2002 first well-working runs!!!
* Joris Heirbaut                19-06-2002 compare in buffer before read position
* Joris Heirbaut        v0.1    20-06-2002 optimized esc-sequences & lengths
* Joris Heirbaut        v0.2    24-06-2002 running okay again
* Joris Heirbaut        v0.2b   01-07-2002 bugfix on length=252
* Joris Heirbaut        v0.2c   09-07-2002 bugfix on divide by zero in statistics
* Joris Heirbaut        v0.3a   09-07-2002 hashtable hint only on samplerate
* Joris Heirbaut          |     09-07-2002 exit code 1 if files are equal
* Joris Heirbaut          |     12-07-2002 bugfix using ufFabPos in function call
* Joris Heirbaut        v0.3a   16-07-2002 backtrack on original file
* Joris Heirbaut        v0.4a   19-07-2002 prescan sourcefile
* Joris Heurbaut          |     30-08-2002 bugfix in ufFabRst and ufFabPos
* Joris Heirbaut          |     03-08-2002 bugfix for backtrack before start-of-file
* Joris Heirbaut          |     09-09-2002 reimplemented filebuffer
* Joris Heirbaut        v0.4a   10-09-2002 take best of multiple possibilities
* Joris Heirbaut        v0.4b   11-09-2002 soft-reading from files
* Joris Heirbaut          |     18-09-2002 moved ufFabCmp from ufFndAhdChk to ufFndAhdAdd/Bst
* Joris Heirbaut          |     18-09-2002 ufFabOpt - optimize a found solution
* Joris Heirbaut          |     10-10-2002 added Fab->ilPosEof to correctly handle EOF condition
* Joris Heirbaut        v0.4b   16-10-2002 replaces ufFabCmpBck and ufFabCmpFwd with ufFabFnd
* Joris Heirbaut        v0.4c   04-11-2002 use ufHshFnd after prescanning
* Joris Heirbaut          |     04-11-2002 no reset of matching table
* Joris Heirbaut          |     21-12-2002 rewrite of matching table logic
* Joris Heirbaut          |     24-12-2002 no compare in ufFndAhdAdd
* Joris Heirbaut          |     02-01-2003 restart finding matches at regular intervals when no matches are found
* Joris Heirbaut          |     09-01-2003 renamed ufFabBkt to ufFabSek, use it for DEL and BKT instructions
* Joris Heirbaut        v0.4c   23-01-2003 distinguish between EOF en EOB 
* Joris Heirbaut        v0.5    27-02-2003 dropped "fast" hash method (it was slow!)
* Joris Heirbaut          |     22-05-2003 started    rewrite of FAB-abstraction
* Joris Heirbaut          |     30-06-2003 terminated rewrite ...
* Joris Heirbaut          |     08-07-2003 correction in ufMchBst (llTstNxt = *alBstNew + 1 iso -1)
* Joris Heirbaut        v0.5    02-09-2003 production
*
* TODO
* - set glMchDst in function of sample rate
* - avoid skipping matched because of full matching table
* - solve problem of all invalid matches in table!!!
* - replace giEqlCnt by better mechanism
*
* BUGS
* - "no backtrace" option does not work
*
* Licence
* -------
*
* This program is free software. Terms of the GNU General Public License apply.
*
* This program is distributed WITHOUT ANY WARRANTY, without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.a
* 
* A copy of the GNU General Public License if found in the file "Licence.txt"
* deliverd along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* Parts or all of this source code may only be reused within other GNU free, open
* source, software.
* So if your project is not an open source project, you are non entitled to read
* any further below this line!
*
* Exit codes
* ----------
*  0  ok, differences found
*  1  ok, no diffrerences found
*  2  error: not enough arguments
*  3  error: could not open first input file
*  4  error: could not open second input file
*  5  error: could not open output file
*  6  error: seek error
*  7  error: malloc failed
*
*******************************************************************************/


/*
 * Includes
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "jdiff.h"



JDiff::JDiff()
	{
	/*
	* Global settings (may be modified by commandline options) 
		*/
		giVerbse = 0;          /* Verbose level 0=no, 1=normal, 2=high            */
		gbCmpAll = 0;          /* Compare even if data not in buffer?             */
		giBufSze = 256*1024; /* File lookahead buffer size                      */
		giEqlThr = 20 ;        /* Equal threshold: throw samples if > 32 eql. chr.*/
		
		gbSrcBkt=true;         /* Backtrace on sourcefile allowed?                */
		giSrcScn = 0 ;         /* Prescan source file: 0=no, 1=do, 2=done         */
		giMchMax = 8 ;         /* Maximum entries in matching table.              */
		giMchMin = 4 ;         /* Minimum entries in matching table.              */
		glMchDst = 1024 ;    /* Max distance between equals in same match.      */
		
								   /* 
								   * Statistics about operations
		*/
		giFabSek=0;         /* Number of times an fseek operation was performed  */
		giOutBytDta=0;    /* Number of data    bytes written                   */
		giOutBytCtl=0;    /* Number of control bytes written (overhead)        */
		giOutBytDel=0;    /* Number of data    bytes deleted                   */
		giOutBytBkt=0;    /* Number of data    bytes backtraced                */
		giOutBytEsc=0;    /* Number of escape  bytes written (overhead)        */
		giOutBytEql=0;    /* Number of data    bytes not written (gain)        */
		giHshHit=0;         /* Number of hash hits                               */
		giHshErr=0;         /* Number of false hash hits                         */
		giHshRpr=0;         /* Number of repaired hash hits (by compare)         */
		giHshEls=0;         /* Number of elements added to hashtable             */
		giHshCol=0;         /* Number of collisions                              */
		giHshColSeq;        /* Number of subsequent collisions.                  */
		giHshColSeqCnt=0;   /* Number of times max number of collisions occurred.*/
		
							/* -----------------------------------------------------------------------------
							* "Constants", modifiable by commandline options
		* ---------------------------------------------------------------------------*/
		giHshIdx = 17 ;        /* choosen prime from giPme                        */
		giHshPme = 16381 ;     /* default largest 16-bit prime for hashing        */

							   /* -----------------------------------------------------------------------------
							   * Hashtable variables
		* ---------------------------------------------------------------------------*/
		glMskChk = 0xfff00000 ; /* mask for getting the checkdigit from a value   */
		glMskPos = 0x000fffff ; /* mask for getting the position   from a value   */
		glMaxVal = 0x00100000 ; /* max position for this size of check digit      */
		
              /* Actual size of the hashtable                   */
		gpHshTbl = NULL;       /* Actual hashtable                               */

		gbOutEsc=false; /* Output buffer for escape character in data stream */

		error = 0;
		errorMsg[0] = 0;

		ufMchIni();
	}
	
JDiff::~JDiff()
{
	if(gpHshTbl) free(gpHshTbl);
}
	










/* -----------------------------------------------------------------------------
 * ufFabOpn: open buffered lookahead file
 * ---------------------------------------------------------------------------*/
int JDiff::ufFabOpn ( Fab *asFab, SDL_RWops *acNam, char *acFid )
{
  asFab->icFid = acFid ;
  asFab->ipFil = acNam;
  if ( asFab->ipFil == NULL ) return(false) ; 

  asFab->ipBuf = (unsigned char *)malloc(giBufSze) ;
  if ( asFab->ipBuf == NULL ) return(false) ;
  asFab->ipMax = asFab->ipBuf + giBufSze ;
  asFab->ipInp = asFab->ipBuf;
  asFab->iiBufSze = 0;
  asFab->ilPosRed = 0;
  asFab->ilPosSek = 0;
  asFab->ilPosEof = -1;
   
  return (true) ;
}

/* -----------------------------------------------------------------------------
 * ufFabCls: closes buffered lookahead file
 * ---------------------------------------------------------------------------*/
int JDiff::ufFabCls ( Fab *asFab )
{
  //SDL_RWclose(asFab->ipFil) ;
  free(asFab->ipBuf) ;
  return (1) ;
}

/* -----------------------------------------------------------------------------
 * ufFabGet: gets one byte from the lookahead file
 *
 * Read mode cosumes bytes: old bytes in the buffer get overwritten.
 * Hard look-ahead mode performs unbuffered reading when the buffer is full.
 * Soft look-ahead mode returns EOB when the buffer is full.
 * ---------------------------------------------------------------------------*/
int JDiff::ufFabGet ( 
    Fab *asFab,      /* filebuffer                           */
    ulong *alPos,    /* position to read from                */
    int aiTyp        /* 0=read, 1=hard ahead, 2=soft ahead   */
)
{
	int lcInp ;
  unsigned char *lpDta ;

  /* Eof ? */
  if (*alPos >= asFab->ilPosEof) 
    return EOF ;

  /* Get data from buffer? */ 
  if ((*alPos >= asFab->ilPosInp - asFab->iiBufSze ) && (*alPos < asFab->ilPosInp)) {
    lpDta = asFab->ipInp - (asFab->ilPosInp - *alPos) ;
    if ( lpDta < asFab->ipBuf ) lpDta += giBufSze ;

    /* store actual read position */
    if (aiTyp == 0) asFab->ilPosRed = *alPos + 1 ;

    (*alPos)++ ;
    return *lpDta ;
  }

  /* Soft ahead: continue only if getting at input position and buffer is not full */
  if (aiTyp == 2 && (*alPos != asFab->ilPosInp || *alPos - asFab->ilPosRed > giBufSze)) 
    return EOB ;

  /* Seek if file position is not at the correct position */
  if (*alPos != asFab->ilPosSek) {
    #if debug
    if (gbDbg[DBGBUF]) fprintf(stddbg, "ufFabGet: Inner seek %d.\n", asFab->ilPosInp);
    #endif
      
    giFabSek++ ;
    if (SDL_RWseek(asFab->ipFil, *alPos, SEEK_SET) == EOF ) 
    {
      sprintf(errorMsg, "Could not reposition on file (inner seek %d)!", asFab->ilPosInp);
	  Error(errorMsg, 6);
	  
    }
    asFab->ilPosSek = *alPos ;
  }

  /* Read data from file */
  lcInp = GetByt(asFab->ipFil) ;
  if (lcInp == EOF) {
    asFab->ilPosEof = *alPos ;
    return EOF ;
  }
  else
    asFab->ilPosSek++;

  /* Read mode: reset buffer */
  if (aiTyp == 0) {
    if (*alPos != asFab->ilPosInp) {
      asFab->iiBufSze = 0 ;
      asFab->ilPosInp = *alPos ;
    }

    /* Set read position */
    asFab->ilPosRed = *alPos ;
  }

  /* Store within buffer */
  if (*alPos == asFab->ilPosInp && *alPos - asFab->ilPosRed < giBufSze) {
    *asFab->ipInp = (unsigned char) lcInp;

    asFab->ilPosInp ++ ;
    asFab->ipInp ++ ;
    if ( asFab->ipInp == asFab->ipMax ) asFab->ipInp = asFab->ipBuf ;
    if ( asFab->iiBufSze < giBufSze   ) asFab->iiBufSze ++ ;
  }
    
  /* Set new read position */
  if (aiTyp == 0) asFab->ilPosRed ++ ; 
  (*alPos)++ ;

  return lcInp ;
} 

/* -----------------------------------------------------------------------------
 * ufFabFnd: compare files at given offsets
 * ---------------------------------------------------------------------------*/
int JDiff::ufFabFnd (
    Fab *asFilOrg, Fab *asFilNew, 
    ulong *alPosOrg, ulong *alPosNew,
    int aiLen, int aiSft
    )
{ 
	int liOrg ;
	int liNew ;
	int liEql=0 ;
	int liRet=0 ;
	
#if debug
	if (gbDbg[DBGCMP]) 
		fprintf( stddbg, "Fnd (%8d,%8d,%4d,%d): ",
		*alPosOrg, *alPosNew, aiLen, aiSft) ;
#endif 
	
	/* Compare bytes */
	for (; aiLen > 24 && liRet == 0 && liEql < 24; aiLen--) 
	{
		liOrg = ufFabGet(asFilOrg, alPosOrg, aiSft) ;
		liNew = ufFabGet(asFilNew, alPosNew, aiSft) ;
		
		if (liOrg <= EOF || liNew <= EOF) 
			liRet = 1 ;
		else if (liOrg != liNew) 
			liEql = 0 ;
		else
			liEql ++ ;
	}
	
	/* Compare last 24 bytes */
	for (; aiLen > 0 && liRet == 0 && liEql < 24; aiLen--) 
	{
		liOrg = ufFabGet(asFilOrg, alPosOrg, aiSft) ;
		liNew = ufFabGet(asFilNew, alPosNew, aiSft) ;
		
		if (liOrg <= EOF || liNew <= EOF) 
			liRet = 1 ;
		else if (liOrg != liNew) 
			liRet = 2 ;
		else
			liEql ++ ;
	}
	
#if debug
	if (gbDbg[DBGCMP]) 
		fprintf( stddbg, "%8d %8d %2d %s (%c)%3o == (%c)%3o\n", 
		*alPosOrg - liEql, *alPosNew - liEql, liEql,  
		(liRet==0)?"OK!":(liRet==1)?"EOF":"NOK",
		(liOrg>=32 && liOrg <= 127)?liOrg:' ',liOrg,  
		(liNew>=32 && liNew <= 127)?liNew:' ',liNew);
#endif
	
	switch (liRet) 
	{ 
	case 0: 
		*alPosOrg = *alPosOrg - liEql ;
		*alPosNew = *alPosNew - liEql ;
		return 0 ; /* equality found */
	case 1:
		if (liOrg == EOF || liNew == EOF)
			return 2 ; /* surely different (hard eof reached) */
		else
		{ 
			*alPosOrg = *alPosOrg + aiLen ;
			*alPosNew = *alPosNew + aiLen ;
			return 1 ; /* may be different (soft eof reached) */
		} 
	case 2: 
		return 2 ; /* surely different */
  }

	return 2; //maks
}

/*******************************************************************************
* Hash table functions:
*  ufHshIni     Initialize hash table (allocate and memset)
*  ufHshAdd     Insert value into hashtable
*  ufHshGet     Lookup value into hashtable 
*  ufHshNxt     Incremental hash function on array of bytes
*
* The hash table stores positions within files. The key reflects the contents
* of the file at that posistion. This way, we can efficiently find regions
* that are equal between both files.
*
* Hash function on array of bytes:
*
* Principle:
* ----------
* Input:  a[23]  24 8-bit values
*         p      prime number
* Output: h = (a[32] x 2^32 + a[22] . 2^22 + .. + a[0]) % 2^32 % p
*
* 
* Largest n-bit primes: 251, 509, 1021, 2039, 4093, 8191, 16381, 32749, 65521, 
*                       131071 (17 bit), ..., 4294967291 (32 bit)
*
* Table entries are 64 bit, 32 bit for storing the
* position and 32 bit for storing the full hash key.
*
* The collision strategy tries to create a uniform distributed set of samples
* over the investigated region, which is either the whole file or the 
* look-ahead region.
* This is achieved by overwriting the original entry only if
* - the original entry lies before the investigatd region (the base position)
* - a number of subsequent non-overwriting collisions occur where
*   number = (region size / hashtable size) + 2
*
* Only samples from the original file are stored. 
* Samples from the new file are looked up.
*
* The investigated region is either
* - the whole file when the prescan/backtrace option is used (default)
* - the look-ahead region otherwise (options -f or -ff)
*
* With prescan/backtrace enabled, the algorithm behaves like a kind of 
* copy/insert algorithm (simulated with insert/delete/modify and backtrace
* instructions). Without prescan/backtrace, the algorithm bvehaves like an
* insert/delete algorithm.
*
*******************************************************************************/







/* -----------------------------------------------------------------------------
 * Initialization
 * ---------------------------------------------------------------------------*/
 void JDiff::ufHshIni ()
{ 
  if ( gpHshTbl == NULL ) {
    giHshSze = giHshPme * 2 * sizeof(ulong) ;
    gpHshTbl = (unsigned long *)malloc(giHshSze) ;

    giHshColSeq    = 0;
    giHshColMax    = 2;
    glHshColMaxPos = giHshPme;
  }
  if ( gpHshTbl == NULL ) 
  {
    Error("Insufficient memory!", 7);
  }
  memset(gpHshTbl, 0, giHshSze) ;
}

/* -----------------------------------------------------------------------------
 * The hash function
 * ---------------------------------------------------------------------------*/
 void JDiff::ufHshNxt ( int aiCurVal, ulong *alCurHsh )
{ *alCurHsh =  ((*alCurHsh) << 1) + aiCurVal ;
  #if debug
  if (gbDbg[DBGHSK]) 
    fprintf(stddbg, "Hash Key %x %x %c\n", *alCurHsh, aiCurVal,
      (aiCurVal>=32 && aiCurVal <= 127)?aiCurVal:' ');
  #endif
}

/* -----------------------------------------------------------------------------
 * The hashtable insert and lookup function
 * ---------------------------------------------------------------------------*/
 void JDiff::ufHshAdd (ulong alCurHsh, ulong alPos, ulong alBse )
{ int   llKey ;         /* alCurHsh % prime */

  /* calculate key and the corresponding entries' address */
  llKey    = (alCurHsh % giHshPme) * 2 ;

  /* keep giHshColMax == glHshColMaxPos / giHshPme + 2 */
  if ( alPos - alBse >= glHshColMaxPos ) 
  {
    glHshColMaxPos += giHshPme ;
    giHshColMax ++ ;
  }
  
  /* store key and value
  *  existing entries are overwritten if they are "old", that is
  *  they are before the actual read position, or if more
  *  than giHshColMax subsequent collisions occured and key is not a 
  *  multiple of four.
  */

  /* count elements and collisions */
  if ((gpHshTbl[llKey] == 0) || (gpHshTbl[llKey+1] < alBse))
    giHshEls++ ; 
  else 
  { 
    giHshCol++ ;
    giHshColSeq++ ;
    if (giHshColSeq >= giHshColMax) giHshColSeqCnt++;
  }

  /* store key and value
   * existing entries are overwritten if
   * - they are old (older than alBse)
   * - more than a maximum number of subsequent collisions occurred
   */
  if ((gpHshTbl[llKey] == 0) || (gpHshTbl[llKey+1] < alBse) || (giHshColSeq >= giHshColMax))
  { 
    gpHshTbl[llKey]   = alCurHsh ;
    gpHshTbl[llKey+1] = alPos ;
    giHshColSeq   = 0 ;
  }
}

 long JDiff::ufHshGet (ulong alCurHsh)
{ 
	int   llKey ;         /* alCurHsh % prime */

  /* calculate key and the corresponding entries' address */
  llKey    = (alCurHsh % giHshPme) * 2 ;

  /* lookup value into hashtable for new file */
  if (gpHshTbl[llKey] == alCurHsh) 
  {
    giHshHit++;
    return gpHshTbl[llKey + 1];
  }
  return 0 ;
}

/* -----------------------------------------------------------------------------
 * Print hashtable content (for debugging or auditing)
 * ---------------------------------------------------------------------------*/
void JDiff::ufHshPnt() 
{ 
	#if debug

	int liHshIdx;
  int liCnt=0;
  int liCol=0;

  for (liHshIdx = 0; liHshIdx < giHshPme * 2; liHshIdx += 4) 
  {
    if (gpHshTbl[liHshIdx] != 0 && gpHshTbl[liHshIdx+2] != 0) liCol++ ;
    if ((gpHshTbl[liHshIdx] != 0) || (gpHshTbl[liHshIdx+2] != 0)) 
    {
      liCnt ++ ;
      fprintf(stddbg, "Hash Pnt %06d %08x-%08x %08x-%08x", liHshIdx, 
          gpHshTbl[liHshIdx], gpHshTbl[liHshIdx+1], 
          gpHshTbl[liHshIdx+2], gpHshTbl[liHshIdx+3]) ;
      if (gpHshTbl[liHshIdx] == gpHshTbl[liHshIdx+2]) 
        fprintf(stddbg, " EQUAL!\n") ;
      else 
        fprintf(stddbg, "\n") ;
    }
  }
  fprintf(stddbg, "Hash table load:       %d%%\n", liCnt / giHshPme / 2) ;
  fprintf(stddbg, "Hash table collisions: %d\n", liCol) ;

#endif
}

 
/*******************************************************************************
* Matching table functions
*
* The matching table contains a series of possibly matching regions between
* the two files. 
* 
* Because of the statistical nature of the hash table, we're not sure that the
* best solution will be found first. Consider, for example, that the samplerate 
* is at 10% on files of about 10Mb and equal regions exist at positions 1000-2000 
* and 1500-1500. Because 10% of 10Mb means one sample every 1024 bytes, 
* it can happen that the 1000-2000 region is only discovered at positions 2000-3000 
* (1000 bytes later). If the 1500-1500 region gets found first at say 1700-1700, 
* and if we would not optimize the found solutions, then 500 equal bytes would 
* get lost.
*
* Therefore we first memorize a number of matching postions found with the hashtable, 
* optimize them (look 1024 bytes back) and then select the first matching solution.
*
* ufMchIni      Initializes the matching table
* ufMchAdd      Adds a potential solution to the mach table
* ufMchFre      Checks if there is room for new solutions
* ufMchBst      Optimizes the matches and returns the best one
* 
*******************************************************************************/


void JDiff::ufMchIni () { memset(giMchCnt, 0, MCH_MAX * sizeof(int)) ; }

/* -----------------------------------------------------------------------------
 * Auxiliary function: ufMchAdd
 * Add given match to the array of matches:
 * - add to colliding match if possible, or
 * - add at the end of the list, or 
 * - override an old match otherwise
 * Returns 
 *  -1   if regions are found different
 *   0   if a new entry has been added and table is full
 *   1   if a new entry has been added
 *   2   if an existing entry has been enlarged
 * ---------------------------------------------------------------------------*/
int JDiff::ufMchAdd (
  Fab   *asFilOrg,
  Fab   *asFilNew,
  ulong alFndOrgAdd,      /* match to add               */
  ulong alFndNewAdd,
  ulong alBseNew
)
{ int liIdx;    /* scrolls through all matches                  */
  int liFre=-1; /* points to the first free entry               */
  int liNxt=-1; /* points to the next  free entry               */

  alFndOrgAdd -= 24 ;
  alFndNewAdd -= 24 ;

  /* entries before alBseNew do not make any sense */
  /* if (alFndNewAdd < alBseNew) return -1 ; */
  
  /* add to existing match or find oldest match */
  for (liIdx=0; liIdx < giMchMax ; liIdx++)
    if (giMchCnt[liIdx] == 0)
    { if (liFre == -1 || giMchCnt[liFre] != 0)
      { liNxt = liFre ;
        liFre = liIdx ;
      }
      else if (liNxt == -1)
      { liNxt = liIdx ;
      }
    } 
    else
    { /* add to colliding match if possible */
      if ((alFndOrgAdd - glMchOrg[liIdx] == alFndNewAdd - glMchNew[liIdx]) && 
          (alFndNewAdd <= glMchNew[liIdx] + glMchDst) &&
          (alFndNewAdd + glMchDst >= glMchBeg[liIdx]))
      {
        /*#if debug
         if (gbDbg[DBGMCH])
          fprintf(stddbg, "Mch (%8d,%8d) Col (%2d,%8d,%8d) Bse (%8d) %s\n",
                  alFndOrgAdd, alFndNewAdd, liIdx, glMchOrg[liIdx], glMchNew[liIdx], alBseNew,
                  (glMchNew[liIdx] == alFndNewAdd)?"Bst":"   ") ; 
        #endif*/
  
        giMchCnt[liIdx] ++ ; 
        if (alFndNewAdd > glMchNew[liIdx])
        { glMchNew[liIdx] = alFndNewAdd ;
          glMchOrg[liIdx] = alFndOrgAdd ;
        } else if (alFndNewAdd < glMchBeg[liIdx])
        { glMchBeg[liIdx] = alFndNewAdd ;
        }
  
        return 2 ;  
      }
  
      /* get oldest match */
      if (glMchNew[liIdx] < alBseNew) 
      { if ((liFre == -1) || (giMchCnt[liFre] != 0 && glMchNew[liIdx] < glMchNew[liFre]))
        { liNxt = liFre ;
          liFre = liIdx ;
        }
        else if (liNxt == -1)
        { liNxt = liIdx ;
        }
      }
    }

  /* override an old entry if there's no more place */
  if (liFre == -1)
  {
    #if debug
    if (gbDbg[DBGMCH]) fprintf(stddbg, "Mch (%8d, %8d) Ful\n", alFndOrgAdd, alFndNewAdd) ;
    #endif
    return 0 ;
  }
  else 
  { 
    glMchOrg[liFre] = alFndOrgAdd ;
    glMchNew[liFre] = alFndNewAdd ;
    glMchBeg[liFre] = alFndNewAdd ;
    glMchTst[liFre] = 0 ;
    glMchFnd[liFre] = 0 ;
    giMchCmp[liFre] = 0 ;
    giMchCnt[liFre] = 1 ;

    #if debug
    if (gbDbg[DBGMCH])
      fprintf(stddbg, "Mch (%8d,%8d) New (%2d,%8d,%8d) Bse (%8d) %s\n",
              alFndOrgAdd, alFndNewAdd, liFre, glMchOrg[liFre], glMchNew[liFre], alBseNew,
              (glMchNew[liFre] == alFndNewAdd)?"Bst":"   ") ;
    #endif

    if (liNxt != -1)
      return 1 ; /* added, still room for new entries */
    else
      return 0 ; /* added, table if now full */
  }
}

/* -----------------------------------------------------------------------------
 * ufMchFre: check if there is free space in the table of matches
 * ---------------------------------------------------------------------------*/
int JDiff::ufMchFre ( ulong alBseNew )
{ int liIdx ;
  for (liIdx=0; liIdx < giMchMax ; liIdx++)
    if (giMchCnt[liIdx] == 0 || glMchNew[liIdx] < alBseNew)
      return true ;
  return false ;
}

/* -----------------------------------------------------------------------------
 * Auxiliary function: ufMchBst
 * Get the best match from the array of matches
 * ---------------------------------------------------------------------------*/
int JDiff::ufMchBst (
  Fab   *asFilOrg,      /* files                */
  Fab   *asFilNew,
  ulong alBseOrg,       /* base positions       */
  ulong alBseNew,
  ulong *alBstOrg,      /* best position found  */
  ulong *alBstNew
)
{
  int liIdx ;           /* investigated  match.                         */
  int liBst=-1 ;        /* selected best match.                         */
  int liChk ;           /* check match to investigate                   */
  int liNxt ;           /* first match to investigate                   */
  int liCmp ;           /* compare status                               */

  ulong llTstNew ;      /* test positions                               */
  ulong llTstOrg ;
  ulong llTstBse ;
  ulong llTstNxt ;

  int lbStr ;

  ulong llSwp ;
  int   liSwp ;

  for (liIdx=0, liNxt=0; liIdx < giMchMax && liNxt >= 0; liIdx ++)
  {  /* find first valuable entry */
    if (liBst == -1)
      liNxt = -1 ;
    else
    { liNxt    = liBst ;
      llTstNxt = *alBstNew + 1 ;
    }
  
    for (liChk = liIdx; liChk < giMchMax && giMchCnt[liChk] > 0; liChk ++)
      if (giMchCnt[liChk] > 0)
      { /* calculate the test position */
        if (glMchNew[liChk] + glMchDst >= alBseNew) 
        { if (glMchBeg[liChk] <= alBseNew + glMchDst - 24)
            llTstNew = alBseNew ;
          else 
            llTstNew = glMchBeg[liChk] - glMchDst + 24 ;
        }
        else
          llTstNew = 0;
  
        /* is this entry the first valuable? */
        if (llTstNew > 0)
        { if (liNxt == -1) 
          { llTstNxt = llTstNew ;
            liNxt    = liChk ;
          } 
          else if (llTstNew < llTstNxt) 
          { llTstNxt = llTstNew ;
            liNxt    = liChk ;
          } 
        }
      }
  
    /* valuable entry found? */
    if (liNxt <= liBst)
      liNxt = -1 ; /* exit from loop */
    else
    { lbStr = false ;
      liCmp = 0 ;
  
      /* swap next entry to current position */
      if (liNxt != liIdx)
      { llSwp = glMchBeg[liIdx] ; glMchBeg[liIdx] = glMchBeg[liNxt] ; glMchBeg[liNxt] = llSwp ;
        llSwp = glMchNew[liIdx] ; glMchNew[liIdx] = glMchNew[liNxt] ; glMchNew[liNxt] = llSwp ;
        llSwp = glMchOrg[liIdx] ; glMchOrg[liIdx] = glMchOrg[liNxt] ; glMchOrg[liNxt] = llSwp ;
        liSwp = giMchCnt[liIdx] ; giMchCnt[liIdx] = giMchCnt[liNxt] ; giMchCnt[liNxt] = liSwp ;
        llSwp = glMchTst[liIdx] ; glMchTst[liIdx] = glMchTst[liNxt] ; glMchTst[liNxt] = llSwp ;
        llSwp = glMchFnd[liIdx] ; glMchFnd[liIdx] = glMchFnd[liNxt] ; glMchFnd[liNxt] = llSwp ;
        liSwp = giMchCmp[liIdx] ; giMchCmp[liIdx] = giMchCmp[liNxt] ; giMchCmp[liNxt] = liSwp ;
      }

      /* calculate corresponding original position */
      llTstNew = llTstNxt ;
      llTstBse = llTstNxt ;
      if (liCmp == 0)
      { if (glMchOrg[liIdx] + llTstNew >= glMchNew[liIdx])
          llTstOrg = glMchOrg[liIdx] + llTstNew - glMchNew[liIdx] ;
        else
          liCmp = 4 ;
      }

      /* compare */
      if (liCmp == 0) 
      { /* do not compare a same entry twice */
        if (glMchTst[liIdx] > 0 && llTstNew >= glMchTst[liIdx] && llTstNew <= glMchFnd[liIdx])
        {
          /* use stored result */
          lbStr = true ;
          liCmp = giMchCmp[liIdx] ;
          llTstNew = glMchFnd[liIdx] ;
          llTstOrg = glMchOrg[liIdx] + llTstNew - glMchNew[liIdx] ;
        }
        else
        { 
          /* find equality */
          liCmp = ufFabFnd(asFilOrg, asFilNew, &llTstOrg, &llTstNew, glMchDst, gbCmpAll?1:2) ;

          /* soft eof reached, then rely on hash function */
          if (liCmp == 1 && giMchCnt[liIdx] <= 3) 
            liCmp = 7 ;

          /* remove false matches */
          if (liCmp == 2 && giMchCnt[liIdx] == 1 && llTstBse <= glMchBeg[liIdx] && llTstNew > glMchBeg[liIdx])
          { giMchCnt[liIdx] = 0 ;
            liCmp = 2 ;
            giHshRpr++ ;
          }

          /* keep the result for later */
          if (liCmp <= 1)
          { glMchTst[liIdx] = llTstBse ;
            glMchFnd[liIdx] = llTstNew ;
            giMchCmp[liIdx] = liCmp ;
          }
        }
      }
  
      /* evaluate */
      if (liCmp <= 1)
      { if ((liBst == -1) || 
            (llTstNew + 4 <= *alBstNew) || 
            ((llTstNew <= *alBstNew + 4) && (giMchCnt[liIdx] > giMchCnt[liBst]))) 
        { *alBstNew = llTstNew ;
          *alBstOrg = llTstOrg ;
          liBst     = liIdx ;
        }
      }

      /* show table */
      #if debug
      if (gbDbg[DBGMCH])
        fprintf(stddbg, "Mch %2d:%2d:%1d%c%c[%8d,%8d,%8d,%4d,%8d] F=%8d=%8d\n", 
          liIdx, liNxt, liCmp, 
          (liBst == liIdx)?'*':
            (liCmp==0)    ?'=':
              (liCmp==1)  ?'?':' ', 
          (lbStr)?'!':' ',
          glMchBeg[liIdx], glMchOrg[liIdx], glMchNew[liIdx], giMchCnt[liIdx],
          glMchOrg[liIdx] - glMchNew[liIdx], 
          llTstBse, llTstNew) ;
      #endif
    } 
  } 

  /* show table */
  #if debug
  for (; liIdx < giMchMax ; liIdx ++)
  if (gbDbg[DBGMCH])
    fprintf(stddbg, "Mch %2d:  :   [%8d,%8d,%8d,%4d,%8d]\n", 
      liIdx, 
      glMchBeg[liIdx], glMchOrg[liIdx], glMchNew[liIdx], giMchCnt[liIdx],
      glMchOrg[liIdx] - glMchNew[liIdx]) ;
  #endif

  if (liBst == -1)
  { 
    #if debug
    if (gbDbg[DBGMCH])
      fprintf(stddbg, "Mch Err\n") ;
    #endif
    return -1 ;
  }
  else 
    return liBst ;
}

/*******************************************************************************
* Find Ahead function
*
* Read ahead on both files until we possibly found an equal series of 32 bytes 
* in both files.
* Then calculate the deplacement vector between two files:
* - positive if characters need to be inserted in the original file,
* - negative if characters need to be removed from the original file.
*******************************************************************************/

/* -----------------------------------------------------------------------------
 * Auxiliary function:
 * Get next character from file (lookahead) and count number of equal chars
 * in current cluster 
 * ---------------------------------------------------------------------------*/
void JDiff::ufFndAhdGet(Fab *asFilOut, ulong *alPos, int *aiVal, int *aiPrv, int *aiEql, int aiSft) 
{
  *aiPrv = *aiVal ;
  *aiVal = ufFabGet(asFilOut, alPos, aiSft) ;
  if (*aiPrv == *aiVal) { if (*aiEql < 32) (*aiEql)++ ; }
  else                  { if (*aiEql > 0)  (*aiEql)-- ; }
}

/* -----------------------------------------------------------------------------
 * Prescan the original file
 * ---------------------------------------------------------------------------*/
void JDiff::ufFndAhdScn (
  Fab *asFilOrg
)
{ ulong llHshOrg=0;     // Current hash value for original file
  int   liValOrg;       // Current file value
  int   liPrvOrg;       // Previous file value
  int   liEqlOrg=0;     // Number of times current value occurs in hash value
  ulong llPosOrg=0;     // Position within original file

  int   liIdx ;

  /* Initialize hash function */
  liValOrg = ufFabGet(asFilOrg, &llPosOrg, 1) ;
  for (liIdx=0;(liIdx < 31) && (liValOrg > EOF); liIdx++) { 
    ufHshNxt(liValOrg, &llHshOrg) ;
    ufFndAhdGet(asFilOrg, &llPosOrg, &liValOrg, &liPrvOrg, &liEqlOrg, 1) ;
  }

  /* Build hashtable */
  while (liValOrg > EOF) {
    ufHshNxt(liValOrg, &llHshOrg) ;
    if ( liEqlOrg < giEqlThr ) 
    { ufHshAdd(llHshOrg, llPosOrg, 0) ;
      #if debug
      if (gbDbg[DBGAHH])
        fprintf(stddbg, "ufHshAdd(%2x -> %8x, %8d, %8d)\n", 
          liValOrg, llHshOrg, llPosOrg, 0);
      #endif
    }
    ufFndAhdGet(asFilOrg, &llPosOrg, &liValOrg, &liPrvOrg, &liEqlOrg, 1) ;
  }
}

/* -----------------------------------------------------------------------------
 * The findahead function
 * ---------------------------------------------------------------------------*/
int JDiff::ufFndAhd (
  Fab *asFilOrg,
  Fab *asFilNew,
  ulong alRedOrg,     /* read position in original file                     */
  ulong alRedNew,     /* read position in new file                          */
  int *aiSkpOrg,      /* number of bytes to skip (delete) in original file  */
  int *aiSkpNew,      /* number of bytes to skip (insert) in new file       */
  int *aiAhd          /* number of bytes to go before similarity is reached */
)
{ static ulong llAhdOrg=0;   // Current ahead position on original file
  static ulong llAhdNew=0;   // Current ahead position on new file
  static ulong llHshOrg=0;   // Current hash value for original file
  static ulong llHshNew=0;   // Current hash value for new file
  static int liValOrg;       // Current file value
  static int liValNew;       // Current file value
  static int liPrvOrg;       // Previous file value
  static int liPrvNew;       // Previous file value
  static int liEqlOrg=0;     // Number of times current value occurs in hash value
  static int liEqlNew=0;     // Number of times current value occurs in hash value
  static int sbIni = false ; // Table of matches initialized?

  ulong llFndOrg=0;   /* Found position within original file                 */
  ulong llFndNew=0;   /* Found position within new file                      */
  ulong llBseOrg;     /* Base position on original file: gbSrcBkt?0:alRedOrg */

  int liIdx;          /* Index for initializing                         */
  int liFnd=0;        /* Number of matches found                        */
  int liBst;          /* Selected match                                 */
  int liSft;          /* 1 = hard look-ahead, 2 = soft look-ahead       */

  /* 
   * Initialize table of matches on first call 
   */
  if (! sbIni)
  { ufMchIni() ;
    sbIni=true;
  }
  liSft = 1 ; /* find at least one match */

  /* Prescan the original file? */
  if (giSrcScn == 1) {
    ufFndAhdScn(asFilOrg) ;
    giSrcScn = 2 ;
  }

  /* 
   * Re-Initialize hash function (read 31 bytes) if 
   * - ahead position has been reset, or
   * - read position has passed the ahead position
   */
  if (giSrcScn == 0 && (llAhdOrg == 0 || llAhdOrg < alRedOrg)) 
  {
    llAhdOrg = alRedOrg ;
    liEqlOrg = 0 ;
    llHshOrg = 0 ;

    liValOrg = ufFabGet(asFilOrg, &llAhdOrg, liSft) ;
    for (liIdx=0;(liIdx < 31) && (liValOrg > EOF); liIdx++)
    { ufHshNxt(liValOrg, &llHshOrg) ;
      ufFndAhdGet(asFilOrg, &llAhdOrg, &liValOrg, &liPrvOrg, &liEqlOrg, liSft) ;
    }
  }
  if (llAhdNew == 0 || llAhdNew < alRedNew) 
  {
    llAhdNew = alRedNew ;
    liEqlNew = 0 ;
    llHshNew = 0 ;

    liValNew = ufFabGet(asFilNew, &llAhdNew, liSft) ;
    for (liIdx=0;(liIdx < 31) && (liValNew > EOF); liIdx++)
    { ufHshNxt(liValNew, &llHshNew) ;
      ufFndAhdGet(asFilNew, &llAhdNew, &liValNew, &liPrvNew, &liEqlNew, liSft) ;
    }
  }

  /*
   * Build the table of matches
   */
  if (ufMchFre(alRedNew))
  {
    /* Do not backtrace before llBseOrg */
    llBseOrg = (gbSrcBkt?0:alRedOrg) ;

    /* Do not read from original file if it has been prescanned */
    if (giSrcScn > 0) liValOrg = EOB ;

    /* Scroll through both files until an equal hash value has been found */
    while (((liValOrg > EOF) || (liValNew > EOF )) && (liFnd < giMchMax))
    {
      /* insert original file's value into hashtable (if no prescanning has been done) */
      if (liValOrg > EOF) 
      { /* hash the new value */
        ufHshNxt(liValOrg, &llHshOrg) ;

        /* add to hash table */
        if ( liEqlOrg < giEqlThr )
        { ufHshAdd(llHshOrg, llAhdOrg, llBseOrg) ;

          #if debug
          if (gbDbg[DBGAHH])
            fprintf(stddbg, "ufHshAdd(%2x -> %8x, %8d, %8d)\n", 
              liValOrg, llHshOrg, llAhdOrg, llBseOrg);
          #endif
        }

        /* get next value from file */
        ufFndAhdGet(asFilOrg, &llAhdOrg, &liValOrg, &liPrvOrg, &liEqlOrg, liSft) ;
      }
      
      /* check new file against original file */
      if (liValNew > EOF)
      { /* hash the new value and lookup in hashtable */
        ufHshNxt(liValNew, &llHshNew) ;
        if ( liEqlNew < giEqlThr )
          llFndOrg = ufHshGet(llHshNew) ;
        else
          llFndOrg = 0 ;

        /* add found position into table of matches */
        if (llFndOrg > llBseOrg)
        { 
          /* add solution to the table of matches */ 
          switch (ufMchAdd(asFilOrg, asFilNew, llFndOrg, llAhdNew, alRedNew))
          { case 0: /* table is full */
              liFnd = giMchMax; 
              break;
            case 1: /* alternative added */
              liFnd ++ ;
              liSft = (liFnd >= giMchMin)?2:1 ;
              break ; 
            case 2:  /* alternative colided */
            case -1:; /* compare failed      */
          }
        }

        /* get next value from file */
        ufFndAhdGet(asFilNew, &llAhdNew, &liValNew, &liPrvNew, &liEqlNew, liSft) ;
      }
    }
  }
 
  /* 
   * Get the best match and calculate the offsets 
   */ 
  liBst = ufMchBst(asFilOrg, asFilNew, alRedOrg, alRedNew, &llFndOrg, &llFndNew) ;
  if (liBst == -1)
  { *aiSkpOrg = 0 ;
    *aiSkpNew = 0 ;
    *aiAhd    = (llAhdNew - alRedNew) / 4 ;
    if (*aiAhd < 256) *aiAhd = 256 ;
    return false ; 
  }
  else
  { 
    if (llFndOrg >= alRedOrg)
    { if (llFndOrg - alRedOrg >= llFndNew - alRedNew) 
      { /* go forward on original file */
        *aiSkpOrg = llFndOrg - alRedOrg + alRedNew - llFndNew ;
        *aiSkpNew = 0 ;
        *aiAhd    = llFndNew - alRedNew ; 
      }
      else
      { /* go forward on new file */
        *aiSkpOrg = 0;
        *aiSkpNew = llFndNew - alRedNew + alRedOrg - llFndOrg ;
        *aiAhd    = llFndOrg - alRedOrg ;
      }
    }
    else
    { /* backtrack on original file */
      *aiSkpOrg = (int) alRedOrg - llFndOrg + llFndNew - alRedNew ;
      if (*aiSkpOrg < alRedOrg) 
      { *aiSkpNew = 0 ;
        *aiSkpOrg = - *aiSkpOrg ;
        *aiAhd = llFndNew - alRedNew ;
      }
      else /* do not bactrace before beginning of file */
      { *aiSkpNew = *aiSkpOrg - (int) alRedOrg ;
        *aiSkpOrg = - (int) alRedOrg ;
        *aiAhd = (llFndNew - alRedNew) - *aiSkpNew ;
      }

      /* reset ahead position when backtracking */
      llAhdOrg = 0 ;
    }

    return true ;
  }
}

/*******************************************************************************
* Output functions
*
* The output has following format
*   <esc> <opcode> [<length>|<data>]
* where
*   <esc>    =   ESC
*   <opcode> =   MOD | INS | DEL | EQL | BKT
*   <data>   :   A series of data bytes. 
*        The series is ended with a new "<esc> <opcode>" sequence.
*        If an "<esc> <opcode>" sequence occurs within the data, it is
*        prefixed with an additional <esc>.
*        E.g.: <ESC><MOD>This data contains an <ESC><ESC><EQL> sequence.
*   <length> :   1 to 5 bytes for specifying a 32-bit unsigned number.
*              1 <= x < 252        1 byte:   0-251
*            252 <= x < 508        2 bytes:  252, x-252
*            508 <= x < 0x10000    3 bytes:  253, xx
*        0x10000 <= x < 0x100000000        5 bytes:  254, xxxx
*                          9 bytes:  255, xxxxxxxx
*
*******************************************************************************/






/* ---------------------------------------------------------------
 * ufPutLen outputs a length as follows
 * byte1  following      formula              if number is
 * -----  ---------      -------------------- --------------------
 * 0-251                 1-252                between 1 and 252
 * 252    x              253 + x              between 253 and 509
 * 253    xx             253 + 256 + xx       a 16-bit number
 * 254    xxxx           253 + 256 + xxxx     a 32-bit number
 * 255    xxxxxxxx       253 + 256 + xxxxxxxx a 64-bit number
 * ---------------------------------------------------------------*/
 void JDiff::ufPutLen ( ulong aiLen, SDL_RWops *asFilOut )
 { 
	 if (aiLen <= 252) {
		 PutByt(aiLen - 1, asFilOut) ;
		 giOutBytCtl += 1;
	 } else if (aiLen < 509) {
		 PutByt(252, asFilOut);
		 PutByt((aiLen - 253), asFilOut) ;
		 giOutBytCtl += 2;
	 } else if (aiLen <= 0xffff) {
		 PutByt(253, asFilOut);
		 PutByt((aiLen >>  8)       , asFilOut) ;
		 PutByt((aiLen      ) & 0xff, asFilOut) ;
		 giOutBytCtl += 3;
	 } else if (aiLen <= 0xffffffff) {
		 PutByt(254, asFilOut);
		 PutByt((aiLen >> 24)       , asFilOut);
		 PutByt((aiLen >> 16) & 0xff, asFilOut) ;
		 PutByt((aiLen >>  8) & 0xff, asFilOut) ;
		 PutByt((aiLen      ) & 0xff, asFilOut) ;
		 giOutBytCtl += 5;
	 }
 }

/* ---------------------------------------------------------------
 * ufPutOpr outputs a new opcode and closes the previous
 * data stream.
 * ---------------------------------------------------------------*/
void JDiff::ufPutOpr ( int aiOpr, SDL_RWops *asFilOut )
{ if (gbOutEsc) {
    PutByt(ESC, asFilOut) ;
    PutByt(ESC, asFilOut) ;
    gbOutEsc = false ;
    giOutBytEsc++ ;
    giOutBytDta++ ;
  }

  if ( aiOpr != ESC ) {
    PutByt(ESC, asFilOut);
    PutByt(aiOpr, asFilOut);
    giOutBytCtl+=2;
  }
}

/* ---------------------------------------------------------------
 * ufPutByt outputs a byte, prefixing a data sequence <esc> <opcode>
 * with an addition <esc> byte.
 * ---------------------------------------------------------------*/
void JDiff::ufPutByt ( int aiByt, SDL_RWops *asFilOut )
{ int liCnt = 0;
  
  if (gbOutEsc) {
    gbOutEsc = false;
    if (aiByt >= BKT && aiByt <= ESC) {
      /* output an additional <esc> byte */
      PutByt(ESC, asFilOut) ;
      giOutBytEsc++ ;
    }
    PutByt(ESC, asFilOut) ;
    giOutBytDta++;
  }
  if (aiByt == ESC) {
    gbOutEsc = true ;
  } else {
    PutByt(aiByt, asFilOut) ;
    giOutBytDta++;
  }
}

/* ---------------------------------------------------------------
 * ufOutBytBin: binary output function for generating patch files
 * ---------------------------------------------------------------*/
void JDiff::ufOutBytBin ( 
  int  aiOpr, 
  int  aiOrg, 
  int  aiNew,
  Fab  *asFilOrg, 
  Fab  *asFilNew,
  SDL_RWops *asFilOut,
  ulong alPosOrg,
  ulong alPosNew 
)
{ static int liOprCur = ESC ; 
  static int liEqlCnt = 0;    /* number of subsequent equal bytes */
  static int liEqlBuf[4];     /* first four equal bytes */
  int liCnt ;

  if (liEqlCnt > 0 && aiOpr != EQL) {
    if (liEqlCnt > 4 || (liOprCur != MOD && aiOpr != MOD)) {
      liOprCur = EQL;
      ufPutOpr(EQL, asFilOut) ;
      ufPutLen(liEqlCnt, asFilOut);

      giOutBytEql+=liEqlCnt;
    } else {
      if (liOprCur != MOD) {
        liOprCur = MOD ;
        ufPutOpr(MOD, asFilOut) ;
      }
      for (liCnt=0; liCnt < liEqlCnt; liCnt++) 
        ufPutByt(liEqlBuf[liCnt], asFilOut) ;
    }
    liEqlCnt=0;
  }

  switch (aiOpr) {
    case ESC : /* before closing the output */
      ufPutOpr(ESC, asFilOut);
      liOprCur = ESC ;
      break;

    case MOD :
    case INS :
      if (liOprCur != aiOpr) {
        liOprCur = aiOpr ;
        ufPutOpr(aiOpr, asFilOut) ;
      }
      ufPutByt(aiNew, asFilOut) ;
      break;
      
    case DEL :
      ufPutOpr(DEL, asFilOut) ;
      ufPutLen(aiNew, asFilOut);

      liOprCur=DEL;
      giOutBytDel+=aiNew;
      break;

    case BKT :
      ufPutOpr(BKT, asFilOut) ;
      ufPutLen(aiNew, asFilOut);

      liOprCur=BKT;
      giOutBytBkt+=aiNew;
      break;

    case EQL :
      if (liEqlCnt < 4) liEqlBuf[liEqlCnt] = aiOrg ; 
      liEqlCnt ++ ;
      break;
  }
}

/*void JDiff::ufOutBytAsc ( 
  int  aiOpr, 
  int  aiOrg, 
  int  aiNew,
  Fab  *asFilOrg, 
  Fab  *asFilNew,
  SDL_RWops *asFilOut,
  ulong alPosOrg,
  ulong alPosNew 
)
{ static int liOprCur ;

  if (alPosOrg > 0) 
    fprintf(asFilOut, "%8d ", alPosOrg) ;
  else
    fprintf(asFilOut,"         ");
  if (alPosNew > 0) 
    fprintf(asFilOut, "%8d ", alPosNew) ;
  else
    fprintf(asFilOut,"         ");

  switch (aiOpr) {
    case (MOD) :
      fprintf(asFilOut, "MOD %3o %3o %c-%c\n", aiOrg, aiNew, 
        ((aiOrg >= 32 && aiOrg <= 127)?(char) aiOrg:' '),
        ((aiNew >= 32 && aiNew <= 127)?(char) aiNew:' '));
          
      if (liOprCur != aiOpr) {
        liOprCur = aiOpr ;
        giOutBytCtl+=2;
      }
      if (aiNew == ESC) giOutBytEsc++;
      giOutBytDta++;
      break;
        
    case (INS) :
      fprintf(asFilOut, "INS     %3o  -%c\n", aiNew,
        ((aiNew >= 32 && aiNew <= 127)?(char) aiNew:' '));
        
      if (liOprCur != aiOpr) {
        liOprCur = aiOpr ;
        giOutBytCtl+=2;
      }
      if (aiNew == ESC) giOutBytEsc++;
      giOutBytDta++;
      break;

    case (DEL) :
      fprintf(asFilOut, "DEL %d\n", aiNew);

      liOprCur=DEL;
      giOutBytCtl+=2+sizeof(aiNew);
      giOutBytDel+=aiNew;
      break;

    case (BKT) :
      fprintf(asFilOut, "BKT %d\n", aiNew);

      liOprCur=BKT;
      giOutBytCtl+=2+sizeof(aiNew);
      giOutBytBkt+=aiNew;
      break;

    case (EQL) :
      fprintf(asFilOut, "EQL %3o %3o %c-%c\n", aiOrg, aiNew, 
        ((aiOrg >= 32 && aiOrg <= 127)?(char) aiOrg:' '),
        ((aiNew >= 32 && aiNew <= 127)?(char) aiNew:' '));
        
      if (liOprCur != aiOpr) {
        liOprCur = aiOpr ;
        giOutBytCtl+=2;
      }
      if (aiNew == ESC) giOutBytEsc++;
      giOutBytDta++;
      break;
  }
}*/

/*void JDiff::ufOutBytRgn ( 
  int  aiOpr, 
  int  aiOrg, 
  int  aiNew,
  Fab  *asFilOrg, 
  Fab  *asFilNew,
  SDL_RWops *asFilOut,
  ulong alPosOrg,
  ulong alPosNew 
)
{ static int liOprCur=ESC ;
  static int liOprCnt ;
  static int liEqlCnt = 0 ;

  // equal count pre-buffer 
  switch (aiOpr)
  { case (EQL) :
      if (liEqlCnt < 4 && liOprCur != EQL)
      { liEqlCnt ++ ;
        return ;
      }
      break ;
    case (MOD) :
      break ;
    default : 
      if (liEqlCnt > 0)
      { fprintf(asFilOut, "%8d %8d EQL %d\n", alPosOrg - liEqlCnt, alPosNew - liEqlCnt, liEqlCnt);
        giOutBytCtl+=2;
        giOutBytEql+=liEqlCnt;
        liEqlCnt = 0;
      }
  }

  // write output when operation code changes 
  if (aiOpr != liOprCur) {
    switch (liOprCur) {
      case (MOD) :
        giOutBytCtl+=2;
        giOutBytDta+=liOprCnt ;
        fprintf(asFilOut, "%8d %8d MOD %d\n", alPosOrg - liOprCnt - liEqlCnt, alPosNew - liOprCnt - liEqlCnt, liOprCnt) ;
        break;
        
      case (INS) :
        giOutBytCtl+=2;
        giOutBytDta+=liOprCnt ;
        fprintf(asFilOut, "         %8d INS %d\n", alPosNew - liOprCnt - liEqlCnt, liOprCnt) ;
        break;
  
      case (DEL) :
        giOutBytCtl+=2;
        giOutBytDel+=liOprCnt;
        fprintf(asFilOut, "%8d %8d DEL %d\n", alPosOrg - liOprCnt - liEqlCnt, alPosNew - liEqlCnt, liOprCnt);
        break;
  
      case (BKT) :
        giOutBytCtl+=2;
        giOutBytBkt+=liOprCnt;
        fprintf(asFilOut, "%8d %8d BKT %d\n", alPosOrg + liOprCnt - liEqlCnt, alPosNew - liEqlCnt, liOprCnt);
        break;
  
      case (EQL) :
        giOutBytCtl+=2;
        giOutBytEql+=liOprCnt ;
        fprintf(asFilOut, "%8d %8d EQL %d\n", alPosOrg - liOprCnt, alPosNew - liOprCnt, liOprCnt);
        break;
    }

    liOprCur = aiOpr;
    liOprCnt = 0;
  }

  // accumulate operation codes 
  switch (aiOpr) {
    case(DEL):
      liOprCnt += aiNew ;
      break ;
    case(BKT):
      liOprCnt += aiNew ;
      break ;
    case(INS):
      liOprCnt++ ;
      if (aiNew == ESC) giOutBytEsc++;
      break;
    case(MOD):
      if (liEqlCnt > 0)
      { liOprCnt += liEqlCnt ;
        liEqlCnt =  0 ;
      } 
      liOprCnt++ ;
      if (aiNew == ESC) giOutBytEsc++;
      break;
    case(EQL): 
      if (liEqlCnt > 0)
      { liOprCnt += liEqlCnt ;
        liEqlCnt =  0 ;
      } 
      liOprCnt++ ;
      break;
  }
}*/

/*******************************************************************************
* Difference function
*
* Takes two files as arguments and writes out the differences
*
* Principle: 
*   Take one byte from each file and compare. If they are equal, then continue.
*   If they are different, start lookahead to find next equal blocks within file.
*   If equal blocks are found, 
*   - first insert or delete the specified number of bytes,
*   - then continue reading on both files until equal blocks are reached,
*
*******************************************************************************/
int JDiff::jdiff ( Fab *asFilOrg, Fab *asFilNew, SDL_RWops *asFilOut )
{ int liOrg ;           /* byte from original file */
  int liNew ;           /* byte from new file */

  ulong llPosOrg = 0 ;  /* read position on original file */
  ulong llPosNew = 0 ;  /* read position on new file */

  int lbFnd=false;      /* offsets are pointing to a valid solution?*/
  int liAhd=0;          /* number of bytes to go ahead after a find */
  int liSkpOrg ;        /* number of bytes to skip in original file */ 
  int liSkpNew ;        /* number of bytes to skip in new file */

  /* Take one byte from each file ... */
  liOrg = ufFabGet(asFilOrg, &llPosOrg, 0);
  liNew = ufFabGet(asFilNew, &llPosNew, 0);
  while (liNew > EOF) {
    if(liAhd > 0) {
      /* 
       * If equal blocks are found
       * Continue reading on both files until equal blocks are reached
       */
      liAhd--;

    } else if (liOrg != liNew) {
      if (liOrg > EOF) 
        llPosOrg -- ;
      llPosNew -- ;

      /* flush output when debugging */
      #if debug
      if (gbDbg[DBGMCH] || gbDbg[DBGAHD])
        ufOutBytBin(ESC, 0, 0, 0, 0, asFilOut, llPosOrg, llPosNew);
      #endif

      /*
       * If they are different, start lookahead to find next equal blocks within file.
       */
      lbFnd = ufFndAhd(asFilOrg, asFilNew, llPosOrg, llPosNew, &liSkpOrg, &liSkpNew, &liAhd) ;
      #if debug
      if (gbDbg[DBGAHD]) 
        fprintf(stddbg, "Findahead on %d %d skip %d %d ahead %d\n", 
          llPosOrg - 1, llPosNew - 1, liSkpOrg, liSkpNew, liAhd) ;
      if (gbDbg[DBGPRG]) 
        fprintf(stddbg, "Current position in new file= %d\n", llPosNew) ;
      #endif

      if (lbFnd) {
        /*
         * If equal blocks are found
         * First insert, delete or backtrace the specified number of bytes
         */
        if (liSkpOrg > 0) {
          ufOutBytBin(DEL, 0, liSkpOrg, asFilOrg, asFilNew, asFilOut, llPosOrg + 1, llPosNew + 1) ;
          llPosOrg += liSkpOrg ;
          liOrg = ufFabGet(asFilOrg, &llPosOrg, 0);
        } 
        else if (liSkpOrg < 0) {
          ufOutBytBin(BKT, 0, - liSkpOrg, asFilOrg, asFilNew, asFilOut, llPosOrg + 1, llPosNew + 1) ;
          llPosOrg += liSkpOrg ;
          liOrg = ufFabGet(asFilOrg, &llPosOrg, 0);
        }
        else if (liOrg > EOF)
          llPosOrg ++ ;

        llPosNew ++ ;
        if (liSkpNew > 0) {
          while (liSkpNew > 0) {
            ufOutBytBin(INS, 0, liNew, asFilOrg, asFilNew, asFilOut, llPosOrg, llPosNew);
            liSkpNew-- ;
            liNew = ufFabGet(asFilNew, &llPosNew, 0);
          }
        }
      }
      else {
        /*if (giVerbse>1){
          fprintf(stddbg, "No valid matches found at positions %d %d, skipping %d bytes.\n", llPosOrg, llPosNew, liAhd) ; 
        }*/
        if (liOrg > EOF) llPosOrg ++ ;
        llPosNew ++ ;
      } 
    }

    if (liAhd==0) {
      liAhd--;
      if (liOrg != liNew) {
        if (lbFnd && (giVerbse>2 || gbCmpAll)) {
          //fprintf(stddbg, "Hash miss!\n");
          giHshErr++ ;
        }
      }
    }

    /*
     * If they are equal, then continue.
     */
    if(liOrg == liNew) {
      ufOutBytBin(EQL, liOrg, liNew, asFilOrg, asFilNew, asFilOut, llPosOrg, llPosNew);
    } else {
      ufOutBytBin(MOD, liOrg, liNew, asFilOrg, asFilNew, asFilOut, llPosOrg, llPosNew);
    }

    /* Take one byte from each file ... */
    liOrg = ufFabGet(asFilOrg, &llPosOrg, 0) ;
    liNew = ufFabGet(asFilNew, &llPosNew, 0) ;
  }
  ufOutBytBin(ESC, 0, 0, 0, 0, asFilOut, llPosOrg, llPosNew);
  return (1);
}




int JDiff::Diff(SDL_RWops *lcFilNamFrt, SDL_RWops *lcFilNamScd, SDL_RWops *lpFilOut)
{
	error = 0;

	try
	{

  Fab lsFilFrt;
  Fab lsFilScd;

  int liOptArgCnt=0 ;
  int lbOptArgDne=false ;
  char lcHlp='\0';

  //int liHshLodMaxPrc;
  //int liHshLodAvgPrc;
  //int liHshSmpRtePrc;

  /* Default settings */
  //ufOutBytBin    = &ufOutBytBin;
  //stddbg   = stderr ;

  gbCmpAll = true ;
  giBufSze = 4096 * 1024 ;
  gbSrcBkt = true;
  giSrcScn = 1 ;
  giMchMin = 8 ;
  giMchMax = 64 ;
  giHshIdx = 13 ; // 4MB (256k x 4b x 2) Hashtables
  giHshPme = giPme[giHshIdx] ;

  /* Read options */
  /*while (! lbOptArgDne && (aiArgCnt-1 > liOptArgCnt)) {
    liOptArgCnt++ ;
    if (strcmp(acArg[liOptArgCnt], "-v") == 0) {
      giVerbse = 1; 
    } else if (strcmp(acArg[liOptArgCnt], "-vv") == 0) {
      giVerbse = 2;
    } else if (strcmp(acArg[liOptArgCnt], "-vvv") == 0) {
      giVerbse = 3;
    } else if (strcmp(acArg[liOptArgCnt], "-h") == 0) {
      lcHlp = 'h' ;

    } else if (strcmp(acArg[liOptArgCnt], "-m") == 0) {
        liOptArgCnt ++;
        if (aiArgCnt > liOptArgCnt) { 
          giBufSze = atoi(acArg[liOptArgCnt]) / 2 ;
          if (giBufSze <  64)        giBufSze =  64 ;
          if (giBufSze > 256 * 1024) giBufSze = 256 * 1024 ;
          giBufSze = giBufSze * 1024 ;
        }
    } else if (strcmp(acArg[liOptArgCnt], "-s") == 0) {
        liOptArgCnt++;
        if (aiArgCnt > liOptArgCnt) { 
          giHshPme = atoi(acArg[liOptArgCnt]) ;
          for (giHshIdx=0; giHshIdx < 23 && giPme[giHshIdx] > giHshPme; giHshIdx++) ;
          giHshPme = giPme[giHshIdx] ;
        }

    } else if (strcmp(acArg[liOptArgCnt], "-l") == 0) {
        ufOutBytBin = &ufOutBytAsc;
    } else if (strcmp(acArg[liOptArgCnt], "-lr") == 0) {
        ufOutBytBin = &ufOutBytRgn;
    } else if (strcmp(acArg[liOptArgCnt], "-b") == 0) {
        // Larger hashtables
        gbCmpAll = true ;
        giBufSze = 4096 * 1024 ;
        gbSrcBkt = true;
        giSrcScn = 1 ;
        giMchMin = 8 ;
        giMchMax = 64 ;
        giHshIdx = 9 ; // 4M x 4b x 2 = 32MB Hashtable
        giHshPme = giPme[giHshIdx] ;
    } else if (strcmp(acArg[liOptArgCnt], "-f") == 0) {
        // No compare out-of-buffer
        gbCmpAll = false ;
        giBufSze = 64 * 1024 ;
        gbSrcBkt = true ;
        giSrcScn = 1  ;
        giMchMin = 8 ;
        giMchMax = 16 ;
        giHshIdx = 13 ; // 256k x 4b x 2 Hashtable
        giHshPme = giPme[giHshIdx] ;
    } else if (strcmp(acArg[liOptArgCnt], "-ff") == 0) {
        // No compare out-of-buffer and no backtracing
        gbCmpAll = false ;
        giBufSze = 64 * 1024 ;
        gbSrcBkt = false ;
        giSrcScn = 0 ;
        giMchMin = 8 ;
        giMchMax = 16 ;
        giHshIdx = 13 ; // 256k x 4b x 2 Hashtable
        giHshPme = giPme[giHshIdx] ;
    } else if (strcmp(acArg[liOptArgCnt], "-e") == 0) {
        liOptArgCnt ++;
        if (aiArgCnt > liOptArgCnt) {
          giEqlThr = atoi(acArg[liOptArgCnt]) ;
        }

    #if debug
    } else if (strcmp(acArg[liOptArgCnt], "-do") == 0) {
      stddbg = stdout;
    } else if (strcmp(acArg[liOptArgCnt], "-dh") == 0) {
      gbDbg[DBGHSH] = true ;
    } else if (strcmp(acArg[liOptArgCnt], "-da") == 0) {
      gbDbg[DBGAHD] = true ;
    } else if (strcmp(acArg[liOptArgCnt], "-dc") == 0) {
      gbDbg[DBGCMP] = true ;
    } else if (strcmp(acArg[liOptArgCnt], "-dp") == 0) {
      gbDbg[DBGPRG] = true ;
    } else if (strcmp(acArg[liOptArgCnt], "-db") == 0) {
      gbDbg[DBGBUF] = true ;
    } else if (strcmp(acArg[liOptArgCnt], "-dk") == 0) {
      gbDbg[DBGHSK] = true ;
    } else if (strcmp(acArg[liOptArgCnt], "-dahh") == 0) {
      gbDbg[DBGAHH] = true ;
    } else if (strcmp(acArg[liOptArgCnt], "-dbkt") == 0) {
      gbDbg[DBGBKT] = true ;
    } else if (strcmp(acArg[liOptArgCnt], "-dred") == 0) {
      gbDbg[DBGRED] = true ;
    } else if (strcmp(acArg[liOptArgCnt], "-dm") == 0) {
      gbDbg[DBGMCH] = true ;
    #endif

    } else {
      lbOptArgDne = true ;
      liOptArgCnt-- ;
    }
  }*/

  /* Output greetings */  
  /*if ((giVerbse>0) || (lcHlp == 'h') || (aiArgCnt - liOptArgCnt < 3)) {
    fprintf(stddbg, "JDIFF - Jojo's binary diff 0.5 (Beta) September 2003\n") ;
    fprintf(stddbg, "Copyright (C) 2002 Joris Heirbaut (joheirba@tijd.com)\n");
    fprintf(stddbg, "\n") ;
    fprintf(stddbg, "This program is free software. Terms of the GNU General Public License apply.\n") ;
    fprintf(stddbg, "\n") ;
    fprintf(stddbg, "This program is distributed WITHOUT ANY WARRANTY, without even the implied\n");
    fprintf(stddbg, "warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");
    fprintf(stddbg, "See the GNU General Public License for more details.\n") ;
    fprintf(stddbg, "\n");
  }*/

  /*if ((aiArgCnt - liOptArgCnt < 3) || (lcHlp == 'h')) { 
    fprintf(stddbg, "Usage: jdiff [options] <original file> <new file> [<output file>]\n") ;
    fprintf(stddbg, "  -v          Verbose (greeting, results and tips).\n");
    fprintf(stddbg, "  -vv         Verbose (debug info).\n");
    fprintf(stddbg, "  -h          Help (this text).\n");
    fprintf(stddbg, "  -l          Listing (ascii output).\n");
    fprintf(stddbg, "  -lr         Regions (ascii output).\n");
    fprintf(stddbg, "  -b          Better: copy/insert, larger hashtable.\n");
    fprintf(stddbg, "  -f          Fast:   copy/insert but no out of buffer compares.\n");
    fprintf(stddbg, "  -ff         Faster: insert/delete and no out of buffer compares.\n");
    fprintf(stddbg, "  -m size     Size (in kB) for look-ahead buffers (default 128).\n");
    fprintf(stddbg, "  -s size     Number of samples per file (default 32749).\n");
    fprintf(stddbg, "Principles:\n");
    fprintf(stddbg, "  JDIFF tries to find equal regions between two binary files using a heuristic\n");
    fprintf(stddbg, "  hash algorithm and outputs the differences between both files.\n");
    fprintf(stddbg, "  Heuristics are generally used for improving performance and memory usage,\n");
    fprintf(stddbg, "  at the cost of accuracy. Therefore, this program may not find a minimal set\n");
    fprintf(stddbg, "  of differences between files.\n");
    fprintf(stddbg, "Notes:\n");
    fprintf(stddbg, "  Options -m and -s should be used after -b, -f or -ff.\n");
    fprintf(stddbg, "  Accuracy may be improved by increasing the number of samples.\n");
    fprintf(stddbg, "  Speed may be increased with option -f or -ff (lower accuracy).\n");
    fprintf(stddbg, "  Sample size is always lowered to the largest n-bit prime (n < 32)\n");
    fprintf(stddbg, "  Original and new file must be random access files.\n");
    fprintf(stddbg, "  Output is sent to standard output if output file is missing.\n");
    fprintf(stddbg, "\n");
    fprintf(stddbg, "  JDIFF uses file buffers to minimize (slower) random accesses in files.\n");
                     
    exit(2);
  }*/

  /* Read filenames */
  /*lcFilNamFrt = acArg[1 + liOptArgCnt];
  lcFilNamScd = acArg[2 + liOptArgCnt];
  if (aiArgCnt - liOptArgCnt >= 4) 
    lcFilNamOut = acArg[3 + liOptArgCnt];
  else 
    lcFilNamOut = "-" ;*/

  /* Initialize */
  ufHshIni();

  /* Output settings */
  /*if (giVerbse>1) {
    fprintf(stddbg, "Lookahead buffers: %d kb. (%d kb. per file).\n", giBufSze * 2 / 1024, giBufSze / 1024) ;
    fprintf(stddbg, "Hastable size    : %d kb. (%d samples).\n", (giHshSze + 512) / 1024, giHshPme) ;
  }
  if (giVerbse>2) {
    fprintf(stddbg, "Equal Threshold  : %d.\n", giEqlThr) ;
  }*/

  /* Open files */
  if ( ! ufFabOpn(&lsFilFrt, lcFilNamFrt, "Org") )
  { 
		Error("Could not open first file for reading.\n", 3);
  }
  if ( ! ufFabOpn(&lsFilScd, lcFilNamScd, "New") )
  { 
	  ufFabCls(&lsFilFrt);
	  Error("Could not open second file for reading.\n", 4);
  }

  /*if (strcmp(lcFilNamOut,"-") == 0 ) 
    lpFilOut = stdout ;
  else 
    lpFilOut = fopen(lcFilNamOut, "wb") ;
  if ( lpFilOut == NULL ) 
  {
	  ufFabCls(&lsFilFrt) ;
	  ufFabCls(&lsFilScd) ;
	  Error("Could not open output file for writing.\n", 5);
    
  }*/

  /* Go ... */
  jdiff(&lsFilFrt, &lsFilScd, lpFilOut);

  /* Terminate */
  ufFabCls(&lsFilFrt);
  ufFabCls(&lsFilScd);
  //SDL_RWclose(lpFilOut);

  /* Write statistics */
  #if debug
  if (gbDbg[DBGHSH]) ufHshPnt() ;
  #endif

  /*if (giVerbse > 0) {
    if (giHshEls + giHshCol == 0)
      liHshSmpRtePrc = -1 ;
    else
      liHshSmpRtePrc = giHshEls * 100 / (giHshEls + giHshCol) ;
  }
  if (giVerbse > 1) {
    fprintf(stddbg, "Hashtable hits          = %d\n",   giHshHit) ;
    fprintf(stddbg, "Hashtable errors        = %d\n",   giHshErr) ;
    fprintf(stddbg, "Hashtable repairs       = %d\n",   giHshRpr) ;
    fprintf(stddbg, "Hashtable collisions    = %d\n",   giHshCol) ;
    fprintf(stddbg, "Hashtable overrides     = %d\n",   giHshColSeqCnt) ;
    fprintf(stddbg, "Samples (rate, load)    = %d (%d%%, %d%%)\n", giHshEls, liHshSmpRtePrc, giHshEls * 100 / giHshPme) ;
    fprintf(stddbg, "Random accesses         = %d\n", giFabSek);
    fprintf(stddbg, "Delete    bytes         = %d\n", giOutBytDel);
    fprintf(stddbg, "Backtrack bytes         = %d\n", giOutBytBkt);
    fprintf(stddbg, "Escape    bytes written = %d\n", giOutBytEsc);
    fprintf(stddbg, "Control   bytes written = %d\n", giOutBytCtl);
  }
  if (giVerbse > 0) {
    fprintf(stddbg, "Equal     bytes         = %d\n", giOutBytEql);
    fprintf(stddbg, "Data      bytes written = %d\n", giOutBytDta);
    fprintf(stddbg, "Overhead  bytes written = %d\n", giOutBytCtl + giOutBytEsc);
    
    // Samplerate is low when below 5% 
    if ((giHshIdx > 8) && (liHshSmpRtePrc < 5) && (liHshSmpRtePrc != -1)) {
      fprintf(stddbg, "Tip: Sample rate is low. You may want to increase the size of the hashtable\n");
      fprintf(stddbg, "     with option -s %d for improved accuracy.\n", giPme[giHshIdx - 1]);

    } else if ((giHshIdx < 20) && (liHshSmpRtePrc > 66)) {
      fprintf(stddbg, "Tip: Sample rate is high. You may want to decrease the size of the hashtable\n");
      fprintf(stddbg, "     with option -s %d for improved performance.\n", giPme[giHshIdx + 1]);
    }

    // Random accesses are high when more than one seek per 64k of data 
    if ((giFabSek > (int) (giOutBytDta + giOutBytEql) / (64 * 1024)) && (giFabSek > 10)) {
      fprintf(stddbg, "Tip: Random accesses are high. You may want to increase the size of the \n");
      fprintf(stddbg, "     lookahead buffers using option -m for improved performance.\n");
    } else if ((giFabSek < (int) (giOutBytDta + giOutBytEql) / (256*1024*1024)) && (giBufSze > 64*1024)) {
      fprintf(stddbg, "Tip: Random accesses are rare. You may want to decrease the size of the \n");
      fprintf(stddbg, "     lookahead buffers using option -m.\n");
    }

    if (giHshErr > 1 && giHshErr > (int) (giOutBytDta + giOutBytEql) / 4 / 1024 / 1024) {
      fprintf(stddbg, "Tip: number of hash errors is high. Do not use option -ff.\n");
    }
  } */

  if (giOutBytDta == 0 && giOutBytDel == 0)
    return 1;    /* no differences found */
  else
     return 0;    /* differences found    */

  }
  catch(...)
  {
	   if(!error) error = -1;
  }

  return error;
}

int JDiff::GetByt( SDL_RWops *lpFil )
{ 
	int c = SDL_RWgetc(lpFil);
	return c;
}

int JDiff::PutByt( int c, SDL_RWops *lpFil )
{ 
	return SDL_RWputc(lpFil, c);
}

int JDiff::GetInt( SDL_RWops *lpFil )
{ 
	int liVal ;

  liVal = GetByt(lpFil) ;
  if (liVal < 252)
    return liVal + 1 ;
  else if (liVal == 252)
    return 253 + GetByt(lpFil) ;
  else if (liVal == 253) {
    liVal = GetByt(lpFil) ;
    liVal = (liVal << 8) + GetByt(lpFil) ;
    return liVal ;
  }
  else if (liVal == 254) {
    liVal = GetByt(lpFil) ;
    liVal = (liVal << 8) + GetByt(lpFil) ;
    liVal = (liVal << 8) + GetByt(lpFil) ;
    liVal = (liVal << 8) + GetByt(lpFil) ;
    return liVal ;
  } else 
  {
    Error("64-bit length numbers not supported!", 6);
    return 0;
  }
}

/*******************************************************************************
* Patch function
*******************************************************************************
* Input stream consists of a series of
*   <op> (<data> || <len>)
* where
*   <op>   = <ESC> (<MOD>||<INS>||<DEL>||<EQL>)
*   <data> = <chr>||<ESC><ESC>
*   <chr>  = any byte different from <ESC><MOD><INS><DEL> or <EQL>
*   <ESC><ESC> yields one <ESC> byte
*******************************************************************************/
void JDiff::jpatch (SDL_RWops *asFilOrg, SDL_RWops *asFilPch, SDL_RWops *asFilOut)
 
{
  int liInp ;         /* Current input from patch file          */
  int liOrg ;         /* Current input from data file           */
  int liOpr ;         /* Current operand                        */
  int liCnt ;         /* Counter                                */
  int lbChg=false ;   /* Changing operand?                      */
  int lbEsc=false ;   /* Non-operand escape char found?	    */

  liOpr = ESC ;
  while ((liInp = GetByt(asFilPch)) != EOF) {
    if (liInp == ESC) {
      liInp = GetByt(asFilPch); 
      switch(liInp) {
        case MOD: 
          liOpr = MOD;
          /*if (giVerbse == 2) {
            fprintf(stddbg, "%8d %8d MOD ...    \n", ftell(asFilOrg)+1, ftell(asFilOut)+1) ;
          }*/
          lbChg = true;
          break ;
          
        case INS: 
          liOpr = INS;
          /*if (giVerbse == 2) {
            fprintf(stddbg, "%8d %8d INS ...    \n", ftell(asFilOrg)+1, ftell(asFilOut)+1)   ;
          }*/
          lbChg = true;
          break ;

        case DEL: 
          liOpr = DEL; 
          liInp = GetInt(asFilPch); 
          /*if (giVerbse > 1) {
            fprintf(stddbg, "%8d %8d DEL %7d\n", ftell(asFilOrg)+1, ftell(asFilOut)+1, liInp)  ;
          }*/

          if (SDL_RWseek(asFilOrg, liInp, SEEK_CUR) == -1) 
		  {
            sprintf(errorMsg, "Could not position on original file (seek back %d).\n", liInp);
			Error(errorMsg, 5);
          }
          lbChg = true;
          break ;

        case EQL: 
          liOpr = EQL; 
          liInp = GetInt(asFilPch); 
          /*if (giVerbse > 1) {
            fprintf(stddbg, "%8d %8d EQL %7d\n", ftell(asFilOrg)+1, ftell(asFilOut)+1, liInp) ;
          }*/

          for (liCnt=0; liCnt < liInp; liCnt++) {
            liOrg = GetByt(asFilOrg) ;
            PutByt(liOrg, asFilOut) ;
          }
          lbChg = true;
          break ;

        case BKT:
          liOpr = BKT ;
          liInp = GetInt(asFilPch) ;
          /*if (giVerbse > 1) {
            fprintf(stddbg, "%8d %8d BKT %7d\n", ftell(asFilOrg)+1, ftell(asFilOut)+1, liInp)   ;
          }*/

          if (SDL_RWseek(asFilOrg, - liInp, SEEK_CUR) == -1) 
		  {
            sprintf(errorMsg, "Could not position on original file (seek back %d).\n", liInp);
            Error(errorMsg, 5);
          }
          lbChg = true;
          break ;

        case ESC:
          /*if (giVerbse > 2) {
            fprintf(stddbg, "%8d %8d ESC ESC\n", ftell(asFilOrg)+1, ftell(asFilOut)+1) ;
          }*/
          break;

        default:
          /*if (giVerbse > 2) {
            fprintf(stddbg, "%8d %8d ESC XXX\n", ftell(asFilOrg)+1, ftell(asFilOut)+1) ;
          }*/
          lbEsc = true;
          break;
      }
    }
    if ( lbChg ) {
      lbChg = false ;
    } else {
      switch (liOpr) {
        case DEL: break;
        case EQL: break;
        case BKT: break;
        case MOD:
          if (lbEsc) {
            PutByt(ESC, asFilOut) ;
            liOrg = GetByt(asFilOrg) ;
            /*if (giVerbse > 2) {
              fprintf(stddbg, "%8d %8d MOD %3o %3o\n", ftell(asFilOrg), ftell(asFilOut), liOrg, liInp) ;
            }*/
          }

          PutByt(liInp, asFilOut) ;
          liOrg = GetByt(asFilOrg) ;
          /*if (giVerbse > 2) {
            fprintf(stddbg, "%8d %8d MOD %3o %3o\n", ftell(asFilOrg), ftell(asFilOut), liOrg, liInp)  ;
          }*/
          break ;

        case INS :
          if (lbEsc) {
            /*if (giVerbse > 2) {
              fprintf(stddbg, "%8d %8d INS     %3o\n", ftell(asFilOrg)+1, ftell(asFilOut)+1, ESC)  ; 
            }*/
            PutByt(ESC, asFilOut) ;
          }
  
          /*if (giVerbse > 2) {
            fprintf(stddbg, "%8d %8d INS     %3o\n", ftell(asFilOrg)+1, ftell(asFilOut)+1, liInp)  ; 
          }*/

          PutByt(liInp, asFilOut) ;
          break ;
      }
    } /* if lbChg */

    lbEsc = false ;
  } /* while */
}

int JDiff::Patch(SDL_RWops *lpFilOrg, SDL_RWops *lpFilPch, SDL_RWops *lpFilOut)
{
	error = 0;

	try
	{



  int lbOptArgDne=false;
  int liOptArgCnt=0;
  char lcHlp='\0';

  /* Read options */
  /*stddbg = stderr;
  while (! lbOptArgDne && (aiArgCnt-1 > liOptArgCnt)) {
    liOptArgCnt++ ;
    if (strcmp(acArg[liOptArgCnt], "-v") == 0) {
      giVerbse = 1;
    } else if (strcmp(acArg[liOptArgCnt], "-vv") == 0) {
      giVerbse = 2;
    } else if (strcmp(acArg[liOptArgCnt], "-vvv") == 0) {
      giVerbse = 3;
    } else if (strcmp(acArg[liOptArgCnt], "-d") == 0) {
      stddbg = stdout;
    } else if (strcmp(acArg[liOptArgCnt], "-h") == 0) {
      lcHlp = 'h' ;
    } else {
      lbOptArgDne = true; 
      liOptArgCnt--;
    }
  }*/

  /* Output greetings */  
  /*if ((giVerbse>0) || (lcHlp == 'h') || (aiArgCnt - liOptArgCnt < 3)) {
    fprintf(stddbg, "JPATCH - Jojo's binary patch v0.2 (Beta) June 2002\n") ;
    fprintf(stddbg, "Copyright (C) 2002 Joris Heirbaut (joheirba@tijd.com)\n");
    fprintf(stddbg, "\n") ;
    fprintf(stddbg, "This program is free software. Terms of the GNU General Public License apply.\n") ;
    fprintf(stddbg, "\n") ;
    fprintf(stddbg, "This program is distributed WITHOUT ANY WARRANTY, without even the implied\n");
    fprintf(stddbg, "warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");
    fprintf(stddbg, "See the GNU General Public License for more details.\n") ;
    fprintf(stddbg, "\n");
  }*/

  /*if ((aiArgCnt - liOptArgCnt < 3) || (lcHlp == 'h')) { 
    fprintf(stddbg, "Usage: jpatch [options] <original file> <patch file> [<output file>]\n") ;
    fprintf(stddbg, "  -v               Verbose (greeting, results and tips).\n");
    fprintf(stddbg, "  -vv              Verbose (debug info).\n");
    fprintf(stddbg, "  -vvv             Verbose (more debug info).\n");
    fprintf(stddbg, "  -h               Help (this text).\n");
    //fprintf(stddbg, "  -l               Ascii patch file.\n");
    fprintf(stddbg, "Principles:\n");
    fprintf(stddbg, "  JPATCH reapplies a diff file, generated by jdiff, to the <original file>,\n") ;
    fprintf(stddbg, "  restoring the <new file>. For example, if jpatch is called as follows:\n");
    fprintf(stddbg, "    jdiff data01.tar data02.tar data02.dif\n");
    fprintf(stddbg, "  then data02.tar can be restored as follows:\n");
    fprintf(stddbg, "    jpatch data01.tar data02.dif data02.tar\n");
    fprintf(stddbg, "\n");
                     
    exit(1);
  }*/

  /* Read filenames */
  /*lcFilNamOrg = acArg[liOptArgCnt + 1];
  lcFilNamPch = acArg[liOptArgCnt + 2];
  if (aiArgCnt > liOptArgCnt + 3)
    lcFilNamOut = acArg[liOptArgCnt + 3] ;
  else
    lcFilNamOut = "-" ;*/

  /* Open files */
  /*if ( strcmp(lcFilNamOrg, "-") == 0 )
    lpFilOrg = stdin ;
  else
    lpFilOrg = fopen(lcFilNamOrg, "rb") ;
  if ( lpFilOrg == NULL ) 
  {
    sprintf(errorMsg, "Could not open data file %s for reading.\n", lcFilNamOrg) ;
    Error(errorMsg, 2);
  }*/

  /*if ( strcmp(lcFilNamPch, "-") == 0 )
    lpFilPch = stdin ;
  else 
    lpFilPch = fopen(lcFilNamPch, "rb") ;
  if ( lpFilPch == NULL ) 
  {
    sprintf(errorMsg, "Could not open patch file %s for reading.\n", lcFilNamPch) ;
    Error(errorMsg, 3);

  }*/

  /*if ( strcmp(lcFilNamOut, "-") == 0 ) 
    lpFilOut = stdout ;
  else 
    lpFilOut = fopen(lcFilNamOut, "wb") ;
  if ( lpFilOut == NULL ) 
  {
    fclose(lpFilOrg) ;
    fclose(lpFilPch) ;

	Error("Could not open output file for writing.\n", 4);
    
  }*/
 
  /* Go... */ 
  jpatch(lpFilOrg, lpFilPch, lpFilOut);

  /* Close files */
  /*fclose(lpFilOrg);
  fclose(lpFilPch);
  fclose(lpFilOut);*/

  }
  catch(...)
  {
	  if(!error) error = -1;
  }

  return error;
}

void JDiff::Error(char *msg, int num)
{
	error = num;
	strcpy(errorMsg, msg);	

	{
		throw(num);
	}
}

/*int main(int aiArgCnt, char *acArg[])
{
	if(aiArgCnt >= 4)
	{
		JDiff diff;
		diff.Diff(acArg[1], acArg[2], acArg[3]);
	}
	else
	{
		printf("Usage: jdiff file1 file2 outfile\n");
	}



	return 0;
}*/

#endif