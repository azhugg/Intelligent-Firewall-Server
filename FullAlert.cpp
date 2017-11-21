// FullAlert.cpp: implementation of the CFullAlert class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "FullAlert.h"
#include "Rules.h"
#include "Preprocessor.h"
#include "StringMan.h"
#include "Log.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFullAlert::CFullAlert()
{

}

CFullAlert::~CFullAlert()
{

}

SpoAlertFullData* CFullAlert::ParseFullAlertArgs(char *args)
{
	CStringMan *objString;
	objString = new CStringMan;
    CRules *objRule;
	objRule = new CRules;
	CLog *objLog;
	objLog = new CLog;
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
	char **toks;
    int num_toks;
    char *filename;
     ptrMainApp->alert_full_data = (SpoAlertFullData *)malloc(sizeof(SpoAlertFullData));
    if(args == NULL){
        ptrMainApp->alert_full_data->file = objLog->OpenAlertFile(NULL);
        return ptrMainApp->alert_full_data;
    }
    toks = objString->mSplit(args, " ", 2, &num_toks, 0);
    filename =objRule->ProcessFileOption(toks[0]);
    ptrMainApp->alert_full_data->file = objLog->OpenAlertFile(filename);
    free(filename);
    return ptrMainApp->alert_full_data;
}

void CFullAlert::FullAlertRestartFunc(int signal, void *arg)
{
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
	fclose(ptrMainApp->alert_full_data->file);
    free(ptrMainApp->alert_full_data);
}


void CFullAlert::FullAlertCleanExitFunc(int signal, void *arg)
{
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
	fclose(ptrMainApp->alert_full_data->file);
	free(ptrMainApp->alert_full_data);
}

void CFullAlert::SpoAlertFull(Packet *p, char *msg, void *arg)
{
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
	CLog *objLog;
	objLog = new CLog;
	objLog->AlertFull(p, msg, ptrMainApp->alert_full_data->file);
    return;
}

void CFullAlert::FullAlertInit(u_char *args)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	ptrMainApp->pv.alert_plugin_active = 1;
    ParseFullAlertArgs((char *)args);
	CRules *objRule;
	objRule = new CRules;
	CPreprocessor *objPrep;
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(Packet *p, char *msg, void *arg);
	ptrfun = &CManipulate::SpoAlertFull;
    objRule->AddFuncToOutputList(ptrfun, NT_OUTPUT_ALERT, ptrMainApp->alert_full_data);
    void (CManipulate::*ptrfun2)(int,void*);
	ptrfun2 = &CManipulate::FullAlertCleanExitFunc;
	objPrep->AddFuncToCleanExitList(ptrfun2, ptrMainApp->alert_full_data);
    ptrfun2 = &CManipulate::FullAlertRestartFunc;
	objPrep->AddFuncToRestartList(ptrfun2, ptrMainApp->alert_full_data);
}

void CFullAlert::SetupFullAlert()
{
	CPreprocessor *objPrep;	
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(u_char *args);
	ptrfun = &CManipulate::FullAlertInit;
	objPrep->RegisterOutputPlugin("alert_full", NT_OUTPUT_ALERT,ptrfun);
}
