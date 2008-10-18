
#ifndef __TIME_HEADERFILE__
#define __TIME_HEADERFILE__

#include <STDLIB.H>


// under windows ce these structs are also defined in winsock.h... :(
#ifndef _WINSOCKAPI_

struct timeval {
  long    tv_sec;         // seconds 
  long    tv_usec;        // and microseconds 
};


#ifndef _TM_DEFINED
struct tm {
        int tm_sec;     /* seconds after the minute - [0,59] */
        int tm_min;     /* minutes after the hour - [0,59] */
        int tm_hour;    /* hours since midnight - [0,23] */
        int tm_mday;    /* day of the month - [1,31] */
        int tm_mon;     /* months since January - [0,11] */
        int tm_year;    /* years since 1900 */
        int tm_wday;    /* days since Sunday - [0,6] */
        int tm_yday;    /* days since January 1 - [0,365] */
        int tm_isdst;   /* daylight savings time flag */
        };
#define _TM_DEFINED
#endif

#endif //_WINSOCKAPI_
  

// these are the methods we implemented since they are just not available under WindowsCE
//
extern int month_to_day[12];

#ifdef  __cplusplus
extern "C" {
#endif


size_t strftime(char * const s, const size_t maxsize, const char * const format, const struct tm * const t);

struct tm * gmtime(const time_t *timer);
struct tm * localtime(const time_t *timer);

time_t mktime(struct tm *t);

time_t time(time_t* timer);


#ifdef  __cplusplus
}
#endif

int calcDayOfWeek(const struct tm* nTM);


#endif //__TIME_HEADERFILE__
