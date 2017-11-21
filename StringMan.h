// StringMan.h: interface for the CStringMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGMAN_H__FECF54A4_55EE_4DE9_ADFC_A3C65F59A992__INCLUDED_)
#define AFX_STRINGMAN_H__FECF54A4_55EE_4DE9_ADFC_A3C65F59A992__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStringMan  
{
public:
	int mSearchCI( char *buf, int blen, char *ptrn, int plen, int *skip, int *shift);
	int* MakeShift(char *ptrn, int plen);
	int* MakeSkip(char *ptrn, int plen);
	int mSearch( char *buf, int blen, char *ptrn, int plen, int *skip, int *shift);
	char** mSplit(char *str, char *sep, int max_strs, int *toks, char meta);
	CStringMan();
	virtual ~CStringMan();

};

#endif // !defined(AFX_STRINGMAN_H__FECF54A4_55EE_4DE9_ADFC_A3C65F59A992__INCLUDED_)
