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

// UndoFrame.cpp: implementation of the CUndoFrame class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME //maks

#include "UndoFrame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUndoFrame::CUndoFrame()
{

}

//////////////////////////////////////////////////////////////////////

CUndoFrame::~CUndoFrame()
{

}

//////////////////////////////////////////////////////////////////////

void CUndoFrame::Clear()
{
   while (!m_actions.Empty())
   {
      m_actions.PopFront();
   }
}

//////////////////////////////////////////////////////////////////////

void CUndoFrame::Add(const CUndoAction &action)
{
   m_actions.PushFront(action);      

   /*
   TRACE("adding '%s', %s, line %d to undo frame\n", 
            action.m_csText,
            (action.m_actionType == CUndoAction::eAddLine ? "AddLine" : 
            action.m_actionType == CUndoAction::eRemoveLine ? "RemoveLine" : 
            "ChangeLine" ),
             action.m_iLineNumber);
             */
}

//////////////////////////////////////////////////////////////////////

bool CUndoFrame::GetNext(CUndoAction &action)
{
   if (m_actions.Empty()) 
   {
      return false;
   }

   action = m_actions.Front();
   m_actions.PopFront();

   return true;
}

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

#endif