/*
  Copyright (C) 2002 Amir Szekely <kichik@netvision.net.il>

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
*/

#ifndef STAND_ALONE_GAME

#if !defined(AFX_RESOURCEEDITOR_H__683BF710_E805_4093_975B_D5729186A89A__INCLUDED_)
#define AFX_RESOURCEEDITOR_H__683BF710_E805_4093_975B_D5729186A89A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define RESOURCE_EDITOR_NOT_API


#include "engine/kyra.h"
#include <Windows.h>
#include <commctrl.h>



#ifndef FOF_NOERRORUI
#define FOF_NOERRORUI 0x0400
#endif

#ifndef DS_SHELLFONT
#define DS_SHELLFONT (DS_SETFONT | DS_FIXEDSYS)
#endif

#ifndef ULONG_PTR
#define ULONG_PTR DWORD
#endif

#ifndef IS_INTRESOURCE
#define IS_INTRESOURCE(_r) (((ULONG_PTR)(_r) >> 16) == 0)
#endif

#ifndef IDC_HAND
#define IDC_HAND MAKEINTRESOURCE(32649)
#endif

#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE 0x0040
#endif

#ifndef TVITEM
#define TVITEM TV_ITEM
#endif

#ifndef TVM_SETITEMHEIGHT
#define TVM_SETITEMHEIGHT (TV_FIRST + 27)
#endif

#ifndef TVM_GETITEMHEIGHT
#define TVM_GETITEMHEIGHT (TV_FIRST + 28)
#endif

#ifndef LVS_EX_LABELTIP
#define LVS_EX_LABELTIP 0x00004000
#endif

#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif

#ifndef CSIDL_FLAG_CREATE
#define CSIDL_FLAG_CREATE 0x8000
#endif

#ifndef CSIDL_WINDOWS
#define CSIDL_WINDOWS 0x0024
#endif

#ifndef CSIDL_SYSTEM
#define CSIDL_SYSTEM 0x0025
#endif

#ifndef CSIDL_PROGRAM_FILES
#define CSIDL_PROGRAM_FILES 0x0026
#endif

#ifndef CSIDL_PROGRAM_FILES_COMMON
#define CSIDL_PROGRAM_FILES_COMMON 0x002b
#endif

#ifndef CSIDL_COMMON_DOCUMENTS
#define CSIDL_COMMON_DOCUMENTS 0x002e
#endif

#ifndef CSIDL_RESOURCES
#define CSIDL_RESOURCES 0x0038
#endif

#ifndef CSIDL_RESOURCES_LOCALIZED
#define CSIDL_RESOURCES_LOCALIZED 0x0039
#endif

#ifndef CSIDL_COMMON_ADMINTOOLS
#define CSIDL_COMMON_ADMINTOOLS 0x002f
#endif

#ifndef CSIDL_ADMINTOOLS
#define CSIDL_ADMINTOOLS 0x0030
#endif

#ifndef CSIDL_MYPICTURES
#define CSIDL_MYPICTURES 0x0027
#endif

#ifndef CSIDL_COMMON_PICTURES
#define CSIDL_COMMON_PICTURES 0x0036
#endif

#ifndef CSIDL_MYMUSIC
#define CSIDL_MYMUSIC 0x000d
#endif

#ifndef CSIDL_COMMON_MUSIC
#define CSIDL_COMMON_MUSIC 0x0035
#endif

#ifndef CSIDL_MYVIDEO
#define CSIDL_MYVIDEO 0x000e
#endif

#ifndef CSIDL_COMMON_VIDEO
#define CSIDL_COMMON_VIDEO 0x0037
#endif

#ifndef CSIDL_CDBURN_AREA
#define CSIDL_CDBURN_AREA 0x003b
#endif

#ifndef CSIDL_TEMPLATES
#define CSIDL_TEMPLATES 0x0015
#endif

#ifndef CSIDL_COMMON_TEMPLATES
#define CSIDL_COMMON_TEMPLATES 0x002d
#endif

#ifndef CSIDL_PROFILE
#define CSIDL_PROFILE 0x0028
#endif

#ifndef CSIDL_COMMON_APPDATA
#define CSIDL_COMMON_APPDATA 0x0023
#endif



class CResourceDirectory;
class CResourceDirectoryEntry;
class CResourceDataEntry;

