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


//
// CDataFile Class Implementation
//
// The purpose of this class is to provide a simple, full featured means to
// store persistent data to a text file.  It uses a simple key/value paradigm
// to achieve this.  The class can read/write to standard Windows .ini files,
// and yet does not rely on any windows specific calls.  It should work as
// well in a linux environment (with some minor adjustments) as it does in
// a Windows one.
//
// Written July, 2002 by Gary McNickle <gary#sunstorm.net>
// If you use this class in your application, credit would be appreciated.
//

//
// CDataFile
// The purpose of this class is to provide the means to easily store key/value
// pairs in a config file, seperated by independant sections. Sections may not
// have duplicate keys, although two or more sections can have the same key.
// Simple support for comments is included. Each key, and each section may have
// it's own multiline comment.
//
// An example might look like this;
//
// [UserSettings]
// Name=Joe User
// Date of Birth=12/25/01
//
// ;
// ; Settings unique to this server
// ;
// [ServerSettings]
// Port=1200
// IP_Address=127.0.0.1
// MachineName=ADMIN
//

#ifndef STAND_ALONE_GAME

#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>


#ifdef linux
using namespace std;
#endif


#ifdef WIN32
#include <windows.h>
#endif

#include "CDataFile.h"
int WriteLn(std::fstream& stream, char* fmt, ...);

