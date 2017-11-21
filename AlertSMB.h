// AlertSMB.h: interface for the CAlertSMB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALERTSMB_H__2FC0C563_E648_4E17_A454_D261A67B6C43__INCLUDED_)
#define AFX_ALERTSMB_H__2FC0C563_E648_4E17_A454_D261A67B6C43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CAlertSMB : public CManipulate  
{
public:
	CAlertSMB();
	virtual ~CAlertSMB();

public:
	void SetupAlertSMB();
	void AlertSmbInit(u_char *args);
	SpoAlertSmbData* ParseAlertSmbArgs(char *args);
	void SpoAlertSmbRestartFunc(int signal, void *arg);
	void SpoAlertSmbCleanExitFunc(int signal, void *arg);
	void SpoAlertSmb(Packet *p, char *msg, void *arg);
//	SpoAlertSmbData *smb_alert_data;
};

#endif // !defined(AFX_ALERTSMB_H__2FC0C563_E648_4E17_A454_D261A67B6C43__INCLUDED_)
