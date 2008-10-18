// WaitCursor.h: interface for the WaitCursor class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_WAITCURSOR_H__14944363_F9B3_414D_98B1_9DBA776F905E__INCLUDED_)
#define AFX_WAITCURSOR_H__14944363_F9B3_414D_98B1_9DBA776F905E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class stCursor;
class WaitCursor  
{
public:
	static stCursor * getWaitCursor();
	WaitCursor(bool bLoadCursor = true);
	virtual ~WaitCursor();


private:
	static int waitCursorCount;
	static stCursor	cursor;	
};

#endif // !defined(AFX_WAITCURSOR_H__14944363_F9B3_414D_98B1_9DBA776F905E__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME
