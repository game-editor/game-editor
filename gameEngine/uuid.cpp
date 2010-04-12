// @(#)root/base:$Name:  $:$Id: TUUID.cxx,v 1.17 2003/04/11 11:48:10 rdm Exp $
// Author: Fons Rademakers   30/9/2001

/*************************************************************************
* Copyright (C) 1995-2001, Rene Brun and Fons Rademakers.               *
* All rights reserved.                                                  *
*                                                                       *
* For the licensing terms see $ROOTSYS/LICENSE.                         *
* For the list of contributors see $ROOTSYS/README/CREDITS.             *
*************************************************************************/

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
// Structure of universal unique IDs (UUIDs).                           //
//                                                                      //
// Depending on the network data representation, the multi-             //
// octet unsigned integer fields are subject to byte swapping           //
// when communicated between dissimilar endian machines.                //
//                                                                      //
// +-----------------------------------+                                //
// |     low 32 bits of time           |  0-3   .fTimeLow               //
// +-------------------------------+----                                //
// |     mid 16 bits of time       |      4-5   .fTimeMid               //
// +-------+-----------------------+                                    //
// | vers. |   hi 12 bits of time  |      6-7   .fTimeHiAndVersion      //
// +-------+-------+---------------+                                    //
// |Res | clkSeqHi |                      8     .fClockSeqHiAndReserved //
// +---------------+                                                    //
// |   clkSeqLow   |                      9     .fClockSeqLow           //
// +---------------+------------------+                                 //
// |            node ID               |   10-15 .fNode                  //
// +----------------------------------+                                 //
//                                                                      //
// The adjusted time stamp is split into three fields, and the          //
// clockSeq is split into two fields.                                   //
//                                                                      //
// The timestamp is a 60-bit value. For TUUID version 1, this            //
// is represented by Coordinated Universal Time (UTC/GMT) as            //
// a count of 100-nanosecond intervals since 00:00:00.00,               //
// 15 October 1582 (the date of Gregorian reform to the                 //
// Christian calendar).                                                 //
//                                                                      //
// The version number is multiplexed in the 4 most significant          //
// bits of the 'fTimeHiAndVersion' field. There are two defined         //
// versions:                                                            //
//               MSB <---                                               //
// Version      4-Bit Code      Description                             //
// ------------------------------------------------------------         //
// |  1           0 0 0 1     DCE version, as specified herein.         //
// |  2           0 0 1 0     DCE Security version, with                //
// |                          embedded POSIX UIDs.                      //
// |  3           0 0 1 1     node id is a random value                 //
// ------------------------------------------------------------         //
//                                                                      //
// Clock Sequence                                                       //
//                                                                      //
// The clock sequence value must be changed whenever:                   //
//                                                                      //
//    The TUUID generator detects that the local value of UTC            //
//    has gone backward; this may be due to re-syncing of the system    //
//    clock.                                                            //
//                                                                      //
// While a node is operational, the TUUID service always saves           //
// the last UTC used to create a TUUID. Each time a new TUUID             //
// is created, the current UTC is compared to the saved value           //
// and if either the current value is less or the saved value           //
// was lost, then the clock sequence is incremented modulo              //
// 16,384, thus avoiding production of duplicted UUIDs.                 //
//                                                                      //
// The clock sequence must be initialized to a random number            //
// to minimize the correlation across system. This provides             //
// maximum protection against node identifiers that may move            //
// or switch from system to system rapidly.                             //
//                                                                      //
// Clock Adjustment                                                     //
//                                                                      //
// UUIDs may be created at a rate greater than the system clock         //
// resolution. Therefore, the system must also maintain an              //
// adjustment value to be added to the lower-order bits of the          //
// time. Logically, each time the system clock ticks, the               //
// adjustment value is cleared. Every time a TUUID is generated,         //
// the current adjustment value is read and incremented, and            //
// then added to the UTC time field of the TUUID.                        //
//                                                                      //
// Clock Overrun                                                        //
//                                                                      //
// The 100-nanosecond granularity of time should prove sufficient       //
// even for bursts of TUUID production in the next generation of         //
// high-performance multiprocessors. If a system overruns the           //
// clock adjustment by requesting too many UUIDs within a single        //
// system clock tick, the TUUID generator will stall until the           //
// system clock catches up.                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "uuid.h"
#include "SDL.h"
#include "SHA1.h"


