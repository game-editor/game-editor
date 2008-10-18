// MuteEx.h: interface for the MuteEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUTEEX_H__846E260D_7FFE_4E2E_B0E3_E11499C67D8F__INCLUDED_)
#define AFX_MUTEEX_H__846E260D_7FFE_4E2E_B0E3_E11499C67D8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct SDL_mutex;
class MuteEx  
{
public:
	MuteEx(SDL_mutex *mutexTimer);
	virtual ~MuteEx();

private:
	SDL_mutex *mutex;
};

#endif // !defined(AFX_MUTEEX_H__846E260D_7FFE_4E2E_B0E3_E11499C67D8F__INCLUDED_)
