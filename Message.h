// Message.h: interface for the CMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGE_H__32DB6CAF_074B_4333_BF53_40A876484FAC__INCLUDED_)
#define AFX_MESSAGE_H__32DB6CAF_074B_4333_BF53_40A876484FAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMessage  
{
public:
	void InformationMessage(CString str);
	void FatalErrorMessage(CString str);
	void ErrorMessage(CString str);
	CMessage();
	virtual ~CMessage();

public:
	CString uMsg;

};

#endif // !defined(AFX_MESSAGE_H__32DB6CAF_074B_4333_BF53_40A876484FAC__INCLUDED_)