#ifndef WIN32
#include <sys/time.h>
#ifndef __MACOSX__
#include <sys/sysinfo.h> 
#endif
#endif
//______________________________________________________________________________
TUUID::TUUID()
{
	// Create a TUUID.

	static uuid_time_t time_last;
	static Uint16    clockseq;

	if(!clockseq)
	{
		GetCurrent_Time(&time_last);
		clockseq = (Uint16)randomMT();
	}

	uuid_time_t timestamp;

	// get current time
	GetCurrent_Time(&timestamp);

	// if clock went backward change clockseq
	if (CmpTime(&timestamp, &time_last) == -1) {
		clockseq = (clockseq + 1) & 0x3FFF;
		if (clockseq == 0) clockseq++;
	}

	Format(clockseq, timestamp);

	time_last = timestamp;
	fUUIDIndex = 1<<30;
}

//______________________________________________________________________________
TUUID::~TUUID()
{
	// delete this TUUID

	//gROOT->GetUUIDs()->RemoveUUID(fUUIDIndex);
}

//______________________________________________________________________________
Sint32 TUUID::CmpTime(uuid_time_t *t1, uuid_time_t *t2)
{
	// Compare two time values.

	if (t1->high < t2->high) return -1;
	if (t1->high > t2->high) return 1;
	if (t1->low  < t2->low)  return -1;
	if (t1->low  > t2->low)  return 1;
	return 0;
}

//______________________________________________________________________________
void TUUID::SetFromString(const char *uuid)
{
	// Set this TUUID to the value specified in uuid ((which must be in
	// TUUID::AsString() format).

	// Format is tttttttt-tttt-cccc-cccc-nnnnnnnnnnnn.
	long    timeLo;
	int     timeMid;
	int     timeHiAndVersion;
	int     clockSeqHiAndRes;
	int     clockSeqLo;
	int     node[6];

	sscanf(uuid, "%8lX-%4X-%4X-%2X%2X-%2X%2X%2X%2X%2X%2X",
		&timeLo,
		&timeMid,
		&timeHiAndVersion,
		&clockSeqHiAndRes,
		&clockSeqLo,
		&node[0], &node[1], &node[2], &node[3], &node[4], &node[5]);

	// Note that we're going through this agony because scanf is
	// defined to know only to scan into "int"s or "long"s.
	fTimeLow               = (Uint32) timeLo;
	fTimeMid               = (Uint16) timeMid;
	fTimeHiAndVersion      = (Uint16) timeHiAndVersion;
	fClockSeqHiAndReserved = (Uint8) clockSeqHiAndRes;
	fClockSeqLow           = (Uint8) clockSeqLo;
	fNode[0]               = (Uint8) node[0];
	fNode[1]               = (Uint8) node[1];
	fNode[2]               = (Uint8) node[2];
	fNode[3]               = (Uint8) node[3];
	fNode[4]               = (Uint8) node[4];
	fNode[5]               = (Uint8) node[5];
	fUUIDIndex             = 1<<30;
}

//______________________________________________________________________________
/*TUUID::TUUID(const char *uuid)
{
	// Initialize a TUUID with uuid (which must be in TUUID::AsString() format).

	fTimeLow               = 0;
	fTimeMid               = 0;
	fTimeHiAndVersion      = 0;
	fClockSeqHiAndReserved = 0;
	fClockSeqLow           = 0;
	fNode[0]               = 0;

	if (!uuid || !*uuid)
		Error("TUUID", "null string not allowed");

	SetFromString(uuid);
}*/