/*
//Sample

void doSomething()
{
	/// Section One ////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// In this section, we are going to create a CDataFile object by opening
	// an allready existing file.  If the file is sucessfully opened, the
	// CDataFile will be populated with it's key/value pair contents.
	////////////////////////////////////////////////////////////////////////////

	// Create the object, passing in the file name. The file will be loaded 
	// (if it can be) and the objects keys set to the contents of the file. 
	// The file is then closed.
	CDataFile ExistingDF("test.ini");

	Report(E_INFO, "[doSomething] The file <test.ini> contains %d sections, & %d keys.",
				   ExistingDF.SectionCount(), ExistingDF.KeyCount());

	// Querry the CDataFile for the values of some keys ////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// Keys that are not found in the object will have the following values;
	//
	//   String based keys: t_Str("")
	//   Int based keys   : INT_MIN
	//   Float based keys : FLT_MIN
	////////////////////////////////////////////////////////////////////////////
	t_Str szAuthor  = t_Str("");
	float fValue	= 0.0f;
	int	  nValue	= 0;

	szAuthor = ExistingDF.GetString("author");
	if ( szAuthor.size() == 0 )
		Report(E_INFO, "[doSomething] Key 'author' was not found.");
	else		
		Report(E_INFO, "[doSomething] Key 'author' contains the value '%s'",
						szAuthor.c_str());

	fValue  = ExistingDF.GetFloat("main_key_float", "Main");
	if ( fValue == FLT_MIN )
		Report(E_INFO, "[doSomething] Key 'main_key_float' was not found.");
	else
		Report(E_INFO, "[doSomething] Key 'main_key_float' contains the value '%f'",
						fValue);

	nValue  = ExistingDF.GetInt("non existent key", "Main");

	if ( nValue == INT_MIN )
		Report(E_INFO, "[doSomething] Key 'non existent key' was not found.");
	else
		Report(E_INFO, "[doSomething] Key 'non existent key' contains the value '%d'",
		               nValue);



	// Test setting some key values ////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////

	// Turn off the AUTOCREATE_KEYS behavior.
	ExistingDF.m_Flags &= ~AUTOCREATE_KEYS;

	// With AUTOCREATE_KEYS off, SetValue will fail if it cannot find
	// the requested key. If it does find it, it will set the keys new
	// value.
	if ( ExistingDF.SetValue("main_key_float", "9.876543210", "", "Main") )
		Report(E_INFO, "[doSomething] Key 'main_key_float' value changed.");
	else
		Report(E_INFO, "[doSomething] Key 'main_key_float' not found. Value not set.");

	// Use SetValue to create a new key

	// Set the AUTOCREATE_KEYS flag. 
	ExistingDF.m_Flags |= AUTOCREATE_KEYS;

	if ( ExistingDF.SetValue("new_key", "new_key_value", "Procedurally generated key", "Main") )
		Report(E_INFO, "[doSomething] Key 'new_key' successfully added.");
	else
		Report(E_INFO, "[doSomething] Key 'new_key' was not added.");


	/// Section Two ////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// In this section, we demonstrate how to create a new CDataFile object
	// and populate it with some keys. We'll also show how to use some of the
	// other methods.
	////////////////////////////////////////////////////////////////////////////

	// Test creating a new data file procedurally.
	CDataFile NewDF;

	// At this point, NewDF contains one section, the default section named "". 

	// Add a couple of new keys to the default section.  
	NewDF.CreateKey("Letters", "abcdefghijklmnopqrstuvwxyz");
	NewDF.CreateKey("Numbers", "0123456789");

	// Create a new section
	// The first value is the section name, the second value is the comment.
	NewDF.CreateSection("ApplicationSettings", 
						"; This section contains all of the generic application settings.");

	// Add some keys to our new section. The arguments are;
	// 1) the key name
	// 2) The key value
	// 3) The comment for the key (empty comments are not written to disk)
	// 4) The section to put this value in. (Section must exist)
	NewDF.CreateKey("install_path", 
				   "c:\\projects\\cdatafile\\", 
		           "; The path where this project has been installed to.", 
				   "ApplicationSettings");

	NewDF.CreateKey("version", 
				   "01", 
				   "; The current version", 
				   "ApplicationSettings");

	// A note about default values /////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// Several of these functions use default empty values (t_Str("")) for 
	// the comment and section fields, this can bite you if your not careful. 
	// For example;
	//
	// NewDF.SetValue("date compiled",
	//			   "July 23rd, 2002",
	//			   "Settings");
	//
	// While it looks like your passing the Section name in (as Settings)
	// in fact, this is the comment field. Doing this will cause unexpected
	// results to show up in your file. (It will look like you have a section
	// named "Settings", but it will not have the [] brackets around
	// it).  This is easy to spot once you know what your looking for, but
	// it's definately something to watch out for.
	////////////////////////////////////////////////////////////////////////////


	// Add a key with SetValue, that contains spaces.
	NewDF.SetValue("date compiled",
				   "July 23rd, 2002",
				   "",  
				   "ApplicationSettings");
	
	// Now, if we had read this data file from disk, then the file name 	
	// would allready be stored, but since we created it procedurally, we
	// have to provide the filename.
	NewDF.SetFileName("new.ini");


	// A note about my use of Save() ///////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// When testing this, what I like to do is open the 'new' and 'test'.ini 
	// files up in a text editor and step through this program line by line. 
	// This way, after each save, I can reload the changes in the editor and 
	// see whether or not the output is what I expected.
	////////////////////////////////////////////////////////////////////////////

	// Ok, test some of our access methods...

	NewDF.SetBool("is_bool", true);
	NewDF.SetInt("is_int", 100);
	NewDF.SetFloat("is_float", 3.21f);

	NewDF.SetSectionComment("ApplicationSettings", "Application Settings Comment");
	NewDF.SetKeyComment("install_path", "; Location installed to.", "ApplicationSettings");
	NewDF.Save(); 
	NewDF.DeleteKey("version", "ApplicationSettings");
	NewDF.Save(); 


	// Uncomment this next line to test Section deletion.
	//	NewDF.DeleteSection("ApplicationSettings");



	// A note about comments ///////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// Comments are generally your responsibility.  While the code does append 
	// a comment indicator (as defined in CDataFile.h, it will append the first 
	// indicator in the constant) it makes no attempt to validate multiline 
	// comments.  
	////////////////////////////////////////////////////////////////////////////

	Report(E_INFO, "[doSomething] The file <new.ini> contains %d sections, & %d keys.",
				   NewDF.SectionCount(), NewDF.KeyCount());

	// If we did not explicitly call Save here, then the destructor would save the
	//  file since the data has been modified.  But we'll call it anyway.
	NewDF.Save();


	/// Section Three //////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// Test our ability to read/write to a standard Windows .ini file.
	////////////////////////////////////////////////////////////////////////////
	CDataFile WinDF("win.ini");

	Report(E_INFO, "[doSomething] The file <win.ini> contains %d sections, & %d keys.",
				   WinDF.SectionCount(), WinDF.KeyCount());

	WinDF.Save();
}
*/

