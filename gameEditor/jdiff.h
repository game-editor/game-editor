#ifndef STAND_ALONE_GAME

#ifndef _JDIFF_
#define _JDIFF_

#include "SDL.h"

/*
 * Local definitions
 */

#define ulong unsigned long int // Positions within files
#define MCH_MAX 128             // Maximum size of matching table
#define EOB EOF - 1             // End-Of-Buffer constant
#define ESC     0xA7    /* Escape       */
#define MOD     0xA6    /* Modify       */
#define INS     0xA5    /* Insert       */
#define DEL     0xA4    /* Delete       */
#define EQL     0xA3    /* Equal        */
#define BKT     0xA2    /* Backtrace    */

//#define debug    1        /* Include debug code? */

#if debug
#define AreWeHere fprintf(stderr, "test\n") ; fflush(stderr) ;

#define DBGHSH 0  // Debug Hash          -dh
#define DBGAHD 1  // Debug Ahead         -da
#define DBGCMP 2  // Debug Compare       -dc
#define DBGPRG 3  // Debug Progress      -dp
#define DBGBUF 4  // Debug Ahead Buffer  -db
#define DBGAHH 5  // Debug Ahead Hash    -dahh
#define DBGHSK 6  // Debug ufHshNxt      -dk
#define DBGBKT 7  // Debug ufFabSek      -dbkt
#define DBGRED 8  // Debug ufFabGet      -dred
#define DBGMCH 9  // Debug ufMch...      -dm
int gbDbg[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

#else
#define AreWeHere ;
#endif

/* List of primes we select from when size is specified on commandline */
static int   giPme[24] = { 2147483647, 1073741789, 536870909,  268435399,  
                     134217689,   67108859,  33554393,   16777213,    
                       8388593,    4194301,   2097143,    1048573,
                        524287,     262139,    131071,      65521,
                         32749,      16381,      8191,       4093,
                          2039,       1021,       509,        251} ;

//void (*ufOutBytBin) (int, int, int, Fab *, Fab *, SDL_RWops *i, ulong, ulong); /* default ufOutBytBin */

class JDiff
{
public:
	JDiff();
	virtual ~JDiff();

	int Diff(SDL_RWops *lcFilNamFrt, SDL_RWops *lcFilNamScd, SDL_RWops *lpFilOut);
	int Patch(SDL_RWops *lpFilOrg, SDL_RWops *lpFilPch, SDL_RWops *lpFilOut);

private:

	int error;
	char errorMsg[256];

	void Error(char *msg, int num);

	/*******************************************************************************
* Look-ahead buffered file abstraction (FAB = File Ahead Buffer)
*
* A look-ahead buffered file is a file with two read modes: the "normal" read
* mode and the look-ahead mode.
*
* In read mode, data is "consumed" so old data is removed from the buffer.
* In look-ahead mode, non-read data in the buffer will not be removed.
*
* ufFabOpn      Open  file and allocate read-ahead buffer.
* ufFabCls      Close file and deallocate read-ahead buffer.
* ufFabGet      Read one byte from file.
* ufFabFnd      Compare and find equal blocks within two files.
*
*******************************************************************************/
class Fab
{ 
public:
	Fab()
	{
		icFid = NULL;
		ipFil = NULL;
		ipBuf = NULL;
		ipMax = NULL;
		ipInp = NULL;
		ilPosRed = 0;
		ilPosInp = 0;
		ilPosSek = 0;
		ilPosEof = 0;
		iiBufSze = 0;
	}

	char *icFid;          /* file id (for debugging)                      */
	SDL_RWops *ipFil;          /* file handle                                  */
	unsigned char *ipBuf; /* read-ahead buffer                            */
	unsigned char *ipMax; /* read-ahead buffer end                        */
	unsigned char *ipInp; /* current input position (null=out of buf)     */
	ulong ilPosRed;       /* current file position                        */
	ulong ilPosInp;       /* last position in buffer                      */
	ulong ilPosSek;       /* current position in file                     */
	ulong ilPosEof;       /* last    position in file                     */
	int iiBufSze;         /* number of bytes in buffer                    */
};

	int ufFabOpn ( Fab *asFab, SDL_RWops *acNam, char *acFid );
	int ufFabCls ( Fab *asFab );
	int ufFabGet ( 
    Fab *asFab,      /* filebuffer                           */
    ulong *alPos,    /* position to read from                */
    int aiTyp        /* 0=read, 1=hard ahead, 2=soft ahead   */
		);
	int ufFabFnd (
    Fab *asFilOrg, Fab *asFilNew, 
    ulong *alPosOrg, ulong *alPosNew,
    int aiLen, int aiSft
    );

	void ufHshIni ();
	void ufHshNxt ( int aiCurVal, ulong *alCurHsh );
	void ufHshAdd (ulong alCurHsh, ulong alPos, ulong alBse );

	long ufHshGet (ulong alCurHsh);
	void ufHshPnt();
	void ufMchIni ();

	int ufMchAdd (
  Fab   *asFilOrg,
  Fab   *asFilNew,
  ulong alFndOrgAdd,      /* match to add               */
  ulong alFndNewAdd,
  ulong alBseNew
	);


	int ufMchFre ( ulong alBseNew );
	int ufMchBst (
  Fab   *asFilOrg,      /* files                */
  Fab   *asFilNew,
  ulong alBseOrg,       /* base positions       */
  ulong alBseNew,
  ulong *alBstOrg,      /* best position found  */
  ulong *alBstNew
	);

	void ufFndAhdGet(Fab *asFilOut, ulong *alPos, int *aiVal, int *aiPrv, int *aiEql, int aiSft) ;
	void ufFndAhdScn(Fab *asFilOrg);


	int ufFndAhd (
  Fab *asFilOrg,
  Fab *asFilNew,
  ulong alRedOrg,     /* read position in original file                     */
  ulong alRedNew,     /* read position in new file                          */
  int *aiSkpOrg,      /* number of bytes to skip (delete) in original file  */
  int *aiSkpNew,      /* number of bytes to skip (insert) in new file       */
  int *aiAhd          /* number of bytes to go before similarity is reached */
	);

	void ufPutLen ( ulong aiLen, SDL_RWops *asFilOut );
	void ufPutOpr ( int aiOpr, SDL_RWops *asFilOut );
	void ufPutByt ( int aiByt, SDL_RWops *asFilOut );

	void ufOutBytBin ( 
  int  aiOpr, 
  int  aiOrg, 
  int  aiNew,
  Fab  *asFilOrg, 
  Fab  *asFilNew,
  SDL_RWops *asFilOut,
  ulong alPosOrg,
  ulong alPosNew 
	);


	/*void ufOutBytAsc ( 
  int  aiOpr, 
  int  aiOrg, 
  int  aiNew,
  Fab  *asFilOrg, 
  Fab  *asFilNew,
  SDL_RWops *asFilOut,
  ulong alPosOrg,
  ulong alPosNew 
	);*/

	/*void ufOutBytRgn ( 
  int  aiOpr, 
  int  aiOrg, 
  int  aiNew,
  Fab  *asFilOrg, 
  Fab  *asFilNew,
  SDL_RWops *asFilOut,
  ulong alPosOrg,
  ulong alPosNew 
	);*/



	int jdiff ( Fab *asFilOrg, Fab *asFilNew, SDL_RWops *asFilOut );

	//Patch
	int GetByt( SDL_RWops *lpFil );
	int PutByt( int c, SDL_RWops *lpFil );
	int GetInt( SDL_RWops *lpFil );
	void jpatch (SDL_RWops *asFilOrg, SDL_RWops *asFilPch, SDL_RWops *asFilOut);



	/*
	* Global settings (may be modified by commandline options) 
	*/
	int giVerbse;          /* Verbose level 0=no, 1=normal, 2=high            */
	int gbCmpAll;          /* Compare even if data not in buffer?             */
	ulong giBufSze; /* File lookahead buffer size                      */
	int giEqlThr;        /* Equal threshold: throw samples if > 32 eql. chr.*/
	//SDL_RWops *stddbg;              /* Debug output to stddbg or stdout                */
	int gbSrcBkt;         /* Backtrace on sourcefile allowed?                */
	int giSrcScn;         /* Prescan source file: 0=no, 1=do, 2=done         */
	int giMchMax;         /* Maximum entries in matching table.              */
	int giMchMin;         /* Minimum entries in matching table.              */
	ulong glMchDst;    /* Max distance between equals in same match.      */
	
							   /* 
							   * Statistics about operations
	*/
	int giFabSek;         /* Number of times an fseek operation was performed  */
	ulong giOutBytDta;    /* Number of data    bytes written                   */
	ulong giOutBytCtl;    /* Number of control bytes written (overhead)        */
	ulong giOutBytDel;    /* Number of data    bytes deleted                   */
	ulong giOutBytBkt;    /* Number of data    bytes backtraced                */
	ulong giOutBytEsc;    /* Number of escape  bytes written (overhead)        */
	ulong giOutBytEql;    /* Number of data    bytes not written (gain)        */
	int giHshHit;         /* Number of hash hits                               */
	int giHshErr;         /* Number of false hash hits                         */
	int giHshRpr;         /* Number of repaired hash hits (by compare)         */
	int giHshEls;         /* Number of elements added to hashtable             */
	int giHshCol;         /* Number of collisions                              */
	int giHshColSeq;        /* Number of subsequent collisions.                  */
	int giHshColSeqCnt;   /* Number of times max number of collisions occurred.*/

	/* -----------------------------------------------------------------------------
							* "Constants", modifiable by commandline options
		* ---------------------------------------------------------------------------*/
		int   giHshIdx;        /* choosen prime from giPme                        */
		int   giHshPme;     /* default largest 16-bit prime for hashing        */

		
							/* -----------------------------------------------------------------------------
							* Hashtable variables
		* ---------------------------------------------------------------------------*/
		ulong glMskChk; /* mask for getting the checkdigit from a value   */
		ulong glMskPos; /* mask for getting the position   from a value   */
		ulong glMaxVal; /* max position for this size of check digit      */
		
		int   giHshColMax;            /* max number of collisions before override       */
		ulong glHshColMaxPos;         /* increment giHshColMax if position gets larger  */
		
		int   giHshSze ;              /* Actual size of the hashtable                   */
		ulong *gpHshTbl;       /* Actual hashtable                               */



		int gbOutEsc; /* Output buffer for escape character in data stream */



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
ulong glMchBeg[MCH_MAX];   /* first found match                                 */ 
ulong glMchOrg[MCH_MAX];   /* last  found match (original file position)        */
ulong glMchNew[MCH_MAX];   /* last  found match (new      file position)        */
int   giMchCnt[MCH_MAX];   /* number of colliding matches                       */

ulong glMchTst[MCH_MAX];   /* result of last find for this entry                */
ulong glMchFnd[MCH_MAX];   
int   giMchCmp[MCH_MAX];  




};

#endif //_JDIFF_
#endif