//______________________________________________________________________________
/*void TUUID::FillBuffer(char *&buffer)
{
	// Stream TUUID into output buffer.

	Version_t version = TUUID::Class_Version();
	tobuf(buffer, version);
	tobuf(buffer, fTimeLow);
	tobuf(buffer, fTimeMid);
	tobuf(buffer, fTimeHiAndVersion);
	tobuf(buffer, fClockSeqHiAndReserved);
	tobuf(buffer, fClockSeqLow);
	for (Sint32 i = 0; i < 6; i++)
		tobuf(buffer, fNode[i]);
}

//______________________________________________________________________________
void TUUID::ReadBuffer(char *&buffer)
{
	// Stream TUUID from input buffer.

	Version_t version;
	frombuf(buffer, &version);
	frombuf(buffer, &fTimeLow);
	frombuf(buffer, &fTimeMid);
	frombuf(buffer, &fTimeHiAndVersion);
	frombuf(buffer, &fClockSeqHiAndReserved);
	frombuf(buffer, &fClockSeqLow);
	for (Sint32 i = 0; i < 6; i++)
		frombuf(buffer, &fNode[i]);
}

//______________________________________________________________________________
void TUUID::StreamerV1(TBuffer &b)
{
	// Stream TUUID from input buffer.
	// This function is for the exclusive use of TDirectory::Streamer() to
	// read a non-versioned version of TUUID.

	b >> fTimeLow;
	b >> fTimeMid;
	b >> fTimeHiAndVersion;
	b >> fClockSeqHiAndReserved;
	b >> fClockSeqLow;
	for (Uint32 i = 0; i < 6; i++) {
		b >> fNode[i];
	}
}*/

//______________________________________________________________________________
void TUUID::Format(Uint16 clockseq, uuid_time_t ts)
{
	// Make a TUUID from timestamp, clockseq and node id.

	fTimeLow = ts.low;
	fTimeMid = (Uint16)(ts.high & 0xFFFF);
	fTimeHiAndVersion = (Uint16)((ts.high >> 16) & 0x0FFF);
	fTimeHiAndVersion |= (1 << 12);
	fClockSeqLow = clockseq & 0xFF;
	fClockSeqHiAndReserved = (clockseq & 0x3F00) >> 8;
	fClockSeqHiAndReserved |= 0x80;
	GetNodeIdentifier();
}

//______________________________________________________________________________
void TUUID::GetCurrent_Time(uuid_time_t *timestamp)
{
	// Get current time as 60 bit 100ns ticks since whenever.
	// Compensate for the fact that real clock resolution is less
	// than 100ns.

	const Uint16 uuids_per_tick = 1024;

	static uuid_time_t time_last;
	static Uint16    uuids_this_tick;
	static bool      init = false;

	if (!init) {
		GetSystemTime(&time_last);
		uuids_this_tick = uuids_per_tick;
		init = true;
	}

	uuid_time_t time_now;

	while (1) {
		GetSystemTime(&time_now);

		// if clock reading changed since last TUUID generated
		if (CmpTime(&time_last, &time_now))  {
			// reset count of uuid's generated with this clock reading
			uuids_this_tick = 0;
			break;
		}
		if (uuids_this_tick < uuids_per_tick) {
			uuids_this_tick++;
			break;
		}
		// going too fast for our clock; spin
	}

	time_last = time_now;

	if (uuids_this_tick != 0) {
		if (time_now.low & 0x80000000) {
			time_now.low += uuids_this_tick;
			if (!(time_now.low & 0x80000000))
				time_now.high++;
		} else
			time_now.low += uuids_this_tick;
	}

	timestamp->high = time_now.high;
	timestamp->low  = time_now.low;
}