// Compatibility Defines ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
  #define snprintf  _snprintf
  #define vsnprintf _vsnprintf
#endif


// CDataFile
// Our default contstructor.  If it can load the file, it will do so and populate
// the section list with the values from the file.
CDataFile::CDataFile(t_Str szFileName)
{
	m_bDirty = false;
	m_szFileName = szFileName;
	m_Flags = (AUTOCREATE_SECTIONS | AUTOCREATE_KEYS);

	t_Section empty;
	m_Sections.PushBack( empty );

	Load(m_szFileName);
}

CDataFile::CDataFile()
{
	Clear();
	m_Flags = (AUTOCREATE_SECTIONS | AUTOCREATE_KEYS);

	t_Section empty;
	m_Sections.PushBack( empty );	
}

// ~CDataFile
// Saves the file if any values have changed since the last save.
CDataFile::~CDataFile()
{
	if ( m_bDirty )
		Save();
}

// Clear
// Resets the member variables to their defaults
void CDataFile::Clear()
{
	m_bDirty = false;
	m_szFileName = t_Str("");
	m_Sections.Clear();
}

// SetFileName
// Set's the m_szFileName member variable. For use when creating the CDataFile
// object by hand (-vs- loading it from a file
void CDataFile::SetFileName(t_Str szFileName)
{
	if (m_szFileName.size() != 0 && CompareNoCase(szFileName, m_szFileName) != 0)
	{
		m_bDirty = true;

		//Report(E_WARN, "[CDataFile::SetFileName] The filename has changed from <%s> to <%s>.",
		//	   m_szFileName.c_str(), szFileName.c_str());
	}

	m_szFileName = szFileName;
}

// Load
// Attempts to load in the text file. If successful it will populate the 
// Section list with the key/value pairs found in the file. Note that comments
// are saved so that they can be rewritten to the file later.
bool CDataFile::Load(t_Str szFileName)
{
	// We dont want to create a new file here.  If it doesn't exist, just
	// return false and report the failure.

	std::fstream File(szFileName.c_str(), std::ios::in/*|ios::nocreate*/);

	if ( File.is_open() )
	{
		bool bDone = false;
		bool bAutoKey = (m_Flags & AUTOCREATE_KEYS) == AUTOCREATE_KEYS;
		bool bAutoSec = (m_Flags & AUTOCREATE_SECTIONS) == AUTOCREATE_SECTIONS;
		
		t_Str szLine;
		t_Str szComment;
		char buffer[MAX_BUFFER_LEN]; 
		t_Section* pSection = GetSection("");

		// These need to be set, we'll restore the original values later.
		m_Flags |= AUTOCREATE_KEYS;
		m_Flags |= AUTOCREATE_SECTIONS;

		while ( !bDone )
		{
			memset(buffer, 0, MAX_BUFFER_LEN);
			File.getline(buffer, MAX_BUFFER_LEN);

			szLine = buffer;
			Trim(szLine);

			bDone = ( File.eof() || File.bad() || File.fail() );

			if ( szLine.find(CommentIndicators[0]) == 0 || szLine.find(CommentIndicators[1]) == 0)
			{
				szComment += "\n";
				szComment += szLine;
			}
			else
			if ( szLine.find('[') == 0 ) // new section
			{
				szLine.erase( 0, 1 );
				szLine.erase( szLine.rfind(']'), 1 );

				CreateSection(szLine, szComment);
				pSection = GetSection(szLine);
				szComment = t_Str("");
			}
			else 
			if ( szLine.size() > 0 ) // we have a key, add this key/value pair
			{
				t_Str szKey = GetNextWord(szLine);
				t_Str szValue = szLine;

				if ( szKey.size() > 0 && szValue.size() > 0 )
				{
					SetValue(szKey, szValue, szComment, pSection->szName);
					szComment = t_Str("");
				}
			}
		}

		// Restore the original flag values.
		if ( !bAutoKey )
			m_Flags &= ~AUTOCREATE_KEYS;

		if ( !bAutoSec )
			m_Flags &= ~AUTOCREATE_SECTIONS;

		File.close();
	}
	else
	{
		//Report(E_INFO, "[CDataFile::Load] Unable to open file. Does it exist?");
		return false;
	}

	

	return true;
}