// Resource directory with entries
typedef struct RESOURCE_DIRECTORY {
  IMAGE_RESOURCE_DIRECTORY Header;
  IMAGE_RESOURCE_DIRECTORY_ENTRY Entries[1];
} *PRESOURCE_DIRECTORY;

class CResourceEditor {
public:
  CResourceEditor(BYTE* pbPE, int iSize);
  virtual ~CResourceEditor();

  bool  UpdateResource(char* szType, char* szName, LANGID wLanguage, BYTE* lpData, DWORD dwSize);
  bool  UpdateResource(WORD szType, char* szName, LANGID wLanguage, BYTE* lpData, DWORD dwSize);
  bool  UpdateResource(char* szType, WORD szName, LANGID wLanguage, BYTE* lpData, DWORD dwSize);
  bool  UpdateResource(WORD szType, WORD szName, LANGID wLanguage, BYTE* lpData, DWORD dwSize);
  BYTE* GetResource(char* szType, char* szName, LANGID wLanguage);
  void  FreeResource(BYTE* pbResource);

  bool  AddExtraVirtualSize2PESection(const char* pszSectionName, int addsize);
  DWORD Save(BYTE* pbBuf, DWORD &dwSize);

  bool InitOk() {return bInit;}

private:
  BYTE* m_pbPE;
  int   m_iSize;
  bool bInit;

  PIMAGE_DOS_HEADER m_dosHeader;
  PIMAGE_NT_HEADERS m_ntHeaders;

  DWORD m_dwResourceSectionIndex;
  DWORD m_dwResourceSectionVA;

  CResourceDirectory* m_cResDir;

  CResourceDirectory* ScanDirectory(PRESOURCE_DIRECTORY rdRoot, PRESOURCE_DIRECTORY rdToScan);

  void WriteRsrcSec(BYTE* pbRsrcSec);
  void SetOffsets(CResourceDirectory* resDir, DWORD newResDirAt);
};

int replace_icon(CResourceEditor* re, WORD wIconId, char* filename);

#ifdef RESOURCE_EDITOR_NOT_API

class CResourceDirectory {
public:
  CResourceDirectory(PIMAGE_RESOURCE_DIRECTORY prd);
  virtual ~CResourceDirectory();

  IMAGE_RESOURCE_DIRECTORY GetInfo();

  CResourceDirectoryEntry* GetEntry(unsigned int i);
  void AddEntry(CResourceDirectoryEntry* entry);
  void RemoveEntry(int i);
  int  CountEntries();
  int  Find(char* szName);
  int  Find(WORD wId);

  DWORD GetSize();

  void Destroy();

  DWORD m_dwWrittenAt;

private:
  IMAGE_RESOURCE_DIRECTORY m_rdDir;
  GlDynArray<CResourceDirectoryEntry*> m_vEntries;
};

class CResourceDirectoryEntry {
public:
  CResourceDirectoryEntry(char* szName, CResourceDirectory* rdSubDir);
  CResourceDirectoryEntry(char* szName, CResourceDataEntry* rdeData);
  virtual ~CResourceDirectoryEntry();

  bool HasName();
  char* GetName();
  int GetNameLength();

  WORD GetId();

  bool IsDataDirectory();
  CResourceDirectory* GetSubDirectory();

  CResourceDataEntry* GetDataEntry();

  DWORD m_dwWrittenAt;

private:
  bool m_bHasName;
  union {
    char* m_szName;
    WORD m_wId;
  };

  bool m_bIsDataDirectory;
  union {
    CResourceDirectory* m_rdSubDir;
    CResourceDataEntry* m_rdeData;
  };
};

class CResourceDataEntry {
public:
  CResourceDataEntry(BYTE* pbData, DWORD dwSize, DWORD dwCodePage = 0);
  ~CResourceDataEntry();

  BYTE* GetData();

  void SetData(BYTE* pbData, DWORD dwSize);
  void SetData(BYTE* pbData, DWORD dwSize, DWORD dwCodePage);

  DWORD GetSize();
  DWORD GetCodePage();

  DWORD m_dwWrittenAt;

private:
  BYTE* m_pbData;
  DWORD m_dwSize;
  DWORD m_dwCodePage;
};

#endif // #ifdef RESOURCE_EDITOR_NOT_API

#endif // !defined(AFX_RESOURCEEDITOR_H__683BF710_E805_4093_975B_D5729186A89A__INCLUDED_)

#endif //STAND_ALONE_GAME