// Hermite.h : header file
//
////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATH_H__10025586_451C_462A_AF90_0250027A26C1__INCLUDED_)
#define AFX_PATH_H__10025586_451C_462A_AF90_0250027A26C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000




#include "Envelope.h"

/////////////////////////////////////////////////////////////////////////////
// Path 

class Path : public Envelope
{
public:
	
	void SetKey(int key, int x, int y);	
	void Load(SDL_RWops *src);
	void Save(SDL_RWops *src);


	
	void ConstantVelocity();
	
	void SetOri(int x, int y);
	Path(const gedString& pathName = "", int nKeys = 0, int pathLenght = 0);
	~Path();

	void GetXY(int &x, int &y, int frame);
	void GetX(int &x, int frame);
	void GetY(int &y, int frame);
	double GetAngle(int frame);
	void AddKey(int x, int y, bool bLinear, double tens = 0.0);
		
	
	int getXOri() {return xOri;};
	int getYOri() {return yOri;};

private:

	//Uniform key
	Uint16 maxKeys;
	Uint16 insertionFrame;
	Uint32 pathLenght;
	Uint32 xOri;
	Uint32 yOri;
};

#endif