// Save
// Attempts to save the Section list and keys to the file. Note that if Load
// was never called (the CDataFile object was created manually), then you
// must set the m_szFileName variable before calling save.
bool CDataFile::Save()
{
	if ( KeyCount() == 0 && SectionCount() == 0 )
	{
		// no point in saving
		//Report(E_INFO, "[CDataFile::Save] Nothing to save.");
		return false; 
	}

	if ( m_szFileName.size() == 0 )
	{
		//Report(E_ERROR, "[CDataFile::Save] No filename has been set.");
		return false;
	}

	std::fstream File(m_szFileName.c_str(), std::ios::out|std::ios::trunc);

	if ( File.is_open() )
	{
		SectionItor s_pos(m_Sections);
		t_Section Section;
		t_Key Key;

		for(s_pos.Begin(); !s_pos.Done(); s_pos.Next())
		{
			Section = s_pos.Current();
			bool bWroteComment = false;

			if ( Section.szComment.size() > 0 )
			{
				bWroteComment = true;
				WriteLn(File, "\n%s", CommentStr(Section.szComment).c_str());
			}

			if ( Section.szName.size() > 0 )
			{
				WriteLn(File, "%s[%s]", 
						bWroteComment ? "" : "\n", 
						Section.szName.c_str());
			}

			KeyItor k_pos(Section.Keys);
			for(k_pos.Begin(); !k_pos.Done(); k_pos.Next())
			{
				Key = k_pos.Current();

				if ( Key.szKey.size() > 0 && Key.szValue.size() > 0 )
				{
					WriteLn(File, "%s%s%s%s%c%s", 
						Key.szComment.size() > 0 ? "\n" : "",
						CommentStr(Key.szComment).c_str(),
						Key.szComment.size() > 0 ? "\n" : "",
						Key.szKey.c_str(),
						EqualIndicators[0],
						Key.szValue.c_str());
				}
			}
		}
		
	}
	else
	{
		//Report(E_ERROR, "[CDataFile::Save] Unable to save file.");
		return false;
	}

	m_bDirty = false;
	
	File.flush();
	File.close();

	return true;
}

// SetKeyComment
// Set the comment of a given key. Returns true if the key is not found.
bool CDataFile::SetKeyComment(t_Str szKey, t_Str szComment, t_Str szSection)
{	
	t_Section* pSection;

	if ( (pSection = GetSection(szSection)) == NULL )
		return false;

	KeyItor k_pos(pSection->Keys);
	for(k_pos.Begin(); !k_pos.Done(); k_pos.Next())
	{
		if ( CompareNoCase( k_pos.Current().szKey, szKey ) == 0 )
		{
			k_pos.Current().szComment = szComment;
			m_bDirty = true;
			return true;
		}
	}

	return false;

}

// SetSectionComment
// Set the comment for a given section. Returns false if the section
// was not found.
bool CDataFile::SetSectionComment(t_Str szSection, t_Str szComment)
{
	SectionItor s_pos(m_Sections);

	for(s_pos.Begin(); !s_pos.Done(); s_pos.Next())
	{
		if ( CompareNoCase( s_pos.Current().szName, szSection ) == 0 ) 
		{
		    s_pos.Current().szComment = szComment;
			m_bDirty = true;
			return true;
		}
	}

	return false;
}


// SetValue
// Given a key, a value and a section, this function will attempt to locate the
// Key within the given section, and if it finds it, change the keys value to
// the new value. If it does not locate the key, it will create a new key with
// the proper value and place it in the section requested.
bool CDataFile::SetValue(t_Str szKey, t_Str szValue, t_Str szComment, t_Str szSection)
{
	t_Key* pKey = GetKey(szKey, szSection);
	t_Section* pSection = GetSection(szSection);

	if (pSection == NULL)
	{
		if ( !(m_Flags & AUTOCREATE_SECTIONS) || !CreateSection(szSection,""))
			return false;

		pSection = GetSection(szSection);
	}

	// Sanity check...
	if ( pSection == NULL )
		return false;

	// if the key does not exist in that section, and the value passed 
	// is not t_Str("") then add the new key.
	if ( pKey == NULL && szValue.size() > 0 && (m_Flags & AUTOCREATE_KEYS))
	{
		t_Key Key;

		Key.szKey = szKey;
		Key.szValue = szValue;
		Key.szComment = szComment;
		
		m_bDirty = true;
		
		pSection->Keys.PushBack(Key);
		pKey = GetKey(szKey, szSection);

		return true;
	}

	if ( pKey != NULL )
	{
		pKey->szValue = szValue;
		pKey->szComment = szComment;

		m_bDirty = true;
		
		return true;
	}

	return false;
}

