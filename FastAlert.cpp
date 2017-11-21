// FastAlert.cpp: implementation of the CFastAlert class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "FastAlert.h"
#include "Log.h"
#include "StringMan.h"
#include "Rules.h"
#include "Preprocessor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFastAlert::CFastAlert()
{

}

CFastAlert::~CFastAlert()
{

}

SpoAlertFastData* CFastAlert::ParseFastAlertArgs(char *args)
{
	char **toks;
    int num_toks;
    char *filename;
	CLog *objLog;
	objLog = new CLog;
	CStringMan *objString;
	objString = new CStringMan;
	CRules *objRule;
	objRule = new CRules;
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
    ptrMainApp->alert_fast_data = (SpoAlertFastData *)malloc(sizeof(SpoAlertFastData));
    if(args == NULL) {
        ptrMainApp->alert_fast_data->file = objLog->OpenAlertFile(NULL);
        return ptrMainApp->alert_fast_data;
    }
    toks = objString->mSplit(args, " ", 2, &num_toks, 0);
    filename = objRule->ProcessFileOption(toks[0]);
    ptrMainApp->alert_fast_data->file = objLog->OpenAlertFile(filename);
    free(filename);
    return ptrMainApp->alert_fast_data;
}

void CFastAlert::SpoAlertFast(Packet *p, char *msg, void *arg)
{
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
    AlertFast(p, msg, ptrMainApp->alert_fast_data->file);
    return;
}

void CFastAlert::AlertFast(Packet *p, char *msg, FILE *file)
{
	char timestamp[23];
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
    bzero((char *)timestamp, 23);
    ptrMainApp->Ts_Print((struct timeval*)(p==NULL?NULL:&p->pkth->ts), timestamp);
    fwrite(timestamp, 22, 1, file);
    if (msg != NULL){
        fwrite(" [**] ", 6, 1, file);
        fwrite(msg, strlen(msg), 1, file);
        fwrite(" [**] ", 6, 1, file);
    }
    if (p){
        if (p->frag_flag){
            fputs(inet_ntoa(p->iph->ip_src), file);
            fwrite(" -> ", 4, 1, file);
            fputs(inet_ntoa(p->iph->ip_dst), file);
        }else{
            switch (p->iph->ip_proto)
            {
		case IPPROTO_UDP:
		case IPPROTO_TCP:
                    fputs(inet_ntoa(p->iph->ip_src), file);
                    fprintf(file, ":%d -> ", p->sp);
                    fputs(inet_ntoa(p->iph->ip_dst), file);
                    fprintf(file, ":%d", p->dp);
		    break;
		case IPPROTO_ICMP:
		default:
                    fputs(inet_ntoa(p->iph->ip_src), file);
                    fwrite(" -> ", 4, 1, file);
                    fputs(inet_ntoa(p->iph->ip_dst), file);
            }
        }
    }fputc('\n', file);
    return;
}

void CFastAlert::FastAlerts(Packet *p, char *msg, void *arg)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	AlertFast(p, msg, ptrMainApp->alert);
    return;	
}

void CFastAlert::FastAlertRestartFunc(int signal, void *arg)
{	
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
    fclose(ptrMainApp->alert_fast_data->file);
    free(ptrMainApp->alert_fast_data);
}

void CFastAlert::FastAlertCleanExitFunc(int signal, void *arg)
{
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
	fclose(ptrMainApp->alert_fast_data->file);
    free(ptrMainApp->alert_fast_data);	
}

void CFastAlert::FastAlertInit(u_char *args)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	ptrMainApp->pv.alert_plugin_active = 1;
    ParseFastAlertArgs((char *)args);
	CRules *objRule;
	objRule = new CRules;
	CPreprocessor *objPrep;
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(Packet *p, char *msg, void *arg);
	ptrfun = &CManipulate::SpoAlertFast;
    objRule->AddFuncToOutputList(ptrfun, NT_OUTPUT_ALERT, ptrMainApp->alert_fast_data);
	void (CManipulate::*ptrfun2)(int,void*);
	ptrfun2 = &CManipulate::FastAlertCleanExitFunc;
	objPrep->AddFuncToCleanExitList(ptrfun2, ptrMainApp->alert_fast_data);
	ptrfun2 = &CManipulate::FastAlertRestartFunc;
    objPrep->AddFuncToRestartList(ptrfun2, ptrMainApp->alert_fast_data);
}

void CFastAlert::SetupFastAlert()
{
	CPreprocessor *objPrep;	
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(u_char *args);
	ptrfun = &CManipulate::FastAlertInit;
	objPrep->RegisterOutputPlugin("alert_fast", NT_OUTPUT_ALERT,ptrfun);
}
