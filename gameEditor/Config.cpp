/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

If GPL v3 does not work for you, we offer a "non-GPL" license 
<http://game-editor.com/License>, thats will help us to pay our developers 
<http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>.

***************************************************************************/


// Config.cpp: implementation of the Config class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "Config.h"
#include "CDataFile.h"
#include "GameControl.h"

extern "C" int dirStrcmp(const char *a, const char *b);
gedString getVersion();

//uuencode
char uuencodeTable[] = {
'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};



#define ENC(c) (((c) >= 0 && (c) < 64)?uuencodeTable[c]:'=')//(char)(((c) & 077) + ' ')

/*
 * output one group of 3 bytes, pointed at by p, on file f.
 */

void outdec(const char *p, gedString &out)
{
      int c1, c2, c3, c4;

      c1 = *p >> 2;
      c2 = ((p[0] << 4) & 060) | ((p[1] >> 4) & 017);
      c3 = p[1]?((p[1] << 2) & 074) | ((p[2] >> 6) & 03):64;
      c4 = p[2]?p[2] & 077:64;
      
	  out += ENC(c1);
	  out += ENC(c2);
	  out += ENC(c3);
	  out += ENC(c4);
}

void uuencode(gedString in, gedString &out)
{
	int pad = ((in.length() % 3) != 0)?(3 - (in.length() % 3)):0;
	int i = 0;

	for(i = 0; i < pad; i++)
	{
		in += (char)0;
	}
	
	const char *buf = in.c_str();
	out.clear();	

	for(i = 0; i < in.length(); i += 3)
	{
		outdec(&buf[i], out);
	}
}



Config *Config::config = NULL;
Config::Config()
{
	bEnableToolTips = true;
	bShowTutorialReminder = true;
	bShowActorPropertyReminder = true;
	bPersistentUndo = true;
	maxUndoLevels = 50;

	//Load
	CDataFile iniFile(GameControl::getEditorPath() + DIR_SEP +  "GameEditor.ini");

	if(iniFile.GetValue("EnableToolTips").size())
	{
		bEnableToolTips = iniFile.GetBool("EnableToolTips");
	}

	if(iniFile.GetValue("ShowTutorialReminder").size())
	{
		bShowTutorialReminder = iniFile.GetBool("ShowTutorialReminder");
	}

	if(iniFile.GetValue("ShowActorPropertyReminder").size())
	{
		bShowActorPropertyReminder = iniFile.GetBool("ShowActorPropertyReminder");
	}

	if(iniFile.GetValue("EmergencySave").size())
	{
		emergencySave = iniFile.GetString("EmergencySave");
	}

	if(iniFile.GetValue("RecentFileList").size())
	{
		recentFileList = iniFile.GetString("RecentFileList");
	}

	if(iniFile.GetValue("PathImageFont").size())
	{
		PathImageFont = iniFile.GetString("PathImageFont");
	}

	if(iniFile.GetValue("PathAnimations").size())
	{
		PathAnimations = iniFile.GetString("PathAnimations");
	}

	if(iniFile.GetValue("PathGame").size())
	{
		PathGame = iniFile.GetString("PathGame");
	}

	if(iniFile.GetValue("PathExport").size())
	{
		PathExport = iniFile.GetString("PathExport");
	}

	if(iniFile.GetValue("PathSounds").size())
	{
		PathSounds = iniFile.GetString("PathSounds");
	}

	if(iniFile.GetValue("ImageEditor").size())
	{
		ImageEditor = iniFile.GetString("ImageEditor");
	}

	if(iniFile.GetValue("MaxUndoLevels").size())
	{
		maxUndoLevels = iniFile.GetInt("MaxUndoLevels");
	}

	if(iniFile.GetValue("PersistentUndo").size())
	{
		bPersistentUndo = iniFile.GetBool("PersistentUndo");
	}

	//For uninstall survey
	if(iniFile.GetValue("Data", "Misc").size())
	{
		UninstallData = iniFile.GetString("Data", "Misc");
	}
}

void Config::Save()
{
	CDataFile iniFile(GameControl::getEditorPath() + DIR_SEP + "GameEditor.ini");

	iniFile.SetBool("EnableToolTips", bEnableToolTips);
	iniFile.SetBool("ShowTutorialReminder", bShowTutorialReminder);
	iniFile.SetBool("ShowActorPropertyReminder", bShowActorPropertyReminder);
	iniFile.SetValue("EmergencySave", emergencySave);
	iniFile.SetValue("RecentFileList", recentFileList);

	iniFile.SetValue("PathImageFont", PathImageFont);
	iniFile.SetValue("PathAnimations", PathAnimations);
	iniFile.SetValue("PathGame", PathGame);
	iniFile.SetValue("PathExport", PathExport);
	iniFile.SetValue("PathSounds", PathSounds);
	iniFile.SetValue("ImageEditor", ImageEditor);

	iniFile.SetInt("MaxUndoLevels", maxUndoLevels);
	iniFile.SetBool("PersistentUndo", bPersistentUndo);

	//For uninstall survey
	UpdateUninstallData();
	iniFile.SetValue("Data", UninstallData, "", "Misc");
}

Config::~Config()
{
	Save();
}

Config *Config::Get()
{
	if(!config) config = new Config();
	return config;
}

void Config::Destroy()
{
	delete config;
	config = NULL;
}


void Config::UpdateUninstallData()
{
#if defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL)
	char buf[32];

	//Format:
	//Data version;Game Editor version;FINGERPRINT;DAYSINSTALLED;

	gedString fingerprint, daysInstalled("0"), dateInstall, version(getVersion());
	
	if(GetEnvironmentVariable("FINGERPRINT",  buf, 31)) fingerprint = buf;
	if(GetEnvironmentVariable("DAYSINSTALLED",  buf, 31)) daysInstalled = buf;
	//if(GetEnvironmentVariable("KEYCREATED",  buf, 31)) dateInstall = buf; //KEYCREATED = YYYY.MM.DD

	UninstallData = "1;";
	UninstallData += version + ";";
	UninstallData += fingerprint + ";";
	UninstallData += daysInstalled + ";";

	//Encode
	gedString out;
	uuencode(UninstallData, out);
	UninstallData = out;

#endif
}

RecentFileList::RecentFileList(int _maxListSize)
{
	maxListSize = _maxListSize;
	array.SetCount(maxListSize);

	ReadList();
}

RecentFileList::~RecentFileList()
{
	WriteList();
}


void RecentFileList::Add(gedString filePath)
{
	// update the MRU list, if an existing MRU string matches file name
	int iMRU;
	bool bNew = true;

	for(iMRU = 0; iMRU < maxListSize - 1; iMRU++)
	{
		if(dirStrcmp(array[iMRU].c_str(), filePath.c_str()) == 0)
		{
			bNew = false;
			break;      // iMRU will point to matching entry
		}
	}


	// move MRU strings before this one down
	for (; iMRU > 0; iMRU--)
	{
		array[iMRU] = array[iMRU-1];
	}

	// place this one at the beginning
	array[0] = filePath;
}

void RecentFileList::Remove(gedString filePath)
{
	int nIndex = 0, iMRU;

	for(nIndex = 0; nIndex < maxListSize; nIndex++)
	{
		if(array[nIndex] == filePath)
		{
			break;
		}
	}

	if(nIndex >= maxListSize)
	{
		return;
	}

	array[nIndex].clear();
	for (iMRU = nIndex; iMRU < maxListSize-1; iMRU++)
		array[iMRU] = array[iMRU+1];

	GLASSERT(iMRU < maxListSize);
	array[iMRU].clear();
}


gedString RecentFileList::GetDisplayName(int nIndex)
{

	if (nIndex < 0 || nIndex > array.Count() || array[nIndex].empty())
		return "";
	
	return AbbreviateDirName(array[nIndex], 32);
}

gedString RecentFileList::GetFullPath(int nIndex)
{

	if (nIndex < 0 || nIndex > array.Count() || array[nIndex].empty())
		return "";
	
	return array[nIndex];
}

void RecentFileList::WriteList()
{
	gedString recentFileList;
	for (int iMRU = 0; iMRU < maxListSize; iMRU++)
	{
		if (!array[iMRU].empty())
		{
			recentFileList += array[iMRU];
			recentFileList += ";";
		}
	}

	Config::Get()->setRecentFileList(recentFileList);
	Config::Destroy();
}

void RecentFileList::ReadList()
{
	if(Config::Get()->getRecentFileList().length())
	{
		gedString recentFileList(Config::Get()->getRecentFileList());

		char *pToken, sep[] = ";";		
		pToken = strtok(recentFileList.getCharBuf(), sep);
		
		for(int iMRU = 0; pToken && iMRU < maxListSize; iMRU++)
		{
			array[iMRU] = pToken;
			pToken = strtok (NULL, sep);
		}

		Config::Destroy();
	}
}

#endif //#ifndef STAND_ALONE_GAME
