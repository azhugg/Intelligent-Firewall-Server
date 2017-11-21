// WinSocket.h: interface for the CWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINSOCKET_H__EA147230_5735_11D6_86D4_006008A601F7__INCLUDED_)
#define AFX_WINSOCKET_H__EA147230_5735_11D6_86D4_006008A601F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWinSocket  
{
public:
	int InitWinSock(void);
	CWinSocket();
	virtual ~CWinSocket();

};

#endif // !defined(AFX_WINSOCKET_H__EA147230_5735_11D6_86D4_006008A601F7__INCLUDED_)
