// AlertUnixSocket.h: interface for the CAlertUnixSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALERTUNIXSOCKET_H__854F2D4E_A515_45CA_B863_99FF45DF7278__INCLUDED_)
#define AFX_ALERTUNIXSOCKET_H__854F2D4E_A515_45CA_B863_99FF45DF7278__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CAlertUnixSocket : public CManipulate  
{
public:
	void SetupAlertUnixSocket();
	void AlertUnixSockInit(u_char *args);
	void SpoAlertUnixSock(Packet *p, char *msg, void *arg);
	void ParseAlertUnixSockArgs(char *args);
	CAlertUnixSocket();
	virtual ~CAlertUnixSocket();

};

#endif // !defined(AFX_ALERTUNIXSOCKET_H__854F2D4E_A515_45CA_B863_99FF45DF7278__INCLUDED_)