//______________________________________________________________________________
void TUUID::GetSystemTime(uuid_time_t *timestamp)
{
	// Get system time with 100ns precision. Time is since Oct 15, 1582.

#ifdef WIN32
	ULARGE_INTEGER time;
	GetSystemTimeAsFileTime((FILETIME *)&time);
	// NT keeps time in FILETIME format which is 100ns ticks since
	// Jan 1, 1601. UUIDs use time in 100ns ticks since Oct 15, 1582.
	// The difference is 17 Days in Oct + 30 (Nov) + 31 (Dec)
	// + 18 years and 5 leap days.
	time.QuadPart +=
		(unsigned __int64) (1000*1000*10)       // seconds
		* (unsigned __int64) (60 * 60 * 24)       // days
		* (unsigned __int64) (17+30+31+365*18+5); // # of days

	timestamp->high = time.HighPart;
	timestamp->low  = time.LowPart;
#else
	struct timeval tp;
	gettimeofday(&tp, 0);

#ifdef R__LONGLONG
	// Offset between TUUID formatted times and Unix formatted times.
	// TUUID UTC base time is October 15, 1582.
	// Unix base time is January 1, 1970.
	ULong64_t uuid_time = ((ULong64_t)tp.tv_sec * 10000000) + (tp.tv_usec * 10) +
		0x01B21DD213814000LL;
	timestamp->high = (Uint32) (uuid_time >> 32);
	timestamp->low  = (Uint32) (uuid_time & 0xFFFFFFFF);
#else
	timestamp->high = tp.tv_sec;
	timestamp->low  = tp.tv_usec * 10;
#endif
#endif
}

//______________________________________________________________________________
void TUUID::GetNodeIdentifier()
{
	// Get node identifier. Try first to get network address, if no
	// network interface try random info based on some machine parameters.

	static Uint32 adr = 0;

	/*if (gSystem) {
		if (!adr) {
			TInetAddress addr = gSystem->GetHostByName(gSystem->HostName());
			if (addr.IsValid())
				adr = addr.GetAddress();
			else
				adr = 1;  // illegal address
		}
		if (adr > 2) {
			memcpy(fNode, &adr, 4);
			fNode[4] = 0xbe;
			fNode[5] = 0xef;
			return;
		}
	}*/

	static Uint8 seed[16];
	if (adr < 2) 
	{
		GetRandomInfo(seed);
		seed[0] |= 0x80;
		//if (gSystem) adr = 2;  // illegal address
	}
	memcpy(fNode, seed, sizeof(fNode));
	fTimeHiAndVersion |= (3 << 12);    // version == 3: random node info
}

//______________________________________________________________________________
void TUUID::GetRandomInfo(Uint8 seed[16])
{
	// Get random info based on some machine parameters.

#ifdef WIN32
	struct randomness {
		MEMORYSTATUS  m;
		SYSTEM_INFO   s;
		FILETIME      t;
		LARGE_INTEGER pc;
		DWORD         tc;
		DWORD         l;
		char          hostname[MAX_COMPUTERNAME_LENGTH + 1];
	};
	randomness r;

	// memory usage stats
	GlobalMemoryStatus(&r.m);
	// random system stats
	GetSystemInfo(&r.s);
	// 100ns resolution time of day
	GetSystemTimeAsFileTime(&r.t);
	// high resolution performance counter
	QueryPerformanceCounter(&r.pc);
	// milliseconds since last boot
	r.tc = SDL_GetTicks();
	r.l = MAX_COMPUTERNAME_LENGTH;
	GetComputerName(r.hostname, &r.l);
#else
	struct randomness {
#if defined(linux)
		struct sysinfo   s;
#endif
		struct timeval   t;
		char             hostname[257];
	};
	randomness r;

#if defined(linux)
	sysinfo(&r.s);
#endif
	gettimeofday(&r.t, 0);
	gethostname(r.hostname, 256);
#endif

	CSHA1 sha1;

	sha1.Update(( unsigned char* )&r, sizeof(randomness));
	sha1.Final();
	memcpy(seed, sha1.GetHash(), 16);
}

//______________________________________________________________________________
void TUUID::Print() const
{
	// Print TUUID.

	printf("%s\n", AsString());
}

//______________________________________________________________________________
const char *TUUID::AsString() const
{
	// Return TUUID as string. Copy string immediately since it will be reused.

	static char uuid[40];

	sprintf(uuid, "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		fTimeLow, fTimeMid, fTimeHiAndVersion, fClockSeqHiAndReserved,
		fClockSeqLow, fNode[0], fNode[1], fNode[2], fNode[3], fNode[4],
		fNode[5]);

	return uuid;
}

