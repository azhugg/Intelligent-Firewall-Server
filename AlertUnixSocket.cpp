// AlertUnixSocket.cpp: implementation of the CAlertUnixSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "AlertUnixSocket.h"
#include "Rules.h"
#include "Log.h"
#include "Preprocessor.h"
#include "StringMan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAlertUnixSocket::CAlertUnixSocket()
{

}

CAlertUnixSocket::~CAlertUnixSocket()
{

}

void CAlertUnixSocket::ParseAlertUnixSockArgs(char *args)
{
	CLog *objLog;
	objLog = new CLog;
	objLog->OpenAlertSock();
}

void CAlertUnixSocket::SpoAlertUnixSock(Packet *p, char *msg, void *arg)
{
	CLog *objLog;
	objLog = new CLog;
	objLog->UnixSockAlert(p, msg, arg);
    return;
}

void CAlertUnixSocket::AlertUnixSockInit(u_char *args)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	
	ptrMainApp->pv.alert_plugin_active = 1;

    ParseAlertUnixSockArgs((char *)args);

	void (CManipulate::*ptrfun)(Packet *p, char *msg, void *arg);
    ptrfun = CManipulate::SpoAlertUnixSock;
	CRules *objRule;
	objRule = new CRules;
	objRule->AddFuncToOutputList(ptrfun, NT_OUTPUT_ALERT, NULL);
}

void CAlertUnixSocket::SetupAlertUnixSocket()
{
	CPreprocessor *objPrep;	
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(u_char *args);
	ptrfun = &CManipulate::AlertUnixSockInit;
	objPrep->RegisterOutputPlugin("alert_unixsock", NT_OUTPUT_ALERT,ptrfun);
}
