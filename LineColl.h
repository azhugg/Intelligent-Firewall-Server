
#if !defined(AFX_LINECOLL_H__EE11D4A3_ED58_11D1_939A_000000000000__INCLUDED_)
#define AFX_LINECOLL_H__EE11D4A3_ED58_11D1_939A_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afx.h>
#include <afxtempl.h>

class LineCollection  
{
public:
	void clear();
	CString & operator =(CString &);
	int GetSize();
	int getLineCount(){return GetSize();};
	const char* line(int i){if (i<GetSize()) return (LPCTSTR) vect[i];else return NULL;};
	LineCollection(CString *s=NULL,int skip=0);
	LineCollection(CArchive &ar,int skip=0);
	virtual ~LineCollection();
	void Insert(CString& s){vect.SetSize(vect.GetSize()+1);vect[vect.GetSize()-1]=s;}
private:
	int m_Skip;
	CArray<CString,CString&> vect;
};

#endif // !defined(AFX_LINECOLL_H__EE11D4A3_ED58_11D1_939A_000000000000__INCLUDED_)