//______________________________________________________________________________
Uint16 TUUID::Hash() const
{
	// Compute 16-bit hash value of the TUUID.

	Sint16  c0 = 0, c1 = 0, x, y;
	char    *c = (char *) &fTimeLow;

	// For speed lets unroll the following loop:
	//   for (i = 0; i < 16; i++) {
	//      c0 += *c++;
	//      c1 += c0;
	//   }

	c0 += *c++; c1 += c0;
	c0 += *c++; c1 += c0;
	c0 += *c++; c1 += c0;
	c0 += *c++; c1 += c0;

	c0 += *c++; c1 += c0;
	c0 += *c++; c1 += c0;
	c0 += *c++; c1 += c0;
	c0 += *c++; c1 += c0;

	c0 += *c++; c1 += c0;
	c0 += *c++; c1 += c0;
	c0 += *c++; c1 += c0;
	c0 += *c++; c1 += c0;

	c0 += *c++; c1 += c0;
	c0 += *c++; c1 += c0;
	c0 += *c++; c1 += c0;
	c0 += *c++; c1 += c0;

	//  Calculate the value for "First octet" of the hash
	x = -c1 % 255;
	if (x < 0)
		x += 255;

	// Calculate the value for "second octet" of the hash
	y = (c1 - c0) % 255;
	if (y < 0)
		y += 255;

	return Uint16((y << 8) + x);
}

//______________________________________________________________________________
Sint32 TUUID::Compare(const TUUID &u) const
{
	// Compare two UUIDs "lexically" and return
	//    -1   this is lexically before u
	//     0   this is equal to u
	//     1   this is lexically after u

#define CHECK(f1, f2) if (f1 != f2) return f1 < f2 ? -1 : 1;
	CHECK(fTimeLow, u.fTimeLow)
		CHECK(fTimeMid, u.fTimeMid)
		CHECK(fTimeHiAndVersion, u.fTimeHiAndVersion)
		CHECK(fClockSeqHiAndReserved, u.fClockSeqHiAndReserved)
		CHECK(fClockSeqLow, u.fClockSeqLow)
		for (int i = 0; i < 6; i++) {
			if (fNode[i] < u.fNode[i])
				return -1;
			if (fNode[i] > u.fNode[i])
				return 1;
		}
		return 0;
}

//______________________________________________________________________________
/*TInetAddress TUUID::GetHostAddress() const
{
	// Get address of host encoded in TUUID. If host id is not an ethernet
	// address, but random info, then the returned TInetAddress is not valid.

	if ((fTimeHiAndVersion >> 12) == 1) {
		Uint32 addr;
		memcpy(&addr, fNode, 4);
		return TInetAddress("????", addr, 0);
	}
	return TInetAddress();
}*/

//______________________________________________________________________________
/*TDatime TUUID::GetTime() const
{
	// Get time from TUUID.

	TDatime     dt;
	uuid_time_t ts;

	ts.low   = fTimeLow;
	ts.high  = (Uint32)fTimeMid;
	ts.high |= (Uint32)((fTimeHiAndVersion & 0x0FFF) << 16);
#ifdef R__LONGLONG
	// Offset between TUUID formatted times and Unix formatted times.
	// TUUID UTC base time is October 15, 1582.
	// Unix base time is January 1, 1970.
	ULong64_t high = ts.high;
	ULong64_t uuid_time = (high << 32) + ts.low;
	uuid_time -= R__LL(0x01B21DD213814000);
	uuid_time /= R__LL(10000000);
	Uint32 tt = (Uint32) uuid_time;
	dt.Set(tt);
#else
	Warning("GetTime", "no long long support, return current time");
#endif
	return dt;
}*/

//______________________________________________________________________________
void TUUID::GetUUID(Uint8 uuid[16]) const
{
	// Return uuid in specified buffer (16 byte = 128 bits).

	memcpy(uuid, &fTimeLow, 16);
}

//______________________________________________________________________________
/*void TUUID::SetUUID(const char *uuid)
{
	// Set this TUUID to the value specified in uuid ((which must be in
	// TUUID::AsString() format).

	if (!uuid || !*uuid)
		Error("SetUUID", "null string not allowed");

	SetFromString(uuid);
}*/