// SetFloat
// Passes the given float to SetValue as a string
bool CDataFile::SetFloat(t_Str szKey, float fValue, t_Str szComment, t_Str szSection)
{
	char szStr[64];

	snprintf(szStr, 64, "%f", fValue);

	return SetValue(szKey, szStr, szComment, szSection);
}

// SetInt
// Passes the given int to SetValue as a string
bool CDataFile::SetInt(t_Str szKey, int nValue, t_Str szComment, t_Str szSection)
{
	char szStr[64];

	snprintf(szStr, 64, "%d", nValue);

	return SetValue(szKey, szStr, szComment, szSection);

}

// SetBool
// Passes the given bool to SetValue as a string
bool CDataFile::SetBool(t_Str szKey, bool bValue, t_Str szComment, t_Str szSection)
{
	t_Str szValue = bValue ?  "True" : "False";

	return SetValue(szKey, szValue, szComment, szSection);
}

// GetValue
// Returns the key value as a t_Str object. A return value of
// t_Str("") indicates that the key could not be found.
t_Str CDataFile::GetValue(t_Str szKey, t_Str szSection) 
{
	t_Key* pKey = GetKey(szKey, szSection);

	return (pKey == NULL) ? t_Str("") : pKey->szValue;
}

// GetString
// Returns the key value as a t_Str object. A return value of
// t_Str("") indicates that the key could not be found.
t_Str CDataFile::GetString(t_Str szKey, t_Str szSection)
{
	return GetValue(szKey, szSection);
}

// GetFloat
// Returns the key value as a float type. Returns FLT_MIN if the key is
// not found.
float CDataFile::GetFloat(t_Str szKey, t_Str szSection)
{
	t_Str szValue = GetValue(szKey, szSection);

	if ( szValue.size() == 0 )
		return FLT_MIN;

	return (float)atof( szValue.c_str() );
}

// GetInt
// Returns the key value as an integer type. Returns INT_MIN if the key is
// not found.
int	CDataFile::GetInt(t_Str szKey, t_Str szSection)
{
	t_Str szValue = GetValue(szKey, szSection);

	if ( szValue.size() == 0 )
		return INT_MIN;

	return atoi( szValue.c_str() );
}

// GetBool
// Returns the key value as a bool type. Returns false if the key is
// not found.
bool CDataFile::GetBool(t_Str szKey, t_Str szSection)
{
	bool bValue = false;
	t_Str szValue = GetValue(szKey, szSection);

	if ( szValue.find("1") == 0 
		|| CompareNoCase(szValue, "true") == 0
		|| CompareNoCase(szValue, "yes") == 0)
	{
		bValue = true;
	}

	return bValue;
}

// DeleteSection
// Delete a specific section. Returns false if the section cannot be 
// found or true when sucessfully deleted.
bool CDataFile::DeleteSection(t_Str szSection)
{
	SectionItor s_pos(m_Sections);

	for(s_pos.Begin(); !s_pos.Done(); s_pos.Next())
	{
		if ( CompareNoCase( s_pos.Current().szName, szSection ) == 0 ) 
		{
			m_Sections.FindAndDelete(s_pos.Current());
			return true;
		}
	}

	return false;
}

// DeleteKey
// Delete a specific key in a specific section. Returns false if the key
// cannot be found or true when sucessfully deleted.
bool CDataFile::DeleteKey(t_Str szKey, t_Str szFromSection)
{
	t_Section* pSection;

	if ( (pSection = GetSection(szFromSection)) == NULL )
		return false;

	KeyItor k_pos(pSection->Keys);
	for(k_pos.Begin(); !k_pos.Done(); k_pos.Next())
	{
		if ( CompareNoCase( k_pos.Current().szKey, szKey ) == 0 )
		{
			pSection->Keys.FindAndDelete(k_pos.Current());
			return true;
		}
	}

	return false;
}

