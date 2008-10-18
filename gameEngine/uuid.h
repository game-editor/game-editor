// @(#)root/base:$Name:  $:$Id: TUUID.h,v 1.9 2002/08/05 12:07:19 rdm Exp $
// Author: Fons Rademakers   30/9/2001

/*************************************************************************
 * Copyright (C) 1995-2001, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TUUID
#define ROOT_TUUID

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TUUID                                                                //
//                                                                      //
// This class defines a TUUID (Universally Unique IDentifier), also      //
// known as GUIDs (Globally Unique IDentifier). A TUUID is 128 bits      //
// long, and if generated according to this algorithm, is either        //
// guaranteed to be different from all other UUIDs/GUIDs generated      //
// until 3400 A.D. or extremely likely to be different. UUIDs were      //
// originally used in the Network Computing System (NCS) and            //
// later in the Open Software Foundation's (OSF) Distributed Computing  //
// Environment (DCE).                                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#define R__LONGLONG
#define ULong64_t long long

struct uuid_time_t {
      Uint32 high;
      Uint32 low;
   };

class TUUID {


protected:
   Uint32    fUUIDIndex;             //!index in the list of UUIDs in TProcessUUID
   Uint32    fTimeLow;               // 60 bit time, lower 32 bits
   Uint16  fTimeMid;               // middle 16 time bits
   Uint16  fTimeHiAndVersion;      // high 12 time bits + 4 TUUID version bits
   Uint8   fClockSeqHiAndReserved; // high 6 clock bits + 2 bits reserved
   Uint8   fClockSeqLow;           // low 8 clock bits
   Uint8   fNode[6];               // 6 node id bytes

   

   Sint32 CmpTime(uuid_time_t *t1, uuid_time_t *t2);
   void  Format(Uint16 clockseq, uuid_time_t ts);
   void  GetNodeIdentifier();
   void  GetCurrent_Time(uuid_time_t *timestamp);
   void  GetSystemTime(uuid_time_t *timestamp);
   void  GetRandomInfo(Uint8 seed[16]);
   void  SetFromString(const char *uuid_str);

   /*void  StreamerV1(TBuffer &b);
   void         FillBuffer(char *&buffer);
   void         ReadBuffer(char *&buffer);
   Sint32        Sizeof() const { return 18; }*/

public:
   TUUID();
   //TUUID(const char *uuid_str);
   virtual ~TUUID();

   const char  *AsString() const;
   Sint32        Compare(const TUUID &u) const;
   Uint16     Hash() const;
   void         Print() const;
   //TInetAddress GetHostAddress() const;
   //TDatime      GetTime() const;
   void         GetUUID(Uint8 uuid[16]) const;
   //void         SetUUID(const char *uuid_str);
   Uint32       GetUUIDNumber() const { return fUUIDIndex; }
   void         SetUUIDNumber(Uint32 index) { fUUIDIndex = index; }
};


#endif
