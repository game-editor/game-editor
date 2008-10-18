// EditorDirectory.h: interface for the EditorDirectory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITORDIRECTORY_H__24F7068C_1581_4FDD_A53B_8F097EA8EF97__INCLUDED_)
#define AFX_EDITORDIRECTORY_H__24F7068C_1581_4FDD_A53B_8F097EA8EF97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class EditorDirectory  
{
public:
	EditorDirectory(gedString newPath = "");
	virtual ~EditorDirectory();

private:
	char oldpath[PATH_LIMIT];
};

#endif // !defined(AFX_EDITORDIRECTORY_H__24F7068C_1581_4FDD_A53B_8F097EA8EF97__INCLUDED_)
