/*********************************************************************

   Copyright (C) 2001 Smaller Animals Software, Inc.

   This software is provided 'as-is', without any express or implied
   warranty.  In no event will the authors be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.

   3. This notice may not be removed or altered from any source distribution.

   http://www.smalleranimals.com
   smallest@smalleranimals.com

   --------

   This code is based, in part, on:

      "Syntax Coloring Text Edit Window Class " - Randy More (9, May 1998)
      http://www.codeguru.com/editctrl/syntax_coloring.shtml

   Though probably only 5% of the original code remains.

**********************************************************************/

// UndoFrame.h: interface for the CUndoFrame class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME //maks

#if !defined(AFX_UNDOFRAME_H__53203B04_E726_46CD_80A8_C1DB02EAF9E0__INCLUDED_)
#define AFX_UNDOFRAME_H__53203B04_E726_46CD_80A8_C1DB02EAF9E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../gameEngine/gedString.h"
#include "../util/gllist.h" //maks

class Point
{
public:
	int cursorPos, line, currentWindowLine;
};

class CUndoAction
{
public:
   CUndoAction& operator = (const CUndoAction& src)
   {
      m_csText = src.m_csText;
      m_actionType = src.m_actionType;
      m_iLineNumber = src.m_iLineNumber;
      m_cursorPos = src.m_cursorPos;
      m_anchorPos = src.m_anchorPos;

      return *this;
   }

   typedef enum {eAddLine, eRemoveLine, eChangeLine} eAction;
   
   gedString  m_csText;
   eAction  m_actionType;
   int      m_iLineNumber;
   Point   m_cursorPos;
   Point   m_anchorPos;

};

typedef GlSList<CUndoAction> undoActionStack;  //push = PushFront, pop = PopFront, top = Front


class CUndoFrame  
{
public:
	CUndoFrame();
	virtual ~CUndoFrame();

   void Clear();

   void Add(const CUndoAction &action);

   bool GetNext(CUndoAction &action);

   undoActionStack  m_actions;
};


typedef GlSList<CUndoFrame> undostack;  //push = PushFront, pop = PopFront, top = Front

#endif // !defined(AFX_UNDOFRAME_H__53203B04_E726_46CD_80A8_C1DB02EAF9E0__INCLUDED_)

#endif