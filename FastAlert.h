// FastAlert.h: interface for the CFastAlert class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FASTALERT_H__EAC83D26_1744_4F4B_86C0_0FE58CD6F9CC__INCLUDED_)
#define AFX_FASTALERT_H__EAC83D26_1744_4F4B_86C0_0FE58CD6F9CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFastAlert : public CManipulate  
{
public:
	SpoAlertFastData* ParseFastAlertArgs(char *args);
	CFastAlert();
	virtual ~CFastAlert();

public:
	void SetupFastAlert();
	void FastAlertInit(u_char *args);
	void FastAlertCleanExitFunc(int signal, void *arg);
	void FastAlertRestartFunc(int signal, void *arg);
	void FastAlerts(Packet *p, char *msg, void *arg);
	void AlertFast(Packet *p, char *msg, FILE *file);
	void SpoAlertFast(Packet *p, char *msg, void *arg);
};

#endif // !defined(AFX_FASTALERT_H__EAC83D26_1744_4F4B_86C0_0FE58CD6F9CC__INCLUDED_)
