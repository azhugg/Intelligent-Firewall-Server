// FullAlert.h: interface for the CFullAlert class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FULLALERT_H__6CB24E08_F397_4F72_81A3_3B11F28336C9__INCLUDED_)
#define AFX_FULLALERT_H__6CB24E08_F397_4F72_81A3_3B11F28336C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CFullAlert : public CManipulate  
{
public:
	SpoAlertFullData* ParseFullAlertArgs(char *args);
	CFullAlert();
	virtual ~CFullAlert();

public:
	void SetupFullAlert();
	void FullAlertInit(u_char *args);
	void SpoAlertFull(Packet *p, char *msg, void *arg);
	void FullAlertCleanExitFunc(int signal, void *arg);
	void FullAlertRestartFunc(int signal, void *arg);
};

#endif // !defined(AFX_FULLALERT_H__6CB24E08_F397_4F72_81A3_3B11F28336C9__INCLUDED_)