// CreateKey
// Given a key, a value and a section, this function will attempt to locate the
// Key within the given section, and if it finds it, change the keys value to
// the new value. If it does not locate the key, it will create a new key with
// the proper value and place it in the section requested.
bool CDataFile::CreateKey(t_Str szKey, t_Str szValue, t_Str szComment, t_Str szSection)
{
	bool bAutoKey = (m_Flags & AUTOCREATE_KEYS) == AUTOCREATE_KEYS;
	bool bReturn  = false;

	m_Flags |= AUTOCREATE_KEYS;

	bReturn = SetValue(szKey, szValue, szComment, szSection);

	if ( !bAutoKey )
		m_Flags &= ~AUTOCREATE_KEYS;

	return bReturn;
}


// CreateSection
// Given a section name, this function first checks to see if the given section
// allready exists in the list or not, if not, it creates the new section and
// assigns it the comment given in szComment.  The function returns true if
// sucessfully created, or false otherwise. 
bool CDataFile::CreateSection(t_Str szSection, t_Str szComment)
{
	t_Section* pSection = GetSection(szSection);

	if ( pSection )
	{
		//Report(E_INFO, "[CDataFile::CreateSection] Section <%s> allready exists. Aborting.", szSection.c_str());
		return false;
	}

	t_Section Section;

	Section.szName = szSection;
	Section.szComment = szComment;
	m_Sections.PushBack(Section);
	m_bDirty = true;

	return true;
}

// CreateSection
// Given a section name, this function first checks to see if the given section
// allready exists in the list or not, if not, it creates the new section and
// assigns it the comment given in szComment.  The function returns true if
// sucessfully created, or false otherwise. This version accpets a KeyList 
// and sets up the newly created Section with the keys in the list.
bool CDataFile::CreateSection(t_Str szSection, t_Str szComment, KeyList Keys)
{
	if ( !CreateSection(szSection, szComment) )
		return false;

	t_Section* pSection = GetSection(szSection);

	if ( !pSection )
		return false;

	KeyItor k_pos(Keys);

	pSection->szName = szSection;
	for(k_pos.Begin(); !k_pos.Done(); k_pos.Next())
	{
		t_Key pKey;
		pKey.szComment = k_pos.Current().szComment;
		pKey.szKey = k_pos.Current().szKey;
		pKey.szValue = k_pos.Current().szValue;

		pSection->Keys.PushBack(pKey);
	}

	m_Sections.PushBack(*pSection);
	m_bDirty = true;

	return true;
}

// SectionCount
// Simply returns the number of sections in the list.
int CDataFile::SectionCount() 
{ 
	return m_Sections.size(); 
}

// KeyCount
// Returns the total number of keys contained within all the sections.
int CDataFile::KeyCount()
{
	int nCounter = 0;
	SectionItor s_pos(m_Sections);

	for(s_pos.Begin(); !s_pos.Done(); s_pos.Next())
		nCounter += s_pos.Current().Keys.size();

	return nCounter;
}


// Protected Member Functions ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

// GetKey
// Given a key and section name, looks up the key and if found, returns a
// pointer to that key, otherwise returns NULL.
t_Key*	CDataFile::GetKey(t_Str szKey, t_Str szSection)
{
	t_Section* pSection;

	// Since our default section has a name value of t_Str("") this should
	// always return a valid section, wether or not it has any keys in it is
	// another matter.
	if ( (pSection = GetSection(szSection)) == NULL )
		return NULL;

	KeyItor k_pos(pSection->Keys);
	for(k_pos.Begin(); !k_pos.Done(); k_pos.Next())
	{
		if ( CompareNoCase( k_pos.Current().szKey, szKey ) == 0 )
			return (t_Key*)&k_pos.Current();
	}

	return NULL;
}

