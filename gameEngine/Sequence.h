// Sequence.h: interface for the Sequence class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEQUENCE_H__A5E67279_A4A5_4729_9B8F_492D1D683795__INCLUDED_)
#define AFX_SEQUENCE_H__A5E67279_A4A5_4729_9B8F_492D1D683795__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Actor.h"

enum
{
	ANIMATION_SEQUENCE = 0,
	DEFINED_SEQUENCE
};

class Sequence  
{
public:
	void setFrameRate(int fps);
	bool CanAnim();
	int getFrameRate();
	gedString GetFrameString();
	static bool IsValideFrameSequence(Actor *actor, const gedString& animationName, const gedString& frameSequence, bool bAllowSequence = false, int maxSequenceFrames = -1);
	void Load(SDL_RWops *src, Uint32 version);

#ifndef STAND_ALONE_GAME
	void Save(SDL_RWops *src);
#endif //#ifndef STAND_ALONE_GAME

	void resetFrame() {frame = -1;}
	int GetFrame(int index);
	bool DoStep(bool bForward, int &actualFrame);
	Sequence(int type);
	virtual ~Sequence();

	int getType() {return sequenceType;}
	const gedString& getAnimationName() {return animationName;}
	const gedString& getSequenceName() {return sequenceName;}
	void setAnimationName(const gedString& name) {animationName = name;}
	void setSequenceName(const gedString& name) {sequenceName = name;}

	void SetFrameSequence(const gedString& frameSequence, int fps);
	void SetFrame(int i) {frame = i;};

	int getNFrames() {return nFrames;}
	Uint16 *getFrames() {return frames;}

private:
	gedString animationName, sequenceName;
	Uint16 nFrames, fps;
	Uint16 *frames;
	double frameInc;

	//Don't save
	int sequenceType; //Animation or defined sequence
	int frame;
};

#endif // !defined(AFX_SEQUENCE_H__A5E67279_A4A5_4729_9B8F_492D1D683795__INCLUDED_)