// GetSection
// Given a section name, locates that section in the list and returns a pointer
// to it. If the section was not found, returns NULL
t_Section* CDataFile::GetSection(t_Str szSection)
{
	SectionItor s_pos(m_Sections);

	for(s_pos.Begin(); !s_pos.Done(); s_pos.Next())
	{
		if ( CompareNoCase( s_pos.Current().szName, szSection ) == 0 ) 
			return (t_Section*)&s_pos.Current();
	}

	return NULL;
}


t_Str CDataFile::CommentStr(t_Str szComment)
{
	t_Str szNewStr = t_Str("");

	Trim(szComment);

        if ( szComment.size() == 0 )
          return szComment;
	
	if ( szComment.find(CommentIndicators[0]) == gedString::npos && szComment.find(CommentIndicators[1]) == gedString::npos)
	{
		szNewStr = (char)CommentIndicators[0];
		szNewStr += " ";
	}

	szNewStr += szComment;

	return szNewStr;
}



// Utility Functions ////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

// GetNextWord
// Given a key +delimiter+ value string, pulls the key name from the string,
// deletes the delimiter and alters the original string to contain the
// remainder.  Returns the key
t_Str GetNextWord(t_Str& CommandLine)
{
	int nPos = CommandLine.find(EqualIndicators[0]);

	if(nPos == gedString::npos)
		nPos = CommandLine.find(EqualIndicators[1]);

	t_Str sWord = t_Str("");

	if ( nPos > -1 )
	{
		sWord = CommandLine.substr(0, nPos);
		CommandLine.erase(0, nPos+1);
	}
	else
	{
		sWord = CommandLine;
		CommandLine = t_Str("");
	}

	Trim(sWord);
	return sWord;
}


// CompareNoCase
// it's amazing what features std::string lacks.  This function simply
// does a lowercase compare against the two strings, returning 0 if they
// match.
int CompareNoCase(t_Str str1, t_Str str2)
{
#ifdef WIN32
	return stricmp(str1.c_str(), str2.c_str());	
#else
	return strcasecmp(str1.c_str(), str2.c_str());
#endif
}

// Trim
// Trims whitespace from both sides of a string.
void Trim(t_Str& szStr)
{
	szStr.trimLeft();
	szStr.trimRight();
}

// WriteLn
// Writes the formatted output to the file stream, returning the number of
// bytes written.
int WriteLn(std::fstream& stream, char* fmt, ...)
{
	char buf[MAX_BUFFER_LEN];
	int nLength;
	t_Str szMsg;

	memset(buf, 0, MAX_BUFFER_LEN);
	va_list args;

	va_start (args, fmt);
	  nLength = vsnprintf(buf, MAX_BUFFER_LEN, fmt, args);
	va_end (args);

	if(nLength >= MAX_BUFFER_LEN || nLength < 0) //maks
	{
		//Some error
		return 0;
	}

	if ( buf[nLength] != '\n' && buf[nLength] != '\r' )
		buf[nLength++] = '\n';


	stream.write(buf, nLength);

	return nLength;
}

// Report
// A simple reporting function. Outputs the report messages to stdout
// This is a dumb'd down version of a simmilar function of mine, so if 
// it looks like it should do more than it does, that's why...
/*void Report(e_DebugLevel DebugLevel, char *fmt, ...)
{
	char buf[MAX_BUFFER_LEN];
	int nLength;
	t_Str szMsg;

	va_list args;

	memset(buf, 0, MAX_BUFFER_LEN);

	va_start (args, fmt);
	  nLength = vsnprintf(buf, MAX_BUFFER_LEN, fmt, args);
	va_end (args);


	if ( buf[nLength] != '\n' && buf[nLength] != '\r' )
		buf[nLength++] = '\n';


	switch ( DebugLevel )
	{
		case E_DEBUG:
			szMsg = "<debug> ";
			break;
		case E_INFO:
			szMsg = "<info> ";
			break;
		case E_WARN:
			szMsg = "<warn> ";
			break;
		case E_ERROR:
			szMsg = "<error> ";
			break;
		case E_FATAL:
			szMsg = "<fatal> ";
			break;
		case E_CRITICAL:
			szMsg = "<critical> ";
			break;
	}


	szMsg += buf;


#ifdef WIN32
	OutputDebugString(szMsg.c_str());
#endif

	printf(szMsg.c_str());

}*/

#endif //#ifndef STAND_ALONE_GAME